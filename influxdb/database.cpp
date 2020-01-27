// database.cpp
// Copyright (C) 2020 Regents of the Leland Stanford Junior University

#include <iostream>

#include "database.h"

EXPORT_CREATE(database);
EXPORT_INIT(database);
EXPORT_FINALIZE(database);

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
    add_log("initialized");
    return 1;
}

int database::finalize(void)
{
    add_log("finalized");
    return 1;
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

DynamicJsonDocument database::post_write(std::string& body)
{
    CURLcode response;
    long code;
    body.insert(0,"q=");
    curl_easy_setopt(curl_write, CURLOPT_POSTFIELDS, body.c_str());
    curl_easy_setopt(curl_write, CURLOPT_POSTFIELDSIZE, (long) body.length());
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
        exception("influxdb post response code = %d, url = '%s', body = '%s'",code,url, body.c_str());
    }
    debug("database::post(body='%s') -> code = %d",body.c_str(),code);
    DynamicJsonDocument result(20);
    deserializeJson(result,body.c_str());
    return result;
}

DynamicJsonDocument database::post_data(std::string& body)
{
    CURLcode response;
    long code;
    curl_easy_setopt(curl_write, CURLOPT_POSTFIELDS, body.c_str());
    curl_easy_setopt(curl_write, CURLOPT_POSTFIELDSIZE, (long) body.length());
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
        exception("influxdb post response code = %d, url = '%s', body = '%s'",code,url, body.c_str());
    }
    debug("database::post(body='%s') -> code = %d",body.c_str(),code);
    DynamicJsonDocument result(20);
    deserializeJson(result,body.c_str());
    return result;
}

void database::start_measurement(measurements & measurement,const char *name)
{
    if ( measurement.size() > 0 && measurement.back() != '\n' )
        exception("cannot start a new measurement until the existing measurement has a value");
    measurement.append(name);
}

void database::write_data(measurements &measurement, const char *data, size_t len, size_t max)
{
    if ( len > 0 && len < max )
    {
        measurement.append(data);
    }
    else
    {
        exception("database::write_data(...,data='%s',len=%ld,max=%ld) write failed", data,len,max);
    }
}

void database::add_tag(measurements &measurement, const char *field, const char *value)
{
    if ( value )
    {
        char buf[1024];
        size_t len = snprintf(buf,sizeof(buf),",%s=%s",field,value);
        write_data(measurement,buf,len,sizeof(buf));
    }
}
void database::add_tag(measurements &measurement, const char *field, double value)
{
    char buf[1024];
    size_t len = snprintf(buf,sizeof(buf),",%s=%lg",field,value);
    write_data(measurement,buf,len,sizeof(buf));
}

void database::add_tag(measurements &measurement, const char *field, long long value)
{
    char buf[1024];
    size_t len = snprintf(buf,sizeof(buf),",%s=%lld",field,value);
    write_data(measurement,buf,len,sizeof(buf));
}

void database::add_tag(measurements &measurement, const char *field, gld_property &value)
{
    char buf[1024];
    size_t len = snprintf(buf,sizeof(buf),",%s=%s",field,(const char*)value.get_string());
    write_data(measurement,buf,len,sizeof(buf));
}

void database::add_tag(measurements &measurement, const char *field, gld_global &value)
{
    char buf[1024];
    size_t len = snprintf(buf,sizeof(buf),",%s=%s",field,(const char*)value.get_string());
    write_data(measurement,buf,len,sizeof(buf));
}

void database::set_value(measurements &measurement, const char *value, TIMESTAMP t)
{
    if ( t == 0 ) t = gl_globalclock;
    char buf[1024];
    size_t len = snprintf(buf,sizeof(buf)," value=\"%s\" %lld\n",value?value:"",t);
    write_data(measurement,buf,len,sizeof(buf));
}

void database::set_value(measurements &measurement, double value, TIMESTAMP t)
{
    if ( t == 0 ) t = gl_globalclock;
    char buf[1024];
    size_t len = snprintf(buf,sizeof(buf)," value=%lg %lld\n",value,t);
    write_data(measurement,buf,len,sizeof(buf));
}

void database::set_value(measurements &measurement, long long value, TIMESTAMP t)
{
    if ( t == 0 ) t = gl_globalclock;
    char buf[1024];
    size_t len = snprintf(buf,sizeof(buf)," value=%lld %lld\n",value,t);
    write_data(measurement,buf,len,sizeof(buf));
}

void database::set_value(measurements &measurement, gld_property &value, TIMESTAMP t)
{
    if ( t == 0 ) t = gl_globalclock;
    char buf[1024];
    size_t len = snprintf(buf,sizeof(buf)," value=%s %lld\n",(const char*)value.get_string(),t);
    write_data(measurement,buf,len,sizeof(buf));
}

void database::set_value(measurements &measurement, gld_global &value, TIMESTAMP t)
{
    if ( t == 0 ) t = gl_globalclock;
    char buf[1024];
    size_t len = snprintf(buf,sizeof(buf)," value=%s %lld\n",(const char*)value.get_string(),t);
    write_data(measurement,buf,len,sizeof(buf));
}

void database::commit_measurements(measurements &measurement)
{
    debug("committing measurements: %s", measurement.c_str());
    post_data(measurement);
}

void database::add_log(const char *format, ...)
{
    va_list ptr;
    va_start(ptr,format);
    char buf[1024] = "(error logging message)";
    vsnprintf(buf,sizeof(buf),format,ptr);
    measurements log;
    start_measurement(log,"log");
    char guid[64];
    sprintf(guid,"%08llx%08llx",get_guid()[0],get_guid()[1]);
    add_tag(log,"guid",guid);
    add_tag(log,"clock",(double)clock()/(double)CLOCKS_PER_SEC);
    add_tag(log,"object",get_name());
    add_tag(log,"hostname",get_hostname());
    add_tag(log,"port",(long long)get_port());
    add_tag(log,"username",get_username());
    add_tag(log,"database",get_dbname());
    set_value(log,buf);
    commit_measurements(log);
    va_end(ptr);
}
