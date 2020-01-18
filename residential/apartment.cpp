/*	File: apartment.cpp

	Copyright (C) 2020, Regents of the Leland Stanford Junior University
*/

#include "residential.h"

EXPORT_CREATE(apartment);
EXPORT_INIT(apartment);
EXPORT_PRECOMMIT(apartment);

CLASS *apartment::oclass = NULL;
apartment *apartment::defaults = NULL;

char1024 apartment::load_property = "base_power";
double apartment::maximum_temperature_update = 0.1;
TIMESTAMP apartment::maximum_timestep = 60;

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
			PT_int16,"building_floors",get_building_floors_offset(), PT_DEFAULT,"0", PT_DESCRIPTION,"number of floors in building",
			PT_double,"building_floor_depth[ft]",get_building_floor_depth_offset(), PT_DEFAULT,"2.0 ft", PT_DESCRIPTION,"ceiling-to-floor depth",
			PT_double,"building_floor_height[ft]",get_building_floor_height_offset(), PT_DEFAULT,"8.0 ft", PT_DESCRIPTION,"floor-to-ceiling height",
			PT_double,"building_heat_leakage[pu]",get_building_heat_leakage_offset(), PT_DEFAULT,"0.10 pu", PT_DESCRIPTION,"fraction of zone heat gains that leak into mass",
			PT_double,"building_occupancy_factor[pu]",get_building_occupancy_factor_offset(), PT_DEFAULT,"0.95 pu", PT_DESCRIPTION,"fraction of building units that are occupied",
			PT_double,"building_outdoor_temperature[degF]",get_building_outdoor_temperature_offset(), PT_DEFAULT,"59 degF", PT_DESCRIPTION,"temperature outside the building",
			PT_double,"building_overdesign_factor[pu]",get_building_overdesign_factor_offset(), PT_DEFAULT, "0.5 pu", PT_DESCRIPTION,"overdesign factor for building systems",
			PT_int16,"building_units",get_building_units_offset(), PT_DEFAULT,"0", PT_DESCRIPTION,"number of units in the building",

			PT_set,"core_configuration",get_core_configuration_offset(), PT_DEFAULT,"NONE", PT_DESCRIPTION,"configuration of the core",
				PT_KEYWORD, "NONE", CC_NONE,
				PT_KEYWORD, "INDOOR", CC_INDOOR,
				PT_KEYWORD, "DOUBLE", CC_DOUBLE,
				PT_KEYWORD, "CONDITIONED", CC_CONDITIONED,
			PT_double,"core_cooling_setpoint[degF]",get_core_cooling_setpoint_offset(), PT_DEFAULT,"0", PT_DESCRIPTION,"cooling setpoint in the core space",
			PT_int16,"core_elevators",get_core_elevators_offset(), PT_DEFAULT,"0", PT_DESCRIPTION,"number of elevators operating in the core space",
			PT_double,"core_heating_setpoint[degF]",get_core_heating_setpoint_offset(), PT_DEFAULT,"0", PT_DESCRIPTION,"heating setpoint in the core space",
			PT_enumeration,"core_mode",get_core_mode_offset(), PT_DEFAULT,"OFF", PT_DESCRIPTION,"operating mode of the core zone",
				PT_KEYWORD, "OFF", SPM_OFF,
				PT_KEYWORD, "VENTILATING", SPM_VENTILATING,
				PT_KEYWORD, "HEATING", SPM_HEATING,
				PT_KEYWORD, "COOLING", SPM_COOLING,
			PT_int16,"core_laundry_units",get_core_laundry_units_offset(), PT_DEFAULT,"", PT_DESCRIPTION,"number of community/shared washer/dryers pairs installed in the core space",
			PT_double,"core_width[ft]",get_core_width_offset(), PT_DEFAULT,"0", PT_DESCRIPTION,"width of the core space",

			PT_double,"parking_capacity_chargers[kW]",get_parking_capacity_chargers_offset(), PT_DEFAULT,"0", PT_DESCRIPTION,"installed vehicle charger capacity",
			PT_double,"parking_capacity_elevators[kW]",get_parking_capacity_elevators_offset(), PT_DEFAULT,"0", PT_DESCRIPTION,"installed elevator capacity",
			PT_double,"parking_capacity_lights[kW]",get_parking_capacity_lights_offset(), PT_DEFAULT,"0", PT_DESCRIPTION,"installed lighting capacity",
			PT_double,"parking_capacity_ventilation[kW]",get_parking_capacity_ventilation_offset(), PT_DEFAULT,"0", PT_DESCRIPTION,"installed ventilation capacity",
			PT_int16,"parking_chargers_active",get_parking_chargers_active_offset(), PT_DEFAULT,"0", PT_DESCRIPTION,"number of EV chargers active",
			PT_int16,"parking_chargers_installed",get_parking_chargers_installed_offset(), PT_DEFAULT,"0", PT_DESCRIPTION,"number of EV chargers installed",
			PT_enumeration,"parking_configuration",get_parking_configuration_offset(), PT_DEFAULT,"OUTDOOR", PT_DESCRIPTION,"parking configuration",
				PT_KEYWORD, "OUTDOOR", PC_OUTDOOR,
				PT_KEYWORD, "INDOOR", PC_INDOOR,
			PT_double,"parking_demand_chargers[pu]",get_parking_demand_chargers_offset(), PT_DEFAULT,"0", PT_DESCRIPTION,"parking EV charger demand as a fraction of installed capacity",
			PT_double,"parking_demand_elevators[pu]",get_parking_demand_elevators_offset(), PT_DEFAULT,"0", PT_DESCRIPTION,"parking elevator demand as a fraction of installed capacity",
			PT_double,"parking_demand_lights[pu]",get_parking_demand_lights_offset(), PT_DEFAULT,"0", PT_DESCRIPTION,"parking lighting demand as a fraction of installed capacity",
			PT_double,"parking_demand_ventilation[pu]",get_parking_demand_ventilation_offset(), PT_DEFAULT,"0", PT_DESCRIPTION,"parking ventilation demand as a fraction of installed capacity",
			PT_int16,"parking_size",get_parking_size_offset(), PT_DEFAULT,"0", PT_DESCRIPTION,"total number of parking spots installed",

			PT_double,"power_core[kW]",get_power_core_offset(), PT_DEFAULT,"0", PT_DESCRIPTION,"power delivered to the core spaces",
			PT_double,"power_parking[kW]",get_power_parking_offset(), PT_DEFAULT,"0", PT_DESCRIPTION,"power delivered to the parking areas",
			PT_double,"power_system[kW]",get_power_system_offset(), PT_DEFAULT,"0", PT_DESCRIPTION,"power delivered to the building systems",
			PT_double,"power_total[kW]",get_power_total_offset(), PT_DEFAULT,"0", PT_DESCRIPTION,"total power delivered to the building",
			PT_double,"power_units[kW]",get_power_units_offset(), PT_DEFAULT,"0", PT_DESCRIPTION,"total power delivered to the apartment units",

			PT_double,"system_cooling_air_temperature[degF]",get_system_cooling_air_temperature_offset(), PT_DEFAULT,"50 degF", PT_DESCRIPTION,"system cooling supply air temperature",
			PT_double,"system_cooling_capacity[kBtu/h]",get_system_cooling_capacity_offset(), PT_DEFAULT,"0", PT_DESCRIPTION,"system cooling capacity",
			PT_double,"system_cooling_efficiency[kBtu/kWh]",get_system_cooling_efficiency_offset(), PT_DEFAULT,"15.0 kBtu/kWh", PT_DESCRIPTION,"system cooling efficiency",
			PT_double,"system_heating_air_temperature[degF]",get_system_heating_air_temperature_offset(), PT_DEFAULT,"", PT_DESCRIPTION,"system heating supply air temperature",
			PT_double,"system_heating_capacity[kBtu/h]",get_system_heating_capacity_offset(), PT_DEFAULT,"0", PT_DESCRIPTION,"system heating capacity",
			PT_double,"system_heating_efficiency[kBtu/kWh]",get_system_heating_efficiency_offset(), PT_DEFAULT,"10.0 kBtu/kWh", PT_DESCRIPTION,"system heating efficiency",
			PT_enumeration,"system_mode",get_system_mode_offset(), PT_DEFAULT,"OFF", PT_DESCRIPTION,"operating mode of the central system",
				PT_KEYWORD, "OFF", SPM_OFF,
				PT_KEYWORD, "VENTILATING", SPM_VENTILATING,
				PT_KEYWORD, "HEATING", SPM_HEATING,
				PT_KEYWORD, "COOLING", SPM_COOLING,
			PT_set,"system_type_central",get_system_type_central_offset(), PT_DEFAULT,"NONE", PT_DESCRIPTION,"central system type, if any",
				PT_KEYWORD, "NONE", STC_NONE,
				PT_KEYWORD, "HEAT", STC_HEAT,
				PT_KEYWORD, "COOL", STC_COOL,
				PT_KEYWORD, "BOTH", STC_BOTH,
			PT_enumeration,"system_type_economizer",get_system_type_economizer_offset(), PT_DEFAULT,"NONE", PT_DESCRIPTION,"type of economizer used by the central cooling system, if any",
				PT_KEYWORD, "NONE", STE_NONE,
				PT_KEYWORD, "DRYBULB", STE_DRYBULB,
				PT_KEYWORD, "WETBULB", STE_WETBULB,
				PT_KEYWORD, "DIFFERENTIAL", STE_DIFFERENTIAL,
			PT_enumeration,"system_type_ventilation",get_system_type_ventilation_offset(), PT_DEFAULT,"NONE", PT_DESCRIPTION,"type of ventilation system, if any",
				PT_KEYWORD, "NONE", STV_NONE,
				PT_KEYWORD, "LOCAL", STV_LOCAL,
				PT_KEYWORD, "CENTRAL", STV_CENTRAL,

			PT_set,"unit_appliance_types",get_unit_appliance_types_offset(), PT_DEFAULT,0, PT_DESCRIPTION,"types of appliances installed in the units",
				PT_KEYWORD,"NONE",(set)UAT_NONE,
				PT_KEYWORD,"DISHWASHER",(enumeration)UAT_DISHWASHER,
				PT_KEYWORD,"REFRIGERATOR",(enumeration)UAT_REFRIGERATOR,
				PT_KEYWORD,"WASHER",(enumeration)UAT_WASHER,
				PT_KEYWORD,"DRYER",(enumeration)UAT_DRYER,
			PT_double,"unit_capacity_cooking[kW]",get_unit_capacity_cooking_offset(), PT_DEFAULT,"3.0 kW", PT_DESCRIPTION,"maximum power of the cooking appliances in units",
			PT_double,"unit_capacity_dishwasher[kW]",get_unit_capacity_dishwasher_offset(), PT_DEFAULT,"1.8 kW", PT_DESCRIPTION,"maximum power of the dishwashing appliances in units",
			PT_double,"unit_capacity_dryer[kW]",get_unit_capacity_dryer_offset(), PT_DEFAULT,"3.0 kW", PT_DESCRIPTION,"maximum power demand of dryers in units",
			PT_double,"unit_capacity_lights[kW]",get_unit_capacity_lights_offset(), PT_DEFAULT,"0", PT_DESCRIPTION,"maximum lighting demand in units",
			PT_double,"unit_capacity_plugs[kW]",get_unit_capacity_plugs_offset(), PT_DEFAULT,"0", PT_DESCRIPTION,"maximum non-lighting plug load demand in units",
			PT_double,"unit_capacity_refrigerator[kW]",get_unit_capacity_refrigerator_offset(), PT_DEFAULT,"200 W", PT_DESCRIPTION,"maximum refrigeration demand in units",
			PT_double,"unit_capacity_washer[kW]",get_unit_capacity_washer_offset(), PT_DEFAULT,"0", PT_DESCRIPTION,"maximum washer power demand in units",
			PT_double,"unit_cooling_capacity[kBtu/h]",get_unit_cooling_capacity_offset(), PT_DEFAULT,"1.0 kW", PT_DESCRIPTION,"cooling capacity of the unit system, if any",
			PT_double,"unit_cooling_efficiency[kBtu/kWh]",get_unit_cooling_efficiency_offset(), PT_DEFAULT,"15.0 kBtu/kWh", PT_DESCRIPTION,"unit cooling system efficiency",
			PT_double,"unit_cooling_setpoint[degF]",get_unit_cooling_setpoint_offset(), PT_DEFAULT,"78.0 degF", PT_DESCRIPTION,"unit cooling system temperature setpoint",
			PT_double,"unit_demand_cooking[pu]",get_unit_demand_cooking_offset(), PT_DEFAULT,"", PT_DESCRIPTION,"fraction of the cooking power capacity that is active",
			PT_double,"unit_demand_dishwasher[pu]",get_unit_demand_dishwasher_offset(), PT_DEFAULT,"0", PT_DESCRIPTION,"fraction of the dishwasher power capacity that is active",
			PT_double,"unit_demand_dryer[pu]",get_unit_demand_dryer_offset(), PT_DEFAULT,"0", PT_DESCRIPTION,"fraction of the dryer power capacity that is active",
			PT_double,"unit_demand_lights[pu]",get_unit_demand_lights_offset(), PT_DEFAULT,"0", PT_DESCRIPTION,"fraction of the lighting power capacity that is active",
			PT_double,"unit_demand_plugs[pu]",get_unit_demand_plugs_offset(), PT_DEFAULT,"0", PT_DESCRIPTION,"fraction of the plugs power capacity that is active",
			PT_double,"unit_demand_refrigerator[pu]",get_unit_demand_refrigerator_offset(), PT_DEFAULT,"0", PT_DESCRIPTION,"fraction of the refrigerator power capacity that is active",
			PT_double,"unit_demand_washer[pu]",get_unit_demand_washer_offset(), PT_DEFAULT,"0", PT_DESCRIPTION,"fraction of the washer power capacity that is active",
			PT_double,"unit_depth[ft]",get_unit_depth_offset(), PT_DEFAULT,"40 ft", PT_DESCRIPTION,"double",
			PT_double,"unit_heating_capacity[kBtu/h]",get_unit_heating_capacity_offset(), PT_DEFAULT,"0", PT_DESCRIPTION,"heating capacity of the unit system, if any",
			PT_double,"unit_heating_efficiency[kBtu/kWh]",get_unit_heating_efficiency_offset(), PT_DEFAULT,"10.0 kBtu/kWh", PT_DESCRIPTION,"unit heating system efficiency",
			PT_double,"unit_heating_setpoint[degF]",get_unit_heating_setpoint_offset(), PT_DEFAULT,"70 degF", PT_DESCRIPTION,"unit heating system temperature setpoint",
			PT_enumeration,"unit_mode",get_unit_mode_offset(), PT_DEFAULT,"", PT_DESCRIPTION,"unit system mode",
				PT_KEYWORD, "OFF", SPM_OFF,
				PT_KEYWORD, "VENTILATING", SPM_VENTILATING,
				PT_KEYWORD, "HEATING", SPM_HEATING,
				PT_KEYWORD, "COOLING", SPM_COOLING,
			PT_set,"unit_system_type",get_unit_system_type_offset(), PT_DEFAULT,"", PT_DESCRIPTION,"UNITSYSTEMTYPE",
				PT_KEYWORD, "NONE", UST_NONE,
				PT_KEYWORD, "HEAT", UST_HEAT,
				PT_KEYWORD, "COOL", UST_COOL,
				PT_KEYWORD, "BOTH", UST_BOTH,
			PT_double,"unit_width[ft]",get_unit_width_offset(), PT_DEFAULT,"", PT_DESCRIPTION,"double",

			PT_double,"vacant_cooling_setpoint[degF]",get_vacant_cooling_setpoint_offset(), PT_DEFAULT,"120 degF", PT_DESCRIPTION,"vacant unit cooling setpoint",
			PT_double,"vacant_heating_setpoint[degF]",get_vacant_heating_setpoint_offset(), PT_DEFAULT,"50 degF", PT_DESCRIPTION,"vacant unit heating setpoint",
			PT_enumeration,"vacant_mode",get_vacant_mode_offset(), PT_DEFAULT,"", PT_DESCRIPTION,"vacant unit system mode",
				PT_KEYWORD, "OFF", SPM_OFF,
				PT_KEYWORD, "VENTILATING", SPM_VENTILATING,
				PT_KEYWORD, "HEATING", SPM_HEATING,
				PT_KEYWORD, "COOLING", SPM_COOLING,

			NULL)<1){
				char msg[256];
				sprintf(msg, "unable to publish properties in %s",__FILE__);
				throw msg;
        gl_global_create("residential::load_property",PT_char1024,&load_property,NULL);
        gl_global_create("residential::maximum_temperature_update",PT_double,&maximum_temperature_update,PT_UNITS,"degF",NULL);
		}
	}
}

int apartment::create(void) 
{
	return 1; /* return 1 on success, 0 on failure */
}

int apartment::init(OBJECT *parent)
{
	// check for missing values
	if ( building_floors <= 1 )
	{
		exception("building_floors must be 1 or more");
	}
	if ( building_units <= 1 )
	{
		exception("building_units must be 1 or more");
	}
	if ( core_cooling_setpoint == 0 )
	{
		core_cooling_setpoint = unit_cooling_setpoint + 5.0;
	}
	if ( core_heating_setpoint == 0 )
	{
		core_heating_setpoint = unit_heating_setpoint - 5.0;
	}
	if ( core_width == 0 )
	{
		core_width = ( core_configuration&CC_DOUBLE ) ? 6.0 : 5.0;
	}
	if ( parking_size == 0 )
	{
		parking_size = building_floors*building_units;
	}
	if ( system_cooling_capacity == 0 )
	{
		// TODO
	}
	if ( system_heating_capacity == 0 )
	{
		// TODO
	}
	if ( unit_cooling_capacity == 0 )
	{
		// TODO
	}
	if ( unit_heating_capacity == 0 )
	{
		// TODO
	}

	solver = msolve("new");

	msolve("set",solver,"N",4);
	
	msolve("copy",solver,"U",
		U_OA,U_OU,U_OC,U_OM,
		U_AU,U_AC,U_AM,
		U_UC,U_UM,
		U_CM);

	msolve("copy",solver,"C",C_A,C_U,C_C,C_M);

	msolve("copy",solver,"umin",
		-(1+building_overdesign_factor) * unit_cooling_capacity * building_units * building_occupancy_factor,
		-(1+building_overdesign_factor) * unit_cooling_capacity * building_units * (1-building_occupancy_factor),
		-(1+building_overdesign_factor) * ( system_cooling_capacity + unit_cooling_capacity * building_units ),
		0.0);

	msolve("copy",solver,"umax",
		(1+building_overdesign_factor) * unit_heating_capacity * building_units * building_occupancy_factor,
		(1+building_overdesign_factor) * unit_heating_capacity * building_units * (1-building_occupancy_factor),
		(1+building_overdesign_factor) * ( system_heating_capacity - unit_heating_capacity * building_units),
		0.0);

	// set the zone-zone leakages
	msolve("set",solver,"a",1-building_heat_leakage);

	update();
	msolve("solve",solver,0);

	return 1;
}

void apartment::update()
{
	// set the heat gain
	msolve("copy",solver,"q",
		building_outdoor_temperature,
		Q_AS + Q_AV + Q_AE,
		Q_US,
		Q_CS + Q_CV);

	// update occupied unit setpoints
	if ( unit_mode == SPM_COOLING )
	{
		msolve("set",solver,"Tset",0,unit_cooling_setpoint);
	}
	else if ( unit_mode == SPM_HEATING )
	{
		msolve("set",solver,"Tset",0,unit_heating_setpoint);
	}
	else
	{
		// setpoint does not change
	}

	// update occupied unit setpoints
	if ( vacant_mode == SPM_COOLING )
	{
		msolve("set",solver,"Tset",1,vacant_cooling_setpoint);
	}
	else if ( vacant_mode == SPM_HEATING )
	{
		msolve("set",solver,"Tset",1,vacant_heating_setpoint);
	}
	else
	{
		// setpoint does not change
	}

	// update occupied unit setpoints
	if ( core_mode == SPM_COOLING )
	{
		msolve("set",solver,"Tset",2,core_cooling_setpoint);
	}
	else if ( core_mode == SPM_HEATING )
	{
		msolve("set",solver,"Tset",2,core_heating_setpoint);
	}
	else
	{
		// setpoint does not change
	}
}

TIMESTAMP apartment::precommit(TIMESTAMP t1)
{
	double dt = t1 - gl_globalclock;

	// calculate the power 

	// calculate the time to the next required solution

	// TIMESTAMP a = ((TIMESTAMP)(t1/maximum_timestep+1))*maximum_timestep; 
	// TIMESTAMP b = t1 + 3600*maximum_temperature_update/abs(dT.max());
	// return min(a,b);
	msolve("solve",solver,dt);
	return TS_NEVER;
}

