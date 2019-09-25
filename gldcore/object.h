/* File: object.h 
 * Copyright (C) 2008, Battelle Memorial Institute

	@addtogroup object
 @{
 **/

#ifndef _OBJECT_H
#define _OBJECT_H

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

#include "complex.h"
#include "timestamp.h"
#include "class.h"
#include "aggregate.h"
#include "exception.h"
#include "globals.h"
#include "random.h"
#include "schedule.h"
#include "transform.h"
#include "enduse.h"

/* this must match property_type list in object.c */
typedef unsigned int OBJECTRANK; /**< Object rank number */
typedef unsigned short OBJECTSIZE; /** Object data size */
typedef unsigned int OBJECTNUM; /** Object id number */
typedef const char * OBJECTNAME; /** Object name */
typedef char FULLNAME[1024]; /** Full object name (including space name) */

#define PADDR_X(X,T) ((char*)&((T)->X)-(char*)(T))

/* object flags */
#define OF_NONE			0x00000000	/**< Object flag; none set */
#define OF_HASPLC		0x00000001	/**< Object flag; external PLC is attached, disables local PLC */
#define OF_LOCKED		0x00000002	/**< Object flag; data write pending, reread recommended after lock clears */
#define OF_RECALC		0x00000008	/**< Object flag; recalculation of derived values is needed */
#define OF_FOREIGN		0x00000010	/**< Object flag; indicates that object was created in a DLL and memory cannot be freed by core */
#define OF_SKIPSAFE		0x00000020	/**< Object flag; indicates that skipping updates is safe */
#define OF_DELTAMODE	0x00000040	/**< Object flag; indicates that delta mode is enabled on this object */
#define OF_FORECAST		0x00000040	/**< Object flag; inidcates that the object has a valid forecast available */
#define OF_DEFERRED		0x00000080	/**< Object flag; indicates that the object started to be initialized, but requested deferral */
#define OF_INIT			0x00000100	/**< Object flag; indicates that the object has been successfully initialized */
#define OF_RERANK		0x00004000	/**< Internal use only */
#define OF_QUIET		0x00010000  /**< Object flag; disables error messages from the object */
#define OF_WARNING		0x00020000  /**< Object flag; disables warning messages from the object */
#define OF_DEBUG		0x00040000  /**< Object flag; disables debug messages from the object */
#define OF_VERBOSE		0x00080000  /**< Object flag; disables verbose messages from the object */
#define OF_SILENT		0x000f0000	/**< Object flag; disables all messages from the object */

typedef struct s_namespace {
	FULLNAME name;
	struct s_namespace *next;
} NAMESPACE;

typedef struct s_forecast {
	char1024 specification; /**< forecast specification (see forecasting docs for details) */
	PROPERTY *propref; /**< property the forecast relates to */
	int n_values; /**< number of values in the forecast */
	TIMESTAMP starttime; /**< the start time of the forecast */
	int32 timestep; /**< number of seconds per forecast timestep */
	double *values; /**< values of the forecast (NULL if no forecast) */
	TIMESTAMP (*external)(void *obj, void *fc); /**< external forecast update call */
	struct s_forecast *next; /**< next forecast data block (NULL for last) */
} FORECAST; /**< Forecast data block */
typedef enum {
	OPI_PRESYNC,
	OPI_SYNC,
	OPI_POSTSYNC,
	OPI_INIT,
	OPI_HEARTBEAT,
	OPI_PRECOMMIT,
	OPI_COMMIT,
	OPI_FINALIZE,
	/* add profile items here */
	_OPI_NUMITEMS,
} OBJECTPROFILEITEM;
typedef struct s_eventhandlers {
	char *init;
	char *precommit;
	char *presync;
	char *sync;
	char *postsync;
	char *commit;
	char *finalize;
} EVENTHANDLERS;
typedef struct s_object_list {
	OBJECTNUM id; /**< object id number; globally unique */
	CLASS *oclass; /**< object class; determine structure of object data */
	OBJECTNAME name;
	char32 groupid;
	struct s_object_list *next; /**< next object in list */
	struct s_object_list *parent; /**< object's parent; determines rank */
	unsigned int child_count; /**< number of object that have this object as a parent */
	OBJECTRANK rank; /**< object's rank */
	TIMESTAMP clock; /**< object's private clock */
	TIMESTAMP valid_to;	/**< object's valid-until time */
	TIMESTAMP schedule_skew; /**< time skew applied to schedule operations involving this object */
	FORECAST *forecast; /**< forecast data block */
	double latitude, longitude; /**< object's geo-coordinates */
	TIMESTAMP in_svc, /**< time at which object begin's operating */
		out_svc; /**< time at which object ceases operating */
	unsigned int in_svc_micro,	/**< Microsecond portion of in_svc */
		out_svc_micro;	/**< Microsecond portion of out_svc */
	double in_svc_double;	/**< Double value representation of in service time */
	double out_svc_double;	/**< Double value representation of out of service time */
	clock_t synctime[_OPI_NUMITEMS]; /**< total time used by this object */
	NAMESPACE *space; /**< namespace of object */
	LOCKVAR lock; /**< object lock */
	unsigned int rng_state; /**< random number generator state */
	TIMESTAMP heartbeat; /**< heartbeat call interval (in sim-seconds) */
	unsigned long long guid[2]; /**< globally unique identifier */
	EVENTHANDLERS events;
	/* IMPORTANT: flags must be last */
	unsigned long long flags; /**< object flags */
} OBJECT; /**< Object header structure */

/* this is the callback table for modules
 * the table is initialized in module.cpp
 */
struct s_enduse;
typedef struct s_callbacks {
	TIMESTAMP *global_clock;
	double *global_delta_curr_clock;
	TIMESTAMP *global_stoptime;
	int (*output_verbose)(const char *format, ...);
	int (*output_message)(const char *format, ...);
	int (*output_warning)(const char *format, ...);
	int (*output_error)(const char *format, ...);
	int (*output_debug)(const char *format, ...);
	int (*output_test)(const char *format, ...);
	CLASS *(*register_class)(MODULE *,CLASSNAME,unsigned int,PASSCONFIG);
	struct {
		OBJECT *(*single)(CLASS*);
		OBJECT *(*array)(CLASS*,unsigned int);
		OBJECT *(*foreign)(OBJECT *);
	} create;
	int (*define_map)(CLASS*,...);
	int (*loadmethod)(CLASS*,const char*,LOADMETHODCALL call);
	CLASS *(*class_getfirst)(void);
	CLASS *(*class_getname)(const char*,CLASS *first);
	PROPERTY *(*class_add_extended_property)(CLASS *,const char *,PROPERTYTYPE,const char *);
	struct {
		FUNCTION *(*define)(CLASS*,FUNCTIONNAME,FUNCTIONADDR);
		FUNCTIONADDR (*get)(const char*,const char*);
	} function;
	int (*define_enumeration_member)(CLASS*,const char*,const char*,enumeration);
	int (*define_set_member)(CLASS*,const char*,const char*,unsigned int64);
	struct {
		OBJECT *(*get_first)(void);
		int (*set_dependent)(OBJECT*,OBJECT*);
		int (*set_parent)(OBJECT*,OBJECT*);
		OBJECTRANK (*set_rank)(OBJECT*,OBJECTRANK);
	} object;
	struct {
		PROPERTY *(*get_property)(OBJECT*,PROPERTYNAME,PROPERTYSTRUCT*);
		int (*set_value_by_addr)(OBJECT *, void*, const char*,PROPERTY*);
		int (*get_value_by_addr)(OBJECT *, void*, char*, int size,PROPERTY*);
		int (*set_value_by_name)(OBJECT *, const char *, const char *);
		int (*get_value_by_name)(OBJECT *, const char *, char *, int size);
		OBJECT *(*get_reference)(OBJECT *, const char *);
		const char *(*get_unit)(OBJECT *, const char *);
		void *(*get_addr)(OBJECT *, const char *);
		int (*set_value_by_type)(PROPERTYTYPE,void *data,const char *);
		bool (*compare_basic)(PROPERTYTYPE ptype, PROPERTYCOMPAREOP op, void* x, void* a, void* b, const char *part);
		PROPERTYCOMPAREOP (*get_compare_op)(PROPERTYTYPE ptype, const char *opstr);
		double (*get_part)(OBJECT*,PROPERTY*,const char*);
		PROPERTYSPEC *(*get_spec)(PROPERTYTYPE);
	} properties;
	struct {
		struct s_findlist *(*objects)(struct s_findlist *,...);
		OBJECT *(*next)(struct s_findlist *,OBJECT *obj);
		struct s_findlist *(*copy)(struct s_findlist *);
		void (*add)(struct s_findlist*, OBJECT*);
		void (*del)(struct s_findlist*, OBJECT*);
		void (*clear)(struct s_findlist*);
	} find;
	PROPERTY *(*find_property)(CLASS *, PROPERTYNAME);
	void *(*malloc)(size_t);
	void (*free)(void*);
	struct {
		struct s_aggregate *(*create)(const char *aggregator, const char *group_expression);
		double (*refresh)(struct s_aggregate *aggregate);
	} aggregate;
	struct {
		void *(*getvar)(MODULE *module, const char *varname);
		MODULE *(*getfirst)(void);
		int (*depends)(const char *name, unsigned char major, unsigned char minor, unsigned short build);
		const char *(*find_transform_function)(TRANSFORMFUNCTION function);
	} module;
	struct {
		double (*uniform)(unsigned int *rng, double a, double b);
		double (*normal)(unsigned int *rng, double m, double s);
		double (*bernoulli)(unsigned int *rng, double p);
		double (*pareto)(unsigned int *rng, double m, double a);
		double (*lognormal)(unsigned int *rng,double m, double s);
		double (*sampled)(unsigned int *rng,unsigned int n, double *x);
		double (*exponential)(unsigned int *rng,double l);
		RANDOMTYPE (*type)(const char *name);
		double (*value)(int type, ...);
		double (*pseudo)(RANDOMTYPE type, unsigned int *state, ...);
		double (*triangle)(unsigned int *rng,double a, double b);
		double (*beta)(unsigned int *rng,double a, double b);
		double (*gamma)(unsigned int *rng,double a, double b);
		double (*weibull)(unsigned int *rng,double a, double b);
		double (*rayleigh)(unsigned int *rng,double a);
	} random;
	int (*object_isa)(OBJECT *obj, const char *type);
	DELEGATEDTYPE* (*register_type)(CLASS *oclass, const char *type,int (*from_string)(void*,const char *),int (*to_string)(void*,char*,int));
	int (*define_type)(CLASS*,DELEGATEDTYPE*,...);
	struct {
		TIMESTAMP (*mkdatetime)(DATETIME *dt);
		int (*strdatetime)(DATETIME *t, char *buffer, int size);
		double (*timestamp_to_days)(TIMESTAMP t);
		double (*timestamp_to_hours)(TIMESTAMP t);
		double (*timestamp_to_minutes)(TIMESTAMP t);
		double (*timestamp_to_seconds)(TIMESTAMP t);
		int (*local_datetime)(TIMESTAMP ts, DATETIME *dt);
		int (*local_datetime_delta)(double ts, DATETIME *dt);
		TIMESTAMP (*convert_to_timestamp)(const char *value);
		TIMESTAMP (*convert_to_timestamp_delta)(const char *value, unsigned int *microseconds, double *dbl_time_value);
		int (*convert_from_timestamp)(TIMESTAMP ts, char *buffer, int size);
		int (*convert_from_deltatime_timestamp)(double ts_v, char *buffer, int size);
	} time;
	int (*unit_convert)(const char *from, const char *to, double *value);
	int (*unit_convert_ex)(UNIT *pFrom, UNIT *pTo, double *pValue);
	UNIT *(*unit_find)(const char *unit_name);
	struct {
		EXCEPTIONHANDLER *(*create_exception_handler)();
		void (*delete_exception_handler)(EXCEPTIONHANDLER *ptr);
		void (*throw_exception)(const char *msg, ...);
		const char *(*exception_msg)(void);
	} exception;
	struct {
		GLOBALVAR *(*create)(const char *name, ...);
		STATUS (*setvar)(const char *def,...);
		const char *(*getvar)(const char *name, char *buffer, size_t size);
		GLOBALVAR *(*find)(const char *name);
	} global;
	struct {
		void (*read)(LOCKVAR *);
		void (*write)(LOCKVAR *);
	} lock, unlock;
	struct {
		const char *(*find_file)(const char *name, const char *path, int mode, char *buffer, int len);
	} file;
	struct s_objvar_struct {
		bool *(*bool_var)(OBJECT *obj, PROPERTY *prop);
		complex *(*complex_var)(OBJECT *obj, PROPERTY *prop);
		enumeration *(*enum_var)(OBJECT *obj, PROPERTY *prop);
		set *(*set_var)(OBJECT *obj, PROPERTY *prop);
		int16 *(*int16_var)(OBJECT *obj, PROPERTY *prop);
		int32 *(*int32_var)(OBJECT *obj, PROPERTY *prop);
		int64 *(*int64_var)(OBJECT *obj, PROPERTY *prop);
		double *(*double_var)(OBJECT *obj, PROPERTY *prop);
		const char *(*string_var)(OBJECT *obj, PROPERTY *prop);
		OBJECT **(*object_var)(OBJECT *obj, PROPERTY *prop);
	} objvar;
	struct s_objvar_name_struct {
		bool *(*bool_var)(OBJECT *obj, const char *name);
		complex *(*complex_var)(OBJECT *obj, const char *name);
		enumeration *(*enum_var)(OBJECT *obj, const char *name);
		set *(*set_var)(OBJECT *obj, const char *name);
		int16 *(*int16_var)(OBJECT *obj, const char *name);
		int32 *(*int32_var)(OBJECT *obj, const char *name);
		int64 *(*int64_var)(OBJECT *obj, const char *name);
		double *(*double_var)(OBJECT *obj, const char *name);
		const char *(*string_var)(OBJECT *obj, const char *name);
		OBJECT **(*object_var)(OBJECT *obj, const char *name);
	} objvarname;
	struct {
		int (*string_to_property)(PROPERTY *prop, void *addr, const char *value);
		int (*property_to_string)(PROPERTY *prop, void *addr, char *value, int size);
	} convert;
	MODULE *(*module_find)(const char *name);
	OBJECT *(*get_object)(const char *name);
	OBJECT *(*object_find_by_id)(OBJECTNUM);
	int (*name_object)(OBJECT *obj, char *buffer, int len);
	int (*get_oflags)(KEYWORD **extflags);
	unsigned int (*object_count)(void);
	struct {
		SCHEDULE *(*create)(const char *name, const char *definition);
		SCHEDULEINDEX (*index)(SCHEDULE *sch, TIMESTAMP ts);
		double (*value)(SCHEDULE *sch, SCHEDULEINDEX index);
		int32 (*dtnext)(SCHEDULE *sch, SCHEDULEINDEX index);
		SCHEDULE *(*find)(const char *name);
		SCHEDULE *(*getfirst)(void);
	} schedule;
	struct {
		int (*create)(void *s);
		int (*init)(struct s_loadshape *s);
	} loadshape;
	struct {
		int (*create)(void *e);
		TIMESTAMP (*sync)(struct s_enduse *e, PASSCONFIG pass, TIMESTAMP t1);
	} enduse;
	struct {
		double (*linear)(double t, double x0, double y0, double x1, double y1);
		double (*quadratic)(double t, double x0, double y0, double x1, double y1, double x2, double y2);
	} interpolate;
	struct {
		FORECAST *(*create)(OBJECT *obj, const char *specs); /**< create a forecast using the specifications and append it to the object's forecast block */
		FORECAST *(*find)(OBJECT *obj, const char *name); /**< find the forecast for the named property, if any */
		double (*read)(FORECAST *fc, TIMESTAMP ts); /**< read the forecast value for the time ts */
		void (*save)(FORECAST *fc, TIMESTAMP ts, int32 tstep, int n_values, double *data);
	} forecast;
	struct {
		void *(*readobj)(void *local, OBJECT *obj, PROPERTY *prop);
		void (*writeobj)(void *local, OBJECT *obj, PROPERTY *prop);
		void *(*readvar)(void *local, GLOBALVAR *var);
		void (*writevar)(void *local, GLOBALVAR *var);
	} remote;
	struct {
		struct s_objlist *(*create)(CLASS *oclass, PROPERTY *match_property, const char *match_part, const char *match_op, void *match_value1, void *match_value2);
		struct s_objlist *(*search)(const char *group);
		void (*destroy)(struct s_objlist *list);
		size_t (*add)(struct s_objlist *list, PROPERTY *match_property, const char *match_part, const char *match_op, void *match_value1, void *match_value2);
		size_t (*del)(struct s_objlist *list, PROPERTY *match_property, const char *match_part, const char *match_op, void *match_value1, void *match_value2);
		size_t (*size)(struct s_objlist *list);
		struct s_object_list *(*get)(struct s_objlist *list,size_t n);
		int (*apply)(struct s_objlist *list, void *arg, int (*function)(struct s_object_list *,void *,int pos));
	} objlist;
	struct {
		struct {
			int (*to_string)(double v, char *buffer, size_t size);
			double (*from_string)(const char *buffer);
		} latitude, longitude;
	} geography;
	struct {
		struct s_http_result* (*read)(const char *url, int maxlen);
		void (*free)(struct s_http_result *result);
	} http;
	struct {
		TRANSFORM *(*getnext)(TRANSFORM*);
		int (*add_linear)(TRANSFORMSOURCE,double*,void*,double,double,OBJECT*,PROPERTY*,SCHEDULE*);
		int (*add_external)(OBJECT*,PROPERTY*,const char*,OBJECT*,PROPERTY*);
		int64 (*apply)(TIMESTAMP,TRANSFORM*,double*);
	} transform;
	struct {
		randomvar *(*getnext)(randomvar*);
		size_t (*getspec)(char *, size_t, const randomvar *);
	} randomvar;
	struct {
		unsigned int (*major)(void);
		unsigned int (*minor)(void);
		unsigned int (*patch)(void);
		unsigned int (*build)(void);
		const char * (*branch)(void);
	} version;
	long unsigned int magic; /* used to check structure alignment */
} CALLBACKS; /**< core callback function table */

#ifdef __cplusplus
extern "C" {
#endif

OBJECT *object_create_single(CLASS *oclass);
OBJECT *object_create_array(CLASS *oclass, unsigned int n_objects);
OBJECT *object_create_foreign(OBJECT *obj);
OBJECT *object_remove_by_id(OBJECTNUM id);
int object_init(OBJECT *obj);
STATUS object_precommit(OBJECT *obj, TIMESTAMP t1);
TIMESTAMP object_commit(OBJECT *obj, TIMESTAMP t1, TIMESTAMP t2);
STATUS object_finalize(OBJECT *obj);
int object_set_dependent(OBJECT *obj, OBJECT *dependent);
int object_set_parent(OBJECT *obj, OBJECT *parent);
unsigned int object_get_child_count(OBJECT *obj);
void *object_get_addr(OBJECT *obj, const char *name);
PROPERTY *object_get_property(OBJECT *obj, PROPERTYNAME name, PROPERTYSTRUCT *part);
PROPERTY *object_prop_in_class(OBJECT *obj, PROPERTY *prop);
int object_set_value_by_name(OBJECT *obj, PROPERTYNAME name, const char *value);
int object_set_value_by_addr(OBJECT *obj, void *addr, const char *value, PROPERTY *prop);
int object_set_int16_by_name(OBJECT *obj, PROPERTYNAME name, int16 value);
int object_set_int32_by_name(OBJECT *obj, PROPERTYNAME name, int32 value);
int object_set_int64_by_name(OBJECT *obj, PROPERTYNAME name, int64 value);
int object_set_double_by_name(OBJECT *obj, PROPERTYNAME name, double value);
bool *object_get_bool(OBJECT *obj, PROPERTY *prop);
bool *object_get_bool_by_name(OBJECT *obj, const char *name);
int object_set_complex_by_name(OBJECT *obj, PROPERTYNAME name, complex value);
int object_get_value_by_name(OBJECT *obj, PROPERTYNAME name, char *value, int size);
int object_get_value_by_addr(OBJECT *obj, void *addr, char *value, int size, PROPERTY *prop);

OBJECT *object_get_reference(OBJECT *obj, const char *name);
int object_isa(OBJECT *obj, const char *type);
OBJECTNAME object_set_name(OBJECT *obj, OBJECTNAME name);
OBJECT *object_find_name(OBJECTNAME name);
int object_build_name(OBJECT *obj, char *buffer, int len);
int object_locate_property(void *addr, OBJECT **pObj, PROPERTY **pProp);
int object_property_getsize(OBJECT *obj, PROPERTY *prop);

int object_get_oflags(KEYWORD **extflags);

TIMESTAMP object_sync(OBJECT *obj, TIMESTAMP to,PASSCONFIG pass);
OBJECT **object_get_object(OBJECT *obj, PROPERTY *prop);
OBJECT **object_get_object_by_name(OBJECT *obj, const char *name);
enumeration *object_get_enum(OBJECT *obj, PROPERTY *prop);
enumeration *object_get_enum_by_name(OBJECT *obj, const char *name);
set *object_get_set(OBJECT *obj, PROPERTY *prop);
set *object_get_set_by_name(OBJECT *obj, const char *name);
int16 *object_get_int16(OBJECT *obj, PROPERTY *prop);
int16 *object_get_int16_by_name(OBJECT *obj, const char *name);
int32 *object_get_int32(OBJECT *obj, PROPERTY *prop);
int32 *object_get_int32_by_name(OBJECT *obj, const char *name);
int64 *object_get_int64(OBJECT *obj, PROPERTY *prop);
int64 *object_get_int64_by_name(OBJECT *obj, const char *name);
double *object_get_double(OBJECT *pObj, PROPERTY *prop);
double *object_get_double_by_name(OBJECT *pObj, const char *name);
complex *object_get_complex(OBJECT *pObj, PROPERTY *prop);
complex *object_get_complex_by_name(OBJECT *pObj, const char *name);
double *object_get_double_quick(OBJECT *pObj, PROPERTY *prop);
complex *object_get_complex_quick(OBJECT *pObj, PROPERTY *prop);
const char *object_get_string(OBJECT *pObj, PROPERTY *prop);
const char *object_get_string_by_name(OBJECT *obj, const char *name);
FUNCTIONADDR object_get_function(CLASSNAME classname, FUNCTIONNAME functionname);
const char *object_property_to_string(OBJECT *obj, const char *name, char *buffer, int sz);
const char *object_get_unit(OBJECT *obj, const char *name);
OBJECTRANK object_set_rank(OBJECT *obj, OBJECTRANK rank);

OBJECT *object_find_by_id(OBJECTNUM id);
OBJECT *object_get_first(void);
OBJECT *object_get_next(OBJECT *obj);
unsigned int object_get_count(void);
size_t object_dump(char *buffer, size_t size, OBJECT *obj);
size_t object_save(char *buffer, size_t size, OBJECT *obj);
int object_saveall(FILE *fp);
int object_saveall_xml(FILE *fp);
void object_stream_fixup(OBJECT *obj, CLASSNAME classname, const char *objname);

const char *object_name(OBJECT *obj, char *, int);
int convert_from_latitude(double,char*,size_t);
int convert_from_longitude(double,char*,size_t);
double convert_to_latitude(const char *buffer);
double convert_to_longitude(const char *buffer);

PROPERTY *object_flag_property(void);
PROPERTY *object_access_property(void);

NAMESPACE *object_current_namespace(); /**< access the current namespace */
void object_namespace(char *buffer, int size); /**< get the namespace */
int object_get_namespace(OBJECT *obj, char *buffer, int size); /**< get the object's namespace */
int object_open_namespace(const char *space); /**< open a new namespace and make it current */
int object_close_namespace(); /**< close the current namespace and restore the previous one */
int object_select_namespace(const char *space); /**< change to another namespace */
int object_push_namespace(const char *space); /**< change to another namespace and push the one onto a stack */
NAMESPACE *object_pop_namespace(); /**< restore the previous namespace from stack */

/* forecasting API */
FORECAST *forecast_create(OBJECT *obj, const char *specs); /**< create a forecast using the specifications and append it to the object's forecast block */
FORECAST *forecast_find(OBJECT *obj, const char *name); /**< find the forecast for the named property, if any */
double forecast_read(FORECAST *fc, TIMESTAMP ts); /**< read the forecast value for the time ts */
void forecast_save(FORECAST *fc, TIMESTAMP ts, int32 tstep, int n_values, double *data);

/* remote data access */
void *object_remote_read(void *local, OBJECT *obj, PROPERTY *prop); /** access remote object data */
void object_remote_write(void *local, OBJECT *obj, PROPERTY *prop); /** access remote object data */

double object_get_part(void *x, const char *name);
int object_set_part(void *x, const char *name, const char *value);
TIMESTAMP object_heartbeat(OBJECT *obj);

int object_loadmethod(OBJECT *obj, const char *name, const char *value);

void object_synctime_profile_dump(const char *filename);

typedef struct s_jsondata {
	const char *name;
	const char *value;
	struct s_jsondata *next;
} JSONDATA;
bool object_set_json(OBJECT *obj, const char *propname, JSONDATA *data);

OBJECT *object_find_by_addr(void *addr);
PROPERTY *object_get_first_property(OBJECT *obj, bool full=true);
PROPERTY *object_get_next_property(PROPERTY *prop, bool full=true);
PROPERTY *object_get_property_by_addr(OBJECT *obj, void *addr, bool full=true);

#ifdef __cplusplus
}
#endif

#define object_size(X) ((X)?(X)->size:-1) /**< get the size of the object X */
#define object_id(X) ((X)?(X)->id:-1) /**< get the id of the object X */
#define object_parent(X) ((X)?(X)->parent:NULL) /**< get the parent of the object */
#define object_rank(X) ((X)?(X)->name:-1) /**< get the rank of the object */

#define OBJECTDATA(X,T) ((T*)((X)?((X)+1):NULL)) /**< get the object data structure */
#define GETADDR(O,P) ((O)?((void*)((char*)((O)+1)+(unsigned int64)((P)->addr))):NULL) /**< get the addr of an object's property */
#define OBJECTHDR(X) ((X)?(((OBJECT*)X)-1):NULL) /**< get the header from the object's data structure */
#define THISOBJECTHDR (((OBJECT*)this)-1)

#define MY (((OBJECT*)this)-1)
#define MYPARENT (MY->parent) /**< get the parent from the object's data structure */
#define MYCLOCK (MY->clock) /**< get an object's own clock */
#define MYRANK (MY->rank) /**< get an object's own rank */

#endif

/** @} **/