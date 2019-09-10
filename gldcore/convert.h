/* 	File: convert.h 
 	Copyright (C) 2008, Battelle Memorial Institute
 */

#ifndef _CONVERT_H
#define _CONVERT_H

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

#include "class.h"
#include "complex.h"

#ifdef __cplusplus
extern "C" {
#endif

// Function: convert_from_void
DEPRECATED int convert_from_void(char *buffer, int size, void *data, PROPERTY *prop);

// Function: convert_to_void
DEPRECATED int convert_to_void(const char *buffer, void *data, PROPERTY *prop);

// Function: convert_from_double
DEPRECATED int convert_from_double(char *buffer, int size, void *data, PROPERTY *prop);

// Function: convert_to_double
DEPRECATED int convert_to_double(const char *buffer, void *data, PROPERTY *prop);

// Function: convert_from_complex
DEPRECATED int convert_from_complex(char *buffer, int size, void *data, PROPERTY *prop);

// Function: convert_to_complex
DEPRECATED int convert_to_complex(const char *buffer, void *data, PROPERTY *prop);

// Function: convert_from_enumeration
DEPRECATED int convert_from_enumeration(char *buffer, int size, void *data, PROPERTY *prop);

// Function: convert_to_enumeration
DEPRECATED int convert_to_enumeration(const char *buffer, void *data, PROPERTY *prop);

// Function: convert_from_set
DEPRECATED int convert_from_set(char *buffer, int size, void *data, PROPERTY *prop);

// Function: convert_to_set
DEPRECATED int convert_to_set(const char *buffer, void *data, PROPERTY *prop);

// Function: convert_from_int16
DEPRECATED int convert_from_int16(char *buffer, int size, void *data, PROPERTY *prop);

// Function: convert_to_int16
DEPRECATED int convert_to_int16(const char *buffer, void *data, PROPERTY *prop);

// Function: convert_from_int32
DEPRECATED int convert_from_int32(char *buffer, int size, void *data, PROPERTY *prop);

// Function: convert_to_int32
DEPRECATED int convert_to_int32(const char *buffer, void *data, PROPERTY *prop);

// Function: convert_from_int64
DEPRECATED int convert_from_int64(char *buffer, int size, void *data, PROPERTY *prop);

// Function: convert_to_int64
DEPRECATED int convert_to_int64(const char *buffer, void *data, PROPERTY *prop);

// Function: convert_from_char8
DEPRECATED int convert_from_char8(char *buffer, int size, void *data, PROPERTY *prop);

// Function: convert_to_char8
DEPRECATED int convert_to_char8(const char *buffer, void *data, PROPERTY *prop);

// Function: convert_from_char32
DEPRECATED int convert_from_char32(char *buffer, int size, void *data, PROPERTY *prop);

// Function: convert_to_char32
DEPRECATED int convert_to_char32(const char *buffer, void *data, PROPERTY *prop);

// Function: convert_from_char256
DEPRECATED int convert_from_char256(char *buffer, int size, void *data, PROPERTY *prop);

// Function: convert_to_char256
DEPRECATED int convert_to_char256(const char *buffer, void *data, PROPERTY *prop);

// Function: convert_from_char1024
DEPRECATED int convert_from_char1024(char *buffer, int size, void *data, PROPERTY *prop);

// Function: convert_to_char1024
DEPRECATED int convert_to_char1024(const char *buffer, void *data, PROPERTY *prop);

// Function: convert_from_object
DEPRECATED int convert_from_object(char *buffer, int size, void *data, PROPERTY *prop);

// Function: convert_to_object
DEPRECATED int convert_to_object(const char *buffer, void *data, PROPERTY *prop);

// Function: convert_from_delegated
DEPRECATED int convert_from_delegated(char *buffer, int size, void *data, PROPERTY *prop);

// Function: convert_to_delegated
DEPRECATED int convert_to_delegated(const char *buffer, void *data, PROPERTY *prop);

// Function: convert_from_boolean
DEPRECATED int convert_from_boolean(char *buffer, int size, void *data, PROPERTY *prop);

// Function: convert_to_boolean
DEPRECATED int convert_to_boolean(const char *buffer, void *data, PROPERTY *prop);

// Function: convert_from_timestamp_stub
DEPRECATED int convert_from_timestamp_stub(char *buffer, int size, void *data, PROPERTY *prop);

// Function: convert_to_timestamp_stub
DEPRECATED int convert_to_timestamp_stub(const char *buffer, void *data, PROPERTY *prop);

// Function: convert_from_double_array
DEPRECATED int convert_from_double_array(char *buffer, int size, void *data, PROPERTY *prop);

// Function: convert_to_double_array
DEPRECATED int convert_to_double_array(const char *buffer, void *data, PROPERTY *prop);

// Function: convert_from_complex_array
DEPRECATED int convert_from_complex_array(char *buffer, int size, void *data, PROPERTY *prop);

// Function: convert_to_complex_array
DEPRECATED int convert_to_complex_array(const char *buffer, void *data, PROPERTY *prop);

// Function: convert_unit_double
DEPRECATED int convert_unit_double(const char *buffer,const char *unit, double *data);

// Function: convert_unit_complex
DEPRECATED int convert_unit_complex(const char *buffer,const char *unit, complex *data);

// Function: convert_from_real
DEPRECATED int convert_from_real(char *a, int b, void *c, PROPERTY *d);

// Function: convert_to_real
DEPRECATED int convert_to_real(const char *a, void *b, PROPERTY *c);

// Function: convert_from_float
DEPRECATED int convert_from_float(char *a, int b, void *c, PROPERTY *d);

// Function: convert_to_float
DEPRECATED int convert_to_float(const char *a, void *b, PROPERTY *c);

// Function: convert_from_struct
DEPRECATED int convert_from_struct(char *buffer, size_t len, void *data, PROPERTY *prop);

// Function: convert_to_struct
DEPRECATED int convert_to_struct(const char *buffer, void *data, PROPERTY *prop);

// Function: convert_from_method
DEPRECATED int convert_from_method(char *buffer, int size, void *data, PROPERTY *prop);

// Function: convert_to_method
DEPRECATED int convert_to_method(const char *buffer, void *data, PROPERTY *prop);

#ifdef __cplusplus
}
#endif

#endif
