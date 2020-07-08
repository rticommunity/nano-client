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

#ifndef nano_core_xrce_transport_serial_h
#define nano_core_xrce_transport_serial_h

#if NANO_FEAT_TRANSPORT_PLUGIN_SERIAL

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NANODllExport NANO_XRCE_SerialClientTransportPropertiesI
{
    NANO_XRCE_ClientTransportProperties base;
    NANO_OSAPI_SerialConnectionProperties connection;
    NANO_u8 *recv_buffer;
    NANO_usize recv_buffer_size;
    NANO_XRCE_TransportLocatorSmall bind_address;

#if NANO_CPP
    NANO_XRCE_SerialClientTransportPropertiesI():
        base(),
        connection(),
        recv_buffer(NULL),
        recv_buffer_size(0),
        bind_address()
    {

    }
#endif /*NANO_CPP */

} NANO_XRCE_SerialClientTransportProperties;

#define NANO_XRCE_SERIALCLIENTTRANSPORTPROPERTIES_INITIALIZER \
{\
    NANO_XRCE_CLIENTTRANSPORTPROPERTIES_INITIALIZER, /* base */\
    NANO_OSAPI_SERIALCONNECTIONPROPERTIES_INITIALIZER, /* connection */\
    NULL, /* recv_buffer */\
    0, /* recv_buffer_size */\
    NANO_XRCE_TRANSPORTLOCATORSMALL_INITIALIZER /* bind_address */\
}

typedef struct NANODllExport NANO_XRCE_SerialClientTransportI
    NANO_XRCE_SerialClientTransport;

/**
 * @brief TODO
 * 
 * @param self 
 * @param properties 
 * @return NANODllExport NANO_XRCE_SerialClientTransport_initialize 
 */
NANODllExport
NANO_RetCode
NANO_XRCE_SerialClientTransport_initialize(
    NANO_XRCE_ClientTransport *const self,
    const NANO_XRCE_ClientTransportListener *const listener,
    const NANO_XRCE_ClientTransportProperties *const properties);

/**
 * @brief TODO
 * 
 * @param self 
 * @return NANODllExport NANO_XRCE_SerialClientTransport_finalize 
 */
NANODllExport
void
NANO_XRCE_SerialClientTransport_finalize(
    NANO_XRCE_ClientTransport *const self);

NANODllExport
void
NANO_XRCE_SerialClientTransport_flush_output(
    NANO_XRCE_ClientTransport *const self);

NANODllExport
NANO_RetCode
NANO_XRCE_SerialClientTransport_process_input(
    NANO_XRCE_ClientTransport *const self,
    NANO_u32 max_messages,
    NANO_Timeout timeout_ms);

#if NANO_FEAT_RELIABILITY && \
    NANO_FEAT_RELIABLESTREAM_RECVQUEUE
NANODllExport
void
NANO_XRCE_SerialClientTransport_return(
    NANO_XRCE_ClientTransport *const self,
    NANO_MessageBuffer *const msg);
#endif /* NANO_FEAT_RELIABILITY && \
            NANO_FEAT_RELIABLESTREAM_RECVQUEUE */

#if NANO_FEAT_TRANSPORT_UPDATE
NANODllExport
NANO_RetCode
NANO_XRCE_SerialClientTransport_update_locator(
    NANO_XRCE_ClientTransport *const self,
    const NANO_XRCE_ClientTransportLocatorType locator_type,
    const NANO_XRCE_TransportLocator *const locator);

#endif /* NANO_FEAT_TRANSPORT_UPDATE */

#if NANO_FEAT_AGENT
NANO_RetCode
NANO_XRCE_SerialClientTransport_receive_frame(
    NANO_XRCE_SerialClientTransport *const self,
    NANO_MessageBuffer *const recv_buf,
    const NANO_Timeout timeout_ms,
    NANO_u16 *const frame_len_out,
    NANO_u8 *const src_addr_out);

NANO_RetCode
NANO_XRCE_SerialClientTransport_send(
    NANO_XRCE_SerialClientTransport *const self,
    const NANO_XRCE_TransportLocatorSmall *const src_addr,
    const NANO_XRCE_TransportLocatorSmall *const dst_addr,
    NANO_MessageBuffer *const msg);
#endif

#define NANO_XRCE_FRAME_HEADER_SIZE \
(\
    sizeof(NANO_u8) /* BEGIN_FLAG */ +\
    sizeof(NANO_u8) /* src_addr */ +\
    sizeof(NANO_u8) /* dst_addr */ +\
    sizeof(NANO_u16) /* payload_len */ \
)

#define NANO_XRCE_FRAME_HEADER_BUFFER_SIZE \
    (NANO_OSAPI_Memory_align_size_up(\
        NANO_XRCE_FRAME_HEADER_SIZE, sizeof(NANO_MessageBufferData)) / \
            sizeof(NANO_MessageBufferData))

#define NANO_XRCE_FRAME_FOOTER_SIZE \
(\
    sizeof(NANO_u16) /* frame_check */ \
)

#define NANO_XRCE_FRAME_FOOTER_BUFFER_SIZE \
    (NANO_OSAPI_Memory_align_size_up(\
        NANO_XRCE_FRAME_FOOTER_SIZE, sizeof(NANO_MessageBufferData)) / \
            sizeof(NANO_MessageBufferData))

#define NANO_XRCE_SERIAL_READ_BUFFER_SIZE \
    (NANO_OSAPI_Memory_align_size_up(\
        NANO_LIMIT_SERIALCLIENTTRANSPORT_READ_BUFFER_SIZE,\
        sizeof(NANO_MessageBufferData)) / \
            sizeof(NANO_MessageBufferData))

struct NANODllExport NANO_XRCE_SerialClientTransportI
{
    NANO_XRCE_ClientTransport base;
    NANO_OSAPI_SerialConnection connection;
    NANO_MessageBuffer recv_msg;
    NANO_u16 recv_msg_size;
    NANO_XRCE_TransportLocatorSmall bind_address;
    NANO_XRCE_TransportLocatorSmall agent_address;
    NANO_MessageBufferData frame_header_in[NANO_XRCE_FRAME_HEADER_BUFFER_SIZE];
    NANO_MessageBufferData frame_header_out[NANO_XRCE_FRAME_HEADER_BUFFER_SIZE];
    NANO_MessageBufferData read_buffer[NANO_XRCE_SERIAL_READ_BUFFER_SIZE];
#if NANO_FEAT_SYSTIME
    NANO_OSAPI_Clock clock;
#endif /* NANO_FEAT_SYSTIME */

#if NANO_CPP
    NANO_XRCE_SerialClientTransportI():
        base(),
        connection(),
        recv_msg(),
        recv_msg_size(),
        bind_address(),
        agent_address(),
        frame_header_in(),
        frame_header_out(),
        read_buffer()
#if NANO_FEAT_SYSTIME
        ,
        clock()
#endif /* NANO_FEAT_SYSTIME */
    {
        this->base.initialize = NANO_XRCE_SerialClientTransport_initialize;
        this->base.finalize = NANO_XRCE_SerialClientTransport_finalize;
        this->base.process_input = NANO_XRCE_SerialClientTransport_process_input;
        this->base.flush_output = NANO_XRCE_SerialClientTransport_flush_output;
#if NANO_FEAT_RELIABILITY && \
    NANO_FEAT_RELIABLESTREAM_RECVQUEUE
        this->base.process_input = NANO_XRCE_SerialClientTransport_return;
#endif /* NANO_FEAT_RELIABILITY */
#if NANO_FEAT_TRANSPORT_UPDATE
        this->base.update_locator =
            NANO_XRCE_SerialClientTransport_update_locator;
#endif /* NANO_FEAT_TRANSPORT_UPDATE */
    }
#endif /*NANO_CPP */

};

#if NANO_FEAT_RELIABILITY && \
    NANO_FEAT_RELIABLESTREAM_RECVQUEUE
#define NANO_XRCE_SERIALCLIENTTRANSPORT_RELIABILITY_INITIALIZER \
    NANO_XRCE_SerialClientTransport_return,
#else
#define NANO_XRCE_SERIALCLIENTTRANSPORT_RELIABILITY_INITIALIZER
#endif /* NANO_FEAT_RELIABILITY */

#if NANO_FEAT_TRANSPORT_UPDATE
#define NANO_XRCE_SERIALCLIENTTRANSPORT_UPDATE_INITIALIZER \
    NANO_XRCE_SerialClientTransport_update_locator,
#else
#define NANO_XRCE_SERIALCLIENTTRANSPORT_UPDATE_INITIALIZER
#endif /* NANO_FEAT_TRANSPORT_UPDATE */

#if NANO_FEAT_SYSTIME
#define NANO_XRCE_SERIALCLIENTTRANSPORT_CLOCK_INITIALIZER \
    ,\
    NANO_OSAPI_CLOCK_INITIALIZER
#else
#define NANO_XRCE_SERIALCLIENTTRANSPORT_CLOCK_INITIALIZER
#endif /* NANO_FEAT_SYSTIME */

#define NANO_XRCE_SERIALCLIENTTRANSPORT_INITIALIZER \
{\
    {\
        NANO_XRCE_SerialClientTransport_initialize, \
        NANO_XRCE_SerialClientTransport_finalize, \
        NANO_XRCE_SerialClientTransport_process_input, \
        NANO_XRCE_SerialClientTransport_flush_output, \
        NANO_XRCE_SERIALCLIENTTRANSPORT_RELIABILITY_INITIALIZER \
        NANO_XRCE_SERIALCLIENTTRANSPORT_UPDATE_INITIALIZER \
        NANO_XRCE_CLIENTTRANSPORT_COMMON_INITIALIZER /* common fields */\
    }, /* base */ \
    NANO_OSAPI_SERIALCONNECTION_INITIALIZER, /* connection */\
    NANO_MESSAGEBUFFER_INITIALIZER, /* recv_msg */\
    0, /* recv_msg_size */\
    NANO_XRCE_TRANSPORTLOCATORSMALL_INITIALIZER, /* bind_address */\
    NANO_XRCE_TRANSPORTLOCATORSMALL_INITIALIZER, /* agent_address */\
    { 0 }, /* frame_header_out */\
    { 0 }, /* frame_header_in */\
    { 0 }  /* read_buffer */\
    NANO_XRCE_SERIALCLIENTTRANSPORT_CLOCK_INITIALIZER /* clock */\
}

#endif /* #if NANO_FEAT_TRANSPORT_PLUGIN_SERIAL  */


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif /* nano_core_xrce_transport_serial_h */