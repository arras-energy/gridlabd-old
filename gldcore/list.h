/** $Id: list.h 4738 2014-07-03 00:55:39Z dchassin $
	Copyright (C) 2008 Battelle Memorial Institute
	@file list.h
	@addtogroup list
	@ingroup core
@{
 **/

#ifndef _LIST_H
#define _LIST_H

#ifndef _GLDCORE_H
#error "this header may only be included from gldcore.h"
#endif

typedef struct s_listitem {
	void *data;
	struct s_listitem *prev;
	struct s_listitem *next;
} LISTITEM;

typedef struct s_list {
	unsigned int size;
	LISTITEM *first;
	LISTITEM *last;
} GLLIST;

#ifdef __cplusplus
extern "C" {
#endif

GLLIST *list_create(void);
void list_destroy(GLLIST *list);
LISTITEM *list_append(GLLIST *list, void *data);
void list_shuffle(GLLIST *list);

#ifdef __cplusplus
}
#endif
	
#endif

/**@}*/
