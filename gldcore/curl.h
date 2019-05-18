// curl.h
// dchassin@slac.stanford.edu

#ifndef _CURL_H
#define _CURL_H

#ifndef _GLDCORE_H
#error "this header may only be included from gldcore.h"
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
