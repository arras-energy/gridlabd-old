/* File: daemon.h 
 * Copyright (C) 2008, Battelle Memorial Institute
 */

#ifndef _DAEMON_H
#define _DAEMON_H

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*	Function: daemon_start
	
	This function starts the local daemon
 */
DEPRECATED int daemon_start(int argc, const char *argv[]);

/*	Function: daemon_stop
	
	This function stops the local daemon
 */
DEPRECATED int daemon_stop(int argc, const char *argv[]);

/*	Function: daemon_restart
	
	This function restarts the local daemon
 */
DEPRECATED int daemon_restart(int argc, const char *argv[]);

/*	Function: daemon_status
	
	This function gets the status of the local daemon
 */
DEPRECATED int daemon_status(int argc, const char *argv[]);

/*	Function: daemon_remote_client
	
	This function start a local client to a remote daemon
 */
DEPRECATED int daemon_remote_client(int argc, const char *argv[]);

#ifdef __cplusplus
}
#endif

#endif
