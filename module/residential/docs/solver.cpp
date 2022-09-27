static char help[] = "Small ODE to test TS accuracy.\n";

#include <petscts.h>
#include "petscksp.h"
#include <petscviewer.h>

/*
     Set the T vector at time t=0 sec
*/
static PetscErrorCode InitialConditions(Vec T, void *ctx)
{
  PetscScalar *t;
 

  PetscFunctionBegin;

  PetscCall(VecGetArray(T,&t));
  t[0] = 55.0;
  t[1] = 65.0;
  t[2] = 60.0;
  t[3] = 45.0;
  PetscCall(VecRestoreArray(T,&t));
  // PetscCall(VecView(T,0));
  PetscCall(PetscPrintf(PETSC_COMM_WORLD,"Initial Conditions t=0\n"));
  PetscFunctionReturn(0);
}


/*
     Set the T setpoint vector
*/
static PetscErrorCode SetTsetVector(Vec T, void *ctx)
{
  PetscScalar *t;
 

  PetscFunctionBegin;

  PetscCall(VecGetArray(T,&t));
  t[0] = 70.0;
  t[1] = 80.0;
  t[2] = 75.0;
  t[3] = 69.2116;
  PetscCall(VecRestoreArray(T,&t));
  PetscCall(PetscPrintf(PETSC_COMM_WORLD,"Set point Conditions t=INFINITY\n"));
  // PetscCall(VecView(T,0));
  PetscFunctionReturn(0);
}

/*
     Set the Q vector
*/
static PetscErrorCode SetQVector(Vec Q, void *ctx)
{
  PetscScalar *q;
  

  PetscFunctionBegin;
  
  PetscCall(VecGetArray(Q,&q));
  q[0] = 0;
  q[1] = 0;
  q[2] = 0;
  q[3] = 0;
  q[4] = 100;
  PetscCall(VecRestoreArray(Q,&q));
  PetscCall(PetscPrintf(PETSC_COMM_WORLD,"External Heat sources\n"));
  // PetscCall(VecView(Q,0));
  PetscFunctionReturn(0);
}

/*
     Set the A matrix
*/
static PetscErrorCode SetAMatrix(Mat A, PetscInt N, PetscInt M, void *ctx)
{
  
  PetscScalar values[N][N] = {
    {-3.7309, 0.3240 , 0.2793 , 2.0452},
    {1.2960 , -4.7029, 0.2793 , 2.0452},
    {2.9787 , 0.7447 , -9.1729, 3.4715},
    {0.0093 , 0.0023 , 0.0015 , -0.0167}
  };
  
  PetscFunctionBegin;
  
  // PetscCall(MatSetValues(A,N,&N,N,&N,values,INSERT_VALUES));
  // MatSetValues(Mat A,PetscInt m,const PetscInt idxm[],PetscInt n,const PetscInt idxn[],const PetscScalar values[],ADD_VALUES);
  
  
  PetscCall(MatSetValue(A,0,0,values[0][0],INSERT_VALUES));
  PetscCall(MatSetValue(A,0,1,values[0][1],INSERT_VALUES));
  PetscCall(MatSetValue(A,0,2,values[0][2],INSERT_VALUES));
  PetscCall(MatSetValue(A,0,3,values[0][3],INSERT_VALUES));

  PetscCall(MatSetValue(A,1,0,values[1][0],INSERT_VALUES));
  PetscCall(MatSetValue(A,1,1,values[1][1],INSERT_VALUES));
  PetscCall(MatSetValue(A,1,2,values[1][2],INSERT_VALUES));
  PetscCall(MatSetValue(A,1,3,values[1][3],INSERT_VALUES));
  
  PetscCall(MatSetValue(A,2,0,values[2][0],INSERT_VALUES));
  PetscCall(MatSetValue(A,2,1,values[2][1],INSERT_VALUES));
  PetscCall(MatSetValue(A,2,2,values[2][2],INSERT_VALUES));
  PetscCall(MatSetValue(A,2,3,values[2][3],INSERT_VALUES));
  
  PetscCall(MatSetValue(A,3,0,values[3][0],INSERT_VALUES));
  PetscCall(MatSetValue(A,3,1,values[3][1],INSERT_VALUES));
  PetscCall(MatSetValue(A,3,2,values[3][2],INSERT_VALUES));
  PetscCall(MatSetValue(A,3,3,values[3][3],INSERT_VALUES));

  PetscCall(MatAssemblyBegin(A,MAT_FINAL_ASSEMBLY));
  PetscCall(MatAssemblyEnd(A,MAT_FINAL_ASSEMBLY));
  PetscCall(PetscPrintf(PETSC_COMM_WORLD,"A Matrix\n"));
  // PetscCall(MatView(A,PETSC_VIEWER_STDOUT_WORLD));
  PetscFunctionReturn(0);
}

/*
     Set the B1 Matrix
*/
static PetscErrorCode SetB1Matrix(Mat B1, PetscInt N, PetscInt M, void *ctx)
{
  

  PetscScalar values[N][M] = {
    {0.0011, 0      , 0      , 0, 1.0824},
    {0     , 0.0045, 0      , 0, 1.0824},
    {0     , 0      , 0.0121, 0, 1.978},
    {0     , 0      , 0      , 0, 0.0036}
  };
  
  PetscFunctionBegin;
 
  // PetscCall(MatSetValues(A,N,idxm[],N,idxn[],values[],INSERT_VALUES));
  // MatSetValues(Mat A,PetscInt m,const PetscInt idxm[],PetscInt n,const PetscInt idxn[],const PetscScalar values[],ADD_VALUES);
  
  
  PetscCall(MatSetValue(B1,0,0,values[0][0],INSERT_VALUES));
  PetscCall(MatSetValue(B1,0,1,values[0][1],INSERT_VALUES));
  PetscCall(MatSetValue(B1,0,2,values[0][2],INSERT_VALUES));
  PetscCall(MatSetValue(B1,0,3,values[0][3],INSERT_VALUES));
  PetscCall(MatSetValue(B1,0,4,values[0][4],INSERT_VALUES));

  PetscCall(MatSetValue(B1,1,0,values[1][0],INSERT_VALUES));
  PetscCall(MatSetValue(B1,1,1,values[1][1],INSERT_VALUES));
  PetscCall(MatSetValue(B1,1,2,values[1][2],INSERT_VALUES));
  PetscCall(MatSetValue(B1,1,3,values[1][3],INSERT_VALUES));
  PetscCall(MatSetValue(B1,1,4,values[1][4],INSERT_VALUES));

  PetscCall(MatSetValue(B1,2,0,values[2][0],INSERT_VALUES));
  PetscCall(MatSetValue(B1,2,1,values[2][1],INSERT_VALUES));
  PetscCall(MatSetValue(B1,2,2,values[2][2],INSERT_VALUES));
  PetscCall(MatSetValue(B1,2,3,values[2][3],INSERT_VALUES));
  PetscCall(MatSetValue(B1,2,4,values[2][4],INSERT_VALUES));

  PetscCall(MatSetValue(B1,3,0,values[3][0],INSERT_VALUES));
  PetscCall(MatSetValue(B1,3,1,values[3][1],INSERT_VALUES));
  PetscCall(MatSetValue(B1,3,2,values[3][2],INSERT_VALUES));
  PetscCall(MatSetValue(B1,3,3,values[3][3],INSERT_VALUES));
  PetscCall(MatSetValue(B1,3,4,values[3][4],INSERT_VALUES));

  PetscCall(MatAssemblyBegin(B1,MAT_FINAL_ASSEMBLY));
  PetscCall(MatAssemblyEnd(B1,MAT_FINAL_ASSEMBLY));
  PetscCall(PetscPrintf(PETSC_COMM_WORLD,"B1 Matrix\n"));

  // PetscCall(MatView(B1,PETSC_VIEWER_STDOUT_WORLD));
  PetscFunctionReturn(0);
}

/*
     Set the B2 Matrix
*/
static PetscErrorCode SetB2Matrix(Mat B2,PetscInt N, PetscInt M,  void *ctx)
{
  

  PetscScalar values[N][M] = {
    {0.0011, 0      , 0     , 0},
    {0     , 0.0045 , 0     , 0},
    {0     , 0      , 0.0121, 0},
    {0     , 0      , 0     , 0}
  };
  
  PetscFunctionBegin;
  
  // PetscCall(MatSetValues(A,N,idxm[],N,idxn[],values[],INSERT_VALUES));
  // MatSetValues(Mat A,PetscInt m,const PetscInt idxm[],PetscInt n,const PetscInt idxn[],const PetscScalar values[],ADD_VALUES);
  
  
  PetscCall(MatSetValue(B2,0,0,values[0][0],INSERT_VALUES));
  PetscCall(MatSetValue(B2,0,1,values[0][1],INSERT_VALUES));
  PetscCall(MatSetValue(B2,0,2,values[0][2],INSERT_VALUES));
  PetscCall(MatSetValue(B2,0,3,values[0][3],INSERT_VALUES));

  PetscCall(MatSetValue(B2,1,0,values[1][0],INSERT_VALUES));
  PetscCall(MatSetValue(B2,1,1,values[1][1],INSERT_VALUES));
  PetscCall(MatSetValue(B2,1,2,values[1][2],INSERT_VALUES));
  PetscCall(MatSetValue(B2,1,3,values[1][3],INSERT_VALUES));

  PetscCall(MatSetValue(B2,2,0,values[2][0],INSERT_VALUES));
  PetscCall(MatSetValue(B2,2,1,values[2][1],INSERT_VALUES));
  PetscCall(MatSetValue(B2,2,2,values[2][2],INSERT_VALUES));
  PetscCall(MatSetValue(B2,2,3,values[2][3],INSERT_VALUES));

  PetscCall(MatSetValue(B2,3,0,values[3][0],INSERT_VALUES));
  PetscCall(MatSetValue(B2,3,1,values[3][1],INSERT_VALUES));
  PetscCall(MatSetValue(B2,3,2,values[3][2],INSERT_VALUES));
  PetscCall(MatSetValue(B2,3,3,values[3][3],INSERT_VALUES));
  
  PetscCall(MatAssemblyBegin(B2,MAT_FINAL_ASSEMBLY));
  PetscCall(MatAssemblyEnd(B2,MAT_FINAL_ASSEMBLY));
  PetscCall(PetscPrintf(PETSC_COMM_WORLD,"B2 Matrix\n"));

  // PetscCall(MatView(B2,PETSC_VIEWER_STDOUT_WORLD));
  PetscFunctionReturn(0);
}


static PetscErrorCode Monitor(TS ts, PetscInt step, PetscReal time, Vec u, void *ctx)
{
  PetscFunctionBegin;
  PetscCall(PetscPrintf(PETSC_COMM_WORLD, "Timestep %D: time = %g \n",step,(double)time));
  PetscCall(VecView(u,0));
  PetscFunctionReturn(0);
}

int main(int argc,char **argv)
{
  TS             ts;            /* ODE integrator */
  Vec            T;             /* numerical solution will be stored here */
  Vec            T_0;             /* numerical solution will be stored here */
  Vec            T_set;             /* numerical solution will be stored here */
  Vec            Q;
  Vec            T_dot;
  Vec            U;
  
  Mat            A;
  Mat            B1;
  Mat            B2;

  PetscMPIInt    size;
  PetscInt       n = 4;
  PetscScalar    *u;              /* Initial condition */
  PetscReal      t, final_time = 130.4, dt = 0.5, max_steps = 1000.0;
  PetscReal      error;
  TSAdapt        adapt;
  PetscViewer    viewer;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     Initialize variables
     - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  struct options
  {
    int N;
    int M;
    double X;
    double Y;
    double Z;
    double W;
    double F;
    double I;
    // 
    // Mechanical, Thermal, Geometric Properties from apartment document go here
    // 
  };
  



  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     Initialize program
     - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  PetscCall(PetscInitialize(&argc,&argv,(char*)0,help));
  PetscCallMPI(MPI_Comm_size(PETSC_COMM_WORLD,&size));
  PetscCheck(size == 1,PETSC_COMM_WORLD,PETSC_ERR_WRONG_MPI_SIZE,"Only for sequential runs");

  // PetscCall(PetscViewerCreate(PETSC_COMM_WORLD, &viewer));
  // PetscCall(PetscViewerSetType(viewer, PETSCVIEWERASCII));
  // PetscCall(PetscViewerFileSetMode(viewer,FILE_MODE_WRITE));
  // PetscCall(PetscViewerFileSetName(viewer,'test.txt'));
  
  // Initial Conditions
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     Set Vectors and Matricies
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  PetscInt   T_n = 4;
  PetscCall(VecCreate(PETSC_COMM_WORLD,&T_0));
  PetscCall(VecSetSizes(T_0,T_n,PETSC_DETERMINE));
  PetscCall(VecSetUp(T_0));
  PetscCall(InitialConditions(T_0,NULL));
  
  PetscCall(VecView(T_0,0));

  /*
    Tsetpoint Vector
    TODO: need to account for offset described in document
  */
  PetscCall(VecCreate(PETSC_COMM_WORLD,&T_set));
  PetscCall(VecSetSizes(T_set,T_n,PETSC_DETERMINE));
  PetscCall(VecSetUp(T_set));
  PetscCall(SetTsetVector(T_set,NULL));
  PetscCall(VecView(T_set,0));

  // Q Vector
  PetscInt   Q_n = 5;
  PetscCall(VecCreate(PETSC_COMM_WORLD,&Q));
  PetscCall(VecSetSizes(Q,Q_n,PETSC_DETERMINE));
  PetscCall(VecSetUp(Q));
  PetscCall(SetQVector(Q,NULL));
  PetscCall(VecView(Q,0));


  // A Matrix
  PetscInt A_n = 4;
  PetscInt A_m = 4;
  PetscCall(MatCreate(PETSC_COMM_WORLD, &A));
  PetscCall(MatSetSizes(A, PETSC_DECIDE, PETSC_DECIDE, A_n, A_m));
  PetscCall(MatSetFromOptions(A));
  PetscCall(MatSetUp(A));
  PetscCall(SetAMatrix(A,A_n,A_m,NULL));
  PetscCall(MatView(A,0));

  // B1 Matrix
  PetscInt B1_n = 4;
  PetscInt B1_m = 5;
  PetscCall(MatCreate(PETSC_COMM_WORLD, &B1));
  PetscCall(MatSetSizes(B1, PETSC_DECIDE, PETSC_DECIDE, B1_n, B1_m));
  PetscCall(MatSetFromOptions(B1));
  PetscCall(MatSetUp(B1));
  PetscCall(SetB1Matrix(B1,B1_n,B1_m,NULL));
  PetscCall(MatView(B1,0));

  // B2 Matrix
  PetscInt B2_n = 4;
  PetscInt B2_m = 4;
  PetscCall(MatCreate(PETSC_COMM_WORLD, &B2));
  PetscCall(MatSetSizes(B2, PETSC_DECIDE, PETSC_DECIDE, B2_n, B2_m));
  PetscCall(MatSetFromOptions(B2));
  PetscCall(MatSetUp(B2));
  PetscCall(SetB2Matrix(B2,B2_n,B2_m,NULL));
  PetscCall(MatView(B2,0));



////////////////////////////////////////////////////////////////////////////////
// Unneccasary, just checking for match with python script
 /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     Solve for Equilibrium temp at time Infinity
     - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  Vec B1Q;
  PetscCall(VecCreate(PETSC_COMM_WORLD,&B1Q));
  PetscCall(VecSetSizes(B1Q,T_n,PETSC_DETERMINE));
  PetscCall(VecSetUp(B1Q));

  PetscCall(MatMult(B1,Q,B1Q));
  PetscCall(VecView(B1Q,0));
 

  Vec x;
  PetscCall(VecCreate(PETSC_COMM_WORLD,&x));
  PetscCall(VecSetSizes(x,T_n,PETSC_DETERMINE));
  PetscCall(VecSetUp(x));

  Mat negA; 
  PetscCall(MatCreate(PETSC_COMM_WORLD, &negA));
  PetscCall(MatSetSizes(negA, PETSC_DECIDE, PETSC_DECIDE, A_n, A_m));
  PetscCall(MatSetFromOptions(negA));
  PetscCall(MatSetUp(negA));
  PetscCall(SetAMatrix(negA,A_n,A_m,NULL));
  PetscCall(MatScale(negA, -1.0));

  PetscCall(MatView(negA,0));
  PetscCall(VecView(B1Q,0));
//////////////////////////////////////////////////////////////////////////////
 /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     Create timestepping solver context
     - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  KSP ksp;
  PetscCall(KSPCreate(PETSC_COMM_WORLD,&ksp));
  PetscCall(KSPSetFromOptions(ksp));
  PetscCall(KSPSetOperators(ksp,negA,negA));
  PetscCall(KSPSetFromOptions(ksp));
  PetscCall(KSPSolve(ksp,B1Q,x));
  PetscCall(VecView(x,0));

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    Solve for T_Mass_equilibrium using the algebraic equation from document
     - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


 /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    Solve for u required at time inifity, where u is the HVAC systems power
     u = -B2 * (A*Tset+B1*q)
     - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  Vec ATset;
  PetscCall(VecCreate(PETSC_COMM_WORLD,&ATset));
  PetscCall(VecSetSizes(ATset,T_n,PETSC_DETERMINE));
  PetscCall(VecSetUp(ATset));

  PetscCall(MatMult(A,T_set,ATset));
  PetscCall(VecView(ATset,0));


  Vec ATsetB1Q;
  PetscCall(VecCreate(PETSC_COMM_WORLD,&ATsetB1Q));
  PetscCall(VecSetSizes(ATsetB1Q,T_n,PETSC_DETERMINE));
  PetscCall(VecSetUp(ATsetB1Q));


  PetscCall(VecWAXPY(ATsetB1Q,1.0,ATset,B1Q ));
  PetscCall(VecView(ATsetB1Q,0));

  Mat negB2; 
  PetscCall(MatCreate(PETSC_COMM_WORLD, &negB2));
  PetscCall(MatSetSizes(negB2, PETSC_DECIDE, PETSC_DECIDE, B2_n, B2_m));
  PetscCall(MatSetFromOptions(negB2));
  PetscCall(MatSetUp(negB2));
  PetscCall(SetB2Matrix(negB2,B2_n, B2_m,NULL));
  PetscCall(MatScale(negB2, -1.0));

  PetscCall(MatView(negB2,0));
  PetscCall(VecView(ATsetB1Q,0));


  PetscCall(KSPCreate(PETSC_COMM_WORLD,&ksp));
  PetscCall(KSPSetFromOptions(ksp));
  PetscCall(KSPSetOperators(ksp,negB2,negB2));
  PetscCall(KSPSetFromOptions(ksp));
  PetscCall(KSPSolve(ksp,ATsetB1Q,x));
  PetscCall(VecView(x,0));

 

  // Should have all vectors required to solve for Temperature in each zone at time t


  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     Create timestepping solver context
     - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  PetscCall(TSCreate(PETSC_COMM_WORLD,&ts));
  PetscCall(TSSetFromOptions(ts));
  PetscCall(TSSetType(ts,TSCN));
  PetscCall(TSSetEquationType(ts,TS_EQ_ODE_IMPLICIT));
  PetscCall(TSMonitorSet(ts, Monitor, 0, 0));

  



  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     Set ODE routines
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  PetscCall(TSSetProblemType(ts,TS_NONLINEAR));
  // PetscCall(TSSetIFunction(ts,NULL,IFunction,NULL));

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     Set initial conditions
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  

  PetscCall(InitialConditions(T,NULL));

  PetscCall(TSSetSolution(ts,T));

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     Set solver options
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  PetscCall(TSSetSaveTrajectory(ts));
  PetscCall(TSSetMaxTime(ts,final_time));
  // PetscCall(TSSetMaxSteps(ts,max_steps));
  PetscCall(TSSetExactFinalTime(ts,TS_EXACTFINALTIME_STEPOVER));
  PetscCall(TSSetTime(ts,0));
  PetscCall(TSSetTimeStep(ts,dt));
  
  // PetscCall(TSMonitorSet(ts,Monitor,&user,NULL);)
  /* The adaptive time step controller is forced to take constant time steps. */
  // PetscCall(TSGetAdapt(ts,&adapt));
  // PetscCall(TSAdaptSetType(adapt,TSADAPTNONE));

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     Run timestepping solver and compute error
     - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  PetscCall(TSSolve(ts,T));
  // PetscCall(TSStep(ts));
  PetscCall(TSGetSolveTime(ts,&t));

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     Free work space.  All PETSc objects should be destroyed when they are no longer needed.
     - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  PetscCall(VecDestroy(&T));
  PetscCall(TSDestroy(&ts));
  PetscCall(PetscViewerDestroy(&viewer));
  PetscCall(PetscFinalize());
  return 0;
}
