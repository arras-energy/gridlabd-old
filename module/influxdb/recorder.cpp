// recorder.cpp
// Copyright (C) 2020 Regents of the Leland Stanford Junior University

#include "recorder.h"

EXPORT_CREATE(recorder);
EXPORT_INIT(recorder);
EXPORT_COMMIT(recorder);
EXPORT_FINALIZE(recorder);
EXPORT_DESTROY(recorder);

CLASS *recorder::oclass = NULL;
recorder *recorder::defaults = NULL;


recorder::recorder(MODULE *module)
{
	if (oclass==NULL)
	{
		// register to receive notice for first top down. bottom up, and second top down synchronizations
		oclass = gld_class::create(module,"recorder",sizeof(recorder),PC_AUTOLOCK|PC_OBSERVER);
		if (oclass==NULL)
			throw "unable to register class recorder";
		else
			oclass->trl = TRL_PROTOTYPE;

		defaults = this;
		if (gl_publish_variable(oclass,
			PT_char1024,"fields",get_property_offset(),
				PT_REQUIRED,
				PT_DESCRIPTION,"target property name",
			PT_char1024,"measurement",get_table_offset(),
				PT_REQUIRED,
				PT_DESCRIPTION,"table name to store samples",
			PT_double,"interval[s]",get_interval_offset(),
				PT_DESCRIPTION,"sampling interval",
			PT_object,"connection",get_connection_offset(),
				PT_REQUIRED,
				PT_DESCRIPTION,"database connection",
			PT_set,"options",get_options_offset(),
				PT_DESCRIPTION,"data insert options",
				PT_KEYWORD,"NONE",(set)MO_NONE,
				PT_KEYWORD,"UNITS",(set)MO_USEUNITS,
			PT_char1024,"tags",get_tags_offset(),
				PT_DESCRIPTION,"tags to include with each record",
			NULL)<1){
				char msg[256];
				snprintf(msg,sizeof(msg)-1, "unable to publish properties in %s",__FILE__);
				throw msg;
		}
	}
}

int recorder::create(void) 
{
	property_list = NULL;
    taglist = new std::list<gld_property>;
    tagtext = new std::string;
	return 1; /* return 1 on success, 0 on failure */
}

void recorder::destroy(void)
{
	delete property_list;
	delete taglist;
	delete tagtext;
}

int recorder::init(OBJECT *parent)
{
	// check connection
	if ( connection == NULL )
	{
		error("connection not defined");
		return 0;
	}
	if ( (MODULE*)(get_connection()->get_oclass()->get_module()) != my()->oclass->module 
		|| ! get_connection()->isa("database") )
	{
		error("connection is not an influxdb database object");
		return 0;
	}
	db = get_connection_object();
	if ( db->is_initialized() )
	{
		db->add_log("connecting recorder %s",my()->name);
	}
	else
	{
		return 2;
	}
	measurement = new measurements(db);

	// connect the target properties
	make_property_list();

	// set heartbeat
	if ( interval>0 )
	{
		set_heartbeat((TIMESTAMP)fabs(interval));
		oldvalues = NULL;
	}

	// diff sampling only
	else if ( interval<0 )
	{
		oldvalues = (char*)malloc(65536);
		strcpy(oldvalues,"");
	}

	else // variable interval sampling
	{
		oldvalues = NULL;
	}

	add_taglist(tags);

	return 1;
}

TIMESTAMP recorder::commit(TIMESTAMP t0, TIMESTAMP t1)
{

	// check sampling interval
	if ( interval != 0 )
	{
		if ( gl_globalclock%((TIMESTAMP)fabs(interval))!=0 )
			return TS_NEVER;
		else
			gl_verbose("%s: sampling time has arrived", get_name());
	}

	const char *name = (const char*)get_table();
	if ( strcmp(name,"") == 0 )
	{
		name = get_object(my())->get_name();
	}
	measurement->start(name,tagtext->c_str(),taglist);
	for ( properties::iterator prop = property_list->begin() ; prop != property_list->end() ; prop++ )
	{
		measurement->add_field(prop->get_name(),*prop,(options&MO_USEUNITS)==MO_USEUNITS);
	}
	measurement->commit();

	return TS_NEVER;
}

int recorder::finalize(void)
{
	measurement->flush();
	return 1;
}

void recorder::make_property_list(const char *delim)
{
    char* saveptr;
    char* token = strtok_r(strdup(get_property()),delim,&saveptr);

    property_list = new properties;

    while ( token != NULL )
    {
    	char oname[256], pname[256];
    	gld_property *prop = NULL;
    	OBJECT *parent = my()->parent;
    	if ( sscanf(token,"%[^:]:%s",oname,pname) == 2 )
    	{
    		prop = new gld_property(oname,pname);
    	}
    	else if ( parent != NULL )
    	{
    		prop = new gld_property(parent,token);
    	}
    	else
    	{
    		exception("field '%s' cannot be used if parent is not specified",token);
    	}
    	
    	if ( ! prop->is_valid() )
    	{
    		exception("property %s is not valid",token);
    	}
        property_list->push_back(*prop);
        token = strtok_r(NULL,delim,&saveptr);
    }
}

int recorder::get_taglist_size()
{
    return get_taglist(NULL,0);
}

int recorder::get_taglist(char *buffer, int size)
{
    size_t pos = snprintf((size==0)?NULL:buffer,size,"%s",tagtext->c_str());
    for ( std::list<gld_property>::iterator tag = taglist->begin() ; tag != taglist->end() ; tag++ )
    {
       pos += snprintf((size==0)?NULL:(buffer+pos),(size==0)?0:(size-pos),"%s%s=%s",pos==0?"":",",tag->get_name(),(const char *)tag->get_string());
    }
    return pos;
}

int recorder::add_taglist(char *buffer)
{
    char value[1024];
    int len = strlen(buffer);
    while ( buffer[0] != '\0' )
    {
        if ( sscanf(buffer,"%[^,]",value) != 1 )
        {
            error("add_taglist(str='%s'): taglist is invalid");
            return 0;
        }
        if ( strchr(value,'=') == NULL ) // simple property
        {
        	if ( my()->parent == NULL )
        	{
        		exception("cannot use tag '%s' unless parent is specified",value);
        	}
            gld_property *prop = new gld_property(my()->parent,value);
            if ( prop->is_valid() )
            {
                taglist->push_back(*prop);
            }
            else
            {
                gld_object *parent = get_object(my()->parent);
                std::string name = parent->get_name();
            	char header_value[1024];
            	if ( database::get_header_value(my()->parent,value,header_value,sizeof(header_value)) == NULL )
    			{
				    error("'%s' is not a valid property of object '%s'", value,name.c_str());
			        return 0;
			    }
			    if ( header_value[0] != '\0' )
				{		    	
	            	char header_tag[2048];
	            	snprintf(header_tag,sizeof(header_tag),"%s=%s",value,header_value);
		            if ( tagtext->size() > 0 )
		                tagtext->append(",");
	    	        tagtext->append(header_tag);
	    	    }
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
        while ( isspace(*buffer) ) buffer++;
    }
    return len;
}

EXPORT TIMESTAMP heartbeat_recorder(OBJECT *obj)
{
	recorder *my = OBJECTDATA(obj,recorder);
	if ( my->get_interval() <= 0 ) 
	{
		return TS_NEVER;
	}
	else
	{
		obj->clock = gl_globalclock;
		TIMESTAMP dt = (TIMESTAMP)my->get_interval();
	
		// recorder is always a soft event
		return -(obj->clock/dt+1)*dt;
	}
}

