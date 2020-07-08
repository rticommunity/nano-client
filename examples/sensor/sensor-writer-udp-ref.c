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
 
   nanoagentd -U -c resource/xml/examples/sensor_agent.xml
 
 ******************************************************************************/

#include "nano/nano_client_udpv4.h"

#include <stdio.h>

#if NANO_PLATFORM == NANO_PLATFORM_WINDOWS
/* Include synchapi.h for SleepEx() */
#include <synchapi.h>
#elif NANO_PLATFORM_IS_POSIX
#if _POSIX_C_SOURCE >= 199309L
#include <time.h>
#define HAVE_NANOSLEEP
#elif (_XOPEN_SOURCE >= 500) && ! (_POSIX_C_SOURCE >= 200809L)\
        || /* Glibc since 2.19: */ _DEFAULT_SOURCE \
        || /* Glibc versions <= 2.19: */ _BSD_SOURCE
#define HAVE_USLEEP
#endif
#endif

#define AGENT_ADDRESS           { 127, 0, 0, 1 }

#define CLIENT_KEY              0x01020304

#define WRITER_ID               0x0010

#define WRITER_RELIABLE         0

#define LOG_LEVEL               LEVEL_INFO

#define REQUEST_TIMEOUT         10000 /* 10s */
#define MAX_SAMPLES             10

#define TRANSPORT_MTU           128
#define RECV_BUFFER_SIZE \
    NANO_OSAPI_Memory_align_size_up(\
        TRANSPORT_MTU, sizeof(NANO_MessageBufferData))

struct SensorData
{
    NANO_u8 id[4];
    NANO_u32 value;
};

#define SensorData_INITIALIZER      { { 0 }, 0 }

int initialize_client(
    NANO_XRCE_Client *const client,
    NANO_XRCE_ClientStorage *const client_storage,
    NANO_XRCE_ClientTransport *const transport,
    NANO_XRCE_ClientTransportProperties *const transport_props,
    const NANO_u32 client_key,
    const NANO_u8 session_id);

int create_objects(
    NANO_XRCE_Client *const client,
    const NANO_u8 request_stream);

int publish_sensor_data(
    NANO_XRCE_Client *const client,
    const NANO_u16 writer_id,
    const NANO_u8 stream_id,
    const struct SensorData *const data);

int main(int argc, char const *argv[])
{
    int rc = 1;

    const NANO_u8 agent_addr[4] = AGENT_ADDRESS;

    NANO_MessageBufferData recv_buffer[
            RECV_BUFFER_SIZE / sizeof(NANO_MessageBufferData)] = { 0 };

    NANO_XRCE_Udpv4Client udp_client = NANO_XRCE_UDPV4CLIENT_INITIALIZER;
    NANO_XRCE_Client *const client = &udp_client.client;

    NANO_u32 published_samples = 0;

    struct SensorData data = SensorData_INITIALIZER;

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
                0 /* default bind port */))
    {
        printf("ERROR: failed to initialize client\n");
        goto done;
    }

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
            NANO_XRCE_Client_register_datawriter(
                client,
                NULL /* request token */,
                NANO_XRCE_STREAMID_BUILTIN_RELIABLE,
                NANO_XRCE_REQUESTFLAGS_SYNC |
                NANO_XRCE_REQUESTFLAGS_CREATE_REUSE,
                REQUEST_TIMEOUT,
                WRITER_ID,
                "Participants::SensorAgent::Publisher::Writer"))
    {
        printf("ERROR: failed to register datawriter\n");
        goto disconnect;
    }

    /* Set data.id with the Big-Endian value of CLIENT_KEY */
    NANO_u32_serialize(CLIENT_KEY, data.id, NANO_BOOL_FALSE);

    while (MAX_SAMPLES <= 0 || published_samples < MAX_SAMPLES)
    {
        /* Update data, and serialize it to data_buffer */
        data.value = published_samples;

        if (NANO_RETCODE_OK != 
                NANO_XRCE_Client_write_data(
                    client,
                    NULL /* request token */,
#if WRITER_RELIABLE
                    NANO_XRCE_STREAMID_BUILTIN_RELIABLE,
#else
                    NANO_XRCE_STREAMID_BUILTIN_BEST_EFFORTS,
#endif
                    NANO_XRCE_REQUESTFLAGS_SYNC,
                    REQUEST_TIMEOUT,
                    WRITER_ID,
                    /* The XCDR serialization of SensorData is the same
                        as its C memory representation */
                    (const NANO_u8*)&data,
                    sizeof(data)))
        {
            printf("ERROR: failed to publish sample\n");
            goto disconnect;
        }

        printf("Sample %u/%u published\n", published_samples + 1, MAX_SAMPLES);
        published_samples += 1;

#if NANO_PLATFORM_IS_POSIX
#ifdef HAVE_NANOSLEEP /* use nanosleep() if available */
        {
            struct timespec tout = { 0, 500000000 /* 500 ms */ };
            nanosleep(&tout, NULL);
        }
#elif defined(HAVE_USLEEP) /* otherwise fall back to usleep() or... */
        usleep(500000 /* 500ms */);
#else  /* finally resort to sleep() */
        sleep(1);
#endif /* HAVE_NANOSLEEP */
#elif NANO_PLATFORM == NANO_PLATFORM_WINDOWS
        SleepEx(500, 0);
#endif
    }

disconnect:

    NANO_LOG_INFO_MSG("CLOSING session on agent")

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
