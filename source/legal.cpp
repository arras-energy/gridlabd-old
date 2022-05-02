/** legal.cpp
	Copyright (C) 2008 Battelle Memorial Institute

	@file legal.cpp
	@author David P. Chassin
	@addtogroup legal Version, Copyright, License, etc.
	@ingroup core

	The branch version are named after 500kV busses on the WECC system.
	The following GridLAB-D branches are planned and/or released:

	- <b>Allson</b>: this is the developmental release for the charter partners
	  of the initial release
    - <b>Buckley</b>: this is the planned first release for the public after
	  the charter partners have completed their first modules.

	@section License
	GridLAB-D is provided under a BSD-style license modified appropriately
	for government-funded software.  See legal_license() for details.

	The	restrictions forbid the removal of the copyright and legal notices.
	This is not actually a restriction, but a reminder of what goes
	without saying in a world where common sense is more common.

	The	restrictions also preclude the use of any contributor's name as an endorsement.
	This should not be interpreted as an actual restriction either, but a clarification
	because without the clause, the license does not grant recipients of the
	software permission to use the licensor's name anyway.  But it makes the legal
	types happy.  That's a good thing.

	See \ref License "The GridLAB License" for details.

 @{
 **/

#include "gldcore.h"

SET_MYCONTEXT(DMC_LEGAL)

/* branch names and histories (named after WECC 500kV busses)
	Allston			Version 1.0 originated at PNNL March 2007, released February 2008
	Buckley			Version 1.1 originated at PNNL January 2008, released April 2008
	Coulee			Version 1.2 originated at PNNL April 2008, released June 2008
	Coyote			Version 1.3 originated at PNNL June 2008, not released
	Diablo			Version 2.0 originated at PNNL August 2008
	Eldorado		Version 2.1 originated at PNNL September 2009
	Four Corners	Version 2.2 originated at PNNL November 2010
	Grizzly			Version 2.3 originated at PNNL November 2011
	Hassayampa		Version 3.0 originated at PNNL November 2011
	Hatwai			Version 3.1 originated at PNNL 2013
	Jojoba			Version 3.2 originated at PNNL 2014
	Keeler			Version 4.0 planned for 2015
	Lugo
	McNary
	Navajo
	Ostrander
	Palo Verde
	Perkins
	Redhawk
	Sacajawea
	Tesla
	Troutdale
	Vantage
	Vincent
	Westwing
	Yavapai
	(continue with 345kV WECC busses on WECC after this)
 */

/** Displays the current legal banner
	@return SUCCESS when conditions of use have been satisfied, FAILED when conditions of use have not been satisfied
 **/
STATUS legal_notice(void)
{
	/* suppress copyright info if copyright file exists */
	int suppress = global_suppress_repeat_messages;
	global_suppress_repeat_messages = 0;
	output_message("%s %s",PACKAGE_NAME,PACKAGE_VERSION);
	output_message(version_copyright());
	global_suppress_repeat_messages = suppress;
	return SUCCESS; /* conditions of use have been met */
}

/** Displays the current user license
	@return SUCCESS when conditions of use have been satisfied, FAILED when conditions of use have not been satisfied
 **/
const char *legal_license_text(void)
{
	char tmp[1024];
	strcpy(tmp,global_execdir);
	char *p = strrchr(tmp,'/');
	if ( p != NULL && strcmp(p,"/bin") == 0 )
	{
		*p = '\0';
	}
	char license_filename[2048];
	snprintf(license_filename,sizeof(license_filename)-1,"%s/LICENSE",tmp);
	FILE *fp = fopen(license_filename,"r");
	if ( fp == NULL )
	{
		char *errmsg;
		asprintf(&errmsg,"file '%s' not found", license_filename);
		return strdup(errmsg);
	}
	static char license_text[65536];
	license_text[fread(license_text,1,sizeof(license_text)-1,fp)] = '\0';
	return license_text;
}

STATUS legal_license(void)
{
	int surpress = global_suppress_repeat_messages;
	global_suppress_repeat_messages = 0;
	legal_notice();
	output_message(legal_license_text());
	global_suppress_repeat_messages = surpress;
	return SUCCESS;
}

/**************************************************************************************
 
 CHECK VERSION

 The purpose of check_version is to look for a file located at 
 http://www.gridlabd.org/versions.xml or http://www.gridlabd.org/versions.txt
 and verify whether the patch and build of this version is the latest one for the 
 major/minor of this version.  If the patch or build is not the latest, a warning 
 notice is displayed.

 **************************************************************************************/
#include <pthread.h>
#include <ctype.h>

#include "http_client.h"

static pthread_t check_version_thread_id;

#define CV_NOINFO 0x0001
#define CV_BADURL 0x0002
#define CV_NODATA 0x0004
#define CV_NEWVER 0x0008
#define CV_NEWPATCH 0x0010
#define CV_NEWBUILD 0x0020

void *check_version_proc(void *ptr)
{
	int patch, build;
	const char *url = "https://code.gridlabd.us/master/source/versions.txt";
	HTTPRESULT *result = http_read(url,0x1000);
	char target[32];
	char *pv = NULL, *nv = NULL;
	int rc = 0;
	int mypatch = version_patch();
	int mybuild = version_build();

	/* if result not found */
	if ( result==NULL || result->body.size==0 )
	{
		output_warning("check_version: unable to read %s", url);
		rc=CV_NOINFO;
		goto Done;
	}

	/** @todo check the version against latest available **/
	if ( result->status>0 && result->status<400 )
	{
		output_warning("check_version: '%s' error %d", url, result->status);
		rc=CV_BADURL;
		goto Done;
	}

	/* read version data */
	snprintf(target,sizeof(target)-1,"%d.%d:",version_major(),version_minor());
	pv = strstr(result->body.data,target);
	if ( pv==NULL )
	{
		output_warning("check_version: '%s' has no entry for version %d.%d", url, version_major(),version_minor());
		rc=CV_NODATA;
		goto Done;
	}
	if ( sscanf(pv,"%*d.%*d:%d:%d", &patch, &build)!=2 )
	{
		output_warning("check_version: '%s' entry for version %d.%d is bad", url, version_major(),version_minor());
		rc=CV_NODATA;
		goto Done;
	}

	nv = strchr(pv,'\n');
	if ( nv!=NULL )
	{
		while ( *nv!='\0' && isspace(*nv) ) nv++;
		if ( *nv!='\0' )
		{
			output_warning("check_version: newer versions than %s (Version %d.%d) are available", version_branch(), version_major(),version_minor());
			rc|=CV_NEWVER;
		}
		/* not done yet */
	}
	if ( mypatch<patch )
	{
		output_warning("check_version: a newer patch of %s (Version %d.%d.%d-%d) is available", version_branch(), version_major(),version_minor(), patch, build);
		rc|=CV_NEWPATCH;
		/* not done yet */
	}
	if ( mybuild>0 && mybuild<build )
	{
		output_warning("check_version: a newer build of %s (Version %d.%d.%d-%d) is available", version_branch(), version_major(),version_minor(), patch, build);
		rc|=CV_NEWBUILD;
	}
	if ( rc==0 )
	{
		IN_MYCONTEXT output_verbose("this version is current");
	}

	/* done */
Done:
	http_delete_result(result);
	return (void*)(int64)rc;
}

void check_version(int mt)
{
	/* start version check thread */
	if ( mt==0 || pthread_create(&check_version_thread_id,NULL,check_version_proc,NULL)!=0 )
	{
		/* unable to create a thread to do this so just do it inline (much slower) */
		check_version_proc(NULL);
	}
}

/**@}*/
