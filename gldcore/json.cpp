/** $Id: json.c 4738 2014-07-03 00:55:39Z dchassin $
	Copyright (C) 2008 Battelle Memorial Institute
	@file json.c
	@addtogroup mapping JSON formatting
	@ingroup core

	JSON files may be extracted using using the
	\p --output file.json command line option.  The function
	json_dump() is called and when a JSON dump
	is performed, the simulation is not run.

 @{
 **/

#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include "class.h"
#include "object.h"
#include "output.h"
#include "exception.h"
#include "module.h"
#include "timestamp.h"
#include "json.h"

static unsigned int json_version = 0;
FILE *json = NULL;
static int json_write(const char *fmt,...)
{
	int len;
	va_list ptr;
	va_start(ptr,fmt);
	len = vfprintf(json,fmt,ptr);
	va_end(ptr);
	return len;
}

#define FIRST(NAME,FORMAT,VALUE) (len += json_write("\n\t\t\t\"%s\" : \""FORMAT"\"",NAME,VALUE))
#define TUPLE(NAME,FORMAT,VALUE) (len += json_write(",\n\t\t\t\"%s\" : \""FORMAT"\"",NAME,VALUE))

static int json_modules(FILE *fp)
{
	int len = 0;
	MODULE *mod;
	len += json_write(",\n\t\"modules\" : {");
	for ( mod = module_get_first() ; mod != NULL ; mod = mod->next )
	{
		if ( mod != module_get_first() )
			len += json_write(",");
		len += json_write("\n\t\t\"%s\" : {",mod->name);
		FIRST("major","%d",mod->major);
		TUPLE("minor","%d",mod->minor);
		// TODO more info
		len += json_write("\n\t\t}");
	}
	len += json_write("\n\t}");
	output_debug("json_modules() wrote %d bytes",len);
	return len;
}

static int json_properties(FILE *fp)
{
	int len = 0;
	PROPERTYTYPE ptype;
	len += json_write(",\n\t\"types\" : {");
	for ( ptype = property_getfirst_type() ; ptype != PT_void ; ptype = property_getnext_type(ptype) )
	{
		PROPERTYSPEC *spec = property_getspec(ptype);
		PROPERTYCOMPAREOP op;
		bool first = true;
		if ( ptype != property_getfirst_type() )
			len += json_write(",");
		len += json_write("\n\t\t\"%s\" : {",spec->name);
		len += json_write("\n\t\t\t\"xsdtype\" : \"%s\"",spec->xsdname);
		if ( spec->default_value != NULL )
			len += json_write(",\n\t\t\t\"default\" : \"%s\"",spec->default_value);
		if ( spec->size > 0 )
			len += json_write(",\n\t\t\t\"memsize\" : \"%d\"",spec->size);
		if ( spec->csize > 0 )
		{
			len += json_write(",\n\t\t\t\"strsize\" : \"%d\"",spec->csize);
			len += json_write(",\n\t\t\t\"has_tostr\" : \"%s\"",spec->data_to_string?"True":"False");
			len += json_write(",\n\t\t\t\"has_fromstr\" : \"%s\"",spec->string_to_data?"True":"False");
		}
		len += json_write(",\n\t\t\t\"has_create\" : \"%s\"",spec->create?"True":"False");
		len += json_write(",\n\t\t\t\"has_stream\" : \"%s\"",spec->stream?"True":"False");
		len += json_write(",\n\t\t\t\"compareops\" : {");
		for ( op = _TCOP_FIRST ; op < _TCOP_LAST ; op = PROPERTYCOMPAREOP(op+1) )
		{
			if ( spec->compare[op].fn != NULL )
			{
				if ( ! first )
					len += json_write(",");
				else
					first = false;
				len += json_write("\n\t\t\t\t\"%s\" : {\"nargs\": \"%d\"}",spec->compare[op].str, spec->compare[op].trinary ? 3 : 2);
			}
		}
		len += json_write("\n\t\t\t}");
		len += json_write("\n\t\t}");
	}
	len += json_write("\n\t}");
	output_debug("json_properties() wrote %d bytes",len);
	return len;
}


static int json_classes(FILE *fp)
{
	int len = 0;
	CLASS *oclass;
	len += json_write(",\n\t\"classes\" : {");
	for ( oclass = class_get_first_class() ; oclass != NULL ; oclass = oclass->next )
	{
		PROPERTY *prop;
		if ( oclass != class_get_first_class() )
			len += json_write(",");
		len += json_write("\n\t\t\"%s\" : {",oclass->name);
		FIRST("object_size","%u",oclass->size);
		if ( oclass->parent )
		{
			TUPLE("parent","%s",oclass->parent->name );
		}
		TUPLE("trl","%u",oclass->trl);
		TUPLE("profiler.numobjs","%u",oclass->profiler.numobjs);
		TUPLE("profiler.clocks","%llu",oclass->profiler.clocks);
		TUPLE("profiler.count","%u",oclass->profiler.count);
		if ( oclass->has_runtime ) TUPLE("runtime","%s",oclass->runtime);
		if ( oclass->pmap != NULL )
			len += json_write(",");
		for ( prop = oclass->pmap ; prop != NULL ; prop=(prop->next?prop->next:(prop->oclass->parent?prop->oclass->parent->pmap:NULL)) )
		{
			KEYWORD *key;
			const char *ptype = class_get_property_typename(prop->ptype);
			if ( ptype == NULL )
				continue;
			if ( prop != oclass->pmap )
				len += json_write(",");
			len += json_write("\n\t\t\t\"%s\" : {",prop->name);
			len += json_write("\n\t\t\t\t\"type\" : \"%s\",",ptype);
			char access[1024] = "";
			switch ( prop->access ) {
			case PA_PUBLIC: strcpy(access,"PUBLIC"); break;
			case PA_REFERENCE: strcpy(access,"REFERENCE"); break;
			case PA_PROTECTED: strcpy(access,"PROTECTED"); break;
			case PA_PRIVATE: strcpy(access,"PRIVATE"); break;
			case PA_HIDDEN: strcpy(access,"HIDDEN"); break;
			case PA_N: strcpy(access,"NONE"); break;
			default:
				if ( prop->access & PA_R ) strcat(access,"R");
				if ( prop->access & PA_W ) strcat(access,"W");
				if ( prop->access & PA_S ) strcat(access,"S");
				if ( prop->access & PA_L ) strcat(access,"L");
				if ( prop->access & PA_H ) strcat(access,"H");
				break;
			}
			len += json_write("\n\t\t\t\t\"access\" : \"%s\"",access);
			for ( key = prop->keywords ; key != NULL ; key = key->next )
			{
				if ( key == prop->keywords )
				{
					len += json_write(",\n\t\t\t\t\"keywords\" : {");
				}
				len += json_write("\n\t\t\t\t\t\"%s\" : \"0x%x\"",key->name,key->value);
				if ( key->next == NULL )
					len += json_write("\n\t\t\t\t}");
				else
					len += json_write(",");
			}
			if ( prop->unit != NULL )
			{
				json_write(",\n\t\t\t\t\"unit\" : \"%s\"",prop->unit->name);
			}
			if ( prop->default_value != NULL )
			{
				PROPERTYSPEC *spec = property_getspec(prop->ptype);
				if ( spec != NULL && prop->default_value != spec->default_value )
				{	
					json_write(",\n\t\t\t\t\"default\" : \"%s\"",prop->default_value);
				}
			}
			len += json_write("\n\t\t\t}");
		}
		len += json_write("\n\t\t}");
	}
	len += json_write("\n\t}");
	output_debug("json_classes() wrote %d bytes",len);
	return len;
}

static int json_globals(FILE *fp)
{
	int len = 0;
	GLOBALVAR *var;
	len += json_write(",\n\t\"globals\" : {");

	/* for each module */
	for ( var = global_find(NULL) ; var != NULL ; var = global_getnext(var) )
	{
		char buffer[1024];
		if ( global_getvar(var->prop->name,buffer,sizeof(buffer)-1) ) // only write globals that can be extracted
		{
			KEYWORD *key;
			PROPERTYSPEC *pspec = property_getspec(var->prop->ptype);
			if ( var != global_find(NULL) )
				len += json_write(",");
			len += json_write("\n\t\t\"%s\" : {", var->prop->name);
			len += json_write("\n\t\t\t\"type\" : \"%s\",", pspec->name);
			for ( key = var->prop->keywords ; key != NULL ; key = key->next )
			{
				if ( key == var->prop->keywords )
				{
					len += json_write("\n\t\t\t\"keywords\" : {");
				}
				len += json_write("\n\t\t\t\t\"%s\" : \"0x%x\"",key->name,key->value);
				if ( key->next == NULL )
					len += json_write("\n\t\t\t}");
				len += json_write(",");
			}
			if ( buffer[0] == '\"' )
				len += json_write("\n\t\t\t\"value\" : %s",buffer);
			else
				len += json_write("\n\t\t\t\"value\" : \"%s\"", buffer);
			len += json_write("\n\t\t}");
		}
	}
	len += json_write("\n\t}");
	output_debug("json_globals() wrote %d bytes",len);
	return len;
}

static int json_objects(FILE *fp)
{
	int len = 0;
	OBJECT *obj;
	len += json_write(",\n\t\"objects\" : {");

	/* scan each object in the model */
	for ( obj = object_get_first() ; obj != NULL ; obj = obj->next )
	{
		PROPERTY *prop;
		CLASS *pclass;
		char buffer[1024];
		if ( obj != object_get_first() )
			len += json_write(",");
		if ( obj->oclass == NULL ) // ignore objects with no defined class
			continue;
		if ( obj->name ) 
			len += json_write("\n\t\t\"%s\" : {",obj->name);
		else
			len += json_write("\n\t\t\"%s:%d\" : {", obj->oclass->name, obj->id);
		FIRST("id","%d",obj->id);
		TUPLE("class","%s",obj->oclass->name);

		/* handle special case for powerflow module handling of parent */
		OBJECT **topological_parent = object_get_object_by_name(obj,"topological_parent");
		if ( topological_parent != NULL )
			obj->parent = *topological_parent;

		if ( obj->parent != NULL )
		{
			if ( obj->parent->name == NULL )
				len += json_write(",\n\t\t\t\"parent\" : \"%s:%d\"",obj->parent->oclass->name,obj->parent->id);
			else
				TUPLE("parent","%s",obj->parent->name);
		}
		if ( ! isnan(obj->latitude) ) TUPLE("latitude","%f",obj->latitude);
		if ( ! isnan(obj->longitude) ) TUPLE("longitude","%f",obj->longitude);
		if ( obj->groupid[0] != '\0' ) TUPLE("groupid","%s",(const char*)obj->groupid);
		TUPLE("rank","%u",(unsigned int)obj->rank);
		if ( convert_from_timestamp(obj->clock,buffer,sizeof(buffer)) )
			TUPLE("clock","%s",buffer);
		if ( obj->valid_to > TS_ZERO && obj->valid_to < TS_NEVER ) TUPLE("valid_to","%llu",(int64)(obj->valid_to));
		TUPLE("schedule_skew","%llu",obj->schedule_skew);
		if ( obj->in_svc > TS_ZERO && obj->in_svc < TS_NEVER ) TUPLE("in","%llu",(int64)(obj->in_svc));
		if ( obj->out_svc > TS_ZERO && obj->out_svc < TS_NEVER ) TUPLE("out","%llu",(int64)(obj->out_svc));
		TUPLE("rng_state","%llu",(int64)(obj->rng_state));
		TUPLE("heartbeat","%llu",(int64)(obj->heartbeat));
		(len += json_write(",\n\t\t\t\"%s\" : \"0x%llx%llx\"","guid",(int64)(obj->guid[0]),(int64)(obj->guid[1])));
		TUPLE("flags","0x%llx",(int64)(obj->flags));
		for ( pclass = obj->oclass ; pclass != NULL ; pclass = pclass->parent )
		{
			for ( prop = pclass->pmap ; prop!=NULL && prop->oclass == pclass->pmap->oclass; prop = prop->next )
			{
				char buffer[1024];
				if ( prop->access != PA_PUBLIC )
					continue;
				if ( prop->ptype == PT_enduse )
					continue;
				const char *value = object_property_to_string(obj,prop->name, buffer, sizeof(buffer)-1);
				if ( value == NULL )
					continue; // ignore values that don't convert propertly
				int len = strlen(value);
				if ( value[0] == '{' && value[len-1] == '}')
					len += json_write(",\n\t\t\t\"%s\" : %s", prop->name, value);
				else if ( value[0] == '"' && value[len-1] == '"')
					len += json_write(",\n\t\t\t\"%s\": %s", prop->name, value);
				else
					TUPLE(prop->name,"%s",value);
			}
		}
		len += json_write("\n\t\t}");
	}

	len += json_write("\n\t}");
	output_debug("json_objects() wrote %d bytes",len);
	return len;
}

// return number of bytes written
int json_output(FILE *fp)
{
	int len = 0;
	json = fp;
	len += json_write("{\t\"application\": \"gridlabd\",\n");
	len += json_write("\t\"version\" : \"%u.%u.%u\"",global_version_major,global_version_minor,json_version);
	len += json_modules(fp);
	len += json_properties(fp);
	len += json_classes(fp);
	len += json_globals(fp);
	len += json_objects(fp);
	len += json_write("\n}\n");
	output_debug("json_output() wrote %d bytes",len);
	return len;
}


// returns 0 on success, non-zero on failure
int json_dump(const char *filename)
{
	const char *ext, *basename;
	size_t b;
	char fname[1024];
	FILE *fp;

	/* handle default filename */
	if (filename==NULL)
		filename="gridlabd.json";

	/* find basename */
	b = strcspn(filename,"/\\:");
	basename = filename + (b<strlen(filename)?b:0);

	/* find extension */
	ext = strrchr(filename,'.');

	/* if extension is valid */
	if (ext!=NULL && ext>basename)

		/* use filename verbatim */
		strcpy(fname,filename);

	else

		/* append default extension */
		strcat(strcpy(fname,filename),".json");

	/* open file */
	fp = fopen(fname,"w");
	if (fp==NULL)
		throw_exception("json_dump(const char *filename='%s'): %s", filename, strerror(errno));
		/* TROUBLESHOOT
			The system was unable to output the json data to the specified file.  
			Follow the recommended solution based on the error message provided and try again.
		 */

	/* output data */
	int len = json_output(fp);

	/* close file */
	fclose(fp);

	return len > 0;
}
