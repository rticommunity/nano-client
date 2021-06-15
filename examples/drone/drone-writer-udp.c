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

#define AGENT_ADDRESS               { 127, 0, 0, 1 }

#define CLIENT_KEY                  0x0C0D0E0F

#define WRITER_SENSOR_DATA_ID       0x4615

#define WRITE_RELIABLE              1

#if WRITE_RELIABLE
#define SENSOR_DATA_STREAM_ID       0xA0
#else
#define SENSOR_DATA_STREAM_ID       0x0A
#endif

#define LOG_LEVEL                   LEVEL_INFO

#define REQUEST_TIMEOUT             10000 /* 10s */
#define MAX_SAMPLES                 (NANO_U16_MAX*2)

#define TRANSPORT_MTU               128
#define RECV_BUFFER_SIZE \
    NANO_OSAPI_Memory_align_size_up(\
        TRANSPORT_MTU, sizeof(NANO_MessageBufferData))

struct SensorCombined {
    uint64_t timestamp;
    float gyro_rad[3];
    uint32_t gyro_integral_dt;
    int32_t accelerometer_timestamp_relative;
    float accelerometer_m_s2[3];
    uint32_t accelerometer_integral_dt;
    unsigned char accelerometer_clipping;
};

#define SensorCombined_INITIALIZER \
{\
    0, /* timestamp */ \
    {.0}, /* gyro_rad */ \
    0, /* gyro_integral_dt */ \
    0, /* accelerometer_timestamp_relative */ \
    {.0}, /* accelerometer_m_s2 */ \
    0, /* accelerometer_integral_dt */ \
    0 /* accelerometer_clipping */ \
}

#define SensorCombined_SERIALIZED_SIZE_MAX \
(\
    8 /* timestamp */ + \
    (4 * 3) /* gyro_rad */ + \
    4 /* gyro_integral_dt */ + \
    4 /* accelerometer_timestamp_relative */ + \
    (4 * 3) /* accelerometer_m_s2 */ + \
    4 /* accelerometer_integral_dt */ + \
    1 /* accelerometer_clipping */ \
)

#define SensorCombined_BUFFER_SIZE \
    NANO_OSAPI_Memory_align_size_up(\
        SensorCombined_SERIALIZED_SIZE_MAX, sizeof(NANO_MessageBufferData))

void serialize_sensor_data(
    const struct SensorCombined *const sensor_data,
    NANO_u8 *const buffer,
    NANO_usize *const buffer_len);

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

#if WRITE_RELIABLE
    NANO_XRCE_DefaultReliableStreamStorage_initialize(&stream_storage_sensor_data);
#else
    NANO_XRCE_DefaultBestEffortStreamStorage_initialize(&stream_storage_sensor_data);
#endif /* WRITE_RELIABLE */

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
                    WRITER_SENSOR_DATA_ID,
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

void serialize_sensor_data(
    const struct SensorCombined *const sensor_data,
    NANO_u8 *const buffer,
    NANO_usize *const buffer_len)
{
    NANO_CDR_Stream stream = NANO_CDR_STREAM_INITIALIZER;
    const NANO_usize buffer_max = *buffer_len;
    const char *str_ptr = NULL;

    *buffer_len = 0;

    NANO_CDR_Stream_initialize(
        &stream,
        buffer,
        buffer_max,
        NANO_CDR_ENDIANNESS_NATIVE,
        NANO_BOOL_FALSE /* owned */);

    NANO_CDR_Stream_serialize_u64(&stream, &sensor_data->timestamp);
    NANO_CDR_Stream_serialize_octets(&stream,
      (const NANO_u8*)sensor_data->gyro_rad, sizeof(sensor_data->gyro_rad));
    NANO_CDR_Stream_serialize_u32(&stream, &sensor_data->gyro_integral_dt);
    NANO_CDR_Stream_serialize_i32(&stream,
      &sensor_data->accelerometer_timestamp_relative);
    NANO_CDR_Stream_serialize_octets(&stream,
      (const NANO_u8*)sensor_data->accelerometer_m_s2,
      sizeof(sensor_data->accelerometer_m_s2));
    NANO_CDR_Stream_serialize_u32(&stream, &sensor_data->accelerometer_integral_dt);
    NANO_CDR_Stream_serialize_u8(&stream, &sensor_data->accelerometer_clipping);

    *buffer_len = NANO_CDR_Stream_offset(&stream);
}
