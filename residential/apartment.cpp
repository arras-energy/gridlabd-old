/*	File: apartment.cpp

	Copyright (C) 2020, Regents of the Leland Stanford Junior University
*/

#include "residential.h"

using namespace arma;

EXPORT_CREATE(apartment);
EXPORT_INIT(apartment);
EXPORT_PRECOMMIT(apartment);

CLASS *apartment::oclass = NULL;
apartment *apartment::defaults = NULL;

char1024 apartment::load_property = "base_power";

#define PASSCONFIG PC_AUTOLOCK

apartment::apartment(MODULE *module)
{
	if (oclass==NULL)
	{
		// register to receive notice for first top down. bottom up, and second top down synchronizations
		oclass = gld_class::create(module,"apartment",sizeof(apartment),PASSCONFIG);
		if (oclass==NULL)
			throw "unable to register class apartment";
		else
			oclass->trl = TRL_PROVEN;

		defaults = this;
		if (gl_publish_variable(oclass,
			PT_int16,"storeys",get_storeys_offset(), PT_DEFAULT,0, PT_DESCRIPTION,"number of floors",
			PT_bool,"circulation_is_indoor",get_circulation_is_indoor_offset(), PT_DEFAULT,FALSE, PT_DESCRIPTION,"circulation is indoors",
			PT_int16,"circulation_elevator_count",get_circulation_elevator_count_offset(), PT_DEFAULT,0, PT_DESCRIPTION,"number of elevators available in the building",
			PT_double,"central_plant_capacity[Btu/h]",get_central_plant_capacity_offset(), PT_DEFAULT,0, PT_DESCRIPTION,"total heating/cooling capacity of the central plant, if any",
			PT_double,"central_plant_efficiency[pu]",get_central_plant_efficiency_offset(), PT_DEFAULT,0, PT_DESCRIPTION,"total heating/cooling efficiency of the central plant, if any",
			PT_enumeration,"central_plant_type",get_central_plant_type_offset(), PT_DEFAULT,0, PT_DESCRIPTION,"the type of central in use, if any",
				PT_KEYWORD,"NONE",(enumeration)CPT_NONE,
				PT_KEYWORD,"STEAM",(enumeration)CPT_STEAM,
				PT_KEYWORD,"HOTWATER",(enumeration)CPT_HOTWATER,
				PT_KEYWORD,"FOURPIPE",(enumeration)CPT_FOURPIPE,
				PT_KEYWORD,"FORCEDAIR",(enumeration)CPT_FORCEDAIR,
			PT_bool,"central_plant_has_cooling",get_central_plant_has_cooling_offset(), PT_DEFAULT,0, PT_DESCRIPTION,"units are provided cooling from a central plant",
			PT_int16,"central_washing_count",get_central_washing_count_offset(), PT_DEFAULT,0, PT_DESCRIPTION,"count of centrally located (common) washing and drying appliances are located in the building",
			PT_int16,"parking_size",get_parking_size_offset(), PT_DEFAULT,0, PT_DESCRIPTION,"number of parking spots installed",
			PT_bool,"parking_is_indoor",get_parking_is_indoor_offset(), PT_DEFAULT,0, PT_DESCRIPTION,"parking garage includes lighting and ventilation loads",
			PT_int16,"parking_charger_count",get_parking_charger_count_offset(), PT_DEFAULT,0, PT_DESCRIPTION,"number of vehicle chargers installed",
			PT_double,"parking_charger_capacity[kW]",get_parking_charger_capacity_offset(), PT_DEFAULT,0, PT_DESCRIPTION,"charging capacity of a charger",
			PT_int16,"parking_charger_active",get_parking_charger_active_offset(), PT_DEFAULT,0, PT_DESCRIPTION,"how many parking chargers are active at any moment in time",
			PT_int16,"unit_count",get_unit_count_offset(), PT_DEFAULT,0, PT_DESCRIPTION,"charging capacity of each charger",
			PT_double,"unit_size[sf]",get_unit_size_offset(), PT_DEFAULT,0, PT_DESCRIPTION,"mean size of units in the building",
			PT_double,"unit_occupancy_factor[pu]",get_unit_occupancy_factor_offset(), PT_DEFAULT,0, PT_DESCRIPTION,"total unit occupancy factor",
			PT_double,"unit_temperature[degF]",get_unit_temperature_offset(), PT_DEFAULT,0, PT_DESCRIPTION,"mean temperature of the occupied units",
			PT_double,"unit_perimeter_conductance[Btu/degF/h]",get_unit_perimeter_conductance_offset(), PT_DEFAULT,0, PT_DESCRIPTION,"thermal conductance between the interior air of the unit and exterior air temperature",
			PT_double,"unit_thermal_mass[Btu/degF]",get_unit_thermal_mass_offset(), PT_DEFAULT,0, PT_DESCRIPTION,"mean thermal mass of units in the building",
			PT_double,"unit_mass_conductance[Btu/degF/h]",get_unit_mass_conductance_offset(), PT_DEFAULT,0, PT_DESCRIPTION,"thermal conductance between unit mass and unit air",
			PT_enumeration,"unit_cooling_type",get_unit_cooling_type_offset(), PT_DEFAULT,0, PT_DESCRIPTION,"type of cooling is available in apartment units",
				PT_KEYWORD,"NONE",(enumeration)UCT_NONE,
				PT_KEYWORD,"CENTRAL",(enumeration)UCT_CENTRAL,
				PT_KEYWORD,"WINDOW_AC",(enumeration)UCT_WINDOW_AC,
				PT_KEYWORD,"UNITARY_AC",(enumeration)UCT_UNITARY_AC,
				PT_KEYWORD,"HEATPUMP",(enumeration)UCT_HEATPUMP,
			PT_double,"unit_cooling_setpoint[degF]",get_unit_cooling_setpoint_offset(), PT_DEFAULT,0, PT_DESCRIPTION,"mean cooling setpoint for the occupied units",
			PT_enumeration,"unit_heating_type",get_unit_heating_type_offset(), PT_DEFAULT,0, PT_DESCRIPTION,"type of heating is available in apartment units",
				PT_KEYWORD,"NONE",(enumeration)UHT_NONE,
				PT_KEYWORD,"CENTRAL",(enumeration)UHT_CENTRAL,
				PT_KEYWORD,"BASEBOARD",(enumeration)UHT_BASEBOARD,
				PT_KEYWORD,"RESISTIVE",(enumeration)UHT_RESISTIVE,
				PT_KEYWORD,"HEATPUMP",(enumeration)UHT_HEATPUMP,
			PT_double,"unit_heating_setpoint[degF]",get_unit_heating_setpoint_offset(), PT_DEFAULT,0, PT_DESCRIPTION,"mean heating setpoint for the occupied units",
			PT_set,"unit_appliance_types",get_unit_appliance_types_offset(), PT_DEFAULT,0, PT_DESCRIPTION,"appliances included in the apartment units",
				PT_KEYWORD,"NONE",(set)UAT_NONE,
				PT_KEYWORD,"DISHWASHER",(enumeration)UAT_DISHWASHER,
				PT_KEYWORD,"REFRIGERATOR",(enumeration)UAT_REFRIGERATOR,
				PT_KEYWORD,"WASHER",(enumeration)UAT_WASHER,
				PT_KEYWORD,"DRYER",(enumeration)UAT_DRYER,
			NULL)<1){
				char msg[256];
				sprintf(msg, "unable to publish properties in %s",__FILE__);
				throw msg;
        gl_global_create("residential::load_property",PT_char1024,&load_property,NULL);
		}
	}
}

int apartment::create(void) 
{
	return 1; /* return 1 on success, 0 on failure */
}

int apartment::init(OBJECT *parent)
{
	A  << -(U_AU+U_AC+U_AM)/C_A << U_AU/C_A << U_AC/C_A << U_AM/C_A << endr
	   << U_AU/C_U << -(U_AU+U_UC+U_UM)/C_U << U_UC/C_U << U_UM/C_U << endr
	   << U_AC/C_C << U_UC/C_C << -(U_AC+U_UC+U_CM)/C_C << U_CM/C_C << endr
	   << U_AM/C_M << U_UM/C_M << U_CM/C_M << -(U_AM+U_UM+U_CM)/C_M << endr;
	B1 << U_OA/C_A << 1/C_A << 1/C_A << 1/C_A << 0 << 0 << 0 << endr
	   << U_OU/C_U << 0 << 0 << 0 << 1/C_U << 0 << 0 << endr
	   << U_OC/C_C << 0 << 0 << 0 << 0 << 1/C_C << 1/C_C << endr
	   << U_OM/C_M << 0 << 0 << 0 << 0 << 0 << 0 << endr;
	B2 << 1/C_A << 0 << 0 << endr
	   << 0 << 1/C_U << 0 << endr
	   << 0 << 0 << 1/C_C << endr
	   << 0 << 0 << 0 << endr;
	B2inv = pinv(B2);
	T << 0 << endr
	  << 0 << endr
	  << 0 << endr
	  << 0 << endr;
	q << T_O << endr
	  << Q_AS << endr
	  << Q_AV << endr
	  << Q_AE << endr
	  << Q_US << endr
	  << Q_CS << endr
	  << Q_CV << endr;
	return 1;
}

TIMESTAMP apartment::precommit(TIMESTAMP t1)
{
	double dt = t1 - gl_globalclock;
	q[0] = T_O;
	q[1] = Q_AS;
	q[2] = Q_AV;
	q[3] = Q_AE;
	q[4] = Q_US;
	q[5] = Q_CS;
	q[6] = Q_CV;
	u = -B2inv * (A*T+B1*q);
	for ( unsigned int i = 0 ; i < u.n_elem ; i++ )
	{
		if ( u_min[i] > u_max[i] ) {continue;}
		if ( u[i] < u_min[i] ) {u[i] = u_min[i];}
		else if ( u[i] > u_max[i] ) {u[i] = u_max[i];}
	}
	dT = A*T + B1*q +B2*u;
	T += dT*dt;
	TIMESTAMP ta = t1 + 3600/abs(dT.max());
	TIMESTAMP tb = ((t1/3600)+1)*3600;
	return min(ta,tb);
}

