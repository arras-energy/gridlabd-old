// module/powerflow/building.cpp
// Copyright (C) 2022 Regents of the Leland Stanford Junior University

#include "powerflow.h"

#define TRACE_DEBUG false // enables debug output of updates
#define STATE_DEBUG false // enables debug output of state variables

#define DUMP(X) if ( STATE_DEBUG ) X.printf(#X ":\n")

EXPORT_CREATE(building);
EXPORT_INIT(building);
EXPORT_SYNC(building);
EXPORT_COMMIT(building);

CLASS *building::oclass = NULL;
building *building::defaults = NULL;

building::building(MODULE *module)
: load(module)
{
	if (oclass==NULL)
	{
		// register to receive notice for first top down. bottom up, and second top down synchronizations
		oclass = gld_class::create(module,"building",sizeof(building),
			PC_PRETOPDOWN|PC_BOTTOMUP|PC_POSTTOPDOWN|PC_UNSAFE_OVERRIDE_OMIT|PC_AUTOLOCK|PC_OBSERVER);
		if (oclass==NULL)
			throw "unable to register class building";
		else
			oclass->trl = TRL_PROVEN;

		defaults = this;
		if (gl_publish_variable(oclass,

			PT_INHERIT, "load",

#define PUBLISH(TYPE,NAME,UNIT,DESCRIPTION) PT_##TYPE, #NAME "[" UNIT "]", get_##NAME##_offset(), PT_DESCRIPTION, DESCRIPTION

			PUBLISH(double,dt,"s","timestep to use when modeling building response to inputs"), PT_DEFAULT, "1h",

			// state variables
			PUBLISH(double,TA,"degC","indoor air temperature"), PT_OUTPUT,
			PUBLISH(double,TM,"degC","building mass temperature"), PT_OUTPUT,
			PUBLISH(double,M,"pu","system mode per unit system capacity"), PT_OUTPUT,

			// thermal parameters
			PUBLISH(double,UA,"W/K","conductance from interior air to outdoor air"), PT_REQUIRED,
			PUBLISH(double,CA,"J/K","heat capacity of indoor air volume"), PT_REQUIRED,
			PUBLISH(double,UI,"W/K","conductance from building mass to indoor air"), PT_REQUIRED,
			PUBLISH(double,CM,"J/K","heat capacity of building mass"), PT_REQUIRED,
			PUBLISH(double,UM,"W/K","conductance of building mass to outdoor air"), PT_REQUIRED,

			// design parameters
			PUBLISH(double,TH,"degC","heating design temperature"), PT_REQUIRED,
			PUBLISH(double,TC,"degC","cooling design temperature"), PT_REQUIRED,
			PUBLISH(double,DF,"pu","system over-design factor"),
			PUBLISH(double,QH,"W","HVAC system capacity"), PT_REQUIRED, PT_OUTPUT,
			PUBLISH(double,QE,"W/unit","nomimal enduse load capacity"), PT_REQUIRED,
			PUBLISH(double,QG,"W/unit","natural gas heat per unit nominal enduse capacity"), PT_REQUIRED,
			PUBLISH(double,QO,"W/unit","heat gain per occupant"), PT_REQUIRED,
			PUBLISH(double,QV,"W/unit","ventilation gain per occupant"), PT_REQUIRED,
			PUBLISH(double,SA,"m^2","building mass area exposed to solar radiation"), PT_REQUIRED,

			// control parameters
			PUBLISH(double,K,"pu","HVAC mode proportional control gain w.r.t indoor temperature"),

			// inputs
			PUBLISH(double,TO,"degC","outdoor air temperature"),
			PUBLISH(double,EU,"unit","enduse load fraction"),
			PUBLISH(double,NG,"unit","natural gas demand"),
			PUBLISH(double,NH,"unit","building occupants"),
			PUBLISH(double,QS,"W/m^2","insolation"),
			PUBLISH(double,TS,"degC","thermostat setpoint"), PT_REQUIRED,

			// outputs
			PUBLISH(double,PZM,"pu","constant impedance HVAC real power per unit nominal capacity"), PT_OUTPUT,
			PUBLISH(double,PPM,"pu","constant power HVAC real power per unit nominal capacity"), PT_OUTPUT,
			PUBLISH(double,QPM,"pu","constant power HVAC reactive power per unit nominal capacity"), PT_OUTPUT,
			PUBLISH(double,PZE,"W/unit","constant impedance end-use real power nominal capacity"), PT_OUTPUT,
			PUBLISH(double,PIE,"W/unit","constant current end-use real power nominal capacity"), PT_OUTPUT,
			PUBLISH(double,PPE,"W/unit","constant power end-use real power nominal capacity"), PT_OUTPUT,
			PUBLISH(double,QZE,"VAr/unit","constant impedance end-use reactive power nominal capacity"), PT_OUTPUT,
			PUBLISH(double,QIE,"VAr/unit","constant current end-use reactive power nominal capacity"), PT_OUTPUT,
			PUBLISH(double,QPE,"VAr/unit","constant power end-use reactive power nominal capacity"), PT_OUTPUT,
			PUBLISH(double,PPH,"pu","constant power ventilation real power per unit nominal capacity"), PT_OUTPUT,
			PUBLISH(double,QPH,"pu","constant power ventilation reactive power per unit nominal capacity"), PT_OUTPUT,

			// meter
			PUBLISH(double,measured_real_power,"W","metered real power demand"), PT_OUTPUT,
			PUBLISH(double,measured_reactive_power,"VAr","metered reactive power demand"), PT_OUTPUT,
			PUBLISH(double,measured_real_energy,"Wh","cumulative metered real energy consumption"), PT_OUTPUT,
			PUBLISH(double,measured_real_energy_delta,"Wh","cumulative metered real energy interval consumption"), PT_OUTPUT,
			PUBLISH(double,measured_reactive_energy,"Wh","cumulative metered reactive energy consumption"), PT_OUTPUT,
			PUBLISH(double,measured_reactive_energy_delta,"Wh","cumulative metered reactive energy interval consumption"), PT_OUTPUT,
			PUBLISH(double,measured_energy_delta_timestep,"s","energy metering interval"), PT_OUTPUT,
			PUBLISH(double,measured_demand,"W","maximum metered real power interval demand"), PT_OUTPUT,
			PUBLISH(double,measured_demand_timestep,"s","maximum power metering interval"), PT_OUTPUT,

			NULL)<1) {
				throw "unable to publish building properties";
		}
	}
}

int building::isa(char *type)
{
	return strcmp(type,"building") || load::isa(type);
}

int building::create(void) 
{
	if ( TRACE_DEBUG ) debug("*** building load create");
	thermal_flag = design_flag = control_flag = input_flag = state_flag = output_flag = true;
	DF = 0.5;
	K = 1.0;
	measured_real_energy = measured_reactive_energy = measured_demand = 0.0;
	measured_energy_delta_timestep = measured_demand_timestep = 3600;

	return load::create(); /* return 1 on success, 0 on failure */
}

int building::init(OBJECT *parent)
{
	if ( TRACE_DEBUG ) debug("*** building load init");
	if ( dt <= 0 )
	{
		exception("timestep must be positive");
	}
	if ( measured_energy_delta_timestep < 0 )
	{
		exception("measured_energy_delta_timestep must be non-negative");
	}
	if ( measured_energy_delta_timestep < dt || (TIMESTAMP)measured_energy_delta_timestep % (TIMESTAMP)dt != 0 )
	{
		warning("measured_energy_delta_timestep must a multiple of dt");
	}
	if ( measured_demand_timestep < 0)
	{
		exception("measured_demand_timestep must be non-negative");	
	}
	if ( measured_demand_timestep < dt || (TIMESTAMP)measured_demand_timestep % (TIMESTAMP)dt != 0 )
	{
		warning("measured_demand_timestep must a multiple of dt");
	}
	if ( measured_demand_timestep < measured_energy_delta_timestep )
	{
		warning("measured_demand_timestep must be greater than or equal to measured_energy_delta_timestep");
	}

	// initialize working matrices
	A = Matrix(3,3,0.0);
	B = Matrix(3,6,0.0);
	C = Matrix(6,3,0.0);
	D = Matrix(6,6,0.0);
	x = Matrix(3,1,0.0);
	u = Matrix(6,1,0.0);
	y = Matrix(6,1,0.0);

	// setup state-space model
	update_equipment();
	update_thermal();
	update_design();
	update_control();
	update_input();

	// solve for initial steady-state
	DUMP(B);
	DUMP(u);
	Matrix b = -B%u;
	DUMP(b);
	DUMP(A);
	x = solve_UL(A,b);
	DUMP(x);
	TA = x[0][0];
	TM = x[1][0];
	M = x[2][0];

	update_output();
	
	return load::init(parent);
}

TIMESTAMP building::presync(TIMESTAMP t0)
{
	if ( TRACE_DEBUG ) debug("*** building load presync");
	if ( t0 % (int)dt == 0 )
	{
		update_input(true);
		update_state(true);
	}
	TIMESTAMP t2 = load::presync(t0);
	TIMESTAMP t1 = (TIMESTAMP)(((TIMESTAMP)(t0/dt)+1)*dt);
	return t1 < t2 ? -t1 : t2;
}

TIMESTAMP building::sync(TIMESTAMP t0)
{
	if ( TRACE_DEBUG ) debug("*** building load sync");
	update_output();
	return load::sync(t0);
}

TIMESTAMP building::postsync(TIMESTAMP t0)
{
	if ( TRACE_DEBUG ) debug("*** building load postsync");
	return load::postsync(t0);
}

TIMESTAMP building::commit(TIMESTAMP t0, TIMESTAMP t1)
{
	double ts = (double)(t0 - last_meter_update) / 3600;
	if ( ts > 0 && measured_energy_delta_timestep > 0 && t0 % (TIMESTAMP)measured_energy_delta_timestep == 0 )
	{
		complex *S = get_power_injection();

		// compute measured power
		measured_real_power = measured_reactive_power = 0.0;
		if ( phases&PHASE_A ) 
		{
			measured_real_power += S[0].r;
			measured_reactive_power += S[0].i;
		}
		if ( phases&PHASE_B )
		{
			measured_real_power += S[1].r;
			measured_reactive_power += S[1].i;
		}
		if ( phases&PHASE_C )
		{
			measured_real_power += S[2].r;
			measured_reactive_power += S[2].i;
		}

		// compute measured energy
		if ( last_meter_update > 0 )
		{
			measured_real_energy += measured_real_power * ts;
			measured_reactive_energy += measured_reactive_power * ts;
			measured_real_energy_delta = measured_real_energy - last_measured_energy.r;
			measured_reactive_energy_delta = measured_reactive_energy - last_measured_energy.i;
		}
		last_measured_energy = complex(measured_real_energy,measured_reactive_energy);

		// reset measured demand
		if ( measured_demand_timestep > 0 && t0 % (TIMESTAMP)measured_demand_timestep == 0 )
		{
			measured_demand = 0;
		}
		last_meter_update = t0;
	}

	// compute measured demand
	if ( measured_real_power > measured_demand )
	{
		measured_demand = measured_real_power;
	}
	return TS_NEVER;
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

	Matrix x(M,K,0.0);
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
	if ( TRACE_DEBUG ) debug("entering update_equipment(void)");
	if ( QH == 0.0 )
	{
		if ( TRACE_DEBUG ) debug("  autosizing equipment");
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
		update_thermal(true);
		update_output(true);
	}
	else
	{
		update_control();
		update_thermal();
		update_design();

		if ( TRACE_DEBUG ) debug("  checking equipment");
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
	if ( TRACE_DEBUG ) debug("exiting update_equipment(void)...");
}

void building::update_thermal(bool flag_only )
{
	if ( TRACE_DEBUG ) debug("entering update_thermal(%s)",flag_only?"true":"");
	if ( ! flag_only && thermal_flag )
	{
		if ( TRACE_DEBUG ) debug("  updating thermal");
		A[0][0] = -(UA+UI)/CA;
		A[0][1] = UI/CA;
		A[0][2] = QH/CA;
		A[1][0] = UI/CM;
		A[1][1] = -(UM+UI)/CM;
		DUMP(A);
		B[0][0] = UA/CA;
		B[0][1] = QE/CA;
		B[0][2] = QG/CA;
		B[0][3] = (QO+QV)/CA;
		B[1][0] = UM/CM;
		B[1][4] = SA/CM;
		DUMP(B);
		thermal_flag = false;
		update_state(true);
	}
	else if ( ! thermal_flag && flag_only ) 
	{
		if ( TRACE_DEBUG ) debug("  flagging thermal");
		update_state(true);
		thermal_flag = true;		
	}
	if ( TRACE_DEBUG ) debug("exiting update_thermal(%s)",flag_only?"true":"");
}

void building::update_design(bool flag_only )
{
	if ( TRACE_DEBUG ) debug("entering update_design(%s)",flag_only?"true":"");
	if ( ! flag_only && design_flag )
	{
		if ( TRACE_DEBUG ) debug("  updating design");
		A[0][2] = QH/CA;
		DUMP(A);
		B[1][4] = SA/CM;
		DUMP(B);
		update_state(true);
		design_flag = false;
	}
	else if ( ! design_flag && flag_only ) 
	{
		if ( TRACE_DEBUG ) debug("  flagging design");
		update_state(true);
		design_flag = true;
	}
	if ( TRACE_DEBUG ) debug("exiting update_design(%s)",flag_only?"true":"");
}

void building::update_control(bool flag_only )
{
	if ( TRACE_DEBUG ) debug("entering update_control(%s)",flag_only?"true":"");
	if ( ! flag_only && control_flag )
	{
		if ( TRACE_DEBUG ) debug("  updating control");
		A[2][0] = K;
		DUMP(A);
		B[2][5] = -K;
		DUMP(B);
		control_flag = false;
	}
	else if ( ! control_flag && flag_only )
	{
		if ( TRACE_DEBUG ) debug("  flagging control");
		control_flag = true;
	}
	if ( TRACE_DEBUG ) debug("exiting update_control(%s)",flag_only?"true":"");
}

void building::update_input(bool flag_only )
{
	if ( TRACE_DEBUG ) debug("entering update_input(%s)",flag_only?"true":"");
	if ( ! flag_only && input_flag )
	{
		if ( TRACE_DEBUG ) debug("  updating input");
		u[0][0] = TO;
		u[1][0] = EU;
		u[2][0] = NG;
		u[3][0] = NH;
		u[4][0] = QS;
		u[5][0] = TS;
		DUMP(u);
		update_state(true);
		update_output(true);
		input_flag = false;
	}
	else if ( ! input_flag && flag_only )
	{
		if ( TRACE_DEBUG ) debug("  flagging input");
		update_state(true);
		update_output(true);
		input_flag = true;
	}
	if ( TRACE_DEBUG ) debug("exiting update_input(%s)",flag_only?"true":"");
}

void building::update_state(bool flag_only )
{
	if ( TRACE_DEBUG ) debug("entering update_state(%s)",flag_only?"true":"");
	if ( ! flag_only && state_flag )
	{
		update_thermal();
		update_design();
		update_control();
		if ( TRACE_DEBUG ) debug("  updating state");
		x += A%x + B%u;
		DUMP(x);
		update_output(true);
		state_flag = false;
	}
	else if ( ! state_flag && flag_only ) 
	{
		if ( TRACE_DEBUG ) debug("  flagging state");
		update_output(true);
		state_flag = true;
	}
	if ( TRACE_DEBUG ) debug("exiting update_state(%s)",flag_only?"true":"");
}

void building::update_output(bool flag_only )
{
	if ( TRACE_DEBUG ) debug("entering update_output(%s)",flag_only?"true":"");
	if ( ! flag_only && output_flag )
	{
		update_input();
		update_state();
		if ( TRACE_DEBUG ) debug("  updating output");
		double QC = ( M<0 ? -QH : QH );
		C[0][2] = PZM*QC;
		C[2][2] = PPM*QC;
		C[5][2] = QPM*QC;
		DUMP(C);
		D[0][1] = PZE;
		D[1][1] = PIE;
		D[2][1] = PPE;
		D[2][3] = PPH*QH;
		D[3][1] = QZE;
		D[4][1] = QIE;
		D[5][1] = QPE;
		D[5][3] = QPH*QH;
		DUMP(D);
		y = C%x + D%y;
		DUMP(y);

		// copy y to load data
		int n_phases = (phases&PHASE_A?1:0) + (phases&PHASE_B?1:0) + (phases&PHASE_C?1:0);
		double P = ((y[0][0]*nominal_voltage+y[1][0])*nominal_voltage+y[2][0]);
		double Q = ((y[3][0]*nominal_voltage+y[4][0])*nominal_voltage+y[5][0]);
		complex S(P,Q);
		debug("M = %.2f pu, P = %.3f W, Q = %.3f VAr, S=%.3f%+.3fj",M,P*n_phases,Q*n_phases,S.r,S.i);
		double Sm = S.Mag();
		double Sn = Sm / n_phases;
		double Pz = complex(y[0][0],y[3][0]).Mag() / Sm;
		double Pi = complex(y[1][0],y[4][0]).Mag() / Sm;
		double Pp = complex(y[2][0],y[5][0]).Mag() / Sm;
		double Fz = (y[3][0] < 0 ? -1:1) * y[0][0] / Sm;
		double Fi = (y[4][0] < 0 ? -1:1) * y[1][0] / Sm;
		double Fp = (y[5][0] < 0 ? -1:1) * y[2][0] / Sm;
		if ( phases&PHASE_A ) 
		{
			base_power[0] = Sn;
			impedance_fraction[0] = Pz;			
			current_fraction[0] = Pi;
			power_fraction[0] = Pp;
			impedance_pf[0] = Fz;
			current_pf[0] = Fi;
			power_pf[0] = Fp;
		}
		if ( phases&PHASE_B ) 
		{
			base_power[1] = Sn;
			impedance_fraction[1] = Pz;
			current_fraction[1] = Pi;
			power_fraction[1] = Pp;
			impedance_pf[1] = Fz;
			current_pf[1] = Fi;
			power_pf[1] = Fp;
		}
		if ( phases&PHASE_C ) 
		{
			base_power[2] = Sn;
			impedance_fraction[2] = Pz;
			current_fraction[2] = Pi;
			power_fraction[2] = Pp;
			impedance_pf[2] = Fz;
			current_pf[2] = Fi;
			power_pf[2] = Fp;
		}

		output_flag = false;
	}
	else if ( ! output_flag && flag_only ) 
	{
		if ( TRACE_DEBUG ) debug("  flagging output");
		output_flag = true;
	}
	if ( TRACE_DEBUG ) debug("exiting update_output(%s)",flag_only?"true":"");
}

