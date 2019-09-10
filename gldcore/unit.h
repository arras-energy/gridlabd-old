/* File: unit.h 
   Copyright (C) 2008, Battelle Memorial Institute

	@file unit.h
	@addtogroup unit
	@ingroup core
 @{
 **/

#ifndef _UNIT_H
#define _UNIT_H

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

#include "complex.h"

typedef struct s_unit {
	char name[64];		/**< the name of the unit */
	double c,e,h,k,m,s,a,b; /**< the unit parameters */
	int prec; /** the precision of the unit definition */
	struct s_unit *next; /**< the next unit is the unit list */
} UNIT; /**< the UNIT structure */

#ifdef __cplusplus
extern "C" {
#endif

int unit_convert(const char *from, const char *to, double *pValue);
int unit_convert_ex(UNIT *from, UNIT *to, double *pValue);
int unit_convert_complex(UNIT *from, UNIT *to, complex *pValue);
UNIT *unit_find(const char *);
int unit_test(void);

#ifdef __cplusplus
}
#endif

#endif

/** @} **/
