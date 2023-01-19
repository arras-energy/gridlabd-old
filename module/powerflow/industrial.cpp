// module/powerflow/industrial.cpp
// Copyright (C) 2023 Regents of the Leland Stanford Junior University

#include "powerflow.h"

EXPORT_CREATE(industrial);
EXPORT_PRECOMMIT(industrial);
EXPORT_INIT(industrial);
EXPORT_SYNC(industrial);
EXPORT_METHOD(industrial,schedule);

CLASS *industrial::oclass = NULL;
industrial *industrial::defaults = NULL;

char1024 industrial::NAICS_datafile = "naics_data_file.csv";
unsigned int industrial::n_naics = 0;
industrial::NAICS industrial::naics[_NAICS_MAX];

industrial::industrial(MODULE *module)
: load(module)
{
    if (oclass==NULL)
    {
        // register to receive notice for first top down. bottom up, and second top down synchronizations
        oclass = gld_class::create(module,"industrial",sizeof(industrial),
            PC_PRETOPDOWN|PC_BOTTOMUP|PC_POSTTOPDOWN|PC_UNSAFE_OVERRIDE_OMIT|PC_AUTOLOCK|PC_OBSERVER);
        if (oclass==NULL)
            throw "unable to register class industrial";
        else
            oclass->trl = TRL_PROVEN;

        defaults = this;
        if (gl_publish_variable(oclass,

            PT_INHERIT, "load",

#define PUBLISH(TYPE,NAME,UNIT,DESCRIPTION) PT_##TYPE, #NAME "[" UNIT "]", get_##NAME##_offset(), PT_DESCRIPTION, DESCRIPTION

            PT_char32, "industry_code", get_industry_code_offset(),
                PT_REQUIRED,
                PT_DESCRIPTION, "industrial facility code (see naics_data_file.csv)",

            PT_method, "schedule", method_industrial_schedule, 
                PT_REQUIRED, 
                PT_DESCRIPTION, "load schedule specification (<month>,<weekday>,<hour>-<hour>,<float>;...)",

            PT_double, "P0[W]", get_P0_offset(),
                PT_REQUIRED,
                PT_DESCRIPTION, "industrial load nameplate power",

            // load composition fractions
            PUBLISH(double,Pz,"pu","constant impedance load fraction"),
            PUBLISH(double,Pi,"pu","constant current load fraction"),
            PUBLISH(double,Ma,"pu","motor type A load fraction"),
            PUBLISH(double,Mb,"pu","motor type B load fraction"),
            PUBLISH(double,Mc,"pu","motor type C load fraction"),
            PUBLISH(double,Md,"pu","motor type D load fraction"),
            PUBLISH(double,Pe,"pu","power electronics load fraction"),

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
                throw "unable to publish industrial properties";
        }
        gl_global_create("powerflow::NAICS_datafile",PT_char1024,(const char*)NAICS_datafile,NULL);
        
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

int industrial::isa(char *type)
{
    return strcmp(type,"industrial") || load::isa(type);
}

int industrial::create(void) 
{
    measured_real_energy = measured_reactive_energy = measured_demand = 0.0;
    measured_energy_delta_timestep = measured_demand_timestep = 3600;
    prev_measured_energy = last_measured_energy = this_measured_demand = 0.0;
    return load::create(); /* return 1 on success, 0 on failure */
}

int industrial::init(OBJECT *parent)
{
    ts_offset = gld_clock(gl_globalclock).get_tzoffset() + TS_DAY0;

    if ( P0 <= 0 )
    {
        exception("nameplate power P0 must be positive");
    }
    if ( strcmp(industry_code,"") == 0 )
    {
        exception("NAICS industry code must be specified");
    }
    get_load_composition();

    return load::init(parent);
}

void industrial::get_load_composition(void)
{
    if ( n_naics == 0 )
    {
        char pathname[1024];
        strcpy(pathname,NAICS_datafile);
        gl_findfile(NAICS_datafile,NULL,R_OK,pathname,sizeof(pathname)-1);

        struct stat info;
        if ( stat(pathname,&info) != 0 )
        {
            GL_THROW("unable to stat file '%s'",pathname);
        }
        FILE *fp = fopen(pathname,"r");
        if ( fp == NULL )
        {
            GL_THROW("file '%s' open failed",pathname);
        }

        char header[1024];
        int line;
        for ( line = -1; ! feof(fp) && ! ferror(fp) && line < _NAICS_MAX ; line++ )
        {

            if ( line == -1 )
            {
                if ( fgets(header,sizeof(header)-1,fp) == NULL )
                {
                    GL_THROW("%s: missing header line",pathname);
                }
#define HEADER "NAICS,Code,Industrial Load Type,IA,IB,IC,MD,PwrEl,Z,I" // valid inverter settings header
                if ( strcmp(header,HEADER "\n") != 0 )
                {
                    GL_THROW("%s: invalid header line (expected '" HEADER "')",pathname);
                }
#undef HEADER
            }
            else
            {
                NAICS &x = naics[line];
                int len = fscanf(fp,"%u,%63[^,],%255[^,],%lg,%lg,%lg,%lg,%lg,%lg,%lg",
                    &x.index, x.code, x.name, &x.ia, &x.ib, &x.ic, &x.md, &x.pe, &x.z, &x.i);
                if ( ! feof(fp) && len < 10 )
                {
                    GL_THROW("%s(%d): missing required data field(s)",pathname,line);
                }
                if ( len > 0 )
                {
                    if ( x.ia < 0 || x. ia > 1 )
                    {
                        GL_THROW("%s(%d): missing required data field(s)",pathname,line);
                    }
                    double sum = x.ia + x.ib + x.ic + x.md + x.pe + x.z + x.i;
                    // fprintf(stderr,"%s(%d): %u,'%s','%s',%lg,%lg,%lg,%lg,%lg,%lg,%lg\n",pathname,line,x.index,x.code,x.name,x.ia,x.ib,x.ic,x.md,x.pe,x.z,x.i);
                    if ( fabs(sum-1.0) > 1e-3 )
                    {
                        warning("%s(%d): index=%u, code='%s', name='%s' components do not sum to 1.0 (sum is %.4lg)\n",pathname,line,x.index,x.code,x.name,sum);
                    }
                    // auto normalize
                    x.ia /= sum;
                    x.ib /= sum;
                    x.ic /= sum;
                    x.md /= sum;
                    x.pe /= sum;
                    x.z /= sum;
                    x.i /= sum;
                    n_naics++;
                }
            }
        }
        if ( line >= _NAICS_MAX )
        {
            warning("%s(%d): exceeded maximum of %u lines allowed",pathname,line,_NAICS_MAX);
        }
        fclose(fp);
    }

    // find the naics data
    unsigned int index = atoi(industry_code); // 0 if industry_code refers to code instead of index
    bool found = false;
    unsigned int n;
    for ( n = 0 ; n < n_naics ; n++ )
    {
        if ( index == 0 )
        {
            if ( strcmp(industry_code,naics[n].code) == 0 )
            {
                found = true;
                break;
            }
        }
        else if ( naics[n].index == index )
        {
            found = true;
            break;
        }
    }
    if ( ! found )
    {
        error("unable to locate industry code '%s' in NAICS data file '%s'",(const char*)industry_code,(const char*)NAICS_datafile);
    }
    else
    {
        NAICS &x = naics[n];
        // fprintf(stderr,"%u,'%s','%s',%lg,%lg,%lg,%lg,%lg,%lg,%lg\n",x.index,x.code,x.name,x.ia,x.ib,x.ic,x.md,x.pe,x.z,x.i);
        Pz = x.z;
        Pi = x.i;
        Ma = x.ia;
        Mb = x.ib;
        Mc = x.ic;
        Md = x.md;
        Pe = x.pe;
    }
}

TIMESTAMP industrial::precommit(TIMESTAMP t0)
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

TIMESTAMP industrial::presync(TIMESTAMP t0)
{
    TIMESTAMP t1 = (TIMESTAMP)(((TIMESTAMP)(t0/3600)+1)*3600);
    TIMESTAMP t2 = load::presync(t0);
    return t1 < t2 ? -t1 : t2;
}

TIMESTAMP industrial::sync(TIMESTAMP t0)
{
    if ( t0 % 3600 == 0 ) // update only at top of the hour
    {
        // compute composite load
        COMPOSITION ls = load_schedule[month][weekday][hour];
        double P,Z,I;
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

        // update load components
        double S = P+Z+I;
        base_power[0] = base_power[1] = base_power[2] = S/3;
        impedance_pf[0] = impedance_pf[1] = impedance_pf[2] = ls.pf;
        impedance_fraction[0] = impedance_fraction[1] = impedance_fraction[2] = (S!=0?Z/S:0);
        current_pf[0] = current_pf[1] = current_pf[2] = ls.pf;
        current_fraction[0] = current_fraction[1] = current_fraction[2] = (S!=0?I/S:0);
        power_pf[0] = power_pf[1] = power_pf[2] = ls.pf;
        power_fraction[0] = power_fraction[1] = power_fraction[2] = (S!=0?P/S:0);
    }
    return load::sync(t0);
}

TIMESTAMP industrial::postsync(TIMESTAMP t0)
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

TIMESTAMP industrial::commit(TIMESTAMP t0)
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

int industrial::schedule(char *buffer, size_t len)
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
            double a,b=1.0,c,d,e,z,i,pf=1.0;
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
                            load_schedule[m-1][d][h].pf = pf;
                            break;
                        default:
                            exception("industrial::schedule(buffer='%s',len=%d): internal error",buffer,len);
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