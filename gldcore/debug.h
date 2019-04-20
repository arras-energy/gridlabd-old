/** $Id: debug.h 4738 2014-07-03 00:55:39Z dchassin $
	Copyright (C) 2008 Battelle Memorial Institute
	@file debug.h
	@version $Id: debug.h 4738 2014-07-03 00:55:39Z dchassin $
	@addtogroup debug
	@ingroup core
 @{
 **/

#ifndef _DEBUG_H
#define _DEBUG_H

#ifdef __cplusplus
extern "C" {
#endif

void exec_sighandler(int sig);
int exec_debug(struct sync_data *data, int pass, int index, OBJECT *obj);
char *strsignal(int sig);

#ifdef __cplusplus
}
#endif

#endif
/**@}*/
