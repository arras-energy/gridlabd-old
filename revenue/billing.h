// File: billing.h
// Copyright (C) 2020 Regents of the Leland Stanford Junior University

#ifndef _BILLING_H

#define _BILLING_H

#include "revenue.h"

class billing : public gld_object 
{

private:

	// TODO: add private data

public:

	// TODO: add published data
	GL_ATOMIC(object,tariff);
	GL_ATOMIC(object,meter);
	GL_ATOMIC(int32,bill_day);
	GL_ATOMIC(TIMESTAMP,bill_date);
	GL_ATOMIC(int32,billing_days);
	GL_ATOMIC(double,total_charges);
	GL_ATOMIC(double,energy_charges);
	GL_ATOMIC(double,capacity_charges);
	GL_ATOMIC(double,peaking_charges);
	GL_ATOMIC(double,program_credits);

private:

	// TODO: add private methods

public:
	
	// required methods
	billing(MODULE *module);
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
	static billing *defaults;
};

#endif // _BILLING_H