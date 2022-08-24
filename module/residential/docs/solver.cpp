static char help[] = "Small ODE to test TS accuracy.\n";

/*
  The ODE
                  u1_t = cos(t),
                  u2_t = sin(u2)
  with analytical solution
                  u1(t) = sin(t),
                  u2(t) = 2 * atan(exp(t) * tan(0.5))
  is used to test the accuracy of TS schemes.
*/

#include <petscts.h>
#include "petscksp.h"
#include <petscviewer.h>

// /*
//      Defines the ODE passed to the ODE solver in explicit form: U_t = F(U)
// */
// static PetscErrorCode RHSFunction(TS ts, PetscReal t, Vec U, Vec F, void *s)
// {
//   PetscScalar       *f;
//   const PetscScalar *u;

//   PetscFunctionBegin;
//   PetscCall(VecGetArrayRead(U,&u));
//   PetscCall(VecGetArray(F,&f));

//   f[0] = PetscCosReal(t);
//   f[1] = PetscSinReal(u[1]);

//   PetscCall(VecRestoreArrayRead(U,&u));
//   PetscCall(VecRestoreArray(F,&f));
//   PetscFunctionReturn(0);
// }
/*
     Set the function to compute F(t,U,U_t) where F() = 0 is the DAE to be solved.
*/
static PetscErrorCode IFunction(TS ts, PetscReal t, Vec U, Vec U_t, Vec F, void *ctx)
{
  const PetscScalar *u, *u_t;
  PetscScalar       *f;
  const double r = 3.0;

  PetscFunctionBegin;
  PetscCall(VecGetArrayRead(U,&u));
  PetscCall(VecGetArrayRead(U_t,&u_t));
  PetscCall(VecGetArray(F,&f));

  f[0] = u_t[0] + (r / 100.0) * u[0] - (r / 4.0);



  PetscCall(VecRestoreArrayRead(U,&u));
  PetscCall(VecRestoreArrayRead(U_t,&u_t));
  PetscCall(VecRestoreArray(F,&f));
  // PetscCall(VecView(U,0));
  // PetscCall(VecView(U_t,0));
  // PetscCall(VecView(F,0));
  PetscFunctionReturn(0);
}
/*
     Set the function to compute F(t,U,U_t) where F() = 0 is the DAE to be solved.
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
  PetscCall(VecView(T,0));
  PetscFunctionReturn(0);
}
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
  PetscCall(VecView(T,0));
  PetscFunctionReturn(0);
}
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
  PetscCall(VecView(Q,0));
  PetscFunctionReturn(0);
}
/*
     Set the function to compute F(t,U,U_t) where F() = 0 is the DAE to be solved.
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
  PetscCall(MatView(A,PETSC_VIEWER_STDOUT_WORLD));
  PetscFunctionReturn(0);
}
/*
     Set the function to compute F(t,U,U_t) where F() = 0 is the DAE to be solved.
*/
static PetscErrorCode SetB1Matrix(Mat B1, PetscInt N, PetscInt M, void *ctx)
{
  

  PetscScalar values[N][M] = {
    {0.0011, 0      , 0      , 0, 1.0824},
    {0     , -4.7029, 0      , 0, 1.0824},
    {0     , 0      , -9.1729, 0, .9780},
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
  PetscCall(MatView(B1,PETSC_VIEWER_STDOUT_WORLD));
  PetscFunctionReturn(0);
}
static PetscErrorCode SetB2Matrix(Mat B2,PetscInt N, PetscInt M,  void *ctx)
{
  

  PetscScalar values[N][M] = {
    {0.0011, 0      , 0     },
    {0     , 0.0045 , 0     },
    {0     , 0      , 0.0121},
    {0     , 0      , 0     }
  };
  
  PetscFunctionBegin;
  
  // PetscCall(MatSetValues(A,N,idxm[],N,idxn[],values[],INSERT_VALUES));
  // MatSetValues(Mat A,PetscInt m,const PetscInt idxm[],PetscInt n,const PetscInt idxn[],const PetscScalar values[],ADD_VALUES);
  
  
  PetscCall(MatSetValue(B2,0,0,values[0][0],INSERT_VALUES));
  PetscCall(MatSetValue(B2,0,1,values[0][1],INSERT_VALUES));
  PetscCall(MatSetValue(B2,0,2,values[0][2],INSERT_VALUES));

  PetscCall(MatSetValue(B2,1,0,values[1][0],INSERT_VALUES));
  PetscCall(MatSetValue(B2,1,1,values[1][1],INSERT_VALUES));
  PetscCall(MatSetValue(B2,1,2,values[1][2],INSERT_VALUES));

  PetscCall(MatSetValue(B2,2,0,values[2][0],INSERT_VALUES));
  PetscCall(MatSetValue(B2,2,1,values[2][1],INSERT_VALUES));
  PetscCall(MatSetValue(B2,2,2,values[2][2],INSERT_VALUES));

  PetscCall(MatSetValue(B2,3,0,values[3][0],INSERT_VALUES));
  PetscCall(MatSetValue(B2,3,1,values[3][1],INSERT_VALUES));
  PetscCall(MatSetValue(B2,3,2,values[3][2],INSERT_VALUES));

  PetscCall(MatAssemblyBegin(B2,MAT_FINAL_ASSEMBLY));
  PetscCall(MatAssemblyEnd(B2,MAT_FINAL_ASSEMBLY));
  PetscCall(MatView(B2,PETSC_VIEWER_STDOUT_WORLD));
  PetscFunctionReturn(0);
}

/*
     Defines the exact solution.
*/
static PetscErrorCode ExactSolution(PetscReal t, Vec U)
{
  PetscScalar       *u;
  const double r = 3.0;

  PetscFunctionBegin;
  PetscCall(VecGetArray(U,&u));

  u[0] = 25.0 + (50.0 -25.0) * PetscExpReal(-(r*t)/100.0);

  PetscCall(VecRestoreArray(U,&u));
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
  Vec            b;
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
  PetscInt   T_n = 4;
  PetscCall(VecCreate(PETSC_COMM_WORLD,&T_0));
  PetscCall(VecSetSizes(T_0,T_n,PETSC_DETERMINE));
  PetscCall(VecSetUp(T_0));
  PetscCall(InitialConditions(T_0,NULL));
  PetscCall(VecView(T_0,0));

  // Teq Vector
  PetscInt   T_n = 4;
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
  PetscInt B2_m = 3;
  PetscCall(MatCreate(PETSC_COMM_WORLD, &B2));
  PetscCall(MatSetSizes(B2, PETSC_DECIDE, PETSC_DECIDE, B2_n, B2_m));
  PetscCall(MatSetFromOptions(B2));
  PetscCall(MatSetUp(B2));
  PetscCall(SetB2Matrix(B2,B2_n,B2_m,NULL));
  PetscCall(MatView(B2,0));





  Vec L;
  KSP ksp;
  PetscCall(VecCreate(PETSC_COMM_WORLD,&U));
  PetscCall(VecSetSizes(U,T_n,PETSC_DETERMINE));
  PetscCall(VecSetUp(U));

  PetscCall(VecCreate(PETSC_COMM_WORLD,&b));
  PetscCall(VecSetSizes(b,T_n,PETSC_DETERMINE));
  PetscCall(VecSetUp(b));

  // PetscCall(VecCreate(PETSC_COMM_WORLD,&L));
  // PetscCall(VecSetSizes(L,4,PETSC_DETERMINE));
  // PetscCall(VecSetUp(L));
  // Solve for T eq


  // A * T_set + B1 * Q

  PetscCall(KSPCreate(PETSC_COMM_WORLD,&ksp));
  PetscCall(KSPSetFromOptions(ksp));
  PetscCall(KSPSetOperators(ksp,B2,B2));
  PetscCall(KSPSolve(ksp,b,U));


  PetscCall(MatMult(B1,Q,L));
  PetscCall(VecView(L,0));




  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     Create timestepping solver context
     - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  PetscCall(TSCreate(PETSC_COMM_WORLD,&ts));
  PetscCall(TSSetFromOptions(ts));
  PetscCall(TSSetType(ts,TSCN));
  PetscCall(TSSetEquationType(ts,TS_EQ_ODE_IMPLICIT));
  PetscCall(TSMonitorSet(ts, Monitor, 0, 0));

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     Set Vectors and Matricies
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */




  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     Set ODE routines
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  PetscCall(TSSetProblemType(ts,TS_NONLINEAR));
  PetscCall(TSSetIFunction(ts,NULL,IFunction,NULL));

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

/*TEST

    test:
      suffix: 3bs
      args: -ts_type rk -ts_rk_type 3bs
      requires: !single

    test:
      suffix: 5bs
      args: -ts_type rk -ts_rk_type 5bs
      requires: !single

    test:
      suffix: 5dp
      args: -ts_type rk -ts_rk_type 5dp
      requires: !single

    test:
      suffix: 6vr
      args: -ts_type rk -ts_rk_type 6vr
      requires: !single

    test:
      suffix: 7vr
      args: -ts_type rk -ts_rk_type 7vr
      requires: !single

    test:
      suffix: 8vr
      args: -ts_type rk -ts_rk_type 8vr
      requires: !single

TEST*/
