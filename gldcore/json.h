/* File: json.h 
 * Copyright (C) 2018, Regents of the Leland Stanford Junior University

	@file json.h
	@addtogroup json
 @{
 **/

#ifndef _JSON_H
#define _JSON_H

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

#include "platform.h"
#include <list>
#include <string>

DEPRECATED CDECL int json_dump(const char *filename);
DEPRECATED CDECL int json_output(FILE *fp);
DEPRECATED CDECL int convert_to_json(const char *s, void *data, PROPERTY *p);
DEPRECATED CDECL int convert_from_json(char *buffer, int len, void *data, PROPERTY *p);
DEPRECATED CDECL int json_create(void *ptr);
DEPRECATED CDECL double json_get_part(void *c, const char *name);
DEPRECATED CDECL int json_set_part(void *c, const char *name, const char *value);

enum e_jsontype {JSON_NULL, JSON_FALSE, JSON_TRUE, JSON_NUMBER, JSON_STRING, JSON_ARRAY, JSON_OBJECT};	
typedef enum e_jsontype JSONTYPE;
class GldJson 
{
private:
	const JSONTYPE type;
	size_t sz;
	size_t refs;
	union {
		std::list<GldJson> *obj;
		double num;
		struct s_strbuf {
			size_t len;
			char *buf;
		} str;
	} data;
public:
	inline GldJson(GldJson &j) : type(j.type) {refs++; }; // TODO: copy constructor
	inline GldJson(JSONTYPE t = JSON_NULL) : type(t) {};
	inline GldJson(double x) : type(JSON_NUMBER) { data.num = x; };
	inline GldJson(const char *x) : type(JSON_STRING) { data.str.buf = strdup(x); data.str.len = strlen(x); };
	inline GldJson(std::list<GldJson> *x) : type(JSON_OBJECT) { data.obj = x; };
	inline ~GldJson(void) {if (--refs==0) set_data(); };
	// get accessors
	inline JSONTYPE get_type(void) const { return type; };
	// set accessors
	void set_data(void);
	void set_data(double x);
	void set_data(const char *x, size_t sz = 0);
	void set_data(GldJson *x);
	inline void append(GldJson &item) { data.obj->push_back(item); };
	inline void append(double x) { data.obj->push_back(*(new GldJson(x))); };
	inline void append(const char *x) { data.obj->push_back(*(new GldJson(x))); };
};

class GldJsonWriter
{
private:
	static unsigned int version;
private:
	const char *filename;
	FILE *json;
public:
	inline const char *get_filename() { return filename; };
public:
	GldJsonWriter(const char *filename);
	~GldJsonWriter(void);
public:
	int dump();
	int write_output(FILE *fp);
private:
	int write_modules(FILE *fp);
	int write_properties(FILE *fp);
	int write_classes(FILE *fp);
	int write_globals(FILE *fp);
	int write_objects(FILE *fp);
	int write_schedules(FILE *fp);
	int write(const char *fmt,...);
};

#endif // _JSON_H
