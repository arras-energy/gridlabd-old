#ifndef _LOAD_TRACKER_H
#define _LOAD_TRACKER_H

#ifndef _POWERFLOW_H
#error "this header must be included by powerflow.h"
#endif

class load_tracker : gld_object
{
private:
	typedef union
	{
		double*	 d;
		complex* c;
		int16*   i16;
		int32*   i32;
		int64*   i64;
	} VALUEPOINTER;
public:
	static CLASS *oclass;
public:
	typedef enum e_operation {REAL=0,IMAGINARY=1,MAGNITUDE=2,ANGLE=3} OPERATION;
	enumeration operation;	///< operation to perform on complex property types
public:
	OBJECT*			target;		///< object to track a property of
	char256			target_prop;	///< the property to track
	double			full_scale;	///< full scale value expected at peak load, used for feed-forward control and to measure percentage error
	double			setpoint;	///< setpoint value to try and track
	double			deadband;	///< percentage deadband
	double			damping;	///< damping factor
	double			output;		///< output value to scale against
	double			feedback;	///< the feedback signal
private:
	PROPERTYTYPE	type;
	VALUEPOINTER	pointer;
private:
	void update_feedback_variable();
public:
	/* get_name acquires the name of an object or 'unnamed' if non set */
	inline const char *get_name(void) const { static char tmp[64]; OBJECT *obj=THISOBJECTHDR; return obj->name?obj->name:(sprintf(tmp,"%s:%d",obj->oclass->name,obj->id)>0?tmp:"(unknown)");};
	/* get_id acquires the object's id */
	inline unsigned int get_id(void) const {return THISOBJECTHDR->id;};
public:
	load_tracker(MODULE *mod);
	inline load_tracker(CLASS *cl=oclass){};
	int create(void);
	int init(OBJECT *parent);
	TIMESTAMP presync(TIMESTAMP t0);
	TIMESTAMP postsync(TIMESTAMP t0, TIMESTAMP t1);
	TIMESTAMP sync(TIMESTAMP t0);
	int isa(CLASSNAME classname);
};

#endif // _LOAD_TRACKER_H

