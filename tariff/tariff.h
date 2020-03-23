// File: main.cpp
// Copyright (C) 2020 Regents of the Leland Stanford Junior University

#ifndef _TARIFF_H

#define _TARIFF_H

#include "gridlabd.h"

#ifdef DLMAIN
#define INIT(X) = X
#define GLOBAL
#else
#define INIT(X)
#define GLOBAL extern
#endif

// TODO: declare module globals here
GLOBAL double dcare_discount INIT(0);
GLOBAL double climate_credit INIT(0);
GLOBAL char1024 climate_credit_months;
GLOBAL char1024 summer_season_months;
GLOBAL char1024 winter_season_months;

// TODO: include classes
// #include "{{{class1}}}.h"
// #include "{{{class2}}}.h"

class tariff : public gld_object 
{

private:

	// TODO: add private data

public:

	// TODO: add published data
	GL_STRING(char32,rate_design)
	GL_ATOMIC(double,minimum_daily_amount)
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