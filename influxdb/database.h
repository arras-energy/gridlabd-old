// File: database.h
// Copyright (C) 2020 Regents of the Leland Stanford Junior University

#ifndef _DATABASE_H
#define _DATABASE_H

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

DECL_METHOD(database,tag);

class database : public gld_object
{

public:

    static char32 default_username;
    static char32 default_password;
    static char256 default_hostname;
    static int32 default_port;
    static char256 default_database;

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
    GL_ATOMIC(char1024,tags);
    GL_METHOD(database,tag);
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
    std::list<gld_property> *taglist;
    int get_taglist_size();
    int get_taglist(char *buffer,int size);
    int add_taglist(char *buffer);

private:
    typedef std::string measurements;
    void start_measurement(measurements & measurement,const char *name);
    void add_tag(measurements &measurement, const char *name, const char *value);
    void add_tag(measurements &measurement, const char *name, double value);
    void add_tag(measurements &measurement, const char *name, long long value);
    void add_tag(measurements &measurement, const char *name, gld_property &value);
    void add_tag(measurements &measurement, const char *name, gld_global &value);
    void set_value(measurements &measurement, const char *value, TIMESTAMP t=0);
    void set_value(measurements &measurement, double value, TIMESTAMP t=0);
    void set_value(measurements &measurement, long long value, TIMESTAMP t=0);
    void set_value(measurements &measurement, gld_property &value, TIMESTAMP t=0);
    void set_value(measurements &measurement, gld_global &value, TIMESTAMP t=0);
    void write_data(measurements &measurement, const char *data, size_t len, size_t max);
    void commit_measurements(measurements &measurement);

private:
    void add_log(const char *format, ...);

public:

    static CLASS *oclass;
    static class database *defaults;
};

#endif
