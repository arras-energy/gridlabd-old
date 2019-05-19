/* File: job.h
 * Copyright (C) 2008, Battelle Memorial Institute
 */

#ifndef _JOB_H
#define _JOB_H

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

#include "platform.h"

typedef enum {
	JO_NONE		= 0x0000,	///< no job options
	JO_SORTA	= 0x0001,	///< run jobs in alphabetical order
	JO_SORTN	= 0x0002,	///< run jobs in numeric order
	JO_SUBDIR	= 0x0004,	///< run jobs in subdirectories
	JO_CLEAN	= 0x0008,	///< clean subdirectories
	JO_DEFAULT	= JO_SORTA,	///< default job options
} JOBOPTIONS;

int job(void *main, int argc, const char *argv[]);

#endif

