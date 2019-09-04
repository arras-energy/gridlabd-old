/* File: enduse.h 
 * Copyright (C) 2008, Battelle Memorial Institute
 */

#ifndef _ENDUSE_H
#define _ENDUSE_H

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

#include "class.h"
#include "object.h"
#include "timestamp.h"
#include "loadshape.h"

/*	Typedef: EUCIRCUIT
		See <e_endusecircuit>

	Enum: e_endusecircuit
	EUC_IS110 = 0 - indicates that the enduse is connected line-to-neutral
	EUC_IS220 = 1 - indicates that the enduse is connected line-to-line
	EUC_HEATLOAD = 2 - indicates that the enduse drives heat gain

	These values are used to indicate how an enduse load is connected
 */
typedef enum e_endusecircuit
{
	EUC_IS110 = 0,
	EUC_IS220 = 1,
	EUC_HEATLOAD = 2,
} EUCIRCUIT;

/*	Typedef: EUMOTORTYPE
		See <e_endusemotortype>

	Enum: e_endusemotortype
	EUMT_MOTOR_A - 3 phase induction motors driving constant torque loads
	EUMT_MOTOR_B - 3 phase induction motors driving high inertia speed-squares torque loads
	EUMT_MOTOR_C - 3 phase induction motors driving low inertia loads speed-squared torque loads
	EUMT_MOTOR_D - 1 phase induction motors driving constant torque loads
	_EUMT_COUNT - provides a count of known motor types; must be last

	These values are used to indicate what type of motor load an enduse is
 */
typedef enum e_endusemotortype
{
	EUMT_MOTOR_A, 
	EUMT_MOTOR_B, 
	EUMT_MOTOR_C, 
	EUMT_MOTOR_D, 
	_EUMT_COUNT, 
} EUMOTORTYPE;

/*	Typedef: EUELECTRONICTYPE
		See <e_enduseelectronictype>

	Enum: e_enduseelectronictype
	EUET_ELECTRONIC_A - simple power electronics (no backfeed)
	EUET_ELECTRONIC_B - advanced power electronics (w/ backfeed)
	_EUET_COUNT - provides a count of known electronics types; must be last

	These values are used to indicate what type of electronics the enduse load uses
 */
typedef enum e_enduseelectronictype
{
	EUET_ELECTRONIC_A, 
	EUET_ELECTRONIC_B, 
	_EUET_COUNT, 
} EUELECTRONICTYPE;

/*	Typedef: EUMOTOR
		See <s_motor>

	Structure: s_motor
	power - motor power when running
	impedance - motor impedance when stalled
	inertia - motor inertia in seconds
	v_stall - motor stall voltage (pu)
	v_start - motor start voltage (pu)
	v_trip - motor trip voltage (pu)
	t_trip - motor thermal trip time in seconds

	This structure contains information about a motor load's dynamic response.
 */
typedef struct s_motor 
{
	complex power;
	complex impedance;
	double inertia;
	double v_stall;
	double v_start;
	double v_trip;
	double t_trip;
	/* TODO add slip data (0 for synchronous motors) */
} EUMOTOR;

/*	Typedef: EUELECTRONIC
		See <s_enduseelectronic>

	Structure: s_enduseelectronic
	power - load power when running
	inertia - load "inertia"
	v_trip - load "trip" voltage (pu)
	v_start - load "start" voltage (pu)

	This structure contains information about electronic enduse loads
 */
typedef struct s_enduseelectronic 
{
	complex power;
	double inertia;
	double v_trip;
	double v_start;
} EUELECTRONIC;

/*	Typedef: enduse
		See <s_enduse>

	Structure: s_enduse
	total - total power in kW
	energy - total energy in kWh
	demand - maximum power in kW (can be reset)
	config  - end-use configuration
	breaker_amps - breaker limit (if any)
	admittance - constant impedance oprtion of load in kW 
	current - constant current portion of load in kW
	power - constant power portion of load in kW
	motor - motor loads (A-D)
	electronic - electronic loads (S/D)
	impedance_fraction - constant impedance fraction (pu load)
	current_fraction - constant current fraction (pu load)
	power_fraction - constant power fraction (pu load)
	power_factor - power factor
	voltage_factor - voltage factor (pu nominal)
	heatgain - internal heat from load (Btu/h)
	cumulative_heatgain - internal cumulative heat gain from load (Btu)
	heatgain_fraction - fraction of power that goes to internal heat (pu Btu/h)
	name -
	shape -
	t_last - last time of update
	end_obj - deprecated ENDUSELOAD;
	next - next enduse;
 */
typedef struct s_enduse 
{
	/* the output value must be first for transform to stream */
	complex total;
	complex energy;
	complex demand;
	set config;
	double breaker_amps;
	complex admittance;
	complex current;
	complex power;
	EUMOTOR motor[_EUMT_COUNT];
	EUELECTRONIC electronic[_EUET_COUNT];
	double impedance_fraction;
	double current_fraction;
	double power_fraction;
	double power_factor;
	double voltage_factor;
	double heatgain;
	double cumulative_heatgain;
	double heatgain_fraction;
	double gas_fraction;
	const char *name;
	loadshape *shape;
	TIMESTAMP t_last;
	// @todo this is obsolete and must be retrofitted with the above values
	DEPRECATED struct s_object_list *end_obj;
	struct s_enduse *next;
#ifdef _DEBUG
	unsigned int magic;
#endif
} enduse;

#ifdef __cplusplus
extern "C" {
#endif

/*	Function: enduse_create

	This function creates an enduse load
 */
DEPRECATED int enduse_create(void *ptr);

/*	Function: enduse_init

	This function initializes an enduse load
 */
DEPRECATED int enduse_init(enduse *e);

/*	Function: enduse_initall

	This function initializes all enduse loads
 */
DEPRECATED int enduse_initall(void);

/*	Function: enduse_sync

	This function synchronizes an enduse load
 */
DEPRECATED TIMESTAMP enduse_sync(enduse *e, PASSCONFIG pass, TIMESTAMP t1);

/*	Function: enduse_syncall

	This function synchronizes all enduse loads
 */
DEPRECATED TIMESTAMP enduse_syncall(TIMESTAMP t1);

/*	Function: convert_to_enduse

	This function converts a string to an enduse load
 */
DEPRECATED int convert_to_enduse(const char *string, void *data, PROPERTY *prop);

/*	Function: convert_from_enduse

	This function converts an enduse to a string
 */
DEPRECATED int convert_from_enduse(char *string,int size,void *data, PROPERTY *prop);

/*	Function: enduse_publish

	This function publishes the properties of an enduse
 */
DEPRECATED int enduse_publish(CLASS *oclass, PROPERTYADDR struct_address, const char *prefix);
DEPRECATED int enduse_test(void);

/*	Function: enduse_get_part

	This function gets a property part of an enduse load
 */
DEPRECATED double enduse_get_part(void *c, const char *name);

/*	Function: enduse_set_part

	This function sets a property part of an enduse load
 */
DEPRECATED int enduse_set_part(void *x, const char *name, const char *value);

#ifdef __cplusplus
}

/*	Class: GldEnduse

	This class implements the <s_enduse> data handler
 */
class GldEnduse
{
private:
	GldMain *instance;
	struct s_enduse *data;

public:
	// Constructor: GldEnduse
	GldEnduse(void);

	// Destructor: GldEnduse
	~GldEnduse(void);

public:
	// Method: initall
	int initall(void);

	// Method: syncall
	TIMESTAMP syncall(TIMESTAMP t1);
public:

	// Method: create
	int create(void *ptr);

	// Method: init
	int init();

	// Method: publish
	int publish(CLASS *oclass, PROPERTYADDR struct_address, const char *prefix);

	// Method: sync
	TIMESTAMP sync(PASSCONFIG pass, TIMESTAMP t1);

	// Method: convert_to_enduse
	int convert_to_enduse(const char *string, void *data, PROPERTY *prop);

	// Method: convert_from_enduse
	int convert_from_enduse(char *string,int size,void *data, PROPERTY *prop);

	// Method: get_part
	double get_part(void *c, const char *name);

	// Method: set_part
	int set_part(void *x, const char *name, const char *value);
};
#endif

#endif
