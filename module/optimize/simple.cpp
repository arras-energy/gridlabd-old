/** $Id: simple.cpp 4738 2014-07-03 00:55:39Z dchassin $
	Copyright (C) 2008 Battelle Memorial Institute
	@file auction.cpp
	@defgroup auction Template for a new object class
	@ingroup market

	The auction object implements the basic auction. 

 **/

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "gridlabd.h"
#include "simple.h"

EXPORT_CREATE(simple);
EXPORT_INIT(simple);
EXPORT_SYNC(simple);
EXPORT_ISA(simple);

CLASS *simple::oclass = NULL;
simple *simple::defaults = NULL;

/* comparison operators */
static inline bool lt(double a, double b) { return a<b; };
static inline bool le(double a, double b) { return a<=b; };
static inline bool eq(double a, double b) { return a==b; };
static inline bool ne(double a, double b) { return a!=b; };
static inline bool ge(double a, double b) { return a>=b; };
static inline bool gt(double a, double b) { return a>b; };
static simple::COMPAREFUNCTION getop(const char *name)
{
	struct
	{
		const char *name;
		simple::COMPAREFUNCTION op;
	} map[] =
	{
		{"<",lt},
		{"<=",le},
		{"==",eq},
		{"!=",ne},
		{">=",ge},
		{">",gt},
	};
	for ( size_t n = 0 ; n < sizeof(map)/sizeof(map[0]) ; n++ )
	{
		if ( strcmp(name,map[n].name) == 0 )
		{
			return map[n].op;
		}
	}
	return NULL;
}

/* Class registration is only called once to register the class with the core */
simple::simple(MODULE *module)
{
	if ( oclass == NULL )
	{
		oclass = gl_register_class(module,"simple",sizeof(simple),PC_PRETOPDOWN|PC_POSTTOPDOWN|PC_AUTOLOCK);
		if ( oclass == NULL )
		{
			throw "unable to register class optimize.simple";
		}
		else
		{
			oclass->trl = TRL_PROOF;
		}

		if ( gl_publish_variable(oclass,
			PT_char1024, "objective", PADDR(objective), 
				PT_DESCRIPTION, "Optimization objective value",
			PT_char1024, "variable", PADDR(variable), 
				PT_DESCRIPTION, "Optimization decision variables",
			PT_char1024, "constraint", PADDR(constraint), 
				PT_DESCRIPTION, "Optimization constraints",
			PT_double, "delta", PADDR(delta), 
				PT_DESCRIPTION, "Change applied to decision variable",
			PT_double, "epsilon", PADDR(epsilon), 
				PT_DESCRIPTION, "Precision of objective value",
			PT_int32, "trials", PADDR(trials), 
				PT_DESCRIPTION, "Limits on number of trials",
			PT_enumeration, "goal", PADDR(goal), 
				PT_DESCRIPTION, "Optimization objective goal",
				PT_DEFAULT, "EXTREMUM",
				PT_KEYWORD, "EXTREMUM", OG_EXTREMUM,
				PT_KEYWORD, "MINIMUM", OG_MINIMUM,
				PT_KEYWORD, "MAXIMUM", OG_MAXIMUM,
			NULL)<1)
		{
			throw "unable to publish properties of class optimize.simple";
		}
		if ( create() == 0 )
		{
			throw "unable to create default optimize.simple object";
		}
		defaults = this;
	}
}


int simple::isa(CLASSNAME classname)
{
	return strcmp((const char*)classname,"simple")==0;
}

/* Object creation is called once for each object that is created by the core */
int simple::create(void)
{
	memset(objective,0,sizeof(object));
	memset(variable,0,sizeof(variable));
	memset(constraint,0,sizeof(constraint));
	delta = 0;
	epsilon = 0;
	trials = 0;
	goal = OG_EXTREMUM;
	return 1; /* return 1 on success, 0 on failure */
}

/* Object initialization is called once after all object have been created */
int simple::init(OBJECT *parent)
{
	OBJECT *my = THISOBJECTHDR;
	char buffer[1024];
	struct 
	{
		const char *name;
		const char *param;
		double **var;
		COMPAREFUNCTION *op;
		double *value;
	} map[] = 
	{
		{"objective",	objective.get_string(),	&pObjective},
		{"variable",	variable.get_string(),	&pVariable},
		{"constraint",	constraint.get_string(),	&pConstraint, &(constrain.op), &(constrain.value)},
	};
	for ( size_t n=0 ; n < sizeof(map)/sizeof(map[0]) ; n++ )
	{
		char oname[1024];
		char pname[1024];
		OBJECT *obj;
		PROPERTY *prop;
		if ( strcmp(map[n].param,"")==0 )
		{
			continue;
		}
		if ( sscanf(map[n].param,"%[^.:].%[a-zA-Z0-9_.]",oname,pname) != 2 )
		{
			gl_error("%s could not be parsed, expected term in the form 'objectname'.'propertyname'", map[n].name);
			return 0;
		}
		
		// find the object
		obj = gl_get_object(oname);
		if ( obj==NULL )
		{
			gl_error("object '%s' could not be found", oname);
			return 0;
		}

		// must outrank dependent objects
		if ( my->rank<=obj->rank ) 
		{
			gl_set_rank(my,obj->rank+1);
		}

		// get property
		prop = gl_get_property(obj,pname);
		if ( prop == NULL )
		{
			gl_error("property '%s' could not be found in object '%s'", pname, oname);
			return 0;
		}
		if ( prop->ptype!=PT_double )
		{
			gl_error("property '%s' in object '%s' is not a double", pname, oname);
			return 0;
		}
		*(map[n].var) = (double*)gl_get_addr(obj,pname);

		// parse constraint
		if ( map[n].op!=NULL )
		{
			char varname[256], opname[32], value[64];
			bool (*op)(double,double);
			switch (sscanf(constraint.get_string(), "%[^ \t<=>!]%[<=>!]%s", varname, opname, value)) 
			{
			case 0:
				break;
			case 1:
				gl_error("constraint '%s' in object '%s' is missing a valid comparison operator", constraint.get_string(), oname);
				return 0;
			case 2:
				gl_error("constraint '%s' in object '%s' is missing a valid comparison value", constraint.get_string(), oname);
				return 0;
			case 3:
				op = getop(opname);
				if ( op == NULL )
				{	
					gl_error("constraint '%s' in object '%s' operator '%s' is invalid", constraint.get_string(), oname, op);
					return 0;
				}
				*(map[n].value) = atof(value);
				break;
			default:
				gl_error("constraint '%s' in object '%s' parsing resulted in an internal error", constraint.get_string(), oname);
				return 0;
			}
		}
	}

	// check delta and epsilon 
	if ( delta <= 0 )
	{
		gl_error("Decision variable 'delta' in simple optimizer object '%s' must be a positive value", gl_name(my,buffer,sizeof(buffer))?buffer:"???");
		return 0;
	}
	if ( epsilon <= 0 )
	{
		gl_error("Objective 'epsilon' in simple optimizer object '%s' must be a positive value", gl_name(my,buffer,sizeof(buffer))?buffer:"???");
		return 0;
	}

	// read the current values
	if ( pObjective != NULL )
	{
		last_y = *pObjective;
	}
	else
	{
		gl_error("The property 'objective' must be set in simple optimizer object '%s'", gl_name(my,buffer,sizeof(buffer))?buffer:"???");
		return 0;
	}
	if ( pVariable != NULL)
	{
		next_x = last_x = *pVariable;
	}
	else
	{
		gl_error("The property 'variable' must be set in simple optimizer object '%s'", gl_name(my,buffer,sizeof(buffer))?buffer:"???");
		return 0;
	}

	// check the trial limit
	if ( trials < 0 )
	{
		gl_error("The trial limit 'trials' in simple optimizer object '%s' must be zero or a positive integer value", gl_name(my,buffer,sizeof(buffer))?buffer:"???");
		return 0;
	}

	gl_verbose("optimization for %s:", gl_name(my,buffer,sizeof(buffer)));
	gl_verbose("  %s(%s)", goal==OG_MINIMUM?"minimum":(goal==OG_MAXIMUM?"maximum":"extremum"),objective.get_string());
	gl_verbose("    given %s", variable.get_string());
	if ( pConstraint )
	{
		gl_verbose("    subject to %s",constraint.get_string());
	}

	return 1; /* return 1 on success, 0 on failure */
}

	/* Presync is called when the clock needs to advance on the first top-down pass */
TIMESTAMP simple::presync(TIMESTAMP t1)
{
	// first pass is never for a constraint
	if ( t1 > gl_globalclock ) 
	{
		search_step = 0;
	}

	// if more tries needed 
	switch (pass) 
	{
	case 0: // zero-order estimate
		last_x = next_x;
		*pVariable = next_x - delta;
		break;
	case 1: // first-order estimate
		*pVariable = next_x;
		break;
	case 2: // second-order estimate
		*pVariable = next_x + delta;
		break;
	case 3: // finalize
		*pVariable = next_x;
		break;
	default:
		break;
	}
	return TS_NEVER; /* return t2>t1 on success, t2=t1 for retry, t2<t1 on failure */
}

/* Postsync is called when the clock needs to advance on the second top-down pass */
TIMESTAMP simple::postsync(TIMESTAMP t1)
{
	OBJECT *my = THISOBJECTHDR;
	char buffer[1024];

	// trial limit reached or objective cannot be calculated
	if ( trials > 0 && trial > trials )
	{
		gl_error("The trial limit of %d in simple optimizer object '%s' has been reached", trials, gl_name(my,buffer,sizeof(buffer))?buffer:"???");
		return TS_INVALID; /* stop */
	}
	if ( isnan(*pObjective) || !isfinite(*pObjective) ) 
	{
		gl_error("The objective '%s' in simple optimizer object '%s' is infinite or indeterminate", objective.get_string(), gl_name(my,buffer,sizeof(buffer))?buffer:"???");
		return TS_INVALID; /* stop */
	}

	switch ( pass ) 
	{
	case 0:	// zero order estimate
		last_y = *pObjective;
		gl_verbose("y  = %.4f", last_y);
		pass = 1;
		return t1; // need another pass
	case 1:	// first order estimate
		last_dy = (*pObjective - last_y)/delta;
		last_y = *pObjective;
		pass = 2;
		return t1; // need another pass
	case 2: { // second order estimate
		double dy = (*pObjective - last_y)/delta;
		double ddy = (dy - last_dy)/delta;
		dy = (dy+last_dy)/2;
		gl_verbose("y' = %.4f", dy);
		gl_verbose("y\" = %.4f", ddy);
		if ( fabs(dy)<epsilon )
		{
			pass = 3;
		}
		else 
		{
			if ( ddy == 0 )
			{
				gl_error("The objective '%s' in simple optimizer object '%s' does not appear to have a non-zero second derivative near '%s=%g', which cannot lead to an extremum", objective.get_string(), gl_name(my,buffer,sizeof(buffer))?buffer:"???", variable.get_string(), last_x);
				return TS_INVALID;
			}
			else if ( ddy < 0 && goal == OG_MINIMUM )
			{
				gl_error("The minimum objective '%s' in '%s' cannot be found from '%s=%g'", objective.get_string(), gl_name(my,buffer,sizeof(buffer))?buffer:"???", variable.get_string(), last_x);
				return TS_INVALID;
			}
			else if ( ddy > 0 && goal == OG_MAXIMUM )
			{
				gl_error("The maximum objective '%s' in '%s' cannot be found from '%s=%g'", objective.get_string(), gl_name(my,buffer,sizeof(buffer))?buffer:"???", variable.get_string(), last_x);
				return TS_INVALID;
			}
			next_x = last_x - dy/ddy;
			gl_verbose("x <- %.4f", next_x);
			if ( pConstraint )
			{
				// determine which constaint violated
				bool violation = constraint_broken(*pConstraint);

				if ( search_step > 0 && fabs(search_step) < epsilon )
				{
					pass = 3;
				}

				// if the constraint is on the decision variable
				else if ( pConstraint == pVariable )
				{
					// and it is constrained
					if ( violation )
					{
						// no brainer--we're done
						*pVariable = constrain.value;
						gl_verbose("%s constrained to %g", variable.get_string(), constrain.value);
						pass = 3;
					}
				}
				else if ( violation ) // out of bounds
				{
					gl_verbose("constraint %s violated", constraint.get_string());
					if ( search_step != 0 ) // was constrained
					{
						// not far enough
						search_step /= 2;
						next_x = last_x + search_step;
						pass = 0;
					}
					else // newly constrained
					{
						// half step
						search_step = -(dy/ddy);
						next_x = last_x + search_step;
						pass = 0;
					}
				}
				else if ( search_step != 0 ) // was constrained but is in bounds now
				{
					// too far
					search_step /= 2;
					next_x = last_x - search_step;
					pass = 0;
				}
				else // not constrained (yet)
				{
					// do nothing
					pass = 0;
				}
			}
			else
			{
				pass = 0;
			}
			gl_verbose("x <- %.4f", next_x);
		}
		*pVariable = last_x;
		trial++;
		return t1;
		} 
	case 3:
	default:
		pass=0;
		return TS_NEVER;
	}
}

bool simple::constraint_broken(double x)
{
	return ! constrain.op(x,constrain.value);
}
