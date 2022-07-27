// powerflow/pole.h
// Copyright (C) 2018, Regents of the Leland Stanford Junior University

#ifndef _POLE_H
#define _POLE_H

#ifndef _POWERFLOW_H
#error "this header must be included by powerflow.h"
#endif

#include <list>

class pole : public gld_object
{
public:
	static double pole_overdesign_factor;
public:
	static CLASS *oclass;
	static CLASS *pclass;
    static pole *defaults;
public:
	enum {PT_WOOD=0, PT_STEEL=1, PT_CONCRETE=2};
	GL_ATOMIC(enumeration,pole_type);
	enum {PS_OK=0, PS_FAILED=1};
	GL_ATOMIC(enumeration, pole_status);
	enum {FT_UNKNOWN=0, FT_FOUNDATION=1, FT_BASE=2, FT_MIDDLE=3, FT_TOP=4};
	GL_ATOMIC(enumeration, failure_type);
	GL_ATOMIC(double, tilt_angle);
	GL_ATOMIC(double, tilt_direction);
	GL_ATOMIC(object, configuration);
	GL_ATOMIC(double, degradation_rate);
	GL_ATOMIC(int16, install_year); // year pole was installed
	GL_ATOMIC(double, repair_time);
    GL_ATOMIC(object, weather);
    GL_ATOMIC(double, wind_speed);
    GL_ATOMIC(double, wind_direction);
    GL_ATOMIC(double, wind_gusts);
    GL_ATOMIC(double, pole_moment);		// (see Section D)
	GL_ATOMIC(double, pole_moment_nowind); // wire moment without the wind component for wind speed at failure calc
	GL_ATOMIC(double, pole_moment_wind); 
    GL_ATOMIC(double, resisting_moment); 	// (see Section B)
	GL_ATOMIC(double, equipment_moment);	// (see Section E)
	GL_ATOMIC(double, equipment_weight);	// (see Section E)
	GL_ATOMIC(double, equipment_moment_nowind); // wire moment without the wind component for wind speed at failure calc
    GL_ATOMIC(double, wire_wind);		// (see Section F)
	GL_ATOMIC(double, wire_load_nowind); // wire moment without the wind component for wind speed at failure calc
	GL_ATOMIC(double, wire_weight);		// (see Section F)
	GL_ATOMIC(double, wire_moment_x);		// wire moment in x-axis due to tension and wind load
	GL_ATOMIC(double, wire_moment_y);		// wire moment in y-axis due to tension and wind load
	GL_ATOMIC(double, equipment_moment_x);	// equipment moment in x-axis due to wind load
	GL_ATOMIC(double, equipment_moment_y);	// equipment moment in y-axis due to wind load
	GL_ATOMIC(double, wire_moment_nowind); // wire moment without the wind component for wind speed at failure calc
	GL_ATOMIC(double, wind_pressure);		// (see Section D)
	GL_ATOMIC(double, wire_tension);	// (see Section G)
	GL_ATOMIC(double, pole_stress);	// ratio of total to resisting moment
	GL_ATOMIC(double, pole_stress_polynomial_a); //polynomial components of pole stress function
	GL_ATOMIC(double, pole_stress_polynomial_b); //polynomial components of pole stress function
	GL_ATOMIC(double, pole_stress_polynomial_c); //polynomial components of pole stress function
	GL_ATOMIC(double, susceptibility);	// d(total_moment)/d(wind_speed)
	GL_ATOMIC(double, total_moment);
	GL_ATOMIC(double, critical_wind_speed);
	GL_ATOMIC(bool, is_deadend);
	GL_ATOMIC(double, current_hollow_diameter);
	GL_ATOMIC(double, guy_height);
private:
    gld_property *wind_speed_ref;
	gld_property *wind_direction_ref;
	gld_property *wind_gusts_ref;
private:
    void reset_commit_accumulators();
    void reset_sync_accumulators();
private:
	class pole_configuration *config;
	double last_wind_speed;
	TIMESTAMP down_time;
public:
	double height; // effective pole height for moment calculations
    bool recalc; // flag for recalculation
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
