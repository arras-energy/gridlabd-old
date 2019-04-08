/** $Id: match.h 4738 2014-07-03 00:55:39Z dchassin $
	Copyright (C) 2008 Battelle Memorial Institute
**/

#ifndef GLD_MATCH
#define GLD_MATCH

#ifdef __cplusplus
extern "C" {
#endif

int match(const char *regexp, const char *text);
int matchhere(const char *regexp, const char *text);
int matchstar(int c, const char *regexp, const char *text);

#ifdef __cplusplus
}
#endif

#endif

/* EOF */
