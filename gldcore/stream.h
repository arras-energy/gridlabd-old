/* File: stream.h 
 * Copyright (C) 2008, Battelle Memorial Institute
 */

#ifndef _STREAM_H
#define _STREAM_H

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

#include <stdio.h>
#include "platform.h"
#include "class.h"
#include "object.h"

/* stream flags */
#define SF_IN		0x0001
#define SF_OUT		0x0002
#define SF_STR		0x0004

typedef const char *TOKEN;
typedef unsigned int uint;
typedef unsigned char uchar;

#ifdef __cplusplus

#ifndef STREAM_MODULE
size_t stream(void *ptr,size_t len,bool is_str=false,void *match=NULL);
#endif

extern "C" {
#endif

typedef size_t (*STREAMCALLBACK)(void*,size_t,bool,void*);

#ifndef STREAM_MODULE
typedef size_t (*STREAMCALL)(int flags,STREAMCALLBACK call);
void stream_register(STREAMCALL);
size_t stream(FILE *fp, int flags);
char* stream_context();
#endif

#define stream_type(T) size_t stream_##T(void*,size_t,PROPERTY*p)
#include "stream_type.h"
#undef stream_type

#ifdef __cplusplus
}

#endif


#endif
