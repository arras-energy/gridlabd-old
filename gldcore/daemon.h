// daemon.h

#ifndef _DAEMON_H
#define _DAEMON_H

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
