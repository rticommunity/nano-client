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

#ifndef nano_client_transport_serial_hpp
#define nano_client_transport_serial_hpp

/**
 * @file nano_client_transport_serial.hpp
 * @brief C++ XRCE Serial Client Transport
 * 
 */


#include <nano/nano_client_transport.hpp>
#include "nano/nano_core_xrce_transport_serial.h"

#if NANO_FEAT_TRANSPORT_PLUGIN_SERIAL

namespace rti { namespace nano { namespace xrce {

/**
 * @addtogroup nano_api_cpp_transport_serial
 * @{
 */

/**
 * @brief TODO
 * 
 */
class SerialTransport : public Transport
{
public:
    /**
     * @brief TODO
     * 
     */
    static const uint8_t DEFAULT_AGENT_ADDRESS = 0xFF;
    /**
     * @brief TODO
     * 
     */
    static const uint8_t DEFAULT_BIND_ADDRESS = 0x01;
    /**
     * @brief TODO
     * 
     */
    static const uint32_t DEFAULT_SPEED = 115200;
#if NANO_PLATFORM == NANO_PLATFORM_ARDUINO
    /**
     * @brief TODO
     * 
     */
    static const uint8_t DEFAULT_PORT = 0;
#else
    static const char *const DEFAULT_PORT;
#endif /* NANO_PLATFORM */

    /**
     * @brief TODO
     * 
     */
    SerialTransport(
        Data *const recv_buffer,
        const size_t recv_buffer_size,
#if NANO_PLATFORM == NANO_PLATFORM_ARDUINO
        const uint8_t port = 
#else
        const char *const port =
#endif
            SerialTransport::DEFAULT_PORT,
        const uint32_t speed = SerialTransport::DEFAULT_SPEED,
        const uint8_t agent_address = SerialTransport::DEFAULT_AGENT_ADDRESS,
        const uint8_t bind_address = SerialTransport::DEFAULT_BIND_ADDRESS);

    /**
     * @brief TODO
     * 
     * @param speed 
     */
    void speed(const uint32_t speed);

#if NANO_PLATFORM == NANO_PLATFORM_ARDUINO
    void port(const uint8_t port);
#else
    /**
     * @brief TODO
     * 
     * @param port 
     */
    void port(const char *const port);
#endif

    /**
     * @brief TODO
     * 
     * @param addr 
     */
    void bind_address(const uint8_t addr);

    /**
     * @brief TODO
     * 
     * @param addr 
     */
    void agent_address(const uint8_t addr);

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
    
private:
    NANO_XRCE_SerialClientTransport _transport;
    NANO_XRCE_SerialClientTransportProperties _properties;
};

/** @} *//* nano_api_cpp_transport_serial */

} /* namespace xrce */ } /* namespace nano */ } /* namespace rti */

#endif /* NANO_FEAT_TRANSPORT_PLUGIN_SERIAL */

#endif /* nano_client_transport_serial_hpp */