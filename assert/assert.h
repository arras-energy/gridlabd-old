/** $Id: assert.h 4738 2014-07-03 00:55:39Z dchassin $

 General purpose assert objects

 **/

#ifndef _ASSERT_H
#define _ASSERT_H

#include "gridlabd.h"

class g_assert : public gld_object {
private:
	TIMESTAMP started;
public:
	typedef enum {AS_INIT=0, AS_TRUE=1, AS_FALSE=2, AS_NONE=3} ASSERTSTATUS;
	GL_ATOMIC(enumeration,status); 
	GL_STRING(char1024,target);		
	GL_STRING(char32,part);
	GL_ATOMIC(enumeration,relation);
	GL_STRING(char1024,value);
	GL_STRING(char1024,value2);
	GL_ATOMIC(TIMESTAMP,start);
	GL_ATOMIC(TIMESTAMP,stop);
	GL_ATOMIC(double,hold);
	GL_STRING(char1024,group);

private:
	std::list<gld_property> *target_list;
	ASSERTSTATUS evaluate_status(gld_property &target_prop);

public:
	/* required implementations */
	g_assert(MODULE *module);
	int create(void);
	int init(OBJECT *parent);
	TIMESTAMP commit(TIMESTAMP t1, TIMESTAMP t2);
	int postnotify(PROPERTY *prop, const char *value);
	inline int prenotify(PROPERTY *prop, const char *value) { return 1; };

public:
	static CLASS *oclass;
	static g_assert *defaults;
};

#endif // _ASSERT_H
