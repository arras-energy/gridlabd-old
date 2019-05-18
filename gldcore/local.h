/* local.h
 *	Copyright (C) 2008 Battelle Memorial Institute
 */

#ifndef _LOCAL_H
#define _LOCAL_H

#ifndef _GLDCORE_H
#error "this header may only be included from gldcore.h"
#endif

#include <time.h>

typedef struct s_locale {
	char tz[32];
	struct s_locale *next;
} LOCALE;

#ifdef __cplusplus 
extern "C" {
#endif

void locale_push(void);
void locale_pop(void);

#ifdef __cplusplus
}
#endif

#endif /* _LOCAL_H */

