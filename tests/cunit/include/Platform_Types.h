/*
 * Copyright (c) 2012 QRTECH AB Gothenburg Sweden. All Rights Reserved.
 *
 * This document is provided to you in a non-exclusive agreement, and
 * shall not without QRTECH AB’s written permission be copied, imparted
 * to a third party or be used for any other unauthorized purpose.
 * QRTECH retain the right to reuse or re-license for other purposes.
 *
 * The document is provided "AS IS", WITHOUT WARRANTY and is only
 * supported by QRTECH in its complete original distribution.
 *
 * QRTECH AB  (http://www.qrtech.se)
 * Mejerigatan 1
 * 412 76 Göteborg
 * SWEDEN
 * Tel: +46 31 773 76 00
 *
 */

#ifndef PLATFORM_TYPES_H_
#define PLATFORM_TYPES_H_

#include <stdint.h>
#include <limits.h>
/*------------------------------------------------------------------------------
 * Published parameters
 *----------------------------------------------------------------------------*/
/* Unknown module */
#define PLATFORM_MODULE_ID 0

/* vendor specific */
#define PLATFORM_SW_MAJOR_VERSION 0
#define PLATFORM_SW_MINOR_VERSION 0
#define PLATFORM_SW_PATCH_VERSION 0

/*complies with autosar release version */
#define PLATFORM_AR_MAJOR_VERSION 2
#define PLATFORM_AR_MINOR_VERSION 2
#define PLATFORM_AR_PATCH_VERSION 2


/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

typedef _Bool          boolean;
typedef int8_t         sint8;
typedef uint8_t        uint8;

typedef int16_t        sint16;
typedef uint16_t       uint16;

typedef int32_t        sint32;
typedef uint32_t       uint32;

typedef int64_t        sint64;
typedef uint64_t       uint64;

typedef uint_least8_t  uint8_least;
typedef uint_least16_t uint16_least;
typedef uint_least32_t uint32_least;
typedef uint_least64_t uint64_least;

typedef int_least8_t   sint8_least;
typedef int_least16_t  sint16_least;
typedef int_least32_t  sint32_least;
typedef int_least64_t  sint64_least;

typedef float         float32;
typedef long double  float64;

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/
#ifndef TRUE
	#define TRUE ((boolean) 1)
#endif

#ifndef FALSE
	#define FALSE ((boolean) 0)
#endif


#define CPU_TYPE_8 8
#define CPU_TYPE_16 16
#define CPU_TYPE_32 32

#define MSB_FIRST 0
#define LSB_FIRST 1
#define HIGH_BYTE_FIRST 0
#define LOW_BYTE_FIRST 1

#define CPU_TYPE CPU_TYPE_32
#define CPU_BIT_ORDER MSB_FIRST
#define CPU_BYTE_ORDER HIGH_BYTE_FIRST

#endif /*PLATFORM_TYPES_H_*/
