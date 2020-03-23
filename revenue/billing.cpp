// File: billing.cpp
// Copyright (C) 2020 Regents of the Leland Stanford Junior University

#include "billing.h"

EXPORT_CREATE(billing);
EXPORT_INIT(billing);
EXPORT_PRECOMMIT(billing);
EXPORT_COMMIT(billing);
EXPORT_SYNC(billing);
EXPORT_NOTIFY(billing);

CLASS *billing::oclass = NULL;
billing *billing::defaults = NULL;

#define CLASSOPTIONS PC_AUTOLOCK

billing::billing(MODULE *module)
{
	if (oclass==NULL)
	{
		// register to receive notice for first top down. bottom up, and second top down synchronizations
		oclass = gld_class::create(module,"billing",sizeof(billing),CLASSOPTIONS);
		if (oclass==NULL)
			throw "unable to register class billing";
		else
			oclass->trl = TRL_PROVEN;

		defaults = this;
		if (gl_publish_variable(oclass,
			// TODO: add properties
			PT_object,"tariff",get_tariff_offset(),
				PT_DESCRIPTION,"reference to the tariff object used for this bill",
			PT_object,"meter",get_meter_offset(),
				PT_DESCRIPTION,"reference to the meter object use for this bill",
			PT_int32,"bill_day",get_bill_day_offset(),
				PT_DESCRIPTION,"day of month when the bill is generated",
			PT_timestamp,"bill_date",get_bill_date_offset(),
				PT_DESCRIPTION,"date of the last bill generated",
			PT_int32,"billing_days",get_billing_days_offset(),
				PT_DESCRIPTION,"number of day of last bill",
			PT_double,"total_bill[$]",get_total_bill_offset(),
				PT_DESCRIPTION,"last bill total",
			PT_double,"total_charges[$]",get_total_charges_offset(),
				PT_DESCRIPTION,"last bill total charges",
			PT_double,"energy_charges[$]",get_energy_charges_offset(),
				PT_DESCRIPTION,"last bill energy charges",
			PT_double,"capacity_charges[$]",get_capacity_charges_offset(),
				PT_DESCRIPTION,"last bill capacity charges",
			PT_double,"ramping_charges[$]",get_ramping_charges_offset(),
				PT_DESCRIPTION,"last bill ramping charges",
			PT_double,"fixed_charges[$]",get_fixed_charges_offset(),
				PT_DESCRIPTION,"last bill fixed charges",
			PT_double,"total_credits[$]",get_total_credits_offset(),
				PT_DESCRIPTION,"last bill total credits",
			PT_double,"energy_credits[$]",get_energy_credits_offset(),
				PT_DESCRIPTION,"last bill energy credits",
			PT_double,"capacity_credits[$]",get_capacity_credits_offset(),
				PT_DESCRIPTION,"last bill capacity credits",
			PT_double,"ramping_credits[$]",get_ramping_charges_offset(),
				PT_DESCRIPTION,"last bill ramping credits",
			PT_double,"fixed_credits[$]",get_fixed_credits_offset(),
				PT_DESCRIPTION,"last bill fixed credits",
			NULL)<1){
				char msg[256];
				sprintf(msg, "unable to publish properties in %s",__FILE__);
				throw msg;
		}
	}
}

int billing::create(void) 
{
	// TODO: create object
	return 1; /* return 1 on success, 0 on failure */
}

int billing::init(OBJECT *parent)
{
	if ( bill_day == 0 )
	{
		bill_day = gl_random_uniform(RNGSTATE,1,28);
	}
	else if ( bill_day > 28 || bill_day < -28 )
	{
		exception("bill_day must be between -28 and 28, inclusive");
	}

	return 1; /* return 2 on deferral, 1 on success, 0 on failure */
}

TIMESTAMP billing::precommit(TIMESTAMP t0)
{
	TIMESTAMP t2 = TS_NEVER;
	// TODO: precommit event
	return t2;
}

TIMESTAMP billing::presync(TIMESTAMP t0)
{
	TIMESTAMP t2 = TS_NEVER;
	// TODO: presync event
	return t2;
}

TIMESTAMP billing::sync(TIMESTAMP t0)
{
	TIMESTAMP t2 = TS_NEVER;
	// TODO: sync event
	return t2;
}

TIMESTAMP billing::postsync(TIMESTAMP t0)
{
	TIMESTAMP t2 = TS_NEVER;
	// TODO: postsync event
	return t2;
}

TIMESTAMP billing::commit(TIMESTAMP t0, TIMESTAMP t1)
{
	TIMESTAMP t2 = TS_NEVER;
	// TODO: commit event
	return t2;
}

TIMESTAMP billing::finalize(TIMESTAMP t0, TIMESTAMP t1)
{
	TIMESTAMP t2 = TS_NEVER;
	// TODO: finalize event
	return t2;
}

int billing::prenotify(PROPERTY *prop, const char *value)
{
	// TODO: handle changing value
	return 1;
}

int billing::postnotify(PROPERTY *prop, const char *value)
{
	// TODO: handle changed value
	return 1;
}

/** @} **/
