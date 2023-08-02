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

            // PT_double, "heading_direction[deg]", get_direction_offset(),
            //     PT_DEFAULT, "0.0 deg",
            //     PT_DESCRIPTION, "direction at which the powerline pulls away from the pole",

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
			snprintf(msg,sizeof(msg)-1, "unable to publish properties in %s",__FILE__);
			throw msg;
        }
    }
}

// double pole_diameter(double g_diameter,double t_diameter, double h, double H)
// {
//   double h_diameter = g_diameter - h*(g_diameter-t_diameter)/H;
//   return h_diameter;
// }

int pole_mount::create(void)
{
	return 1;
}

int pole_mount::init(OBJECT *parent)
{
    // Input validation
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
        // An array containing the 3 phases and neutral line connected to the pole. 
        // If a pole does not have a given line connected to it, that will be NULL in the array. ?? Double Check this.
        class overhead_line_conductor *conductors[] = {
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
        double pole_tilt = mount->get_tilt_degree()*PI/180;
        double D1 = pole_config->top_diameter/12;
        double D0 = pole_config->ground_diameter/12;
        double H = pole_config->pole_length - pole_config->pole_depth; // height above ground
        // E means Earth, so distance_#toE means distance from that wire to the ground in feet:
        double radius_A = D0 - spacing->distance_AtoE *(D0-D1)/H;
        double radius_B = D0 - spacing->distance_BtoE *(D0-D1)/H;
        double radius_C = D0 - spacing->distance_CtoE *(D0-D1)/H;
        double radius_N = D0 - spacing->distance_NtoE *(D0-D1)/H;
        // The sum of the weights of all the conductors on the pole.
        // Uses the ternary operator (condition ? execute_on_true : execute_on_false) 
        //     to check if each conductor exists, uses the weight if so, zero if not.
        weight = (
            (conductors[0]?conductors[0]->get_cable_weight():0.0) + // cable_weight is in lbs/ft
            (conductors[1]?conductors[1]->get_cable_weight():0.0) +
            (conductors[2]?conductors[2]->get_cable_weight():0.0) +
            (conductors[3]?conductors[3]->get_cable_weight():0.0) ) * pole_spacing / 2;
        verbose("cable weight = %g lbs (half)",weight);
        if (pole_tilt == 0.0)
        {
            // when the pole is not tilted, the weight moment is cable_weight*radius 
            wire_moment_weight = pole_config->overload_factor_vertical * (
                radius_A*(conductors[0]?conductors[0]->get_cable_weight() + 
                    0.39744*PI* (conductors[0]->cable_diameter + line->get_ice_thickness()) *line->get_ice_thickness():0.0) +
                radius_B*(conductors[1]?conductors[1]->get_cable_weight() +
                    0.39744*PI*(conductors[1]->cable_diameter+line->get_ice_thickness())*line->get_ice_thickness():0.0) +
                radius_C * (conductors[2]?conductors[2]->get_cable_weight() +
                    0.39744*PI*(conductors[2]->cable_diameter+line->get_ice_thickness())*line->get_ice_thickness():0.0) +
                radius_N * (conductors[3]?conductors[3]->get_cable_weight() +
                    0.39744*PI*(conductors[3]->cable_diameter+line->get_ice_thickness())*line->get_ice_thickness():0.0) 
                ) * pole_spacing / 2; // 1/2 works for poles at the same elevation level, 0.39744 is ice density in lbs/in2ft
        }
        else 
        {
            // when the pole is tilted, the weight moment is part of cable_weight*radius and cable_weight*mount_hight
            // everything below the guyed height is IGNORED for now
            wire_moment_weight = pole_config->overload_factor_vertical * ((
                radius_A*(conductors[0]?conductors[0]->get_cable_weight()+0.39744*PI*(
                    conductors[0]->cable_diameter+line->get_ice_thickness())*line->get_ice_thickness():0.0)
                + radius_B*(conductors[0]?conductors[0]->get_cable_weight()+0.39744*PI*(
                    conductors[0]->cable_diameter+line->get_ice_thickness())*line->get_ice_thickness():0.0)
                + radius_C*(conductors[0]?conductors[0]->get_cable_weight()+0.39744*PI*(
                    conductors[0]->cable_diameter+line->get_ice_thickness())*line->get_ice_thickness():0.0)
                + radius_N*(conductors[0]?conductors[0]->get_cable_weight()+0.39744*PI*(
                    conductors[0]->cable_diameter+line->get_ice_thickness())*line->get_ice_thickness():0.0) 
                ) * cos(pole_tilt) + (
                (spacing->distance_AtoE-mount->get_guy_height())*(conductors[0]?conductors[0]->get_cable_weight()+0.39744*PI*(
                    conductors[0]->cable_diameter+line->get_ice_thickness())*line->get_ice_thickness():0.0)
                +(spacing->distance_BtoE-mount->get_guy_height())*(conductors[0]?conductors[0]->get_cable_weight()+0.39744*PI*(
                    conductors[0]->cable_diameter+line->get_ice_thickness())*line->get_ice_thickness():0.0)
                +(spacing->distance_CtoE-mount->get_guy_height())*(conductors[0]?conductors[0]->get_cable_weight()+0.39744*PI*(
                    conductors[0]->cable_diameter+line->get_ice_thickness())*line->get_ice_thickness():0.0)
                +(spacing->distance_NtoE-mount->get_guy_height())*(conductors[0]?conductors[0]->get_cable_weight()+0.39744*PI*(
                    conductors[0]->cable_diameter+line->get_ice_thickness())*line->get_ice_thickness():0.0)
                ) * sin(pole_tilt)) * pole_spacing / 2; // 1/2 works for poles at the same elevation level
        }
        verbose("wire_moment_weight = %g ft*lb (due to the weight of conductors and ice)",wire_moment_weight);
        double transverse_load_per_wind = 1 * (
            (conductors[0]?conductors[0]->cable_diameter+2*line->get_ice_thickness():0.0) +
            (conductors[1]?conductors[1]->cable_diameter+2*line->get_ice_thickness():0.0) +
            (conductors[2]?conductors[2]->cable_diameter+2*line->get_ice_thickness():0.0) +
            (conductors[3]?conductors[3]->cable_diameter+2*line->get_ice_thickness():0.0)
            ) / 12; // unit transverse load on cables when wind load is 1 lb/ft^2
        verbose("transverse_load_per_wind = %g lb/ft (wind load is 1 lb/sf)",transverse_load_per_wind);
        line_load_per_wind = transverse_load_per_wind * pole_spacing;
        verbose("line_load_per_wind = %g lb (wind load is 1 lb/sf)",line_load_per_wind);
        // Moment on the pole due to wind on wires = wind pressure * wire_moment_per_wind 
        // wire_moment_per_wind = wind cross section * height * fractional component of wind force that acts to increase pole tilt
        //      for every conductor attached.
        // wind cross section = wire length * conductor diameter + 2 * ice radius
        //                      wire length (wind span) is approximated as pole_spacing
        // The greater the pole tilt, the smaller the component of wind force oriented perpendicular to the pole, 
        //      pulling it down. Hence: cos(pole_tilt).
        // Heights are calculated from above guy height.
        wire_moment_per_wind = 1 * pole_spacing * cos(pole_tilt) * pole_config->overload_factor_transverse_wire * (
        (spacing->distance_AtoE - mount->get_guy_height()) * (conductors[0]?conductors[0]->cable_diameter+2*line->get_ice_thickness():0.0) +
        (spacing->distance_BtoE - mount->get_guy_height()) * (conductors[1]?conductors[1]->cable_diameter+2*line->get_ice_thickness():0.0) +
        (spacing->distance_CtoE - mount->get_guy_height()) * (conductors[2]?conductors[2]->cable_diameter+2*line->get_ice_thickness():0.0) +
        (spacing->distance_NtoE - mount->get_guy_height()) * (conductors[3]?conductors[3]->cable_diameter+2*line->get_ice_thickness():0.0)
        ) / 12; // Unit conversion because cable diameter and ice thickness is in inches.
        verbose("load_moment_per_wind = %g ft*lb (wind load is 1 lb/sf)", wire_moment_per_wind);
        strength = (conductors[0]?conductors[0]->get_cable_strength():0.0)
            + (conductors[1]?conductors[1]->get_cable_strength():0.0)
            + (conductors[2]?conductors[2]->get_cable_strength():0.0)
            + (conductors[3]?conductors[3]->get_cable_strength():0.0);
        verbose("strength = %g lb", strength);
        wire_moment_tension = pole_config->overload_factor_transverse_wire * cos(pole_tilt) * (
            (spacing->distance_AtoE - mount->get_guy_height()) * (conductors[0]?conductors[0]->get_cable_strength():0.0) + 
            (spacing->distance_BtoE - mount->get_guy_height()) * (conductors[1]?conductors[1]->get_cable_strength():0.0) + 
            (spacing->distance_CtoE - mount->get_guy_height()) * (conductors[2]?conductors[2]->get_cable_strength():0.0) + 
            (spacing->distance_NtoE - mount->get_guy_height()) * (conductors[3]?conductors[3]->get_cable_strength():0.0));
        verbose("wire_moment_tension = %g ft*lb (moments due to line tension)",wire_moment_tension); // moment due to conductors tension
    }
    else
    {
        // equipment
        verbose("equipment = %s",get_equipment()->name);
        pole *mount = OBJECTDATA(my()->parent,pole);
        pole_configuration *pole_config = OBJECTDATA(mount->get_configuration(),pole_configuration);
        double pole_tilt = mount->get_tilt_degree()*PI/180;
        verbose("equipment area = %g sf",area);
        verbose("equipment weight = %g lbs",weight);
        // the first term is contributed by the weight perpendicular to the pole and the second term is weight moment
        equipment_offset = (height - mount->get_guy_height()) * sin(pole_tilt) + offset*cos(pole_tilt); 
        verbose("equipment_offset = %g (measured in ft)",equipment_offset);
        equipment_moment_weight = weight * equipment_offset * pole_config->overload_factor_vertical;
        verbose("equipment_moment_weight = %g ft*lb",equipment_moment_weight);
        equipment_load_per_wind = 1 * area * cos(pole_tilt);
        verbose("equipment_load_per_wind = %g lb (wind load is 1 lb/sf)",equipment_load_per_wind);
        equipment_moment_per_wind = (height-mount->get_guy_height())*pole_config->overload_factor_transverse_general*equipment_load_per_wind;
        verbose("equipment_moment_per_wind = %g ft*lb (wind load is 1 lb/sf)", equipment_moment_per_wind);
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

    //  - pole_mount    get initial status    
    if ( equipment_is_line )
    {
        verbose("line weight = %g lbs",weight);
        verbose("wire_moment_weight = %g ft*lb (due to the weight of conductors and ice)",wire_moment_weight);
        verbose("line strength = %g lb", strength);
        verbose("wire_moment_tension = %g ft*lb (moments due to line tension)", wire_moment_tension); // moment due to conductors tension
        verbose("line_load_per_wind = %g lb",line_load_per_wind);
        verbose("load_moment_per_wind = %g ft*lb (wind load is 1 lb/sf)", wire_moment_per_wind);
	}
    else
    {
        verbose("equipment weight = %g lbs",weight);
        verbose("equipment_moment_weight = %g ft*lb (due to the weight of conductors and ice)",equipment_moment_weight);
        verbose("equipment_load_per_wind = %g lb",equipment_load_per_wind);
        verbose("equipment_moment_per_wind = %g ft*lb (wind load is 1 lb/sf)", equipment_moment_per_wind);
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
    double pole_tilt = mount->get_tilt_degree()*PI/180;
    double pole_tilt_dir = mount->get_tilt_direction()*PI/180;
    double mount_dir = direction*PI/180; // direction from pole centerline at which equipment is mounted.
    if ( mount->recalc )
    {
        verbose("%s recalculation flag set",my()->parent->name);
        double wind_pressure = 0.00256 * (2.24*wind_speed) * (2.24*wind_speed); // 2.24 account for m/s to mph conversion
        verbose("pole_mount wind_speed = %g m/s",wind_speed);
        if ( equipment_is_line )
        {
            // wire_moment_per_wind represents the moment on the pole due to wind on the lines per unit wind pressure
            // (i.e. cross-sectional area * torque radius)
            wire_moment_wind = wire_moment_per_wind * wind_pressure * abs(sin((wind_direction-direction)*PI/180)); 
            verbose("wire_moment_wind = %g ft*lb",wire_moment_wind);
            mount->set_wire_wind(mount->get_wire_wind() + wire_moment_wind ); // moment due to wind load on wires
            mount->set_wire_load_per_wind(mount->get_wire_load_per_wind() + line_load_per_wind); 
            mount->set_wire_weight(mount->get_wire_weight() + wire_moment_weight); // moment due to conductors weight
            mount->set_wire_moment_per_wind(mount->get_wire_moment_per_wind() + wire_moment_per_wind);
            mount->set_wire_tension(mount->get_wire_tension() + wire_moment_tension); // moment due to conductors tension
            double x = ( wire_moment_tension + wire_moment_wind ) * cos(mount_dir);
            double y = ( wire_moment_tension + wire_moment_wind ) * sin(mount_dir);
            if ( pole_tilt > 0.0 )
            {
                x += wire_moment_weight * cos(pole_tilt_dir);
                y += wire_moment_weight * sin(pole_tilt_dir);
            }
            else
            {
                x += wire_moment_weight * cos(mount_dir);
                y += wire_moment_weight * sin(mount_dir);
            }
            verbose("wire moment (tension, wind and weight) x = %g ft*lb",x);
            verbose("wire moment (tension, wind and weight) y = %g ft*lb",y);
            mount->set_wire_moment_x(mount->get_wire_moment_x() + x); // wire moment in x-axis due to tension and wind load
            mount->set_wire_moment_y(mount->get_wire_moment_y() + y); // wire moment in x-axis due to tension and wind load
        }
        else
        {   
            mount->set_equipment_moment_per_wind(mount->get_equipment_moment_per_wind()+equipment_moment_per_wind);
            equipment_moment_wind = wind_pressure*equipment_moment_per_wind;
            verbose("equipment_moment_wind = %g ft*lb",equipment_moment_wind);
            double x = equipment_moment_wind * cos(wind_direction*PI/180);
            double y = equipment_moment_wind * sin(wind_direction*PI/180);
            if ( pole_tilt > 0.0 )
            {
                x += equipment_moment_weight * cos(pole_tilt_dir);
                y += equipment_moment_weight * sin(pole_tilt_dir);
            }
            else
            {
                x += equipment_moment_weight * cos(mount_dir);
                y += equipment_moment_weight * sin(mount_dir);
            }
            verbose("equipment_moment_wind x = %g ft*lb",x);
            verbose("equipment_moment_wind y = %g ft*lb",y);
            mount->set_equipment_weight(mount->get_equipment_weight() + equipment_moment_weight);
            mount->set_equipment_moment_x(mount->get_equipment_moment_x() + x); // equipment moment in x-axis due to pole tilt and wind load
            mount->set_equipment_moment_y(mount->get_equipment_moment_y() + y); // equipment moment in y-axis due to pole tilt and wind load
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
