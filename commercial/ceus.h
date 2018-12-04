// ceus.h
// Copyright (C) 2018 Stanford University
// Author: dchassin@stanford.edu
//
// Commercial building loads using CEUS data
//
// Author: dchassin@stanford.edu
//

#ifndef _CEUS_H
#define _CEUS_H

#include "gridlabd.h"

DECL_METHOD(ceus,composition);

#define DATASIZE 2016
class ceus : public gld_object 
{
public:
	typedef enum {
		WEEKDAY = 10,
		SATURDAY = 11,
		SUNDAY = 12,
		HOLIDAY = 13,
		PEAK = 14,
		HOTTEST = 15,
		COLDEST = 16,
	} DAYTYPE;
public:
	typedef struct s_ceusdata 
	{
		char *filename;
		char *segment;
		char *enduse;
		double data[2016];
		struct s_ceusdata *next;
	} CEUSDATA;
	static CEUSDATA *repository;
	static CEUSDATA *get_first_repository(void);
	static CEUSDATA *get_next_repository(CEUSDATA *repo);
	static CEUSDATA *find_repository(char *filename=NULL, char *segment=NULL, char *enduse=NULL);
	static CEUSDATA *new_repository();
	static CEUSDATA *set_repository_filename(CEUSDATA *repo, char *filename);
	static CEUSDATA *set_repository_segment(CEUSDATA *repo, char *segment);
	static CEUSDATA *set_repository_enduse(CEUSDATA *repo, char *enduse);
	static CEUSDATA *set_repository_value(CEUSDATA *repo, int month, int day, int hour, double value);
	static double get_repository_value(CEUSDATA *repo, TIMESTAMP ts, double scalar=1.0);
	CEUSDATA *data;
public:
	typedef struct s_component 
	{
		char *enduse;  // enduse name
		double Zr, Zi; // constant impedance factors (real, imaginary)
		double Ir, Ii; // constant current factors (real, imaginary)
		double Pr, Pi; // constant power factors (real, imaginary)
		struct s_component *next;
	} COMPONENT;
	COMPONENT *get_first_component();
	COMPONENT *get_next_component(COMPONENT *c);
	COMPONENT *add_component(char *enduse);
	COMPONENT *set_component(char *term, double value);
	COMPONENT *find_component();
	COMPONENT *components;
protected:
	GL_ATOMIC(char1024,filename);
	GL_ATOMIC(enumeration,building_type);
	GL_ATOMIC(double,floor_area);
	GL_METHOD(ceus,composition);
private:
	bool load_data(void);
public:
	ceus(MODULE *module);
	int create(void);
	int init(OBJECT *parent);
	TIMESTAMP precommit(TIMESTAMP t1);
public:
	static CLASS *oclass;
	static ceus *defaults;
};

#endif // _CEUS_H
