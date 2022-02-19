// module/resilience/metrics.h
// Copyright (C) 2022 Regents of the Leland Stanford Junior University

#ifndef _GRIDLABD_MODULE_RESILIENCE_METRICS_H
#define _GRIDLABD_MODULE_RESILIENCE_METRICS_H

#include "gridlabd.h"

#define MRF_ANNUALLY 0
#define MRF_SEASONALLY 1
#define MRF_MONTHLY 2
#define MRF_WEEKLY 3
#define MRF_DAILY 4

class metrics : public gld_object 
{

private:

	GL_ATOMIC(double,outage); 
	GL_ATOMIC(double,cost);		
	GL_ATOMIC(int32,impact);
	GL_STRING(char1024,report_file);
	GL_ATOMIC(enumeration,report_frequency);

private:

	FILE *report_fh;
	TIMESTAMP last_report;
	void update_report(bool final=false);
	void reset_metrics(void);

public:
	
	void report_impact(int n);
	void report_cost(double x);
	void report_outage(double t);

public:

	metrics(MODULE *module);
	int create(void);
	int init(OBJECT *parent);
	TIMESTAMP commit(TIMESTAMP t1, TIMESTAMP t2);
	int finalize(void);

public:

	static CLASS *oclass;
	static metrics *defaults;

};

#endif // _GRIDLABD_MODULE_RESILIENCE_METRICS_H
