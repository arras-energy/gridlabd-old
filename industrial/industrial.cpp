// industrial.h
// Copyright (C) 2020, Regents of the Leland Stanford Junior University

#include "industrial.h"

EXPORT_CREATE(industrial);
EXPORT_INIT(industrial);

CLASS *industrial::oclass = NULL;
industrial *industrial::defaults = NULL;

char1024 industrial::naics_data_file = "naics_data_file.csv";

industrial::industrial(MODULE *module)
{
	if (oclass==NULL)
	{
		// register to receive notice for first top down. bottom up, and second top down synchronizations
		oclass = gld_class::create(module,"industrial",sizeof(industrial),PC_AUTOLOCK|PC_OBSERVER);
		if (oclass==NULL)
			throw "unable to register class industrial";
		else
			oclass->trl = TRL_PROVEN;

		defaults = this;
		if (gl_publish_variable(oclass,
			
			PT_int32,"naics_code",get_naics_code_offset(), PT_DESCRIPTION,"NAICS facility code",

			NULL)<1){
				char msg[256];
				sprintf(msg, "unable to publish properties in %s",__FILE__);
				throw msg;
		}
		gl_global_create("industrial::naics_data_file",PT_char1024,&naics_data_file,
			PT_DESCRIPTION, "data file that contains NAICS load data model",
			NULL);
	}
}

int industrial::create(void) 
{
	return 1; /* return 1 on success, 0 on failure */
}

int industrial::init(OBJECT *parent)
{

	return 1;
}


