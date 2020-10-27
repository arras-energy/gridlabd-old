// init.cpp
// Copyright (C) 2020, Regents of the Leland Stanford Junior University

#define DLMAIN

#include "industrial.h"

EXPORT CLASS *init(CALLBACKS *fntable, MODULE *module, int argc, char *argv[])
{
	if (set_callback(fntable)==NULL)
	{
		errno = EINVAL;
		return NULL;
	}

	INIT_MMF(industrial);

	new industrial(module);

	/* always return the first class registered */
	return industrial::oclass;
}

EXPORT int do_kill(void*)
{
	/* if global memory needs to be released, this is a good time to do it */
	return 0;
}

EXPORT int check(){
	/* if any assert objects have bad filenames, they'll fail on init() */
	return 0;
}
