/*	File: cmdarg.h 
 
 	Copyright (C) 2008, Battelle Memorial Institute

 */

#ifndef _CMDARG_H
#define _CMDARG_H

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

/*	Section: Command Argument Processing

	Typedef: CMDSTATUS
		See <e_cmdstatus>

	Enum: e_cmdstatus
	CMDOK = (-1) -  Command processing ok
	CMDERR = (-2) - Command processing failed
 */
typedef enum e_cmdstatus {
	CMDOK = -1,
	CMDERR = -2,
} CMDSTATUS;

/*	Typedef: PNTREE
		See <s_pntree>

	Structure: s_pntree
	name - item name
	oclass - class reference
	left - left branch reference
	right - right branch reference

	Sorting tree for <modhelp>.
 */
DEPRECATED typedef struct s_pntree
{
	const char *name;
	CLASS *oclass;
	struct s_pntree *left, *right;
} PNTREE;

/*	Typedef: CMDARGCALL
		Command argument processing function
 */
DEPRECATED typedef int (*CMDARGCALL)(void*,int,const char*[]);

/*	Typedef: CMDARG
		See <s_cmdarg>.

	Structure: s_cmdarg
	lopt - text for long option string (if any)
	sopt - text for short option string (if any)
	call - command procession function call
	args - description of argument list
	desc - description of command option
 */
DEPRECATED typedef struct s_cmdarg 
{
	const char *lopt;
	const char *sopt;
	CMDARGCALL call;
	const char *args;
	const char *desc;
} CMDARG;

/*	Function: cmdarg_load
		This function is obsolete.
 */
DEPRECATED CDECL STATUS cmdarg_load(int argc, const char *argv[]);

/*	Function: cmdarg_runoption
		This function is obsolete.
 */
DEPRECATED CDECL int cmdarg_runoption(const char *value);

#ifdef __cplusplus // DEPRECATED

/*	Class: GldCmdarg

	This class implements the command line argument processor.
 */
class GldCmdarg {

private:
	class GldMain *instance;
	clock_t loader_time;

public:

	/*	Method: get_instance

			This function returns the main instance that this command
			line processor is used for.
	 */ 
	inline GldMain *get_instance() { return instance; };

	/*	Method: get_loader_time

			This function returns the clock time used by the loader.
	 */
	inline clock_t get_loader_time() { return loader_time; };

public:

	/*	Constructor: GldCmdarg
	
			This constructor is used to build a command line argument processor
			for an instance.  

			If argc == 0, then nothing is processed.  
			If argc > 0 and argv != NULL, then the arguments are processed immediately in the
			order in which they are presented in argv.  

			If the load fails, a GldException is thrown.

			Caveat: environ set is not supported yet. If environ is set, a GldException is thrown.
	 */
	GldCmdarg(class GldMain *, int argc = 0, const char *argv[] = NULL, const char *environ[] = NULL);

	// Destructor: ~GldCmdarg
	~GldCmdarg();

public:
	STATUS load(int argc, const char *argv[]);
	int runoption(const char *value);
	STATUS no_cmdargs(void);
	STATUS load_module_list(FILE *fd,int* test_mod_num);
	void modhelp_alpha(PNTREE **ctree, CLASS *oclass);
	void set_tabs(char *tabs, int tabdepth);
	void print_class_d(CLASS *oclass, int tabdepth);
	void print_class(CLASS *oclass);
	void print_modhelp_tree(PNTREE *ctree);
	int help(int argc, const char *argv[]);
	int copyright(int argc, const char *argv[]);
	int warn(int argc, const char *argv[]);
	int bothstdout(int argc, const char *argv[]);
	int check(int argc, const char *argv[]);
	int debug(int argc, const char *argv[]);
	int debugger(int argc, const char *argv[]);
	int dumpall(int argc, const char *argv[]);
	int quiet(int argc, const char *argv[]);
	int verbose(int argc, const char *argv[]);
	int _check_version(int argc, const char *argv[]);
	int profile(int argc, const char *argv[]);
	int mt_profile(int argc, const char *argv[]);
	int pauseatexit(int argc, const char *argv[]);
	int compile(int argc, const char *argv[]);
	int license(int argc, const char *argv[]);
	int server_portnum(int argc, const char *argv[]);
	int server_inaddr(int argc, const char *argv[]);
	int version(int argc, const char *argv[]);
	int dsttest(int argc, const char *argv[]);
	int randtest(int argc, const char *argv[]);
	int unitstest(int argc, const char *argv[]);
	int scheduletest(int argc, const char *argv[]);
	int loadshapetest(int argc, const char *argv[]);
	int endusetest(int argc, const char *argv[]);
	int xmlstrict(int argc, const char *argv[]);
	int globaldump(int argc, const char *argv[]);
	int relax(int argc, const char *argv[]);
	int pidfile(int argc, const char *argv[]);
	int kml(int argc, const char *argv[]);
	int avlbalance(int argc, const char *argv[]);
	int testall(int argc, const char *argv[]);
	int modhelp(int argc, const char *argv[]);
	int modlist(int argc, const char *argv[]);
	int modtest(int argc, const char *argv[]);
	int test(int argc, const char *argv[]);
	int define(int argc, const char *argv[]);
	int globals(int argc, const char *argv[]);
	int redirect(int argc, const char *argv[]);
	int libinfo(int argc, const char *argv[]);
	int threadcount(int argc, const char *argv[]);
	int output(int argc, const char *argv[]);
	int environment(int argc, const char *argv[]);
	int xmlencoding(int argc, const char *argv[]);
	int xsd(int argc, const char *argv[]);
	int xsl(int argc, const char *argv[]);
	int _stream(int argc, const char *argv[]);
	int server(int argc, const char *argv[]);
	int clearmap(int argc, const char *argv[]);
	int pstatus(int argc, const char *argv[]);
	int pkill(int argc, const char *argv[]);
	int plist(int argc, const char *argv[]);
	int pcontrol(int argc, const char *argv[]);
	int info(int argc, const char *argv[]);
	int slave(int argc, const char *argv[]);
	int slavenode(int argc, const char *argv[]);
	int slave_id(int argc, const char *argv[]);
	int example(int argc, const char *argv[]);
	int mclassdef(int argc, const char *argv[]);
	int locktest(int argc, const char *argv[]);
	int workdir(int argc, const char *argv[]);
	int local_daemon(int argc, const char *argv[]);
	int remote_client(int argc, const char *argv[]);
	int printenv(int argc, const char *argv[]);
	int origin(int argc, const char *argv[]);
};

#endif // DEPRECATED

#endif
/**@}**/

