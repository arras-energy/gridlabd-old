/** $Id: battery.h,v 1.0 2008/07/18
	Copyright (C) 2008 Battelle Memorial Institute
	@file battery.h
	@addtogroup battery

 @{  
 **/

#ifndef _BATTERY_H
#define _BATTERY_H

#ifndef _GENERATORS_H
#error "this header must be included by generators.h only"
#endif

EXPORT STATUS preupdate_battery(OBJECT *obj,TIMESTAMP t0, unsigned int64 delta_time);
EXPORT SIMULATIONMODE interupdate_battery(OBJECT *obj, unsigned int64 delta_time, unsigned long dt, unsigned int iteration_count_val);
EXPORT STATUS postupdate_battery(OBJECT *obj, complex *useful_value, unsigned int mode_pass);


class battery : public energy_storage
{
private:
	TIMESTAMP prev_time;
	int first_time_step;
	int number_of_phases_out;
	int prev_state; //1 is charging, 0 is nothing, -1 is discharging

	/* TODO: put private variables here */
	double n_series; //the number of cells in series
	double internal_battery_load; //the power out of the battery on the source side of the internal resistance
	double r_in; // the internal resistance of the battery as a function of soc
	double v_oc; // the open circuit voltage as a function of soc
	double v_t; // the terminal voltage as a function of soc and battery load.
	double p_br;
	//complex AMx[3][3];//generator impedance matrix

	bool deltamode_inclusive;	   //Boolean for deltamode calls - pulled from object flags
	bool first_run;
	bool enableDelta; // is true only if battery is use_internal_battery_model, and its parent inverter is FQM_CONSTANT_PQ mode


protected:
	/* TODO: put unpublished but inherited variables */
public:
	/* TODO: put published variables here */
	set phases;	/**< device phases (see PHASE codes) */
	double power_transferred;
	enum GENERATOR_MODE {
		GM_UNKNOWN=0,
		GM_CONSTANT_V=1, 
		GM_CONSTANT_PQ, 
		GM_CONSTANT_PF, 
		GM_SUPPLY_DRIVEN, 
		GM_POWER_DRIVEN, 
		GM_VOLTAGE_CONTROLLED, 
		GM_POWER_VOLTAGE_HYBRID
	};
	enumeration gen_mode_v;  //operating mode of the generator


	//note battery panel will always operate under the SUPPLY_DRIVEN generator mode
	enum GENERATOR_STATUS {
		OFFLINE=1, 
		ONLINE=2
	};
	enumeration gen_status_v;

	enum ADDITIONAL_CONTROLS {
		AC_NONE=0,
		AC_LINEAR_TEMPERATURE=1
	};
	enumeration additional_controls;

	enum POWER_TYPE {
		DC=1, 
		AC=2
	};
	enumeration power_type_v;

	enum RFB_SIZE {
		SMALL=1, 
		MED_COMMERCIAL, 
		MED_HIGH_ENERGY, 
		LARGE, 
		HOUSEHOLD
	};
	enumeration rfb_size_v;

	enum BATTERY_STATE {
		BS_WAITING = 0,
		BS_CHARGING = 1,
		BS_DISCHARGING = 2,
		BS_FULL = 3,
		BS_EMPTY = 4,
		BS_CONFLICTED = 5,
	};
	enumeration battery_state;

		
	complex *pCircuit_V;		//< pointer to the three voltages on three lines
	complex *pLine_I;			//< pointer to the three current on three lines
	complex *pLine12;			//< used in triplex metering
	complex *pPower;
	double power_set_high;
	double power_set_low;
	double power_set_high_highT;
	double power_set_low_highT;
	double voltage_set_high;
	double voltage_set_low;
	double deadband;
	double check_power;
	double pf;
	//double lockout_time;
	//int lockout_flag;
	//TIMESTAMP next_time;
	complex last_current[3];
	double no_of_cycles;
	bool Iteration_Toggle;			// "Off" iteration tracker
	double *pTout;
	double *pSolar;
	double parasitic_power_draw;
	double high_temperature;
	double low_temperature;
	double midpoint_temperature;
	double sensitivity;
	double check_power_high;
	double check_power_low;
	double B_scheduled_power;

	//Internal battery model parameters
	bool use_internal_battery_model;
	enum {LI_ION=1, LEAD_ACID};
	enumeration battery_type;
	double soc; //state of charge of the battery
	double bat_load; //current load of the battery
	double *pSoc;
	double *pBatteryLoad;
	double last_bat_load;
	double b_soc_reserve;
	double *pSocReserve;
	TIMESTAMP state_change_time;


	//battery module parameters
	double v_max; //the maximum DC voltage of the battery in V
	double p_max; // the rated DC power the battery can supply or draw in W
	double *pRatedPower;
	double e_max; // the battery's internal capacity in Wh
	double eta_rt; // the roundtrip efficiency of the battery at rated power.
	enumeration *pControlMode; // parent inverter control mode
	double *peff; // parent inverter efficiency
	complex *inverter_VA_Out; // inverter AC power output

	double deltat; // delta mode time interval in second
	unsigned int64 state_change_time_delta;
	double pre_soc; //store the soc value during iterations
	double Pout_delta; //Power output from parent inverter

public:
	/* required implementations */
	battery(MODULE *module);
	int create(void);
	int init(OBJECT *parent);
	//double timestamp_to_hours(TIMESTAMP t);
	TIMESTAMP rfb_event_time(TIMESTAMP t0, complex power, double e);
	TIMESTAMP presync(TIMESTAMP t0, TIMESTAMP t1);
	TIMESTAMP sync(TIMESTAMP t0, TIMESTAMP t1);
	TIMESTAMP postsync(TIMESTAMP t0, TIMESTAMP t1);

	STATUS pre_deltaupdate(TIMESTAMP t0, unsigned int64 delta_time);
	SIMULATIONMODE inter_deltaupdate(unsigned int64 delta_time, unsigned long dt, unsigned int iteration_count_val);
	STATUS post_deltaupdate(complex *useful_value, unsigned int mode_pass);
	void update_soc(unsigned int64 delta_time);
	double check_state_change_time_delta(unsigned int64 delta_time, unsigned long dt);

	double calculate_efficiency(complex voltage, complex current);
	complex *get_complex(OBJECT *obj, const char *name);
	complex calculate_v_terminal(complex v, complex i);

	void fetch_double(double **prop, const char *name, OBJECT *parent);
	void fetch_enumeration(enumeration **prop, const char *name, OBJECT *parent);
	void fetch_complex(complex **prop, const char *name, OBJECT *parent);

public:
	static CLASS *oclass;
	static battery *defaults;
#ifdef OPTIONAL
	static CLASS *pclass; /**< defines the parent class */
	TIMESTAMP plc(TIMESTAMP t0, TIMESTAMP t1); /**< defines the default PLC code */
#endif
};

#endif

/**@}*/
