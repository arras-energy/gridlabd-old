// module/loads/building.cpp
// Copyright (C) 2022 Regents of the Leland Stanford Junior University

#include "powerflow.h"

EXPORT_CREATE_C(building,building);
EXPORT_INIT_C(building,building);

CLASS *building::oclass = NULL;
building *building::defaults = NULL;

building::building(MODULE *module)
: pqload(module)
{
	if (oclass==NULL)
	{
		// register to receive notice for first top down. bottom up, and second top down synchronizations
		oclass = gld_class::create(module,"building",sizeof(building),PC_PRETOPDOWN|PC_BOTTOMUP|PC_POSTTOPDOWN|PC_UNSAFE_OVERRIDE_OMIT|PC_AUTOLOCK);
		if (oclass==NULL)
			throw "unable to register class building";
		else
			oclass->trl = TRL_PROVEN;

		defaults = this;
		if (gl_publish_variable(oclass,

			PT_INHERIT, "pqload",
			
			PT_char32, "building_class", get_building_class_offset(),
				PT_DESCRIPTION, "building class (e.g., commercial, residential)",

			PT_char32, "building_type", get_building_type_offset(),
				PT_DESCRIPTION, "building type (e.g., house, office)",

			PT_char256, "address", get_address_offset(),
				PT_DESCRIPTION, "building address",

			PT_double, "floor_area[sf]", get_floor_area_offset(),
				PT_DESCRIPTION, "building gross floor area",

			PT_int16, "storeys", get_storeys_offset(),
				PT_DESCRIPTION, "number of floors in building",

			PT_double, "glazing[pu]", get_glazing_offset(),
				PT_DESCRIPTION, "fraction of exterior wall glazing",

			PT_double, "perimeter[ft]", get_perimeter_offset(),
				PT_DESCRIPTION, "building exterior wall perimeter",

			PT_double, "height[ft]", get_height_offset(),
				PT_DESCRIPTION, "building exterior wall height",

			PT_char32, "system_type", get_system_type_offset(),
				PT_DESCRIPTION, "building system type",

			PT_char32, "construction_type", get_construction_type_offset(),
				PT_DESCRIPTION, "building",

			NULL)<1){
				char msg[256];
				snprintf(msg,sizeof(msg)-1, "unable to publish properties in %s",__FILE__);
				throw msg;
		}
	}
}

int building::create(void) 
{
	return pqload::create(); /* return 1 on success, 0 on failure */
}

int building::init(OBJECT *parent)
{
	return pqload::init(parent);
}

TIMESTAMP building::precommit(TIMESTAMP t1, TIMESTAMP t2)
{
	return TS_NEVER;
}

TIMESTAMP building::presync(TIMESTAMP t0)
{
	return pqload::presync(t0);
}

TIMESTAMP building::sync(TIMESTAMP t0)
{
	return pqload::sync(t0);
}

