// File: tariff.h
// Copyright (C) 2020 Regents of the Leland Stanford Junior University

#ifndef _TARIFF_H

#define _TARIFF_H

#include "revenue.h"

typedef struct s_energy_rate
{
	double minimum;
	double maximum;
	UNIT *units; // e.g., kWh
	int history; // month
	// TODO
} ENERGYRATE;

typedef struct s_demand_rate
{
	double minimum;
	double maximum;
	UNIT* units; // e.g., kW
	int history; // months
	// TODO
} DEMANDRATE;

typedef struct s_fixed_rate
{
	// TODO
} FIXEDRATE;

class tariff : public gld_object 
{

private:

	// TODO: add private data
	ENERGYRATE energy_rate;
	DEMANDRATE demand_rate;
	FIXEDRATE fixed_rate;

public:

	GL_ATOMIC(int64,rate_id);
	GL_ATOMIC(int64,eia_id);
	GL_STRING(char256,utility_name);
	GL_STRING(char256,rate_name);
	GL_ATOMIC(bool,is_verified);
	GL_ATOMIC(bool,is_default);
	GL_ATOMIC(TIMESTAMP,effective_date);
	GL_ATOMIC(TIMESTAMP,end_date);
	GL_ATOMIC(enumeration,sector);
	GL_STRING(char256,service_type);
	GL_STRING(char1024,description);
	GL_STRING(char1024,source);
	GL_STRING(char1024,source_parent);
	GL_STRING(char256,dg_compensation);

	GL_ATOMIC(double,minimum_voltage);
	GL_ATOMIC(double,maximum_voltage);

	// TODO: add published data
	GL_STRING(char32,rate_design)
	GL_ATOMIC(double,minimum_daily_charge)
	GL_ATOMIC(double,energy_charge_base)
	GL_ATOMIC(double,energy_charge_100)
	GL_ATOMIC(double,energy_charge_400)
	GL_ATOMIC(double,average_total_rate)
	GL_ATOMIC(double,minimum_average_rate_limit)
	GL_ATOMIC(double,discount)

private:

	// TODO: add private methods

public:
	
	// required methods
	tariff(MODULE *module);
	int create(void);
	int init(OBJECT *parent);

public:

	// TODO: optional methods
	TIMESTAMP precommit(TIMESTAMP t1);
	TIMESTAMP presync(TIMESTAMP t1);
	TIMESTAMP sync(TIMESTAMP t1);
	TIMESTAMP postsync(TIMESTAMP t1);
	TIMESTAMP commit(TIMESTAMP t1, TIMESTAMP t2);
	TIMESTAMP finalize(TIMESTAMP t1, TIMESTAMP t2);

	int postnotify(PROPERTY *prop, const char *value);
	int prenotify(PROPERTY *prop, const char *value);

public:
	static CLASS *oclass;
	static tariff *defaults;
};

#endif // _TARIFF_H