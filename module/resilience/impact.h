// module/resilience/impact.h
// Copyright (C) 2022 Regents of the Leland Stanford Junior University

#ifndef _GRIDLABD_MODULE_RESILIENCE_IMPACT_H
#define _GRIDLABD_MODULE_RESILIENCE_IMPACT_H

#include "gridlabd.h"

class impact : public gld_object 
{
	GL_ATOMIC(char256,asset_class); 
	GL_ATOMIC(double,value);

public:
	/* required implementations */
	impact(MODULE *module);
	int create(void);
	int init(OBJECT *parent);
	TIMESTAMP commit(TIMESTAMP t1, TIMESTAMP t2);

public:
	static CLASS *oclass;
	static impact *defaults;
};

#endif // _GRIDLABD_MODULE_RESILIENCE_IMPACT_H
