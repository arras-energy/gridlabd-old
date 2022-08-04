// File: tariff.cpp
// Copyright (C) 2020 Regents of the Leland Stanford Junior University

#include "tariff.h"

EXPORT_CREATE(tariff);
EXPORT_INIT(tariff);
EXPORT_PRECOMMIT(tariff);
EXPORT_COMMIT(tariff);
EXPORT_SYNC(tariff);
EXPORT_NOTIFY(tariff);

CLASS *tariff::oclass = NULL;
tariff *tariff::defaults = NULL;

#define CLASSOPTIONS PC_AUTOLOCK

tariff::tariff(MODULE *module)
{
	if (oclass==NULL)
	{
		// register to receive notice for first top down. bottom up, and second top down synchronizations
		oclass = gld_class::create(module,"tariff",sizeof(tariff),CLASSOPTIONS);
		if (oclass==NULL)
			throw "unable to register class tariff";
		else
			oclass->trl = TRL_PROVEN;

		defaults = this;
		if (gl_publish_variable(oclass,
			PT_int64, "rate_id", get_rate_id(),
				PT_REQUIRED,
				PT_DESCRIPTION, "OpenEI rate id",

			NULL)<1){
				char msg[256];
				sprintf(msg, "unable to publish properties in %s",__FILE__);
				throw msg;
		}
	}
}

int tariff::create(void) 
{
	// TODO: create object
	return 1; /* return 1 on success, 0 on failure */
}

int tariff::init(OBJECT *parent)
{
	// TODO: initialize object
	return 1; /* return 2 on deferral, 1 on success, 0 on failure */
}

TIMESTAMP tariff::precommit(TIMESTAMP t0)
{
	TIMESTAMP t2 = TS_NEVER;
	// TODO: precommit event
	return t2;
}

TIMESTAMP tariff::presync(TIMESTAMP t0)
{
	TIMESTAMP t2 = TS_NEVER;
	// TODO: presync event
	return t2;
}

TIMESTAMP tariff::sync(TIMESTAMP t0)
{
	TIMESTAMP t2 = TS_NEVER;
	// TODO: sync event
	return t2;
}

TIMESTAMP tariff::postsync(TIMESTAMP t0)
{
	TIMESTAMP t2 = TS_NEVER;
	// TODO: postsync event
	return t2;
}

TIMESTAMP tariff::commit(TIMESTAMP t0, TIMESTAMP t1)
{
	TIMESTAMP t2 = TS_NEVER;
	// TODO: commit event
	return t2;
}

TIMESTAMP tariff::finalize(TIMESTAMP t0, TIMESTAMP t1)
{
	TIMESTAMP t2 = TS_NEVER;
	// TODO: finalize event
	return t2;
}

int tariff::prenotify(PROPERTY *prop, const char *value)
{
	// TODO: handle changing value
	return 1;
}

int tariff::postnotify(PROPERTY *prop, const char *value)
{
	// TODO: handle changed value
	return 1;
}

/** @} **/
