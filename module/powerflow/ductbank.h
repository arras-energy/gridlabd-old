// module/powerflow/ductbank.h
// Copyright (C) 2023 Regents of the Leland Stanford Junior University

#ifndef _G_DUCTBANK_H
#define _G_DUCTBANK_H

#include "gridlabd.h"

typedef struct s_ductbank_data
{
	char *name;
	double D; // duct inner diameter (cm)
	double H, V; // duct horizontal and vertical spacing (cm)
	double T, S, B; // duct top, side, and bottom spacing (cm)
	double G; // soil depth to duct
	double Rg, Rf, Rd; // ground, fill, ductbank, and air resistivity/cm

	unsigned int N, M; // number of channel rows and columns
	struct s_ductbank_data *next;
} DUCTBANK_DATA;

class ductbank : public gld_object 
{

public:

	static DUCTBANK_DATA *ductbank_data;
	static double default_ground_temperature;
	static double warning_cable_temperature;
	static double alert_cable_temperature;
	static char1024 ductbank_configuration_file;
	static double air_effective_conductivity;

public:

	typedef enum {CS_OK=0,CS_WARNING=1,CS_ALERT=2,CS_UNKNOWN=3} CABLESTATUS;

private:

	// inputs
	GL_STRING(char256,temperature_source);

	// settings
	GL_STRING(char1024,configuration);

private:

	double Ug, Ut, Us, Ub, Uh, Uv, Ua; // U-values
	double *A, *B, *x, *u, *Ainv; // data matrices
	gld_property *temperature; // 

public:

	ductbank(MODULE *module);
	int create(void);
	int init(OBJECT *parent);
	TIMESTAMP precommit(TIMESTAMP t1);
	TIMESTAMP commit(TIMESTAMP t1, TIMESTAMP t2);

private:

	void load_data(const char *name);

public:

	void add_cable(int ductid,double diameter,double R_value=0.1); // 1cm of rubber at 0.1 W/K.m
	void add_heatgain(double losses, double cable_length=0.0);

public:

	static CLASS *oclass;
	static class ductbank *defaults;

};

#endif // _DUCTBANK_H
