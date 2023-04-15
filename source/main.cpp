/** main.cpp
	Copyright (C) 2008 Battelle Memorial Institute

	@file main.c
	@author David P. Chassin
 @{
 **/
#define _MAIN_C

#include "gldcore.h"

#include <poll.h>

SET_MYCONTEXT(DMC_MAIN)

/** Implements a pause on exit capability for Windows consoles
 **/
void GldMain::pause_at_exit(void)
{
	if (global_pauseatexit)
	{
		output_verbose("pausing at exit");
		fprintf(stdout,"Press [RETURN] to exit... ");
		fflush(stdout);
		char buffer[80];
		fgets(buffer,sizeof(buffer)-1,stdin);
	}
}

/** The main entry point of GridLAB-D
    @returns Exit codes XC_SUCCESS, etc. (see gridlabd.h)
 **/
GldMain *my_instance = NULL; // TODO: move this to main to make main reentrant
#ifdef HAVE_PYTHON
extern "C" int main_python
#else
int main
#endif
(	int argc, /**< the number entries on command-line argument list \p argv */
	const char *argv[]) /**< a list of pointers to the command-line arguments */
{
	int return_code = XC_SUCCESS;
	try {
		my_instance = new GldMain(argc,argv);
		if ( my_instance == NULL )
		{
			output_error("unable to create new instance");
			return_code = XC_SHFAILED;
		}
		else
		{
			return_code = my_instance->mainloop(argc,argv);
		}
	}
	catch (const char *msg)
	{
		output_error("%s", msg);
		return_code = XC_EXCEPTION;
	}
	catch (GldException *exc)
	{
		output_error("%s", exc->get_message());
		return_code = XC_EXCEPTION;
	}
	catch (...)
	{
		output_error("unknown exception");
		return_code = XC_EXCEPTION;
	}
	return_code = my_instance->run_on_exit(return_code);
	if ( my_instance != NULL )
	{
		delete my_instance;
		my_instance = NULL;
	}
	if ( global_server_keepalive )
	{
		int stat;
		wait(&stat);
	}
	return return_code;
}
unsigned int GldMain::next_id = 0;
GldMain::GldMain(int argc, const char *argv[])
: 	globals(this),
	exec(this),
	cmdarg(this),
	gui(this),
	loader(this)
{
	starttime = time(NULL);
	id = next_id++;
	// TODO: remove this when reetrant code is done
	my_instance = this;

	python_embed_init(argc,argv);

	set_global_browser();

	/* set the default timezone */
	timestamp_set_tz(NULL);

	exec.clock(); /* initialize the wall clock */
	realtime_starttime(); /* mark start */

	/* set the process info */
	global_process_id = getpid();
	atexit(pause_at_exit);

#ifdef WIN32
	kill_starthandler();
	atexit(kill_stophandler);
#endif

	/* capture the execdir */
	set_global_execname(argv[0]);
	set_global_execdir(argv[0]);

	/* determine current working directory */
	set_global_workdir();

	/* capture the command line */
	set_global_command_line(argc,argv);

	/* main initialization */
	if (!output_init(argc,argv) || !exec.init())
	{
		exec.mls_done();
		return;
	}

	/* set thread count equal to processor count if not passed on command-line */
	if (global_threadcount == 0)
		global_threadcount = processor_count();
	IN_MYCONTEXT output_verbose("detected %d processor(s)", processor_count());
	IN_MYCONTEXT output_verbose("using %d helper thread(s)", global_threadcount);

	/* process command line arguments */
	if ( cmdarg.load(argc,argv) == FAILED )
	{
		output_fatal("shutdown after command line rejected");
		/*	TROUBLESHOOT
			The command line is not valid and the system did not
			complete its startup procedure.  Correct the problem
			with the command line and try again.
		 */
		exec.mls_done();
		return;
	}
	if ( loader.load_resolve_all() == FAILED )
	{
		output_fatal("shutdown after command loader name resolution failed");
		/*	TROUBLESHOOT
			The loaded files are not valid and the system did not
			complete its startup procedure.  Correct the problem
			with the command line and try again.
		 */
		exec.mls_done();
		return;
	}

	/* stitch clock */
	global_clock = global_starttime;

	/* initialize scheduler */
	sched_init(0);

	/* recheck threadcount in case user set it 0 */
	if (global_threadcount == 0)
	{
		global_threadcount = processor_count();
		IN_MYCONTEXT output_verbose("using %d helper thread(s)", global_threadcount);
	}

	/* see if newer version is available */
	if ( global_check_version )
		check_version(1);

	/* setup the random number generator */
	random_init();

	/* pidfile */
	create_pidfile();

	/* do legal stuff */
#ifdef LEGAL_NOTICE
	if (strcmp(global_pidfile,"")==0 && legal_notice()==FAILED)
	{
		exec.mls_done();
		return;
	}
#endif

	return;
}

GldMain::~GldMain(void)
{
#ifndef HAVE_PYTHON
	python_embed_term();
#endif

	// TODO: add general destruction calls
	object_destroy_all();

	// TODO: remove this when reetrant code is done
	my_instance = NULL;

	return;
}

int GldMain::mainloop(int argc, const char *argv[])
{
	/* start the processing environment */
	IN_MYCONTEXT output_verbose("load time: %d sec", realtime_runtime());
	IN_MYCONTEXT output_verbose("starting up %s environment", global_environment);
	if (environment_start(argc,argv)==FAILED)
	{
		output_fatal("environment startup failed: %s", strerror(errno));
		/*	TROUBLESHOOT
			The requested environment could not be started.  This usually
			follows a more specific message regarding the startup problem.
			Follow the recommendation for the indicated problem.
		 */
		if ( exec.getexitcode()==XC_SUCCESS )
			exec.setexitcode(XC_ENVERR);
	}
	return exec.getexitcode();
}

void GldMain::set_global_browser(const char *path)
{
	const char *browser = path ? path : getenv("BROWSER");

	/* specify the default browser */
	if ( browser != NULL )
		strncpy(global_browser,browser,sizeof(global_browser)-1);

}

void GldMain::set_global_execname(const char *path)
{
	strcpy(global_execname,path);
}

void GldMain::set_global_execdir(const char *path)
{
	char *pd1, *pd2;
	strcpy(global_execdir,path);
	pd1 = strrchr(global_execdir,'/');
	pd2 = strrchr(global_execdir,'\\');
	if (pd1>pd2) *pd1='\0';
	else if (pd2>pd1) *pd2='\0';
	return;
}

void GldMain::set_global_command_line(int argc, const char *argv[])
{
	int i, pos=0;
	for (i=0; i<argc; i++)
	{
		if (pos < (int)(sizeof(global_command_line)-strlen(argv[i])))
		{
			snprintf(global_command_line+pos,sizeof(global_command_line)-pos-1,"%s%s",pos>0?" ":"",argv[i]);
			pos = strlen(global_command_line);
		}
	}
	return;
}

void GldMain::set_global_workdir(const char *path)
{
	if ( path )
		strncpy(global_workdir,path,sizeof(global_workdir)-1);
	else if ( getcwd(global_workdir,sizeof(global_workdir)-1) == NULL )
		output_error("unable to read current working directory");
	return;
}

void GldMain::create_pidfile()
{
	if (strcmp(global_pidfile,"")!=0)
	{
		FILE *fp = fopen(global_pidfile,"w");
		if (fp==NULL)
		{
			output_fatal("unable to create pidfile '%s'", global_pidfile);
			/*	TROUBLESHOOT
				The system must allow creation of the process id file at
				the location indicated in the message.  Create and/or
				modify access rights to the path for that file and try again.
			 */
			exit(XC_PRCERR);
		}
#ifdef WIN32
#define getpid _getpid
#endif
		fprintf(fp,"%d\n",getpid());
		IN_MYCONTEXT output_verbose("process id %d written to %s", getpid(), global_pidfile);
		fclose(fp);
		atexit(delete_pidfile);
	}
	return;
}

void GldMain::delete_pidfile(void)
{
	unlink(global_pidfile);
}

int GldMain::add_on_exit(EXITCALL call)
{
	exitcalls.push_back(call);
	size_t n = exitcalls.size();
	IN_MYCONTEXT output_verbose("add_on_exit(%p) -> %d", call, n);
	return n;
}

int GldMain::add_on_exit(int xc, const char *cmd)
{
	try
	{
		onexitcommand *item = new onexitcommand(xc,cmd);
		exitcommands.push_back(*item);
		size_t n = exitcommands.size();
		IN_MYCONTEXT output_verbose("added on_exit(%d,'%s') -> %d", xc, cmd, n);
		return n;
	}
	catch (...)
	{
		output_error("unable to add on_exit %d '%s'", xc, cmd);
		return 0;
	}
}

int GldMain::run_on_exit(int return_code)
{
	int new_return_code = return_code;
	save_outputs();

	/* save the model */
	if (strcmp(global_savefile,"")!=0)
	{
		if (saveall(global_savefile)==FAILED)
			output_error("save to '%s' failed", global_savefile);
	}

	/* do module dumps */
	if (global_dumpall!=FALSE)
	{
		IN_MYCONTEXT output_verbose("dumping module data");
		module_dumpall();
	}

	/* KML output */
	if (strcmp(global_kmlfile,"")!=0)
		kml_dump(global_kmlfile);

	/* terminate */
	module_termall();

	/* wrap up */
	IN_MYCONTEXT output_verbose("shutdown complete");

	/* profile results */
	if (global_profiler)
	{
		class_profiles();
		module_profiles();
	}

#ifdef DUMP_SCHEDULES
	/* dump a copy of the schedules for reference */
	schedule_dumpall("schedules.txt");
#endif

	/* restore locale */
	locale_pop();

	/* compute elapsed runtime */
	IN_MYCONTEXT output_verbose("elapsed runtime %d seconds", realtime_runtime());
	IN_MYCONTEXT output_verbose("exit code %d", exec.getexitcode());

	for ( std::list<onexitcommand>::iterator cmd = exitcommands.begin() ; cmd != exitcommands.end() ; cmd++ )
	{
		if ( cmd->get_exitcode() == return_code
			|| ( return_code != 0 && cmd->get_exitcode() == -1 )
			)
		{
		    output_flushall();
			new_return_code = cmd->run() >> 8;
			if ( new_return_code != 0 )
			{
				output_error("on_exit %d '%s' command failed (return code %d)", cmd->get_exitcode(), cmd->get_command(), new_return_code);
				exec.setexitcode(EXITCODE(new_return_code));
				goto Done;
			}
			else
			{
				IN_MYCONTEXT output_verbose("running on_exit(%d,'%s') -> code %d", cmd->get_exitcode(), cmd->get_command(), new_return_code);
			}
		}
	}

	for ( std::list<EXITCALL>::iterator call = exitcalls.begin() ; call != exitcalls.end() ; call++ )
	{
	    output_flushall();
		new_return_code = (*call)(return_code);
		if ( new_return_code != 0 )
		{
			output_error("on_exit call failed (return code %d)", new_return_code);
			exec.setexitcode(EXITCODE(new_return_code));
			goto Done;
		}
		else
		{
			IN_MYCONTEXT output_verbose("exitcall() -> code %d", new_return_code);
		}
	}

Done:
	/* compute elapsed runtime */
	IN_MYCONTEXT output_verbose("elapsed runtime %d seconds", realtime_runtime());
	IN_MYCONTEXT output_verbose("exit code %d", new_return_code);
	// simulation   handler   exitcode
	// ---------   ---------  ---------
	//      0          0          0
	//      0          Y          Y
	//      X          0          0
	//      X          Y          Y
	return new_return_code;
}

#include <sys/param.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <paths.h>

extern char **environ;

/*	Function: popens

	Runs a program and connects its stdout and stderr to the FILEs. Only the
	pipes for which file handles are provided are connected.

	Returns:
		struct s_pipes*		success
		NULL				failed (see errno)
 */
struct s_pipes *popens(const char *program, FILE **input, FILE **output, FILE **error)
{
	// extract base command name from program call
	char *command = strdup(program);

	// open dummy pipes to protect from accidental use of stdin, stdout, and stderr
	int t1[2], t2[2];
	pipe(t1);
	pipe(t2);

	// create new pipes
		int readpipe[2] = {-1,-1}, writepipe[2] = {-1,-1}, errorpipe[2] = {-1,-1};
	if ( ( input && pipe(writepipe) < 0 ) || ( output && pipe(readpipe) < 0 ) || ( error && pipe(errorpipe) < 0 ) )
	{
		output_debug("gldcore/main.cpp:popens(const char *program='%s', FILE **output=%p, FILE **error=%p): %s",program,output,error,
			"pipe() failed");
		return NULL;
	}

	// close dummy pipes
	close(t1[0]);
	close(t1[1]);
	close(t2[0]);
	close(t2[1]);

#define READ_END 0
#define WRITE_END 1
#define PARENT_READ readpipe[READ_END] // parent read from child stdout
#define CHILD_WRITE readpipe[WRITE_END] // child write to stdout
#define CHILD_READ writepipe[READ_END] // child read on stdin
#define PARENT_WRITE writepipe[WRITE_END] // parent write to child stdin
#define PARENT_ERROR errorpipe[READ_END] // parent read from child stderr
#define CHILD_ERROR errorpipe[WRITE_END] // child write to stderr

	// allocate new pipeset structure
	struct s_pipes *pipes = (struct s_pipes*)malloc(sizeof(struct s_pipes));
	if ( pipes == NULL )
	{
		output_debug("gldcore/main.cpp:popens(const char *program='%s', FILE **output=%p, FILE **error=%p): %s",program,output,error,
			"malloc() of 'pipes' failed");
		return NULL;
	}

	// fork child process
	pipes->child_pid = fork();
	if ( pipes->child_pid == -1 ) // fork failed
	{
		free(pipes);
		output_debug("gldcore/main.cpp:popens(const char *program='%s', FILE **output=%p, FILE **error=%p): %s",program,output,error,
			"fork() failed");
		return NULL;
	}
	else if ( pipes->child_pid == 0 ) // child process
	{
		close(PARENT_READ);
		close(PARENT_WRITE);
		close(PARENT_ERROR);

		// TODO: this might not work if any of the standard descriptors are closed
		if ( input && CHILD_READ > 0 )
		{
			if ( dup2(CHILD_READ,0) < 0 )
			{
				output_error("unable to prepare child read end of parent output pipe (errno %d, %s, fd=%d)", errno, strerror(errno), CHILD_READ);
			}
		}
		close(CHILD_READ);
		if ( output && CHILD_WRITE > 1 )
		{
			if ( dup2(CHILD_WRITE,1) < 0 )
			{
				output_error("unable to prepare child write end of parent input pipe (errno %d, %s, fd=%d)", errno, strerror(errno), CHILD_WRITE);
			}
		}
		close(CHILD_WRITE);
		if ( error && CHILD_ERROR > 2 )
		{
			if ( dup2(CHILD_ERROR,2) < 0 )
			{
				output_error("unable to prepare child write end of parent error pipe (errno %d, %s, fd=%d)", errno, strerror(errno), CHILD_ERROR);
			}
		}
		close(CHILD_ERROR);
		const int maxargs = 1024;
		char *argv[maxargs];
		int n = 0;
		argv[n++] = strdup("/bin/bash");
		argv[n++] = strdup("-c");
		argv[n++] = command;
		// char *next = NULL, *last = NULL;
		// while ( (next=strtok_r(next?NULL:command," ",&last)) != NULL && n < maxargs-5 )
		// {
		// 	argv[n++] = next;
		// }
		argv[n++] = NULL;
		execve(argv[0], argv, environ);
		output_error("unable run command '%s'",program);
		exit(XC_PRCERR);
	}
	else // parent process
	{
		close(CHILD_READ);
		close(CHILD_WRITE);
		close(CHILD_ERROR);

		if ( input && PARENT_WRITE >= 0 )
		{
			pipes->child_input = *input = fdopen(PARENT_WRITE,"w");
			if ( pipes->child_input == NULL )
			{
				output_error("unable to prepare parent write end of child input pipe (errno %d, %s, fd=%d)", errno, strerror(errno), PARENT_READ);
				free(pipes);
				free(command);
				return NULL;
			}
		}
		else
		{
			pipes->child_input = NULL;
			close(PARENT_WRITE);
		}
		if ( output && PARENT_READ >= 0 )
		{
			pipes->child_output = *output = fdopen(PARENT_READ,"r");
			if ( pipes->child_output == NULL )
			{
				output_error("unable to prepare parent read end of child output pipe (errno %d, %s, fd=%d)", errno, strerror(errno), PARENT_READ);
				free(pipes);
				free(command);
				return NULL;
			}
		}
		else
		{
			pipes->child_output = NULL;
			close(PARENT_READ);
		}
		if ( error && PARENT_ERROR >= 0 )
		{
			pipes->child_error = *error = fdopen(PARENT_ERROR,"r");
			if ( pipes->child_error == NULL )
			{
				output_error("unable to prepare parent read end of child error pipe (errno %d, %s, fd=%d)", errno, strerror(errno), PARENT_READ);
				free(pipes);
				free(command);
				return NULL;
			}
		}
		else
		{
			pipes->child_error = NULL;
			close(PARENT_ERROR);
		}
		pipes->child_command = command;
		return pipes;
	}
}

/*	Function: pcloses
	Waits for the process associated with the stream to terminate and closes its pipes.

 	Returns:
 	-1  	if stream is not associated with a `popen3' command, if already closed, or waitpid returns an error.
 	status	if ok
 */
int pcloses(struct s_pipes *pipes, bool wait)
{
	int pstat = 0;
	pid_t pid = 0;

	if ( ! wait )
	{
		// immediate terminate of the child
		if ( pipes->child_input ) (void)fclose(pipes->child_input);
		if ( pipes->child_output ) (void)fclose(pipes->child_output);
		if ( pipes->child_error ) (void)fclose(pipes->child_error);
		kill(pipes->child_pid,SIGHUP);
	}
	else
	{
		pid = waitpid(pipes->child_pid, &pstat, 0);
		if ( pipes->child_input ) (void)fclose(pipes->child_input);
		if ( pipes->child_output ) (void)fclose(pipes->child_output);
		if ( pipes->child_error ) (void)fclose(pipes->child_error);
	}
	free(pipes->child_command);
	free(pipes);
	if ( pid == -1 )
	{
		return errno;
	}
	else if ( WIFEXITED(pstat) )
	{
		return WEXITSTATUS(pstat);
	}
	else if ( WIFSIGNALED(pstat) )
	{
		return -WTERMSIG(pstat);
	}
	else
	{
		return -127;
	}
}

int ppolls(struct s_pipes *pipes, FILE* input_stream, FILE* output_stream, FILE *error_stream)
{
	struct pollfd polldata[3];
	polldata[0].fd = pipes->child_input ? fileno(pipes->child_input) : 0;
	polldata[0].events = POLLOUT|POLLERR|POLLHUP;
	polldata[1].fd = pipes->child_output ? fileno(pipes->child_output) : 1;
	polldata[1].events = POLLIN|POLLERR|POLLHUP;
	polldata[2].fd = pipes->child_error ? fileno(pipes->child_error) : 2;
	polldata[2].events = POLLIN|POLLERR|POLLHUP;
	char line[65536];
	bool has_error = false;
	// fprintf(stderr,"poll() starting\n"); fflush(stderr);
	while ( poll(polldata,sizeof(polldata)/sizeof(polldata[0]),-1) > 0 )
	{
		// fprintf(stderr,"poll() ok (input.revents=%x, output.revents=%x, error.revents=%x)\n",polldata[0].revents,polldata[1].revents,polldata[2].revents); fflush(stderr);
		if ( polldata[1].revents&POLLIN )
		{
			// fprintf(stderr,"poll() output line received\n"); fflush(stderr);
			if ( pipes->child_output )
			{
				while ( fgets(line, sizeof(line)-1, pipes->child_output) != NULL )
				{
					if ( output_stream )
					{
						fprintf(output_stream,"%s",line);
					}
				}
			}
		}
		if ( polldata[2].revents&POLLIN )
		{
			// fprintf(stderr,"poll() error line received\n"); fflush(stderr);
			if ( pipes->child_error )
			{
				while ( fgets(line, sizeof(line)-1, pipes->child_error) != NULL )
				{
					if ( error_stream )
					{
						fprintf(error_stream,"%s",line);
					}
				}
			}
		}
		if ( polldata[0].revents&POLLOUT )
		{
			// fprintf(stderr,"poll() input line requested\n"); fflush(stderr);
			if ( input_stream )
			{
				while ( fgets(line, sizeof(line)-1, input_stream) != NULL )
				{
					if ( pipes->child_input )
					{
						fprintf(pipes->child_input,"%s",line);
					}
				}
				if ( feof(input_stream) && pipes->child_input )
				{
					// fprintf(stderr,"poll() EOF; closing child input\n"); fflush(stderr);
					fclose(pipes->child_input);
				}
			}
			else if ( pipes->child_input )
			{
					// fprintf(stderr,"poll() no input; closing child input\n"); fflush(stderr);
					fclose(pipes->child_input);
			}
		}
		if ( polldata[0].revents&POLLHUP || polldata[1].revents&POLLHUP || polldata[2].revents&POLLHUP )
		{
			// fprintf(stderr,"poll() hangup\n"); fflush(stderr);
			IN_MYCONTEXT output_verbose("GldMain::subcommand(command='%s'): end of output", pipes->child_command);
			break;
		}
		if ( polldata[0].revents&POLLERR || polldata[1].revents&POLLERR || polldata[2].revents&POLLERR )
		{
			// fprintf(stderr,"poll() error\n"); fflush(stderr);
			output_error("GldMain::subcommand(command='%s'): pipe error", pipes->child_command);
			has_error = true;
			break;
		}
		if ( polldata[0].revents&POLLNVAL )
		{
			// fprintf(stderr,"poll() pipe 0 invalid\n"); fflush(stderr);
			output_error("GldMain::subcommand(command='%s'): input pipe invalid", pipes->child_command);
			has_error = true;
			break;
		}
		if ( polldata[1].revents&POLLNVAL )
		{
			// fprintf(stderr,"poll() pipe 1 invalid\n"); fflush(stderr);
			output_error("GldMain::subcommand(command='%s'): output pipe invalid", pipes->child_command);
			has_error = true;
			break;
		}
		if ( polldata[2].revents&POLLNVAL )
		{
			// fprintf(stderr,"poll() pipe 2 invalid\n"); fflush(stderr);
			output_error("GldMain::subcommand(command='%s'): error pipe invalid", pipes->child_command);
			has_error = true;
			break;
		}
	}
	// fprintf(stderr,"poll() done\n"); fflush(stderr);
	return has_error;
}

int ppolls(struct s_pipes *pipes, char *output_buffer, size_t output_size, FILE *error_stream)
{
	struct pollfd polldata[2];
	polldata[0].fd = pipes->child_output ? fileno(pipes->child_output) : 0;
	polldata[0].events = POLLIN|POLLERR|POLLHUP;
	polldata[1].fd = pipes->child_error ? fileno(pipes->child_error) : 0;
	polldata[1].events = POLLIN|POLLERR|POLLHUP;
	bool has_error = false;
	char line[65536];
	size_t len = 0;
	while ( poll(polldata,sizeof(polldata)/sizeof(polldata[0]),-1) > 0 && len < output_size-1)
	{
		if ( pipes->child_output && len < output_size-1 && polldata[0].revents&POLLIN )
		{
			while ( fgets(line, sizeof(line)-1, pipes->child_output) != NULL )
			{
				snprintf(output_buffer+len,output_size-len-1,"%s",line);
				len = strlen(output_buffer);
			}
			if ( ferror(pipes->child_output) )
			{
				output_error("pipe read failed on child stdout (errno=%d %s, fd=%d)\n",errno,strerror(errno),fileno(pipes->child_output)); fflush(stderr);
			}
			if ( ! feof(pipes->child_output) )
			{
				output_error("pipe read incomplete on child stdout (errno=%d %s, fd=%d)\n",errno,strerror(errno),fileno(pipes->child_output)); fflush(stderr);
			}
		}
		if ( pipes->child_error && error_stream && polldata[1].revents&POLLIN )
		{
			while ( fgets(line, sizeof(line)-1, pipes->child_error) != NULL )
			{
				fprintf(error_stream,"%s",line);
			}
			if ( ferror(pipes->child_error) )
			{
				output_error("pipe read on child stderr (errno=%d %s, fd=%d)\n",errno,strerror(errno),fileno(pipes->child_error)); fflush(stderr);
			}
			if ( ! feof(pipes->child_error) )
			{
				output_error("pipe read incomplete on child stderr (errno=%d %s, fd=%d)\n",errno,strerror(errno),fileno(pipes->child_output)); fflush(stderr);
			}
		}
		if ( polldata[0].revents&POLLHUP || polldata[1].revents&POLLHUP )
		{
			IN_MYCONTEXT output_verbose("GldMain::subcommand(command='%s'): end of output", pipes->child_command);
			break;
		}
		if ( polldata[0].revents&POLLERR || polldata[1].revents&POLLERR )
		{
			output_error("GldMain::subcommand(command='%s'): pipe error", pipes->child_command);
			has_error = true;
			break;
		}
		if ( polldata[0].revents&POLLNVAL )
		{
			output_error("GldMain::subcommand(command='%s'): output pipe invalid", pipes->child_command);
			has_error = true;
			break;
		}
		if ( polldata[1].revents&POLLNVAL )
		{
			output_error("GldMain::subcommand(command='%s'): error pipe invalid", pipes->child_command);
			has_error = true;
			break;
		}
	}
	return has_error;
}


int GldMain::subcommand(const char *format, ...)
{
	char *command = NULL;
	va_list ptr;
	va_start(ptr,format);
	if ( vasprintf(&command,format,ptr) < 0 || command == NULL )
	{
		output_error("GldMain::subcommand(format='%s',...): memory allocation failed",format);
		return -1;
	}
	va_end(ptr);

	FILE *output = NULL, *error = NULL;
	int rc = 0;
	struct s_pipes * pipes = popens(command, NULL, &output, &error);
	if ( pipes == NULL )
	{
		output_error("GldMain::subcommand(format='%s',...): unable to run command '%s' (%s)",format,command,strerror(errno));
		rc = -1;
	}
	else
	{
		IN_MYCONTEXT output_verbose("running subcommand '%s'",command);
		FILE *output_stream = output_get_stream("output");
		FILE *error_stream = output_get_stream("error");
        if ( global_echo )
        {
            int len = strlen(global_execdir);
            if ( strncmp(command,global_execdir,len) == 0 )
                fprintf(output_stream,"# %s",command+len+10);
            else
            fprintf(output_stream,"# %s",command);            
        }
		ppolls(pipes,NULL,output_stream,error_stream);
		rc = pcloses(pipes);
		if ( rc > 0 )
		{
			output_error("GldMain::subcommand(format='%s',...): command '%s' returns code %d",format,command,rc);
		}
		IN_MYCONTEXT output_verbose("subcommand '%s' -> status = %d",command,rc);
	}
	free(command);
	return rc;
}

bool GldMain::check_runtime(bool use_exception)
{
	time_t now = time(NULL);
	if ( global_maximum_runtime > 0 && (now-starttime) > global_maximum_runtime )
	{
		if ( use_exception )
		{
			throw_exception("GldMain::check_runtime(): maximum runtime %lld reached", (int64)global_maximum_runtime);
		}
		else
		{
			output_error("maximum runtime of %lld seconds reached", (int64)global_maximum_runtime);
			return false;
		}
	}
	return true;
}

/** @} **/
