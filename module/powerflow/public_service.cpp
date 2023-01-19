// module/powerflow/public_service.cpp
// Copyright (C) 2023 Regents of the Leland Stanford Junior University

#include "powerflow.h"

EXPORT_CREATE(public_service);
EXPORT_PRECOMMIT(public_service);
EXPORT_INIT(public_service);
EXPORT_SYNC(public_service);
EXPORT_METHOD(public_service,schedule);

CLASS *public_service::oclass = NULL;
public_service *public_service::defaults = NULL;

public_service::public_service(MODULE *module)
: load(module)
{
    if (oclass==NULL)
    {
        // register to receive notice for first top down. bottom up, and second top down synchronizations
        oclass = gld_class::create(module,"public_service",sizeof(public_service),
            PC_PRETOPDOWN|PC_BOTTOMUP|PC_POSTTOPDOWN|PC_UNSAFE_OVERRIDE_OMIT|PC_AUTOLOCK|PC_OBSERVER);
        if (oclass==NULL)
            throw "unable to register class public_service";
        else
            oclass->trl = TRL_PROVEN;

        defaults = this;
        if (gl_publish_variable(oclass,

            PT_INHERIT, "load",

#define PUBLISH(TYPE,NAME,UNIT,DESCRIPTION) PT_##TYPE, #NAME "[" UNIT "]", get_##NAME##_offset(), PT_DESCRIPTION, DESCRIPTION

            PT_method, "schedule", method_public_service_schedule, 
                PT_REQUIRED, 
                PT_DESCRIPTION, "load schedule specification (<month>,<weekday>,<hour>-<hour>,<float>;...)",

            PT_double, "P0[W]", get_P0_offset(),
                PT_REQUIRED,
                PT_DESCRIPTION, "public_service load nameplate power",

            // load sensitivities
            PT_char256,"threshold_source",get_threshold_source_offset(),
                PT_DESCRIPTION,"source for threshold value",
            PT_double,"threshold_value",get_threshold_value_offset(),
                PT_DESCRIPTION,"threshold value for turning on load",
            PT_bool,"threshold_invert",get_threshold_invert_offset(),
                PT_DESCRIPTION,"invert effect of threshold, i.e., 'on' if less than",

            // load composition fractions
            PUBLISH(double,Pz,"pu","constant impedance load fraction"),
            PUBLISH(double,Pi,"pu","constant current load fraction"),
            PUBLISH(double,Ma,"pu","motor type A load fraction"),
            PUBLISH(double,Mb,"pu","motor type B load fraction"),
            PUBLISH(double,Mc,"pu","motor type C load fraction"),
            PUBLISH(double,Md,"pu","motor type D load fraction"),
            PUBLISH(double,Pe,"pu","power electronics load fraction"),
            PUBLISH(double,Pf,"pu","power factor"), PT_DEFAULT, "+1 pu",

            // meter
            PUBLISH(double,measured_real_power,"W","metered real power demand"), PT_OUTPUT,
            PUBLISH(double,measured_reactive_power,"VAr","metered reactive power demand"), PT_OUTPUT,
            PUBLISH(double,measured_real_energy,"Wh","cumulative metered real energy consumption"), PT_OUTPUT,
            PUBLISH(double,measured_real_energy_delta,"Wh","metered real energy interval consumption"), PT_OUTPUT,
            PUBLISH(double,measured_reactive_energy,"Wh","cumulative metered reactive energy consumption"), PT_OUTPUT,
            PUBLISH(double,measured_reactive_energy_delta,"Wh","metered reactive energy interval consumption"), PT_OUTPUT,
            PUBLISH(double,measured_energy_delta_timestep,"s","energy metering interval"), PT_OUTPUT,
            PUBLISH(double,measured_demand,"W","maximum metered real power interval demand"), PT_OUTPUT,
            PUBLISH(double,measured_demand_timestep,"s","maximum power metering interval"), PT_OUTPUT,

            NULL)<1) {
                throw "unable to publish public_service properties";
        }
        
        //Publish deltamode functions
        if (gl_publish_function(oclass, "delta_linkage_node", (FUNCTIONADDR)delta_linkage)==NULL)
            GL_THROW("Unable to publish load delta_linkage function");
        if (gl_publish_function(oclass, "interupdate_pwr_object", (FUNCTIONADDR)interupdate_load)==NULL)
            GL_THROW("Unable to publish load deltamode function");
        if (gl_publish_function(oclass, "delta_freq_pwr_object", (FUNCTIONADDR)delta_frequency_node)==NULL)
            GL_THROW("Unable to publish load deltamode function");
        if (gl_publish_function(oclass, "pwr_object_swing_swapper", (FUNCTIONADDR)swap_node_swing_status)==NULL)
            GL_THROW("Unable to publish load swing-swapping function");
        if (gl_publish_function(oclass, "pwr_current_injection_update_map", (FUNCTIONADDR)node_map_current_update_function)==NULL)
            GL_THROW("Unable to publish load current injection update mapping function");
        if (gl_publish_function(oclass, "attach_vfd_to_pwr_object", (FUNCTIONADDR)attach_vfd_to_node)==NULL)
            GL_THROW("Unable to publish load VFD attachment function");
        if (gl_publish_function(oclass, "pwr_object_reset_disabled_status", (FUNCTIONADDR)node_reset_disabled_status) == NULL)
            GL_THROW("Unable to publish load island-status-reset function");
        if (gl_publish_function(oclass, "pwr_object_load_update", (FUNCTIONADDR)update_load_values) == NULL)
            GL_THROW("Unable to publish load impedance-conversion/update function");
        if (gl_publish_function(oclass, "pwr_object_swing_status_check", (FUNCTIONADDR)node_swing_status) == NULL)
            GL_THROW("Unable to publish load swing-status check function");
    }
}

int public_service::isa(char *type)
{
    return strcmp(type,"public_service") || load::isa(type);
}

int public_service::create(void) 
{
    measured_real_energy = measured_reactive_energy = measured_demand = 0.0;
    measured_energy_delta_timestep = measured_demand_timestep = 3600;
    prev_measured_energy = last_measured_energy = this_measured_demand = 0.0;
    Pf = 1.0;
    return load::create(); /* return 1 on success, 0 on failure */
}

int public_service::init(OBJECT *parent)
{
    ts_offset = gld_clock(gl_globalclock).get_tzoffset() + TS_DAY0;

    // check nameplate load
    if ( P0 <= 0 )
    {
        exception("nameplate power P0 must be positive");
    }

    // check load composition
    double sum = Pz+Pi+Ma+Mb+Mc+Md+Pe;
    if ( sum == 0 )
    {
        exception("load components sum to zero");
    }
    if ( fabs(sum-1.0) > 1e-3 )
    {
        warning("load components do not sum to 1.0 (sum=%.4lg), normalizing automatically ",sum);
    }
    Pz /= sum;
    Pi /= sum;
    Ma /= sum;
    Mb /= sum;
    Mc /= sum;
    Md /= sum;
    Pe /= sum;

    // check threshold source
    if ( strcmp(threshold_source,"") != 0 )
    {
        threshold = new gld_property(threshold_source);
        if ( threshold == NULL || ! threshold->is_valid() )
        {
            error("threshold source '%s' is not valid",(const char*)threshold_source);
        }
    }

    // check schedule
    sum = 0;
    for ( unsigned int m = 0 ; m < 12 ; m++ )
    {
        for ( unsigned int d = 0 ; d < 7; d++ )
        {
            for ( unsigned int h = 0 ; h < 24 ; h++ )
            {
                COMPOSITION x = load_schedule[m][d][h];
                if ( x.total < 0 || x.total > 1 )
                {
                    warning("invalid load schedule %s total %.4g for month %d, weekday %d, hour %d",x.aggregate?"aggregated":"disaggregated",x.total,m,d,h);
                }
                else if ( x.pf < -1 || x.pf > 1 )
                {
                    warning("invalid load schedule power factor %.4g for month %d, weekday %d, hour %d",x.pf,m,d,h);
                }
                else
                {
                    sum += x.total;
                }
            }
        }
    }
    if ( sum == 0 )
    {
        warning("missing or null load schedule");
    }

    return load::init(parent);
}

TIMESTAMP public_service::precommit(TIMESTAMP t0)
{
    if ( gl_globalclock != last_timestamp )
    {
        gl_localtime(gl_globalclock,&last_datetime);
        last_timestamp = gl_globalclock;
        hour = last_datetime.hour;
        month = last_datetime.month-1;
        weekday = last_datetime.weekday;
    }
    return TS_NEVER;
}

TIMESTAMP public_service::presync(TIMESTAMP t0)
{
    TIMESTAMP t1 = (TIMESTAMP)(((TIMESTAMP)(t0/3600)+1)*3600);
    TIMESTAMP t2 = load::presync(t0);
    return t1 < t2 ? -t1 : t2;
}

static bool threshold_test(gld_property *threshold,double value,bool invert)
{
    bool result = invert;
    if ( threshold != NULL )
    {
        if ( threshold->get_double() > value )
        {
            result = !invert;
        }
    }
    return result;
}

TIMESTAMP public_service::sync(TIMESTAMP t0)
{
    if ( t0 % 3600 == 0 ) // update only at top of the hour
    {
        COMPOSITION ls = load_schedule[month][weekday][hour];
        double P = 0, Z = 0, I = 0;
        if ( threshold_test(threshold,threshold_value,threshold_invert) )
        {
            // compute composite load
            if ( ls.aggregate )
            {
                Z = Pz * ls.total * P0;
                I = Pi * ls.total * P0;
                P = (Ma+Mb+Mc+Md+Pe) * ls.total * P0;
            }
            else
            { 
                Z = Pz * ls.z * P0;
                I = Pi * ls.i * P0;
                P = (Ma*ls.a+Mb*ls.b+Mc*ls.c+Md*ls.d+Pe*ls.e) * P0;
            }

        } 

        // update load components
        double S = P + Z + I;
        base_power[0] = base_power[1] = base_power[2] = S/3;
        impedance_pf[0] = impedance_pf[1] = impedance_pf[2] = ls.pf;
        impedance_fraction[0] = impedance_fraction[1] = impedance_fraction[2] = (S!=0 ? Z/S : 0);
        current_pf[0] = current_pf[1] = current_pf[2] = ls.pf;
        current_fraction[0] = current_fraction[1] = current_fraction[2] = (S!=0 ? I/S : 0);
        power_pf[0] = power_pf[1] = power_pf[2] = ls.pf;
        power_fraction[0] = power_fraction[1] = power_fraction[2] = (S!=0 ? P/S : 0);
    }
    return load::sync(t0);
}

TIMESTAMP public_service::postsync(TIMESTAMP t0)
{
    double ts = (double)(t0 - last_meter_update) / 3600;
    if ( ts > 0 && measured_energy_delta_timestep > 0 && (t0-ts_offset) % (TIMESTAMP)measured_energy_delta_timestep == 0 )
    {
        // compute measured power
        measured_real_power = measured_reactive_power = 0.0;
        COMPOSITION ls = load_schedule[month][weekday][hour];
        double q = (ls.pf<0?-1:1)*tan(acos(ls.pf));
        if ( phases&PHASE_A ) 
        {
            measured_real_power += base_power[0];
            measured_reactive_power += base_power[0] * q;
        }
        if ( phases&PHASE_B )
        {
            measured_real_power += base_power[1];
            measured_reactive_power += base_power[1] * q;
        }
        if ( phases&PHASE_C )
        {
            measured_real_power += base_power[2];
            measured_reactive_power += base_power[2] * q;
        }

        // compute measured energy
        if ( last_meter_update > 0 )
        {
            // update measured energy value for this iteration
            complex measured_energy(measured_real_power*ts,measured_reactive_power*ts);
            measured_real_energy += measured_energy.r - prev_measured_energy.r;
            measured_reactive_energy += measured_energy.i - prev_measured_energy.i;
            prev_measured_energy = measured_energy;
            
            // update measured energy delta
            measured_real_energy_delta = measured_real_energy - last_measured_energy.r;
            measured_reactive_energy_delta = measured_reactive_energy - last_measured_energy.i;
        }

        // update measured demand
        if ( measured_real_power > this_measured_demand )
        {
            this_measured_demand = measured_real_power;
        }
    }
    return load::postsync(t0);
}

TIMESTAMP public_service::commit(TIMESTAMP t0)
{
    // update/reset meter demand values
    if ( measured_demand_timestep > 0 && (t0-ts_offset) % (TIMESTAMP)measured_demand_timestep == 0 )
    {
        measured_demand = this_measured_demand;
    }

    // update/reset meter energy values
    last_measured_energy = complex(measured_real_energy,measured_reactive_energy);
    last_meter_update = t0;
    prev_measured_energy = complex(0);

    return TS_NEVER;
}

int public_service::schedule(char *buffer, size_t len)
{
    if ( buffer == NULL ) 
    {
        // get length of data
        return snprintf(NULL,0,"%s",schedule_text);
    }
    else if ( len == 0 ) 
    {
        if ( schedule_text == NULL )
        {
            schedule_text = strdup(buffer);
        }
        else
        {
            schedule_text = (char*)realloc(schedule_text,strlen(schedule_text)+strlen(buffer)+3);
            if ( schedule_text[strlen(schedule_text)-1] != ';' )
            {   // make sure record separator is present
                strcat(schedule_text,";");
            }
            strcat(schedule_text,buffer);
        }
        // read data from buffer
        int result = strlen(buffer);
        char *data, *last = NULL;
        while ( (data=strtok_r(last?NULL:buffer,";\n",&last)) != NULL )
        {
            int start_month,stop_month,start_day,stop_day,start_hour,stop_hour;
            double a,b=Pf,c,d,e,z,i,pf=Pf;
            int count = sscanf(data,"%u-%u,%u-%u,%u-%u,%lg,%lg,%lg,%lg,%lg,%lg,%lg,%lg", 
                &start_month, &stop_month, &start_day, &stop_day, &start_hour, &stop_hour, 
                &a,&b,&c,&d,&e,&z,&i,&pf);
            // 7 -> a only: a=b=c=d=e=i=z, pf=1.0
            // 8 -> a,b only: a=b=c=d=e=i=z, pf=b
            // 13 -> a-z only: pf=1.0
            // 14 -> a-z,pf
            if ( count != 7 && count != 8 && count != 13 && count != 14 )
            {
                error("composition '%s': missing required value(s_",data);
                return 0;                
            }
            if ( start_month < 1 || stop_month > 12 || start_month > stop_month )
            {
                error("composition '%s': month %d-%d is invalid",data,start_month,stop_month);
                return 0;
            }
            if ( start_day < 0 || stop_day > 6 || start_day > stop_day )
            {
                error("composition '%s': weekday %d-%d is invalid",data,start_day,stop_day);
                return 0;
            }
            if ( start_hour < 0 || stop_hour > 23 || start_hour > stop_hour )
            {
                error("composition '%s': hours %d-%d is invalid",data,start_hour,stop_hour);
                return 0;
            }
            for ( int m = start_month; m <= stop_month ; m++ )
            {
                for ( int d = start_day ; d <= stop_day ; d++ )
                {                        
                    for ( int h = start_hour ; h <= stop_hour ; h++ )
                    {
                        switch (count) {
                        case 7:
                        case 8:
                            load_schedule[m-1][d][h].aggregate = true;
                            load_schedule[m-1][d][h].total = a;
                            load_schedule[m-1][d][h].pf = b;
                            load_schedule[m-1][d][h].a = 0;
                            load_schedule[m-1][d][h].b = 0;
                            load_schedule[m-1][d][h].c = 0;
                            load_schedule[m-1][d][h].d = 0;
                            load_schedule[m-1][d][h].e = 0;
                            load_schedule[m-1][d][h].z = 0;
                            load_schedule[m-1][d][h].i = 0;
                            break;
                        case 13:
                        case 14:
                            load_schedule[m-1][d][h].aggregate = false;
                            load_schedule[m-1][d][h].a = a;
                            load_schedule[m-1][d][h].b = b;
                            load_schedule[m-1][d][h].c = c;
                            load_schedule[m-1][d][h].d = d;
                            load_schedule[m-1][d][h].e = e;
                            load_schedule[m-1][d][h].z = z;
                            load_schedule[m-1][d][h].i = i;
                            load_schedule[m-1][d][h].total = a+b+c+d+e+i+z;
                            load_schedule[m-1][d][h].pf = pf;
                            break;
                        default:
                            exception("public_service::schedule(buffer='%s',len=%d): internal error",buffer,len);
                            break;
                        }
                    }
                }
            }
            if ( strchr(buffer,';') == NULL && strchr(buffer,'\n') == NULL ) // special case for unterminated string
            {
                break;
            }
        }
        return result; // failed

    }
    else 
    {
        int result = snprintf(buffer,len-1,"%s",schedule_text);
        return result;
    }
}