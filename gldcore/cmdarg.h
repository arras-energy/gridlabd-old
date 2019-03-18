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
	char *name;
	CLASS *oclass;
	struct s_pntree *left, *right;
} pntree;

typedef struct s_cmdarg {
	char *lopt;
	char *sopt;
	int (*call)(int argc, char *argv[]);
	char *args;
	char *desc;
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
	STATUS no_cmdargs(void);

public:
	STATUS load_module_list(FILE *fd,int* test_mod_num);
	void modhelp_alpha(pntree **ctree, CLASS *oclass);
	void set_tabs(char *tabs, int tabdepth);
	void print_class_d(CLASS *oclass, int tabdepth);
	void print_class(CLASS *oclass);
	void print_modhelp_tree(pntree *ctree);
	int help(int argc, char *argv[]);
	int copyright(int argc, char *argv[]);
};

#endif

#endif
/**@}**/

