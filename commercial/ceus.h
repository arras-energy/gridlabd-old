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

#define DATASIZE (12*_DT_SIZE*24) // N_months x N_daytypes x N_hours (e.g., 1152)

class ceus : public gld_object 
{
public: // globals
	static complex default_nominal_voltage_A;
	static complex default_nominal_voltage_B;
	static complex default_nominal_voltage_C;
	static double default_nominal_voltage;
	static char32 default_month_heading;
	static char32 default_daytype_heading;
	static char32 default_hour_heading;
	static char32 default_weekday_code;
	static char32 default_saturday_code;
	static char32 default_sunday_code;
	static char32 default_holiday_code;
	static char1024 temperature_variable_name;
	static char1024 solargain_variable_name;
	static char1024 price_variable_name;
	static char1024 occupancy_variable_name;
	static double default_temperature_heating_balance;
	static double default_temperature_cooling_balance;
	static double default_temperature_heating_base;
	static double default_temperature_cooling_base;
	static double default_temperature_heating_design;
	static double default_temperature_cooling_design;
	static double default_solargain_base;
	static double default_price_base;
	static double default_occupancy_base;
public:
	typedef enum {
		DT_WEEKDAY = 0,
		DT_SATURDAY = 1,
		DT_SUNDAY = 2,
		DT_HOLIDAY = 3,
		_DT_SIZE
	} DAYTYPE;
public:
	typedef struct s_ceusdata 
	{
		char *filename;
		char *enduse;
		double data[DATASIZE];
		struct s_ceusdata *next_file;
		struct s_ceusdata *next_enduse;
	} CEUSDATA;
	static CEUSDATA *repository;
public:
	static CEUSDATA *add_file(const char *filename);
	static CEUSDATA *get_first_file(void);
	inline static CEUSDATA *get_next_file(CEUSDATA *repo);
	static CEUSDATA *find_file(const char *filename);
	static CEUSDATA *add_enduse(const char *filename, const char *enduse);
	static CEUSDATA *add_enduse(CEUSDATA *repo, const char *enduse);
	static CEUSDATA *get_first_enduse(const char *filename);
	inline static CEUSDATA *get_next_enduse(CEUSDATA *repo);
	static CEUSDATA *find_enduse(const char *filename, const char *enduse);
	static CEUSDATA *find_enduse(CEUSDATA *repo, const char *enduse);
	static size_t get_index(unsigned int month, unsigned int day, unsigned int hour);
	static size_t get_index(TIMESTAMP ts);
	static size_t get_index(void);
	void set_value(CEUSDATA *repo, TIMESTAMP ts, double value);
	static double get_value(CEUSDATA *repo, TIMESTAMP ts, double scalar=1.0);
public:
	typedef struct s_minmax {
		double min;
		double max;
	} MINMAX;
	typedef struct s_sensitivity {
		double base; // variable basis value
		double intercept; // intercept value
		double slope; // slope value
		MINMAX domain; // domain (input min/max)
		MINMAX range; // range (output min/max)
	} SENSITIVITY;
	typedef struct s_component 
	{
		CEUSDATA *data; // enduse name
		double Zr, Zi; // constant impedance factors (real, imaginary)
		double Ir, Ii; // constant current factors (real, imaginary)
		double Pr, Pi; // constant power factors (real, imaginary)
		double fraction; // fraction of total floor area affected by this component
		SENSITIVITY heating; // heating temperature sensitivity
		SENSITIVITY cooling; // cooling temperature sensitivity
		SENSITIVITY solar; // solar gain sensitivity
		SENSITIVITY price; // price sensitivity
		SENSITIVITY occupancy; // occupancy sensitivity
		struct s_component *next;
	} COMPONENT;
	COMPONENT *components;
public:
	COMPONENT *get_first_component();
	inline COMPONENT *get_next_component(COMPONENT *c);
	COMPONENT *add_component(const char *enduse, const char* composition=NULL);
	bool set_component(const char *enduse, const char *term, double value);
	bool set_component(COMPONENT *component, const char *term, double value);
	COMPONENT *find_component(const char *enduse);
	double apply_sensitivity(SENSITIVITY &component, double *variable);
public:
	GL_ATOMIC(char32,building_type);
	GL_ATOMIC(double,floor_area);
	GL_ATOMIC(object,weather); // source of temperature and solar sensitivity variables
	GL_ATOMIC(object,tariff); // source of price variable
	GL_ATOMIC(object,occupants); // source of occupancy variable
	GL_ATOMIC(complex,total_power_A);
	GL_ATOMIC(complex,total_power_B);
	GL_ATOMIC(complex,total_power_C);
	GL_ATOMIC(double,total_real_power);
	GL_ATOMIC(double,total_reactive_power);
	GL_METHOD(ceus,composition);
private:
	template<class T> void link_property(T *&ptr, gld_object *obj, const char *name)
	{
		if ( obj ) link_property(ptr,obj->my(),name);
	}
	template<class T> void link_property(T *&ptr, gld_object &obj, const char *name)
	{
		link_property(ptr,obj.my(),name);
	}
	template<class T> void link_property(T *&ptr, OBJECT *obj, const char *name, bool no_exception = true)
	{
		if ( ! obj ) return;
		gld_property prop(obj,name);
		if ( prop.is_valid() )
			ptr = (T*)prop.get_addr();
		else if ( ! no_exception )
			exception("unable to link property '%s' in object '%s'",name,get_object(obj)->get_name());
		else
			warning("unable to link property '%s' in object '%s'",name,get_object(obj)->get_name());
	}
private:
	complex *voltage_A;
	complex *voltage_B;
	complex *voltage_C;
	double *nominal_voltage;
private:
	double *temperature;
	double *price;
	double *solar;
	double *occupancy;
	CEUSDATA *data;
private:
	complex *power_A;
	complex *power_B;
	complex *power_C;
	complex *current_A;
	complex *current_B;
	complex *current_C;
	complex *shunt_A;
	complex *shunt_B;
	complex *shunt_C;
public:
	ceus(MODULE *module);
	int create(void);
	int init(OBJECT *parent);
	TIMESTAMP presync(TIMESTAMP t1);
	TIMESTAMP sync(TIMESTAMP t1);
	TIMESTAMP postsync(TIMESTAMP t1);
public:
	int filename(const char*);
public:
	static CLASS *oclass;
	static ceus *defaults;
};

#endif // _CEUS_H
