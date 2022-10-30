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
DECL_METHOD(building,temperature_input);
DECL_METHOD(building,temperature_response);
DECL_METHOD(building,occupancy_input);
DECL_METHOD(building,occupancy_response);
DECL_METHOD(building,solar_input);
DECL_METHOD(building,solar_response);
DECL_METHOD(building,loads_input);
DECL_METHOD(building,loads_response);
DECL_METHOD(building,ventilation_input);
DECL_METHOD(building,ventilation_response);
DECL_METHOD(building,systemmode_input);
DECL_METHOD(building,systemmode_response);

class building : public load 
{
	typedef struct s_input
	{
		char *source;
		gld_property *prop;
		double *addr;
		double b[3];
		struct s_input *next;
	} INPUT;

private:

	// private data
	INPUT *input_list;
	gld_property *Tin;
	double *pT; // source of temperature input (if any)
	gld_property *Nin;
	double *pN; // source of occupancy input (if any)
	gld_property *Sin;
	double *pS; // source of solar input (if any)
	gld_property *Lin;
	double *pL; // source of loads input (if any)
	gld_property *Vin;
	double *pV; // source of ventilation input (if any)
	gld_property *Min;
	double *pM; // source of system control mode input (if any)
	double T[3]; // temperature history
	double N[3]; // occupancy history
	double S[3]; // solar history
	double L[3]; // loads history
	double V[3]; // ventilation history
	double M[3]; // system mode history
	double x0[3]; // internal state history (TA)
	double x1[3]; // internal state history (TM)
	double y[2]; // output (P,Q)

	// transfer function
	double ts; // timestep
	double bT[3], bN[3], bS[3], bL[3], bV[3], bM[3]; // transfer function numerator terms
	double a0[3], a1[3]; // transfer function denominator terms
	complex p0, p1; // transfer function poles

public:

	// public data
	GL_ATOMIC(double,loads_powerfactor); // loads power factor
	GL_ATOMIC(double,system_powerfactor); // system power factor
	GL_ATOMIC(double,timestep); // model timestep
	GL_ATOMIC(double,output_timestep); // model output timestep

	GL_METHOD(building,airtemperature_history); // previous air temperature values
	GL_METHOD(building,masstemperature_history); // previous mass temperature values
	GL_METHOD(building,building_response); // set tf denominator terms
	GL_METHOD(building,input); // add tf input
	
	GL_METHOD(building,temperature_input); // set temperature input
	GL_METHOD(building,temperature_response); // set temperature tf numerator terms

	GL_METHOD(building,occupancy_input); // set occupancy input
	GL_METHOD(building,occupancy_response); // set occupancy tf numerator terms

	GL_METHOD(building,solar_input); // set solar input
	GL_METHOD(building,solar_response); // set solar tf numerator terms

	GL_METHOD(building,loads_input); // set loads input
	GL_METHOD(building,loads_response); // set loads tf numerator terms

	GL_METHOD(building,ventilation_input); // set ventilation input
	GL_METHOD(building,ventilation_response); // set ventilation tf numerator terms

	GL_METHOD(building,systemmode_input); // set systemode input
	GL_METHOD(building,systemmode_response); // set systemode tf numerator terms

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
