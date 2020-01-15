/* 	File: apartment.h

	Copyright (C) 2020, Regents of the Leland Stanford Junior University
 */

#ifndef _G_APARTMENT_H
#define _G_APARTMENT_H

#include "residential.h"

// system type local
#define SYSTEMTYPELOCAL set
#define STL_NONE 0x0000
#define STL_HEAT 0x0001
#define STL_COOL 0x0002
#define STL_BOTH 0x0003

// system type central
#define SYSTEMTYPECENTRAL set
#define STC_NONE 0x0000
#define STC_HEAT 0x0001
#define STC_COOL 0x0002
#define STC_BOTH 0x0003

// system type ventilation
#define SYSTEMTYPEVENTILATION enumeration
#define STV_NONE 0x0000
#define STV_CENTRAL 0x0001

// system type economizer
#define SYSTEMTYPEECONOMIZER enumeration
#define STE_NONE 0x0000
#define STE_DRYBULB 0x0001 // single temperature measurement only
#define STE_WEBBULB 0x0002 // single temperature and humidity measurement
#define STE_DIFFERENTIAL 0x0003 // dual temperature and humidity measurement

// unit appliance types
#define UNITAPPLIANCETYPE set
#define UAT_NONE         (0x0000)
#define UAT_DISHWASHER   (0x0001)
#define UAT_REFRIGERATOR (0x0002)
#define UAT_WASHER       (0x0004)
#define UAT_DRYER        (0x0008)
#define UAT_ALL          (0x000f)

class apartment : public gld_object {

public:

	// global variables
	static char1024 load_property;
	static double maximum_temperature_update;

public:

	// published variables
	GL_ATOMIC(int16,building_floors); // must be set by user
	GL_ATOMIC(double,building_floor_height);
	GL_ATOMIC(double,building_floor_depth);
	GL_ATOMIC(double,building_occupancy_factor);
	GL_ATOMIC(int16,building_units); // must be set by user
	GL_ATOMIC(double,building_outdoor_temperature);

	GL_ATOMIC(double,system_cooling_capacity);
	GL_ATOMIC(double,system_cooling_efficiency);
	GL_ATOMIC(double,system_heating_capacity);
	GL_ATOMIC(double,system_heating_efficiency);
	GL_ATOMIC(SYSTEMTYPELOCAL,system_type_local);
	GL_ATOMIC(SYSTEMTYPECENTRAL,system_type_central);
	GL_ATOMIC(SYSTEMTYPEVENTILATION,system_type_ventilation);
	GL_ATOMIC(SYSTEMTYPEECONOMIZER,system_type_economizer);
	GL_ATOMIC(double,system_cooling_air_temperature);
	GL_ATOMIC(double,system_heating_air_temperature);

	GL_ATOMIC(bool,core_is_indoor);
	GL_ATOMIC(int16,core_elevators);
	GL_ATOMIC(double,core_dimension_width);
	GL_ATOMIC(double,core_temperature_cooling_setpoint);
	GL_ATOMIC(double,core_temperature_heating_setpoint);
	GL_ATOMIC(int16,core_laundry_units);

	GL_ATOMIC(set,unit_appliance_types)
	GL_ATOMIC(double,unit_demand_lights);
	GL_ATOMIC(double,unit_demand_plugs);
	GL_ATOMIC(double,unit_demand_cooking);
	GL_ATOMIC(double,unit_demand_dishwasher);
	GL_ATOMIC(double,unit_demand_refrigerator);
	GL_ATOMIC(double,unit_demand_washer);
	GL_ATOMIC(double,unit_demand_dryer);
	GL_ATOMIC(double,unit_dimension_depth);
	GL_ATOMIC(double,unit_dimension_width);
	GL_ATOMIC(double,unit_cooling_capacity);
	GL_ATOMIC(double,unit_cooling_efficiency);
	GL_ATOMIC(double,unit_heating_capacity);
	GL_ATOMIC(double,unit_heating_efficiency);
	GL_ATOMIC(double,unit_temperature_cooling_setpoint);
	GL_ATOMIC(double,unit_temperature_heating_setpoint);

	GL_ATOMIC(double,unoccupied_temperature_cooling_setpoint);
	GL_ATOMIC(double,unoccupied_temperature_heating_setpoint);

	GL_ATOMIC(int16,parking_size);
	GL_ATOMIC(bool,parking_is_indoor);
	GL_ATOMIC(int16,parking_charger_count);
	GL_ATOMIC(double,parking_charger_capacity);
	GL_ATOMIC(int16,parking_charger_active);
	GL_ATOMIC(double,parking_demand_lights);
	GL_ATOMIC(double,parking_demand_ventilation);
	GL_ATOMIC(double,parking_demand_chargers);

	GL_ATOMIC(double,power_system);
	GL_ATOMIC(double,power_units);
	GL_ATOMIC(double,power_core);
	GL_ATOMIC(double,power_parking);
	GL_ATOMIC(double,power_total);

public:

	// required methods
	apartment(MODULE *module);
	int create(void);
	int init(OBJECT *parent);
	TIMESTAMP precommit(TIMESTAMP t1);

private:

	// thermal properties
	double U_OA;
	double U_OU;
	double U_OC;
	double U_OM;
	double U_AU;
	double U_AC;
	double U_AM;
	double U_UC;
	double U_UM;
	double U_CM;
	
	// zone capacitance
	double C_A;
	double C_U;
	double C_C;
	double C_M;
	
	// zone heat gains
	double Q_AS;
	double Q_AV;
	double Q_AE;
	double Q_US;
	double Q_CS;
	double Q_CV;
	
	// modes
	int mode; // central system mode
	int m[3]; // occupied, unoccupied, core zones modes

	// temperature (w.r.t. apartment setpoint temperature
	double Tout;
	matrix Tbal;

	// input constraints
	matrix u_min;
	matrix u_max;

	// internal model 
	matrix A, Ainv;
	matrix B1, B1inv;
	matrix B2, B2inv;

	// model inputs
	matrix q;
	matrix u;

	// state variables
	matrix T;
	matrix dT; 

public:

	// required support variables
	static CLASS *oclass;
	static apartment *defaults;
};

#endif // _G_APARTMENT_H
