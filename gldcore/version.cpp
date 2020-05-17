/* version.cpp
 * Copyright (C) 2008, Battelle Memorial Institute
 */

#define _VERSION_C // flag special consideration for this file

#include "gldcore.h"

// SET_MYCONTEXT(DMC_VERSION) // used only if IN_MYCONTEXT is present in this module

#ifndef BRANCH
#define BRANCH "Keeler" // update this from legal.h each time trunk is branched
#endif

#ifndef BUILDNUM
#error("gldcore/build.h was not updated properly (BUILDNUM is missing) - try deleting it and rebuilding again")
#ifdef BUILD
#undef BUILD
#endif
#define BUILDNUM 0
#endif

#ifndef REV_YEAR
#error("gldcore/build.h was not updated properly (REV_YEAR is missing) - try deleting it and rebuilding again")
#endif

const char *version_copyright(void)
{
	static char buffer[1024];
	sprintf(buffer,"Copyright (C) 2016-%d, Regents of the Leland Stanford Junior University\nCopyright (C) 2004-%d, Battelle Memorial Institute\nAll Rights Reserved\n"
		"\n"
		"This program comes with ABSOLUTELY NO WARRANTY; for details type `gridlabd --license'.\n"
    	"This is free software, and you are welcome to redistribute it under certain conditions.\n"
		, REV_YEAR, REV_YEAR);
	return buffer;
}

unsigned int version_major(void)
{
	return REV_MAJOR;
}
unsigned int version_minor(void)
{
	return REV_MINOR;
}
unsigned int version_patch(void)
{
	return REV_PATCH;
}
unsigned int version_build(void)
{
	return BUILDNUM;
}

const char *version_branch(void)
{
	return BRANCH;
}

