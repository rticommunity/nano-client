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
 
   nanoagentd -S -Sd /dev/ttyUSB0 -Ss 115200 -St 1000 \
              -c nano-client-arduino/extras/sensor_agent.xml
 
 ******************************************************************************/

#include <nano_client_arduino.h>

#define SENSOR_ID           0x00000001
#define CLIENT_KEY          0x01020304
#define BASE_OBJECT_ID      0x0010
#define TRANSPORT_MTU       128
#define PUBLISH_DELAY       1000
#define INIT_DELAY          5000

XrceData transport_recv_buffer[
            XRCE_TRANSPORT_RECV_BUFFER_SIZE(TRANSPORT_MTU)] = { 0 };

XrceSerialTransport transport(
    transport_recv_buffer, sizeof(transport_recv_buffer));

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

void setup()
{
    /* Initialize XRCE client and connect to XRCE agent */
    if (!client.initialize())
    {
        // Failed to initialize XRCE client
        while (1) {}
    }

    while (!client.connected())
    {
        client.connect(INIT_DELAY);
    }

    /* Attach XRCE objects to client session */
    if (!participant.create())
    {
        // Failed to create DomainParticipant
        while (1) {}
    }
    if (!publisher.create())
    {
        // Failed to create Publisher
        while (1) {}
    }

    if (!writer.create())
    {
        // Failed to create DataWriter
        while (1) {}
    }

    /* Initialize sensor data */
    data.value = 0;
    /* Store sensor id as big endian */
    NANO_u32_serialize(SENSOR_ID, data.id, false);
}

void loop()
{
    data.value += 1;
    writer.write_data((uint8_t*)&data, sizeof(data));
    delay(PUBLISH_DELAY);
}
