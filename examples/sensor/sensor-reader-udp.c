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
 
   nanoagentd -U -a -c resource/xml/examples/sensor_agent.xml
 
 ******************************************************************************/

#include "nano/nano_client_udpv4.h"

#include <stdio.h>

#define AGENT_ADDRESS           { 127, 0, 0, 1 }
#define BIND_PORT               7403

#define CLIENT_KEY              0x05060708

#define READER_ID               0x5BB6

#define READER_RELIABLE         1

#define LOG_LEVEL               LEVEL_INFO

#define REQUEST_TIMEOUT         10000 /* 10s */
#define MAX_SAMPLES             10
#define LOOP_TIMEOUT            1000 /* 1s */

#define TRANSPORT_MTU           128

#define RECV_BUFFER_SIZE \
    (NANO_OSAPI_Memory_align_size_up(\
        TRANSPORT_MTU, sizeof(NANO_MessageBufferData)))

struct SensorData
{
    NANO_u8 id[4];
    NANO_u32 value;
};

#define SensorData_INITIALIZER      { { 0 }, 0 }

static
void on_data(
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
    struct SensorData sensor = SensorData_INITIALIZER;
    const NANO_u8 *data_ptr = data;
    NANO_u32 *received_samples = (NANO_u32 *)self->user_data;

    /* Deserialize 4 byte SensorData.id */
    memcpy(sensor.id, data_ptr, sizeof(NANO_u32));
    data_ptr += sizeof(NANO_u32);
    
    /* Deserialize 32-bit SensorData.value */
    NANO_u32_deserialize(&sensor.value, data_ptr, little_endian);
    
    *received_samples += 1;

    printf("Received sample %u/%u: id=%02X%02X%02X%02X, value=%u\n",
        *received_samples,
        MAX_SAMPLES,
        sensor.id[0], sensor.id[1], sensor.id[2], sensor.id[3],
        sensor.value);
}

int main(int argc, char const *argv[])
{
    int rc = 1;

    const NANO_u8 agent_addr[4] = AGENT_ADDRESS;

    NANO_MessageBufferData recv_buffer[
        RECV_BUFFER_SIZE / sizeof(NANO_MessageBufferData)] = { 0 };

    NANO_XRCE_Udpv4Client udp_client = NANO_XRCE_UDPV4CLIENT_INITIALIZER;
    NANO_XRCE_Client *const client = &udp_client.client;

    NANO_XRCE_ClientRequestToken read_req =
        NANO_XRCE_CLIENTREQUESTTOKEN_INITIALIZER;

    NANO_RetCode retcode = NANO_RETCODE_ERROR;

    NANO_u32 received_samples = 0;

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

    NANO_XRCE_Client_liveliness_assertion_period(client, 2000);

    NANO_XRCE_Client_on_data(client, on_data, &received_samples);

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
                &read_req,
                /* stream for the read request */
                NANO_XRCE_STREAMID_BUILTIN_RELIABLE,
                /* block until ack. Add NANO_XRCE_REQUESTFLAGS_CONFIRM to
                   request explicit confirmation from agent */
                NANO_XRCE_REQUESTFLAGS_SYNC,
                REQUEST_TIMEOUT,
                READER_ID,
                /* stream on which data will be forwarded */
#if READER_RELIABLE
                NANO_XRCE_STREAMID_BUILTIN_RELIABLE,
#else
                NANO_XRCE_STREAMID_BUILTIN_BEST_EFFORTS,
#endif
                NULL /* no data delivery ctrl */,
                NULL /* no content filter */))
    {
        printf("ERROR: failed to request data from Agent\n");
        goto disconnect;
    }

    while (MAX_SAMPLES <= 0 || received_samples < MAX_SAMPLES)
    {
        printf("WAITING for data... max=%u, received=%u, loop_t=%d\n",
            MAX_SAMPLES, received_samples, LOOP_TIMEOUT);
        
        /* Run XRCE Session until a sample is received for request */
        retcode = NANO_XRCE_Client_wait_for_data_received(
                        client, &read_req, LOOP_TIMEOUT);

        if (NANO_RETCODE_OK != retcode && NANO_RETCODE_TIMEOUT != retcode)
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
