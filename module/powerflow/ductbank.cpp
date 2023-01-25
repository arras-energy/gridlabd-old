// module/powerflow/ductbank.cpp
// Copyright (C) 2023 Regents of the Leland Stanford Junior University

#include "powerflow.h"

EXPORT_CREATE(ductbank);
EXPORT_INIT(ductbank);
EXPORT_COMMIT(ductbank);
EXPORT_PRECOMMIT(ductbank);

CLASS *ductbank::oclass = NULL;
ductbank *ductbank::defaults = NULL;
double ductbank::default_ground_temperature = 10; // degC
double ductbank::warning_cable_temperature = 80; // degC
double ductbank::alert_cable_temperature = 90; // degC
char1024 ductbank::ductbank_configuration_file = "ductbank_configurations.csv";
DUCTBANK_DATA *ductbank::ductbank_data = NULL;

ductbank::ductbank(MODULE *module)
{
	if (oclass==NULL)
	{
		// register to receive notice for first top down. bottom up, and second top down synchronizations
		oclass = gld_class::create(module,"ductbank",sizeof(ductbank),PC_AUTOLOCK|PC_OBSERVER);
		if (oclass==NULL)
			throw "unable to register class ductbank";
		else
			oclass->trl = TRL_PROVEN;

		defaults = this;
		if (gl_publish_variable(oclass,

			PT_char1024, "configuration", get_configuration_offset(),
				PT_DESCRIPTION, "ductbank configuration name",

			PT_double, "ground_temperature[degC]", get_ground_temperature_offset(),
				PT_DESCRIPTION, "duct ground temperature",

			PT_double, "duct_R[K*m/W]", get_duct_R_offset(),
				PT_DESCRIPTION, "duct thermal resistance per unit duct length",

			PT_double, "airgap_R[K*m/W]", get_airgap_R_offset(),
				PT_DESCRIPTION, "air gap thermal resistance per unit duct length",

			PT_double, "insulation_R[K*m/W]", get_insulation_R_offset(),
				PT_DESCRIPTION, "cable insulation thermal resistance per unit duct length",

			PT_double, "section_area[m^2]", get_section_area_offset(),
				PT_DESCRIPTION, "cross sectional area of interior air space",

			PT_double, "heatgain[W/m]", get_heatgain_offset(),
				PT_OUTPUT,
				PT_DESCRIPTION, "cable heat per unit length",

			PT_double, "duct_temperature[degC]", get_duct_temperature_offset(),
				PT_OUTPUT,
				PT_DESCRIPTION, "duct shell temperature",

			PT_double, "air_temperature[degC]", get_air_temperature_offset(),
				PT_OUTPUT,
				PT_DESCRIPTION, "duct air temperature",

			PT_double, "cable_temperature[degC]", get_cable_temperature_offset(),
				PT_OUTPUT,
				PT_DESCRIPTION, "duct cable temperature",

			PT_enumeration, "cable_status", get_cable_status_offset(),
				PT_OUTPUT,
				PT_KEYWORD, "OK", (enumeration)CS_OK,
				PT_KEYWORD, "WARNING", (enumeration)CS_WARNING,
				PT_KEYWORD, "ALERT", (enumeration)CS_ALERT,
				PT_DESCRIPTION, "cable status",

			NULL)<1)
		{
				throw "unable to publish properties in " __FILE__;
		}

		gl_global_create("powerflow::ground_temperature",PT_double,&default_ground_temperature,PT_UNITS,"degC",NULL);
		gl_global_create("powerflow::warning_cable_temperature",PT_double,&warning_cable_temperature,PT_UNITS,"degC",NULL);
		gl_global_create("powerflow::alert_cable_temperature",PT_double,&alert_cable_temperature,PT_UNITS,"degC",NULL);
		gl_global_create("powerflow::ductbank_configuration_file",PT_char1024,&ductbank_configuration_file,NULL);
	}
}

int ductbank::create(void) 
{
	return 1; /* return 1 on success, 0 on failure */
}

int ductbank::init(OBJECT *parent)
{
	cable_temperature = air_temperature = ground_temperature = default_ground_temperature;

	load_data(configuration);
#define CHECK_POSITIVE(X) if ( X <= 0 ) { error(#X " must be positive"); }
	CHECK_POSITIVE(section_area)
	CHECK_POSITIVE(duct_R)
	CHECK_POSITIVE(airgap_R)
	CHECK_POSITIVE(insulation_R)

	air_area = section_area;

	return 1;
}

void ductbank::load_data(const char *name)
{
	if ( ductbank_data == NULL )
	{
		char pathname[1024];
		if ( ! gl_findfile(ductbank_configuration_file,NULL,R_OK,pathname,sizeof(pathname)-1) )
		{
			strcpy(pathname,ductbank_configuration_file);
		}
		FILE *fp = fopen(pathname,"rt");
		char header[1024] = "";
		int lineno = 0;
		while ( fp != NULL && ! feof(fp) && ! ferror(fp) )
		{
			char item[256];
			double A=0, RI=0, RA=0, RD=0;
			if ( lineno == 0 )
			{
				if ( fgets(header,sizeof(header)-1,fp) == NULL )
				{
					break;
				}

				if ( strcmp(header,"name,section_area,insulation_R,airgap_R,duct_R\n") != 0 )
				{
					error("invalid header in ductbank configuration file '%s' (%s)", pathname, header);
				}
				else
				{
					lineno = 1;
				}
			}
			lineno++;
			if ( fscanf(fp,"%[^,],%lf,%lf,%lf,%lf",item,&A,&RI,&RA,&RD) != 5 )
			{
				error("%s@%d: invalid header (%s)", pathname, lineno, header);			
			}
			DUCTBANK_DATA *data = new DUCTBANK_DATA;
			data->name = strdup(item);
			data->A = A;
			data->R[0] = RI;
			data->R[1] = RA;
			data->R[2] = RD;
			data->next = ductbank_data;
			ductbank_data = data;
		}
		if ( fp == NULL )
		{
			error("unable to open ductbank configuration file '%s' (%s)", pathname, strerror(errno));
		}
		else if ( ferror(fp) )
		{
			error("unable to read ductbank configuration file '%s' (%s)", pathname, strerror(errno));
		}
	}

	DUCTBANK_DATA *data = NULL;
	for ( data = ductbank_data ; data != NULL ; data = data->next )
	{
		if ( strcmp(data->name,name) == 0 )
		{
			break;
		}
	}
	if ( data == NULL )
	{
		error("ductbank configuration '%s' not found",name);		
	}
	if ( section_area == 0 ) section_area = data->A;
	if ( duct_R == 0 ) duct_R = data->R[0];
	if ( airgap_R == 0 ) airgap_R = data->R[1];
	if ( insulation_R == 0 ) insulation_R = data->R[2];
}

TIMESTAMP ductbank::precommit(TIMESTAMP t1)
{
	heatgain = 140000.0/2000.0;
	return TS_NEVER;
}

void ductbank::add_cable(double diameter)
{
	double area = diameter*diameter*PI/4.0;
	if ( area > section_area )
	{
		error("cannot fit anymore cables");
	}
	else
	{
		section_area -= area;
		double d = sqrt(section_area)/2; // mean distance from cable to shell
		double u = 0.02572; // W/m.K air conductivity
		airgap_R = d/u;
		// fprintf(stderr,"adding cable: cable area = %.4lg m^2, duct area = %.4lg m^2, airgap_R = %.04lg\n",area,section_area,airgap_R);
	}
}

void ductbank::add_heatgain(double losses, double length)
{
	heatgain += length>0 ? losses / length : losses;
}

TIMESTAMP ductbank::commit(TIMESTAMP t1, TIMESTAMP t2)
{
	duct_temperature = ground_temperature + heatgain*duct_R;
	air_temperature = duct_temperature + heatgain*airgap_R;
	cable_temperature = air_temperature + heatgain*insulation_R;
	if ( cable_temperature > alert_cable_temperature )
	{
		cable_status = CS_ALERT;
		warning("cable temperature alert (%.0lf degC)",cable_temperature);
	}
	else if ( cable_temperature > warning_cable_temperature )
	{
		cable_status = CS_WARNING;
		warning("cable temperature warning (%.0lf degC)",cable_temperature);
	}
	else
	{
		cable_status = CS_OK;
	}
	return TS_NEVER;
}
