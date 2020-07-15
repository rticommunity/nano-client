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
 
   nanoagentd -U
 
 ******************************************************************************/

#include "nano/nano_client_udpv4.h"

#include <stdio.h>

#define AGENT_ADDRESS           { 127, 0, 0, 1 }
#define BIND_PORT               7403

#define CLIENT_KEY              0x05060708

#define TYPE_ID                 0x0010
#define DOMAIN_ID               0x0010
#define PARTICIPANT_ID          0x0010
#define READER_ID               0x0010

#define LOG_LEVEL               LEVEL_INFO

#define REQUEST_TIMEOUT         10000 /* 10s */
#define MAX_SAMPLES             10
#define LOOP_TIMEOUT            1000 /* 1s */

#define TRANSPORT_MTU           128
#define RECV_BUFFER_SIZE \
    (NANO_OSAPI_Memory_align_size_up(\
        TRANSPORT_MTU, sizeof(NANO_MessageBufferData)))

#define MAX_UNFRAGMENTED        512
#define FRAG_BUFFER_SIZE \
    NANO_XRCE_OUTPUT_FRAGMENTS_POOL_SIZE(\
        SESSION_ID, TRANSPORT_MTU, MAX_UNFRAGMENTED)

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
    NANO_u32 received_samples = 0;

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
                0 /* default local port */))
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
            NANO_XRCE_Client_create_type_xml(
                client,
                NULL /* request token */,
                NANO_XRCE_STREAMID_BUILTIN_RELIABLE,
                NANO_XRCE_REQUESTFLAGS_SYNC |
                NANO_XRCE_REQUESTFLAGS_CREATE_REUSE,
                REQUEST_TIMEOUT,
                TYPE_ID,
                "<types>"
                    "<struct name=\"SensorData\">"
                        "<member name=\"id\" key=\"true\""
                                "type=\"uint8\" arrayDimensions=\"4\"/>"
                        "<member name=\"value\" type=\"uint32\"/>"
                    "</struct>"
                "</types>"))
    {
        printf("ERROR: failed to create type\n");
        goto done;
    }

    if (NANO_RETCODE_OK !=
            NANO_XRCE_Client_create_domain_xml(
                client,
                NULL /* request token */,
                NANO_XRCE_STREAMID_BUILTIN_RELIABLE,
                NANO_XRCE_REQUESTFLAGS_SYNC |
                NANO_XRCE_REQUESTFLAGS_CREATE_REUSE,
                REQUEST_TIMEOUT,
                DOMAIN_ID,
                "<domain_library name=\"Domains\">"
                    "<domain name=\"Sensors\" domain_id=\"0\">"
                        "<register_type name=\"SensorData\" type_ref=\"SensorData\" />"
                        "<topic name=\"SensorReadings\" register_type_ref=\"SensorData\"/>"
                    "</domain>"
                "</domain_library>"))
    {
        printf("ERROR: failed to create domain\n");
        goto done;
    }

    if (NANO_RETCODE_OK !=
            NANO_XRCE_Client_create_participant_xml(
                client,
                NULL /* request token */,
                NANO_XRCE_STREAMID_BUILTIN_RELIABLE,
                NANO_XRCE_REQUESTFLAGS_SYNC |
                NANO_XRCE_REQUESTFLAGS_CREATE_REUSE,
                REQUEST_TIMEOUT,
                PARTICIPANT_ID,
                "<domain_participant_library name=\"Participants\">"
                    "<domain_participant name=\"SensorAgent\" "
                                        "domain_ref=\"Domains::Sensors\">"
                        "<subscriber name=\"Subscriber\">"
                            "<data_reader name=\"Reader\" topic_ref=\"SensorReadings\"/>"
                        "</subscriber>"
                    "</domain_participant>"
                "</domain_participant_library>"))
    {
        printf("ERROR: failed to create domain\n");
        goto done;
    }

    if (NANO_RETCODE_OK !=
            NANO_XRCE_Client_register_datareader(
                client,
                NULL /* request token */,
                NANO_XRCE_STREAMID_BUILTIN_RELIABLE,
                NANO_XRCE_REQUESTFLAGS_SYNC |
                NANO_XRCE_REQUESTFLAGS_CREATE_REUSE,
                REQUEST_TIMEOUT,
                READER_ID,
                "Participants::SensorAgent::Subscriber::Reader"))
    {
        printf("ERROR: failed to register datareader\n");
        goto done;
    }

    if (NANO_RETCODE_OK != 
            NANO_XRCE_Client_read_data(
                client,
                &read_req /* request token */,
                NANO_XRCE_STREAMID_BUILTIN_RELIABLE,
                NANO_XRCE_REQUESTFLAGS_SYNC |
                NANO_XRCE_REQUESTFLAGS_CREATE_REUSE,
                REQUEST_TIMEOUT,
                READER_ID,
                NANO_XRCE_STREAMID_BUILTIN_RELIABLE /* stream on which data will be forwarded */,
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

