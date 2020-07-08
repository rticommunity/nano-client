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

#include <nano/nano_client_transport_udp.hpp>

#if NANO_FEAT_TRANSPORT_PLUGIN_UDPV4

using namespace rti::nano::xrce;

UdpTransport::UdpTransport(
    Data *const recv_buffer,
    const size_t recv_buffer_size,
    const uint8_t *agent_addr,
    const uint16_t agent_port,
    const uint16_t bind_port)
    : Transport(recv_buffer, recv_buffer_size),
        _transport(),
        _properties()
{
#ifdef NANO_HAVE_SOCKET
    this->_properties.socket.flags |= NANO_SOCKETFLAGS_NONBLOCK;
#endif /* NANO_HAVE_SOCKET */
    this->_properties.recv_buffer = (NANO_u8*)this->_recv_buffer.buffer();
    this->_properties.recv_buffer_size = this->_recv_buffer.len();
    this->_properties.base.mtu = this->_recv_buffer.len();
    
    this->agent_address(agent_addr, agent_port);
    this->bind_port(bind_port);
}

ClientTransportProperties* UdpTransport::c_properties()
{
    return &this->_properties.base;
}
ClientTransport* UdpTransport::c_transport()
{
    return &this->_transport.base;
}
void UdpTransport::bind_port(const uint16_t port)
{
    this->_properties.bind_address.port = port;
}
uint16_t UdpTransport::bind_port() const
{
    return this->_properties.bind_address.port;
}

void UdpTransport::bind_address(
    const uint8_t *addr,
    const uint16_t port)
{
    NANO_OSAPI_Memory_copy(
        this->_properties.bind_address.address,
        addr,
        sizeof(NANO_u32));
    this->bind_port(port);
}

void UdpTransport::agent_port(const uint16_t port)
{
    this->_properties.base.agent_address.value.medium.port = port;
}

void UdpTransport::agent_address(
    const uint8_t *addr,
    const uint16_t port)
{
    this->_properties.base.agent_address.format =
        NANO_XRCE_ADDRESS_FORMAT_MEDIUM;
    
    NANO_OSAPI_Memory_copy(
        this->_properties.base.agent_address.value.medium.address,
        addr,
        sizeof(NANO_u32));
    
    this->agent_port(port);
}

#endif /* NANO_FEAT_TRANSPORT_PLUGIN_UDPV4 */