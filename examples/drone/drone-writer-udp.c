/******************************************************************************
 *
 * (c) 2021 Copyright, Real-Time Innovations, Inc. (RTI)
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
 
   nanoagentd -U -a -c resource/xml/examples/drone_agent.xml
 
 ******************************************************************************/

#include "drone-api.h"

#define CLIENT_KEY                  0x0C0D0E0F


int main(int argc, char const *argv[])
{
    int rc = 1;

    const NANO_u8 agent_addr[4] = AGENT_ADDRESS;

    NANO_MessageBufferData recv_buffer[
            RECV_BUFFER_SIZE / sizeof(NANO_MessageBufferData)] = { 0 };

    NANO_XRCE_Udpv4Client udp_client = NANO_XRCE_UDPV4CLIENT_INITIALIZER;
    NANO_XRCE_Client *const client = &udp_client.client;

#if WRITE_RELIABLE
    NANO_XRCE_DefaultReliableStreamStorage
        stream_storage_sensor_data =
            NANO_XRCE_DEFAULTRELIABLESTREAMSTORAGE_INITIALIZER;
#else
    NANO_XRCE_DefaultBestEffortStreamStorage
        stream_storage_sensor_data =
            NANO_XRCE_DEFAULTBESTEFFORTSTREAMSTORAGE_INITIALIZER;
#endif /* WRITE_RELIABLE */

    NANO_RetCode retcode = NANO_RETCODE_ERROR;

    NANO_u32 published_samples = 0;
    
    struct SensorCombined sensor_data = SensorCombined_INITIALIZER;

    NANO_MessageBufferData sensor_data_buffer[
            SensorCombined_BUFFER_SIZE / sizeof(NANO_MessageBufferData)] = { 0 };
    
    NANO_usize sensor_data_buffer_size = sizeof(sensor_data_buffer);

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

#if RELIABLE
    NANO_XRCE_DefaultReliableStreamStorage_initialize(&stream_storage_sensor_data);
#else
    NANO_XRCE_DefaultBestEffortStreamStorage_initialize(&stream_storage_sensor_data);
#endif /* RELIABLE */

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

    while (MAX_SAMPLES <= 0 ||
        (published_samples * 3) < MAX_SAMPLES)
    {
        /* Serialize Shape to a buffer in XCDR format */
        sensor_data_buffer_size = sizeof(sensor_data_buffer);
        serialize_sensor_data(
          &sensor_data, (NANO_u8*)sensor_data_buffer, &sensor_data_buffer_size);
        
        printf("Publishing sensor data %u\n", published_samples);

        if (NANO_RETCODE_OK != 
                NANO_XRCE_Client_write_data(
                    client,
                    NULL,
                    SENSOR_DATA_STREAM_ID,
                    NANO_XRCE_REQUESTFLAGS_SYNC,
                    REQUEST_TIMEOUT,
                    SENSOR_DATA_WRITER_ID,
                    (const NANO_u8*)sensor_data_buffer,
                    sensor_data_buffer_size))
        {
            printf("ERROR: failed to publish sample\n");
        }

        published_samples += 1;

#if NANO_PLATFORM_IS_POSIX
#if defined(HAVE_NANOSLEEP) /* use nanosleep() if available */
        {
            struct timespec tout = { 5, 0 /* 5s */ };
            nanosleep(&tout, NULL);
        }
#elif defined(HAVE_USLEEP) /* otherwise fall back to usleep() or... */
        usleep(5000000 /* 5000ms */);
#else  /* finally resort to sleep() */
        sleep(5);
#endif /* HAVE_NANOSLEEP */
#elif NANO_PLATFORM == NANO_PLATFORM_WINDOWS
        SleepEx(5000, 0);
#endif
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
