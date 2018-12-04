// ceus.cpp
// Copyright (C) 2018 Stanford University
//
// Commercial building loads using CEUS data
//
// Author: dchassin@stanford.edu
//

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>
#include <complex.h>

#include "ceus.h"

EXPORT_CREATE(ceus);
EXPORT_INIT(ceus);
EXPORT_PRECOMMIT(ceus);
EXPORT_METHOD(ceus,composition);

CLASS *ceus::oclass = NULL;
ceus *ceus::defaults = NULL;

//////////////////////////
// CEUS DATA REPOSITORY
//////////////////////////
ceus::CEUSDATA *ceus::repository = NULL;
ceus::CEUSDATA *ceus::get_first_repository(void)
{
	return repository;
}
ceus::CEUSDATA *ceus::get_next_repository(CEUSDATA *repo)
{
	return repo->next;
}
ceus::CEUSDATA *ceus::find_repository(char *filename, char *enduse)
{
	CEUSDATA *repo;
	for ( repo = get_first_repository() ; repo != NULL ; repo = get_next_repository(repo) ) 
	{
		if ( strcmp(filename,repo->filename)!=0 )
			continue;
		if ( enduse==NULL || strcmp(enduse,repo->enduse)==0 )
			return repo;
	}
	return NULL;
}
ceus::CEUSDATA *ceus::new_repository(char *filename, char *enduse)
{
	CEUSDATA *repo = (CEUSDATA*)malloc(sizeof(CEUSDATA));
	if ( repo != NULL ) 
	{
		memset(repo,0,sizeof(CEUSDATA));
		repo->next = repository;
		repository = repo;
	}
	return repo;
}

///////////////////////////////
// CEUS CLASS IMPLEMENTATION
///////////////////////////////
ceus::ceus(MODULE *module)
{
	if (oclass==NULL)
	{
		// register to receive notice for first top down. bottom up, and second top down synchronizations
		oclass = gld_class::create(module,"ceus",sizeof(ceus),PC_AUTOLOCK|PC_OBSERVER);
		if (oclass==NULL)
			throw "unable to register class ceus";
		else
			oclass->trl = TRL_PROVEN;

		defaults = this;
		if (gl_publish_variable(oclass,
			PT_char1024, "filename", get_filename_offset(), PT_DESCRIPTION, "Data source file name",
			PT_double,"floor_area[sf]", get_floor_area_offset(), PT_DESCRIPTION, "Building floor area",
			PT_method, "composition", method_ceus_composition, PT_DESCRIPTION, "Load composition specification",
			NULL)<1){
				char msg[256];
				sprintf(msg, "unable to publish properties in %s",__FILE__);
				throw msg;
		}
		memset(this,0,sizeof(ceus));
	}
}

int ceus::create(void) 
{
	memcpy(this,defaults,sizeof(*this));
	// TODO
	return 1; /* return 1 on success, 0 on failure */
}

int ceus::init(OBJECT *parent)
{
	// TODO
	return load_data() ? 1 : 0 ; /* return 1 on success, 0 on failure */
}

TIMESTAMP ceus::precommit(TIMESTAMP t1)
{
	// TODO
	return TS_NEVER;
}

int ceus::composition(char *buffer, size_t len)
{
	if ( len == 0 ) // read
	{
		char enduse[1024];
		char composition[1024];
		if ( sscanf(buffer,"%[^:]:{%[^}]}",enduse,composition) < 2 )
		{
			gl_error("composition '%s' is not formatted correctly (expected 'enduse:{component:factor;...}')",buffer);
			return 0;
		}
	}
	else // write
	{
		// TODO
	}
	return 1; /* return non-zero on success, 0 on failure */
}

bool ceus::load_data(void)
{
	// link to existing data if already loaded
	CEUSDATA *data = find_repository(filename);
	if ( data != NULL )
	{
		gl_verbose("CEUS repository '%s' already loaded", (const char*)filename);
		return true;
	}

	// open file
	FILE *fp = fopen(filename,"r");
	if ( fp == NULL )
	{
		gl_error("CEUS file '%s' not found",(const char*)filename);
		return false;
	}

	// load header
	char header[1024];
	if ( fgets(header,sizeof(header),fp) == NULL )
	{
		gl_error("unable to read header in CEUS  ile '%s'",(const char*)filename);
		fclose(fp);
		return false;
	}
	char *item, *last = NULL;
	typedef enum {DT_STRING, DT_INTEGER, DT_REAL} DATATYPE;
	struct s_colspec {
		char *name;
		char *format;
		DATATYPE type;
		union {
			char string[32];
			int integer;
			double real;
		} buffer;
	} map[32];
	size_t max_column = 0;
	memset(map,sizeof(map),0);
	while ( (item=strtok_r(last?header:NULL,",",&last)) != NULL )
	{
		if ( max_column >= sizeof(map)/sizeof(map[0]) )
		{
			gl_error("too many columns of data in CEUS file '%s'",(const char*)filename);
			fclose(fp);
			return false;
		}
		map[max_column].name = strdup(item);
		if ( strcmp(item,"SegID")==0 )
		{
			map[max_column].type = DT_STRING;
			map[max_column].format = "%s";
		}
		else if ( strcmp(item,"Mth")==0 || strcmp(item,"Dy")==0 || strcmp(item,"Hr")==0 )
		{
			map[max_column].type = DT_INTEGER;
			map[max_column].format = "%d";
		}
		else // enduse
		{
			map[max_column].type = DT_REAL;
			map[max_column].format = "%g";
		}
		max_column++;
	}

	// load records
	char line[1024];
	size_t count = 0;
	while ( fgets(line,sizeof(line),fp) != NULL )
	{
		if ( count >= DATASIZE )
		{
			gl_error("ignoring extra data in CEUS file '%s' after line %d",(const char*)filename,count);
			fclose(fp);
		}
		last = NULL;
		size_t column = 0;
		while ( (item=strtok_r(last?header:NULL,",",&last)) != NULL )
		{
			if ( column == max_column )
			{
				gl_error("too many columns of data in CEUS file '%s' at line %d",(const char*)filename, count+1);
				fclose(fp);
				return false;
			}
			if ( sscanf(item,map[column].format,map[column].buffer) == 1 )
			{
				// TODO: process item
			}
			column++;
		}
		count++;
	}
	if ( count != DATASIZE )
		gl_error("missing data in CEUS file '%s' (only %d records found, expected %d",(const char*)filename,count,DATASIZE);
	else
		gl_verbose("CEUS file '%s' %d records loaded ok",(const char*)filename, count);
	fclose(fp);
	return true;
}
