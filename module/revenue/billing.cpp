// File: billing.cpp
// Copyright (C) 2020 Regents of the Leland Stanford Junior University

#include "billing.h"

EXPORT_CREATE(billing);
EXPORT_INIT(billing);
EXPORT_COMMIT(billing);

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
				PT_REQUIRED,
				PT_DESCRIPTION,"reference to the tariff object used for this bill",
			PT_object,"meter",get_meter_offset(),
				PT_REQUIRED,
				PT_DESCRIPTION,"reference to the meter object use for this bill",
			PT_int32,"bill_day",get_bill_day_offset(),
				PT_DESCRIPTION,"day of month when the bill is generated",
			PT_timestamp,"bill_date",get_bill_date_offset(),
				PT_OUTPUT,
				PT_DESCRIPTION,"date of the last bill generated",
			PT_int32,"billing_days",get_billing_days_offset(),
				PT_OUTPUT,
				PT_DESCRIPTION,"number of day of last bill",
			PT_double,"total_bill[$]",get_total_bill_offset(),
				PT_OUTPUT,
				PT_DESCRIPTION,"last bill total",
			PT_double,"total_charges[$]",get_total_charges_offset(),
				PT_OUTPUT,
				PT_DESCRIPTION,"last bill total charges",
			PT_double,"energy_charges[$]",get_energy_charges_offset(),
				PT_OUTPUT,
				PT_DESCRIPTION,"last bill energy charges",
			PT_double,"capacity_charges[$]",get_capacity_charges_offset(),
				PT_OUTPUT,
				PT_DESCRIPTION,"last bill capacity charges",
			PT_double,"ramping_charges[$]",get_ramping_charges_offset(),
				PT_OUTPUT,
				PT_DESCRIPTION,"last bill ramping charges",
			PT_double,"fixed_charges[$]",get_fixed_charges_offset(),
				PT_OUTPUT,
				PT_DESCRIPTION,"last bill fixed charges",
			PT_double,"total_credits[$]",get_total_credits_offset(),
				PT_OUTPUT,
				PT_DESCRIPTION,"last bill total credits",
			PT_double,"energy_credits[$]",get_energy_credits_offset(),
				PT_OUTPUT,
				PT_DESCRIPTION,"last bill energy credits",
			PT_double,"capacity_credits[$]",get_capacity_credits_offset(),
				PT_OUTPUT,
				PT_DESCRIPTION,"last bill capacity credits",
			PT_double,"ramping_credits[$]",get_ramping_charges_offset(),
				PT_OUTPUT,
				PT_DESCRIPTION,"last bill ramping credits",
			PT_double,"fixed_credits[$]",get_fixed_credits_offset(),
				PT_OUTPUT,
				PT_DESCRIPTION,"last bill fixed credits",
			PT_double,"metering_interval[s]",get_metering_interval_offset(),
				// TODO: PT_DEFAULT, "1 day",
				PT_DESCRIPTION,"interval at which meter is observed",
			PT_double,"baseline_demand[kWh/day]",get_baseline_demand_offset(),
				PT_DESCRIPTION,"daily demand of baseline tier",
			NULL)<1){
				char msg[256];
				sprintf(msg, "unable to publish properties in %s",__FILE__);
				throw msg;
		}
	}
}

int billing::create(void) 
{
	python_module = python_import(billing_module,billing_library);
	if ( python_module == NULL )
	{
		exception("unable to load python billing module %s from library %s",(const char*)billing_module,(const char*)billing_library);
		return 0;
	}
	python_data = PyDict_New();
	return 1; /* return 1 on success, 0 on failure */
}

int billing::init(OBJECT *parent)
{

	if ( bill_day == 0 )
	{
		bill_day = gl_random_uniform(RNGSTATE,1,31);
	}
	else if ( abs(bill_day) > 31 )
	{
		exception("bill_day must be between -31 and 31, inclusive");
	}

	if ( metering_interval == 0 )
	{
		metering_interval = 86400;
	}

	// need to call billing code once to initialize it
	compute_bill();

	return 1; /* return 2 on deferral, 1 on success, 0 on failure */
}

TIMESTAMP billing::commit(TIMESTAMP t0, TIMESTAMP t1)
{
	gld_clock dt0(t0);
	if ( is_billing_time(dt0) )
	{
		compute_bill();
		bill_date = gl_globalclock;
	}
	return (TIMESTAMP)(ceil(t0/metering_interval)*metering_interval);
}

bool billing::is_billing_time(gld_clock &dt0)
{
	int year = dt0.get_year();
	bool is_leapyear = false;
	if ( year % 4 == 0 ) is_leapyear = true;
	if ( year % 100 == 0 ) is_leapyear = false;
	if ( year % 400 == 0 ) is_leapyear = true;
	int days_in_month[12] = {31,is_leapyear?29:28,31,30,31,30,31,31,30,31,30,31};
	int effective_bill_day = bill_day;
	int month = dt0.get_month()-1;
	if ( bill_day > days_in_month[month] )
		effective_bill_day = days_in_month[month];
	return ( effective_bill_day == dt0.get_day() && dt0.get_hour() == 0 && dt0.get_minute() == 0 && dt0.get_second() == 0 );
}

void billing::compute_bill(void)
{
	if ( ! python_call(python_module,NULL,billing_function,"{sssisO}","classname",my()->oclass->name,"id",get_id(),"data",python_data) )
	{
		error("call to %s.%s() failed", (const char*)billing_module, (const char*)billing_function);
	}
	return;
}
