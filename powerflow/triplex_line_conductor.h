// $Id: triplex_line_conductor.h 4738 2014-07-03 00:55:39Z dchassin $
//	Copyright (C) 2008 Battelle Memorial Institute

#ifndef _TRIPLEXLINECONDUCTOR_H
#define _TRIPLEXLINECONDUCTOR_H

#ifndef _POWERFLOW_H
#error "this header must be included by powerflow.h"
#endif

class triplex_line_conductor : public powerflow_library
{
public:
	double resistance;
	double geometric_mean_radius;
	LINERATINGS winter, summer;
public:
	static CLASS *oclass;
	static CLASS *pclass;
	
	triplex_line_conductor(MODULE *mod);
	inline triplex_line_conductor(CLASS *cl=oclass):powerflow_library(cl){};
	int isa(char *classname);
	int create(void);
};

#endif // _TRIPLEXLINECONDUCTOR_H
