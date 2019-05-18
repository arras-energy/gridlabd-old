/* legal.h
	Copyright (C) 2008 Battelle Memorial Institute
 *
 */

#ifndef _LEGAL_H
#define _LEGAL_H

#ifndef _GLDCORE_H
#error "this header may only be included from gldcore.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

STATUS legal_notice(void);
STATUS legal_license(void);

void check_version(int mt);

#ifdef __cplusplus
}
#endif

#endif
