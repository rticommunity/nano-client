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
 
   nanoagentd -U -c nano-client-arduino/extras/shape_agent.xml
 
 ******************************************************************************/

#include <nano_client_arduino.h>

#define CLIENT_KEY          0x05060708
#define TRANSPORT_MTU       128

#define WIFI_SSID           "changeme"
#define WIFI_PASSWORD       "changeme"
#define AGENT_ADDRESS       { 192, 168, 1, 1 }
#define SERIAL_SPEED        115200

#define INIT_DELAY          1000
#define LOOP_DELAY          1000

#define READER_CIRCLE_ID    0xB266
#define READER_TRIANGLE_ID  0xAB36
#define READER_SQUARE_ID    0xAB36

#define SUBSCRIBE_RELIABLE  0

#if SUBSCRIBE_RELIABLE
#define READER_CIRCLE_STREAM_ID    0xA0
#define READER_TRIANGLE_STREAM_ID  0xB0
#define READER_SQUARE_STREAM_ID    0xC0
#else
#define READER_CIRCLE_STREAM_ID    0x0A
#define READER_TRIANGLE_STREAM_ID  0x0B
#define READER_SQUARE_STREAM_ID    0x0C
#endif

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

const uint8_t agent_address[4] = AGENT_ADDRESS;

XrceData transport_recv_buffer[XRCE_DATA_BUFFER_SIZE(TRANSPORT_MTU)] = { 0 };

XrceUdpTransport transport(
    transport_recv_buffer,
    sizeof(transport_recv_buffer),
    agent_address);

XrceClient client(transport, CLIENT_KEY);

XrceReliableStream stream_circle(client, READER_CIRCLE_STREAM_ID);
XrceReliableStream stream_triangle(client, READER_TRIANGLE_STREAM_ID);
XrceReliableStream stream_square(client, READER_SQUARE_STREAM_ID);

XrceDataReader reader_circle(
    client,
    READER_CIRCLE_ID,
    XRCE_OBJECT_ID_INVALID,
    NULL,
    false,
    READER_CIRCLE_STREAM_ID);

XrceDataReader reader_square(
    client,
    READER_SQUARE_ID,
    XRCE_OBJECT_ID_INVALID,
    NULL,
    false,
    READER_SQUARE_STREAM_ID);

XrceDataReader reader_triangle(
    client,
    READER_TRIANGLE_ID,
    XRCE_OBJECT_ID_INVALID,
    NULL,
    false,
    READER_TRIANGLE_STREAM_ID);


#define Shape_COLOR_MAX_LENGTH      128

struct Shape
{
    char color[Shape_COLOR_MAX_LENGTH + 1];
    int32_t x;
    int32_t y;
    int32_t shapesize;
};

/**
 * Deserialize a Shape object from a XCDR-encoded buffer
 */
bool deserialize_shape(
    Shape& s,
    const XrceData *const buf,
    const size_t buf_len)
{
    XcdrStream stream(buf, buf_len);

    if (!stream.deserialize_string(
            s.color, NULL, Shape_COLOR_MAX_LENGTH) ||
        !stream.deserialize(&s.x) ||
        !stream.deserialize(&s.y) ||
        !stream.deserialize(&s.shapesize))
    {
        return false;
    }
    
    return true;
}

/**
 * Print out the value of a Shape object for debugging purposes
 */
void print_shape(Shape& s, const char *s_type, const size_t shape_updates)
{
    Serial.print(s_type);
    Serial.print(" position (");
    Serial.print(shape_updates);
    Serial.print("): color=");
    Serial.print(s.color);
    Serial.print(", x=");
    Serial.print(s.x);
    Serial.print(", y=");
    Serial.print(s.y);
    Serial.print(", size=");
    Serial.println(s.shapesize);
}

size_t circle_updates = 0;
size_t square_updates = 0;
size_t triangle_updates = 0;

/**
 * Listener callback used to notify the application of
 * new XRCE data received by the client.
 */
void on_data(
    void *const listener,
    XrceClient& client,
    const XrceReceivedData& recv_data)
{
    Shape shape;

    deserialize_shape(
        shape, (const XrceData*)recv_data.data(), recv_data.data_len());
    
    if (recv_data.reader_id() == reader_circle.id())
    {
        circle_updates += 1;
        print_shape(shape, "CIRCLE", circle_updates);
    }
    else if (recv_data.reader_id() == reader_square.id())
    {
        square_updates += 1;
        print_shape(shape, "SQUARE", square_updates);
    }
    else if (recv_data.reader_id() == reader_triangle.id())
    {
        triangle_updates += 1;
        print_shape(shape, "TRIANGLE", triangle_updates);
    }
}

/**
 * Initialize Serial port for debugging messages
 */
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
#include <WiFi.h>
#endif

/**
 * Connect to a WiFi network
 */
void wifi_connect()
{
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    delay(INIT_DELAY);

    while (WiFi.status() != WL_CONNECTED) {
        delay(INIT_DELAY);
    }
}

/**
 * Initialize local XRCE client and connect to a remote XRCE agent.
 */
bool xrce_connect()
{
    if (!client.initialize())
    {
      Serial.println("Failed to initialize XRCE client");
      return false;
    }

    /* Enable custom XRCE streams */
    stream_circle.enable();
    stream_square.enable();
    stream_triangle.enable();
    
    while (!client.connected())
    {
      Serial.println("Connecting to XRCE agent...");
      client.connect(INIT_DELAY);
    }

    /* Set callback to be notified of received XRCE data */
    client.on_data_callback(on_data);

    /* Request samples from agent */
    if (!reader_circle.read_data())
    {
        Serial.println("Failed to request Circle data from agent");
        return false;
    }
    if (!reader_square.read_data())
    {
        Serial.println("Failed to request Square data from agent");
        return false;
    }
    if (!reader_triangle.read_data())
    {
        Serial.println("Failed to request Triangle data from agent");
        return false;
    }

    Serial.println("Connected to XRCE agent");
    return true;
}


void setup()
{
    /* Initialize serial port */
    serial_init();
    
    /* Initialize wifi connection */
    wifi_connect();

    /* Initialize XRCE Client and register custom streams */
    if (!xrce_connect())
    {
        while (1) {}
    }
}

void loop()
{
    /* Run XRCE session to automatically send and receive messages */
    client.run_session(0, LOOP_DELAY);
}
