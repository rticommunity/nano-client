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

/******************************************************************************
 Start nanoagentd with the following command (change paths and other arguments
 according to your environment):
 
   nanoagentd -U -a -c resource/xml/examples/shape_agent.xml
 
 ******************************************************************************/

#include "drone-api.h"

#define BIND_PORT                   7403
#define CLIENT_KEY                  0x0A0B0C0D

NANO_u32 sensor_data_updates = 0;

void 
on_data(
    NANO_XRCE_ClientListener *const self,
    NANO_XRCE_Client *const client,
    const NANO_XRCE_StreamId stream_id,
    const NANO_XRCE_RequestId *const read_request,
    const NANO_XRCE_ObjectId *const reader,
    const NANO_XRCE_DataFormat format,
    const NANO_bool little_endian,
    const NANO_u8 *const data,
    const NANO_usize data_len);

int main(int argc, char const *argv[])
{
    int rc = 1;

    const NANO_u8 agent_addr[4] = AGENT_ADDRESS;

    NANO_MessageBufferData recv_buffer[
        RECV_BUFFER_SIZE / sizeof(NANO_MessageBufferData)] = { 0 };

    NANO_XRCE_Udpv4Client udp_client = NANO_XRCE_UDPV4CLIENT_INITIALIZER;
    NANO_XRCE_Client *const client = &udp_client.client;

#if RELIABLE
    NANO_XRCE_DefaultReliableStreamStorage
        stream_storage_sensor_data =
            NANO_XRCE_DEFAULTRELIABLESTREAMSTORAGE_INITIALIZER;
#else
    NANO_XRCE_DefaultBestEffortStreamStorage
        stream_storage_sensor_data =
            NANO_XRCE_DEFAULTBESTEFFORTSTREAMSTORAGE_INITIALIZER;
#endif /* RELIABLE */
        
    NANO_XRCE_ClientRequestToken
        read_req_sensor_data = NANO_XRCE_CLIENTREQUESTTOKEN_INITIALIZER;

    NANO_RetCode retcode = NANO_RETCODE_ERROR;

    NANO_LOG_SET_LEVEL(LOG_LEVEL)

    if (NANO_RETCODE_OK != 
            NANO_XRCE_Udpv4Client_initialize(
                &udp_client,
                CLIENT_KEY,
                TRANSPORT_MTU,
                (NANO_u8*)recv_buffer,
                sizeof(recv_buffer),
                agent_addr,
                0 /* default agent port */,
                NULL /* default bind address */,
                BIND_PORT /* custom bind port */))
    {
        printf("ERROR: failed to initialize client\n");
        goto done;
    }

    NANO_XRCE_Client_on_data(client, on_data, NULL);

#if RELIABLE
    NANO_XRCE_DefaultReliableStreamStorage_initialize(
        &stream_storage_sensor_data);
#else
    NANO_XRCE_DefaultBestEffortStreamStorage_initialize(
        &stream_storage_sensor_data);
#endif /* RELIABLE */

    NANO_XRCE_Client_liveliness_assertion_period(client, 2000);

    NANO_XRCE_Client_enable_stream(
        client, SENSOR_DATA_STREAM_ID, &stream_storage_sensor_data.base);

    do
    {
        retcode = NANO_XRCE_Client_connect(client, REQUEST_TIMEOUT);
        
        if (NANO_RETCODE_OK != retcode && NANO_RETCODE_TIMEOUT != retcode)
        {
            printf("ERROR: failed to connect to agent\n");
            goto done;
        }
    } while (!NANO_XRCE_Client_connected(client));

    if (NANO_RETCODE_OK != 
            NANO_XRCE_Client_read_data(
                client,
                &read_req_sensor_data,
                /* stream for the read request */
                NANO_XRCE_STREAMID_BUILTIN_RELIABLE,
                /* default behavior (block until ack) */
                NANO_XRCE_REQUESTFLAGS_SYNC,
                REQUEST_TIMEOUT,
                SENSOR_DATA_READER_ID,
                /* stream on which data will be forwarded */
                SENSOR_DATA_STREAM_ID,
                NULL /* no data delivery ctrl */,
                NULL /* no content filter */))
    {
        printf("ERROR: failed to request circles from Agent\n");
        goto disconnect;
    }

    while (MAX_SAMPLES <= 0 || sensor_data_updates < MAX_SAMPLES)
    {

        retcode = NANO_XRCE_Client_run_session(client, 0, RECV_DATA_TIMEOUT);

        if (NANO_RETCODE_TIMEOUT == retcode)
        {
            printf("TIMEOUT: while waiting for data from agent\n");
        }
        else if (NANO_RETCODE_OK != retcode)
        {
            printf("ERROR: failed to wait for data from agent\n");
            goto disconnect;
        }
    }

disconnect:

    if (NANO_RETCODE_OK !=
            NANO_XRCE_Client_disconnect(
                client,
                NULL /* req_token */,
                NANO_XRCE_STREAMID_BUILTIN_RELIABLE,
                NANO_XRCE_REQUESTFLAGS_SYNC,
                REQUEST_TIMEOUT))
    {
        printf("ERROR: failed to disconnect from Agent\n");
        goto done;
    }

    NANO_XRCE_Client_finalize(client);

    rc = 0;
done:
    return rc;
}

void 
on_data(
    NANO_XRCE_ClientListener *const self,
    NANO_XRCE_Client *const client,
    const NANO_XRCE_StreamId stream_id,
    const NANO_XRCE_RequestId *const read_request,
    const NANO_XRCE_ObjectId *const reader,
    const NANO_XRCE_DataFormat format,
    const NANO_bool little_endian,
    const NANO_u8 *const data,
    const NANO_usize data_len)
{
    struct SensorCombined sensor_data = SensorCombined_INITIALIZER;
    const NANO_u16 reader_id = NANO_XRCE_ObjectId_to_u16(reader);
    
    deserialize_sensor_data(&sensor_data, data, data_len, little_endian);
    sensor_data_updates += 1;
    printf("received data: %u\n", sensor_data_updates);
}
