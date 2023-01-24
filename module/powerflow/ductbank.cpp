// module/powerflow/ductbank.cpp
// Copyright (C) 2023 Regents of the Leland Stanford Junior University

#include "powerflow.h"

EXPORT_CREATE(ductbank);
EXPORT_INIT(ductbank);
EXPORT_COMMIT(ductbank);

CLASS *ductbank::oclass = NULL;
ductbank *ductbank::defaults = NULL;
double *ductbank::default_ground_temperature = 10; // degC
double *ductbank::warning_cable_temperature = 80; // degC
double *ductbank::alert_cable_temperature = 90; // degC

ductbank::ductbank(MODULE *module)
{
	if (oclass==NULL)
	{
		// register to receive notice for first top down. bottom up, and second top down synchronizations
		oclass = gld_class::create(module,"ductbank",sizeof(ductbank),PC_AUTOLOCK|PC_OBSERVER);
		if (oclass==NULL)
			throw "unable to register class ductbank";
		else
			oclass->trl = TRL_PROVEN;

		defaults = this;
		if (gl_publish_variable(oclass,

			PT_double, "air_temperature[degC]", get_air_temperature_offset(),
				PT_DESCRIPTION, "duct air temperature",

			PT_double, "ground_temperature[degC]", get_ground_temperature_offset(),
				PT_DESCRIPTION, "duct ground temperature",

			PT_double, "cable_temperature[degC]", get_cable_temperature_offset(),
				PT_DESCRIPTION, "duct cable temperature",

			PT_double, "cross_section[m^2]", get_cross_section_offset(),
				PT_DESCRIPTION, "cross sectional area of duct bank",

			PT_double, "insulation_conductance[W/K]", get_insulation_offset(),
				PT_DESCRIPTION, "insulation conductance",

			NULL)<1)
		{
				throw "unable to publish properties in " __FILE__;
		}

		gl_global_create("powerflow::ground_temperature",PT_double,&default_ground_temperature,PT_UNITS,"degC",NULL);
		gl_global_create("powerflow::warning_cable_temperature",PT_double,&warning_cable_temperature,PT_UNITS,"degC",NULL);
		gl_global_create("powerflow::alert_cable_temperature",PT_double,&alert_cable_temperature,PT_UNITS,"degC",NULL);
	}
}

int ductbank::create(void) 
{
	return 1; /* return 1 on success, 0 on failure */
}

int ductbank::init(OBJECT *parent)
{
	air_temperature = shell_temperature = ground_temperature = default_ground_temperature;
	return 1;
}

TIMESTAMP ductbank::commit(TIMESTAMP t1, TIMESTAMP t2)
{
	double Q = 0.0; // TODO read from line losses
	double UA = 100.0; // TODO calculate from duct cross section - cable cross section
	air_temperature = Q/UA + ground_temperature;
	cable_temperature = air_temperature + Q/insulation_conductance;
	if ( cable_temperature > warning_cable_temperature )
	{
		warning("cable temperature warning (%.0lf degC)",cable_temperature);
	}
	else if ( cable_temperature > alert_cable_temperature )
	{
		warning("cable temperature alert (%.0lf degC)",cable_temperature);
	}
	return TS_NEVER;
}
