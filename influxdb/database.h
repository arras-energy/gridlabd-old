// File: database.h
// Copyright (C) 2020 Regents of the Leland Stanford Junior University

#ifndef _DATABASE_H
#define _DATABASE_H

#include "jsondata.h"

#include "gridlabd.h"

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

typedef std::list<gld_property> references;

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
    GL_ATOMIC(double,sync_interval);
    GL_ATOMIC(int32,tz_offset);
    GL_ATOMIC(bool,uses_dst);

public:

    int create(void);
    int init(OBJECT *parent);
    void term(void);

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

    bool find_database(const char *name);
    bool create_database(const char *name);
    bool drop_database(const char *name);

    bool find_table(const char *name=NULL);
    bool create_table(const char *name=NULL);
    bool drop_table(const char *name=NULL);

    bool add_data(const char *table,TIMESTAMP t, references list);
    bool get_data(const char *table,TIMESTAMP t0, TIMESTAMP t1, references list);

public:

    static CLASS *oclass;
    static class database *defaults;
};

#endif
