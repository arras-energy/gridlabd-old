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

/*	Class: GldCurl

		This class implements basic curl functionality to maintain a local copy
		of a remote file using HTTP GET.
 */
class GldCurl
{
private:
	CURL *curl;
	FILE *fp;
public:

	// Constructor: GldCurl
	GldCurl(const char *remote, const char *local);

	// Destructor: GldCurl
	~GldCurl(void);
public:

	/*	Method: get_option

		This function retrieves the value of the global variable "wget_options".
	 */
	bool get_option(const char *name, long long &llval);
};

#endif
