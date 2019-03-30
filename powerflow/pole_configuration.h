// powerflow/pole_configuration.h
// Copyright (C) 2018, Stanford University

#ifndef _POLECONFIGURATION_H
#define _POLECONFIGURATION_H

#include "powerflow_library.h"

class pole_configuration : public powerflow_library
{
public:
	static CLASS *oclass;
	static CLASS *pclass;
	typedef enum {
		CIZ_NONE = 0,
		CIZ_LOW = 1,
		CIZ_MODERATE = 2,
		CIZ_INTERMEDIATE = 3,
		CIZ_HIGH = 4,
		CIZ_EXTREME = 5,
		_CIZ_SIZE,
	} CLIMATEIMPACTZONE;
	static KEYWORD kw_ciz[_CIZ_SIZE];
	static enumeration climate_impact_zone;
public:
	enum {PT_WOOD=0, PT_STEEL=1, PT_CONCRETE=2};
	enumeration pole_type;
	double repair_time;
public: // wood pole model parameters
	double design_ice_thickness; 		// (see Chart 1)
	double design_wind_loading; 		// (see Chart 1)
	double design_temperature; 		// (see Chart 1)
	// overload factors (see Chart 2)
	double overload_factor_vertical; 	
	double overload_factor_transverse_general; 	
	double overload_factor_transverse_crossing; 	
	double overload_factor_transverse_wire; 	
	double overload_factor_longitudinal_general; 	
	double overload_factor_longitudinal_deadend; 	
	// strength factors (see Chart 3)
	double strength_factor_250b_wood; 	
	double strength_factor_250b_support; 	
	double strength_factor_250c_wood; 	
	double strength_factor_250c_support; 	
	double cable_diameter;		// (see Section F)
	double ice_thickness;		// (see Section F)
	double pole_length; 		// (see Chart 4)
	double pole_depth;		// (see Section A)
	double ground_diameter; 	// (see Chart 4)
	double top_diameter; 		// (see Chart 4)
	double fiber_strength; 		// (see Chart 5)
	double wire_height_A;
	double wire_height_B;
	double wire_height_C;
	double wire_height_N;
	double wire_height_P;
	double wire_height_T;
	double degradation_rate;
	typedef enum {
		PTM_NONE = 0,
		PTM_CREOSOTE = 1,
		PTM_PENTA = 2,
		PTM_CCA = 3,
		_PTM_SIZE,
	} POLETREATMENTMETHOD;
	enumeration treatment_method;
public:
	pole_configuration(MODULE *mod);
	inline pole_configuration(CLASS *cl=oclass) : powerflow_library(cl) {};
	double get_pole_diameter(double);
	int isa(CLASSNAME classname);
	int create(void);
	int init(OBJECT *obj);

};

#endif // _POLECONFIGURATION_H