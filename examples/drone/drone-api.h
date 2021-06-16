#ifndef drone_api_h
#define drone_api_h

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
#define RELIABLE                    1
#if RELIABLE
#define SENSOR_DATA_STREAM_ID       0xA0
#else
#define SENSOR_DATA_STREAM_ID       0x0A
#endif /* RELIABLE */

#define LOG_LEVEL                   LEVEL_INFO

#define REQUEST_TIMEOUT             10000 /* 10s */
#define MAX_SAMPLES                 (NANO_U16_MAX*2)
#define RECV_DATA_TIMEOUT           1000 /* 1s */

#define TRANSPORT_MTU               128
#define RECV_BUFFER_SIZE \
    NANO_OSAPI_Memory_align_size_up(\
        TRANSPORT_MTU, sizeof(NANO_MessageBufferData))

#define SENSOR_DATA_WRITER_ID       0x4615
#define SENSOR_DATA_READER_ID       0x80A6

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

int deserialize_sensor_data(
    struct SensorCombined *const sensor_data,
    const NANO_u8 *const buffer,
    const NANO_usize buffer_len,
    const NANO_bool little_endian);

#endif /* drone_api_h */