/*	File: index.h 
 	Copyright (C) 2008, Battelle Memorial Institute
 */

#ifndef _INDEX_H
#define _INDEX_H

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

#include "globals.h"
#include "list.h"

/*	Structure: s_index
	unsigned int id - the index id
	GLLIST **ordinal - the list of ordinals
	int first_ordinal - the first ordinal in the list
	int last_ordinal - the last ordinal in the list
	int last_used - the last ordinal in use
	int first_used - the first ordinal in use
 */
struct s_index 
{
	unsigned int id;	/**< the index id */
	GLLIST **ordinal;		/**< the list of ordinals */
	int first_ordinal;	/**< the first ordinal in the list */
	int last_ordinal;	/**< the last ordinal in the list */
	int last_used;		/**< the last ordinal in use */
	int first_used;		/**< the first ordinal in use */
};

// Typedef: INDEX
// See <s_index>
typedef struct s_index INDEX;	/**< the index structure */

#ifdef __cplusplus
extern "C" {
#endif

// Function: index_create
DEPRECATED INDEX *index_create(int first_ordinal, int last_ordinal);

// Function: index_insert
DEPRECATED STATUS index_insert(INDEX *index, void *data, int ordinal);

// Function: index_shuffle
DEPRECATED void index_shuffle(INDEX *index);

#ifdef __cplusplus
}

class GldExec;

// Class: GldIndex
class GldIndex
{
private:
	GldExec *exec;
public:

	// Constructor: GldIndex
	GldIndex(GldExec *ptr);

	// Destructor: ~GldIndex
	~GldIndex(void);
};

#endif

#endif

/**@}*/
