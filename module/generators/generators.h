/** $Id: generators.h,v 1.2 2008/02/12 00:28:08 d3g637 Exp $
	Copyright (C) 2008 Battelle Memorial Institute
 **/

#ifndef _GENERATORS_H
#define _GENERATORS_H

#include "gridlabd.h"

//Phase definitions pilfered from powerflow_object.h for readability
#define PHASE_A		0x0001		/**< A phase connection */
#define PHASE_B		0x0002		/**< B phase connection */
#define PHASE_C		0x0004		/**< C phase connection */
#define PHASE_ABC	0x0007		/**< three phases connection */
#define PHASE_N		0x0008		/**< N phase connected */
#define PHASE_ABCN	0x000f		/**< three phases neutral connection */
#define GROUND		0x0080		/**< ground line connection */
/* delta configuration */
#define PHASE_D		0x0100		/**< delta connection (requires ABCN) */
/* split phase configurations */
#define PHASE_S1	0x0010		/**< split line 1 connection */
#define PHASE_S2	0x0020		/**< split line 2 connection */
#define PHASE_SN	0x0040		/**< split line neutral connection */
#define PHASE_S		0x0070		/**< Split phase connection */
#define TSNVRDBL 9223372036854775808.0

EXTERN bool enable_subsecond_models INIT(false); /* normally not operating in delta mode */
EXTERN unsigned long deltamode_timestep INIT(10000000); /* 10 ms timestep */
EXTERN double deltamode_timestep_publish INIT(10000000.0); /* 10 ms timestep */
EXTERN OBJECT **delta_objects INIT(NULL);				/* Array pointer objects that need deltamode interupdate calls */
EXTERN FUNCTIONADDR *delta_preupdate_functions INIT(NULL);	/* Array pointer functions for objects that need deltamode preupdate calls */
EXTERN FUNCTIONADDR *delta_functions INIT(NULL);			/* Array pointer functions for objects that need deltamode interupdate calls */
EXTERN FUNCTIONADDR *post_delta_functions INIT(NULL);		/* Array pointer functions for objects that need deltamode postupdate calls */
EXTERN int gen_object_count INIT(0);		/* deltamode object count */
EXTERN int gen_object_current INIT(-1);		/* Index of current deltamode object */
EXTERN TIMESTAMP deltamode_starttime INIT(TS_NEVER);	/* Tracking variable for next desired instance of deltamode */
EXTERN TIMESTAMP deltamode_endtime INIT(TS_NEVER);		/* Tracking variable to see when deltamode ended - so differential calculations don't get messed up */
EXTERN double deltamode_endtime_dbl INIT(TSNVRDBL);		/* Tracking variable to see when deltamode ended - double valued for explicit movement calculations */
EXTERN TIMESTAMP deltamode_supersec_endtime INIT(TS_NEVER);	/* Tracking variable to indicate the "floored" time of detamode_endtime */
EXTERN double deltatimestep_running INIT(-1.0);			/** Value of the current deltamode simulation - used primarily to tell if we're in deltamode or not for VSI */

void schedule_deltamode_start(TIMESTAMP tstart);	/* Anticipated time for a deltamode start, even if it is now */
void allocate_deltamode_arrays(void);				/* Overall function to allocate deltamode capabilities - rather than having to edit everything */

/*** DO NOT DELETE THE NEXT LINE ***/
//NEWCLASS
#include "power_electronics.h"
#include "energy_storage.h"
#include "battery.h"
#include "central_dg_control.h"
#include "controller_dg.h"
#include "dc_dc_converter.h"
#include "diesel_dg.h"
#include "generators.h"
#include "inverter.h"
#include "microturbine.h"
#include "rectifier.h"
#include "solar.h"
#include "windturb_dg.h"

#define UNKNOWN 0

/* optional exports */
#ifdef OPTIONAL

/* TODO: define this function to enable checks routine */
EXPORT int check(void);

/* TODO: define this function to allow direct import of models */
EXPORT int import_file(char *filename);

/* TODO: define this function to allow direct export of models */
EXPORT int export_file(char *filename);

/* TODO: define this function to allow export of KML data for a single object */
EXPORT int kmldump(FILE *fp, OBJECT *obj);
#endif

#endif
