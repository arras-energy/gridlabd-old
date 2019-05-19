/* $Id: ufile.h 4738 2014-07-03 00:55:39Z dchassin $
 * Copyright (C) 2012 Battelle Memorial Institute
 */

#ifndef _UFILE_H
#define _UFILE_H

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

typedef struct s_ufile {
	enum {UFT_NONE, UFT_FILE, UFT_HTTP} type;
	void *handle;
} UFILE;

UFILE *uopen(const char *fname, void *arg);
size_t uread(void *buffer, size_t count, UFILE *rp);

#endif
