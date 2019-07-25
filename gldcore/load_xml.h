/* File: load_xml.h 
 * Copyright (C) 2008, Battelle Memorial Institute
 * 
 * Authors:
 *	Matthew Hauer <matthew.hauer@pnl.gov>, 6 Nov 07 -
 *
 * Versions:
 *	1.0 - MH - initial version
 *
 * Credits:
 *	adapted from SAX2Print.h
 *
 *	@file load_xml.h
 *	@addtogroup load XML file loader
 *	@ingroup core
 *
 */

#ifndef _XML_LOAD_H_
#define _XML_LOAD_H_

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
	#include "output.h"

	int loadall_xml(char *file);
}
#else
int loadall_xml(char *file);
#endif

#endif

/* EOF */
