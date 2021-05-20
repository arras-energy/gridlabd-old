// module/powerflow/pole_mount.cpp
// Copyright (C) 2018, Regents of the Leland Stanford Junior University

#include "powerflow.h"

EXPORT_CREATE(pole_mount)
EXPORT_INIT(pole_mount)
EXPORT_SYNC(pole_mount)

CLASS *pole_mount::oclass = NULL;
CLASS *pole_mount::pclass = NULL;
pole_mount *pole_mount::defaults = NULL;

pole_mount::pole_mount(MODULE *mod)
{
	if ( oclass == NULL )
	{
		pclass = node::oclass;
		oclass = gl_register_class(mod,"pole_mount",sizeof(pole_mount),PC_PRETOPDOWN|PC_UNSAFE_OVERRIDE_OMIT|PC_AUTOLOCK);
		if ( oclass == NULL )
			throw "unable to register class pole_mount";
		oclass->trl = TRL_PROTOTYPE;
		if ( gl_publish_variable(oclass,

            PT_object, "equipment", get_equipment_offset(),
                PT_DESCRIPTION, "powerflow link object id",

			PT_double, "height[ft]", get_height_offset(),
                PT_DEFAULT, "0.0 ft",
                PT_DESCRIPTION, "height above ground at which equipment is mounted",

                PT_double, "offset[ft]", get_offset_offset(),
                    PT_DEFAULT, "0.0 ft",
                    PT_DESCRIPTION, "distance from pole centerline at which equipment is mounted",

            PT_double, "area[sf]", get_area_offset(),
                PT_DEFAULT, "0.0 sf",
                PT_DESCRIPTION, "cross sectional area of equipment",

            PT_double, "direction[deg]", get_direction_offset(),
                PT_DEFAULT, "0.0 deg",
                PT_DESCRIPTION, "direction from pole centerline at which equipment is mounted",

            PT_double, "weight[lb]", get_weight_offset(),
                PT_DEFAULT, "0.0 lb",
                PT_DESCRIPTION, "weight of equipment mounted",
                NULL)<1)
        {
			char msg[256];
			sprintf(msg, "unable to publish properties in %s",__FILE__);
			throw msg;
        }
    }
}

int pole_mount::create(void)
{
	return 1;
}

int pole_mount::init(OBJECT *parent)
{
    if ( my()->parent == NULL || ! get_parent()->isa("pole") )
    {
        error("pole_mount must have a pole as parent object");
    }

    if ( ! get_object(get_equipment())->isa("link") )
    {
        error("equipment must be a powerflow link");
    }

    pole_status = new gld_property(parent,"status");
    if ( pole_status == NULL || ! pole_status->is_valid() )
    {
        error("pole status property is not valid");
    }

    equipment_status = new gld_property(equipment,"status");
    if ( equipment_status == NULL || ! equipment_status->is_valid() )
    {
        error("equipment status property is not valid");
    }
	return 1;
}

TIMESTAMP pole_mount::presync(TIMESTAMP t0)
{
    switch ( pole_status->get_enumeration() )
    {
    case pole::PS_OK:
        equipment_status->setp((enumeration)LS_CLOSED);
        break;
    case pole::PS_FAILED:
        equipment_status->setp((enumeration)LS_OPEN);
        break;
    default:
        error("pole_status %d is not valid",(int)pole_status->get_enumeration());
        break;
    }
    return TS_NEVER;
}

TIMESTAMP pole_mount::sync(TIMESTAMP t0)
{
    return TS_NEVER;
}
TIMESTAMP pole_mount::postsync(TIMESTAMP t0)
{
    return TS_NEVER;
}
