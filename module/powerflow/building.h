// module/loads/building.h
// Copyright (C) 2022 Regents of the Leland Stanford Junior University

#ifndef _BUILDING_H
#define _BUILDING_H

#ifndef _POWERFLOW_H
#error "this header must be included by powerflow.h"
#endif

DECL_METHOD(building,airtemperature_history);
DECL_METHOD(building,masstemperature_history);
DECL_METHOD(building,building_response);
DECL_METHOD(building,input);
DECL_METHOD(building,zip);

class building : public load 
{
	typedef struct s_input
	{
		char *source;
		gld_property *prop;
		double *addr;
		int state;
		double b[3]; // tf terms
		double u[3]; // input history
		double d[2]; // real and reactive power contributions
		struct s_input *next;
	} INPUT;

private:

	// private data
	INPUT *input_list;
	double x[2][3]; // internal state history (TA)

	// transfer function
	double ts; // timestep
	double a[2][3]; // transfer function denominator terms
	complex p0, p1; // transfer function poles
	double zip[2][3]; // output ZIP components

public:

	// public data
	GL_ATOMIC(double,timestep); // model timestep
	GL_ATOMIC(double,output_timestep); // model output timestep

	GL_METHOD(building,airtemperature_history); // previous air temperature values
	GL_METHOD(building,masstemperature_history); // previous mass temperature values
	GL_METHOD(building,building_response); // set tf denominator terms
	GL_METHOD(building,input); // add tf input
	GL_METHOD(building,zip); // zip components
	
private:

	// private methods
	void check_poles(double *a,const char *name);
	void update_state(unsigned int n=1);
	void update_input(void);
	void update_output();

public:

	// event handlers
	building(MODULE *module);
	int create(void);
	int init(OBJECT *parent);
	TIMESTAMP precommit(TIMESTAMP t1, TIMESTAMP t2);
	TIMESTAMP presync(TIMESTAMP t0);
	TIMESTAMP sync(TIMESTAMP t0);
	int isa(char *classname);

public:

	// required data
	static CLASS *oclass;
	static building *defaults;
};

#endif // _BUILDING_H
