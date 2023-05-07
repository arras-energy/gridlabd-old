// module/powerflow/industrial.h
// Copyright (C) 2023 Regents of the Leland Stanford Junior University

#ifndef _INDUSTRIAL_H
#define _INDUSTRIAL_H

DECL_METHOD(industrial,schedule);

#define _NAICS_MAX 256

class industrial : public load
{
private:

    static char1024 NAICS_datafile;
    typedef struct s_composition
    {
        bool aggregate; 
        double total,z,i,a,b,c,d,e,pf;
    } COMPOSITION;
    typedef struct s_naics
    {
        unsigned int index;
        char code[64];
        char name[256];
        double ia,ib,ic,md,pe,z,i;
    } NAICS;
private:

    // schedule handling
    COMPOSITION load_schedule[12][7][24];
    char *schedule_text; // backup copy of schedule data
    TIMESTAMP last_timestamp;
    DATETIME last_datetime;
    unsigned int hour, weekday, month;
    static unsigned int n_naics;
    static NAICS naics[_NAICS_MAX]; // load composition buffer
    
    // meter handling
    TIMESTAMP last_meter_update; // time of last meter update
    complex last_measured_energy; // energy at last meter commit
    complex prev_measured_energy; // energy at previous meter postsync iteration
    double this_measured_demand; // measured demand for this interval
    int ts_offset; // offset to use for interval checks

    // load composition
    void get_load_composition(void);

public:

    // load model
    GL_ATOMIC(char32,industry_code); // load composition data (see naics_data_file.csv)
    GL_METHOD(industrial,schedule); // operating schedule name
    GL_ATOMIC(double,P0); // nameplate power

    // composition fractions
    GL_ATOMIC(double,Pz); // constant impedance load fraction
    GL_ATOMIC(double,Pi); // constant current load fraction
    GL_ATOMIC(double,Ma); // motor type A load fraction
    GL_ATOMIC(double,Mb); // motor type B load fraction
    GL_ATOMIC(double,Mc); // motor type C load fraction
    GL_ATOMIC(double,Md); // motor type D load fraction
    GL_ATOMIC(double,Pe); // power electronics load fraction

    // meter
    GL_ATOMIC(double,measured_real_power);
    GL_ATOMIC(double,measured_reactive_power);
    GL_ATOMIC(double,measured_real_energy);
    GL_ATOMIC(double,measured_real_energy_delta);
    GL_ATOMIC(double,measured_reactive_energy);
    GL_ATOMIC(double,measured_reactive_energy_delta);
    GL_ATOMIC(double,measured_energy_delta_timestep);
    GL_ATOMIC(double,measured_demand);
    GL_ATOMIC(double,measured_demand_timestep);

public:

    industrial(MODULE *module);
    int create(void);
    int init(OBJECT *parent);
    TIMESTAMP precommit(TIMESTAMP t0);
    TIMESTAMP presync(TIMESTAMP t0);
    TIMESTAMP sync(TIMESTAMP t0);
    TIMESTAMP postsync(TIMESTAMP t0);
    TIMESTAMP commit(TIMESTAMP t0);
    int isa(char *classname);

public:

    // required data
    static CLASS *oclass;
    static industrial *defaults;

};

#endif
