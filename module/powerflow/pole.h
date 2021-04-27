// powerflow/pole.h
// Copyright (C) 2018, Stanford University

#ifndef _POLE_H
#define _POLE_H

#ifndef _POWERFLOW_H
#error "this header must be included by powerflow.h"
#endif

#include <list>

typedef struct s_wiredata {
	overhead_line *line; // overhead line object
	double height; // height of wire
	double diameter; // diameter of wire
	double heading; // direction of wire from pole
	double tension; // tension on wire
	double span; // distance wire spans
	OBJECT *protection; // protection object selected by fault
	int fault; // fault code
	char fault_type[8]; // fault name
	TIMESTAMP repair; // fault repair time
	char data[32]; // extra data space
} WIREDATA;
class pole : public node
{
public:
	inline void add_wire(overhead_line *line, double height, double diameter, double heading, double tension, double span) {
		WIREDATA *item = new WIREDATA;
		item->line = line;
		item->height = height;
		item->diameter = diameter;
		item->heading = heading;
		item->tension = tension;
		item->span = span;
		item->protection = NULL;
		item->fault = 0;
		memset(item->fault_type,0,sizeof(item->fault_type));
		strcpy(item->fault_type,"TLL");
		item->repair = TS_NEVER;
		memset(item->data,0,sizeof(item->data));
		wire_data->push_back(*item);
	};
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
	double repair_time;
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
	std::list<WIREDATA> *wire_data;
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

