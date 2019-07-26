// $Id: impedance_dump.h 1182 2008-12-22 22:08:36Z dchassin $
//	Copyright (C) 2008 Battelle Memorial Institute

#ifndef _impedance_dump_H
#define _impedance_dump_H

#ifndef _POWERFLOW_H
#error "this header must be included by powerflow.h"
#endif

typedef enum {
	IDM_RECT,
	IDM_POLAR
} IDMODE;

class impedance_dump : public gld_object
{
public:
	int first_run;
	char32 group;
	char256 filename;
	link_object **pFuse;
	line **pOhLine;
	link_object **pRecloser;
	regulator **pRegulator;
	link_object **pRelay;
	link_object **pSectionalizer;
	link_object **pSeriesReactor;
	switch_object **pSwitch;
	transformer **pTransformer;
	line **pTpLine;
	line **pUgLine;
	capacitor **pCapacitor;
	TIMESTAMP runtime;
	int32 runcount;
	complex *node_voltage;
public:
	static CLASS *oclass;
public:
	impedance_dump(MODULE *mod);
	int create(void);
	int init(OBJECT *parent);
	TIMESTAMP commit(TIMESTAMP t);
	int isa(CLASSNAME classname);
	int dump(TIMESTAMP t);
	complex *get_complex(OBJECT *obj, const char *name);
};

#endif // _impedance_dump_H

