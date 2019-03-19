/** $Id: cmdarg.h 4738 2014-07-03 00:55:39Z dchassin $
	Copyright (C) 2008 Battelle Memorial Institute
	@file cmdarg.h
	@addtogroup cmdarg
 @{
 **/


#ifndef _CMDARG_H
#define _CMDARG_H

#ifdef __cplusplus
extern "C" {
#endif

#define CMDOK (-1)
#define CMDERR (-2)

STATUS cmdarg_load(int argc,char *argv[]);
int cmdarg_runoption(const char *value);

typedef struct s_pntree{
	const char *name;
	CLASS *oclass;
	struct s_pntree *left, *right;
} pntree;

typedef struct s_cmdarg {
	const char *lopt;
	const char *sopt;
	int (*call)(void*,int,char*[]);
	const char *args;
	const char *desc;
} CMDARG;

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

class GldCmdarg {
private:
	class GldMain *instance;
	clock_t loader_time;
public:
	GldCmdarg(class GldMain *);
	~GldCmdarg();

public:
	STATUS load(int argc, char *argv[]);
	int runoption(const char *value);
	STATUS no_cmdargs(void);
	STATUS load_module_list(FILE *fd,int* test_mod_num);
	void modhelp_alpha(pntree **ctree, CLASS *oclass);
	void set_tabs(char *tabs, int tabdepth);
	void print_class_d(CLASS *oclass, int tabdepth);
	void print_class(CLASS *oclass);
	void print_modhelp_tree(pntree *ctree);
	int help(int argc, char *argv[]);
	int copyright(int argc, char *argv[]);
	int warn(int argc, char *argv[]);
	int bothstdout(int argc, char *argv[]);
	int check(int argc, char *argv[]);
	int debug(int argc, char *argv[]);
	int debugger(int argc, char *argv[]);
	int dumpall(int argc, char *argv[]);
	int quiet(int argc, char *argv[]);
	int verbose(int argc, char *argv[]);
	int _check_version(int argc, char *argv[]);
	int profile(int argc, char *argv[]);
	int mt_profile(int argc, char *argv[]);
	int pauseatexit(int argc, char *argv[]);
	int compile(int argc, char *argv[]);
	int license(int argc, char *argv[]);
	int server_portnum(int argc, char *argv[]);
	int server_inaddr(int argc, char *argv[]);
	int version(int argc, char *argv[]);
	int dsttest(int argc, char *argv[]);
	int randtest(int argc, char *argv[]);
	int unitstest(int argc, char *argv[]);
	int scheduletest(int argc, char *argv[]);
	int loadshapetest(int argc, char *argv[]);
	int endusetest(int argc, char *argv[]);
	int xmlstrict(int argc, char *argv[]);
	int globaldump(int argc, char *argv[]);
	int relax(int argc, char *argv[]);
	int pidfile(int argc, char *argv[]);
	int kml(int argc, char *argv[]);
	int avlbalance(int argc, char *argv[]);
	int testall(int argc, char *argv[]);
	int modhelp(int argc, char *argv[]);
	int modlist(int argc, char *argv[]);
	int modtest(int argc, char *argv[]);
	int test(int argc, char *argv[]);
	int define(int argc, char *argv[]);
	int globals(int argc, char *argv[]);
	int redirect(int argc, char *argv[]);
	int libinfo(int argc, char *argv[]);
	int threadcount(int argc, char *argv[]);
	int output(int argc, char *argv[]);
	int environment(int argc, char *argv[]);
	int xmlencoding(int argc, char *argv[]);
	int xsd(int argc, char *argv[]);
	int xsl(int argc, char *argv[]);
	int _stream(int argc, char *argv[]);
	int server(int argc, char *argv[]);
	int clearmap(int argc, char *argv[]);
	int pstatus(int argc, char *argv[]);
	int pkill(int argc, char *argv[]);
	int plist(int argc, char *argv[]);
	int pcontrol(int argc, char *argv[]);
	int info(int argc, char *argv[]);
	int slave(int argc, char *argv[]);
	int slavenode(int argc, char *argv[]);
	int slave_id(int argc, char *argv[]);
	int example(int argc, char *argv[]);
	int mclassdef(int argc, char *argv[]);
	int locktest(int argc, char *argv[]);
	int workdir(int argc, char *argv[]);
	int local_daemon(int argc, char *argv[]);
	int remote_client(int argc, char *argv[]);
	int printenv(int argc, char *argv[]);
	int origin(int argc, char *argv[]);
};

#endif

#endif
/**@}**/

