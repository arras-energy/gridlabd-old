// File: recorder.h
// Copyright (C) 2020 Regents of the Leland Stanford Junior University

#ifndef _RECORDER_H
#define _RECORDER_H

#include "influxdb.h"

#define MO_USEUNITS		0x0001	// add units to column names

class recorder : public gld_object 
{

public:

	GL_STRING(char1024,property);
	GL_STRING(char1024,table);
	GL_ATOMIC(double,interval);
	GL_ATOMIC(object,connection);
	GL_ATOMIC(set,options);
	GL_STRING(char1024,tags);

private:

	database::properties property_list;

public:

	recorder(MODULE *module);
	int create(void);
	int init(OBJECT *parent);
	TIMESTAMP commit(TIMESTAMP t0, TIMESTAMP t1);

public:

	static CLASS *oclass;
	static recorder *defaults;
};

#endif
