/** $Id: main.cpp 4738 2014-07-03 00:55:39Z dchassin $
	Copyright (C) 2008 Battelle Memorial Institute
	@file init.cpp
	@defgroup market Module template
	@ingroup modules

	The market module implements various power market structures.

 @{ 
 **/

/* DO NOT EDIT THE NEXT LINE 
MODULE:market
 */

#define DLMAIN

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>
#include "gridlabd.h"

#include "optimize.h"
#include "simple.h"

EXPORT CLASS *init(CALLBACKS *fntable, MODULE *module, int argc, char *argv[])
{
	if (set_callback(fntable)==NULL)
	{
		errno = EINVAL;
		return NULL;
	}

	INIT_MMF(optimize);

	new simple(module);

	/*** DO NOT EDIT NEXT LINE ***/
	//NEWCLASS

	/* always return the first class registered */
	return simple::oclass;
}


EXPORT int do_kill(void*)
{
	/* if global memory needs to be released, this is a good time to do it */
	return 0;
}

/**@}*/
