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
 
   nanoagentd -U -c RTI_Nano/extras/shape_agent.xml
 
 ******************************************************************************/

#include <nano_client_arduino.h>

#define CLIENT_KEY          0x01020304
#define TRANSPORT_MTU       128

#define WIFI_SSID           "changeme"
#define WIFI_PASSWORD       "changeme"
#define AGENT_ADDRESS       { 192, 168, 1, 1 }
#define SERIAL_SPEED        115200

#define INIT_DELAY          1000
#define PUBLISH_DELAY       100

#define WRITER_CIRCLE_ID    0xE515
#define WRITER_TRIANGLE_ID  0x9E05
#define WRITER_SQUARE_ID    0x2575

#define PUBLISH_RELIABLE    0

#if PUBLISH_RELIABLE
#define WRITER_CIRCLE_STREAM_ID    0xA0
#define WRITER_TRIANGLE_STREAM_ID  0xB0
#define WRITER_SQUARE_STREAM_ID    0xC0
#else
#define WRITER_CIRCLE_STREAM_ID    0x0A
#define WRITER_TRIANGLE_STREAM_ID  0x0B
#define WRITER_SQUARE_STREAM_ID    0x0C
#endif

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

const uint8_t agent_address[4] = AGENT_ADDRESS;

XrceData transport_recv_buffer[XRCE_DATA_BUFFER_SIZE(TRANSPORT_MTU)] = { 0 };

XrceUdpTransport transport(
    transport_recv_buffer,
    sizeof(transport_recv_buffer),
    agent_address);

XrceClient client(transport, CLIENT_KEY);

XrceReliableStream stream_circle(client, WRITER_CIRCLE_STREAM_ID);
XrceReliableStream stream_triangle(client, WRITER_TRIANGLE_STREAM_ID);
XrceReliableStream stream_square(client, WRITER_SQUARE_STREAM_ID);

XrceDataWriter writer_circle(
    client,
    WRITER_CIRCLE_ID,
    XRCE_OBJECT_ID_INVALID,
    NULL,
    false,
    WRITER_CIRCLE_STREAM_ID);

XrceDataWriter writer_square(
    client,
    WRITER_SQUARE_ID,
    XRCE_OBJECT_ID_INVALID,
    NULL,
    false,
    WRITER_SQUARE_STREAM_ID);

XrceDataWriter writer_triangle(
    client,
    WRITER_TRIANGLE_ID,
    XRCE_OBJECT_ID_INVALID,
    NULL,
    false,
    WRITER_TRIANGLE_STREAM_ID);

#define Shape_COLOR_MAX_LENGTH      128

struct Shape
{
    char color[Shape_COLOR_MAX_LENGTH + 1];
    int32_t x;
    int32_t y;
    int32_t shapesize;

    Shape(
        const char *const color,
        const int32_t x,
        const int32_t y,
        const int32_t shapesize)
        : x(x), y(y), shapesize(shapesize)
    {
        strncpy(this->color, color, Shape_COLOR_MAX_LENGTH + 1);
        this->color[Shape_COLOR_MAX_LENGTH] = '\0';
    }
};

#define Shape_SERIALIZED_SIZE_MAX \
   (NANO_OSAPI_Memory_align_size_up(\
        sizeof(int32_t) + Shape_COLOR_MAX_LENGTH, sizeof(int32_t)) + \
    sizeof(int32_t) + \
    sizeof(int32_t) + \
    sizeof(int32_t))

#define Shape_DIMENSION_MAX         255

bool serialize_shape(
    Shape& s,
    XrceData *const buf,
    size_t *const buf_len)
{
    XcdrStream stream(buf, *buf_len);
    
    *buf_len = 0;

    if (!stream.serialize_string(s.color) ||
        !stream.serialize(s.x) ||
        !stream.serialize(s.y) ||
        !stream.serialize(s.shapesize))
    {
        return false;
    }
    
    *buf_len = stream.offset();

    return true;
}

void print_shape(Shape& s, const char *s_type)
{
    Serial.print(s_type);
    Serial.print(" position: color=");
    Serial.print(s.color);
    Serial.print(", x=");
    Serial.print(s.x);
    Serial.print(", y=");
    Serial.print(s.y);
    Serial.print(", size=");
    Serial.println(s.shapesize);
}

struct Speed
{
    int32_t x;
    int32_t y;
    
    Speed(const int32_t x, const int32_t y)
    : x(x), y(y)
    {

    }
};

void move_dimension(
    int32_t& cur,
    int32_t& speed,
    const int32_t dim_max = Shape_DIMENSION_MAX)
{
    const bool neg_speed = speed < 0;
    const int32_t speed_abs = -1 * speed;

    if (neg_speed && speed_abs > cur)
    {
        /* bounce bottom */
        cur = speed_abs - cur;
        speed = speed_abs;
    }
    else if (!neg_speed && speed + cur > dim_max)
    {
        /* bounce upper */
        cur = (2 * dim_max) - speed - cur;
        speed = -1 * speed;
    }
    else
    {
        cur = cur + speed;
        speed = speed;
    }
}

void move_shape(
        Shape& shape,
        Speed& speed,
        const int32_t dim_max = Shape_DIMENSION_MAX)
{
    move_dimension(shape.x, speed.x, dim_max);
    move_dimension(shape.y, speed.y, dim_max);
}

XrceData shape_buffer[XRCE_DATA_BUFFER_SIZE(Shape_SERIALIZED_SIZE_MAX)] ={ 0 };

Shape circle(
    CIRCLE_COLOR,
    CIRCLE_X,
    CIRCLE_Y,
    CIRCLE_SIZE);

Speed speed_circle(CIRCLE_SPEED_X, CIRCLE_SPEED_Y);

Shape triangle(
    TRIANGLE_COLOR,
    TRIANGLE_X,
    TRIANGLE_Y,
    TRIANGLE_SIZE);

Speed speed_triangle(TRIANGLE_SPEED_X, TRIANGLE_SPEED_Y);

Shape square(
    SQUARE_COLOR,
    SQUARE_X,
    SQUARE_Y,
    SQUARE_SIZE);

Speed speed_square(SQUARE_SPEED_X, SQUARE_SPEED_Y);

void serial_init()
{
  Serial.begin(SERIAL_SPEED);
  delay(INIT_DELAY);
  while(!Serial && !Serial.available()){
    delay(INIT_DELAY);
  }
}

#if defined(ESP8266)
#include "ESP8266WiFi.h"
#elif defined(ESP32)
#include <WiFi.h>
#else
#error "unsupported target platform"
#endif

void wifi_connect()
{
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    delay(INIT_DELAY);

    while (WiFi.status() != WL_CONNECTED) {
        delay(INIT_DELAY);
    }
}

void setup()
{
    /* Initialize serial port */
    serial_init();
    
    /* Initialize wifi connection */
    wifi_connect();

    /* Initialize XRCE Client and register custom streams */
    client.initialize();
    stream_circle.enable();
    stream_square.enable();
    stream_triangle.enable();

    /* Initialize XRCE client and connect to XRCE agent */
    client.connect();
}

void loop()
{
    move_shape(circle, speed_circle);
    move_shape(square, speed_square);
    move_shape(triangle, speed_triangle);

    size_t shape_buffer_len = sizeof(shape_buffer);
    serialize_shape(circle, shape_buffer, &shape_buffer_len);
    writer_circle.write_data((uint8_t*)&shape_buffer, shape_buffer_len);
    print_shape(circle, "CIRCLE");

    shape_buffer_len = sizeof(shape_buffer);
    serialize_shape(triangle, shape_buffer, &shape_buffer_len);
    writer_triangle.write_data((uint8_t*)&shape_buffer, shape_buffer_len);
    print_shape(triangle, "TRIANGLE");
    
    shape_buffer_len = sizeof(shape_buffer);
    serialize_shape(square, shape_buffer, &shape_buffer_len);
    writer_square.write_data((uint8_t*)&shape_buffer, shape_buffer_len);
    print_shape(square, "SQUARE");

    delay(PUBLISH_DELAY);
}
