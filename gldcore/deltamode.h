/*	File: deltamode.h 
 	Copyright (C) 2008, Battelle Memorial Institute

	Deltamode is the primary solution method for subsecqnd quasi-dynamic simulations.
 */

#ifndef _DELTAMODE_H
#define _DELTAMODE_H

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

#include "globals.h"

/*	Typedef: DELTAPROFILE
		See <s_deltaprofile>

	Structure: s_deltaprofile
	t_init - elapsed time in initialization
	t_preupdate - elapsed time in pre-update
	t_update - elapsed time in update
	t_clockupdate - elapsed time in clock update
	t_interupdate - elapsed time in inter-update
	t_postupdate - elapsed time in post-update
	t_delta - total elapsed time in deltamode
	t_count - count of updates
	t_max - maximum delta time (ns)
	t_min - minimum delta time (ns)
	module_list - list of active module in deltamode

	This structure stores all the deltamode profile data
 */
typedef struct s_deltaprofile {
	clock_t t_init; 
	clock_t t_preupdate; 
	clock_t t_update; 
	clock_t t_clockupdate; 
	clock_t t_interupdate; 
	clock_t t_postupdate; 
	unsigned int64 t_delta; 
	unsigned int64 t_count; 
	unsigned int64 t_max;	
	unsigned int64 t_min;	
	char module_list[1024]; 
} DELTAPROFILE;

#ifdef __cplusplus
extern "C" {
#endif

/*	Function: delta_init

	This function initializes deltamode.

	Return: 
	!= 0 - success
	== 0 - failure
 */
DEPRECATED STATUS delta_init(void);

/*	Function: delta_update

	This function performs an update in deltamode

	Return:
	<= 0 - failure
	dt > 0 - success, dt is the seconds to advance
 */
DEPRECATED DT delta_update(void); 

/*	Function: delta_modedesired

	This function asks module how many seconds until deltamode is required

	Return:
	DT_INVALID - error
	DT_INFINITY - no delta mode needed
 */
DEPRECATED DT delta_modedesired(DELTAMODEFLAGS *flags); 

/*	Function: delta_preupdate

	This function dispatches preupdate deltamode calls

	Return:
	dt == DT_INVALID - failed
	dt > 0 - timestep desired in deltamode
 */
DEPRECATED DT delta_preupdate(void); 

/*	Function: delta_interupdate

	This function dispatches interupdate deltamode calls
 
 	Return:
 	INIT = 0 - (reserved)
 	EVENT = 1 - end deltamode and switches back to eventmode solver
 	DELTA = 2 - continue in deltamode
 	DELTA_ITER = 3 - interation in deltamode needed
 	ERROR = 255 - error
  */
DEPRECATED SIMULATIONMODE delta_interupdate(DT timestep, unsigned int iteration_count_val); 

/*	Function: delta_clockupdate

	This function notifies that the current deltamode timestep is finished and are moving to the next timestep.

	Return:
 */
DEPRECATED SIMULATIONMODE delta_clockupdate(DT timestep, SIMULATIONMODE interupdate_result); 

/*	Function: delta_postupdate

	This function dispatches postupdate deltamode calls

	Return:
	FAILED - failed
	SUCCESS - success
 */
DEPRECATED STATUS delta_postupdate(void); 

/*	Function: delta_getprofile

	This function get the profile data for deltamode.

	Return:
	- Reference to a <DELTAPROFILE> structure
 */
DEPRECATED DELTAPROFILE *delta_getprofile(void);

#ifdef __cplusplus
}
#endif

#endif
