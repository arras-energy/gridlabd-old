/* File: setup.h 
 * Copyright (C) 2008, Battelle Memorial Institute
 */

#ifndef _SETUP_H
#define _SETUP_H

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

int setup(void *main, int argc, const char *argv[]);

#endif
