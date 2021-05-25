// powerflow/pole.cpp
// Copyright (C) 2018, Regents of the Leland Stanford Junior University
//
// Processing sequence for pole failure analysis:
//
// Commit (random):
//  - pole          update weather/degredation/resisting moment
//  - pole_mount    get initial equipment status
//
// Presync (top-down):
//  - pole          initialize moment accumulators,
//  - pole_mount    set interim equipment status
//
// Sync (bottom-up)
//  - pole_mount    update moment accumulators
//  - pole          (nop)
//
// Postsync (top-down):
//  - pole          calculate total moment and failure status
//  - pole_mount    set interim equipment status
//
// Commit (random):
//  - pole          finalize pole status
//  - pole_mount    finalize equipment status
//

#include "powerflow.h"
using namespace std;

EXPORT_CREATE(pole)
EXPORT_INIT(pole)
EXPORT_PRECOMMIT(pole)
EXPORT_SYNC(pole)
EXPORT_COMMIT(pole)

CLASS *pole::oclass = NULL;
CLASS *pole::pclass = NULL;
pole *pole::defaults = NULL;

static char32 wind_speed_name = "wind_speed";
static char32 wind_dir_name = "wind_dir";
static char32 wind_gust_name = "wind_gust";
static double default_repair_time = 24.0;

pole::pole(MODULE *mod)
{
	if ( oclass == NULL )
	{
		pclass = node::oclass;
		oclass = gl_register_class(mod,"pole",sizeof(pole),PC_PRETOPDOWN|PC_BOTTOMUP|PC_POSTTOPDOWN|PC_UNSAFE_OVERRIDE_OMIT|PC_AUTOLOCK);
		if ( oclass == NULL )
			throw "unable to register class pole";
		oclass->trl = TRL_PROTOTYPE;
		if ( gl_publish_variable(oclass,

			PT_enumeration, "status", get_pole_status_offset(),
                PT_KEYWORD, "OK", (enumeration)PS_OK,
                PT_KEYWORD, "FAILED", (enumeration)PS_FAILED,
                PT_DEFAULT, "OK",
                PT_DESCRIPTION, "pole status",

            PT_double, "tilt_angle[deg]", get_tilt_angle_offset(),
                PT_DEFAULT, "0.0 deg",
                PT_DESCRIPTION, "tilt angle of pole",

            PT_double, "tilt_direction[deg]", get_tilt_direction_offset(),
                PT_DEFAULT, "0.0 deg",
                PT_DESCRIPTION, "tilt direction of pole",

            PT_object, "weather", get_weather_offset(),
                PT_DESCRIPTION, "weather data",

            PT_object, "configuration", get_configuration_offset(),
                PT_REQUIRED,
                PT_DESCRIPTION, "configuration data",

            PT_int32, "install_year", PADDR(install_year),
                PT_REQUIRED,
                PT_DESCRIPTION, "the year of pole was installed",

            PT_double, "repair_time[h]", PADDR(repair_time),
                PT_DESCRIPTION, "typical repair time after pole failure",

            PT_double, "wind_speed[m/s]", get_wind_speed_offset(),
                PT_DEFAULT, "0 m/s",
                PT_DESCRIPTION, "local wind speed",

            PT_double, "wind_direction[deg]", get_wind_direction_offset(),
                PT_DEFAULT, "0 deg",
                PT_DESCRIPTION, "local wind direction",

            PT_double, "wind_gusts[m/s]", get_wind_gusts_offset(),
                PT_DEFAULT, "0 m/s",
                PT_DESCRIPTION, "local wind gusts",

            PT_double, "pole_stress[pu]", PADDR(pole_stress),
                PT_OUTPUT,
                PT_DESCRIPTION, "ratio of actual stress to critical stress",

            PT_double, "pole_stress_polynomial_a[ft*lb]", PADDR(pole_stress_polynomial_a),
                PT_OUTPUT,
                PT_DESCRIPTION, "constant a of the pole stress polynomial function",
            PT_double, "pole_stress_polynomial_b[ft*lb]", PADDR(pole_stress_polynomial_b),
                PT_OUTPUT,
                PT_DESCRIPTION, "constant b of the pole stress polynomial function",

            PT_double, "pole_stress_polynomial_c[ft*lb]", PADDR(pole_stress_polynomial_c),
                PT_OUTPUT,
                PT_DESCRIPTION, "constant c of the pole stress polynomial function",

            PT_double, "susceptibility[pu*s/m]", PADDR(susceptibility),
                PT_OUTPUT,
                PT_DESCRIPTION, "susceptibility of pole to wind stress (derivative of pole stress w.r.t wind speed)",

            PT_double, "total_moment[ft*lb]", PADDR(total_moment),
                PT_OUTPUT,
                PT_DESCRIPTION, "the total moment on the pole",

            PT_double, "resisting_moment[ft*lb]", PADDR(resisting_moment),
                PT_OUTPUT,
                PT_DESCRIPTION, "the resisting moment on the pole",

            PT_double, "pole_moment[ft*lb]", PADDR(pole_moment),
                PT_OUTPUT,
                PT_DESCRIPTION, "the moment of the pole",

            PT_double, "pole_moment_nowind[ft*lb]", PADDR(pole_moment_nowind),
                PT_OUTPUT,
                PT_DESCRIPTION, "the moment of the pole without wind",

            PT_double, "equipment_moment[ft*lb]", PADDR(equipment_moment),
                PT_OUTPUT,
                PT_DESCRIPTION, "the moment of the equipment",

            PT_double, "equipment_moment_nowind[ft*lb]", PADDR(equipment_moment_nowind),
                PT_OUTPUT,
                PT_DESCRIPTION, "the moment of the equipment without wind",

            PT_double, "critical_wind_speed[m/s]", PADDR(critical_wind_speed),
                PT_OUTPUT,
                PT_DESCRIPTION, "wind speed at pole failure",

            NULL) < 1 ) throw "unable to publish properties in " __FILE__;
		gl_global_create("powerflow::repair_time[h]",PT_double,&default_repair_time,NULL);
        gl_global_create("powerflow::wind_speed_name",PT_char32,&wind_speed_name,NULL);
        gl_global_create("powerflow::wind_dir_name",PT_char32,&wind_dir_name,NULL);
        gl_global_create("powerflow::wind_gust_name",PT_char32,&wind_gust_name,NULL);
	}
}

void pole::reset_accumulators()
{
	equipment_moment = 0.0;
	equipment_moment_nowind = 0.0;
	wire_load = 0.0;
	wire_load_nowind = 0.0;
	wire_moment = 0.0;
	wire_moment_nowind = 0.0;
	wire_tension = 0.0;
    total_moment = 0.0;
    wind_pressure = 0.0;
    pole_moment = 0.0;
    pole_moment_nowind = 0.0;
    pole_stress = 0.0;
    critical_wind_speed = 0.0;
    susceptibility = 0.0;
}

int pole::create(void)
{
	configuration = NULL;
	is_deadend = FALSE;
	config = NULL;
	last_wind_speed = 0.0;
	last_wind_speed = 0.0;
	down_time = TS_NEVER;
	current_hollow_diameter = 0.0;
    reset_accumulators();
    wind_speed_ref = NULL;
    wind_direction_ref = NULL;
    wind_gusts_ref = NULL;
	return 1;
}

int pole::init(OBJECT *parent)
{
	// configuration
	if ( configuration == NULL || ! gl_object_isa(configuration,"pole_configuration") )
	{
		error("configuration is not set to a pole_configuration object");
		return 0;
	}
	config = OBJECTDATA(configuration,pole_configuration);

	if ( repair_time <= 0.0 )
	{
		double *pRepairTime = (double*)gl_get_addr(configuration, "repair_time");
		if ( pRepairTime && *pRepairTime > 0 )
		{
			repair_time = *pRepairTime;
		}
		else if ( default_repair_time > 0 )
		{
			repair_time = default_repair_time;
		}
		else
		{
			exception("pole::default_repair_time must be positive");
		}
	}

    // weather check
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

        wind_gusts_ref = new gld_property(weather,(const char*)wind_gust_name);
        if ( ! wind_gusts_ref->is_valid() )
        {
            warning("weather data does not include %s, using local wind %s data only",(const char*)wind_gust_name,"gusts");
            delete wind_gusts_ref;
            wind_gusts_ref = NULL;
        }
        else if ( wind_gusts != 0.0 )
        {
            warning("weather data will overwrite local wind %s data","gusts");
        }
    }

	// tilt
	if ( tilt_angle < 0 || tilt_angle > 90 )
	{
		error("pole tilt angle is not between and 0 and 90 degrees");
		return 0;
	}
	if ( tilt_direction < 0 || tilt_direction >= 360 )
	{
		error("pole tilt direction is not between 0 and 360 degrees");
		return 0;
	}

	// calculation resisting moment
	resisting_moment = 0.008186
		* config->strength_factor_250b_wood
		* config->fiber_strength
		* ( config->ground_diameter * config->ground_diameter * config->ground_diameter);
	verbose("resisting moment %.0f ft*lb",resisting_moment);

	double pole_height = config->pole_length - config->pole_depth;

	pole_moment_nowind = pole_height * pole_height * (config->ground_diameter+2*config->top_diameter)/72 * config->overload_factor_transverse_general;

	if ( install_year > gl_globalclock )
		warning("pole install year in the future are assumed to be current time");

	return 1;
}

TIMESTAMP pole::precommit(TIMESTAMP t0)
{
    resisting_moment = 0.0;

    // update pole degradation model
	if ( install_year > 0 )
	{
		double t0_year = 1970 + (int)(t0/86400/365.24);
		double age = t0_year - install_year;
		if ( age > 0 )
			current_hollow_diameter = 2.0 * age * config->degradation_rate;
		else
			current_hollow_diameter = 0.0; // ignore future installation years
	}

    // calculation resisting moment
	resisting_moment = 0.008186 // constant * pi^3
		* config->strength_factor_250b_wood
		* config->fiber_strength
		* (( config->ground_diameter * config->ground_diameter * config->ground_diameter)
			- (current_hollow_diameter * current_hollow_diameter * current_hollow_diameter));
	verbose("resisting moment %.0f ft*lb",resisting_moment);

    return ( pole_status == PS_FAILED ? down_time + (int)(repair_time*3600) : TS_NEVER );
}

TIMESTAMP pole::presync(TIMESTAMP t0)
{
    // wind data
    if ( wind_speed_ref )
    {
        wind_speed = wind_speed_ref->get_double();
    }
    if ( wind_direction_ref )
    {
        wind_direction = wind_direction_ref->get_double();
    }
    if ( wind_gusts_ref )
    {
        wind_gusts = wind_gusts_ref->get_double();
    }

    reset_accumulators();

	return TS_NEVER;
}

TIMESTAMP pole::sync(TIMESTAMP t0)
{
    //  - pole          (nop)
	return TS_NEVER;
}

TIMESTAMP pole::postsync(TIMESTAMP t0)
{
    //  - pole          calculate total moment and failure status
    if ( pole_status == PS_FAILED && (gl_globalclock-down_time)/3600.0 > repair_time )
	{
		gl_debug("pole repaired");
		tilt_angle = 0.0;
		tilt_direction = 0.0;
		pole_status = PS_OK;
		install_year = 1970 + (unsigned int)(t0/86400/365.24);
	}
	if ( pole_status == PS_OK && last_wind_speed != wind_speed )
	{
		gld_clock dt;
		wind_pressure = 0.00256*2.24 * (wind_speed)*(wind_speed); //2.24 account for m/s to mph conversion
		double pole_height = config->pole_length - config->pole_depth;
        if ( tilt_angle > 0.0 )
        {
            double D1_3 = config->top_diameter*config->top_diameter*config->top_diameter/1728;
            double D0_3 = config->ground_diameter*config->ground_diameter*config->ground_diameter/1728;
            double H_4 = pole_height*pole_height*pole_height*pole_height;
            pole_moment = PI/96*config->material_density*H_4*9.8*sin(tilt_angle/180*PI)*abs(D0_3-D1_3) * 0.13/3.28; // convert metric parts to english
            // // crude calculation that should be close to above results for small tilt angles
            // double r0 = config->top_diameter/2;
            // double r1 = config->ground_diameter/2;
            // double r2 = (r0+r1)/2;
            // pole_moment = (0.5*sin(tilt_angle/180*PI)*pole_height)*(config->material_density*PI*(r2*r2)*9.8*pole_height);
        }
        else
        {
            pole_moment = 0.0;
        }
        if ( wind_pressure > 0.0 )
        {
            double beta = (tilt_direction-wind_direction)/180*PI; // wind angle on pole
            double wind_moment = wind_pressure * pole_height * pole_height * (config->ground_diameter/12+2*config->top_diameter/12)/72 * config->overload_factor_transverse_general;
            pole_moment = abs(pole_moment + abs(wind_moment*sin(beta)) + wind_moment*cos(beta));
        }

		total_moment = pole_moment + equipment_moment + wire_moment + wire_tension;
		pole_stress = total_moment/resisting_moment;
		if ( wind_speed > 0.0 )
			susceptibility = 2*(pole_moment+equipment_moment+wire_moment)/resisting_moment/(wind_speed)/(0.00256)/(2.24);
		else
			susceptibility = 0.0;
		verbose("%s: wind %4.1f psi, pole %4.0f ft*lb, equipment %4.0f ft*lb, wires %4.0f ft*lb, stress %.0f%%",
			(const char*)(dt.get_string()), wind_pressure, pole_moment, equipment_moment, wire_moment, pole_stress*100);
		pole_status = ( pole_stress < 1.0 ? PS_OK : PS_FAILED );
		if ( pole_status == PS_FAILED )
		{
			debug("pole failed at %.0f%% loading, time to repair is %g h",pole_stress*100,repair_time);
			down_time = gl_globalclock;
		}
		last_wind_speed = wind_speed;

        pole_moment_nowind = pole_height * pole_height * (config->ground_diameter+2*config->top_diameter)/72 * config->overload_factor_transverse_general;
		double wind_pressure_failure = (resisting_moment - wire_tension) / (pole_moment_nowind + equipment_moment_nowind + wire_moment_nowind);
		critical_wind_speed = sqrt(wind_pressure_failure / (0.00256 * 2.24));
	}

	pole_stress_polynomial_a = pole_moment_nowind+equipment_moment_nowind+wire_moment_nowind;
	pole_stress_polynomial_b = 0.0;
	pole_stress_polynomial_c = wire_tension;

	return TS_NEVER;
}

TIMESTAMP pole::commit(TIMESTAMP t1, TIMESTAMP t2)
{
    //  - pole          finalize pole status
    // TODO
	return TS_NEVER;
}
