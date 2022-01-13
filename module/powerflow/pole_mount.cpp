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

            PT_double, "area[sf]", get_area_offset(),
                PT_DEFAULT, "0.0 sf",
                PT_DESCRIPTION, "cross sectional area of equipment",

            PT_double, "direction[deg]", get_direction_offset(),
                PT_DEFAULT, "0.0 deg",
                PT_DESCRIPTION, "direction from pole centerline at which equipment is mounted",

            PT_double, "weight[lb]", get_weight_offset(),
                PT_DEFAULT, "0.0 lb",
                PT_DESCRIPTION, "weight of equipment mounted",

            PT_double, "pole_spacing[ft]", get_pole_spacing_offset(),
                PT_DEFAULT, "0.0 ft",
                PT_DESCRIPTION, "Mean spacing between poles adjacent",

            PT_double, "angle[deg]", get_angle_offset(),
                PT_DEFAULT, "0.0 deg",
                PT_DESCRIPTION, "Angle of the pole line",

            PT_object, "weather", get_weather_offset(),
                PT_DESCRIPTION, "weather data",

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
        pole_configuration *pole_config = OBJECTDATA(mount->get_configuration(),pole_configuration);
        line_moment = pole_config->overload_factor_vertical * (
            spacing->distance_AtoE * (conductor[0]?conductor[0]->get_cable_weight():0.0) +
            spacing->distance_BtoE * (conductor[1]?conductor[1]->get_cable_weight():0.0) +
            spacing->distance_CtoE * (conductor[2]?conductor[2]->get_cable_weight():0.0) +
            spacing->distance_NtoE * (conductor[3]?conductor[3]->get_cable_weight():0.0) ) * pole_spacing;
        verbose("line_moment = %g ft*lb",line_moment);
        weight = (
            (conductor[0]?conductor[0]->get_cable_weight():0.0) +
            (conductor[1]?conductor[1]->get_cable_weight():0.0) +
            (conductor[2]?conductor[2]->get_cable_weight():0.0) +
            (conductor[3]?conductor[3]->get_cable_weight():0.0) ) * pole_spacing;
        verbose("weight = %g lbs",weight);
        double transverse_load = 1 * (
            (conductor[0]?conductor[0]->cable_diameter:0.0) +
            (conductor[1]?conductor[1]->cable_diameter:0.0) +
            (conductor[2]?conductor[2]->cable_diameter:0.0) +
            (conductor[3]?conductor[3]->cable_diameter:0.0) +
            2*line->get_ice_thickness() ) / 12; // unit transverse load when wind load is 1lb/ft^2
        verbose("transverse_load = %g lb/ft (wind load is 1 lb/sf)",transverse_load);
        line_moment_nowind = 1 * pole_spacing * pole_config->overload_factor_transverse_wire * (
        spacing->distance_AtoE * ((conductor[0]?conductor[0]->cable_diameter:0.0)+2*line->get_ice_thickness()) +
        spacing->distance_BtoE * ((conductor[1]?conductor[1]->cable_diameter:0.0)+2*line->get_ice_thickness()) +
        spacing->distance_CtoE * ((conductor[2]?conductor[2]->cable_diameter:0.0)+2*line->get_ice_thickness()) +
        spacing->distance_NtoE * ((conductor[3]?conductor[3]->cable_diameter:0.0)+2*line->get_ice_thickness())
        ) / 12;
        verbose("load_moment_nowind = %g ft*lb (wind load is 1 lb/sf)", line_moment_nowind);
        double strength = (conductor[0]?conductor[0]->get_cable_strength():0.0)
            + (conductor[1]?conductor[1]->get_cable_strength():0.0)
            + (conductor[2]?conductor[2]->get_cable_strength():0.0)
            + (conductor[3]?conductor[3]->get_cable_strength():0.0);
        verbose("strength = %g lb", strength);
        tension = pole_config->overload_factor_transverse_wire * sin(0.017453*angle/2) * (
            spacing->distance_AtoE * (conductor[0]?conductor[0]->get_cable_strength():0.0) + 
            spacing->distance_BtoE * (conductor[1]?conductor[1]->get_cable_strength():0.0) + 
            spacing->distance_CtoE * (conductor[2]?conductor[2]->get_cable_strength():0.0) + 
            spacing->distance_NtoE * (conductor[3]?conductor[3]->get_cable_strength():0.0));
        verbose("tension = %g ft*lb (moments due to line tension)",tension); // moment due to conductor tension
        line_load_nowind = transverse_load * pole_spacing;
        verbose("line_load_nowind = %g lb (wind load is 1 lb/sf)",line_load_nowind);
    }

    if ( weather )
    {
        wind_speed_ref = new gld_property(weather,(const char*)wind_speed_name);
        if ( ! wind_speed_ref->is_valid() )
        {
            warning("weather data does not include %s, using local wind %s data only",(const char*)wind_speed_name,"speed");
            delete wind_speed_ref;
            wind_speed_ref = NULL;
        }
        else if ( wind_speed != 0.0 )
        {
            warning("weather data will overwrite local wind %s data","speed");
        }
        else
        {
            verbose("wind_speed = %g m/s (ref '%s')", wind_speed, weather->name);
        }

        wind_direction_ref = new gld_property(weather,(const char*)wind_dir_name);
        if ( ! wind_direction_ref->is_valid() )
        {
            warning("weather data does not include %s, using local wind %s data only",(const char*)wind_dir_name,"direction");
            delete wind_direction_ref;
            wind_direction_ref = NULL;
        }
        else if ( wind_direction != 0.0 )
        {
            warning("weather data will overwrite local wind %s data","direction");
        }
        else
        {
            verbose("wind_direction = %g deg (ref '%s')", wind_direction, weather->name);
        }
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
        verbose("weight = %g lbs",weight);
        verbose("tension = %g ft*lb (moments due to line tension)",tension); // moment due to conductor tension
        verbose("line_load_nowind = %g lb",line_load_nowind);
        verbose("load_moment_nowind = %g ft*lb (wind load is 1 lb/sf)", line_moment_nowind);
	}
    else
    {
        pole_configuration *pole_config = OBJECTDATA(mount->get_configuration(),pole_configuration);
        equipment_moment_nowind = 1 * area * ( height - mount->get_guy_height() ) * pole_config->overload_factor_transverse_general;
        verbose("equipment_moment_nowind = %g ft*lb (wind load is 1 lb/sf)",equipment_moment_nowind);

        equipment_moment = weight * offset * pole_config->overload_factor_vertical;
        verbose("equipment_moment = %g ft*lb (no tilt)",equipment_moment);
    }

    return TS_NEVER;
}

TIMESTAMP pole_mount::presync(TIMESTAMP t0)
{
    //  - pole_mount    set interim equipment status
    return TS_NEVER;
}

TIMESTAMP pole_mount::sync(TIMESTAMP t0)
{
    //  - pole_mount    update moment accumulators
    pole *mount = OBJECTDATA(my()->parent,pole);
    pole_configuration *pole_config = OBJECTDATA(mount->get_configuration(),pole_configuration);

    // wind data
    if ( wind_speed_ref )
    {
        wind_speed = wind_speed_ref->get_double();
    }
    if ( wind_direction_ref )
    {
        wind_direction = wind_direction_ref->get_double();
    }

    if ( mount->recalc )
    {
        if ( equipment_is_line )
        {
            verbose("equipment_moment = %g ft*lb",equipment_moment);
            line_load = line_moment_nowind * 0.00256 * (2.24*wind_speed) * (2.24*wind_speed) * sin(abs(wind_direction-direction)*PI/180); // 2.24 account for m/s to mph conversion
            mount->set_wire_load(mount->get_wire_load() + line_load ); // moment due to wind load on wires
            mount->set_wire_load_nowind(mount->get_wire_load_nowind() + line_load_nowind); 
            mount->set_wire_moment(mount->get_wire_moment() + line_moment); // moment due to conductor weight
            mount->set_wire_moment_nowind(mount->get_wire_moment_nowind() + line_moment_nowind);
            mount->set_wire_tension(mount->get_wire_tension() + tension); // moment due to conductor tension
        }
        else
        {
            verbose("%s recalculation flag set",my()->parent->name);
            verbose("equipment_moment = %g ft*lb",equipment_moment);
            verbose("equipment_moment_nowind = %g ft*lb",equipment_moment_nowind);
            mount->set_equipment_moment_nowind(mount->get_equipment_moment_nowind()+equipment_moment_nowind);
            double alpha = mount->get_tilt_angle()*PI/180;
            verbose("alpha = %g rad",alpha);
            double beta = (mount->get_tilt_direction()-direction)*PI/180;
            verbose("beta = %g rad",beta);
            double x = weight * ((height-mount->get_guy_height())*sin(alpha) + offset*cos(beta)) * pole_config->overload_factor_vertical;
            verbose("x = %g ft*lb",x);
            double y = weight * offset * sin(beta) * pole_config->overload_factor_vertical; // moment arm changes when the pole tilts
            verbose("y = %g ft*lb",y);
            double equipment_wind_moment = 0.00256*(2.24*wind_speed)*(2.24*wind_speed)*area*(height-mount->get_guy_height())*pole_config->overload_factor_transverse_general;
            verbose("equipment_wind_moment = %g ft*lb",equipment_wind_moment);
            double moment = sqrt(x*x+y*y) + equipment_wind_moment; 
            verbose("moment = %g ft*lb",moment);
            mount->set_equipment_moment(mount->get_equipment_moment()+moment); // moment due to equipment weight and pole tilt
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
