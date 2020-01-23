// industrial.h
// Copyright (C) 2020, Regents of the Leland Stanford Junior University

#include "industrial.h"

EXPORT_CREATE(industrial);
EXPORT_INIT(industrial);
EXPORT_PRECOMMIT(industrial);

CLASS *industrial::oclass = NULL;
industrial *industrial::defaults = NULL;

char1024 industrial::naics_data_file = "naics_data_file.csv";
naics *industrial::naics_data = NULL;

industrial::industrial(MODULE *module)
{
	if (oclass==NULL)
	{
		// register to receive notice for first top down. bottom up, and second top down synchronizations
		oclass = gld_class::create(module,"industrial",sizeof(industrial),PC_AUTOLOCK|PC_OBSERVER);
		if (oclass==NULL)
			throw "unable to register class industrial";
		else
			oclass->trl = TRL_PROVEN;

		defaults = this;
		if (gl_publish_variable(oclass,
			
			PT_int32,"naics_code",get_naics_code_offset(), PT_DESCRIPTION,"NAICS facility code",

			NULL)<1){
				char msg[256];
				sprintf(msg, "unable to publish properties in %s",__FILE__);
				throw msg;
		}
		gl_global_create("industrial::naics_data_file",PT_char1024,&naics_data_file,
			PT_DESCRIPTION, "data file that contains NAICS load data model",
			NULL);
	}
}

int industrial::create(void) 
{
	return 1; /* return 1 on success, 0 on failure */
}

int industrial::init(OBJECT *parent)
{
	naics_data = new naics(naics_data_file);
	naics::RECORD &zip = naics_data->find(naics_code);
	return 1;
}

TIMESTAMP industrial::precommit(TIMESTAMP t1)
{

	return TS_NEVER;
}

naics::naics(const char *filename)
{
	FILE *fp = fopen(filename,"r");
	if ( fp == NULL )
	{
		throw "unable to open naics data file";
	}
	char line[1024];
	while ( fgets(line,sizeof(line),fp) != NULL )
	{
		add(line);
	}
	fclose(fp);
}

naics::~naics(void)
{
	data.clear();
}

void naics::add(const char *str)
{
	RECORD *item = new RECORD;
	if ( item == NULL )
		throw "memory allocation failed";
	if ( sscanf(str,"%d,%*s,%*s,%lg,%lg,%lg,%lg,%lg,%lg,%lg",
		&item->n, &item->a, &item->b, &item->c, &item->d, &item->e, &item->z, &item->i) == 8 )
	{
		data.insert(data.end(),*item);
	}
}

naics::RECORD &naics::find(int n)
{
	for ( std::list<RECORD>::iterator p = data.begin() ; p != data.end() ; p++ )
	{
		if ( p->n == n )
			return *p;
	}
	throw "naics code not found";
}