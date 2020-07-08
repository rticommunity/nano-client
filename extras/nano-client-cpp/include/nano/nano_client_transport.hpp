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

#ifndef rti_nano_client_transport_hpp
#define rti_nano_client_transport_hpp

#include <nano/nano_client_c.hpp>

namespace rti { namespace nano { namespace xrce {

#define XRCE_TRANSPORT_RECV_BUFFER_SIZE(mtu_) \
    XRCE_DATA_BUFFER_SIZE((mtu_))

class Transport
{
    public:
        Transport(
            Data *const recv_buffer,
            const size_t recv_buffer_size) :
            _recv_buffer(recv_buffer, recv_buffer_size)
        {
            
        }
    
        virtual ClientTransportProperties* c_properties() = 0;
        
        virtual ClientTransport* c_transport() = 0;

    protected:
        Buffer _recv_buffer;

};

} /* namespace xrce */ } /* namespace nano */ } /* namespace rti */

#endif /* rti_nano_client_transport_hpp */