// File: recorder.h
// Copyright (C) 2020 Regents of the Leland Stanford Junior University

#ifndef _RECORDER_H
#define _RECORDER_H

#include "jsondata.h"
#include "gridlabd.h"
#include "database.h"

#define MO_NONE     0x0000 // no measurement options specified
#define MO_USEUNITS	0x0001 // add units to column names

class recorder : public gld_object 
{

public:

	GL_STRING(char1024,property);
	GL_STRING(char1024,table);
	GL_ATOMIC(double,interval);
	GL_OBJECT(database,connection);
	GL_ATOMIC(set,options);
	GL_STRING(char1024,tags);

private:

	void make_property_list(const char *delim = ", \t;");
	properties *property_list;
	database *db;
	char *oldvalues;
	measurements *measurement;
    properties *taglist;
    std::string *tagtext;
    int get_taglist_size();
    int get_taglist(char *buffer,int size);
    int add_taglist(char *buffer);

public:

	recorder(MODULE *module);
	int create(void);
	int init(OBJECT *parent);
	int finalize(void);
	TIMESTAMP commit(TIMESTAMP t0, TIMESTAMP t1);

public:

	static CLASS *oclass;
	static recorder *defaults;
};

#endif
