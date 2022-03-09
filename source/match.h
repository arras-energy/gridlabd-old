/* File: match.h 
 * Copyright (C) 2008, Battelle Memorial Institute
 */

#ifndef GLD_MATCH
#define GLD_MATCH

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

#include "platform.h"

DEPRECATED CDECL int match(const char *regexp, const char *text);

#ifdef __cplusplus

class GldRegex 
{
private:
	const char *re;
public:
	GldRegex(const char *regex);
	~GldRegex(void);
public:
	int match(const char *t);
private:
	int match(const char *regexp, const char *text);
	int matchhere(const char *regexp, const char *text);
	int matchstar(int c, const char *regexp, const char *text);
	int matchhere_orig(const char *regexp, const char *text);
};

#endif

#endif

/* EOF */
