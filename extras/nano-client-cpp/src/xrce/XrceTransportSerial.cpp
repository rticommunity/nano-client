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

#include <nano/nano_client_transport_serial.hpp>

using namespace rti::nano::xrce;

#if NANO_PLATFORM != NANO_PLATFORM_ARDUINO
const char *const DEFAULT_PORT = "/dev/ttyUSB0";
#endif /* NANO_PLATFORM */

SerialTransport::SerialTransport(
    Data *const recv_buffer,
    const size_t recv_buffer_size,
#if NANO_PLATFORM == NANO_PLATFORM_ARDUINO
    const uint8_t port,
#else
    const char *const port,
#endif
    const uint32_t speed,
    const uint8_t agent_address,
    const uint8_t bind_address)
    : Transport(recv_buffer, recv_buffer_size),
    _transport(),
    _properties()
{
    this->_properties.recv_buffer = (NANO_u8*)this->_recv_buffer.buffer();
    this->_properties.recv_buffer_size = this->_recv_buffer.len();
    this->_properties.base.mtu = this->_recv_buffer.len();
    // this->agent_address(agent_address);
    this->agent_address(agent_address);
    this->bind_address(bind_address);
    this->speed(speed);
    this->port(port);
}

void SerialTransport::speed(const uint32_t speed)
{
    this->_properties.connection.speed = speed;
}

#if NANO_PLATFORM == NANO_PLATFORM_ARDUINO
void SerialTransport::port(const uint8_t port)
{
    if (port == 1)
    {
        this->_properties.connection.port = NANO_ARDUINO_SERIAL_PORT_1;
    }
    else
    {
        this->_properties.connection.port = NANO_ARDUINO_SERIAL_PORT_0;
    }
}
#else
void SerialTransport::port(const char *const port)
{
    this->_properties.connection.device = port;
}
#endif /* NANO_PLATFORM == NANO_PLATFORM_ARDUINO */

void SerialTransport::bind_address(const uint8_t addr)
{
    this->_properties.bind_address.address[0] = addr;
}
void SerialTransport::agent_address(const uint8_t addr)
{
    this->_properties.base.agent_address.format =
        NANO_XRCE_ADDRESS_FORMAT_SMALL;
    this->_properties.base.agent_address.value.small.address[0] = addr;
}


ClientTransportProperties* SerialTransport::c_properties()
{
    return &this->_properties.base;
}
ClientTransport* SerialTransport::c_transport()
{
    return &this->_transport.base;
}
