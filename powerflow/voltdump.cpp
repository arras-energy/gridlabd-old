// $Id: voltdump.cpp 4738 2014-07-03 00:55:39Z dchassin $
/**	Copyright (C) 2008 Battelle Memorial Institute

	@file voltdump.cpp

	@{
*/

#include "powerflow.h"
using namespace std;

//////////////////////////////////////////////////////////////////////////
// voltdump CLASS FUNCTIONS
//////////////////////////////////////////////////////////////////////////

CLASS* voltdump::oclass = NULL;

voltdump::voltdump(MODULE *mod)
{
	if (oclass==NULL)
	{
		// register the class definition
		oclass = gl_register_class(mod,"voltdump",sizeof(voltdump),PC_BOTTOMUP|PC_AUTOLOCK);
		if (oclass==NULL)
			throw "unable to register class voltdump";
		else
			oclass->trl = TRL_PROVEN;

		// publish the class properties
		if (gl_publish_variable(oclass,
			PT_char32,"group",PADDR(group),
				PT_DESCRIPTION,"the group ID to output data for (all nodes if empty)",
			PT_timestamp,"runtime",PADDR(runtime),
				PT_DEFAULT, "NEVER",
				PT_DESCRIPTION,"the time to check voltage data",
			PT_char256,"filename",PADDR(filename),
				PT_REQUIRED,
				PT_DESCRIPTION,"the file to dump the voltage data into", // must keep this for compatibility
			PT_char256,"file",PADDR(filename),
				PT_DEPRECATED,
				PT_DESCRIPTION,"the file to dump the voltage data into", // added 2012-07-10, adds convenience
			PT_int32,"runcount",PADDR(runcount),
				PT_ACCESS, PA_REFERENCE,
				PT_DEFAULT, "0",
				PT_DESCRIPTION,"the number of times the file has been written to",
			PT_int32,"maxcount",PADDR(maxcount),
				PT_DEFAULT, "-1",
				PT_DESCRIPTION,"the maximum number of times the file is written",
			PT_enumeration, "mode", PADDR(mode),
				PT_DEFAULT, "rect",
				PT_DESCRIPTION,"dumps the voltages in either polar or rectangular notation",
				PT_KEYWORD, "rect", (enumeration)VDM_RECT,
				PT_KEYWORD, "polar", (enumeration)VDM_POLAR,
			PT_char8, "filemode", PADDR(filemode),
				PT_DEFAULT, "",
				PT_DESCRIPTION,"sets the file write mode",
			PT_int32, "version", PADDR(version),
				PT_DEFAULT, "0",
				PT_DESCRIPTION, "dump format version",
			PT_double, "interval[s]", PADDR(interval),
				PT_DEFAULT, "0 s",
				PT_DESCRIPTION, "interval at which voltdump runs",
			NULL)<1) GL_THROW("unable to publish properties in %s",__FILE__);
		
	}
}


int voltdump::create(void)
{
	group.erase();
	return 1;
}

int voltdump::init(OBJECT *parent)
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
	if ( version < 0 || version > 2 )
	{
		gl_error("format version %d is invalid", version);
	}
	else if ( version == 0 )
	{
		version = 2;
	}
	return 1;
}

int voltdump::isa(CLASSNAME classname)
{
	return strcmp(classname,"voltdump")==0;
}

void voltdump::dump(TIMESTAMP t)
{
	char timestr[64];
	FINDLIST *nodes = NULL;
	OBJECT *obj = NULL;
	FILE *outfile = NULL;
	node *pnode;

	if ( group[0] == 0 )
	{
		nodes = gl_find_objects(FL_NEW,FT_MODULE,SAME,"powerflow",FT_END);
	} 
	else 
	{
		nodes = gl_find_objects(FL_NEW,FT_MODULE,SAME,"powerflow",AND,FT_GROUPID,SAME,group.get_string(),FT_END);
	}

	if ( nodes == NULL )
	{
		gl_warning("no nodes were found to dump");
		return;
	}

	if (strcmp(filemode,"a")==0)
	{
		gl_verbose("voltdump is appending data to %s",filename.get_string());
	}
	outfile = fopen(filename, filemode);
	if ( outfile == NULL ) 
	{
		gl_error("voltdump unable to open %s for output", filename.get_string());
		return;
	}

	int node_count = 0;
	while ( (obj=gl_find_next(nodes,obj)) )
	{
		if ( gl_object_isa(obj, "node", "powerflow") )
		{
			node_count += 1;
		}
	}
	/* print column names */
	gl_printtime(t, timestr, 64);
	if ( version < 2 )
	{
		fprintf(outfile,"# %s run at %s on %i nodes\nnode_name,", filename.get_string(), timestr, node_count);
	}
	else
	{
		fprintf(outfile,"node_name,datetime,");
	}
	if (mode == VDM_RECT)
		fprintf(outfile,"voltA_real,voltA_imag,voltB_real,voltB_imag,voltC_real,voltC_imag\n");
	else if (mode == VDM_POLAR)
		fprintf(outfile,"voltA_mag,voltA_angle,voltB_mag,voltB_angle,voltC_mag,voltC_angle\n");
	obj = 0;
	while ( (obj=gl_find_next(nodes,obj)) )
	{
		if ( gl_object_isa(obj, "node", "powerflow") )
		{
			pnode = OBJECTDATA(obj,node);
			if ( obj->name == NULL ) 
			{
				fprintf(outfile, "%s:%i,", obj->oclass->name, obj->id);
			}
			else
			{
				fprintf(outfile, "%s,", obj->name);				
			}
			if ( version > 1 )
			{
				fprintf(outfile,"%s,",timestr);
			}
			if ( mode == VDM_RECT )
			{
				fprintf(outfile,"%f,%f,%f,%f,%f,%f\n",pnode->voltage[0].Re(),pnode->voltage[0].Im(),pnode->voltage[1].Re(),pnode->voltage[1].Im(),pnode->voltage[2].Re(),pnode->voltage[2].Im());
			} 
			else if ( mode == VDM_POLAR )
			{
				fprintf(outfile,"%f,%f,%f,%f,%f,%f\n",pnode->voltage[0].Mag(),pnode->voltage[0].Arg(),pnode->voltage[1].Mag(),pnode->voltage[1].Arg(),pnode->voltage[2].Mag(),pnode->voltage[2].Arg());
			}
		}
	}
	fclose(outfile);
}

TIMESTAMP voltdump::commit(TIMESTAMP t)
{	
	gl_verbose("Interval is equal to : %f", interval);
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
	
	if ( runtime == 0 )
	{
		runtime = t;
	}
	if ( (t >= runtime || runtime == TS_NEVER) && (runcount < maxcount || maxcount < 0) )
	{
		/* dump */
		dump(t);
		++runcount;
	}
	return TS_NEVER;
}

//////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION OF CORE LINKAGE: voltdump
//////////////////////////////////////////////////////////////////////////

/**
* REQUIRED: allocate and initialize an object.
*
* @param obj a pointer to a pointer of the last object in the list
* @param parent a pointer to the parent of this object
* @return 1 for a successfully created object, 0 for error
*/
EXPORT int create_voltdump(OBJECT **obj, OBJECT *parent)
{
	try
	{
		*obj = gl_create_object(voltdump::oclass);
		if (*obj!=NULL)
		{
			voltdump *my = OBJECTDATA(*obj,voltdump);
			gl_set_parent(*obj,parent);
			return my->create();
		}
		else
			return 0;
	}
	CREATE_CATCHALL(voltdump);
}

EXPORT int init_voltdump(OBJECT *obj)
{
	try {
		voltdump *my = OBJECTDATA(obj,voltdump);
		return my->init(obj->parent);
	}
	INIT_CATCHALL(voltdump);
}

EXPORT TIMESTAMP sync_voltdump(OBJECT *obj, TIMESTAMP t1, PASSCONFIG pass)
{
	try
	{
		voltdump *my = OBJECTDATA(obj,voltdump);
		TIMESTAMP rv;
		obj->clock = t1;
		rv = my->runtime > t1 ? my->runtime : TS_NEVER;
		return rv;
	}
	SYNC_CATCHALL(voltdump);
}

EXPORT TIMESTAMP commit_voltdump(OBJECT *obj, TIMESTAMP t1, TIMESTAMP t2){
	try {
		voltdump *my = OBJECTDATA(obj,voltdump);
		return my->commit(t1);
	} 
	I_CATCHALL(commit,voltdump);
}

EXPORT int isa_voltdump(OBJECT *obj, CLASSNAME classname)
{
	return OBJECTDATA(obj,voltdump)->isa(classname);
}

/**@}*/
