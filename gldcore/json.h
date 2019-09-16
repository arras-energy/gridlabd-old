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

DEPRECATED CDECL int json_dump(const char *filename);
DEPRECATED CDECL int json_output(FILE *fp);
DEPRECATED CDECL int convert_to_json(const char *s, void *data, PROPERTY *p);
DEPRECATED CDECL int convert_from_json(char *buffer, int len, void *data, PROPERTY *p);
DEPRECATED CDECL int json_create(void *ptr);
DEPRECATED CDECL double json_get_part(void *c, const char *name);
DEPRECATED CDECL int json_set_part(void *c, const char *name, const char *value);

#ifdef JSON_PARSER_CPP
class GldJson 
{
private:
	const enum e_jsontype {JSON_NULL, JSON_FALSE, JSON_TRUE, JSON_NUMBER, JSON_STRING, JSON_ARRAY, JSON_VALUE, JSON_OBJECT} type;	
	typedef enum e_jsontype JSONTYPE;
	union {
		std::list<GldJson> *val;
		double num;
		string *str;
	} data;
public:
	inline GldJson(JSONTYPE t = JSON_NULL) : type(t) {};
	inline ~GldJson(void) {};
	inline JSONTYPE get_type(void) { return type; };
	inline void set_data(double *x) { data.num = x; };
	inline void set_data(string *x) { data.str = x;};
	inline void set_data(GldJson *x) { data.val = x;};
	inline void append(GldJson &item) { data.push_back(item); };
};
#else
enum e_jsontype {JSON_VALUE, JSON_OBJECT, JSON_ARRAY, JSON_NUMBER, JSON_STRING, JSON_TRUE, JSON_FALSE, JSON_NULL};
typedef enum e_jsontype JSONTYPE;
struct s_jsondata {
	const char *name;
	const char *value;
	// TODO: remove old above and replace with new below
	JSONTYPE type;
	union {
		double number;
		const char *string;
		struct s_jsondata *json;
	} data;
	struct s_jsondata *next;
};
typedef struct s_jsondata JSONDATA;
#endif

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
