/*	File: class

	Copyright (C) 2008, Battelle Memorial Institute

 */

#ifndef _CLASS_H
#define _CLASS_H

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

#include "property.h"
#include "lock.h"

/*	Section: Function calls

	Typedef: FUNCTIONADDR

		Entry point of a module function
 */
typedef int64 (*FUNCTIONADDR)(void*,...); 

/*	Typedef: PASSCONFIG

		Synchronization options. See <e_passconfig>

	Enum: e_passconfig

		PC_NOSYNC = 0x0000 - used when the class requires no synchronization passes
		PC_PRETOPDOWN = 0x0001 - used when the class requires synchronization on the first top-down pass
		PC_BOTTOMUP = 0x0002 - used when the class requires synchronization on the bottom-up pass
		PC_POSTTOPDOWN = 0x0004 - used when the class requires synchronization on the second top-down pass
		PC_FORCE_NAME = 0x0020 - used to indicate the this class must define names for all its objects
		PC_PARENT_OVERRIDE_OMIT = 0x0040 - used to ignore parent's use of PC_UNSAFE_OVERRIDE_OMIT
		PC_UNSAFE_OVERRIDE_OMIT = 0x0080 - used to flag that omitting overrides is unsafe
		PC_ABSTRACTONLY = 0x0100 - used to flag that the class should never be instantiated itself, only inherited classes should
		PC_AUTOLOCK = 0x0200 - used to flag that sync operations should not be automatically write locked
		PC_OBSERVER = 0x0400 - used to flag whether commit process needs to be delayed with respect to ordinary "in-the-loop" objects
 */
typedef enum e_passconfig
{
	PC_NOSYNC 				= 0x0000,
	PC_PRETOPDOWN 			= 0x0001,
	PC_BOTTOMUP				= 0x0002,
	PC_POSTTOPDOWN 			= 0x0004,
	PC_FORCE_NAME 			= 0x0020,
	PC_PARENT_OVERRIDE_OMIT = 0x0040,
	PC_UNSAFE_OVERRIDE_OMIT = 0x0080,
	PC_ABSTRACTONLY 		= 0x0100,
	PC_AUTOLOCK 			= 0x0200,
	PC_OBSERVER 			= 0x0400,
} PASSCONFIG;

/*	Typedef: NOTIFYMODULE

		Notification message types. See <e_notifymodule>

	Enum: e_notifymodule

		NM_PREUPDATE = 0 - notify module before property change
		NM_POSTUPDATE = 1 - notify module after property change
		NM_RESET = 2 - notify module of system reset event
 */
typedef enum e_notifymodule {
	NM_PREUPDATE = 0, 
	NM_POSTUPDATE = 1,
	NM_RESET = 2,
} NOTIFYMODULE; 

/*	Typedef: FUNCTION
	
		Class function definition. See <s_function_map>

	Structure: s_function_map

		<oclass> - class implementing the function
		<name> - function name
		<addr> - entry to function
		<next> - next function in list
 */
typedef struct s_function_map {
	// Field: oclass
	CLASS *oclass;
	// Field: name
	FUNCTIONNAME name;
	// Field: addr
	FUNCTIONADDR addr;
	// Field: next
	struct s_function_map *next;
} FUNCTION;

/*	Section: Load methods

	Typedef: LOADMETHODCALL
	
		int LOADMETHODCALL(<OBJECT> *object, const char *name)
 */
typedef int (*LOADMETHODCALL)(struct s_object_list *object, const char *name);

/*	Structure: s_loadmethod

		<name> - Method name 
		<call> - Method call address
		<next> - Next method in method list (NULL if none)
 */
typedef struct s_loadmethod {
	// Field: name
	const char *name;
	// Field: call
	LOADMETHODCALL call;
	// Field: next
	struct s_loadmethod *next;
} LOADMETHOD;

/* Section: Set operations

	<SET_ADD> - Add a bit
	<SET_DEL> - Delete a bit
	<SET_CLEAR> - Clear bits
	<SET_HAS> - Test bigs
 */
#define SET_MASK 0xffff
/*	Define: SET_ADD

	Add a bit in a <set> value
 */
#define SET_ADD(set,value) (set = set | value )
/*	Define: SET_DEL

	Delete a bit in a <set> value
 */
#define SET_DEL(set,value) (set = (value^SET_MASK)&set) /*SET_HAS(set,value)?set^value:set) */
/*	Define: SET_CLEAR

	Clear bits in a <set> value
 */
#define SET_CLEAR(set) (set = 0)
/*	Define: SET_HAS

	Test bits in a <set> value
 */
#define SET_HAS(set,value) (set & value)

/*	Group: Magic numbers

	Typedef: CLASSMAGIC

		Magic value used to detect class. See <e_classmagic>.

	Enum: e_classmagic

		CLASSVALID = 0xc44d822e - uniquely identify classes
 */
typedef enum e_classmagic {CLASSVALID=0xc44d822e} CLASSMAGIC; 

/*	Group: Technology Readiness Levels

	Typedef: TECHNOLOGYREADINESSLEVEL

		Technology readiness levels. See <e_technologyreadinesslevel>.

	Enum: e_technologyreadinesslevel

		TRL_UNKNOWN			= 0 - TRL 0
		TRL_PRINCIPLE		= 1 - TRL 1
		TRL_CONCEPT			= 2 - TRL 2
		TRL_PROOF			= 3 - TRL 3
		TRL_STANDALONE		= 4 - TRL 4
		TRL_INTEGRATED		= 5 - TRL 5
		TRL_DEMONSTRATED	= 6 - TRL 6
		TRL_PROTOTYPE		= 7 - TRL 7
		TRL_QUALIFIED		= 8 - TRL 8
		TRL_PROVEN			= 9 - TRL 9
 */
typedef enum e_technologyreadinesslevel {
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

/*	Group: Module data

	Typedef: MODULE
		See <s_module_list>
 */
typedef struct s_module_list MODULE;

/*	Structure: s_class_list
	magic - magic number
	name - class name
	size - object size
	module - module reference
	pmap - property list
	fmap - function list
	create - create function
	init - init function
	precommit - precommit function
	sync - sync function
	commit - commit function
	finalize - finalize function
	notify - notify function
	isa - isa function
	plc - plc function
	passconfig - passconfig flags
	recalc - recalc function
	update - deltamode update function
	heartbeat - heartbeat function
	loadmethods - loadmethod list
	parent - parent class
	profiler - profiler structure
		profiler.lock - lock count
		profiler.numobjs - object count
		profiler.clocks - clock count
		profiler.count - event count
	trl - technology readiness level
	has_runtime - flag to indicate runtime DLL is used
	runtime - filename of runtime DLL used
	next - next class in class list
 */
struct s_class_list {
	// Field: magic
	CLASSMAGIC magic;
	// Field: id
	int id;
	// Field: name
	CLASSNAME name;
	// Field: size
	unsigned int size;
	// Field: module
	MODULE *module; 
	// Field: pmap
	PROPERTY *pmap;
	// Field: fmap
	FUNCTION *fmap;
	// Field: create
	FUNCTIONADDR create;
	// Field: init
	FUNCTIONADDR init;
	// Field: precommit
	FUNCTIONADDR precommit;
	// Field: sync
	FUNCTIONADDR sync;
	// Field: commit
	FUNCTIONADDR commit;
	// Field: finalize
	FUNCTIONADDR finalize;
	// Field: notify
	FUNCTIONADDR notify;
	// Field: isa
	FUNCTIONADDR isa;
	// Field: plc
	FUNCTIONADDR plc;
	// Field: passconfig
	PASSCONFIG passconfig;
	// Field: recalc
	FUNCTIONADDR recalc;
	// Field: update
	FUNCTIONADDR update;
	// Field: 
	FUNCTIONADDR heartbeat;
	// Field: loadmethods
	LOADMETHOD *loadmethods;
	// Field: parent
	CLASS *parent;
	// Field: profiler
	struct {
		// Field: lock
		LOCKVAR lock;
		// Field: numobjs
		int32 numobjs;
		// Field: clocks
		int64 clocks;
		// Field: count
		int32 count;
	} profiler;
	// Field: trl
	TECHNOLOGYREADINESSLEVEL trl; 
	// Field: has_runtime
	bool has_runtime;
	// Field: runtime
	char runtime[1024];
	// Field: next
	struct s_class_list *next;
}; /* CLASS */

#ifdef __cplusplus
extern "C" {
#endif

/* Function: class_get_first_property

	This function is obsolete.

 */
DEPRECATED PROPERTY *class_get_first_property(CLASS *oclass);

/* Function: class_get_next_property

	This function is obsolete.
	
 */
DEPRECATED PROPERTY *class_get_next_property(PROPERTY *prop);

/* Function: class_get_first_property_inherit

	This function is obsolete.
	
 */
DEPRECATED PROPERTY *class_get_first_property_inherit(CLASS *oclass);

/* Function: class_get_next_property_inherit

	This function is obsolete.
	
 */
DEPRECATED PROPERTY *class_get_next_property_inherit(PROPERTY *prop);

/* Function: class_prop_in_class

	This function is obsolete.
	
 */
DEPRECATED PROPERTY *class_prop_in_class(CLASS *oclass, PROPERTY *prop);

/* Function: class_find_property

	This function is obsolete.
	
 */
DEPRECATED PROPERTY *class_find_property(CLASS *oclass, PROPERTYNAME name);

/* Function: class_add_property

	This function is obsolete.
	
 */
DEPRECATED void class_add_property(CLASS *oclass, PROPERTY *prop);

/* Function: class_add_extended_property

	This function is obsolete.
	
 */
DEPRECATED PROPERTY *class_add_extended_property(CLASS *oclass, const char *name, PROPERTYTYPE ptype, const char *unit);

/* Function: class_get_propertytype_from_typename

	This function is obsolete.
	
 */
DEPRECATED PROPERTYTYPE class_get_propertytype_from_typename(char *name);

/* Function: class_string_to_property

	This function is obsolete.
	
 */
DEPRECATED int class_string_to_property(PROPERTY *prop, void *addr, const char *value);

/* Function: class_string_to_propertytype

	This function is obsolete.
	
 */
DEPRECATED int class_string_to_propertytype(PROPERTYTYPE type, void *addr, const char *value);

/* Function: class_property_to_string

	This function is obsolete.
	
 */
DEPRECATED int class_property_to_string(PROPERTY *prop, void *addr, char *value, int size);

/* Function: class_get_first_class

	This function is obsolete.
	
 */
DEPRECATED CLASS *class_get_first_class(void);

/* Function: class_get_last_class

	This function is obsolete.
	
 */
DEPRECATED CLASS *class_get_last_class(void);

/* Function: class_get_class_from_classname

	This function is obsolete.
	
 */
DEPRECATED CLASS *class_get_class_from_classname(CLASSNAME name, CLASS *first=NULL);

/* Function: class_get_class_from_classname_in_module

	This function is obsolete.
	
 */
DEPRECATED CLASS *class_get_class_from_classname_in_module(CLASSNAME name, MODULE *mod);

/* Function: class_get_property_typename

	This function is obsolete.
	
 */
DEPRECATED const char *class_get_property_typename(PROPERTYTYPE type);

/* Function: class_get_property_typexsdname

	This function is obsolete.
	
 */
DEPRECATED const char *class_get_property_typexsdname(PROPERTYTYPE type);

/* Function: class_saveall

	This function is obsolete.
	
 */
DEPRECATED int class_saveall(FILE *fp);

/* Function: class_saveall_xml

	This function is obsolete.
	
 */
DEPRECATED int class_saveall_xml(FILE *fp);

/* Function: class_get_count

	This function is obsolete.
	
 */
DEPRECATED unsigned int class_get_count(void);

/* Function: class_profiles

	This function is obsolete.
	
 */
DEPRECATED void class_profiles(void);

/* Function: class_get_xsd

	This function is obsolete.
	
 */
DEPRECATED int class_get_xsd(CLASS *oclass, char *buffer, size_t len);

/* Function: class_get_runtimecount

	This function is obsolete.
	
 */
DEPRECATED size_t class_get_runtimecount(void);

/* Function: class_get_first_runtime

	This function is obsolete.
	
 */
DEPRECATED CLASS *class_get_first_runtime(void);

/* Function: class_get_next_runtime

	This function is obsolete.
	
 */
DEPRECATED CLASS *class_get_next_runtime(CLASS *oclass);

/* Function: class_get_extendedcount

	This function is obsolete.
	
 */
DEPRECATED size_t class_get_extendedcount(CLASS *oclass);

/* Function: class_register

	This function is obsolete.
	
 */
DEPRECATED CLASS *class_register(MODULE *module,CLASSNAME classname,unsigned int datasize,PASSCONFIG passconfig);

/* Function: class_define_map

	This function is obsolete.
	
 */
DEPRECATED int class_define_map(CLASS *oclass, ...);

/* Function: class_define_enumeration_member

	This function is obsolete.
	
 */
DEPRECATED int class_define_enumeration_member(CLASS *oclass,const char *property_name,const char *member,enumeration value);

/* Function: class_define_set_member

	This function is obsolete.
	
 */
DEPRECATED int class_define_set_member(CLASS *oclass,const char *property_name,const char *member,unsigned int64 value);

/* Function: class_define_function

	This function is obsolete.
	
 */
DEPRECATED FUNCTION *class_define_function(CLASS *oclass, FUNCTIONNAME functionname, FUNCTIONADDR call);

/* Function: class_get_function

	This function is obsolete.
	
 */
DEPRECATED FUNCTIONADDR class_get_function(CLASSNAME classname, FUNCTIONNAME functionname);

/* Function: class_register_type

	This function is obsolete.
	
 */
DEPRECATED DELEGATEDTYPE *class_register_type(CLASS *oclass, const char *type,int (*from_string)(void*,const char*),int (*to_string)(void*,char*,int));

/* Function: class_define_type

	This function is obsolete.
	
 */
DEPRECATED int class_define_type(CLASS *oclass, DELEGATEDTYPE *delegation, ...);

/* Function: class_add_loadmethod

	This function is obsolete.
	
 */
DEPRECATED int class_add_loadmethod(CLASS *oclass, const char *name, LOADMETHODCALL call);

/* Function: class_get_loadmethod

	This function is obsolete.
	
 */
DEPRECATED LOADMETHOD *class_get_loadmethod(CLASS *oclass,const char *name);

#ifdef __cplusplus
}

class GldClass;

/* class: GldProfiler
	Field:
	lock - lock count
	numobjs - object count
	clocks - time count
	count - event count
 */
class GldProfiler {
public:
	GldClass *oclass;
public:
	// Field: lock
	LOCKVAR lock;
	// Field: numobjs
	int32 numobjs;
	// Field: clocks
	int64 clocks;
	// Field: count
	int32 count;
public:
	// Constructor: GldProfiler
	GldProfiler(GldClass &cls);

	// Destructor: ~GldProfiler
	~GldProfiler(void);
};

class GldMain;
class GldProperty;

class GldClassList
{
private:
	size_t class_count;
	GldClass *first_class;
	GldClass *last_class;
public:
	GldClassList(GldMain*inst);
	~GldClassList(void);
public:
	inline size_t get_count(void) { return class_count;};
	inline GldClass* get_first(void) { return first_class;};
	inline GldClass* get_next(void) { return last_class;};
	void add_class(GldClass*);
};

/* Class: GldClass */
class GldClass {
private:
	GldMain &instance;
private:
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
	FUNCTIONADDR update;
	FUNCTIONADDR heartbeat;
	LOADMETHOD *loadmethods;
	CLASS *parent;
	GldProfiler profiler;
	TECHNOLOGYREADINESSLEVEL trl; 
	bool has_runtime;
	char runtime[1024];
	struct s_class_list *next;

public:
	// Constructor: GldClass
	GldClass(GldMain &main);

	// Destructor: ~GldClass
	~GldClass(void);

public:
	// Function: get_first_property
	PROPERTY *get_first_property(void);

	// Function: get_next_property
	PROPERTY *get_next_property(PROPERTY *prop);
	
	// Function: get_first_property_inherit
	PROPERTY *get_first_property_inherit();
	
	// Function: get_next_property_inherit
	PROPERTY *get_next_property_inherit(PROPERTY *prop);
	
	// Function: prop_in_class
	PROPERTY *prop_in_class(PROPERTY *prop);
	
	// Function: find_property
	PROPERTY *find_property(PROPERTYNAME name);
	
	// Function: add_property
	void add_property(PROPERTY *prop);
	
	// Function: add_extended_property
	PROPERTY *add_extended_property(CLASS *oclass, const char *name, PROPERTYTYPE ptype, const char *unit);
	
	// Function: get_propertytype_from_typename
	PROPERTYTYPE get_propertytype_from_typename(char *name);
	
	// Function: string_to_property
	int string_to_property(PROPERTY *prop, void *addr, const char *value);
	
	// Function: string_to_propertytype
	int string_to_propertytype(PROPERTYTYPE type, void *addr, const char *value);
	
	// Function: property_to_string
	int property_to_string(PROPERTY *prop, void *addr, char *value, int size);
	
	// Function: get_first_class
	CLASS *get_first_class(void);
	
	// Function: get_last_class
	CLASS *get_last_class(void);
	
	// Function: get_class_from_classname
	CLASS *get_class_from_classname(CLASSNAME name);
	
	// Function: get_class_from_classname_in_module
	CLASS *get_class_from_classname_in_module(CLASSNAME name, MODULE *mod);
	
	// Function: get_property_typename
	const char *get_property_typename(PROPERTYTYPE type);
	
	// Function: get_property_typexsdname
	const char *get_property_typexsdname(PROPERTYTYPE type);
	
	// Function: saveall
	int saveall(FILE *fp);
	
	// Function: saveall_xml
	int saveall_xml(FILE *fp);
	
	// Function: get_count
	unsigned int get_count(void);
	
	// Function: profiles
	void profiles(void);
	
	// Function: class_get_xsd
	int get_xsd(char *buffer, size_t len);
	
	// Function: get_runtimecount
	size_t get_runtimecount(void);
	
	// Function: get_first_runtime
	CLASS *get_first_runtime(void);
	
	// Function: get_next_runtime
	CLASS *get_next_runtime(CLASS *oclass);
	
	// Function: get_extendedcount
	size_t get_extendedcount(CLASS *oclass);
	
	// Function: class_register
	CLASS *class_register(MODULE *module,CLASSNAME classname,unsigned int datasize,PASSCONFIG passconfig);
	
	// Function: define_map
	int define_map(...);
	
	// Function: define_enumeration_member
	int define_enumeration_member(const char *property_name,const char *member,enumeration value);
	
	// Function: define_set_member
	int define_set_member(const char *property_name,const char *member,unsigned int64 value);
	
	// Function: define_function
	FUNCTION *define_function(FUNCTIONNAME functionname, FUNCTIONADDR call);
	
	// Function: get_function
	FUNCTIONADDR get_function(CLASSNAME classname, FUNCTIONNAME functionname);
	
	// Function: register_type
	DELEGATEDTYPE *register_type(const char *type,int (*from_string)(void*,const char*),int (*to_string)(void*,char*,int));
	
	// Function: define_type
	int define_type(DELEGATEDTYPE *delegation, ...);
	
	// Function: add_loadmethod
	int add_loadmethod(const char *name, LOADMETHODCALL call);
	
	// Function: get_loadmethod
	LOADMETHOD *get_loadmethod(const char *name);
};

#endif

#endif

/* EOF */
