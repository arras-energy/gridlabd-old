/* 	File: apartment.h

	Copyright (C) 2020, Regents of the Leland Stanford Junior University
 */

#ifndef _G_APARTMENT_H
#define _G_APARTMENT_H

#include "gridlabd.h"

#define UAT_NONE         (0x0000)
#define UAT_DISHWASHER   (0x0001)
#define UAT_REFRIGERATOR (0x0002)
#define UAT_WASHER       (0x0004)
#define UAT_DRYER        (0x0008)
#define UAT_ALL          (0x000f)

class apartment : public gld_object {

public:
	GL_ATOMIC(int16,storeys);
	GL_ATOMIC(bool,circulation_is_indoor);
	GL_ATOMIC(int16,elevator_count);
	GL_ATOMIC(double,central_plant_capacity);
	GL_ATOMIC(double,central_plant_efficiency);
	typedef enum {CPT_NONE,CPT_STEAM,CPT_HOTWATER,CPT_FOURPIPE,CPT_FORCEDAIR} CENTRALPLANTTYPE;
	GL_ATOMIC(enumeration,central_plant_type);
	GL_ATOMIC(bool,central_plant_has_cooling);
	GL_ATOMIC(int16,central_washing_count);
	GL_ATOMIC(int16,parking_size);
	GL_ATOMIC(bool,parking_is_indoor);
	GL_ATOMIC(int16,parking_charger_count);
	GL_ATOMIC(double,parking_charger_capacity);
	GL_ATOMIC(int16,parking_charger_active);
	GL_ATOMIC(int16,unit_count);
	GL_ATOMIC(double,unit_size);
	GL_ATOMIC(double,unit_occupancy_factor);
	GL_ATOMIC(double,unit_temperature);
	GL_ATOMIC(double,unit_perimeter_conductance);
	GL_ATOMIC(double,unit_thermal_mass);
	GL_ATOMIC(double,unit_mass_conductance);
	typedef enum {UCT_NONE,UCT_CENTRAL,UCT_WINDOW_AC,UCT_UNITARY_AC,UCT_HEATPUMP} UNITCOOLINGTYPE;
	GL_ATOMIC(enumeration,unit_cooling_type);
	GL_ATOMIC(double,unit_cooling_setpoint); 
	typedef enum {UHT_NONE,UHT_CENTRAL,UHT_BASEBOARD,UHT_RESISTIVE,UHT_HEATPUMP} UNITHEATINGTYPE;
	GL_ATOMIC(enumeration,unit_heating_type);
	GL_ATOMIC(double,unit_heating_setpoint);
	typedef set UNITAPPLIANCETYPE;
	GL_ATOMIC(set,unit_appliance_types);

public:
	/* required implementations */
	apartment(MODULE *module);
	int create(void);
	int init(OBJECT *parent);
	TIMESTAMP precommit(TIMESTAMP t1);
	TIMESTAMP presync(TIMESTAMP t0, TIMESTAMP t1);
	TIMESTAMP sync(TIMESTAMP t0, TIMESTAMP t1);
	TIMESTAMP postsync(TIMESTAMP t0, TIMESTAMP t1);
	TIMESTAMP commit(TIMESTAMP t1, TIMESTAMP t2);

public:
	static CLASS *oclass;
	static g_assert *defaults;
};

#endif // _G_APARTMENT_H
