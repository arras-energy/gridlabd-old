// powerflow/solver_ml.h
// dchassin@slac.stanford.edu

#ifndef _SOLVER_PY
#define _SOLVER_PY

#include <sys/timeb.h>

#include "solver_nr.h"

//	Typedef: SOLVERPYTHON
//	Solution data type
typedef struct s_solver_python SOLVERPYTHON;

//	Enum: e_solverpythonstatus
//	SPS_INIT - initializing
//	SPS_READY - ready
//	SPS_DISABLED - disabled
//	SPS_FAILED - failed
enum e_solverpythonstatus {SPS_INIT=0, SPS_READY=1, SPS_DISABLED=2, SPS_FAILED=3};

//	Typedef: SOLVERPYTHONSTATUS
//	Solver python status
typedef enum e_solverpythonstatus SOLVERPYTHONSTATUS;

//	Function: solver_model_init
//	Initialize the python solver system
//
//	Return: 0 on success, or non-zero error code
int solver_python_init(void);

//	Function: solver_model_find
//	Prepare the python solver system for a solution
//
//	Return: iterations runs; negative is solver failed, >=0 on success
int solver_python_solve (
	unsigned int &bus_count,
	BUSDATA *&bus,
	unsigned int &branch_count,
	BRANCHDATA *&branch,
	NR_SOLVER_STRUCT *powerflow_values, 
	NRSOLVERMODE powerflow_type , 
	NR_MESHFAULT_IMPEDANCE *mesh_imped_values, 
	bool *bad_computations,
	int64 &iterations);

//	Function: solver_model_apply
//	Post a solution to the python solver
void solver_python_learn (
	unsigned int bus_count,
	BUSDATA *bus, 
	unsigned int branch_count, 
	BRANCHDATA *branch, 
	NR_SOLVER_STRUCT *powerflow_values, 
	NRSOLVERMODE powerflow_type , 
	NR_MESHFAULT_IMPEDANCE *mesh_imped_values, 
	bool *bad_computations,
	int64 iterations);

//	Function: solver_dump
//	Dump solver data
void solver_dump (
	unsigned int &bus_count,
	BUSDATA *&bus,
	unsigned int &branch_count,
	BRANCHDATA *&branch,
	bool allow_generic_dumpfile);

#endif
