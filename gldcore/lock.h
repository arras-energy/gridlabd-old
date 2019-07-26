/* File: lock.h 
 * Copyright (C) 2008, Battelle Memorial Institute

 @{
 **/

#ifndef _LOCK_H
#define _LOCK_H

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

#include "platform.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>

typedef int LOCKVAR;

void rlock(LOCKVAR *lock);
void wlock(LOCKVAR *lock);
void runlock(LOCKVAR *lock);
void wunlock(LOCKVAR *lock);

void register_lock(const char *name, LOCKVAR *lock);

#ifdef __cplusplus
}
#endif

#endif /* _LOCK_H */

/**@}**/

