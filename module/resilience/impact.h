// module/resilience/impact.h
// Copyright (C) 2022 Regents of the Leland Stanford Junior University

#ifndef _GRIDLABD_MODULE_RESILIENCE_IMPACT_H
#define _GRIDLABD_MODULE_RESILIENCE_IMPACT_H

#include "gridlabd.h"

class impact : public gld_object 
{
public:

	static double default_impact_duration;

private:

	GL_ATOMIC(object,node); 
	GL_ATOMIC(double,value);
	GL_ATOMIC(double,duration);

public:
	/* required implementations */
	impact(MODULE *module);
	int create(void);
	int init(OBJECT *parent);
	TIMESTAMP commit(TIMESTAMP t1, TIMESTAMP t2);

public:
	TIMESTAMP start;
	set *status;

public:
	static CLASS *oclass;
	static impact *defaults;
};

#endif // _GRIDLABD_MODULE_RESILIENCE_IMPACT_H
