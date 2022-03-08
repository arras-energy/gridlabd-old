// File: load.h 
// Copyright (C) 2008, Battelle Memorial Institute
// Copyright (C) 2020, Regents of the Leland Stanford Junior University

#ifndef _LOAD_H
#define _LOAD_H

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

#include <map>

#include "globals.h"
#include "module.h"

// Define: UR_NONE
// Specifies no unresolved reference flags are 
#define UR_NONE  0x00     

// Define: UR_RANKS
// Specifies unresolved reference has ranking impact
#define UR_RANKS 0x01

// Define: UR_TRANSFORM
// Specifies reference is via a transform
#define UR_TRANSFORM 0x02 

// Function: loadall
// Loads the contents of a file into the current instance
// Returns: STATUS
DEPRECATED STATUS loadall(const char *filename);

// Function: load_get_current_object
// Obtains the object currently being processed by the loaded
// Returns: OBJECT*
DEPRECATED OBJECT *load_get_current_object(void);

// Function: load_get_current_module
// Obtains the module current being processed by the loaded
// Returns: MODULE*
DEPRECATED MODULE *load_get_current_module(void);

#define PARSER const char *_p

// Class: GldLoader
// Implements the GLM parser
class GldLoader
{

public:

	// Constructor: GldLoader
	// Constructs a loader for a simulation instance
	GldLoader(GldMain *main);

	// Destructor: ~GldLoader
	// Destroys a loader for a simulation instance
	~GldLoader(void);

public:

	// Method: load
	// Loads a file into the loader's instance
	bool load(const char *filename);

	// Method: get_current_object
	// Obtains the object currently being processed by the loader
	GldObject get_current_object(void);

	// Method: get_current_module
	// Obtains the module currently being processed b the loader
	GldModule get_current_module(void);

	// Method: get_depends
	// Obtains the current simulation's dependency tree
	std::string get_depends(const char *format=NULL);

private:

	typedef struct s_languagemap 
	{
		const char *name;
		void *(*init)(int argc, const char **argv);
		bool (*parser)(const char *buffer, void *context);
		struct s_languagemap *next;
	} LANGUAGE;

	typedef struct s_unresolved 
	{
		OBJECT *by;
		PROPERTYTYPE ptype;
		void *ref;
		int flags;
		CLASS *oclass;
		const char *id;
		const char *file;
		unsigned int line;
		struct s_unresolved *next;
	} UNRESOLVED;

	typedef struct s_unresolved_func 
	{
		char1024 funcstr;
		OBJECT *obj;
		double *targ;
		unsigned int line;
		struct s_unresolved_func *next;
	} UNR_FUNC;

	typedef struct s_unresolved_static 
	{
		char256	member_name;
		char256 class_name;
		struct s_unresolved_static *next;
	} UNR_STATIC;

	// used for tracking #include directives in files
	typedef struct s_include_list 
	{
		char file[256];
		struct s_include_list *next;
	} INCLUDELIST;

	typedef int (*PARSERCALL)(PARSER);

	typedef struct s_loaderhook 
	{
		PARSERCALL call;
		struct s_loaderhook *next;
	} LOADERHOOK;
	
	typedef int (*LOADERINIT)(void);

	// TODO: convert this to a class so nesting is possible
	typedef enum e_forloopstate 
	{
		FOR_NONE   = 0, // no for loop active
		FOR_BODY   = 1, // for loop started, body capture in progress
		FOR_REPLAY = 2, // for loop replay in progress
	} FORLOOPSTATE;

	static struct s_threadlist 
	{
		pthread_t *data;
		struct s_threadlist *next;
	} *threadlist;

	typedef struct s_indexmap
	{
		OBJECT *obj;
		size_t count;
	} INDEXITEM;

	typedef std::map<OBJECTNUM,INDEXITEM> INDEXMAP;

	typedef std::map<std::string, std::list<std::string> > DEPENDENCY_TREE;

private:

	GldMain &instance;

	int include_fail;
	time_t modtime;

	char filename[1024];
	unsigned int linenum;

	std::string code_block;
	std::string global_block;
	std::string init_block;
	int code_used;

	INCLUDELIST *include_list;
	INCLUDELIST *header_list;

	int outlinenum;
	char *outfilename;

	// OBJECT **object_index;
	// char *object_linked;
	// unsigned int object_index_size;
	INDEXMAP indexmap;

	UNRESOLVED *first_unresolved;

	OBJECT *current_object;
	MODULE *current_module;

	LOADERHOOK *loaderhooks;

	int suppress;
	int nesting;
	int macro_line[64];

	// TODO: convert this to a class so nesting is possible
	FORLOOPSTATE forloopstate;
	char *forloop; // for loop value list
	char *lastfor; // pointer to last strtok_r value
	char *forvar; // global variable to use
	const char *forvalue; // current value of global variable
	std::list<std::string> forbuffer; // captured body
	std::list<std::string>::const_iterator forbufferline; // body line iterator
	bool forloop_verbose;

	LANGUAGE *language_list;
	LANGUAGE *language;

	DEPENDENCY_TREE dependency_tree;

	const char *last_term;
	char *last_term_buffer;
	size_t last_term_buffer_size;

private:

	void syntax_error(const char *filename, const int linenum, const char *format, ...);
	std::string format_object(OBJECT *obj);
	char *strip_right_white(char *b);
	std::string forward_slashes(const char *a);
	void filename_parts(const char *fullname, char *path, char *name, char *ext);
	int append_init(const char* format,...);
	int append_code(const char* format,...);
	int append_global(const char* format,...);
	void mark_linex(const char *filename, int linenum);
	void mark_line(void);
	STATUS exec(const char *format,...);
	STATUS debugger(const char *target);
	std::string setup_class(CLASS *oclass);
	int write_file(FILE *fp, const char *data, ...);
	int mkdirs(const char *path);
	STATUS compile_code(CLASS *oclass, int64 functions);
	STATUS load_set_index(OBJECT *obj, OBJECTNUM id);
	OBJECT *load_get_index(OBJECTNUM id);
	OBJECT *get_next_unlinked(CLASS *oclass);
	void free_index(void);	
	UNRESOLVED *add_unresolved(OBJECT *by, PROPERTYTYPE ptype, void *ref, CLASS *oclass, char *id, char *file, unsigned int line, int flags);
	int resolve_object(UNRESOLVED *item, const char *filename, bool deferred);
	int resolve_double(UNRESOLVED *item, const char *context, bool deferred);
	STATUS resolve_list(UNRESOLVED *item, bool deferred);
public:
	STATUS load_resolve_all(bool deferred=false);
private:
	void start_parse(int &mm, int &m, int &n, int &l, int linenum);
	void syntax_error_here(const char *p);
	int white(PARSER);
	int pattern(PARSER, const char *pattern, char *result, int size);
	int literal(PARSER, const char *text);
	int quoted_string(PARSER,char *result, int size);
	int dashed_name(PARSER, char *result, int size);
	int name(PARSER, char *result, int size);
	int namelist(PARSER, char *result, int size);
	int variable_list(PARSER, char *result, int size);
	int property_list(PARSER, char *result, int size);
	int unitspec(PARSER, UNIT **unit);
	int unitsuffix(PARSER, UNIT **unit);
	int nameunit(PARSER,char *result,int size,UNIT **unit);
	int dotted_name(PARSER, char *result, int size);
	int hostname(PARSER, char *result, int size);
	int delim_value(PARSER, char *result, int size, const char *delims);
	int structured_value(PARSER, char *result, int size);
	int multiline_value(PARSER,char *result,int size);
	int value(PARSER, char *result, int size);
	int functional_int(PARSER, int64 *value);
	int integer(PARSER, int64 *value);
	int unsigned_integer(PARSER, unsigned int64 *value);
	int integer32(PARSER, int32 *value);
	int integer16(PARSER, int16 *value);
	int real_value(PARSER, double *value);
	int functional(PARSER, double *pValue);
	int rpnfunc(PARSER, int *val);
	int expression(PARSER, double *pValue, UNIT **unit, OBJECT *obj);
	int functional_unit(PARSER,double *pValue,UNIT **unit);
	int complex_value(PARSER, complex *pValue);
	int complex_unit(PARSER,complex *pValue,UNIT **unit);
	int time_value_seconds(PARSER, TIMESTAMP *t);
	int time_value_minutes(PARSER, TIMESTAMP *t);
	int time_value_hours(PARSER, TIMESTAMP *t);
	int time_value_days(PARSER, TIMESTAMP *t);
	int time_value_weeks(PARSER, TIMESTAMP *t);
	int time_value_years(PARSER, TIMESTAMP *t);
	int time_value_datetime(PARSER, TIMESTAMP *t);
	int time_value_datetimezone(PARSER, TIMESTAMP *t);
	int time_value_isodatetime(PARSER, TIMESTAMP *t);
	int time_value(PARSER, TIMESTAMP *t);
	int delta_time(PARSER, TIMESTAMP *t);
	double load_latitude(char *buffer);
	double load_longitude(char *buffer);
	int clock_properties(PARSER);
	int pathname(PARSER, char *path, int size);
	int expanded_value(const char *text, char *result, int size, const char *delims);
	int alternate_value(PARSER, char *value, int size);
	int line_spec(PARSER);
	int clock_block(PARSER);
	int module_properties(PARSER, MODULE *mod);
	int module_block(PARSER);
	int property_specs(PARSER, KEYWORD **keys);
	int property_type(PARSER, PROPERTYTYPE *ptype, KEYWORD **keys);
	int class_intrinsic_function_name(PARSER, CLASS *oclass, int64 *function, const char **ftype, const char **fname);
	int argument_list(PARSER, char *args, int size);
	int source_code(PARSER, char *code, int size);
	int class_intrinsic_function(PARSER, CLASS *oclass, int64 *functions, char *code, int size);
	int class_export_function(PARSER, CLASS *oclass, char *fname, int fsize, char *arglist, int asize, char *code, int csize);
	int class_explicit_declaration(PARSER, char *type, int size);
	int class_explicit_definition(PARSER, CLASS *oclass);
	int class_external_function(PARSER, CLASS *oclass, CLASS **eclass,char *fname, int fsize);
	int class_event_handler(PARSER, CLASS *oclass);
	int class_parent_definition(PARSER, CLASS *oclass);
	int class_properties(PARSER, CLASS *oclass, int64 *functions, char *initcode, int initsize);
	int class_block(PARSER);
	int set_flags(OBJECT *obj, char *propval);
	int is_int(PROPERTYTYPE pt);
	int schedule_ref(PARSER, SCHEDULE **sch);
	int property_ref(PARSER, TRANSFORMSOURCE *xstype, void **ref, OBJECT *from);
	int transform_source(PARSER, TRANSFORMSOURCE *xstype, void **source, OBJECT *from);
	int filter_transform(PARSER, TRANSFORMSOURCE *xstype, char *sources, size_t srcsize, char *filtername, size_t namesize, OBJECT *from);
	int external_transform(PARSER, TRANSFORMSOURCE *xstype, char *sources, size_t srcsize, char *functionname, size_t namesize, OBJECT *from);
public:
	int linear_transform(PARSER, TRANSFORMSOURCE *xstype, void **source, double *scale, double *bias, OBJECT *from);
private:
	void json_free(JSONDATA **data);
	bool json_append(JSONDATA **data, const char *name, size_t namelen, const char *value, size_t valuelen);
	int json_data(PARSER,JSONDATA **data);
	int json_block(PARSER, OBJECT *obj, const char *propname);
	int object_properties(PARSER, CLASS *oclass, OBJECT *obj);
	int object_name_id(PARSER,char *classname, int64 *id);
	int object_name_id_range(PARSER,char *classname, int64 *from, int64 *to);
	int object_name_id_count(PARSER,char *classname, int64 *count);
	int object_block(PARSER, OBJECT *parent, OBJECT **subobj);
	int import(PARSER);
	int export_model(PARSER);
	int library(PARSER);
	int comment_block(PARSER);
	int schedule(PARSER);
	int linkage_term(PARSER,::instance *inst);
	int instance_block(PARSER);
	int gnuplot(PARSER, GUIENTITY *entity);
	int gui_link_globalvar(PARSER, GLOBALVAR **var);
	int gui_entity_parameter(PARSER, GUIENTITY *entity);
	int gui_entity_action(PARSER, GUIENTITY *parent);
	int gui_entity_type(PARSER, GUIENTITYTYPE *type);
	int gui_entity(PARSER, GUIENTITY *parent);
	int gui(PARSER);
	int C_code_block(PARSER, char *buffer, int size);
	int filter_name(PARSER, char *result, int size);
	int double_timestep(PARSER,double *step);
	int filter_mononomial(PARSER,char *domain,double *a, unsigned int *n);
	int filter_polynomial(PARSER,char *domain,double *a,unsigned int *n);
	int filter_option(PARSER, unsigned int64 *flags, unsigned int64 *resolution, double *minimum, double *maximum);
	int filter_block(PARSER);
	int extern_block(PARSER);
	int global_declaration(PARSER);
	int link_declaration(PARSER);
	int script_directive(PARSER);
	int dump_directive(PARSER);
	int modify_directive(PARSER);
	void loader_addhook(PARSERCALL call);
	int loader_hook(PARSER);
	int gridlabd_file(PARSER);
	int replace_variables(char *to,char *from,int len,int warn);
	int buffer_read(FILE *fp, char *buffer, char *filename, int size);
	inline FORLOOPSTATE for_get_state(void) { return forloopstate; };
	FORLOOPSTATE for_set_state(FORLOOPSTATE n);
	const char * for_setvar(void);
	bool for_is_state(FORLOOPSTATE n);
	bool for_open(const char *var, const char *range);
	bool for_capture(const char *line);
	const char *for_replay(void);
	int set_language(const char *name);
	inline const LANGUAGE *get_language(void) { return language; };
	int buffer_read_alt(FILE *fp, char *buffer, char *filename, int size);
	int include_file(char *incname, char *buffer, int size, int _linenum);
	int process_macro(char *line, int size, char *_filename, int linenum);
	static void kill_processes(void);
	static void wait_processes(void);
	void* start_process(const char *cmd);
	void load_add_language(const char *name, bool (*parser)(const char*,void *context), void* (*init)(int,const char**)=NULL);
public:
	STATUS loadall_glm(const char *file);
private:
	TECHNOLOGYREADINESSLEVEL calculate_trl(void);
	bool load_import(const char *from, char *to, int len);
	STATUS load_python(const char *filename);
	STATUS loadall(const char *fname);
	void add_depend(const char *filename, const char *dependency);
	void clear_last_term(void);
	void set_last_term(const char *p);
	void save_last_term(const char *p);
	const char *get_last_term(void);
private:
	void inc_linenum() { linenum++; global_loader_linenum = linenum; };
public:
	void sublime_syntax(void);
};

#endif
