// line_sensor.h
// Copyright (C) 2016, Stanford University
// Author: David P. Chassin (dchassin@slac.stanford.edu)

#ifndef _SWITCHCOORDINATOR_H
#define _SWITCHCOORDINATOR_H

#include "gismo.h"

DECL_METHOD(switch_coordinator,connect);
DECL_METHOD(switch_coordinator,arm);
DECL_METHOD(switch_coordinator,disarm);

class switch_coordinator : public gld_object {
public:
	enum {SCS_IDLE=0, SCS_ARMED=1, SCS_TOGGLE=2, SCS_DIRECT=3} SWITCHCOORDINATIONSTATUS;
public:
	GL_ATOMIC(enumeration,status);
	GL_ATOMIC(set,armed);
	GL_METHOD(switch_coordinator,connect);
	GL_METHOD(switch_coordinator,arm);
	GL_METHOD(switch_coordinator,disarm);
private:
	unsigned int n_switches;
	unsigned int64 states;
	gld_property *index[64];
public:
	/* required implementations */
	switch_coordinator(MODULE *module);
	int create(void);
	int init(OBJECT *parent);
	int precommit(TIMESTAMP t1);
	TIMESTAMP sync(TIMESTAMP t1);
	inline TIMESTAMP postsync(TIMESTAMP t1) { return TS_NEVER; };
	inline TIMESTAMP presync(TIMESTAMP t1) { return TS_NEVER; };
	TIMESTAMP commit(TIMESTAMP t1, TIMESTAMP t2);
	inline int prenotify(PROPERTY *prop, const char *value=NULL) { return 1; };
	int postnotify(PROPERTY *prop, const char *value=NULL);

public:
	static CLASS *oclass;
	static switch_coordinator *defaults;
};

#endif // _LINESENSOR_H
