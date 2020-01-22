// industrial.h
// Copyright (C) 2020, Regents of the Leland Stanford Junior University

#ifndef _G_INDUSTRIAL_H
#define _G_INDUSTRIAL_H

#include "gridlabd.h"

class industrial : public gld_object {

public:
	char1024 naics_datafile;

public:

	GL_ATOMIC(int32,naics_code);

	/* required implementations */
	industrial(MODULE *module);
	int create(void);
	int init(OBJECT *parent);

public:
	static CLASS *oclass;
	static industrial *defaults;
};

#endif // _ASSERT_H
