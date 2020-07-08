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

#ifndef nano_client_transport_udp_hpp
#define nano_client_transport_udp_hpp

#include <nano/nano_client_transport.hpp>
#include "nano/nano_core_xrce_transport_udpv4.h"

#if NANO_FEAT_TRANSPORT_PLUGIN_UDPV4

namespace rti { namespace nano { namespace xrce {

class UdpTransport : public Transport
{
public:
    static const uint16_t DEFAULT_AGENT_PORT =
        NANO_XRCE_TRANSPORT_UDP_AGENT_PORT_DEFAULT;
    
    static const uint16_t DEFAULT_CLIENT_PORT =
        NANO_XRCE_TRANSPORT_UDP_CLIENT_PORT_DEFAULT;

    UdpTransport(
        Data *const recv_buffer,
        const size_t recv_buffer_size,
        const uint8_t *agent_addr,
        const uint16_t agent_port = DEFAULT_AGENT_PORT,
        const uint16_t bind_port = DEFAULT_CLIENT_PORT);
    
    ClientTransportProperties* c_properties();
    ClientTransport* c_transport();

    void bind_port(const uint16_t port);
    uint16_t bind_port() const;

    void bind_address(
        const uint8_t *addr,
        const uint16_t port = DEFAULT_CLIENT_PORT);
    
    void agent_port(const uint16_t port);

    void agent_address(
        const uint8_t *addr,
        const uint16_t port = DEFAULT_AGENT_PORT);
    
private:
    NANO_XRCE_Udpv4ClientTransport _transport;
    NANO_XRCE_Udpv4ClientTransportProperties _properties;
};

} /* namespace xrce */ } /* namespace nano */ } /* namespace rti */

#endif /* NANO_FEAT_TRANSPORT_PLUGIN_UDPV4 */

#endif /* nano_client_transport_udp_hpp */