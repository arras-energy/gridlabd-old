/* $Id: reliability.h 4738 2014-07-03 00:55:39Z dchassin $ 
	Copyright (C) 2008 Battelle Memorial Institute
*/

#ifndef _RELIABILITY_H
#define _RELIABILITY_H

#include <stdarg.h>
#include "gridlabd.h"

//Module EXTERNs
EXTERN double event_max_duration INIT(432000.0);	/**< Maximum length of any event on the system - given in seconds - defaults to 5 days */
EXTERN bool enable_subsecond_models INIT(false);	/**< normally not operating in delta mode */
EXTERN unsigned long deltamode_timestep INIT(10000000); /* 10 ms timestep */
EXTERN FUNCTIONADDR *delta_functions INIT(NULL);			/* Array pointer functions for objects that need deltamode interupdate calls */
EXTERN OBJECT **delta_objects INIT(NULL);				/* Array pointer objects that need deltamode interupdate calls */
EXTERN int eventgen_object_count INIT(0);		/* deltamode object count */
EXTERN int eventgen_object_current INIT(-1);		/* Index of current deltamode object */
EXTERN TIMESTAMP deltamode_starttime INIT(TS_NEVER);	/* Tracking variable for next desired instance of deltamode */
EXTERN TIMESTAMP deltamode_endtime INIT(TS_NEVER);		/* Tracking variable to see when deltamode ended - so differential calculations don't get messed up */

void schedule_deltamode_start(TIMESTAMP tstart);

#endif
