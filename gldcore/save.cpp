/* save.cpp
 * Copyright (C) 2008 Battelle Memorial Institute
 *
 * Top level save routine.  Dispatches saves to subcomponents.  Format of save must be compatible with load module.
 */

#include "gldcore.h"

SET_MYCONTEXT(DMC_SAVE)

#define DEFAULT_FORMAT "gld"


int saveglm(const char *filename, FILE *fp);
int savexml(const char *filename, FILE *fp);
int savejson(const char *filename, FILE *fp);
int savexml_strict(const char *filename, FILE *fp);

int saveall(const char *filename)
{
	FILE *fp;
	const char *ext = strrchr(filename,'.');
	struct {
		const char *format;
		int (*save)(const char*,FILE*);
	} map[] = {
		{"gld", saveglm},
		{"glm", saveglm},
		//{"xml", savexml_strict},
		{"xml", savexml},
		{"json", savejson},
	};
	size_t i;

	/* identify output format */
	if (ext==NULL)
	{	/* no extension given */
		if (filename[0]=='-') /* stdout */
		{
			ext=filename+1; /* format is specified after - */
		}
		else
		{
			ext=DEFAULT_FORMAT;
		}
	}
	else
		ext++;

	bool known_format = false;
	for (i=0; i<sizeof(map)/sizeof(map[0]); i++)
	{
		if (strcmp(ext,map[i].format)==0)
		{
			known_format = true;
		}
	}
	if ( ! known_format )
	{
		/* try using python output converter through json */
		char converter_name[1024];
		sprintf(converter_name,"json2%s.py",ext);
		const char *converter_path = find_file(converter_name,NULL,R_OK);
		if ( ! converter_path )
		{
			output_error("saveall: extension '.%s' not a known format", ext);
			/*	TROUBLESHOOT
				Only the format extensions ".txt", ".gld", and ".xml" are recognized by
				GridLAB-D.  Please end the specified output field accordingly, or omit the
				extension entirely to force use of the default format.
			*/
			errno = EINVAL;
			return 0;
		}
		char converter_command[1024];
		char input_name[1024];
		strcpy(input_name,filename);
		char *in_ext = strrchr(input_name,'.');
		if ( in_ext == NULL )
		{
			output_error("intermediate file '%s' extension not found", input_name);
			errno = EINVAL;
			return 0;
		}
		strcpy(in_ext,".json");
		set old_fso = global_filesave_options;
		global_filesave_options = FSO_ALL;
		fp = fopen(input_name,"wb");
		if ( fp == NULL )
		{
			output_error("unable to open intermediate file '%s' for writing", input_name);
			return 0;
		}
		int rc = savejson(input_name,fp);
		fclose(fp);
		global_filesave_options = old_fso;
		if ( rc == 0 )
		{
			output_error("save to intermediate file '%s' failed (code %d)", input_name, rc);;
			return 0;
		}
		char options_name[1024];
		sprintf(options_name,"%s_save_options",ext);
		char buffer[1024];
		const char *save_options = global_getvar(options_name,buffer,sizeof(buffer));
		if ( buffer[0] == '"' )
		{
			save_options++;
			buffer[strlen(buffer)-1] = '\0';
		}
		sprintf(converter_command,"/usr/local/bin/python3 %s -i %s -o %s %s",converter_path,input_name,filename,save_options?save_options:"");
		output_verbose("system('%s')",converter_command);
		rc = system(converter_command);
		if ( rc != 0 )
		{
			output_error("conversion from intermediate file '%s' to output file '%s' failed (code %d)", input_name, filename, rc);
			return 0;
		}
		struct stat info;
		info.st_size = 0;
		if ( stat(filename,&info) != 0 )
		{
			output_error("unable to determine size of output file '%s' (%s)", filename, strerror(errno));
		}
		return info.st_size;
	}

	/* setup output stream */
	if (filename[0]=='-')
	{
		fp = stdout;
	}
	else if ( (fp=fopen(filename,"wb")) == NULL )
	{
		output_error("saveall: unable to open stream \'%s\' for writing", filename);
		return 0;
	}

	IN_MYCONTEXT output_debug("starting dump to %s",filename);
	/* internal streaming used */
	if (global_streaming_io_enabled)
	{
		int res = stream(fp,SF_OUT)>0 ? SUCCESS : FAILED;
		if ( res == 0 )
		{
			output_error("stream context is %s",stream_context());
		}
		if ( fp != stdout ) 
		{
			fclose(fp);
		}
		return res;
	}

	/* general purpose format used */
	for (i=0; i<sizeof(map)/sizeof(map[0]); i++)
	{
		if (strcmp(ext,map[i].format)==0)
		{
			int rc = (*(map[i].save))(filename,fp);
			if ( fp != stdout )
			{
				fclose(fp);
			}
			IN_MYCONTEXT output_debug("dump to %s completed ok (%d bytes written)",filename,rc);
			return rc;
		}
	}
	return 0;
}

int saveglm(const char *filename,FILE *fp)
{
	unsigned int count = 0;
	time_t now = time(NULL);
	char buffer[1024];

	count += fprintf(fp,"////////////////////////////////////////////////////////\n");
	count += fprintf(fp,"// BEGIN");
	count += fprintf(fp,"\n////////////////////////////////////////////////////////\n");
	count += fprintf(fp,"// filename... %s\n", filename);
	count += fprintf(fp,"// workdir.... %s\n", global_workdir);
	count += fprintf(fp,"// command.... %s\n", global_command_line);
	count += fprintf(fp,"// created.... %s", asctime(localtime(&now)));
	count += fprintf(fp,"// user....... %s\n",
#ifdef WIN32
		getenv("USERNAME")
#else
		getenv("USER")
#endif
		);
	count += fprintf(fp,"// host....... %s\n",
#ifdef WIN32
		getenv("COMPUTERNAME")
#else
		getenv("HOSTNAME")
#endif
		);
	count += fprintf(fp,"// modules.... %zu\n", module_getcount());
	count += fprintf(fp,"// classes.... %d\n", class_get_count());
	count += fprintf(fp,"// objects.... %d\n", object_get_count());
	if ( global_getvar("glm_save_options",buffer,sizeof(buffer)-1) )
	{
		count += fprintf(fp,"// options.... %s\n", buffer);
	}

	// loader flags
	if ( (global_glm_save_options&GSO_NOMACROS)==0 )
	{
		count += fprintf(fp,"\n// flags to enable GLM definitions not supported by standard GLM loader");
		count += fprintf(fp,"//#define INCLUDE_PARENT_CLASS=TRUE // class inheritance definitions\n");
		count += fprintf(fp,"//#define INCLUDE_FUNCTIONS=TRUE // class function definitions\n");
		count += fprintf(fp,"//#define INCLUDE_ROOT=TRUE // object root definitions\n");
		count += fprintf(fp,"//#define INCLUDE_REFERENCE=TRUE // reference property definitions\n");
		count += fprintf(fp,"//#define INCLUDE_PROTECTED=TRUE // protected property definitions\n");
		count += fprintf(fp,"//#define INCLUDE_PRIVATE=TRUE // private property definitions\n");
		count += fprintf(fp,"//#define INCLUDE_HIDDEN=TRUE // hidden property definitions\n");
	}

	/* save gui, if any */
	if (gui_get_root()!=NULL)
	{
		count += fprintf(fp,"\n////////////////////////////////////////////////////////\n");
		count += fprintf(fp,"\n# GUI\n");
		count += (int)gui_glm_write_all(fp);
		count += fprintf(fp,"\n");
	}

	/* save clock */
	if ( (global_filesave_options&FSO_CLOCK) == FSO_CLOCK )
	{
		count += fprintf(fp,"\n////////////////////////////////////////////////////////\n");
		count += fprintf(fp,"\n// CLOCK\n");
		count += fprintf(fp,"clock {\n");
		count += fprintf(fp,"\ttimezone \"%s\";\n", timestamp_current_timezone());
		if ( convert_from_timestamp(global_starttime,buffer,sizeof(buffer))>0 )
		{
			count += fprintf(fp,"\tstarttime \"%s\";\n", buffer);
		}
		if ( convert_from_timestamp(global_stoptime,buffer,sizeof(buffer))>0 )
		{
			count += fprintf(fp,"\tstoptime \"%s\";\n", buffer);
		}
		count += fprintf(fp,"}\n");
	}

	/* save parts */
	if ( (global_glm_save_options&GSO_NOGLOBALS)==0 && (global_filesave_options&FSO_GLOBALS)==FSO_GLOBALS )
	{
		count += global_saveall(fp);
	}
	if ( (global_filesave_options&FSO_MODULES) == FSO_MODULES )
	{
		count += module_saveall(fp);
	}
	if ( (global_filesave_options&FSO_CLASSES) == FSO_CLASSES )
	{
		count += class_saveall(fp);
	}
	if ( (global_glm_save_options&GSO_NOINTERNALS)==0 && (global_filesave_options&FSO_SCHEDULES) == FSO_SCHEDULES )
	{
		count += schedule_saveall(fp);
	}
	if ( (global_filesave_options&FSO_FILTERS) == FSO_FILTERS )
	{
		count += transform_saveall(fp);
	}
	if ( (global_filesave_options&FSO_OBJECTS) == FSO_OBJECTS )
	{
		count += object_saveall(fp);
	}
	count += fprintf(fp,"\n////////////////////////////////////////////////////////\n");
	count += fprintf(fp,"// END");
	count += fprintf(fp,"\n////////////////////////////////////////////////////////\n");

	return count;
}

int savexml_strict(const char *filename,FILE *fp)
{
	unsigned int count = 0;
	char buffer[1024];
	GLOBALVAR *global=NULL;
	MODULE *module;
	GLOBALVAR *stylesheet = global_find("stylesheet");

	int old_suppress_deprecated = global_suppress_deprecated_messages;
	global_suppress_deprecated_messages = 1;

	count += fprintf(fp,"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
	if (stylesheet==NULL || stylesheet->prop->ptype!=PT_char1024) /* only char1024 is allowed */
		count += fprintf(fp,"<?xml-stylesheet href=\"%sgridlabd-%d_%d.xsl\" type=\"text/xsl\"?>\n",global_urlbase,global_version_major,global_version_minor);
	else 
		count += fprintf(fp,"<?xml-stylesheet href=\"%s.xsl\" type=\"text/xsl\"?>\n",stylesheet->prop->name);
	count += fprintf(fp,"<gridlabd>\n");
	
		/* globals */
		while ((global=global_getnext(global))!=NULL)
		{
			/* ignore module globals */
			if ( strchr(global->prop->name,':') )
				continue;
			if ( global_getvar(global->prop->name,buffer,sizeof(buffer)) )
				count += fprintf(fp,"\t<%s>%s</%s>\n", global->prop->name, buffer, global->prop->name);
		}
		count += fprintf(fp,"\t<timezone>%s</timezone>\n", timestamp_current_timezone());

		/* rank index */
		fprintf(fp,"\t<sync-order>\n");
		{
#define PASSINIT(p) (p % 2 ? ranks[p]->first_used : ranks[p]->last_used)
#define PASSCMP(i, p) (p % 2 ? i <= ranks[p]->last_used : i >= ranks[p]->first_used)
#define PASSINC(p) (p % 2 ? 1 : -1)
			INDEX **ranks = exec_getranks();
			LISTITEM *item;
			int i;
			size_t pass;
			for (pass=0; ranks!=NULL && ranks[pass]!=NULL; pass++)
			{
				const char *passname[]={"pretopdown","bottomup","posttopdown"};
				OBJECTRANK lastrank=-1;
				fprintf(fp,"\t\t<pass>\n\t\t\t<name>%s</name>\n",passname[pass]);
				for (i = PASSINIT(pass); PASSCMP(i, pass); i += PASSINC(pass))
				{
					if (ranks[pass]->ordinal[i]!=NULL)
					{
						for (item=ranks[pass]->ordinal[i]->first; item!=NULL; item=item->next)
						{
							OBJECT *obj = (OBJECT*)(item->data);
							if (obj->rank!=lastrank)
							{
								if (lastrank>=0)
									fprintf(fp,"\t\t\t</rank>\n");
								fprintf(fp,"\t\t\t<rank>\n");
								fprintf(fp,"\t\t\t\t<ordinal>%d</ordinal>\n",obj->rank);
								lastrank = obj->rank;
							}
							fprintf(fp,"\t\t\t\t<object>\n");
							if (obj->name) 
								fprintf(fp,"\t\t\t\t\t<name>%s</name>\n",obj->name);
							else
								fprintf(fp,"\t\t\t\t\t<name>(%s:%d)</name>\n",obj->oclass->name, obj->id);
							fprintf(fp,"\t\t\t\t\t<id>%d</id>\n",obj->id);
							fprintf(fp,"\t\t\t\t</object>\n");
						}
					}
				}
				if (lastrank>=0)
					fprintf(fp,"\t\t\t</rank>\n");
				fprintf(fp,"\t\t</pass>\n");
			}
		}
		fprintf(fp,"\t</sync-order>\n");

		/* modules */
		for (module=module_get_first(); module!=NULL; module=module->next)
		{
			CLASS *oclass;
			count += fprintf(fp, "\t<%s>\n",module->name);
			count += fprintf(fp,"\t\t<version.major>%d</version.major>\n", module->major);
			count += fprintf(fp,"\t\t<version.minor>%d</version.minor>\n", module->minor);

			/* globals */
			while ((global=global_getnext(global))!=NULL)
			{
				/* ignore globals not belonging to this module */
				char modname[64], name[64];
				if (sscanf(global->prop->name,"%s:%s",modname,name)<2 || strcmp(modname,module->name)!=0)
					continue;
				count += fprintf(fp,"\t\t<%s>%s</%s>\n", name, global_getvar(global->prop->name,buffer,sizeof(buffer))==NULL?"[error]":buffer,name);
			}

			/* objects */
			for (oclass=module->oclass; oclass!=NULL && oclass->module==module; oclass=oclass->next)
			{
				OBJECT *obj;
				count += fprintf(fp,"\t\t<%s_list>\n", oclass->name);
				if (oclass->parent) count += fprintf(fp,"\t\t\t<inherits_from>%s</inherits_from>\n",oclass->parent->name);
				for (obj=object_get_first(); obj!=NULL; obj=obj->next)
				{
					if (obj->oclass==oclass)
					{
						CLASS *pclass;
						PROPERTY *prop;
						DATETIME dt;
						count += fprintf(fp,"\t\t\t<%s>\n",oclass->name);
						count += fprintf(fp,"\t\t\t\t<id>%d</id>\n",obj->id);
						count += fprintf(fp,"\t\t\t\t<rank>%d</rank>\n",obj->rank);
						if ( isfinite(obj->latitude) && convert_from_latitude(obj->latitude,buffer,sizeof(buffer)) > 0 )
							count += fprintf(fp,"\t\t\t\t<latitude>%s</latitude>\n",buffer);
						if ( isfinite(obj->longitude) && convert_from_longitude(obj->longitude,buffer,sizeof(buffer)) > 0 )
							count += fprintf(fp,"\t\t\t\t<longitude>%s</longitude>\n",buffer);
						strcpy(buffer,"NEVER");
						if (obj->in_svc==TS_NEVER || (obj->in_svc>0 && local_datetime(obj->in_svc,&dt) && strdatetime(&dt,buffer,sizeof(buffer))>0)) 
							count += fprintf(fp,"\t\t\t\t<in_svc>%s</in_svc>\n",buffer);
						strcpy(buffer,"NEVER");
						if (obj->out_svc==TS_NEVER || (obj->out_svc>0 && local_datetime(obj->out_svc,&dt) && strdatetime(&dt,buffer,sizeof(buffer))>0))
							count += fprintf(fp,"\t\t\t\t<out_svc>%s</out_svc>\n",buffer);
						if (obj->parent)
						{
							if (obj->parent->name)
								count+=fprintf(fp,"\t\t\t\t<parent>%s</parent>\n",obj->parent->name);
							else
								count+=fprintf(fp,"\t\t\t\t<parent>%s:%d</parent>\n",obj->parent->oclass->name, obj->parent->id);
						}
						strcpy(buffer,"NEVER");
						if (obj->clock==TS_NEVER || (obj->clock>0 && local_datetime(obj->clock,&dt) && strdatetime(&dt,buffer,sizeof(buffer))>0)) 
							count += fprintf(fp,"\t\t\t\t<clock>%s</clock>\n",buffer);
						if (obj->name!=NULL) 
							count += fprintf(fp,"\t\t\t\t<name>%s</name>\n",obj->name);
						else
							count += fprintf(fp,"\t\t\t\t<name>%s:%d</name>\n",obj->oclass->name,obj->id);
						for (pclass=oclass; pclass!=NULL; pclass=pclass->parent)
						{
							for (prop=pclass->pmap; prop!=NULL && prop->oclass==pclass->pmap->oclass; prop=prop->next)
							{
								if (prop->unit!=NULL && strcmp(prop->unit->name,"V")==0 && prop->ptype==PT_complex)
								{
									complex *pval = object_get_complex(obj,prop);
									if (pval)
										pval->Notation() = A;
								}
								if ( prop->ptype != PT_method && object_get_value_by_name(obj,prop->name,buffer,sizeof(buffer))>0 && strcmp(buffer,"") != 0 )
									count += fprintf(fp,"\t\t\t\t<%s>%s</%s>\n",prop->name,buffer,prop->name);
							}
						}
						count += fprintf(fp,"\t\t\t</%s>\n",oclass->name);
					}
				}
				count += fprintf(fp,"\t\t</%s_list>\n", oclass->name);
			}

			count += fprintf(fp, "\t</%s>\n",module->name);
		}

	count += fprintf(fp,"</gridlabd>\n");

	global_suppress_deprecated_messages = old_suppress_deprecated;

	return count;
}

/*
 *	savexml() results in an XML file that can be reflexively read by the XML loader.  Note that the output
 *	of savexml_strict results in output that cannot be parsed back in, but can be parsed by automatically
 *	generated XSD files.
 */
int savexml(const char *filename,FILE *fp)
{
	unsigned int count = 0;
	char buffer[1024];
	GLOBALVAR *gvptr = global_getnext(NULL);

	if (global_xmlstrict)
		return savexml_strict(filename, fp);

	count += fprintf(fp,"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
	count += fprintf(fp,"<load>\n");
	count += fprintf(fp, "\t<global>\n");
	count += fprintf(fp,"\t\t<class_count>%d</class_count>\n", class_get_count());
	count += fprintf(fp,"\t\t<object_count>%d</object_count>\n", object_get_count());
	/* add global variables */
	while(gvptr != NULL){
		const char *testp = strchr(gvptr->prop->name, ':');
		if(testp == NULL){
			count += fprintf(fp, "\t\t<%s>%s</%s>\n", gvptr->prop->name, class_property_to_string(gvptr->prop,(void*)gvptr->prop->addr,buffer,1024)>0 ? buffer : "...", gvptr->prop->name);
		} // else we have a module::prop name
		gvptr = global_getnext(gvptr);
	}
	count += fprintf(fp, "\t</global>\n");

	/* save clock */
	count += fprintf(fp,"\t<clock>\n");
	count += fprintf(fp,"\t\t<tick>1e%+d</tick>\n",TS_SCALE);
	count += fprintf(fp,"\t\t<timestamp>%s</timestamp>\n", convert_from_timestamp(global_clock,buffer,sizeof(buffer))>0?buffer:"(invalid)");
	if (getenv("TZ"))
		count += fprintf(fp,"\t\t<timezone>%s</timezone>\n", getenv("TZ"));
	count += fprintf(fp,"\t</clock>\n");

	/* save parts */
	module_saveall_xml(fp);

	count += fprintf(fp,"</load>\n");
	return count;
}

int savejson(const char *filename, FILE *fp)
{
	GldJsonWriter json(filename);
	return json.write_output(fp);
}
