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
 
   nanoagentd -U -c RTI_Nano/extras/sensor_agent.xml
 
 ******************************************************************************/

#include <nano_client_arduino.h>

#define CLIENT_KEY          0x01020304
#define BASE_OBJECT_ID      0x0010
#define TRANSPORT_MTU       128
#define WIFI_SSID           "changeme"
#define WIFI_PASSWORD       "changeme"
#define SERIAL_SPEED        115200
#define INIT_DELAY          1000
#define PUBLISH_DELAY       1000
#define AGENT_ADDRESS       { 192, 168, 1, 1 }

const uint8_t agent_address[4] = AGENT_ADDRESS;

XrceData transport_recv_buffer[
            XRCE_TRANSPORT_RECV_BUFFER_SIZE(TRANSPORT_MTU)] = { 0 };

XrceUdpTransport transport(
    transport_recv_buffer, sizeof(transport_recv_buffer), agent_address);

XrceClient client(transport, CLIENT_KEY);

XrceDomainParticipant participant(
    client, BASE_OBJECT_ID, "Participants::SensorAgent");

XrcePublisher publisher(
    client, BASE_OBJECT_ID, participant.id(), "Publisher");

XrceDataWriter writer(
    client, BASE_OBJECT_ID, publisher.id(), "Writer");

struct SensorData
{
    uint8_t id[4];
    uint32_t value;
};

SensorData data;

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

    /* Initialize XRCE client and connect to XRCE agent */
    client.connect();

    /* Attach XRCE objects to client session */
    participant.create();
    publisher.create();
    writer.create();

    /* Initialize sensor data */
    data.value = 0;
    /* Store client key as big endian */
    NANO_u32_serialize(CLIENT_KEY, data.id, false);
}

void loop()
{
    data.value += 1;
    writer.write_data((uint8_t*)&data, sizeof(data));
    
    Serial.print("SENSOR DATA: id=0x");
    for (size_t i = 0; i < 4; i++)
    {
        Serial.print(data.id[i], HEX);
    }
    Serial.print(", value=");
    Serial.println(data.value);

    delay(PUBLISH_DELAY);
}
