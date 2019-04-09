/** $Id: environment.h 4738 2014-07-03 00:55:39Z dchassin $
	Copyright (C) 2008 Battelle Memorial Institute
	@file environment.h
	@addtogroup environment
	@ingroup core
@{
 **/

#ifndef _ENVIRONMENT_H
#define _ENVIRONMENT_H

#include "globals.h"
#include "output.h"

#ifdef __cplusplus 
extern "C" {
#endif

STATUS environment_start(int argc,const char *argv[]);

#ifdef __cplusplus 
}
#endif

#endif

/**@}*/
