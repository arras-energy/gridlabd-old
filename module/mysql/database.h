/** $Id: database.h 4738 2014-07-03 00:55:39Z dchassin $

 General purpose assert objects

 **/

#ifndef _DATABASE_H
#define _DATABASE_H

#include "gridlabd.h"

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>
#include <ctype.h>

#ifdef WIN32
#undef int64
#include <winsock2.h>
#define int64 long long
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/errno.h>
#define SOCKET int
#define INVALID_SOCKET (-1)
#endif

#include <mysql.h>

EXTERN char default_hostname[256] INIT("127.0.0.1");
EXTERN char default_username[32] INIT("gridlabd");
EXTERN char default_password[32] INIT("gridlabd");
EXTERN char default_schema[256] INIT("gridlabd");
EXTERN int32 default_port INIT(3306);
EXTERN char default_socketname[1024] INIT("/tmp/mysql.sock");
EXTERN int64 default_clientflags INIT(CLIENT_LOCAL_FILES);
EXTERN char default_table_prefix[256] INIT(""); ///< table prefix

#define DBO_SHOWQUERY 0x0001 ///< show SQL query when verbose is on
#define DBO_NOCREATE 0x0002 ///< prevent automatic creation of schema
#define DBO_DROPSCHEMA 0x0004 ///< drop schema before using it
#define DBO_OVERWRITE 0x0008	///< overwrite existing file when dumping and backing up

class database : public gld_object {
public:
	GL_STRING(char256,hostname);
	GL_STRING(char32,username);
	GL_STRING(char32,password);
	GL_STRING(char256,schema);
	GL_ATOMIC(int32,port);
	GL_STRING(char1024,socketname);
	GL_ATOMIC(int64,clientflags);
	GL_ATOMIC(set,options);
	GL_STRING(char1024,on_init);
	GL_STRING(char1024,on_sync);
	GL_STRING(char1024,on_term);
	GL_ATOMIC(double,sync_interval);
	GL_ATOMIC(int32,tz_offset);
	GL_ATOMIC(bool,uses_dst);

	// mysql handle
private:
	MYSQL *mysql_client;
	MYSQL *mysql;
public:
	inline MYSQL *get_handle() { return mysql; };

	// term list
private:
	database *next;
	static database *first;
	static database *last;
	static database *last_used;
	char last_table_checked[1024];
public:
	static inline database *get_first(void) { return first; };
	inline database *get_next(void) { return next; }

public:
	/* required implementations */
	database(MODULE *module);
	int create(void);
	int init(OBJECT *parent);
	TIMESTAMP commit(TIMESTAMP from, TIMESTAMP to);
	void term(void);

public:
	static CLASS *oclass;
	static database *defaults;

public:
	// special functions
	const char *get_last_error(void);
	bool table_exists(const char *table);
	bool query(const char *query,...);
	bool query_ex(const char *query,...);
	size_t get_last_index(void);
	MYSQL_RES *select(const char *query,...);
	MYSQL_RES get_next(MYSQL_RES*res);
	int run_script(const char *file);
	size_t backup(const char *file);
#define TD_APPEND 0x0001 ///< table dump is appended to file
#define TD_BACKUP 0x0002 ///< table dump is formatted as SQL backup dump
	size_t dump(const char *table, const char *file=NULL, unsigned long options=0x0000);

	const char *get_sqltype(gld_property &p);
	char *get_sqldata(char *buffer, size_t size, gld_property &p, double scale=1.0);
	char *get_sqldata(char *buffer, size_t size, gld_property &p, gld_unit *unit=NULL);
	bool get_sqlbind(MYSQL_BIND &value,gld_property &target, my_bool *error=NULL);
	bool check_field(const char *table, const char *field);

	TIMESTAMP convert_from_dbtime(TIMESTAMP);
	TIMESTAMP convert_to_dbtime(TIMESTAMP);

	void start_transaction(void);
	void commit(void);
	void rollback(void);
};

EXTERN database *last_database INIT(NULL);

#include "recorder.h"
#include "player.h"
#include "collector.h"

#endif // _ASSERT_H
