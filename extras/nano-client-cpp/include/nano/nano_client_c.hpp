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

#ifndef nano_client_c_hpp
#define nano_client_c_hpp

#include "nano/nano_client.h"
#include <stdint.h>

namespace rti { namespace nano { namespace xrce {

#define XRCE_LOG_LEVEL_NONE     NANO_LOG_LEVEL_NONE
#define XRCE_LOG_LEVEL_ERROR    NANO_LOG_LEVEL_ERROR
#define XRCE_LOG_LEVEL_WARNING  NANO_LOG_LEVEL_WARNING
#define XRCE_LOG_LEVEL_INFO     NANO_LOG_LEVEL_INFO
#define XRCE_LOG_LEVEL_DEBUG    NANO_LOG_LEVEL_DEBUG
#define XRCE_LOG_LEVEL_TRACE    NANO_LOG_LEVEL_TRACE
#define XRCE_LOG_LEVEL_TRACE_FN NANO_LOG_LEVEL_TRACE_FN
#define XRCE_LOG_LEVEL_DEFAULT  XRCE_LOG_LEVEL_ERROR

#define XRCE_CLIENT_KEY_INVALID             0x0000
#define XRCE_SESSION_ID_INVALID             0x0000
#define XRCE_OBJECT_ID_INVALID              0x0000
#define XRCE_STREAM_ID_INVALID              0x0000
#define XRCE_TIMEOUT_INFINITE               NANO_TIMEOUT_INFINITE

typedef NANO_MessageBuffer MessageBuffer;

typedef NANO_XRCE_StreamId StreamId;

typedef NANO_XRCE_RequestId RequestId;

typedef NANO_XRCE_ObjectId ObjectId;

#if NANO_FEAT_SESSION_USER_STREAMS

typedef NANO_XRCE_UserStreamsStorage UserStreamsStorage;

typedef NANO_XRCE_DefaultUserStreamsStorage DefaultUserStreamsStorage;

#endif /* typedef NANO_XRCE_UserStreamsStorage UserStreamsStorage; */

typedef NANO_XRCE_ClientRequestsStorage ClientRequestsStorage;

typedef NANO_XRCE_DefaultClientRequestsStorage DefaultClientRequestsStorage;

typedef NANO_XRCE_StreamStorage StreamStorage;

typedef NANO_XRCE_DefaultBestEffortStreamStorage DefaultBestEffortStreamStorage;

#if NANO_FEAT_RELIABILITY

typedef NANO_XRCE_DefaultReliableStreamStorage DefaultReliableStreamStorage;

typedef NANO_XRCE_ReliableMessageLostReason ReliableMessageLostReason;

#endif /* NANO_FEAT_RELIABILITY */

typedef NANO_XRCE_ClientTransportProperties ClientTransportProperties;

typedef NANO_XRCE_ClientTransport ClientTransport;

typedef NANO_XRCE_ClientProperties ClientProperties;

typedef NANO_MessageBufferData  Data;

typedef NANO_XRCE_DataFormat DataFormat;

typedef NANO_XRCE_ObjectKind ObjectKind;

typedef NANO_XRCE_ReadSpecification ReadSpecification;

typedef NANO_XRCE_SeqNum SeqNum;

#define XRCE_DATA_BUFFER_SIZE(mtu_) \
    (NANO_OSAPI_Memory_align_size_up(\
        (mtu_), sizeof(rti::nano::xrce::Data)) / sizeof(rti::nano::xrce::Data))

class Buffer {
    public:
        Buffer(Data *const buffer, const size_t len)
        {
            this->set(buffer, len);
        }

        Data* buffer() const
        {
            return this->_buffer;
        }
        size_t len() const
        {
            return this->_len;
        }

        void set(Data *const buffer, const size_t len)
        {
            this->_buffer = buffer;
            this->_len = len;
        }

        bool valid() const
        {
            return this->_buffer != NULL && this->_len > 0;
        }

    private:
        Data *_buffer;
        size_t   _len;
};

} /* namespace xrce */ } /* namespace nano */ } /* namespace rti */

#endif /* nano_client_c_hpp */