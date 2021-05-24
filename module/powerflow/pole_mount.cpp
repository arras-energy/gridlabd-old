// module/powerflow/pole_mount.cpp
// Copyright (C) 2018, Regents of the Leland Stanford Junior University
//
// See pole.cpp for processing sequence details
//

#include "powerflow.h"

EXPORT_CREATE(pole_mount)
EXPORT_INIT(pole_mount)
EXPORT_PRECOMMIT(pole_mount)
EXPORT_SYNC(pole_mount)
EXPORT_COMMIT(pole_mount)

CLASS *pole_mount::oclass = NULL;
CLASS *pole_mount::pclass = NULL;
pole_mount *pole_mount::defaults = NULL;

pole_mount::pole_mount(MODULE *mod)
{
	if ( oclass == NULL )
	{
		pclass = node::oclass;
		oclass = gl_register_class(mod,"pole_mount",sizeof(pole_mount),PC_PRETOPDOWN|PC_POSTTOPDOWN|PC_UNSAFE_OVERRIDE_OMIT|PC_AUTOLOCK);
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
    else
    {
        equipment_is_line = get_object(get_equipment())->isa("line")
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

TIMESTAMP pole_mount::precommit(TIMESTAMP t0)
{
    //  - pole_mount    get initial equipment status
    // TODO

    return TS_NEVER;
}

TIMESTAMP pole_mount::presync(TIMESTAMP t0)
{
    //  - pole_mount    set interim equipment status
    // TODO

    extern bool NR_admit_change;
    switch ( pole_status->get_enumeration() )
    {
    case pole::PS_OK:
        if ( equipment_status->get_enumeration() != LS_CLOSED )
        {
            equipment_status->setp((enumeration)LS_CLOSED);
            NR_admit_change = true;
        }
        break;
    case pole::PS_FAILED:
        if ( equipment_status->get_enumeration() != LS_OPEN )
        {
            equipment_status->setp((enumeration)LS_OPEN);
            NR_admit_change = true;
        }
        break;
    default:
        error("pole_status %d is not valid",(int)pole_status->get_enumeration());
        break;
    }
    return TS_NEVER;
}

TIMESTAMP pole_mount::sync(TIMESTAMP t0)
{
    //  - pole_mount    update moment accumulators
    // TODO

    // for ( std::list<WIREDATA>::iterator wire = wire_data->begin() ; wire != wire_data->end() ; wire++ )
	// {
	// 	double load_nowind = (wire->diameter+2*ice_thickness)/12;
	// 	wire_load_nowind += load_nowind;
	// 	wire_moment_nowind += wire->span * load_nowind * wire->height * config->overload_factor_transverse_wire;
	// }

    return TS_NEVER;
}
TIMESTAMP pole_mount::postsync(TIMESTAMP t0)
{
    //  - pole_mount    set interim equipment status
    // TODO

    return TS_NEVER;
}

TIMESTAMP pole_mount::commit(TIMESTAMP t1,TIMESTAMP t2)
{
    //  - pole_mount    finalize equipment status

    return TS_NEVER;
}
