// recorder.cpp
// Copyright (C) 2020 Regents of the Leland Stanford Junior University

#include "recorder.h"

EXPORT_CREATE(recorder);
EXPORT_INIT(recorder);
EXPORT_COMMIT(recorder);
EXPORT_FINALIZE(recorder);

CLASS *recorder::oclass = NULL;
recorder *recorder::defaults = NULL;
using namespace std;

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
			PT_char1024,"property",get_property_offset(),PT_DESCRIPTION,"target property name",
			PT_char1024,"table",get_table_offset(),PT_DESCRIPTION,"table name to store samples",
			PT_char1024,"file",get_table_offset(),PT_DESCRIPTION,"file name (for tape compatibility)",
			PT_double,"interval[s]",get_interval_offset(),PT_DESCRIPTION,"sampling interval",
			PT_object,"connection",get_connection_offset(),PT_DESCRIPTION,"database connection",
			PT_set,"options",get_options_offset(),PT_DESCRIPTION,"SQL options",
				PT_KEYWORD,"UNITS",(set)MO_USEUNITS,PT_DESCRIPTION,"include units in column names",
			PT_char1024,"tags",get_tags_offset(),PT_DESCRIPTION,"tags to include with each record",
			NULL)<1){
				char msg[256];
				sprintf(msg, "unable to publish properties in %s",__FILE__);
				throw msg;
		}
	}
}

int recorder::create(void) 
{
	property_list = NULL;
	return 1; /* return 1 on success, 0 on failure */
}

int recorder::init(OBJECT *parent)
{
	if ( my()->name == NULL )
	{
		error("recorder must have a name for the measurement");
		return 0;
	}

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
		db->add_log("connecting recorder %s",my()->name);
	else
		return 2;
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

	const char *name = get_object(my())->get_name();
	measurement->start(name,tags);
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
    	gld_property *prop = new gld_property(my()->parent,token);
    	if ( ! prop->is_valid() )
    		exception("property %s is not valid",token);
        property_list->push_back(*prop);
        token = strtok_r(NULL,delim,&saveptr);
    }
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

