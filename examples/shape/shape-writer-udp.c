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

#define WRITER_CIRCLE_ID            0xE515
#define WRITER_TRIANGLE_ID          0x9E05
#define WRITER_SQUARE_ID            0x2575

#define WRITE_RELIABLE            1

#if WRITE_RELIABLE
#define CIRCLE_STREAM_ID            0xA0
#define TRIANGLE_STREAM_ID          0xB0
#define SQUARE_STREAM_ID            0xC0
#else
#define CIRCLE_STREAM_ID            0x0A
#define TRIANGLE_STREAM_ID          0x0B
#define SQUARE_STREAM_ID            0x0C
#endif

#define LOG_LEVEL                   LEVEL_INFO

#define REQUEST_TIMEOUT             10000 /* 10s */
#define MAX_SAMPLES                 (NANO_U16_MAX*2)

#define TRANSPORT_MTU               128
#define RECV_BUFFER_SIZE \
    NANO_OSAPI_Memory_align_size_up(\
        TRANSPORT_MTU, sizeof(NANO_MessageBufferData))

#define CIRCLE_COLOR        "SALMON"
#define CIRCLE_SIZE         35
#define CIRCLE_X            0
#define CIRCLE_Y            0
#define CIRCLE_SPEED_X      10
#define CIRCLE_SPEED_Y      -5

#define TRIANGLE_COLOR      "AQUAMARINE"
#define TRIANGLE_SIZE       35
#define TRIANGLE_X          50
#define TRIANGLE_Y          30
#define TRIANGLE_SPEED_X    1
#define TRIANGLE_SPEED_Y    4

#define SQUARE_COLOR        "NAVY"
#define SQUARE_SIZE         35
#define SQUARE_X            70
#define SQUARE_Y            180
#define SQUARE_SPEED_X      -8
#define SQUARE_SPEED_Y      -10

#define Shape_COLOR_MAX_LENGTH      128

struct Shape
{
    char color[Shape_COLOR_MAX_LENGTH + 1];
    NANO_u32 x;
    NANO_u32 y;
    NANO_u32 shapesize;
};

#define Shape_INITIALIZER \
{\
    { '\0' }, /* color */\
    0, /* x */\
    0, /* y */\
    0 /* shapesize */\
}

#define Shape_DIMENSION_MAX         255

#define Shape_SERIALIZED_SIZE_MAX \
    (NANO_OSAPI_Memory_align_size_up(\
        sizeof(NANO_u32) + Shape_COLOR_MAX_LENGTH, sizeof(NANO_u32)) + \
        sizeof(NANO_u32) + \
        sizeof(NANO_u32) + \
        sizeof(NANO_u32))

#define Shape_BUFFER_SIZE \
    NANO_OSAPI_Memory_align_size_up(\
        Shape_SERIALIZED_SIZE_MAX, sizeof(NANO_MessageBufferData))

struct Speed
{
    NANO_u32 x;
    NANO_u32 y;
};

#define Speed_INITIALIZER { 0, 0 }

int move_and_publish_shape(
    NANO_XRCE_Client *const client,
    struct Shape *const shape,
    struct Speed *const speed,
    const NANO_u16 writer_id,
    const NANO_u8 stream_id);

int main(int argc, char const *argv[])
{
    int rc = 1;

    const NANO_u8 agent_addr[4] = AGENT_ADDRESS;

    NANO_MessageBufferData recv_buffer[
            RECV_BUFFER_SIZE / sizeof(NANO_MessageBufferData)] = { 0 };

    NANO_XRCE_Udpv4Client udp_client = NANO_XRCE_UDPV4CLIENT_INITIALIZER;
    NANO_XRCE_Client *const client = &udp_client.client;

#if WRITE_RELIABLE
    NANO_XRCE_DefaultBestEffortStreamStorage
        stream_storage_circle =
            NANO_XRCE_DEFAULTBESTEFFORTSTREAMSTORAGE_INITIALIZER,
        stream_storage_square = 
            NANO_XRCE_DEFAULTBESTEFFORTSTREAMSTORAGE_INITIALIZER,
        stream_storage_triangle = 
            NANO_XRCE_DEFAULTBESTEFFORTSTREAMSTORAGE_INITIALIZER;
#else
    NANO_XRCE_DefaultReliableStreamStorage
        stream_storage_circle =
            NANO_XRCE_DEFAULTRELIABLESTREAMSTORAGE_INITIALIZER,
        stream_storage_square = 
            NANO_XRCE_DEFAULTRELIABLESTREAMSTORAGE_INITIALIZER,
        stream_storage_triangle = 
            NANO_XRCE_DEFAULTRELIABLESTREAMSTORAGE_INITIALIZER;
#endif /* WRITE_RELIABLE */

    NANO_RetCode retcode = NANO_RETCODE_ERROR;

    NANO_u32 published_samples = 0;
    
    struct Shape
        shape_circle =
            { CIRCLE_COLOR, CIRCLE_X, CIRCLE_Y, CIRCLE_SIZE },
        shape_square =
            { SQUARE_COLOR, SQUARE_X, SQUARE_Y, SQUARE_SIZE },
        shape_triangle =
            { TRIANGLE_COLOR, TRIANGLE_X, TRIANGLE_Y, TRIANGLE_SIZE };
    
    struct Speed speed_circle = { CIRCLE_SPEED_X, CIRCLE_SPEED_Y },
                 speed_square = { SQUARE_SPEED_X, SQUARE_SPEED_Y },
                 speed_triangle = { TRIANGLE_SPEED_X, TRIANGLE_SPEED_Y };

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
    NANO_XRCE_DefaultBestEffortStreamStorage_initialize(&stream_storage_circle);
    NANO_XRCE_DefaultBestEffortStreamStorage_initialize(&stream_storage_square);
    NANO_XRCE_DefaultBestEffortStreamStorage_initialize(&stream_storage_triangle);
#else
    NANO_XRCE_DefaultReliableStreamStorage_initialize(&stream_storage_circle);
    NANO_XRCE_DefaultReliableStreamStorage_initialize(&stream_storage_square);
    NANO_XRCE_DefaultReliableStreamStorage_initialize(&stream_storage_triangle);
#endif /* WRITE_RELIABLE */

    NANO_XRCE_Client_enable_stream(
        client, CIRCLE_STREAM_ID, &stream_storage_circle.base);
    
    NANO_XRCE_Client_enable_stream(
        client, SQUARE_STREAM_ID, &stream_storage_square.base);
    
    NANO_XRCE_Client_enable_stream(
        client, TRIANGLE_STREAM_ID, &stream_storage_triangle.base);

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
        if (0 != move_and_publish_shape(
                    client,
                    &shape_square, &speed_square,
                    WRITER_SQUARE_ID, SQUARE_STREAM_ID))
        {
            goto disconnect;
        }
        published_samples += 1;

        if (0 != move_and_publish_shape(
                    client,
                    &shape_circle, &speed_circle,
                    WRITER_CIRCLE_ID, CIRCLE_STREAM_ID))
        {
            goto disconnect;
        }
        published_samples += 1;

        if (0 != move_and_publish_shape(
                    client,
                    &shape_triangle, &speed_triangle,
                    WRITER_TRIANGLE_ID, TRIANGLE_STREAM_ID))
        {
            goto disconnect;
        }
        published_samples += 1;

#if NANO_PLATFORM_IS_POSIX
#if defined(HAVE_NANOSLEEP) /* use nanosleep() if available */
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

void move_dimension(
    NANO_i32 *const dim,
    NANO_i32 *const speed)
{
    const NANO_bool neg_speed = (*speed < 0);
    const NANO_i32 speed_inv = (-1 * *speed);

    if (neg_speed && speed_inv > *dim)
    {
        /* bounce bottom */
        *dim = speed_inv - *dim;
        *speed = speed_inv;
    }
    else if (!neg_speed && *speed + *dim > Shape_DIMENSION_MAX)
    {
        /* bounce upper */
        *dim = (2 * Shape_DIMENSION_MAX) - *speed - *dim;
        *speed = speed_inv;
    }
    else
    {
        *dim = *dim + *speed;
    }
}

void serialize_shape(
    const struct Shape *const shape,
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

    str_ptr = shape->color;
    NANO_CDR_Stream_serialize_string(&stream, &str_ptr, Shape_COLOR_MAX_LENGTH);
    NANO_CDR_Stream_serialize_i32(&stream, &shape->x);
    NANO_CDR_Stream_serialize_i32(&stream, &shape->y);
    NANO_CDR_Stream_serialize_i32(&stream, &shape->shapesize);

    *buffer_len = NANO_CDR_Stream_offset(&stream);
}

int move_and_publish_shape(
    NANO_XRCE_Client *const client,
    struct Shape *const shape,
    struct Speed *const speed,
    const NANO_u16 writer_id,
    const NANO_u8 stream_id)
{
    NANO_MessageBufferData shape_buffer[
        Shape_BUFFER_SIZE / sizeof(NANO_MessageBufferData)] = { 0 };
    NANO_usize shape_buffer_size = sizeof(shape_buffer);

    move_dimension(&shape->x, &speed->x);
    move_dimension(&shape->y, &speed->y);

    /* Serialize Shape to a buffer in XCDR format */
    shape_buffer_size = sizeof(shape_buffer);
    serialize_shape(shape, (NANO_u8*)shape_buffer, &shape_buffer_size);

    if (NANO_RETCODE_OK != 
            NANO_XRCE_Client_write_data(
                client,
                NULL,
                stream_id,
                NANO_XRCE_REQUESTFLAGS_SYNC,
                REQUEST_TIMEOUT,
                writer_id,
                (const NANO_u8*)shape_buffer,
                shape_buffer_size))
    {
        printf("ERROR: failed to publish sample\n");
        return 1;
    }
    
    printf("%s [0x%04X, 0x%02X] "
        "color=%s, size=%d, speed=(%d,%d)\n"
        "\tx=%d, y=%d\n",
        (writer_id == WRITER_CIRCLE_ID)?
            "CIRCLE":
        (writer_id == WRITER_TRIANGLE_ID)?
            "TRIANGLE":
            "SQUARE",
        writer_id, stream_id,
        shape->color, shape->shapesize,
        speed->x, speed->y,
        shape->x, shape->y);

    return 0;
}
