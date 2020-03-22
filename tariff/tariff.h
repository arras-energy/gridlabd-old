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
GLOBAL double_array climate_credit_months;
GLOBAL double_array summer_season_months;
GLOBAL double_array winter_season_months;

// TODO: include classes
// #include "{{{class1}}}.h"
// #include "{{{class2}}}.h"

class tariff : public gld_object 
{

private:

	// TODO: add private data

public:

	// TODO: add published data

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