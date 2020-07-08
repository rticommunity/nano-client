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
 * @file nano_core_xrce_transport_udpv4.h
 * @brief UDPv4 Transport Plugin.
 * 
 */

#ifndef nano_core_xrce_transport_udpv4_h
#define nano_core_xrce_transport_udpv4_h

#ifdef __cplusplus
extern "C" {
#endif

#if NANO_FEAT_TRANSPORT_PLUGIN_UDPV4

#if NANO_FEAT_TRANSPORT_METADATA
#define NANO_XRCE_UDPV4CLIENTTRANSPORT_METADATA_LOCATOR_INITIALIZER \
{\
    { 0xEF, 0xFF, 0x00, 0x02 }, /* address */\
    7400 /* port */ \
}

extern const NANO_XRCE_TransportLocatorMedium
    NANO_XRCE_UDPV4CLIENTTRANSPORT_DEFAULT_METADATA_LOCATOR;
#endif /* NANO_FEAT_TRANSPORT_METADATA */


#define NANO_XRCE_TRANSPORT_UDP_METADATA_PORT_DEFAULT     7400
#define NANO_XRCE_TRANSPORT_UDP_AGENT_PORT_DEFAULT        7401
#define NANO_XRCE_TRANSPORT_UDP_CLIENT_PORT_DEFAULT       7402

/**
 * @brief Configuration structure for NANO_XRCE_Udpv4ClientTransport.
 */
typedef struct NANODllExport NANO_XRCE_Udpv4ClientTransportPropertiesI
{
    
    NANO_XRCE_ClientTransportProperties base;

    NANO_OSAPI_Udpv4SocketProperties socket;

    NANO_XRCE_TransportLocatorMedium bind_address;

    NANO_u8 *recv_buffer;
    NANO_usize recv_buffer_size;

#if NANO_FEAT_TRANSPORT_METADATA
    NANO_OSAPI_Udpv4SocketProperties metadata_socket;
    NANO_XRCE_TransportLocatorMedium metadata_bind_address;
#endif


#if NANO_CPP
    NANO_XRCE_Udpv4ClientTransportPropertiesI():
        base(),
        socket(),
        bind_address(),
        recv_buffer(),
        recv_buffer_size(),
        metadata_socket(),
        metadata_bind_address()
    {

    }
#endif /*NANO_CPP */

} NANO_XRCE_Udpv4ClientTransportProperties;

#if NANO_FEAT_TRANSPORT_METADATA
#define NANO_XRCE_UDPV4CLIENTTRANSPORTPROPERTIES_METADATA_INITIALIZER \
    ,\
    NANO_OSAPI_UDPV4SOCKETPROPERTIES_INITIALIZER,\
    NANO_XRCE_TRANSPORTLOCATORMEDIUM_INITIALIZER
#else
#define NANO_XRCE_UDPV4CLIENTTRANSPORTPROPERTIES_METADATA_INITIALIZER
#endif /* NANO_FEAT_TRANSPORT_METADATA */

/**
 * @brief Static initialized for instances of
 * NANO_XRCE_Udpv4ClientTransportProperties.
 */
#define NANO_XRCE_UDPV4CLIENTTRANSPORTPROPERTIES_INITIALIZER \
{\
    NANO_XRCE_CLIENTTRANSPORTPROPERTIES_INITIALIZER, /* base */ \
    NANO_OSAPI_UDPV4SOCKETPROPERTIES_INITIALIZER, /* socket */ \
    NANO_XRCE_TRANSPORTLOCATORMEDIUM_INITIALIZER, /* bind_address */\
    NULL, /* recv_buffer */\
    0 /* recv_buffer_size */\
    NANO_XRCE_UDPV4CLIENTTRANSPORTPROPERTIES_METADATA_INITIALIZER /* discovery */\
}

/**
 * @brief TODO
 * 
 * @param self 
 * @param properties 
 * @return NANODllExport NANO_XRCE_Udpv4ClientTransport_initialize 
 */
NANODllExport
NANO_RetCode
NANO_XRCE_Udpv4ClientTransport_initialize(
    NANO_XRCE_ClientTransport *const self,
    const NANO_XRCE_ClientTransportListener *const listener,
    const NANO_XRCE_ClientTransportProperties *const properties);

/**
 * @brief TODO
 * 
 * @param self 
 * @return NANODllExport NANO_XRCE_Udpv4ClientTransport_finalize 
 */
NANODllExport
void
NANO_XRCE_Udpv4ClientTransport_finalize(
    NANO_XRCE_ClientTransport *const self);

NANODllExport
void
NANO_XRCE_Udpv4ClientTransport_flush_output(
    NANO_XRCE_ClientTransport *const self);

NANODllExport
NANO_RetCode
NANO_XRCE_Udpv4ClientTransport_process_input(
    NANO_XRCE_ClientTransport *const self,
    NANO_u32 max_messages,
    NANO_Timeout timeout_ms);

#if NANO_FEAT_RELIABILITY && \
    NANO_FEAT_RELIABLESTREAM_RECVQUEUE
NANODllExport
void
NANO_XRCE_Udpv4ClientTransport_return(
    NANO_XRCE_ClientTransport *const self,
    NANO_MessageBuffer *const msg);
#endif /* NANO_FEAT_RELIABILITY && \
            NANO_FEAT_RELIABLESTREAM_RECVQUEUE */

#if NANO_FEAT_TRANSPORT_UPDATE
NANODllExport
NANO_RetCode
NANO_XRCE_Udpv4ClientTransport_update_locator(
    NANO_XRCE_ClientTransport *const self,
    const NANO_XRCE_ClientTransportLocatorType locator_type,
    const NANO_XRCE_TransportLocator *const locator);

#endif /* NANO_FEAT_TRANSPORT_UPDATE */

/**
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_Udpv4ClientTransportI
{
    NANO_XRCE_ClientTransport base;
    /**
     * @brief A UDPv4 socket used for communication with the outside world.
     */
    NANO_OSAPI_Udpv4Socket sock;

    /**
     * @brief TODO
     * 
     */
    NANO_XRCE_TransportLocatorMedium agent_address;

#if NANO_FEAT_TRANSPORT_METADATA

    NANO_XRCE_TransportLocatorMedium metadata_address;
#endif /* NANO_FEAT_TRANSPORT_METADATA */

    NANO_MessageBuffer recv_msg;
    NANO_u16 recv_msg_size;
#if NANO_FEAT_SYSTIME
    NANO_OSAPI_Clock clock;
#endif /* NANO_FEAT_SYSTIME */

#if NANO_CPP
    NANO_XRCE_Udpv4ClientTransportI()
    {
        this->base.initialize = NANO_XRCE_Udpv4ClientTransport_initialize;
        this->base.finalize = NANO_XRCE_Udpv4ClientTransport_finalize;
        this->base.process_input = NANO_XRCE_Udpv4ClientTransport_process_input;
        this->base.flush_output = NANO_XRCE_Udpv4ClientTransport_flush_output;
#if NANO_FEAT_RELIABILITY && \
    NANO_FEAT_RELIABLESTREAM_RECVQUEUE
        this->base.process_input = NANO_XRCE_Udpv4ClientTransport_return;
#endif /* NANO_FEAT_RELIABILITY */
#if NANO_FEAT_TRANSPORT_UPDATE
        this->base.update_locator =
            NANO_XRCE_Udpv4ClientTransport_update_locator;
#endif /* NANO_FEAT_TRANSPORT_UPDATE */
    }

#endif /* NANO_CPP */

} NANO_XRCE_Udpv4ClientTransport;

#if NANO_FEAT_RELIABILITY && \
    NANO_FEAT_RELIABLESTREAM_RECVQUEUE
#define NANO_XRCE_UDPV4CLIENTTRANSPORT_RELIABILITY_INITIALIZER \
    NANO_XRCE_Udpv4ClientTransport_return,
#else
#define NANO_XRCE_UDPV4CLIENTTRANSPORT_RELIABILITY_INITIALIZER
#endif /* NANO_FEAT_RELIABILITY */

#if NANO_FEAT_TRANSPORT_METADATA
#define NANO_XRCE_UDPV4CLIENTTRANSPORT_METADATA_INITIALIZER \
    NANO_XRCE_UDPV4CLIENTTRANSPORT_METADATA_LOCATOR_INITIALIZER,
#else
#define NANO_XRCE_UDPV4CLIENTTRANSPORT_METADATA_INITIALIZER
#endif /* NANO_FEAT_TRANSPORT_METADATA */


#if NANO_FEAT_TRANSPORT_UPDATE
#define NANO_XRCE_UDPV4CLIENTTRANSPORT_UPDATE_INITIALIZER \
    NANO_XRCE_Udpv4ClientTransport_update_locator,
#else
#define NANO_XRCE_UDPV4CLIENTTRANSPORT_UPDATE_INITIALIZER
#endif /* NANO_FEAT_TRANSPORT_UPDATE */

#if NANO_FEAT_SYSTIME
#define NANO_XRCE_UDPV4CLIENTTRANSPORT_CLOCK_INITIALIZER \
    ,\
    NANO_OSAPI_CLOCK_INITIALIZER
#else
#define NANO_XRCE_UDPV4CLIENTTRANSPORT_CLOCK_INITIALIZER
#endif /* NANO_FEAT_SYSTIME */


#define NANO_XRCE_UDPV4CLIENTTRANSPORT_INITIALIZER \
{\
    {\
        NANO_XRCE_Udpv4ClientTransport_initialize, \
        NANO_XRCE_Udpv4ClientTransport_finalize, \
        NANO_XRCE_Udpv4ClientTransport_process_input, \
        NANO_XRCE_Udpv4ClientTransport_flush_output, \
        NANO_XRCE_UDPV4CLIENTTRANSPORT_RELIABILITY_INITIALIZER \
        NANO_XRCE_UDPV4CLIENTTRANSPORT_UPDATE_INITIALIZER \
        NANO_XRCE_CLIENTTRANSPORT_COMMON_INITIALIZER /* common fields */\
    }, /* base */ \
    NANO_OSAPI_UDPV4SOCKET_INITIALIZER, /* sock */ \
    NANO_XRCE_TRANSPORTLOCATORMEDIUM_INITIALIZER, /* agent_address */ \
    NANO_XRCE_UDPV4CLIENTTRANSPORT_METADATA_INITIALIZER /* discovery */\
    NANO_MESSAGEBUFFER_INITIALIZER, /* recv_msg */\
    0 /* recv_msg_size */ \
    NANO_XRCE_UDPV4CLIENTTRANSPORT_CLOCK_INITIALIZER /* clock */\
}

#endif /* NANO_FEAT_TRANSPORT_PLUGIN_UDPV4 */

#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif /* nano_core_xrce_transport_udpv4_h */
