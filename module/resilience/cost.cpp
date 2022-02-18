// module/resilience/cost.cpp
// Copyright (C) 2022 Regents of the Leland Stanford Junior University

#include "cost.h"

EXPORT_CREATE(cost);
EXPORT_INIT(cost);
EXPORT_COMMIT(cost);

CLASS *cost::oclass = NULL;
cost *cost::defaults = NULL;

cost::cost(MODULE *module)
{
	if (oclass==NULL)
	{
		// register to receive notice for first top down. bottom up, and second top down synchronizations
		oclass = gld_class::create(module,"cost",sizeof(cost),PC_AUTOLOCK|PC_OBSERVER);
		if (oclass==NULL)
			throw "unable to register class cost";
		else
			oclass->trl = TRL_PROVEN;

		defaults = this;
		if (gl_publish_variable(oclass,

			PT_double, "value[$]", get_value_offset(),
				PT_DESCRIPTION, "number of customer cost days",

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

int cost::create(void) 
{
	return 1; /* return 1 on success, 0 on failure */
}

int cost::init(OBJECT *parent)
{
	return 1;
}

TIMESTAMP cost::commit(TIMESTAMP t1, TIMESTAMP t2)
{
	return TS_NEVER;
}
