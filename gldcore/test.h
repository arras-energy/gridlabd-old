/* test.h
	Copyright (C) 2008 Battelle Memorial Institute
 *
 */

#ifndef _TEST_H
#define _TEST_H

typedef int (*TESTFUNCTION)(void);

#ifdef __cplusplus
extern "C" {
#endif

/* general testing API */
int test_register(char *name, TESTFUNCTION call);
int test_request(char *name);
STATUS test_exec(void);

int test_lock(void);

#ifdef __cplusplus
}
#endif

#endif
