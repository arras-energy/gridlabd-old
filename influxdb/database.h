// File: database.h
// Copyright (C) 2020 Regents of the Leland Stanford Junior University

#ifndef _DATABASE_H
#define _DATABASE_H

#include "gridlabd.h"
#include "jsondata.h"

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
    jsondata get(const std::string& query);
    jsondata post(std::string& post);

public:

    static CLASS *oclass;
    static class database *defaults;
};

#endif
