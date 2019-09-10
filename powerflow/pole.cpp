// powerflow/pole.cpp
// Copyright (C) 2018, Stanford University

#include "powerflow.h"
using namespace std;

CLASS *pole::oclass = NULL;
CLASS *pole::pclass = NULL;

static double default_repair_time = 24.0;

pole::pole(MODULE *mod) : node(mod)
{
	if ( oclass == NULL )
	{
		pclass = node::oclass;
		oclass = gl_register_class(mod,"pole",sizeof(pole),PC_PRETOPDOWN|PC_BOTTOMUP|PC_POSTTOPDOWN|PC_UNSAFE_OVERRIDE_OMIT|PC_AUTOLOCK);
		if ( oclass == NULL )
			throw "unable to register class pole";
		oclass->trl = TRL_PROTOTYPE;
		if ( gl_publish_variable(oclass,
			PT_INHERIT, "node",
			PT_enumeration, "pole_status", PADDR(pole_status), PT_DESCRIPTION, "pole status",
				PT_KEYWORD, "OK", (enumeration)PS_OK,
				PT_KEYWORD, "FAILED", (enumeration)PS_FAILED,
			PT_double, "tilt_angle[rad]", PADDR(tilt_angle), PT_DESCRIPTION, "tilt angle of pole",
			PT_double, "tilt_direction[deg]", PADDR(tilt_direction), PT_DESCRIPTION, "tilt direction of pole",
			PT_object, "weather", PADDR(weather), PT_DESCRIPTION, "weather data",
			PT_object, "configuration", PADDR(configuration), PT_DESCRIPTION, "configuration data",
			PT_double, "equipment_area[sf]", PADDR(equipment_area), PT_DESCRIPTION, "equipment cross sectional area",
			PT_double, "equipment_height[ft]", PADDR(equipment_height), PT_DESCRIPTION, "equipment height on pole",
			PT_double, "pole_stress[pu]", PADDR(pole_stress), PT_DESCRIPTION, "ratio of actual stress to critical stress",
			PT_double, "pole_stress_polynomial_a[ft*lb]", PADDR(pole_stress_polynomial_a), PT_DESCRIPTION, "constant a of the pole stress polynomial function",
			PT_double, "pole_stress_polynomial_b[ft*lb]", PADDR(pole_stress_polynomial_b), PT_DESCRIPTION, "constant b of the pole stress polynomial function",
			PT_double, "pole_stress_polynomial_c[ft*lb]", PADDR(pole_stress_polynomial_c), PT_DESCRIPTION, "constant c of the pole stress polynomial function",
			PT_double, "susceptibility[pu*s/m]", PADDR(susceptibility), PT_DESCRIPTION, "susceptibility of pole to wind stress (derivative of pole stress w.r.t wind speed)",
			PT_double, "total_moment[ft*lb]", PADDR(total_moment), PT_DESCRIPTION, "the total moment on the pole.",
			PT_double, "resisting_moment[ft*lb]", PADDR(resisting_moment), PT_DESCRIPTION, "the resisting moment on the pole.",
			PT_double, "critical_wind_speed[m/s]", PADDR(critical_wind_speed), PT_DESCRIPTION, "wind speed at pole failure",
			PT_int32, "install_year", PADDR(install_year), PT_DESCRIPTION, "the year of pole was installed",
			PT_double, "repair_time[h]", PADDR(repair_time), PT_DESCRIPTION, "typical repair time after pole failure",
			NULL) < 1 ) throw "unable to publish properties in " __FILE__;
		gl_global_create("pole::repair_time[h]",PT_double,&default_repair_time,NULL);
	}
}

int pole::isa(char *classname)
{
	return strcmp(classname,"pole")==0 || node::isa(classname);
}

int pole::create(void)
{
	int res = node::create();

	pole_status = PS_OK;
	tilt_angle = 0.0;
	tilt_direction = 0.0;
	weather = NULL;
	configuration = NULL;
	equipment_area = 0.0;
	equipment_height = 0.0;

	ice_thickness = 0.0;
	resisting_moment = 0.0;
	pole_moment = 0.0;
	equipment_moment = 0.0;
	equipment_moment_nowind = 0.0;
	wire_load = 0.0;
	wire_load_nowind = 0.0;
	wire_moment = 0.0;
	wire_moment_nowind = 0.0;
	wind_pressure = 0.0;
	wire_tension = 0.0;
	pole_stress = 0.0;
	susceptibility = 0.0;
	total_moment = 0.0;
	critical_wind_speed = 0.0;
	configuration = NULL;
	is_deadend = FALSE;

	config = NULL;
	last_wind_speed = 0.0;
	wind_speed = NULL;
	wind_direction = NULL;
	wind_gust = NULL;
	last_wind_speed = 0.0;
	wire_data = new std::list<WIREDATA>;
	down_time = TS_NEVER;
	equipment_area = 0.0;
	equipment_height = 0.0;
	install_year = 0;
	current_hollow_diameter = 0.0;
	repair_time = 0.0;

	return res;
}

int pole::init(OBJECT *parent)
{
	OBJECT *my = THISOBJECTHDR;
	if ( ! node::init(parent) )
		return 0;

	// configuration
	if ( configuration == NULL || ! gl_object_isa(configuration,"pole_configuration") )
	{
		gl_error("configuration is not set to a pole_configuration object");
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
			throw "pole::default_repair_time must be positive";
		}
	}

	// tilt
	if ( tilt_angle < 0 || tilt_angle > 90 )
	{
		gl_error("pole tilt angle is not between and 0 and 90 degrees");
		return 0;
	}
	if ( tilt_direction < 0 || tilt_direction >= 360 )
	{
		gl_error("pole tilt direction is not between 0 and 360 degrees");
		return 0;
	}

	// weather
	if ( weather == NULL || ! gl_object_isa(weather,"climate") )
	{
		gl_error("weather is not set to a climate object");
		return 0;
	}
	wind_speed = (double*)gl_get_addr(weather,"wind_speed");
	if ( wind_speed == NULL )
	{
		gl_error("weather object does not provide wind speed data");
		return 0;
	}
	wind_direction = (double*)gl_get_addr(weather,"wind_dir");
	if ( wind_direction == NULL )
	{
		gl_error("weather object does not provide wind direction data");
		return 0;
	}
	wind_gust = (double*)gl_get_addr(weather,"wind_gust");
	if ( wind_gust == NULL )
	{
		gl_error("weather object does not provide wind gust data");
		return 0;
	}

	// calculation resisting moment
	resisting_moment = 0.008186 
		* config->strength_factor_250b_wood 
		* config->fiber_strength
		* ( config->ground_diameter * config->ground_diameter * config->ground_diameter);
	gl_verbose("resisting moment %.0f ft*lb",resisting_moment);

	// collect wire data
	static FINDLIST *all_ohls = NULL;
	if ( all_ohls == NULL )
		all_ohls = gl_find_objects(FL_NEW,FT_CLASS,SAME,"overhead_line",FT_END);
	OBJECT *obj = NULL;
	int n_lines = 0;
	while ( ( obj = gl_find_next(all_ohls,obj) ) != NULL )
	{
		overhead_line *line = OBJECTDATA(obj,overhead_line);
		if ( line->from == my || line->to == my )
		{
			n_lines++;
			line_configuration *line_config = OBJECTDATA(line->configuration,line_configuration);
			if ( line_config == NULL )
			{
				gl_warning("line %s has no line configuration--skipping",line->get_name());
				break;
			}
			line_spacing *spacing = OBJECTDATA(line_config->line_spacing,line_spacing);
			if ( spacing == NULL )
			{
				gl_warning("line configure %s has no line spacing data--skipping",line_config->get_name());
				break;
			}
			if ( line->length == 0.0 )
				gl_warning("wire has no length--wire moment will not be calculated");
			overhead_line_conductor *phaseA = OBJECTDATA(line_config->phaseA_conductor,overhead_line_conductor);
			if ( phaseA != NULL )
				add_wire(line,spacing->distance_AtoE,phaseA->cable_diameter,0.0,4430,line->length/2);
			overhead_line_conductor *phaseB = OBJECTDATA(line_config->phaseB_conductor,overhead_line_conductor);
			if ( phaseB != NULL )
				add_wire(line,spacing->distance_BtoE,phaseB->cable_diameter,0.0,4430,line->length/2);
			overhead_line_conductor *phaseC = OBJECTDATA(line_config->phaseC_conductor,overhead_line_conductor);
			if ( phaseC != NULL )
				add_wire(line,spacing->distance_CtoE,phaseC->cable_diameter,0.0,4430,line->length/2);
			overhead_line_conductor *phaseN = OBJECTDATA(line_config->phaseN_conductor,overhead_line_conductor);
			if ( phaseN != NULL )
				add_wire(line,spacing->distance_NtoE,phaseN->cable_diameter,0.0,2190,line->length/2);
			gl_verbose("found link %s",(const char*)(line->get_name()));
		}
	}
	if ( wire_data == NULL )
	{
		gl_warning("no wire data found--wire loading is not included");
	}
	is_deadend = ( n_lines < 2 );

	wire_load = 0.0;
	wire_moment = 0.0;
	wire_tension = 0.0;
	wire_load_nowind = 0.0;
	wire_moment_nowind = 0.0;
	double pole_height = config->pole_length - config->pole_depth;
	for ( std::list<WIREDATA>::iterator wire = wire_data->begin() ; wire != wire_data->end() ; wire++ )
	{
		double load_nowind = (wire->diameter+2*ice_thickness)/12;
		wire_load_nowind += load_nowind;
		wire_moment_nowind += wire->span * load_nowind * wire->height * config->overload_factor_transverse_wire;
	}

	pole_moment_nowind = pole_height * pole_height * (config->ground_diameter+2*config->top_diameter)/72 * config->overload_factor_transverse_general;
	equipment_moment_nowind = equipment_area * equipment_height * config->overload_factor_transverse_general;
	pole_stress_polynomial_a = pole_moment_nowind+equipment_moment_nowind+wire_moment_nowind;
	pole_stress_polynomial_b = 0.0;
	pole_stress_polynomial_c = wire_tension;

	if ( install_year > gl_globalclock )
		gl_warning("pole install year in the future are assumed to be current time");

	return 1;
}

TIMESTAMP pole::presync(TIMESTAMP t0)
{
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

	double wind_pressure_failure = (resisting_moment - wire_tension) / (pole_moment_nowind + equipment_moment_nowind + wire_moment_nowind);
	critical_wind_speed = sqrt(wind_pressure_failure / (0.00256 * 2.24));

	if ( pole_status == PS_FAILED && (gl_globalclock-down_time)/3600.0 > repair_time )
	{
		for ( std::list<WIREDATA>::iterator wire = wire_data->begin() ; wire != wire_data->end() ; wire++ )
		{
			wire->line->link_fault_off(&wire->fault,wire->fault_type,&wire->data);
		}
		gl_debug("pole repaired");
		tilt_angle = 0.0;
		tilt_direction = 0.0;
		pole_status = PS_OK;
		install_year = 1970 + (unsigned int)(t0/86400/365.24);

	}
	if ( pole_status == PS_OK && last_wind_speed != *wind_speed )
	{

		gld_clock dt;
		wind_pressure = 0.00256*2.24 * (*wind_speed)*(*wind_speed); //2.24 account for m/s to mph conversion
		critical_wind_speed = 0.0;
		double pole_height = config->pole_length - config->pole_depth;
		pole_moment = wind_pressure * pole_height * pole_height * (config->ground_diameter+2*config->top_diameter)/72 * config->overload_factor_transverse_general;
		equipment_moment = wind_pressure * equipment_area * equipment_height * config->overload_factor_transverse_general;

		wire_load = 0.0;
		wire_moment = 0.0;
		wire_tension = 0.0;	
		for ( std::list<WIREDATA>::iterator wire = wire_data->begin() ; wire != wire_data->end() ; wire++ )
		{
			double load = wind_pressure * (wire->diameter+2*ice_thickness)/12;
			wire_load += load;
			wire_moment += wire->span * load * wire->height * config->overload_factor_transverse_wire;
			wire_tension += wire->tension * config->overload_factor_transverse_wire * sin(tilt_angle/2) * wire->height;

		}

		total_moment = pole_moment + equipment_moment + wire_moment + wire_tension;
		pole_stress = total_moment/resisting_moment;
		if ( (*wind_speed) > 0 )
			susceptibility = 2*(pole_moment+equipment_moment+wire_moment)/resisting_moment/(*wind_speed)/(0.00256)/(2.24);
		else
			susceptibility = 0.0;
		gl_verbose("%s: wind %4.1f psi, pole %4.0f ft*lb, equipment %4.0f ft*lb, wires %4.0f ft*lb, stress %.0f%%", 
			(const char*)(dt.get_string()), wind_pressure, pole_moment, equipment_moment, wire_moment, pole_stress*100);
		pole_status = ( pole_stress < 1.0 ? PS_OK : PS_FAILED );
		if ( pole_status == PS_FAILED )
		{
			gl_debug("pole failed at %.0f%% loading, time to repair is %g h",pole_stress*100,repair_time);
			down_time = gl_globalclock;
			for ( std::list<WIREDATA>::iterator wire = wire_data->begin() ; wire != wire_data->end() ; wire++ )
			{
				wire->repair = repair_time*3600;
				wire->line->link_fault_on(&wire->protection,wire->fault_type,&wire->fault,&wire->repair,&wire->data);
			}
		}
		last_wind_speed = *wind_speed;

		pole_moment_nowind = pole_height * pole_height * (config->ground_diameter+2*config->top_diameter)/72 * config->overload_factor_transverse_general;
		equipment_moment_nowind = equipment_area * equipment_height * config->overload_factor_transverse_general;
		double wind_pressure_failure = (resisting_moment - wire_tension) / (pole_moment_nowind + equipment_moment_nowind + wire_moment_nowind);
		critical_wind_speed = sqrt(wind_pressure_failure / (0.00256 * 2.24));
	}
	
	TIMESTAMP t1 = node::presync(t0);
	TIMESTAMP t2 = ( pole_status == PS_FAILED ? down_time + (int)(repair_time*3600) : TS_NEVER );
	return ( t1 > t0 && t2 < t1 ) ? t1 : t2;
}

TIMESTAMP pole::sync(TIMESTAMP t0)
{
	return node::sync(t0);
}

TIMESTAMP pole::postsync(TIMESTAMP t0)
{
	return node::postsync(t0);
}

EXPORT int create_pole(OBJECT **obj, OBJECT *parent)
{
   try
    {
        *obj = gl_create_object(pole::oclass);
        if (*obj!=NULL)
        {
            pole *my = OBJECTDATA(*obj,pole);
            gl_set_parent(*obj,parent);
            return my->create();
        }
        else
        {
            return 0;
        }
    }
    CREATE_CATCHALL(pole);
}

EXPORT int init_pole(OBJECT *obj)
{
    try 
    {
        pole *my = OBJECTDATA(obj,pole);
        return my->init(obj->parent);
    }
    INIT_CATCHALL(pole);
}

EXPORT int isa_pole(OBJECT *obj, char *classname)
{
	return OBJECTDATA(obj,pole)->isa(classname);
}

EXPORT TIMESTAMP sync_pole(OBJECT *obj, TIMESTAMP t0, PASSCONFIG pass)
{
	try {
		pole *pObj = OBJECTDATA(obj,pole);
		TIMESTAMP t1 = TS_NEVER;
		switch ( pass ) 
		{
		case PC_PRETOPDOWN:
			return pObj->presync(t0);
		case PC_BOTTOMUP:
			return pObj->sync(t0);
		case PC_POSTTOPDOWN:
			t1 = pObj->postsync(t0);
			obj->clock = t0;
			return t1;
		default:
			throw "invalid pass request";
		}
	}
	SYNC_CATCHALL(pole);
}
