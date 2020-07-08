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
 
   nanoagentd -a -c resource/xml/examples/sensor_agent.xml \
              -S -Sd /dev/your/tty/device -Ss 115200 -St 5000
 
 ******************************************************************************/

#include "nano/nano_client_serial.h"

#include <stdio.h>

#define SERIAL_PORT             "/dev/pts/2"
#define SERIAL_SPEED            115200

#define BIND_ADDR               0xAA
#define AGENT_ADDR              0xFF

#define CLIENT_KEY              0x01020304

#define READER_ID               0x5BB6

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

    NANO_MessageBufferData recv_buffer[
        RECV_BUFFER_SIZE / sizeof(NANO_MessageBufferData)] = { 0 };

    NANO_XRCE_SerialClient serial_client = NANO_XRCE_SERIALCLIENT_INITIALIZER;
    NANO_XRCE_Client *const client = &serial_client.client;

    NANO_XRCE_ClientRequestToken read_req =
        NANO_XRCE_CLIENTREQUESTTOKEN_INITIALIZER;

    NANO_RetCode retcode = NANO_RETCODE_ERROR;

    NANO_u32 received_samples = 0;

    NANO_LOG_SET_LEVEL(LOG_LEVEL)

    if (NANO_RETCODE_OK != 
            NANO_XRCE_SerialClient_initialize(
                &serial_client,
                CLIENT_KEY,
                TRANSPORT_MTU,
                (NANO_u8*)recv_buffer,
                sizeof(recv_buffer),
                SERIAL_PORT,
                SERIAL_SPEED,
                AGENT_ADDR,
                BIND_ADDR))
    {
        printf("ERROR: failed to initialize client\n");
        goto done;
    }

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
                NANO_XRCE_STREAMID_BUILTIN_BEST_EFFORTS,
                NULL /* no data delivery ctrl */,
                NULL /* no content filter */))
    {
        printf("ERROR: failed to request data from Agent\n");
        goto disconnect;
    }

    while (MAX_SAMPLES <= 0 || received_samples < MAX_SAMPLES)
    {
        printf("WAITING for data... max=%u, received=%u\n",
            received_samples, MAX_SAMPLES);
        
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