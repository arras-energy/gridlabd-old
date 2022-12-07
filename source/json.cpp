/** json.cpp
	Copyright (C) 2018 Regents of the Leland Stanford Junior University 

	@file json.c
	@addtogroup mapping JSON formatting
	@ingroup core

	JSON files may be extracted using using the
	\p --output file.json command line option.  The function
	json_dump() is called and when a JSON dump
	is performed, the simulation is not run.

 @{
 **/

#include "gldcore.h"

SET_MYCONTEXT(DMC_SAVE)

DEPRECATED int json_dump(const char *filename)
{
	GldJsonWriter json(filename);
	return json.dump();
}

unsigned int GldJsonWriter::version = 0;

GldJsonWriter::GldJsonWriter(const char *f)
{
	filename = strdup(f);
}

GldJsonWriter::~GldJsonWriter(void)
{
	free((void*)filename);
}

const char * escape(const char *buffer, size_t len)
{
	static char *result = NULL;
	static size_t result_len = 0;
	if ( len >= result_len*5+1 )
	{
		result_len = (len>1024?len:1024)*5+1;
		result = (char*)realloc(result,result_len);
		if ( result == NULL )
			throw_exception("escape(const char *buffer=%p, size_t len=%llu): memory allocation failed",buffer,len);
	}
	char *p = result;
	const char *c;
	for ( c = buffer ; *c != '\0' && c < buffer+len ; c++)
	{
		switch ( *c )
		{
		case '"':
			*p++ = '\\';
			*p++ = '"';
			break;
		case '\\':
			*p++ = '\\';
			*p++ = '\\';
			break;
		// DPC: solidus is in the JSON spec (http://www.ecma-international.org/publications/files/ECMA-ST/ECMA-404.pdf, Figure 5)
		// but it's not desirable or necessary (so far as I can tell) to escape it for gridlabd strings
		// case '/':
		// 	*p++ = '\\';
		// 	*p++ = '/';
		// 	break;
		case '\b':
			*p++ = '\\';
			*p++ = 'b';
			break;
		case '\f':
			*p++ = '\\';
			*p++ = 'f';
			break;
		case '\n':
			*p++ = '\\';
			*p++ = 'n';
			break;
		case '\r':
			*p++ = '\\';
			*p++ = 'r';
			break;
		case '\t':
			*p++ = '\\';
			*p++ = 't';
			break;
		default:
			if ( *c >= 32 && *c < 127 )
			{
				*p++ = *c;
			}
			else
			{
				p += sprintf(p,"\\u%04hX", (unsigned short)*c);
			}
			break;
		}
	}
	*p = '\0';
	return result;
}

int GldJsonWriter::write(const char *fmt,...)
{
	int len;
	va_list ptr;
	va_start(ptr,fmt);
	len = vfprintf(json,fmt,ptr);
	va_end(ptr);
	return len;
}

#define FIRST(N,F,V) (len += write("\n\t\t\t\"%s\" : \"" F "\"",N,V))
#define TUPLE(N,F,V) (len += write(",\n\t\t\t\"%s\" : \"" F "\"",N,V))

int GldJsonWriter::write_modules(FILE *fp)
{
	int len = 0;
	MODULE *mod;
	len += write(",\n\t\"modules\" : {");
	for ( mod = module_get_first() ; mod != NULL ; mod = mod->next )
	{
		if ( mod != module_get_first() )
			len += write(",");
		len += write("\n\t\t\"%s\" : {",mod->name);
		FIRST("major","%d",mod->major);
		TUPLE("minor","%d",mod->minor);
		// TODO more info
		len += write("\n\t\t}");
	}
	len += write("\n\t}");
	IN_MYCONTEXT output_debug("GldJsonWriter::modules() wrote %d bytes",len);
	return len;
}

int GldJsonWriter::write_properties(FILE *fp)
{
	int len = 0;
	PROPERTYTYPE ptype;
	len += write(",\n\t\"types\" : {");
	for ( ptype = property_getfirst_type() ; ptype != PT_void ; ptype = property_getnext_type(ptype) )
	{
		PROPERTYSPEC *spec = property_getspec(ptype);
		PROPERTYCOMPAREOP op;
		bool first = true;
		if ( ptype != property_getfirst_type() )
			len += write(",");
		len += write("\n\t\t\"%s\" : {",spec->name);
		len += write("\n\t\t\t\"xsdtype\" : \"%s\"",spec->xsdname);
		if ( spec->default_value != NULL )
			len += write(",\n\t\t\t\"default\" : \"%s\"",spec->default_value);
		if ( spec->size > 0 )
			len += write(",\n\t\t\t\"memsize\" : \"%d\"",spec->size);
		if ( spec->csize > 0 )
		{
			len += write(",\n\t\t\t\"strsize\" : \"%d\"",spec->csize);
			len += write(",\n\t\t\t\"has_tostr\" : \"%s\"",spec->data_to_string?"True":"False");
			len += write(",\n\t\t\t\"has_fromstr\" : \"%s\"",spec->string_to_data?"True":"False");
		}
		len += write(",\n\t\t\t\"has_create\" : \"%s\"",spec->create?"True":"False");
		len += write(",\n\t\t\t\"has_stream\" : \"%s\"",spec->stream?"True":"False");
		len += write(",\n\t\t\t\"compareops\" : {");
		for ( op = _TCOP_FIRST ; op < _TCOP_LAST ; op = PROPERTYCOMPAREOP(op+1) )
		{
			if ( spec->compare[op].fn != NULL )
			{
				if ( ! first )
					len += write(",");
				else
					first = false;
				len += write("\n\t\t\t\t\"%s\" : {\"nargs\": \"%d\"}",spec->compare[op].str, spec->compare[op].trinary ? 3 : 2);
			}
		}
		len += write("\n\t\t\t}");
		len += write("\n\t\t}");
	}
	len += write("\n\t}");
	IN_MYCONTEXT output_debug("GldJsonWriter::properties() wrote %d bytes",len);
	return len;
}


int GldJsonWriter::write_classes(FILE *fp,bool noattr)
{
	int len = 0;

	len += write(",\n\t\"header\" : {");
	for ( HEADERDATA *item = object_headerdata_getfirst() ; item != NULL ; item = object_headerdata_getnext(item) )
	{
		if ( item != object_headerdata_getfirst() )
		{
			len += write(",");
		}
		len += write("\n\t\t\"%s\" : {", item->name);
		len += write("\n\t\t\t\"type\" : \"%s\",", item->ptype);
		len += write("\n\t\t\t\"access\" : \"%s\"", item->access);
		len += write("\n\t\t}");
	}
	len += write("\n\t}");

	len += write(",\n\t\"classes\" : {");
	for ( CLASS *oclass = class_get_first_class() ; oclass != NULL ; oclass = oclass->next )
	{
		PROPERTY *prop;
		if ( oclass != class_get_first_class() )
			len += write(",");
		if ( ! noattr || oclass->module == NULL )
		{
			len += write("\n\t\t\"%s\" : {",oclass->name);
		}
		else
		{
			len += write("\n\t\t\"%s.%s\" : {",oclass->module->name,oclass->name);
		}
		if ( ! noattr )
		{
			FIRST("object_size","%u",oclass->size);
			if ( oclass->parent )
			{
				TUPLE("parent","%s",oclass->parent->name );
			}
			TUPLE("trl","%u",oclass->trl);
			if ( oclass->module )
			{
				TUPLE("module","%s",oclass->module->name);
			}
			TUPLE("profiler.numobjs","%u",oclass->profiler.numobjs);
			TUPLE("profiler.clocks","%llu",oclass->profiler.clocks);
			TUPLE("profiler.count","%u",oclass->profiler.count);
			if ( oclass->has_runtime ) TUPLE("runtime","%s",oclass->runtime);
			if ( oclass->pmap != NULL )
				len += write(",");
		}
		for ( prop = oclass->pmap ; prop != NULL && prop->oclass == oclass ; prop=prop->next ) // note: do not output parent classes properties
		{
			KEYWORD *key;
			const char *ptype = class_get_property_typename(prop->ptype);
			if ( ptype == NULL )
				continue;
			if ( prop != oclass->pmap )
				len += write(",");
			len += write("\n\t\t\t\"%s\" : {",prop->name);
			len += write("\n\t\t\t\t\"type\" : \"%s\",",ptype);
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
			len += write("\n\t\t\t\t\"access\" : \"%s\"",access);
			for ( key = prop->keywords ; key != NULL ; key = key->next )
			{
				if ( key == prop->keywords )
				{
					len += write(",\n\t\t\t\t\"keywords\" : {");
				}
				len += write("\n\t\t\t\t\t\"%s\" : \"0x%x\"",key->name,key->value);
				if ( key->next == NULL )
					len += write("\n\t\t\t\t}");
				else
					len += write(",");
			}
			char flags[1024] = "";
			if ( prop->flags&PF_RECALC ) strcat(flags,"RECALC");
			if ( prop->flags&PF_CHARSET ) { if ( flags[0] != '\0' ) strcat(flags,"|"); strcat(flags,"CHARSET"); }
			if ( prop->flags&PF_EXTENDED ) { if ( flags[0] != '\0' ) strcat(flags,"|"); strcat(flags,"EXTENDED"); }
			if ( prop->flags&PF_DEPRECATED ) { if ( flags[0] != '\0' ) strcat(flags,"|"); strcat(flags,"DEPRECATED"); }
			if ( prop->flags&PF_DEPRECATED_NONOTICE ) { if ( flags[0] != '\0' ) strcat(flags,"|"); strcat(flags,"DEPRECATED_NONOTICE"); }
			if ( prop->flags&PF_REQUIRED ) strcat(flags,flags[0]?"|":""),strcat(flags,"REQUIRED");
			if ( prop->flags&PF_OUTPUT ) strcat(flags,flags[0]?"|":""),strcat(flags,"OUTPUT");
			if ( prop->flags&PF_DYNAMIC ) strcat(flags,flags[0]?"|":""),strcat(flags,"DYNAMIC");
			if ( flags[0] != '\0' )
			{
				len += write(",\n\t\t\t\t\"flags\" : \"%s\"",flags);
			}
			if ( prop->unit != NULL )
			{
				len += write(",\n\t\t\t\t\"unit\" : \"%s\"",prop->unit->name);
			}
			if ( prop->default_value != NULL )
			{
				PROPERTYSPEC *spec = property_getspec(prop->ptype);
				if ( spec != NULL && prop->default_value != spec->default_value )
				{	
					len += write(",\n\t\t\t\t\"default\" : \"%s\"",prop->default_value);
				}
			}
			if ( prop->description != NULL )
			{
				len += write(",\n\t\t\t\t\"description\" : \"%s\"",prop->description);
			}
			len += write("\n\t\t\t}");
		}
		len += write("\n\t\t}");
	}
	len += write("\n\t}");
	IN_MYCONTEXT output_debug("GldJsonWriter::classes() wrote %d bytes",len);
	return len;
}

int GldJsonWriter::write_globals(FILE *fp)
{
	int len = 0;
	GLOBALVAR *var;
	len += write(",\n\t\"globals\" : {");

	/* for each module */
	for ( var = global_find(NULL) ; var != NULL ; var = global_getnext(var) )
	{
		char buffer[1024];
		if ( global_getvar(var->prop->name,buffer,sizeof(buffer)-1) ) // only write globals that can be extracted
		{
			KEYWORD *key;
			PROPERTYSPEC *pspec = property_getspec(var->prop->ptype);
			if ( var != global_find(NULL) )
				len += write(",");
			len += write("\n\t\t\"%s\" : {", var->prop->name);
			len += write("\n\t\t\t\"type\" : \"%s\",", pspec->name);
			for ( key = var->prop->keywords ; key != NULL ; key = key->next )
			{
				if ( key == var->prop->keywords )
				{
					len += write("\n\t\t\t\"keywords\" : {");
				}
				len += write("\n\t\t\t\t\"%s\" : \"0x%x\"",key->name,key->value);
				if ( key->next == NULL )
					len += write("\n\t\t\t}");
				len += write(",");
			}
			PROPERTY *prop = var->prop;
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
			char flags[1024] = "";
			if ( prop->flags&PF_EXTENDED ) strcat(flags,"EXTENDED");
			if ( prop->flags&PF_CHARSET ) strcat(flags,flags[0]?"|":""),strcat(flags,"CHARSET");
			if ( prop->flags&PF_DEPRECATED ) strcat(flags,flags[0]?"|":""),strcat(flags,"DEPRECATED");
			if ( prop->flags&PF_DEPRECATED_NONOTICE ) strcat(flags,flags[0]?"|":""),strcat(flags,"DEPRECATED_NONOTICE");
			if ( prop->flags&PF_REQUIRED ) strcat(flags,flags[0]?"|":""),strcat(flags,"REQUIRED");
			if ( prop->flags&PF_OUTPUT ) strcat(flags,flags[0]?"|":""),strcat(flags,"OUTPUT");
			if ( prop->flags&PF_DYNAMIC ) strcat(flags,flags[0]?"|":""),strcat(flags,"DYNAMIC");
			if ( prop->flags ) len += write("\t\t\t\t\"flags\" : \"%s\",",flags);
			if ( var->initial != NULL )
			{
				len += write("\n\t\t\t\"initial\" : \"%s\",", escape(var->initial));
			}
			len += write("\n\t\t\t\"access\" : \"%s\",",access);		
			if ( buffer[0] == '\"' )
				len += write("\n\t\t\t\"value\" : \"%s\"", escape(buffer+1,strlen(buffer)-2));
			else
				len += write("\n\t\t\t\"value\" : \"%s\"", escape(buffer));
			len += write("\n\t\t}");
		}
	}
	len += write("\n\t}");
	IN_MYCONTEXT output_debug("GldJsonWriter::globals() wrote %d bytes",len);
	return len;
}

static const char *escape(const char *text, char *buffer, size_t len)
{
	char *p = buffer;
	while ( *text != '\0' && p < buffer+len )
	{
		switch ( *text ) {
		case '\n':
			*p++ = '\\';
			*p++ = 'n';
			text++;
			break;
		case '\t':
			*p++ = '\\';
			*p++ = 't';
			text++;
			break;
		case '\r':
			*p++ = '\\';
			*p++ = 'r';
			text++;
			break;
		case '"':
			*p++ = '\\';
			*p++ = '"';
			text++;
			break;
		default:
			*p++ = *text++;
			break;
		}
	}
	*p = '\0';
	return buffer;
}
int GldJsonWriter::write_schedules(FILE *fp) 
{
	int len = 0;
	SCHEDULE *sch;
	len += write(",\n\t\"schedules\" : {");

	/* for each module */
	bool first = true;
	for ( sch = schedule_getfirst() ; sch != NULL ; sch = schedule_getnext(sch) )
	{
		if ( sch->flags&SN_USERDEFINED )
		{
			if ( ! first )
				len += write(",");
			first = false;
			char buffer[sizeof(sch->definition)*2];
			len += write("\n\t\t\"%s\" : \"%s\"", sch->name, escape(sch->definition,buffer,sizeof(buffer)));
		}
	}
	len += write("\n\t}");
	IN_MYCONTEXT output_debug("GldJsonWriter::schedules() wrote %d bytes",len);
	return len;
}

int GldJsonWriter::write_objects(FILE *fp)
{
	int len = 0;
	OBJECT *obj;
	len += write(",\n\t\"objects\" : {");

	/* scan each object in the model */
	size_t buffer_size = 5000;
	char *buffer = (char*)malloc(buffer_size);
	for ( obj = object_get_first() ; obj != NULL ; obj = obj->next )
	{
		PROPERTY *prop;
		if ( obj != object_get_first() )
			len += write(",");
		if ( obj->oclass == NULL ) // ignore objects with no defined class
			continue;
		if ( obj->name ) 
			len += write("\n\t\t\"%s\" : {",obj->name);
		else
			len += write("\n\t\t\"%s:%d\" : {", obj->oclass->name, obj->id);
		FIRST("id","%d",obj->id);
		TUPLE("class","%s",obj->oclass->name);

		/* handle special case for powerflow module handling of parent */
		OBJECT **topological_parent = object_get_object_by_name(obj,"topological_parent");
		if ( topological_parent != NULL )
			obj->parent = *topological_parent;

		if ( obj->parent != NULL )
		{
			if ( obj->parent->name == NULL )
				len += write(",\n\t\t\t\"parent\" : \"%s:%d\"",obj->parent->oclass->name,obj->parent->id);
			else
				TUPLE("parent","%s",obj->parent->name);
		}
		if ( ! isnan(obj->latitude) ) TUPLE("latitude","%f",obj->latitude);
		if ( ! isnan(obj->longitude) ) TUPLE("longitude","%f",obj->longitude);
		if ( obj->groupid[0] != '\0' ) TUPLE("groupid","%s",(const char*)obj->groupid);
		TUPLE("rank","%u",(unsigned int)obj->rank);
		if ( convert_from_timestamp(obj->clock,buffer,buffer_size) )
			TUPLE("clock","%s",buffer);
		if ( obj->valid_to > TS_ZERO && obj->valid_to < TS_NEVER ) TUPLE("valid_to","%llu",(int64)(obj->valid_to));
		if ( obj->schedule_skew != 0 ) TUPLE("schedule_skew","%lld",obj->schedule_skew);
		if ( obj->in_svc > TS_ZERO && obj->in_svc < TS_NEVER ) TUPLE("in","%llu",(int64)(obj->in_svc));
		if ( obj->out_svc > TS_ZERO && obj->out_svc < TS_NEVER ) TUPLE("out","%llu",(int64)(obj->out_svc));
		TUPLE("rng_state","%llu",(int64)(obj->rng_state));
		if ( obj->heartbeat != 0 ) TUPLE("heartbeat","%llu",(int64)(obj->heartbeat));
		(len += write(",\n\t\t\t\"%s\" : \"%llX%llX\"","guid",(int64)(obj->guid[0]),(int64)(obj->guid[1])));
		TUPLE("flags","0x%llx",(int64)(obj->flags));
		for ( prop = object_get_first_property(obj) ; prop != NULL ; prop = object_get_next_property(prop) )
		{
            size_t sz = object_property_getsize(obj,prop)+1;
            if ( buffer_size < sz )
            {
            	buffer = (char*)realloc(buffer,sz);
            	buffer_size = sz;
            }
            sz = buffer_size;
        	const char *value = NULL;

        	// prepare output value
			if ( prop->access != PA_PUBLIC )
			{
				continue; // ignore private values
			}
        	else if ( (global_filesave_options&FSO_INITIAL) == FSO_INITIAL )
        	{
        		// initialization value is desired
        		value = object_property_to_initial(obj,prop->name, buffer, buffer_size);
        	}
			else if ( prop->ptype == PT_enduse )
			{
				// ignore enduse values
				continue;
			}
			else if ( prop->ptype == PT_method )
            {
            	// special handling required for methods
                if ( sz > 0 )
                {
                    strcpy(buffer,"");
                    object_property_to_string(obj,prop->name,buffer,sz);
					len += write(",\n\t\t\t\"%s\": \"%s\"", prop->name, escape(buffer));
                }
                else if ( sz == 0 )
                {
					len += write(",\n\t\t\t\"%s\": \"\"", prop->name);
                }
                else
                {
                	// no output allowed for this property
                }
            }
            else if ( prop->ptype == PT_double )
            {
            	double *x = object_get_double_quick(obj,prop);
            	if ( prop->unit )
					len += write(",\n\t\t\t\"%s\": \"%g %s\"", prop->name, *x, prop->unit->name);
				else
					len += write(",\n\t\t\t\"%s\": \"%g\"", prop->name, *x);
            }
            else if ( prop->ptype == PT_complex )
            {
				complex *c = object_get_complex_quick(obj,prop);
				const char *xs="real", *ys="imag", *nt="j";
				double x = c->Re(), y = c->Im();
				if ( global_json_complex_format&JCF_DEGREES )
				{
					x = c->Mag(); xs = "mag";
					y = c->Ang(); ys = "ang";
					nt = "d";
				}
				else if ( global_json_complex_format&JCF_RADIANS )
				{
					x = c->Mag(); xs = "mag";
					y = c->Arg(); ys = "arg";
					nt = "r";
				}
            	if ( global_json_complex_format&JCF_LIST )
            	{
					if ( prop->unit )
						len += write(",\n\t\t\t\"%s\": [%g,%g,\"%s\"]", prop->name, x, y, prop->unit->name);
					else
						len += write(",\n\t\t\t\"%s\": [%g,%g]", prop->name, x, y);
				}
				else if ( global_json_complex_format&JCF_DICT )
				{
					if ( prop->unit )
						len += write(",\n\t\t\t\"%s\": {\"%s\":%g,\"%s\":%g,\"unit\":\"%s\"}", prop->name, xs, x, ys, y, prop->unit->name);
					else
						len += write(",\n\t\t\t\"%s\": {\"%s\":%g,\"%s\":%g}", prop->name, xs, x, ys, y);
				}
				else
				{
					if ( (global_json_complex_format&0x03) != 0 )
					{
						output_warning("global_json_complex_format=%d is not valid, using STRING=0 instead", global_json_complex_format);
					}
					if ( prop->unit )
						len += write(",\n\t\t\t\"%s\": \"%g%+g%s %s\"", prop->name, x, y, nt, prop->unit->name);
					else
						len += write(",\n\t\t\t\"%s\": \"%g%+g%s\"", prop->name, x, y, nt);
            	}
            }
            else
            {
				value = object_property_to_string(obj, prop->name, buffer, buffer_size);
			}

			// process output value
			if ( value == NULL )
			{
				continue; // ignore values that don't convert propertly
			}
			else if ( (global_filesave_options&FSO_MINIMAL) == FSO_MINIMAL 
					&& prop->default_value != NULL 
					&& strcmp(value,prop->default_value) == 0 )
			{
				continue; // ignore values that are precisely the default value
			}
			else
			{
				int size = strlen(value);
				// TODO: proper JSON formatted is needed for data that is either a dict or a list
				// if ( value[0] == '{' && value[len] == '}')
				// 	len += write(",\n\t\t\t\"%s\" : %s", prop->name, value);
				// else if ( value[0] == '[' && value[len] == ']')
				// 	len += write(",\n\t\t\t\"%s\" : %s", prop->name, value);
				// else 
				if ( value[0] == '"' && value[size-1] == '"')
				{
					len += write(",\n\t\t\t\"%s\": \"%s\"", prop->name, escape(value+1,size-2));
				}
				else
				{
					len += write(",\n\t\t\t\"%s\": \"%s\"", prop->name, escape(value,size));
				}
			}
		}
		len += write("\n\t\t}");
	}
	if ( buffer ) free(buffer);

	len += write("\n\t}");
	IN_MYCONTEXT output_debug("GldJsonWriter::objects() wrote %d bytes",len);
	return len;
}

int GldJsonWriter::write_filters(FILE *fp)
{
	int len = 0;
	TRANSFERFUNCTION *tf = transfer_function_getfirst();
	if ( tf )
	{
		len += write(",\n\t\"filters\" : {\n\t\t");
	}
	for ( tf = transfer_function_getfirst() ; tf != NULL ; tf = tf->next )
	{
		char buffer[1024];
		if ( transfer_function_to_json(buffer,sizeof(buffer),tf) > 0 )
		{
			if ( tf != transfer_function_getfirst() )
			{
				write(",\n\t\t");
			}
			write(buffer);
		}
	}
	if ( len > 0 )
	{
		len += write("\n\t}");
	}
	return len;
}

// return number of bytes written
int GldJsonWriter::write_output(FILE *fp)
{
	int len = 0;
	json = fp;
	len += write("{\t\"application\": \"gridlabd\",\n");
	len += write("\t\"version\" : \"%u.%u.%u\",\n",global_version_major,global_version_minor,global_version_patch);
	len += write("\t\"branch\": \"%s\",\n",BRANCH);
	len += write("\t\"build\": \"%06d\"",BUILDNUM);
	if ( (global_filesave_options&FSO_MODULES) == FSO_MODULES )
	{
		len += write_modules(fp);
	}
	if ( (global_filesave_options&FSO_PROPERTIES) == FSO_PROPERTIES )
	{
		len += write_properties(fp);
	}
	if ( (global_filesave_options&FSO_CLASSES) == FSO_CLASSES )
	{
		len += write_classes(fp);
	}
	if ( (global_filesave_options&FSO_GLOBALS) == FSO_GLOBALS )
	{
		len += write_globals(fp);
	}
	if ( (global_filesave_options&FSO_SCHEDULES) == FSO_SCHEDULES )
	{
		len += write_schedules(fp);
	}
	len += write_filters(fp);
	if ( (global_filesave_options&FSO_OBJECTS) == FSO_OBJECTS )
	{
		len += write_objects(fp);
	}
	len += write("\n}\n");
	IN_MYCONTEXT output_debug("GldJsonWriter::output() wrote %d bytes",len);
	return len;
}

// returns 0 on success, non-zero on failure
int GldJsonWriter::dump()
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
		throw_exception("GldJsonWriter::dump(const char *filename='%s'): %s", filename, strerror(errno));
		/* TROUBLESHOOT
			The system was unable to output the json data to the specified file.  
			Follow the recommended solution based on the error message provided and try again.
		 */

	/* output data */
	int len = write_output(fp);

	/* close file */
	fclose(fp);

	return len > 0;
}

int GldJsonWriter::dump_modules()
{
	int len = 0;
	json = stdout;
	len += write("{\t\"application\": \"gridlabd\",\n");
	len += write("\t\"version\" : \"%u.%u.%u\",\n",global_version_major,global_version_minor,global_version_patch);
	len += write("\t\"branch\": \"%s\",\n",BRANCH);
	len += write("\t\"build\": \"%06d\"",BUILDNUM);
	len += write_modules(json);
	len += write_classes(json,true);
	len += write("\n}\n");
	return len;
}

int json_to_glm(const char *jsonfile, char *glmfile)
{
	// TODO: convert JSON file to GLM
	return 0;
}
