/* curl.cpp
 * Copyright (C) 2019, Regents of the Leland Stanford Junior University
 * dchassin@slac.stanford.edu
 */

#include "gldcore.h"

SET_MYCONTEXT(DMC_HTTPCLIENT)

GldCurl::GldCurl(const char *remote, const char *local)
{
	// initialize libcurl
	curl = curl_easy_init();
	if ( ! curl )
		throw "GldCurl: curl_easy_init() failed";
	IN_MYCONTEXT output_debug("GldCurl(remote='%s', local='%s'): curl init ok", remote, local);

	char errbuf[CURL_ERROR_SIZE];
	// set error message buffer
	if ( curl_easy_setopt(curl, CURLOPT_ERRORBUFFER,errbuf) != CURLE_OK )
	{
		IN_MYCONTEXT output_debug("GldCurl(remote='%s', local='%s'): unable to set error message buffer", remote, local);
		throw "GldCurl: curl_easy_setopt(CURLOPT_ERRORBUFFER) failed";
	}
	IN_MYCONTEXT output_debug("GldCurl(remote='%s', local='%s'): curl setopt(ERRORBUFFER) ok", remote, local);

	// access local file
	fp = fopen(local,"w");
	if ( ! fp )
		throw "GldCurl: local fopen failed";
	if ( curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)fp) != CURLE_OK )
	{
		IN_MYCONTEXT output_debug("GldCurl(remote='%s', local='%s'): %s", remote, local, errbuf);
		throw "GldCurl: curl_easy_setopt(CURLOPT_WRITEDATA) failed";
	}
	IN_MYCONTEXT output_debug("GldCurl(remote='%s', local='%s'): curl setopt(WRITEDATA) ok", remote, local);

	// set remote URL
	if ( curl_easy_setopt(curl, CURLOPT_URL, remote) != CURLE_OK )
	{
		IN_MYCONTEXT output_debug("GldCurl(remote='%s', local='%s'): %s", remote, local, errbuf);
		throw "GldCurl: curl_easy_setopt(CURLOPT_URL) failed";
	}
	IN_MYCONTEXT output_debug("GldCurl(remote='%s', local='%s'): curl setopt(URL) ok", remote, local);

	// TODO: set size limit with units
	// curl_off_t size; 
	// if ( get_option("maxsize",size) )
	// {
	// 	if ( curl_easy_setopt(curl, CURLOPT_MAXFILESIZE_LARGE, size) != CURLE_OK )
	// 	{
	// 		IN_MYCONTEXT output_debug("GldCurl(remote='%s', local='%s'): %s", remote, local, errbuf);
	// 		throw "GldCurl: curl_easy_setopt(CURLOPT_MAXFILESIZE_LARGE) failed";
	// 	}
	// 	IN_MYCONTEXT output_debug("GldCurl(remote='%s', local='%s'): curl setopt(MAXFILESIZE_LARGE,%lld) ok", remote, local,size);
	// }

	// TODO: time conditional operations

	// perform the CURL operation
	CURLcode res = curl_easy_perform(curl);
	if ( res != CURLE_OK )
	{
		IN_MYCONTEXT output_debug("GldCurl(remote='%s', local='%s'): %s", remote, local, errbuf);
		throw "GldCurl: curl_easy_perform() failed";
	}
	IN_MYCONTEXT output_debug("GldCurl(remote='%s', local='%s'): curl perform() ok", remote, local);
}

GldCurl::~GldCurl(void)
{
	if ( ! curl ) curl_easy_cleanup(curl);
	if ( ! fp ) fclose(fp);
}

bool GldCurl::get_option(const char *name, long long &llval)
{
	char tmp[1024];
	if ( global_getvar("wget_options",tmp,sizeof(tmp)) )
	{
		char *last = NULL;
		char *tag = NULL;
		while ( (tag=strtok_r(tag?tmp:NULL,";",&last)) )
		{
			char token[256];
			long long value;
			if ( sscanf(tag,"%s:%lld",token,&value) == 2 && strcmp(token,name)==0 )
			{
				llval = value;
				return true;
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}