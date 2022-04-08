/* File: environment.h 
 * Copyright (C) 2008, Battelle Memorial Institute

	@file environment.h
	@addtogroup environment
	@ingroup core
@{
 **/

#ifndef _ENVIRONMENT_H
#define _ENVIRONMENT_H

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

#include "globals.h"
#include "output.h"

#ifdef __cplusplus 
extern "C" {
#endif

/*	Function: environment_start

	Parameters
	argc - argument count
	argv - argument values

	Return:
	SUCCESS - environment run ok
	FAILURE - environment run failed
 */
STATUS environment_start(int argc,const char *argv[]);

#ifdef __cplusplus 
}
#endif

#endif

/**@}*/
