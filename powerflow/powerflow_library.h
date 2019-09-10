/** $Id: powerflow_library.h 4738 2014-07-03 00:55:39Z dchassin $
	Copyright (C) 2008 Battelle Memorial Institute
	@file powerflow_library.h
	@addtogroup powerflow_library

	@{
 **/

#ifndef _POWERFLOWLIBRARY_H
#define _POWERFLOWLIBRARY_H

#ifndef _POWERFLOW_H
#error "this header must be included by powerflow.h"
#endif

class powerflow_library : public gld_object
{
public:
	static CLASS *oclass; 
	static CLASS *pclass;
	inline const char *get_name(void) const { static char tmp[64]; OBJECT *obj=THISOBJECTHDR; return obj->name?obj->name:(sprintf(tmp,"%s:%d",obj->oclass->name,obj->id)>0?tmp:"(unknown)");};
	inline unsigned int get_id(void) const {return THISOBJECTHDR->id;};

public:
	int create(void);
	int init(OBJECT *parent);
	powerflow_library(MODULE *mod);
	powerflow_library(CLASS *cl=oclass);
	int isa(char *classname);
};

#endif // _POWERFLOWOBJECT_H
/**@}*/

