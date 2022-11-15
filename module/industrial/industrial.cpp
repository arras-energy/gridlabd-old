// industrial.h
// Copyright (C) 2020, Regents of the Leland Stanford Junior University

#include "industrial.h"

EXPORT_CREATE(industrial);
EXPORT_INIT(industrial);
EXPORT_PRECOMMIT(industrial);

CLASS *industrial::oclass = NULL;
industrial *industrial::defaults = NULL;

char256 industrial::load_property_name = "base_power_";
char1024 industrial::naics_data_file = "naics_data_file.csv";
naics *industrial::naics_data = NULL;

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
			
			PT_int32, "naics_code", get_naics_code_offset(), 
				PT_REQUIRED,
				PT_DEFAULT,"0", 
				PT_DESCRIPTION, "NAICS facility code",

			PT_double, "power_pf", get_power_pf_offset(), 
				PT_DEFAULT, "1.0", 
				PT_DESCRIPTION, "power factor for constant power load",

			PT_double, "impedance_pf", get_power_pf_offset(), 
				PT_DEFAULT, "1.0", 
				PT_DESCRIPTION, "power factor for constant impedance load",

			// PT_double, "stall_voltage[pu]", get_stall_voltage_offset(),
			// 	PT_DEFAULT, "0.6 pu",
			// 	PT_DESCRIPTION, "voltage at which motors will stall",

			// PT_double, "start_voltage[pu]", get_start_voltage_offset(),
			// 	PT_DEFAULT, "0.7 pu",
			// 	PT_DESCRIPTION, "voltage at which motors will start",

			PT_double, "operating_factor[pu]", get_operating_factor_offset(),
				PT_DESCRIPTION, "fraction of facility power capacity that is operating",

			PT_double, "operating_capacity[MW]", get_operating_capacity_offset(),
				PT_DESCRIPTION, "facility power when operating at full capacity",

			PT_double, "total_power[W]", get_total_power_offset(),
				PT_OUTPUT,
				PT_DEFAULT, "0 MW",
				PT_DESCRIPTION, "total facility power demand",

			// PT_bool, "motor_stalled", get_motor_stalled_offset(),
			// 	PT_DESCRIPTION, "motor are stalled",

			NULL)<1){
				char msg[256];
				snprintf(msg,sizeof(msg)-1, "unable to publish properties in %s",__FILE__);
				throw msg;
		}
		gl_global_create("industrial::naics_data_file",PT_char1024,&naics_data_file,
			PT_DESCRIPTION, "data file that contains NAICS load data model",
			NULL);
		gl_global_create("industrial::load_property_name",PT_char256,&load_property_name,
			PT_DESCRIPTION, "parent base load property name to update",
			NULL);
	}
}

int industrial::create(void) 
{
	return 1; /* return 1 on success, 0 on failure */
}

int industrial::init(OBJECT *parent)
{
	naics_data = new naics(naics_data_file);
	naics::RECORD &zip = naics_data->find(naics_code);

	double power_fraction = zip.a + zip.b + zip.c + zip.d + zip.e;
	gld_property(parent,"power_fraction_A").setp(power_fraction);
	gld_property(parent,"power_fraction_B").setp(power_fraction);
	gld_property(parent,"power_fraction_C").setp(power_fraction);

	gld_property(parent,"current_fraction_A").setp(zip.i);
	gld_property(parent,"current_fraction_B").setp(zip.i);
	gld_property(parent,"current_fraction_C").setp(zip.i);

	gld_property(parent,"impedance_fraction_A").setp(zip.z);
	gld_property(parent,"impedance_fraction_B").setp(zip.z);
	gld_property(parent,"impedance_fraction_C").setp(zip.z);

	base_load[0] = new gld_property(parent,std::string(load_property_name).append("A").c_str());
	base_load[1] = new gld_property(parent,std::string(load_property_name).append("B").c_str());
	base_load[2] = new gld_property(parent,std::string(load_property_name).append("C").c_str());
	if ( ! base_load[0] || ! base_load[1] || ! base_load[2] )
		exception("unable to link base load property");
	return 1;
}

TIMESTAMP industrial::precommit(TIMESTAMP t1)
{
	if ( operating_capacity != 0.0 && operating_factor != 0.0 )
	{
		total_power = operating_capacity * operating_factor * 1000.0;
	}
	double phase_power = total_power/3;
	base_load[0]->setp(phase_power);
	base_load[1]->setp(phase_power);
	base_load[2]->setp(phase_power);
	// TODO: implement stalling behavior based on voltage
	return TS_NEVER;
}

/////////////////////////////////////////////////
//
// NAICS database
//
/////////////////////////////////////////////////

naics::naics(const char *filename)
{
	char pathname[1024];
	FILE *fp = fopen(gl_findfile(filename,NULL,R_OK,pathname,sizeof(pathname)) ? pathname : filename,"r");
	if ( fp == NULL )
	{
		throw "unable to open naics data file";
	}
	char line[1024];
	int linenum = 0;
	while ( fgets(line,sizeof(line),fp) != NULL )
	{
		linenum++;
		try
		{
			add(line);
		}
		catch (const char *error)
		{
			if ( linenum > 1 )
			{
				gl_warning("%s(%d): %s", filename, linenum, error);
			}
		}
	}
	fclose(fp);
}

naics::~naics(void)
{
	data.clear();
}

void naics::add(const char *str)
{
	RECORD *item = new RECORD;
	if ( item == NULL )
		throw "memory allocation failed";
	if ( sscanf(str,"%d,%*[^,],%*[^,],%lg,%lg,%lg,%lg,%lg,%lg,%lg",
		&item->n, &item->a, &item->b, &item->c, &item->d, &item->e, &item->z, &item->i) == 8 )
	{
		if ( item->n > 0 ) // ignore "0" codes (undefined NAICS code)
		{
			data.insert(data.end(),*item);
		}
	}
	else
	{
		throw "invalid NAICS record";
	}
}

naics::RECORD &naics::find(int n)
{
	for ( std::list<RECORD>::iterator p = data.begin() ; p != data.end() ; p++ )
	{
		if ( p->n == n )
			return *p;
	}
	throw "naics code not found";
}