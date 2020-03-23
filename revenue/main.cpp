// File: main.cpp
// Copyright (C) 2020 Regents of the Leland Stanford Junior University

#define DLMAIN

#include "revenue.h"

EXPORT CLASS *init(CALLBACKS *fntable, MODULE *module, int argc, char *argv[])
{
	if (set_callback(fntable)==NULL)
	{
		errno = EINVAL;
		return NULL;
	}

	INIT_MMF(revenue);

	// TODO: add the first class here
	new billing(module);

	// TODO: add more classes here
	// new {{{class2}}}(module);

	// TODO: register module globals here
	//gl_global_create("revenue::{{{global1_name}}}",PT_{{{global1_type}}},&{{{global1_name}}},NULL);

	// always return the first class registered */
	return billing::oclass;
}


EXPORT int do_kill(void*)
{
	// TODO: actions when module is unloaded
	return 0;
}

EXPORT int check()
{
	// TODO: actions when --check command line option is used
	return 0;
}
