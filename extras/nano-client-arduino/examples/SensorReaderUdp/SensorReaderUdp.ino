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
 
   nanoagentd -U -a -c nano-client-arduino/extras/sensor_agent.xml
 
 ******************************************************************************/

#include <nano_client_arduino.h>

#define CLIENT_KEY          0x05060708
#define READER_ID           0x5BB6
#define TRANSPORT_MTU       128
#define WIFI_SSID           "changeme"
#define WIFI_PASSWORD       "changeme"
#define AGENT_ADDRESS       { 192, 168, 1, 1 }
#define INIT_DELAY          1000
#define PUBLISH_DELAY       1000
#define SERIAL_SPEED        115200

const uint8_t agent_address[4] = AGENT_ADDRESS;

XrceData transport_recv_buffer[
            XRCE_TRANSPORT_RECV_BUFFER_SIZE(TRANSPORT_MTU)] = { 0 };

XrceUdpTransport transport(
    transport_recv_buffer,
    sizeof(transport_recv_buffer),
    agent_address);

XrceClient client(transport, CLIENT_KEY);

XrceDataReader reader(client, READER_ID);

struct SensorData
{
    uint8_t id[4];
    uint32_t value;
};

/**
 * Listener callback used to notify the application of
 * new XRCE data received by the client.
 */
void on_data(
    void *const listener,
    XrceClient& client,
    const XrceReceivedData& recv_data)
{
    SensorData data;
    const uint8_t *data_ptr = recv_data.data();
    memcpy(data.id, data_ptr, sizeof(data.id));
    data_ptr += sizeof(data.id);
    NANO_u32_deserialize(
        &data.value, data_ptr, recv_data.little_endian());

    Serial.print("SENSOR DATA: id=0x");
    for (size_t i = 0; i < 4; i++)
    {
        Serial.print(data.id[i], HEX);
    }
    Serial.print(", value=");
    Serial.println(data.value);
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
    
    while (!client.connected())
    {
      Serial.println("Connecting to XRCE agent...");
      client.connect(INIT_DELAY);
    }

    /* Set callback to be notified of received XRCE data */
    client.on_data_callback(on_data);

    Serial.println("Connected to XRCE agent");
    return true;
}

void setup()
{
    /* Initialize serial port */
    serial_init();
    
    /* Initialize wifi connection */
    wifi_connect();

    /* Initialize XRCE client and connect to XRCE agent */
    if (!xrce_connect())
    {
        while (1) {}
    }
}

void loop()
{
    /* Block until next sample is received */
    reader.wait_for_data();
}
