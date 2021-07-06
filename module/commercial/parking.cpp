/*  File: commercial/parking.cpp

    Copyright (C) 2020, Regents of the Leland Stanford Junior University
 */

#include "parking.h"

EXPORT_CREATE(parking);
EXPORT_INIT(parking);
EXPORT_PRECOMMIT(parking);
// EXPORT_COMMIT(parking);

#define SYNCEVENTS PC_AUTOLOCK// PC_PRETOPDOWN|PC_BOTTOMUP|PC_POSTTOPDOWN

CLASS *parking::oclass = NULL;
parking *parking::defaults = NULL;

double parking::default_nightlight_threshold = 0.0;

parking::parking(MODULE *module)
{
    if (oclass==NULL)
    {
        // register to receive notice for first top down. bottom up, and second top down synchronizations
        oclass = gld_class::create(module,"parking",sizeof(parking),SYNCEVENTS);
        if (oclass==NULL)
            throw "unable to register class parking";
        else
            oclass->trl = TRL_PROVEN;

        defaults = this;
        if (gl_publish_variable(oclass,
            PT_object,"weather",get_weather_offset(),PT_DESCRIPTION,"weather object for lighting data",
            PT_bool,"lighting_nightonly",get_lighting_nightonly_offset(),PT_DEFAULT,"TRUE",PT_DESCRIPTION,"lighting is used only at night (requires weather)",
            PT_complex,"lighting_capacity[VA]",get_lighting_capacity_offset(),PT_DEFAULT,"0 VA",PT_DESCRIPTION,"installed capacity for lighting",
            PT_int16,"charger_installed",get_charger_installed_offset(),PT_DEFAULT,"0",PT_DESCRIPTION,"number of chargers installed",
            PT_double,"charger_active",get_charger_active_offset(),PT_DEFAULT,"0",PT_DESCRIPTION,"number of chargers active",
            PT_complex,"charger_unit_power[VA]",get_charger_unit_power_offset(),PT_DEFAULT,"0 VA",PT_DESCRIPTION,"power demand for a single charger",
            PT_complex,"lighting_power[VA]",get_lighting_power_offset(),PT_DEFAULT,"0 VA",PT_DESCRIPTION,"power demand for lighting",
            PT_complex,"ventilation_power[VA]",get_ventilation_power_offset(),PT_DEFAULT,"0 VA",PT_DESCRIPTION,"power demand for ventilation",
            PT_complex,"charger_power[VA]",get_charger_power_offset(),PT_DEFAULT,"0 VA",PT_DESCRIPTION,"power demand for active chargers",
            PT_complex,"total_power[VA]",get_total_power_offset(),PT_DEFAULT,"0 VA",PT_DESCRIPTION,"power total demand for parking structure",
            NULL)<1){
                char msg[256];
                sprintf(msg, "unable to publish properties in %s",__FILE__);
                throw msg;
        }
        gl_global_create("commercial::nightlight_threshold[W/m^2]",PT_double,&default_nightlight_threshold,NULL);
    }
}

int parking::create(void) 
{
    return 1; /* return 1 on success, 0 on failure */
}

int parking::init(OBJECT *parent)
{
    if ( parent )
    {
        gld_object *obj = get_object(parent);
        if ( ! obj->isa("load") )
        {
            exception("'parent' is not a load object");
        }
        p_power_A = (double*)(gld_property(obj,"base_power_A").get_addr());
        p_power_B = (double*)(gld_property(obj,"base_power_B").get_addr());
        p_power_C = (double*)(gld_property(obj,"base_power_C").get_addr());
    }
    else
    {
        p_power_A = p_power_B = p_power_C = NULL;
    }

    if ( weather )
    {
        gld_object *obj = get_object(weather);
        if ( ! obj->isa("climate") )
            exception("'weather' is not a climate object");
        gld_property insolation(weather,"solar_global");
        if ( ! insolation.is_valid() )
            exception("%s does not publish 'solar_global' property", obj->get_name());
    }

    return 1;
}

TIMESTAMP parking::precommit(TIMESTAMP t1)
{
    // base power calculations
    charger_power = charger_unit_power * charger_active;
    lighting_power = ( ! lighting_nightonly || lighting_needed(t1) ) ? lighting_capacity : 0.0;
    total_power = lighting_power + ventilation_power + charger_power;
    int n_phases = (p_power_A?1:0) + (p_power_B?1:0) + (p_power_C?1:0);
    if ( p_power_A) *p_power_A = total_power.Re()/n_phases;
    if ( p_power_B) *p_power_B = total_power.Re()/n_phases;
    if ( p_power_C) *p_power_C = total_power.Re()/n_phases;
    return TS_NEVER;
}

bool parking::lighting_needed(TIMESTAMP t1)
{
    if ( weather )
    {
        // use daylight sensor
        gld_property insolation(weather,"solar_global");
        double solar_global = insolation.get_double();
        return solar_global < default_nightlight_threshold;
    }
    else
    {
        // use timeclock
        gld_clock timeclock(t1);
        int hour = timeclock.get_hour();
        return hour >= 18 || hour <= 6;
    }
}

TIMESTAMP parking::commit(TIMESTAMP t1, TIMESTAMP t2)
{  
    return TS_NEVER;
}