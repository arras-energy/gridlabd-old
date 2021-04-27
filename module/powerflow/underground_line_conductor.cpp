/** $Id: underground_line_conductor.cpp 1182 2008-12-22 22:08:36Z dchassin $
	Copyright (C) 2008 Battelle Memorial Institute
	@file underground_line_conductor.cpp
	@addtogroup underground_line_conductor 
	@ingroup line

	@{
**/

#include "powerflow.h"
using namespace std;

CLASS* underground_line_conductor::oclass = NULL;
CLASS* underground_line_conductor::pclass = NULL;

underground_line_conductor::underground_line_conductor(MODULE *mod) : powerflow_library(mod)
{
	if(oclass == NULL)
	{
		oclass = gl_register_class(mod,"underground_line_conductor",sizeof(underground_line_conductor),0x00);
		if (oclass==NULL)
			throw "unable to register class underground_line_conductor";
		else
			oclass->trl = TRL_PROVEN;

        if(gl_publish_variable(oclass,
			PT_double, "outer_diameter[in]",PADDR(outer_diameter),
				PT_REQUIRED,
				PT_DESCRIPTION,"Outer diameter of conductor and sheath",
			PT_double, "conductor_gmr[ft]", PADDR(conductor_gmr),
				PT_DESCRIPTION,"Geometric mean radius of the conductor",
			PT_double, "conductor_diameter[in]",PADDR(conductor_diameter),
				PT_REQUIRED,
				PT_DESCRIPTION,"Diameter of conductor",
			PT_double, "conductor_resistance[Ohm/mile]",PADDR(conductor_resistance),
				PT_DESCRIPTION,"Resistance of conductor in ohm/mile",
			PT_double, "neutral_gmr[ft]",PADDR(neutral_gmr),
				PT_DESCRIPTION,"Geometric mean radius of an individual neutral conductor/strand",
			PT_double, "neutral_diameter[in]",PADDR(neutral_diameter),
				PT_REQUIRED,
				PT_DESCRIPTION,"Diameter of individual neutral conductor/strand of concentric neutral",
			PT_double, "neutral_resistance[Ohm/mile]",PADDR(neutral_resistance),
				PT_DESCRIPTION,"Resistance of an individual neutral conductor/strand in ohm/mile",
			PT_int16,  "neutral_strands",PADDR(neutral_strands),
				PT_DESCRIPTION,"Number of cable strands in neutral conductor",
			PT_double, "shield_thickness[in]",PADDR(shield_thickness),
				PT_DESCRIPTION,"The thickness of Tape shield in inches",
			PT_double, "shield_diameter[in]",PADDR(shield_diameter),
				PT_DESCRIPTION,"The outside diameter of Tape shield in inches",
			PT_double, "insulation_relative_permitivitty[unit]", PADDR(insulation_rel_permitivitty),
				PT_DEFAULT, "1 unit",
				PT_DESCRIPTION, "Permitivitty of insulation, relative to air",
			PT_double, "shield_gmr[ft]",PADDR(shield_gmr),
				PT_DESCRIPTION,"Geometric mean radius of shielding sheath",
			PT_double, "shield_resistance[Ohm/mile]",PADDR(shield_resistance),
				PT_DESCRIPTION,"Resistance of shielding sheath in ohms/mile",
			PT_double, "rating.summer.continuous[A]", PADDR(summer.continuous),
				PT_DEFAULT, "1000 A",
				PT_DESCRIPTION,"amp rating in summer, continuous",
			PT_double, "rating.summer.emergency[A]", PADDR(summer.emergency),
				PT_DEFAULT, "2000 A",
				PT_DESCRIPTION,"amp rating in summer, short term",
			PT_double, "rating.winter.continuous[A]", PADDR(winter.continuous),
				PT_DEFAULT, "1000 A",
				PT_DESCRIPTION,"amp rating in winter, continuous",
			PT_double, "rating.winter.emergency[A]", PADDR(winter.emergency),
				PT_DEFAULT, "2000 A",
				PT_DESCRIPTION,"amp rating in winter, short term",
            NULL) < 1) GL_THROW("unable to publish underground_line_conductor properties in %s",__FILE__);
    }
}
int underground_line_conductor::create(void)
{
	int result = powerflow_library::create();
	return result;
}

int underground_line_conductor::init(OBJECT *parent)
{
	if (outer_diameter <= conductor_diameter)
	{
		GL_THROW("outer_diameter was specified as less than or equal to the conductor_diameter");
		/* TROUBLESHOOT
		The outer diameter is the diameter of the entire cable, and therefore should be the largest value. Please check your values
		and refer to Fig. 4.11 of "Distribution System Modeling and Analysis, Third Edition" by William H. Kersting for a diagram.
		*/
	}
	if (outer_diameter <= neutral_diameter)
	{
		GL_THROW("outer_diameter was specified as less than or equal to the neutral_diameter");
		/* TROUBLESHOOT
		The outer diameter is the diameter of the entire cable, and therefore should be the largest value. Please check your values
		and refer to Fig. 4.11 of "Distribution System Modeling and Analysis, Third Edition" by William H. Kersting for a diagram.
		*/
	}
	if (shield_diameter <= shield_thickness)
	{
		GL_THROW("shield_diameter was specified as less than or equal to the tapeshield_thickness");
		/* TROUBLESHOOT
		Refer to Example 5.4 in "Distribution System Modeling and Analysis, Third Edition" by William H. Kersting for a diagram.
		*/
	}
	return 1;
}

int underground_line_conductor::isa(char *classname)
{
	return strcmp(classname,"underground_line_conductor")==0;
}


//////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION OF CORE LINKAGE: underground_line_conductor
//////////////////////////////////////////////////////////////////////////

/**
* REQUIRED: allocate and initialize an object.
*
* @param obj a pointer to a pointer of the last object in the list
* @param parent a pointer to the parent of this object
* @return 1 for a successfully created object, 0 for error
*/
EXPORT int create_underground_line_conductor(OBJECT **obj, OBJECT *parent)
{
	try
	{
		*obj = gl_create_object(underground_line_conductor::oclass);
		if (*obj!=NULL)
		{
			underground_line_conductor *my = OBJECTDATA(*obj,underground_line_conductor);
			gl_set_parent(*obj,parent);
			return my->create();
		}
		else
			return 0;
	}
	CREATE_CATCHALL(underground_line_conductor);
}

EXPORT TIMESTAMP sync_underground_line_conductor(OBJECT *obj, TIMESTAMP t1, PASSCONFIG pass)
{
	return TS_NEVER;
}

EXPORT int isa_underground_line_conductor(OBJECT *obj, char *classname)
{
	return OBJECTDATA(obj,underground_line_conductor)->isa(classname);
}

/**@}**/
