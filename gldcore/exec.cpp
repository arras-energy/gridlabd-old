/** exec.cpp
	Copyright (C) 2008 Battelle Memorial Institute
	
	@file exec.cpp
	@addtogroup exec Main execution loop
	@ingroup core
	
	The main execution loop sets up the main simulation, initializes the
	objects, and runs the simulation until it either settles to equilibrium
	or runs into a problem.  It also takes care multicore/multiprocessor
	parallelism when possible.  Objects of the same rank will be synchronized
	simultaneously, resources permitting.

	The main processing loop calls each object passing to it a TIMESTAMP
	indicating the desired synchronization time.  The sync() call attempts to
	advance the object's internal clock to the time indicated, and if successful it
	returns the time of the next expected change in the object's state.  An
	object state change is one which requires the equilibrium equations of
	the object to be updated.  When an object's state changes, all the other
	objects in the simulator are given an opportunity to consider the change
	and possibly alter the time of their next state change.  The core
	continues calling objects, advancing the global clock when
	necessary, and continuing in this way until all objects indicate that
	no further state changes are expected.  This is the equilibrium condition
	and the simulation consequently ends.

	\section exec_sync Sync Event API

	 Sync handling is done using an API implemented in #core_exec
	 There are several types of sync events that are handled.
	 - Hard sync is a sync time that should always be considered
	 - Soft sync is a sync time should only be considered when
	   the hard sync is not TS_NEVER
	 - Status is SUCCESS if the sync time is valid

	 Sync logic table:
@verbatim
 hard t
 
  sync_to    | t==INVALID  | t<sync_to   | t==sync_to  | sync_to<t<NEVER | t==NEVER
  ---------- | ----------- | ----------- | ----------- | --------------- | ----------
  INVALID    | INVALID     | INVALID     | INVALID     | INVALID         | INVALID
  soft       | INVALID     | t           | t           | sync_to         | sync_to
  hard       | INVALID     | t           | sync_to     | sync_to         | sync_to
  NEVER      | INVALID     | t           | sync_to     | sync_to         | NEVER
@endverbatim

@verbatim
 soft t
 
  sync_to    | t==INVALID  | t<sync_to   | t==sync_to  | sync_to<t<NEVER | t==NEVER
  ---------- | ----------- | ----------- | ----------- | --------------- | ----------
  INVALID    | INVALID     | INVALID     | INVALID     | INVALID         | INVALID
  soft       | INVALID     | t           | t           | sync_to         | sync_to
  hard       | INVALID     | t*          | t*          | sync_to         | sync_to
  NEVER      | INVALID     | t           | sync_to     | sync_to         | NEVER
 
 * indicates soft event is made hard
@endverbatim

    The Sync Event API functions are as follows:
	- #exec_sync_reset is used to reset a sync event to initial steady state sync (NEVER)
	- #exec_sync_merge is used to update an existing sync event with a new sync event
	- #exec_sync_set is used to set a sync event
	- #exec_sync_get is used to get a sync event
	- #exec_sync_getevents is used to get the number of hard events in a sync event
	- #exec_sync_getstatus is used to get the status of a sync event
	- #exec_sync_ishard is used to determine whether a sync event is a hard event
	- #exec_sync_isinvalid is used to determine whether a sync event is valid
	- #exec_sync_isnever is used to determine whether a sync event is pending

	@future [Chassin Oct'07]

	There is some value in exploring whether it is necessary to update all
	objects when a particular objects implements a state change.  The idea is
	based on the fact that updates propagate through the model based on known
	relations, such at the parent-child relation or the link-node relation.
	Consequently, it should obvious that unless a value in a related object
	has changed, there can be no significant change to an object that hasn't reached
	it's declared update time.  Thus only the object that "won" the next update
	time and those that are immediately related to it need be updated.  This 
	change could result in a very significant improvement in performance,
	particularly in models with many lightly coupled objects. 

 @{
 **/

#include "gldcore.h"

SET_MYCONTEXT(DMC_EXEC)

/* TODO: remove these when reentrant code is completed */
DEPRECATED extern GldMain *my_instance;
DEPRECATED static MTIITEM exec_commit_get0(MTIITEM item)
{
	return my_instance->get_exec()->commit_get0(item);
}
DEPRECATED static MTIITEM exec_commit_get1(MTIITEM item)
{
	return my_instance->get_exec()->commit_get1(item);
}
DEPRECATED static void exec_commit_call(MTIDATA output, MTIITEM item, MTIDATA input)
{
	my_instance->get_exec()->commit_call(output,item,input);
}
DEPRECATED static MTIDATA exec_commit_set(MTIDATA to, MTIDATA from)
{
	return my_instance->get_exec()->commit_set(to,from);
}
DEPRECATED static int exec_commit_compare(MTIDATA a, MTIDATA b)
{
	return my_instance->get_exec()->commit_compare(a,b);
}
DEPRECATED static void exec_commit_gather(MTIDATA a, MTIDATA b)
{
	my_instance->get_exec()->commit_gather(a,b);
}
DEPRECATED static int exec_commit_reject(MTI *mti, MTIDATA value)
{
	return my_instance->get_exec()->commit_reject(mti,value);
}
DEPRECATED static void *exec_obj_syncproc(void *ptr)
{
	OBJSYNCDATA *data = (OBJSYNCDATA*)ptr;
	return data->main->get_exec()->obj_syncproc(data);
}
DEPRECATED void *exec_slave_node_proc(void *args)
{
	return my_instance->get_exec()->slave_node_proc(args);
}

/* TODO: remove then debug.c is reentrant */
DEPRECATED const char *exec_simtime(void)
{
	return my_instance->get_exec()->simtime();
}
DEPRECATED int exec_get_iteration_counter(void)
{
	return my_instance->get_exec()->iteration_counter;
}
DEPRECATED PASSCONFIG exec_get_passtype(PASSCONFIG pass)
{
	return my_instance->get_exec()->passtype[pass];
}

/* TODO: remove when instance.c is reentrant */
DEPRECATED extern pthread_mutex_t mls_inst_lock;
DEPRECATED extern pthread_cond_t mls_inst_signal;

/* TODO: remove when load.c is reentrant */
DEPRECATED int exec_schedule_dump(TIMESTAMP interval,char *filename)
{
	return my_instance->get_exec()->schedule_dump(interval,filename);
}

/* TODO: remove when debug.c, instance_slave.c, server.c, and job.c are reentrant */
DEPRECATED EXITCODE exec_setexitcode(EXITCODE xc)
{
	return my_instance->get_exec()->setexitcode(xc);
}

/* TODO: remove when environment.c is reentrant */
DEPRECATED STATUS exec_start(void)
{
	return my_instance->get_exec()->exec_start();
}

/* TODO: remove when enduse.c, instance.c, loadshape.c, object.c, random.c, schedule.c, threadpool.c, and transform.c are reentrant */
DEPRECATED int64 exec_clock(void)
{
	return my_instance->get_exec()->clock();
}

/* TODO: remove when cmex.c is reentrant */
DEPRECATED int exec_init()
{
	return my_instance->get_exec()->init();
}

/* TODO: remove when save.c is reentrant */
DEPRECATED INDEX **exec_getranks(void)
{
	return my_instance->get_exec()->getranks();
}

/* TODO: remove when gui.c, test.c, and load.c are reentrant */
DEPRECATED void exec_sleep(unsigned int usec)
{
	my_instance->get_exec()->sleep(usec);
}
DEPRECATED int exec_add_scriptexport(const char *name)
{
	return my_instance->get_exec()->add_scriptexport(name);
}
DEPRECATED int exec_add_createscript(const char *file)
{
	return my_instance->get_exec()->add_createscript(file);
}
DEPRECATED int exec_add_initscript(const char *file)
{
	return my_instance->get_exec()->add_initscript(file);
}
DEPRECATED int exec_add_syncscript(const char *file)
{
	return my_instance->get_exec()->add_syncscript(file);
}
DEPRECATED int exec_add_precommitscript(const char *file)
{
	return my_instance->get_exec()->add_precommitscript(file);
}
DEPRECATED int exec_add_commitscript(const char *file)
{
	return my_instance->get_exec()->add_commitscript(file);
}
DEPRECATED int exec_add_termscript(const char *file)
{
	return my_instance->get_exec()->add_termscript(file);
}
EXITCODE exec_getexitcode(void)
{
	return my_instance->get_exec()->getexitcode();
}
const char *exec_getexitcodestr(void)
{
	return my_instance->get_exec()->getexitcodestr(global_exit_code);
}

/* TODO: remove when instance_slave.c is reentrant */
DEPRECATED void exec_mls_create(void)
{
	my_instance->get_exec()->mls_create();
}
DEPRECATED void exec_sync_reset(struct sync_data *d) /**< sync data to reset (NULL to reset main)  **/
{
	my_instance->get_exec()->sync_reset(d);
}
DEPRECATED void exec_sync_merge(struct sync_data *to, /**< sync data to merge to (NULL to update main)  **/
					struct sync_data *from) /**< sync data to merge from */
{
	my_instance->get_exec()->sync_merge(to,from);
}
DEPRECATED void exec_sync_set(struct sync_data *d, /**< sync data to update (NULL to update main) */
				  TIMESTAMP t,/**< timestamp to update with (negative time means soft event, 0 means failure) */
				  bool deltaflag)/**< flag to let us know this was a deltamode exit - force it forward, otherwise can fail to exit */
{
	my_instance->get_exec()->sync_set(d,t,deltaflag);
}
DEPRECATED TIMESTAMP exec_sync_get(struct sync_data *d) /**< Sync data to get sync time from (NULL to read main)  */
{
	return my_instance->get_exec()->sync_get(d);
}

/* TODO: remove when server.c is reentrant */
DEPRECATED void exec_mls_resume(TIMESTAMP ts)
{
	my_instance->get_exec()->mls_resume(ts);
}

/* TODO: remove when cmdarg.c is reentrant */
DEPRECATED void exec_slave_node()
{
	my_instance->get_exec()->slave_node();
}
DEPRECATED void throwf(const char *format, ...)
{
	char buffer[1024];
	va_list ptr;
	va_start(ptr,format);
	vsprintf(buffer,format,ptr);
	va_end(ptr);
	throw(buffer);
}

// TODO: remove when python.cpp is reentrant
DEPRECATED void exec_mls_done(void)
{
	my_instance->get_exec()->mls_done();
}
DEPRECATED void exec_mls_statewait(unsigned states)
{
	my_instance->get_exec()->mls_statewait(states);
}
DEPRECATED void exec_mls_suspend(void)
{
	my_instance->get_exec()->mls_suspend();
}
DEPRECATED void exec_rlock_sync(void)
{
	my_instance->get_exec()->rlock_sync();
}
DEPRECATED void exec_runlock_sync(void)
{
	my_instance->get_exec()->runlock_sync();
}
DEPRECATED void exec_wlock_sync(void)
{
	my_instance->get_exec()->wlock_sync();
}
DEPRECATED void exec_wunlock_sync(void)
{
	my_instance->get_exec()->wunlock_sync();
}



////////////////////////////////////////////
// GldExec implementation
////////////////////////////////////////////

const PASSCONFIG GldExec::passtype[3] = {PC_PRETOPDOWN, PC_BOTTOMUP, PC_POSTTOPDOWN};

GldExec::GldExec(GldMain *main)
	: instance(*main)
{
	memset(dumpfile,0,sizeof(dumpfile));
	dumpinterval = TS_NEVER;
	ranks = NULL;
	thread_data = NULL;
	cstart = 0;
	cend = 0;
	arg_data_array = NULL;
	object_heartbeats = NULL;
	n_object_heartbeats = 0;
	max_object_heartbeats = 0;
	memset(commit_list,0,sizeof(commit_list));
	create_scripts = NULL;
	init_scripts = NULL;
	precommit_scripts = NULL;
	sync_scripts = NULL;
	commit_scripts = NULL;
	term_scripts = NULL;
	pthread_mutex_init(&mls_svr_lock,NULL);
	pthread_cond_init(&mls_svr_signal,NULL);
	mls_created = 0;
	mls_destroyed = 0;
	startlock = NULL;
	donelock = NULL;
	start = NULL;
	done = NULL;
	next_t1 = NULL;
	donecount = NULL;
	n_threads = NULL; 
	main_sync.step_to = TS_NEVER;
	main_sync.hard_event = 0;
	main_sync.status = SUCCESS;
	sync_lock = 0;
	realtime_metric_decay = 0.9;
	script_exports = NULL;
	pass = 0;
	iteration_counter = 0;
}

#define PASSINIT(p) (p % 2 ? ranks[p]->first_used : ranks[p]->last_used)
#define PASSCMP(i, p) (p % 2 ? i <= ranks[p]->last_used : i >= ranks[p]->first_used)
#define PASSINC(p) (p % 2 ? 1 : -1)

GldExec::~GldExec(void)
{
	if ( object_heartbeats ) free(object_heartbeats);
	free_simplelinklist(commit_list[0]);
	free_simplelinklist(commit_list[1]);
	free_simplelist(create_scripts);
	free_simplelist(init_scripts);
	free_simplelist(precommit_scripts);
	free_simplelist(sync_scripts);
	free_simplelist(commit_scripts);
	free_simplelist(term_scripts);
	free_simplelist(script_exports);
	if ( thread_data ) free(thread_data);
	if ( arg_data_array ) free(arg_data_array);
	if ( next_t1 ) free(next_t1);
	if ( donecount ) free(donecount);
	if ( n_threads ) free(n_threads);
	if ( startlock ) free(startlock);
	if ( donelock ) free(donelock);
	if ( start ) free(start);
	if ( done ) free(done);
}

void GldExec::free_simplelist(SIMPLELIST *list)
{
	if ( list )
	{
		free_simplelist(list->next);
		if ( list->data ) free(list->data);
		free(list);
	}	
}

void GldExec::free_simplelinklist(SIMPLELINKLIST *list)
{
	if ( list )
	{
		free_simplelinklist(list->next);
		free(list);
	}
}

struct thread_data *GldExec::create_threaddata(size_t count)
{
	return (struct thread_data *)malloc(sizeof(struct thread_data)+sizeof(struct sync_data)*count);
}

struct arg_data *GldExec::create_argdata(size_t count)
{
	return (struct arg_data *)malloc(sizeof(struct arg_data)*count);
}

void GldExec::run_dump(void)
{
	if ( dumpfile[0] == '\0' || dumpinterval==TS_NEVER )
		return;
	else if ( ( dumpinterval == 0 || global_clock % dumpinterval == 0 ) && saveall(dumpfile) <= 0 )
	{
		char buffer[64];
		output_error("dump to %s at %s failed",dumpfile,convert_from_timestamp(global_clock,buffer,sizeof(buffer))?buffer:"unknown time");
	}
}

int GldExec::schedule_dump(TIMESTAMP interval, const char *filename)
{
	output_verbose("scheduling dump to %s every %d seconds",filename,interval);
	dumpinterval = interval;
	strcpy(dumpfile,filename);
	return 1;
}

void GldExec::init_thread_data(void)
{
	thread_data = create_threaddata(global_threadcount);
}

EXITCODE GldExec::setexitcode(EXITCODE xc)
{
	EXITCODE oldxc = (EXITCODE)global_exit_code;
	if ( oldxc != XC_SUCCESS && xc != oldxc )
	{
		output_warning("new exitcode %d overwrites existing exitcode %d", xc,oldxc);
	}
	global_exit_code = xc;
	IN_MYCONTEXT output_debug("exit code %d", xc);
	return oldxc;
}

EXITCODE GldExec::getexitcode(void)
{
	return global_exit_code;
}

const char *GldExec::getexitcodestr(EXITCODE xc)
{
	switch (xc) {
	case XC_SUCCESS: /* per system(3) */ return "ok";
	case XC_EXFAILED: /* exec/wait failure - per system(3) */ return "exec/wait failed";
	case XC_ARGERR: /* error processing command line arguments */ return "bad command";
	case XC_ENVERR: /* bad environment startup */ return "environment startup/load failed";
	case XC_TSTERR: /* test failed */ return "test failed";
	case XC_USRERR: /* user reject terms of use */ return "user rejected license terms";
	case XC_RUNERR: /* simulation did not complete as desired */ return "simulation failed";
	case XC_INIERR: /* initialization failed */ return "initialization failed";
	case XC_PRCERR: /* process control error */ return "process control error";
	case XC_SVRKLL: /* server killed */ return "server killed";
	case XC_IOERR: /* I/O error */ return "I/O error";
	case XC_LDERR: /* model load error */ return "load error";
	case XC_SHFAILED: /* shell failure - per system(3) */ return "shell failed";
	case XC_SIGNAL: /* signal caught - must be or'd with SIG value if known */ return "signal caught";
	case XC_SIGINT: /* SIGINT caught */ return "interrupt received";
	case XC_EXCEPTION: /* exception caught */ return "exception caught";
	default: return "unknown exception";
	}
}

int64 GldExec::clock(void)
{
	static struct timeb t0;
	struct timeb t1={0,0,0,0};
	if ( t0.time==0 )
	{
		ftime(&t0);
		t1 = t0;
	}
	else
		ftime(&t1);
	return (t1.time-t0.time)*CLOCKS_PER_SEC + (t1.millitm-t0.millitm)*CLOCKS_PER_SEC/1000;
}

/** The main system initialization sequence
	@return 1 on success, 0 on failure
 **/

int GldExec::init()
{
	/* set thread count equal to processor count if not passed on command-line */
	if (global_threadcount == 0)
		global_threadcount = processor_count();
	IN_MYCONTEXT output_verbose("detected %d processor(s)", processor_count());
	IN_MYCONTEXT output_verbose("using %d helper thread(s)", global_threadcount);

	/* setup clocks */
	if ( global_starttime==0 )
		global_starttime = realtime_now();

	/* set the start time */
	//global_clock = global_starttime + local_tzoffset(global_starttime);
	global_clock = global_starttime;

	/* save locale for simulation */
	locale_push();

	if (global_init()==FAILED)
		return 0;
	return 1;
}

void GldExec::initranks(void)
{
	size_t sz = sizeof(passtype)/sizeof(passtype[0]);
	INDEX **passlist = new INDEX*[sz+1];
	memset(passlist,0,sizeof(INDEX*)*(sz+1));
	setranks(passlist);
}

STATUS GldExec::setup_ranks(void)
{
	OBJECT *obj;
	size_t i;

	initranks();
	INDEX **ranks = getranks();

	/* create index object */
	ranks[0] = index_create(0,10);
	ranks[1] = index_create(0,10);
	ranks[2] = index_create(0,10);

	/* build the ranks of each pass type */
	for (i=0; i<sizeof(passtype)/sizeof(passtype[0]); i++)
	{
		if (ranks[i]==NULL)
			return FAILED;

		/* add every object to index based on rank */
		for (obj=object_get_first(); obj!=NULL; obj=object_get_next(obj))
		{
			/* ignore objects that don't use this passconfig */
			if ((obj->oclass->passconfig&passtype[i])==0 )
				continue;

			/* add this object to the ranks for this passconfig */
			if (index_insert(ranks[i],obj,obj->rank)==FAILED) 
				return FAILED;
			//sjin: print out obj id, pass, rank information
			//else 
			//	printf("obj[%d]: pass = %d, rank = %d\n", obj->id, passtype[i], obj->rank);
		}

		if (global_debug_mode==0 && global_nolocks==0)

			/* shuffle the objects in the index */
			index_shuffle(ranks[i]);
	}

	return SUCCESS;
}

const char *GldExec::simtime(void)
{
	static char buffer[64];
	return convert_from_timestamp(global_clock,buffer,sizeof(buffer))>0?buffer:"(invalid)";
}

STATUS GldExec::show_progress(void)
{
	output_progress();
	/* reschedule report */
	realtime_schedule_event(realtime_now()+1,show_progress);
	return SUCCESS;
}

/***********************************************************************/
/* CHECKPOINTS (DPC Apr 2011) */
void GldExec::do_checkpoint(void)
{
	/* last checkpoint value */
	static TIMESTAMP last_checkpoint = 0;
	TIMESTAMP now = 0;

	/* check point type selection */
	switch (global_checkpoint_type) {

	/* wallclock checkpoint interval */
	case CPT_WALL: 

		/* checkpoint based on wall time */
		now = time(NULL);

		/* default checkpoint for WALL */
		if ( global_checkpoint_interval==0 )
			global_checkpoint_interval = 3600;

		break;
	
		/* simulation checkpoint interval */
	case CPT_SIM: 

		/* checkpoint based on sim time */
		now = global_clock;

		/* default checkpoint for SIM */
		if ( global_checkpoint_interval==0 )
			global_checkpoint_interval = 86400;

		break;

	/* no checkpoints used */
	case CPT_NONE: 
		now = 0;
		break;
	}

	/* checkpoint may be needed */
	if ( now > 0 )
	{
		/* initial value of last checkpoint */
		if ( last_checkpoint==0 )
			last_checkpoint = now;

		/* checkpoint time lapsed */
		if ( last_checkpoint + global_checkpoint_interval <= now )
		{
			static char fn[1024] = "";
			FILE *fp = NULL;

			/* default checkpoint filename */
			if ( strcmp(global_checkpoint_file,"")==0 )
			{
				char *ext;

				/* use the model name by default */
				strcpy(global_checkpoint_file, global_modelname);
				ext = strrchr(global_checkpoint_file,'.');

				/* trim off the extension, if any */
				if ( ext!=NULL && ( strcmp(ext,".glm")==0 || strcmp(ext,".xml")==0 ) )
					*ext = '\0';
			}

			/* delete old checkpoint file if not desired */
			if ( global_checkpoint_keepall==0 && strcmp(fn,"")!=0 )
				unlink(fn);

			/* create current checkpoint save filename */
			sprintf(fn,"%s.%d",global_checkpoint_file,global_checkpoint_seqnum++);
			fp = fopen(fn,"w");
			if ( fp==NULL )
				output_error("unable to open checkpoint file '%s' for writing");
			else
			{
				if ( stream(fp,SF_OUT)<=0 )
					output_error("checkpoint failure (stream context is %s)",stream_context());
				fclose(fp);
				last_checkpoint = now;
			}
		}
	}

}

/***********************************************************************/
// implement new ss_do_object_sync for pthreads
void GldExec::ss_do_object_sync(int thread, void *item)
{
	struct thread_data *thread_data = get_thread_data();
	struct sync_data *data = &thread_data->data[thread];
	OBJECT *obj = (OBJECT *) item;
	TIMESTAMP this_t;
	char b[64];

	//printf("thread %d\t%d\t%s\n", thread, obj->rank, obj->name);
	//this_t = object_sync(obj, global_clock, passtype[pass]);

	/* check in and out-of-service dates */
	if (global_clock<obj->in_svc)
		this_t = obj->in_svc; /* yet to go in service */
	else if ((global_clock==obj->in_svc) && (obj->in_svc_micro != 0))	/* If our in service is a little higher, delay to next time */
		this_t = obj->in_svc + 1;	/* Technically yet to go into service -- deltamode handled separately */
	else if (global_clock<=obj->out_svc)
	{
		this_t = object_sync(obj, global_clock, passtype[pass]);
		if (this_t == global_clock)
		{
			IN_MYCONTEXT output_verbose("%s: object %s calling for re-sync", simtime(), object_name(obj, b, 63));
		}

#ifdef _DEBUG
		/* sync dumpfile */
		if (global_sync_dumpfile[0]!='\0')
		{
			static FILE *fp = NULL;
			if (fp==NULL)
			{
				static int tried = 0;
				if (!tried)
				{
					fp = fopen(global_sync_dumpfile,"wt");
					if (fp==NULL)
						output_error("sync_dumpfile '%s' is not writeable", global_sync_dumpfile);
					else
						fprintf(fp,"timestamp,pass,iteration,thread,object,sync\n");	
					tried = 1;
				}
			}
			if (fp!=NULL)
			{
				static int64 lasttime = 0;
				static char lastdate[64]="";
				char syncdate[64]="";
				static char *passname;
				static int lastpass = -1;
				char objname[1024];
				if (lastpass!=passtype[pass])
				{
					lastpass=passtype[pass];
					switch(lastpass) {
					case PC_PRETOPDOWN: passname = "PRESYNC"; break;
					case PC_BOTTOMUP: passname = "SYNC"; break;
					case PC_POSTTOPDOWN: passname = "POSTSYNC"; break;
					default: passname = "UNKNOWN"; break;
					}
				}
				if (lasttime!=global_clock)
				{
					lasttime = global_clock;
					convert_from_timestamp(global_clock,lastdate,sizeof(lastdate));
				}
				convert_from_timestamp(this_t<0?-this_t:this_t,syncdate,sizeof(syncdate));
				if (obj->name==NULL) sprintf(objname,"%s:%d", obj->oclass->name, obj->id);
				else strcpy(objname,obj->name);
				fprintf(fp,"%s,%s,%d,%d,%s,%s\n",lastdate,passname,global_iteration_limit-iteration_counter,thread,objname,syncdate);
			}
		}
#endif
	}
	else 
		this_t = TS_NEVER; /* already out of service */

	/* check for "soft" event (events that are ignored when stopping) */
	if (this_t < -1)
		this_t = -this_t;
	else if (this_t != TS_NEVER)
		data->hard_event++;  /* this counts the number of hard events */

	/* check for stopped clock */
	if (this_t < global_clock) {
		char b[64];
		output_error("%s: object %s stopped its clock (exec)!", simtime(), object_name(obj, b, 63));
		/* TROUBLESHOOT
			This indicates that one of the objects in the simulator has encountered a
			state where it cannot calculate the time to the next state.  This usually
			is caused by a bug in the module that implements that object's class.
		 */
		data->status = FAILED;
	} else {
		/* check for iteration limit approach */
		if (iteration_counter == 2 && this_t == global_clock) {
			char b[64];
			IN_MYCONTEXT output_verbose("%s: object %s iteration limit imminent",
								simtime(), object_name(obj, b, 63));
		}
		else if (iteration_counter == 1 && this_t == global_clock) {
			output_error("convergence iteration limit reached for object %s:%d", obj->oclass->name, obj->id);
			/* TROUBLESHOOT
				This indicates that the core's solver was unable to determine
				a steady state for all objects for any time horizon.  Identify
				the object that is causing the convergence problem and contact
				the developer of the module that implements that object's class.
			 */
		}

		/* manage minimum timestep */
		if (global_minimum_timestep>1 && this_t>global_clock && this_t<TS_NEVER)
			this_t = (((this_t-1)/global_minimum_timestep)+1)*global_minimum_timestep;

		/* if this event precedes next step, next step is now this event */
		if (data->step_to > this_t) {
			//LOCK(data);
			data->step_to = this_t;
			//UNLOCK(data);
		}
		//printf("data->step_to=%d, this_t=%d\n", data->step_to, this_t);
	}
}

// implement new ss_do_object_sync_list for pthreads
void *GldExec::ss_do_object_sync_list(void *threadarg)
{
	LISTITEM *ptr;
	int iPtr;

	struct arg_data *mydata = (struct arg_data *) threadarg;
	int thread = mydata->thread;
	LISTITEM *item = (LISTITEM*)mydata->item;
	int incr = mydata->incr;

	iPtr = 0;
	for (ptr = item; ptr != NULL; ptr=ptr->next) {
		if (iPtr < incr) {
			ss_do_object_sync(thread, ptr->data);
			iPtr++;
		}
	}
	return NULL;
}

STATUS GldExec::init_by_creation(void)
{
	OBJECT *obj;
	char b[64];
	STATUS rv = SUCCESS;
	try 
	{
		for ( obj = object_get_first() ; obj != NULL ; obj = object_get_next(obj) )
		{
			if ( object_init(obj) == FAILED )
			{
				memset(b, 0, 64);
				output_error("init_all(): object %s initialization failed", object_name(obj, b, 63));
				/* TROUBLESHOOT
					The initialization of the named object has failed.  Make sure that the object's
					requirements for initialization are satisfied and try again.
				 */
				rv = FAILED;
				break;
			}
			if ((obj->oclass->passconfig & PC_FORCE_NAME) == PC_FORCE_NAME)
			{
				if (0 == strcmp(obj->name, ""))
				{
					output_warning("init: object %s:%d should have a name, but doesn't", obj->oclass->name, obj->id);
					/* TROUBLESHOOT
					   The object indicated has been flagged by the module which implements its class as one which must be named
					   to work properly.  Please provide the object with a name and try again.
					 */
				}
			}
			obj->clock = global_starttime;
		}
	} 
	catch (const char *msg) 
	{
		output_error("init failure: %s", msg);
		/* TROUBLESHOOT
			The initialization procedure failed.  This is usually preceded 
			by a more detailed message that explains why it failed.  Follow
			the guidance for that message and try again.
		 */
		rv = FAILED;
	}
	
	return rv;
}

STATUS GldExec::init_by_deferral_retry(OBJECT **def_array, int def_ct)
{
	OBJECT *obj;
	int ct = 0, i = 0, obj_rv = 0;
	OBJECT **next_arr, **tarray;
	STATUS rv = SUCCESS;
	char b[64];
	int retry = 1, tries = 0;
	tarray = NULL;

	//Split out the malloc so it can be checked
	next_arr = (OBJECT **)malloc(def_ct * sizeof(OBJECT *));

	//Check it, like a proper programmer
	if (next_arr == NULL)
	{
		output_error("init_by_deferral_retry(): error allocating temporary array");
		return FAILED;
	}

	if (global_init_max_defer < 1)
	{
		output_warning("init_max_defer is less than 1, disabling deferred initialization");
	}
	while (retry)
	{
		if (global_init_max_defer <= tries)
		{
			output_error("init_by_deferral_retry(): exhausted initialization attempts");
			rv = FAILED;
			break;
		}

		//Zero the temp array AND its tracking variable
		memset(next_arr, 0, def_ct * sizeof(OBJECT *));
		ct = 0;

		// initialize each object in def_array
		for (i = 0; i < def_ct; ++i)
		{
			obj = def_array[i];
			obj_rv = object_init(obj);
			switch (obj_rv)
			{
				case 0:
					rv = FAILED;
					memset(b, 0, 64);
					output_error("init_by_deferral_retry(): object %s initialization failed", object_name(obj, b, 63));
					break;
				case 1:
					wlock(&obj->lock);
					obj->flags |= OF_INIT;
					obj->flags -= OF_DEFERRED;
					wunlock(&obj->lock);
					break;
				case 2:
					next_arr[ct] = obj;
					++ct;
					break;
				// no default
			}
			if (rv == FAILED)
			{
				free(next_arr);
				next_arr = NULL;
				return rv;
			}
		}

		if (ct == def_ct)
		{
			output_error("init_by_deferral_retry(): all uninitialized objects deferred, model is unable to initialize");
			rv = FAILED;
			retry = 0;

			//See if we iterated
			if (tries > 0)
			{
				//Yes - fix the pointers before leaving, otherwise we'll double-free things!
				tarray = def_array;
				def_array = next_arr;
				next_arr = tarray;
			}
			//Default else - we failed first try, so pointer swap-around below didn't occur
		}
		else if (0 == ct)
		{
			rv = SUCCESS;
			retry = 0;

			//See if we iterated
			if (tries > 0)
			{
				//Yes - fix the pointers before leaving, otherwise we'll double-free things!
				tarray = def_array;
				def_array = next_arr;
				next_arr = tarray;
			}
			//Default else - we succeeded first try, so pointer swap-around below didn't occur
		}
		else
		{
			++tries;
			retry = 1;
			tarray = next_arr;
			next_arr = def_array;
			def_array = tarray;
			def_ct = ct;
			// three-point turn to swap the 'next' and the 'old' arrays, memset 0'ing at the top, along with ct reset
		}
	}

	free(next_arr);
	next_arr = NULL;
	return rv;
}

STATUS GldExec::init_by_deferral()
{
	OBJECT **def_array = 0;
	int obj_rv = 0, def_ct = 0;
	OBJECT *obj = 0;
	STATUS rv = SUCCESS;
	char b[64];

	def_array = (OBJECT **)malloc(sizeof(OBJECT *) * object_get_count());

	//Check the malloc, like we should
	if (def_array == NULL)
	{
		output_error("init_by_deferral(): failed to allocate memory");
		return FAILED;
	}

	obj = object_get_first();
	while (obj != 0)
	{
		obj_rv = object_init(obj);
		switch (obj_rv)
		{
			case 0:
				rv = FAILED;
				memset(b, 0, 64);
				output_error("init_by_deferral(): object %s initialization failed", object_name(obj, b, 63));
				break;
			case 1:
				wlock(&obj->lock);
				obj->flags |= OF_INIT;
				wunlock(&obj->lock);
				break;
			case 2:
				def_array[def_ct] = obj;
				++def_ct;
				wlock(&obj->lock);
				obj->flags |= OF_DEFERRED;
				wunlock(&obj->lock);
				break;
			// no default
		}

		if (rv == FAILED)
		{
			free(def_array);
			def_array = NULL;
			return rv;
		}

		obj = obj->next;
	}

	// recursecursecursive
	if (def_ct > 0)
	{
		rv = init_by_deferral_retry(def_array, def_ct);
		if (rv == FAILED) // got hung up retrying
		{ 
			free(def_array);
			def_array = NULL;
			return FAILED;
		}
	}
	free(def_array);
	def_array = NULL;

	obj = object_get_first();
	while (obj != 0)
	{
		if ((obj->oclass->passconfig & PC_FORCE_NAME) == PC_FORCE_NAME)
		{
			if (0 == strcmp(obj->name, ""))
			{
				output_warning("init: object %s:%d should have a name, but doesn't", obj->oclass->name, obj->id);
				/* TROUBLESHOOT
				   The object indicated has been flagged by the module which implements its class as one which must be named
				   to work properly.  Please provide the object with a name and try again.
				 */
			}
		}
		obj = obj->next;
	}
	return SUCCESS;
}

STATUS GldExec::init_all(void)
{
	OBJECT *obj;
	STATUS rv = SUCCESS;
	IN_MYCONTEXT output_verbose("initializing objects...");

	/* run initialization callbacks */
	if ( run_initcalls(global_starttime) < 0 )
	{
		return FAILED;
	}

	/* initialize modules */
	if ( ! module_initall() )
	{
		return FAILED;
	}

	/* initialize instances */
	if ( instance_initall() == FAILED )
	{
		return FAILED;
	}

	/* initialize loadshapes */
	if (loadshape_initall()==FAILED || enduse_initall()==FAILED)
	{
		return FAILED;
	}

	switch (global_init_sequence)
	{
		case IS_CREATION:
			rv = init_by_creation();
			break;
		case IS_DEFERRED:
			rv = init_by_deferral();
			break;
		case IS_BOTTOMUP:
			output_fatal("Bottom-up rank-based initialization mode not yet supported");
			rv = FAILED;
			break;
		case IS_TOPDOWN:
			output_fatal("Top-down rank-based initialization mode not yet supported");
			rv = FAILED;
			break;
		default:
			output_fatal("Unrecognized initialization mode");
			rv = FAILED;
	}
	errno = EINVAL;
	if ( rv == FAILED ) 
	{
		return FAILED;
	}

	/* collect heartbeat objects */
	for ( obj = object_get_first() ; obj != NULL ; obj = obj->next )
	{
		/* this is a heartbeat object */
		if ( obj->heartbeat > 0 )
		{
			/* need more space */
			if ( n_object_heartbeats >= max_object_heartbeats )
			{
				OBJECT **bigger;
				int size = ( max_object_heartbeats==0 ? 256 : (max_object_heartbeats*2) );
				bigger = (OBJECT**)malloc(size*sizeof(OBJECT*));
				if ( bigger==NULL )
				{
					output_error("unsufficient memory to allocate hearbeat object list");
					return FAILED;
				}
				if ( max_object_heartbeats>0 )
				{
					memcpy(bigger,object_heartbeats,max_object_heartbeats*sizeof(OBJECT*));
					free(object_heartbeats);
				}
				object_heartbeats = bigger;
				max_object_heartbeats = size;
			}

			/* add this one */
			object_heartbeats[n_object_heartbeats++] = obj;
		}
	}

	/* initialize external links */
	return link_initall();
}


/*
 *	STATUS precommit(t0)
 *		This callback function allows an object to perform actions at the beginning
 *		of a timestep, before the sync process.  This callback is only triggered
 *		once per timestep, and will not fire between iterations.
 */

/**************************************************************************
 ** PRECOMMIT ITERATOR
 **************************************************************************/
STATUS GldExec::precommit_all(TIMESTAMP t0)
{
	STATUS rv=SUCCESS;
	static int first=1;
	/* TODO implement this multithreaded */
	static SIMPLELINKLIST *precommit_list = NULL;
	SIMPLELINKLIST *item;
	if ( first )
	{
		OBJECT *obj;
		for ( obj=object_get_first() ; obj!=NULL ; obj=object_get_next(obj) )
		{
			if ( obj->oclass->precommit!=NULL || obj->events.precommit!=NULL )
			{
				item = (SIMPLELINKLIST*)malloc(sizeof(SIMPLELINKLIST));
				if ( item==NULL )
				{
					char name[64];
					output_error("object %s precommit memory allocation failed", object_name(obj,name,sizeof(name)-1));
					/* TROUBLESHOOT
					   Insufficient memory remains to perform the precommit operation.
					   Free up memory and try again.
					 */
					return FAILED;
				}
				item->data = (void*)obj;
				item->next = precommit_list;
				precommit_list = item;
			}
		}
		first = 0;
	}

	try 
	{
		for ( item=precommit_list ; item!=NULL ; item=item->next )
		{
			OBJECT *obj = (OBJECT*)item->data;
			if ((obj->in_svc <= t0 && obj->out_svc >= t0) && (obj->in_svc_micro >= obj->out_svc_micro))
			{
				if ( object_precommit(obj, t0)==FAILED )
				{
					char name[64];
					output_error("object %s precommit failed", object_name(obj,name,sizeof(name)-1));
					/* TROUBLESHOOT
						The precommit function of the named object has failed.  Make sure that the object's
						requirements for precommit'ing are satisfied and try again.  (likely internal state aberations)
					 */
					rv=FAILED;
					break;
				}
			}
		}
	} 
	catch (const char *msg)
	{
		output_error("precommit_all() failure: %s", msg);
		/* TROUBLESHOOT
			The precommit'ing procedure failed.  This is usually preceded 
			by a more detailed message that explains why it failed.  Follow
			the guidance for that message and try again.
		 */
		rv=FAILED;
	}
	return ( rv && module_precommitall(t0) ) ? SUCCESS : FAILED;
}

/**************************************************************************
 ** COMMIT ITERATOR
 **************************************************************************/
/* initialize commit_list - must be called only once */
int GldExec::commit_init(void)
{
	int n_commits = 0;
	OBJECT *obj;
	SIMPLELINKLIST *item;

	/* build commit list */
	for ( obj=object_get_first() ; obj!=NULL ; obj=object_get_next(obj) )
	{
		if ( obj->oclass->commit!=NULL || obj->events.commit!=NULL )
		{
			/* separate observers */
			unsigned int pc = ((obj->oclass->passconfig&PC_OBSERVER)==PC_OBSERVER)?1:0;
			item = (SIMPLELINKLIST*)malloc(sizeof(SIMPLELINKLIST));
			if ( item==NULL ) 
				throw_exception("commit_init memory allocation failure");
			item->data = (void*)obj;
			item->next = commit_list[pc];
			commit_list[pc] = item;
			n_commits++;
		}
	}
	return n_commits;
}
/* commit_list iterator */
MTIITEM GldExec::commit_get0(MTIITEM item)
{
	if ( item==NULL )
		return (MTIITEM)commit_list[0];
	else
		return (MTIITEM)(((SIMPLELINKLIST*)item)->next);
}
MTIITEM GldExec::commit_get1(MTIITEM item)
{
	if ( item==NULL )
		return (MTIITEM)commit_list[1];
	else
		return (MTIITEM)(((SIMPLELINKLIST*)item)->next);
}
/* commit function call */
void GldExec::commit_call(MTIDATA output, MTIITEM item, MTIDATA input)
{
	OBJECT *obj = (OBJECT*)(((SIMPLELINKLIST*)item)->data);
	TIMESTAMP *t2 = (TIMESTAMP*)output;
	TIMESTAMP *t0 = (TIMESTAMP*)input;
	if ( *t0<obj->in_svc )
		*t2 = obj->in_svc;
	else if ((*t0 == obj->in_svc) && (obj->in_svc_micro != 0))
		*t2 = obj->in_svc + 1;
	else if ( obj->out_svc>=*t0 )
		*t2 = object_commit(obj,*t0,*t2);
	else
		*t2 = TS_NEVER;
}
/* commit data set accessor */
MTIDATA GldExec::commit_set(MTIDATA to, MTIDATA from)
{
	/* allocation request */
	// TODO: it's not clear how this malloc is freed
	if ( to==NULL ) to = (MTIDATA)malloc(sizeof(TIMESTAMP));

	/* clear request (may follow allocation request) */
	if ( from==NULL ) *(TIMESTAMP*)to = TS_NEVER;
	
	/* copy request */ 
	else memcpy(to,from,sizeof(TIMESTAMP));	

	return to;
}
/* commit data compare accessor */
int GldExec::commit_compare(MTIDATA a, MTIDATA b)
{
	TIMESTAMP t0 = (a?*(TIMESTAMP*)a:TS_NEVER);
	TIMESTAMP t1 = (b?*(TIMESTAMP*)b:TS_NEVER);
	if ( t0>t1  ) return 1;
	if ( t0<t1 ) return -1;
	return 0;
}
/* commit data gather accessor */
void GldExec::commit_gather(MTIDATA a, MTIDATA b)
{
	TIMESTAMP *t0 = (TIMESTAMP*)a;
	TIMESTAMP *t1 = (TIMESTAMP*)b;
	if ( a==NULL || b==NULL ) return;
	if ( *t1<*t0 ) *t0 = *t1;
}
/* commit iterator reject test */
int GldExec::commit_reject(MTI *mti, MTIDATA value)
{
	TIMESTAMP *t1 = (TIMESTAMP*)value;
	TIMESTAMP *t2 = (TIMESTAMP*)mti->output;
	if ( value==NULL ) return 0;
	return ( *t2>*t1 && *t2<TS_NEVER ) ? 1 : 0;
}
/* single threaded version of commit_all */
TIMESTAMP GldExec::commit_all_st(TIMESTAMP t0, TIMESTAMP t2)
{
	TIMESTAMP result = TS_NEVER;
	SIMPLELINKLIST *item;
	unsigned int pc;
	for ( pc=0 ; pc<2 ; pc ++ )
	{
		for ( item=commit_list[pc] ; item!=NULL ; item=item->next )
		{
			OBJECT *obj = (OBJECT*)item->data;
			if ( t0<obj->in_svc )
			{
				if ( obj->in_svc<result ) result = obj->in_svc;
			}
			else if ((t0 == obj->in_svc) && (obj->in_svc_micro != 0))
			{
				if (obj->in_svc == result)
					result = obj->in_svc + 1;
			}
			else if ( obj->out_svc>=t0 )
			{
				TIMESTAMP next = object_commit(obj,t0,t2);
				if ( next==TS_INVALID )
				{
					char name[64];
					throw_exception("object %s commit failed", object_name(obj,name,sizeof(name)-1));
					/* TROUBLESHOOT
						The commit function of the named object has failed.  Make sure that the object's
						requirements for committing are satisfied and try again.  (likely internal state aberations)
					 */
				}
				if ( next<result ) result = next;
			}
		}
	}
	return result;
}
/* multi-threaded version of commit_all */
TIMESTAMP GldExec::commit_all(TIMESTAMP t0, TIMESTAMP t2)
{
	static int n_commits = -1;
	static MTI *mti[] = {NULL,NULL};
	static int init_tried = FALSE;
	MTIDATA input = (MTIDATA)&t0;
	MTIDATA output = (MTIDATA)&t2;
	TIMESTAMP result = TS_NEVER;

	try 
	{
		unsigned int pc;

		/* build commit list */
		if ( n_commits==-1 ) n_commits = commit_init();

		/* if no commits found, stop here */
		if ( n_commits==0 )
		{
			result = TS_NEVER;
		}
		else
		{
			/* initialize MTI */
			for ( pc=0 ; pc<2 ; pc++ )
			{
				if ( mti[pc]==NULL && global_threadcount!=1 && !init_tried )
				{
					/* build mti */
					static MTIFUNCTIONS fns[] = {
						{exec_commit_get0, exec_commit_call, exec_commit_set, exec_commit_compare, exec_commit_gather, exec_commit_reject},
						{exec_commit_get1, exec_commit_call, exec_commit_set, exec_commit_compare, exec_commit_gather, exec_commit_reject},
					};
					mti[pc] = mti_init("commit",&fns[pc],8);
					if ( mti[pc]==NULL )
					{
						output_warning("commit_all multi-threaded iterator initialization failed - using single-threaded iterator as fallback");
						init_tried = TRUE;
					}
				}

				/* attempt to run multithreaded iterator */
				if ( mti[pc]!=NULL && mti_run(output,mti[pc],input) )
					result = *(TIMESTAMP*)output;

				/* resort to single threaded iterator (which handles both passes) */
				else if ( pc==0 )
					result = commit_all_st(t0,t2);
			}
		}
	}
	catch (const char *msg)
	{
		output_error("commit_all() failure: %s", msg);
		/* TROUBLESHOOT
			The commit'ing procedure failed.  This is usually preceded 
			by a more detailed message that explains why it failed.  Follow
			the guidance for that message and try again.
		 */
		result = TS_INVALID;
	}
	return result && module_commitall(t0) ? TS_NEVER : TS_INVALID;
}

/**************************************************************************
 ** FINALIZE ITERATOR
 **************************************************************************/
STATUS GldExec::finalize_all()
{
	STATUS rv=SUCCESS;
	static int first=1;
	/* TODO implement this multithreaded */
	static SIMPLELINKLIST *finalize_list = NULL;
	SIMPLELINKLIST *item;
	if ( first )
	{
		OBJECT *obj;
		for ( obj=object_get_first() ; obj!=NULL ; obj=object_get_next(obj) )
		{
			if ( obj->oclass->finalize!=NULL || obj->events.finalize!=NULL )
			{
				item = (SIMPLELINKLIST*)malloc(sizeof(SIMPLELINKLIST));
				if ( item==NULL )
				{
					char name[64];
					output_error("object %s finalize memory allocation failed", object_name(obj,name,sizeof(name)-1));
					/* TROUBLESHOOT
					   Insufficient memory remains to perform the finalize operation.
					   Free up memory and try again.
					 */
					return FAILED;
				}
				item->data = (void*)obj;
				item->next = finalize_list;
				finalize_list = item;
			}
		}
		first = 0;
	}

	try 
	{
		for ( item=finalize_list ; item!=NULL ; item=item->next )
		{
			OBJECT *obj = (OBJECT*)item->data;
			if ( object_finalize(obj)==FAILED )
			{
				char name[64];
				output_error("object %s finalize failed", object_name(obj,name,sizeof(name)-1));
				/* TROUBLESHOOT
					The finalize function of the named object has failed.  Make sure that the object's
					requirements for finalizing are satisfied and try again.  (likely internal state aberations)
				 */
				rv=FAILED;
				break;
			}
		}
	} 
	catch (const char *msg)
	{
		output_error("finalize_all() failure: %s", msg);
		/* TROUBLESHOOT
			The finalizing procedure failed.  This is usually preceded 
			by a more detailed message that explains why it failed.  Follow
			the guidance for that message and try again.
		 */
		rv=FAILED;
	}
	return rv;
}

STATUS GldExec::t_sync_all(PASSCONFIG pass)
{
	struct sync_data sync = {TS_NEVER,0,SUCCESS};
	int pass_index = ((int)(pass/2)); /* 1->0, 2->1, 4->2; NB: if a fourth pass is added this won't work right */
	INDEX **ranks = getranks();

	/* scan the ranks of objects */
	if (ranks[pass_index] != NULL)
	{
		int i;

		/* process object in order of rank using index */
		for (i = PASSINIT(pass_index); PASSCMP(i, pass_index); i += PASSINC(pass_index))
		{
			LISTITEM *item;
			/* skip empty lists */
			if (ranks[pass_index]->ordinal[i] == NULL) 
				continue;
			
			
			for (item=ranks[pass_index]->ordinal[i]->first; item!=NULL; item=item->next)
			{
				OBJECT *obj = (OBJECT*)(item->data);
				if (test(&sync,pass,obj)==FAILED)
					return FAILED;
			}
		}
	}

	/* run all non-schedule transforms */
	{
		TIMESTAMP st = transform_syncall(global_clock,(TRANSFORMSOURCE)(XS_ALL&(~(XS_SCHEDULE|XS_LOADSHAPE))));// if (abs(t)<t2) t2=t;
		if (st<sync.step_to)
			sync.step_to = st;
	}

	return SUCCESS;
}

TIMESTAMP GldExec::sync_heartbeats(void)
{
	TIMESTAMP t1 = TS_NEVER;
	unsigned int n;
	for ( n=0 ; n<n_object_heartbeats ; n++ )
	{
		TIMESTAMP t2 = object_heartbeat(object_heartbeats[n]);
		if ( absolute_timestamp(t2) < absolute_timestamp(t1) ) t1 = t2;
	}

	/* heartbeats are always soft updates */
	return t1<TS_NEVER ? -absolute_timestamp(t1) : TS_NEVER;
}

/* this function synchronizes all internal behaviors */
TIMESTAMP GldExec::syncall_internals(TIMESTAMP t1)
{
	TIMESTAMP h1, h2, s1, s2, s3, s4, s5, s6, se, sa;

	/* external link must be first */
	h1 = link_syncall(t1);

	/* @todo add other internal syncs here */
	h2 = instance_syncall(t1);	
	s1 = randomvar_syncall(t1);
	s2 = schedule_syncall(t1);
	s3 = loadshape_syncall(t1);
	s4 = transform_syncall(t1,(TRANSFORMSOURCE)(XS_ALL&(~(XS_SCHEDULE|XS_LOADSHAPE))));
	s5 = enduse_syncall(t1);

	/* heartbeats go last */
	s6 = sync_heartbeats();

	/* earliest soft event */
	se = absolute_timestamp(earliest_timestamp(s1,s2,s3,s4,s5,s6,TS_ZERO));

	/* final event */
	sa = earliest_timestamp(h1,h2,se!=TS_NEVER?-se:TS_NEVER,TS_ZERO);

		// Round off to the minimum timestep
	if (global_minimum_timestep>1 && absolute_timestamp(sa)>global_clock && sa<TS_NEVER)
	{
		if (sa > 0)
			sa = (((sa-1)/global_minimum_timestep)+1)*global_minimum_timestep;
		else
			sa = -(((-sa-1)/global_minimum_timestep)+1)*global_minimum_timestep;
	}
	return sa;
}

void GldExec::sleep(unsigned int usec)
{
#ifdef WIN32
	Sleep(usec/1000);
#else
	usleep(usec);
#endif
}

void *GldExec::obj_syncproc(OBJSYNCDATA *data)
{
	LISTITEM *s;
	unsigned int n;
	int i = data->i;

	// begin processing loop
	while (data->ok)
	{
		// lock access to start condition
		pthread_mutex_lock(&startlock[i]);

		// wait for thread start condition)
		while (data->t0 == next_t1[i]) 
			pthread_cond_wait(&start[i], &startlock[i]);
		// unlock access to start count
		pthread_mutex_unlock(&startlock[i]);

		// process the list for this thread
		for ( s = data->ls, n = 0 ; s != NULL || n < data->nObj ; s = s->next, n++ ) 
		{
			my_instance->get_exec()->ss_do_object_sync(data->n, s->data);
		}

		// signal completed condition
		data->t0 = next_t1[i];

		// lock access to done condition
		pthread_mutex_lock(&donelock[i]);

		// signal thread is done for now
		donecount[i] --; 

		// signal change in done condition
		pthread_cond_broadcast(&done[i]);

		//unlock access to done count
		pthread_mutex_unlock(&donelock[i]);
	}
	pthread_exit((void*)0);
	return (void*)0;
}

/** MAIN LOOP CONTROL ******************************************************************/
void GldExec::mls_create(void)
{
	if ( mls_destroyed )
	{
		output_debug("gldcore/exec.c/exec_mls_create(): cannot create mutex after it was destroyed");
		return;
	}
	int rv = 0;

	mls_created = 1;
	global_mainloopstate = MLS_INIT;
	
	IN_MYCONTEXT output_debug("GldExec::mls_create()");
	rv = pthread_mutex_init(&mls_svr_lock,NULL);
	if ( rv != 0 )
	{
		output_error("gldcore/exec.c/GldExec::mls_create(): pthread_mutex_init() error %d (%s)", rv, strerror(rv));
	}
	rv = pthread_cond_init(&mls_svr_signal,NULL);
	if ( rv != 0 )
	{
		output_error("gldcore/exec.c/GldExec::mls_create(): pthread_cond_init() error %d (%s)", rv, strerror(rv));
	}
}

void GldExec::mls_init(void)
{
	if ( mls_destroyed )
	{
		output_debug("gldcore/exec.c/exec_mls_init(): cannot init mutex after it was destroyed");
		return;
	}
	if (mls_created == 0)
	{
		my_instance->get_exec()->mls_create();
	}
	if (global_mainloopstate==MLS_PAUSED)
		my_instance->get_exec()->mls_suspend();
	else
		sched_update(global_clock,global_mainloopstate);
}

void GldExec::mls_start()
{
	if ( mls_destroyed )
	{
		output_debug("gldcore/exec.c/exec_mls_start(): cannot start mutex after it was destroyed");
		return;
	}
	if ( ! mls_created )
	{
		output_debug("gldcore/exec.c/exec_mls_start(): cannot start mutex before it was created");
		return;
	}
	int rv = 0;
	rv = pthread_mutex_lock(&mls_svr_lock);
	if ( rv != 0 )
	{
		output_error("gldcore/exec.c/GldExec::mls_start(): pthread_mutex_lock() error %d (%s)", rv, strerror(rv));
	}
	sched_update(global_clock,global_mainloopstate = MLS_RUNNING);
	rv = pthread_mutex_unlock(&mls_svr_lock);
	if ( rv != 0 && rv != EINVAL )
	{
		output_error("gldcore/exec.c/GldExec::mls_start(): pthread_mutex_unlock() error %d (%s)", rv, strerror(rv));
	}
	rv = pthread_cond_broadcast(&mls_svr_signal);
	if ( rv != 0 && rv != EINVAL )
	{
		output_error("gldcore/exec.c/GldExec::mls_start(): pthread_cond_broadcast() error %d (%s)", rv, strerror(rv));
	}
}

void GldExec::mls_suspend(void)
{
	if ( mls_destroyed )
	{
		output_debug("gldcore/exec.c/exec_mls_suspend(): cannot suspend mutex after it was destroyed");
		return;
	}
	if ( ! mls_created )
	{
		output_debug("gldcore/exec.c/exec_mls_suspend(): cannot suspend mutex before it was created");
		return;
	}
	int loopctr = 10;
	int rv = 0;
	IN_MYCONTEXT output_debug("pausing simulation");
	if ( global_multirun_mode==MRM_STANDALONE && strcmp(global_environment,"server")!=0 )
		output_warning("suspending simulation with no server/multirun active to control mainloop state");
	IN_MYCONTEXT output_debug("lock_ (%x->%x)", &mls_svr_lock, mls_svr_lock);
	rv = pthread_mutex_lock(&mls_svr_lock);
	if (0 != rv)
	{
		output_error("gldcore/exec.c/GldExec::mls_suspend(): pthread_mutex_lock() error %d (%s)", rv, strerror(rv));
	}
	IN_MYCONTEXT output_debug("sched update_");
	sched_update(global_clock,global_mainloopstate=MLS_PAUSED);
	IN_MYCONTEXT output_debug("wait loop_");
	while ( global_clock==TS_ZERO || (global_clock>=global_mainlooppauseat && global_mainlooppauseat<TS_NEVER) ) {
		if (loopctr > 0)
		{
			IN_MYCONTEXT output_debug(" * tick (%i)", --loopctr);
		}
		rv = pthread_cond_wait(&mls_svr_signal, &mls_svr_lock);
		if ( rv != 0 && rv != EINVAL )
		{
			output_error("gldcore/exec.c/GldExec::mls_suspend(): pthread_cond_wait() error %d (%s)", rv, strerror(rv));
		}
	}
	IN_MYCONTEXT output_debug("sched update_");
	sched_update(global_clock,global_mainloopstate=MLS_RUNNING);
	IN_MYCONTEXT output_debug("unlock_");
	rv = pthread_mutex_unlock(&mls_svr_lock);
	if (rv != 0)
	{
		output_error("gldcore/exec.c/GldExec::mls_suspend(): pthread_mutex_unlock() error %d (%s)", rv, strerror(rv));
	}
}

void GldExec::mls_resume(TIMESTAMP ts)
{
	if ( mls_destroyed )
	{
		output_debug("gldcore/exec.c/exec_mls_resume(): cannot resume mutex after it was destroyed");
		return;
	}
	if ( ! mls_created )
	{
		output_debug("gldcore/exec.c/exec_mls_resume(): cannot resume mutex before it was created");
		return;
	}
	int rv = 0;
	rv = pthread_mutex_lock(&mls_svr_lock);
	if (rv != 0 && rv != EINVAL )
	{
		output_error("gldcore/exec.c/GldExec::mls_resume(): pthread_mutex_lock() error %d (%s)", rv, strerror(rv));
	}
	global_mainlooppauseat = ts;
	if ( rv != EINVAL )
	{
		rv = pthread_mutex_unlock(&mls_svr_lock);
		if (rv != 0)
		{
			output_error("gldcore/exec.c/GldExec::mls_resume(): pthread_mutex_unlock() error %d (%s)", rv, strerror(rv));
		}
		rv = pthread_cond_broadcast(&mls_svr_signal);
		if (rv != 0)
		{
		output_error("gldcore/exec.c/GldExec::mls_suspend(): pthread_cond_broadcast() error %d (%s)", rv, strerror(rv));
		}
	}
}

void GldExec::mls_statewait(unsigned states)
{
	if ( mls_destroyed )
	{
		output_debug("gldcore/exec.c/exec_mls_statewait(): cannot statewait mutex after it was destroyed");
		return;
	}
	if ( ! mls_created )
	{
		output_debug("gldcore/exec.c/exec_mls_statewait(): cannot statewait mutex before it was created");
		return;
	}

	int rv = 0;
	rv = pthread_mutex_lock(&mls_svr_lock);
	if (rv != 0 && rv!=EINVAL)
	{
		output_error("gldcore/exec.c/GldExec::mls_statewait(): pthread_mutex_lock() error %d (%s)", rv, strerror(rv));
	}
	if ( rv != EINVAL )
	{
		while ( (global_mainloopstate&states) == 0 ) 
		{
			rv = pthread_cond_wait(&mls_svr_signal, &mls_svr_lock);
			if ( rv != 0 && rv != EINVAL )
			{
				output_error("gldcore/exec.c/GldExec::mls_statewait(): pthread_cond_wait() error %d (%s)", rv, strerror(rv));
			}
		}
		rv = pthread_mutex_unlock(&mls_svr_lock);
		if ( rv != 0 && rv != EINVAL )
		{
			output_error("gldcore/exec.c/GldExec::mls_statewait(): pthread_mutex_unlock() error %d (%s)", rv, strerror(rv));
		}
	}
	else
	{	// very inefficient fallback method
		output_debug("mutex lock failed (%s) -- using usleep(100) instead",strerror(rv));
		while ( (global_mainloopstate&states) == 0 )
			usleep(100);
	}
}

void GldExec::mls_done(void)
{
	if ( mls_destroyed )
	{
		output_debug("gldcore/exec.c/exec_mls_statewait(): cannot destroy mutex after it was destroyed");
		return;
	}
	if ( ! mls_created )
	{
		output_debug("gldcore/exec.c/exec_mls_destroy(): cannot destroy mutex before it was created");
		return;
	}
	int rv = 0;
	if ( global_mainloopstate == MLS_DONE )
		return;
	rv = pthread_mutex_lock(&mls_svr_lock);
	if (rv != 0 && rv!=EINVAL)
	{
		output_error("gldcore/exec.c/GldExec::mls_done(): pthread_mutex_lock() error %d (%s)", rv, strerror(rv));
	}
	sched_update(global_clock,global_mainloopstate=MLS_DONE);
	if ( rv != EINVAL )
	{
		rv = pthread_mutex_unlock(&mls_svr_lock);
		if ( rv != 0 && rv != EINVAL )
		{
			output_error("gldcore/exec.c/GldExec::mls_done(): pthread_mutex_unlock() error %d (%s)", rv, strerror(rv));
		}
		rv = pthread_cond_broadcast(&mls_svr_signal);
		if ( rv != 0 && rv != EINVAL )
		{
			output_error("gldcore/exec.c/GldExec::mls_suspend(): pthread_cond_broadcast() error %d (%s)", rv, strerror(rv));
		}
	}
	mls_destroyed = 1;
	pthread_mutex_destroy(&mls_svr_lock);
	pthread_cond_destroy(&mls_svr_signal);
}

/******************************************************************
 SYNC HANDLING API
 *******************************************************************/

/** Reset the sync time data structure 

	This call clears the sync data structure
	and prepares it for a new interation pass.
	If no sync event are posted, the result of
	the pass will be a successful soft NEVER,
	which usually means the simulation stops at
	steady state.
 **/
void GldExec::sync_reset(struct sync_data *d)
{
	if ( d==NULL ) d=&main_sync;
	d->step_to = ( global_mainlooppauseat > global_clock ? global_mainlooppauseat : TS_NEVER );
	d->hard_event = 0;
	d->status = SUCCESS;
}
/** Merge the sync data structure 

    This call posts a new sync event \p to into
	an existing sync data structure \p from.
	If \p to is \p NULL, then the main exec sync
	event is updated.  If the status of \p from
	is \p FAILED, then the \p to sync time is 
	set to \p TS_INVALID.  If the time of \p from
	is \p TS_NEVER, then \p to is not updated.  In
	all other cases, the \p to sync time is updated
	with #sync_set.

	@see #sync_set
 **/
void GldExec::sync_merge(struct sync_data *to, /**< sync data to merge to (NULL to update main)  **/
					struct sync_data *from) /**< sync data to merge from */
{
	if ( to==NULL ) to = &main_sync;
	if ( from==NULL ) from = &main_sync;
	if ( from==to ) return;
	if ( sync_isinvalid(from) ) 
		sync_set(to,TS_INVALID,false);
	else if ( sync_isnever(from) ) 
		{} /* do nothing */	
	else if ( sync_ishard(from) )
		sync_set(to,sync_get(from),false);
	else
		sync_set(to,-sync_get(from),false);
}
/** Update the sync data structure 

	This call posts a new time to a sync event.
	If the event is \p NULL, then the main sync event is updated.
	If the new time is \p TS_NEVER, then the event is not updated.
	If the new time is \p TS_INVALID, then the event status is changed to FAILED.
	If the new time is not between -TS_MAX and TS_MAX, then an exception is thrown.
	If the event time is TS_NEVER, then if the time is positive a hard event is posted,
	if the time is negative a soft event is posted, otherwise the event status is changed to FAILED.
	Otherwise, if the event is hard, then the hard event count is increment and if the time is earlier it is posted.
	Otherwise, if the event is soft, then if the time is earlier it is posted.
	Otherwise, the event status is changed to FAILED.
 **/
void GldExec::sync_set(struct sync_data *d, /**< sync data to update (NULL to update main) */
				  TIMESTAMP t,/**< timestamp to update with (negative time means soft event, 0 means failure) */
				  bool deltaflag)/**< flag to let us know this was a deltamode exit - force it forward, otherwise can fail to exit */
{
	if ( d==NULL ) d=&main_sync;
	if ( t==TS_NEVER ) return; /* nothing to do */
	if ( t==TS_INVALID )
	{
		d->status = FAILED;
		return;
	}
	if ( t<=-TS_MAX || t>TS_MAX ) 
		throw_exception("set_synctime(struct sync_data *d={TIMESTAMP step_to=%lli,int32 hard_event=%d, STATUS=%s}, TIMESTAMP t=%lli): timestamp is not valid", d->step_to, d->hard_event, d->status==SUCCESS?"SUCCESS":"FAILED", t);
	if ( d->step_to==TS_NEVER )
	{
		if ( t<TS_NEVER && t>0 )
		{
			d->step_to = t;
			d->hard_event++;
		}
		else if ( t<0 )
			d->step_to = -t;
		else 
			d->status = FAILED;
	}
	else if ( t>0 ) /* hard event */
	{
		d->hard_event++;
		if (deltaflag==false)
		{
			if ( d->step_to>t )
				d->step_to = t;
		}
		else	/* Deltamode exit - override us */
		{
			d->step_to = t;
		}
	}
	else if ( t<0 ) /* soft event */
	{
		if ( d->step_to>-t )
			d->step_to = -t;
	}
	else // t==0 -> invalid
	{
		d->status = FAILED;
	}
}
/** Get the current sync time
	@return the proper (positive) event sync time, TS_NEVER, or TS_INVALID.
 **/
TIMESTAMP GldExec::sync_get(struct sync_data *d) /**< Sync data to get sync time from (NULL to read main)  */
{
	if ( d==NULL ) d=&main_sync;
	if ( sync_isnever(d) ) return TS_NEVER;
	if ( sync_isinvalid(d) ) return TS_INVALID;
	return absolute_timestamp(d->step_to);
}
/** Get the current hard event count 
	@return the number of hard events associated with this sync event.
 **/
unsigned int GldExec::sync_getevents(struct sync_data *d) /**< Sync data to get sync events from (NULL to read main)  */
{
	if ( d==NULL ) d=&main_sync;
	return d->hard_event;
}
/** Determine whether the current sync data is a hard sync
	@return non-zero if the event is a hard event, 0 if the event is a soft event
 **/
int GldExec::sync_ishard(struct sync_data *d) /**< Sync data to read hard sync status from (NULL to read main)  */
{
	if ( d==NULL ) d=&main_sync;
	return d->hard_event>0;
}
/** Determine whether the current sync data time is never 
	@return non-zero if the event is NEVER, 0 otherwise
 **/
int GldExec::sync_isnever(struct sync_data *d) /**< Sync data to read never sync status from (NULL to read main)  */
{
	if ( d==NULL ) d=&main_sync;
	return d->step_to==TS_NEVER;
}
/** Determine whether the currenet sync time is invalid (NULL to read main)
    @return non-zero if the status if FAILED, 0 otherwise
 **/
int GldExec::sync_isinvalid(struct sync_data *d) /**< Sync data to read invalid sync status from */
{
	if ( d==NULL ) d=&main_sync;
	return sync_getstatus(d)==FAILED;
}
/** Determine the current sync status
	@return the event status (SUCCESS or FAILED)
 **/
STATUS GldExec::sync_getstatus(struct sync_data *d) /**< Sync data to read sync status from (NULL to read main)  */
{
	if ( d==NULL ) d=&main_sync;
	return d->status;
}
/** Determine whether sync time is a running simulation
    @return true if the simulation should keep going, false if it should stop
 **/
bool GldExec::sync_isrunning(struct sync_data *d)
{
	return sync_get(d)<=global_stoptime && !sync_isnever(d) && sync_ishard(d);
}

void GldExec::clock_update_modules()
{
	TIMESTAMP t1 = sync_get(NULL);
	MODULE *mod;
	int ok = 0;
	while ( !ok )
	{
		ok = 1;
		for ( mod=module_get_first() ; mod!=NULL ; mod=mod->next )
		{
			if ( mod->clockupdate!=NULL )
			{
				TIMESTAMP t2 = mod->clockupdate(t1);
				if ( t2<t1 )
				{
					t1 = t2;
					ok = 0;
				}
			}
		}
	}
	sync_set(NULL,t1,false);
}

/** Main loop sync lock control **/
void GldExec::rlock_sync(void)
{
	rlock(&sync_lock);
}

void GldExec::runlock_sync(void)
{
	runlock(&sync_lock);
}

void GldExec::wlock_sync(void)
{
	wlock(&sync_lock);
}

void GldExec::wunlock_sync(void)
{
	wunlock(&sync_lock);
}

void GldExec::create_lockdata(int nObjRankList)
{
	size_t k;
	startlock = (pthread_mutex_t*)malloc(sizeof(startlock[0])*nObjRankList);
	donelock = (pthread_mutex_t*)malloc(sizeof(donelock[0])*nObjRankList);
	start = (pthread_cond_t*)malloc(sizeof(start[0])*nObjRankList);
	done = (pthread_cond_t*)malloc(sizeof(done[0])*nObjRankList);
	for ( k = 0 ; k < (size_t)nObjRankList ; k++ ) 
	{
		pthread_mutex_init(&startlock[k], NULL);
		pthread_mutex_init(&donelock[k], NULL);
		pthread_cond_init(&start[k], NULL);
		pthread_cond_init(&done[k], NULL);
	}
}

/******************************************************************
 *  MAIN EXEC LOOP
 ******************************************************************/

/** This is the main simulation loop
	@return STATUS is SUCCESS if the simulation reached equilibrium, 
	and FAILED if a problem was encountered.
 **/
STATUS GldExec::exec_start(void)
{
	int64 passes = 0, tsteps = 0;
	int pc_rv = 0; // precommit return value
	STATUS fnl_rv = FAILED; // finalize all return value
	time_t started_at = realtime_now(); // for profiler
	int j, k;
	LISTITEM *ptr;
	int incr;
	INDEX **ranks = getranks();

	// Only setup threadpool for each object rank list at the first iteration;
	// After the first iteration, setTP = false;
	bool setTP = true; 
	//int n_threads; //number of thread used in the threadpool of an object rank list
	OBJSYNCDATA *thread = NULL;

	int nObjRankList, iObjRankList;

	/* run create scripts, if any */
	if ( run_createscripts()!=XC_SUCCESS )
	{
		output_error("create script(s) failed");
		return FAILED;
	}

	/* initialize the main loop state control */
	mls_init();

	/* perform object initialization */
	if (init_all() == FAILED)
	{
		output_error("model initialization failed");
		/* TROUBLESHOOT
			The initialization procedure failed.  This is usually preceded 
			by a more detailed message that explains why it failed.  Follow
			the guidance for that message and try again.
		 */
		return FAILED;
	}

	/* establish rank index if necessary */
	if (ranks == NULL && setup_ranks() == FAILED)
	{
		output_error("ranks setup failed");
		/* TROUBLESHOOT
			The rank setup procedure failed.  This is usually preceded 
			by a more detailed message that explains why it failed.  Follow
			the guidance for that message and try again.
		 */
		return FAILED;
	}
	else
	{
		ranks = getranks();
	}

	/* run checks */
	if (global_runchecks)
		return module_checkall() > 0 ? SUCCESS : FAILED ;

	/* compile only check */
	if (global_compileonly)
		return SUCCESS;

	/* enable non-determinism check, if any */
	if (global_randomseed!=0 && global_threadcount>1)
		global_nondeterminism_warning = 1;

	if (!global_debug_mode)
	{
		/* schedule progress report event */
		if (global_show_progress)
		{
			realtime_schedule_event(realtime_now()+1,show_progress);
		}

		/* set thread count equal to processor count if not passed on command-line */
		if (global_threadcount == 0)
		{
			global_threadcount = processor_count();
			IN_MYCONTEXT output_verbose("using %d helper thread(s)", global_threadcount);
		}

		/* allocate arg_data_array to store pthreads creation argument */
		arg_data_array = create_argdata(global_threadcount);

		/* allocate thread synchronization data */
		init_thread_data();
		struct thread_data * thread_data = get_thread_data();
		if (!thread_data) {
			output_error("thread memory allocation failed");
			/* TROUBLESHOOT
				A thread memory allocation failed.  
				Follow the standard process for freeing up memory ang try again.
			 */
			return FAILED;
		}
		thread_data->count = global_threadcount;
		thread_data->data = (struct sync_data *) (thread_data + 1);
		for (j = 0; j < thread_data->count; j++) 
			thread_data->data[j].status = SUCCESS;
	}
	else
	{
		IN_MYCONTEXT output_debug("debug mode running single threaded");
		output_message("GridLAB-D entering debug mode");
	}

	/* run init scripts, if any */
	if ( run_initscripts()!=XC_SUCCESS )
	{
		output_error("init script(s) failed");
		return FAILED;
	}

	/* realtime startup */
	if (global_run_realtime>0)
	{
		char buffer[64];
		time_t gtime;
		time(&gtime);
		global_clock = gtime;
		IN_MYCONTEXT output_verbose("realtime mode requires using now (%s) as starttime", convert_from_timestamp(global_clock,buffer,sizeof(buffer))>0?buffer:"invalid time");
		if ( global_stoptime<global_clock )
			global_stoptime = TS_NEVER;
	}

	/*** GET FIRST SIGNAL FROM MASTER HERE ****/
	if (global_multirun_mode == MRM_SLAVE)
	{
		pthread_cond_broadcast(&mls_inst_signal); // tell slaveproc() it's time to get rolling
		IN_MYCONTEXT output_debug("GldExec::start(), slave waiting for first time signal");
		pthread_mutex_lock(&mls_inst_lock);
		pthread_cond_wait(&mls_inst_signal, &mls_inst_lock);
		pthread_mutex_unlock(&mls_inst_lock);
		// will have copied data down and updated step_to with slave_cache
		//global_clock = exec_sync_get(NULL); // copy time signal to gc
		IN_MYCONTEXT output_debug("GldExec::start(), slave received first time signal of %lli", global_clock);
	}
	// maybe that's all we need...
	iteration_counter = global_iteration_limit;

	/* reset sync event */
	sync_reset(NULL);
	sync_set(NULL,global_clock,false);
	if ( global_stoptime<TS_NEVER )
		sync_set(NULL,global_stoptime+1,false);

	/* signal handler */
	signal(SIGABRT, exec_sighandler);
	signal(SIGINT, exec_sighandler);
	signal(SIGTERM, exec_sighandler);

	/* initialize delta mode */
	if ( !delta_init() )
	{
		output_error("delta mode initialization failed");
		/* TROUBLESHOOT
		   The initialization of the deltamode subsystem failed.
		   The failure message is preceded by one or more errors that will provide more information.
	     */
	}

	// count how many object rank list in one iteration
	nObjRankList = 0;
	/* scan the ranks of objects */
	for (pass = 0; ranks[pass] != NULL; pass++)
	{
		int i;
		/* process object in order of rank using index */
		for (i = PASSINIT(pass); PASSCMP(i, pass); i += PASSINC(pass))
		{
			/* skip empty lists */
			if (ranks[pass]->ordinal[i] == NULL) 
				continue;
			nObjRankList++; // count how many object rank list in one iteration
		}
	}

	/* allocate and initialize thread data */
	IN_MYCONTEXT output_debug("nObjRankList=%d ",nObjRankList);

	next_t1 = (unsigned int*)malloc(sizeof(next_t1[0])*nObjRankList);
	memset(next_t1,0,sizeof(next_t1[0])*nObjRankList);

	donecount = (unsigned int*)malloc(sizeof(donecount[0])*nObjRankList);
	memset(donecount,0,sizeof(donecount[0])*nObjRankList);

	n_threads = (unsigned int*)malloc(sizeof(n_threads[0])*nObjRankList);
	memset(n_threads,0,sizeof(n_threads[0])*nObjRankList);

	// allocation and nitialize mutex and cond for object rank lists
	create_lockdata(nObjRankList);

	// global test mode
	if ( global_test_mode==TRUE )
		return test_exec();

	/* check for a model */
	if (object_get_count()==0)

		/* no object -> nothing to do */
		return SUCCESS;

	//sjin: GetMachineCycleCount
	cstart = (clock_t)clock();

	/* main loop exception handler */
	wlock_sync();
	try 
	{

		/* main loop runs for iteration limit, or when nothing futher occurs (ignoring soft events) */
		while ( iteration_counter>0 && sync_isrunning(NULL) )
		{	
			if ( getexitcode() != XC_SUCCESS && ! global_ignore_errors ) 
			{
				break;
			}
			wunlock_sync();
			TIMESTAMP internal_synctime;
			IN_MYCONTEXT output_debug("*** main loop event at %lli; stoptime=%lli, n_events=%i, exitcode=%i ***", sync_get(NULL), global_stoptime, sync_getevents(NULL), getexitcode());

			/* update the process table info */
			mls_start();

			/* main loop control */
			if ( global_clock>=global_mainlooppauseat && global_mainlooppauseat<TS_NEVER )
				mls_suspend();

			rlock_sync();
			do_checkpoint();
			runlock_sync();

			/* realtime control of global clock */
			if (global_run_realtime==0 && global_clock >= global_enter_realtime)
				global_run_realtime = 1;

			if (global_run_realtime>0 && iteration_counter>0)
			{
				double metric=0;
#ifdef WIN32
				struct timeb tv;
				ftime(&tv);
				if (1000-tv.millitm >= 0)
				{
					IN_MYCONTEXT output_verbose("waiting %d msec", 1000-tv.millitm);
					Sleep(1000-tv.millitm );
					metric = (1000-tv.millitm)/1000.0;
				}
				else
					output_error("simulation failed to keep up with real time");
#else
				struct timeval tv;
				gettimeofday(&tv, NULL);
				if (1000000-tv.tv_usec >= 0)
				{
					IN_MYCONTEXT output_verbose("waiting %d usec", 1000000-tv.tv_usec);
					usleep(1000000-tv.tv_usec);
					metric = (1000000-tv.tv_usec)/1000000.0;
				}
				else
					output_error("simulation failed to keep up with real time");
#endif
				wlock_sync();
				global_clock += global_run_realtime;
				global_realtime_metric = global_realtime_metric*realtime_metric_decay + metric*(1-realtime_metric_decay);
				sync_reset(NULL);
				sync_set(NULL,global_clock,false);
				IN_MYCONTEXT output_verbose("realtime clock advancing to %d", (int)global_clock);
			}

			/* internal control of global clock */
			else
			{
				wlock_sync();
				global_clock = sync_get(NULL);
			}

			/* operate delta mode if necessary (but only when event mode is active, e.g., not right after init) */
			/* note that delta mode cannot be supported for realtime simulation */
			global_deltaclock = 0;

			/* determine whether any modules seek delta mode */
			DELTAMODEFLAGS flags=DMF_NONE;
			DT delta_dt = delta_modedesired(&flags);
			TIMESTAMP t = TS_NEVER;
			IN_MYCONTEXT output_debug("delta_dt is %d", (int)delta_dt);
			switch ( delta_dt ) {
			case DT_INFINITY: /* no dt -> event mode */
				global_simulation_mode = SM_EVENT;
				t = TS_NEVER;
				break; 
			case DT_INVALID: /* error dt  */
				global_simulation_mode = SM_ERROR;
				t = TS_INVALID;
				break; /* simulation mode error */
			default: /* valid dt */
				if ( global_minimum_timestep>1 )
				{
					global_simulation_mode = SM_ERROR;
					output_error("minimum_timestep must be 1 second to operate in deltamode");
					t = TS_INVALID;
					break;
				}
				else
				{
					if (delta_dt==0)	/* Delta mode now */
					{
						global_simulation_mode = SM_DELTA;
						t = global_clock;
					}
					else	/* Normal sync - get us to delta point */
					{
						global_simulation_mode = SM_EVENT;
						t = global_clock + delta_dt;
					}
				}
				break;
			}
			if ( global_simulation_mode==SM_ERROR )
			{
				output_error("a simulation mode error has occurred");
				break; /* terminate main loop immediately */
			}
			sync_set(NULL,t,false);
			
			/* synchronize all internal schedules */
			if ( global_clock < 0 )
				throw_exception("clock time is negative (global_clock=%lli)", global_clock);
			else if ( global_debug_output )
			{
				char dt[64]="(invalid)"; convert_from_timestamp(global_clock,dt,sizeof(dt));
				IN_MYCONTEXT output_debug("global_clock -> %s\n",dt);
			}
			/* set time context */
			output_set_time_context(global_clock);

			/* reset for a new sync event */
			sync_reset(NULL);

			/* account for stoptime only if global clock is not already at stoptime */
			if ( global_clock<=global_stoptime && global_stoptime!=TS_NEVER )
				sync_set(NULL,global_stoptime+1,false);

			/* synchronize all internal schedules */
			internal_synctime = syncall_internals(global_clock);
			if( internal_synctime!=TS_NEVER && absolute_timestamp(internal_synctime)<global_clock )
			{
				// must be able to force reiterations for m/s mode.
				throw("internal property sync failure");
				/* TROUBLESHOOT
					An internal property such as schedule, enduse or loadshape has failed to synchronize and the simulation aborted.
					This message should be preceded by a more informative message that explains which element failed and why.
					Follow the troubleshooting recommendations for that message and try again.
				 */
			}
			sync_set(NULL,internal_synctime,false);

			/* prepare multithreading */
			if (!global_debug_mode)
			{
				struct thread_data * thread_data = get_thread_data();
				for (j = 0; j < thread_data->count; j++) {
					thread_data->data[j].hard_event = 0;
					thread_data->data[j].step_to = TS_NEVER;
				}
			}

			/* run precommit only on first iteration */
			if (iteration_counter == global_iteration_limit)
			{
				/* run commit scripts, if any */
				if ( sync_get(NULL)!=global_clock && run_precommitscripts()!=XC_SUCCESS )
				{
					throw("script precommit failure");
				}
			
				pc_rv = precommit_all(global_clock);
				if(SUCCESS != pc_rv)
				{
					throw("precommit failure");
				}
			}
			iObjRankList = -1;

			/* scan the ranks of objects for each pass */
			for (pass = 0; ranks[pass] != NULL; pass++)
			{
				int i;

				/* top-down module events */
				if ( pass == 0 )
				{
					TIMESTAMP mt = module_presyncall(global_clock);
					if ( mt == TS_INVALID )
					{
						throw("module on_presync failed");
					}
					sync_set(NULL,mt,false);
				}
				else if ( pass == 2 )
				{
					TIMESTAMP mt = module_postsyncall(global_clock);
					if ( mt == TS_INVALID )
					{
						throw("module on_postsync failed");
					}
					sync_set(NULL,mt,false);
				}

				/* process object in order of rank using index */
				for (i = PASSINIT(pass); PASSCMP(i, pass); i += PASSINC(pass))
				{
					/* skip empty lists */
					if (ranks[pass]->ordinal[i] == NULL) 
						continue;

					iObjRankList ++;

					if (global_debug_mode)
					{
						LISTITEM *item;
						for (item=ranks[pass]->ordinal[i]->first; item!=NULL; item=item->next)
						{
							OBJECT *obj = (OBJECT*)(item->data);
							// @todo change debug so it uses sync API
							if (exec_debug(&main_sync,passtype[pass],i,obj)==FAILED)
							{
								throw("debugger quit");
							}
						}
					}
					else
					{
						//sjin: if global_threadcount == 1, no pthread multhreading
						if (global_threadcount == 1) 
						{
							for (ptr = ranks[pass]->ordinal[i]->first; ptr != NULL; ptr=ptr->next) {
								OBJECT *obj = (OBJECT*)(ptr->data);
								ss_do_object_sync(0, ptr->data);					
								
								if (obj->valid_to == TS_INVALID)
								{
									//Get us out of the loop so others don't exec on bad status
									break;
								}
								///printf("%d %s %d\n", obj->id, obj->name, obj->rank);
							}
							//printf("\n");
						} 
						else 
						{ //sjin: implement pthreads
							unsigned int n_items,objn=0,n;
							unsigned int n_obj = ranks[pass]->ordinal[i]->size;

							// Only create threadpool for each object rank list at the first iteration. 
							// Reuse the threadppol of each object rank list at all other iterations.
							if (setTP == true) { 
								incr = (int)ceil((float) n_obj / global_threadcount);
								// if the number of objects is less than or equal to the number of threads, each thread process one object 
								if (incr <= 1) {
									n_threads[iObjRankList] = n_obj;
									n_items = 1;
								// if the number of objects is greater than the number of threads, each thread process the same number of 
								// objects (incr), except that the last thread may process less objects 
								} else {
									n_threads[iObjRankList] = (int)ceil((float) n_obj / incr);
									n_items = incr;
								}
								if ( (int)n_threads[iObjRankList] > global_threadcount ) 
								{
									throw("Running threads > global_threadcount");
								}

								// allocate thread list
								thread = (OBJSYNCDATA*)malloc(sizeof(OBJSYNCDATA)*n_threads[iObjRankList]);
								memset(thread,0,sizeof(OBJSYNCDATA)*n_threads[iObjRankList]);
								// assign starting obj for each thread
								for (ptr=ranks[pass]->ordinal[i]->first;ptr!=NULL;ptr=ptr->next)
								{
									if (thread[objn].nObj==n_items)
										objn++;
									if (thread[objn].nObj==0) {
										thread[objn].ls=ptr;
									}
									thread[objn].nObj++;
								}
								// create threads
								for (n=0; n<n_threads[iObjRankList]; n++) {
									thread[n].ok = true;
									thread[n].i = iObjRankList;
									thread[n].main = &instance;
									if (pthread_create(&(thread[n].pt),NULL,exec_obj_syncproc,&(thread[n]))!=0) {
										output_fatal("obj_sync thread creation failed");
										thread[n].ok = false;
									} else
										thread[n].n = n;
								}

							}
														
							// lock access to done count
							pthread_mutex_lock(&donelock[iObjRankList]);
							
							// initialize wait count
							donecount[iObjRankList] = n_threads[iObjRankList];

							// lock access to start condition
							pthread_mutex_lock(&startlock[iObjRankList]);

							// update start condition
							next_t1[iObjRankList] ++;

							// signal all the threads
							pthread_cond_broadcast(&start[iObjRankList]);
							// unlock access to start count
							pthread_mutex_unlock(&startlock[iObjRankList]);

							// begin wait
							while (donecount[iObjRankList]>0)
								pthread_cond_wait(&done[iObjRankList],&donelock[iObjRankList]);
							// unlock done count
							pthread_mutex_unlock(&donelock[iObjRankList]);
						}

						struct thread_data * thread_data = get_thread_data();
						for (j = 0; j < thread_data->count; j++) {
							if (thread_data->data[j].status == FAILED) 
							{
								sync_set(NULL,TS_INVALID,false);
								throw("synchronization failed");
							}
						}
					}
				}

				/* bottom-up module event */
				if ( pass == 1 )
				{
					TIMESTAMP mt = module_syncall(global_clock);
					if ( mt == TS_INVALID )
					{
						throw("module on_sync failed");
					}
					sync_set(NULL,mt,false);
				}

				/* run all non-schedule transforms */
				{
					TIMESTAMP st = transform_syncall(global_clock,(TRANSFORMSOURCE)(XS_DOUBLE|XS_COMPLEX|XS_ENDUSE));// if (abs(t)<t2) t2=t;
					sync_set(NULL,st,false);
				}
			}
			setTP = false;

			if (!global_debug_mode)
			{
				struct thread_data * thread_data = get_thread_data();
				for (j = 0; j < thread_data->count; j++) 
				{
					sync_merge(NULL,&thread_data->data[j]);
				}

				/* report progress */
				realtime_run_schedule();
			}

			/* count number of passes */
			passes++;

			/**** LOOPED SLAVE PAUSE HERE ****/
			if(global_multirun_mode == MRM_SLAVE)
			{
				IN_MYCONTEXT output_debug("step_to = %lli", sync_get(NULL));
				IN_MYCONTEXT output_debug("GldExec::start(), slave waiting for looped time signal");

				pthread_cond_broadcast(&mls_inst_signal);

				pthread_mutex_lock(&mls_inst_lock);
				pthread_cond_wait(&mls_inst_signal, &mls_inst_lock);
				pthread_mutex_unlock(&mls_inst_lock);

				IN_MYCONTEXT output_debug("GldExec::start(), slave received looped time signal (%lli)", sync_get(NULL));
			}

			/* run sync scripts, if any */
			if ( run_syncscripts()!=XC_SUCCESS )
			{
				throw("script synchronization failure");
			}


			/* check for clock advance (indicating last pass) */
			if ( sync_get(NULL)!=global_clock )
			{
				OBJECT *obj;
				TIMESTAMP commit_time = TS_NEVER;
				commit_time = commit_all(global_clock, sync_get(NULL));
				if ( absolute_timestamp(commit_time) <= global_clock)
				{
					// commit cannot force reiterations, and any event where the time is less than the global clock
					//  indicates that the object is reporting a failure
					throw("model commit failed");
					/* TROUBLESHOOT
						The commit procedure failed.  This is usually preceded 
						by a more detailed message that explains why it failed.  Follow
						the guidance for that message and try again.
					 */
				} else if( absolute_timestamp(commit_time) < sync_get(NULL) )
				{
					sync_set(NULL,commit_time,false);
				}

				/* make sure all clocks are set */
				for ( obj = object_get_first() ; obj != NULL ; obj = object_get_next(obj) )
					obj->clock = global_clock;

				/* reset iteration count */
				iteration_counter = global_iteration_limit;

				/* count number of timesteps */
				tsteps++;
			}

			/* check iteration limit */
			else if (--iteration_counter == 0)
			{
				sync_set(NULL,TS_INVALID,false);
				throwf("convergence iteration limit reached at %s (exec)", simtime());
				/* TROUBLESHOOT
					This indicates that the core's solver was unable to determine
					a steady state for all objects for any time horizon.  Identify
					the object that is causing the convergence problem and contact
					the developer of the module that implements that object's class.
				 */
			}

			/* run commit scripts, if any */
			if ( sync_get(NULL)!=global_clock && run_commitscripts()!=XC_SUCCESS )
			{
				throw("commit script(s) failed");
			}

			/* run scheduled dump, if any */
			run_dump();
			
			/* handle delta mode operation */
			if ( global_simulation_mode==SM_DELTA && sync_get(NULL)>=global_clock )
			{
				DT deltatime = delta_update();
				if ( deltatime==DT_INVALID )
				{
					output_error("delta_update() failed, deltamode operation cannot continue");
					/*  TROUBLESHOOT
					An error was encountered while trying to perform a deltamode update.  Look for
					other relevant deltamode messages for indications as to why this may have occurred.
					If the error persists, please submit your code and a bug report via the trac website.
					*/
					global_simulation_mode = SM_ERROR;
					throw("Deltamode simulation failure");
				}
				sync_set(NULL,global_clock + deltatime,true);
				global_simulation_mode = SM_EVENT;
			}

			/* clock update is the very last chance to change the next time */
			if(sync_get(NULL) != global_clock){
				clock_update_modules();
			}
		} // end of while loop

		/* disable signal handler */
		signal(SIGINT,NULL);

		/* check end state */
		if ( sync_isnever(NULL) )
		{
			char buffer[64];
			IN_MYCONTEXT output_verbose("simulation at steady state at %s", convert_from_timestamp(global_clock,buffer,sizeof(buffer))?buffer:"invalid time");
		}

	}
	catch (const char *msg)
	{
		output_error("exec halted: %s", msg);
		sync_set(NULL,TS_INVALID,false);
		/* TROUBLESHOOT
			This indicates that the core's solver shut down.  This message
			is usually preceded by more detailed messages.  Follow the guidance
			for those messages and try again.
		 */
	}

	/* terminate main loop state control */
	mls_done();
	wunlock_sync();

	IN_MYCONTEXT output_debug("*** main loop ended at %lli; stoptime=%lli, n_events=%i, exitcode=%i ***", sync_get(NULL), global_stoptime, sync_getevents(NULL), getexitcode());
	if(global_multirun_mode == MRM_MASTER)
	{
		instance_master_done(TS_NEVER); // tell everyone to pack up and go home
	}

	//sjin: GetMachineCycleCount
	cend = (clock_t)clock();

	fnl_rv = finalize_all();
	if(FAILED == fnl_rv)
	{
		output_error("finalize_all() failed");
	}

	/* run term scripts, if any */
	if ( run_termscripts() != XC_SUCCESS )
	{
		output_error("term script(s) failed");
	}

	/* deallocate threadpool */
	if (!global_debug_mode)
	{
		struct thread_data * thread_data = get_thread_data();
		free(thread_data);
		thread_data = NULL;
	}

	/* run term calls, if any */
	int rc = run_termcalls(global_clock);
	if ( rc < 0 )
	{
		output_error("termcall %d failed", rc);
	}

	// Destroy mutex and cond
	for(k=0;k<nObjRankList;k++) {
		pthread_mutex_destroy(&startlock[k]);
		pthread_mutex_destroy(&donelock[k]);
		pthread_cond_destroy(&start[k]);
		pthread_cond_destroy(&done[k]);
	}

	/* report performance */
	if (global_profiler && !sync_isinvalid(NULL) )
	{
		double elapsed_sim = timestamp_to_hours(global_clock)-timestamp_to_hours(global_starttime);
		double elapsed_wall = (double)(realtime_now()-started_at+1);
		double sync_time = 0;
		double sim_speed = object_get_count()/1000.0*elapsed_sim/elapsed_wall;

		clock_t loader_time = get_instance()->get_cmdarg()->get_loader_time();
		extern clock_t instance_synctime;
		extern clock_t randomvar_synctime;
		extern clock_t schedule_synctime;
		extern clock_t loadshape_synctime;
		extern clock_t enduse_synctime;
		extern clock_t transform_synctime;

		CLASS *cl;
		DELTAPROFILE *dp = delta_getprofile();
		double delta_runtime = 0, delta_simtime = 0;
		if (global_threadcount==0) global_threadcount=1;
		for (cl=class_get_first_class(); cl!=NULL; cl=cl->next)
			sync_time += ((double)cl->profiler.clocks)/CLOCKS_PER_SEC;
		sync_time /= global_threadcount;
		delta_runtime = dp->t_count>0 ? (dp->t_preupdate+dp->t_update+dp->t_postupdate)/CLOCKS_PER_SEC : 0;
		delta_simtime = dp->t_count*(double)dp->t_delta/(double)dp->t_count/1e9;

		output_profile("\nCore profiler results");
		output_profile("======================\n");
		output_profile("Total objects           %8d objects", object_get_count());
		output_profile("Parallelism             %8d thread%s", global_threadcount,global_threadcount>1?"s":"");
		output_profile("Total time              %8.1f seconds", elapsed_wall);
		output_profile("  Core time             %8.1f seconds (%.1f%%)", (elapsed_wall-sync_time-delta_runtime),(elapsed_wall-sync_time-delta_runtime)/elapsed_wall*100);
		output_profile("    Compiler            %8.1f seconds (%.1f%%)", (double)loader_time/CLOCKS_PER_SEC,((double)loader_time/CLOCKS_PER_SEC)/elapsed_wall*100);
		output_profile("    Instances           %8.1f seconds (%.1f%%)", (double)instance_synctime/CLOCKS_PER_SEC,((double)instance_synctime/CLOCKS_PER_SEC)/elapsed_wall*100);
		output_profile("    Random variables    %8.1f seconds (%.1f%%)", (double)randomvar_synctime/CLOCKS_PER_SEC,((double)randomvar_synctime/CLOCKS_PER_SEC)/elapsed_wall*100);
		output_profile("    Schedules           %8.1f seconds (%.1f%%)", (double)schedule_synctime/CLOCKS_PER_SEC,((double)schedule_synctime/CLOCKS_PER_SEC)/elapsed_wall*100);
		output_profile("    Loadshapes          %8.1f seconds (%.1f%%)", (double)loadshape_synctime/CLOCKS_PER_SEC,((double)loadshape_synctime/CLOCKS_PER_SEC)/elapsed_wall*100);
		output_profile("    Enduses             %8.1f seconds (%.1f%%)", (double)enduse_synctime/CLOCKS_PER_SEC,((double)enduse_synctime/CLOCKS_PER_SEC)/elapsed_wall*100);
		output_profile("    Transforms          %8.1f seconds (%.1f%%)", (double)transform_synctime/CLOCKS_PER_SEC,((double)transform_synctime/CLOCKS_PER_SEC)/elapsed_wall*100);
		output_profile("  Model time            %8.1f seconds/thread (%.1f%%)", sync_time,sync_time/elapsed_wall*100);
		if ( dp->t_count>0 )
			output_profile("  Deltamode time        %8.1f seconds/thread (%.1f%%)", delta_runtime,delta_runtime/elapsed_wall*100);	
		output_profile("Simulation time         %8.0f days", elapsed_sim/24);
		if (sim_speed>10.0)
			output_profile("Simulation speed         %7.0lfk object.hours/second", sim_speed);
		else if (sim_speed>1.0)
			output_profile("Simulation speed         %7.1lfk object.hours/second", sim_speed);
		else
			output_profile("Simulation speed         %7.0lf object.hours/second", sim_speed*1000);
		output_profile("Passes completed        %8d passes", passes);
		output_profile("Time steps completed    %8d timesteps", tsteps);
		output_profile("Convergence efficiency  %8.02lf passes/timestep", (double)passes/tsteps);
#ifndef NOLOCKS
		output_profile("Read lock contention    %7.01lf%%", (my_instance->rlock_spin>0 ? (1-(double)my_instance->rlock_count/(double)my_instance->rlock_spin)*100 : 0));
		output_profile("Write lock contention   %7.01lf%%", (my_instance->wlock_spin>0 ? (1-(double)my_instance->wlock_count/(double)my_instance->wlock_spin)*100 : 0));
#endif
		output_profile("Average timestep        %7.0lf seconds/timestep", (double)(global_clock-global_starttime)/tsteps);
		output_profile("Simulation rate         %7.0lf x realtime", (double)(global_clock-global_starttime)/elapsed_wall);
		if ( dp->t_count>0 )
		{
			double total = dp->t_preupdate + dp->t_update + dp->t_interupdate + dp->t_postupdate;
			output_profile("\nDelta mode profiler results");
			output_profile("===========================\n");
			output_profile("Active modules          %s", dp->module_list);
			output_profile("Initialization time     %8.1lf seconds", (double)(dp->t_init)/(double)CLOCKS_PER_SEC);
			output_profile("Number of updates       %8" FMT_INT64 "u", dp->t_count);
			output_profile("Average update timestep %8.4lf ms", (double)dp->t_delta/(double)dp->t_count/1e6);
			output_profile("Minumum update timestep %8.4lf ms", dp->t_min/1e6);
			output_profile("Maximum update timestep %8.4lf ms", dp->t_max/1e6);
			output_profile("Total deltamode simtime %8.1lf s", delta_simtime/1000);
			output_profile("Preupdate time          %8.1lf s (%.1f%%)", (double)(dp->t_preupdate)/(double)CLOCKS_PER_SEC, (double)(dp->t_preupdate)/total*100); 
			output_profile("Object update time      %8.1lf s (%.1f%%)", (double)(dp->t_update)/(double)CLOCKS_PER_SEC, (double)(dp->t_update)/total*100); 
			output_profile("Interupdate time        %8.1lf s (%.1f%%)", (double)(dp->t_interupdate)/(double)CLOCKS_PER_SEC, (double)(dp->t_interupdate)/total*100); 
			output_profile("Postupdate time         %8.1lf s (%.1f%%)", (double)(dp->t_postupdate)/(double)CLOCKS_PER_SEC, (double)(dp->t_postupdate)/total*100);
			output_profile("Total deltamode runtime %8.1lf s (100%%)", delta_runtime);
			output_profile("Simulation rate         %8.1lf x realtime", delta_simtime/delta_runtime/1000);
		}
		output_profile("\n");
		object_synctime_profile_dump(NULL);
	}

	/* terminate links */
	if ( thread ) free(thread);
	return sync_getstatus(NULL);
}

/** Starts the executive test loop 
	@return STATUS is SUCCESS if all test passed, FAILED is any test failed.
 **/
STATUS GldExec::test(struct sync_data *data, /**< the synchronization state data */
				 PASSCONFIG pass, /**< the pass number */
				 OBJECT *obj)  /**< the current object */
{
	TIMESTAMP this_t;
	/* check in and out-of-service dates */
	if (global_clock<obj->in_svc)
		this_t = obj->in_svc; /* yet to go in service */
	else if ((global_clock==obj->in_svc) && (obj->in_svc_micro != 0))
		this_t = obj->in_svc + 1;	/* Round up for service (deltamode handled separately) */
	else if (global_clock<=obj->out_svc)
		this_t = object_sync(obj, global_clock, pass);
	else 
		this_t = TS_NEVER; /* already out of service */

	/* check for "soft" event (events that are ignored when stopping) */
	if (this_t < -1)
		this_t = -this_t;
	else if (this_t != TS_NEVER)
		data->hard_event++;  /* this counts the number of hard events */

	/* check for stopped clock */
	if (this_t < global_clock) {
		char b[64];
		output_error("%s: object %s stopped its clock! (test)", simtime(), object_name(obj, b, 63));
		/* TROUBLESHOOT
			This indicates that one of the objects in the simulator has encountered a
			state where it cannot calculate the time to the next state.  This usually
			is caused by a bug in the module that implements that object's class.
		 */
		data->status = FAILED;
	} else {
		/* check for iteration limit approach */
		if (iteration_counter == 2 && this_t == global_clock) {
			char b[64];
			IN_MYCONTEXT output_verbose("%s: object %s iteration limit imminent",
								simtime(), object_name(obj, b, 63));
		}
		else if (iteration_counter == 1 && this_t == global_clock) {
			output_error("convergence iteration limit reached for object %s:%d (test)", obj->oclass->name, obj->id);
			/* TROUBLESHOOT
				This indicates that one of the objects in the simulator has encountered a
				state where it cannot calculate the time to the next state.  This usually
				is caused by a bug in the module that implements that object's class.
			 */
		}

		/* manage minimum timestep */
		if (global_minimum_timestep>1 && this_t>global_clock && this_t<TS_NEVER)
			this_t = ((this_t/global_minimum_timestep)+1)*global_minimum_timestep;

		/* if this event precedes next step, next step is now this event */
		if (data->step_to > this_t)
			data->step_to = this_t;
		data->status = SUCCESS;
	}
	return data->status;
}

void *GldExec::slave_node_proc(void *args)
{
	SOCKET **args_in = (SOCKET **)args;
	SOCKET	*sockfd_ptr = (SOCKET *)args_in[1],
			 masterfd = *(SOCKET*)(args_in[2]);
	bool *done_ptr = (bool *)(args_in[0]);
	struct sockaddr_in *addrin = (struct sockaddr_in *)(args_in[3]);

	char buffer[1024], response[1024], addrstr[17], *paddrstr, *token_to;
#ifdef WIN32
	char *params;
#endif
	char dirname[256], filename[256];
	unsigned int64 mtr_port, id;
	const char *token[5]={
		HS_CMD,
		"dir=\"", // CMD absorbs dir's leading whitespace
		" file=\"",
		" port=",
		" id="
	};
	size_t token_len[] = {
		strlen(token[0]),
		strlen(token[1]),
		strlen(token[2]),
		strlen(token[3]),
		strlen(token[4])
	};
	int /* rsp_port = global_server_portnum,*/ rv = 0;
	size_t offset = 0, tok_len = 0;

	// input checks
	if(0 == sockfd_ptr)
	{
		output_error("slave_node_proc(): no pointer to listener socket");
		return 0;
	}
	if(0 == done_ptr)
	{
		output_error("slave_node_proc(): no pointer to stop condition");
		return 0;
	}
	if(0 > masterfd)
	{
		output_error("slave_node_proc(): no accepted socket");
		return 0;
	}
	if(0 == addrin)
	{
		output_error("slave_node_proc(): no address struct");
		return 0;
	}
	// sanity checks
	if(0 != *done_ptr)
	{
		// something else errored while the thread was starting
		output_error("slave_node_proc(): slavenode finished while thread started");
		closesocket(masterfd);
		free(addrin);
		return 0;
	}
	// socket has been accept()ed

	// read handshake
	rv = recv(masterfd, buffer, 1023, 0);
	if (rv < 0)
	{
		output_error("slave_node_proc(): error receiving handshake");
		closesocket(masterfd);
		free(addrin);
		return 0;
	} else if (rv == 0)
	{
		output_error("slave_node_proc(): socket closed before receiving handshake");
		closesocket(masterfd);
		free(addrin);
		return 0;
	}
	if (0 != strcmp(buffer, HS_SYN))
	{
		output_error("slave_node_proc(): received handshake mismatch (\"%s\")", buffer);
		closesocket(masterfd);
		free(addrin);
		return 0;
	}

	sprintf(response, HS_ACK);
	// send response
	//	* see above
	rv = send(masterfd, response, (int)strlen(response), 0);
	if (rv < 0)
	{
		output_error("slave_node_proc(): error sending handshake response");
		closesocket(masterfd);
		free(addrin);
		return 0;
	} 
	else if (rv == 0)
	{
		output_error("slave_node_proc(): socket closed before sending handshake response");
		closesocket(masterfd);
		free(addrin);
		return 0;
	}

	// receive command
	rv = recv(masterfd, buffer, 1023, 0);
	if (0 > rv)
	{
		output_error("slave_node_proc(): error receiving command instruction");
		closesocket(masterfd);
		free(addrin);
		return 0;
	} 
	else if(0 == rv)
	{
		output_error("slave_node_proc(): socket closed before receiving command instruction");
		closesocket(masterfd);
		free(addrin);
		return 0;
	}

	// process command (what kinds do we expect?)
	// HS_CMD dir file r_port cacheid profile relax debug verbose warn quiet avlbalance
	//	the first four tokens are dir="%s" file="%s" port=%d id=%I64
	//	subsequent toekns are as legitimate GLD cmdargs
	//	ex: [HS_CMD]dir="C:\mytemp\mls\slave\" file="model.glm" port="6762" id="1234567890" --profile --relax --quiet
	IN_MYCONTEXT output_debug("cmd: \'%s\'", buffer);
	// HS_CMD
	if ( memcmp(token[0],buffer,token_len[0])!=0 )
	{
		output_error("slave_node_proc(): bad command instruction token");
		closesocket(masterfd);
		free(addrin);
		return 0;
	}
	offset += token_len[0];
	// dir="%s"
	if ( memcmp(token[1], buffer+offset, token_len[1])!=0 )
	{
		output_error("slave_node_proc(): error in command instruction dir token");
		IN_MYCONTEXT output_debug("t=\"%5s\" vs c=\"%5s\"", token[1], buffer+offset);
		closesocket(masterfd);
		free(addrin);
		return 0;
	}
	offset += token_len[1];
	//tok_len = strcspn(buffer+offset, "\"\n\r\t\0"); // whitespace in path allowable
	//tok_to = strchr(buffer+offset+1, '"');
	//tok_len = tok_to - (buffer+offset+1) - 1; // -1 to fudge the last "
	// strchr doesn't like when you start with a ", it seems
	tok_len = 0;
	while ( buffer[offset+tok_len]!='"' && buffer[offset+tok_len]!=0 )
	{
		++tok_len;
	}
	IN_MYCONTEXT output_debug("tok_len = %d", tok_len);
	if (tok_len > 0)
	{
		char temp[256];
		sprintf(temp, "%%d offset and %%d len for \'%%%lus\'", tok_len);
		IN_MYCONTEXT output_debug(temp, offset, tok_len, buffer+offset);
		memcpy(dirname, buffer+offset, (tok_len > sizeof(dirname) ? sizeof(dirname) : tok_len));
	} else {
		dirname[0] = 0;
	}
	offset += 1 + tok_len; // one for "
	// zero-len dir is allowable
	// file=""
	if ( memcmp(token[2],buffer+offset,token_len[2])!=0 )
	{
		output_error("slave_node_proc(): error in command instruction file token");
		IN_MYCONTEXT output_debug("(%d)t=\"%7s\" vs c=\"%7s\"", offset, token[2], buffer+offset);
		closesocket(masterfd);
		free(addrin);
		return 0;
	}
	offset += token_len[2];
	tok_len = strcspn(buffer+offset, "\"\n\r\t\0"); // whitespace in filename allowable
	if (tok_len > 0)
	{
		char temp[256];
		memcpy(filename, buffer+offset, (tok_len > sizeof(filename) ? sizeof(filename) : tok_len));
		filename[tok_len]=0;
		sprintf(temp, "%%d offset and %%d len for \'%%%lus\'", tok_len);
		IN_MYCONTEXT output_debug(temp, offset, tok_len, buffer+offset);
	} 
	else 
	{
		filename[0] = 0;
	}
	offset += 1 + tok_len;
	// port=
	if (0 != memcmp(token[3], buffer+offset, token_len[3]))
	{
		output_error("slave_node_proc(): error in command instruction port token");
		closesocket(masterfd);
		free(addrin);
		return 0;
	}
	offset += token_len[3];
	mtr_port = strtol(buffer+offset, &token_to, 10);
	if (mtr_port < 0)
	{
		output_error("slave_node_proc(): bad return port specified in command instruction");
		closesocket(masterfd);
		free(addrin);
		return 0;
	} 
	else if (mtr_port < 1024)
	{
		output_warning("slave_node_proc(): return port %d specified, may cause system conflicts", mtr_port);
	}

	// id=
	if (0 != memcmp(token_to, token[4], token_len[4]))
	{
		output_error("slave_node_proc(): error in command instruction id token");
		closesocket(masterfd);
		free(addrin);
		return 0;
	}
	offset = token_len[4]; // not += since we updated our zero point
	IN_MYCONTEXT output_debug("%12s -> ???", token_to);
	id = strtoll(token_to+offset, &token_to, 10);
	if (id < 0)
	{
		output_error("slave_node_proc(): id %" FMT_INT64 " specified, may cause system conflicts", id);
		closesocket(masterfd);
		free(addrin);
		return 0;
	} 
	else 
	{
		IN_MYCONTEXT output_debug("id = %llu", id);
	}

#ifdef WIN32
	// then zero or more CL args
	params = 1 + token_to;
#endif

	// if unable to locate model file,
	//	* request model
	//	* receive model file (raw or packaged)
	// else
	//	* receipt model file found

	// run command
//	rsp_port = ntohs(addrin->sin_port);
	paddrstr = inet_ntoa(addrin->sin_addr);
	if (0 == paddrstr)
	{
		output_error("slave_node_proc(): unable to write address to a string");
		closesocket(masterfd);
		free(addrin);
		return 0;
	} 
	else 
	{
		memcpy(addrstr, paddrstr, sizeof(addrstr));
		IN_MYCONTEXT output_debug("snp(): connect to %s:%d", addrstr, mtr_port);
	}

#ifdef WIN32
	// write, system() --slave command
	sprintf(filepath, "%s%s%s", dirname, (dirname[0] ? "\\" : ""), filename);
	IN_MYCONTEXT output_debug("filepath = %s", filepath);
	sprintf(ippath, "--slave %s:%d", addrstr, mtr_port);
	IN_MYCONTEXT output_debug("ippath = %s", ippath);
	sprintf(cmd, "%s%sgridlabd.exe %s --id %" FMT_INT64 "d %s %s",
		(global_execdir[0] ? global_execdir : ""), (global_execdir[0] ? "\\" : ""), params, id, ippath, filepath);//addrstr, mtr_port, filepath);//,
	IN_MYCONTEXT output_debug("system(\"%s\")", cmd);

	rv = system(cmd);
#endif

	// cleanup
	closesocket(masterfd);
	free(addrin);

	return NULL;
}

/**	slave_node
	Variant startup mode for GridLAB-D that causes the system to run a simple
	server that will spawn new instances of GridLAB-D as requested to run as
	remote slave nodes (see cmdarg.c:slave() )
 **/
void GldExec::slave_node()
{
	static bool node_done = FALSE;
	static SOCKET sockfd = -1;
	static SOCKET cnx;
	SOCKET *args[4];
	struct sockaddr_in server_addr;
	struct sockaddr_in *inaddr = NULL;
	int inaddrsz;
	fd_set reader_fdset, master_fdset;
	struct timeval timer;
	pthread_t slave_thread;
	int rct;
#ifdef WIN32
	static WSADATA wsaData;
#endif

	inaddrsz = sizeof(struct sockaddr_in);
#ifdef WIN32
	// if we're on windows, we're using WinSock2, so we need WSAStartup.
	IN_MYCONTEXT output_debug("starting WS2");
	if (WSAStartup(MAKEWORD(2,0),&wsaData)!=0)
	{
		output_error("GldExec::slave_node(): socket library initialization failed: %s",strerror(GetLastError()));
		return;	
	}
#endif

	// init listener socket
	sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sockfd)
	{
		output_fatal("GldExec::slave_node(): unable to open IPv4 TCP socket");
		return;
	}

	// bind to global_slave_port
	//  * this port shall not be located on Tatooine.
	memset(&server_addr, 0, inaddrsz);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(global_slave_port);
	if (0 != bind(sockfd, (struct sockaddr *)&server_addr, inaddrsz))
	{
		output_fatal("GldExec::slave_node(): unable to bind socket to port %d", global_slave_port);
		perror("bind()");
		closesocket(sockfd);
		return;
	}

	// listen
	if ( 0 != listen(sockfd, 5))
	{
		output_fatal("GldExec::slave_node(): unable to listen to socket");
		closesocket(sockfd);
		return;
	}
	IN_MYCONTEXT output_debug("GldExec::slave_node(): listening on port %d", global_slave_port);

	// set up fd_set
	FD_ZERO(&master_fdset);
	FD_SET(sockfd, &master_fdset);
	
	args[0] = (SOCKET *)&node_done;
	args[1] = (SOCKET *)&sockfd;

	IN_MYCONTEXT output_debug("esn(): starting loop");
	while (!node_done)
	{
		reader_fdset = master_fdset;
		timer.tv_sec = 3; // check for kaputness every three (not five) seconds
		timer.tv_usec = 0;
		// wait for connection
		rct = select(1 + (int)sockfd, &reader_fdset, 0, 0, &timer);
		if (rct < 0)
		{
			output_error("slavenode select() error");
			return;
		} 
		else if (rct == 0)
		{
			// Waited three seconds without any input.  Play it again, Sam.
			//IN_MYCONTEXT output_debug("esn(): select ");
		} 
		else if (rct > 0)
		{
			if ( ! inaddr ) inaddr = (struct sockaddr_in*)malloc(inaddrsz);
			args[3] = (SOCKET *)inaddr;
			//IN_MYCONTEXT output_debug("esn(): got client");
			memset(inaddr, 0, inaddrsz);
			cnx = accept(sockfd, (struct sockaddr *)inaddr, (socklen_t*)&inaddrsz);
			args[2] = (SOCKET *)&cnx;
			IN_MYCONTEXT output_debug("esn(): accepted client");
			if (-1 == (int64)(args[2]))
			{
				output_error("unable to accept connection");
				perror("accept()");
				node_done = TRUE;
				closesocket(sockfd);
				free(inaddr);
				return;
			}

			// thread off connection
			//	* include &node_done to handle 'stop' messages
			//	* include &sock to unblock thread on stop condition
			//	* detatch, since we don't care about it after we start it
			//	! I have no idea if the reuse of slave_thread will fly. Change
			//	!  this if strange things start to happen.
			if ( pthread_create(&slave_thread, NULL, exec_slave_node_proc, (void *)args) )
			{
				output_error("slavenode unable to thread off connection");
				node_done = TRUE;
				closesocket(sockfd);
				closesocket(*(SOCKET*)(args[2]));
				free(inaddr);
				return;
			}
			//IN_MYCONTEXT output_debug("esn(): thread created");
			if ( pthread_detach(slave_thread) )
			{
				output_error("slavenode unable to detach connection thread");
				node_done = TRUE;
				closesocket(sockfd);
				closesocket(*(SOCKET*)(args[2]));
				free(inaddr);
				return;
			}
			//IN_MYCONTEXT output_debug("esn(): thread detached");
		} // end if rct
	} // end while
	if ( inaddr ) free(inaddr);
}

/*************************************
 * Script support
 *************************************/

int GldExec::add_scriptexport(const char *name)
{
	SIMPLELIST *item = (SIMPLELIST*)malloc(sizeof(SIMPLELIST));
	if ( !item ) return 0;
	item->data = strdup(name);
	item->next = script_exports;
	script_exports = item;
	return 1;
}

int GldExec::update_exports(void)
{
	SIMPLELIST *item;
	for ( item=script_exports ; item!=NULL ; item=item->next )
	{
		char *name = item->data;
		char value[1024];
		if ( global_getvar(name,value,sizeof(value)) )
		{
			IN_MYCONTEXT output_debug("export %s=%s",name,value);
			if ( setenv(name,value,true)!=0 )
				IN_MYCONTEXT output_error("unable to update script export '%s' with value '%s'", name, value);
		}
	}
	return 1;
}

int GldExec::add_script(SIMPLELIST **list, const char *file)
{
	SIMPLELIST *item = (SIMPLELIST*)malloc(sizeof(SIMPLELIST));
	if ( !item ) return 0;
	item->data = (char*)malloc(strlen(file)+1);
	strcpy(item->data,file);
	item->next = *list;
	*list = item;
	return 1;
}

EXITCODE GldExec::run_system_script(char *call)
{
	EXITCODE rc = (EXITCODE)system(call);
	if ( rc != XC_SUCCESS )
	{
		output_error("script '%s' return with exit code %d", call,rc);
		return rc;
	}
	else
	{
		IN_MYCONTEXT output_verbose("script '%s'' returned ok", call);
		return XC_SUCCESS;
	}	
}

EXITCODE GldExec::run_gridlabd_script(char *call)
{
	char name[1024];
	char arg[1024];
	int narg = sscanf(call,"%s %[^\n]",name,arg);
	if ( narg > 0 && strcmp(name,"dump")==0 )
	{
		return saveall(arg) > 0 ? XC_SUCCESS : XC_IOERR;
	}
	else if ( narg > 0 )
	{
		output_error("script '%s' is not valid in environment 'gridlabd'", name);
		return XC_RUNERR;
	}
	else
	{
		output_error("script missing for environment 'gridlabd'", name);
		return XC_RUNERR;
	}
}

EXITCODE GldExec::run_scripts(SIMPLELIST *list)
{
	SIMPLELIST *item;
	update_exports();
	for ( item=list ; item!=NULL ; item=item->next )
	{
		char group[1024] = "system";
		char call[1024] = "";
		if ( sscanf(item->data,"%[a-z]:%[^\n]",group,call) == 2 && strcmp(group,"system") != 0 )
		{
			// special access
			if ( strcmp(group,"gridlabd") == 0 )
			{
				EXITCODE rc = run_gridlabd_script(call);
				if ( rc != XC_SUCCESS )
				{
					return rc;
				}
			}
			else 
			{
				output_error("script '%s' is not recognized in environment '%s'", call, group);
				return XC_SHFAILED;
			}
		}
		else
		{
			EXITCODE rc = run_system_script(item->data);
			if ( rc != XC_SUCCESS )
			{
				return rc;
			}
		}
	}
	return XC_SUCCESS;
}

int GldExec::add_createscript(const char *file)
{
	IN_MYCONTEXT output_debug("adding create script '%s'", file);
	return add_script(&create_scripts,file);
}

int GldExec::add_initscript(const char *file)
{
	IN_MYCONTEXT output_debug("adding init script '%s'", file);
	return add_script(&init_scripts,file);
}

int GldExec::add_syncscript(const char *file)
{
	IN_MYCONTEXT output_debug("adding sync script '%s'", file);
	return add_script(&sync_scripts,file);
}

int GldExec::add_precommitscript(const char *file)
{
	IN_MYCONTEXT output_debug("adding precommit script '%s'", file);
	return add_script(&precommit_scripts,file);
}

int GldExec::add_commitscript(const char *file)
{
	IN_MYCONTEXT output_debug("adding commit script '%s'", file);
	return add_script(&commit_scripts,file);
}

int GldExec::add_termscript(const char *file)
{
	IN_MYCONTEXT output_debug("adding term script '%s'", file);
	return add_script(&term_scripts,file);
}

int GldExec::run_createscripts(void)
{
	return run_scripts(create_scripts);
}

int GldExec::run_initscripts(void)
{
	return run_scripts(init_scripts);
}

int GldExec::run_precommitscripts(void)
{
	return run_scripts(precommit_scripts);
}

int GldExec::run_syncscripts(void)
{
	return run_scripts(sync_scripts);
}

int GldExec::run_commitscripts(void)
{
	return run_scripts(commit_scripts);
}

int GldExec::run_termscripts(void)
{
	return run_scripts(term_scripts);
}

size_t GldExec::add_initcall(INITCALL call)
{
	initcalls.push_back(call);
	size_t n = initcalls.size();
	IN_MYCONTEXT output_verbose("add_initcall(%p) -> %d", call, n);
	return n;
}

int GldExec::run_initcalls(TIMESTAMP t0)
{
	int n = 0;
	for ( std::list<INITCALL>::iterator call = initcalls.begin() ; call != initcalls.end() ; call++, n++ )
	{
		int rc = (*call)(t0);
		if ( rc != 0 )
		{
			return -n;
		}
	}
	return n;
}

size_t GldExec::add_termcall(TERMCALL call)
{
	termcalls.push_back(call);
	size_t n = termcalls.size();
	IN_MYCONTEXT output_verbose("add_termcall(%p) -> %d", call, n);
	return n;
}

int GldExec::run_termcalls(TIMESTAMP tn)
{
	int n = 0;
	for ( std::list<TERMCALL>::iterator call = termcalls.begin() ; call != termcalls.end() ; call++, n++ )
	{
		int rc = (*call)(tn);
		if ( rc != 0 )
		{
			return -n;
		}
	}
	return n;
}


/**@}*/
