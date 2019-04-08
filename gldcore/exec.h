/** $Id: exec.h 4738 2014-07-03 00:55:39Z dchassin $
	Copyright (C) 2008 Battelle Memorial Institute
	@file exec.h
	@addtogroup exec
	@ingroup core
 @{
 **/


#ifndef _EXEC_H
#define _EXEC_H

#include <setjmp.h>
#include "globals.h"
#include "index.h"
#include "object.h"
#include "threadpool.h"
#include "lock.h"

DEPRECATED struct sync_data {
	TIMESTAMP step_to; /**< time to advance to */
	unsigned int hard_event; /**< non-zero for hard events that can effect the advance step-to */
	STATUS status; /**< the current status */
}; /**< the synchronization state structure */

DEPRECATED struct thread_data {
	int count; /**< the thread count */
	struct sync_data *data; /**< pointer to the sync state structure */
};

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
DEPRECATED CDECL int exec_get_passtype(int pass);

// TODO: replace with C++ std list
DEPRECATED typedef struct s_simplelist 
{
	char *data;
	struct s_simplelist *next;
} SIMPLELIST;

// TODO: replace with C++ std list
DEPRECATED typedef struct s_simplelinklist 
{ 
	void *data;
	struct s_simplelinklist *next;
} SIMPLELINKLIST;

DEPRECATED typedef struct s_objsyncdata 
{
	GldMain *main;
	unsigned int n; // thread id 0~n_threads for this object rank list
	pthread_t pt;
	bool ok;
	//void *item;
	LISTITEM *ls;
	unsigned int nObj; // number of obj in this object rank list
	unsigned int t0;
	int i; // index of mutex or cond this object rank list uses 
} OBJSYNCDATA;

DEPRECATED struct arg_data {
	int thread;
	void *item;
	int incr;
};

class GldExec
{
private:
	GldMain &instance;
	char dumpfile[1024];
	TIMESTAMP dumpinterval;
	INDEX **ranks;
	struct thread_data *thread_data;
	clock_t cstart, cend;
	struct arg_data *arg_data_array;
	OBJECT **object_heartbeats;
	unsigned int n_object_heartbeats;
	unsigned int max_object_heartbeats;
	SIMPLELINKLIST *commit_list[2];
	SIMPLELIST *create_scripts;
	SIMPLELIST *init_scripts;
	SIMPLELIST *precommit_scripts;
	SIMPLELIST *sync_scripts;
	SIMPLELIST *commit_scripts;
	SIMPLELIST *term_scripts;
	SIMPLELIST *script_exports;
	pthread_mutex_t mls_svr_lock;
	pthread_cond_t mls_svr_signal;
	int mls_created;
	int mls_destroyed;
	pthread_mutex_t *startlock;
	pthread_mutex_t *donelock;
	pthread_cond_t *start;
	pthread_cond_t *done;
	unsigned int *next_t1;
	unsigned int *donecount;
	unsigned int *n_threads;
	sync_data main_sync;
	LOCKVAR sync_lock;
	double realtime_metric_decay;
	unsigned int pass;
public:
	inline GldMain *get_instance() { return &instance; };

public: // TODO: make private once lock.cpp is reentrant
	static const PASSCONFIG passtype[3];
	size_t iteration_counter;
#ifndef NOLOCKS
	int64 rlock_count;
	int64 rlock_spin;
	int64 wlock_count;
	int64 wlock_spin;
#endif

public:
	GldExec(GldMain *main);
	~GldExec(void);

public:
	static STATUS show_progress(void);

public:
	void free_simplelist(SIMPLELIST *list);
	void free_simplelinklist(SIMPLELINKLIST *list);
	struct thread_data *create_threaddata(size_t count);
	struct arg_data *create_argdata(size_t count);
	int schedule_dump(TIMESTAMP interval, const char *filename = "gridlabd.json");
	void run_dump(void);
	// ranks
	inline INDEX **getranks(void) { return ranks;};
	void initranks(void);
	inline void setranks(INDEX **ptr) { ranks = ptr;};
	// threads
	inline struct thread_data *get_thread_data(void) { return thread_data;};
	void init_thread_data(void);
	int setexitcode(int xc);
	int getexitcode(void);
	const char *getexitcodestr(EXITCODE xc);
	int64 clock(void);
	int init(void);
	STATUS setup_ranks(void);
	const char *simtime(void);
	void do_checkpoint(void);
	void ss_do_object_sync(int thread, void *item);
	void *ss_do_object_sync_list(void *threadarg);
	STATUS init_by_creation();
	STATUS init_by_deferral_retry(OBJECT **def_array, int def_ct);
	STATUS init_by_deferral();
	STATUS init_all(void);
	STATUS precommit_all(TIMESTAMP t0);
	int commit_init(void);
	MTIITEM commit_get0(MTIITEM item);
	MTIITEM commit_get1(MTIITEM item);
	MTIDATA commit_set(MTIDATA to, MTIDATA from);
	void commit_call(MTIDATA output, MTIITEM item, MTIDATA input);
	int commit_compare(MTIDATA a, MTIDATA b);
	void commit_gather(MTIDATA a, MTIDATA b);
	int commit_reject(MTI *mti, MTIDATA value);
	TIMESTAMP commit_all_st(TIMESTAMP t0, TIMESTAMP t2);
	TIMESTAMP commit_all(TIMESTAMP t0, TIMESTAMP t2);
	STATUS finalize_all();
	STATUS t_sync_all(PASSCONFIG pass);
	TIMESTAMP sync_heartbeats(void);
	TIMESTAMP syncall_internals(TIMESTAMP t1);
	void sleep(unsigned int usec);
	void *obj_syncproc(OBJSYNCDATA *data);
	void mls_create(void);
	void mls_init(void);
	void mls_start(void);
	void mls_suspend(void);
	void mls_resume(TIMESTAMP ts);
	void mls_statewait(unsigned states);
	void mls_done(void);
	void sync_reset(struct sync_data *d);
	void sync_merge(struct sync_data *to, struct sync_data *from);
	void sync_set(struct sync_data *d, TIMESTAMP t, bool deltaflag);
	TIMESTAMP sync_get(struct sync_data *d);
	unsigned int sync_getevents(struct sync_data *d);
	int sync_ishard(struct sync_data *d);
	int sync_isnever(struct sync_data *d);
	int sync_isinvalid(struct sync_data *d);
	STATUS sync_getstatus(struct sync_data *d);
	bool sync_isrunning(struct sync_data *d);
	void clock_update_modules();
	void rlock_sync(void);
	void runlock_sync(void);
	void wlock_sync(void);
	void wunlock_sync(void);
	void create_lockdata(int nObjRankList);
	STATUS exec_start(void);
	STATUS test(struct sync_data *data, int pass, OBJECT *obj);
	void *slave_node_proc(void *args);
	void slave_node();
	int add_scriptexport(const char *name);
	int update_exports(void);
	int add_script(SIMPLELIST **list, const char *file);
	EXITCODE run_system_script(char *call);
	EXITCODE run_gridlabd_script(char *call);
	EXITCODE run_scripts(SIMPLELIST *list);
	int add_createscript(const char *file);
	int add_initscript(const char *file);
	int add_syncscript(const char *file);
	int add_precommitscript(const char *file);
	int add_commitscript(const char *file);
	int add_termscript(const char *file);
	int run_createscripts(void);
	int run_initscripts(void);
	int run_precommitscripts(void);
	int run_syncscripts(void);
	int run_commitscripts(void);
	int run_termscripts(void);
};

#endif

/**@}*/
