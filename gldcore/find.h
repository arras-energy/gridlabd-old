/*	File: find.h 
 	Copyright (C) 2008, Battelle Memorial Institute
 */

#ifndef _FIND_H
#define _FIND_H

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

#include <stdarg.h>
#include <stdlib.h>
#include "object.h"
#include "match.h"
#include "convert.h"

struct s_object_list;

/*	Typedef: FINDOP
		See <e_findop>

	Enum: e_findop
	OR = (-2) - alternative condition
	AND = (-1) - concurrent condition
	EQ = 0 - equality
	LT = 1 - less than
	GT = 2 - greater than
	NE = 3 - not equal
	LE = 4 - less than or equal
	GE = 5 - greater than or equal
	NOT = 6 - inverse 
	BETWEEN = 7 - values between
	BEFORE = 8 - string before alphabetically
	AFTER = 9 - string after alphabetically
	SAME = 10 - string equal
	DIFF = 11 - string not equal
	MATCH = 12 - string matches using regular expression
	LIKE = 13 - string like using regular expression
	UNLIKE = 14 - string unlike regular expression
	ISA = 15 - 

	These values are used to index lookups in find operations
 */
typedef enum e_findop
{
	OR=-2,
	AND=-1, 
	EQ=0,
	LT=1,
	GT=2,
	NE=3,
	LE=4,
	GE=5,
	NOT=6,
	BETWEEN=7,
	BEFORE=8,
	AFTER=9,
	SAME=10,
	DIFF=11,
	MATCH=12,
	LIKE=13,
	UNLIKE=14,
	ISA=15,
	FINDOP_END
} FINDOP;

/*	Typedef: FINDTYPE
		See <e_findtype>

	Enum: e_findtype
	FT_OR = (-2) - or
	FT_AND = (-1) - and
	FT_END = 0 - end of list
	FT_ID = 1 - object id
	FT_SIZE = 2 - object size
	FT_CLASS = 3 - object class
	FT_PARENT = 4 - object parent
	FT_RANK = 5 - object rank
	FT_CLOCK = 6 - object clock
	FT_PROPERTY = 7 - object property
	FT_NAME = 8 - object name
	FT_LAT = 9 - object latitude
	FT_LONG  =10 - object longitude
	FT_INSVC = 11 - object in service
	FT_OUTSVC = 12 - object out of service
	FT_FLAGS = 13 - object flags
	FT_MODULE = 14 - object module
	FT_GROUPID = 15 - object group id
	FT_ISA = 16 - object isa
 */
typedef enum e_findtype
{
	FT_OR=-2,
	FT_AND=-1, 
	FT_END=0, 
	FT_ID=1, 
	FT_SIZE=2, 
	FT_CLASS=3, 
	FT_PARENT=4, 
	FT_RANK=5, 
	FT_CLOCK=6, 
	FT_PROPERTY=7, 
	FT_NAME=8,
	FT_LAT=9, 
	FT_LONG=10, 
	FT_INSVC=11, 
	FT_OUTSVC=12, 
	FT_FLAGS=13, 
	FT_MODULE=14, 
	FT_GROUPID=15, 
	FT_ISA=16
} FINDTYPE;

/*	Typedef: FINDLIST
		See <s_findlist>

	Structure: s_findlist
	result_size - size of result buffer
	hit_count - number of object found
	result - buffer containing result

	This structure stores the result of a find operation.
 */
typedef struct s_findlist 
{
	unsigned int result_size;
	unsigned int hit_count;
	char result[1]; /* this will expand according to need */
} FINDLIST;

/*	Typedef: FINDVALUE
		See <u_findvalue>

	Structure: u_findvalue
	integer - integer result
	pointer - pointer result
	real - real result
	string - string result

	This union stores the result of a find operation.
 */
typedef union u_findvalue 
{
	int64 integer;
	void *pointer;
	double real;
	char string[256];
} FINDVALUE; /**< a search value */

/*	Typedef: COMPAREFUNC

	Comparison function used to find objects.
 */
typedef int (*COMPAREFUNC)(void*, FINDVALUE);

/*	Typedef: FOUNDACTION

	Found action function used by find operations.
 */
typedef void (*FOUNDACTION)(FINDLIST *, struct s_object_list *);

/*	Typedef: PGMCONSTFLAGS
		See <e_pgmconstflags>

	Enum: e_pgmconstflags
	CF_NULL		= 0x0000 - no elements are invariant (or error)
	CF_SIZE		= 0x0001 - size criteria is invariant
	CF_ID		= 0x0002 - id criteria is invariant
	CF_CLASS	= 0x0004 - class criteria is invariant
	CF_RANK		= 0x0008 - rank criteria is invariant
	CF_CLOCK	= 0x0010 - clock criteria is variant
	CF_PARENT	= 0x0020 - parent criteria is invariant
	CF_PROPERTY	= 0x0040 - property criteria is variant
	CF_NAME		= 0x0080 - name criteria is invariant
	CF_LAT		= 0x0100 - latitude criteria is invariant
	CF_LONG		= 0x0200 - longitude criteria is invariant
	CF_INSVC	= 0x0400 - in-service criteria is invariant
	CF_OUTSVC	= 0x0800 - out-service criteria is invariant
	CF_FLAGS	= 0x1000 - flags criteria is variant
	CF_MODULE	= 0x2000 - module criteria is invariant
	CF_GROUPID	= 0x4000 - groupid criteria is invariant
	CF_CONSTANT = 0x8000 - entire criteria is invariant

	Flags that indicate whether a property of a find result is time invariant
 */
typedef enum e_pgmconstflags
{
	CF_NULL		= 0x0000,
	CF_SIZE		= 0x0001,
	CF_ID		= 0x0002,
	CF_CLASS	= 0x0004,
	CF_RANK		= 0x0008,
	CF_CLOCK	= 0x0010,
	CF_PARENT	= 0x0020,
	CF_PROPERTY	= 0x0040,
	CF_NAME		= 0x0080,
	CF_LAT		= 0x0100,
	CF_LONG		= 0x0200,
	CF_INSVC	= 0x0400,
	CF_OUTSVC	= 0x0800,
	CF_FLAGS	= 0x1000,
	CF_MODULE	= 0x2000,
	CF_GROUPID	= 0x4000,
	CF_CONSTANT = 0x8000,
} PGMCONSTFLAGS; 

/*	Typedef: FINDPGM
		See <s_findpgm>

	Structure: s_findpgm
	PGMCONSTFLAGS constflags - bits to indicate which criteria result in constant sets
	op - comparison operator
	target - offset from start of object header 
	value - value to compare to
	pos - action to call when the comparison result is positive
	neg - action to call when the comparison result is negative
	next - next program in element in find engine

	This implements a find engine
 */
typedef struct s_findpgm 
{
	PGMCONSTFLAGS constflags;
	COMPAREFUNC op;
	unsigned short target;
	FINDVALUE value;
	FOUNDACTION pos, neg;
	struct s_findpgm *next;
} FINDPGM;

#ifdef __cplusplus
extern "C" {
#endif

// Function: find_objects
FINDLIST *find_objects(FINDLIST *list, ...);

// Function: findlist_copy
FINDLIST *findlist_copy(FINDLIST *list);

// Function: findlist_add
void findlist_add(FINDLIST *list, struct s_object_list *obj);

// Function: findlist_del
void findlist_del(FINDLIST *list, struct s_object_list *obj);

// Function: findlist_clear
void findlist_clear(FINDLIST *list);

// Function: findlist_create
FINDLIST *findlist_create(FINDLIST *, const char *search);

// Function: find_first
struct s_object_list *find_first(FINDLIST *list);

// Function: find_next
struct s_object_list *find_next(FINDLIST *list, struct s_object_list *obj);

// Function: find_makearray
int find_makearray(FINDLIST *list, struct s_object_list ***objs);

// Function: find_pgm_run
FINDLIST *find_pgm_run(FINDLIST *list, FINDPGM *pgm);

// Function: find_pgm_new
FINDPGM *find_pgm_new(const char *search);

// Function: find_pgm_delete
void find_pgm_delete(FINDPGM *pgm);

// Function: find_pgmconstants
PGMCONSTFLAGS find_pgmconstants(FINDPGM *pgm);

// Function: find_file
const char *find_file(const char *name, const char *path=NULL, int mode=R_OK, char *buffer=NULL, int len=1024);

// Function: find_make_invariant
FINDPGM *find_make_invariant(FINDPGM *pgm, int mode);

#ifdef __cplusplus
}
#endif

// Define: FL_NEW
#define FL_NEW (FINDLIST*)(0)

// Define: FL_GROUP
#define FL_GROUP (FINDLIST*)(-1)

/* 	Typedef: OBJLIST
		See <s_objlist>

	Structure: s_objlist
	oclass - class of object
	asize - size of arrays
	size - number of items in list
	objlist - pointer to array of objects that match
 */
typedef struct s_objlist 
{
	CLASS *oclass;
	size_t asize;
	size_t size;
	struct s_object_list **objlist;
} OBJLIST;

#ifdef __cplusplus
extern "C" {
#endif

// Function: objlist_create
OBJLIST *objlist_create(CLASS *oclass, PROPERTY *match_property, const char *match_part, const char *match_op, void *match_value1, void *match_value2);

// Function: objlist_search
OBJLIST *objlist_search(const char *group);

// Function: objlist_destroy
void objlist_destroy(OBJLIST *list);

// Function: objlist_add
size_t objlist_add(OBJLIST *list, PROPERTY *match_property, const char *match_part, const char *match_op, void *match_value1, void *match_value2);

// Function: objlist_del
size_t objlist_del(OBJLIST *list, PROPERTY *match_property, const char *match_part, const char *match_op, void *match_value1, void *match_value2);

// Function: objlist_size
size_t objlist_size(OBJLIST *list);

// Function: objlist_get
struct s_object_list *objlist_get(OBJLIST *list,size_t n);

// Function: objlist_apply
int objlist_apply(OBJLIST *list, void *arg, int (*function)(struct s_object_list *,void *,int pos));

#ifdef __cplusplus
}
#endif

#endif

