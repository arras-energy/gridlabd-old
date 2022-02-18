// module/resilience/metrics.cpp
// Copyright (C) 2022 Regents of the Leland Stanford Junior University

#include "metrics.h"

EXPORT_CREATE(metrics);
EXPORT_INIT(metrics);
EXPORT_COMMIT(metrics);

CLASS *metrics::oclass = NULL;
metrics *metrics::defaults = NULL;

metrics::metrics(MODULE *module)
{
	if (oclass==NULL)
	{
		// register to receive notice for first top down. bottom up, and second top down synchronizations
		oclass = gld_class::create(module,"metrics",sizeof(metrics),PC_AUTOLOCK|PC_OBSERVER);
		if (oclass==NULL)
			throw "unable to register class metrics";
		else
			oclass->trl = TRL_PROVEN;

		defaults = this;
		if (gl_publish_variable(oclass,

			PT_double, "outage[unit*day]", get_outage_offset(),
				PT_DESCRIPTION, "total number of customer outage days",

			PT_double, "cost[$]", get_cost_offset(),
				PT_DESCRIPTION, "total repair cost",

			PT_int32, "impact", get_impact_offset(),
				PT_DESCRIPTION, "total emergency services impacted more than 48 hours per event",

			PT_char1024, "report_file", get_report_file_offset(),
				PT_DESCRIPTION, "file in which metric data is recorded",

			PT_enumeration, "report_frequency", get_report_frequency_offset(),
				PT_KEYWORD, "DAILY", (enumeration)MRF_DAILY,
				PT_KEYWORD, "WEEKLY", (enumeration)MRF_WEEKLY,
				PT_KEYWORD, "MONTHLY", (enumeration)MRF_MONTHLY,
				PT_KEYWORD, "SEASONALLY", (enumeration)MRF_SEASONALLY,
				PT_KEYWORD, "ANNUALLY", (enumeration)MRF_ANNUALLY,
				PT_DEFAULT, "ANNUALLY",
				PT_DESCRIPTION, "frequency with which metric data is recorded",
			
			NULL)<1){
				char msg[256];
				sprintf(msg, "unable to publish properties in %s",__FILE__);
				throw msg;
		}
	}
}

int metrics::create(void) 
{
	return 1; /* return 1 on success, 0 on failure */
}

int metrics::init(OBJECT *parent)
{
	return 1;
}

TIMESTAMP metrics::commit(TIMESTAMP t1, TIMESTAMP t2)
{
	return TS_NEVER;
}
