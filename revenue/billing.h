// File: billing.h
// Copyright (C) 2020 Regents of the Leland Stanford Junior University

#ifndef _BILLING_H

#define _BILLING_H

#include "revenue.h"

class billing : public gld_object 
{

private:

	// TODO: add private data
	PyObject *python_module;
	PyObject *python_data;

public:

	// TODO: add published data
	GL_ATOMIC(object,tariff);
	GL_ATOMIC(object,meter);
	GL_ATOMIC(int32,bill_day);
	GL_ATOMIC(TIMESTAMP,bill_date);
	GL_ATOMIC(int32,billing_days);
	GL_ATOMIC(double,total_bill);
	GL_ATOMIC(double,total_charges);
	GL_ATOMIC(double,energy_charges);
	GL_ATOMIC(double,capacity_charges);
	GL_ATOMIC(double,ramping_charges);
	GL_ATOMIC(double,fixed_charges);
	GL_ATOMIC(double,total_credits);
	GL_ATOMIC(double,energy_credits);
	GL_ATOMIC(double,capacity_credits);
	GL_ATOMIC(double,ramping_credits);
	GL_ATOMIC(double,fixed_credits);
	GL_ATOMIC(double,metering_interval);

private:

	// TODO: add private methods
	bool is_billing_time(gld_clock &dt);
	void compute_bill(void);

public:
	
	// required methods
	billing(MODULE *module);
	int create(void);
	int init(OBJECT *parent);

public:

	// TODO: optional methods
	TIMESTAMP commit(TIMESTAMP t1, TIMESTAMP t2);

public:

	static CLASS *oclass;
	static billing *defaults;
};

#endif // _BILLING_H