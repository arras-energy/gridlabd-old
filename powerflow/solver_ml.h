// powerflow/solver_ml.h
// dchassin@slac.stanford.edu

#ifndef _SOLVER_ML
#define _SOLVER_ML

#include "solver_nr.h"

typedef unsigned long long PROBLEMID;
typedef struct s_solution {
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
	struct s_solution *prev;
	struct s_solution *next;
} SOLVERMODEL;
typedef enum {SMS_INIT, SMS_READY, SMS_DISABLED, SMS_FAILED} SOLVERMODELSTATUS;

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
double solver_model_find(SOLVERMODEL *&model,
						 unsigned int &bus_count,
						 BUSDATA *&bus,
						 unsigned int &branch_count,
						 BRANCHDATA *&branch);
int64 solver_model_apply(SOLVERMODEL *model,
						NR_SOLVER_STRUCT *&powerflow_values,
						NRSOLVERMODE &powerflow_type,
						NR_MESHFAULT_IMPEDANCE *&mesh_imped_values,
						bool *&bad_computations,
						int64 iterations);
double solver_model_get_maximum_distance();

#endif
