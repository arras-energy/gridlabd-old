/*	File: aggregate.h

		Property aggregation
 */

#ifndef _AGGREGATE_H
#define _AGGREGATE_H

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

#include "platform.h"
#include "find.h"

/*	Typedef: AGGREGATOR 

		See enum <e_aggregate>

	Enum: e_aggregate

		Defines a type of aggregation

	Members:

		AGGR_NOP = 0 - no operation
		AGGR_MIN = 1 - minimum 
		AGGR_MAX = 2 - maximum 
		AGGR_AVG = 3 - average 
		AGGR_STD = 4 - standard deviation
		AGGR_MBE = 5 - mean bias error
		AGGR_MEAN = 6 - mean 
		AGGR_VAR = 7 - variance
		AGGR_SKEW = 8 - skew
		AGGR_KUR = 9 - kurtosis
		AGGR_GAMMA = 10 - gamma
		AGGR_COUNT = 11 - count
		AGGR_SUM = 12 - sum
		AGGR_PROD = 13 - product

	See Also: 

		<AGGREGATION>

 */
DEPRECATED typedef enum e_aggregate 
{
	AGGR_NOP = 0, 
	AGGR_MIN = 1, 
	AGGR_MAX = 2, 
	AGGR_AVG = 3, 
	AGGR_STD = 4, 
	AGGR_MBE = 5, 
	AGGR_MEAN = 6, 
	AGGR_VAR = 7, 
	AGGR_SKEW = 8, 
	AGGR_KUR = 9, 
	AGGR_GAMMA = 10, 
	AGGR_COUNT = 11, 
	AGGR_SUM = 12, 
	AGGR_PROD = 13,
} AGGREGATOR; 

/*	Typedef: AGGRPART

		See enum <e_aggregate_part>

	Enum: e_aggregate_part

		Aggregation part for <complex> value

	Members:

		AP_NONE = 0 - No aggregation part 
		AP_REAL = 1 - Real part
		AP_IMAG = 2 - Imaginary part
		AP_MAG = 3 - Magnitude 
		AP_ANG = 4 - Angle in degrees
		AP_ARG = 5 - Angle in radians

	See Also: 

		<AGGREGATION>
 */
DEPRECATED typedef enum e_aggregate_part 
{
	AP_NONE = 0, 
	AP_REAL = 1, 
	AP_IMAG = 2, 
	AP_MAG = 3, 
	AP_ANG = 4, 
	AP_ARG = 5,
} AGGRPART;
/*	Typedef: AGGRFLAGS

		See enum <e_aggregate_flags>

	Enum: e_aggregate_flags

		Aggregation method options

	Members:

		AF_NONE = 0x00 - No options specified
		AF_ABS	= 0x01 - Absolute value option

	See Also: 

		<AGGREGATION>
 */
DEPRECATED typedef enum e_aggregate_flags 
{
	AF_NONE = 0x00,
	AF_ABS	= 0x01,
} AGGRFLAGS; 

/*	Typedef: AGGREGATION

		Aggregator data

	Members:

		<AGGREGATOR> *op - the aggregation operator
		struct <s_findpmg> *group - the find program used to build the aggregation
		<PROPERTY> *pinfo - the property over which the aggregation is performed
		<UNIT> *punit - the unit in which the output value is generated
		double scale - the scalar used to convert the output unit
		<AGGRPART> part - the property part over which the aggregator is performed
		<AGGRFLAGS> flags - aggregation flags (e.g., see <AGGRFLAGS>)
		size_t refcnt - count of references to this aggregator
		struct <s_findlist> *last - the result of the last aggregation run
		AGGREGATION *next - the next aggregation in the list of aggregators
 */
DEPRECATED typedef struct s_aggregate {
	AGGREGATOR op; 
	struct s_findpgm *group; 
	PROPERTY *pinfo; 
	UNIT *punit; 
	double scale; 
	AGGRPART part; 
	AGGRFLAGS flags; 
	size_t refcnt; 
	struct s_findlist *last; 
	struct s_aggregate *next; 
} AGGREGATION; 

DEPRECATED CDECL AGGREGATION *aggregate_mkgroup(const char *aggregator, const char *group_expression);
DEPRECATED CDECL double aggregate_value(AGGREGATION *aggregate);

#ifdef __cplusplus // DEPRECATED

/*	Class: GldAggregator

		Aggregator implementation class
*/
class GldAggregator 
{
private:
	AGGREGATION *aggr;

public:
	/*	Constructor: GldAggregator
			Use an existing aggregator
	
		Parameters:
			aggr - a reference to the aggregation data
	
		Example:
			--- C++ ---
			GldAggregator T_min("min(air_temperature)","class=house");
			GldAggregator ref(T_min.get_aggregator());
			--- C++ ---
	 */
	GldAggregator(AGGREGATION *aggr);

	/*	Constructor: GldAggregator
			Implement a new aggregator
	
		Parameters:
			aggregator - the aggregation method to use (e.g., "min", "max", "avg")
			group_expression - the grouping expression (e.g., "class=my_class")

		Example:
			--- C++ ---
			GldAggregator T_min("min(air_temperature)","class=house");
			--- C++ ---
	*/
	GldAggregator(const char *aggregator, const char *group_expression);

	/* Destructor: ~GldAggregator
			Decrements the reference count and deletes the aggregator if zero left
	 */
	~GldAggregator(void);

public:
	/* 	Method: get_value
			Compute the aggregate value

		Example:
			--- C++ ---
			GldAggregator T_min("min(air_temperature)","class=house");
			double T = T_min.get_value();
			--- C++ ---
	 */
	double get_value(void);

	/*	Method: get_aggregator
			Obtain a reference to the aggregation data
	
		Example:
			--- C++ ---
			GldAggregator T_min("min(air_temperature)","class=house");
			GldAggregator ref(T_min.get_aggregator());
			--- C++ ---
	 */
	DEPRECATED inline AGGREGATION *get_aggregator(void) { return aggr; };

	/*	Method: AGGREGATION
			Cast to a reference to the aggregation data
	
		Example:
			--- C++ ---
			GldAggregator T_min("min(air_temperature)","class=house");
			GldAggregator ref((AGGREGATION*)T_min);
			--- C++ ---
	 */
	DEPRECATED inline operator AGGREGATION*(void) { return aggr; };
};

#endif // DEPRECATED

#endif

/**@}**/
