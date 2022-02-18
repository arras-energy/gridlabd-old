// module/resilience/outage.cpp
// Copyright (C) 2022 Regents of the Leland Stanford Junior University

#include "outage.h"

EXPORT_CREATE(outage);
EXPORT_INIT(outage);
EXPORT_COMMIT(outage);

CLASS *outage::oclass = NULL;
outage *outage::defaults = NULL;

outage::outage(MODULE *module)
{
	if (oclass==NULL)
	{
		// register to receive notice for first top down. bottom up, and second top down synchronizations
		oclass = gld_class::create(module,"outage",sizeof(outage),PC_AUTOLOCK|PC_OBSERVER);
		if (oclass==NULL)
			throw "unable to register class outage";
		else
			oclass->trl = TRL_PROVEN;

		defaults = this;
		if (gl_publish_variable(oclass,

			PT_double, "value[unit*day]", get_value_offset(),
				PT_DESCRIPTION, "number of customer outage days",

			PT_char256, "meter_class", get_meter_class_offset(),
				PT_REQUIRED,
				PT_DESCRIPTION, "class of meter on which to measure metric",
			
			NULL)<1){
				char msg[256];
				sprintf(msg, "unable to publish properties in %s",__FILE__);
				throw msg;
		}
	}
}

int outage::create(void) 
{
	return 1; /* return 1 on success, 0 on failure */
}

int outage::init(OBJECT *parent)
{
	return 1;
}

TIMESTAMP outage::commit(TIMESTAMP t1, TIMESTAMP t2)
{
	return TS_NEVER;
}
