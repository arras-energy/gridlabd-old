// module/loads/building.cpp
// Copyright (C) 2022 Regents of the Leland Stanford Junior University

#include "powerflow.h"

EXPORT_CREATE(building);
EXPORT_INIT(building);
EXPORT_SYNC(building);
EXPORT_METHOD(building,airtemperature_history);
EXPORT_METHOD(building,masstemperature_history);
EXPORT_METHOD(building,building_response);
EXPORT_METHOD(building,input);
EXPORT_METHOD(building,zip);

CLASS *building::oclass = NULL;
building *building::defaults = NULL;

building::building(MODULE *module)
: load(module)
{
	if (oclass==NULL)
	{
		// register to receive notice for first top down. bottom up, and second top down synchronizations
		oclass = gld_class::create(module,"building",sizeof(building),PC_PRETOPDOWN|PC_BOTTOMUP|PC_POSTTOPDOWN|PC_UNSAFE_OVERRIDE_OMIT|PC_AUTOLOCK);
		if (oclass==NULL)
			throw "unable to register class building";
		else
			oclass->trl = TRL_PROVEN;

		defaults = this;
		if (gl_publish_variable(oclass,

			PT_INHERIT, "load",

			PT_double, "timestep[s]", get_timestep_offset(),
				PT_DEFAULT, "60s",
				PT_DESCRIPTION, "timestep to use when modeling building response to inputs",

			PT_double, "output_timestep[s]", get_output_timestep_offset(),
				PT_DESCRIPTION, "timestep to use when outputting response to inputs",

			PT_method, "airtemperature_history", method_building_airtemperature_history,
				PT_DESCRIPTION, "air temperature history",
			
			PT_method, "masstemperature_history", method_building_masstemperature_history,
				PT_DESCRIPTION, "mass temperature history",
			
			PT_method, "building_response", method_building_building_response,
				PT_DESCRIPTION, "building response parameters (second-order transfer function denominator terms)",
			
			PT_method, "input", method_building_input,
				PT_DESCRIPTION, "transfer function input specification input (source and numerator coefficients)",

			PT_method, "zip", method_building_zip,
				PT_DESCRIPTION, "ZIP real and reactive load components",

			PT_double, "air_temperature", PADDR(x[0][0]), 
				PT_ACCESS, PA_REFERENCE,
				PT_DESCRIPTION, "indoor air temperature",

			PT_double, "mass_temperature", PADDR(x[1][0]),
				PT_ACCESS, PA_REFERENCE,
				PT_DESCRIPTION, "building mass temperature",
			
			NULL)<1){
				char msg[256];
				snprintf(msg,sizeof(msg)-1, "unable to publish properties in %s",__FILE__);
				throw msg;
		}
	}
}

int building::create(void) 
{
	return load::create(); /* return 1 on success, 0 on failure */
}

void building::check_poles(double *a,const char *name)
{
	double zr = a[1]*a[1] - 4*a[0]*a[2];
	if ( zr < 0 ) // complex poles
	{
		double x = -0.5*a[1]/a[0];
		double y = 0.5*sqrt(-zr)/a[0];
		if ( sqrt(x*x + y*y) >= 1.0 )
		{
			warning("building %s response is not stable (poles are %f+/-%fj)",name,x,y);
		}
		p0 = complex(x,y);
		p1 = complex(x,-y);
	}
	else // real poles
	{
		double z0 = 0.5/a[0]*(-a[1]-sqrt(zr));
		double z1 = 0.5/a[0]*(-a[1]+sqrt(zr));
		if ( z0 >= 1.0 || z1 >= 1.0 )
		{
			warning("building %s response is not stable (poles are %f+,%f)",name,z0,z1);
		}
		p0 = complex(z0,0);
		p1 = complex(z1,0);
	}
}

int building::init(OBJECT *parent)
{
	if ( timestep <= 0 )
	{
		exception("timestep must be positive");
	}
	if ( output_timestep == 0.0 )
	{
		output_timestep = timestep;
	}
	if ( input_list == NULL )
	{
		warning("no inputs specified");
	}
	for ( INPUT *input = input_list ; input != NULL ; input = input->next )
	{
		if ( input->state < 0 || input->state > 1 )
		{
			exception("%s state %d is not valid",input->source,input->state);
		}
		if ( input->b[0] == 0.0 && input->b[1] == 0.0 && input->b[2] == 0.0 )
		{
			warning("%s inputs response is null",input->source);
		}
	}
	if ( a[0][0] == 0.0 && a[0][1] == 0.0 && a[0][2] == 0.0 )
	{
		warning("building air response is null");
	}
	else
	{
		check_poles(a[0],"air");
	}
	if ( a[1][0] == 0.0 && a[1][1] == 0.0 && a[1][2] == 0.0 )
	{
		warning("building mass response is null");
	}
	else
	{
		check_poles(a[1],"mass");
	}
	return load::init(parent);
}

TIMESTAMP building::precommit(TIMESTAMP t1, TIMESTAMP t2)
{
	if ( t2 % (int)timestep == 0 )
	{
		update_input();
		update_state((t2-t1)/timestep);
		update_output();
	}
	return TS_NEVER;
}

TIMESTAMP building::presync(TIMESTAMP t0)
{
	TIMESTAMP t2 = load::presync(t0);
	TIMESTAMP t1 = (TIMESTAMP)((TIMESTAMP)(t0/output_timestep)+1)*output_timestep;
	return t1 < t2 ? -t1 : t2;
}

TIMESTAMP building::sync(TIMESTAMP t0)
{
	return load::sync(t0);
}

int building::airtemperature_history(char *buffer, size_t len)
{
	if ( buffer != NULL && len == 0 ) // read values in buffer
	{
		return sscanf(buffer,"%lf,%lf,%lf",&x[0][0],&x[0][1],&x[0][2]);
	}
	else // save values to buffer (or return buffer size if buffer is null)
	{
		return snprintf(buffer,len,"%lf,%lf,%lf",x[0][0],x[0][1],x[0][2]);
	}
}

int building::masstemperature_history(char *buffer, size_t len)
{
	if ( buffer != NULL && len == 0 ) // read values in buffer
	{
		return sscanf(buffer,"%lf,%lf,%lf",&x[1][0],&x[1][1],&x[1][2]);
	}
	else // save values to buffer (or return buffer size if buffer is null)
	{
		return snprintf(buffer,len,"%lf,%lf,%lf",x[1][0],x[1][1],x[1][2]);
	}
}

int building::building_response(char *buffer, size_t len)
{
	if ( buffer != NULL && len == 0 ) // read values in buffer
	{
		return sscanf(buffer,"%lf,%lf,%lf;%lf,%lf,%lf",&a[0][0],&a[0][1],&a[0][2],&a[1][0],&a[1][1],&a[1][2]);
	}
	else // save values to buffer (or return buffer size if buffer is null)
	{
		return snprintf(buffer,len,"%lf,%lf,%lf;%lf,%lf,%lf",a[0][0],a[0][1],a[0][2],a[1][0],a[1][1],a[1][2]);
	}
}

int building::input(char *buffer, size_t len)
{
	char objectname[64];
	char propertyname[64];
	double b0,b1,b2;
	double d0, d1;
	double u0=0,u1=0,u2=0;
	int state;
	if ( buffer != NULL && len == 0 )
	{
		if ( sscanf(buffer,"%63[^.].%63[^.],%d,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf",objectname,propertyname,&state,&b0,&b1,&b2,&d0,&d1,&u0,&u1,&u2) < 7 )
		{
			exception("input '%s' specification is not valid",buffer);
		}
		INPUT *last = input_list;
		input_list = new INPUT;
		asprintf(&(input_list->source),"%s.%s",objectname,propertyname);
		input_list->prop = new gld_property(objectname,propertyname);
		if ( ! input_list->prop->is_valid() )
		{
			exception("input '%s' not found",input_list->source);
		}
		input_list->state = state;
		input_list->b[0] = b0;
		input_list->b[1] = b1;
		input_list->b[2] = b2;
		input_list->d[0] = d0;
		input_list->d[1] = d1;
		input_list->u[0] = u0;
		input_list->u[1] = u1;
		input_list->u[2] = u2;
		input_list->addr = (double*)input_list->prop->get_addr();
		input_list->next = last;
		return strlen(buffer);
	}
	else if ( buffer == NULL )
	{
		int result = 0;
		for ( INPUT *item = input_list ; item != NULL ; item = item->next )
		{
			result += snprintf(NULL,0,"%s,%d,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",
				item->source,item->state,item->b[0],item->b[1],item->b[2],item->d[0],item->d[1],item->u[0],item->u[1],item->u[2]);
		}
		return result;
	}
	else
	{
		int result = 0;
		for ( INPUT *item = input_list ; item != NULL ; item = item->next )
		{
			result += snprintf(buffer+result,len-result,"%s,%d,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",
				item->source,item->state,item->b[0],item->b[1],item->b[2],item->d[0],item->d[1],item->u[0],item->u[1],item->u[2]);
		}
		return result;
	}
}

void building::update_input(void)
{
	// update history
	for ( INPUT *input = input_list ; input != NULL ; input = input->next )
	{
		input->u[2] = input->u[1];
		input->u[1] = input->u[0];
		input->u[0] = *input->addr;
	}
}

void building::update_state(unsigned int n)
{
	// update timesteps
	// TODO: use poles to do this faster when doing more than 3 timesteps
	while ( n-- > 0 )
	{
		// initialize update
		x[0][2] = x[0][1]; 
		x[0][1] = x[0][0]; 
		x[0][0] = -a[0][1]*x[0][1] - a[0][2]*x[0][2];
		x[1][2] = x[1][1]; 
		x[1][1] = x[1][0]; 
		x[1][0] = -a[1][1]*x[1][1] - a[1][2]*x[1][2];

		// add inputs
		for ( INPUT *input = input_list ; input != NULL ; input = input->next )
		{
			x[input->state][0] += input->b[0]*input->u[0] + input->b[1]*input->u[1] + input->b[2]*input->u[2];
		}

		// finalize update
		x[0][0] /= a[0][0];
		x[1][0] /= a[1][0];
	}
}

void building::update_output(void)
{
	double P = 0.0;
	double Q = 0.0;
	for ( INPUT *input = input_list ; input != NULL ; input = input->next )
	{
		P += input->d[0]*input->u[0];
		Q += input->d[1]*input->u[0];
	}
	constant_power[0] = constant_power[1] = constant_power[2] = complex(P,Q)/3.0;
	return;
}