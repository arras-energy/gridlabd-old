// module/resilience/cost.h
// Copyright (C) 2022 Regents of the Leland Stanford Junior University

#ifndef _GRIDLABD_MODULE_RESILIENCE_COST_H
#define _GRIDLABD_MODULE_RESILIENCE_COST_H

#include "resilience.h"

class cost : public gld_object 
{
public:

	static char1024 metrics_cost_data;

private:

	GL_ATOMIC(char256,asset_class); 
	GL_ATOMIC(double,value);

private:

	ASSET *assetlist;
	inline ASSET *get_first_asset() { return assetlist;}
	inline ASSET *get_next_asset(ASSET *asset) { return asset->next; };
	inline ASSET *add_asset(OBJECT *obj)
	{
		set *status = (set*)gl_get_addr(obj,"violation_detected");
		if ( status )
		{
			ASSET *item = new ASSET;
			item->obj = obj;
			item->start = TS_ZERO;
			item->next = assetlist;
			item->status = status;
			assetlist = item;
			return item;
		}
		else
		{
			warning("asset '%s' <%s:%d> has no violated_detected property",obj->name,obj->oclass->name,obj->id);
			return NULL;
		}
	};
	inline set get_asset_status(ASSET *asset) { return *(asset->status); };
	inline TIMESTAMP get_asset_start(ASSET *asset) { return asset->start; };
	inline void set_asset_start(ASSET *asset, TIMESTAMP start) { asset->start = start; };
	inline void reset_asset_start(ASSET *asset, TIMESTAMP start) { asset->start = TS_ZERO; };

public:
	
	cost(MODULE *module);
	int create(void);
	int init(OBJECT *parent);
	TIMESTAMP commit(TIMESTAMP t1, TIMESTAMP t2);

public:
	
	static CLASS *oclass;
	static cost *defaults;
};

#endif // _GRIDLABD_MODULE_RESILIENCE_COST_H
