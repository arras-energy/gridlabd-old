// module/resilience/impact.cpp
// Copyright (C) 2022 Regents of the Leland Stanford Junior University

#include "impact.h"

EXPORT_CREATE(impact);
EXPORT_INIT(impact);
EXPORT_COMMIT(impact);

CLASS *impact::oclass = NULL;
impact *impact::defaults = NULL;

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
				PT_DESCRIPTION, "number of customer impact days",

			PT_char256, "asset_class", get_asset_class_offset(),
				PT_REQUIRED,
				PT_DESCRIPTION, "class of asset on which to measure metric",
			
			NULL)<1){
				char msg[256];
				sprintf(msg, "unable to publish properties in %s",__FILE__);
				throw msg;
		}
	}
}

int impact::create(void) 
{
	return 1; /* return 1 on success, 0 on failure */
}

int impact::init(OBJECT *parent)
{
	return 1;
}

TIMESTAMP impact::commit(TIMESTAMP t1, TIMESTAMP t2)
{
	return TS_NEVER;
}
