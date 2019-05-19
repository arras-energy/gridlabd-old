/* $Id: server.h 4738 2014-07-03 00:55:39Z dchassin $
 * server.h
 */

#ifndef _SERVER_H
#define _SERVER_H

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

#include "globals.h"

#ifndef WIN32
#include <sys/stat.h>
#else
#include <io.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif
	
STATUS server_startup(int argc, const char *argv[]);
STATUS server_join(void);

#ifndef WIN32
int filelength(int fd);
#endif

#ifdef __cplusplus
}
#endif
	
#endif
