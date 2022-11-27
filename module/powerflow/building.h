// module/loads/building.h
// Copyright (C) 2022 Regents of the Leland Stanford Junior University

#ifndef _BUILDING_H
#define _BUILDING_H

#ifndef _POWERFLOW_H
#error "this header must be included by powerflow.h"
#endif

#include "../../third_party/tnt_126/tnt.h"
#include "../../third_party/jama125/jama_cholesky.h"

using namespace TNT;

class building : public load 
{

	typedef Array2D<double> Matrix;

private:

	Matrix A, B, C, D, x, u, y;
	bool thermal_flag, design_flag, control_flag, input_flag, output_flag;

public:

	// public data
	GL_ATOMIC(double,dt); // model timestep

	// state variables
	GL_ATOMIC(double,TA); // indoor air temperature (degC)
	GL_ATOMIC(double,TM); // building mass temperature (degC)
	GL_ATOMIC(double,M); // HVAC system mode (pu.QH)

	// thermal parameters
	GL_ATOMIC(double,UA); // conductance from interior air to outdoor air (W/K)
	GL_ATOMIC(double,CA); // heat capacity of indoor air volume (J/K)
	GL_ATOMIC(double,UI); // conductance from building mass to indoor air (W/K)
	GL_ATOMIC(double,CM); // heat capacity of building mass (J/K)
	GL_ATOMIC(double,UM); // conductance of building mass to outdoor air (W/K)

	// design parameters
	GL_ATOMIC(double,TH); // heating design temperature (degC)
	GL_ATOMIC(double,TC); // cooling design temperature (degC)
	GL_ATOMIC(double,DF); // over-design factor (pu)
	GL_ATOMIC(double,QH); // HVAC system capacity (W)
	GL_ATOMIC(double,QE); // enduse loads (W)
	GL_ATOMIC(double,QG); // gas load heat gains (W/kg)
	GL_ATOMIC(double,QO); // occupant heat gain (W/person)
	GL_ATOMIC(double,QV); // ventilation heat gain (W/person)
	GL_ATOMIC(double,SA); // mass area exposed to solar radiation (m^2)

	// control parameters
	GL_ATOMIC(double,K); // HVAC control gain w.r.t temperature

	// inputs
	GL_ATOMIC(double,TO); // outdoor air temperature (degC_
	GL_ATOMIC(double,EU); // fraction of end-uses active
	GL_ATOMIC(double,NG); // natural usage in kG
	GL_ATOMIC(double,NH); // number of occupants
	GL_ATOMIC(double,QS); // solar radiation (W/m^2)
	GL_ATOMIC(double,TS); // indoor temperature setpoint (degC)

	// outputs
	GL_ATOMIC(double,PZM); // constant impedance HVAC real power (W)
	GL_ATOMIC(double,PPM); // constant power HVAC real power (W)
	GL_ATOMIC(double,QPM); // constant power HVAC reactive power (VAr)
	GL_ATOMIC(double,PZE); // constant impedance end-use real power (W)
	GL_ATOMIC(double,PIE); // constant current end-use real power (W)
	GL_ATOMIC(double,PPE); // constant power end-use real power (W)
	GL_ATOMIC(double,QZE); // constant impedance end-use reactive power (VAr)
	GL_ATOMIC(double,QIE); // constant current end-use reactive power (VAr)
	GL_ATOMIC(double,QPE); // constant power end-use reactive power (VAr)
	GL_ATOMIC(double,PPH); // constant power ventilation real power (W/person)
	GL_ATOMIC(double,QPH); // constant power ventilation reactive power (VAr/person)
	
private:

	// private methods
	void update_thermal(bool flag_only=false);
	void update_design(bool flag_only=false);
	void update_control(bool flag_only=false);
	void update_input(bool flag_only=false);
	void update_output(bool flag_only=false);
	void update_equipment(void);

	// solvers
	Matrix solve_UL(Matrix &A, Matrix &b);

public:

	// event handlers
	building(MODULE *module);
	int create(void);
	int init(OBJECT *parent);
	TIMESTAMP precommit(TIMESTAMP t1, TIMESTAMP t2);
	TIMESTAMP presync(TIMESTAMP t0);
	TIMESTAMP sync(TIMESTAMP t0);
	int isa(char *classname);

public:

	// required data
	static CLASS *oclass;
	static building *defaults;
};

#endif // _BUILDING_H
