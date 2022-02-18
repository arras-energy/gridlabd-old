// module/resilience/outage.h
// Copyright (C) 2022 Regents of the Leland Stanford Junior University

#ifndef _GRIDLABD_MODULE_RESILIENCE_OUTAGE_H
#define _GRIDLABD_MODULE_RESILIENCE_OUTAGE_H

#include "gridlabd.h"

class outage : public gld_object 
{
	GL_ATOMIC(char256,meter_class); 
	GL_ATOMIC(double,value);

public:
	/* required implementations */
	outage(MODULE *module);
	int create(void);
	int init(OBJECT *parent);
	TIMESTAMP commit(TIMESTAMP t1, TIMESTAMP t2);

public:
	static CLASS *oclass;
	static outage *defaults;
};

#endif // _GRIDLABD_MODULE_RESILIENCE_OUTAGE_H
