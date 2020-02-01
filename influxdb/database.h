// File: database.h
// Copyright (C) 2020 Regents of the Leland Stanford Junior University

#ifndef _DATABASE_H
#define _DATABASE_H

#include <time.h>

#include "jsondata.h"
#include "gridlabd.h"

// Define: DBO_NONE
//  No option specified
#define DBO_NONE       0x0000

// Define: DBO_SHOWQUERY
//  Show query when verbose is on
#define DBO_SHOWQUERY  0x0001 

// Define: DBO_NOCREATE
//  Prevent automatic creation of schema
#define DBO_NOCREATE   0x0002 

// Define: DBO_DROPSCHEMA
//  Drop schema before using it
#define DBO_DROPSCHEMA 0x0004 

// Define: DBO_OVERWRITE
//  Overwrite existing file when dumping and backing up
#define DBO_OVERWRITE  0x0008 

DECL_METHOD(database,logtag);

typedef std::list<gld_property> properties;

class database : public gld_object
{

public:

    static char32 default_username;
    static char32 default_password;
    static char256 default_hostname;
    static int32 default_port;
    static char256 default_database;
    static bool use_background_insert;

public:

    database(MODULE *module);
    ~database(void);

public:

    GL_STRING(char32,username);
    GL_STRING(char32,password);
    GL_STRING(char256,hostname);
    GL_ATOMIC(int16,port);
    GL_STRING(char256,dbname);
    GL_ATOMIC(set,options);
    GL_STRING(char32,logname);
    GL_METHOD(database,logtag);
    GL_ATOMIC(double,sync_interval);
    GL_ATOMIC(int32,tz_offset);
    GL_ATOMIC(bool,uses_dst);

public:

    int create(void);
    int init(OBJECT *parent);
    int finalize(void);

private:

    CURL *curl_write;
    CURL *curl_read;
    char *url;
    void curl_init();
    DynamicJsonDocument get(const std::string& query);
    DynamicJsonDocument get(const char *format,...);
    DynamicJsonDocument post_query(std::string& query);
    DynamicJsonDocument post_query(const char *format,...);
    DynamicJsonDocument post_write(std::string& post);
    DynamicJsonDocument post_write(const char *format,...);
    DynamicJsonDocument post_data(std::string& body);

    bool find_database(const char *name);
    bool create_database(const char *name);
    bool drop_database(const char *name);

    bool find_table(const char *name=NULL);
    bool create_table(const char *name=NULL);
    bool drop_table(const char *name=NULL);

private:

    bool initialized_ok;
    properties *taglist;
    std::string *tagtext;
    int get_taglist_size();
    int get_taglist(char *buffer,int size);
    int add_taglist(char *buffer);
    size_t log_id;

public:

    inline bool is_initialized(void) { return initialized_ok; };
    void add_log(const char *format, ...);
    static const char *get_header_value(OBJECT *obj, const char *item, char *buffer, size_t len);
    
public:

    static CLASS *oclass;
    static class database *defaults;

    friend class measurements;
};

class measurements 
{

private:

    database &db;
    std::string measurement;
    std::string tags;
    std::string values;
    TIMESTAMP time;
    size_t count;
    const size_t limit;

public:

    measurements(database *d, int n_max = 65536) : db(*d), limit((unsigned)n_max) { reset(false); };
    ~measurements() { flush(); };

public:

    void reset(bool do_flush=true);
    void flush(void);
    void start(const char *name,const char *static_tags=NULL,properties *dynamic_tags=NULL);
    void add_tag(const char *name, const char *value);
    void add_tag(const char *name, double value);
    void add_tag(const char *name, long long value);
    void add_tag(const char *name, bool value);
    void add_tag(const char *name, gld_property &value);
    void add_tag(const char *name, gld_global &value);
    void add_field(const char *name, const char *value);
    void add_field(const char *name, double value);
    void add_field(const char *name, long long value);
    void add_field(const char *name, bool value);
    void add_field(const char *name, gld_property &value, bool with_units = false);
    void add_field(const char *name, gld_global &value);
    void set_time(TIMESTAMP t=0);
    void commit();

};

#endif
