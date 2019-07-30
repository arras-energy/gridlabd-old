// powerflow/solver_ml.h
// dchassin@slac.stanford.edu

#ifndef _SOLVER_ML
#define _SOLVER_ML

#include <sys/timeb.h>

#include "solver_nr.h"

//	Typedef: PROBLEMID
//	Problem identifier
typedef unsigned long long PROBLEMID;

//	Structure: s_solution
//	Solution data
struct s_solution {
	PROBLEMID id;
	unsigned int bus_count;
	BUSDATA *bus;
	unsigned int branch_count;
	BRANCHDATA *branch;
	NR_SOLVER_STRUCT *powerflow_values;
	NRSOLVERMODE powerflow_type;
	NR_MESHFAULT_IMPEDANCE *mesh_imped_values;
	bool *bad_computations;
	int64 iterations; 
	double runtime;
	struct s_solution *prev;
	struct s_solution *next;
};

//	Typedef: SOLVERMODEL
//	Solution data type
typedef struct s_solution SOLVERMODEL;

//	Enum: e_solvermodelstatus
//	SMS_INIT - initializing
//	SMS_READY - ready
//	SMS_DISABLED - disabled
//	SMS_FAILED - failed
enum e_solvermodelstatus {SMS_INIT, SMS_READY, SMS_DISABLED, SMS_FAILED};

//	Typedef: SOLVERMODELSTATUS
//	Solver model status
typedef enum e_solvermodelstatus SOLVERMODELSTATUS;

//	Function: solver_model_init
//	Initialize the solver modeling system
int solver_model_init(void);

SOLVERMODEL *solver_model_new(unsigned int bus_count,
							  BUSDATA *bus, 
							  unsigned int branch_count, 
							  BRANCHDATA *branch, 
							  NR_SOLVER_STRUCT *powerflow_values, 
							  NRSOLVERMODE powerflow_type , 
							  NR_MESHFAULT_IMPEDANCE *mesh_imped_values, 
							  bool *bad_computations,
							  int64 iterations);

//	Function: solver_model_find
//	Find a solver model for the specified problem
bool solver_model_find(SOLVERMODEL *&model,
						 unsigned int &bus_count,
						 BUSDATA *&bus,
						 unsigned int &branch_count,
						 BRANCHDATA *&branch);

//	Function: solver_model_apply
//	Apply a solver model
int64 solver_model_apply(SOLVERMODEL *model,
						NR_SOLVER_STRUCT *&powerflow_values,
						NRSOLVERMODE &powerflow_type,
						NR_MESHFAULT_IMPEDANCE *&mesh_imped_values,
						bool *&bad_computations,
						int64 iterations);

#endif
