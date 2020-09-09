#include "config.h"

/*
 * Types
 */

#ifndef __types_h__
#define __types_h__

#include "headers.h"

/*
 * uint8
 */
#if SIZEOF_UNSIGNED_CHAR == 1
	typedef unsigned char uint8;
# define UINT8_CTYPE(v) ((unsigned int)(unsigned char)(v))
#elif SIZEOF_UNSIGNED_SHORT == 1
	typedef unsigned short uint8;
# define UINT8_CTYPE(v) ((unsigned short)(v))
#elif SIZEOF_UNSIGNED_INT == 1
	typedef unsigned int uint8;
# define UINT8_CTYPE(v) ((unsigned int)(v))
#elif SIZEOF_UNSIGNED_LONG == 1
	typedef unsigned long uint8;
# define UINT8_CTYPE(v) ((unsigned long)(v))
#elif SIZEOF_UNSIGNED_LONG_LONG == 1
	typedef unsigned long long uint8;
# define UINT8_CTYPE(v) ((unsigned long long)(v))
#else
#	error *** ERROR: No 8-bit type found to use as uint8
#endif

#define MAX_UINT8 ((uint8)-1)
#if SIZEOF_SIZE_T == 1
#define MAX_SIZE_T MAX_UINT8
#endif

/*
 * uint16
 */
#if SIZEOF_UNSIGNED_CHAR == 2
	typedef unsigned char uint16;
# define UINT16_CTYPE(v) ((unsigned int)(unsigned char)(v))
#elif SIZEOF_UNSIGNED_SHORT == 2
	typedef unsigned short uint16;
# define UINT16_CTYPE(v) ((unsigned short)(v))
#elif SIZEOF_UNSIGNED_INT == 2
	typedef unsigned int uint16;
# define UINT16_CTYPE(v) ((unsigned int)(v))
#elif SIZEOF_UNSIGNED_LONG == 2
	typedef unsigned long uint16;
# define UINT16_CTYPE(v) ((unsigned long)(v))
#elif SIZEOF_UNSIGNED_LONG_LONG == 2
	typedef unsigned long long uint16;
# define UINT16_CTYPE(v) ((unsigned long long)(v))
#else
#	error *** ERROR: No 16-bit type found to use as uint16
#endif

#define MAX_UINT16 ((uint16)-1)
#if SIZEOF_SIZE_T == 2
#define MAX_SIZE_T MAX_UINT16
#endif

/*
 * uint32
 */
#if SIZEOF_UNSIGNED_CHAR == 4
	typedef unsigned char uint32;
# define UINT32_CTYPE(v) ((unsigned int)(unsigned char)(v))
#elif SIZEOF_UNSIGNED_SHORT == 4
	typedef unsigned short uint32;
# define UINT32_CTYPE(v) ((unsigned short)(v))
#elif SIZEOF_UNSIGNED_INT == 4
	typedef unsigned int uint32;
# define UINT32_CTYPE(v) ((unsigned int)(v))
#elif SIZEOF_UNSIGNED_LONG == 4
	typedef unsigned long uint32;
# define UINT32_CTYPE(v) ((unsigned long)(v))
#elif SIZEOF_UNSIGNED_LONG_LONG == 4
	typedef unsigned long long uint32;
# define UINT32_CTYPE(v) ((unsigned long long)(v))
#else
#	error *** ERROR: No 32-bit type found to use as uint32
#endif

#define MAX_UINT32 ((uint32)-1)
#if SIZEOF_SIZE_T == 4
#define MAX_SIZE_T MAX_UINT32
#endif

/*
 * uint64
 */
#if SIZEOF_UNSIGNED_CHAR == 8
	typedef unsigned char uint64;
# define UINT64_CTYPE(v) ((unsigned int)(unsigned char)(v))
#elif SIZEOF_UNSIGNED_SHORT == 8
	typedef unsigned short uint64;
# define UINT64_CTYPE(v) ((unsigned short)(v))
#elif SIZEOF_UNSIGNED_INT == 8
	typedef unsigned int uint64;
# define UINT64_CTYPE(v) ((unsigned int)(v))
#elif SIZEOF_UNSIGNED_LONG == 8
	typedef unsigned long uint64;
# define UINT64_CTYPE(v) ((unsigned long)(v))
#elif SIZEOF_UNSIGNED_LONG_LONG == 8
	typedef unsigned long long uint64;
# define UINT64_CTYPE(v) ((unsigned long long)(v))
#else
#	error *** ERROR: No 64-bit type found to use as uint64
#endif

#define MAX_UINT64 ((uint64)-1)
#if SIZEOF_SIZE_T == 8
#define MAX_SIZE_T MAX_UINT64
#endif

#endif
