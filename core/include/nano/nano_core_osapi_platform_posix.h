/******************************************************************************
 *
 * (c) 2020 Copyright, Real-Time Innovations, Inc. (RTI)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 ******************************************************************************/ 

/**
 * @file nano_core_osapi_platform_posix.h
 * @brief OS abstraction layer for POSIX systems.
 * 
 * This header file implements the OSAPI abstraction layer for POSIX systems.
 */

#ifndef nano_core_osapi_platform_posix_h
#define nano_core_osapi_platform_posix_h

#if NANO_PLATFORM_IS_POSIX

/*****************************************************************************
 *                            Platform Libraries
 *****************************************************************************/
#include <unistd.h>
#include <string.h>
#if NANO_FEAT_SYSTIME
#include <time.h>
#endif /* NANO_FEAT_SYSTIME */

/******************************************************************************
 *                          Clock Helper Functions
 ******************************************************************************/
#if NANO_FEAT_SYSTIME




#if defined(_POSIX_MONOTONIC_CLOCK) && \
    _POSIX_MONOTONIC_CLOCK >= 0 && \
    defined(CLOCK_MONOTONIC)
#define NANO_HAVE_CLOCKID
#define NANO_HAVE_CLOCK_MONOTONIC
#define NANO_POSIX_CLOCK_TYPE_DEFAULT       CLOCK_MONOTONIC
#elif defined(CLOCK_REALTIME)
#define NANO_HAVE_CLOCKID
#define NANO_POSIX_CLOCK_TYPE_DEFAULT       CLOCK_REALTIME
#endif

#ifndef NANO_HAVE_CLOCKID

#if NANO_PLATFORM == NANO_PLATFORM_LINUX
#define NANO_HAVE_GETTIMEOFDAY
#include <sys/time.h>
#endif /* NANO_PLATFORM */

#endif

typedef struct NANODllExport NANO_OSAPI_PosixClockI NANO_OSAPI_PosixClock;

void
NANO_OSAPI_PosixClock_initialize(NANO_OSAPI_PosixClock *self);

struct NANO_OSAPI_PosixClockI
{
#ifdef NANO_HAVE_CLOCKID
    clockid_t type;
    struct timespec resolution;
#else
    int foo;
#endif /* NANO_HAVE_CLOCKID */

#if NANO_CPP
    NANO_OSAPI_PosixClockI()
    {
        NANO_OSAPI_PosixClock_initialize(this);
    }
#endif /* NANO_CPP */

};


#ifdef NANO_HAVE_CLOCKID
#define NANO_OSAPI_POSIXCLOCK_INITIALIZER \
{\
    NANO_POSIX_CLOCK_TYPE_DEFAULT,/* type */\
    { 0, 0 } /* resolution */\
}
#else
#define NANO_OSAPI_POSIXCLOCK_INITIALIZER \
{\
    0 /* foot */\
}
#endif /* NANO_HAVE_CLOCKID */

#define NANO_OSAPI_Clock                NANO_OSAPI_PosixClock
#define NANO_OSAPI_CLOCK_INITIALIZER    NANO_OSAPI_POSIXCLOCK_INITIALIZER


#define NANO_OSAPI_Clock_initialize \
    NANO_OSAPI_PosixClock_initialize

void
NANO_OSAPI_PosixClock_millis(NANO_OSAPI_Clock *self, NANO_u64 *const ms_out);

#define NANO_OSAPI_Clock_millis \
    NANO_OSAPI_PosixClock_millis

#endif /* NANO_FEAT_SYSTIME */

/******************************************************************************
 *                         String Helper Functions
 ******************************************************************************/
#define NANO_OSAPI_String_length                strlen

/******************************************************************************
 *         Memory Manipulation Functions
 ******************************************************************************/
#define NANO_OSAPI_Memory_copy                  memcpy
#define NANO_OSAPI_Memory_set                   memset
#define NANO_OSAPI_Memory_compare               memcmp
#define NANO_OSAPI_Memory_zero(ptr_,size_)      (memset((ptr_),0,(size_)))
#define NANO_OSAPI_Memory_move                  memmove

/******************************************************************************
 *          Debug helpers
 ******************************************************************************/
#if NANO_FEAT_LOG
#include <stdio.h>
#define NANO_OSAPI_Debug_printf                 printf
#define NANO_HAVE_PRINTF
#endif /* NANO_FEAT_LOG */

/******************************************************************************
 *          Serial Support
 ******************************************************************************/
#define NANO_HAVE_STD_SERIAL

/* Select BSD socket-based network stack */
#define NANO_HAVE_SOCKET

#endif /* NANO_PLATFORM_IS_POSIX */

#endif /* nano_core_osapi_h */
