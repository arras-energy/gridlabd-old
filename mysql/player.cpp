/** $Id: player.cpp 4738 2014-07-03 00:55:39Z dchassin $
    DP Chassin - July 2012
    Copyright (C) 2012 Battelle Memorial Institute
 **/

#ifdef HAVE_MYSQL

#include "database.h"

EXPORT_CREATE(player);
EXPORT_PRECOMMIT(player);
EXPORT_COMMIT(player);
EXPORT_INIT(player);

CLASS *player::oclass = NULL;
player *player::defaults = NULL;
int32_t player::maximum_threads = 0;

player::player(MODULE *module)
{
	if (oclass==NULL)
	{
		// register to receive notice for first top down. bottom up, and second top down synchronizations
		oclass = gld_class::create(module,"player",sizeof(player),PC_AUTOLOCK);
		if (oclass==NULL)
			throw "unable to register class player";
		else
			oclass->trl = TRL_PROTOTYPE;

		defaults = this;
		if (gl_publish_variable(oclass,
			PT_char256,"property",get_property_offset(),PT_DESCRIPTION,"target property name",
			PT_char1024,"table",get_table_offset(),PT_DESCRIPTION,"table name from which to read samples",
			PT_char1024,"query",get_query_offset(),PT_DESCRIPTION,"query to use to obtain data from table",
			PT_char1024,"file",get_table_offset(),PT_DESCRIPTION,"table name from which to read samples",
			PT_char8,"filetype",get_filetype_offset(),PT_DESCRIPTION,"table type from which samples are read",
			PT_char32,"mode",get_mode_offset(),PT_DESCRIPTION,"table output mode",
			PT_object,"connection",get_connection_offset(),PT_DESCRIPTION,"database connection",
			NULL)<1){
				char msg[256];
				sprintf(msg, "unable to publish properties in %s",__FILE__);
				throw msg;
		}
		gl_global_create("mysql::maximum_threads",PT_int32,&maximum_threads,PT_ACCESS,PA_PUBLIC,PT_DESCRIPTION,"maximum number of theads allowed during initialization sequence",NULL);
		memset(this,0,sizeof(player));
	}
}

int player::create(void) 
{
	memcpy(this,defaults,sizeof(*this));
	db = last_database;
	thread_rv = -1;
	thread_status = PTS_NONE;
	return 1; /* return 1 on success, 0 on failure */
}

void *init_proc(void *arg)
{
	player *instance = (player*)arg;
	instance->set_thread_rv(instance->init_async());
	return (void*)&(instance->get_thread_rv());
}

int player::init(OBJECT *parent)
{
	if ( maximum_threads == 0 )
	{
		// no async processing allowed
		return init_async();
	}
	else if ( thread_status == PTS_NONE ) 
	{
		// TODO: handle maximum_threads > 0 case
		debug("starting thread %lld",thread_id);
		// first time in -- begin async process and defer init
		set_parent(parent);
		thread_rv = -1;
		if ( ! pthread_create(&thread_id,NULL,init_proc,(void*)this) )
		{
			thread_status = PTS_ERROR;
			return 0;
		}
		else
		{
			thread_status = PTS_RUNNING;
			return 2;
		}
	}
	else if ( thread_status == PTS_RUNNING )
	{	
		// second time in -- wait for async process to complete
		debug("waiting for thread %lld",thread_id);
		int rv = pthread_join(thread_id,NULL);
		debug("thread %lld done, retval=%d",thread_id,thread_rv);
		thread_status = PTS_DONE;
		return rv ? thread_rv : 0;
	}
	else if ( thread_status == PTS_ERROR )
	{
		exception("thread error status detected");
		return 0;
	}
	else if ( thread_status == PTS_DONE )
	{
		// thread already done but init if still deferred
		thread_rv = init_async();
		return thread_rv;
	}
	else
	{
		exception("invalid thread status %d", thread_status);
		return 0;
	}
}

int player::init_async()
{
	// check the connection
	if ( get_connection()!=NULL )
		db = (database*)(get_connection()+1);
	if ( db==NULL )
		exception("no database connection available or specified");
	if ( !db->isa("database") )
		exception("connection is not a mysql database");
	gl_verbose("connection to mysql server '%s', schema '%s' ok", db->get_hostname(), db->get_schema());

	// check mode
	if ( strlen(mode)>0 )
	{
		unsigned int options = 0xffffffff;
		struct {
			const char *str;
			set bits;
		} modes[] = {
			{"r",	0x0000},
			{"r+",	0x0000},
			{"w",	0xffff},
			{"w+",	0xffff},
			{"a",	0xffff},
			{"a+",	0xffff},
		};
		size_t n;
		for ( n=0 ; n<sizeof(modes)/sizeof(modes[0]) ; n++ )
		{
			if ( strcmp(mode,modes[n].str)==0 )
			{
				options = modes[n].bits;
				break;
			}
		}
		if ( options == 0xffffffff )
			exception("mode '%s' is not recognized",(const char*)mode);
		else if ( options==0xffff )
			exception("mode '%s' is not valid for a recorder", (const char*)mode);
	}

	// connect the target property
	if ( get_parent()==NULL )
		exception("parent is not set");
	target.set_object(get_parent());
	char propname[64]="", propunit[64]="";
	switch ( sscanf(get_property(),"%[^[][%[^]]",propname,propunit) ) {
	case 2:
		if ( !unit.set_unit(propunit) )
			exception("property '%s' has an invalid unit", get_property());
		// drop through
	case 1:
		strncpy(field,propname,sizeof(field)-1);
		target.set_property(propname);
		scale = 1.0;
		if ( unit.is_valid() && target.get_unit()!=NULL )
		{
			target.get_unit()->convert(unit,scale);
			sprintf(field,"%s[%s]",propname,propunit);
		}
		else if ( propunit[0]=='\0' && options&MO_USEUNITS && target.get_unit() )
			sprintf(field,"%s[%s]",propname,target.get_unit()->get_name());
		break;
	default:
		exception("property '%s' is not valid", get_property());
		break;
	}

	// check for table existence and create if not found
	if ( !target.is_valid() )
		exception("target property '%s' is not valid", get_property());
	if ( strcmp(get_table(),"")!=0 && !db->table_exists(get_table()) )
		exception("table '%s' does not exist", get_table());

	// run the main query
	gld_clock start;
	if ( strcmp(get_query(),"")==0 )
		data = db->select("SELECT t,`%s` FROM `%s` WHERE t>=from_unixtime(%llu) ORDER BY id",
			get_property(),get_table(),db->convert_to_dbtime(start.get_timestamp()));
	else
		data = db->select(get_query());
	if ( data==NULL )
		return 0; // no data
	n_rows = mysql_num_rows(data);
	n_fields = mysql_num_fields(data);
	row_num = 0;
	gl_verbose("%s: %d rows with %d fields found", get_name(), n_rows, n_fields);
	if ( n_rows == 0 )
		return 0; // no data

	// fetch first row
	row = mysql_fetch_row(data);
	row_num++;
	fields = mysql_fetch_fields(data);

	// get the time of the next data item (assume in first column)
	gld_clock next(row[0]);
	next_t = db->convert_from_dbtime(next.get_timestamp());
	gl_verbose("%s: row %d, %s='%s', %s='%s'", get_name(), row_num, fields[0].name, row[0], fields[1].name, row[1]);
	return 1;
}

int player::precommit(TIMESTAMP t0)
{
	if ( row && t0==next_t)
	{
		target.from_string(row[1]);
		row = mysql_fetch_row(data);
		if ( row==NULL )
			return 1;
		row_num++;
		gl_verbose("%s: row %d, %s='%s', %s='%s'", get_name(), row_num, fields[0].name, row[0], fields[1].name, row[1]);
		gld_clock next(row[0]);
		next_t = db->convert_from_dbtime(next.get_timestamp());
	}
	return 1;
}

TIMESTAMP player::commit(TIMESTAMP t0, TIMESTAMP t1)
{
	if ( row )
		return next_t;
	else
		return TS_NEVER;
}

#endif // HAVE_MYSQL
