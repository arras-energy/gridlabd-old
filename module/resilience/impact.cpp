// module/resilience/impact.cpp
// Copyright (C) 2022 Regents of the Leland Stanford Junior University

#include "impact.h"
#include "metrics.h"

EXPORT_CREATE(impact);
EXPORT_INIT(impact);
EXPORT_COMMIT(impact);

CLASS *impact::oclass = NULL;
impact *impact::defaults = NULL;

double impact::default_impact_duration = 48.0;

impact::impact(MODULE *module)
{
	if (oclass==NULL)
	{
		// register to receive notice for first top down. bottom up, and second top down synchronizations
		oclass = gld_class::create(module,"impact",sizeof(impact),PC_AUTOLOCK|PC_OBSERVER);
		if (oclass==NULL)
			throw "unable to register class impact";
		else
			oclass->trl = TRL_PROVEN;

		defaults = this;
		if (gl_publish_variable(oclass,

			PT_double, "value[$]", get_value_offset(),
				PT_DESCRIPTION, "cumulative number of reportable violations events detected",

			PT_object, "node", get_node_offset(),
				PT_REQUIRED,
				PT_DESCRIPTION, "node at which violations are monitored",
			
			PT_double, "duration[h]", get_duration_offset(),
				PT_DESCRIPTION, "reportable violation event duration threshold",

			NULL)<1){
				char msg[256];
				snprintf(msg,sizeof(msg)-1, "unable to publish properties in %s",__FILE__);
				throw msg;
		}

        gl_global_create("resilience::default_impact_duration",
            PT_double, &impact::default_impact_duration,
            PT_UNITS, "h",
            PT_ACCESS, PA_PUBLIC,
            PT_DESCRIPTION, "default reportable violation duration threshold",
            NULL);
	}
}

int impact::create(void) 
{
	value = 0.0;
	node = NULL;
	status = NULL;
	start = TS_NEVER;
	duration = 0.0;
	return 1; /* return 1 on success, 0 on failure */
}

int impact::init(OBJECT *parent)
{
	if ( node == NULL )
	{
		error("node to monitor is not specified");
		return 0;
	}
	status = (set*)gl_get_addr(node,"violation_detected");
	if ( status == NULL )
	{
		error("unable to find violation_detected property in node");
		return 0;
	}
	if ( duration <= 0 )
	{
		duration = default_impact_duration;
	}
	return 1;
}

TIMESTAMP impact::commit(TIMESTAMP t1, TIMESTAMP t2)
{
	if ( *status != 0 && start == TS_NEVER)
	{
		debug("%s violation detected",(const char*)node->name);
		start = gl_globalclock;
	}
	else if ( *status == 0 && start != TS_NEVER )
	{
		debug("%s violation resolved",(const char*)node->name);
		if ( gl_globalclock > start + duration*3600 )
		{
			debug("long %s violation reported",(const char*)node->name);
			metrics* pMetrics = (metrics*)get_parent();
			pMetrics->report_impact(1);
			value += 1;
		}
		start = TS_NEVER;
	}
	return TS_NEVER;
}
