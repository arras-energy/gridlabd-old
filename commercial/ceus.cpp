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

double ceus::default_nominal_voltage = 240.0;
complex ceus::default_nominal_voltage_A(240.0,0.0,A);
complex ceus::default_nominal_voltage_B(240.0,-120.0,A);
complex ceus::default_nominal_voltage_C(240.0,+120.0,A);
char32 ceus::default_weekday_code ="WEEKDAY";
char32 ceus::default_saturday_code ="SATURDAY";
char32 ceus::default_sunday_code ="SUNDAY";
char32 ceus::default_holiday_code ="HOLIDAY";
char32 ceus::default_month_heading = "Month";
char32 ceus::default_daytype_heading = "Daytype";
char32 ceus::default_hour_heading = "Hour";
char1024 ceus::temperature_variable_name = "temperature";
char1024 ceus::solargain_variable_name = "solar_direct";
char1024 ceus::price_variable_name = "energy_price";
char1024 ceus::occupancy_variable_name = "occupancy_fraction";
double ceus::default_temperature_cooling_balance = 70.0;
double ceus::default_temperature_cooling_base = 70.0;
double ceus::default_temperature_cooling_design = 100.0;
double ceus::default_temperature_heating_balance = 55.0;
double ceus::default_temperature_heating_base = 55.0;
double ceus::default_temperature_heating_design = 0.0;
double ceus::default_solargain_base = 0.0;
double ceus::default_price_base = 0.0;
double ceus::default_occupancy_base = 1.0;

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
ceus::CEUSDATA * ceus::add_enduse(const char *filename, const char *enduse)
{
	CEUSDATA *repo = find_file(filename);
	if ( repo == NULL )
	{
		repo = add_file(filename);
	}
	return add_enduse(repo,enduse);
}
ceus::CEUSDATA * ceus::add_enduse(CEUSDATA *repo, const char *enduse)
{
	while ( repo->next_enduse != NULL && repo->enduse != NULL )
	{
		repo = repo->next_enduse;
	}
	if ( repo->enduse != NULL ) // this is aleady in use
	{
		CEUSDATA *item = (CEUSDATA*)malloc(sizeof(CEUSDATA));
		memset(item,0,sizeof(CEUSDATA));
		item->filename = repo->filename;
		item->next_file = repo->next_file;
		repo->next_enduse = item;
		repo = item;
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
		{
			break;
		}
		repo = repo->next_enduse;
	}
	return repo;
}
size_t ceus::get_index(unsigned int month, unsigned int daytype, unsigned int hour)
{
	size_t index = ((((month-1)*_DT_SIZE)+(daytype-0))*24)+hour;
	return index;
}
size_t ceus::get_index(TIMESTAMP ts)
{
	// simple caching of last index
	static gld_clock dt;
	static size_t index = 0;
	if ( (TIMESTAMP)dt != ts )
	{
		dt = gld_clock(ts);
		unsigned int daytype;
		switch ( dt.get_weekday() ) {
		case 0:
			daytype = DT_SUNDAY;
			break;
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			daytype = DT_WEEKDAY;
			break;
		case 6:
			daytype = DT_SATURDAY;
			break;
		default:
			throw "invalid daytype";
			break;
		}
		index = get_index(dt.get_month(), daytype, dt.get_hour());
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
	double value = repo->data[index]*scalar;
	return value;
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
		warning("unable to add composition '%s' -- enduse '%s' not found",composition,enduse);
		return NULL;	
	}

	COMPONENT *c = (COMPONENT*)malloc(sizeof(COMPONENT));
	memset(c,0,sizeof(COMPONENT));
	c->fraction = 1.0;
	c->data = data;
	char *buffer = strdup(composition);
	char *item, *last = NULL;
	while ( (item=strtok_r((last?NULL:buffer),";}",&last)) != NULL )
	{
		char term[64];
		double value = 0;
		if ( sscanf(item,"%63[^:]:%lg",term,&value) < 2 )
		{
			error("unable to parse term '%s' of enduse '%s' ", item, enduse);
			goto Error;
		}
		debug("%s.%s <- %lg",enduse,term,value);
		if ( ! set_component(c,term,value) )
		{
			error("unable to set term '%s' of enduse '%s' ", item, enduse);
			goto Error;
		}
		if ( last == NULL )
		{
			break;
		}
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
		const char *item;
		double &value;
	} map[] = {
		{"Zr", component->Zr},
		{"Zi", component->Zi},

		{"Ir", component->Ir},
		{"Ii", component->Ii},

		{"Pr", component->Pr},
		{"Pi", component->Pi},

		{"Th", component->heating.slope},
		{"Thb", component->heating.base},
		{"Thc", component->heating.intercept},
		{"Th0", component->heating.domain.min},
		{"Th1", component->heating.domain.max},

		{"Tc", component->cooling.slope},
		{"Tcb", component->cooling.base},
		{"Tcc", component->cooling.intercept},
		{"Tc0", component->cooling.domain.min},
		{"Tc1", component->cooling.domain.max},

		{"S", component->solar.slope},
		{"Sb", component->solar.base},
		{"Sb", component->solar.intercept},
		{"S0", component->solar.domain.min},
		{"S1", component->solar.domain.max},

		{"E", component->price.slope},
		{"Eb", component->price.base},
		{"Eb", component->price.intercept},
		{"E0", component->price.domain.min},
		{"E1", component->price.domain.max},

		{"O", component->occupancy.slope},
		{"Ob", component->occupancy.base},
		{"Ob", component->occupancy.intercept},
		{"O0", component->occupancy.domain.min},
		{"O1", component->occupancy.domain.max},

		{"Area", component->fraction},
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
		{
			break;
		}
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
		oclass = gld_class::create(module,"ceus",sizeof(ceus),PC_PRETOPDOWN|PC_BOTTOMUP|PC_AUTOLOCK|PC_OBSERVER);
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
			PT_object,"weather",get_weather_offset(), PT_DESCRIPTION, "weather object for temperature and solar gain",
			PT_object,"tariff",get_tariff_offset(), PT_DESCRIPTION, "tariff object for energy price sensitivity",
			NULL)<1)
		{
				char msg[256];
				sprintf(msg, "unable to publish properties in %s",__FILE__);
				throw msg;
		}
		gl_global_create("default_nominal_voltage_A",PT_complex,&default_nominal_voltage_A,NULL);
		gl_global_create("default_nominal_voltage_B",PT_complex,&default_nominal_voltage_B,NULL);
		gl_global_create("default_nominal_voltage_C",PT_complex,&default_nominal_voltage_C,NULL);
		gl_global_create("default_nominal_voltage",PT_double,&default_nominal_voltage,NULL);
		gl_global_create("default_weekday_code",PT_char32,&default_weekday_code,NULL);
		gl_global_create("default_saturday_code",PT_char32,&default_saturday_code,NULL);
		gl_global_create("default_sunday_code",PT_char32,&default_sunday_code,NULL);
		gl_global_create("default_holiday_code",PT_char32,&default_holiday_code,NULL);
		gl_global_create("default_month_heading",PT_char32,&default_month_heading,NULL);
		gl_global_create("default_daytype_heading",PT_char32,&default_daytype_heading,NULL);
		gl_global_create("default_hour_heading",PT_char32,&default_hour_heading,NULL);
		gl_global_create("default_temperature_heating_balance",PT_double,&default_temperature_heating_balance,NULL);
		gl_global_create("default_temperature_cooling_balance",PT_double,&default_temperature_cooling_balance,NULL);
		gl_global_create("default_temperature_heating_base",PT_double,&default_temperature_heating_base,NULL);
		gl_global_create("default_temperature_cooling_base",PT_double,&default_temperature_cooling_base,NULL);
		gl_global_create("default_temperature_heating_design",PT_double,&default_temperature_heating_design,NULL);
		gl_global_create("default_temperature_cooling_design",PT_double,&default_temperature_cooling_design,NULL);
		gl_global_create("default_solargain_base",PT_double,&default_solargain_base,NULL);
		gl_global_create("default_price_base",PT_double,&default_price_base,NULL);
		gl_global_create("default_occupancy_base",PT_double,&default_occupancy_base,NULL);
		gl_global_create("temperature_variable_name",PT_char1024,&temperature_variable_name,NULL);
		gl_global_create("solargain_variable_name",PT_char1024,&solargain_variable_name,NULL);
		gl_global_create("price_variable_name",PT_char1024,&price_variable_name,NULL);
		gl_global_create("occupancy_variable_name",PT_char1024,&occupancy_variable_name,NULL);
	}
}

int ceus::create(void) 
{

	memcpy(this,defaults,sizeof(*this));
	return 1; 
}

int ceus::init(OBJECT *parent)
{
	if ( floor_area <= 0.0 )
	{
		warning("zero or negative floor area disables the load composition model");
		floor_area = 0.0;
	}
	if ( get_parent() == NULL )
		warning("parent is not specified -- using default voltages");
	else {
		if ( ! get_parent()->isa("meter") )
			warning("parent is not a meter -- using default voltages if necessary");
		link_property(voltage_A,get_parent(),"voltage_A");
		link_property(voltage_B,get_parent(),"voltage_B");
		link_property(voltage_C,get_parent(),"voltage_C");
		link_property(power_A,get_parent(),"power_A");
		link_property(power_B,get_parent(),"power_B");
		link_property(power_C,get_parent(),"power_C");
		link_property(current_A,get_parent(),"current_A");
		link_property(current_B,get_parent(),"current_B");
		link_property(current_C,get_parent(),"current_C");
		link_property(shunt_A,get_parent(),"shunt_A");
		link_property(shunt_B,get_parent(),"shunt_B");
		link_property(shunt_C,get_parent(),"shunt_C");
		link_property(nominal_voltage,get_parent(),"nominal_voltage");
	}
	if ( ! voltage_A ) voltage_A = &default_nominal_voltage_A;
	if ( ! voltage_B ) voltage_B = &default_nominal_voltage_B;
	if ( ! voltage_C ) voltage_C = &default_nominal_voltage_C;
	if ( ! nominal_voltage) nominal_voltage = &default_nominal_voltage;
	if ( weather )
	{
		link_property(temperature, weather, temperature_variable_name);
		link_property(solar, weather, solargain_variable_name);
	}
	if ( tariff )
	{
		link_property(price, tariff, price_variable_name);
	}
	if ( occupants != NULL )
	{
		link_property(occupancy, occupants, occupancy_variable_name);
	}
	if ( ! data )
	{
		exception("filename not specified");
	}
	return 1; 
}

TIMESTAMP ceus::presync(TIMESTAMP t1)
{
	// TODO: this is not ideal, but until node clears the accumulators itself, it has to be done here instead
	complex P(0,0,J);
	*power_A = P;
	*power_B = P;
	*power_C = P;
	complex I(0,0,J);
	*current_A = I;
	*current_B = I;
	*current_C = I;
	complex S(0,0,J);
	*shunt_A = S;
	*shunt_B = S;
	*shunt_C = S;
	return TS_NEVER;
}
double ceus::apply_sensitivity(SENSITIVITY &component, double *variable)
{
	if ( variable == NULL || component.slope == 0.0 )
		return ( component.range.min + component.range.max ) / 2.0; 
	double dir = ( component.slope > 0.0 ? +1.0 : -1.0 );
	double x = *variable;
	if ( component.domain.min < component.domain.max ) // domain is valid
	{
		if ( x * dir < component.domain.min * dir ) // below domain min
			x = component.domain.min;
		else if ( x * dir > component.domain.max * dir ) // above domain max
			x = component.domain.max;
	}
	double y = ( x - component.base ) * component.slope + component.intercept;
	if ( component.range.min < component.range.max )
	{
		if ( y < component.range.min )
			return component.range.min;
		else if ( y > component.range.max )
			return component.range.max;
	}
	return y;
}
TIMESTAMP ceus::sync(TIMESTAMP t1)
{
	double Pr = 0.0, Pi = 0.0;
	double Ir = 0.0, Ii = 0.0;
	double Zr = 0.0, Zi = 0.0;
	total_power_A = total_power_B = total_power_C = complex(0,0,J);
	CEUSDATA *enduse;
	for ( enduse = data ; enduse != NULL ; enduse = get_next_enduse(enduse) )
	{
		COMPONENT *c;
		double load = get_value(enduse,gl_globalclock,floor_area)/3.0;
		for ( c = get_first_component() ; c != NULL ; c = get_next_component(c) )
		{
			double scalar = load * c->fraction / 3.0 ;
			scalar += apply_sensitivity(c->cooling,temperature);
			scalar += apply_sensitivity(c->heating,temperature);
			scalar += apply_sensitivity(c->solar,solar);
			scalar += apply_sensitivity(c->price,price);
			scalar += apply_sensitivity(c->occupancy,occupancy);
			Pr += c->Pr * scalar;
			Ir += c->Ir * scalar;
			Zr += c->Zr * scalar;
			Pi += c->Pi * scalar;
			Ii += c->Ii * scalar;
			Zi += c->Zi * scalar;
		}
	}
	complex P(Pr,Pi,J);
	*power_A += P;
	*power_B += P;
	*power_C += P;
	complex I(Ir,Ii,J);
	*current_A += I;
	*current_B += I;
	*current_C += I;
	complex S = complex(1,0,J)/complex(Zr,Zi,J);
	*shunt_A += S;
	*shunt_B += S;
	*shunt_C += S;
	total_power_A = ((*voltage_A/(*nominal_voltage)*S + I)*(*voltage_A)/(*nominal_voltage) + P);
	total_power_B = ((*voltage_B/(*nominal_voltage)*S + I)*(*voltage_B)/(*nominal_voltage) + P);
	total_power_C = ((*voltage_C/(*nominal_voltage)*S + I)*(*voltage_C)/(*nominal_voltage) + P);
	total_real_power = total_power_A.Re() + total_power_B.Re() + total_power_C.Re();
	total_reactive_power = total_power_A.Im() + total_power_B.Im() + total_power_C.Im();
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
		return 1; 
	}
	else // write
	{
		// TODO
		return 0;
	}
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
		const char *name;
		const char *format;
		DATATYPE type;
		union {
			char string[32];
			int integer;
			double real;
		} buffer;
		CEUSDATA *data;
	} map[MAXDATA];
	size_t max_column = 0;
	memset(map,0,sizeof(map));
	size_t daytype_ndx = 0;
	size_t enduse_ndx = 0;
	while ( (item=strtok_r(last?NULL:header,",\r\n",&last)) != NULL )
	{
		if ( max_column >= MAXDATA )
		{
			error("too many columns of data in file '%s' (maximum is %d)",(const char*)filename, MAXDATA);
			fclose(fp);
			return false;
		}
		map[max_column].name = strdup(item);
		if ( strcmp(item,default_month_heading)==0 )
		{
			map[max_column].type = DT_INTEGER;
			map[max_column].format = "%d";
		}
		else if ( strcmp(item,default_daytype_heading)==0 )
		{
			map[max_column].type = DT_STRING;
			map[max_column].format = "%s";
			daytype_ndx = max_column;
		}
		else if ( strcmp(item,default_hour_heading)==0 )
		{
			map[max_column].type = DT_INTEGER;
			map[max_column].format = "%d";
		}
		else // enduse
		{
			map[max_column].type = DT_REAL;
			map[max_column].format = "%lg";
			if ( enduse_ndx == 0 )
				enduse_ndx = max_column;
			map[max_column].data = add_enduse(data,item);
		}
		max_column++;
		if ( last == NULL ) 
		{
			break;
		}
	}
	debug("%s: found %d columns", filename, max_column);

	// load records
	char line[1024];
	size_t count = 0;
	struct {
		char *label;
		DAYTYPE code;
	} codes[] = {
		{default_weekday_code,  DT_WEEKDAY},
		{default_saturday_code, DT_SATURDAY},
		{default_sunday_code,   DT_SUNDAY},
		{default_holiday_code,  DT_HOLIDAY},
	};
	while ( fgets(line,sizeof(line),fp) != NULL )
	{
		last = NULL;
		size_t column = 0;
		while ( (item=strtok_r(last?NULL:line,",\r\n",&last)) != NULL)
		{
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
			if ( last == NULL ) 
			{
				break;
			}
		}
		size_t n;
		for ( n = 0 ; n < sizeof(codes)/sizeof(codes[0]) ; n++ )
		{
			if ( strcmp(codes[n].label,map[daytype_ndx].buffer.string) == 0 )
			{
				break;
			}
		}
		if ( n == _DT_SIZE )
		{
			error("%s[%d,%d] -- '%s' is not a valid daytype code (must one of ceus::default_{weekday,saturday,sunday,holiday}_code globals)", (const char*)filename, count+1, column+1, map[daytype_ndx].buffer.string);
			fclose(fp);
			return 0;
		}
		if ( count >= DATASIZE )
		{
			error("ignore extra data in '%s' after '%s'",(const char*)filename,line);
			fclose(fp);
		}
		// unsigned int month = map[month_ndx].buffer.integer;
		// unsigned int hour = map[hour_ndx].buffer.integer;
		for ( n = enduse_ndx ; n < column ; n++ )
		{
			map[n].data->data[count] = map[n].buffer.real;
		}
		count++;
	}
	if ( count != DATASIZE )
	{
		error("missing data in CEUS file '%s' (only %d records found, expected %d",(const char*)filename,count,DATASIZE);
	}
	else
	{
		verbose("%d records loaded from file '%s'", count, (const char*)filename);
	}
	fclose(fp);
	return 1;
}
