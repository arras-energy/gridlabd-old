/*	File: class

	Copyright (C) 2008, Battelle Memorial Institute

 */

#ifndef _CLASS_H
#define _CLASS_H

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "platform.h"
#include "complex.h"
#include "unit.h"
#include "property.h"
#include "lock.h"

/*	Typedef: FUNCTIONADDR
		Entry point of a module function
 */
typedef int64 (*FUNCTIONADDR)(void*,...); 

/*	Typedef: PASSCONFIG
		See <e_passconfig>

	Enum: e_passconfig
	PC_NOSYNC - used when the class requires no synchronization passes
	PC_PRETOPDOWN - used when the class requires synchronization on the first top-down pass
	PC_BOTTOMUP - used when the class requires synchronization on the bottom-up pass
	PC_POSTTOPDOWN - used when the class requires synchronization on the second top-down pass
	PC_FORCE_NAME - used to indicate the this class must define names for all its objects
	PC_PARENT_OVERRIDE_OMIT - used to ignore parent's use of PC_UNSAFE_OVERRIDE_OMIT
	PC_UNSAFE_OVERRIDE_OMIT - used to flag that omitting overrides is unsafe
	PC_ABSTRACTONLY - used to flag that the class should never be instantiated itself, only inherited classes should
	PC_AUTOLOCK - used to flag that sync operations should not be automatically write locked
	PC_OBSERVER - used to flag whether commit process needs to be delayed with respect to ordinary "in-the-loop" objects
 */
typedef enum e_passconfig
{
	PC_NOSYNC 				= 0x00,
	PC_PRETOPDOWN 			= 0x01, /**< used when the class requires synchronization on the first top-down pass */
	PC_BOTTOMUP				= 0x02, /**< used when the class requires synchronization on the bottom-up pass */
	PC_POSTTOPDOWN 			= 0x04, /**< used when the class requires synchronization on the second top-down pass */
	PC_FORCE_NAME 			= 0x20, /**< used to indicate the this class must define names for all its objects */
	PC_PARENT_OVERRIDE_OMIT = 0x40, /**< used to ignore parent's use of PC_UNSAFE_OVERRIDE_OMIT */
	PC_UNSAFE_OVERRIDE_OMIT = 0x80, /**< used to flag that omitting overrides is unsafe */ 
	PC_ABSTRACTONLY 		= 0x100, /**< used to flag that the class should never be instantiated itself, only inherited classes should */
	PC_AUTOLOCK 			= 0x200, /**< used to flag that sync operations should not be automatically write locked */
	PC_OBSERVER 			= 0x400, /**< used to flag whether commit process needs to be delayed with respect to ordinary "in-the-loop" objects */
} PASSCONFIG;

typedef enum e_notifymodule {
	NM_PREUPDATE = 0, /**< notify module before property change */
	NM_POSTUPDATE = 1, /**< notify module after property change */
	NM_RESET = 2,/**< notify module of system reset event */
} NOTIFYMODULE; /**< notification message types */

typedef struct s_function_map {
	CLASS *oclass;
	FUNCTIONNAME name;
	FUNCTIONADDR addr;
	struct s_function_map *next;
} FUNCTION;

typedef int (*LOADMETHODCALL)(struct s_object_list*,const char*);
typedef struct s_loadmethod {
	const char *name;
	LOADMETHODCALL call;
	struct s_loadmethod *next;
} LOADMETHOD;

/* Set operations */
#define SET_MASK 0xffff
#define SET_ADD(set,value) (set = set | value )
#define SET_DEL(set,value) (set = (value^SET_MASK)&set) /*SET_HAS(set,value)?set^value:set) */
#define SET_CLEAR(set) (set = 0)
#define SET_HAS(set,value) (set & value)

typedef enum {CLASSVALID=0xc44d822e} CLASSMAGIC; /* this is used to uniquely identify classes */

/* Technology readiness levels (see http://sourceforge.net/apps/mediawiki/gridlab-d/index.php?title=Technology_Readiness_Levels) */
typedef enum {
	TRL_UNKNOWN			= 0,
	TRL_PRINCIPLE		= 1,
	TRL_CONCEPT			= 2,
	TRL_PROOF			= 3,
	TRL_STANDALONE		= 4,
	TRL_INTEGRATED		= 5,
	TRL_DEMONSTRATED	= 6,
	TRL_PROTOTYPE		= 7,
	TRL_QUALIFIED		= 8,
	TRL_PROVEN			= 9,
} TECHNOLOGYREADINESSLEVEL;

#define _MODULE_DEFINED_
typedef struct s_module_list MODULE;
struct s_class_list {
	CLASSMAGIC magic;
	int id;
	CLASSNAME name;
	unsigned int size;
	MODULE *module; 
	PROPERTY *pmap;
	FUNCTION *fmap;
	FUNCTIONADDR create;
	FUNCTIONADDR init;
	FUNCTIONADDR precommit;
	FUNCTIONADDR sync;
	FUNCTIONADDR commit;
	FUNCTIONADDR finalize;
	FUNCTIONADDR notify;
	FUNCTIONADDR isa;
	FUNCTIONADDR plc;
	PASSCONFIG passconfig;
	FUNCTIONADDR recalc;
	FUNCTIONADDR update;	/**< deltamode related */
	FUNCTIONADDR heartbeat;
	LOADMETHOD *loadmethods;
	CLASS *parent;			/**< parent class from which properties should be inherited */
	struct {
		LOCKVAR lock;
		int32 numobjs;
		int64 clocks;
		int32 count;
	} profiler;
	TECHNOLOGYREADINESSLEVEL trl; // technology readiness level (1-9, 0=unknown)
	bool has_runtime;	///< flag indicating that a runtime dll, so, or dylib is in use
	char runtime[1024]; ///< name of file containing runtime dll, so, or dylib
	struct s_class_list *next;
}; /* CLASS */

#ifdef __cplusplus
extern "C" {
#endif

PROPERTY *class_get_first_property(CLASS *oclass);
PROPERTY *class_get_next_property(PROPERTY *prop);
PROPERTY *class_get_first_property_inherit(CLASS *oclass);
PROPERTY *class_get_next_property_inherit(PROPERTY *prop);
PROPERTY *class_prop_in_class(CLASS *oclass, PROPERTY *prop);
PROPERTY *class_find_property(CLASS *oclass, PROPERTYNAME name);
void class_add_property(CLASS *oclass, PROPERTY *prop);
PROPERTY *class_add_extended_property(CLASS *oclass, const char *name, PROPERTYTYPE ptype, const char *unit);
PROPERTYTYPE class_get_propertytype_from_typename(char *name);
int class_string_to_property(PROPERTY *prop, void *addr, const char *value);
int class_string_to_propertytype(PROPERTYTYPE type, void *addr, const char *value);
int class_property_to_string(PROPERTY *prop, void *addr, char *value, int size);
CLASS *class_get_first_class(void);
CLASS *class_get_last_class(void);
CLASS *class_get_class_from_classname(CLASSNAME name);
CLASS *class_get_class_from_classname_in_module(CLASSNAME name, MODULE *mod);
const char *class_get_property_typename(PROPERTYTYPE type);
const char *class_get_property_typexsdname(PROPERTYTYPE type);
int class_saveall(FILE *fp);
int class_saveall_xml(FILE *fp);
unsigned int class_get_count(void);
void class_profiles(void);
int class_get_xsd(CLASS *oclass, char *buffer, size_t len);
size_t class_get_runtimecount(void);
CLASS *class_get_first_runtime(void);
CLASS *class_get_next_runtime(CLASS *oclass);
size_t class_get_extendedcount(CLASS *oclass);

CLASS *class_register(MODULE *module,CLASSNAME classname,unsigned int datasize,PASSCONFIG passconfig);
int class_define_map(CLASS *oclass, ...);
int class_define_enumeration_member(CLASS *oclass,const char *property_name,const char *member,enumeration value);
int class_define_set_member(CLASS *oclass,const char *property_name,const char *member,unsigned int64 value);
FUNCTION *class_define_function(CLASS *oclass, FUNCTIONNAME functionname, FUNCTIONADDR call);
FUNCTIONADDR class_get_function(CLASSNAME classname, FUNCTIONNAME functionname);
DELEGATEDTYPE *class_register_type(CLASS *oclass, const char *type,int (*from_string)(void*,const char*),int (*to_string)(void*,char*,int));
int class_define_type(CLASS *oclass, DELEGATEDTYPE *delegation, ...);

int class_add_loadmethod(CLASS *oclass, const char *name, LOADMETHODCALL call);
LOADMETHOD *class_get_loadmethod(CLASS *oclass,const char *name);

#ifdef __cplusplus
}
#endif

#endif

/* EOF */
