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

#if NANO_FEAT_TRANSPORT_PLUGIN_SERIAL

#include "nano/nano_client_serial.h"

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
    const NANO_u8 bind_addr)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_XRCE_SerialClientTransportProperties serial_props =
        NANO_XRCE_SERIALCLIENTTRANSPORTPROPERTIES_INITIALIZER;
    NANO_XRCE_ClientProperties client_props =
        NANO_XRCE_CLIENTPROPERTIES_INITIALIZER;
    
    NANO_LOG_FN_ENTRY
    
    NANO_PCOND(self != NULL)
    
    serial_props.base.mtu = mtu;
    serial_props.recv_buffer = (NANO_u8*) recv_buffer;
    serial_props.recv_buffer_size = recv_buffer_size;
#if defined(NANO_HAVE_STD_SERIAL)
    serial_props.connection.device = serial_port;
    serial_props.connection.speed = serial_speed;
#elif defined(NANO_HAVE_ARDUINO_SERIAL)
    if (strcmp(serial_port, "0") == 0)
    {
        serial_props.connection.port = NANO_ARDUINO_SERIAL_PORT_0;
    }
    else if (strcmp(serial_port, "1") == 0)
    {
        serial_props.connection.port = NANO_ARDUINO_SERIAL_PORT_1;
    }
    serial_props.connection.speed = serial_speed;
#endif

    if (agent_addr != 0)
    {
        serial_props.base.agent_address.format = NANO_XRCE_ADDRESS_FORMAT_SMALL;
        serial_props.base.agent_address.value.small.address[0] = agent_addr;
    }

    if (bind_addr != 0)
    {
        serial_props.bind_address.address[0] = bind_addr;
    }
    
    NANO_XRCE_ClientKey_from_u32(&client_props.key, client_key);

    client_props.storage = &self->storage.base;
    client_props.transport = &self->transport.base;
    client_props.transport_properties = &serial_props.base;

    NANO_XRCE_DefaultClientStorage_initialize(&self->storage);

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

#endif /* NANO_FEAT_TRANSPORT_PLUGIN_SERIAL */


#endif /* NANO_FEAT_EXTENDED_API */