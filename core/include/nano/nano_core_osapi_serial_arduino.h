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

#ifndef nano_core_osapi_serial_arduino_h
#define nano_core_osapi_serial_arduino_h

#if NANO_FEAT_TRANSPORT_PLUGIN_SERIAL && defined(ARDUINO)

#include "nano/nano_arduino_c.h"

typedef struct NANO_OSAPI_ArduinoSerialConnectionI
{
    NANO_ARDUINO_SerialPort port;
    NANO_i32 block_timeout_ms;


#if NANO_CPP
    NANO_OSAPI_ArduinoSerialConnectionI()
    : port(NANO_ARDUINO_SERIAL_PORT_0),
      block_timeout_ms(0)
    {
        this->port = NANO_ARDUINO_SERIAL_PORT_0;
    }
#endif /*NANO_CPP */

} NANO_OSAPI_ArduinoSerialConnection;

#define NANO_OSAPI_ARDUINOSERIALCONNECTION_INITIALIZER \
{\
    NANO_ARDUINO_SERIAL_PORT_0, /* port */\
    0 /* block_timeout_ms */\
}

typedef struct NANO_OSAPI_ArduinoSerialConnectionPropertiesI
{
    NANO_ARDUINO_SerialPort port;
    NANO_u32 speed;
    NANO_bool init;
    NANO_u32 init_delay;
    NANO_i32 block_timeout_ms;

#if NANO_CPP
    NANO_OSAPI_ArduinoSerialConnectionPropertiesI()
    : port(NANO_ARDUINO_SERIAL_PORT_0),
      speed(115200),
      init(NANO_BOOL_FALSE),
      init_delay(1000),
      block_timeout_ms(500)
    {}
#endif /*NANO_CPP */

} NANO_OSAPI_ArduinoSerialConnectionProperties;

#define NANO_OSAPI_ARDUINOSERIALCONNECTIONPROPERTIES_INITIALIZER \
{\
    NANO_ARDUINO_SERIAL_PORT_0, /* port */\
    115200, /* speed */\
    NANO_BOOL_FALSE, /* init */\
    1000, /* init_delay */\
    500 /* block_timeout_ms */\
}

#define NANO_OSAPI_SerialConnection \
        NANO_OSAPI_ArduinoSerialConnection

#define NANO_OSAPI_SERIALCONNECTION_INITIALIZER \
        NANO_OSAPI_ARDUINOSERIALCONNECTION_INITIALIZER

#define NANO_OSAPI_SerialConnectionProperties \
        NANO_OSAPI_ArduinoSerialConnectionProperties

#define NANO_OSAPI_SERIALCONNECTIONPROPERTIES_INITIALIZER \
        NANO_OSAPI_ARDUINOSERIALCONNECTIONPROPERTIES_INITIALIZER

NANO_RetCode
NANO_OSAPI_ArduinoSerialConnection_open(
    NANO_OSAPI_SerialConnection *const self,
    const NANO_OSAPI_SerialConnectionProperties *const properties);

NANO_RetCode
NANO_OSAPI_ArduinoSerialConnection_write(
    NANO_OSAPI_SerialConnection *const self,
    const NANO_u8 *const data,
    const NANO_usize data_len);

NANO_RetCode
NANO_OSAPI_ArduinoSerialConnection_read(
    NANO_OSAPI_SerialConnection *const self,
    NANO_u8 *const data,
    NANO_usize *const data_len);

NANO_RetCode
NANO_OSAPI_ArduinoSerialConnection_close(
    NANO_OSAPI_SerialConnection *const self);

#define NANO_OSAPI_SerialConnection_open \
        NANO_OSAPI_ArduinoSerialConnection_open

#define NANO_OSAPI_SerialConnection_write \
        NANO_OSAPI_ArduinoSerialConnection_write

#define NANO_OSAPI_SerialConnection_read \
        NANO_OSAPI_ArduinoSerialConnection_read

#define NANO_OSAPI_SerialConnection_close \
        NANO_OSAPI_ArduinoSerialConnection_close

#endif /* NANO_FEAT_TRANSPORT_PLUGIN_SERIAL && defined(ARDUINO) */

#endif /* nano_core_osapi_serial_arduino_h */