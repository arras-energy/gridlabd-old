// module/powerflow/ductbank.cpp
// Copyright (C) 2023 Regents of the Leland Stanford Junior University

#include "powerflow.h"

EXPORT_CREATE(ductbank);
EXPORT_INIT(ductbank);
EXPORT_COMMIT(ductbank);
EXPORT_PRECOMMIT(ductbank);

CLASS *ductbank::oclass = NULL;
ductbank *ductbank::defaults = NULL;
double ductbank::default_ground_temperature = 25; // degC
double ductbank::warning_cable_temperature = 80; // degC
double ductbank::alert_cable_temperature = 90; // degC
char1024 ductbank::ductbank_configuration_file = "ductbank_configurations.csv";
DUCTBANK_DATA *ductbank::ductbank_data = NULL;
double ductbank::air_effective_conductivity = 0.20; // W/K.m

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

			PT_char256, "temperature_source", get_temperature_source_offset(),
				PT_DESCRIPTION, "source for surface temperature",

			PT_double, "surface_temperature[degC]", get_ground_temperature_offset(),
				PT_DESCRIPTION, "ground temperature at the surface of fill",

			PT_int32, "channels", get_channels_offset(),
				PT_DESCRIPTION, "number of channels in duct",

			PT_double, "fill_R[K/W]", get_fill_R_offset(),
				PT_DESCRIPTION, "fill soil thermal resistance",

			PT_double, "duct_R[K/W]", get_duct_R_offset(),
				PT_DESCRIPTION, "duct thermal resistance",

			PT_double, "air_R[K/W]", get_airgap_R_offset(),
				PT_DESCRIPTION, "air gap thermal resistance",

			PT_double, "insulation_R[K/W]", get_insulation_R_offset(),
				PT_DESCRIPTION, "cable insulation thermal",

			PT_double, "duct_area[m^2]", get_duct_area_offset(),
				PT_DESCRIPTION, "cross sectional area of interior air space",

			PT_double, "heatgain[W/m]", get_heatgain_offset(),
				PT_OUTPUT,
				PT_DESCRIPTION, "cable heat per unit length",

			PT_double, "cable_temperature[degC]", get_cable_temperature_offset(),
				PT_OUTPUT,
				PT_DESCRIPTION, "cable temperature in duct",

			PT_double, "peak_temperature[degC]", get_peak_temperature_offset(),
				PT_OUTPUT,
				PT_DESCRIPTION, "peak cable temperature in duct",

			PT_enumeration, "cable_status", get_cable_status_offset(),
				PT_OUTPUT,
				PT_KEYWORD, "OK", (enumeration)CS_OK,
				PT_KEYWORD, "WARNING", (enumeration)CS_WARNING,
				PT_KEYWORD, "ALERT", (enumeration)CS_ALERT,
				PT_KEYWORD, "UNKNOWN", (enumeration)CS_UNKNOWN,
				PT_DESCRIPTION, "cable status",

			NULL)<1)
		{
				throw "unable to publish properties in " __FILE__;
		}

		gl_global_create("powerflow::ground_temperature",PT_double,&default_ground_temperature,PT_UNITS,"degC",NULL);
		gl_global_create("powerflow::warning_cable_temperature",PT_double,&warning_cable_temperature,PT_UNITS,"degC",NULL);
		gl_global_create("powerflow::alert_cable_temperature",PT_double,&alert_cable_temperature,PT_UNITS,"degC",NULL);
		gl_global_create("powerflow::ductbank_configuration_file",PT_char1024,&ductbank_configuration_file,NULL);
		gl_global_create("powerflow::air_effective_conductivity",PT_double,&air_effective_conductivity,PT_UNITS,"W/K*m",NULL);
	}
}

int ductbank::create(void) 
{
	cable_area = air_area = 0.0;
	peak_temperature = cable_temperature = ground_temperature = default_ground_temperature;
	return 1; /* return 1 on success, 0 on failure */
}

int ductbank::init(OBJECT *parent)
{
	load_data(configuration);
#define CHECK_POSITIVE(X) if ( X <= 0 ) { error(#X " must be positive"); }
	CHECK_POSITIVE(duct_area)
	CHECK_POSITIVE(fill_R)
	CHECK_POSITIVE(duct_R)
	CHECK_POSITIVE(channels)

	if ( strcmp(temperature_source,"") != 0 )
	{
		temperature = new gld_property(temperature_source);
		if ( temperature == NULL || ! temperature->is_valid() )
		{
			error("temperature source '%s' is not valid",(const char*)temperature_source);
		}
	}

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
			double D; // duct inner diameter
			double H, V; // duct horizontal and vertical spacing
			double T, S, B; // duct top, side, and bottom spacing
			double G; // soil depth to duct
			double ROHg, ROHf, Rd; // ground, fill, duct thermal resistance
			unsigned int N;
			if ( lineno == 0 )
			{
				if ( fgets(header,sizeof(header)-1,fp) == NULL )
				{
					break;
				}
				if ( strcmp(header,"name,duct_diameter[cm],horizontal_spacing[cm],vertical_spacing[cm],top_spacing[cm],side_spacing[cm],bottom_spacing[cm],soil_depth[cm],ROH_ground,ROH_fill,R_duct
\n") != 0 )
				{
					error("invalid header in ductbank configuration file '%s' (%s)", pathname, header);
				}
				else
				{
					lineno = 1;
				}
			}
			lineno++;
			// name,duct_diameter[cm],horizontal_spacing[cm],vertical_spacing[cm],top_spacing[cm],side_spacing[cm],bottom_spacing[cm],soil_depth[cm],ROH_ground,ROH_fill,R_duct
			int len = fscanf(fp,"%[^,],%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",
				item,&D,&H,&V,&T,&S,&B,&G,&ROHg,&ROHf,&Rd);
			if ( len == 0 || item[0] == '#' || item[0] == '\n' )
			{
				continue;
			}
			else if ( len != 10 )
			{
				error("%s@%d: invalid/missing data ('%s...')", pathname, lineno, item);			
			}
			DUCTBANK_DATA *data = new DUCTBANK_DATA;
			data->name = strdup(item);
			data->D = D;
			data->H = H;
			data->V = V;
			data->T = T;
			data->S = S;
			data->B = B;
			data->G = G;
			data->ROHg = ROHg;
			data->ROHf = ROHf;
			data->Rd = Rd;
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
	if ( duct_area == 0 && data->A > 0 ) duct_area = data->A;
	if ( fill_R == 0 && data->RF > 0 ) fill_R = data->RF;
	if ( duct_R == 0 && data->RD > 0 ) duct_R = data->RD;
	if ( channels == 0 && data->N > 0 ) channels = data->N;
}

TIMESTAMP ductbank::precommit(TIMESTAMP t1)
{
	heatgain = 0;
	if ( air_area == 0 )
	{
		if ( cable_area > duct_area )
		{
			error("cannot fit anymore cables (cable=%.2lg m^2, open=%.2lg m^2, total=%.2lg)",cable_area,air_area,duct_area);
		}
		else
		{
			air_area = duct_area - cable_area;
			double d = sqrt(air_area/channels)/2; // mean distance from cable to duct
			airgap_R = 1/((airgap_R>0?1/airgap_R:0) + air_effective_conductivity/d);
		}
	}
	return TS_NEVER;
}

void ductbank::add_cable(int ductid, double diameter, double R_value)
{
	cable_area += diameter*diameter*PI/4;
	if ( R_value > 0 )
	{
		insulation_R = 1/((insulation_R>0?1/insulation_R:0)+1/R_value);
	}
}

void ductbank::add_heatgain(double losses, double cable_length)
{
	heatgain += cable_length>0 ? losses / cable_length : losses;
}

TIMESTAMP ductbank::commit(TIMESTAMP t1, TIMESTAMP t2)
{
	if ( temperature )
	{
		ground_temperature = temperature->get_double("degC"); // convert to degC
	}
	double total_R = fill_R + duct_R + airgap_R + insulation_R;
	if ( total_R > 0 )
	{	
		cable_temperature = ground_temperature + heatgain / total_R;
		if ( cable_temperature > peak_temperature )
		{
			peak_temperature = cable_temperature;
		}
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
	}
	else
	{
		cable_status = CS_UNKNOWN;
	}
	return TS_NEVER;
}
