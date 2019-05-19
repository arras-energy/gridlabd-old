// daemon.h

#ifndef _DAEMON_H
#define _DAEMON_H

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

int daemon_start(int argc, const char *argv[]);
int daemon_stop(int argc, const char *argv[]);
int daemon_restart(int argc, const char *argv[]);
int daemon_status(int argc, const char *argv[]);
int daemon_remote_client(int argc, const char *argv[]);

#ifdef __cplusplus
}
#endif

#endif
