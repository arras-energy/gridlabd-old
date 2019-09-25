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
DEPRECATED CDECL int json_to_glm(const char *jsonfile, char *glmfile);

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
