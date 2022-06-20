/*
 * Copyright (c) Yann Collet, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under both the BSD-style license (found in the
 * LICENSE file in the root directory of this source tree) and the GPLv2 (found
 * in the COPYING file in the root directory of this source tree).
 * You may select, at your option, one of the above-listed licenses.
 */

#ifndef TIME_FN_H_MODULE_287987
#define TIME_FN_H_MODULE_287987

#if defined (__cplusplus)
extern "C" {
#endif


/*-****************************************
*  Dependencies
******************************************/
#define _POSIX_C_SOURCE	200809L /* clock_gettime */
#include <time.h>         /* clock_t, clock, CLOCKS_PER_SEC */



/*-****************************************
*  Local Types
******************************************/

#if !defined (__VMS) && (defined (__cplusplus) || (defined (__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) /* C99 */) )
# if defined(_AIX)
#  include <inttypes.h>
# else
#  include <stdint.h> /* intptr_t */
# endif
  typedef uint64_t           PTime;  /* Precise Time */
#else
  typedef unsigned long long PTime;  /* does not support compilers without long long support */
#endif

/* The following macro test is supposed to probe the existence of timespec_get() */
#if (  defined (__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L) /* C11 */ \
    && defined(TIME_UTC)  /* some OS, like FreeBSD 11, claim C11 compliance yet don't provide timespec_get */ \
    && !defined(__ANDROID__) /* Android <= 10 even provides TIME_UTC but still does not provide timespec_get */ \
    )
# define TIMEFN_HAS_TIMESPEC_GET
#endif

/*-****************************************
*  Time functions
******************************************/
#if defined(_WIN32)   /* Windows */

    #include <windows.h>   /* LARGE_INTEGER */
    typedef LARGE_INTEGER UTIL_time_t;
    #define UTIL_TIME_INITIALIZER { { 0, 0 } }

#elif defined(__APPLE__) && defined(__MACH__)

    #include <mach/mach_time.h>
    typedef PTime UTIL_time_t;
    #define UTIL_TIME_INITIALIZER 0

#elif defined(TIMEFN_HAS_TIMESPEC_GET) \
    || defined(CLOCK_MONOTONIC) /* POSIX.1-2001 (optional) */

    typedef struct timespec UTIL_time_t;
    #define UTIL_TIME_INITIALIZER { 0, 0 }

#else   /* relies on standard C90 (note : clock_t measurements can be wrong when using multi-threading) */

    #define UTIL_TIME_USES_C90_CLOCK
    typedef clock_t UTIL_time_t;
    #define UTIL_TIME_INITIALIZER 0

#endif


UTIL_time_t UTIL_getTime(void);
PTime UTIL_getSpanTimeMicro(UTIL_time_t clockStart, UTIL_time_t clockEnd);
PTime UTIL_getSpanTimeNano(UTIL_time_t clockStart, UTIL_time_t clockEnd);

#define SEC_TO_MICRO ((PTime)1000000)
PTime UTIL_clockSpanMicro(UTIL_time_t clockStart);
PTime UTIL_clockSpanNano(UTIL_time_t clockStart);

void UTIL_waitForNextTick(void);


#if defined (__cplusplus)
}
#endif

#endif /* TIME_FN_H_MODULE_287987 */
