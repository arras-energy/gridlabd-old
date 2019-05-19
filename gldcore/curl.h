/* File: curl.h 
 * Copyright (C) 2019, Regents of the Leland Stanford Junior University
 */

#ifndef _CURL_H
#define _CURL_H

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

#include <stdlib.h>
#include <curl/curl.h>

class GldCurl
{
private:
	CURL *curl;
	FILE *fp;
public:
	GldCurl(const char *remote, const char *local);
	~GldCurl(void);
public:
	bool get_option(const char *name, long long &llval);
};

#endif
