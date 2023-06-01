// module/resilience/cost.cpp
// Copyright (C) 2022 Regents of the Leland Stanford Junior University

#include <string.h>

#include "resilience.h"

EXPORT_CREATE(cost);
EXPORT_INIT(cost);
EXPORT_COMMIT(cost);

CLASS *cost::oclass = NULL;
cost *cost::defaults = NULL;

char1024 cost::metrics_cost_data = "metrics_cost_data.csv";

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
				snprintf(msg,sizeof(msg)-1, "unable to publish properties in %s",__FILE__);
				throw msg;
		}

        gl_global_create("resilience::metrics_cost_data",
            PT_char1024, (const char*)cost::metrics_cost_data,
            PT_ACCESS, PA_PUBLIC,
            PT_DESCRIPTION, "metrics cost data CSV file",
            NULL);
	}
}

int cost::create(void) 
{
	assetlist = NULL;
	value = 0.0;
	return 1; /* return 1 on success, 0 on failure */
}

typedef struct s_costdata
{
	char classname[64];
	set violations;
	double fixedcost;
	double variablecost;
	struct s_costdata *next;
} COSTDATA;
static COSTDATA *costdata = NULL;

static bool set_header(char *header)
{
	return strcmp(header,"classname,violations,firstcost,variablecost\n");
}

static COSTDATA *add_cost(const char *row)
{
	char classname[256] = "";
	int32 violations = 0;
	double fixedcost = 0.0;
	double variablecost = 0.0;
	if ( sscanf(row,"%[^,],%d,%lf,%lf",classname,&violations,&fixedcost,&variablecost) == 4 )
	{
		COSTDATA *item = new COSTDATA;
		if ( snprintf(item->classname,sizeof(item->classname)-1,"%.*s",(int)(sizeof(item->classname)-2),classname) < (int)strlen(classname) )
		{
			gl_warning("add_cost(row='%s'): class name '%s' is too long to store",row,classname);
		}
		item->violations = violations;
		item->fixedcost = fixedcost;
		item->variablecost = variablecost;
		item->next = costdata;
		costdata = item;
		return item;
	}
	else
	{
		return NULL;
	}
}
static COSTDATA *get_cost(const char *classname)
{
	for ( COSTDATA *item = costdata ; item != NULL ; item = item->next )
	{
		if ( strcmp(item->classname,classname) == 0 )
		{
			return item;
		}
	}
	return NULL;
}

int cost::init(OBJECT *parent)
{
	FILE *fp = fopen(metrics_cost_data,"r");
	if ( fp == NULL )
	{
		error("unable to open '%s'",(const char*)metrics_cost_data);
		return 0;
	}
	char header[1024];
	if ( fgets(header,sizeof(header),fp) == NULL )
	{
		error("unable to read header row from '%s' (%s)", (const char*)metrics_cost_data, strerror(errno));
		fclose(fp);
		return 0;
	}
	if ( ! set_header(header) )
	{
		error("header fields in '%s' are not valid", (const char*)metrics_cost_data);
		fclose(fp);
		return 0;
	}
	char row[1024];
	int n = 0;
	while ( !feof(fp) && !ferror(fp) && fgets(row,sizeof(row),fp) != NULL )
	{
		n++;
		if ( add_cost(row) == NULL )
		{
			error("unable to parse row %d",n);
			fclose(fp);
			return 0;
		}
	}
	if ( ferror(fp) )
	{
		error("unable to read '%s' (%s)",(const char*)metrics_cost_data,strerror(errno));
		fclose(fp);
		return 0;
	}
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

TIMESTAMP cost::commit(TIMESTAMP t1, TIMESTAMP t2)
{
	for ( ASSET *asset = get_first_asset() ; asset != NULL ; asset = get_next_asset(asset) )
	{
		if ( get_asset_start(asset) != TS_ZERO && get_asset_status(asset) == 0 )
		{
			debug("%s violation ends",(const char*)my()->name);
			TIMESTAMP duration = gl_globalclock - get_asset_start(asset);
			reset_asset_start(asset,gl_globalclock);
			metrics* pMetrics = (metrics*)get_parent();
			COSTDATA *costdata = get_cost(asset->obj->oclass->name);
			if ( costdata == NULL )
			{
				error("cost data for object '%s' <%s:%d> not found",asset->obj->name,asset->obj->oclass->name,asset->obj->id);
			}
			else
			{
				double cost = costdata->fixedcost + costdata->variablecost*duration/3600;
				pMetrics->report_cost(cost);
				value += cost;
			}
		}
		else if ( get_asset_status(asset) != 0 && get_asset_start(asset) == TS_ZERO )
		{
			debug("%s violation starts",(const char*)my()->name);
			set_asset_start(asset,gl_globalclock);
		}
	}
	return TS_NEVER;
}
