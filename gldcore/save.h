/* save.h
 * 	Copyright (C) 2008 Battelle Memorial Institute
 */

#ifndef _SAVE_H
#define _SAVE_H

#ifndef _GLDCORE_H
#error "this header may only be included from gldcore.h"
#endif

#include "globals.h"
#include "module.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

int saveall(const char *);

#ifdef __cplusplus
}
#endif // __cplusplus
	
#endif // _SAVE_H

