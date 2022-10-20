/** $Id: init.cpp 4738 2014-07-03 00:55:39Z dchassin $
	Copyright (C) 2008 Battelle Memorial Institute
**/

#include "climate.h"

char climate_library_path[sizeof(char1024)] = strcat(std::getenv("GLD_ETC"),"/gridlabd/weather/US");

EXPORT CLASS *init(CALLBACKS *fntable, MODULE *module, int argc, char *argv[])
{
	if (set_callback(fntable)==NULL)
	{
		errno = EINVAL;
		return NULL;
	}

	INIT_MMF(climate);

	new climate(module);
	new weather(module);
	new csv_reader(module);

	if ( gl_global_getvar("datadir",climate_library_path,sizeof(climate_library_path)) )
	{
		strcat(climate_library_path,"/weather/US");
	}
	gl_global_create("climate::library_path",PT_char1024,climate_library_path,NULL);

	/* always return the first class registered */
	return climate::oclass;
}

CDECL int do_kill()
{
	/* if global memory needs to be released, this is a good time to do it */
	return 0;
}

EXPORT int check(){
	/* if any climate objects have bad filenames, they'll fail on init() */
	return 0;
}
