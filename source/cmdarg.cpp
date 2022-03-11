/** cmdarg.cpp
	Copyright (C) 2008 Battelle Memorial Institute

	@file cmdarg.cpp
	@addtogroup cmdarg Command-line arguments
	@ingroup core

	The command-line argument processing module processes arguments as they are encountered.

	Use the --help command line option to obtain a list of valid options.

 @{
 **/

#include "gldcore.h"
#include <iostream>
#include <map>

using namespace std;

SET_MYCONTEXT(DMC_CMDARG)

/* TODO: remove when reentrant code is completed */
#include "main.h"
extern GldMain *my_instance;

/* TODO: remove when daemon.cpp is reentrant */
DEPRECATED CDECL STATUS cmdarg_load(int argc, /**< the number of arguments in \p argv */
				   const char *argv[]) /**< a list pointers to the argument string */
{
	return my_instance->get_cmdarg()->load(argc,argv);
}

/* TODO: remove then load.c is reentrant */
DEPRECATED CDECL int cmdarg_runoption(const char *value)
{
	return my_instance->get_cmdarg()->runoption(value);
}

///////////////////////////////////////////////
// Command argument processor implementation
///////////////////////////////////////////////

GldCmdarg::GldCmdarg(GldMain *main, int argc, const char *argv[], const char *environ[])
{
	instance = main;
	loader_time = 0;
	if ( argc > 0 && argv != NULL && load(argc,argv) == FAILED )
		throw new GldException("GldCmdarg(): load failed");
	if ( environ )
		throw new GldException("GldCmdarg(): environ set not supported");
}

GldCmdarg::~GldCmdarg(void)
{

}

DEPRECATED STATUS load_module_list(FILE *fd,int* test_mod_num)
{
	return my_instance->get_cmdarg()->load_module_list(fd,test_mod_num);
}
DEPRECATED STATUS GldCmdarg::load_module_list(FILE *fd,int* test_mod_num)
{
	char mod_test[1024];
	char line[100];
	while(fscanf(fd,"%s",line) != EOF)
	{
		printf("Line: %s",line);
		snprintf(mod_test,sizeof(mod_test)-1,"mod_test%d=%s",(*test_mod_num)++,line);
		if (global_setvar(mod_test)!=SUCCESS)
		{
			output_fatal("Unable to store module name");
			/*	TROUBLESHOOT
				This error is caused by a failure to set up a module test, which
				requires that the name module being tested be stored in a global
				variable called mod_test<num>.  The root cause will be identified
				by determining what error in the global_setvar call occurred.
			 */
			return FAILED;
		}
	}

	return SUCCESS;
}

DEPRECATED void modhelp_alpha(PNTREE **ctree, CLASS *oclass)
{
	my_instance->get_cmdarg()->modhelp_alpha(ctree,oclass);
}
void GldCmdarg::modhelp_alpha(PNTREE **ctree, CLASS *oclass)
{
	int cmpval = 0;
	PNTREE *targ = *ctree;

	cmpval = strcmp(oclass->name, targ->name);

	if(cmpval == 0){
		; /* exception? */
	} if(cmpval < 0){ /*  class < root ~ go left */
		if(targ->left == NULL){
			targ->left = (PNTREE *)malloc(sizeof(PNTREE));
			memset(targ->left, 0, sizeof(PNTREE));
			targ->left->name = oclass->name;
			targ->left->name = oclass->name;
			targ->left->oclass = oclass;
			targ->left->left = targ->left->right = 0;
		} else { /* non-null, follow upwards */
			modhelp_alpha(&targ->left, oclass);
		}
	} else {
		if(targ->right == NULL){
			targ->right = (PNTREE *)malloc(sizeof(PNTREE));
			memset(targ->right, 0, sizeof(PNTREE));
			targ->right->name = oclass->name;
			targ->right->name = oclass->name;
			targ->right->oclass = oclass;
			targ->right->right = targ->right->left = 0;
		} else {
			modhelp_alpha(&targ->right, oclass);
		}
	}
}

DEPRECATED void set_tabs(char *tabs, int tabdepth)
{
	my_instance->get_cmdarg()->set_tabs(tabs,tabdepth);
}
void GldCmdarg::set_tabs(char *tabs, int tabdepth)
{
	if(tabdepth > 32){
		throw_exception("print_class_d: tabdepth > 32, which is mightily deep!");
		/* TROUBLESHOOT
			This means that there is very deep nesting and this is unexpected.
			This suggests a problem with the internal model and should be reported.
		 */
	} else {
		int i = 0;
		memset(tabs, 0, 33);
		for(i = 0; i < tabdepth; ++i)
			tabs[i] = '\t';
	}
}

DEPRECATED void print_class_d(CLASS *oclass, int tabdepth)
{
	my_instance->get_cmdarg()->print_class_d(oclass,tabdepth);
}
void GldCmdarg::print_class_d(CLASS *oclass, int tabdepth)
{
	PROPERTY *prop;
	FUNCTION *func;
	char tabs[33];

	set_tabs(tabs, tabdepth);

	printf("%sclass %s {\n", tabs, oclass->name);
	if (oclass->parent){
		printf("%s\tparent %s;\n", tabs, oclass->parent->name);
		print_class_d(oclass->parent, tabdepth+1);
	}
	for (func=oclass->fmap; func!=NULL && func->oclass==oclass; func=func->next)
		printf( "%s\tfunction %s();\n", tabs, func->name);
	for (prop=oclass->pmap; prop!=NULL && prop->oclass==oclass; prop=prop->next)
	{
		const char *propname = class_get_property_typename(prop->ptype);
		if (propname!=NULL)
		{
			if ( (prop->access&PA_HIDDEN)==PA_HIDDEN )
				continue;
			if (prop->unit != NULL)
			{
				printf("%s\t%s %s[%s];", tabs, propname, prop->name, prop->unit->name);
			}
			else if (prop->ptype==PT_set || prop->ptype==PT_enumeration)
			{
				KEYWORD *key;
				printf("%s\t%s {", tabs, propname);
				for (key=prop->keywords; key!=NULL; key=key->next)
					printf("%s=%" FMT_INT64 "u%s", key->name, (int64)key->value, key->next==NULL?"":", ");
				printf("} %s;", prop->name);
			}
			else
			{
				printf("%s\t%s %s;", tabs, propname, prop->name);
			}
			char flags[1024] = "";
			if ( prop->flags&PF_DEPRECATED ) strcat(flags,flags[0]?",":"("),strcat(flags,"DEPRECATED");
			if ( prop->flags&PF_REQUIRED ) strcat(flags,flags[0]?",":"("),strcat(flags,"REQUIRED");
			if ( prop->flags&PF_OUTPUT ) strcat(flags,flags[0]?",":"("),strcat(flags,"OUTPUT");
			if ( prop->flags&PF_DYNAMIC ) strcat(flags,flags[0]?",":"("),strcat(flags,"DYNAMIC");
			if ( flags[0] ) strcat(flags,") ");
			if ( flags[0] != '\0' || prop->description != NULL )
			printf(" // %s%s",flags,prop->description?prop->description:"");
			printf("\n");
		}
	}
	printf("%s}\n\n", tabs);
}

DEPRECATED void print_class(CLASS *oclass)
{
	my_instance->get_cmdarg()->print_class(oclass);
}
void GldCmdarg::print_class(CLASS *oclass)
{
	print_class_d(oclass, 0);
}

DEPRECATED void print_modhelp_tree(PNTREE *ctree)
{
	my_instance->get_cmdarg()->print_modhelp_tree(ctree);
}
void GldCmdarg::print_modhelp_tree(PNTREE *ctree)
{
	if(ctree->left != NULL){
		print_modhelp_tree(ctree->left);
		free(ctree->left);
		ctree->left = 0;
	}
	print_class(ctree->oclass);
	if(ctree->right != NULL){
		print_modhelp_tree(ctree->right);
		free(ctree->right);
		ctree->right = 0;
	}
}

DEPRECATED static int compare(const void *a, const void *b)
{
	return stricmp(*(char**)a,*(char**)b);
}

DEPRECATED static int help(void *main, int argc, const char *argv[]);

/************************************************************************/
/* COMMAND LINE PARSING ROUTINES
 *
 * All cmdline parsing routines must use the prototype int (*)(int,char *[])
 *
 * The return value must be the number of args processed (excluding primary
 * one). A return value of CMDOK indicates that processing must stop immediately
 * and the return status is the current status.  A return value of CMDERR indicates
 * that processing must stop immediately and FAILED status is returned.
 *
 */

STATUS GldCmdarg::no_cmdargs(void)
{
	char guiname[1024] = "gridlabd-editor.py";
	char guipath[1024];
	if ( find_file(guiname,NULL,R_OK,guipath,sizeof(guipath)) )
	{
		char command[2048];
		snprintf(command,sizeof(command),"/usr/local/bin/python3 %s &",guipath);
		system(command);
		return SUCCESS;
	}
	else
	{
		output_error("%s not found",guiname);
		return FAILED;
	}
}

DEPRECATED static int copyright(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->copyright(argc,argv);
}
int GldCmdarg::copyright(int argc, const char *argv[])
{
	legal_notice();
	return 0;
}

DEPRECATED static int warn(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->warn(argc,argv);
}
int GldCmdarg::warn(int argc, const char *argv[])
{
	global_warn_mode = !global_warn_mode;
	return 0;
}

DEPRECATED static int bothstdout(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->bothstdout(argc,argv);
}
int GldCmdarg::bothstdout(int argc, const char *argv[])
{
	output_both_stdout();
	return 0;
}

DEPRECATED static int check(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->check(argc,argv);
}
int GldCmdarg::check(int argc, const char *argv[])
{
	/* check main core implementation */
	if ( property_check()==FAILED )
	{
		throw_exception("main core property implementation failed size checks");
	}
	global_runchecks = !global_runchecks;
	return 0;
}

DEPRECATED static int debug(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->debug(argc,argv);
}
int GldCmdarg::debug(int argc, const char *argv[])
{
	global_debug_output = !global_debug_output;
	return 0;
}

DEPRECATED static int debugger(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->debugger(argc,argv);
}
int GldCmdarg::debugger(int argc, const char *argv[])
{
	global_debug_mode = 1;
	global_debug_output = 1;
	return 0;
}

DEPRECATED static int dumpall(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->dumpall(argc,argv);
}
int GldCmdarg::dumpall(int argc, const char *argv[])
{
	global_dumpall = !global_dumpall;
	return 0;
}

DEPRECATED static int quiet(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->quiet(argc,argv);
}
int GldCmdarg::quiet(int argc, const char *argv[])
{
	global_quiet_mode = !global_quiet_mode;
	return 0;
}

DEPRECATED static int verbose(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->verbose(argc,argv);
}
int GldCmdarg::verbose(int argc, const char *argv[])
{
	global_verbose_mode=!global_verbose_mode;
	return 0;
}

DEPRECATED static int _check_version(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->_check_version(argc,argv);
}
int GldCmdarg::_check_version(int argc, const char *argv[])
{
	check_version(0);
	return 0;
}

DEPRECATED static int profile(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->profile(argc,argv);
}
int GldCmdarg::profile(int argc, const char *argv[])
{
	const char *opt = strchr(argv[0],'=');
	if ( opt++ != NULL )
	{
		if ( strcmp(opt,"text") == 0 )
		{
			global_profile_output_format = POF_TEXT;
		}
		else if ( strcmp(opt,"csv") == 0 )
		{
			global_profile_output_format = POF_CSV;
		}
		else if ( strcmp(opt,"json") == 0 )
		{
			global_profile_output_format = POF_JSON;
		}
		else
		{
			output_error("profiler option '%s' is not valid",opt);
			return CMDERR;
		}
	}
	global_profiler = !global_profiler;
	return 0;
}

DEPRECATED static int mt_profile(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->mt_profile(argc,argv);
}
int GldCmdarg::mt_profile(int argc, const char *argv[])
{
	if ( argc>1 )
	{
		global_profiler = 1;
		global_mt_analysis = atoi(*++argv);
		argc--;
		if ( global_threadcount>1 )
		{
			IN_MYCONTEXT output_warning("--mt_profile forces threadcount=1");
		}
		if ( global_mt_analysis<2 )
		{
			output_error("--mt_profile <n-threads> value must be 2 or greater");
			return CMDERR;
		}
		else
		{
			global_threadcount = 1;
			return 1;
		}
	}
	else
	{
		output_fatal("missing mt_profile thread count");
		return CMDERR;
	}
}

DEPRECATED static int pauseatexit(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->pauseatexit(argc,argv);
}
int GldCmdarg::pauseatexit(int argc, const char *argv[])
{
	global_pauseatexit = !global_pauseatexit;
	return 0;
}

DEPRECATED static int compile(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->compile(argc,argv);
}
int GldCmdarg::compile(int argc, const char *argv[])
{
	global_compileonly = !global_compileonly;
	return 0;
}

DEPRECATED static int library(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->library(argc,argv);	
}
int GldCmdarg::library(int argc, const char *argv[])
{
	if ( argc > 1 )
	{
		char pathname[4096];
		const char *etcpath = getenv("GLD_ETC");
		if ( etcpath == NULL )
		{
			etcpath = "/usr/local/share/gridlabd";
		}
		snprintf(pathname,sizeof(pathname),"%s/library/%s/%s/%s/%s",getenv("GLD_ETC"),(const char*)global_country,(const char*)global_region,(const char*)global_organization,argv[1]);
		return get_instance()->get_loader()->loadall_glm(pathname) == SUCCESS ? 1 : CMDERR;
	}
	else
	{
		output_fatal("missing library filename");
		return CMDERR;
	}
}

DEPRECATED static int _template(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->_template(argc,argv);	
}
int GldCmdarg::_template(int argc, const char *argv[])
{
	if ( argc > 1 )
	{
		char pathname[4096];
		const char *etcpath = getenv("GLD_ETC");
		if ( etcpath == NULL )
		{
			etcpath = "/usr/local/share/gridlabd";
		}
		snprintf(pathname,sizeof(pathname),"%s/template/%s/%s/%s/%s",getenv("GLD_ETC"),(const char*)global_country,(const char*)global_region,(const char*)global_organization,argv[1]);
		if ( strstr(global_pythonpath,pathname) == NULL )
		{
			if ( strcmp(global_pythonpath,":") != 0 )
			{
				strcat(global_pythonpath,":");
			}
			strcat(global_pythonpath,pathname);
		}
		snprintf(pathname,sizeof(pathname),"%s/template/%s/%s/%s/%s/%s.glm",getenv("GLD_ETC"),(const char*)global_country,(const char*)global_region,(const char*)global_organization,argv[1],argv[1]);
		return get_instance()->get_loader()->loadall_glm(pathname) == SUCCESS ? 1 : CMDERR;
	}
	else
	{
		output_fatal("missing template filename");
		return CMDERR;
	}
}

DEPRECATED static int initialize(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->initialize(argc,argv);
}
int GldCmdarg::initialize(int argc, const char *argv[])
{
	global_initializeonly = !global_initializeonly;
	return 0;
}

DEPRECATED static int license(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->license(argc,argv);
}
int GldCmdarg::license(int argc, const char *argv[])
{
	legal_license();
	return 0;
}

DEPRECATED static int server_portnum(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->server_portnum(argc,argv);
}
int GldCmdarg::server_portnum(int argc, const char *argv[])
{
	if (argc>1)
	{
		global_server_portnum = (argc--,atoi(*++argv));
		return 1;
	}
	else
	{
		output_fatal("missing server port number");
		/*	TROUBLESHOOT
			The <b>-P</b> or <b>--server_portnum</b> command line directive
			was not followed by a valid number.  The correct syntax is
			<b>-P <i>number</i></b> or <b>--server_portnum <i>number</i></b>.
		 */
		return CMDERR;
	}
}

DEPRECATED static int server_inaddr(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->server_inaddr(argc,argv);
}
int GldCmdarg::server_inaddr(int argc, const char *argv[])
{
	if ( argc>1 )
	{
		strncpy(global_server_inaddr,(argc--,*++argv),sizeof(global_server_inaddr)-1);
		return 1;
	}
	else
	{
		output_fatal("missing server ip address");
		/*	TROUBLESHOOT
			The <b>--server_inaddr</b> command line directive
			was not followed by a valid IP address.  The correct syntax is
			<b>--server_inaddr <i>interface address</i></b>.
		 */
		return CMDERR;
	}
}

DEPRECATED static int version(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->version(argc,argv);
}
int GldCmdarg::version(int argc, const char *argv[])
{
	if ( argv[0][9] == '=' && argv[0][10] == '-' )
	{
		if ( version_check(argv[0]+10) )
		{
			return CMDOK;
		} 
		output_error("version '%s' does not satisfy the version requirement '%s'",VERSION,argv[0]+10);
		return CMDERR;
	}
	const char *opt = strchr(argv[0],'=');
	if ( opt++ == NULL )
	{
		opt = "default";
	}
	if ( strcmp(opt,"default") == 0 )
	{
		output_message("%s %s-%d", PACKAGE_NAME, PACKAGE_VERSION, BUILDNUM);
		return 0;
	}
	else if ( strcmp(opt,"all" ) == 0 )
	{
		output_message("%s %s-%d (%s) "
#if defined MACOSX
			"Darwin"
#else // LINUX
			"Linux"
#endif
			, PACKAGE_NAME, PACKAGE_VERSION, BUILDNUM, BRANCH);
		return 0;
	}
	else if ( strcmp(opt,"number" ) == 0 || strcmp(opt,"version") == 0 )
	{
		output_message("%s", PACKAGE_VERSION);
		return 0;
	}
	else if ( strcmp(opt,"build") == 0 || strcmp(opt,"build_number") == 0 )
	{
		output_message("%d", BUILDNUM);
		return 0;
	}
	else if ( strcmp(opt,"package") == 0 || strcmp(opt,"application") == 0 )
	{
		output_message("%s", PACKAGE_NAME);
		return 0;
	}
	else if ( strcmp(opt,"branch") == 0 )
	{
		output_message("%s", BRANCH);
		return 0;
	}
	else if ( strcmp(opt,"git-branch") == 0 )
	{
		output_message("%s", BUILD_BRANCH);
		return 0;
	}
	else if ( strcmp(opt,"git-repo") == 0 )
	{
		output_message("%s", BUILD_URL);
		return 0;
	}
	else if ( strcmp(opt,"git-commit") == 0 )
	{
		output_message("%s", BUILD_ID);
		return 0;
	}
	else if ( strcmp(opt,"platform") == 0 || strcmp(opt,"system") == 0 )
	{
		output_message(
#if defined MACOSX
			"Darwin"
#else // LINUX
			"Linux"
#endif
		);
		return 0;
	}
	else if ( strcmp(opt,"release") == 0 )
	{
		output_message("%s",BUILD_RELEASE);
		return 0;
	}
	else if ( strcmp(opt,"commit") == 0 )
	{
		output_message("%s",BUILD_ID);
		return 0;
	}
	else if ( strcmp(opt,"email") == 0 )
	{
		output_message("%s",PACKAGE_BUGREPORT);
		return 0;
	}
	else if ( strcmp(opt,"origin") == 0 )
	{
		output_message("%s",BUILD_URL);
		return 0;
	}
	else if ( strcmp(opt,"install") == 0 )
	{
		// IMPORTANT: this needs to be consistent with Makefile.am, install.sh and build-aux/*.sh
		char tmp[1024];
		strcpy(tmp,global_execdir);
		char *p = strrchr(tmp,'/');
		if ( p != NULL && strcmp(p,"/bin") == 0 )
		{
			*p = '\0';
		}
		output_message("%s", tmp);
		return 0;
	}
	else if ( strcmp(opt,"name") == 0 )
	{
		// IMPORTANT: this needs to be consistent with Makefile.am, install.sh and build-aux/*.sh
		output_message("%s-%s-%d-%s", PACKAGE, PACKAGE_VERSION, BUILDNUM, BRANCH);
		return 0;
	}
	else if ( strcmp(opt,"json") == 0 )
	{
		bool old = global_suppress_repeat_messages;
		global_suppress_repeat_messages = false;
		output_message("{");
#define OUTPUT(TAG,FORMAT,VALUE) output_message("\t\"%s\" : \"" FORMAT "\",",TAG,VALUE)
#define OUTPUT_LAST(TAG,FORMAT,VALUE) output_message("\t\"%s\" : \"" FORMAT "\"\n}",TAG,VALUE)
#define OUTPUT_LIST_START(TAG) output_message("\t\"%s\" : [",TAG)
#define OUTPUT_LIST_ITEM(VALUE) output_message("\t\t\"%s\",",VALUE)
#define OUTPUT_LIST_END(VALUE) output_message("\t\t\"%s\"],",VALUE)
#define OUTPUT_MULTILINE(TAG,VALUE) {\
		const char *value = VALUE;\
		char *token=NULL, *last=NULL;\
		char buffer[strlen(value)+1];\
		strcpy(buffer,value);\
		OUTPUT_LIST_START(TAG);\
		while ( (token=strtok_r(token?NULL:buffer,"\n",&last)) != NULL )\
		{\
			OUTPUT_LIST_ITEM(token);\
		}\
		OUTPUT_LIST_END("");\
	}
		OUTPUT("application","%s",PACKAGE);
		OUTPUT("version","%s",PACKAGE_VERSION);
		OUTPUT("build_number","%06d",BUILDNUM);
		OUTPUT("branch","%s",BRANCH);
		OUTPUT("package-name","%s",PACKAGE_NAME);
		OUTPUT("options","%s",BUILD_OPTIONS);
		OUTPUT_MULTILINE("status",BUILD_STATUS);
		OUTPUT_MULTILINE("copyright",version_copyright());
		OUTPUT_MULTILINE("license",legal_license_text());
		OUTPUT("system","%s",BUILD_SYSTEM);
		char tmp[1024];
		strcpy(tmp,global_execdir);
		char *p = strrchr(tmp,'/');
		if ( p != NULL && strcmp(p,"/bin") == 0 )
		{
			*p = '\0';
		}
		OUTPUT("install","%s",tmp);
		output_message("\t\"name\" : \"%s-%s-%d-%s\",", PACKAGE, PACKAGE_VERSION, BUILDNUM, BRANCH);
		OUTPUT("release","%s",BUILD_RELEASE);
		OUTPUT("commit","%s",BUILD_ID);
		OUTPUT("email","%s",PACKAGE_BUGREPORT);
		OUTPUT_LAST("origin","%s",BUILD_URL);
		global_suppress_repeat_messages = old;
		return 0;
	}
	else
	{
		output_error("version option '%s' is not valid", opt);
		return CMDERR;
	}
}

DEPRECATED static int build_info(void *main, int argc, const char *argv[])
{
	enum e_format {RAW, JSON} format = RAW;
	int parsed = 0;
	if ( argc > 1 )
	{
		char tag[1024];
		if ( sscanf(argv[1],"format=%1023[a-z]",tag) == 1 )
		{
			if ( strcmp(tag,"raw") == 0 )
			{
				format = RAW;
				parsed = 1;
			}
			else if ( strcmp(tag,"json") == 0 )
			{
				format = JSON;
				parsed = 1;
			}
			else
			{
				output_error("build-info format '%s' is not valid",tag);
				return CMDERR;
			}
		}
	}
	char status[]=BUILD_STATUS, *ptr=NULL, *last=NULL;
	bool old = global_suppress_repeat_messages;
	global_suppress_repeat_messages = false;
	switch(format)
	{
		case JSON:
			output_message("{");
			output_message("\t\"application\": \"%s\",", PACKAGE_NAME);
			output_message("\t\"version\": \"%s\",", PACKAGE_VERSION);
			output_message("\t\"build\": \"%d\",", BUILDNUM);
			output_message("\t\"origin\": \"%s\",", BUILD_NAME);
			output_message("\t\"source\": \"%s\",", BUILD_URL);
			output_message("\t\"system\": \"%s\",", BUILD_SYSTEM);
			output_message("\t\"release\": \"%s\",", BUILD_RELEASE);
			output_message("\t\"id\": \"%s\",", BUILD_ID);
			output_message("\t\"options\": \"%s", BUILD_OPTIONS
#ifdef HAVE_NCURSES_H
				" ncurses"
#endif
				" python"
#ifdef HAVE_MYSQL
				" mysql"
#endif
#ifdef HAVE_MATLAB
				" matlab"
#endif
				"\","
		);
			output_message("\t\"status\": [");
			for ( ptr = strtok_r(status,"\n",&last) ; ptr != NULL ; ptr = strtok_r(NULL,"\n",&last) )
			{
				if ( strcmp(ptr,"") != 0 )
				{
					output_message("\t\t\"%s\",",ptr);
				}
			}
			output_message("\t\t\"\"]");
			output_message("}");
			break;
		case RAW:
		default:
			output_message("application: %s", PACKAGE_NAME);
			output_message("version: %s", PACKAGE_VERSION);
			output_message("build: %d", BUILDNUM);
			output_message("origin: %s", BUILD_NAME);
			output_message("source: %s", BUILD_URL);
			output_message("system: %s", BUILD_SYSTEM);
			output_message("release: %s", BUILD_RELEASE);
			output_message("id: %s", BUILD_ID);
			output_message("options: %s", BUILD_OPTIONS
#ifdef HAVE_NCURSES_H
				" ncurses"
#endif
				" python"
#ifdef HAVE_MYSQL
				" mysql"
#endif
#ifdef HAVE_MATLAB
				" matlab"
#endif
		);
			output_message("status: %s", BUILD_STATUS);
			break;
	}
	global_suppress_repeat_messages = old;

	return parsed;
}

DEPRECATED static int dsttest(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->dsttest(argc,argv);
}
int GldCmdarg::dsttest(int argc, const char *argv[])
{
	timestamp_test();
	return 0;
}

DEPRECATED static int randtest(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->randtest(argc,argv);
}
int GldCmdarg::randtest(int argc, const char *argv[])
{
	random_test();
	return 0;
}

DEPRECATED static int unitstest(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->unitstest(argc,argv);
}
int GldCmdarg::unitstest(int argc, const char *argv[])
{
	unit_test();
	return 0;
}

DEPRECATED static int scheduletest(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->scheduletest(argc,argv);
}
int GldCmdarg::scheduletest(int argc, const char *argv[])
{
	schedule_test();
	return 0;
}

DEPRECATED static int loadshapetest(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->loadshapetest(argc,argv);
}
int GldCmdarg::loadshapetest(int argc, const char *argv[])
{
	loadshape_test();
	return 0;
}

DEPRECATED static int endusetest(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->endusetest(argc,argv);
}
int GldCmdarg::endusetest(int argc, const char *argv[])
{
	enduse_test();
	return 0;
}

DEPRECATED static int xmlstrict(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->xmlstrict(argc,argv);
}
int GldCmdarg::xmlstrict(int argc, const char *argv[])
{
	global_xmlstrict = !global_xmlstrict;
	IN_MYCONTEXT output_verbose("xmlstrict is %s", global_xmlstrict?"enabled":"disabled");
	return 0;
}

DEPRECATED static int globaldump(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->globaldump(argc,argv);
}
int GldCmdarg::globaldump(int argc, const char *argv[])
{
	global_dump();
	return CMDOK;
}

DEPRECATED static int relax(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->relax(argc,argv);
}
int GldCmdarg::relax(int argc, const char *argv[])
{
	global_strictnames = FALSE;
	return 0;
}

DEPRECATED static int pidfile(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->pidfile(argc,argv);
}
int GldCmdarg::pidfile(int argc, const char *argv[])
{
	const char *filename = strchr(*argv,'=');
	if (filename==NULL)
		strcpy(global_pidfile,"gridlabd.pid");
	else
		strcpy(global_pidfile,filename+1);
	return 0;
}

DEPRECATED static int kml(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->kml(argc,argv);
}
int GldCmdarg::kml(int argc, const char *argv[])
{
	const char *filename = strchr(*argv,'=');
	if (filename)
		strcpy(global_kmlfile,filename+1);
	else
		strcpy(global_kmlfile,"gridlabd.kml");
	return 0;
}

DEPRECATED static int avlbalance(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->avlbalance(argc,argv);
}
int GldCmdarg::avlbalance(int argc, const char *argv[])
{
	global_no_balance = !global_no_balance;
	return 0;
}

DEPRECATED static int testall(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->testall(argc,argv);
}
int GldCmdarg::testall(int argc, const char *argv[])
{
	int test_mod_num = 1;
	FILE *fd = NULL;
	if(*++argv != NULL)
		fd = fopen(*argv,"r");
	else {
		output_fatal("no filename for testall");
		/*	TROUBLESHOOT
			The --testall parameter was found on the command line, but
			if was not followed by a filename containing the test
			description file.
		*/
		return CMDERR;
	}
	argc--;
	global_test_mode=TRUE;

	if(fd == NULL)
	{
		output_fatal("incorrect module list file name");
		/*	TROUBLESHOOT
			The --testall parameter was found on the command line, but
			if was not followed by a valid filename containing the test
			description file.
		*/
		return CMDERR;
	}
	if(load_module_list(fd,&test_mod_num) == FAILED)
		return CMDERR;
	return 1;
}

DEPRECATED static int modhelp(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->modhelp(argc,argv);
}
int GldCmdarg::modhelp(int argc, const char *argv[])
{
	const char *options = strchr(argv[0],'=');
	if ( options != NULL )
	{
		options++;
	}
	if ( argc > 1 )
	{
		MODULE *mod = NULL;
		CLASS *oclass = NULL;
		argv++;
		argc--;
		char module_name[1024];
		char class_name[1024];
		if ( sscanf(argv[0],"%[^:]:%s",module_name,class_name) == 1 )
		{
			// no class
			mod = module_load(module_name,0,NULL);
		}
		else
		{
			GLOBALVAR *var=NULL;
			mod = module_load(module_name,0,NULL);
			oclass = class_get_class_from_classname(class_name);
			if ( oclass == NULL )
			{
				output_fatal("Unable to find class '%s' in module '%s'", class_name, module_name);
				/*	TROUBLESHOOT
					The <b>--modhelp</b> parameter was found on the command line, but
					if was followed by a class specification that isn't valid.
					Verify that the class exists in the module you specified.
				*/
				return FAILED;
			}

			/* dump module globals */
			if ( ! options )
			{
				printf("module %s {\n", mod->name);
				while ((var=global_getnext(var))!=NULL)
				{
					PROPERTY *prop = var->prop;
					const char *proptype = class_get_property_typename(prop->ptype);
					if ( strncmp(var->prop->name,mod->name,strlen(mod->name))!=0 )
						continue;
					if ( (prop->access&PA_HIDDEN)==PA_HIDDEN )
						continue;
					if (proptype!=NULL)
					{
						if ( prop->unit!=NULL )
						{
							printf("\t%s %s[%s];", proptype, strrchr(prop->name,':')+1, prop->unit->name);
						}
						else if (prop->ptype==PT_set || prop->ptype==PT_enumeration)
						{
							KEYWORD *key;
							const char *fmt = ( sizeof(uint64) < sizeof(long long) ? "%s=%lu%s" : "%s=%llu%s");
							printf("\t%s {", proptype);
							for (key=prop->keywords; key!=NULL; key=key->next)
								printf(fmt, key->name, key->value, key->next==NULL?"":", ");
							printf("} %s;", strrchr(prop->name,':')+1);
						}
						else
						{
							printf("\t%s %s;", proptype, strrchr(prop->name,':')+1);
						}
						if (prop->description!=NULL)
							printf(" // %s%s",prop->flags&PF_DEPRECATED?"(DEPRECATED) ":"",prop->description);
						printf("\n");
					}
				}
				printf("}\n");
			}
		}
		if ( mod == NULL )
		{
			output_fatal("module %s is not found",*argv);
			/*	TROUBLESHOOT
				The <b>--modhelp</b> parameter was found on the command line, but
				if was followed by a module specification that isn't valid.
				Verify that the module exists in GridLAB-D's <b>lib</b> folder.
			*/
			return FAILED;
		}
		if ( options )
		{
			if ( strcmp(options,"md") == 0 )
			{
				module_help_md(mod,oclass);
			}
			else if ( strcmp(options,"json") == 0 )
			{
				GldJsonWriter writer("/dev/stdout");
				return writer.dump_modules() > 0 ? 1 : CMDERR;
			}
		}
		else if ( oclass != NULL )
		{
			print_class(oclass);
		}
		else
		{
			CLASS	*oclass;
			PNTREE	*ctree;
			/* lexographically sort all elements from class_get_first_class & oclass->next */

			oclass=class_get_first_class();
			ctree = (PNTREE *)malloc(sizeof(PNTREE));

			if(ctree == NULL){
				throw_exception("--modhelp: malloc failure");
				/* TROUBLESHOOT
					The memory allocation needed for module help to function has failed.  Try freeing up system memory and try again.
				 */
			}

			ctree->name = oclass->name;
			ctree->oclass = oclass;
			ctree->left = ctree->right = 0;

			for(; oclass != NULL; oclass = oclass->next){
				modhelp_alpha(&ctree, oclass);
				//print_class(oclass);
			}

			/* flatten tree */
			print_modhelp_tree(ctree);
		}
	}
	return 1;
}

DEPRECATED static int modlist(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->modlist(argc,argv);
}
int GldCmdarg::modlist(int arvc, const char *argv[])
{
	module_list();
	return 1;
}

DEPRECATED static int modtest(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->modtest(argc,argv);
}
int GldCmdarg::modtest(int argc, const char *argv[])
{
	if (argc>1)
	{
		MODULE *mod = module_load(argv[1],0,NULL);
		if (mod==NULL)
			output_fatal("module %s is not found",argv[1]);
			/*	TROUBLESHOOT
				The <b>--modtest</b> parameter was found on the command line, but
				if was followed by a module specification that isn't valid.
				Verify that the module exists in GridLAB-D's <b>lib</b> folder.
			*/
		else
		{
			argv++;argc--;
			if (mod->test==NULL)
				output_fatal("module %s does not implement a test routine", argv[0]);
				/*	TROUBLESHOOT
					The <b>--modtest</b> parameter was found on the command line, but
					if was followed by a specification for a module that doesn't
					implement any test procedures.  See the <b>--libinfo</b> command
					line parameter for information on which procedures the
					module supports.
				*/
			else
			{
				output_test("*** modtest of %s beginning ***", argv[0]);
				mod->test(0,NULL);
				output_test("*** modtest of %s ended ***", argv[0]);
			}
		}
	}
	else
	{
		output_fatal("definition is missing");
		/*	TROUBLESHOOT
			The <b>--modtest</b> parameter was found on the command line, but
			if was not followed by a module specification.  The correct
			syntax is <b>gridlabd --modtest <i>module_name</i></b>.
		*/
		return FAILED;
	}
	return 1;
}

DEPRECATED static int test(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->test(argc,argv);
}
int GldCmdarg::test(int argc, const char *argv[])
{
	int n=0;
	global_test_mode = TRUE;
	while (argc>1)
	{
		test_request(*++argv);
		argc--;
		n++;
	}
	return n;
}

DEPRECATED static int define(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->define(argc,argv);
}
int GldCmdarg::define(int argc, const char *argv[])
{
	if (argc>1)
	{
		bool namestate = global_strictnames;
		global_strictnames = FALSE;
		if (global_setvar(*++argv,NULL)==SUCCESS){
			argc--;
		}
		global_strictnames = namestate;
		return 1;
	}
	else
	{
		output_fatal("definition is missing");
		/* TROUBLESHOOT
			The <b>-D</b> or <b>--define</b> command line parameters was given, but
			it was not followed by a variable definition.  The correct syntax
			<b>-D </i>variable</i>=<i>value</i></b> or
			<b>--define </i>variable</i>=<i>value</i></b>
		 */
		return CMDERR;
	}
}

DEPRECATED static int globals(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->globals(argc,argv);
}
int GldCmdarg::globals(int argc, const char *argv[])
{
	bool use_json = (strstr(argv[0],"=json")!=NULL);
	const char *list[65536];
	int i, n=0;
	GLOBALVAR *var = NULL;

	/* load the list into the array */
	while ((var=global_getnext(var))!=NULL)
	{
		if ( n < (int)(sizeof(list)/sizeof(list[0])) )
			list[n++] = var->prop->name;
		else
		{
			output_fatal("--globals has insufficient buffer space to sort globals list");
			return FAILED;
		}
	}

	/* sort the array */
	qsort(list,n,sizeof(list[0]),compare);

	/* output sorted array */
	if ( use_json )
	{
		printf("{\n");
	}
	for (i=0; i<n; i++)
	{
		char buffer[1024];
		var = global_find(list[i]);
		if ( (var->prop->access&PA_HIDDEN)==PA_HIDDEN )
			continue;
		const char *value = global_getvar(var->prop->name,buffer,sizeof(buffer));
		if ( use_json && value != NULL )
		{
			printf("\t\"%s\" : {\n",var->prop->name);
			if ( value[0] == '"' )
			{
				printf("\t\t\"value\" : %s,\n",value);
			}
			else
			{
				printf("\t\t\"value\" : \"%s\",\n",value);
			}
			printf("\t\t\"flags\" : \"%s\",\n",(var->prop->flags&PF_DEPRECATED)?"DEPRECATED":"");
			printf("\t\t\"description\" : \"%s\"\n",var->prop->description?var->prop->description:"");
			printf("\t}%s\n",i<n-1?",":"");
		}
		else
		{
			printf("%s=%s;",var->prop->name,value);
			if (var->prop->description || var->prop->flags&PF_DEPRECATED)
				printf(" // %s%s", (var->prop->flags&PF_DEPRECATED)?"DEPRECATED ":"", var->prop->description?var->prop->description:"");
			printf("\n");
		}
	}
	if ( use_json )
	{
		printf("}\n");
	}
	return 0;
}

DEPRECATED static int redirect(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->redirect(argc,argv);
}
int GldCmdarg::redirect(int argc, const char *argv[])
{
	if (argc>1)
	{
		char buffer[1024]; char *p;
		strcpy(buffer,*++argv); argc--;
		if (strcmp(buffer,"none")==0)
		{} /* used by validate to block default --redirect all behavior */
		else if (strcmp(buffer,"all")==0)
		{
			if (output_redirect("output",NULL)==NULL ||
				output_redirect("error",NULL)==NULL ||
				output_redirect("warning",NULL)==NULL ||
				output_redirect("debug",NULL)==NULL ||
				output_redirect("verbose",NULL)==NULL ||
				output_redirect("profile",NULL)==NULL ||
				output_redirect("progress",NULL)==NULL)
			{
				output_fatal("redirection of all failed");
				/* TROUBLESHOOT
					An attempt to close all standard stream from the
					command line using <b>--redirect all</b> has failed.
					One of the streams cannot be closed.  Try redirecting
					each stream separately until the problem stream is
					identified and the correct the problem with that stream.
				 */
				return FAILED;
			}
		}
		else if ((p=strchr(buffer,':'))!=NULL)
		{
			*p++='\0';
			if (output_redirect(buffer,p)==NULL)
			{
				output_fatal("redirection of %s to '%s' failed: %s",buffer,p, strerror(errno));
				/*	TROUBLESHOOT
					An attempt to redirect a standard stream from the
					command line using <b>--redirect <i>stream</i>:<i>destination</i></b>
					has failed.  The message should provide an indication of why the
					attempt failed. The remedy will depend on the nature of the problem.
				 */
				return FAILED;
			}
		}
		else if (output_redirect(buffer,NULL)==NULL)
		{
				output_fatal("default redirection of %s failed: %s",buffer, strerror(errno));
				/*	TROUBLESHOOT
					An attempt to close a standard stream from the
					command line using <b>--redirect <i>stream</i></b>
					has failed.  The message should provide an indication of why the
					attempt failed. The remedy will depend on the nature of the problem.

				 */
				return FAILED;
		}
	}
	else
	{
		output_fatal("redirection is missing");
		/*	TROUBLESHOOT
			A <b>--redirect</b> directive on the command line is missing
			its redirection specification.  The correct syntax is
			<b>--redirect <i>stream</i>[:<i>destination</i>]</b>.
		 */
		return FAILED;
	}
	return 1;
}

DEPRECATED static int libinfo(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->libinfo(argc,argv);
}
int GldCmdarg::libinfo(int argc, const char *argv[])
{
	bool use_json = (strstr(argv[0],"=json")!=NULL);
	if (argc-1>0)
	{	argc--;
		module_libinfo(*++argv,use_json);
		return CMDOK;
	}
	else
	{
		output_fatal("missing library name");
		/*	TROUBLESHOOT
			The <b>-L</b> or <b>--libinfo</b> command line directive
			was not followed by a module name.  The correct syntax is
			<b>-L <i>module_name</i></b> or <b>--libinfo <i>module_name</i></b>.
		 */
	}
	return CMDERR;
}

DEPRECATED static int threadcount(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->threadcount(argc,argv);
}
int GldCmdarg::threadcount(int argc, const char *argv[])
{
	if (argc>1)
		global_threadcount = (argc--,atoi(*++argv));
	else
	{
		output_fatal("missing thread count");
		/*	TROUBLESHOOT
			The <b>-T</b> or <b>--threadcount</b> command line directive
			was not followed by a valid number.  The correct syntax is
			<b>-T <i>number</i></b> or <b>--threadcount <i>number</i></b>.
		 */
		return CMDERR;
	}
	return 1;
}

DEPRECATED static int output(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->output(argc,argv);
}
int GldCmdarg::output(int argc, const char *argv[])
{
	if (argc>1)
	{
		strcpy(global_savefile,(argc--,*++argv));
		return 1;
	}
	else
	{
		output_fatal("missing output file");
		/* TROUBLESHOOT
			The <b>-o</b> or <b>--output</b> command line directive
			was not followed by a valid filename.  The correct syntax is
			<b>-o <i>file</i></b> or <b>--output <i>file</i></b>.
		 */
		return CMDERR;
	}
}

DEPRECATED static int environment(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->environment(argc,argv);
}
int GldCmdarg::environment(int argc, const char *argv[])
{
	if (argc>1)
		strcpy(global_environment,(argc--,*++argv));
	else
	{
		output_fatal("environment not specified");
		/*	TROUBLESHOOT
			The <b>-e</b> or <b>--environment</b> command line directive
			was not followed by a valid environment specification.  The
			correct syntax is <b>-e <i>keyword</i></b> or <b>--environment <i>keyword</i></b>.
		 */
		return CMDERR;
	}
	return 1;
}

DEPRECATED static int xmlencoding(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->xmlencoding(argc,argv);
}
int GldCmdarg::xmlencoding(int argc, const char *argv[])
{
	if (argc>1)
	{
		global_xml_encoding = atoi(*++argv);
		argc--;
	}
	else
	{
		output_fatal("xml encoding not specified");
		/*	TROUBLESHOOT
			The <b>--xmlencoding</b> command line directive
			was not followed by a encoding specification.  The
			correct syntax is <b>--xmlencoding <i>keyword</i></b>.
		 */
		return FAILED;
	}
	return 1;
}

DEPRECATED static int xsd(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->xsd(argc,argv);
}
int GldCmdarg::xsd(int argc, const char *argv[])
{
	if (argc>0)
	{
		argc--;
		output_xsd(*++argv);
		return CMDOK;
	}
	else
	{
		MODULE *mod;
		for (mod=module_get_first(); mod!=NULL; mod=mod->next)
			output_xsd(mod->name);
		return 0;
	}
}

DEPRECATED static int xsl(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->xsl(argc,argv);
}
int GldCmdarg::xsl(int argc, const char *argv[])
{
	if (argc-1>0)
	{
		char fname[1024];
		int n_args = 0;
		char *p_args[256];
		char *buffer = strdup(argv[1]);
		p_args[n_args++] = buffer;
		for ( char *p = buffer ; *p != '\0' ; p++ )
		{
			if ( *p == ',' )
			{
				*p++ = '\0';
				p_args[n_args] = p;
			}
		}
		snprintf(fname,sizeof(fname)-1,"gridlabd-%d_%d.xsl",global_version_major,global_version_minor);
		output_xsl(fname,n_args,(const char**)p_args);
		free(buffer);
		return CMDOK;
	}
	else
	{
		output_fatal("module list not specified");
		/*	TROUBLESHOOT
			The <b>--xsl</b> command line directive
			was not followed by a validlist of modules.  The
			correct syntax is <b>--xsl <i>module1</i>[,<i>module2</i>[,...]]</b>.
		 */
		return CMDERR;
	}
}

DEPRECATED static int _stream(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->_stream(argc,argv);
}
int GldCmdarg::_stream(int argc, const char *argv[])
{
	global_streaming_io_enabled = !global_streaming_io_enabled;
	return 0;
}

DEPRECATED static int server(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->server(argc,argv);
}
int GldCmdarg::server(int argc, const char *argv[])
{
	strcpy(global_environment,"server");
	return 0;
}

DEPRECATED static int clearmap(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->clearmap(argc,argv);
}
int GldCmdarg::clearmap(int argc, const char *argv[])
{
	sched_clear();
	return 0;
}

DEPRECATED static int pstatus(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->pstatus(argc,argv);
}
int GldCmdarg::pstatus(int argc, const char *argv[])
{
	sched_init(1);
	const char *opt = strchr(argv[0],'=');
	if ( opt != NULL )
		sched_print(0,opt+1);
	else
		sched_print(0);
	return 0;
}

DEPRECATED static int pkill(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->pkill(argc,argv);
}
int GldCmdarg::pkill(int argc, const char *argv[])
{
	if (argc>0)
	{
		int signal = SIGINT;
		int m = 1;
		if ( *(argv[1]) == '-' )
		{
			signal = 0;
			if ( ! isdigit(argv[1][1]) )
			{
				struct {
					int signal;
					const char *shortname;
					const char *longname;
				} map[] = {
					{SIGINT,"-INT","-SIGINT"},
					{SIGQUIT,"-QUIT","-SIGQUIT"},
					{SIGTRAP,"-TRAP","-SIGTRAP"},
					{SIGABRT,"-ABRT","-SIGABRT"},
					{SIGKILL,"-KILL","-SIGKILL"},
					{SIGSTOP,"-STOP","-SIGSTOP"},
					{SIGCONT,"-CONT","-SIGCONT"},
					{SIGTERM,"-TERM","-SIGTERM"},
				};
				for ( size_t n = 0 ; n < sizeof(map)/sizeof(map[0]) ; n++ )
				{
					if ( strcmp(map[n].shortname,argv[1]) == 0 || strcmp(map[n].longname,argv[1]) == 0 )
					{
						signal = map[n].signal;
						break;
					}
				}
			}
			else
			{
				signal = atoi(argv[1]+1);
			}
			if ( signal == 0 )
			{
				output_error("'%s' is an invalid signal",argv[1]);
				return CMDERR;
			}
			argc--;
			argv++;
			m++;
		}
		if ( ! isdigit(*(argv[1])) )
		{
			output_error("'%s' is an invalid processor number",argv[1]);
		}
		sched_pkill(atoi(argv[1]),signal);
		return m;
	}
	else
	{
		output_fatal("processor number not specified");
		/*	TROUBLESHOOT
			The <b>--pkill</b> command line directive
			was not followed by a valid processor number.
			The correct syntax is <b>--pkill <i>processor_number</i></b>.
		 */
		return CMDERR;
	}
}

DEPRECATED static int plist(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->plist(argc,argv);
}
int GldCmdarg::plist(int argc, const char *argv[])
{
	sched_init(1);
	sched_print(0);
	return 0;
}

DEPRECATED static int pcontrol(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->pcontrol(argc,argv);
}
int GldCmdarg::pcontrol(int argc, const char *argv[])
{
	sched_init(1);
	sched_controller();
	return 0;
}

DEPRECATED static int info(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->info(argc,argv);
}
int GldCmdarg::info(int argc, const char *argv[])
{
	if ( argc>1 )
	{
		char cmd[4096];
#ifdef WIN32
		snprintf(cmd,sizeof(cmd)-1,"start %s \"%s%s\"", global_browser, global_infourl, argv[1]);
#elif defined(MACOSX)
		snprintf(cmd,sizeof(cmd)-1,"open -a %s \"%s%s\"", global_browser, global_infourl, argv[1]);
#else
		snprintf(cmd,sizeof(cmd)-1,"%s \"%s%s\" & ps -p $! >/dev/null", global_browser, global_infourl, argv[1]);
#endif
		IN_MYCONTEXT output_verbose("Starting browser using command [%s]", cmd);
		if (my_instance->subcommand(cmd)!=0)
		{
			output_error("unable to start browser");
			return CMDERR;
		}
		else
		{
			IN_MYCONTEXT output_verbose("starting interface");
		}
		return 1;
	}
	else
	{
		output_fatal("info subject not specified");
		/*	TROUBLESHOOT
			The <b>--info</b> command line directive
			was not followed by a valid subject to lookup.
			The correct syntax is <b>--info <i>subject</i></b>.
		 */
		return CMDERR;
	}
}

DEPRECATED static int slave(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->slave(argc,argv);
}
int GldCmdarg::slave(int argc, const char *argv[])
{
	char host[256], port[256];

	if ( argc < 2 )
	{
		output_error("slave connection parameters are missing");
		return CMDERR;
	}

	IN_MYCONTEXT output_debug("slave()");
	if(2 != sscanf(argv[1],"%255[^:]:%255s",host,port))
	{
		output_error("unable to parse slave parameters");
	}

	strncpy(global_master,host,sizeof(global_master)-1);
	if ( strcmp(global_master,"localhost")==0 ){
		sscanf(port,"%" FMT_INT64 "x",&global_master_port); /* port is actual mmap/shmem */
		global_multirun_connection = MRC_MEM;
	}
	else
	{
		global_master_port = atoi(port);
		global_multirun_connection = MRC_SOCKET;
	}

	if ( FAILED == instance_slave_init() )
	{
		output_error("slave instance init failed for master '%s' connection '%" FMT_INT64 "x'", global_master, global_master_port);
		return CMDERR;
	}

	IN_MYCONTEXT output_verbose("slave instance for master '%s' using connection '%" FMT_INT64 "x' started ok", global_master, global_master_port);
	return 1;
}

DEPRECATED static int slavenode(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->slavenode(argc,argv);
}
int GldCmdarg::slavenode(int argc, const char *argv[])
{
	exec_slave_node();
	return CMDOK;
}

DEPRECATED static int slave_id(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->slave_id(argc,argv);
}
int GldCmdarg::slave_id(int argc, const char *argv[])
{
	if(argc < 2){
		output_error("--id requires an ID number argument");
		return CMDERR;
	}
	if(1 != sscanf(argv[1], "%" FMT_INT64 "d", &global_slave_id)){
		output_error("slave_id(): unable to read ID number");
		return CMDERR;
	}
	IN_MYCONTEXT output_debug("slave using ID %" FMT_INT64 "d", global_slave_id);
	return 1;
}

DEPRECATED static int example(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->example(argc,argv);
}
int GldCmdarg::example(int argc, const char *argv[])
{
	MODULE *module;
	CLASS *oclass;
	PROPERTY *prop;
	char modname[1024], classname[1024];

	if ( argc < 2 )
	{
		output_error("--example requires a module:class argument");
		return CMDERR;
	}

	int n = sscanf(argv[1],"%1023[A-Za-z_]:%1024[A-Za-z_0-9]",modname,classname);
	if ( n!=2 )
	{
		output_error("--example: %s name is not valid",n==0?"module":"class");
		return CMDERR;
	}
	module = module_load(modname,0,NULL);
	if ( module==NULL )
	{
		output_error("--example: module %d is not found", modname);
		return CMDERR;
	}
	oclass = class_get_class_from_classname(classname);
	if ( oclass==NULL )
	{
		output_error("--example: class %d is not found", classname);
		return CMDERR;
	}
	output_raw("class %s {\n",oclass->name);
	bool first = true;
	for ( prop = class_get_first_property_inherit(oclass) ; prop != NULL ; prop = class_get_next_property_inherit(prop) )
	{
		if ( (prop->flags&PF_REQUIRED) && ! (prop->flags&PF_DEPRECATED) )
		{
			if ( first ) output_raw("\t// required input properties\n");
			first = false;
			output_raw("\t%s \"%s\";\n", prop->name, prop->default_value ? prop->default_value : "");
		}
	}
	first = true;
	for ( prop = class_get_first_property_inherit(oclass) ; prop != NULL ; prop = class_get_next_property_inherit(prop) )
	{
		if ( ! (prop->flags&PF_REQUIRED) && ! (prop->flags&PF_OUTPUT) && ! (prop->flags&PF_DYNAMIC) && ! (prop->flags&PF_DEPRECATED) )
		{
			if ( first ) output_raw("\t// optional input properties\n");
			first = false;
			output_raw("\t%s \"%s\";\n", prop->name, prop->default_value ? prop->default_value : "");
		}
	}
	first = true;
	for ( prop = class_get_first_property_inherit(oclass) ; prop != NULL ; prop = class_get_next_property_inherit(prop) )
	{
		if ( ! (prop->flags&PF_DYNAMIC) && (prop->flags&PF_DEPRECATED) )
		{
			if ( first ) output_raw("\t// dynamic properties\n");
			first = false;
			output_raw("\t%s \"%s\";\n", prop->name, prop->default_value ? prop->default_value : "");
		}
	}
	first = true;
	for ( prop = class_get_first_property_inherit(oclass) ; prop != NULL ; prop = class_get_next_property_inherit(prop) )
	{
		if ( (prop->flags&PF_OUTPUT) && ! (prop->flags&PF_DEPRECATED) )
		{
			if ( first ) output_raw("\t// output properties\n");
			first = false;
			output_raw("\t%s \"%s\";\n", prop->name, prop->default_value ? prop->default_value : "");
		}
	}
	first = true;
	for ( prop = class_get_first_property_inherit(oclass) ; prop != NULL ; prop = class_get_next_property_inherit(prop) )
	{
		if ( prop->flags&PF_DEPRECATED )
		{
			if ( first ) output_raw("\t// deprecated properties\n");
			first = false;
			output_raw("\t%s \"%s\";\n", prop->name, prop->default_value ? prop->default_value : "");
		}
	}
	output_raw("}\n");
	return CMDOK;
}
DEPRECATED static int mclassdef(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->mclassdef(argc,argv);
}
int GldCmdarg::mclassdef(int argc, const char *argv[])
{
	MODULE *module;
	CLASS *oclass;
	OBJECT *obj;
	char modname[1024], classname[1024];
	int n;
	char buffer[65536];
	PROPERTY *prop;
	int count = 0;

	/* generate the object */
	if ( argc < 2 )
	{
		output_error("--mclassdef requires a module:class argument");
		return CMDERR;
	}
	n = sscanf(argv[1],"%1023[A-Za-z_]:%1024[A-Za-z_0-9]",modname,classname);
        if ( n!=2 )
        {
                output_error("--mclassdef: %s name is not valid",n==0?"module":"class");
                return CMDERR;
        }
        module = module_load(modname,0,NULL);
        if ( module==NULL )
        {
                output_error("--mclassdef: module %d is not found", modname);
                return CMDERR;
        }
        oclass = class_get_class_from_classname(classname);
        if ( oclass==NULL )
        {
                output_error("--mclassdef: class %d is not found", classname);
                return CMDERR;
        }
        obj = object_create_single(oclass);
        if ( obj==NULL )
        {
                output_error("--mclassdef: unable to create mclassdef object from class %s", classname);
                return CMDERR;
        }
        global_clock = time(NULL);
        output_redirect("error",NULL);
        output_redirect("warning",NULL);
        if ( !object_init(obj) )
        {
                IN_MYCONTEXT output_warning("--mclassdef: unable to initialize mclassdef object from class %s", classname);
        }

	/* output the classdef */
	count = snprintf(buffer,sizeof(buffer)-1,"struct('module','%s','class','%s'", modname, classname);
	for ( prop = oclass->pmap ; prop!=NULL && prop->oclass==oclass ; prop=prop->next )
	{
		char temp[1024];
		const char *value = object_property_to_string(obj, prop->name, temp, 1023);
		if ( strchr(prop->name,'.')!=NULL )
		{
			continue; /* do not output structures */
		}
		if ( value!=NULL )
		{
			count += snprintf(buffer+count,sizeof(buffer)-1-count,",...\n\t'%s','%s'", prop->name, value);
		}
	}
	count += snprintf(buffer+count,sizeof(buffer)-1-count,");\n");
	output_raw("%s",buffer);
        return CMDOK;
}

DEPRECATED static int locktest(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->locktest(argc,argv);
}
int GldCmdarg::locktest(int argc, const char *argv[])
{
	test_lock();
	return CMDOK;
}

DEPRECATED static int workdir(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->workdir(argc,argv);
}
int GldCmdarg::workdir(int argc, const char *argv[])
{
	if ( argc<2 )
	{
		output_error("--workdir requires a directory argument");
		return CMDERR;
	}
	strcpy(global_workdir,argv[1]);
	if ( chdir(global_workdir)!=0 )
	{
		output_error("%s is not a valid workdir", global_workdir);
		return CMDERR;
	}
	if ( getcwd(global_workdir,sizeof(global_workdir)) )
	{
		IN_MYCONTEXT output_verbose("working directory is '%s'", global_workdir);
		return 1;
	}
	else
	{
		output_error("unable to read current working directory");
		return CMDERR;
	}
}

DEPRECATED static int local_daemon(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->local_daemon(argc,argv);
}
int GldCmdarg::local_daemon(int argc, const char *argv[])
{
	if ( argc < 2 )
	{
		output_error("--daemon requires a command");
		return CMDERR;
	}
	else if ( strcmp(argv[1],"start") == 0 )
	{
		return daemon_start(argc-1,argv+1);
	}
	else if ( strcmp(argv[1],"stop") == 0 )
	{
		return daemon_stop(argc-1,argv+1);
	}
	else if ( strcmp(argv[1],"restart") == 0 )
	{
		return daemon_restart(argc-1,argv+1);
	}
	else if ( strcmp(argv[1],"status") == 0 )
	{
		return daemon_status(argc-1,argv+1);
	}
	else
	{
		output_error("%s is not a valid daemon command", argv[1]);
		return CMDERR;
	}
}

DEPRECATED static int remote_client(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->remote_client(argc,argv);
}
int GldCmdarg::remote_client(int argc, const char *argv[])
{
	if ( argc < 2 )
	{
		output_error("--remote requires hostname");
		return CMDERR;
	}
	else
		return daemon_remote_client(argc,argv);
}

DEPRECATED static int printenv(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->printenv(argc,argv);
}
int GldCmdarg::printenv(int argc, const char *argv[])
{
	return my_instance->subcommand("printenv") == 0 ? 0 : CMDERR;
}

DEPRECATED static int formats(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->formats(argc,argv);
}
int GldCmdarg::formats(int argc, const char *argv[])
{
	cout << "{" << endl;

	cout << "\t\"glm\" : {" << endl;
	cout << "\t\t\"json\" : {" << endl;
	cout << "\t\t\t\"run\" : \"" << global_execname << " {inputfile} -o {outputfile}\"" << endl;
	cout << "\t\t}" << endl;
	cout << "\t}," << endl;

	// TODO: use a directory listing to get all available converters
	cout << "\t\"json\" : {" << endl;
	cout << "\t\t\"glm\" : {" << endl;
	cout << "\t\t\t\"run\" : \"" << global_datadir << "/json2glm.py {inputfile} -o {outputfile}\"" << endl;
	cout << "\t\t}," << endl;

	cout << "\t\t\"png\" : {" << endl;
	cout << "\t\t\t\"run\" : \"" << global_datadir << "/json2png.py {inputfile} -o {outputfile}\"" << endl;
	cout << "\t\t\t\"type\" : [" << endl;
	cout << "\t\t\t\t\"summary\"," << endl;
	cout << "\t\t\t\t\"profile\"" << endl;
	cout << "\t\t\t]" << endl;
	cout << "\t\t}" << endl;
	cout << "\t}" << endl;

	cout << "}" << endl;
	return 0;
}

DEPRECATED static int origin(void *main, int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->origin(argc,argv);
}
int GldCmdarg::origin(int argc, const char *argv[])
{
	FILE *fp;
	char originfile[1024];
	if ( find_file("origin.txt",NULL,R_OK,originfile,sizeof(originfile)-1) == NULL )
	{
		IN_MYCONTEXT output_error("origin file not found");
		return CMDERR;
	}
	fp = fopen(originfile,"rt");
	if ( fp == NULL )
	{
		IN_MYCONTEXT output_error("unable to open origin file");
		return CMDERR;
	}
	while ( ! feof(fp) )
	{
		char line[1024];
		size_t len = fread(line,sizeof(line[0]),sizeof(line)-1,fp);
		if ( ferror(fp) )
		{
			IN_MYCONTEXT output_error("error reading origin file");
			return CMDERR;
		}
		if ( len >= 0 )
		{
			int old = global_suppress_repeat_messages;
			global_suppress_repeat_messages = 0;
			line[len] = '\0';
			output_raw("%s",line);
			global_suppress_repeat_messages = old;
		}
	}
	fclose(fp);
	return 1;
}

DEPRECATED static int cite(void *main, int argc, const char *argv[])
{
	FILE *fp;
	char originfile[1024];
	if ( find_file("origin.txt",NULL,R_OK,originfile,sizeof(originfile)-1) == NULL )
	{
		IN_MYCONTEXT output_error("origin file not found");
		return CMDERR;
	}
	fp = fopen(originfile,"r");
	if ( fp == NULL )
	{
		IN_MYCONTEXT output_error("unable to open origin file");
		return CMDERR;
	}
	char url[1024] = "";
	if ( ! feof(fp) )
	{
		char line[1024];
		size_t len = fread(line,sizeof(line[0]),sizeof(line)-1,fp);
		if ( ferror(fp) )
		{
			IN_MYCONTEXT output_error("error reading origin file");
			return CMDERR;
		}
		if ( len >= 0 && url[0] == '\0' && strncmp(line,"# http",6)==0 )
		{
			char *end = strstr(line,"/commits/");
			if ( end == NULL )
				end = strchr(line,'\n');
			if ( end )
				*end = '\0';
			strcpy(url,line+2);
		}
		else
		{
			strcpy(url,global_urlbase);
		}
	}
	fclose(fp);

#if defined MACOSX
		const char *platform = "Darwin";
#else // LINUX
	const char *platform = "Linux";
#endif
	int year = 2000+BUILDNUM/10000;
	int month = (BUILDNUM%10000)/100;
	int day = (BUILDNUM%100);
	const char *Month[] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
	bool use_json = (strstr(argv[0],"=json")!=NULL);
	bool use_bibtex = (strstr(argv[0],"=bibtex")!=NULL);
	int old = global_suppress_repeat_messages;
	global_suppress_repeat_messages = 0;
	if ( use_json )
	{
		output_message("{");
		output_message("  \"%s\" : \"%s\",","author","Chassin, D.P. et al.");
		output_message("  \"%s\" : \"%s\",","name",PACKAGE_NAME);
		output_message("  \"%s\" : \"%s\",","version",PACKAGE_VERSION);
		output_message("  \"%s\" : \"%s\",","branch",BRANCH);
		output_message("  \"%s\" : %d,","year",year);
		output_message("  \"%s\" : \"%d-%d-%d\",","date",year,month,day);
		output_message("  \"%s\" : \"%s\",","url",url);
		output_message("  \"%s\" : %d,","buildnum",BUILDNUM);
		output_message("  \"%s\" : \"%s\"","platform",platform);
		output_message("}");
	}
	else if ( use_bibtex )
	{
		output_message("@misc{gridlabd,");
		output_message("\tauthor = {{Chassin, D.P., et al.}},");
		output_message("\ttitle = {{%s (%s-%d-%s for %s)}},", PACKAGE_NAME, PACKAGE_VERSION, BUILDNUM, BRANCH, platform);
		output_message("\tyear = {%d},", year);
		output_message("\turl = {%s}",url);
		output_message("}");
	}
	else
	{
		output_message("Chassin, D.P., et al., \"%s %s-%d (%s)"
			" %s\" (%d) [online]."
			" Available at %s. Accessed %s. %d, %d",
			PACKAGE_NAME, PACKAGE_VERSION, BUILDNUM, BRANCH,
			platform, year, url, Month[month-1], day, year);
	}
	global_suppress_repeat_messages = old;
	return 0;
}

DEPRECATED static int depends(void *main, int argc, const char *argv[])
{
	const char *format = strchr(argv[0],'=');
	fprintf(stdout,"%s",my_instance->get_loader()->get_depends(format?format+1:NULL).c_str());
	return 0;
}

DEPRECATED static int rusage(void *main, int argc, const char *argv[])
{
	global_rusage_rate = 1;
	return 0;
}

DEPRECATED static int nprocs(void *main, int argc, const char *argv[])
{
    fprintf(stdout,"%d\n",processor_count());
    return 0;
}

DEPRECATED static int sublime_syntax(void *main, int argc, const char *argv[])
{
	((GldMain*)main)->get_loader()->sublime_syntax();
	return 0;
}

DEPRECATED static int csvloadshape(void *main, int argc, const char *argv[])
{
	FILE *fh = stdout;
	if ( strchr(argv[0],'=') )
	{
		const char *fname = strchr(argv[0],'=') + 1;
		fh = fopen(fname,"w");
		if ( fh == NULL )
		{
			output_error("unable to open '%s' for write",fname);
		}
	}
	char schedules[65536] = "";
	if ( argc == 1 || strcmp(argv[1],"*") == 0 )
	{
		for ( SCHEDULE *sch = schedule_getfirst() ; sch != NULL ; sch = schedule_getnext(sch) )
		{
			if ( schedules[0] != '\0' )
			{
				strcat(schedules,",");
			}
			strcat(schedules,sch->name);
		}
	}
	else
	{
		strncpy(schedules,argv[1],sizeof(schedules));
	}

	SCHEDULE *schedule_list[1024];
	unsigned int n_schedules = 0;
	char *next = NULL, *last = NULL;
	for ( next = strtok_r(schedules,",",&last) ; next != NULL && n_schedules < sizeof(schedule_list)/sizeof(schedule_list[0]) ; next = strtok_r(NULL,",",&last) )
	{
		SCHEDULE *schedule = schedule_find_byname(next);
		if ( schedule == NULL )
		{
			output_error("schedule '%s' not found",next);
			return CMDERR;
		}
		schedule_list[n_schedules++] = schedule;
	}

	fprintf(fh,"%s","timestamp");
	for ( unsigned int n = 0 ; n < n_schedules ; n++ )
	{
		fprintf(fh,",%s",schedule_list[n]->name);
	}
	fprintf(fh,"%s","\n");

	for ( TIMESTAMP ts = global_starttime ; ts < global_stoptime ; )
	{
		char tmp[64];
		if ( convert_from_timestamp(ts,tmp,sizeof(tmp)-1) )
		{
			fprintf(fh,"%s",tmp);
		}
		TIMESTAMP tn = global_stoptime;
		for ( unsigned int n = 0 ; n < n_schedules ; n++ )
		{
			SCHEDULEINDEX ndx = schedule_index(schedule_list[n],ts);
			TIMESTAMP t = ts + schedule_dtnext(schedule_list[n],ndx)*60;
			if ( t < tn )
			{
				tn = t;
			}
		}
		for ( unsigned int n = 0 ; n < n_schedules ; n++ )
		{
			SCHEDULEINDEX ndx = schedule_index(schedule_list[n],tn);
			fprintf(fh,",%g",schedule_value(schedule_list[n],ndx));
		}
		fprintf(fh,"%s","\n");
		ts = tn;
	}
	return argc;
}

/*********************************************/
/* ADD NEW CMDARG PROCESSORS ABOVE THIS HERE */
/* Then make the appropriate entry in the    */
/* CMDARG structure below                    */
/*********************************************/

#include "job.h"
#include "validate.h"

DEPRECATED static CMDARG main_commands[] = {

	/* NULL,NULL,NULL,NULL, "Section heading */
	{NULL,NULL,NULL,NULL, "Command-line options"},

	/* long_str, short_str, processor_call, arglist_desc, help_desc */
	{"check",		"c",	check,			NULL, "Performs module checks before starting simulation" },
	{"debug",		NULL,	debug,			NULL, "Toggles display of debug messages" },
	{"debugger",	NULL,	debugger,		NULL, "Enables the debugger" },
	{"dumpall",		NULL,	dumpall,		NULL, "Dumps the global variable list" },
	{"mt_profile",	NULL,	mt_profile,		"<n-threads>", "Analyses multithreaded performance profile" },
	{"profile",		NULL,	profile,		NULL, "Toggles performance profiling of core and modules while simulation runs" },
	{"quiet",		"q",	quiet,			NULL, "Toggles suppression of all but error and fatal messages" },
	{"verbose",		"v",	verbose,		NULL, "Toggles output of verbose messages" },
	{"warn",		"w",	warn,			NULL, "Toggles display of warning messages" },
	{"workdir",		"W",	workdir,		NULL, "Sets the working directory" },
	{"rusage",      NULL,   rusage,         NULL, "Collect resource usage statistics" },

	{NULL,NULL,NULL,NULL, "Global, environment and module information"},
	{"define",		"D",	define,			"<name>=[<module>:]<value>", "Defines or sets a global (or module) variable" },
	{"globals",		NULL,	globals,		NULL, "Displays a sorted list of all global variables" },
	{"libinfo",		"L",	libinfo,		"<module>", "Displays information about a module" },
	{"printenv",	"E",	printenv,		NULL, "Displays the default environment variables" },

	{NULL,NULL,NULL,NULL, "Information"},
	{"copyright",	NULL,	copyright,		NULL, "Displays copyright" },
	{"license",		NULL,	license,		NULL, "Displays the license agreement" },
	{"version",		"V",	version,		"[all,number,build,package,branch,platform]", "Displays the version information" },
	{"build-info",	NULL,	build_info,		NULL, "Displays the build information" },
	{"setup",		NULL,	setup,			NULL, "Open simulation setup screen" },
	{"origin",		NULL,	origin,			NULL, "Display origin information" },
	{"cite",		NULL,	cite,			NULL, "Print the complete citation for this version"},
	{"depends",		NULL,	depends,		NULL, "Generate dependency tree"},

	{NULL,NULL,NULL,NULL, "Test processes"},
	{"dsttest",		NULL,	dsttest,		NULL, "Perform daylight savings rule test" },
	{"endusetest",	NULL,	endusetest,		NULL, "Perform enduse pseudo-object test" },
	{"globaldump",	NULL,	globaldump,		NULL, "Perform a dump of the global variables" },
	{"loadshapetest", NULL,	loadshapetest,	NULL, "Perform loadshape pseudo-object test" },
	{"locktest",	NULL,	locktest,		NULL, "Perform memory locking test" },
	{"modtest",		NULL,	modtest,		"<module>", "Perform test function provided by module" },
	{"randtest",	NULL,	randtest,		NULL, "Perform random number generator test" },
	{"scheduletest", NULL,	scheduletest,	NULL, "Perform schedule pseudo-object test" },
	{"test",		NULL,	test,			"<module>", "Perform unit test of module (deprecated)" },
	{"testall",		NULL,	testall,		"=<filename>", "Perform tests of modules listed in file" },
	{"unitstest",	NULL,	unitstest,		NULL, "Perform unit conversion system test" },
	{"validate",	NULL,	validate,		"...", "Perform model validation check" },

	{NULL,NULL,NULL,NULL, "File and I/O Formatting"},
	{"kml",			NULL,	kml,			"[=<filename>]", "Output to KML (Google Earth) file of model (only supported by some modules)" },
	{"stream",		NULL,	_stream,		NULL, "Toggles streaming I/O" },
	{"sanitize",	NULL,	sanitize,		"<options> <indexfile> <outputfile>", "Output a sanitized version of the GLM model"},
	{"xmlencoding",	NULL,	xmlencoding,	"8|16|32", "Set the XML encoding system" },
	{"xmlstrict",	NULL,	xmlstrict,		NULL, "Toggle strict XML formatting (default is enabled)" },
	{"xsd",			NULL,	xsd,			"[module[:class]]", "Prints the XSD of a module or class" },
	{"xsl",			NULL,	xsl,			"module[,module[,...]]]", "Create the XSL file for the module(s) listed" },
	{"formats",     NULL,   formats,        NULL, "get a list supported file formats"},
	{"sublime_syntax", NULL, sublime_syntax, NULL, "generate sublime syntax file"},

	{NULL,NULL,NULL,NULL, "Help"},
	{"help",		"h",	help,			NULL, "Displays command line help" },
	{"info",		NULL,	info,			"<subject>", "Obtain online help regarding <subject>"},
	{"modhelp",		NULL,	modhelp,		"module[:class]", "Display structure of a class or all classes in a module" },
	{"modlist",		NULL,	modlist,		NULL, "Display list of available modules"},
	{"example",		NULL,	example,		"module:class", "Display an example of an instance of the class after init" },
	{"mclassdef",	NULL,	mclassdef,		"module:class", "Generate Matlab classdef of an instance of the class after init" },
	{"loadshape",   NULL,   csvloadshape,   "name:year", "Generate the named schedule as a timeseries for given year"},

	{NULL,NULL,NULL,NULL, "Process control"},
	{"pidfile",		NULL,	pidfile,		"[=<filename>]", "Set the process ID file (default is gridlabd.pid)" },
	{"threadcount", "T",	threadcount,	"<n>", "Set the maximum number of threads allowed" },
	{"job",			NULL,	job,			"...", "Start a job"},
    {"nprocs",      NULL,   nprocs,         NULL, "Display the number of processors available to run jobs"},

	{NULL,NULL,NULL,NULL, "System options"},
	{"avlbalance",	NULL,	avlbalance,		NULL, "Toggles automatic balancing of object index" },
	{"bothstdout",	NULL,	bothstdout,		NULL, "Merges all output on stdout" },
	{"check_version", NULL,	_check_version,	NULL, "Perform online version check to see if any updates are available" },
	{"compile",		"C",	compile,		NULL, "Toggles compile-only flags" },
	{"initialize",	"I",	initialize,		NULL, "Toggles initialize-only flags" },
	{"library",     "l",    library,        "<filename>", "Loads a library GLM file"},
	{"environment",	"e",	environment,	"<appname>", "Set the application to use for run environment" },
	{"output",		"o",	output,			"<file>", "Enables save of output to a file (default is gridlabd.glm)" },
	{"pause",		NULL,	pauseatexit,	NULL, "Toggles pause-at-exit feature" },
	{"relax",		NULL,	relax,			NULL, "Allows implicit variable definition when assignments are made" },
	{"template",	"t",	_template,		NULL, "Load template" },

	{NULL,NULL,NULL,NULL, "Server mode"},
	{"server",		NULL,	server,			NULL, "Enables the server"},
	{"daemon",		"d",	local_daemon,	"<command>", "Controls the daemon process"},
	{"remote",		"r",	remote_client,	"<command>", "Connects to a remote daemon process"},
	{"clearmap",	NULL,	clearmap,		NULL, "Clears the process map of defunct jobs (deprecated form)" },
	{"pclear",		NULL,	clearmap,		NULL, "Clears the process map of defunct jobs" },
	{"pcontrol",	NULL,	pcontrol,		NULL, "Enters process controller" },
	{"pkill",		NULL,	pkill,			"<procnum>", "Kills a run on a processor" },
	{"plist",		NULL,	plist,			NULL, "List runs on processes" },
	{"pstatus",		NULL,	pstatus,		NULL, "Prints the process list" },
	{"redirect",	NULL,	redirect,		"<stream>[:<file>]", "Redirects an output to stream to a file (or null)" },
	{"server_portnum", "P", server_portnum, NULL, "Sets the server port number (default is 6267)" },
	{"server_inaddr", NULL, server_inaddr,	NULL, "Sets the server interface address (default is INADDR_ANY, any interface)"},
	{"slave",		NULL,	slave,			"<master>", "Enables slave mode under master"},
	{"slavenode",	NULL,	slavenode,		NULL, "Sets a listener for a remote GridLAB-D call to run in slave mode"},
	{"id",			NULL,	slave_id,		"<idnum>", "Sets the ID number for the slave to inform its using to the master"},
};

int GldCmdarg::runoption(const char *value)
{
	int i;
	char buffer[1024];
	strcpy(buffer,value);
	int m = strcspn(buffer," \t");
	char *args[2] = {buffer,buffer[m]=='\0'?NULL:buffer+m+1};
	buffer[m] = '\0';
	char opt[64];
	strcpy(opt,args[0]);
	char *cut = strchr(opt,'=');
	if ( cut )
	{
		*cut = '\0';
	}
	for ( i=0 ; i<(int)(sizeof(main_commands)/sizeof(main_commands[0])) ; i++ )
	{
		if ( main_commands[i].lopt != NULL && strcmp(opt,main_commands[i].lopt)==0 )
		{
			return main_commands[i].call(instance,args[1]==NULL?1:2,(const char**)args);
		}
	}
	return 0;
}

DEPRECATED static int help(void *main,int argc, const char *argv[])
{
	return ((GldMain*)main)->get_cmdarg()->help(argc,argv);
}

int GldCmdarg::help(int argc, const char *argv[])
{
	int i;
	int old = global_suppress_repeat_messages;
	size_t indent = 0;
	global_suppress_repeat_messages = 0;
	output_message("Syntax: gridlabd [<options>] file1 [file2 [...]]");

	for ( i=0 ; i<(int)(sizeof(main_commands)/sizeof(main_commands[0])) ; i++ )
	{
		CMDARG arg = main_commands[i];
		size_t len = (arg.sopt?strlen(arg.sopt):0) + (arg.lopt?strlen(arg.lopt):0) + (arg.args?strlen(arg.args):0);
		if (len>indent) indent = len;
	}

	for ( i=0 ; i<(int)(sizeof(main_commands)/sizeof(main_commands[0])) ; i++ )
	{
		CMDARG arg = main_commands[i];

		/* if this entry is a heading */
		if ( arg.lopt==NULL && arg.sopt==NULL && arg.call==NULL && arg.args==NULL)
		{
			size_t l = strlen(arg.desc);
			char buffer[1024];
			memset(buffer,0,1024);
			while ( l-->0 )
				buffer[l] = '-';
			output_message("");
			output_message("%s", arg.desc);
			output_message("%s", buffer);
		}
		else
		{
			char buffer[1024] = "  ";
			if ( arg.lopt )
			{
				strcat(buffer,"--");
				strcat(buffer,arg.lopt);
			}
			if ( arg.sopt )
			{
				if ( arg.lopt )
					strcat(buffer,"|");
				strcat(buffer,"-");
				strcat(buffer,arg.sopt);
			}
			if ( arg.args==NULL || ( arg.args[0]!='=' && strncmp(arg.args,"[=",2)!=0 ) )
				strcat(buffer," ");
			if ( arg.args )
			{
				strcat(buffer,arg.args);
				strcat(buffer," ");
			}
			while ( strlen(buffer) < indent+8 )
				strcat(buffer," ");
			strcat(buffer,arg.desc);
			output_message("%s", buffer);
		}
	}
	global_suppress_repeat_messages = old;
	return 0;
}

/** Load and process the command-line arguments
	@return a STATUS value

	Arguments are processed immediately as they are seen.  This means that
	models are loaded when they are encountered, and in relation to the
	other flags.  Thus
	@code
	gridlabd --warn model1 --warn model2
	@endcode
	will load \p model1 with warnings on, and \p model2 with warnings off.
 **/
STATUS GldCmdarg::load(int argc,const char *argv[])
{
	STATUS status = SUCCESS;

	/* special case for no args */
	if (argc==1)
		return no_cmdargs();

	/* process command arguments */
	while (argv++,--argc>0)
	{
		int found = 0;
		int i;
		for ( i=0 ; i<(int)(sizeof(main_commands)/sizeof(main_commands[0])) ; i++ )
		{
			CMDARG arg = main_commands[i];
			char tmp[1024];
			snprintf(tmp,sizeof(tmp)-1,"%s=",arg.lopt);
			if ( ( arg.sopt && strncmp(*argv,"-",1)==0 && strcmp((*argv)+1,arg.sopt)==0 )
			  || ( arg.lopt && strncmp(*argv,"--",2)==0 && strcmp((*argv)+2,arg.lopt)==0 )
			  || ( arg.lopt && strncmp(*argv,"--",2)==0 && strncmp((*argv)+2,tmp,strlen(tmp))==0 ) )
			{
				int n = arg.call(instance,argc,argv);
				switch (n) {
				case CMDOK:
					return status;
				case CMDERR:
					return FAILED;
				default:
					found = 1;
					argc -= n;
					argv += n;
				}
				break;
			}
		}

		/* cmdarg not processed */
		if ( !found )
		{
			/* file name */
			if (**argv!='-')
			{
				if (global_test_mode)
				{
					IN_MYCONTEXT output_warning("file '%s' ignored in test mode", *argv);
					/* TROUBLESHOOT
					   This warning is caused by an attempt to read an input file in self-test mode.
					   The use of self-test model precludes reading model files.  Try running the system
					   in normal more or leaving off the model file name.
					 */
				}
				else
				{
					clock_t start = clock();

					/* preserve name of first model only */
					if (strcmp(global_modelname,"")==0)
					{
						strcpy(global_modelname,*argv);
					}

					if ( ! instance->load_file(*argv) )
					{
						status = FAILED;
					}
					loader_time += clock() - start;
				}
			}

			/* send cmdarg to modules */
			else
			{
				int n = module_cmdargs(argc,argv);
				if (n==0)
				{
					output_error("command line option '%s' is not recognized",*argv);
					/* TROUBLESHOOT
						The command line option given is not valid where it was found.
						Check the command line for correct syntax and order of options.
					 */
					status = FAILED;
				}
			}
		}
	}
	return status;
}

/**@}**/
