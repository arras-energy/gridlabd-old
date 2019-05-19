/* File: version.h 
   Copyright (C) 2008, Pacific Northwest National Laboratory
 */

#ifndef _GRIDLABD_VERSION_H
#define _GRIDLABD_VERSION_H

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

#define REV_MAJOR 4
#define REV_MINOR 2
#define REV_PATCH 0

#ifdef HAVE_CONFIG_H
#include "config.h"
#else
#define _V(X,Y,Z) #X"."#Y"."#Z
#define PACKAGE "gridlabd"
#define PACKAGE_NAME "GridLAB-D"
#define PACKAGE_VERSION _V(REV_MAJOR,REV_MINOR,REV_PATCH)
#define PACKAGE_STRING PACKAGE_NAME" "PACKAGE_VERSION
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

#ifdef __cplusplus
}
#endif

#endif

// EOF
