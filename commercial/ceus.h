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
	static complex default_nominal_voltage_A;
	static complex default_nominal_voltage_B;
	static complex default_nominal_voltage_C;
	static double default_nominal_voltage;
public:
	typedef struct s_ceusdata 
	{
		char *filename;
		char *enduse;
		double data[2016];
		struct s_ceusdata *next_file;
		struct s_ceusdata *next_enduse;
	} CEUSDATA;
	static CEUSDATA *repository;
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
	CEUSDATA *data;
public:
	typedef struct s_component 
	{
		CEUSDATA *data; // enduse name
		double Zr, Zi; // constant impedance factors (real, imaginary)
		double Ir, Ii; // constant current factors (real, imaginary)
		double Pr, Pi; // constant power factors (real, imaginary)
		double fraction; // fraction of total floor area affected by this component
		double Tn, Tb, Tm; // nominal, balance, and min/max temperatures
		double Ts; // temperature sensitivity
		double E[24]; // price elasticities
		struct s_component *next;
	} COMPONENT;
	COMPONENT *components;
	COMPONENT *get_first_component();
	inline COMPONENT *get_next_component(COMPONENT *c);
	COMPONENT *add_component(const char *enduse, const char* composition=NULL);
	bool set_component(const char *enduse, const char *term, double value);
	bool set_component(COMPONENT *component, const char *term, double value);
	COMPONENT *find_component(const char *enduse);
public:
	GL_ATOMIC(char32,building_type);
	GL_ATOMIC(double,floor_area);
	GL_ATOMIC(object,weather);
	GL_ATOMIC(complex,total_power_A);
	GL_ATOMIC(complex,total_power_B);
	GL_ATOMIC(complex,total_power_C);
	GL_ATOMIC(double,total_real_power);
	GL_ATOMIC(double,total_reactive_power);
	GL_METHOD(ceus,composition);
private:
	template<class T> void link_property(T *&ptr, gld_object *obj, char *name)
	{
		return link_property(ptr,obj->my(),name);
	}
	template<class T> void link_property(T *&ptr, gld_object &obj, char *name)
	{
		return link_property(ptr,obj.my(),name);
	}
	template<class T> void link_property(T *&ptr, OBJECT *obj, char *name)
	{
		gld_property prop(obj,name);
		if ( prop.is_valid() )
			ptr = (T*)prop.get_addr();
		else
			exception("unable to link property '%s' in object '%s'",name,get_object(obj)->get_name());
	}
	complex *voltage_A;
	complex *voltage_B;
	complex *voltage_C;
	double *nominal_voltage;
	double *temperature;
	double *price;
	double *price_base;
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
