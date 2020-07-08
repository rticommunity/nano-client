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

#include "nano/nano_client.h"

#if NANO_FEAT_EXTENDED_API

#if NANO_FEAT_TRANSPORT_PLUGIN_UDPV4

#include "nano/nano_client_udpv4.h"

NANODllExport
NANO_RetCode
NANO_XRCE_Udpv4Client_initialize(
    NANO_XRCE_Udpv4Client *const self,
    const NANO_u32 client_key,
    const NANO_u16 mtu,
    NANO_u8 *const recv_buffer,
    const NANO_usize recv_buffer_size,
    const NANO_u8 *const agent_addr,
    const NANO_u16 agent_port,
    const NANO_u8 *const bind_addr,
    const NANO_u16 bind_port)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_XRCE_Udpv4ClientTransportProperties udp_props =
        NANO_XRCE_UDPV4CLIENTTRANSPORTPROPERTIES_INITIALIZER;
    NANO_XRCE_ClientProperties client_props =
        NANO_XRCE_CLIENTPROPERTIES_INITIALIZER;
    
    NANO_LOG_FN_ENTRY
    
    NANO_PCOND(self != NULL)
    
    udp_props.base.mtu = mtu;
    udp_props.recv_buffer = (NANO_u8*) recv_buffer;
    udp_props.recv_buffer_size = recv_buffer_size;

    if (agent_addr != NULL)
    {
        udp_props.base.agent_address.format = NANO_XRCE_ADDRESS_FORMAT_MEDIUM;
        memcpy(
            udp_props.base.agent_address.value.medium.address,
            agent_addr,
            sizeof(NANO_Ipv4Addr));
    }
    udp_props.base.agent_address.value.medium.port = agent_port;
    
    if (bind_addr != NULL)
    {
        udp_props.base.agent_address.format = NANO_XRCE_ADDRESS_FORMAT_MEDIUM;
        memcpy(
            udp_props.bind_address.address,
            bind_addr,
            sizeof(NANO_Ipv4Addr));
    }
    udp_props.bind_address.port = bind_port;

    NANO_XRCE_ClientKey_from_u32(&client_props.key, client_key);

    client_props.storage = &self->storage.base;
    client_props.transport = &self->transport.base;
    client_props.transport_properties = &udp_props.base;

    NANO_XRCE_DefaultSimpleClientStorage_initialize(&self->storage);

    if (NANO_RETCODE_OK !=
            NANO_XRCE_Client_initialize(&self->client, &client_props))
    {
        goto done;
    }
    
    
    rc = NANO_RETCODE_OK;
    
done:
    if (NANO_RETCODE_OK != rc)
    {
        /* handle failure */
    }
    
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

#endif /* NANO_FEAT_TRANSPORT_PLUGIN_UDPV4 */


#endif /* NANO_FEAT_EXTENDED_API */