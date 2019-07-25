/* File: compare.h 
 * Copyright (C) 2008, Battelle Memorial Institute
 */

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

#ifndef _COMPARE_H

/*	Define: TCNONE

		Null comparison operators

	See Also: 
	- <PROPERTYSPEC>
	- <property_type>
 */
#define TCNONE \
	{TCOP_NOP,"",NULL},\
	{TCOP_NOP,"",NULL},\
	{TCOP_NOP,"",NULL},\
	{TCOP_NOP,"",NULL},\
	{TCOP_NOP,"",NULL},\
	{TCOP_NOP,"",NULL},\
	{TCOP_NOP,"",NULL},\
	{TCOP_NOP,"",NULL}

/*	Define: TCOPB

		Boolean comparison operators

	See Also: 
	- <PROPERTYSPEC>
	- <property_type>
 */
#define TCOPB(X) \
	{TCOP_EQ,"==",compare_tc_##X##_eq}, \
	{TCOP_NOP,"",NULL}, \
	{TCOP_NOP,"",NULL}, \
	{TCOP_NE,"!=",compare_tc_##X##_ne}, \
	{TCOP_NOP,"",NULL}, \
	{TCOP_NOP,"",NULL}, \
	{TCOP_NOP,"",NULL}, \
	{TCOP_NOP,"",NULL}

/*	Define: TCOPS

		String comparison operators

	See Also: 
	- <PROPERTYSPEC>
	- <property_type>
 */
#define TCOPS(X) \
	{TCOP_EQ,"==",compare_tc_##X##_eq}, \
	{TCOP_LE,"<=",compare_tc_##X##_le}, \
	{TCOP_GE,">=",compare_tc_##X##_ge}, \
	{TCOP_NE,"!=",compare_tc_##X##_ne}, \
	{TCOP_LT,"<",compare_tc_##X##_lt}, \
	{TCOP_GT,">",compare_tc_##X##_gt}, \
	{TCOP_IN,"inside",compare_tc_##X##_in,1}, \
	{TCOP_NI,"outside",compare_tc_##X##_ni,1}

/*	Define: TCOPD

		Variant comparison operators

	See Also: 
	- <PROPERTYSPEC>
	- <property_type>
 */
#define TCOPD(X) \
	int compare_tc_##X##_eq(void*,void*,void*); \
	int compare_tc_##X##_le(void*,void*,void*); \
	int compare_tc_##X##_ge(void*,void*,void*); \
	int compare_tc_##X##_ne(void*,void*,void*); \
	int compare_tc_##X##_lt(void*,void*,void*); \
	int compare_tc_##X##_gt(void*,void*,void*); \
	int compare_tc_##X##_in(void*,void*,void*); \
	int compare_tc_##X##_ni(void*,void*,void*); 

TCOPD(double);
TCOPD(float);
TCOPD(uint16);
TCOPD(uint32);
TCOPD(uint64);
TCOPD(string);
TCOPD(bool);
TCOPD(timestamp);
TCOPD(object);

#endif
