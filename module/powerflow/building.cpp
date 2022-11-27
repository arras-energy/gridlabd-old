// module/loads/building.cpp
// Copyright (C) 2022 Regents of the Leland Stanford Junior University

#include "powerflow.h"

EXPORT_CREATE(building);
EXPORT_INIT(building);
EXPORT_SYNC(building);

CLASS *building::oclass = NULL;
building *building::defaults = NULL;

building::building(MODULE *module)
: load(module)
{
	if (oclass==NULL)
	{
		// register to receive notice for first top down. bottom up, and second top down synchronizations
		oclass = gld_class::create(module,"building",sizeof(building),
			PC_PRETOPDOWN|PC_BOTTOMUP|PC_POSTTOPDOWN|PC_UNSAFE_OVERRIDE_OMIT|PC_AUTOLOCK);
		if (oclass==NULL)
			throw "unable to register class building";
		else
			oclass->trl = TRL_PROVEN;

		defaults = this;
		if (gl_publish_variable(oclass,

			PT_INHERIT, "load",

			PT_double, "dt[s]", get_dt_offset(),
				PT_DEFAULT, "1h",
				PT_DESCRIPTION, "timestep to use when modeling building response to inputs",

#define PUBLISH(TYPE,NAME,UNIT,DESCRIPTION) PT_##TYPE, #NAME "[" UNIT "]", get_##NAME##_offset(), PT_DESCRIPTION, DESCRIPTION,

			// state variables
			PUBLISH(double,TA,"degC","indoor air temperature") PT_REQUIRED,
			PUBLISH(double,TM,"degC","building mass temperature") PT_REQUIRED,
			PUBLISH(double,M,"pu","system mode per unit system capacity") PT_REQUIRED,

			// thermal parameters
			PUBLISH(double,UA,"W/K","conductance from interior air to outdoor air") PT_REQUIRED,
			PUBLISH(double,CA,"J/K","heat capacity of indoor air volume") PT_REQUIRED,
			PUBLISH(double,UI,"W/K","conductance from building mass to indoor air") PT_REQUIRED,
			PUBLISH(double,CM,"J/K","heat capacity of building mass") PT_REQUIRED,
			PUBLISH(double,UM,"W/K","conductance of building mass to outdoor air") PT_REQUIRED,

			// design parameters
			PUBLISH(double,TH,"degC","heating design temperature") PT_REQUIRED,
			PUBLISH(double,TC,"degC","cooling design temperature") PT_REQUIRED,
			PUBLISH(double,DF,"pu","system over-design factor")
			PUBLISH(double,QH,"W","HVAC system capacity") PT_REQUIRED,
			PUBLISH(double,QE,"W/unit","nomimal enduse load capacity") PT_REQUIRED,
			PUBLISH(double,QG,"W/unit","natural gas heat per unit nominal enduse capacity") PT_REQUIRED,
			PUBLISH(double,QO,"W/unit","heat gain per occupant") PT_REQUIRED,
			PUBLISH(double,QV,"W/unit","ventilation gain per occupant") PT_REQUIRED,
			PUBLISH(double,SA,"m^2","building mass area exposed to solar radiation") PT_REQUIRED,

			// control parameters
			PUBLISH(double,K,"pu","HVAC mode proportional control gain w.r.t indoor temperature")

			// inputs
			PUBLISH(double,TO,"degC","outdoor air temperature")
			PUBLISH(double,EU,"unit","enduse load fraction")
			PUBLISH(double,NG,"unit","natural gas demand")
			PUBLISH(double,NH,"unit","building occupants")
			PUBLISH(double,QS,"W/m^2","insolation")
			PUBLISH(double,TS,"degC","thermostat setpoint") PT_REQUIRED,

			// outputs

			NULL)<1){
				char msg[256];
				snprintf(msg,sizeof(msg)-1, "unable to publish properties in %s",__FILE__);
				throw msg;
		}
	}
}

int building::isa(char *type)
{
	return strcmp(type,"building") || load::isa(type);
}

int building::create(void) 
{
	thermal_flag = design_flag = control_flag = input_flag = output_flag = true;
	DF = 0.5;
	K = 1.0;
	return load::create(); /* return 1 on success, 0 on failure */
}

int building::init(OBJECT *parent)
{
	if ( dt <= 0 )
	{
		exception("timestep must be positive");
	}

	// initialize working matrices
	A = Matrix(3,3,0.0);
	B = Matrix(3,6,0.0);
	C = Matrix(6,3,0.0);
	D = Matrix(6,6,0.0);
	x = Matrix(3,1,0.0);
	u = Matrix(6,1,0.0);
	y = Matrix(6,1,0.0);

	// equipment size
	update_thermal();
	update_design();
	update_control();
	update_equipment();
	update_input();
	
	return load::init(parent);
}

TIMESTAMP building::precommit(TIMESTAMP t1, TIMESTAMP t2)
{
	if ( t2 % (int)dt == 0 )
	{
		update_input();
		update_output();
	}
	return TS_NEVER;
}

TIMESTAMP building::presync(TIMESTAMP t0)
{
	TIMESTAMP t2 = load::presync(t0);
	TIMESTAMP t1 = (TIMESTAMP)(((TIMESTAMP)(t0/dt)+1)*dt);
	return t1 < t2 ? -t1 : t2;
}

TIMESTAMP building::sync(TIMESTAMP t0)
{
	return load::sync(t0);
}

building::Matrix building::solve_UL(building::Matrix  &A, building::Matrix  &b)
{
	int M = A.dim1();
	int N = A.dim2();
	int K = b.dim2();
	assert ( M == N ); // check that A is square
	assert ( M != b.dim1() ); // check for dimension match
	for ( int m = 1 ; m < M ; m++ )
	{
		for ( int n = m ; n < N ; n++ )
		{
			assert( A[m][n] == 0.0 );
		}
	}

	Matrix  x(M,K,0.0);
	for ( int k = 0 ; k < K ; k++ )
	{
		for ( int m = 0 ; m < M ; m++ ) // rows
		{
			x[m][k] = b[M-m-1][k];
			for ( int n = 0 ; n < m ; n++ ) // columns
			{
				x[m][k] -= A[M-m-1][n]*x[n][k];
			}
			x[m][k] /= A[M-m-1][m];
		}
	}
	return x;
}

void building::update_equipment(void)
{
	if ( QH == 0.0 )
	{
		// autosize heating system
		Matrix Ah(2,2,0.0);
		Ah.set(UI/CA,DF/CA,
			-(UM+UI)/CM,0.0);

		Matrix Bh(2,2,0.0);
		Bh.set(UA/CA,-(UA+UI)/CA,
			UM/CM,UI/CM);

		Matrix uh(2,1);
		uh.set(TH,TS);

		Matrix bh = -Bh % uh;
		Matrix xh = solve_UL(Ah,bh);

		// autosize cooling system
		Matrix Ac(2,2,0.0);
		Ac.set(UI/CA, DF/CA, 
			-(UM+UI)/CM, 0.0);

		Matrix Bc(2,3,0.0);
		Bc.set(UA/CA,-(UA+UI)/CA, 0.0,
			UM/CM, UI/CM, (QE+QG+QO+QV+1300*SA)/CM);

		Matrix uc(3,1,0.0);
		uc.set(TC,TS,1.0);

		Matrix bc = -Bc % uc;
		Matrix xc = solve_UL(Ac,bc);

		// autosize with larger system
		QH = max(-xc[1][0],xh[1][0]);
	}
	else
	{
		// check heating equipment size
		Matrix uh(6,1,0.0);
		uh[0][0] = TH;
		uh[5][0] = TS;
		Matrix bh(-B%uh);
		Matrix xh(solve_UL(A,bh));
		if ( xh[2][0] > DF )
		{
			warning("system is undersized for TH = %.1f degC, MH = %.2f",TH,xh[2][0]);
		}
		
		// check cooling equipment size
		Matrix uc(6,1,1.0);
		uc[0][0] = TC;
		uc[5][0] = TS;
		Matrix bc(-B%uc);
		Matrix xc(solve_UL(A,bc));
		if ( -xc[2][0] > DF )
		{
			warning("system is undersized for TC = %.1f degC, MC = %.2f",TC,xc[2][0]);
		}
		
	}
}

void building::update_thermal(bool flag_only)
{
	if ( ! flag_only && thermal_flag )
	{
		A[0][0] = -(UA+UI)/CA;
		A[0][1] = UI/CA;
		A[0][2] = QH/CA;
		A[1][0] = UI/CM;
		A[1][1] = -(UM+UI)/CM;
		B[0][0] = UA/CA;
		B[0][1] = QE/CA;
		B[0][2] = QG/CA;
		B[0][3] = (QO+QV)/CA;
		B[1][0] = UM/CM;
		B[1][4] = SA/CM;
	}
	else
	{
		thermal_flag |= flag_only;		
	}
}

void building::update_design(bool flag_only)
{
	if ( ! flag_only && design_flag )
	{
		A[0][2] = QH/CA;
		B[1][4] = SA/CM;
	}
	else
	{
		design_flag |= flag_only;
	}
}

void building::update_control(bool flag_only)
{
	if ( ! flag_only && control_flag )
	{
		A[2][0] = K;
		B[2][5] = -K;
	}
	else
	{
		control_flag |= flag_only;
	}
}

void building::update_input(bool flag_only)
{
	if ( ! flag_only && input_flag )
	{
		u[0][0] = TO;
		u[1][0] = EU;
		u[2][0] = NG;
		u[3][0] = NH;
		u[4][0] = QS;
		u[5][0] = TS;
	}
	else
	{
		input_flag |= flag_only;
	}
}

void building::update_output(bool flag_only)
{
	if ( ! flag_only && output_flag )
	{
		// TODO
	}
	else
	{
		output_flag |= flag_only;
	}
}

