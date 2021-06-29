/* File: version.h 
   Copyright (C) 2008, Pacific Northwest National Laboratory
 */

#ifndef _GRIDLABD_VERSION_H
#define _GRIDLABD_VERSION_H

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

const char *version_copyright(void);
unsigned int version_major(void);
unsigned int version_minor(void);
unsigned int version_patch(void);
unsigned int version_build(void);
const char *version_branch(void);
bool version_check(const char *expression);

#ifdef __cplusplus
}
#endif

#endif

// EOF
