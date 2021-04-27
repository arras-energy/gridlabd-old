// $Id: underground_line.h 1182 2008-12-22 22:08:36Z dchassin $
//	Copyright (C) 2008 Battelle Memorial Institute

#ifndef _UNDERGROUNDLINE_H
#define _UNDERGROUNDLINE_H

#ifndef _POWERFLOW_H
#error "this header must be included by powerflow.h"
#endif

class underground_line : public line
{
public:
    static CLASS *oclass;
    static CLASS *pclass;

public:
	int init(OBJECT *parent);
	void recalc(void);
	underground_line(MODULE *mod);
	inline underground_line(CLASS *cl=oclass):line(cl){};
	int isa(CLASSNAME classname);
	int create(void);
private:
	void test_phases(line_configuration *config, const char ph);
};

EXPORT int create_fault_ugline(OBJECT *thisobj, OBJECT **protect_obj, const char *fault_type, int *implemented_fault, TIMESTAMP *repair_time, void *Extra_Data);
EXPORT int fix_fault_ugline(OBJECT *thisobj, int *implemented_fault, char *imp_fault_name, void* Extra_Data);
EXPORT int recalc_underground_line(OBJECT *obj);
#endif // _UNDERGROUNDLINE_H
