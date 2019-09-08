/** assert/assert.cpp
    Copyright 2014 Battelle Memorial Institute

    @file assert.cpp
    @addtogroup assert
    @ingroup assert

    The \p assert class implements general purpose assert objects

 @{
 **/

#include "assert.h"

EXPORT_CREATE_C(assert,g_assert);
EXPORT_INIT_C(assert,g_assert);
EXPORT_COMMIT_C(assert,g_assert);
EXPORT_NOTIFY_C(assert,g_assert);

CLASS *g_assert::oclass = NULL;
g_assert *g_assert::defaults = NULL;

g_assert::g_assert(MODULE *module)
{
	if (oclass==NULL)
	{
		// register to receive notice for first top down. bottom up, and second top down synchronizations
		oclass = gld_class::create(module,"assert",sizeof(g_assert),PC_AUTOLOCK|PC_OBSERVER);
		if (oclass==NULL)
			throw "unable to register class assert";
		else
			oclass->trl = TRL_PROVEN;

		defaults = this;
		if (gl_publish_variable(oclass,
			PT_enumeration,"status",get_status_offset(),PT_DEFAULT,"INIT",PT_DESCRIPTION,"desired outcome of assert test",
				PT_KEYWORD,"INIT",(enumeration)AS_INIT,
				PT_KEYWORD,"TRUE",(enumeration)AS_TRUE,
				PT_KEYWORD,"FALSE",(enumeration)AS_FALSE,
				PT_KEYWORD,"NONE",(enumeration)AS_NONE,
			PT_char1024, "target", get_target_offset(),PT_DESCRIPTION,"the target property to test",
			PT_char32, "part", get_part_offset(),PT_DESCRIPTION,"the target property part to test",
			PT_enumeration,"relation",get_relation_offset(),PT_DESCRIPTION,"the relation to use for the test",
				PT_KEYWORD,"==",(enumeration)TCOP_EQ,
				PT_KEYWORD,"<",(enumeration)TCOP_LT,
				PT_KEYWORD,"<=",(enumeration)TCOP_LE,
				PT_KEYWORD,">",(enumeration)TCOP_GT,
				PT_KEYWORD,">=",(enumeration)TCOP_GE,
				PT_KEYWORD,"!=",(enumeration)TCOP_NE,
				PT_KEYWORD,"inside",(enumeration)TCOP_IN,
				PT_KEYWORD,"outside",(enumeration)TCOP_NI,
			PT_char1024, "value", get_value_offset(),PT_DESCRIPTION,"the value to compare with for binary tests",
			PT_char1024, "within", get_value2_offset(),PT_DESCRIPTION,"the bounds within which the value must bed compared",
			PT_char1024, "lower", get_value_offset(),PT_DESCRIPTION,"the lower bound to compare with for interval tests",
			PT_char1024, "upper", get_value2_offset(),PT_DESCRIPTION,"the upper bound to compare with for interval tests",
			PT_char1024,"group", get_group_offset(),PT_DEFAULT,"",PT_DESCRIPTION,"a target group specification to use instead of parent object",
			PT_timestamp,"start", get_start_offset(),PT_DEFAULT,"INIT",PT_DESCRIPTION,"time to start assertion",
			PT_timestamp,"stop", get_stop_offset(),PT_DEFAULT,"NEVER",PT_DESCRIPTION,"time to stop assertion",
			PT_double,"hold[s]", get_hold_offset(),PT_DEFAULT,"0",PT_DESCRIPTION,"a duration over which the assertion must be violated before failing",
			NULL)<1){
				char msg[256];
				sprintf(msg, "unable to publish properties in %s",__FILE__);
				throw msg;
		}
	}
	init_status();
	init_target();
	init_part();
	init_relation();
	init_value();
	init_value2();
	init_group();
	init_hold();
	init_start();
	init_stop();
}

int g_assert::create(void) 
{
	target_list = NULL;
	started = TS_NEVER;
	return 1; /* return 1 on success, 0 on failure */
}

int g_assert::init(OBJECT *parent)
{
	gl_verbose("group = '%s'", get_group());

	target_list = new std::list<gld_property>;
	if ( strcmp(get_group(),"") == 0 )
	{
		gld_property* target = new gld_property(parent,get_target());
		if ( ! target->is_valid() )
		{
			exception("target '%s' property '%s' does not exist", parent?get_parent()->get_name():"global",get_target());
		}
		target_list->push_back(*target);
	}
	else
	{
		gld_finder *finder = new gld_finder();
		if ( ! finder )
		{
			exception("unable to create finder for group '%s'", get_group());
		}
		if ( ! finder->create(get_group()) )
		{
			exception("group '%s' is not valid", get_group());
		}
		OBJECT *obj;
		for ( obj = finder->get_first() ; obj != NULL ; obj = finder->get_next(obj) )
		{
			gld_property *target = new gld_property(obj,get_target());
			if ( ! target->is_valid() )
			{
				exception("group target '%s' property '%s' does not exist", get_object(obj)->get_name(),get_target());
			}
			target_list->push_back(*target);
		}
		if ( target_list->size() == 0 )
		{
			gl_warning("group '%s' contains no objects",get_group());
		}
	}
	set_status(AS_TRUE);
	return 1;
}

TIMESTAMP g_assert::commit(TIMESTAMP t1, TIMESTAMP t2)
{
	// determine the relation status
	if ( status==AS_NONE ) 
	{
		gl_verbose("%s: assert test is not being run", get_name());
		return TS_NEVER;
	}
	else if ( t1 < start )
	{
		return start;
	}
	else if ( t1 > stop )
	{
		return TS_NEVER;
	}

	// process tests
	int start_count = 0;
	for ( std::list<gld_property>::iterator target_prop = target_list->begin() ; target_prop != target_list->end() ; target_prop++ )
	{
		OBJECT *obj = target_prop->get_object();
		if ( evaluate_status(*target_prop) != get_status() )
		{
			if ( hold == 0 || t1 > started + hold )
			{
				gld_property relation_prop(my(),"relation");
				gld_keyword *pKeyword = relation_prop.find_keyword(relation);
				char buf[1024];
				gl_error("%s: assert failed on %s %s.%s.%s %s %s %s %s", get_name(), status==AS_TRUE?"":"NOT",
					obj?get_object(obj)->get_name():"global variable", get_target(), get_part(), target_prop->to_string(buf,sizeof(buf))?buf:"(void)", pKeyword->get_name(), get_value(), get_value2());
				return TS_INVALID;
			}
			start_count++;
			if ( started == TS_NEVER ) 
			{
				started = t1;
				gl_verbose("%s: assert %s started at %lld with hold for %g s", 
					get_name(), obj?get_object(obj)->get_name():"global variable",
					t1, hold);
			}
		}
		else
		{
			gl_verbose("%s: assert passed on %s", get_name(), obj?get_object(obj)->get_name():"global variable");
		}
	}

	// reset started if no failures detected
	if ( start_count == 0 )
	{
		started = TS_NEVER;
	}
	return TS_NEVER;
}

g_assert::ASSERTSTATUS g_assert::evaluate_status(gld_property &target_prop)
{
	if ( strcmp(get_part(),"")==0 )
		return target_prop.compare(relation,get_value(),get_value2()) ? AS_TRUE : AS_FALSE ;
	else
		return target_prop.compare(relation,get_value(),get_value2(),get_part()) ? AS_TRUE : AS_FALSE ;
}

int g_assert::postnotify(PROPERTY *prop, const char *value)
{
	// TODO notify handler for changed value
	return 1;
}

/** @} **/
