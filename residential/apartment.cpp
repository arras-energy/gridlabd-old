/*	File: apartment.cpp

	Copyright (C) 2020, Regents of the Leland Stanford Junior University
*/

#include "residential.h"

using namespace arma;

EXPORT_CREATE(apartment);
EXPORT_INIT(apartment);
EXPORT_PRECOMMIT(apartment);

CLASS *apartment::oclass = NULL;
apartment *apartment::defaults = NULL;

char1024 apartment::load_property = "base_power";
double apartment::maximum_temperature_update = 0.1;

#define PASSCONFIG PC_AUTOLOCK

apartment::apartment(MODULE *module)
{
	if (oclass==NULL)
	{
		// register to receive notice for first top down. bottom up, and second top down synchronizations
		oclass = gld_class::create(module,"apartment",sizeof(apartment),PASSCONFIG);
		if (oclass==NULL)
			throw "unable to register class apartment";
		else
			oclass->trl = TRL_PROVEN;

		defaults = this;
		if (gl_publish_variable(oclass,
			PT_set,"unit_appliance_types",get_unit_appliance_types_offset(), PT_DEFAULT,0, PT_DESCRIPTION,"appliances included in the apartment units",
				PT_KEYWORD,"NONE",(set)UAT_NONE,
				PT_KEYWORD,"DISHWASHER",(enumeration)UAT_DISHWASHER,
				PT_KEYWORD,"REFRIGERATOR",(enumeration)UAT_REFRIGERATOR,
				PT_KEYWORD,"WASHER",(enumeration)UAT_WASHER,
				PT_KEYWORD,"DRYER",(enumeration)UAT_DRYER,
			NULL)<1){
				char msg[256];
				sprintf(msg, "unable to publish properties in %s",__FILE__);
				throw msg;
        gl_global_create("residential::load_property",PT_char1024,&load_property,NULL);
        gl_global_create("residential::maximum_temperature_update",PT_double,&maximum_temperature_update,PT_UNITS,"degF",NULL);
		}
	}
}

int apartment::create(void) 
{
	return 1; /* return 1 on success, 0 on failure */
}

int apartment::init(OBJECT *parent)
{
	A  << -(U_OA+U_AU+U_AC+U_AM)/C_A << U_AU/C_A << U_AC/C_A << U_AM/C_A << endr
	   << U_AU/C_U << -(U_OU+U_AU+U_UC+U_UM)/C_U << U_UC/C_U << U_UM/C_U << endr
	   << U_AC/C_C << U_UC/C_C << -(U_OC+U_AC+U_UC+U_CM)/C_C << U_CM/C_C << endr
	   << U_AM/C_M << U_UM/C_M << U_CM/C_M << -(U_OM+U_AM+U_UM+U_CM)/C_M << endr;
	B1 << U_OA/C_A << 1/C_A << 1/C_A << 1/C_A << 0 << 0 << 0 << endr
	   << U_OU/C_U << 0 << 0 << 0 << 1/C_U << 0 << 0 << endr
	   << U_OC/C_C << 0 << 0 << 0 << 0 << 1/C_C << 1/C_C << endr
	   << U_OM/C_M << 0 << 0 << 0 << 0 << 0 << 0 << endr;
	B2 << 1/C_A << 0 << 0 << endr
	   << 0 << 1/C_U << 0 << endr
	   << 0 << 0 << 1/C_C << endr
	   << 0 << 0 << 0 << endr;
	try
	{
		Ainv = inv(A);
		B1inv = inv(B1);
		B2inv = inv(B2);
	}
	catch (...)
	{
		error("unable to invert matrices");
		return 0;
	}
	T << 0 << endr
	  << 0 << endr
	  << 0 << endr
	  << 0 << endr;
	return 1;
}

TIMESTAMP apartment::precommit(TIMESTAMP t1)
{
	double dt = t1 - gl_globalclock;

	// get the loads
	q << Tout << endr
	  << Q_AS + Q_AV + Q_AE << endr
	  << Q_US << endr
	  << Q_CS + Q_CV << endr;

	// compute the zone balance temperatures
	Tbal = -Ainv*B1*q;
	Tbal[3] = 0;
	m = sign(Tbal-Tout);

	// determine then

	u = -B2inv * (A*T+B1*q);
	for ( unsigned int i = 0 ; i < u.n_elem ; i++ )
	{
		if ( u_min[i] > u_max[i] ) {continue;}
		if ( u[i] < u_min[i] ) {u[i] = u_min[i];}
		else if ( u[i] > u_max[i] ) {u[i] = u_max[i];}
	}
	dT = A*T + B1*q +B2*u;
	T += dT*dt;
	return t1 + 3600*0.1/abs(dT.max()); // 0.1 degF change max
}

