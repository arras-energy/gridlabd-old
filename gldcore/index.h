/* File: index.h 
 * Copyright (C) 2008, Battelle Memorial Institute

	@file index.h
	@addtogroup index
	@ingroup core
@{
 **/

#ifndef _INDEX_H
#define _INDEX_H

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

#include "globals.h"
#include "list.h"

typedef struct s_index {
	unsigned int id;	/**< the index id */
	GLLIST **ordinal;		/**< the list of ordinals */
	int first_ordinal;	/**< the first ordinal in the list */
	int last_ordinal;	/**< the last ordinal in the list */
	int last_used;		/**< the last ordinal in use */
	int first_used;		/**< the first ordinal in use */
} INDEX;	/**< the index structure */

#ifdef __cplusplus
extern "C" {
#endif

INDEX *index_create(int first_ordinal, int last_ordinal);
STATUS index_insert(INDEX *index, void *data, int ordinal);
void index_shuffle(INDEX *index);

#ifdef __cplusplus
}
#endif

#endif

/**@}*/
