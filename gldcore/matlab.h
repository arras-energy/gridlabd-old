/* File: matlab.h 
 * Copyright (C) 2008, Battelle Memorial Institute

	Copyright (C) 2008 Battelle Memorial Institute
	@file matlab.h
	@addtogroup matlab
 @{
 **/

#ifndef _MATLAB_H
#define _MATLAB_H

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

#include "globals.h"

STATUS matlab_startup(int argc, const char *argv[]);

MODULE *load_matlab_module(const char *file, int argc, const char *argv[]);

#ifdef __cplusplus
extern "C" {
#endif

/** @todo move java and python modules to their own implementation files (ticket #121) */
MODULE *load_java_module(const char *file, int argc, const char *argv[]);
MODULE *load_python_module(const char *file, int argc, const char *argv[]);

#ifdef __cplusplus
}
#endif

#endif

/**@}*/
