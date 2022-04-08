/* File: kill.h 
 * Copyright (C) 2008, Battelle Memorial Institute
 */
#ifndef _KILL_H
#define _KILL_H

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

#ifdef WIN32
#ifdef __MINGW32__
#include <sys/types.h>
#endif
void kill_starthandler(void);
void kill_stophandler(void);
int kill(pid_t pid, int sig);
#endif

#endif
