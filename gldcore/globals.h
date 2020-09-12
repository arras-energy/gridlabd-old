/* File: globals.h 
 * Copyright (C) 2008, Battelle Memorial Institute

	@file globals.h
	@addtogroup globals
 @{
 **/
#ifndef _GLOBALS_H
#define _GLOBALS_H

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

#include "version.h"
#include "build.h"
#include "validate.h"
#include "sanitize.h"

#ifdef _MAIN_C
#define GLOBAL 
#define INIT(A) = A
#else
#define GLOBAL extern
#define INIT(A)
#endif

/*	Typedef: GLOBALVAR
		See <s_globalvar>

	Structure: s_globalvar
		Global variable data structure

	PROPERTY prop - A reference to the property data
 	uint32 next - A reference to the next global variable
	void (*callback)(const char *) - Function to call whenever the variable is set
	LOCKVAR lock - Lock variable for concurrent access control
	GLOBALVAR *next - Reference to the next global variable in the global variable list
 */
typedef struct s_globalvar 
{
	PROPERTY *prop;
	uint32 flags;
	void (*callback)(const char *);
	LOCKVAR lock;
	struct s_globalvar *next;
} GLOBALVAR;

/*	Typedef: EXITCODE
		See e_exitcode

	Enum: e_exitcode
		Exit codes

	XC_EXFAILED = (-1) - Failure of exec/wait per system(3)  
	XC_SUCCESS = 0 - Successful completion per system(3) 
 	XC_ARGERR = 1 - Error processing command line arguments
	XC_ENVERR = 2 - Bad environment startup
	XC_TSTERR = 3 - Requested test failed
	XC_USRERR = 4 - User reject terms of use
	XC_RUNERR = 5 - Simulation did not complete as desired
	XC_INIERR = 6 - Initialization failed
	XC_PRCERR = 7 - Process control error
	XC_SVRKLL = 8 - Server killed
	XC_IOERR = 9 - I/O error
	XC_LDERR = 10 - Model load error
	XC_SHFAILED = 127 - Shell failure per system(3)
	XC_SIGNAL = 128 - Signal caught; must be or'd with SIG value if known 
	XC_SIGINT = (XC_SIGNAL|SIGINT) - SIGINT caught
	XC_SIGHUP = (XC_SIGNAL|SIGHUP) - SIGHUP caught
	XC_SIGKILL = (XC_SIGNAL|SIGKILL) - SIGKILL caught
	XC_SIGTERM = (XC_SIGNAL|SIGTERM) - SIGTERM caught
	XC_EXCEPTION = 255 - Exception caught
 */
typedef enum e_exitcode 
{
	XC_EXFAILED = -1,
	XC_SUCCESS = 0,
	XC_ARGERR = 1, 
	XC_ENVERR = 2, 
	XC_TSTERR = 3, 
	XC_USRERR = 4, 
	XC_RUNERR = 5, 
	XC_INIERR = 6, 
	XC_PRCERR = 7, 
	XC_SVRKLL = 8,
	XC_IOERR = 9, 
	XC_LDERR = 10, 
	XC_SHFAILED = 127,
	XC_SIGNAL = 128, 
	XC_SIGINT = (XC_SIGNAL|SIGINT), 
	XC_SIGHUP = (XC_SIGNAL|SIGHUP), 
	XC_SIGKILL = (XC_SIGNAL|SIGKILL), 
	XC_SIGTERM = (XC_SIGNAL|SIGTERM), 
	XC_EXCEPTION = 255, 
} EXITCODE;

#ifdef __cplusplus
extern "C" {
#endif

STATUS global_init(void);
GLOBALVAR *global_getnext(GLOBALVAR *previous);
GLOBALVAR *global_find(const char *name);
GLOBALVAR *global_create(const char *name, ...);
STATUS global_setvar(const char *def,...);
const char *global_getvar(const char *name, char *buffer, size_t size);
int global_isdefined(const char *name);
void global_dump(void);
size_t global_getcount(void);
void global_restore(GLOBALVAR *pos);
void global_push(char *name, char *value);
size_t global_saveall(FILE *fp);

#ifdef __cplusplus
}
#endif

/*	Typedef: DATEFORMAT
		See <e_dateformat>

	Enum: e_dateformat
		DF_ISO = 0 - ISO standard format
		DF_US = 1 - USA date format (i.e., mm/dd/yyyy)
		DF_EURO = 2 - EU data format (i.e., dd/mm/yyyy)
		DF_ISO8601 = 3 - ISO8601 standard format

	See Also:
	- <global_dateformat>
 */
typedef enum e_dateformat {
	DF_ISO		= 0, 
	DF_US		= 1, 
	DF_EURO		= 2,
	DF_ISO8601	= 3,
} DATEFORMAT;

/*	Typedef INITSEQ
		See <e_initseq>

	Enum: e_initseq
		IS_CREATION = 0 - Initialization in order of creation (legacy)
		IS_DEFERRED = 1 - Initialization may be deferred by objects (default)
		IS_BOTTOMUP = 2 - Initialization performed by rank bottom-up
		IS_TOPDOWN = 3 - Initialization performed by rank top-down

	See Also:
	- global_init_sequence
 */
typedef enum e_initseq {
	IS_CREATION=0, 
	IS_DEFERRED=1, 
	IS_BOTTOMUP=2, 
	IS_TOPDOWN=3,
} INITSEQ;

/* Typedef CHECKPOINTTYPE
	See <e_checkpointtype>

	Enum: e_checkpointtype
	CPT_NONE = 0 - checkpoints not enabled (default)
	CPT_WALL = 1 - checkpoints run on wall clock interval
	CPT_SIM = 2 - checkpoints run on sim clock interval 

	See Also:
	- global_checkpoint_type
 */
typedef enum e_checkpointtype {
	CPT_NONE=0,  /**< checkpoints is not enabled */
	CPT_WALL=1, /**< checkpoints run on wall clock interval */
	CPT_SIM=2,  /**< checkpoints run on sim clock interval */
} CHECKPOINTTYPE; /**< checkpoint type determines how checkpoint intervals are used */

/* Typedef RANDOMNuMBERGENERATOR
	See: e_randomnumbergenerator

	Enum: e_randomnumbergenerator
	RNG2 = 2 - Random numbers generated using the pre-3.0 method
	RNG3 = 3 - Random numbers generated using the post-2.x method (default)

	See also:
	- global_randomnumbergenerator
 */
typedef enum {
	RNG2=2, /**< random numbers generated using pre-V3 method */
	RNG3=3, /**< random numbers generated using post-V2 method */
} RANDOMNUMBERGENERATOR; /**< identifies the type of random number generator used */

/* 	Section: Global Variables 
		Global variables may be accessed using <GldGlobalvar>
 */

/* Variable: global_version_major */
GLOBAL unsigned global_version_major INIT(REV_MAJOR); /**< The software's major version */

/* Variable: global_version_minor */
GLOBAL unsigned global_version_minor INIT(REV_MINOR); /**< The software's minor version */

/* Variable: global_version_patch */
GLOBAL unsigned global_version_patch INIT(REV_PATCH); /**< The software's patch version */

/* Variable: global_version_build */
GLOBAL unsigned global_version_build INIT(BUILDNUM); /**< The software's build number */

/* Variable: global_version_branch */
GLOBAL char global_version_branch[256] INIT(BRANCH); /**< The software's branch designator */

/* Variable: global_version */
GLOBAL char global_version[1024] INIT(""); /**< The software's official version designation */

/* Variable: global_command_line */
GLOBAL char global_command_line[1024]; /**< The current command-line */

/* Variable: global_environment */
GLOBAL char global_environment[1024] INIT("batch"); /**< The processing environment in use */

/* Variable: global_quiet_mode */
GLOBAL int global_quiet_mode INIT(FALSE); /**< The quiet mode flag */

/* Variable: global_warn_mode */
GLOBAL int global_warn_mode INIT(TRUE); /**< The warning mode flag */

/* Variable: global_debug_mode */
GLOBAL int global_debug_mode INIT(FALSE); /**< Enables the debugger */

/* Variable: global_test_mode */
GLOBAL int global_test_mode INIT(FALSE); /**< The test mode flag */

/* Variable: global_verbose_mode */
GLOBAL int global_verbose_mode INIT(FALSE); /**< The verbose mode flag */

/* Variable: global_debug_output */
GLOBAL int global_debug_output INIT(FALSE); /**< Enables debug output */

/* Variable: global_keep_progress */
GLOBAL int global_keep_progress INIT(FALSE); /**< Flag to keep progress reports */

/* Variable: global_progress */
GLOBAL double global_progress INIT(0.0); /**< Progress fraction */

/* Variable: global_iteration_limit */
GLOBAL unsigned global_iteration_limit INIT(100); /**< The global iteration limit */

/* Variable: global_workdir */
GLOBAL char global_workdir[1024] INIT("."); /**< The current working directory */

/* Variable: global_dumpfile */
GLOBAL char global_dumpfile[1024] INIT("gridlabd.json"); /**< The dump file name */

/* Variable: global_savefile */
GLOBAL char global_savefile[1024] INIT(""); /**< The save file name */

/* Variable: global_dumpall */
GLOBAL int global_dumpall INIT(FALSE);	/**< Flags all modules to dump data after run complete */

/* Variable: global_runchecks */
GLOBAL int global_runchecks INIT(FALSE); /**< Flags module check code to be called after initialization */

/** @todo Set the threadcount to zero to automatically use the maximum system resources (tickets 180) */
/* Variable: global_threadcount */
GLOBAL int global_threadcount INIT(1); /**< the maximum thread limit, zero means automagically determine best thread count */

/* Variable: global_profiler */
GLOBAL int global_profiler INIT(0); /**< Flags the profiler to process class performance data */

/* Variable: global_pauseatexit */
GLOBAL int global_pauseatexit INIT(0); /**< Enable a pause for user input after exit */

/* Variable: global_testoutputfile */
GLOBAL char global_testoutputfile[1024] INIT("test.txt"); /**< Specifies the test output file */

/* Variable: global_xml_encoding */
GLOBAL int global_xml_encoding INIT(8);  /**< Specifies XML encoding (default is 8) */

/* Variable: global_pidfile */
GLOBAL char global_pidfile[1024] INIT(""); /**< Specifies that a process id file should be created */

/* Variable: global_no_balance */
GLOBAL unsigned char global_no_balance INIT(FALSE);

/* Variable: global_kmlfile */
GLOBAL char global_kmlfile[1024] INIT(""); /**< Specifies KML file to dump */

/* Variable: global_kmlhost */
GLOBAL char global_kmlhost[1024] INIT("https://raw.githubusercontent.com/slacgismo/gridlabd/master/gldcore/rt"); /**< Specifies the KML image library server */

/* Variable: global_modelname */
GLOBAL char global_modelname[1024] INIT(""); /**< Name of the current model */

/* Variable: global_execdir */
GLOBAL char global_execdir[1024] INIT(""); /**< Path to folder containing installed application files */

/* Variable: global_strictnames */
GLOBAL bool global_strictnames INIT(true); /**< Enforce strict global naming (prevents globals from being implicitly created by assignment) */

/* Variable: global_xmlstrict */
GLOBAL bool global_xmlstrict INIT(true); /**< Causes XML I/O to use strict XML data structures */

/* Variable: global_relax_naming_rules */
GLOBAL int global_relax_naming_rules INIT(0); /**< Causes the error to relax to a warning when object names start with numbers or special characters */

/* Variable: global_urlbase */
GLOBAL char global_urlbase[1024] /**< default urlbase used for online resources */
#ifdef _DEBUG
	INIT("./");
#else
	INIT("http://www.gridlabd.org/"); 
#endif

/* Variable: global_randomseed */
GLOBAL unsigned int global_randomseed INIT(0); /**< random number seed (default 0 means true randomization, non-zero means deterministic random state) */

/* Variable: global_randomstate */
GLOBAL unsigned int global_randomstate INIT(0);

/* Variable: global_include */
GLOBAL char global_include[1024] INIT(""); /**< include path for models and code headers */

/* Variable: global_gdb */
GLOBAL int global_gdb INIT(0); /**< select gdb debugger */

/* Variable: global_trace */
GLOBAL char global_trace[1024] INIT(""); /**< comma separate list of runtime calls that will be traced */

/* Variable: global_gdb_window */
GLOBAL int global_gdb_window INIT(0); /**< start gdb in a separate window */

/* Variable: global_process_id */
GLOBAL int global_process_id INIT(0); /**< the main process id */

/* Variable: global_execname*/
GLOBAL char global_execname[1024] INIT(""); /**< the main program full path */

/* Variable: global_tmp */
GLOBAL char global_tmp[1024] /**< location for temp files */
#ifdef WIN32
							INIT("C:\\WINDOWS\\TEMP");
#else
							INIT("/tmp"); 
#endif

/* Variable: global_force_compile */
GLOBAL int global_force_compile INIT(0); /** flag to force recompile of GLM file even when up to date */

/* Variable: global_nolocks */
GLOBAL int global_nolocks INIT(0); /** flag to disable memory locking */

/* Variable: global_forbid_multiload */
GLOBAL int global_forbid_multiload INIT(0); /** flag to disable multiple GLM file loads */

/* Variable: global_skipsafe */
GLOBAL int global_skipsafe INIT(0); /** flag to allow skipping of safe syncs (see OF_SKIPSAFE) */

/* Variable: global_dateformat */
GLOBAL int global_dateformat INIT(DF_ISO); /** date format (ISO=0, US=1, EURO=2) */

/* Variable: global_init_sequence */
GLOBAL int global_init_sequence INIT(IS_DEFERRED); /** initialization sequence, default is ordered-by-creation */

/* Variable: global_clock */
GLOBAL TIMESTAMP global_clock INIT(TS_ZERO); /**< The main clock timestamp */

/** @todo: global_nextTime violates the naming conventions (mixed case variable name) */
/* Variable: global_nextTime */
GLOBAL TIMESTAMP global_nextTime INIT(TS_ZERO); /** < The next smallest event timestamp */

/* Variable: global_starttime */
GLOBAL TIMESTAMP global_starttime INIT(946684800); /**< The simulation starting time (default is 2000-01-01 0:00) */

/* Variable: global_stoptime */
GLOBAL TIMESTAMP global_stoptime INIT(TS_NEVER); /**< The simulation stop time (default is 1 year after start time) */

/* Variable: global_double_format */
GLOBAL char global_double_format[32] INIT("%+lg"); /**< the format to use when processing real numbers */

/* Variable: global_complex_format */
GLOBAL char global_complex_format[256] INIT("%+lg%+lg%c"); /**< the format to use when processing complex numbers */

/* Variable: global_object_format */
GLOBAL char global_object_format[32] INIT("%s:%d"); 

/* Variable: global_object_scan */
GLOBAL char global_object_scan[32] INIT("%[^:]:%d"); /**< the format to use when scanning for object ids */

/* Variable: global_minimum_timestep */
GLOBAL int global_minimum_timestep INIT(1); /**< the minimum timestep allowed */

/* Variable: global_maximum_synctime */
GLOBAL int global_maximum_synctime INIT(60); /**< the maximum time allotted to any single sync call */

/* Variable: global_platform */
GLOBAL char global_platform[8] /**< the host operating platform */
#ifdef WIN32
	INIT("WINDOWS");
#elif __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ >= 1050
	INIT("MACOSX");
#else
	INIT("LINUX");
#endif

/* Variable: global_suppress_repeat_messages */
GLOBAL int global_suppress_repeat_messages INIT(1); /**< flag that allows repeated messages to be suppressed */

/* Variable: global_suppress_deprecated_messages */
GLOBAL int global_suppress_deprecated_messages INIT(0); /**< flag to suppress output notice of deprecated properties usage */

/* Variable: global_run_realtime */
GLOBAL int global_run_realtime INIT(0); /**< flag to force simulator into realtime mode */

/* Variable: global_enter_realtime */
GLOBAL TIMESTAMP global_enter_realtime INIT(TS_NEVER); /**< The simulation transitions from simtime to realtime at this timestep */

/* Variable: global_realtime_metric */
GLOBAL double global_realtime_metric INIT(0); /**< realtime performance metric (0=poor, 1=great) */

#ifdef _DEBUG /** @todo: consider making global_sync_dumpfile always available */
/* Variable: global_sync_dumpfile */
GLOBAL char global_sync_dumpfile[1024] INIT(""); /**< enable sync event dump file */
#endif

/* Variable: global_streaming_io_enabled */
GLOBAL int global_streaming_io_enabled INIT(0); /**< flag to enable compact streams instead of XML or GLM */

/* Variable: global_nondeterminism_warning */
GLOBAL int global_nondeterminism_warning INIT(0); /**< flag to enable nondeterminism warning (use of rand when multithreading */

/* Variable: global_compileonly */
GLOBAL int global_compileonly INIT(0); /**< flag to enable compile-only option (does not actually start the simulation) */

/* Variable: global_compileonly */
GLOBAL int global_initializeonly INIT(0); /**< flag to enable initialize-only option (does not actually start the simulation) */

/* Variable: global_server_portnum */
GLOBAL int global_server_portnum INIT(0); /**< port used in server mode (6267 was assigned by IANA Dec 2010) */

/* Variable: global_server_inaddr */
GLOBAL char global_server_inaddr[1024] INIT(""); /**< interface address to bind server to */

/* Variable: global_client_allowed */
GLOBAL char global_client_allowed[1024] INIT(""); /**< internet address from which clients can be accepted */

/* Variable: global_browser */
GLOBAL char global_browser[1024] /**< default browser to use for GUI */
#ifdef WIN32
	INIT("iexplore"); 
#elif defined(MACOSX)
	INIT("safari");
#else
	INIT("firefox"); 
#endif

/* Variable: global_server_quit_on_close */
GLOBAL int global_server_quit_on_close INIT(0); /** server will quit when connection is closed */

/* Variable: global_autoclean */
GLOBAL int global_autoclean INIT(1); /** server will automatically clean up defunct jobs */

/* Variable: technology_readiness_level */
GLOBAL int technology_readiness_level INIT(0); /**< the TRL of the model (see http://sourceforge.net/apps/mediawiki/gridlab-d/index.php?title=Technology_Readiness_Levels) */

/* Variable: global_show_progress */
GLOBAL int global_show_progress INIT(1);

/* Variable: global_checkpoint_type */
GLOBAL int global_checkpoint_type INIT(CPT_NONE); /**< checkpoint type determines whether and how checkpoints are used */

/* Variable: global_checkpoint_file */
GLOBAL char global_checkpoint_file[1024] INIT(""); /**< checkpoint file name is base name used for checkpoint save files */

/* Variable: global_checkpoint_seqnum */
GLOBAL int global_checkpoint_seqnum INIT(0); /**< checkpoint sequence file number */

/* Variable: global_checkpoint_interval */
GLOBAL int global_checkpoint_interval INIT(0); /** checkpoint interval (default is 3600 for CPT_WALL and 86400 for CPT_SIM */

/* Variable: global_checkpoint_keepall */
GLOBAL int global_checkpoint_keepall INIT(0); /** determines whether all checkpoint files are kept, non-zero keeps files, zero delete all but last */

/* Variable: global_check_version */
GLOBAL int global_check_version INIT(0); /**< check version flag */

/* Variable: global_randomnumbergenerator */
GLOBAL int global_randomnumbergenerator INIT(RNG3); /**< select which random number generator to use */

typedef enum {
	MLS_INIT = 0, /**< main loop initializing */
	MLS_RUNNING = 1, /**< main loop is running */
	MLS_PAUSED = 2, /**< main loop is paused (waiting) */
	MLS_DONE = 3, /**< main loop is done (steady) */
	MLS_LOCKED = 4, /**< main loop is locked (possible deadlock) */
} MAINLOOPSTATE; /**< identifies the main loop state */

/* Variable:  */
GLOBAL int global_mainloopstate INIT(MLS_INIT); /**< main loop processing state */

/* Variable:  */
GLOBAL TIMESTAMP global_mainlooppauseat INIT(TS_NEVER); /**< time at which to pause main loop */

/* Variable:  */
GLOBAL char global_infourl[1024] INIT("http://docs.gridlabd.us/index.html?owner=slacgismo&project=gridlabd&search="); /**< URL for info calls */

/* Variable:  */
GLOBAL char global_hostname[1024] INIT("localhost"); /**< machine hostname */

/* Variable:  */
GLOBAL char global_hostaddr[32] INIT("127.0.0.1"); /**< machine ip addr */

/* Variable:  */
GLOBAL int global_autostartgui INIT(1); /**< autostart GUI when no command args are given */

/* delta mode support */
typedef enum {
	SM_INIT			= 0x00, /**< initial state of simulation */
	SM_EVENT		= 0x01, /**< event driven simulation mode */
	SM_DELTA		= 0x02, /**< finite difference simulation mode */
	SM_DELTA_ITER	= 0x03, /**< Iteration of finite difference simulation mode */
	SM_ERROR		= 0xff, /**< simulation mode error */
} SIMULATIONMODE; /**< simulation mode values */
typedef enum {
	DMF_NONE		= 0x00,	/**< no flags */
	DMF_SOFTEVENT	= 0x01,/**< event is soft */
} DELTAMODEFLAGS; /**< delta mode flags */

/* Variable:  */
GLOBAL SIMULATIONMODE global_simulation_mode INIT(SM_INIT); /**< simulation mode */

/* Variable: global_allow_deltamode 

	Flag to allow simulation in delta-mode
*/
GLOBAL bool global_deltamode_allowed INIT(FALSE);

/* Variable:  */
GLOBAL DT global_deltamode_timestep INIT(10000000); /**< delta mode time step in ns (default is 10ms) */

/* Variable:  */
GLOBAL DELTAT global_deltamode_maximumtime INIT(3600000000000); /**< the maximum time (in ns) delta mode is allowed to run without an event (default is 1 hour) */

/* Variable:  */
GLOBAL DELTAT global_deltaclock INIT(0); /**< the cumulative delta runtime with respect to the global clock */

/* Variable:  */
GLOBAL double global_delta_curr_clock INIT(0.0);	/**< Deltamode clock offset by main clock (not just delta offset) */

/* Variable:  */
GLOBAL char global_deltamode_updateorder[1025] INIT(""); /**< the order in which modules are updated */

/* Variable:  */
GLOBAL unsigned int global_deltamode_iteration_limit INIT(10);	/**< Global iteration limit for each delta timestep (object and interupdate calls) */

/* Variable:  */
GLOBAL unsigned int global_deltamode_forced_extra_timesteps INIT(0);	/**< Deltamode forced extra time steps -- once all items want SM_EVENT, this will force this many more updates */

/* Variable:  */
GLOBAL bool global_deltamode_forced_always INIT(false);	/**< Deltamode flag - prevents exit from deltamode (no SM_EVENT) -- mainly for debugging purposes */

/* Variable:  */
GLOBAL char global_master[1024] INIT(""); /**< master hostname */

/* Variable:  */
GLOBAL unsigned int64 global_master_port INIT(0);	/**< master port/mmap/shmem info */

/* Variable:  */
GLOBAL int16 global_slave_port INIT(6267); /**< default port for slaves to listen on. slaves will not run in server mode, but multiple slaves per node will require changing this. */

/* Variable:  */
GLOBAL unsigned int64 global_slave_id INIT(0); /**< ID number used by remote slave to identify itself when connecting to the master */
typedef enum {
	MRM_STANDALONE, /**< multirun is not enabled (standalone run) */
	MRM_MASTER,     /**< multirun is enabled and this run is the master run */
	MRM_SLAVE,      /**< multirun is enabled and this run is the slace run */
	MRM_LIBRARY,	/**< running as a library in another system */
} MULTIRUNMODE; /**< determines the type of run */

/* Variable:  */
GLOBAL MULTIRUNMODE global_multirun_mode INIT(MRM_STANDALONE);	/**< multirun mode */
typedef enum {
	MRC_NONE,	/**< isn't actually connected upwards */
	MRC_MEM,	/**< use shared mem or the like */
	MRC_SOCKET,	/**< use a socket */
} MULTIRUNCONNECTION;	/**< determines the connection mode for a slave run */

/* Variable:  */
GLOBAL MULTIRUNCONNECTION global_multirun_connection INIT(MRC_NONE);	/**< multirun mode connection */

/* Variable:  */
GLOBAL int32 global_signal_timeout INIT(5000); /**< signal timeout in milliseconds (-1 is infinite) */

/* Variable:  */
GLOBAL int global_return_code INIT(0); /**< return code from last system call */

/* Variable:  */
GLOBAL EXITCODE global_exit_code INIT(XC_SUCCESS);

/* Variable:  */
GLOBAL int global_init_max_defer INIT(64); /**< maximum number of times objects will be deferred for initialization */

/* remote data access */
void *global_remote_read(void *local, GLOBALVAR *var); /** access remote global data */
void global_remote_write(void *local, GLOBALVAR *var); /** access remote global data */

/* module compile flags */
typedef enum {
	MC_NONE     = 0x00, /**< no module compiler flags */
	MC_CLEAN    = 0x01, /**< clean build */
	MC_KEEPWORK = 0x02, /**< keep intermediate files */ 
	MC_DEBUG    = 0x04, /**< debug build */
	MC_VERBOSE  = 0x08, /**< verbose output */
} MODULECOMPILEFLAGS;

/* Variable:  */
GLOBAL MODULECOMPILEFLAGS global_module_compiler_flags INIT(MC_NONE); /** module compiler flags */

/* Variable:  */
GLOBAL unsigned int global_mt_analysis INIT(0); /**< perform multithread analysis (requires profiler) */

/* Variable:  */
GLOBAL unsigned int global_inline_block_size INIT(16*65536); /**< inline code block size */

/* Variable:  */
GLOBAL TIMESTAMP global_runaway_time INIT(2209017600); /**< signal runaway clock on 1/1/2040 */

/* Variable:  */
GLOBAL set global_validateoptions INIT(VO_TSTSTD|VO_RPTALL); /**< validation options */

/* Variable:  */
GLOBAL set global_sanitizeoptions INIT(SO_NAMES|SO_GEOCOORDS); /**< sanitizing options */

/* Variable:  */
GLOBAL char8 global_sanitizeprefix INIT("GLD_"); /**< sanitized name prefix */

/* Variable:  */
GLOBAL char1024 global_sanitizeindex INIT(".txt"); /**< sanitize index file spec */

/* Variable:  */
GLOBAL char32 global_sanitizeoffset INIT(""); /**< sanitize lat/lon offset */

/* Variable:  */
GLOBAL bool global_run_powerworld INIT(false);

/* Variable:  */
GLOBAL bool global_bigranks INIT(true); /**< enable non-recursive set_rank function (good for very deep models) */

/* Variable:  */
GLOBAL char1024 global_svnroot INIT("http://gridlab-d.svn.sourceforge.net/svnroot/gridlab-d");

/* Variable:  */
GLOBAL char1024 global_github INIT("https://github.com/gridlab-d");

/* Variable:  */
GLOBAL char1024 global_gitraw INIT("https://raw.githubusercontent.com/gridlab-d");

/* Variable:  */
GLOBAL char1024 global_wget_options INIT("maxsize:100MB;update:newer"); /**< maximum size of wget request */

/* Variable:  */
GLOBAL bool global_reinclude INIT(false); /**< allow the same include file to be included multiple times */

/* Variable:  */
GLOBAL bool global_relax_undefined_if INIT(false); /**< allow #if macro to handle undefined global variables */

/* Variable:  */
GLOBAL bool global_literal_if INIT(true); /**< do not interpret lhs of #if as a variable name */

/* Variable:  */
GLOBAL bool global_allow_variant_aggregates INIT(false); /* allow aggregates to include time varying results */

/* Variable:  */
GLOBAL char1024 global_daemon_configfile INIT("gridlabd.cnf"); /**< name of daemon configuration file */

/* Variable: global_server_keepalive */
GLOBAL bool global_server_keepalive INIT(FALSE); /**< keep server alive after simulation finishes */

/* Variable: global_json_complex_format */
#define JCF_STRING  0x0000
#define JCF_LIST    0x0001
#define JCF_DICT    0x0002
#define JCF_DEGREES 0x0010
#define JCF_RADIANS 0x0020

GLOBAL enumeration global_json_complex_format INIT(JCF_STRING);

typedef enum {
	DMC_MAIN		= 0x0000000000000001,
	DMC_CMDARG		= 0x0000000000000002,
	DMC_LOAD		= 0x0000000000000004,
	DMC_CREATE		= 0x0000000000000008,
	DMC_EXEC		= 0x0000000000000010,
	DMC_TIME		= 0x0000000000000020,
	DMC_FIND		= 0x0000000000000040,
	DMC_CLASS		= 0x0000000000000080,
	DMC_OBJECT		= 0x0000000000000100,
	DMC_MODULE		= 0x0000000000000200,
	DMC_INDEX		= 0x0000000000000400,
	DMC_GLOBALS		= 0x0000000000000800,
	DMC_EXCEPTION	= 0x0000000000001000,
	DMC_AGGREGATE	= 0x0000000000002000,
	DMC_COMPARE		= 0x0000000000004000,
	DMC_CONVERT 	= 0x0000000000008000,
	DMC_DELTAMODE	= 0x0000000000010000,
	DMC_ENDUSE		= 0x0000000000020000,
	DMC_ENVIRONMENT	= 0x0000000000040000,
	DMC_GUI			= 0x0000000000080000,
	DMC_HTTPCLIENT	= 0x0000000000100000,
	DMC_INSTANCE	= 0x0000000000200000,
	DMC_INTERPOLATE	= 0x0000000000400000,
	DMC_JOB			= 0x0000000000800000,
	DMC_KML			= 0x0000000001000000,
	DMC_LEGAL		= 0x0000000002000000,
	DMC_LINK		= 0x0000000004000000,
	DMC_LIST		= 0x0000000008000000,
	DMC_XML			= 0x0000000010000000,
	DMC_LOADSHAPE	= 0x0000000020000000,
	DMC_LOCALE		= 0x0000000040000000,
	DMC_LOCK		= 0x0000000080000000,
	DMC_MATCH		= 0x0000000100000000,
	DMC_MATLAB		= 0x0000000200000000,
	DMC_PROPERTY	= 0x0000000400000000,
	DMC_RANDOM		= 0x0000000800000000,
	DMC_REALTIME	= 0x0000001000000000,
	DMC_SANITIZE	= 0x0000002000000000,
	DMC_SAVE		= 0x0000004000000000,
	DMC_SCHEDULE	= 0x0000008000000000,
	DMC_SERVER		= 0x0000010000000000,
	DMC_SETUP		= 0x0000020000000000,
	DMC_STREAM		= 0x0000040000000000,
	DMC_TEST		= 0x0000080000000000,
	DMC_THREADPOOL	= 0x0000100000000000,
	DMC_TRANSFORM	= 0x0000200000000000,
	DMC_HTTP		= 0x0000400000000000,
	DMC_UNIT		= 0x0000800000000000,
	DMC_VALIDATE	= 0x0001000000000000,
	DMC_VERSION		= 0x0002000000000000,
	DMC_XCORE		= 0x0004000000000000,
	DMC_NONE		=  0, /**< no messages allowed */
	DMC_ALL			= 0x0007ffffffffffff, /**< all messages allowed */
} GLOBALMESSAGECONTEXT;

/* Variable:  */
GLOBAL set global_output_message_context INIT(DMC_ALL); /**< message context control variable */

/* Variable:  */
GLOBAL int32 global_permissive_access INIT(FALSE); /**< enable permission property access */

typedef enum {
	VTC_SYNC	= 0x00,
	VTC_PRECOMMIT = 0x01,
	VTC_COMMIT 	= 0x02,
} GLOBALVALIDTOCONTEXT;

/* Variable:  */
GLOBAL bool global_validto_context INIT(VTC_SYNC); /**< events for which valid_to applies, rather than just sync passes */

/* Variable:  */
GLOBAL char1024 global_timezone_locale INIT("UTC"); /**< timezone specification */

/* Type: GLMSAVEOPTIONS */
typedef enum {
	GSO_LEGACY 		= 0x0000,
	GSO_NOINTERNALS = 0x0001,
	GSO_NOMACROS 	= 0x0002,
	GSO_NOGLOBALS	= 0x0004,
	GSO_NODEFAULTS	= 0x0008,
	GSO_MINIMAL 	= 0x000f,
	GSO_ORIGINAL	= 0x0010,
} GLMSAVEOPTIONS;

/* Variable: */
GLOBAL bool global_ignore_errors INIT(FALSE); 

/* Type: FILESAVEOPTIONS */
typedef enum 
{
	FSO_MODULES 	= 0x0001,
	FSO_PROPERTIES	= 0x0002,
	FSO_CLASSES		= 0x0004,
	FSO_GLOBALS		= 0x0008,
	FSO_OBJECTS		= 0x0010,
	FSO_SCHEDULES   = 0x0020,
	FSO_FILTERS     = 0x0040,
	FSO_SCRIPTS     = 0x0080,
	FSO_CLOCK		= 0x0100,
	FSO_ALL         = 0x01ff,
	FSO_MINIMAL		= 0x8000,
	FSO_INITIAL     = 0x4000,
	FSO_ALLMINIMAL	= (FSO_ALL|FSO_MINIMAL),
	FSO_ALLINITIAL	= (FSO_ALLMINIMAL|FSO_INITIAL),
} FILESAVEOPTIONS;

/* Variable:  */
GLOBAL set global_glm_save_options INIT(GSO_LEGACY);	/**< GLM save options */

/* Variable: global_filesave_options */
GLOBAL set global_filesave_options INIT(FSO_ALL); 		/**< save options */

/* Variable: global_datadir */
GLOBAL char1024 global_datadir INIT("");

/* Variable: global_pythonpath */
GLOBAL char1024 global_pythonpath INIT(".");

/* Variable: global_rusage_rate */
GLOBAL int64 global_rusage_rate INIT(0);

/* Variable: global_rusage_file */
GLOBAL char1024 global_rusage_file INIT("gridlabd-rusage.csv");

GLOBAL char1024 global_rusage_data INIT("{}");

#undef GLOBAL
#undef INIT

#ifdef __cplusplus
class GldMain;

/*	Class: GldGlobalvar
		Global variable access class
 */
class GldGlobalvar 
{
private:
	GldMain *my_instance;
	GLOBALVAR *spec;
public:
	GldGlobalvar(GldMain *instance, const char *name, const char *value, PROPERTYACCESS access = PA_PUBLIC, const char *description = NULL, bool is_deprecated = false);
	GldGlobalvar(GldMain *instance, const char *name, int64 *value, PROPERTYACCESS access = PA_PUBLIC, const char *description = NULL, bool is_deprecated = false);
	GldGlobalvar(GldMain *instance, const char *name, int32 *value, PROPERTYACCESS access = PA_PUBLIC, const char *description = NULL, bool is_deprecated = false);
	GldGlobalvar(GldMain *instance, const char *name, int16 *value, PROPERTYACCESS access = PA_PUBLIC, const char *description = NULL, bool is_deprecated = false);
	GldGlobalvar(GldMain *instance, const char *name, double *value, const char *unit = NULL, PROPERTYACCESS access = PA_PUBLIC, const char *description = NULL, bool is_deprecated = false);
	GldGlobalvar(GldMain *instance, const char *name, complex *value, const char *unit = NULL, PROPERTYACCESS access = PA_PUBLIC, const char *description = NULL, bool is_deprecated = false);
	GldGlobalvar(GldMain *instance, const char *name, enumeration *value, KEYWORD *keys, PROPERTYACCESS access = PA_PUBLIC, const char *description = NULL, bool is_deprecated = false);
	GldGlobalvar(GldMain *instance, const char *name, set *value, KEYWORD *keys, PROPERTYACCESS access = PA_PUBLIC, const char *description = NULL, bool is_deprecated = false);
	~GldGlobalvar(void);
public: // accessors
	inline void set_callback(void (*callback)(const char *)) { if (!spec) throw "GldGlobavar::set_callback(): spec is NULL"; spec->callback = callback;};
};

class GldGlobals 
{
private:
	GldMain &instance;
	GLOBALVAR *varlist;
	GLOBALVAR *last;
public:
	// Constructor: GldGlobals
	//	Create a global environment for a simulation instance
	GldGlobals(GldMain *inst);
	// Destructor: ~GldGlobals
	//	Destroy a global environment
	~GldGlobals(void);
public:
	// Method: init
	STATUS init(void);
	// Method: find
	GLOBALVAR *find(const char *name);
	// Method: getnext
	GLOBALVAR *getnext(const GLOBALVAR *previous);
	// Method: restore
	void restore(GLOBALVAR *previous);
	// Method: push
	void push(char *name, char *value);
	// Method: create
	GLOBALVAR *create(const char *name, ...);
	// Method: create_v
	GLOBALVAR *create_v(const char *name, va_list ptr);
	// Method: setvar
	STATUS setvar(const char *def, ...);
	// Method: setvar_v
	STATUS setvar_v(const char *def, va_list ptr);
	// Method: isdefined
	bool isdefined(const char *name);
	// Method: getvar
	const char *getvar(const char *name, char *buffer, size_t size);
	// Method: getcount
	size_t getcount(void);
	// Method: dump
	void dump(void);
	// Method: remote_read
	void *remote_read(void *local, GLOBALVAR *var);
	// Method: remote_write
	void remote_write(void *local, GLOBALVAR *var);
	// Method: saveall
	size_t saveall(FILE *fp);
private:
	// Method: parameter_expansion
	bool parameter_expansion(char *buffer, size_t size, const char *spec);
};
#endif

#endif /* _GLOBAL_H */
/**@}**/
