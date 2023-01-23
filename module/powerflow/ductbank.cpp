// module/powerflow/ductbank.cpp
// Copyright (C) 2023 Regents of the Leland Stanford Junior University

#include "powerflow.h"

EXPORT_CREATE(ductbank);
EXPORT_INIT(ductbank);
EXPORT_COMMIT(ductbank);

CLASS *ductbank::oclass = NULL;
ductbank *ductbank::defaults = NULL;

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

			PT_double, "shell_temperature[degC]", get_shell_temperature_offset(),
				PT_DESCRIPTION, "duct shell temperature",

			PT_double, "ground_temperature[degC]", get_ground_temperature_offset(),
				PT_DESCRIPTION, "duct ground temperature",


			NULL)<1)
		{
				throw "unable to publish properties in " __FILE__;
		}
	}
}

int ductbank::create(void) 
{
	return 1; /* return 1 on success, 0 on failure */
}

int ductbank::init(OBJECT *parent)
{
	return 1;
}

TIMESTAMP ductbank::commit(TIMESTAMP t1, TIMESTAMP t2)
{
	return TS_NEVER;
}
