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
              -a -c nano-client-arduino/extras/sensor_agent.xml
 
 ******************************************************************************/

#include <nano_client_arduino.h>

#define CLIENT_KEY          0x05060708
#define READER_ID           0x5BB6
#define TRANSPORT_MTU       128
#define SERIAL_PORT         1
#define INIT_DELAY          5000

XrceData transport_recv_buffer[
            XRCE_TRANSPORT_RECV_BUFFER_SIZE(TRANSPORT_MTU)] = { 0 };

XrceSerialTransport transport(
    transport_recv_buffer, sizeof(transport_recv_buffer));

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

    /* Do something with deserialized data */
}

void setup()
{
    if (!client.initialize())
    {
        // Failed to initialize XRCE client
        while (1) {}
    }

    /* Initialize XRCE client and connect to XRCE agent */
    while (!client.connected())
    {
        client.connect(INIT_DELAY);
    }

    /* Set callback to be notified of received XRCE data */
    client.on_data_callback(on_data);

    /* Request data from agent */
    if (!reader.read_data())
    {
        // Failed to request data from agent
        while (1) {}
    }
}

void loop()
{
    /* Block until next sample is received */
    reader.wait_for_data();
}
