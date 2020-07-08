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
 * @file nano_core_osapi_platform_arduino.h
 * @brief OS abstraction layer for systems using the Arduino SDK.
 * 
 */

#ifndef nano_core_osapi_platform_arduino_h
#define nano_core_osapi_platform_arduino_h

#if NANO_PLATFORM == NANO_PLATFORM_ARDUINO

/*****************************************************************************
 *                            Platform Libraries
 *****************************************************************************/
#if defined(ESP8266)

/* Header file provided by the ESPRESSIF esp8266 toolkit */
#include "osapi.h"
/* Required for ets_memset() */
#include "ets_sys.h"
#if NANO_FEAT_LOG
#include "user_interface.h"
#endif /* NANO_FEAT_LOG */



#elif defined(ESP32)

/* The ESP32 toolkit exposes standard libc functions */
#include <stdlib.h>
#include <string.h>
#if NANO_FEAT_LOG
#include <stdio.h>
#endif /* NANO_FEAT_LOG */

#else

#include <stdlib.h>
#include <string.h>
#if NANO_FEAT_LOG
#include <stdio.h>
#endif /* NANO_FEAT_LOG */

#endif /* platform libs */

#ifndef ARDUINO
#include "Arduino.h"
#endif

/******************************************************************************
 *                          Network stack selection
 ******************************************************************************/
#if defined(ESP8266)

/* Espressif ESP8266 boards */

/* Select Lwip-based network stack */
#define NANO_HAVE_LWIP
#define NANO_HAVE_LWIP_SOCKET

#elif defined(ESP32)

/* Espressif ESP32 boards */

/* Select BSD socket-based network stack */
#define NANO_HAVE_SOCKET
#define NANO_HAVE_LWIP

#elif defined(ARDUINO_AMEBA)

/* Realtek Ameba boards */

/* Select BSD socket-based network stack */
// #define NANO_HAVE_SOCKET
#define NANO_HAVE_LWIP

#else

// #warning "Unsupported Arduino target"

/* Assume we have LwIP on Arduino */
#if NANO_FEAT_TRANSPORT_IPV4
#define NANO_HAVE_LWIP
#endif 

#endif


#if defined(ESP8266)
/******************************************************************************
 *                         String Helper Functions
 ******************************************************************************/
#define NANO_OSAPI_String_length                os_strlen

/******************************************************************************
 *         Memory Manipulation Functions
 ******************************************************************************/
#define NANO_OSAPI_Memory_copy                  os_memcpy
#define NANO_OSAPI_Memory_set                   os_memset
#define NANO_OSAPI_Memory_compare               os_memcmp
#define NANO_OSAPI_Memory_zero(ptr_,size_)      (os_memset((ptr_),0,(size_)))
#define NANO_OSAPI_Memory_move                  os_memmove

#if NANO_FEAT_LOG
#define NANO_OSAPI_Debug_printf                 printf
#define NANO_HAVE_PRINTF
#define NANO_HAVE_PRINTF_CR
#endif /* NANO_FEAT_LOG */

#else /* Implementation with standard libc functions */

/******************************************************************************
 *                         String Helper Functions
 ******************************************************************************/
#define NANO_OSAPI_String_length                strlen

/******************************************************************************
 *                      Memory Manipulation Functions
 ******************************************************************************/
#define NANO_OSAPI_Memory_copy                  memcpy
#define NANO_OSAPI_Memory_set                   memset
#define NANO_OSAPI_Memory_compare               memcmp
#define NANO_OSAPI_Memory_zero(ptr_,size_)      (memset((ptr_),0,(size_)))
#define NANO_OSAPI_Memory_move                  memmove

#if NANO_FEAT_LOG
#define NANO_OSAPI_Debug_printf                 printf
#define NANO_HAVE_PRINTF
#endif /* NANO_FEAT_LOG */

#endif /* defined(ESP32) */

/******************************************************************************
 *                          Clock API
 ******************************************************************************/
#if NANO_FEAT_SYSTIME

extern unsigned long millis(void);

typedef struct NANODllExport NANO_OSAPI_ArduinoClockI
{
    NANO_u8 foo;
} NANO_OSAPI_ArduinoClock;

#define NANO_OSAPI_ARDUINOCLOCK_INITIALIZER \
{\
    0 \
}

#define NANO_OSAPI_Clock                    NANO_OSAPI_ArduinoClock
#define NANO_OSAPI_CLOCK_INITIALIZER        NANO_OSAPI_ARDUINOCLOCK_INITIALIZER

#define NANO_OSAPI_Clock_initialize(s_)

#define NANO_OSAPI_Clock_millis(s_,ms_out_) \
{\
    UNUSED_ARG((s_));\
    *(ms_out_) = millis();\
}
#endif /* NANO_FEAT_SYSTIME */


/******************************************************************************
 *          Debug helpers
 ******************************************************************************/
// #if NANO_FEAT_LOG
// #define NANO_OSAPI_Debug_printf                 printf
// #define NANO_HAVE_PRINTF
// #endif /* NANO_FEAT_LOG */


/******************************************************************************
 *                  Scheduler Helpers
 ******************************************************************************/
extern void esp_yield();

#define NANO_OSAPI_Scheduler_yield()              delay(0)
#define NANO_HAVE_YIELD

/*****************************************error*************************************
 *                 Interrupt Helpers
 ******************************************************************************/
#if 0
#define NANO_OSAPI_System_disable_interrupts() \
         noInterrupts()
#define NANO_OSAPI_System_enable_interrupts() \
         interrupts()
#else
#define NANO_OSAPI_System_disable_interrupts()
#define NANO_OSAPI_System_enable_interrupts()
#endif
#define NANO_HAVE_INTERRUPTS

/******************************************************************************
 *                  Serial API
 ******************************************************************************/
#define NANO_HAVE_ARDUINO_SERIAL

#endif /* NANO_PLATFORM_ARDUINO */

#endif /* nano_core_osapi_platform_arduino_h */
