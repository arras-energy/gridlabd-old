/* File: load.h 
 * Copyright (C) 2008, Battelle Memorial Institute

	@file load.h
	@addtogroup load_glm
	@ingroup core
 @{
 **/

#ifndef _LOAD_H
#define _LOAD_H

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

#include "globals.h"
#include "module.h"

#include "load_xml.h"

#define UR_NONE  0x00 /* no flags */
#define UR_RANKS 0x01 /* reference has ranking impact */
#define UR_TRANSFORM 0x02 /* reference is via a transform */

#ifdef __cplusplus
extern "C" {
#endif
STATUS loadall(const char *filename);
#ifdef __cplusplus
}
#endif

typedef struct s_unresolved {
	OBJECT *by;
	PROPERTYTYPE ptype;
	void *ref;
	int flags;
	CLASS *oclass;
	const char *id;
	const char *file;
	unsigned int line;
	struct s_unresolved *next;
} UNRESOLVED;

typedef struct s_unresolved_func {
	char1024 funcstr;
	OBJECT *obj;
	double *targ;
	unsigned int line;
	struct s_unresolved_func *next;
} UNR_FUNC;

typedef struct s_unresolved_static {
	char256	member_name;
	char256 class_name;
	struct s_unresolved_static *next;
} UNR_STATIC;

typedef struct s_languagemap LANGUAGE;
// set loader language
// returns TRUE on success, FALSE on failure
struct s_languagemap {
	const char *name;
	bool (*parser)(const char *buffer);
	struct s_languagemap *next;
};

#ifdef __cplusplus
extern "C" {
#endif

STATUS load_set_index(OBJECT *obj, OBJECTNUM id);
OBJECT *load_get_index(OBJECTNUM id);
double load_latitude(char *buffer);
double load_longitude(char *buffer);
int time_value(char *, TIMESTAMP *t);
int time_value_datetime(char *c, TIMESTAMP *t);
int time_value_datetimezone(char *c, TIMESTAMP *t);
int set_flags(OBJECT *obj, char *propval);
UNRESOLVED *add_unresolved(OBJECT *by, PROPERTYTYPE ptype, void *ref, CLASS *oclass, char *id, char *file, unsigned int line, int flags);
STATUS load_resolve_all();
OBJECT *load_get_current_object(void);
MODULE *load_get_current_module(void);
void load_add_language(const char *name, bool (*parser)(const char*));

#ifdef __cplusplus
}
#endif

#endif

/**@}*/
