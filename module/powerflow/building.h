// module/loads/building.h
// Copyright (C) 2022 Regents of the Leland Stanford Junior University

#ifndef _BUILDING_H
#define _BUILDING_H

#ifndef _POWERFLOW_H
#error "this header must be included by powerflow.h"
#endif


class building : public pqload {

private:

	// private data
	double UA; // air conductivity
	double UM; // mass conductivity
	double CA; // air heat capacity
	double CM; // mass heat capacity
	double Q[4]; // system mode heat flux
	double S[4]; // system mode efficiency (Q/P)
	double pf[4]; // system mode power factor (P/S)

	// state-space model
	double ts;
	double x[2]; // 
	double a[2][2];
	double b[5][2]; 
	double y[2];
	double c[2][2];
	double d[5][2];

public:

	// public data
	GL_STRING(char32,building_class);
	GL_STRING(char32,building_type);
	GL_STRING(char256,address);
	GL_ATOMIC(double,floor_area);
	GL_ATOMIC(int16,storeys);
	GL_ATOMIC(double,glazing);
	GL_ATOMIC(double,perimeter);
	GL_ATOMIC(double,height);
	GL_STRING(char32,system_type);
	GL_STRING(char32,construction_type);

private:

	// private methods

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
