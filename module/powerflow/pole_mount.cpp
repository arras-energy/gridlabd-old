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
// EXPORT_COMMIT(pole_mount)

CLASS *pole_mount::oclass = NULL;
CLASS *pole_mount::pclass = NULL;
pole_mount *pole_mount::defaults = NULL;

static char32 wind_speed_name = "wind_speed";
static char32 wind_dir_name = "wind_dir";
static char32 wind_gust_name = "wind_gust";

pole_mount::pole_mount(MODULE *mod)
{
	if ( oclass == NULL )
	{
		pclass = node::oclass;
		oclass = gl_register_class(mod,"pole_mount",sizeof(pole_mount),PC_PRETOPDOWN|PC_BOTTOMUP|PC_UNSAFE_OVERRIDE_OMIT|PC_AUTOLOCK);
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

            PT_double, "direction[deg]", get_direction_offset(),
                PT_DEFAULT, "0.0 deg",
                PT_DESCRIPTION, "direction from pole centerline at which equipment is mounted",

            PT_double, "area[sf]", get_area_offset(),
                PT_DEFAULT, "0.0 sf",
                PT_DESCRIPTION, "cross sectional area of equipment",

            PT_double, "heading_direction[deg]", get_direction_offset(),
                PT_DEFAULT, "0.0 deg",
                PT_DESCRIPTION, "direction at which the powerline pulls away from the pole",

            PT_double, "weight[lb]", get_weight_offset(),
                PT_DEFAULT, "0.0 lb",
                PT_DESCRIPTION, "weight of equipment mounted",

            PT_double, "pole_spacing[ft]", get_pole_spacing_offset(),
                PT_DEFAULT, "0.0 ft",
                PT_DESCRIPTION, "Mean spacing between poles adjacent",

            PT_double, "wind_speed[m/s]", get_wind_speed_offset(),
                PT_DEFAULT, "0 m/s",
                PT_DESCRIPTION, "local wind speed",

            PT_double, "wind_direction[deg]", get_wind_direction_offset(),
                PT_DEFAULT, "0 deg",
                PT_DESCRIPTION, "local wind direction",

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
        return 0;
    }

    if ( equipment == NULL )
    {
        error("equipment not specified");
        return 0;
    }

    if ( get_object(get_equipment()) == NULL )
    {
        error("equipment is not specified");
    }
    else if ( ! get_object(get_equipment())->isa("link") )
    {
        warning("equipment is not a powerflow link object");
    }
    else
    {
        equipment_is_line = get_object(get_equipment())->isa("line");
    }

    pole_status = new gld_property(parent,"status");
    if ( pole_status == NULL || ! pole_status->is_valid() )
    {
        error("pole status property is not valid");
        return 0;
    }

    equipment_status = new gld_property(equipment,"status");
    if ( equipment_status == NULL || ! equipment_status->is_valid() )
    {
        error("equipment status property is not valid");
        return 0;
    }

    if ( equipment_is_line )
    {
        class overhead_line *line = OBJECTDATA(get_equipment(),class overhead_line);
        verbose("overhead_line = %s",get_equipment()->name);
        class line_configuration *config = OBJECTDATA(line->configuration,class line_configuration);
        verbose("line_configuration = %s",line->configuration ? line->configuration->name : "(NA)");
        class line_spacing *spacing = OBJECTDATA(config->line_spacing,class line_spacing);
        verbose("line_spacing = %s",config->line_spacing ? config->line_spacing->name : "(NA)");
        class overhead_line_conductor *conductor[] = {
            OBJECTDATA(config->phaseA_conductor,class overhead_line_conductor),
            OBJECTDATA(config->phaseB_conductor,class overhead_line_conductor),
            OBJECTDATA(config->phaseC_conductor,class overhead_line_conductor),
            OBJECTDATA(config->phaseN_conductor,class overhead_line_conductor)};
        verbose("overhead_line_conductor = {A:%s, B:%s, C:%s, N:%s}",
            config->phaseA_conductor ? config->phaseA_conductor->name : "(NA)",
            config->phaseB_conductor ? config->phaseB_conductor->name : "(NA)",
            config->phaseC_conductor ? config->phaseC_conductor->name : "(NA)",
            config->phaseN_conductor ? config->phaseN_conductor->name : "(NA)");
        pole *mount = OBJECTDATA(my()->parent,pole);
        double alpha = mount->get_tilt_angle()*PI/180;
        pole_configuration *pole_config = OBJECTDATA(mount->get_configuration(),pole_configuration);
        weight = (
            (conductor[0]?conductor[0]->get_cable_weight():0.0) +
            (conductor[1]?conductor[1]->get_cable_weight():0.0) +
            (conductor[2]?conductor[2]->get_cable_weight():0.0) +
            (conductor[3]?conductor[3]->get_cable_weight():0.0) ) * pole_spacing / 2;
        verbose("line weight = %g lbs",weight);
        line_moment_weight = pole_config->overload_factor_vertical * (
            (spacing->distance_AtoE - mount->get_guy_height()) * (conductor[0]?conductor[0]->get_cable_weight() + 
                0.3384*PI*(conductor[0]->cable_diameter+line->get_ice_thickness())*line->get_ice_thickness():0.0) +
            (spacing->distance_BtoE - mount->get_guy_height()) * (conductor[1]?conductor[1]->get_cable_weight() +
                0.3384*PI*(conductor[1]->cable_diameter+line->get_ice_thickness())*line->get_ice_thickness():0.0) +
            (spacing->distance_CtoE - mount->get_guy_height()) * (conductor[2]?conductor[2]->get_cable_weight() +
                0.3384*PI*(conductor[2]->cable_diameter+line->get_ice_thickness())*line->get_ice_thickness():0.0) +
            (spacing->distance_NtoE - mount->get_guy_height()) * (conductor[3]?conductor[3]->get_cable_weight() +
                0.3384*PI*(conductor[3]->cable_diameter+line->get_ice_thickness())*line->get_ice_thickness():0.0) 
            ) * pole_spacing * sin(alpha) / 2; // 1/2 works for poles at the same elevation level
        verbose("line_moment_weight = %g ft*lb (due to the weight of conductor and ice)",line_moment_weight);
        double transverse_load_nowind = 1 * (
            (conductor[0]?conductor[0]->cable_diameter+2*line->get_ice_thickness():0.0) +
            (conductor[1]?conductor[1]->cable_diameter+2*line->get_ice_thickness():0.0) +
            (conductor[2]?conductor[2]->cable_diameter+2*line->get_ice_thickness():0.0) +
            (conductor[3]?conductor[3]->cable_diameter+2*line->get_ice_thickness():0.0)
            ) / 12; // unit transverse load when wind load is 1lb/ft^2
        verbose("transverse_load_nowind = %g lb/ft (wind load is 1 lb/sf)",transverse_load_nowind);
        line_load_nowind = transverse_load_nowind * pole_spacing;
        verbose("line_load_nowind = %g lb (wind load is 1 lb/sf)",line_load_nowind);
        line_moment_nowind = 1 * pole_spacing * cos(alpha) * pole_config->overload_factor_transverse_wire * (
        (spacing->distance_AtoE-mount->get_guy_height()) * (conductor[0]?conductor[0]->cable_diameter+2*line->get_ice_thickness():0.0) +
        (spacing->distance_BtoE-mount->get_guy_height()) * (conductor[1]?conductor[1]->cable_diameter+2*line->get_ice_thickness():0.0) +
        (spacing->distance_CtoE-mount->get_guy_height()) * (conductor[2]?conductor[2]->cable_diameter+2*line->get_ice_thickness():0.0) +
        (spacing->distance_NtoE-mount->get_guy_height()) * (conductor[3]?conductor[3]->cable_diameter+2*line->get_ice_thickness():0.0)
        ) / 12;
        verbose("load_moment_nowind = %g ft*lb (wind load is 1 lb/sf)", line_moment_nowind);
        strength = (conductor[0]?conductor[0]->get_cable_strength():0.0)
            + (conductor[1]?conductor[1]->get_cable_strength():0.0)
            + (conductor[2]?conductor[2]->get_cable_strength():0.0)
            + (conductor[3]?conductor[3]->get_cable_strength():0.0);
        verbose("strength = %g lb", strength);
        line_moment_tension = pole_config->overload_factor_transverse_wire * cos(alpha) * (
            (spacing->distance_AtoE-mount->get_guy_height()) * (conductor[0]?conductor[0]->get_cable_strength():0.0) + 
            (spacing->distance_BtoE-mount->get_guy_height()) * (conductor[1]?conductor[1]->get_cable_strength():0.0) + 
            (spacing->distance_CtoE-mount->get_guy_height()) * (conductor[2]?conductor[2]->get_cable_strength():0.0) + 
            (spacing->distance_NtoE-mount->get_guy_height()) * (conductor[3]?conductor[3]->get_cable_strength():0.0));
        verbose("line_moment_tension = %g ft*lb (moments due to line tension)",line_moment_tension); // moment due to conductor tension
    }
    else
    {
        // equipment
        pole *mount = OBJECTDATA(my()->parent,pole);
        double alpha = mount->get_tilt_angle()*PI/180;
        double beta = mount->get_tilt_direction()*PI/180;
        double theta = direction*PI/180;
        double equipment_x = offset * cos(theta);
        double equipment_y = offset * sin(theta);
        double equipment_z = height;
        equipment_x_tilt = (cos(beta)*cos(beta)*cos(alpha) + sin(beta)*sin(beta)) * equipment_x +
                                (cos(beta)*sin(beta)*cos(alpha) - cos(beta)*sin(beta)) * equipment_y +
                                (cos(beta)*sin(alpha)) * equipment_z;
        equipment_y_tilt = (cos(beta)*sin(beta)*cos(alpha) - cos(beta)*sin(beta)) * equipment_x +
                                (sin(beta)*sin(beta)*cos(alpha) + cos(beta)*cos(beta)) * equipment_y +
                                (sin(beta)*sin(alpha)) * equipment_z;
        equipment_z_tilt = -sin(alpha)*cos(beta)*equipment_x - sin(alpha)*sin(beta)*equipment_y + cos(alpha)*equipment_z;
        offset_tilt = sqrt(equipment_x_tilt*equipment_x_tilt + equipment_y_tilt*equipment_y_tilt);
        verbose("equipment coordinate = (%g, %g, %g) (measured in ft)",equipment_x,equipment_y,equipment_z);
        verbose("equipment tilt coordinate = (%g, %g, %g) (measured in ft)",equipment_x_tilt,equipment_y_tilt,equipment_z_tilt);
    }
	return 1;
}

TIMESTAMP pole_mount::precommit(TIMESTAMP t0)
{
    extern bool NR_admit_change;
    switch ( pole_status->get_enumeration() )
    {
    case pole::PS_OK:
        if ( equipment_status->get_enumeration() != LS_CLOSED )
        {
            equipment_status->setp((enumeration)LS_CLOSED);
            NR_admit_change = true;
            verbose("equipment status is now CLOSED");
        }
        break;
    case pole::PS_FAILED:
        if ( equipment_status->get_enumeration() != LS_OPEN )
        {
            equipment_status->setp((enumeration)LS_OPEN);
            NR_admit_change = true;
            verbose("equipment status is now OPEN");
        }
        break;
    default:
        error("pole_status %d is not valid",(int)pole_status->get_enumeration());
        break;
    }

    //  - pole_mount    get initial equipment status
    pole *mount = OBJECTDATA(my()->parent,pole);

    if ( equipment_is_line )
    {
        verbose("line weight = %g lbs",weight);
        verbose("line_moment_weight = %g ft*lb (due to the weight of conductor and ice)",line_moment_weight);
        verbose("line strength = %g lb", strength);
        verbose("line_moment_tension = %g ft*lb (moments due to line tension)", line_moment_tension); // moment due to conductor tension
        verbose("line_load_nowind = %g lb",line_load_nowind);
        verbose("load_moment_nowind = %g ft*lb (wind load is 1 lb/sf)", line_moment_nowind);
	}
    else
    {
        pole_configuration *pole_config = OBJECTDATA(mount->get_configuration(),pole_configuration);
        equipment_moment_nowind = area*(equipment_z_tilt-mount->get_guy_height())*pole_config->overload_factor_transverse_general;
        verbose("equipment_moment_nowind = %g ft*lb (wind load is 1 lb/sf)",equipment_moment_nowind);

        equipment_moment_weight = weight * offset_tilt * pole_config->overload_factor_vertical;
        verbose("equipment_moment_weight = %g ft*lb",equipment_moment_weight);
    }

    return TS_NEVER;
}

TIMESTAMP pole_mount::presync(TIMESTAMP t0)
{
    //  - pole_mount    set interim equipment status
    pole *mount = OBJECTDATA(my()->parent,pole);
    wind_speed = mount->get_wind_speed();
    wind_direction = mount->get_wind_direction();
    return TS_NEVER;
}

TIMESTAMP pole_mount::sync(TIMESTAMP t0)
{
    //  - pole_mount    update moment accumulators
    pole *mount = OBJECTDATA(my()->parent,pole);
    double theta = direction*PI/180;
    if ( mount->recalc )
    {
        if ( equipment_is_line )
        {
            line_moment_wind = line_moment_nowind * 0.00256 * (2.24*wind_speed) * (2.24*wind_speed) * abs(sin((wind_direction-direction)*PI/180)); // 2.24 account for m/s to mph conversion
            verbose("wire wind_speed = %g m/s",wind_speed);
            verbose("line_moment_wind = %g ft*lb",line_moment_wind); // the line moment due to wind
            mount->set_wire_wind(mount->get_wire_wind() + line_moment_wind ); // moment due to wind load on wires
            mount->set_wire_load_nowind(mount->get_wire_load_nowind() + line_load_nowind); 
            mount->set_wire_weight(mount->get_wire_weight() + line_moment_weight); // moment due to conductor weight
            mount->set_wire_moment_nowind(mount->get_wire_moment_nowind() + line_moment_nowind);
            mount->set_wire_tension(mount->get_wire_tension() + line_moment_tension); // moment due to conductor tension
            double x = ( line_moment_tension + line_moment_wind ) * cos(theta);
            double y = ( line_moment_tension + line_moment_wind ) * sin(theta);
            verbose("wire moment tension and wind x = %g ft*lb",x);
            verbose("wire moment tension and wind y = %g ft*lb",y);
            verbose("line_moment_weight = %g ft*lb",line_moment_weight);
            mount->set_wire_moment_x(mount->get_wire_moment_x() + x); // wire moment in x-axis due to tension and wind load
            mount->set_wire_moment_y(mount->get_wire_moment_y() + y); // wire moment in x-axis due to tension and wind load
            mount->set_wire_moment_z(mount->get_wire_moment_z() + line_moment_weight); // wire moment in z-axis due to weight
        }
        else
        {
            verbose("%s recalculation flag set",my()->parent->name);
            mount->set_equipment_moment_nowind(mount->get_equipment_moment_nowind()+equipment_moment_nowind);
            double equipment_moment_wind = 0.00256*(2.24*wind_speed)*(2.24*wind_speed)*equipment_moment_nowind;
            verbose("equipment_moment_wind = %g ft*lb",equipment_moment_wind);
            verbose("equipment wind_speed = %g m/s",wind_speed);
            double x = equipment_moment_wind * cos(wind_direction*PI/180);
            double y = equipment_moment_wind * sin(wind_direction*PI/180);
            verbose("equipment_moment_wind x = %g ft*lb",x);
            verbose("equipment_moment_wind y = %g ft*lb",y);
            verbose("equipment_moment_weight = %g ft*lb",equipment_moment_weight);
            mount->set_equipment_weight(mount->get_equipment_weight() + equipment_moment_weight);
            mount->set_equipment_moment_x(mount->get_equipment_moment_x() + x); // equipment moment in x-axis due to pole tilt and wind load
            mount->set_equipment_moment_y(mount->get_equipment_moment_y() + y); // equipment moment in y-axis due to pole tilt and wind load
            mount->set_equipment_moment_z(mount->get_equipment_moment_z() + equipment_moment_weight); // equipment moment in z-axis due to pole tilt and weight
        }
    }
    else
    {
        verbose("pole recalculation not flagged");
    }
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
