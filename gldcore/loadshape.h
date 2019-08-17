/* File: loadshape.h 
 * Copyright (C) 2008, Battelle Memorial Institute

	@file loadshape.h
	@addtogroup loadshape Built-in loadshapes
**/

#ifndef _SFSM_H
#define _SFSM_H

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

#include "schedule.h"
#include "timestamp.h"
#include "class.h"

typedef struct s_loadshape loadshape;

typedef enum {
	MT_UNKNOWN=0,
	MT_ANALOG,		/**< machine output an analog signal */
	MT_PULSED,		/**< machine outputs pulses of fixed area with varying frequency to match value */
	MT_MODULATED,	/**< machine outputs pulses of fixed frequency with varying area to match value */
	MT_QUEUED,		/**< machine accrues values and output pulses of fixed area with varying frequency */
	MT_SCHEDULED,	/**< machine output values on a schedule */
} MACHINETYPE; /** type of machine */
typedef enum {
	MPT_UNKNOWN=0,
	MPT_TIME,		/**< pulses are of fixed time (either total period or on-time duration); power is energy/duration */
	MPT_POWER,		/**< pulses are of fixed power; duration is energy/power */
} MACHINEPULSETYPE; /**< the type of pulses generated by the machine */
typedef enum {
	MMT_UNKNOWN=0,	/**< unspecified modulation */
	MMT_AMPLITUDE=1, /**< amplitude modulation */
	MMT_PULSEWIDTH=2, /**< pulse-width modulation */
	MMT_FREQUENCY=3, /**< frequency modulation */
} MACHINEMODULATIONTYPE;
typedef enum {
	MS_OFF=0,
	MS_ON=1,
	MS_RAMPUP=2,
	MS_RAMPDOWN=3,
} MACHINESTATE;
struct s_loadshape {
	/* the output value must be first for transform to stream */
	double load;		/**< the actual load magnitude */
	
	/* machine specification */
	SCHEDULE *schedule;	/**< the schedule driving this machine */
	MACHINETYPE type;	/**< the type of this machine */
	union {
		struct {
			double energy;		/**< the total energy used over the shape (0 if power is used) */
			double power;		/**< the power scaling factor of the shape (0 if energy is used) */
		} analog;
		struct {
			double energy;		/**< the total energy used over the shape */
			double scalar;		/**< the number of pulses over the shape */
			MACHINEPULSETYPE pulsetype;	/**< the fixed part of the pulse (time or power) */
			double pulsevalue;	/**< the value of the fixed part of the pulse */
		} pulsed;
		struct {
			double energy;		/**< the total energy used over the shape */
			double scalar;		/**< the number of pulses over the shape */
			MACHINEPULSETYPE pulsetype;	/**< the fixed part of the pulse (time or power) */
			double pulsevalue;	/**< the value of the fixed part of the pulse */
			double pulseenergy;	/**< the pulse energy */
			MACHINEMODULATIONTYPE modulation; /**< the type of modulation used */
		} modulated;
		struct {
			double energy;		/**< the total energy used over the shape */
			double scalar;		/**< the number of pulses over the shape */
			MACHINEPULSETYPE pulsetype;	/**< the fixed part of the pulse (time or power) */
			double pulsevalue;	/**< the value of the fixed part of the pulse */
			double q_on, q_off;	/**< the queue thresholds (in units of 1 pulse) */
		} queued;
		struct {
			double low, high;			/**< the low and high values in the schedule */
			double on_time, off_time;	/**< the hour of day when the high value comes on and goes off */
			double on_ramp, off_ramp;	/**< the rate at which the high value comes on and off */
			double on_end, off_end;		/**< ramp end times are computed internally to ease sync calcs */
			unsigned char weekdays;		/**< bitfield indicating which weekdays the schedule is active (Sun=b0, Mon=b1, etc.) */
			unsigned int dt;			/**< the time resolution of the schedule (default is 3600s) */
		} scheduled;
	} params;	/**< the machine parameters (depends on #type) */

	/* internal machine parameters */
	double r;			/**< the state rate */
	double re[2];		/**< the state rate stdevs (not used yet) */ 
	double d[2];		/**< the state transition thresholds */
	double de[2];		/**< the state transition threshold stdevs (not used yet) */
	double dPdV;		/**< the voltage sensitivity of the load */

	/* state variables */
	double q;			/**< the internal state of the machine */
	MACHINESTATE s;		/**< the current state of the machine (0 or 1) */
	TIMESTAMP t0;	/**< time of last update (in seconds since epoch) */
	TIMESTAMP t2;	/**< time of next update (in seconds since epoch) */
	unsigned int rng_state; /**< state of the random number generator */

	struct s_loadshape *next;	/* next loadshape in list */
};

#ifdef __cplusplus
extern "C" {
#endif

int loadshape_create(void *ptr);
int loadshape_init(loadshape *shape);
int loadshape_initall(void);
TIMESTAMP loadshape_sync(loadshape *m, TIMESTAMP t1);
TIMESTAMP loadshape_syncall(TIMESTAMP t1);

int loadshape_test(void);

int convert_from_loadshape(char *string,int size,void *data, PROPERTY *prop); /**< convert from a loadshape to a string */
int convert_to_loadshape(const char *string, void *data, PROPERTY *prop); /**< convert from a string to a loadshape */

#ifdef __cplusplus
}
#endif

#endif
