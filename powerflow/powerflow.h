/** $Id: powerflow.h 1182 2008-12-22 22:08:36Z dchassin $
	Copyright (C) 2008 Battelle Memorial Institute
	@file powerflow.h
	@ingroup powerflow

 @{
 **/


#ifndef _POWERFLOW_H
#define _POWERFLOW_H

#include "gridlabd.h"
#include "solver_nr.h"

#ifdef _DEBUG
void print_matrix(complex mat[3][3]);
#endif

#define GETOBJECT(obj) ((OBJECT *) obj - 1)
#define IMPORT_CLASS(name) extern CLASS *name##_class

//Deltamode use
#define TSNVRDBL 9223372036854775808.0

typedef enum {SM_FBS=0, SM_GS=1, SM_NR=2} SOLVERMETHOD;		/**< powerflow solver methodology */
typedef enum {MM_SUPERLU=0, MM_EXTERN=1} MATRIXSOLVERMETHOD;	/**< NR matrix solver methodlogy */
typedef enum {
	MD_NONE=0,			///< No matrix dump desired
	MD_ONCE=1,			///< Single matrix dump desired
	MD_PERCALL=2,		///< Matrix dump every call desired
	MD_ALL=3			///< Matrix dump on every iteration desired
} MATRIXDUMPMETHOD;

typedef enum {
	LS_OPEN=0,			///< defines that that link is open
	LS_CLOSED=1			///< defines that that link is closed
} LINESTATUS;	//Line/link status - made at powerflow level for reusability

//Structure to hold external LU solver calls
typedef struct s_ext_fxn {
	void *dllLink;
	void *ext_init;
	void *ext_alloc;
	void *ext_solve;
	void *ext_destroy;
} EXT_LU_FXN_CALLS;

EXTERN char256 LUSolverName INIT("");				/**< filename for external LU solver */
EXTERN EXT_LU_FXN_CALLS LUSolverFcns;				/**< links to external LU solver functions */
EXTERN SOLVERMETHOD solver_method INIT(SM_FBS);		/**< powerflow solver methodology */
EXTERN char256 MDFileName INIT("");					/**< filename for matrix dump */
EXTERN MATRIXDUMPMETHOD NRMatDumpMethod INIT(MD_NONE);	/**< NR-based matrix output method */
EXTERN bool NRMatReferences INIT(false);			/**< Flag to indicate if the decoding information for the matrix is dumped - row/col to bus */
EXTERN bool use_line_cap INIT(false);				/**< Flag to include line capacitance quantities */
EXTERN bool use_link_limits INIT(true);				/**< Flag to include line/transformer ratings and provide a warning if exceeded */
EXTERN MATRIXSOLVERMETHOD matrix_solver_method INIT(MM_SUPERLU);	/**< Newton-Raphson uses superLU as the default solver */
EXTERN unsigned int NR_bus_count INIT(0);			/**< Newton-Raphson bus count - used for determining size of bus vector */
EXTERN unsigned int NR_branch_count INIT(0);		/**< Newton-Raphson branch count - used for determining size of branch vector */
EXTERN BUSDATA *NR_busdata INIT(NULL);				/**< Newton-Raphson bus data pointer array */
EXTERN BRANCHDATA *NR_branchdata INIT(NULL);		/**< Newton-Raphson branch data pointer array */
EXTERN NR_SOLVER_STRUCT NR_powerflow;				/**< Newton-Raphson solver working variables - "steady-state" powerflow version */
EXTERN int NR_curr_bus INIT(-1);					/**< Newton-Raphson current bus indicator - used to populate NR_busdata */
EXTERN int NR_curr_branch INIT(-1);					/**< Newton-Raphson current branch indicator - used to populate NR_branchdata */
EXTERN int64 NR_iteration_limit INIT(500);			/**< Newton-Raphson iteration limit (per GridLAB-D iteration) */
EXTERN bool NR_dyn_first_run INIT(true);			/**< Newton-Raphson first run indicator - used by deltamode functionality for initialization powerflow */
EXTERN bool NR_admit_change INIT(true);				/**< Newton-Raphson admittance matrix change detector - used to prevent complete recalculation of admittance at every timestep */
EXTERN int NR_superLU_procs INIT(1);				/**< Newton-Raphson related - superLU MT processor count to request - separate from thread_count */
EXTERN TIMESTAMP NR_retval INIT(TS_NEVER);			/**< Newton-Raphson current return value - if t0 objects know we aren't going anywhere */
EXTERN OBJECT *NR_swing_bus INIT(NULL);				/**< Newton-Raphson swing bus */
EXTERN int NR_swing_bus_reference INIT(-1);			/**< Newton-Raphson swing bus index reference in NR_busdata */
EXTERN int64 NR_delta_iteration_limit INIT(10);		/**< Newton-Raphson iteration limit (per deltamode timestep) */
EXTERN bool FBS_swing_set INIT(false);				/**< Forward-Back Sweep swing assignment variable */
EXTERN bool show_matrix_values INIT(false);			/**< flag to enable dumping matrix calculations as they occur */
EXTERN double primary_voltage_ratio INIT(60.0);		/**< primary voltage ratio (@todo explain primary_voltage_ratio in powerflow (ticket #131) */
EXTERN double nominal_frequency INIT(60.0);			/**< nomimal operating frequencty */
EXTERN double warning_underfrequency INIT(55.0);	/**< frequency below which a warning is posted */
EXTERN double warning_overfrequency INIT(65.0);		/**< frequency above which a warning is posted */
EXTERN double warning_undervoltage INIT(0.8);		/**< voltage magnitude (per unit) below which a warning is posted */
EXTERN double warning_overvoltage INIT(1.2);		/**< voltage magnitude (per unit) above which a warning is posted */
EXTERN double warning_voltageangle INIT(2.0);		/**< voltage angle (over link) above which a warning is posted */
EXTERN bool require_voltage_control INIT(false);	/**< flag to enable voltage control source requirement */
EXTERN double geographic_degree INIT(0.0);			/**< topological degree factor */
EXTERN complex fault_Z INIT(complex(1e-6,0));		/**< fault impedance */
EXTERN complex ground_Z INIT(complex(1e-6,0));		/**< ground impedance */
EXTERN double default_maximum_voltage_error INIT(1e-6);	/**< default sync voltage convergence limit [puV] */
EXTERN double default_maximum_power_error INIT(0.0001);	/**< default power convergence limit for multirun */
EXTERN OBJECT *restoration_object INIT(NULL);		/**< restoration object of the system */
EXTERN OBJECT *fault_check_object INIT(NULL);		/**< fault_check object of the system */
EXTERN bool meshed_fault_checking_enabled INIT(false);	/*** fault_check object flag for possible meshing -- adjusts how reliability-related code runs */
EXTERN bool restoration_checks_active INIT(false);	/***< Overall flag for when reconfigurations are occurring - special actions in devices */

#ifndef _GENERATORS_H
EXTERN bool enable_subsecond_models INIT(false);		/* normally not operating in delta mode */
EXTERN bool all_powerflow_delta INIT(false);			/* Flag to make all powerflow objects participate in deltamode -- otherwise is individually flagged per object */
EXTERN unsigned long deltamode_timestep INIT(10000000); /* deltamode timestep value - 10 ms timestep, at first - intermnal */
EXTERN double deltamode_timestep_publish INIT(10000000.0); /* deltamode module-published 10 ms timestep, at first -- module property version, to be converted*/
EXTERN OBJECT **delta_objects INIT(NULL);				/* Array pointer objects that need deltamode interupdate calls */
EXTERN FUNCTIONADDR *delta_functions INIT(NULL);	/* Array pointer functions for objects that need deltamode interupdate calls */
EXTERN FUNCTIONADDR *post_delta_functions INIT(NULL);		/* Array pointer functions for objects that need deltamode postupdate calls */
EXTERN int pwr_object_count INIT(0);				/* deltamode object count */
EXTERN int pwr_object_current INIT(-1);				/* Index of current deltamode object */
EXTERN TIMESTAMP deltamode_starttime INIT(TS_NEVER);	/* Tracking variable for next desired instance of deltamode */
EXTERN TIMESTAMP deltamode_endtime INIT(TS_NEVER);		/* Tracking variable to see when deltamode ended - so differential calculations don't get messed up */
EXTERN double deltamode_endtime_dbl INIT(TSNVRDBL);		/* Tracking variable to see when deltamode ended - double valued for explicit movement calculations */
EXTERN TIMESTAMP deltamode_supersec_endtime INIT(TS_NEVER);	/* Tracking variable to indicate the "floored" time of detamode_endtime */
EXTERN double current_frequency INIT(60.0);			/**< Current operating frequency of the system - used by deltamode stuff */
EXTERN bool master_frequency_update INIT(false);	/**< Whether a generator has designated itself "keeper of frequency" -- temporary deltamode override */
EXTERN bool enable_frequency_dependence INIT(false);	/**< Flag to enable frequency-based updates of impedance values, namely loads and lines */
EXTERN double default_resistance INIT(1e-4);		/**< sets the default resistance for safety devices */

//In-rush deltamode stuff
EXTERN bool enable_inrush_calculations INIT(false);	/**< Flag to enable in-rush calculations in deltamode */
EXTERN double impedance_conversion_low_pu INIT(0.7);	/** Lower PU voltage level to convert all loads to impedance */
EXTERN double deltatimestep_running INIT(-1.0);			/** Value of the current deltamode simulation - used for integration method in in-rush */
#endif

//Mesh fault current stuff
EXTERN bool enable_mesh_fault_current INIT(false);	/** Flag to enable mesh-based fault current calculations */

// Convergence error handling
typedef enum {CEH_FATAL=0, CEH_IGNORE=1, CEH_COLLAPSE=2} CONVERGENCEERRORHANDLING;
EXTERN enumeration convergence_error_handling INIT(CEH_FATAL); /** Flag to control how convergence errors are handled */

// Deltamode stuff
void schedule_deltamode_start(TIMESTAMP tstart);	/* Anticipated time for a deltamode start, even if it is now */

/* used by many powerflow enums */
#define UNKNOWN 0
#define ROUNDOFF 1e-6			// numerical accuracy for zero in float comparisons


#include "powerflow_object.h"

#endif // _POWERFLOW_H

/**@}*/
