// module/powerflow/building.h
// Copyright (C) 2022 Regents of the Leland Stanford Junior University

#ifndef _BUILDING_H
#define _BUILDING_H

#ifndef _POWERFLOW_H
#error "this header must be included by powerflow.h"
#endif

#include "../../third_party/tnt_126/tnt.h"
#include "../../third_party/jama125/jama_cholesky.h"

using namespace TNT;

#define TS_DAY0 3*86400

class input 
{
public:

	typedef struct s_loadshape
	{
		const char *building_type;
		const char *fuel;
		double value[4*2*24]; // season (0-3), daytype (0-1), hour (0-23)
		struct s_loadshape *next;
	} LOADSHAPE;

private:

	static LOADSHAPE *first_shape;
	static char *buffer;
	static TIMESTAMP last_timestamp;
	static unsigned int last_offset;

public:

	LOADSHAPE *shape;

public:

	LOADSHAPE *get_loadshape(const char *building_type, const char *fuel);
	double get_load(const TIMESTAMP timestamp, const double scale=1.0);

public:

	input(const char *filename);
	~input(void);
};

class building : public load 
{

	typedef Array2D<double> Matrix;

	typedef struct s_occupancy {
		unsigned int start, stop;
		double occupied, unoccupied;
	} OCCUPANCY;

private:

	Matrix A, B, C, D, x, u, y; // model data
	bool thermal_flag, design_flag, control_flag, input_flag, state_flag, output_flag; // update flags
	TIMESTAMP last_meter_update; // time of last meter update
	complex last_measured_energy; // energy at last meter commit
	complex prev_measured_energy; // energy at previous meter postsync iteration
	double this_measured_demand; // measured demand for this interval
	int ts_offset; // offset to use for interval checks

public:

	static char1024 electric_enduses_filename;
	static char1024 gas_enduses_filename;
	static char1024 occupancies_filename;
	static char1024 setpoints_filename;
	static char1024 building_defaults_filename;
	static char1024 building_loadshapes_filename;
	static char1024 building_occupancy_filename;
	static bool dynamic_solver;
	gld_property *temperature;
	gld_property *solar;
	input *electric_load;
	input *gas_load;
	OCCUPANCY occupancy_schedule[2]; // weekday, weekend occupancy fraction per unit occupancy

public:

	// model data
	GL_ATOMIC(double,dt); // model timestep

	// state variables
	GL_ATOMIC(double,TA); // indoor air temperature (degC)
	GL_ATOMIC(double,TM); // building mass temperature (degC)
	GL_ATOMIC(double,M); // HVAC system mode (pu.QH)
	GL_ATOMIC(double,ES); // stored energy

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
	GL_ATOMIC(double,floor_area); // floor area (m^2)
	GL_ATOMIC(double,electric_gain_fraction); // fraction of electric end-use that go to building heat gains
	GL_ATOMIC(double,gas_gain_fraction); // fraction of gas end-use that go to building heat gains
	GL_ATOMIC(double,electrification_fraction); // fraction of gas end-use that are electrified
	GL_ATOMIC(double,electrification_efficiency); // performance of electric end-use relative to gas enduse
	GL_ATOMIC(bool,electric_heat); // flag whether heating is from electric
	GL_ATOMIC(double,PV); // area of photovoltaic panels (m^2)
	GL_ATOMIC(double,BS); // battery capacity (Wh)
	GL_ATOMIC(double,PX); // maximum export power (W)

	// control parameters
	GL_ATOMIC(double,K); // HVAC control gain w.r.t temperature

	// inputs
	GL_STRING(char32,building_type); // building type (used to lookup data)
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

	// meter
	GL_ATOMIC(double,measured_real_power);
	GL_ATOMIC(double,measured_reactive_power);
	GL_ATOMIC(double,measured_real_energy);
	GL_ATOMIC(double,measured_real_energy_delta);
	GL_ATOMIC(double,measured_reactive_energy);
	GL_ATOMIC(double,measured_reactive_energy_delta);
	GL_ATOMIC(double,measured_energy_delta_timestep);
	GL_ATOMIC(double,measured_demand);
	GL_ATOMIC(double,measured_demand_timestep);

	// schedules
	GL_ATOMIC(int32,occupancy);
	GL_STRING(char256,temperature_source);
	GL_STRING(char256,solar_source);
	GL_STRING(char256,cooling_design);
	GL_STRING(char256,heating_design);

private:

	// private methods (flag_only=true to mark for update, flag_only=false to perform update if marked)
	void update_thermal(bool flag_only=false);
	void update_design(bool flag_only=false);
	void update_control(bool flag_only=false);
	void update_input(bool flag_only=false);
	void update_state(bool flag_only=false);
	void update_output(bool flag_only=false);
	void update_equipment(void); // only called when QH needs to be checked (or autosized if QH=0)

	// loaders
	int load_defaults(void);
	int load_loadshapes(void);
	int load_occupancy(void);
	double get_occupancy(TIMESTAMP timestamp);

	// solvers
	Matrix solve_UL(Matrix &A, Matrix &b);

public:

	// event handlers
	building(MODULE *module);
	int create(void);
	int init(OBJECT *parent);
	TIMESTAMP presync(TIMESTAMP t0);
	TIMESTAMP sync(TIMESTAMP t0);
	TIMESTAMP postsync(TIMESTAMP t0);
	TIMESTAMP commit(TIMESTAMP t0, TIMESTAMP t1);
	int isa(char *classname);

public:

	// required data
	static CLASS *oclass;
	static building *defaults;
};

#endif // _BUILDING_H
