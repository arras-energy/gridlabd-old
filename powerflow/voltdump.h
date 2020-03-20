// $Id: voltdump.h 4738 2014-07-03 00:55:39Z dchassin $
//	Copyright (C) 2008 Battelle Memorial Institute

#ifndef _VOLTDUMP_H
#define _VOLTDUMP_H

#ifndef _POWERFLOW_H
#error "this header must be included by powerflow.h"
#endif

typedef enum {
	VDM_RECT,
	VDM_POLAR
} VDMODE;

class voltdump : public gld_object
{
public:
	TIMESTAMP runtime;
	char32 group;
	char256 filename;
	int32 runcount;
	int32 maxcount;
	enumeration mode;		///< dumps the voltages in either polar or rectangular notation
	char8 filemode;
	double interval;
	int32 version;
public:
	static CLASS *oclass;
public:
	voltdump(MODULE *mod);
	int create(void);
	int init(OBJECT *parent);
	TIMESTAMP commit(TIMESTAMP t);
	int isa(CLASSNAME classname);

	void dump(TIMESTAMP t);
};

#endif // _VOLTDUMP_H

