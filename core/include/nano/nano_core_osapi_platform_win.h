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
 * @file nano_core_osapi_platform_win.h
 * @brief OS abstraction layer for POSIX systems.
 * 
 * This header file implements the OSAPI abstraction layer for POSIX systems.
 */

#ifndef nano_core_osapi_win_h
#define nano_core_osapi_win_h

#if NANO_PLATFORM == NANO_PLATFORM_WINDOWS

/*****************************************************************************
 *                            Platform Libraries
 *****************************************************************************/
#include <string.h>

/******************************************************************************
 *                         String Helper Functions
 ******************************************************************************/
#define NANO_OSAPI_String_length                strlen
#define NANO_OSAPI_String_compare               strcmp
#define NANO_OSAPI_String_to_long               strtol
#define NANO_OSAPI_String_find_substring        strstr
#define NANO_OSAPI_String_is_equal(s_,o_) \
(((s_) == NULL && (o_) == NULL) || \
    ((s_) != NULL && (o_) != NULL && \
        NANO_OSAPI_String_compare((s_),(o_)) == 0))

/******************************************************************************
 *         Memory Allocation and Manipulation Helper Functions
 ******************************************************************************/
#define NANO_OSAPI_Heap_allocate                malloc
#define NANO_OSAPI_Heap_free                    free
#define NANO_OSAPI_Memory_copy                  memcpy
#define NANO_OSAPI_Memory_set                   memset
#define NANO_OSAPI_Memory_compare               memcmp
#define NANO_OSAPI_Memory_zero(ptr_,size_)      (memset((ptr_),0,(size_)))
#define NANO_OSAPI_Memory_move                  memmove

#if NANO_FEAT_LOG
#define NANO_OSAPI_Debug_printf                 printf
#define NANO_HAVE_PRINTF
#endif /* NANO_FEAT_LOG */

/******************************************************************************
 *                          Clock Helper Functions
 ******************************************************************************/
#if NANO_FEAT_SYSTIME

typedef struct NANODllExport NANO_OSAPI_WinClockI
{
    int type;
} NANO_OSAPI_WinClock;

#define NANO_OSAPI_WINCLOCK_INITIALIZER \
{\
    0 /* type */\
}

#define NANO_OSAPI_Clock                NANO_OSAPI_WinClock
#define NANO_OSAPI_CLOCK_INITIALIZER    NANO_OSAPI_WINCLOCK_INITIALIZER

void
NANO_OSAPI_WinClock_initialize(NANO_OSAPI_Clock *self);

#define NANO_OSAPI_Clock_initialize \
    NANO_OSAPI_WinClock_initialize

NANODllExport
void
NANO_OSAPI_WinClock_millis(NANO_OSAPI_Clock *self, NANO_u64 *const ms_out);

#define NANO_OSAPI_Clock_millis \
    NANO_OSAPI_WinClock_millis

#endif /* NANO_FEAT_SYSTIME */

#ifdef NANO_FEAT_TRANSPORT_IPV4
#define NANO_HAVE_SOCKET_WIN
#endif /* NANO_FEAT_TRANSPORT_IPV4 */

#ifdef NANO_FEAT_TRANSPORT_PLUGIN_SERIAL
#define NANO_HAVE_WIN_SERIAL
#endif /* NANO_FEAT_TRANSPORT_PLUGIN_SERIAL */

#endif /* NANO_PLATFORM_WINDOWS */

#endif /* nano_core_osapi_h */
