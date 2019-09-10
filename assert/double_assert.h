/** Assert function
**/

#ifndef _DOUBLE_ASSERT_H
#define _DOUBLE_ASSERT_H

#include "gridlabd.h"

#ifndef _isnan
#define _isnan isnan
#endif

class double_assert : public gld_object {
public:
	enum {ONCE_FALSE=0, ONCE_TRUE=1, ONCE_DONE=2};
	enum {IN_ABS=0, IN_RATIO=1};
	enum {ASSERT_TRUE=1, ASSERT_FALSE, ASSERT_NONE};

	GL_ATOMIC(enumeration,status); 
	GL_STRING(char1024,target);		
	GL_ATOMIC(double,value);
	GL_ATOMIC(enumeration,once);
	GL_ATOMIC(double,once_value);
	GL_ATOMIC(enumeration,within_mode);
	GL_ATOMIC(double,within);

public:
	/* required implementations */
	double_assert(MODULE *module);
	int create(void);
	int init(OBJECT *parent);
	TIMESTAMP commit(TIMESTAMP t1, TIMESTAMP t2);
	int postnotify(PROPERTY *prop, const char *value);
	inline int prenotify(PROPERTY*,const char *) { return 1; };
public:
	static CLASS *oclass;
	static double_assert *defaults;
};
#endif
