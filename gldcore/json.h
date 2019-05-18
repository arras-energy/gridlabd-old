/** $Id: json.h 4738 2014-07-03 00:55:39Z dchassin $
	Copyright (C) 2008 Battelle Memorial Institute
	@file json.h
	@addtogroup json
 @{
 **/

#ifndef _JSON_H
#define _JSON_H

#ifndef _GLDCORE_H
#error "this header may only be included from gldcore.h"
#endif

#include "platform.h"

DEPRECATED CDECL int json_dump(const char *filename);
DEPRECATED CDECL int json_output(FILE *fp);

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
	int write(const char *fmt,...);
};

#endif // _JSON_H
