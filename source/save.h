/* File: save.h 
 * Copyright (C) 2008, Battelle Memorial Institute
 */

#ifndef _SAVE_H
#define _SAVE_H

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

#include "globals.h"
#include "module.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

int saveall(const char *);
void save_on_exit(const char *name, const char *options="");
void save_outputs(void);

#ifdef __cplusplus
}
#endif // __cplusplus
	
#endif // _SAVE_H

