// switch_coordinator.cpp
// Copyright (C) 2016, Stanford University
// Author: David P. Chassin (dchassin@slac.stanford.edu)
//
// switch_coordinator - General purpose switch_coordinator objects
//
// == Synopsis ==
//
//   object switch_coordinator {
//     status {IDLE, ARMED, TOGGLE, DIRECT};
//     connect <name1>;
//     connect <name2>;
//     ...
//     armed [<name1>|<name2>|...];
//   }
//
// Description
// -----------
//
// Use 'connect' to add switches to the list of switches coordinated by the object.  Up to 64 switches can
// be coordinated. Each connected switch can be armed by setting the 'armed' object using the switch's name.
// Once all the armed switches have be set, use 'status' to activate the switching scheme.
//
// See also
// --------
// * powerflow
// ** switch
//

#include "powerflow.h"

EXPORT_CREATE(switch_coordinator);
EXPORT_INIT(switch_coordinator);
EXPORT_PRECOMMIT(switch_coordinator);
EXPORT_SYNC(switch_coordinator);
EXPORT_COMMIT(switch_coordinator);
EXPORT_NOTIFY(switch_coordinator);
EXPORT_METHOD(switch_coordinator,connect);
EXPORT_METHOD(switch_coordinator,arm);
EXPORT_METHOD(switch_coordinator,disarm);

CLASS *switch_coordinator::oclass = NULL;
switch_coordinator *switch_coordinator::defaults = NULL;

switch_coordinator::switch_coordinator(MODULE *module)
{
	if (oclass==NULL)
	{
		// register to receive notice for first top down. bottom up, and second top down synchronizations
		oclass = gld_class::create(module,"switch_coordinator",sizeof(switch_coordinator),PC_BOTTOMUP|PC_AUTOLOCK|PC_OBSERVER);
		if (oclass==NULL)
			throw "unable to register class switch_coordinator";
		else
			oclass->trl = TRL_PROVEN;

		defaults = this;
		if (gl_publish_variable(oclass,
			PT_enumeration,"status",get_status_offset(), 
				PT_DESCRIPTION,"switch coordination status",
				PT_KEYWORD, "IDLE", (enumeration)SCS_IDLE,
				PT_KEYWORD, "ARMED", (enumeration)SCS_ARMED,
				PT_KEYWORD, "TOGGLE", (enumeration)SCS_TOGGLE,
				PT_KEYWORD, "DIRECT", (enumeration)SCS_DIRECT,
			PT_method,"arm",get_arm_offset(), 
				PT_DESCRIPTION, "method to arm a switch",
			PT_method,"disarm",get_disarm_offset(), 
				PT_DESCRIPTION, "method to disarm a switch",
			PT_method,"connect",get_connect_offset(), 
				PT_DESCRIPTION,"method to connect switches",
			PT_set,"armed",get_armed_offset(), 
				PT_OUTPUT,
				PT_DESCRIPTION,"set of armed switches",
				PT_KEYWORD, "NONE", (set)0,
			NULL)<1){
				char msg[256];
				sprintf(msg, "unable to publish properties in %s",__FILE__);
				throw msg;
		}
		memset(this,0,sizeof(switch_coordinator));
	}
}

int switch_coordinator::create(void)
{
	debug("switch_coordinator::create()");
	memcpy(this,defaults,sizeof(*this));
	debug("switch_coordinator::create() -> 1");
	return 1; /* return 1 on success, 0 on failure */
}

int switch_coordinator::connect(char *value, size_t len)
{
	if ( value == NULL )
	{
		int len = -1;
		gld_property prop(this,"armed");
		for ( gld_keyword *kw = prop.get_first_keyword() ; kw != NULL ; kw = kw->get_next() )
		{
			len += strlen(kw->get_name()) + 1;
		}
		return len;
	}
	else if ( len == 0 ) // copy from name to object
	{
		debug("switch_coordinator::connect(char *value='%s', size_t len=0)", value);
		char *values = strdup(value);
		char *last_token = NULL;
		for ( char *this_token = strtok_r(values,"|",&last_token) ; this_token != NULL ; this_token = strtok_r(NULL,"|",&last_token) )
		{
			if ( strcmp(this_token,"NONE") == 0 )
				continue;
			gld_object *obj = gld_object::find_object(this_token);
			if ( obj == NULL || !obj->is_valid() )
			{
				warning("object '%s' is not found", this_token);
				debug("switch_coordinator::connect(char *value='%s', size_t len=0) -> 0", value);
				return 0;
			}
			if ( !obj->isa("switch") )
			{
				error("object '%s' is not a switch", this_token);
				debug("switch_coordinator::connect(char *name='%s', size_t len=0) -> 0", value);
				return 0;
			}
			if ( n_switches < 63 )
			{
				index[n_switches] = new gld_property(obj,"status");
				if ( index[n_switches]==NULL || ! index[n_switches]->is_valid() )
				{
					error("unable to get property 'status' of object '%s'", this_token);
					debug("switch_coordinator::connect(char *value='%s', size_t len=0) -> 0", value);
					return 0;
				}
				gld_property prop(this,"armed");
				gld_keyword *last;
				for ( last = prop.get_first_keyword() ; last->get_next()!=NULL ; last = last->get_next() ) 
				{
				}
				KEYWORD *next = new KEYWORD;
				strncpy(next->name,this_token,sizeof(next->name)-1);
				next->value = (unsigned int64)(pow(2,n_switches));
				next->next = NULL;
				((KEYWORD*)last)->next = next;
				n_switches++;
				verbose("added switch '%s' to arming list", this_token);
				debug("switch_coordinator::connect(char *value='%s', size_t len=0) -> 1", value);
			}
			else
			{
				error("too many switches connected (max 32) to connect '%s'", this_token);
				debug("switch_coordinator::connect(char *value='%s', size_t len=0) -> 0", value);
				free(values);
				return 0;
			}
		}
		free(values);
		debug("switch_coordinator::connect(char *value='%s', size_t len=0) -> 1", value);
		return 1;
	}
	else // copy from object to value
	{
		int sz = -1;
		gld_property prop(this,"armed");
		for ( gld_keyword *kw = prop.get_first_keyword() ; kw != NULL ; kw = kw->get_next() )
		{
			if ( strcmp(kw->get_name(),"NONE") != 0 )
				sz += strlen(kw->get_name()) + 1;
		}
		if ( (int)len >= sz )
		{
			int rv = 0;
			for ( gld_keyword *kw = prop.get_first_keyword() ; kw != NULL ; kw = kw->get_next() )
			{
				if ( strcmp(kw->get_name(),"NONE") != 0 )
					rv += sprintf(value+rv,"%s%s",rv>0?"|":"",kw->get_name());
			}
			return rv;
		}
		else
		{
			error("insufficient buffer space to store result of connect method read (needed %d bytes but only given %d bytes)", sz, len);
			return -1;
		}
	}
}

int switch_coordinator::arm(char *name, size_t len)
{
	if ( name == NULL )
		return 0;
	else if ( len == 0 )
	{
		if ( strcmp(name,"") == 0 )
			return 1;
		debug("switch_coordinator::arm(char *name='%s)", name);
		gld_property prop(this,"armed");
		gld_keyword *key;
		for ( key = prop.get_first_keyword() ; key->get_next()!=NULL ; key = key->get_next() )
		{
			if ( *key==name )
			{
				armed |= key->get_set_value();
				postnotify(prop);
				verbose("arming switch '%s'",name);
				debug("switch_coordinator::arm(char *name='%s) -> 1", name);
				return 1;
			}
		}
		error("unable to arm '%s', no such device connected", name);
		debug("switch_coordinator::arm(char *name='%s) -> 0", name);
		return 0;
	}
	else
	{
		strcpy(name,"");
		return 0;
	}
}

int switch_coordinator::disarm(char *name, size_t len)
{
	if ( name == NULL )
	{
		return 0;
	}
	else if ( len == 0 )
	{
		if ( strcmp(name,"") == 0 )
			return 1;
		debug("switch_coordinator::disarm(char *name='%s)", name);
		gld_property prop(this,"armed");
		gld_keyword *key;
		for ( key = prop.get_first_keyword() ; key->get_next()!=NULL ; key = key->get_next() )
		{
			if ( *key==name )
			{
				armed &= ~key->get_set_value();
				verbose("disarming switch '%s'",name);
				postnotify(prop);
				debug("switch_coordinator::disarm(char *name='%s) -> 1", name);
				return 1;
			}
		}
		error("unable to disarm '%s', no such device connected", name);
		debug("switch_coordinator::disarm(char *name='%s) -> 0", name);
		return 0;
	}
	else
	{
		strcpy(name,"");
		return 0;
	}
}

int switch_coordinator::init(OBJECT *parent)
{
	debug("switch_coordinator::init(OBJECT *parent=%s) -> 1", parent ? (const char*)get_object(parent)->get_name() : "(none)");
	return 1;
}

int switch_coordinator::precommit(TIMESTAMP t1)
{
	debug("switch_coordinator::precommit(TIMESTAMP t1='%s')", (const char*)gld_clock(t1).get_string());
	if ( status==SCS_TOGGLE )
	{
		// record the initial state of the armed switches
		states = armed;
		unsigned int n;
		for ( n = 0 ; n < n_switches ; n++ )
		{
			verbose("recording state of '%s'", (const char*)index[n]->get_name());
			states ^= ( index[n]->get_enumeration()==switch_object::OPEN ? 0 : 1<<n ); 
		}
	}
	debug("switch_coordinator::precommit(TIMESTAMP t1='%s') -> 1", (const char*)gld_clock(t1).get_string());
	return 1;
}

TIMESTAMP switch_coordinator::sync(TIMESTAMP t1)
{
	debug("switch_coordinator::sync(TIMESTAMP t1='%s')", (const char*)gld_clock(t1).get_string());
	if ( status==SCS_TOGGLE )
	{
		verbose("activating toggle scheme with armed=%s)", (const char*)get_armed_property().get_string());
		// set state of armed devices
		unsigned int n;
		for ( n = 0 ; n < n_switches ; n++ )
		{
			if ( armed&(1<<n) )
			{
				verbose("changing state for index[%d]='%s.%s'", n, (const char*)index[n]->get_object()->name, (const char*)index[n]->get_name());
				index[n]->setp((states&(1<<n)) ? ((enumeration)(switch_object::CLOSED)) : ((enumeration)(switch_object::OPEN)) );
			}
		}
	}
	else if ( status==SCS_DIRECT )
	{
		verbose("activating direct scheme with armed=%s)", (const char*)get_armed_property().get_string());
		// set state of armed devices
		unsigned int n;
		for ( n = 0 ; n < n_switches ; n++ )
		{
			enumeration old_state = index[n]->get_enumeration();
			enumeration new_state = (armed&(1<<n)) ? switch_object::CLOSED : switch_object::OPEN;
			if ( old_state != new_state )
			{
				verbose("setting state for index[%d]='%s.%s' = %s -> %s", n, (const char*)index[n]->get_object()->name, (const char*)index[n]->get_name(), old_state==switch_object::CLOSED ? "CLOSED" : "OPEN" , new_state==switch_object::CLOSED ? "CLOSED" : "OPEN" );
				index[n]->setp(new_state);
			}
		}
	}
	else
		verbose("no action, status=%s", (const char*)get_status_property().get_string());
	debug("switch_coordinator::sync(TIMESTAMP t1='%s') -> TS_NEVER", (const char*)gld_clock(t1).get_string());
	return TS_NEVER;
}

TIMESTAMP switch_coordinator::commit(TIMESTAMP t1, TIMESTAMP t2)
{
	debug("switch_coordinator::commit(TIMESTAMP t1='%s', TIMESTAMP t2='%s')", (const char*)gld_clock(t1).get_string(), (const char*)gld_clock(t2).get_string());
	if ( status==SCS_TOGGLE || status==SCS_DIRECT )
	{
		armed = 0;
		status = SCS_IDLE;
		verbose("switching done--changing status to IDLE");
	}
	debug("switch_coordinator::commit(TIMESTAMP t1='%s', TIMESTAMP t2='%s') -> TS_NEVER", (const char*)gld_clock(t1).get_string(), (const char*)gld_clock(t2).get_string());
	return TS_NEVER;
}

int switch_coordinator::postnotify(PROPERTY *prop, const char *value)
{
	debug("switch_coordinator::notify(PROPERTY *prop='%s', char *value='%s')", prop->name, value);
	if ( status==SCS_IDLE )
	{
		verbose("first switch armed--changing status to ARMED");
		status = SCS_ARMED;
	}
	debug("switch_coordinator::notify(PROPERTY *prop='%s', char *value='%s') -> 1", prop->name, value);
	return 1;
}
	
