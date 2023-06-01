/* version.cpp
 * Copyright (C) 2008, Battelle Memorial Institute
 */

#define _VERSION_C // flag special consideration for this file

#include "gldcore.h"
#include <string.h>

// SET_MYCONTEXT(DMC_VERSION) // used only if IN_MYCONTEXT is present in this module

#ifndef BRANCH
#error("gldcore/build.h was not updated properly (BUILDNUM is missing) - try deleting it and rebuilding again")
#endif

#ifndef BUILDNUM
#error("gldcore/build.h was not updated properly (BUILDNUM is missing) - try deleting it and rebuilding again")
#endif

#ifndef REV_YEAR
#error("gldcore/build.h was not updated properly (REV_YEAR is missing) - try deleting it and rebuilding again")
#endif

const char *version_copyright(void)
{
	static char buffer[1024];
	snprintf(buffer,sizeof(buffer)-1,"Copyright (C) 2016-%d, Regents of the Leland Stanford Junior University\nCopyright (C) 2004-%d, Battelle Memorial Institute\nAll Rights Reserved\n"
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

bool version_check(const char *expression)
{
	char line[strlen(expression)+1];
	strcpy(line,expression);
	int criteria = 0;
	bool invert = false;
	char *next = NULL, *last = NULL;
	bool ok = false;
	bool done = false;
	while ( (next=strtok_r(next?NULL:line," \t",&last)) )
	{
		int major=0, minor=-1, patch=-1, build=0;
		char value1[1024], value2[1024];
		if ( next[0] == '\0' )
		{
			continue;
		}
		if ( next[0] == '-' )
		{
			if ( strcmp(next,"-lt") == 0 )
			{
				criteria = -1;
				invert = false;					
			}
			else if ( strcmp(next,"-le") == 0 )
			{
				criteria = +1;
				invert = true;
			}
			else if ( strcmp(next,"-eq") == 0 )
			{
				criteria = 0;
				invert = false;
			}
			else if ( strcmp(next,"-ge") == 0 )
			{
				criteria = -1;
				invert = true;					
			}
			else if ( strcmp(next,"-gt") == 0 )
			{
				criteria = +1;
				invert = false;
			}
			else if ( strcmp(next,"-ne") == 0 )
			{
				criteria = 0;
				invert = true;
			}
			else
			{
				output_error("version_check(expression='%s'): expression '%s' is invalid",expression,next);
				return FALSE;
			}
			continue;
		}
		else if ( sscanf(next,"%u.%u.%u",&major,&minor,&patch) > 1 )
		{
			if ( minor == -1 )
			{
				minor = global_version_minor;
			}
			if ( patch == -1 )
			{
				patch = global_version_patch;
			}
			snprintf(value1,sizeof(value1)-1,"%06u.%06u.%06u",global_version_major, global_version_minor, global_version_patch);
			snprintf(value2,sizeof(value2)-1,"%06u.%06u.%06u",major,minor,patch);
		}
		else if ( sscanf(next,"%u",&build) == 1 )
		{
			snprintf(value1,sizeof(value1)-1,"%06d",global_version_build);
			snprintf(value2,sizeof(value2)-1,"%06d",build);
		}
		else
		{
			snprintf(value1,sizeof(value1)-1,"%s",global_version_branch);
			snprintf(value2,sizeof(value2)-1,"%s",next);
		}
		int result = strcmp(value1,value2);
		if ( result > 0)
		{
			result = 1;
		}
		else if ( result < 0 )
		{
			result = -1;
		}
		bool test = ( result == criteria);
		ok |= ( test ^ invert );
		output_debug("version_check(expression='%s'): strcmp('%s','%s') %s %d -> %s, result is now %s",expression,value1,value2,invert?"!=":"==",criteria,test?"true":"false",ok?"true":"false");
		done = true;
	}
	if ( ! done )
	{
		output_error("version_check(expression='%s'): expression is invalid",expression);
		return false;
	}
	else
	{
		return ok;
	}
}
