// module/resilience/outage.cpp
// Copyright (C) 2022 Regents of the Leland Stanford Junior University

#include "outage.h"
#include "metrics.h"

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

			PT_double, "value[unit*h]", get_value_offset(),
				PT_DESCRIPTION, "number of customer outage hours",

			PT_char256, "asset_class", get_asset_class_offset(),
				PT_DEFAULT, "powerflow_object",
				PT_DESCRIPTION, "class of powerflow object on which to measure outages",
			
			NULL)<1){
				char msg[256];
				snprintf(msg,sizeof(msg)-1, "unable to publish properties in %s",__FILE__);
				throw msg;
		}
	}
}

int outage::create(void) 
{
	assetlist = NULL;
	value = 0.0;
	return 1; /* return 1 on success, 0 on failure */
}

int outage::init(OBJECT *parent)
{
	for ( OBJECT *asset = gl_object_get_first() ; asset != NULL ; asset = asset->next )
	{
		if ( gl_object_isa(asset,asset_class) )
		{
			debug("adding object '%s' to assetlist", asset->name);
			add_asset(asset);
		}
	}
	return 1;
}

TIMESTAMP outage::commit(TIMESTAMP t1, TIMESTAMP t2)
{
	for ( ASSET *asset = get_first_asset() ; asset != NULL ; asset = get_next_asset(asset) )
	{
		if ( get_asset_start(asset) != TS_ZERO && get_asset_status(asset) == 0 )
		{
			debug("%s violation ends",(const char*)my()->name);
			TIMESTAMP duration = gl_globalclock - get_asset_start(asset);
			reset_asset_start(asset,gl_globalclock);
			metrics* pMetrics = (metrics*)get_parent();
			pMetrics->report_outage(duration/3600.0);
			value += duration/3600.0;
		}
		else if ( get_asset_status(asset) != 0 && get_asset_start(asset) == TS_ZERO )
		{
			debug("%s violation starts",(const char*)my()->name);
			set_asset_start(asset,gl_globalclock);
		}
	}
	return TS_NEVER;
}
