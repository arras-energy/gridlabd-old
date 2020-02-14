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

OBJECT *load_get_current_object(void);
MODULE *load_get_current_module(void);

#ifdef __cplusplus
}
#endif

#endif

/**@}*/
