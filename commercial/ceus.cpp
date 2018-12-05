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
EXPORT_SYNC(ceus);
EXPORT_METHOD(ceus,composition);
EXPORT_LOADMETHOD(ceus,filename);

CLASS *ceus::oclass = NULL;
ceus *ceus::defaults = NULL;

complex ceus::default_nominal_voltage_A(240.0,0.0,A);
complex ceus::default_nominal_voltage_B(240.0,-120.0,A);
complex ceus::default_nominal_voltage_C(240.0,+120.0,A);

//////////////////////////
// CEUS DATA REPOSITORY
//////////////////////////
ceus::CEUSDATA *ceus::repository = NULL;
ceus::CEUSDATA *ceus::add_file(const char *filename)
{
	CEUSDATA *repo = (CEUSDATA*)malloc(sizeof(CEUSDATA));
	if ( repo != NULL ) 
	{
		memset(repo,0,sizeof(CEUSDATA));
		repo->filename = strdup(filename);
		repo->next_file = repository;
		repository = repo;
	}
	return repo;
}
ceus::CEUSDATA *ceus::get_first_file(void)
{
	return repository;
}
ceus::CEUSDATA *ceus::get_next_file(CEUSDATA *repo)
{
	return repo->next_file;
}
ceus::CEUSDATA *ceus::find_file(const char *filename)
{
	CEUSDATA *repo;
	for ( repo = get_first_file() ; repo != NULL ; repo = get_next_file(repo) ) 
	{
		if ( strcmp(filename,repo->filename) == 0 )
			return repo;
	}
	return NULL;
}
ceus::CEUSDATA *ceus::add_enduse(const char *filename, const char *enduse)
{
	CEUSDATA *repo = find_file(filename);
	if ( repo == NULL )
		repo = add_file(filename);
	return add_enduse(repo,enduse);
}
ceus::CEUSDATA *ceus::add_enduse(CEUSDATA *repo, const char *enduse)
{
	if ( repo->enduse != NULL )
	{
		CEUSDATA *next = (CEUSDATA*)malloc(sizeof(CEUSDATA));
		memset(next,0,sizeof(CEUSDATA));
		next->filename = repo->filename;
		next->next_enduse = repo;
		repo = next;
	}
	repo->enduse = strdup(enduse);
	return repo;
}
ceus::CEUSDATA *ceus::get_first_enduse(const char *filename)
{
	CEUSDATA *repo = find_file(filename);
	return repo;
}
ceus::CEUSDATA *ceus::get_next_enduse(CEUSDATA *repo)
{
	return repo->next_enduse;
}
ceus::CEUSDATA *ceus::find_enduse(const char *filename, const char *enduse)
{
	CEUSDATA *repo = find_file(filename);
	return find_enduse(repo,enduse);
}
ceus::CEUSDATA *ceus::find_enduse(CEUSDATA *repo, const char *enduse)
{
	while ( repo != NULL )
	{
		if ( strcmp(repo->enduse,enduse) == 0 )
			break;
	}
	return repo;
}
size_t ceus::get_index(unsigned int month, unsigned int daytype, unsigned int hour)
{
	return ((((month-1)*7)+(daytype-10))*24)+(hour-1);
}
size_t ceus::get_index(TIMESTAMP ts)
{
	// simple caching of last index
	static gld_clock dt;
	static size_t index = 0;
	if ( (TIMESTAMP)dt != ts )
	{
		dt = gld_clock(ts);
		unsigned int weekday = dt.get_weekday();
		bool is_weekend = (weekday==0 || weekday>5);
		unsigned int daytype = 10 + (is_weekend?1:0);
		index = get_index(dt.get_month(),daytype,dt.get_hour());
	}
	return index;
}
size_t ceus::get_index(void)
{
	return get_index((TIMESTAMP)gld_clock());
}
void ceus::set_value(CEUSDATA *repo, TIMESTAMP ts, double value)
{
	size_t index = get_index(ts);
	repo->data[index] = value;
}
double ceus::get_value(CEUSDATA *repo, TIMESTAMP ts, double scalar)
{
	size_t index = get_index(ts);
	return repo->data[index]*scalar;
}

//////////////////////////
// CEUS LOAD COMPONENTS
//////////////////////////
ceus::COMPONENT *ceus::get_first_component()
{
	return components;	
}
ceus::COMPONENT *ceus::get_next_component(COMPONENT *c)
{
	return c->next;
}
ceus::COMPONENT *ceus::add_component(const char *enduse, const char *composition)
{
	CEUSDATA *e = find_enduse(data, enduse);
	if ( e == NULL )
	{
		error("unable to add composition '%s' -- enduse '%s' unknown",composition,enduse);
		return NULL;	
	}

	COMPONENT *c = (COMPONENT*)malloc(sizeof(COMPONENT));
	memset(c,0,sizeof(COMPONENT));
	c->data = data;
	char *buffer = strdup(composition);
	char *item, *last = NULL;
	while ( (item=strtok_r((last?NULL:buffer),";",&last)) != NULL )
	{
		char term[64];
		double value = 0;
		//debug("parsing enduse '%s' component '%s'",enduse,item);
		if ( sscanf(item,"%63[^:]:%lg",term,&value) < 2 )
		{
			error("unable to parse term '%s' of enduse '%s' ", item, enduse);
			goto Error;
		}
		//debug("setting enduse '%s' composition '%s' to %lg",enduse,term,value);
		if ( ! set_component(c,term,value) )
		{
			error("unable to set term '%s' of enduse '%s' ", item, enduse);
			goto Error;
		}
		if ( last == NULL )
			break;
	}
	c->next = components;
	components = c;
Done:
	free(buffer);
	return c;
Error:
	free(c);
	c = NULL;
	goto Done;
}
bool ceus::set_component(const char *enduse, const char *term, double value)
{
	COMPONENT *c = find_component(enduse);
	return c ? set_component(c,term,value) : false;
}
bool ceus::set_component(COMPONENT *component, const char *term, double value)
{
	struct {
		char *item;
		double &value;
	} map[] = {
		{"Zr", component->Zr},
		{"Zi", component->Zi},
		{"Ir", component->Ir},
		{"Ii", component->Ii},
		{"Pr", component->Pr},
		{"Pi", component->Pi},
	};
	size_t n;
	for ( n = 0 ; n < sizeof(map)/sizeof(map[0]) ; n++ )
	{
		if ( strcasecmp(map[n].item,term)==0 )
		{
			map[n].value = value;
			debug("%s.: %s.%s <- %g",component->data->filename, component->data->enduse, map[n].item,value);
			return true;
		}
	}
	error("term '%s' is not recognized",term);
	return false;
}
ceus::COMPONENT *ceus::find_component(const char *enduse)
{
	COMPONENT *c;
	for ( c = get_first_component() ; c != NULL ; c = get_next_component(c) )
	{
		if ( strcmp(c->data->enduse,enduse) == 0 )
			break;
	}
	return c;
}

///////////////////////////////
// CEUS CLASS IMPLEMENTATION
///////////////////////////////
ceus::ceus(MODULE *module)
{
	if (oclass==NULL)
	{
		// register to receive notice for first top down. bottom up, and second top down synchronizations
		oclass = gld_class::create(module,"ceus",sizeof(ceus),PC_BOTTOMUP|PC_AUTOLOCK|PC_OBSERVER);
		if (oclass==NULL)
			throw "unable to register class ceus";
		else
			oclass->trl = TRL_PROVEN;

		defaults = this;
		if (gl_publish_variable(oclass,
			PT_double,"floor_area[sf]", get_floor_area_offset(), PT_DESCRIPTION, "building floor area",
			PT_method,"filename", loadmethod_ceus_filename, PT_DESCRIPTION, "CEUS data file",
			PT_method,"composition", method_ceus_composition, PT_DESCRIPTION, "load composition specification",
			PT_double,"total_real_power[W]", get_total_real_power_offset(), PT_DESCRIPTION, "total real power",
			PT_double,"total_reactive_power[W]", get_total_reactive_power_offset(), PT_DESCRIPTION, "total reactive power",
			PT_complex,"total_power_A[W]", get_total_power_A_offset(), PT_DESCRIPTION, "total complex power on phase A",
			PT_complex,"total_power_B[W]", get_total_power_B_offset(), PT_DESCRIPTION, "total complex power on phase B",
			PT_complex,"total_power_C[W]", get_total_power_C_offset(), PT_DESCRIPTION, "total complex power on phase C",
			NULL)<1){
				char msg[256];
				sprintf(msg, "unable to publish properties in %s",__FILE__);
				throw msg;
		}
		gl_global_create("default_nominal_voltage_A",PT_double,&default_nominal_voltage_A,NULL);
		gl_global_create("default_nominal_voltage_B",PT_double,&default_nominal_voltage_B,NULL);
		gl_global_create("default_nominal_voltage_C",PT_double,&default_nominal_voltage_C,NULL);
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
	if ( floor_area <= 0.0 )
	{
		warning("zero or negative floor area disables the load composition model");
		floor_area = 0.0;
	}
	if ( get_parent() == NULL )
	{
		warning("parent is not a meter -- using default_nominal_voltages");
		voltage_A = &default_nominal_voltage_A;
		voltage_B = &default_nominal_voltage_B;
		voltage_C = &default_nominal_voltage_C;
	}
	else if ( ! get_parent()->isa("meter") )
	{
		gld_property VA(get_parent(),"voltage_A");
		gld_property VB(get_parent(),"voltage_B");
		gld_property VC(get_parent(),"voltage_C");
		if ( VA.is_valid() ) 
			voltage_A = (complex*)VA.get_addr();
		else
			exception("unable to find 'voltage_A' in meter '%s'",get_parent()->get_name());
		if ( VB.is_valid() ) 
			voltage_B = (complex*)VB.get_addr();
		else
			exception("unable to find 'voltage_B' in meter '%s'",get_parent()->get_name());
		if ( VC.is_valid() ) 
			voltage_C = (complex*)VC.get_addr();
		else
			exception("unable to find 'voltage_C' in meter '%s'",get_parent()->get_name());
	}
	return data ? 1 : 0 ; /* return 1 on success, 0 on failure */
}

TIMESTAMP ceus::presync(TIMESTAMP t1)
{
	return TS_NEVER;
}
TIMESTAMP ceus::sync(TIMESTAMP t1)
{
	size_t index = get_index();
	COMPONENT *c;
	total_power_A = total_power_B = total_power_C;
	for ( c = get_first_component() ; c != NULL ; c = get_next_component(c) )
	{
		total_power_A += ((~(*voltage_A)*c->Zr + c->Ir)*(*voltage_A) + c->Pr) * floor_area;
		total_power_B += ((~(*voltage_B)*c->Zr + c->Ir)*(*voltage_B) + c->Pr) * floor_area;
		total_power_C += ((~(*voltage_C)*c->Zr + c->Ir)*(*voltage_C) + c->Pr) * floor_area;
		verbose("%s:%s{Zr:%g;Zi:%g;Ir:%g;Ii:%g;Pr:%g;Pi:%g,A:%g",
			c->data->filename, c->data->enduse,
			c->Zr, c->Zi, c->Ir, c->Ii, c->Pr, c->Pi, floor_area);
	}
	total_real_power = (total_power_A+total_power_A+total_power_C).Re();
	total_reactive_power = (total_power_A+total_power_A+total_power_C).Im();
	verbose("total P=%g, Q=%g",total_real_power, total_reactive_power);
	return (gl_globalclock/3600+1)*3600;
}
TIMESTAMP ceus::postsync(TIMESTAMP t1)
{
	return TS_NEVER;
}

int ceus::composition(char *buffer, size_t len)
{
	if ( len == 0 ) // read
	{
		verbose("processing composition '%s'",buffer);
		char enduse[1024];
		char composition[1024];
		if ( sscanf(buffer,"%[^:]:{%[^}]}",enduse,composition) < 2 )
		{
			error("composition '%s' is not formatted correctly (expected 'enduse:{component:factor;...}')",buffer);
			return 0;
		}
		if ( find_component(enduse) )
		{
			error("composition '%s' has already been specified",enduse);
			return 0;
		}
		add_component(enduse,composition);
	}
	else // write
	{
		// TODO
	}
	return 1; /* return non-zero on success, 0 on failure */
}

int ceus::filename(const char *filename)
{
	// link to existing data if already loaded
	data = find_file(filename);
	if ( data != NULL )
	{
		verbose("file '%s' already loaded", (const char*)filename);
		return 1;
	}

	// open file
	FILE *fp = fopen(filename,"r");
	if ( fp == NULL )
	{
		error("file '%s' not found",(const char*)filename);
		return 0;
	}
	data = add_file(filename);

	// load header
	char header[1024];
	if ( fgets(header,sizeof(header),fp) == NULL )
	{
		error("unable to read header in file '%s'",(const char*)filename);
		fclose(fp);
		return 0;
	}
	char *item, *last = NULL;
	typedef enum {DT_STRING, DT_INTEGER, DT_REAL} DATATYPE;
#define MAXDATA 32
	struct s_colspec {
		char *name;
		char *format;
		DATATYPE type;
		union {
			char string[32];
			int integer;
			double real;
		} buffer;
		CEUSDATA *data;
	} map[MAXDATA];
	size_t max_column = 0;
	memset(map,sizeof(map),0);
	size_t segment_ndx = 0;
	size_t month_ndx = 0;
	size_t daytype_ndx = 0;
	size_t hour_ndx = 0;
	size_t enduse_ndx = 0;
	while ( (item=strtok_r(last?NULL:header,",",&last)) != NULL )
	{
		//debug("column %d: name '%s'", max_column, item);
		if ( max_column >= MAXDATA )
		{
			error("too many columns of data in file '%s' (maximum is %d)",(const char*)filename, MAXDATA);
			fclose(fp);
			return false;
		}
		map[max_column].name = strdup(item);
		if ( strcmp(item,"SegID")==0 )
		{
			map[max_column].type = DT_STRING;
			map[max_column].format = "%s";
			segment_ndx = max_column;
		}
		else if ( strcmp(item,"Mth")==0 )
		{
			map[max_column].type = DT_INTEGER;
			map[max_column].format = "%d";
			month_ndx = max_column;
		}
		else if ( strcmp(item,"Dy")==0 )
		{
			map[max_column].type = DT_INTEGER;
			map[max_column].format = "%d";
			daytype_ndx = max_column;
		}
		else if ( strcmp(item,"Hr")==0 )
		{
			map[max_column].type = DT_INTEGER;
			map[max_column].format = "%d";
			hour_ndx = max_column;
		}
		else // enduse
		{
			map[max_column].type = DT_REAL;
			map[max_column].format = "%g";
			if ( enduse_ndx == 0 )
				enduse_ndx = max_column;
			map[max_column].data = add_enduse(data,item);
		}
		max_column++;
		if ( last == NULL ) break;
	}
	debug("%s: found %d columns", filename, max_column);

	// load records
	char line[1024];
	size_t count = 0;
	while ( fgets(line,sizeof(line),fp) != NULL )
	{
		// debug("%s: processing line '%s'", filename, line);
		if ( count >= DATASIZE )
		{
			error("ignoring extra data in '%s' after line %d",(const char*)filename,count);
			fclose(fp);
		}
		last = NULL;
		size_t column = 0;
		while ( (item=strtok_r(last?NULL:line,",\n",&last)) != NULL)
		{
			//debug("column %d, value '%s', format '%s', ", column, item, map[column].format);
			if ( column == max_column )
			{
				error("too many columns of data in '%s' at line %d",(const char*)filename, count+1);
				fclose(fp);
				return 0;
			}
			if ( sscanf(item,map[column].format,&(map[column].buffer)) != 1 )
			{
				error("error parsing data in '%s' line %d column %d",(const char*)filename, count+1, column+1);
				fclose(fp);
				return 0;
			}
			column++;
			if ( last == NULL ) break;
		}
		size_t n;
		unsigned int month = map[month_ndx].buffer.integer;
		unsigned int daytype = map[daytype_ndx].buffer.integer;
		unsigned int hour = map[hour_ndx].buffer.integer;
		for ( n = enduse_ndx ; n < column ; n++ )
			map[n].data->data[count] = map[n].buffer.real;
		count++;
	}
	if ( count != DATASIZE )
		error("missing data in CEUS file '%s' (only %d records found, expected %d",(const char*)filename,count,DATASIZE);
	else
		verbose("%d records loaded from file '%s'", count, (const char*)filename);
	fclose(fp);
	return 1;
}
