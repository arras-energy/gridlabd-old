// module/powerflow/ductbank.h
// Copyright (C) 2023 Regents of the Leland Stanford Junior University

#ifndef _G_DUCTBANK_H
#define _G_DUCTBANK_H

#include "gridlabd.h"

class ductbank : public gld_object 
{

private:

	GL_ATOMIC(double,air_temperature);
	GL_ATOMIC(double,shell_temperature);
	GL_ATOMIC(double,ground_temperature);

public:

	/* required implementations */
	ductbank(MODULE *module);
	int create(void);
	int init(OBJECT *parent);
	TIMESTAMP commit(TIMESTAMP t1, TIMESTAMP t2);

public:

	static CLASS *oclass;
	static class ductbank *defaults;

};

#endif // _DUCTBANK_H
