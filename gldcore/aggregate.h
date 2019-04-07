/** $Id: aggregate.h 4738 2014-07-03 00:55:39Z dchassin $
	Copyright (C) 2008 Battelle Memorial Institute
	@file aggregate.h
	@addtogroup aggregate
 @{
 **/

#ifndef _AGGREGATE_H
#define _AGGREGATE_H

#include "platform.h"
#include "find.h"

// deprecated as of 4.2
DEPRECATED typedef enum e_aggregate {AGGR_NOP, AGGR_MIN, AGGR_MAX, AGGR_AVG, AGGR_STD, AGGR_MBE, AGGR_MEAN, AGGR_VAR, AGGR_SKEW, AGGR_KUR, AGGR_GAMMA, AGGR_COUNT, AGGR_SUM, AGGR_PROD} AGGREGATOR; /**< the aggregation method to use */
DEPRECATED typedef enum e_aggregate_part {AP_NONE, AP_REAL, AP_IMAG, AP_MAG, AP_ANG, AP_ARG} AGGRPART; /**< the part of complex values to aggregate */
DEPRECATED typedef enum e_aggregate_flags {
	AF_NONE = 0x00,
	AF_ABS	= 0x01,
} AGGRFLAGS; /**< absolute value aggregation flag */
DEPRECATED typedef struct s_aggregate {
	AGGREGATOR op; /**< the aggregation operator (min, max, etc.) */
	struct s_findpgm *group; /**< the find program used to build the aggregation */
	PROPERTY *pinfo; /**< the property over which the aggregation is done */
	UNIT *punit; /**< the unit we want to output the property in */
	double scale; /**< the scalar to convert from the old units to the desired units */
	AGGRPART part; /**< the property part (complex only) */
	AGGRFLAGS flags; /**< aggregation flags (e.g., AF_ABS) */
	struct s_findlist *last; /**< the result of the last run */
	struct s_aggregate *next; /**< the next aggregation in the core's list of aggregators */
} AGGREGATION; /**< the aggregation type */
DEPRECATED CDECL AGGREGATION *aggregate_mkgroup(const char *aggregator, const char *group_expression);
DEPRECATED CDECL double aggregate_value(AGGREGATION *aggregate);

#ifdef __cplusplus // DEPRECATED

class GldAggregator 
{
private:
	AGGREGATION *aggr;
	size_t refcnt;
public:
	GldAggregator(AGGREGATION *a);
	GldAggregator(const char *aggregator, const char *group_expression);
	~GldAggregator(void);
public:
	double get_value(void);
	inline AGGREGATION *get_aggregator(void) { return aggr; };
public:
	DEPRECATED inline operator AGGREGATION*(void) { return aggr; };
};

#endif // DEPRECATED

#endif

/**@}**/
