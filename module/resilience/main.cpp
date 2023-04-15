// module/resilience/main.cpp
// Copyright (C) 2022 Regents of the Leland Stanford Junior University

#define DLMAIN
#include "gridlabd.h"
#undef DLMAIN

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>
#include "gridlabd.h"

#include "resilience.h"

EXPORT CLASS *init(CALLBACKS *fntable, MODULE *module, int argc, char *argv[])
{
	if (set_callback(fntable)==NULL)
	{
		errno = EINVAL;
		return NULL;
	}

	INIT_MMF(resilience);

	new metrics(module);
	new outage(module);
	new cost(module);
	new impact(module);

	return metrics::oclass; // always return the first class registered
}


EXPORT int do_kill(void*)
{
	return 0;
}

EXPORT int check()
{
	return 0;
}
