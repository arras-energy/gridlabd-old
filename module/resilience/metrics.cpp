// module/resilience/metrics.cpp
// Copyright (C) 2022 Regents of the Leland Stanford Junior University

#include "metrics.h"

EXPORT_CREATE(metrics)
EXPORT_INIT(metrics)
EXPORT_COMMIT(metrics)
EXPORT_FINALIZE(metrics)

CLASS *metrics::oclass = NULL;
metrics *metrics::defaults = NULL;

metrics::metrics(MODULE *module)
{
	if (oclass==NULL)
	{
		// register to receive notice for first top down. bottom up, and second top down synchronizations
		oclass = gld_class::create(module,"metrics",sizeof(metrics),PC_AUTOLOCK|PC_OBSERVER);
		if (oclass==NULL)
			throw "unable to register class metrics";
		else
			oclass->trl = TRL_PROVEN;

		defaults = this;
		if (gl_publish_variable(oclass,

			PT_double, "outage[unit*day]", get_outage_offset(),
				PT_DESCRIPTION, "total number of customer outage days",

			PT_double, "cost[$]", get_cost_offset(),
				PT_DESCRIPTION, "total repair cost",

			PT_int32, "impact", get_impact_offset(),
				PT_DESCRIPTION, "total emergency services impacted more than 48 hours per event",

			PT_char1024, "report_file", get_report_file_offset(),
				PT_DESCRIPTION, "file in which metric data is recorded",

			PT_enumeration, "report_frequency", get_report_frequency_offset(),
				PT_KEYWORD, "DAILY", (enumeration)MRF_DAILY,
				PT_KEYWORD, "WEEKLY", (enumeration)MRF_WEEKLY,
				PT_KEYWORD, "MONTHLY", (enumeration)MRF_MONTHLY,
				PT_KEYWORD, "SEASONALLY", (enumeration)MRF_SEASONALLY,
				PT_KEYWORD, "ANNUALLY", (enumeration)MRF_ANNUALLY,
				PT_DEFAULT, "ANNUALLY",
				PT_DESCRIPTION, "frequency with which metric data is recorded",
			
			NULL)<1){
				char msg[256];
				sprintf(msg, "unable to publish properties in %s",__FILE__);
				throw msg;
		}
	}
}

int metrics::create(void) 
{
	outage = 0.0;
	cost = 0.0;
	impact = 0;
	memset(report_file,0,sizeof(report_file));
	report_fh = NULL;
	return 1; /* return 1 on success, 0 on failure */
}

int metrics::init(OBJECT *parent)
{

	if ( strlen(report_file) > 0 )
	{
		report_fh = fopen(report_file,"w");
		if ( report_fh == NULL )
		{
			warning("unable to open '%s' for write access",(const char*)report_file);
		}
		else
		{
			fprintf(report_fh,"%s","timestamp,outage[unit.day],cost[$],impact\n");
		}
	}
	return 1;
}

TIMESTAMP metrics::commit(TIMESTAMP t1, TIMESTAMP t2)
{
	DATETIME dt;
	gl_localtime(gl_globalclock, &dt);
	if (dt.hour!=0 && dt.minute!=0 && dt.second!=0)
	{
		return TS_NEVER;
	}
	bool need_update = false;
	TIMESTAMP t3 = TS_NEVER;
	switch ( report_frequency )
	{
	case MRF_DAILY:
		need_update = true;
		t3 = t2 + 86400;
		break;
	case MRF_WEEKLY:
		need_update = (dt.weekday==0);
		t3 = t2 + 86400;
		break;
	case MRF_MONTHLY:
		need_update = (dt.day==0);
		t3 = t2 + 86400;
		break;
	case MRF_SEASONALLY:
		need_update = (dt.month%3==0 && dt.weekday==0);
		t3 = t2 + 86400;
		break;
	case MRF_ANNUALLY:
		need_update = (dt.month==0);
		t3 = t2 + 86400;
		break;
	default:
		break;
	}
	if ( need_update )
	{
		update_report();
	}
	return t3;
}

int metrics::finalize(void)
{
	update_report();
	return 1;
}

void metrics::update_report(bool final)
{
	if ( report_fh )
	{
		char buffer[64];
		if ( gl_strftime(gl_globalclock,buffer,sizeof(buffer)-1) )
		{
			fprintf(report_fh,"%s,%.1f,%.2f,%d\n",buffer,outage,cost,impact);
		}
		else
		{
			fprintf(report_fh,"%lld,%.1f,%.2f,%d\n",gl_globalclock,outage,cost,impact);
		}
		if ( final )
		{
			fclose(report_fh);
			report_fh = NULL;
		}
	}
}