/** $Id: deltamode.h 4738 2014-07-03 00:55:39Z dchassin $
	Copyright (C) 2011 Battelle Memorial Institute
 **/

#ifndef _DELTAMODE_H
#define _DELTAMODE_H

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

#include "globals.h"

typedef struct {
	clock_t t_init; /**< time in initiation */
	clock_t t_preupdate; /**< time in preupdate */
	clock_t t_update; /**< time in update */
	clock_t t_clockupdate; /**< time in clockupdate */
	clock_t t_interupdate; /**< time in interupdate */
	clock_t t_postupdate; /**< time in postupdate */
	unsigned int64 t_delta; /**< total elapsed delta mode time (s) */
	unsigned int64 t_count; /**< number of updates */
	unsigned int64 t_max;	/**< maximum delta (ns) */
	unsigned int64 t_min;	/**< minimum delta (ns) */
	char module_list[1024]; /**< list of active modules */
} DELTAPROFILE;

#ifdef __cplusplus
extern "C" {
#endif

STATUS delta_init(void); /* initialize delta mode - 0 on fail */
DT delta_update(void); /* update in delta mode - <=0 on fail, seconds to advance clock if ok */
DT delta_modedesired(DELTAMODEFLAGS *flags); /* ask module how many seconds until deltamode is needed, 0xfffffff(DT_INVALID)->error, oxfffffffe(DT_INFINITY)->no delta mode needed */
DT delta_preupdate(void); /* send preupdate messages ; dt==0|DT_INVALID failed, dt>0 timestep desired in deltamode  */
SIMULATIONMODE delta_interupdate(DT timestep, unsigned int iteration_count_val); /* send interupdate messages  - 0=INIT (used?), 1=EVENT, 2=DELTA, 3=DELTA_ITER, 255=ERROR */
SIMULATIONMODE delta_clockupdate(DT timestep, SIMULATIONMODE interupdate_result); /* notification that we are finished with the current deltamode timestep and are moving to the next timestep. */
STATUS delta_postupdate(void); /* send postupdate messages - 0 = FAILED, 1=SUCCESS */
DELTAPROFILE *delta_getprofile(void);

#ifdef __cplusplus
}
#endif

#endif
