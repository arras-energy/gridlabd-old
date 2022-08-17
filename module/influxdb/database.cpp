// database.cpp
// Copyright (C) 2020 Regents of the Leland Stanford Junior University

#include <iostream>

#include "database.h"

EXPORT_CREATE(database);
EXPORT_INIT(database);
EXPORT_FINALIZE(database);
EXPORT_DESTROY(database);

CLASS *database::oclass = NULL;
database *database::defaults = NULL;

char32 database::connection_protocol = "http";
char32 database::default_username = "gridlabd";
char32 database::default_password = "gridlabd";
char256 database::default_hostname = "localhost";
int32 database::default_port = 8086;
char256 database::default_database = "gridlabd";
bool database::synchronous_postdata = false;

static FILE *devnull = NULL;

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
                PT_KEYWORD,"NONE",(set)DBO_NONE,PT_DESCRIPTION,"no database options are specified",
                PT_KEYWORD,"SHOWQUERY",(set)DBO_SHOWQUERY,PT_DESCRIPTION,"show SQL queries when running",
                PT_KEYWORD,"NOCREATE",(set)DBO_NOCREATE,PT_DESCRIPTION,"prevent automatic creation of non-existent schemas",
                PT_KEYWORD,"NEWDB",(set)DBO_DROPSCHEMA,PT_DESCRIPTION,"destroy existing schemas before use them (risky)",
                PT_KEYWORD,"OVERWRITE",(int64)DBO_OVERWRITE,PT_DESCRIPTION,"destroy existing files before output dump/backup results (risky)",
            PT_double,"sync_interval[s]",get_sync_interval_offset(),PT_ACCESS,PA_PUBLIC,PT_DESCRIPTION,"interval at which on_sync is called",
            PT_int32,"tz_offset",get_tz_offset_offset(),PT_ACCESS,PA_PUBLIC,PT_DESCRIPTION,"timezone offset used by timestamp in the database",
            PT_bool,"uses_dst",get_uses_dst_offset(),PT_ACCESS,PA_PUBLIC,PT_DESCRIPTION,"timestamps in database include summer time offsets",
            PT_char32,"logname",get_logname_offset(),PT_ACCESS,PA_PUBLIC,PT_DESCRIPTION,"name of log table",
            PT_method,"logtag",get_logtag_offset(),PT_ACCESS,PA_PUBLIC,PT_DESCRIPTION,"property tag add method",
            NULL)<1){
                char msg[256];
                snprintf(msg,sizeof(msg)-1, "unable to publish properties in %s",__FILE__);
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

        gl_global_create("influxdb::connection_protocol",
            PT_char32,database::connection_protocol.get_addr(),
            PT_ACCESS,PA_PUBLIC,
            PT_DESCRIPTION,"default InfluxDB connection protocol",
            NULL);

        gl_global_create("influxdb::synchronous_postdata",
            PT_bool,&database::synchronous_postdata,
            PT_ACCESS,PA_PUBLIC,
            PT_DESCRIPTION,"default InfluxDB connection protocol",
            NULL);
    }
    devnull = fopen("/dev/null","w+");
}

database::~database(void)
{
    fclose(devnull);
}

int database::create(void) 
{
    username = default_username;
    dbname = default_database;
    password = default_password;
    hostname = default_hostname;
    port = default_port;
    taglist = new std::list<gld_property>;
    tagtext = new std::string;
    log_id = 1;
    initialized_ok = false;
    url = NULL;
    curl_write = NULL;
    curl_read = NULL;
    post = new postlist;
    return 1; /* return 1 on success, 0 on failure */
}

void database::destroy(void)
{
    if ( taglist ) delete taglist;
    if ( tagtext ) delete tagtext;
    if ( curl_write ) curl_easy_cleanup(curl_write);
    if ( curl_read ) curl_easy_cleanup(curl_read);
    if ( url ) free(url);
    for ( postlist::iterator item = post->begin() ; item != post->end() ; item ++ )
    {
        pthread_join(item->thread_id,NULL);
    }
    delete post;
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
    initialized_ok = true;
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
    curl_write = curl_easy_init();
    asprintf(&url,"%s://%s:%d/write?db=%s",(const char*)connection_protocol,(const char*)hostname,port,(const char*)dbname);
    curl_easy_setopt(curl_write, CURLOPT_URL, url);
    free(url);
    curl_easy_setopt(curl_write, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(curl_write, CURLOPT_CONNECTTIMEOUT, 10);
    curl_easy_setopt(curl_write, CURLOPT_TIMEOUT, 10);
    curl_easy_setopt(curl_write, CURLOPT_POST, 1);
    curl_easy_setopt(curl_write, CURLOPT_TCP_KEEPIDLE, 120L);
    curl_easy_setopt(curl_write, CURLOPT_TCP_KEEPINTVL, 60L);
    curl_easy_setopt(curl_write, CURLOPT_WRITEDATA, devnull);
    curl_easy_setopt(curl_write, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
    curl_easy_setopt(curl_write, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl_write, CURLOPT_USERPWD, (const char *)password);

    // setup curl read
    curl_read = curl_easy_init();
    asprintf(&url,"%s://%s:%d/query?db=%s&q=%%s",(const char*)connection_protocol,(const char*)hostname,port,(const char*)dbname);
    // Note: URL is saved for later when running queries
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

DynamicJsonDocument auto_deserialize(const char *buffer, size_t size=1024)
{
    if ( buffer[0] == '\0' )
    {
        DynamicJsonDocument result(1);
        deserializeJson(result,"[]");
        return result;
    }

    while ( true )
    {
        DynamicJsonDocument result(size);
        DeserializationError error = deserializeJson(result,buffer);
        if ( error == DeserializationError::Ok )
        {
            return result;
        }
        else if ( error == DeserializationError::NoMemory )
        {
            size *= 2;
        }
        else
        {
            gl_error("json error '%s', data=[%s]",error.c_str(),buffer);
            throw "database::auto_deserialize() failed";
        }
    }
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
        gl_debug("database::get(query='%s') -> query_string = %s, response %d", query.c_str(), query_string, response);
        exception(curl_easy_strerror(response));
    }
    free(query_string);
    gl_debug("database::get(query='%s') -> %s",query.c_str(),buffer.c_str());
    return auto_deserialize(buffer.c_str());
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
        gl_debug("database::post_query(query='%s') -> response %d",query.c_str(), response);
        exception(curl_easy_strerror(response));
    }
    free(query_string);
    gl_debug("database::post_query(query='%s') -> %s",query.c_str(),buffer.c_str());
    DynamicJsonDocument result = auto_deserialize(buffer.c_str(),buffer.size()/10);
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
    std::string buffer;
    long code;
    body.insert(0,"q=");
    curl_easy_setopt(curl_write, CURLOPT_POSTFIELDS, body.c_str());
    curl_easy_setopt(curl_write, CURLOPT_POSTFIELDSIZE, (long) body.length());
    curl_easy_setopt(curl_read, CURLOPT_WRITEDATA, &buffer);
    response = curl_easy_perform(curl_write);
    curl_easy_getinfo(curl_write, CURLINFO_RESPONSE_CODE, &code);
    if ( response != CURLE_OK ) 
    {
        gl_debug("database::post_write(body='%s') -> response %d",body.c_str(), response);
        exception(curl_easy_strerror(response));
    }
    if ( code < 200 || code > 206 ) 
    {
        char *url;
        curl_easy_getinfo(curl_write, CURLINFO_EFFECTIVE_URL, &url);
        exception("influxdb post response code = %d, url = '%s', body = '%s'",code,url, body.c_str());
    }
    gl_debug("database::post(body='%s') -> code = %d, result = '%s'",body.c_str(),code,buffer.c_str());
    DynamicJsonDocument result = auto_deserialize(buffer.c_str(),buffer.size()/10);
    return result;
}

void *background_postdata(void *ptr)
{
    struct s_postdata *item = (struct s_postdata *)ptr;
    CURL *curl_write = curl_easy_init();
    char *url;
    asprintf(&url,"%s://%s:%d/write?db=%s",item->protocol,item->hostname,item->port,item->dbname);
    curl_easy_setopt(curl_write, CURLOPT_URL, url);
    free(url);
    curl_easy_setopt(curl_write, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(curl_write, CURLOPT_CONNECTTIMEOUT, 10);
    curl_easy_setopt(curl_write, CURLOPT_TIMEOUT, 10);
    curl_easy_setopt(curl_write, CURLOPT_POST, 1);
    curl_easy_setopt(curl_write, CURLOPT_TCP_KEEPIDLE, 120L);
    curl_easy_setopt(curl_write, CURLOPT_TCP_KEEPINTVL, 60L);
    curl_easy_setopt(curl_write, CURLOPT_WRITEDATA, devnull);
    curl_easy_setopt(curl_write, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
    curl_easy_setopt(curl_write, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl_write, CURLOPT_USERPWD, item->password);
    curl_easy_setopt(curl_write, CURLOPT_POSTFIELDS, item->data);
    curl_easy_setopt(curl_write, CURLOPT_POSTFIELDSIZE, (long) item->size);
    curl_easy_setopt(curl_write, CURLOPT_WRITEDATA, &item->result);
    item->response = curl_easy_perform(curl_write);
    if ( item->response != CURLE_OK ) 
    {
        gl_error(curl_easy_strerror(item->response));
    }
    else
    {
        curl_easy_getinfo(curl_write, CURLINFO_RESPONSE_CODE, &item->code);
        if ( item->code < 200 || item->code > 206 ) 
        {
            gl_error("influxdb post item response code = %d, url = '%s', body = '%s'",item->code,url,item->data);
        }
    }
    gl_debug("background_postdata(data='%s') -> response %d, code %d, result = %s",item->data, item->response, item->code, item->result.c_str());
    curl_easy_cleanup(curl_write);
    free((void*)item->data);
    item->data = NULL;
    return NULL;
}

void database::post_data(std::string& body)
{
    struct s_postdata *item = new struct s_postdata;
    item->protocol = connection_protocol;
    item->hostname = hostname;
    item->port = port;
    item->dbname = dbname;
    item->username = username;
    item->password = password;
    item->data = strdup(body.c_str());
    item->size = body.size();
    if ( synchronous_postdata || pthread_create(&item->thread_id,NULL,background_postdata,item) != 0 )
    {
        if ( ! synchronous_postdata )
        {
            warning("background_postdata failed: running query synchronously (which is slower)");
        }
        background_postdata((void*)item);
        delete item;
    }
    else
    {
        post->push_back(*item);
    }
}

void database::add_log(const char *format, ...)
{
    if ( ! initialized_ok )
        exception("add_log(format='%s',...): database not initialized yet",format);
    if ( logname[0] != '\0' )
    {
        measurements log(this);
        log.start(logname,tagtext->c_str(),taglist);
        log.set_time(log_id++);
        log.add_field("clock",(long long)gl_globalclock);
        log.add_field("cputime",(double)clock()/(double)CLOCKS_PER_SEC);
        va_list ptr;
        va_start(ptr,format);
        char buf[1024] = "(error formatting log message)";
        vsnprintf(buf,sizeof(buf),format,ptr);
        log.add_field("message",buf);
        log.commit();
        va_end(ptr);
    }
}

int database::get_taglist_size()
{
    return get_taglist(NULL,0);
}

int database::get_taglist(char *buffer, int size)
{
    size_t pos = snprintf((size==0)?NULL:buffer,size,"%s",tagtext->c_str());
    for ( std::list<gld_property>::iterator tag = taglist->begin() ; tag != taglist->end() ; tag++ )
    {
       pos += snprintf((size==0)?NULL:(buffer+pos),(size==0)?0:(size-pos),"%s%s=%s",pos==0?"":",",tag->get_name(),(const char *)tag->get_string());
    }
    return pos;
}

int database::add_taglist(char *buffer)
{
    char value[1024];
    int len = strlen(buffer);
    while ( true )
    {
        if ( sscanf(buffer,"%[^,]",value) != 1 )
        {
            error("add_taglist(str='%s'): taglist is invalid");
            return 0;
        }
        if ( strchr(value,'=') == NULL ) // simple property
        {
            gld_property *prop = new gld_property(my(),value);
            if ( prop->is_valid() )
            {
                taglist->push_back(*prop);
            }
            else
            {
                error("'%s' is not a valid property", value);
                return 0;
            }
        }
        else // literal tag=value
        {
            if ( tagtext->size() > 0 )
                tagtext->append(",");
            tagtext->append(value);
        }
        buffer = strchr(buffer,',');
        if ( buffer++ == NULL )
            break;
    }
    return len;
}

int database::logtag(char *buffer, size_t size)
{
    if ( buffer == NULL ) // size query/check
    {
        int need = get_taglist_size();
        return size == 0 ? need : ( need < (int)size ? need : 0 );
    }
    else if ( size == 0 )
    {
        return add_taglist(buffer);
    }
    else
    {
        return get_taglist(buffer,size);
    }
}

const char *database::get_header_value(OBJECT *obj, const char *item, char *buffer, size_t len)
{
    buffer[0] = '\0';
    if ( strcmp(item,"name") == 0 )
    {
        if ( obj->name == NULL )
            snprintf(buffer,len,"%s:%d", obj->oclass->name, obj->id);
        else
            snprintf(buffer,len,"%s", obj->oclass->name);
    }
    else if ( strcmp(item,"class") == 0 )
    {
        snprintf(buffer,len,"%s", obj->oclass->name);
    }
    else if ( strcmp(item,"module") == 0 )
    {
        if ( obj->oclass->module ) snprintf(buffer,len,"%s", obj->oclass->module->name);
    }
    else if ( strcmp(item,"id") == 0 )
    {
        snprintf(buffer,len,"%llu", (unsigned long long)obj->id);    
    }
    else if ( strcmp(item,"parent") == 0 )
    {
        if ( obj->parent ) get_header_value(obj->parent,"name",buffer,len);
    }
    else if ( strcmp(item,"child_count") == 0 )
    {
        if ( obj->child_count > 0 ) snprintf(buffer,len,"%llu",(unsigned long long)obj->child_count);
    }
    else if ( strcmp(item,"schedule_skew") == 0 )
    {
        if ( obj->schedule_skew != 0 ) snprintf(buffer,len,"%lld",(long long)obj->schedule_skew);
    }
    else if ( strcmp(item,"rank") == 0 )
    {
        snprintf(buffer,len,"%llu",(unsigned long long)obj->rank); 
    }
    else if ( strcmp(item,"clock") == 0 )
    {
        snprintf(buffer,len,"%lld",(long long)obj->clock);
    }
    else if ( strcmp(item,"valid_to") == 0 )
    {
        if ( obj->valid_to < TS_NEVER ) snprintf(buffer,len,"%lld",(long long)obj->clock);
    }
    else if ( strcmp(item,"latitude") == 0 )
    {
        if ( isfinite(obj->latitude) ) snprintf(buffer,len,"%lg",(double)obj->latitude);
    }
    else if ( strcmp(item,"longitude") == 0 )
    {
        if ( isfinite(obj->longitude) ) snprintf(buffer,len,"%lg",(double)obj->longitude);
    }
    else if ( strcmp(item,"in_svc") == 0 )
    {
        if ( obj->in_svc > 0 ) snprintf(buffer,len,"%lld",(long long)obj->in_svc);
    }
    else if ( strcmp(item,"out_svc") == 0 )
    {
        if ( obj->out_svc < TS_NEVER ) snprintf(buffer,len,"%lld",(long long)obj->out_svc);
    }
    else if ( strcmp(item,"flags") == 0 )
    {
        snprintf(buffer,len,"%llx",(unsigned long long)obj->flags);
    }
    else if ( strcmp(item,"heartbeat") == 0 )
    {
        if ( obj->heartbeat > 0 ) snprintf(buffer,len,"%llu",(unsigned long long)obj->heartbeat);
    }
    else if ( strcmp(item,"groupid") == 0 )
    {
        char *c, *p = buffer;
        for ( c = obj->groupid ; *c != '\0' && p < buffer+len-1 ; c++ )
        {
            if ( strchr(", \t",*c) )
            {
                *p++ = '\\';
            }
            *p++ = *c;
        }
    }
    else if ( strcmp(item,"rng_state") == 0 )
    {
        snprintf(buffer,len,"%llu",(long long)obj->rng_state);
    }
    else if ( strcmp(item,"guid") == 0 )
    {
        snprintf(buffer,len,"%08llX%08llX",obj->guid[0],obj->guid[1]);
    }
    else if ( strcmp(item,"on_init") == 0 )
    {
        snprintf(buffer,len,"%s", obj->events.init ? obj->events.init : "");
    }
    else if ( strcmp(item,"on_precommit") == 0 )
    {
        snprintf(buffer,len,"%s", obj->events.precommit ? obj->events.precommit : "");
    }
    else if ( strcmp(item,"on_presync") == 0 )
    {
        snprintf(buffer,len,"%s", obj->events.presync ? obj->events.presync : "");
    }
    else if ( strcmp(item,"on_sync") == 0 )
    {
        snprintf(buffer,len,"%s", obj->events.sync ? obj->events.sync : "");
    }
    else if ( strcmp(item,"on_postsync") == 0 )
    {
        snprintf(buffer,len,"%s", obj->events.postsync ? obj->events.postsync : "");
    }
    else if ( strcmp(item,"on_commit") == 0 )
    {
        snprintf(buffer,len,"%s", obj->events.commit ? obj->events.commit : "");
    }
    else if ( strcmp(item,"on_finalize") == 0 )
    {
        snprintf(buffer,len,"%s", obj->events.finalize ? obj->events.finalize : "");
    }
    else
    {
        gl_error("get_header_value(obj=<%s:%d>,item='%s',...): invalid header item name",obj->oclass->name,obj->id,item);
        return NULL;
    }   
    return buffer;
}

EXPORT int method_database_logtag(OBJECT *obj, char *buffer, size_t size)
{
    class database *my = OBJECTDATA(obj,class database);
    return my->logtag(buffer,size);
}

////////////////////
// measurements
////////////////////

void measurements::reset(bool do_flush)
{
    if ( do_flush ) 
    {
        flush();
    }
    else
    {    
        measurement.clear(); 
        tags.clear(); 
        values.clear(); 
        time = 0;
        count = 0;
    }
}

void measurements::flush()
{
    if ( count > 0 )
    {
        db.post_data(measurement);
        reset(false);
    }    
}

void measurements::start(const char *name, const char *static_tags, properties *dynamic_tags)
{
    if ( measurement.size() > 0 && measurement.back() != '\n' )
        throw "cannot use start() before commit() or reset() is called";
    tags = name;
    if ( static_tags != NULL && static_tags[0] != '\0' )
    {
        tags.append(",");
        tags.append(static_tags);
    }
    if ( dynamic_tags != NULL && dynamic_tags->size() > 0 )
    {
        for ( properties::iterator prop = dynamic_tags->begin() ; prop != dynamic_tags->end() ; prop++ )
        {
            add_tag(prop->get_name(),*prop);
        }
    }
    values = "";
    time = gl_globalclock;
}

void measurements::add_tag(const char *field, const char *value)
{
    if ( value )
    {
        char buf[1024] = "(format error)";
        snprintf(buf,sizeof(buf),",%s=%s",field,value);
        tags.append(buf);
    }
}
void measurements::add_tag(const char *field, double value)
{
    char buf[1024] = "(format error)";
    snprintf(buf,sizeof(buf),",%s=%lg",field,value);
    tags.append(buf);
}

void measurements::add_tag(const char *field, long long value)
{
    char buf[1024] = "(format error)";
    snprintf(buf,sizeof(buf),",%s=%lld",field,value);
    tags.append(buf);
}

void measurements::add_tag(const char *field, bool value)
{
    char buf[1024] = "(format error)";
    snprintf(buf,sizeof(buf),",%s=%s",field,value?"true":"false");
    tags.append(buf);
}

void measurements::add_tag(const char *field, gld_property &value)
{
    char buf[1024] = "(format error)";
    snprintf(buf,sizeof(buf),",%s=%s",field,(const char*)value.get_string());
    tags.append(buf);
}

void measurements::add_tag(const char *field, gld_global &value)
{
    char buf[1024] = "(format error)";
    snprintf(buf,sizeof(buf),",%s=%s",field,(const char*)value.get_string());
    tags.append(buf);
}

void measurements::add_field(const char *name, const char *value)
{
    char buf[1024] = "(format error)";
    snprintf(buf,sizeof(buf),"%s%s=\"%s\"",values.size()>0?",":"",name,value?value:"");
    values.append(buf);
}

void measurements::add_field(const char *name, double value)
{
    char buf[1024] = "(format error)";
    snprintf(buf,sizeof(buf),"%s%s=%lg",values.size()>0?",":"",name,value);
    values.append(buf);
}

void measurements::add_field(const char *name, long long value)
{
    char buf[1024] = "(format error)";
    snprintf(buf,sizeof(buf),"%s%s=%lld",values.size()>0?",":"",name,value);
    values.append(buf);
}

void measurements::add_field(const char *name, bool value)
{
    char buf[1024] = "(format error)";
    snprintf(buf,sizeof(buf),"%s%s=%s",values.size()>0?",":"",name,value?"true":"false");
    values.append(buf);
}

void measurements::add_field(const char *name, gld_property &value, bool with_units)
{
    char buf[1024] = "(format error)";
    gld_unit *unit = value.get_unit();
    if ( with_units && unit->is_valid() )
    {
        complex z;
        switch ( value.get_type() ) 
        {
        case PT_double:
            snprintf(buf,sizeof(buf),"%s\"%s[%s]\"=%g",values.size()>0?",":"",name?name:value.get_name(),unit->get_name(),value.get_double());
            break;
        case PT_complex:
            z = value.get_complex();
            snprintf(buf,sizeof(buf),"%s\"%s[%s]\"=\"%g%+gj\"",values.size()>0?",":"",name?name:value.get_name(),unit->get_name(),z.Re(),z.Im());
            break;
        default:
            snprintf(buf,sizeof(buf),"%s\"%s[%s]\"=\"%s\"",values.size()>0?",":"",name?name:value.get_name(),unit->get_name(),(const char*)value.get_string());
            break;
        }
    }
    else 
    {
        switch ( value.get_type() )
        {
        case PT_bool:
            snprintf(buf,sizeof(buf),"%s%s=%s",values.size()>0?",":"",name?name:value.get_name(),value.get_bool()?"true":"false");
            break;
        case PT_int16:
        case PT_int32:
        case PT_int64:
            snprintf(buf,sizeof(buf),"%s%s=%lld",values.size()>0?",":"",name?name:value.get_name(),(long long)value.get_integer());
            break;
        default:
            snprintf(buf,sizeof(buf),"%s%s=\"%s\"",values.size()>0?",":"",name?name:value.get_name(),(const char*)value.get_string());
            break;
        }
    }
    values.append(buf);
}

void measurements::add_field(const char *name, gld_global &value)
{
    char buf[1024] = "(format error)";
    snprintf(buf,sizeof(buf),"%s%s=\"%s\"",values.size()>0?",":"",name?name:value.get_name(),(const char*)value.get_string());
    values.append(buf);
}

void measurements::set_time(TIMESTAMP t)
{
    time = ( t == 0 ? gl_globalclock : t);
}

void measurements::commit()
{
    measurement.append(tags);
    measurement.append(" ");
    measurement.append(values);
    char buf[1024] = "(format error)";
    snprintf(buf,sizeof(buf)," %lld\n",time);
    measurement.append(buf);
    if ( ++count >= limit )
    {
        flush();
    }

}


