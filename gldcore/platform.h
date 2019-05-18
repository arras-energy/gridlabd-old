/*	File: platform.h
		The platform include file is obsolete. All macros, typedefs, templates, and functions
		should be removed and only the selection of appropriate include files should be used.
 */

#ifndef _PLATFORM_H
#define _PLATFORM_H

#ifndef _GLDCORE_H
#error "this header may only be included from gldcore.h"
#endif

/* 	Define: DEPRECATED
		Flag to indicate a deprecated usage. Every effort should be made to eliminate use of 
		the capability.
 */
#define DEPRECATED /* Obsolete as of this version */

#include <inttypes.h>

/*	Define: HAVE_CONFIG_H
		Macro to indicate that the file <config.h> is used
 */
#ifdef HAVE_CONFIG_H
	#include "config.h"
#else
	#include "config_external.h"
#endif

#ifdef __APPLE__ /* although not advised, seems reliable enough */

	/*	Define: MACOSX
	 		Flag for MacOS builds
	 */
	#define MACOSX

	/*	Define: __WORDSIZE__
	 		CPU word size macro
	 */
	#define __WORDSIZE__ 64

	/* 	Define:
			Flag for 64-bit builds
	 */
	#define X64
#else
	#define __WORDSIZE__ __WORDSIZE
#endif

#include <sys/unistd.h>

#if __WORDSIZE__ == 64
	#define X64
#endif

/*	Define: int64
		int64 equivalence to long long

	See Also:
	- <DEPRECATED>
 */	
#define int64 DEPRECATED long long /**< standard 64-bit integers on 64-bit machines */

/*	Define: FMT_INT64
		Format string for 64-bit integer

	See Also:
	- <DEPRECATED>
 */	
#define FMT_INT64 DEPRECATED "ll" /**< standard version of 64-bit integer printf format string */

/*	Define: atoi64
		Macro for string to 64-bit integer conversion

	See Also:
	- <DEPRECATED>
 */	
#define atoi64 DEPRECATED atoll

/*	Define: stricmp
		Macro for strcasecmp equivalence

	See Also:
	- <DEPRECATED>
 */
#define stricmp DEPRECATED strcasecmp

/*	Define: strnicmp
		Macro for strncasecmp equivalence

	See Also:
	- <DEPRECATED>
 */
#define strnicmp DEPRECATED strncasecmp

/*	Define: strtok_s
		Macro for strtok_r equivalence

	See Also:
	- <DEPRECATED>
 */
#define strtok_s DEPRECATED strtok_r

#ifdef __cplusplus

	/*	Function: min
			Template function for evaluating the minimum value

		See Also:
		- <DEPRECATED>
	 */
	DEPRECATED template <class T> inline T min(T a, T b) { return a < b ? a : b; };

	/*	Function: max
			Template function for evaluating the maximum value

		See Also:
		- <DEPRECATED>
	 */
	DEPRECATED template <class T> inline T max(T a, T b) { return a > b ? a : b; };
#else // __cplusplus
	#define min(A,B) DEPRECATED ((A)<(B)?(A):(B)) /**< min macro */
	#define max(A,B) DEPRECATED ((A)>(B)?(A):(B))  /**< max macro */
#endif // __cplusplus

#ifdef X64

	/*	Define: NATIVE
			Macro to define the native integer type

		See Also:
		- <DEPRECATED>
	 */
	#define NATIVE int64	/**< native integer size */
#else
	#define NATIVE int32	/**< native integer size */
#endif

#include <math.h>

/*	Define: QNAN
		Macro to define a quiet NaN value

	See Also:
	- <NAN>
	- <DEPRECATED>
 */
#define QNAN NAN

/*	Define: CDECL
		Declaration for C-style function

	See Also:
	- <DEPRECATED>
 */
#ifdef __cplusplus
#define CDECL DEPRECATED extern "C" 
#else
#define CDECL DEPRECATED
#endif

/*	Define: EXPORT
		Declaration for exported library function

	See Also:
	- <CDECL>
	- <DEPRECATED>
 */
#define EXPORT DEPRECATED CDECL

#endif
/**@}**/
