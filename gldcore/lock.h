/** $Id: lock.h 4738 2014-07-03 00:55:39Z dchassin $
 @{
 **/

#ifndef _LOCK_H
#define _LOCK_H

#ifndef _GLDCORE_H
#error "this header may only be included from gldcore.h"
#endif

#include "platform.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>

typedef int64_t LOCKVAR;
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

