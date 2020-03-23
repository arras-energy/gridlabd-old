// File: main.cpp
// Copyright (C) 2020 Regents of the Leland Stanford Junior University

#define DLMAIN

#include "revenue.h"

EXPORT CLASS *init(CALLBACKS *fntable, MODULE *module, int argc, char *argv[])
{
	if (set_callback(fntable)==NULL)
	{
		errno = EINVAL;
		return NULL;
	}

	INIT_MMF(revenue);

	new tariff(module);
	new billing(module);
	// TODO: add more classes here

	gl_global_create("revenue::dcare_discount",PT_double,&dcare_discount,PT_UNITS,"$",NULL);
	gl_global_create("revenue::climate_credit",PT_double,&climate_credit,PT_UNITS,"$",NULL);
	gl_global_create("revenue::climate_credit_months",PT_char1024,&climate_credit_months,NULL);
	gl_global_create("revenue::summer_season_months",PT_char1024,&summer_season_months,NULL);
	gl_global_create("revenue::winter_season_months",PT_char1024,&winter_season_months,NULL);
	// TODO: register module globals here

	// always return the first class registered */
	return tariff::oclass;
}


EXPORT int do_kill(void*)
{
	// TODO: actions when module is unloaded
	return 0;
}

EXPORT int check()
{
	// TODO: actions when --check command line option is used
	return 0;
}
