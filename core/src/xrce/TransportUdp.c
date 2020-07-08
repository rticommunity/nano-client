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

#include "nano/nano_core.h"
#include "nano/nano_core_xrce_transport_udpv4.h"

#if NANO_FEAT_TRANSPORT_PLUGIN_UDPV4

NANO_PRIVATE
const NANO_u32 ZERO = 0;

#define NANO_XRCE_Udpv4ClientTransport_valid_locator_medium(l_) \
    ((NANO_OSAPI_Memory_compare(\
        (l_)->address, &ZERO, sizeof(ZERO)) != 0) && (l_)->port > 0)


#define NANO_XRCE_Udpv4ClientTransport_agent_enabled(s_) \
    NANO_XRCE_Udpv4ClientTransport_valid_locator_medium(&(s_)->agent_address)

#if NANO_FEAT_TRANSPORT_METADATA

#define NANO_XRCE_Udpv4ClientTransport_metadata_enabled(s_) \
    NANO_XRCE_Udpv4ClientTransport_valid_locator_medium(&(s_)->metadata_address)


const NANO_XRCE_TransportLocatorMedium
    NANO_XRCE_UDPV4CLIENTTRANSPORT_DEFAULT_METADATA_LOCATOR =
    NANO_XRCE_UDPV4CLIENTTRANSPORT_METADATA_LOCATOR_INITIALIZER;
#endif /* NANO_FEAT_TRANSPORT_METADATA */


NANO_RetCode
NANO_XRCE_Udpv4ClientTransport_initialize(
    NANO_XRCE_ClientTransport *const transport,
    const NANO_XRCE_ClientTransportListener *const listener,
    const NANO_XRCE_ClientTransportProperties *const properties)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_XRCE_Udpv4ClientTransport *self =
        (NANO_XRCE_Udpv4ClientTransport*)transport;
    const NANO_XRCE_Udpv4ClientTransportProperties *u_props =
        (const NANO_XRCE_Udpv4ClientTransportProperties*)properties;
    const NANO_XRCE_TransportLocatorMedium *agent_address = NULL;
    NANO_XRCE_TransportLocatorMedium bind_address =
        NANO_XRCE_TRANSPORTLOCATORMEDIUM_INITIALIZER;
#if NANO_FEAT_TRANSPORT_METADATA
    const NANO_XRCE_TransportLocatorMedium *metadata_address = NULL;
    const NANO_u16 metadata_port = NANO_XRCE_TRANSPORT_UDP_METADATA_PORT_DEFAULT;
#endif /* NANO_FEAT_TRANSPORT_METADATA */

    NANO_LOG_FN_ENTRY

    NANO_PCOND(transport != NULL)
    NANO_PCOND(NANO_XRCE_ClientTransport_is_valid(transport))
    NANO_PCOND(properties != NULL)
    NANO_PCOND(properties->mtu > 0)
    NANO_PCOND(
        NANO_XRCE_ClientTransportListener_is_valid(listener))

    self->base.mtu = properties->mtu;
    self->base.send_queue = NULL;
    self->base.send_queue_len = 0;
    self->base.listener = *listener;

    /* We need a "medium" server address */
    agent_address = 
        NANO_XRCE_TransportLocatorMedium_narrow(&u_props->base.agent_address);
#if !NANO_FEAT_TRANSPORT_UPDATE
    if (agent_address == NULL)
    {
        NANO_LOG_ERROR("INVALID server address",
            NANO_LOG_LOCATOR("found", &u_props->base.agent_address))
        goto done;
    }
#else
    if (agent_address != NULL)
    {
#endif

    self->agent_address = *agent_address;
    self->agent_address.port =
        (self->agent_address.port > 0)?
            self->agent_address.port :
            NANO_XRCE_TRANSPORT_UDP_AGENT_PORT_DEFAULT;

#if NANO_FEAT_TRANSPORT_UPDATE
    }
#endif /* NANO_FEAT_TRANSPORT_UPDATE */

#if NANO_FEAT_TRANSPORT_METADATA
    /* Check if a metadata address was provided. If no metadata address is
       specified, then the transport will return an error if a METADATA
       message is added to the send queue */
    metadata_address = 
        NANO_XRCE_TransportLocatorMedium_narrow(&u_props->base.metadata_address);
    if (metadata_address != NULL)
    {
        self->metadata_address = *metadata_address;
    }
    self->metadata_address.port = 
        (self->metadata_address.port > 0)?
            self->metadata_address.port : metadata_port;

#endif /* NANO_FEAT_TRANSPORT_METADATA */

    NANO_CHECK_RC(
        NANO_XRCE_ClientTransport_initialize_recv_buffer(
            &self->recv_msg,
            u_props->recv_buffer,
            u_props->recv_buffer_size,
            self->base.mtu,
            &self->recv_msg_size),
        NANO_LOG_ERROR_MSG("FAILED to initialize receive buffer"));

    bind_address = u_props->bind_address;
    NANO_LOG_DEBUG("PRE opening UDP socket",
        NANO_LOG_LOCATOR_MEDIUM("bind_address",&bind_address))
    bind_address.port = 
        (bind_address.port > 0)?
            bind_address.port : NANO_XRCE_TRANSPORT_UDP_CLIENT_PORT_DEFAULT;

    NANO_LOG_DEBUG("opening UDP socket",
        NANO_LOG_LOCATOR_MEDIUM("bind_address",&bind_address))

    NANO_CHECK_RC(
        NANO_OSAPI_Udpv4Socket_open(
            &self->sock,
            bind_address.address,
            bind_address.port,
            &u_props->socket),
        NANO_LOG_ERROR("socket::open failed",
            NANO_LOG_LOCATOR_MEDIUM("bind_addr", &u_props->bind_address)));

    NANO_LOG_DEBUG("UDPv4 client transport OK",
        NANO_LOG_USIZE("mtu", self->base.mtu)
        NANO_LOG_LOCATOR_MEDIUM("server_addr",&self->agent_address)
        NANO_LOG_LOCATOR_MEDIUM("bind_addr",&u_props->bind_address)
        NANO_LOG_PTR("recv_buffer",
            NANO_MessageBuffer_data_ptr(&self->recv_msg))
        NANO_LOG_USIZE("recv_buffer_size",
            NANO_MessageBuffer_data_len(&self->recv_msg))
        )
    
    rc = NANO_RETCODE_OK;
    
done:
    if (NANO_RETCODE_OK != rc)
    {
        if (NANO_OSAPI_Udpv4Socket_is_valid(&self->sock))
        {
            NANO_OSAPI_Udpv4Socket_close(&self->sock);
        }
    }
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

void
NANO_XRCE_Udpv4ClientTransport_finalize(
    NANO_XRCE_ClientTransport *const transport)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_XRCE_Udpv4ClientTransport *self =
        (NANO_XRCE_Udpv4ClientTransport*)transport;
    
    NANO_LOG_FN_ENTRY

    NANO_PCOND(transport != NULL)
    
    if (NANO_OSAPI_Udpv4Socket_is_valid(&self->sock))
    {
        NANO_CHECK_RC(
            NANO_OSAPI_Udpv4Socket_close(&self->sock),
            NANO_LOG_ERROR("socket::close failed", NANO_LOG_RC(rc)));
    }

    rc = NANO_RETCODE_OK;

done:
    if (rc != NANO_RETCODE_OK)
    {

    }
    NANO_LOG_FN_EXIT
}

NANO_PRIVATE
NANO_RetCode
NANO_XRCE_Udpv4ClientTransport_send(
    NANO_XRCE_Udpv4ClientTransport *const self,
    NANO_MessageBuffer *const msg)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_usize data_len = 0;
    const NANO_XRCE_TransportLocatorMedium *locator = NULL;
    
    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)
    NANO_PCOND(msg != NULL)
    NANO_PCOND(NANO_OSAPI_Udpv4Socket_is_valid(&self->sock))

    NANO_MessageBuffer_data_len_msg(msg, &data_len);

    if (data_len > self->base.mtu)
    {
        NANO_LOG_ERROR("message exceeds MTU",
            NANO_LOG_USIZE("msg_len", data_len)
            NANO_LOG_USIZE("mtu", self->base.mtu))
        rc = NANO_RETCODE_INVALID_ARGS;
        goto done;
    }

#if NANO_FEAT_TRANSPORT_METADATA
    if (NANO_MessageBuffer_flags(msg, 
            NANO_XRCE_MESSAGEFLAGS_DISCOVERY))
    {
        if (!NANO_XRCE_Udpv4ClientTransport_metadata_enabled(self))
        {
            NANO_LOG_ERROR_MSG("no address for METADATA")
            goto done;
        }
        NANO_LOG_TRACE("sending on METADATA locator",
            NANO_LOG_LOCATOR_MEDIUM("locator",&self->metadata_address))
        locator = &self->metadata_address;
    }
    else
    {
        if (!NANO_XRCE_Udpv4ClientTransport_agent_enabled(self))
        {
            NANO_LOG_ERROR_MSG("no address for AGENT")
            goto done;
        }
#endif /* NANO_FEAT_TRANSPORT_METADATA */
        NANO_LOG_TRACE("sending on AGENT locator",
            NANO_LOG_LOCATOR_MEDIUM("locator",&self->agent_address))
        locator = &self->agent_address;
#if NANO_FEAT_TRANSPORT_METADATA
    }
#endif /* NANO_FEAT_TRANSPORT_METADATA */

    NANO_CHECK_RC(
        NANO_OSAPI_Udpv4Socket_send(
            &self->sock,
            locator->address,
            locator->port,
            msg),
        NANO_LOG_ERROR("socket::send failed",
            NANO_LOG_LOCATOR_MEDIUM("locator",locator)));
    
    NANO_LOG_TRACE("UDP message sent", 
        NANO_LOG_LOCATOR_MEDIUM("locator",locator)
        NANO_LOG_MSGHDR("hdr",
            *((NANO_XRCE_MessageHeader*)NANO_MessageBuffer_data_ptr(msg)))
        NANO_LOG_SUBMSGHDR("submsg",
            *((NANO_XRCE_SubmessageHeader*)NANO_XRCE_InlineHeaderBuffer_submsgid_ptr(msg)))
        NANO_LOG_MBUF("mbuf",msg))
    
    rc = NANO_RETCODE_OK;
    
done:
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

#if !(NANO_FEAT_RELIABILITY && \
        NANO_FEAT_RELIABLESTREAM_SENDQUEUE)
NANO_PRIVATE
#endif
void
NANO_XRCE_Udpv4ClientTransport_return(
    NANO_XRCE_ClientTransport *const transport,
    NANO_MessageBuffer *const msg)
{
    NANO_XRCE_Udpv4ClientTransport *const self =
        (NANO_XRCE_Udpv4ClientTransport*)transport;

    NANO_LOG_FN_ENTRY

    NANO_PCOND(transport != NULL)

    NANO_PCOND(msg == &self->recv_msg)
    UNUSED_ARG(msg);

    NANO_PCOND(NANO_MessageBuffer_flags(
        &self->recv_msg, NANO_XRCE_MESSAGEFLAGS_OWNED_BY_TRANSPORT))
    
    NANO_MessageBuffer_flags_unset(
        &self->recv_msg,
        NANO_XRCE_MESSAGEFLAGS_OWNED_BY_TRANSPORT);
    
    NANO_MessageBuffer_set_data_len(&self->recv_msg, self->recv_msg_size);
    NANO_OSAPI_Memory_zero(
        NANO_MessageBuffer_data_ptr(&self->recv_msg),
        NANO_MessageBuffer_data_len(&self->recv_msg));

    // NANO_LOG_TRACE_MSG("receive buffer returned")

    NANO_LOG_FN_EXIT
}

NANO_PRIVATE
NANO_MessageBuffer *
NANO_XRCE_Udpv4ClientTransport_allocate_message(
    NANO_XRCE_Udpv4ClientTransport *const self)
{
    NANO_MessageBuffer *msg = NULL;

    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)

    if (NANO_MessageBuffer_flags(
            &self->recv_msg, NANO_XRCE_MESSAGEFLAGS_OWNED_BY_TRANSPORT))
    {
        NANO_LOG_ERROR_MSG("receive buffer already in use")
        goto done;
    }

    NANO_MessageBuffer_flags_set(
        &self->recv_msg, NANO_XRCE_MESSAGEFLAGS_OWNED_BY_TRANSPORT);
    
    NANO_MessageBuffer_set_data_len(&self->recv_msg, self->recv_msg_size);

    msg = &self->recv_msg;

done:
    NANO_LOG_FN_EXIT_PTR(msg)

    return msg;
}

void
NANO_XRCE_Udpv4ClientTransport_flush_output(
    NANO_XRCE_ClientTransport *const transport)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_XRCE_Udpv4ClientTransport *const self =
        (NANO_XRCE_Udpv4ClientTransport*)transport;
    NANO_MessageBuffer *msg = NULL; 

    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)
    
    while (NANO_XRCE_ClientTransport_pending_send(&self->base))
    {
        msg = NANO_XRCE_ClientTransport_next_send(&self->base);
        NANO_PCOND(msg != NULL)
        rc = NANO_XRCE_Udpv4ClientTransport_send(self, msg);
        NANO_XRCE_ClientTransport_send_complete(&self->base, 0, msg, rc);
    }

    NANO_LOG_FN_EXIT
}

NANODllExport
NANO_RetCode
NANO_XRCE_Udpv4ClientTransport_process_input(
    NANO_XRCE_ClientTransport *const transport,
    NANO_u32 max_messages,
    NANO_Timeout timeout_ms)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_XRCE_Udpv4ClientTransport *const self =
        (NANO_XRCE_Udpv4ClientTransport*)transport;
    NANO_XRCE_TransportLocatorMedium src =
            NANO_XRCE_TRANSPORTLOCATORMEDIUM_INITIALIZER;
    NANO_MessageBuffer *msg = NULL; 
    NANO_usize remaining = 0,
               msg_len = 0;
    NANO_bool retained = NANO_BOOL_FALSE;
#if NANO_FEAT_SYSTIME
    NANO_u64 ms_start = 0,
             ms_now = 0,
             ms_diff = 0;
#endif /* NANO_FEAT_SYSTIME */

    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)
    NANO_PCOND(max_messages > 0)

#if NANO_FEAT_SYSTIME
    NANO_OSAPI_Clock_millis(&self->clock, &ms_start);
#endif /* NANO_FEAT_SYSTIME */

    // if (timeout_ms == 0)
    // {
    //     NANO_LOG_WARNING_MSG("ZERO timeout");
    // }
    for (remaining = max_messages; remaining > 0; retained = NANO_BOOL_FALSE)
    {
        msg = NANO_XRCE_Udpv4ClientTransport_allocate_message(self);
        if (msg == NULL)
        {
            NANO_LOG_ERROR("FAILED to allocate receive message",
                NANO_LOG_RC(rc));
            goto done;
        }

        NANO_CHECK_RC(
            NANO_OSAPI_Udpv4Socket_recv(
                &self->sock,
                src.address,
                &src.port,
                msg,
                &msg_len),
            NANO_LOG_ERROR("socket::recv failed",NANO_LOG_RC(rc)));
        NANO_PCOND(msg_len <= self->base.mtu)

        if ((msg_len >= NANO_XRCE_MESSAGE_SERIALIZED_SIZE_MIN))
        {
            NANO_LOG_TRACE("possible XRCE(UDP) data RECEIVED",
                    NANO_LOG_LOCATOR_MEDIUM("src",&src)
                    NANO_LOG_USIZE("size",msg_len))

            remaining -= 1;

            NANO_XRCE_ClientTransportListener_on_message_received(
                &self->base.listener,
                transport,
                msg,
                msg_len,
                &retained);
        }
        else
        {
            // NANO_LOG_TRACE("UDP data IGNORED",
            //     NANO_LOG_LOCATOR_MEDIUM("src",&src)
            //     NANO_LOG_USIZE("size",msg_len))
        }

#if NANO_FEAT_RELIABILITY && \
    NANO_FEAT_RELIABLESTREAM_SENDQUEUE
        NANO_PCOND(!retained)
#endif /* NANO_FEAT_RELIABILITY && \
        NANO_FEAT_RELIABLESTREAM_SENDQUEUE */
        
        if (!retained)
        {
            NANO_XRCE_Udpv4ClientTransport_return(transport, msg);
        }
        else
        {
            NANO_LOG_WARNING("MESSAGE RETAINED",
                NANO_LOG_MBUF("msg", msg))
        }
        

#if NANO_FEAT_SYSTIME
        if (timeout_ms > 0 && remaining > 0)
        {
            NANO_OSAPI_Clock_millis(&self->clock, &ms_now);
            ms_diff = ms_now - ms_start;
            if (ms_diff >= (NANO_u64)(timeout_ms))
            {
                NANO_LOG_DEBUG("TIMEOUT exceeded",
                    NANO_LOG_I32("timeout",timeout_ms)
                    NANO_LOG_U32("passed",ms_diff))
                rc = NANO_RETCODE_TIMEOUT;
                goto done;
            }
        }
#endif /* NANO_FEAT_SYSTIME */
        if (timeout_ms == 0)
        {
            break;
        }
    }
    
    rc = NANO_RETCODE_OK;
    
done:
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}



#if NANO_FEAT_TRANSPORT_UPDATE
NANO_RetCode
NANO_XRCE_Udpv4ClientTransport_update_locator(
    NANO_XRCE_ClientTransport *const transport,
    const NANO_XRCE_ClientTransportLocatorType locator_type,
    const NANO_XRCE_TransportLocator *const locator)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    const NANO_XRCE_TransportLocatorMedium *locator_med = NULL;
    NANO_XRCE_Udpv4ClientTransport *const self =
        (NANO_XRCE_Udpv4ClientTransport*)transport;

    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)
    NANO_PCOND(NANO_XRCE_ClientTransportLocatorType_is_valid(locator_type))

    NANO_LOG_DEBUG("UPDATE locator",
        NANO_LOG_ENUM("type", locator_type)
        NANO_LOG_PTR("locator",locator))
    

    if (locator != NULL)
    {
        locator_med = 
            NANO_XRCE_TransportLocatorMedium_narrow(locator);
        if (locator_med == NULL)
        {
            NANO_LOG_ERROR("INVALID locator, expected medium locator",
                NANO_LOG_LOCATOR("found", locator))
            goto done;
        }
        if (!NANO_XRCE_Udpv4ClientTransport_valid_locator_medium(locator_med))
        {
            NANO_LOG_ERROR("INVALID locator",
                NANO_LOG_LOCATOR("locator", locator))
            goto done;
        }
        switch (locator_type)
        {
        case NANO_XRCE_CLIENTTRANSPORTLOCATOR_AGENT:
        {
            self->agent_address = *locator_med;
            NANO_LOG_DEBUG("UPDATED agent locator",
                NANO_LOG_LOCATOR_MEDIUM("address",&self->agent_address))
            break;
        }
#if NANO_FEAT_TRANSPORT_METADATA
        case NANO_XRCE_CLIENTTRANSPORTLOCATOR_METADATA:
        {
            self->metadata_address = *locator_med;
            NANO_LOG_DEBUG("UPDATED metadata locator",
                NANO_LOG_LOCATOR_MEDIUM("address",&self->metadata_address))
            break;
        }
#endif /* NANO_FEAT_TRANSPORT_METADATA */
        default:
        {
            NANO_UNREACHABLE_CODE
            goto done;
        }
        }
    }
    else
    {
        switch (locator_type)
        {
        case NANO_XRCE_CLIENTTRANSPORTLOCATOR_AGENT:
        {
            NANO_OSAPI_Memory_zero(
                &self->agent_address, sizeof(self->agent_address));
            NANO_LOG_DEBUG_MSG("RESET agent locator")
            break;
        }
#if NANO_FEAT_TRANSPORT_METADATA
        case NANO_XRCE_CLIENTTRANSPORTLOCATOR_METADATA:
        {
            NANO_OSAPI_Memory_zero(
                &self->metadata_address, sizeof(self->metadata_address));
            NANO_LOG_DEBUG_MSG("RESET metadata locator")
            break;
        }
#endif /* NANO_FEAT_TRANSPORT_METADATA */
        default:
        {
            NANO_UNREACHABLE_CODE
            goto done;
        }
        }
    }

    
    rc = NANO_RETCODE_OK;
    
done:

    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

#endif /* #if NANO_FEAT_TRANSPORT_UPDATE */

#endif /* NANO_FEAT_TRANSPORT_PLUGIN_UDPV4 */