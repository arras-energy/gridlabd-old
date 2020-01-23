// industrial.h
// Copyright (C) 2020, Regents of the Leland Stanford Junior University

#ifndef _G_INDUSTRIAL_H
#define _G_INDUSTRIAL_H

#include "gridlabd.h"

class naics
{
public:
	typedef struct s_record
	{
		int n;
		double a,b,c,d,e,i,z;
	} RECORD;
private:
	std::list<RECORD> data;
public:
	naics(const char *file);
	~naics(void);
	void add(const char *str);
	RECORD &find(int n);
};

class industrial : public gld_object {

public:
	static char1024 naics_data_file;
	static naics *naics_data;

public:

	GL_ATOMIC(int32,naics_code);

public:

	/* required implementations */
	industrial(MODULE *module);
	int create(void);
	int init(OBJECT *parent);
	TIMESTAMP precommit(TIMESTAMP t1);

public:
	static CLASS *oclass;
	static industrial *defaults;
};

#endif // _ASSERT_H
