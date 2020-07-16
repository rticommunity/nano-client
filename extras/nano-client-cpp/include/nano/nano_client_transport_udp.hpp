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

/**
 * @file nano_client_transport_udp.hpp
 * @brief C++ XRCE UDPv4 Client Transport
 * 
 */

#include <nano/nano_client_transport.hpp>
#include "nano/nano_core_xrce_transport_udpv4.h"

#if NANO_FEAT_TRANSPORT_PLUGIN_UDPV4

namespace rti { namespace nano { namespace xrce {

/**
 * @addtogroup nano_api_cpp_transport_udpv4
 * @{
 */

/**
 * @brief TODO
 * 
 */
class UdpTransport : public Transport
{
public:
    /**
     * @brief TODO
     * 
     */
    static const uint16_t DEFAULT_AGENT_PORT =
        NANO_XRCE_TRANSPORT_UDP_AGENT_PORT_DEFAULT;
    
    /**
     * @brief TODO
     * 
     */
    static const uint16_t DEFAULT_CLIENT_PORT =
        NANO_XRCE_TRANSPORT_UDP_CLIENT_PORT_DEFAULT;

    /**
     * @brief Construct a new Udp Transport
     * 
     * @param recv_buffer 
     * @param recv_buffer_size 
     * @param agent_addr 
     * @param agent_port 
     * @param bind_port 
     */
    UdpTransport(
        Data *const recv_buffer,
        const size_t recv_buffer_size,
        const uint8_t *agent_addr,
        const uint16_t agent_port = DEFAULT_AGENT_PORT,
        const uint16_t bind_port = DEFAULT_CLIENT_PORT);
    
    /**
     * @brief TODO
     * 
     * @return ClientTransportProperties* 
     */
    ClientTransportProperties* c_properties();
    /**
     * @brief TODO
     * 
     * @return ClientTransport* 
     */
    ClientTransport* c_transport();

    /**
     * @brief TODO
     * 
     * @param port 
     */
    void bind_port(const uint16_t port);
    /**
     * @brief TODO
     * 
     * @return uint16_t 
     */
    uint16_t bind_port() const;

    /**
     * @brief TODO
     * 
     * @param addr 
     * @param port 
     */
    void bind_address(
        const uint8_t *addr,
        const uint16_t port = DEFAULT_CLIENT_PORT);
    /**
     * @brief TODO
     * 
     * @param port 
     */
    void agent_port(const uint16_t port);

    /**
     * @brief TODO
     * 
     * @param addr 
     * @param port 
     */
    void agent_address(
        const uint8_t *addr,
        const uint16_t port = DEFAULT_AGENT_PORT);
    
private:
    NANO_XRCE_Udpv4ClientTransport _transport;
    NANO_XRCE_Udpv4ClientTransportProperties _properties;
};

/** @} *//* nano_api_cpp_transport_udpv4 */

} /* namespace xrce */ } /* namespace nano */ } /* namespace rti */

#endif /* NANO_FEAT_TRANSPORT_PLUGIN_UDPV4 */

#endif /* nano_client_transport_udp_hpp */