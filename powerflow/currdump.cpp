// $Id: currdump.cpp 4738 2014-07-03 00:55:39Z dchassin $
/**	Copyright (C) 2008 Battelle Memorial Institute

	@file currdump.cpp

	@{
*/

#include "powerflow.h"
using namespace std;

//////////////////////////////////////////////////////////////////////////
// currdump CLASS FUNCTIONS
//////////////////////////////////////////////////////////////////////////

CLASS* currdump::oclass = NULL;

currdump::currdump(MODULE *mod)
{
	if (oclass==NULL)
	{
		// register the class definition
		oclass = gl_register_class(mod,"currdump",sizeof(currdump),PC_BOTTOMUP|PC_AUTOLOCK);
		if (oclass==NULL)
			GL_THROW("unable to register object class implemented by %s",__FILE__);

		// publish the class properties
		if (gl_publish_variable(oclass,
			PT_char32,"group",PADDR(group),PT_DESCRIPTION,"the group ID to output data for (all links if empty)",
			PT_timestamp,"runtime",PADDR(runtime),PT_DESCRIPTION,"the time to check current data",
			PT_char256,"filename",PADDR(filename),PT_DESCRIPTION,"the file to dump the current data into",
			PT_int32,"runcount",PADDR(runcount),PT_ACCESS,PA_REFERENCE,PT_DESCRIPTION,"the number of times the file has been written to",
			PT_int32,"maxcount",PADDR(maxcount),PT_DESCRIPTION,"the maximum number of times the file is written",
			PT_double, "interval[s]", PADDR(interval), PT_DESCRIPTION, "interval at which currdump runs",
			PT_char8, "filemode", PADDR(filemode), PT_DESCRIPTION,"sets the file write mode",
			PT_enumeration, "mode", PADDR(mode),
				PT_KEYWORD, "RECT", (enumeration)CDM_RECT,
				PT_KEYWORD, "POLAR", (enumeration)CDM_POLAR,
			NULL)<1) GL_THROW("unable to publish properties in %s",__FILE__);
		
	}
}


int currdump::create(void)
{
	group.erase();
	runtime = TS_NEVER;
	runcount = 0;
	mode = CDM_RECT;
	interval = 0;
	strcpy(filemode,"w");
	maxcount = 1;
	return 1;
}

int currdump::init(OBJECT *parent)
{
	unlink(filename);
	if ( interval < 0 )
	{
		gl_error("negative interval is not permitted");
		return 0;
	}
	else if ( interval > 0 )
	{
		if ( maxcount < 0 ) 
		{
			maxcount = 0; 
		}
		if ( strcmp(filemode,"") == 0 )
		{
			strcpy(filemode,"a");
		}
		runtime = TS_NEVER;
	}
	else
	{
		if ( maxcount < 0 )
		{
			maxcount = 1;
		}
		if ( strcmp(filemode,"") == 0 )
		{
			strcpy(filemode,"w");
		}
	}
	return 1;
}

int currdump::isa(char *classname)
{
	return strcmp(classname,"currdump")==0;
}

void currdump::dump(TIMESTAMP t){
	char namestr[64];
	char timestr[64];
	FINDLIST *links = NULL;
	OBJECT *obj = NULL;
	FILE *outfile = NULL;
	link_object *plink;
//	CLASS *nodeclass = NULL;
//	PROPERTY *vA, *vB, *vC;

	if(group[0] == 0){
		links = gl_find_objects(FL_NEW,FT_MODULE,SAME,"powerflow",FT_END);
	} else {
		links = gl_find_objects(FL_NEW,FT_MODULE,SAME,"powerflow",AND,FT_GROUPID,SAME,group.get_string(),FT_END);
	}

	if(links == NULL){
		gl_warning("no links were found to dump");
		return;
	}

	if (strcmp(filemode,"a")==0)
	{
		gl_verbose("voltdump is appending data to %s",filename.get_string());
	}

	outfile = fopen(filename, filemode);
	if(outfile == NULL){
		gl_error("currdump unable to open %s for output", filename.get_string());
		return;
	}

	//nodeclass = node::oclass;
	//vA=gl_find_property(nodeclass, "

	int link_count = 0;
	while ( (obj=gl_find_next(links, obj)) != NULL )
	{
		if(gl_object_isa(obj, "link", "powerflow")){
			++link_count;
		}
	}
	/* print column names */
	gl_printtime(t, timestr, 64);
	fprintf(outfile,"# %s run at %s on %i links\n", filename.get_string(), timestr, link_count);
	if(mode == CDM_RECT){
		fprintf(outfile,"link_name,currA_real,currA_imag,currB_real,currB_imag,currC_real,currC_imag\n");
	}
	else if (mode == CDM_POLAR){
		fprintf(outfile,"link_name,currA_mag,currA_angle,currB_mag,currB_angle,currC_mag,currC_angle\n");
	}
	obj = 0;
	while ( (obj=gl_find_next(links,obj)) != NULL )
	{
		if(gl_object_isa(obj, "link", "powerflow")){
			plink = OBJECTDATA(obj,link_object);
			if(obj->name == NULL){
				sprintf(namestr, "%s:%i", obj->oclass->name, obj->id);
			}
			if(mode == CDM_RECT){
				fprintf(outfile,"%s,%f,%f,%f,%f,%f,%f\n",(obj->name ? obj->name : namestr),plink->read_I_in[0].Re(),plink->read_I_in[0].Im(),plink->read_I_in[1].Re(),plink->read_I_in[1].Im(),plink->read_I_in[2].Re(),plink->read_I_in[2].Im());
			} else if (mode == CDM_POLAR){
				fprintf(outfile,"%s,%f,%f,%f,%f,%f,%f\n",(obj->name ? obj->name : namestr),plink->read_I_in[0].Mag(),plink->read_I_in[0].Arg(),plink->read_I_in[1].Mag(),plink->read_I_in[1].Arg(),plink->read_I_in[2].Mag(),plink->read_I_in[2].Arg());
			}
		}
	}
	fclose(outfile);
}

TIMESTAMP currdump::commit(TIMESTAMP t){
	if(runtime == 0){
		runtime = t;
	}
	if ( interval != 0 ) 
	{	
		unsigned long long dt = (unsigned long long)interval;
		if ( t % dt == 0 )
		{
			dump(t);
			++runcount;
		}
		return ( maxcount > 0 && runcount > maxcount) ? TS_NEVER : ((t/dt)+1)*dt;
	}
	if((t >= runtime || runtime == TS_NEVER) && (runcount < maxcount || maxcount <0))
	{
		/* dump */
		dump(t);
		++runcount;
	}
	return TS_NEVER;
}

//////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION OF CORE LINKAGE: currdump
//////////////////////////////////////////////////////////////////////////

/**
* REQUIRED: allocate and initialize an object.
*
* @param obj a pointer to a pointer of the last object in the list
* @param parent a pointer to the parent of this object
* @return 1 for a successfully created object, 0 for error
*/
EXPORT int create_currdump(OBJECT **obj, OBJECT *parent)
{
	try
	{
		*obj = gl_create_object(currdump::oclass);
		if (*obj!=NULL)
		{
			currdump *my = OBJECTDATA(*obj,currdump);
			gl_set_parent(*obj,parent);
			return my->create();
		}
	}
	catch (const char *msg)
	{
		gl_error("create_currdump: %s", msg);
	}
	return 0;
}

EXPORT int init_currdump(OBJECT *obj)
{
	currdump *my = OBJECTDATA(obj,currdump);
	try {
		return my->init(obj->parent);
	}
	catch (const char *msg)
	{
		gl_error("%s (currdump:%d): %s", obj->name, obj->id, msg);
		return 0; 
	}
}

EXPORT TIMESTAMP sync_currdump(OBJECT *obj, TIMESTAMP t1, PASSCONFIG pass)
{
	currdump *my = OBJECTDATA(obj,currdump);
	TIMESTAMP rv;
	obj->clock = t1;
	rv = my->runtime > t1 ? my->runtime : TS_NEVER;
	return rv;
}

EXPORT TIMESTAMP commit_currdump(OBJECT *obj, TIMESTAMP t1, TIMESTAMP t2){
	currdump *my = OBJECTDATA(obj,currdump);
	try {
		return my->commit(t1);
	} catch(const char *msg){
		gl_error("%s (currdump:%d): %s", obj->name, obj->id, msg);
		return 0;
	}
}

EXPORT int isa_currdump(OBJECT *obj, char *classname)
{
	return OBJECTDATA(obj,currdump)->isa(classname);
}

/**@}*/
