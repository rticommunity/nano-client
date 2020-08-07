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

#define AGENT_ADDRESS               { 127, 0, 0, 1 }
#define BIND_PORT                   7403

#define CLIENT_KEY                  0x0A0B0C0D

#define READER_CIRCLE_ID            0xB266
#define READER_TRIANGLE_ID          0xAB36
#define READER_SQUARE_ID            0xFA56

#define SUBSCRIBE_RELIABLE          0

#if SUBSCRIBE_RELIABLE
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
#define MAX_SAMPLES                 NANO_U16_MAX
#define RECV_DATA_TIMEOUT           1000 /* 1s */

#define TRANSPORT_MTU               128
#define RECV_BUFFER_SIZE \
    NANO_OSAPI_Memory_align_size_up(\
        TRANSPORT_MTU, sizeof(NANO_MessageBufferData))

#define CIRCLE_COLOR        "RED"
#define CIRCLE_SIZE         35
#define CIRCLE_X            0
#define CIRCLE_Y            0
#define CIRCLE_SPEED_X      10
#define CIRCLE_SPEED_Y      -5

#define TRIANGLE_COLOR      "GREEN"
#define TRIANGLE_SIZE       35
#define TRIANGLE_X          50
#define TRIANGLE_Y          30
#define TRIANGLE_SPEED_X    1
#define TRIANGLE_SPEED_Y    4

#define SQUARE_COLOR        "BLUE"
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

int deserialize_shape(
    struct Shape *const shape,
    const NANO_u8 *const buffer,
    const NANO_usize buffer_len,
    const NANO_bool little_endian);

NANO_u32 circle_updates = 0;
NANO_u32 square_updates = 0;
NANO_u32 triangle_updates = 0;

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

void
print_shape(
    struct Shape *const shape,
    const char *s_type,
    const NANO_u32 shape_updates);

int main(int argc, char const *argv[])
{
    int rc = 1;

    const NANO_u8 agent_addr[4] = AGENT_ADDRESS;

    NANO_MessageBufferData recv_buffer[
        RECV_BUFFER_SIZE / sizeof(NANO_MessageBufferData)] = { 0 };

    NANO_XRCE_Udpv4Client udp_client = NANO_XRCE_UDPV4CLIENT_INITIALIZER;
    NANO_XRCE_Client *const client = &udp_client.client;

#if SUBSCRIBE_RELIABLE
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
#endif /* SUBSCRIBE_RELIABLE */
        
    NANO_XRCE_ClientRequestToken
        read_req_circle = NANO_XRCE_CLIENTREQUESTTOKEN_INITIALIZER,
        read_req_square = NANO_XRCE_CLIENTREQUESTTOKEN_INITIALIZER,
        read_req_triangle = NANO_XRCE_CLIENTREQUESTTOKEN_INITIALIZER;

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

#if SUBSCRIBE_RELIABLE
    NANO_XRCE_DefaultBestEffortStreamStorage_initialize(
        &stream_storage_circle);
    NANO_XRCE_DefaultBestEffortStreamStorage_initialize(
        &stream_storage_square);
    NANO_XRCE_DefaultBestEffortStreamStorage_initialize(
        &stream_storage_triangle);
#else
    NANO_XRCE_DefaultReliableStreamStorage_initialize(
        &stream_storage_circle);
    NANO_XRCE_DefaultReliableStreamStorage_initialize(
        &stream_storage_square);
    NANO_XRCE_DefaultReliableStreamStorage_initialize(
        &stream_storage_triangle);
#endif /* SUBSCRIBE_RELIABLE */

    NANO_XRCE_Client_liveliness_assertion_period(client, 2000);

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

    if (NANO_RETCODE_OK != 
            NANO_XRCE_Client_read_data(
                client,
                &read_req_circle,
                /* stream for the read request */
                NANO_XRCE_STREAMID_BUILTIN_RELIABLE,
                /* default behavior (block until ack) */
                NANO_XRCE_REQUESTFLAGS_SYNC,
                REQUEST_TIMEOUT,
                READER_CIRCLE_ID,
                /* stream on which data will be forwarded */
                CIRCLE_STREAM_ID,
                NULL /* no data delivery ctrl */,
                NULL /* no content filter */))
    {
        printf("ERROR: failed to request circles from Agent\n");
        goto disconnect;
    }

    if (NANO_RETCODE_OK != 
            NANO_XRCE_Client_read_data(
                client,
                &read_req_square,
                /* stream for the read request */
                NANO_XRCE_STREAMID_BUILTIN_RELIABLE,
                /* default behavior (block until ack) */
                NANO_XRCE_REQUESTFLAGS_SYNC,
                REQUEST_TIMEOUT,
                READER_SQUARE_ID,
                /* stream on which data will be forwarded */
                SQUARE_STREAM_ID,
                NULL /* no data delivery ctrl */,
                NULL /* no content filter */))
    {
        printf("ERROR: failed to request square from Agent\n");
        goto disconnect;
    }

    if (NANO_RETCODE_OK != 
            NANO_XRCE_Client_read_data(
                client,
                &read_req_triangle,
                /* stream for the read request */
                NANO_XRCE_STREAMID_BUILTIN_RELIABLE,
                /* default behavior (block until ack) */
                NANO_XRCE_REQUESTFLAGS_SYNC,
                REQUEST_TIMEOUT,
                READER_TRIANGLE_ID,
                /* stream on which data will be forwarded */
                TRIANGLE_STREAM_ID,
                NULL /* no data delivery ctrl */,
                NULL /* no content filter */))
    {
        printf("ERROR: failed to request triangle from Agent\n");
        goto disconnect;
    }

    while (MAX_SAMPLES <= 0 ||
            (circle_updates + triangle_updates + square_updates) < MAX_SAMPLES)
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
    struct Shape shape = Shape_INITIALIZER;
    const NANO_u16 reader_id = NANO_XRCE_ObjectId_to_u16(reader);
    
    deserialize_shape(&shape, data, data_len, little_endian);

    if (reader_id == READER_CIRCLE_ID)
    {
        circle_updates += 1;
        print_shape(&shape, "CIRCLE", circle_updates);
    }
    else if (reader_id == READER_SQUARE_ID)
    {
        square_updates += 1;
        print_shape(&shape, "SQUARE", square_updates);
    }
    else if (reader_id == READER_TRIANGLE_ID)
    {
        triangle_updates += 1;
        print_shape(&shape, "TRIANGLE", triangle_updates);
    }
}

void print_shape(
    struct Shape *const shape,
    const char *s_type,
    const NANO_u32 shape_updates)
{
    printf("%s position (%u): color=%s, x=%d, y=%d, size=%d\n",
        s_type,
        shape_updates,
        shape->color,
        shape->x,
        shape->y,
        shape->shapesize);
}

int deserialize_shape(
    struct Shape *const shape,
    const NANO_u8 *const buffer,
    const NANO_usize buffer_len,
    const NANO_bool little_endian)
{
    int rc = 1;
    const NANO_u8 *data_ptr = buffer;
    NANO_CDR_Stream stream = NANO_CDR_STREAM_INITIALIZER;
    char *str_buf = NULL;
    NANO_usize str_len = 0;

    if (NANO_RETCODE_OK !=
            NANO_CDR_Stream_initialize(
                &stream,
                (NANO_u8*)buffer,
                buffer_len,
                little_endian,
                NANO_BOOL_FALSE))
    {
        goto done;
    }

    str_buf = shape->color;
    if (NANO_RETCODE_OK !=
            NANO_CDR_Stream_deserialize_string(
                &stream,
                &str_buf,
                Shape_COLOR_MAX_LENGTH,
                &str_len))
    {
        goto done;
    }

    if (NANO_RETCODE_OK !=
            NANO_CDR_Stream_deserialize_i32(&stream, &shape->x))
    {
        goto done;
    }
    
    if (NANO_RETCODE_OK !=
            NANO_CDR_Stream_deserialize_i32(&stream, &shape->y))
    {
        goto done;
    }

    if (NANO_RETCODE_OK !=
            NANO_CDR_Stream_deserialize_i32(&stream, &shape->shapesize))
    {
        goto done;
    }

    rc = 0;
done:
    return rc;
}
