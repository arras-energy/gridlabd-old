// module/powerflow/agricultural.h
// Copyright (C) 2023 Regents of the Leland Stanford Junior University

#ifndef _AGRICULTURAL_H
#define _AGRICULTURAL_H

DECL_METHOD(agricultural,schedule);

class agricultural : public load
{
private:

    typedef struct s_composition
    {
        bool aggregate; 
        double total,z,i,a,b,c,d,e,pf;
    } COMPOSITION;

private:

    // schedule handling
    char *schedule_text; // backup copy of schedule data
    COMPOSITION load_schedule[12][7][24];
    TIMESTAMP last_timestamp;
    DATETIME last_datetime;
    unsigned int hour, weekday, month;
    
    // meter handling
    TIMESTAMP last_meter_update; // time of last meter update
    complex last_measured_energy; // energy at last meter commit
    complex prev_measured_energy; // energy at previous meter postsync iteration
    double this_measured_demand; // measured demand for this interval
    int ts_offset; // offset to use for interval checks

    // sensitivity handling
    gld_property *sensitivity;

public:

    // load model
    GL_METHOD(agricultural,schedule); // operating schedule name
    GL_ATOMIC(double,P0); // nameplate power

    // sensitivities
    GL_STRING(char256,sensitivity_source); // object name and property name
    GL_ATOMIC(double,sensitivity_base); // constant component of sensitivity function
    GL_ATOMIC(double,sensitivity_value); // slope component of sensitivity function

    // composition fractions
    GL_ATOMIC(double,Pz); // constant impedance load fraction
    GL_ATOMIC(double,Pi); // constant current load fraction
    GL_ATOMIC(double,Ma); // motor type A load fraction
    GL_ATOMIC(double,Mb); // motor type B load fraction
    GL_ATOMIC(double,Mc); // motor type C load fraction
    GL_ATOMIC(double,Md); // motor type D load fraction
    GL_ATOMIC(double,Pe); // power electronics load fraction
    GL_ATOMIC(double,Pf); // power factor

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

    agricultural(MODULE *module);
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
    static agricultural *defaults;

};

#endif // _AGRICULTURAL_H
