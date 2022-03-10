/* 	File: gld_sock.h 
 	Copyright (C) 2008, Battelle Memorial Institute
 */

#ifndef _GLD_SOCKETS_H_
#define _GLD_SOCKETS_H_

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

#ifdef WIN32

#include <winsock2.h>

#else

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/errno.h>

/*	Define: SOCKET

	Type definition for general socket identifier
 */
#define SOCKET int

/*	Define: INVALID_SOCKET

	Return value for socket error
 */
#define INVALID_SOCKET (-1)

/*	Define: closesocket

	Alternate definition of socket close function
 */
#define closesocket close

#endif

#include <memory.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

#endif

// EOF
