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

#ifndef nano_core_osapi_serial_win_h
#define nano_core_osapi_serial_win_h

#if NANO_PLATFORM == NANO_PLATFORM_WINDOWS && \
    NANO_FEAT_TRANSPORT_PLUGIN_SERIAL

#include <windows.h>

typedef struct NANODllExport NANO_OSAPI_WinSerialConnectionI
{
    HANDLE port;

#if NANO_CPP
    NANO_OSAPI_WinSerialConnectionI()
    : port(INVALID_HANDLE_VALUE)
    {
        
    }
#endif /*NANO_CPP */

} NANO_OSAPI_WinSerialConnection;

#define NANO_OSAPI_WINSERIALCONNECTION_INITIALIZER \
{\
    INVALID_HANDLE_VALUE /* port */\
}

typedef struct NANODllExport NANO_OSAPI_WinSerialConnectionPropertiesI
{
    const char *device;
    NANO_u32 speed;
    NANO_i32 recv_timeout_ms;

#if NANO_CPP
    NANO_OSAPI_WinSerialConnectionPropertiesI()
    : device(NULL),
      speed(115200),
      recv_timeout_ms(500)
    {}
#endif /*NANO_CPP */

} NANO_OSAPI_WinSerialConnectionProperties;

#define NANO_OSAPI_WINSERIALCONNECTIONPROPERTIES_INITIALIZER \
{\
    NULL, /* device */\
    115200, /* speed */\
    100 /* recv_timeout_ms */\
}

#define NANO_OSAPI_SerialConnection \
        NANO_OSAPI_WinSerialConnection

#define NANO_OSAPI_SERIALCONNECTION_INITIALIZER \
        NANO_OSAPI_WINSERIALCONNECTION_INITIALIZER

#define NANO_OSAPI_SerialConnectionProperties \
        NANO_OSAPI_WinSerialConnectionProperties

#define NANO_OSAPI_SERIALCONNECTIONPROPERTIES_INITIALIZER \
        NANO_OSAPI_WINSERIALCONNECTIONPROPERTIES_INITIALIZER

NANODllExport
NANO_RetCode
NANO_OSAPI_WinSerialConnection_open(
    NANO_OSAPI_SerialConnection *const self,
    const NANO_OSAPI_SerialConnectionProperties *const properties);

NANODllExport
NANO_RetCode
NANO_OSAPI_WinSerialConnection_write(
    NANO_OSAPI_SerialConnection *const self,
    const NANO_u8 *const data,
    const NANO_usize data_len);

NANODllExport
NANO_RetCode
NANO_OSAPI_WinSerialConnection_read(
    NANO_OSAPI_SerialConnection *const self,
    NANO_u8 *const data,
    NANO_usize *const data_len);

NANODllExport
NANO_RetCode
NANO_OSAPI_WinSerialConnection_close(
    NANO_OSAPI_SerialConnection *const self);

#define NANO_OSAPI_SerialConnection_open \
        NANO_OSAPI_WinSerialConnection_open

#define NANO_OSAPI_SerialConnection_write \
        NANO_OSAPI_WinSerialConnection_write

#define NANO_OSAPI_SerialConnection_read \
        NANO_OSAPI_WinSerialConnection_read

#define NANO_OSAPI_SerialConnection_close \
        NANO_OSAPI_WinSerialConnection_close

#endif /* NANO_PLATFORM == NANO_PLATFORM_WINDOWS &&
            NANO_FEAT_TRANSPORT_PLUGIN_SERIAL */

#endif /* nano_core_osapi_serial_win_h */

