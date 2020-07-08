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

#ifndef nano_core_osapi_serial_std_h
#define nano_core_osapi_serial_std_h

#if defined(NANO_HAVE_STD_SERIAL)

typedef struct NANODllExport NANO_OSAPI_StdSerialConnectionI
{
    int fd;
    NANO_usize max_tx_size;
    NANO_usize max_rx_size;

#if NANO_CPP
    NANO_OSAPI_StdSerialConnectionI()
    {
        this->fd = NANO_FD_INVALID;
        this->max_tx_size = 0;
        this->max_rx_size = 0;
    }
#endif /*NANO_CPP */

} NANO_OSAPI_StdSerialConnection;

#define NANO_OSAPI_STDSERIALCONNECTION_INITIALIZER \
{\
    NANO_FD_INVALID, /* fd */\
    0, /* max_tx_size */\
    0 /* max_rx_size */\
}

#define NANO_OSAPI_SerialConnection \
        NANO_OSAPI_StdSerialConnection

#define NANO_OSAPI_SERIALCONNECTION_INITIALIZER \
        NANO_OSAPI_STDSERIALCONNECTION_INITIALIZER

typedef struct NANODllExport NANO_OSAPI_StdSerialConnectionPropertiesI
{
    const char *device;
    NANO_u32 speed;
    NANO_u32 block_time_ms;
    NANO_bool parity;
    NANO_bool sync;
    NANO_usize max_tx_size;
    NANO_usize max_rx_size;

#if NANO_CPP
    NANO_OSAPI_StdSerialConnectionPropertiesI()
    {
        this->device = NULL;
        this->speed = 115200;
        this->block_time_ms = 500;
        this->parity = NANO_BOOL_FALSE;
        this->sync = NANO_BOOL_FALSE;
        this->max_tx_size = 0;
        this->max_rx_size = 0;
    }
#endif /*NANO_CPP */

} NANO_OSAPI_StdSerialConnectionProperties;

#define NANO_OSAPI_STDSERIALCONNECTIONPROPERTIES_INITIALIZER \
{\
    NULL, /* device */\
    115200, /* speed */\
    500, /* block_time_ms */\
    NANO_BOOL_FALSE, /* parity */\
    NANO_BOOL_FALSE, /* sync */\
    0, /* max_tx_size */\
    0 /* max_rx_size */\
}

#define NANO_OSAPI_SerialConnectionProperties \
        NANO_OSAPI_StdSerialConnectionProperties

#define NANO_OSAPI_SERIALCONNECTIONPROPERTIES_INITIALIZER \
        NANO_OSAPI_STDSERIALCONNECTIONPROPERTIES_INITIALIZER

NANO_RetCode
NANO_OSAPI_StdSerialConnection_open(
    NANO_OSAPI_SerialConnection *const self,
    const NANO_OSAPI_SerialConnectionProperties *const properties);

NANO_RetCode
NANO_OSAPI_StdSerialConnection_write(
    NANO_OSAPI_SerialConnection *const self,
    const NANO_u8 *const data,
    const NANO_usize data_len);

NANO_RetCode
NANO_OSAPI_StdSerialConnection_read(
    NANO_OSAPI_SerialConnection *const self,
    NANO_u8 *const data,
    NANO_usize *const data_len);

NANO_RetCode
NANO_OSAPI_StdSerialConnection_close(
    NANO_OSAPI_SerialConnection *const self);

#define NANO_OSAPI_SerialConnection_open \
        NANO_OSAPI_StdSerialConnection_open

#define NANO_OSAPI_SerialConnection_read \
        NANO_OSAPI_StdSerialConnection_read

#define NANO_OSAPI_SerialConnection_write \
        NANO_OSAPI_StdSerialConnection_write

#define NANO_OSAPI_SerialConnection_close \
        NANO_OSAPI_StdSerialConnection_close

#endif /* defined(NANO_HAVE_STD_SERIAL) */

#endif /* nano_core_osapi_serial_std_h */