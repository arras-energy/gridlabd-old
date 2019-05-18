/** $Id: cmdarg.h 4738 2014-07-03 00:55:39Z dchassin $
	Copyright (C) 2008 Battelle Memorial Institute
	@file cmdarg.h
	@addtogroup cmdarg
 @{
 **/


#ifndef _CMDARG_H
#define _CMDARG_H

#ifndef _GLDCORE_H
#error "this header may only be included from gldcore.h"
#endif

#define CMDOK (-1)
#define CMDERR (-2)

// deprecated as of 4.2
DEPRECATED CDECL STATUS cmdarg_load(int argc, const char *argv[]);
DEPRECATED CDECL int cmdarg_runoption(const char *value);
DEPRECATED typedef struct s_pntree
{
	const char *name;
	CLASS *oclass;
	struct s_pntree *left, *right;
} pntree;
DEPRECATED typedef int (*CMDARGCALL)(void*,int,const char*[]);
DEPRECATED typedef struct s_cmdarg 
{
	const char *lopt;
	const char *sopt;
	CMDARGCALL call;
	const char *args;
	const char *desc;
} CMDARG;

#ifdef __cplusplus // DEPRECATED

class GldCmdarg {
private:
	class GldMain *instance;
	clock_t loader_time;
public:
	inline GldMain *get_instance() { return instance; };
	inline clock_t get_loader_time() { return loader_time; };
public:
	GldCmdarg(class GldMain *);
	~GldCmdarg();

public:
	STATUS load(int argc, const char *argv[]);
	int runoption(const char *value);
	STATUS no_cmdargs(void);
	STATUS load_module_list(FILE *fd,int* test_mod_num);
	void modhelp_alpha(pntree **ctree, CLASS *oclass);
	void set_tabs(char *tabs, int tabdepth);
	void print_class_d(CLASS *oclass, int tabdepth);
	void print_class(CLASS *oclass);
	void print_modhelp_tree(pntree *ctree);
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

