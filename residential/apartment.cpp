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
		oclass = gld_class::create(module,"apartment",sizeof(apartment),PC_AUTOLOCK|PC_OBSERVER);
		if (oclass==NULL)
			throw "unable to register class apartment";
		else
			oclass->trl = TRL_PROVEN;

		defaults = this;
		if (gl_publish_variable(oclass,
			PT_int16,"storeys",get_storeys_offset(), PT_DEFAULT,0, PT_DESCRIPTION,"determines the exterior wall area to interior volume/mass ratios used for the thermal model",
			PT_bool,"circulation_is_indoor",get_circulation_is_indoor_offset(), PT_DEFAULT,FALSE, PT_DESCRITON,"specifies whether circulation is indoors",
			PT_int16,"circulation_elevator_count",get_circulation_elevator_count_offset(), PT_DEFAULT,0, PT_DESCRIPTION,"determines the number of elevators available in the building",
			PT_double,"central_plant_capacity[Btu/h]",get_central_plant_capacity_offset(), PT_DEFAULT,0, PT_DESCRIPTION,"",
			PT_double,"central_plant_efficiency[pu]",get__offset(), PT_DEFAULT,0, PT_DESCRIPTION,"",
			PT_enumeration,"central_plant_type",get__offset(), PT_DEFAULT,0, PT_DESCRIPTION,"",
				PT_KEYWORD,"NONE",(enumeration)CPT_NONE,
				PT_KEYWORD,"STEAM",(enumeration)CPT_STEAM,
				PT_KEYWORD,"HOTWATER",(enumeration)CPT_HOTWATER,
				PT_KEYWORD,"FOURPIPE",(enumeration)CPT_FOURPIPE,
				PT_KEYWORD,"FORCEDAIR",(enumeration)CPT_FORCEDAIR,
			PT_bool,"central_plant_has_cooling",get__offset(), PT_DEFAULT,0, PT_DESCRIPTION,"",
			PT_int16,"central_washing_count",get__offset(), PT_DEFAULT,0, PT_DESCRIPTION,"",
			PT_int16,"parking_size",get__offset(), PT_DEFAULT,0, PT_DESCRIPTION,"",
			PT_bool,"parking_is_indoor",get__offset(), PT_DEFAULT,0, PT_DESCRIPTION,"",
			PT_int16,"parking_charger_count",get__offset(), PT_DEFAULT,0, PT_DESCRIPTION,"",
			PT_double,"parking_charger_capacity[kW]",get__offset(), PT_DEFAULT,0, PT_DESCRIPTION,"",
			PT_int16,"parking_charger_active",get__offset(), PT_DEFAULT,0, PT_DESCRIPTION,"",
			PT_int16,"unit_count",get__offset(), PT_DEFAULT,0, PT_DESCRIPTION,"",
			PT_double,"unit_size[sf]",get__offset(), PT_DEFAULT,0, PT_DESCRIPTION,"",
			PT_double,"unit_occupancy_factor[pu]",get__offset(), PT_DEFAULT,0, PT_DESCRIPTION,"",
			PT_double,"unit_temperature[degF]",get__offset(), PT_DEFAULT,0, PT_DESCRIPTION,"",
			PT_double,"unit_perimeter_conductance[Btu/degF/h]",get__offset(), PT_DEFAULT,0, PT_DESCRIPTION,"",
			PT_double,"unit_thermal_mass[Btu/degF]",get__offset(), PT_DEFAULT,0, PT_DESCRIPTION,"",
			PT_double,"unit_mass_conductance[Btu/degF/h]",get__offset(), PT_DEFAULT,0, PT_DESCRIPTION,"",
			PT_enumeration,"unit_cooling_type",get__offset(), PT_DEFAULT,0, PT_DESCRIPTION,"",
				PT_KEYWORD,"NONE",(enumeration)UCT_NONE,
				PT_KEYWORD,"CENTRAL",(enumeration)UCT_CENTRAL,
				PT_KEYWORD,"WINDOW_AC",(enumeration)UCT_WINDOW_AC,
				PT_KEYWORD,"UNITARY_AC",(enumeration)UCT_UNITARY_AC,
				PT_KEYWORD,"HEATPUMP",(enumeration)UCT_HEATPUMP,
			PT_double,"unit_cooling_setpoint[degF]",get__offset(), PT_DEFAULT,0, PT_DESCRIPTION,"",
			PT_enumeration,"unit_heating_type",get__offset(), PT_DEFAULT,0, PT_DESCRIPTION,"",
				PT_KEYWORD,"NONE",(enumeration)UHT_NONE,
				PT_KEYWORD,"CENTRAL",(enumeration)UHT_CENTRAL,
				PT_KEYWORD,"BASEBOARD",(enumeration)UHT_BASEBOARD,
				PT_KEYWORD,"RESISTIVE",(enumeration)UHT_RESISTIVE,
				PT_KEYWORD,"HEATPUMP",(enumeration)UHT_HEATPUMP,
			PT_double,"unit_heating_setpoint[degF]",get__offset(), PT_DEFAULT,0, PT_DESCRIPTION,"",
			PT_set,"unit_appliance_types",get__offset(), PT_DEFAULT,0, PT_DESCRIPTION,"",
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

TIMESTAMP apartment::presync(TIMESTAMP t1, TIMESTAMP t2)
{	
	return TS_NEVER;
}

TIMESTAMP apartment::sync(TIMESTAMP t1, TIMESTAMP t2)
{	
	return TS_NEVER;
}

TIMESTAMP apartment::postsync(TIMESTAMP t1, TIMESTAMP t2)
{	
	return TS_NEVER;
}

TIMESTAMP apartment::commit(TIMESTAMP t1, TIMESTAMP t2)
{	
	return TS_NEVER;
}

