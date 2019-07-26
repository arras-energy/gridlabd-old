/* File: exec.h 
 * Copyright (C) 2008, Battelle Memorial Institute
 */

#ifndef _EXEC_H
#define _EXEC_H

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

#include "index.h"
#include "threadpool.h"

/* Structure: sync_data
	Synchronization state data
 */
DEPRECATED struct sync_data {

	/* Field: step_to
		Time to advance to 
	 */
	TIMESTAMP step_to; 

	/* Field: hard_event
		Hard event flag -- non-zero for hard events that can effect the advance step-to
	 */
	unsigned int hard_event;

	/* Field: status
		The current synchronization status
	 */
	STATUS status; 
}; 

/* Structure: thread_data
	Thread control data
 */
DEPRECATED struct thread_data {

	/* Field: count
		The thread count
	 */
	int count; 

	/* Field: sync_data
		Thread sync data
	 */
	struct sync_data *data; 
};

/* TODO: these should be replaced with calls to GldExec methods */
DEPRECATED CDECL int exec_init(void);
DEPRECATED CDECL STATUS exec_start(void);
DEPRECATED CDECL const char *exec_simtime(void);
DEPRECATED CDECL STATUS t_setup_ranks(void);
DEPRECATED CDECL INDEX **exec_getranks(void);
DEPRECATED CDECL void exec_sleep(unsigned int usec);
DEPRECATED CDECL int64 exec_clock(void);
DEPRECATED CDECL void exec_mls_create(void);
DEPRECATED CDECL void exec_mls_init(void);
DEPRECATED CDECL void exec_mls_suspend(void);
DEPRECATED CDECL void exec_mls_resume(TIMESTAMP next_pause);
DEPRECATED CDECL void exec_mls_done(void);
DEPRECATED CDECL void exec_mls_statewait(unsigned states);
DEPRECATED CDECL void exec_slave_node();
DEPRECATED CDECL int exec_run_createscripts(void);
DEPRECATED CDECL void exec_sync_reset(struct sync_data *d);
DEPRECATED CDECL void exec_sync_merge(struct sync_data *to, struct sync_data *from);
DEPRECATED CDECL void exec_sync_set(struct sync_data *d, TIMESTAMP t,bool deltaflag);
DEPRECATED CDECL TIMESTAMP exec_sync_get(struct sync_data *d);
DEPRECATED CDECL unsigned int exec_sync_getevents(struct sync_data *d);
DEPRECATED CDECL int exec_sync_ishard(struct sync_data *d);
DEPRECATED CDECL int exec_sync_isnever(struct sync_data *d);
DEPRECATED CDECL int exec_sync_isinvalid(struct sync_data *d);
DEPRECATED CDECL STATUS exec_sync_getstatus(struct sync_data *d);
DEPRECATED CDECL EXITCODE exec_setexitcode(EXITCODE);
DEPRECATED EXITCODE exec_getexitcode(void);
DEPRECATED const char *exec_getexitcodestr(void);
DEPRECATED CDECL int exec_add_createscript(const char *file);
DEPRECATED CDECL int exec_add_initscript(const char *file);
DEPRECATED CDECL int exec_add_precommitscript(const char *file);
DEPRECATED CDECL int exec_add_syncscript(const char *file);
DEPRECATED CDECL int exec_add_commitscript(const char *file);
DEPRECATED CDECL int exec_add_termscript(const char *file);
DEPRECATED CDECL int exec_add_scriptexport(const char *file);
DEPRECATED CDECL EXITCODE exec_run_initscripts(void);
DEPRECATED CDECL EXITCODE exec_run_precommitscripts(void);
DEPRECATED CDECL EXITCODE exec_run_syncscripts(void);
DEPRECATED CDECL EXITCODE exec_run_commitscripts(void);
DEPRECATED CDECL EXITCODE exec_run_termscripts(void);
DEPRECATED CDECL int exec_schedule_dump(TIMESTAMP interval,char *filename);
DEPRECATED CDECL int64 exec_clock(void);
DEPRECATED CDECL void exec_rlock_sync(void);
DEPRECATED CDECL void exec_runlock_sync(void);
DEPRECATED CDECL void exec_wlock_sync(void);
DEPRECATED CDECL void exec_wunlock_sync(void);
DEPRECATED CDECL int exec_get_iteration_counter(void);
DEPRECATED CDECL PASSCONFIG exec_get_passtype(PASSCONFIG);

/* Typedef: SIMPLELIST
	See s_simplelist

	Structure: s_simplelist
		A simple link list data structure

	Todo: Replace with C++ std list
*/
DEPRECATED typedef struct s_simplelist 
{
	/* Field: data
		Reference to the data 
	 */
	char *data;

	/* Field: next
		Reference to the next list item
	 */
	struct s_simplelist *next;
} SIMPLELIST;

/* Typedef: SIMPLELINKLIST
	See s_simplelinklist

	Structure: s_simplelinklist
		A simple link list data structure

	Todo: Replace with C++ std list
*/
DEPRECATED typedef struct s_simplelinklist 
{ 
	/* Field: data
		Reference to the data 
	 */
	void *data;

	/* Field: next
		Reference to the next list item
	 */
	struct s_simplelinklist *next;
} SIMPLELINKLIST;

/*	Structure: s_objsyncdata

	Fields:
	main - main instance
	n - thread id 0~n_threads for this object rank list
	pt - thread data
	ok - thread status
	ls - list
	nOjb - number of obj in this object rank list
	t0 - start time
	i - index of mutex or cond this object rank list uses
 */
DEPRECATED typedef struct s_objsyncdata 
{
	GldMain *main;
	unsigned int n; 
	pthread_t pt;
	bool ok;
	LISTITEM *ls;
	unsigned int nObj; 
	unsigned int t0;
	int i; 
} OBJSYNCDATA;

/*	Structure: arg_data

	Fields:
	thread - thread id
	item - reference to object data
	incr - increment
 */
DEPRECATED struct arg_data {
	int thread;
	void *item;
	int incr;
};

/* 	Class: GldExec
	
	Simulation execution flow control class
 */
class GldExec
{
private:
	/* Field: instance
		Reference to the GldMain simulation instance
	 */
	GldMain &instance;

	/* Field: dumpfile
		Name of file to which dump is written
	 */
	char dumpfile[1024];

	/* Field: dumpinterval
		Time delay between dumps
	 */
	TIMESTAMP dumpinterval;

	/* Field: ranks
		List of indexes used to manage object ranks
	 */
	INDEX **ranks;

	/* Field: thread_data
		Reference to thread control data
	 */
	struct thread_data *thread_data;

	/* Field: cstart
		Multithreading start time
	 */
	clock_t cstart;

	/* Field: cend
		Multithreading end time
	 */
	clock_t cend;

	/* Field: arg_data_array
		Reference to multithreading argument data
	 */
	struct arg_data *arg_data_array;

	/* Field: object_heartbeats
		Object heartbeat working data
	 */
	OBJECT **object_heartbeats;

	/* Field: n_object_heartbeats
		Number of object heartbeat data items
	 */
	unsigned int n_object_heartbeats;

	/* Field: max_object_heartbeats
		Maximum number of object heartbeat data items
	 */
	unsigned int max_object_heartbeats;

	/* Field: commit_list
		Commit pass data lists
	 */
	SIMPLELINKLIST *commit_list[2];

	/* Field: create_scripts
		Create script call list
	 */
	SIMPLELIST *create_scripts;

	/* Field: init_scripts
		Initialization script call list
	 */
	SIMPLELIST *init_scripts;

	/* Field: precommit_scripts
		Pre-commit script call list
	 */
	SIMPLELIST *precommit_scripts;

	/* Field: sync_scripts
		Sync script call list
	 */
	SIMPLELIST *sync_scripts;

	/* Field: commit_scripts
		Commit script call list
	 */
	SIMPLELIST *commit_scripts;

	/* Field: term_scripts
		Termination script call list
	 */
	SIMPLELIST *term_scripts;

	/* Field: script_exports
		Export script call list
	 */
	SIMPLELIST *script_exports;

	/* Field: mls_svr_lock
		Mainloop status server lock variable
	 */
	pthread_mutex_t mls_svr_lock;

	/* Field: mls_svr_signal
		Mainloop status signal variable
	 */
	pthread_cond_t mls_svr_signal;

	/* Field: mls_created
		Flag to indicate mainloop status is created
	 */
	int mls_created;

	/* Field: mls_destroyed
		Flag to indicate mainloop status is destroyed
	 */
	int mls_destroyed;

	/* Field: startlock
		Execution startup lock variable
	 */
	pthread_mutex_t *startlock;

	/* Field: donelock
		Execution done lock variable
	 */
	pthread_mutex_t *donelock;

	/* Field: start
		Mutex condition for start lock
	 */
	pthread_cond_t *start;

	/* Field: done
		Mutex condition for done lock
	 */
	pthread_cond_t *done;

	/* Field: next_t1
		Reference to timestamp values for threads
	 */
	unsigned int *next_t1;

	/* Field: donecount
		Reference to counters for threads
	 */
	unsigned int *donecount;

	/* Field: n_threads
		Reference to number of threads
	 */
	unsigned int *n_threads;

	/* Field: main_sync
		Main time synchronization data
	 */
	sync_data main_sync;

	/* Field: sync_lock
		Lock for main_sync
	 */
	LOCKVAR sync_lock;

	/* Field: realtime_metric_decay
		Measured fraction of execution control idle time
	 */
	double realtime_metric_decay;

	/* Field: pass
		Pass type number
	 */
	unsigned int pass;
public:
	/* Method: get_instance
		Get a reference to GldMain simulation instance
	 */
	inline GldMain *get_instance() { return &instance; };

public: // TODO: make private once lock.cpp is reentrant

	/* Field: passtype
		Pass type array
	 */
	static const PASSCONFIG passtype[3];

	/* Field: iteration_counter
		Counter of iteration
	 */
	size_t iteration_counter;

public:
	/* Constructor: GldExec
		Construct an execution controller for a simulation instance
	 */
	GldExec(GldMain *main);

	/* Destructor: ~GldExec
		Destroys an execution controller for a simulation instance
	 */
	~GldExec(void);

public:
	/* Method: show_progress
		Sends current progress to the appropriate output stream
	 */
	static STATUS show_progress(void);

public:

	/*	Method: free_simplelist
			Free a simple list
	 */
	DEPRECATED void free_simplelist(SIMPLELIST *list);

	/*	Method: free_simplelinklist
			Free a simple link list
	 */
	DEPRECATED void free_simplelinklist(SIMPLELINKLIST *list);

	/*	Method: create_threaddata
			Create thread data needed for multithreaded operation

		Returns:
			Reference to thread data structure
	*/
	struct thread_data *create_threaddata(size_t count);

	/*	Method: 
			
		Returns:

	*/
	struct arg_data *create_argdata(size_t count);

	/*	Method: 
			
		Returns:

	*/
	int schedule_dump(TIMESTAMP interval, const char *filename = "gridlabd.json");

	/*	Method: 
			
		Returns:

	*/
	void run_dump(void);

	/*	Method: 
			
		Returns:

	*/
	inline INDEX **getranks(void) { return ranks;};

	/*	Method: 
			
		Returns:

	*/
	void initranks(void);

	/*	Method: 
			
		Returns:

	*/
	inline void setranks(INDEX **ptr) { ranks = ptr;};

	/*	Method: 
			
		Returns:

	*/
	inline struct thread_data *get_thread_data(void) { return thread_data;};

	/*	Method: 
			
		Returns:

	*/
	void init_thread_data(void);

	/*	Method: 
			
		Returns:

	*/
	EXITCODE setexitcode(EXITCODE xc);

	/*	Method: 
			
		Returns:

	*/
	EXITCODE getexitcode(void);

	/*	Method: 
			
		Returns:

	*/
	const char *getexitcodestr(EXITCODE xc);

	/*	Method: 
			
		Returns:

	*/
	int64 clock(void);
	
	/*	Method: init
			Initialize the execution flow control system

		Returns: 
		0 - Failure
		1 - Success
	 */
	int init(void);

	/*	Method: 
			
		Returns:

	*/
	STATUS setup_ranks(void);

	/* 	Method: simtime
			Obtain the simulation global clock date/time
		
		Returns:
			const char * - The global clock formatted using <global_datetime_format>
	 */
	const char *simtime(void);

	/*	Method: 
			
		Returns:

	*/
	void do_checkpoint(void);

	/*	Method: 
			
		Returns:

	*/
	void ss_do_object_sync(int thread, void *item);

	/*	Method: 
			
		Returns:

	*/
	void *ss_do_object_sync_list(void *threadarg);

	/*	Method: 
			
		Returns:

	*/
	STATUS init_by_creation();

	/*	Method: 
			
		Returns:

	*/
	STATUS init_by_deferral_retry(OBJECT **def_array, int def_ct);

	/*	Method: 
			
		Returns:

	*/
	STATUS init_by_deferral();

	/*	Method: 
			
		Returns:

	*/
	STATUS init_all(void);

	/*	Method: 
			
		Returns:

	*/
	STATUS precommit_all(TIMESTAMP t0);

	/*	Method: 
			
		Returns:

	*/
	int commit_init(void);

	/*	Method: 
			
		Returns:

	*/
	MTIITEM commit_get0(MTIITEM item);

	/*	Method: 
			
		Returns:

	*/
	MTIITEM commit_get1(MTIITEM item);

	/*	Method: 
			
		Returns:

	*/
	MTIDATA commit_set(MTIDATA to, MTIDATA from);

	/*	Method: 
			
		Returns:

	*/
	void commit_call(MTIDATA output, MTIITEM item, MTIDATA input);

	/*	Method: 
			
		Returns:

	*/
	int commit_compare(MTIDATA a, MTIDATA b);

	/*	Method: 
			
		Returns:

	*/
	void commit_gather(MTIDATA a, MTIDATA b);

	/*	Method: 
			
		Returns:

	*/
	int commit_reject(MTI *mti, MTIDATA value);

	/*	Method: 
			
		Returns:

	*/
	TIMESTAMP commit_all_st(TIMESTAMP t0, TIMESTAMP t2);

	/*	Method: 
			
		Returns:

	*/
	TIMESTAMP commit_all(TIMESTAMP t0, TIMESTAMP t2);

	/*	Method: 
			
		Returns:

	*/
	STATUS finalize_all();

	/*	Method: 
			
		Returns:

	*/
	STATUS t_sync_all(PASSCONFIG pass);

	/*	Method: 
			
		Returns:

	*/
	TIMESTAMP sync_heartbeats(void);

	/*	Method: 
			
		Returns:

	*/
	TIMESTAMP syncall_internals(TIMESTAMP t1);

	/*	Method: 
			
		Returns:

	*/
	void sleep(unsigned int usec);

	/*	Method: 
			
		Returns:

	*/
	void *obj_syncproc(OBJSYNCDATA *data);

	/*	Method: 
			
		Returns:

	*/
	void mls_create(void);

	/*	Method: 
			
		Returns:

	*/
	void mls_init(void);

	/*	Method: 
			
		Returns:

	*/
	void mls_start(void);

	/*	Method: 
			
		Returns:

	*/
	void mls_suspend(void);

	/*	Method: 
			
		Returns:

	*/
	void mls_resume(TIMESTAMP ts);

	/*	Method: 
			
		Returns:

	*/
	void mls_statewait(unsigned states);

	/*	Method: 
			
		Returns:

	*/
	void mls_done(void);

	/*	Method: 
			
		Returns:

	*/
	void sync_reset(struct sync_data *d);

	/*	Method: 
			
		Returns:

	*/
	void sync_merge(struct sync_data *to, struct sync_data *from);

	/*	Method: 
			
		Returns:

	*/
	void sync_set(struct sync_data *d, TIMESTAMP t, bool deltaflag);

	/*	Method: 
			
		Returns:

	*/
	TIMESTAMP sync_get(struct sync_data *d);

	/*	Method: 
			
		Returns:

	*/
	unsigned int sync_getevents(struct sync_data *d);

	/*	Method: 
			
		Returns:

	*/
	int sync_ishard(struct sync_data *d);

	/*	Method: 
			
		Returns:

	*/
	int sync_isnever(struct sync_data *d);

	/*	Method: 
			
		Returns:

	*/
	int sync_isinvalid(struct sync_data *d);

	/*	Method: 
			
		Returns:

	*/
	STATUS sync_getstatus(struct sync_data *d);

	/*	Method: 
			
		Returns:

	*/
	bool sync_isrunning(struct sync_data *d);

	/*	Method: 
			
		Returns:

	*/
	void clock_update_modules();

	/*	Method: 
			
		Returns:

	*/
	void rlock_sync(void);

	/*	Method: 
			
		Returns:

	*/
	void runlock_sync(void);

	/*	Method: 
			
		Returns:

	*/
	void wlock_sync(void);

	/*	Method: 
			
		Returns:

	*/
	void wunlock_sync(void);

	/*	Method: 
			
		Returns:

	*/
	void create_lockdata(int nObjRankList);

	/*	Method: exec_start
			Start the execution flow control system

		Returns: 
		SUCCESS - Start succeeded
		FAILED - Start failed
	 */
	STATUS exec_start(void);

	/*	Method: 
			
		Returns:

	*/
	STATUS test(struct sync_data *data, PASSCONFIG pass, OBJECT *obj);

	/*	Method: 
			
		Returns:

	*/
	void *slave_node_proc(void *args);

	/*	Method: 
			
		Returns:

	*/
	void slave_node();

	/*	Method: 
			
		Returns:

	*/
	int add_scriptexport(const char *name);

	/*	Method: 
			
		Returns:

	*/
	int update_exports(void);

	/*	Method: 
			
		Returns:

	*/
	int add_script(SIMPLELIST **list, const char *file);

	/*	Method: 
			
		Returns:

	*/
	EXITCODE run_system_script(char *call);

	/*	Method: 
			
		Returns:

	*/
	EXITCODE run_gridlabd_script(char *call);

	/*	Method: 
			
		Returns:

	*/
	EXITCODE run_scripts(SIMPLELIST *list);

	/*	Method: 
			
		Returns:

	*/
	int add_createscript(const char *file);

	/*	Method: 
			
		Returns:

	*/
	int add_initscript(const char *file);

	/*	Method: 
			
		Returns:

	*/
	int add_syncscript(const char *file);

	/*	Method: 
			
		Returns:

	*/
	int add_precommitscript(const char *file);

	/*	Method: 
			
		Returns:

	*/
	int add_commitscript(const char *file);

	/*	Method: 
			
		Returns:

	*/
	int add_termscript(const char *file);

	/*	Method: 
			
		Returns:

	*/
	int run_createscripts(void);

	/*	Method: 
			
		Returns:

	*/
	int run_initscripts(void);

	/*	Method: 
			
		Returns:

	*/
	int run_precommitscripts(void);

	/*	Method: 
			
		Returns:

	*/

	/*	Method: 
			
		Returns:

	*/
	int run_syncscripts(void);

	/*	Method: 
			
		Returns:

	*/
	int run_commitscripts(void);

	/*	Method: 
			
		Returns:

	*/
	int run_termscripts(void);
};

#endif

/**@}*/
