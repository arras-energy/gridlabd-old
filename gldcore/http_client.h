/*	File: http_client.h 
 	Copyright (C) 2008, Battelle Memorial Institute
 */

#ifndef _HTTPCLIENT_H
#define _HTTPCLIENT_H

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

#ifdef WIN32

#ifdef int64
#undef int64 // wtypes.h uses the term int64
#endif

#include <winsock2.h>

#ifndef int64
#define int64 __int64
#endif

#else

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/errno.h>
#include <netdb.h>

// Define: SOCKET
#define SOCKET int

// Define: INVALID_SOCKET
#define INVALID_SOCKET (-1)

#endif

// Section: Type Definitions

/*	Structure: s_http
	sd - socket id
	len - size of buffer
	pos - position of next item in buffer
	buf - buffer
 */
struct s_http {
	SOCKET sd;
	size_t len;
	size_t pos;
	char *buf;
};

/*	Structure: s_http_buffer
	data - content of buffer
	size - size of content
 */
struct s_http_buffer
{
	char *data;
	int size;
};

/*	Structure: s_http_result
	header - HTTP header
	body - HTTP body
	status - HTTP status
 */
struct s_http_result
{
	struct s_http_buffer header;
	struct s_http_buffer body;
	int status;
};

// Typedef: HTTP
typedef struct s_http HTTP;

// Typedef: HTTPBUFFER
typedef struct s_http_buffer HTTPBUFFER;

// Typedef: HTTPRESULT
typedef struct s_http_result HTTPRESULT;

#ifdef __cplusplus
extern "C" {
#endif

// Function: hopen
DEPRECATED HTTP* hopen(const char *url, int maxlen);

// Function: hclose
DEPRECATED int hclose(HTTP*http);

// Function: hread
DEPRECATED size_t hread(char *buffer, size_t size, HTTP* http);

// Function: http_read
DEPRECATED HTTPRESULT *http_read(const char *url, int maxlen); 

// Function: http_delete_result
DEPRECATED void http_delete_result(HTTPRESULT *result);

// Function: http_new_result
DEPRECATED HTTPRESULT *http_new_result(void);

// Function: http_saveas
DEPRECATED int http_saveas(const char *url, const char *file);

#ifdef __cplusplus
}

// Class: GldHttpClient
class GldHttpClient
{
private:
public:	

	// Constructor: GldHttpClient
	GldHttpClient();

	// Destructor: ~GldHttpClient
	~GldHttpClient();
};
#endif

#endif
