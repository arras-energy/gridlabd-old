// powerflow/pole.h
// Copyright (C) 2018, Regents of the Leland Stanford Junior University

#ifndef _POLE_H
#define _POLE_H

#ifndef _POWERFLOW_H
#error "this header must be included by powerflow.h"
#endif

#include <list>

class pole : gld_object
{
public:
	static CLASS *oclass;
	static CLASS *pclass;
    static pole *defaults;
public:
	enum {PT_WOOD=0, PT_STEEL=1, PT_CONCRETE=2};
	enum {PS_OK=0, PS_FAILED=1};
	GL_ATOMIC(enumeration,pole_status);
	GL_ATOMIC(double,tilt_angle);
	GL_ATOMIC(double,tilt_direction);
	GL_ATOMIC(object,configuration);
	GL_ATOMIC(double,degradation_rate);
	GL_ATOMIC(int16,install_year); // year pole was installed
	GL_ATOMIC(double,repair_time);
    GL_ATOMIC(object,weather);
    GL_ATOMIC(double,wind_speed);
    GL_ATOMIC(double,wind_direction);
    GL_ATOMIC(double,wind_gusts);
private:
    gld_property *wind_speed_ref;
	gld_property *wind_direction_ref;
	gld_property *wind_gusts_ref;
private:
	double resisting_moment; 	// (see Section B)
	double pole_moment;		// (see Section D)
	double pole_moment_nowind; // wire moment without the wind component for wind speed at failure calc
	double equipment_moment;	// (see Section E)
	double equipment_moment_nowind; // wire moment without the wind component for wind speed at failure calc
	double wire_load;		// (see Section F)
	double wire_load_nowind; // wire moment without the wind component for wind speed at failure calc
	double wire_moment;		// (see Section F)
	double wire_moment_nowind; // wire moment without the wind component for wind speed at failure calc
	double wind_pressure;		// (see Section D)
	double wire_tension;	// (see Section G)
	double pole_stress;	// ratio of total to resisting moment
	double pole_stress_polynomial_a; //polynomial components of pole stress function
	double pole_stress_polynomial_b; //polynomial components of pole stress function
	double pole_stress_polynomial_c; //polynomial components of pole stress function
	double susceptibility;	// d(pole_stress)/d(wind_speed)
	double total_moment;
	double critical_wind_speed;
	bool is_deadend;
	double current_hollow_diameter;
    void reset_accumulators();
private:
	class pole_configuration *config;
	double last_wind_speed;
	TIMESTAMP down_time;
public:
	pole(MODULE *);
	int create(void);
	int init(OBJECT *);
    TIMESTAMP precommit(TIMESTAMP);
	TIMESTAMP presync(TIMESTAMP);
	TIMESTAMP sync(TIMESTAMP);
	TIMESTAMP postsync(TIMESTAMP);
    TIMESTAMP commit(TIMESTAMP,TIMESTAMP);
};

#endif // _POLE_H
