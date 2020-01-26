// database.cpp
// Copyright (C) 2020 Regents of the Leland Stanford Junior University

#include <iostream>

#include "database.h"

EXPORT_CREATE(database);
EXPORT_INIT(database);
// EXPORT_COMMIT(database);

CLASS *database::oclass = NULL;
database *database::defaults = NULL;

char32 database::default_username = "gridlabd";
char32 database::default_password = "gridlabd";
char256 database::default_hostname = "localhost";
int32 database::default_port = 8086;
char256 database::default_database = "gridlabd";

CURLcode curl_status = CURLE_FAILED_INIT;

database::database(MODULE *module)
{
    if (oclass==NULL)
    {
        // register to receive notice for first top down. bottom up, and second top down synchronizations
        oclass = gld_class::create(module,"database",sizeof(database),PC_AUTOLOCK);
        if (oclass==NULL)
            throw "unable to register class database";
        else
            oclass->trl = TRL_PROTOTYPE;

        defaults = this;
        if (gl_publish_variable(oclass,
            PT_char32,"username",get_username_offset(),PT_ACCESS,PA_PUBLIC,PT_DESCRIPTION,"user name",
            PT_char32,"password",get_password_offset(),PT_ACCESS,PA_PUBLIC,PT_DESCRIPTION,"user password",
            PT_char256,"hostname",get_hostname_offset(),PT_ACCESS,PA_PUBLIC,PT_DESCRIPTION,"server hostname",
            PT_int32,"port",get_port_offset(),PT_ACCESS,PA_PUBLIC,PT_DESCRIPTION,"TCP port number",
            PT_char256,"database",get_dbname_offset(),PT_ACCESS,PA_PUBLIC,PT_DESCRIPTION,"database name",
            PT_set,"options",get_options_offset(),PT_ACCESS,PA_PUBLIC,PT_DESCRIPTION,"database connection options",
                PT_KEYWORD,"SHOWQUERY",(int64)DBO_SHOWQUERY,PT_DESCRIPTION,"show SQL queries when running",
                PT_KEYWORD,"NOCREATE",(int64)DBO_NOCREATE,PT_DESCRIPTION,"prevent automatic creation of non-existent schemas",
                PT_KEYWORD,"NEWDB",(int64)DBO_DROPSCHEMA,PT_DESCRIPTION,"destroy existing schemas before use them (risky)",
                PT_KEYWORD,"OVERWRITE",(int64)DBO_OVERWRITE,PT_DESCRIPTION,"destroy existing files before output dump/backup results (risky)",
            PT_double,"sync_interval[s]",get_sync_interval_offset(),PT_ACCESS,PA_PUBLIC,PT_DESCRIPTION,"interval at which on_sync is called",
            PT_int32,"tz_offset",get_tz_offset_offset(),PT_ACCESS,PA_PUBLIC,PT_DESCRIPTION,"timezone offset used by timestamp in the database",
            PT_bool,"uses_dst",get_uses_dst_offset(),PT_ACCESS,PA_PUBLIC,PT_DESCRIPTION,"timestamps in database include summer time offsets",
            NULL)<1){
                char msg[256];
                sprintf(msg, "unable to publish properties in %s",__FILE__);
                throw msg;
        }

        gl_global_create("influxdb::default_username",
            PT_char32,database::default_username.get_addr(),
            PT_ACCESS,PA_PUBLIC,
            PT_DESCRIPTION,"default InfluxDB username",
            NULL);

        gl_global_create("influxdb::default_password",
            PT_char32,database::default_password.get_addr(),
            PT_ACCESS,PA_PUBLIC,
            PT_DESCRIPTION,"default InfluxDB password",
            NULL);

        gl_global_create("influxdb::default_hostname",
            PT_char256,database::default_hostname.get_addr(),
            PT_ACCESS,PA_PUBLIC,
            PT_DESCRIPTION,"default InfluxDB hostname",
            NULL);

        gl_global_create("influxdb::default_port",
            PT_int32,&database::default_port,
            PT_ACCESS,PA_PUBLIC,
            PT_DESCRIPTION,"default InfluxDB port",
            NULL);

        gl_global_create("influxdb::default_database",
            PT_char256,database::default_database.get_addr(),
            PT_ACCESS,PA_PUBLIC,
            PT_DESCRIPTION,"default InfluxDB database",
            NULL);
    }
}

database::~database(void)
{

}

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}


void database::curl_init()
{
    // initialize curl
    if ( curl_status != CURLE_OK )
    {
        curl_status = curl_global_init(CURL_GLOBAL_ALL);
    }
    if ( curl_status != CURLE_OK )
    {
        throw "curl_global_init(CURL_GLOBAL_ALL) failed";
    }

    // setup curl write
    url = new char[65536];
    sprintf(url,"http://%s:%d/write?db=%s",(const char*)hostname,port,(const char*)dbname);
    curl_write = curl_easy_init();
    curl_easy_setopt(curl_write, CURLOPT_URL, url);
    curl_easy_setopt(curl_write, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(curl_write, CURLOPT_CONNECTTIMEOUT, 10);
    curl_easy_setopt(curl_write, CURLOPT_TIMEOUT, 10);
    curl_easy_setopt(curl_write, CURLOPT_POST, 1);
    curl_easy_setopt(curl_write, CURLOPT_TCP_KEEPIDLE, 120L);
    curl_easy_setopt(curl_write, CURLOPT_TCP_KEEPINTVL, 60L);
    FILE *devnull = fopen("/dev/null", "w+");
    curl_easy_setopt(curl_write, CURLOPT_WRITEDATA, devnull);
    curl_easy_setopt(curl_write, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
    curl_easy_setopt(curl_write, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl_write, CURLOPT_USERPWD, (const char *)password);

    // setup curl read
    sprintf(url,"http://%s:%d/query?db=%s&q=%%s",(const char*)hostname,port,(const char*)dbname);
    curl_read = curl_easy_init();
    curl_easy_setopt(curl_read, CURLOPT_SSL_VERIFYPEER, 0); 
    curl_easy_setopt(curl_read, CURLOPT_CONNECTTIMEOUT, 10);
    curl_easy_setopt(curl_read, CURLOPT_TIMEOUT, 10);
    curl_easy_setopt(curl_read, CURLOPT_TCP_KEEPIDLE, 120L);
    curl_easy_setopt(curl_read, CURLOPT_TCP_KEEPINTVL, 60L);
    curl_easy_setopt(curl_read, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl_read, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
    curl_easy_setopt(curl_read, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl_read, CURLOPT_USERPWD, (const char *)password);
}

bool database::find_table(const char *name)
{
    if ( name == NULL )
        name = dbname;
    return find_database(name);
}

bool database::create_table(const char *name)
{
    if ( name == NULL )
        name = dbname;
    return create_database(name);
}

bool database::drop_table(const char *name)
{
    if ( name == NULL )
        name = dbname;
    return drop_database(name);
}

bool database::find_database(const char *name)
{
    DynamicJsonDocument result = get("show databases");
    bool found = false;
#define GET(result,n) (result["results"][0]["series"][0]["values"][n][0])
    const char *item;
    for ( size_t n = 0; (item=GET(result,n)) != NULL ; n++ )
    {
        if ( strcmp(item,name) == 0 )
        {
            found = true;
            break;
        }
    }
    return found;
}

bool database::create_database(const char *name)
{
    verbose("creating database %s", name);
    DynamicJsonDocument result = post_query("create database %s",name);
    if ( result["results"][0]["statement_id"] == 0 )
        return true;
    else
    {
        exception("database::create_database(name='%s'): failed, result = %s", name, (const char*)result["results"]);
    }
    return find_database(name);
}

bool database::drop_database(const char *name)
{
    verbose("dropping database %s", name);
    DynamicJsonDocument result = post_query("drop database %s",name);
    if ( result["results"][0]["statement_id"] == 0 )
        return true;
    else
    {
        exception("database::drop_database(name='%s'): failed, result = %s", name, (const char*)result["results"]);
    }
    return ! find_database(name);
}

DynamicJsonDocument database::get(const char *format, ...)
{
    va_list ptr;
    va_start(ptr,format);
    char *buffer;
    vasprintf(&buffer,format,ptr);
    DynamicJsonDocument result = get(std::string(buffer));
    free(buffer);
    va_end(ptr);
    return result;
}
DynamicJsonDocument database::get(const std::string& query)
{
    std::string buffer;
    char *query_string;
    asprintf(&query_string,url,curl_easy_escape(curl_read, query.c_str(), query.size()));
    CURLcode response;
    curl_easy_setopt(curl_read, CURLOPT_URL, query_string);
    curl_easy_setopt(curl_read, CURLOPT_WRITEDATA, &buffer);
    curl_easy_setopt(curl_read, CURLOPT_POST, 0);
    response = curl_easy_perform(curl_read);
    if (response != CURLE_OK) 
    {
        exception(curl_easy_strerror(response));
    }
    free(query_string);
    DynamicJsonDocument result(1024);
    DeserializationError error = deserializeJson(result,buffer);
    if ( error )
    {
        exception("json error: %s , data follows\n%s",error.c_str(),buffer.c_str());
    }
    debug("database::get(query='%s') -> '%s'",query_string,buffer.c_str());
    return result;
}

DynamicJsonDocument database::post_query(const char *format, ...)
{
    va_list ptr;
    va_start(ptr,format);
    char *buffer;
    vasprintf(&buffer,format,ptr);
    std::string post(buffer);
    DynamicJsonDocument result = post_query(post);
    free(buffer);
    va_end(ptr);
    return result;
}
DynamicJsonDocument database::post_query(std::string& query)
{
    std::string buffer;
    char *query_string;
    asprintf(&query_string,url,curl_easy_escape(curl_read, query.c_str(), query.size()));
    CURLcode response;
    curl_easy_setopt(curl_read, CURLOPT_URL, query_string);
    curl_easy_setopt(curl_read, CURLOPT_WRITEDATA, &buffer);
    query.insert(0,"q=");
    curl_easy_setopt(curl_read, CURLOPT_POST, 1);
    curl_easy_setopt(curl_read, CURLOPT_POSTFIELDS, query.c_str());
    curl_easy_setopt(curl_read, CURLOPT_POSTFIELDSIZE, (long) query.length());
    response = curl_easy_perform(curl_read);
    if (response != CURLE_OK) 
    {
        exception(curl_easy_strerror(response));
    }
    // std::cout << query_string << " -> " << buffer.c_str() << std::endl;
    free(query_string);

    // test result
    DynamicJsonDocument result(1024);
    DeserializationError error = deserializeJson(result,buffer);
    if ( error )
    {
        exception("json error: %s , data follows\n%s",error.c_str(),buffer.c_str());
    }
    debug("database::post_query(query='%s') -> '%s'",query_string,buffer.c_str());
    return result;
}

DynamicJsonDocument database::post_write(const char *format, ...)
{
    va_list ptr;
    va_start(ptr,format);
    char *buffer;
    vasprintf(&buffer,format,ptr);
    std::string data(buffer);
    DynamicJsonDocument result = post_write(data);
    free(buffer);
    va_end(ptr);
    return result;
}
DynamicJsonDocument database::post_write(std::string& post)
{
    CURLcode response;
    long code;
    post.insert(0,"q=");
    curl_easy_setopt(curl_write, CURLOPT_POSTFIELDS, post.c_str());
    curl_easy_setopt(curl_write, CURLOPT_POSTFIELDSIZE, (long) post.length());
    response = curl_easy_perform(curl_write);
    curl_easy_getinfo(curl_write, CURLINFO_RESPONSE_CODE, &code);
    if ( response != CURLE_OK ) 
    {
        exception(curl_easy_strerror(response));
    }
    if ( code < 200 || code > 206 ) 
    {
        char *url;
        curl_easy_getinfo(curl_write, CURLINFO_EFFECTIVE_URL, &url);
        exception("influxdb post response code = %d, url = '%s', post = '%s'",code,url, post.c_str());
    }
    debug("database::get(post='%s') -> code = %d",post.c_str(),code);
    DynamicJsonDocument result(20);
    deserializeJson(result,post.c_str());
    return result;
}

bool database::add_data(const char *table, TIMESTAMP t, references items)
{
    exception("database::add_record() not implemented yet");
    std::string buffer;
    for ( references::iterator prop = items.begin() ; prop != items.end() ; prop++ )
    {
        OBJECT *obj = prop->get_object();
        char record[4096];
        if ( obj == NULL )
        {
            exception("database::add_data(table='%s',t=%lld,items=...) object is NULL");
        }
        else if ( obj->name == NULL )
        {
            sprintf(record,"%s_%d,name=\"%s\",value=\"%s\" %lld\n",obj->oclass->name,obj->id,prop->get_name(),(const char*)prop->get_string(),t);
        }
        else
        {
            sprintf(record,"%s,name=\"%s\",value=\"%s\" %lld\n",obj->name,prop->get_name(),(const char*)prop->get_string(),t);
        }
        buffer.append(record);
    }
    post_write(buffer);
    return false;
}

bool database::get_data(const char *table, TIMESTAMP t0, TIMESTAMP t1, references items)
{
    exception("database::get_record() not implemented yet");
    for ( references::iterator prop = items.begin() ; prop != items.end() ; prop++ )
    {
        std::cout << (const char*)prop->get_name() << " = " << (const char*)prop->get_string();
    }
    return false;
}

int database::create(void) 
{
    username = default_username;
    dbname = default_database;
    password = default_password;
    hostname = default_hostname;
    port = default_port;
    return 1; /* return 1 on success, 0 on failure */
}

int database::init(OBJECT *parent)
{
    curl_init();
    if ( find_database(dbname) && (options&DBO_DROPSCHEMA) )
    {
        if ( ! drop_database(dbname) )
        {
            exception("unable to drop database %s", (const char*)dbname);
        }
    }
    if ( ! find_database(dbname) && ! (options&DBO_NOCREATE) )
    {
        if ( ! create_database(dbname) )
        {
            exception("unable to create database %s",(const char*)dbname);
        }
    }

    gld_property("")

    return 1;
}

void database::term(void)
{
    return;
}
