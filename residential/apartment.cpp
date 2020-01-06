/*	File: apartment.cpp

	Copyright (C) 2020, Regents of the Leland Stanford Junior University
*/

#include "apartment.h"

EXPORT_CREATE(apartment);
EXPORT_INIT(apartment);
EXPORT_PRECOMMIT(apartment);
EXPORT_SYNC(apartment);
EXPORT_COMMIT(apartment);

CLASS *apartment::oclass = NULL;
apartment *apartment::defaults = NULL;

apartment::apartment(MODULE *module)
{
	if (oclass==NULL)
	{
		// register to receive notice for first top down. bottom up, and second top down synchronizations
		oclass = gld_class::create(module,"apartment",sizeof(apartment),PC_AUTOLOCK|PC_PRETOPDOWN|PC_BOTTOMUP|PC_POSTTOPDOWN);
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
		}
	}
}

int apartment::create(void) 
{
	return 1; /* return 1 on success, 0 on failure */
}

int apartment::init(OBJECT *parent)
{
	return 1;
}

TIMESTAMP apartment::precommit(TIMESTAMP t1)
{
	return TS_NEVER;
}

TIMESTAMP apartment::presync(TIMESTAMP t1)
{	
	return TS_NEVER;
}

TIMESTAMP apartment::sync(TIMESTAMP t1)
{	
	return TS_NEVER;
}

TIMESTAMP apartment::postsync(TIMESTAMP t1)
{	
	return TS_NEVER;
}

TIMESTAMP apartment::commit(TIMESTAMP t1, TIMESTAMP t2)
{	
	return TS_NEVER;
}

