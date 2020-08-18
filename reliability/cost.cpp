// File: reliability/cost.cpp
// Author: dchassin@slac.stanford.edu
// Copyright: 2020 Regents of the Leland Stanford Junior University
//

#include "cost.h"

EXPORT_CREATE(cost);
EXPORT_INIT(cost);
EXPORT_COMMIT(cost);

static COST cost_data[] = 
{   // category delay(h) fixed($) variable($/h)
    {"FOOD",         8.0,   72.00,   0.00 },
    {"WATER",        1.0,    0.00,   6.25 },
    {"SEPTIC",      24.0, 1492.00,   0.00 },
    {"GEN_BASIC",    0.0, 4000.00,  39.00 },
    {"GEN_COMFORT",  0.0, 4000.00,  83.00 },
    {"GEN_ALL",      0.0, 8000.00, 222.00 },
    {"LODGING",     48.0,    0.00,  12.50 },
    {"BUSINESS",    72.0,    0.00,  10.00 },
    {"ELDERCARE",   24.0,    0.00,  12.50 },
    {"AUXHEAT",      1.0,  100.00,   2.00 },
    {"COOKING",     12.0,  100.00,   1.50 },
    {"LIGHTING",     0.0,  100.00,   1.50 },
    {"LOSTWAGES",   48.0,    0.00,  12.00 },
    { NULL } // sentinel
};

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
                PT_DESCRIPTION,"total cost",
            NULL) < 1 )
        {
            throw "unable to publish cost properties";
        }
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
            if ( duration > item->delay )
            {
                total += item->fixed + item->variable * ( duration - item->delay );
            }
        }        
        interrupted_at = TS_NEVER;
    }
    return TS_NEVER;
}