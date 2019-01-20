// main.h
// Copyright (C) 2018 Regents of the Leland Stanford Junior University

#ifndef _MAIN_H
#define _MAIN_H

#include "globals.h"

class GldMain {
public:		// public variables

public:	// private variables
	GldGlobals globals;

public:		// constructor/destructor
	GldMain(int argc = 0, char *argv[] = NULL);
	~GldMain(void);
#if defined WIN32 && _DEBUG 
	static void pause_at_exit(void);
#endif

public:		// public methods
	void mainloop(int argc = 0, char *argv[] = NULL);

private:	// private methods
	void set_global_browser(const char *path = NULL);
	void set_global_execname(const char *path);
	void set_global_execdir(const char *path);
	void set_global_command_line(int argc = 0,char *argv[] = NULL);
	void set_global_workdir(const char *path = NULL);
	void create_pidfile(void);
	static void delete_pidfile(void);

public:		// global methods
	inline STATUS global_init(void) { return globals.init();};
	inline GLOBALVAR *global_getfirst(void) { return globals.getnext(NULL);};
	inline GLOBALVAR *global_getnext(const GLOBALVAR *var) { return globals.getnext(var);};
	inline GLOBALVAR *global_find(const char *name) { return globals.find(name);};
	inline GLOBALVAR *global_create(const char *name, ...) { va_list ptr; va_start(ptr,name); GLOBALVAR *var = globals.create_v(name,ptr); va_end(ptr); return var;};
	inline STATUS global_setvar(char *def,...) { va_list ptr; va_start(ptr,def); STATUS res = globals.setvar_v(def,ptr); va_end(ptr); return res;};
	inline const char *global_getvar(const char *name, char *buffer, size_t size) { return globals.getvar(name,buffer,size);};
	inline bool global_isdefined(const char *name) { return globals.isdefined(name);};
	inline void global_dump(void) { return globals.dump();};
	inline size_t global_getcount(void) { return globals.getcount();};
	inline void global_restore(GLOBALVAR *pos) { return globals.restore(pos);};
	inline void global_push(char *name, char *value) { return globals.push(name,value);};
	inline void dump(void) { return globals.dump();};
	inline void *remote_read(void *local, GLOBALVAR *var) { return globals.remote_read(local,var);};
	inline void remote_write(void *local, GLOBALVAR *var) { return globals.remote_write(local,var);};
	inline size_t global_saveall(FILE *fp) { return globals.saveall(fp);};

};
extern GldMain *my_instance; // TODO: move this into main() to make system globally reentrant

#endif
