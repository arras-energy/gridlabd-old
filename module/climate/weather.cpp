/** $Id: weather.cpp 4738 2014-07-03 00:55:39Z dchassin $
	Copyright (C) 2009 Battelle Memorial Institute
	@file weather.cpp
	@author Matthew L Hauer
 **/

#include "weather.h"

CLASS *weather::oclass = 0;
weather *weather::defaults = NULL;

weather::weather()
{
	temperature = 0.0;
	humidity = 0.0;
	solar_dir = 0.0;
	solar_diff = 0.0;
	solar_global = 0.0;
  	global_horizontal_extra = 0.0;
	wind_speed = 0.0;
	wind_dir = 0.0;
	opq_sky_cov = 0.0;
	tot_sky_cov = 0.0;
	rainfall = 0.0;
	snowdepth = 0.0;
	pressure = 0.0;
	month = 0;
	day = 0;
	hour = 0;
	minute = 0;
	second = 0;
	next = NULL;
}

weather::weather(MODULE *module)
{
	if (oclass==NULL)
	{
		oclass = gl_register_class(module,"weather",sizeof(weather),PC_NOSYNC);
		if (oclass==NULL)
			throw "unable to register class weather";
		else
			oclass->trl = TRL_CONCEPT;

		if (gl_publish_variable(oclass,
			PT_double,"temperature[degF]",PADDR(temperature),
				PT_OUTPUT,
				PT_DEFAULT, "0.0 degF",
				PT_DESCRIPTION, "dry-bulb air temperature",
			PT_double,"humidity[%]",PADDR(humidity),
				PT_OUTPUT,
				PT_DEFAULT, "0%",
				PT_DESCRIPTION, "relative humidity",
			PT_double,"solar_direct[W/sf]",PADDR(solar_dir),
				PT_DESCRIPTION, "direct solar flux",
				PT_DEFAULT, "0.0 W/sf",
			PT_double,"solar_diffuse[W/sf]",PADDR(solar_diff),
				PT_OUTPUT,
				PT_DEFAULT, "0.0 W/sf",
				PT_DESCRIPTION, "diffuse solar flux",
			PT_double,"solar_global[W/sf]",PADDR(solar_global),
				PT_OUTPUT,
				PT_DEFAULT, "0.0 W/sf",
				PT_DESCRIPTION, "global solar flux",
      		PT_double,"global_horizontal_extra[W/sf]",PADDR(global_horizontal_extra),
				PT_OUTPUT,
				PT_DEFAULT, "0.0 W/sf",
				PT_DESCRIPTION, "horizontal surface solar flux",
			PT_double,"wind_speed[mph]", PADDR(wind_speed),
				PT_OUTPUT,
				PT_DEFAULT, "0.0 mph",
				PT_DESCRIPTION, "wind speed",
			PT_double,"wind_dir[deg]", PADDR(wind_dir),
				PT_OUTPUT,
				PT_DEFAULT, "0.0 deg",
				PT_DESCRIPTION, "wind direction",
			PT_double,"opq_sky_cov[pu]",PADDR(opq_sky_cov),
				PT_OUTPUT,
				PT_DEFAULT, "0.0 pu",
				PT_DESCRIPTION, "opaque sky cover",
			PT_double,"tot_sky_cov[pu]",PADDR(tot_sky_cov),
				PT_OUTPUT,
				PT_DEFAULT, "0.0 pu",
				PT_DESCRIPTION, "total sky cover",
			PT_double,"rainfall[in/h]",PADDR(rainfall),
				PT_OUTPUT,
				PT_DEFAULT, "0.0 in/h",
				PT_DESCRIPTION, "rainfall rate",
			PT_double,"snowdepth[in]",PADDR(snowdepth),
				PT_OUTPUT,
				PT_DEFAULT, "0.0 in",
				PT_DESCRIPTION, "depth of snow on the ground",
			PT_double,"pressure[mbar]",PADDR(pressure),
				PT_OUTPUT,
				PT_DEFAULT, "0.0 mbar",
				PT_DESCRIPTION, "air pressure",
			PT_int16,"month",PADDR(month),
				PT_OUTPUT,
				PT_DESCRIPTION, "current month",
			PT_int16,"day",PADDR(day),
				PT_OUTPUT,
				PT_DESCRIPTION, "current day",
			PT_int16,"hour",PADDR(hour),
				PT_OUTPUT,
				PT_DESCRIPTION, "current hour",
			PT_int16,"minute",PADDR(minute),
				PT_OUTPUT,
				PT_DESCRIPTION, "current minute",
			PT_int16,"second",PADDR(second),
				PT_OUTPUT,
				PT_DESCRIPTION, "current second",
			NULL)<1) GL_THROW("unable to publish properties in %s",__FILE__);
	}
}

EXPORT int create_weather(OBJECT **obj, OBJECT *parent)
{
	weather *my = (weather*)(obj+1);
	my->next = NULL;
	return 1;	// don't want it to get called, but better to have it not be fatal
}

/// Synchronize the cliamte object
EXPORT TIMESTAMP sync_weather(OBJECT *obj, TIMESTAMP t0)
{
	return TS_NEVER; // really doesn't do anything
}

// EOF
