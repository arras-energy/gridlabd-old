/* File: linkage.h 
 * Copyright (C) 2008, Battelle Memorial Institute
 */

#ifndef _LINKAGE_H
#define _LINKAGE_H

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

#include "timestamp.h"
#include "property.h"
#include "object.h"

typedef enum {
	LT_UNKNOWN = 0,			 ///<
	LT_MASTERTOSLAVE = 1,	 ///<
	LT_SLAVETOMASTER = 2,	 ///<
} LINKAGETYPE;
typedef struct s_linkage {
	LINKAGETYPE type; ///<
	struct {
		OBJECT *obj; ///<
		PROPERTY *prop; ///<
	} target;
	struct {
		OBJECTNAME obj; ///<
		PROPERTYNAME prop; ///<
	} local,  ///<
	  remote; ///<
	char *addr;  ///< buffer addr in MESSAGE
	size_t size;	 ///< buffer size in MESSAGE
	size_t name_size;
	size_t prop_size;
	struct s_linkage *next; ///<
} linkage; ///<

#endif
