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
  // Vec            T;             /* numerical solution will be stored here */
  // Vec            Q;
  // Vec           T_dot;
  // Vec           U;
  // Mat            A;
  // Mat            B_1;
  // Mat            B_2;

// Example problem
// T(t)/dt = r/100 * T(t) - r/4
// r = 3
// T_dot = B + A * T
// T(t0) = T(0) = 50
// T(t1) = 25.5
// t1 = 130.4 mins

  Vec           T_dot;
  Vec           A;
  Vec           B;
  Vec           T;


  Vec            Uex;           /* analytical (exact) solution will be stored here */
  PetscMPIInt    size;
  PetscInt       n = 1;
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
  PetscCall(TSSetIFunction(ts,NULL,IFunction,NULL));

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     Set initial conditions
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  PetscCall(VecCreate(PETSC_COMM_WORLD,&T));
  PetscCall(VecSetSizes(T,n,PETSC_DETERMINE));
  PetscCall(VecSetUp(T));
  PetscCall(VecGetArray(T,&u));
  u[0] = 50.0;
  PetscCall(VecRestoreArray(T,&u));
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


  if (PetscAbsReal(t-final_time)>100*PETSC_MACHINE_EPSILON) {
    PetscCall(PetscPrintf(PETSC_COMM_WORLD,"Note: There is a difference of %g between the prescribed final time %g and the actual final time.\n",(double)(final_time-t),(double)final_time));
  }
  PetscCall(VecDuplicate(T,&Uex));
  PetscCall(ExactSolution(t,Uex));
  PetscCall(VecView(Uex,0));
  PetscCall(VecView(T,0));

  PetscCall(VecAYPX(Uex,-1.0,T));
  PetscCall(VecNorm(Uex,NORM_2,&error));
  PetscCall(PetscPrintf(PETSC_COMM_WORLD,"Error at final time: %.2E\n",(double)error));

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     Free work space.  All PETSc objects should be destroyed when they are no longer needed.
     - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  PetscCall(VecDestroy(&T));
  PetscCall(VecDestroy(&Uex));
  PetscCall(TSDestroy(&ts));
  PetscCall(PetscViewerDestroy(&viewer));
  PetscCall(PetscFinalize());

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
