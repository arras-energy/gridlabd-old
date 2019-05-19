/* test.h
	Copyright (C) 2008 Battelle Memorial Institute
 *
 */

#ifndef _TEST_H
#define _TEST_H

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

typedef int (*TESTFUNCTION)(void);

#ifdef __cplusplus
extern "C" {
#endif

/* general testing API */
int test_register(const char *name, TESTFUNCTION call);
int test_request(const char *name);
STATUS test_exec(void);

int test_lock(void);

#ifdef __cplusplus
}
#endif

#endif
