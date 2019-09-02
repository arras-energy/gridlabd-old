// powerflow/pole.h
// Copyright (C) 2018, Stanford University

#ifndef _POLE_H
#define _POLE_H

#ifndef _POWERFLOW_H
#error "this header must be included by powerflow.h"
#endif

class pole : public node
{
public:
	typedef struct s_wiredata {
		double height;
		double diameter;
		double heading;
		double tension;
		double span;
		struct s_wiredata *next;
	} WIREDATA;
	inline void add_wire(double height, double diameter, double heading, double tension, double span) {
		WIREDATA *item = new WIREDATA;
		item->height = height;
		item->diameter = diameter;
		item->heading = heading;
		item->tension = tension;
		item->span = span;
		item->next = wire_data;
		wire_data = item;
	};
	inline WIREDATA *get_next_wire(WIREDATA *wire) { return wire->next;};
	inline WIREDATA *get_first_wire(void) { return wire_data;};
public:
	static CLASS *oclass;
	static CLASS *pclass;
public:
	enum {PT_WOOD=0, PT_STEEL=1, PT_CONCRETE=2};
	enum {PS_OK=0, PS_FAILED=1,};
	enumeration pole_status;
	double tilt_angle;
	double tilt_direction;
	object weather;
	object configuration;
	double equipment_area;		// (see Section E)
	double equipment_height;	// (see Section E)
	double degradation_rate;
	int install_year; // year pole was installed
private:
	double ice_thickness;
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
private:
	class pole_configuration *config;
	double last_wind_speed;
	double *wind_speed;
	double *wind_direction;
	double *wind_gust;
	WIREDATA *wire_data;
	TIMESTAMP down_time;
public:
	pole(MODULE *);
	int isa(char *);
	int create(void);
	int init(OBJECT *);
	TIMESTAMP presync(TIMESTAMP);
	TIMESTAMP sync(TIMESTAMP);
	TIMESTAMP postsync(TIMESTAMP);
};

#endif // _POLE_H

