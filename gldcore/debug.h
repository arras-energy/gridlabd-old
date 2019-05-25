/*	File: debug.h 
 	Copyright (C) 2008, Battelle Memorial Institute
 */

#ifndef _DEBUG_H
#define _DEBUG_H

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*	Function: exec_sighandler
	
	This function executes the signal handler for unix signals
 */
void exec_sighandler(int sig);

/*	Function: exec_debug
	
	This function run the main execution loop in debugging mode.
 */
int exec_debug(struct sync_data *data, PASSCONFIG pass, int index, struct s_object_list *obj);

/*	Function: strsignal
	
	This function convert signal numbers to strings
 */
char *strsignal(int sig);

#ifdef __cplusplus
}
#endif

#endif

