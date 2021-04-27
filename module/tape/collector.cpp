/** $Id: collector.c 4738 2014-07-03 00:55:39Z dchassin $
	Copyright (C) 2008 Battelle Memorial Institute
	@file collector.c
	@addtogroup collector Collectors
	@ingroup tapes

	Tape collectors are different from recorders in that they aggregate multiple
	object properties into a single value.  They do not use the parent property, but
	instead use the 'group' property to form a collection of objects over which the
	aggregate is taken.  The following properties are also available for collectors
	- \p group specifies the grouping rule for creating the collection.  Groups
	   may be specified using \p property \p condition \p value, where \p property is one
	   of \p class, \p size, \p parent, \p id, \p rank, or any registered property of the object.
	- \p property value of collectors must be in the form \p aggregator(property) where
	the \p aggregator is one of \p min, \p max, \p count, \p avg, \p std, \p mean, \p var, 
	\p mbe, \p kur. If a \p | is used instead of parentheses, then the absolute value 
	of the property is used.
	If the property is a complex number, the property must be specified in the form
	\p property.part, where \p part is one of \p real, \p imag, \p mag, \p ang, or \p arg.  
	Angles are in degrees, and \p arg is in radians. 
 @{
 **/

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <stdarg.h>
#include "gridlabd.h"
#include "object.h"
#include "aggregate.h"

#include "tape.h"
#include "file.h"
#include "odbc.h"

CLASS *collector_class = NULL;
static OBJECT *last_collector = NULL;

EXPORT int create_collector(OBJECT **obj, OBJECT *parent)
{
	*obj = gl_create_object(collector_class);
	if (*obj!=NULL)
	{
		struct collector *my = OBJECTDATA(*obj,struct collector);
		last_collector = *obj;
		gl_set_parent(*obj,parent);
		strcpy(my->file,"");
		strcpy(my->filetype,"txt");
		strcpy(my->delim,",");
		strcpy(my->group,"");
		my->interval = TS_NEVER; /* transients only */
		my->dInterval = -1.0;
		my->last.ts = -1;
		strcpy(my->last.value,"");
		my->limit = 0;
		my->samples = 0;
		my->status = TS_INIT;
		my->trigger[0]='\0';
		my->format = 0;
		my->aggr = NULL;
		my->property = NULL;
		my->property_len = 0;
		return 1;
	}
	return 0;
}

static int collector_open(OBJECT *obj)
{
	char32 type="file";
	char1024 fname="";
	char32 flags="w";
	TAPEFUNCS *tf = 0;
	struct collector *my = OBJECTDATA(obj,struct collector);
	
	if ( my->property == NULL )
	{
		gl_error("at least one property must be specified");
		return 0;
	}

	my->interval = (int64)(my->dInterval/TS_SECOND);

	/* if prefix is omitted (no colons found) */
	if (sscanf(my->file,"%32[^:]:%1024[^:]:%[^:]",(char*)type,(char*)fname,(char*)flags)==1)
	{
		/* filename is file by default */
		strcpy(fname,my->file);
		strcpy(type,"file");
	}

	/* if no filename given */
	if (strcmp(fname,"")==0)
	{
		char *p;
		/* use group spec as default file name */
		sprintf(fname,"%s.%s",(char*)(my->group),(char*)(my->filetype));

		/* but change disallowed characters to _ */
		for (p=fname; *p!='\0'; p++)
		{
			if (!isalnum(*p) && *p!='-' && *p!='.')
				*p='_';
		}
	}

	/* if type is file or file is stdin */
	tf = get_ftable(type);
	if(tf == NULL)
		return 0;
	my->ops = tf->collector;
	if(my->ops == NULL)
		return 0;
	set_csv_options();
	return my->ops->open(my, fname, flags);
}

static int write_collector(struct collector *my, char *ts, char *value)
{
	int rc=my->ops->write(my, ts, value);
	if ( (my->flush==0 || (my->flush>0 && gl_globalclock%my->flush==0)) && my->ops->flush!=NULL ) 
		my->ops->flush(my);
	return rc;
}

static void close_collector(struct collector *my){
	if(my->ops){
		my->ops->close(my);
	}
}

static TIMESTAMP collector_write(OBJECT *obj)
{
	struct collector *my = OBJECTDATA(obj,struct collector);
	char ts[64];
	if (my->format==0)
	{
		//time_t t = (time_t)(my->last.ts*TS_SECOND);
		//strftime(ts,sizeof(ts),timestamp_format, gmtime(&t));

		DATETIME dt;
		gl_localtime(my->last.ts, &dt);
		gl_strtime(&dt, ts, sizeof(ts));
	}
	else
		sprintf(ts,"%" FMT_INT64 "d", my->last.ts);
	if ((my->limit>0 && my->samples>my->limit) /* limit reached */
		|| write_collector(my,ts,my->last.value)==0) /* write failed */
	{
		if (my->ops){
			close_collector(my);
		} else {
			gl_error("collector_write: no TAPEOP structure when closing the tape");
		}
		my->status = TS_DONE;
	}
	else
		my->samples++;
	return TS_NEVER;
}

#define BLOCKSIZE 1024
EXPORT int method_collector_property(OBJECT *obj, ...)
{
	struct collector *my = OBJECTDATA(obj,struct collector);
	va_list args;
	va_start(args,obj);
	void *arg0 = va_arg(args,void*);

	// legacy calls
	char *value = (char*)arg0;
	size_t size = va_arg(args,size_t);
	if ( value == NULL ) // check size needed to hold result
	{
		if ( size == 0 )
			return my->property?strlen(my->property)+1:0;
		else
			return (my->property?strlen(my->property)+1:0) < size;
	}
	else if ( size == 0 ) // copy from data
	{
		size_t len = strlen(value);
		gl_verbose("adding property '%s' to collector:%d",value,obj->id);
		if ( my->property_len < len || my->property == NULL )
		{
			size_t need = len + (my->property==NULL ? 0 : strlen(my->property)) + 1;
	                my->property_len = ((need/BLOCKSIZE)+1)*BLOCKSIZE;
	                my->property = (char*)realloc(my->property,my->property_len);
			if ( my->property == NULL )
			{
				gl_error("memory allocation failed");
				my->property_len = 0;
				return 0;
			}
			strcpy(my->property,value);
		}	
		else
		{
			strcat(my->property,",");
			strcat(my->property,value);
		}
		return 1;
	}
	else // copy to data
	{
		if ( size > strlen(my->property) )
		{
			strcpy(value,my->property);
			return strlen(my->property);
		}
		else
		{
			return 0;
		}
	}

	va_end(args);
}

AGGREGATION *link_aggregates(char *aggregate_list, char *group)
{
	char *item;
	AGGREGATION *first=NULL, *last=NULL;
	char *list = strdup(aggregate_list);
	char *last_token;
	for (item=strtok_r(list,",",&last_token); item!=NULL; item=strtok_r(NULL,",",&last_token))
	{
		AGGREGATION *aggr = gl_create_aggregate(item,group);
		if (aggr!=NULL)
		{
			/* TODO: ideally the aggregation group program from the previous should be reused */
			if (first==NULL) first=aggr; else last->next=aggr;
			last=aggr;
			aggr->next = NULL;
		}
		else {
			free(list);
			return NULL; // allowable to have null (zero-length) aggrs, but only give time-varying aggregates
		}
	}
	free(list);
	return first;
}

int read_aggregates(AGGREGATION *aggr, char *buffer, int size)
{
	AGGREGATION *p;
	int count=0;
	char tmp[1024];
	char32 fmt;

	gl_global_getvar("double_format", fmt, 32);
	for ( p = aggr; p != NULL ; p = p->next )
	{
		int sz = snprintf(tmp,sizeof(tmp)-1,fmt,gl_run_aggregate(p));
		if ( count + sz >= size )
		{
			gl_error("tape/collector.c:read_aggregates(): buffer too small to handle output size");
			return 0;
		}
		if ( count > 0 ) 
		{
			strcpy(buffer+count++,",");
		}
		strcpy(buffer+count,tmp);
		count += sz;
	}
	return count;
}



EXPORT TIMESTAMP sync_collector(OBJECT *obj, TIMESTAMP t0, PASSCONFIG pass)
{
	struct collector *my = OBJECTDATA(obj,struct collector);
	typedef enum {NONE='\0', LT='<', EQ='=', GT='>'} COMPAREOP;
	COMPAREOP comparison;
	char1024 buffer = "";
	
	if(my->status == TS_DONE){
		return TS_NEVER;
	}

	/* connect to property */
	if (my->aggr==NULL)
		my->aggr = link_aggregates(my->property,my->group);

	/* read property */
	if (my->aggr==NULL)
	{
		sprintf(buffer,"'%s' contains an aggregate that is not found in the group '%s'", my->property, (char*)my->group);
		my->status = TS_ERROR;
		goto Error;
	}
	
	if((my->status == TS_OPEN) && (t0 > obj->clock)){
		obj->clock = t0;
		if((my->interval > 0) && (my->last.ts < t0) && (my->last.value[0] != 0)){
			collector_write(obj);
			//my->last.ts = t0;
			my->last.value[0] = 0;
		}
	}

	//if(my->aggr != NULL && (my->aggr = link_aggregates(my->property,my->group)),read_aggregates(my->aggr,buffer,sizeof(buffer))==0)
	if(my->aggr != NULL && (my->interval == 0 || my->interval == -1)){
		if(read_aggregates(my->aggr,buffer,sizeof(buffer))==0)
		{
			sprintf(buffer,"unable to read aggregate '%s' of group '%s'", my->property, (char*)my->group);
			close_collector(my);
			my->status = TS_ERROR;
		}
	}

	if(my->aggr != NULL && my->interval > 0){
		if((t0 >= my->last.ts + my->interval) || (t0 == my->last.ts)){
			if(read_aggregates(my->aggr,buffer,sizeof(buffer))==0){
				sprintf(buffer,"unable to read aggregate '%s' of group '%s'", my->property, (char*)my->group);
				close_collector(my);
				my->status = TS_ERROR;
			}
			my->last.ts = t0;
		}
	}

	/* check trigger, if any */
	comparison = (COMPAREOP)my->trigger[0];
	if (comparison!=NONE)
	{
		int desired = comparison==LT ? -1 : (comparison==EQ ? 0 : (comparison==GT ? 1 : -2));

		/* if not trigger or can't get access */
		int actual = strcmp(buffer,my->trigger+1);
		if (actual!=desired || (my->status==TS_INIT && !collector_open(obj))){
			/* better luck next time */
			return (my->interval==0 || my->interval==-1) ? TS_NEVER : t0+my->interval;
		}
	}
	else if (my->status==TS_INIT && !collector_open(obj)){
		close_collector(my);
		return TS_NEVER;
	}

	if(my->last.ts < 1 && my->interval != -1){
		my->last.ts = t0;
	}

	/* write tape */
	if(my->status == TS_OPEN){	
		if(my->interval == 0 /* sample on every pass */
			|| ((my->interval == -1) && my->last.ts != t0 && strcmp(buffer, my->last.value) != 0) /* sample only when value changes */
			){
			strncpy(my->last.value, buffer, sizeof(my->last.value));
			my->last.ts = t0;
			collector_write(obj);
		} else if(my->interval > 0 && my->last.ts == t0){
			strncpy(my->last.value, buffer, sizeof(my->last.value));
		}
	}
Error:
	if (my->status==TS_ERROR)
	{
		gl_error("collector %d %s\n",obj->id, (char*)buffer);
		my->status=TS_DONE;
		return 0; /* failed */
	}

	return (my->interval==0 || my->interval==-1) ? TS_NEVER : my->last.ts+my->interval;
}

/**@}*/
