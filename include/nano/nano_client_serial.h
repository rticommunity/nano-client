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
 ******************************************************************************/ 

#ifndef nano_client_serial_h
#define nano_client_serial_h

#include "nano/nano_client.h"

#if NANO_FEAT_TRANSPORT_PLUGIN_SERIAL

#include "nano/nano_core_xrce_transport_serial.h"

typedef struct NANO_XRCE_SerialClientI
{
    NANO_XRCE_Client client;
    NANO_XRCE_DefaultClientStorage storage;
    NANO_XRCE_SerialClientTransport transport;
} NANO_XRCE_SerialClient;

#define NANO_XRCE_SERIALCLIENT_INITIALIZER \
{\
    NANO_XRCE_CLIENT_INITIALIZER, /* client */\
    NANO_XRCE_DEFAULTCLIENTSTORAGE_INITIALIZER, /* storage */\
    NANO_XRCE_SERIALCLIENTTRANSPORT_INITIALIZER /* transport */\
}

NANODllExport
NANO_RetCode
NANO_XRCE_SerialClient_initialize(
    NANO_XRCE_SerialClient *const self,
    const NANO_u32 client_key,
    const NANO_u16 mtu,
    NANO_u8 *const recv_buffer,
    const NANO_usize recv_buffer_size,
    const char *const serial_port,
    const NANO_u32 serial_speed,
    const NANO_u8 agent_addr,
    const NANO_u8 bind_addr);

NANO_XRCE_Client*
NANO_XRCE_SerialClient_client(NANO_XRCE_SerialClient *const self);

#define NANO_XRCE_SerialClient_client(s_)          (&(s_)->client)

NANO_XRCE_SerialClientTransport*
NANO_XRCE_SerialClient_transport(NANO_XRCE_SerialClient *const self);

#define NANO_XRCE_SerialClient_transport(s_)       (&(s_)->transport)


#endif /* NANO_FEAT_TRANSPORT_PLUGIN_SERIAL */

#endif /* nano_client_serial_h */