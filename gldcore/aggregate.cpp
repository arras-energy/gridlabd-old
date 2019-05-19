/** aggregate.cpp
	Copyright (C) 2008 Battelle Memorial Institute

	@file aggregate.cpp
	@addtogroup aggregate Aggregation of object properties
	@ingroup core
	
	Aggregation functions support calculations over properties of multiple objects.
	This is used primarily by the collector object in the tape module to define groups
	(see the \p group property in aggregate_mkgroupthe collector object).  The \p group can be defined
	by specifying a boolean series are relationship of object properties, e.g.,
	\verbatim class=node and parent=root \endverbatim.
	
	Aggregations also must specify the property that is to be aggregated.  Most common
	aggregations and some uncommon ones are supported.  In addition, if the aggregation is
	over a complex quantity, the aggregation must specific how a double is to be obtained
	from it, e.g., magnitude, angle, real, imaginary.  Some examples aggregate \p property 
	definitions are
	\verbatim
	sum(cost)
	max(power.angle)
	mean(energy.mag)
	std(price)
	\endverbatim

	@bug Right now, not all allowed aggregations are invariant (meaning that the members of the group
	do not change over time).  However, the collector object requires invariant aggregations.  Using 
	an aggregation that isn't invariant will cause the simulation to fail. (ticket #112)
 @{
 **/

#include "gldcore.h"

//TODO: uncomment if context warnings 
// SET_MYCONTEXT(DMC_AGGREGATE)

DEPRECATED CDECL AGGREGATION *aggregate_mkgroup(const char *aggregator, /**< aggregator (min,max,avg,std,sum,prod,mbe,mean,var,skew,kur,count,gamma) */
							   const char *group_expression) /**< grouping rule; see find_pgm_new(char *)*/
{
	try 
	{
		GldAggregator *aggr = new GldAggregator(aggregator,group_expression);
		return aggr->get_aggregator();
	}
	catch (...)
	{
		return NULL;
	}
}
DEPRECATED CDECL double aggregate_value(AGGREGATION *aggr) /**< the aggregation to perform */
{
	return GldAggregator(aggr).get_value();
}

/** This constructor creates an instance of an existing aggregator.
	A reference to existing aggregation can be obtained using get_aggregator().
	The aggregation can be run using aggregate_value(AGGREGATION*).
 **/
GldAggregator::GldAggregator(AGGREGATION *a) ///< an existing aggregator
{
	aggr = a;
	aggr->refcnt++;
}

GldAggregator::~GldAggregator(void)
{
	if ( --aggr->refcnt == 0 )
		delete aggr;
}

/** This constructor builds a new collection of objects into an aggregation.  
	The aggregation can be run using \p get_value.
 **/
GldAggregator::GldAggregator(const char *aggregator, /**< aggregator (min,max,avg,std,sum,prod,mbe,mean,var,skew,kur,count,gamma) */
							 const char *group_expression) /**< grouping rule; see find_pgm_new(char *)*/
{
	AGGREGATOR op = AGGR_NOP;
	AGGREGATION *result=NULL;
	char aggrop[9], aggrval[257], *aggrpart = NULL, nulpart[1];
	char aggrprop[33], aggrunit[9];
	AGGRFLAGS flags = AF_NONE;
	nulpart[0] = '\0';

	//Change made for collector to handle propeties of objects
	OBJECT *obj = NULL;
	PROPERTY *pinfo=NULL;
	FINDPGM *pgm = NULL;
	FINDLIST *list=NULL;	
	//Change ends here

	UNIT *from_unit = NULL, *to_unit = NULL;
	double scale = 1.0;

	if (sscanf(aggregator," %8[A-Za-z0-9_](%256[][A-Za-z0-9_.^])",aggrop,aggrval)!=2 &&
		(flags=AF_ABS,
		sscanf(aggregator," %8[A-Za-z0-9_]|%256[][A-Za-z0-9_.^]|",aggrop,aggrval)!=2 
		))
	{
		output_error("aggregate group '%s' is not valid", aggregator);
		/* TROUBLESHOOT
			An aggregation expression does not have the required syntax, e.g., <i>aggregation</i>(<i>value</i>[.<i>part</i>]).
			Check the aggregation's syntax and make sure it conforms to the required syntax.
		 */
		errno = EINVAL;
		throw NULL;
	}

	//Change made for collector to handle propeties of objects
	pgm = find_pgm_new(group_expression);
	if(pgm != NULL){
		list = find_pgm_run(NULL,pgm);
		if(list != NULL){
			obj = find_first(list);
			if(obj != NULL){
				pinfo = class_find_property(obj->oclass,aggrval);
				if (pinfo==NULL)
				{
					aggrpart = strrchr(aggrval,'.');
					/* if an aggregate part is found */
					if (aggrpart!=NULL)
						*aggrpart++ = '\0';	// split the value and the part
					else
						aggrpart=nulpart; // no part given
				}
					else
				{
					aggrpart=nulpart; // no part given
				}
			}
		}
	}
	//Change ends here

	if(sscanf(aggrval, "%32[A-Za-z0-9_][%[A-Za-z0-9_^]]", aggrprop, aggrunit) == 2){
		to_unit = unit_find(aggrunit);
		if(to_unit == NULL){
			output_error("aggregate group '%s' has invalid units (%s)", aggrval, aggrunit);
			/* TROUBLESHOOT
				An aggregation expression include a unit specification in the value expression, but the unit is not found.
				Check your aggregations and make sure all the units are defined.
			 */
			errno = EINVAL;
			throw NULL;
		}
		strcpy(aggrval, aggrprop); // write property back into value, sans unit
	}

	if (stricmp(aggrop,"min")==0) op=AGGR_MIN;
	else if (stricmp(aggrop,"max")==0) op=AGGR_MAX;
	else if (stricmp(aggrop,"avg")==0) op=AGGR_AVG;
	else if (stricmp(aggrop,"std")==0) op=AGGR_STD;
	else if (stricmp(aggrop,"sum")==0) op=AGGR_SUM;
	else if (stricmp(aggrop,"prod")==0) op=AGGR_SUM;
	else if (stricmp(aggrop,"mbe")==0) op=AGGR_MBE;
	else if (stricmp(aggrop,"mean")==0) op=AGGR_MEAN;
	else if (stricmp(aggrop,"var")==0) op=AGGR_VAR;
	else if (stricmp(aggrop,"skew")==0) op=AGGR_SKEW;
	else if (stricmp(aggrop,"kur")==0) op=AGGR_KUR;
	else if (stricmp(aggrop,"count")==0) op=AGGR_COUNT;
	else if (stricmp(aggrop,"gamma")==0) op=AGGR_GAMMA;
	else
	{
		output_error("aggregate group '%s' does not use a known aggregator", aggregator);
		/* TROUBLESHOOT
			An aggregation expression uses an aggregator that is not defined.  
			Check that all your aggregators used allowed functions (e.g., min, max, avg, std, sum, count, etc.).
		 */
		errno = EINVAL;
		throw NULL;
	}
	if (op!=AGGR_NOP)
	{		
		AGGRPART part=AP_NONE;	
		
		if (pgm==NULL)
		{
			output_error("aggregate group expression '%s' failed", group_expression);
			/* TROUBLESHOOT
				A group expression failed to generate a useful group.  
				Check that all your groups are correctly defined.
			 */
			errno = EINVAL;
			throw NULL;
		}
		else
		{
			PGMCONSTFLAGS flags = find_pgmconstants(pgm); 
			
			/* the search must be over the same class so that the property offset is known in advance */
			if ((flags&CF_CLASS)!=CF_CLASS)
			{
				output_error("aggregate group expression '%s' does not result in a set with a fixed class", group_expression);
				/* TROUBLESHOOT
					A group expression generated a group whose members vary over time.  
					This is not allowed.  
					Check that all your groups are defined such that the group membership is constant.
				 */
				errno = EINVAL;
				find_pgm_delete(pgm);
				pgm = NULL;
				throw NULL;
			}
			else
			{				
				if (list==NULL)
				{
					output_error("aggregate group expression '%s' does not result is a usable object list", group_expression);
					/* TROUBLESHOOT
						A group expression failed to generate a useful group.  
						Check that all your groups are correctly defined.
					 */
					find_pgm_delete(pgm);
					pgm = NULL;
					errno=EINVAL;
					throw NULL;
				}
				
				if (obj==NULL)
				{
					output_error("aggregate group expression '%s' results is an empty object list", group_expression);
					/* TROUBLESHOOT
						A group expression generated an empty group.  
						Check that all your groups are correctly defined.
					 */
					find_pgm_delete(pgm);
					pgm = NULL;
					free(list);
					list = NULL;
					errno=EINVAL;
					throw NULL;
				}
				pinfo = class_find_property(obj->oclass,aggrval);
				if (pinfo==NULL)
				{
					output_error("aggregate group property '%s' is not found in the objects satisfying search criteria '%s'", aggrval, group_expression);
					/* TROUBLESHOOT
						A group expression failed to generate a group that contains objects that meet the required criteria.  
						Check that all your groups are correctly defined.
					 */
					errno = EINVAL;
					find_pgm_delete(pgm);
					pgm = NULL;
					free(list);
					list = NULL;
					throw NULL;
				}
				else if (pinfo->ptype==PT_double || pinfo->ptype==PT_random || pinfo->ptype==PT_loadshape )
				{
					if (strcmp(aggrpart,"")!=0)
					{	/* doubles cannot have parts */
						output_error("aggregate group property '%s' cannot have part '%s'", aggrval, aggrpart);
						/* TROUBLESHOOT
							An aggregate part refers to a property that is a real number and does not have any parts.  
							Check that all your aggregate parts refer a property with parts, e.g., a complex value.
						 */
						errno = EINVAL;
						find_pgm_delete(pgm);
						pgm = NULL;
						free(list);
						list = NULL;
						throw NULL;
					}
					part = AP_NONE;
				}
				else if (pinfo->ptype==PT_complex || pinfo->ptype==PT_enduse)
				{	/* complex must have parts */
					if (strcmp(aggrpart,"real")==0)
						part = AP_REAL;
					else if (strcmp(aggrpart,"imag")==0)
						part = AP_IMAG;
					else if (strcmp(aggrpart,"mag")==0)
						part = AP_MAG;
					else if (strcmp(aggrpart,"ang")==0)
						part = AP_ANG;
					else if (strcmp(aggrpart,"arg")==0)
						part = AP_ARG;
					else
					{
						output_error("aggregate group property '%s' cannot have part '%s'", aggrval, aggrpart);
						/* TROUBLESHOOT
							The aggregate part requested is not recognized for the property given.  
							Check that your aggregate part is defined for a complex value.
						 */
						errno = EINVAL;
						find_pgm_delete(pgm);
						pgm = NULL;
						free(list);
						list = NULL;
						throw NULL;
					}
				}
				else
				{
					output_error("aggregate group property '%s' cannot be aggregated", aggrval);
					/* TROUBLESHOOT
						The aggregate referred to a type of property that cannot be aggregated.  
						Check that your aggregate part refers to a numeric value.
					 */
					errno = EINVAL;
					find_pgm_delete(pgm);
					pgm = NULL;
					free(list);
					list = NULL;
					throw NULL;
				}
				from_unit = pinfo->unit;
				if(to_unit != NULL && from_unit == NULL){
					output_error("aggregate group property '%s' is unitless and cannot be converted", aggrval);
					/* TROUBLESHOOT
						The aggregate attempted to convert the units of a property that is unitless.  
						Check that your aggregate part does not include a unit specification.
					 */
					errno = EINVAL;
					find_pgm_delete(pgm);
					pgm = NULL;
					free(list);
					list = NULL;
					throw NULL;
				}
				if (from_unit != NULL && to_unit != NULL && unit_convert_ex(from_unit, to_unit, &scale) == 0){
					output_error("aggregate group property '%s' cannot use units '%s'", aggrval, aggrunit);
					/* TROUBLESHOOT
						The aggregate attempted to convert a property to a unit that is not compatible with the
						property's original unit.  Check that your aggregate uses a unit that is fundamentally 
						compatible.
					 */
					errno = EINVAL;
					find_pgm_delete(pgm);
					pgm = NULL;
					free(list);
					list = NULL;
					throw NULL;
				}
			}
		}

		/* build aggregation unit */
		result = (AGGREGATION*)malloc(sizeof(AGGREGATION));
		if (result!=NULL)
		{
			result->op = op;
			result->group = pgm;
			result->pinfo = pinfo;
			result->part = part;
			result->last = list;
			result->next = NULL;
			result->flags = flags;
			result->punit = to_unit;
			result->scale = scale;
		}
		else
		{
			errno=ENOMEM;
			find_pgm_delete(pgm);
			pgm = NULL;
			free(list);
			list = NULL;
			throw NULL;
		}
	}

	aggr = result;
	aggr->refcnt = 1;
}

/** This function performs an aggregate calculation given by the aggregation 
 **/
double GldAggregator::get_value(void)
{
	OBJECT *obj;
	double numerator=0, denominator=0, secondary=0;

	/* non-constant groups need search program rerun */
	if ((aggr->group->constflags & CF_CONSTANT) != CF_CONSTANT){
		aggr->last = find_pgm_run(NULL,aggr->group); /** @todo use constant part instead of NULL (ticket #3) */
	}

	for(obj = find_first(aggr->last); obj != NULL; obj = find_next(aggr->last, obj)){
		double value=0;
		double *pdouble = NULL;
		complex *pcomplex = NULL;

		/* add time-sensitivity to verify that we are only aggregating objects that are in-service and not out-service. */
		if(obj->in_svc >= global_clock || obj->out_svc <= global_clock)
			continue;

		switch (aggr->pinfo->ptype) {
		case PT_complex:
		case PT_enduse:
			pcomplex = object_get_complex(obj,aggr->pinfo);
			if (pcomplex!=NULL)
			{
				switch (aggr->part) {
				case AP_REAL: value = pcomplex->Re(); break;
				case AP_IMAG: value = pcomplex->Im(); break;
				case AP_MAG: value = pcomplex->Mag(); break;
				case AP_ARG: value = pcomplex->Arg(); break;
				case AP_ANG: value = pcomplex->Ang();  break;
				default: pcomplex = NULL; break; /* invalidate the result */
				}
			}
			break;
		case PT_double:
		case PT_loadshape:
		case PT_random:
			pdouble = object_get_double(obj,aggr->pinfo);
			if (pdouble!=NULL){
				value = *pdouble;
				if(aggr->pinfo->unit != 0 && aggr->punit != 0){
					int rv = unit_convert_ex(aggr->pinfo->unit, aggr->punit, &value);
					if(rv == 0){ // error
						;
					}
				} // else don't worry
			}
			break;
		default:
			break;
		}

		if (pdouble!=NULL || pcomplex!=NULL) /* valid value */
		{
			if ((aggr->flags&AF_ABS)==AF_ABS) value=fabs(value);
			switch (aggr->op) {
			case AGGR_MIN:
				if (value<numerator || denominator==0) numerator=value;
				denominator = 1;
				break;
			case AGGR_MAX:
				if (value>numerator || denominator==0) numerator=value;
				denominator = 1;
				break;
			case AGGR_COUNT:
				numerator++;
				denominator=1;
				break;
			case AGGR_MBE:
				denominator++;
				numerator += value;
				secondary += (value-secondary)/denominator;
				break;
			case AGGR_AVG:
			case AGGR_MEAN:
				numerator+=value;
				denominator++;
				break;
			case AGGR_SUM:
				numerator+=value;
				denominator = 1;
				break;
			case AGGR_PROD:
				numerator*=value;
				denominator = 1;
				break;
			case AGGR_GAMMA:
				denominator+=log(value);
				if (numerator==0 || secondary>value)
					secondary = value;
				numerator++;
				break;
			case AGGR_STD:
			case AGGR_VAR:
				denominator++;
				// note this uses a compensated on-line algorithm (see Knuth 1998)
				// it's better than the obvious method because it doesn't suffer from numerical instability when mean(x)-x is near zero
				{	double delta = value-secondary;
					secondary += delta/denominator;
					numerator += delta*(value-secondary);
				}
				break;
			case AGGR_SKEW:
			case AGGR_KUR:
			default:
				break;
			}
		}
	}
	switch (aggr->op) {
	case AGGR_GAMMA:
		return 1 + numerator/(denominator-numerator*log(secondary));
	case AGGR_STD:
		return sqrt(numerator/(denominator-1));// * scale;
	case AGGR_MBE:
		return numerator/denominator - secondary;
	case AGGR_SKEW:
		/** @todo implement skewness aggregate (no ticket) */
		throw_exception("skewness aggregation is not implemented");
		/* TROUBLESHOOT
			An attempt to use the skew aggregator failed because it is not implemented yet.
			Remove or replace the reference to the skew aggregate and try again.
		 */
	case AGGR_KUR:
		/** @todo implement kurtosis aggregate (no ticket) */
		throw_exception("kurtosis aggregation is not implemented");
		/* TROUBLESHOOT
			An attempt to use the kurtosis aggregator failed because it is not implemented yet.
			Remove or replace the reference to the
		 */
	default:
		return numerator/denominator;// * scale;
	}
}

/**@}**/
