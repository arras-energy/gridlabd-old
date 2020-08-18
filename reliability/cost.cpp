// File: reliability/cost.cpp
// Author: dchassin@slac.stanford.edu
// Copyright: 2020 Regents of the Leland Stanford Junior University
//

#include "cost.h"

EXPORT_CREATE(cost);
EXPORT_INIT(cost);
EXPORT_COMMIT(cost);

static COST cost_data[] = 
{   // category     type       delay(h) fixed($) variable($/h)
    {"FOOD",        CT_FOOD,        8.0,   72.00,   0.00 },
    {"WATER",       CT_WATER,       1.0,    0.00,   6.25 },
    {"SEPTIC",      CT_SEPTIC,     24.0, 1492.00,   0.00 },
    {"GEN_BASIC",   CT_GEN_BASIC,   0.0, 4000.00,  39.00 },
    {"GEN_COMFORT", CT_GEN_COMFORT, 0.0, 4000.00,  83.00 },
    {"GEN_ALL",     CT_GEN_ALL,     0.0, 8000.00, 222.00 },
    {"LODGING",     CT_LODGING,    48.0,    0.00,  12.50 },
    {"BUSINESS",    CT_BUSINESS,   72.0,    0.00,  10.00 },
    {"ELDERCARE",   CT_ELDERCARE,  24.0,    0.00,  12.50 },
    {"HEATING",     CT_HEATING,     1.0,  100.00,   2.00 },
    {"COOKING",     CT_COOKING,    12.0,  100.00,   1.50 },
    {"LIGHTS",      CT_LIGHTS,      0.0,  100.00,   1.50 },
    {"WAGES",       CT_WAGES,      48.0,    0.00,  12.00 },
    { NULL } // sentinel
};

static set default_cost_type = CT_FOOD|CT_HEATING|CT_COOKING|CT_LIGHTS;

CLASS *cost::oclass = NULL;
cost *cost::defaults = NULL;

cost::cost(MODULE *module)
{
    if ( oclass == NULL )
    {
        oclass = gl_register_class(module,"cost",sizeof(cost),PC_AUTOLOCK);
        if ( oclass == NULL )
        {
            throw "unable to register class cost";
        }
        else
        {
            oclass->trl = TRL_PROTOTYPE;
        }
        if ( gl_publish_variable(oclass,
            PT_double,"total[$]",get_total_offset(),
                PT_OUTPUT,
                PT_DESCRIPTION,"Total cost of outages",
            PT_set,"type",get_type_offset(),
                PT_KEYWORD,"FOOD",CT_FOOD,
                PT_KEYWORD,"WATER",CT_WATER,
                PT_KEYWORD,"SEPTIC",CT_SEPTIC,
                PT_KEYWORD,"GEN_BASIC",CT_GEN_BASIC,
                PT_KEYWORD,"GEN_COMFORT",CT_GEN_COMFORT,
                PT_KEYWORD,"GEN_ALL",CT_GEN_ALL,
                PT_KEYWORD,"LODGING",CT_LODGING,
                PT_KEYWORD,"BUSINESS",CT_BUSINESS,
                PT_KEYWORD,"ELDERCARE",CT_ELDERCARE,
                PT_KEYWORD,"HEATING",CT_HEATING,
                PT_KEYWORD,"COOKING",CT_COOKING,
                PT_KEYWORD,"LIGHTS",CT_LIGHTS,
                PT_KEYWORD,"WAGES",CT_WAGES,
                PT_DESCRIPTION,"Type of costs incurred during outages",
            NULL) < 1 )
        {
            throw "unable to publish cost properties";
        }
        gl_global_create("default_cost_type",PT_set,&default_cost_type,
                PT_DESCRIPTION, "Default cost types to include in total cost",
                PT_KEYWORD,"FOOD",CT_FOOD,
                PT_KEYWORD,"WATER",CT_WATER,
                PT_KEYWORD,"SEPTIC",CT_SEPTIC,
                PT_KEYWORD,"GEN_BASIC",CT_GEN_BASIC,
                PT_KEYWORD,"GEN_COMFORT",CT_GEN_COMFORT,
                PT_KEYWORD,"GEN_ALL",CT_GEN_ALL,
                PT_KEYWORD,"LODGING",CT_LODGING,
                PT_KEYWORD,"BUSINESS",CT_BUSINESS,
                PT_KEYWORD,"ELDERCARE",CT_ELDERCARE,
                PT_KEYWORD,"HEATING",CT_HEATING,
                PT_KEYWORD,"COOKING",CT_COOKING,
                PT_KEYWORD,"LIGHTS",CT_LIGHTS,
                PT_KEYWORD,"WAGES",CT_WAGES,
                NULL
            );
        for ( COST *item = cost_data ; item->name != NULL ; item++ )
        {
            char *name;
            char *desc;

            asprintf(&name,"reliability::%s_delay_time[h]",item->name);
            asprintf(&desc,"Value for '%s' delay time (h)",item->name);
            gl_global_create(name, PT_double, &(item->delay),PT_DESCRIPTION,desc,NULL);

            asprintf(&name,"reliability::%s_fixed_cost[$]",item->name);
            asprintf(&desc,"Value for '%s' fixed cost ($)",item->name);
            gl_global_create(name, PT_double, &(item->fixed),PT_DESCRIPTION,desc,NULL);

            asprintf(&name,"reliability::%s_variable_cost[$/h]",item->name);
            asprintf(&desc,"Value for '%s' variable cost ($/h)",item->name);
            gl_global_create(name, PT_double, &(item->variable),PT_DESCRIPTION,desc,NULL);
        }
    }
}

int cost::create(void)
{
    customer_interrupted = NULL;
    interrupted_at = TS_NEVER;
    if ( type == 0 )
    {
        type = default_cost_type;
    }
    return 1;
}

int cost::init(OBJECT *parent)
{
    if ( ! gl_object_isa(parent,"triplex_meter","powerflow") )
    {
        throw "parent is not a triplex_meter object";
    }
    customer_interrupted = (bool*)gl_get_addr(parent,"customer_interrupted");
    if ( customer_interrupted == NULL )
    {
        throw "parent does not publish meter status (customer_interrupted)";
    }
    return 1;
}

TIMESTAMP cost::commit(TIMESTAMP t0, TIMESTAMP t1)
{
    if ( *customer_interrupted == true && interrupted_at == TS_NEVER )
    {
        interrupted_at = t0;
    }
    else if ( *customer_interrupted == false && interrupted_at != TS_NEVER )
    {
        double duration = ( t0 - interrupted_at ) / 3600.0;
        for ( COST *item = cost_data ; item->name != NULL ; item++ )
        {
            if ( duration > item->delay && (type&item->type) == item->type )
            {
                total += item->fixed + item->variable * ( duration - item->delay );
            }
        }        
        interrupted_at = TS_NEVER;
    }
    return TS_NEVER;
}