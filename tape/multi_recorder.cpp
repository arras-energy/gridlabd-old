/** $Id: multi_recorder.c 4738 2014-07-03 00:55:39Z dchassin $
	Copyright (C) 2008 Battelle Memorial Institute
	@file tape.c
	@addtogroup recorder Recorders
	@ingroup tapes

	Tape recorders use the following additional properties
	- \p format specifies whether to use raw timestamp instead of date-time format
	- \p interval specifies the sampling interval to use (0 means every pass, -1 means only transients)
	- \p limit specifies the maximum length limit for the number of samples taken
	- \p trigger specifies a trigger condition on a property to start recording
	the condition is specified in the format \p property \p comparison \p value
	- \p The \p loop property is not available in recording.
 @{
 **/

 /*
  *	The idea behind the multi-recorder is that it is easier to reuse the recorder structure with logic
  *	that anticipates the use of a different mechanism for determining the object to associate with each
  *	property listed for the recorder.
  *	The differences assumed by the multi-recorder are that the parent is not used to determine the object
  *	that each property is associated with, or that the recorder is associated with, that the property
  *	list will include the name of the object that the property will correspond to (and corresponds with
  *	the parent object if no object is parsed from the string), and that the parent property may be
  * null.  The multi-recorder will also use the RECORDER_MAP struct in order to store the target
  *	properties, rather than the PROPERTY struct.
  *		-Matt Hauer <d3p988>
  */
 
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include "gridlabd.h"
#include "object.h"
#include "aggregate.h"

#include "tape.h"
#include "file.h"
#include "odbc.h"

#if defined WIN32 && defined  __MINGW32__
inline char* strtok_t(char *str, const char *delim, char **nextp)
{
    char *ret;

    if (str == NULL)
    {
        str = *nextp;
    }

    str += strspn(str, delim);

    if (*str == '\0')
    {
        return NULL;
    }

    ret = str;

    str += strcspn(str, delim);

    if (*str)
    {
        *str++ = '\0';
    }

    *nextp = str;

    return ret;
} 
#define strtok_s strtok_t
#endif

CLASS *multi_recorder_class = NULL;
static OBJECT *last_recorder = NULL;

EXPORT int create_multi_recorder(OBJECT **obj, OBJECT *parent)
{
	*obj = gl_create_object(multi_recorder_class);
	if (*obj!=NULL)
	{
		struct recorder *my = OBJECTDATA(*obj,struct recorder);
		last_recorder = *obj;
		gl_set_parent(*obj,parent);
		strcpy(my->file,"");
		strcpy(my->multifile,"");
		strcpy(my->filetype,"txt");
		strcpy(my->delim,",");
		strcpy(my->mode, "file");
		my->interval = -1; /* transients only */
		my->dInterval = -1.0;
		my->last.ts = -1;
		strcpy(my->last.value,"");
		my->limit = 0;
		my->samples = 0;
		my->status = TS_INIT;
		my->trigger[0]='\0';
		my->format = 0;
		strcpy(my->plotcommands,"");
		my->target = NULL;
		my->header_units = HU_DEFAULT;
		my->line_units = LU_DEFAULT;
		my->property = NULL;
		my->property_len = 0;
		return 1;
	}
	return 0;
}

static int multi_recorder_open(OBJECT *obj)
{
	char32 type="file";
	char1024 fname="";
	char32 flags="w";
	struct recorder *my = OBJECTDATA(obj,struct recorder);
	TAPEFUNCS *tf = 0;

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

		/* use object name-id as default file name */
		sprintf(fname,"%s-%d.%s",obj->parent->oclass->name,obj->parent->id, (char*)(my->filetype));

	/* open multiple-run input file & temp output file */
	if(my->type == FT_FILE && my->multifile[0] != 0){
		if(my->interval < 1){
			gl_error("multirecorder: transient recorders cannot use multi-run output files");
			return 0;
		}
		sprintf(my->multitempfile, "temp_%s", (char*)(my->file));
		my->multifp = fopen(my->multitempfile, "w");
		if(my->multifp == NULL){
			gl_error("multirecorder: unable to open \'%s\' for multi-run output", (char*)(my->multitempfile));
		} else {
			time_t now=time(NULL);

			my->inputfp = fopen(my->multifile, "r");

			// write header into temp file
			fprintf(my->multifp,"# file...... %s\n", (char*)(my->file));
			fprintf(my->multifp,"# date...... %s", asctime(localtime(&now)));
#ifdef WIN32
			fprintf(my->multifp,"# user...... %s\n", getenv("USERNAME"));
			fprintf(my->multifp,"# host...... %s\n", getenv("MACHINENAME"));
#else
			fprintf(my->multifp,"# user...... %s\n", getenv("USER"));
			fprintf(my->multifp,"# host...... %s\n", getenv("HOST"));
#endif
			if(obj->parent){
				fprintf(my->multifp,"# target.... %s %d\n", obj->parent->oclass->name, obj->parent->id);
			} else {
				fprintf(my->multifp,"# target.... (none)\n");
			}
			fprintf(my->multifp,"# trigger... %s\n", my->trigger[0]=='\0'?"(none)":(char*)(my->trigger));
			fprintf(my->multifp,"# interval.. %lld\n", my->interval);
			fprintf(my->multifp,"# limit..... %d\n", my->limit);
			fprintf(my->multifp,"# property.. %s\n", my->property);
			//fprintf(my->multifp,"# timestamp,%s\n", my->property);
		}
		if(my->inputfp != NULL){
			char1024 inbuffer;
			char *data;
			int get_col = 0;
			do{
				if(0 != fgets(inbuffer, 1024, my->inputfp)){
					char *end = strchr(inbuffer, '\n');
					data = inbuffer+strlen("# file...... ");
					if(end != 0){
						*end = 0; // trim the trailing newline
					}
					//					   "# columns... "
					if(strncmp(inbuffer, "# file", strlen("# file")) == 0){
						; // ignore
					} else if(strncmp(inbuffer, "# date", strlen("# date")) == 0){
						; // ignore
					} else if(strncmp(inbuffer, "# user", strlen("# user")) == 0){
						; // ignore
					} else if(strncmp(inbuffer, "# host", strlen("# host")) == 0){
						; // ignore
					} else if(strncmp(inbuffer, "# target", strlen("# target")) == 0){
						// verify same target
						char256 target;
						if(obj->parent != 0){
							sprintf(target, "%s %d", obj->parent->oclass->name, obj->parent->id);
							if(0 != strncmp(target, data, strlen(data))){
								gl_error("multirecorder:%i: re-recording target mismatch: was %s, now %s", obj->id, data, (char*)target);
							}
						} else {
							if(strncmp("(none)", target, 6)){
								gl_error("multirecorder:%i: re-recording target mismatch: was %s, now blank", obj->id, data);
							}
						}
					} else if(strncmp(inbuffer, "# trigger", strlen("# trigger")) == 0){
						// verify same trigger, or absence thereof
						;
					} else if(strncmp(inbuffer, "# interval", strlen("# interval")) == 0){
						// verify same interval
						int interval = atoi(data);
						if(interval != my->interval){
							gl_error("multirecorder:%i: re-recording interval mismatch: was %i, now %i", obj->id, interval, my->interval);
						}
					} else if(strncmp(inbuffer, "# limit", strlen("# limit")) == 0){
						// verify same limit
						int limit = atoi(data);
						if(limit != my->limit){
							gl_error("multirecorder:%i: re-recording limit mismatch: was %i, now %i", obj->id, limit, my->limit);
						}
					} else if(strncmp(inbuffer, "# property", strlen("# property")) == 0){
						// verify same columns
						if(0 != strncmp(my->property, data, strlen(my->property))){
							gl_error("multirecorder:%i: re-recording property mismatch: was %s, now %s", obj->id, data, my->property);
						}
						// next line is full header column list
						get_col = 1;
					}
				} else {
					gl_error("multirecorder: error reading multi-read input file \'%s\'", (char*)(my->multifile));
					break;
				}
			} while(inbuffer[0] == '#' && get_col == 0);
			// get full column list
			if(0 != fgets(inbuffer, 1024, my->inputfp)){
				int rep=0;
				int replen = (int)strlen("# repetition");
				int len, lenmax = 1024, i = 0;
				char1024 propstr, shortstr;
				PROPERTY *tprop = my->target;
				gl_verbose("read last buffer");
				if(strncmp(inbuffer, "# repetition", replen) == 0){
					char *trim;
					rep = atoi(inbuffer + replen + 1); // skip intermediate space
					++rep;
					fprintf(my->multifp, "# repetition %i\n", rep);
					fgets(inbuffer, 1024, my->inputfp);
					trim = strchr(inbuffer, '\n');
					if(trim) *trim = 0;
				} else { // invalid input file or somesuch, we could error ... or we can trample onwards with our output file.
					rep = 0;
					fprintf(my->multifp, "# repetition %i\n", rep);
				}
				// following block matches below
				while(tprop != NULL){
					sprintf(shortstr, ",%s(%i)", tprop->name, rep);
					len = (int)strlen(shortstr);
					if(len > lenmax){
						gl_error("multirecorder: multi-run recorder output full property list is larger than the buffer, please start a new file!");
						break; // will still print everything up to this one
					}
					strncpy(propstr+i, shortstr, len+1);
					i += len;
					tprop = tprop->next;
				}
				fprintf(my->multifp, "%s%s\n", (char*)inbuffer, (char*)propstr);
			}
		} else { /* new file, so write repetition & properties with (0) */
			char1024 propstr, shortstr;
/*							*
 *		UPDATE THIS PART	*
 *							*/
			int len, lenmax = 1024, i = 0;
			PROPERTY *tprop = my->target;
			fprintf(my->multifp, "# repetition 0\n");
			// no string from previous runs to append new props to
			sprintf(propstr, "# timestamp");
			len = (int)strlen(propstr);
			lenmax-=len;
			i = len;
			// following block matches above
			while(tprop != NULL){
				sprintf(shortstr, ",%s(0)", tprop->name);
				len = (int)strlen(shortstr);
				if(len > lenmax){
					gl_error("multirecorder: multi-run recorder output full property list is larger than the buffer, please start a new file!");
					break; // will still print everything up to this one
				}
				strncpy(propstr+i, shortstr, len+1);
				i += len;
				tprop = tprop->next;
			}
			fprintf(my->multifp, "%s\n", (char*)propstr);
/*							*
 *		End update part		*
 *							*/
		}
	}

	/* if type is file or file is stdin */
	tf = get_ftable(type);
	if(tf == NULL)
		return 0;
	my->ops = tf->recorder;
	if(my->ops == NULL)
		return 0;
	set_csv_options();

	// set out_property here
	{size_t offset = 0;
		char unit_buffer[1024];
		char *token = 0, *prop_ptr = 0, *unit_ptr = 0;
		char objstr[1024], bigpropstr[1024], propstr[1024], unitstr[64];
		PROPERTY *prop = 0;
		UNIT *unit = 0;
		int first = 1;
		OBJECT *myobj = 0;
		char *last;
		switch(my->header_units){
			case HU_DEFAULT:
				strcpy(my->out_property, my->property);
				break;
			case HU_ALL:
				strcpy(unit_buffer, my->property);
				for(token = strtok_r(unit_buffer, ",", &last); token != NULL; token = strtok_r(NULL, ",",&last)){
					unit = 0;
					prop = 0;
					unitstr[0] = 0;
					propstr[0] = 0;
					objstr[0] = 0;
					prop_ptr = strchr(token, ':');
					if(prop_ptr != 0){
						unit_ptr = strchr(prop_ptr, '[');
					} else {
						unit_ptr = strchr(token, '[');
					}
					// detect if this points at a different object and split accordingly
					if(prop_ptr == 0){
						prop_ptr = token;
						myobj = obj->parent;
						strcpy(bigpropstr, token);
					} else {
						sscanf(token, "%[^:]:%[^\n\r\t;]", objstr, bigpropstr);
						myobj = gl_get_object(objstr);
						if(myobj == 0){
							gl_error("multi_recorder:%d: unable to find object '%s'", obj->id, objstr);
							return 0;
						}
					}
					// split unit from property, if present
					if(unit_ptr == 0){
						// no explicit unit
						prop = gl_get_property(myobj, bigpropstr,NULL);
						if(prop == 0){
							gl_error("multi_recorder:%d: unable to find property '%s' for object '%s'", obj->id, propstr, myobj->name);
							return 0;
						}

						if(prop->ptype == PT_double){
							strcpy(unitstr, prop->unit->name);
						} else {
							; // not double, no unit, nothing to do
						}
						strcpy(propstr, bigpropstr);
					} else {
						// has explicit unit
						if(2 == sscanf(bigpropstr, "%[A-Za-z0-9_.][%[^]\n,]]", propstr, unitstr)){
							unit = gl_find_unit(unitstr);
							if(unit == 0){
								gl_error("multi_recorder:%d: unable to find unit '%s' for property '%s'", obj->id, unitstr, propstr);
								return 0;
							}
							prop = gl_get_property(myobj, propstr,NULL);
							if(prop == 0){
								gl_error("multi_recorder:%d: unable to find property '%s' for object '%s'", obj->id, propstr, myobj->name);
								return 0;
							}
						} else {
							gl_error("oops");
						}
					}
					// check if property exists in object
					// find property

/*					// breakpoint
					if(3 == sscanf(token, "%[A-Za-z0-9_.][%[^]\n,\0]:%[A-Za-z0-9_.][%[^]\n,\0]", objstr, propstr, unitstr)){
						myobj = gl_get_object(objstr);
					} else if(2 == sscanf(token, "%[A-Za-z0-9_.][%[^]\n,\0]", propstr, unitstr)){
						unit = gl_find_unit(unitstr);
						if(unit == 0){
							gl_error("multi_recorder:%d: unable to find unit '%s' for property '%s'", obj->id, unitstr, propstr);
							return 0;
						}
					}
					prop = gl_get_property(obj->parent, propstr);
					if(prop->unit != 0 && unit == 0){
						unit = prop->unit;
					}*/
					// print the property, and if there is one, the unit
					if(myobj != obj->parent){
						// need to include target object name in string
						if(unit != 0){
							sprintf(my->out_property+offset, "%s%s:%s[%s]", (first ? "" : ","), myobj->name, propstr, (unitstr[0] ? unitstr : unit->name));
							offset += strlen(propstr) + (first ? 0 : 1) + 2 + strlen(unitstr[0] ? unitstr : unit->name) + strlen(myobj->name) + 1;
						} else {
							sprintf(my->out_property+offset, "%s%s:%s", (first ? "" : ","), myobj->name, propstr);
							offset += strlen(propstr) + (first ? 0 : 1 + strlen(myobj->name) + 1);
						}
					} else {
						// parent object, so no explicit object name
						if(unit != 0){
							sprintf(my->out_property+offset, "%s%s[%s]", (first ? "" : ","), propstr, (unitstr[0] ? unitstr : unit->name));
							offset += strlen(propstr) + (first ? 0 : 1) + 2 + strlen(unitstr[0] ? unitstr : unit->name);
						} else {
							sprintf(my->out_property+offset, "%s%s", (first ? "" : ","), propstr);
							offset += strlen(propstr) + (first ? 0 : 1);
						}
					}
					first = 0;
				}
				break;
			case HU_NONE:
				strcpy(unit_buffer, my->property);
				for(token = strtok_r(unit_buffer, ",",&last); token != NULL; token = strtok_r(NULL, ",",&last)){
					if(2 == sscanf(token, "%[A-Za-z0-9_:.][%[^]\n,]]", propstr, unitstr)){
						; // no logic change
					}
					// print just the property, regardless of type or explicitly declared property
					sprintf(my->out_property+offset, "%s%s", (first ? "" : ","), propstr);
					offset += strlen(propstr) + (first ? 0 : 1);
					first = 0;
				}
				break;
			default:
				// error
				break;
		}
	}
	return my->ops->open(my, fname, flags);
}

static int write_multi_recorder(struct recorder *my, char *ts, char *value)
{
	return my->ops->write(my, ts, value);
}

static void close_multi_recorder(struct recorder *my)
{
	if (my->ops){
		my->ops->close(my);
	}
	if(my->multifp){
		if(0 != fclose(my->multifp)){
			gl_error("multirecorder: unable to close multi-run temp file \'%s\'", (char*)(my->multitempfile));
			perror("fclose(): ");
		}

		my->multifp = 0; // since it's closed

		if(my->inputfp != NULL){
			fclose(my->inputfp);
			if(0 != remove(my->multifile)){ // old file
				gl_error("multirecorder: unable to remove out-of-data multi-run file \'%s\'", (char*)(my->multifile));
				perror("remove(): ");
			}
		}
		if(0 != rename(my->multitempfile, my->multifile)){
			gl_error("multirecorder: unable to rename multi-run file \'%s\' to \'%s\'", (char*)(my->multitempfile), (char*)(my->multifile));
			perror("rename(): ");
		}
		
	}
}

static TIMESTAMP multi_recorder_write(OBJECT *obj)
{
	struct recorder *my = OBJECTDATA(obj,struct recorder);
	char ts[64]="0"; /* 0 = INIT */
	if (my->format==0)
	{
		if (my->last.ts>TS_ZERO)
		{
			DATETIME dt;
			gl_localtime(my->last.ts,&dt);
			gl_strtime(&dt,ts,sizeof(ts));
		}
		/* else leave INIT in the buffer */
	}
	else
		sprintf(ts,"%" FMT_INT64 "d", my->last.ts);
	if ((my->limit>0 && my->samples > my->limit) /* limit reached */
		|| write_multi_recorder(my, ts, my->last.value)==0) /* write failed */
	{
		close_multi_recorder(my);
		my->status = TS_DONE;
	}
	else
		my->samples++;

	/* at this point we've written the sample to the normal recorder output */
	if ( my->flush==0 || ( my->flush > 0 && gl_globalclock % my->flush == 0 ) )
	{
		my->ops->flush(my);
	}

	// if file based
	if(my->multifp != NULL){
		char1024 inbuffer;
		char1024 outbuffer;
		char *lasts = 0;
		char *in_ts = 0;
		char *in_tok = 0;

		memset(inbuffer, 0, sizeof(inbuffer));
		
		if(my->inputfp != NULL){
			// read line
			do{
				if(0 == fgets(inbuffer, 1024, my->inputfp)){
					if(feof(my->inputfp)){
						// if there is no more data to append rows to, we're done with the aggregate 
						//fclose(my->multifp); // happens in close()
						//fclose(my->inputfp); // one-over read never happens
						return TS_NEVER;
					} else {
						gl_error("multirecorder: error reading past recordings file");
						my->status = TS_ERROR;
						return TS_NEVER;
					}
				}
				// if first char == '#', re-read
			} while(inbuffer[0] == '#');
			
			// NOTE: this is not thread safe!
			// split on first comma
			in_ts = strtok_s(inbuffer, ",\n", &lasts);
			in_tok = strtok_s(NULL, "\n", &lasts);

			if(in_ts == NULL){
				gl_error("multirecorder: unable to indentify a timestamp within the line read from ");
			}

			// compare timestamps if my->format == 0
				//	warn if timestamps mismatch
			if(strcmp(in_ts, ts) != 0){
				gl_warning("multirecorder: timestamp mismatch between current input line and simulation time");
			}
			sprintf(outbuffer, "%s,%s", in_tok, (char*)(my->last.value));
		} else { // no input file ~ write normal output
			strcpy(outbuffer, my->last.value);
		}
		// fprintf 
		fprintf(my->multifp, "%s,%s\n", ts, (char*)outbuffer);
	}
	return TS_NEVER;
}

#define BLOCKSIZE 1024
EXPORT int method_multi_recorder_property(OBJECT *obj, char *value, size_t size)
{
	struct recorder *my = OBJECTDATA(obj,struct recorder);
	if ( value == NULL ) // size query
	{
		if ( size == 0 )
			return my->property?strlen(my->property)+1:0;
		else
			return (my->property?strlen(my->property)+1:0) < size;
	}
	else if ( size == 0 ) // incoming value
	{
		size_t len = strlen(value) + (my->property==NULL ? 0 : strlen(my->property));
		gl_verbose("adding property '%s' to multi_recorder:%d",value,obj->id);
		if ( my->property_len < len )
		{
			my->property_len = ((my->property_len+len+1)/BLOCKSIZE+1)*BLOCKSIZE;
			char *old_value = my->property;
			my->property = (char*)realloc(my->property,my->property_len);
			if ( my->property == NULL )
			{
				gl_error("memory allocation failed");
				my->property_len = 0;
				return 0;
			}
			if ( old_value )
			{
				strcat(my->property,",");
				strcat(my->property,value);
			}
			else
			{
				strcpy(my->property,value);
			}
		}	
		else
		{
			strcat(my->property,",");
			strcat(my->property,value);
		}
		return 1;
	}
	else 
	{ 
		// outgoing value
		size_t len = ( my->property ? strlen(my->property) : 0) ;
		if ( size < len+1 ) // not enough room for the full contents
		{
			return -1;
		}
		strcpy(value,my->property ? my->property : "");
		return len;
	}
}

RECORDER_MAP *link_multi_properties(OBJECT *obj, char *property_list)
{
	char *item, *lastitem;
	RECORDER_MAP *first=NULL, *last=NULL, *rmap;
	char *list = strdup(property_list);
	complex oblig;
	char objname[128];
	gl_name_object(obj, objname, 128);

	for ( item = strtok_r(list,", \n", &lastitem); item != NULL; item = strtok_r(NULL,", \n",&lastitem))
	{
		char objstr[128] = "";
		char propstr[128] = "";
		char *cpart = 0;
		int64 cid = -1;
		PROPERTY *target_prop = NULL;
		OBJECT *target_obj = NULL;
		UNIT *unit = NULL;
		char256 pstr="", ustr="";
		double scale = 1.0;

		rmap = (RECORDER_MAP *)malloc(sizeof(RECORDER_MAP));
		if ( rmap == NULL )
		{
			gl_error("%s -- memory allocation failed", objname);
			free(list);
			return NULL;
		}	
		memset(rmap, 0, sizeof(RECORDER_MAP));

		if ( strlen(item)>0 && strstr(item,"::") != NULL )
		{
			char *name = strncmp(item,"::",2)==0 ? item+2 : item;
 			GLOBALVAR *var = gl_global_find(name);
 			if ( var == NULL )
 			{
				gl_error("%s: unable to find global '%s'", objname, objstr);
				continue;
 			}
 			target_obj = NULL;
 			target_prop = var->prop; 			
		}
		else
		{
			switch ( sscanf(item, " %127[^:]:%127s", objstr, propstr) ) {
			case 2:	
				if ( strcmp(objstr,"") == 0 )
				{
					target_obj = obj->parent;
				}
				target_obj = gl_get_object(objstr);
				if ( target_obj == NULL )
				{
					gl_error("%s: unable to find object '%s'", objname, strcmp(objstr,"")==0 ? "(parent)" : objstr);
					continue;
				}
				break;
			case 1: 	// only the one part (implies parent)
				if ( obj->parent == NULL )
				{
					gl_error("%s: object has no parent for property '%s'", objname, objstr);
					continue;
				}
				target_obj = obj->parent;
				strcpy(propstr,objstr);
				break;
			default:
				gl_error("%s: property '%s' is not valid", objname, item);
				free(list);
				return NULL;
			}
			gl_debug("target object = '%s:%d'", target_obj->oclass->name, target_obj->id);

			// everything that looks like a property name, then read units up to ]
			if ( sscanf(propstr,"%255[A-Za-z0-9_.][%255[^]\n,]]", (char*)pstr, (char*)ustr) == 2 )
			{
				unit = gl_find_unit(ustr);
				if ( unit == NULL )
				{
					gl_error("%s: unable to find unit '%s' for property '%s' in object '%s %i'", objname, (char*)ustr,(char*)pstr,target_obj->oclass->name, target_obj->id);
					continue;
				}
			}
			
			/* branch: test to see if we're trying to split up a complex property */
			/* must occur w/ *cpart=0 before gl_get_property in order to properly reformat the property name string */
			cpart = strchr(pstr, '.');
			if ( cpart != NULL ) 
			{
				if ( strcmp("imag", cpart+1) == 0 ) 
				{
					cid = (int)((int64)&(oblig.Im()) - (int64)&oblig);
					*cpart = 0;
				} 
				else if ( strcmp("real", cpart+1) == 0 )
				{
					cid = (int)((int64)&(oblig.Re()) - (int64)&oblig);
					*cpart = 0;
				} 
			}

			target_prop = gl_get_property(target_obj,pstr,NULL);
		}	

		if ( target_prop != NULL )
		{
			if ( unit != NULL && target_prop->unit == NULL )
			{
				gl_error("%s: property '%s' is unitless, ignoring unit conversion", objname, item);
			}
			else if ( unit != NULL && gl_convert_ex(target_prop->unit, unit, &scale) == 0 )
			{
				gl_error("%s: unable to convert property '%s' units to '%s'", objname, item, (char*)ustr);
				continue;
			}
			if ( first == NULL )
			{
				first = rmap;
			} 
			else 
			{
				last->next=rmap;
			}
			last = rmap;
			rmap->obj = target_obj;
			memcpy(&(rmap->prop), target_prop, sizeof(PROPERTY));
			rmap->prop.next = NULL; // unlink from main property list
			rmap->prop.unit = unit;
			rmap->scale = scale;
			rmap->next = NULL;
			if ( cid >= 0 ) 
			{ 	/* doing the complex part thing */
				rmap->prop.ptype = PT_double;
				(rmap->prop.addr) = (PROPERTYADDR)((int64)(rmap->prop.addr) + cid);
			}
			gl_debug("%s: linked %s ok", objname, item);
		}
		else
		{
			if ( target_obj )
			{
				gl_error("%s: property '%s' not found in object", objname, item);
				continue;
			}
			else
			{
				gl_error("%s: global '%s' not found", objname, item);
				continue;
			}
		}
	}
	free(list);
	return first;
}

int read_multi_properties(struct recorder *my, OBJECT *obj, RECORDER_MAP *rmap, char *buffer, int size)
{
	RECORDER_MAP *r;
	int offset = 0;
	int count = 0;
	double x;
	complex z; 
	for ( r = rmap ; r != NULL && offset < size - 33; r = r->next )
	{
		void *addr = ( r->obj == NULL || r->prop.oclass == NULL ? r->prop.addr : GETADDR(r->obj,&(r->prop)) );
		if ( offset > 0)
		{
			strcpy(buffer+offset++,",");
		}
		if ( r->prop.ptype == PT_double && ( r->scale != 1.0 || my->line_units == LU_NONE ) )
		{
			// copy value into local value, use fake PROP, feed into gl_get_vaule
			x = *gl_get_double(r->obj, &(r->prop));
			x *= r->scale;
			offset += snprintf(buffer+offset,size-offset-1,"%g",x);
			if ( my->line_units != LU_NONE && r->prop.unit != NULL )
			{
				offset += snprintf(buffer+offset,size-offset-1," %s",r->prop.unit->name);
			}
		} 
		else if ( r->prop.ptype == PT_complex && ( r->scale != 1.0 || my->line_units == LU_NONE ) )
		{
			// copy value into local value, use fake PROP, feed into gl_get_vaule
			z = *gl_get_complex(r->obj, &(r->prop));
			z *= r->scale;
			offset += snprintf(buffer+offset,size-offset-1,"%g%+g%c",z.Re(),z.Im(),z.Notation());
			if ( my->line_units != LU_NONE && r->prop.unit != NULL )
			{
				offset += snprintf(buffer+offset,size-offset-1," %s",r->prop.unit->name);
			}
		} 
		else 
		{
			offset += gl_get_value(r->obj, addr, buffer+offset, size-offset-1, &(r->prop)); /* pointer => int64 */
		}
		buffer[offset] = '\0';
		count++;
	}
	return count;
}

EXPORT TIMESTAMP sync_multi_recorder(OBJECT *obj, TIMESTAMP t0, PASSCONFIG pass)
{
	struct recorder *my = OBJECTDATA(obj,struct recorder);
	typedef enum {NONE='\0', LT='<', EQ='=', GT='>'} COMPAREOP;
	COMPAREOP comparison;
	char buffer[4096] = "";
	
	if (my->status==TS_DONE)
	{
		close_multi_recorder(my); /* note: potentially called every sync pass for multiple timesteps, catch fp==NULL in tape ops */
		return TS_NEVER;
	}

	/* legitimate to lack a parent by the definition of a multi-recorder.  -MH */
	/*
	if(obj->parent == NULL){
		char tb[32];
		sprintf(buffer, "'%s' lacks a parent object", obj->name ? obj->name : tb, sprintf(tb, "recorder:%i", obj->id));
		close_recorder(my);
		my->status = TS_ERROR;
		goto Error;
	}*/

	if(my->last.ts < 1 && my->interval != -1)
		my->last.ts = t0;

	/* connect to property */
	if (my->rmap==NULL){
		my->rmap = link_multi_properties(obj,my->property); // allowable use of obj->parent
	}
	/*	invalid target object must be handled individually */
	/*if (my->target==NULL)
	{
		sprintf(buffer,"'%s' contains a property of %s %d that is not found", my->property, obj->parent->oclass->name, obj->parent->id);
		close_multi_recorder(my);
		my->status = TS_ERROR;
		goto Error;
	}*/

	if (my->rmap==NULL)
	{
		if ( my->property )
		{
			sprintf(buffer,"property '%s' contains a reference that was not found", my->property);
		}
		else
		{
			sprintf(buffer,"property reference is missing");
		}
		close_multi_recorder(my);
		my->status = TS_ERROR;
		goto Error;
	}

	// update clock
	if ((my->status==TS_OPEN) && (t0 > obj->clock)) 
	{	
		obj->clock = t0;
		// if the recorder is clock-based, write the value
		if((my->interval > 0) && (my->last.ts < t0) && (my->last.value[0] != 0)){
			multi_recorder_write(obj);
			my->last.value[0] = 0; // once it's been finalized, dump it
		}
	}

	/* update property value */
	if ((my->rmap != NULL) && (my->interval == 0 || my->interval == -1)){	
		if(read_multi_properties(my, obj->parent,my->rmap,buffer,sizeof(buffer))==0) // vestigal use of parent
		{
			//sprintf(buffer,"unable to read property '%s' of %s %d", my->property, obj->parent->oclass->name, obj->parent->id);
			sprintf(buffer,"unable to read a property");
			close_multi_recorder(my);
			my->status = TS_ERROR;
		}
	}
	if ((my->rmap != NULL) && (my->interval > 0)){
		if((t0 >=my->last.ts + my->interval) || (t0 == my->last.ts)){
			if(read_multi_properties(my, obj->parent,my->rmap,buffer,sizeof(buffer))==0)
			{
				//sprintf(buffer,"unable to read property '%s' of %s %d", my->property, obj->parent->oclass->name, obj->parent->id);
				sprintf(buffer,"unable to read a property");
				close_multi_recorder(my);
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
		if (actual!=desired || (my->status==TS_INIT && !multi_recorder_open(obj)))
		{
			/* better luck next time */
			return (my->interval==0 || my->interval==-1) ? TS_NEVER : t0+my->interval;
		}
	}
	else if (my->status==TS_INIT && !multi_recorder_open(obj))
	{
		close_multi_recorder(my);
		return TS_NEVER;
	}

	if(my->last.ts < 1 && my->interval != -1)
		my->last.ts = t0;

	/* write tape */
	if (my->status==TS_OPEN)
	{	
		if (my->interval==0 /* sample on every pass */
			|| ((my->interval==-1) && my->last.ts!=t0 && strcmp(buffer,my->last.value)!=0) /* sample only when value changes */
			)

		{
			strncpy(my->last.value,buffer,sizeof(my->last.value));
			my->last.ts = t0;
			multi_recorder_write(obj);
		} else if (my->interval > 0 && my->last.ts == t0){
			strncpy(my->last.value,buffer,sizeof(my->last.value));
		}
	}
Error:
	if (my->status==TS_ERROR)
	{
		gl_error("recorder %d %s\n",obj->id, (char*)buffer);
		close_multi_recorder(my);
		my->status=TS_DONE;
		return TS_NEVER;
	}

	if (my->interval==0 || my->interval==-1) 
	{
		return TS_NEVER;
	}
	else
	{
		return my->last.ts+my->interval;
	}
}

/**@}*/
