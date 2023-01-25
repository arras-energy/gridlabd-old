// module/powerflow/ductbank.h
// Copyright (C) 2023 Regents of the Leland Stanford Junior University

#ifndef _G_DUCTBANK_H
#define _G_DUCTBANK_H

#include "gridlabd.h"

typedef struct s_ductbank_data
{
	char *name;
	double A;
	double R[3];
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

public:

	typedef enum {CS_OK=0,CS_WARNING=1,CS_ALERT=2} CABLESTATUS;

private:

	// settings
	GL_STRING(char1024,configuration);
	GL_ATOMIC(double,ground_temperature);
	GL_ATOMIC(double,duct_R);
	GL_ATOMIC(double,airgap_R);
	GL_ATOMIC(double,insulation_R);
	GL_ATOMIC(double,section_area);

	// outputs
	GL_ATOMIC(double,heatgain);
	GL_ATOMIC(double,duct_temperature);
	GL_ATOMIC(double,air_temperature);
	GL_ATOMIC(double,cable_temperature);
	GL_ATOMIC(enumeration,cable_status);

	// internal variables
	double air_area;

public:

	ductbank(MODULE *module);
	int create(void);
	int init(OBJECT *parent);
	TIMESTAMP precommit(TIMESTAMP t1);
	TIMESTAMP commit(TIMESTAMP t1, TIMESTAMP t2);

private:

	void load_data(const char *name);

public:

	void add_cable(double diameter);
	void add_heatgain(double losses, double length=0.0);

public:

	static CLASS *oclass;
	static class ductbank *defaults;

};

#endif // _DUCTBANK_H
