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

/**
 * @file nano_client_c.hpp
 * @brief C++ wrappers for nano-client C API types.
 * 
 */

#ifndef nano_client_c_hpp
#define nano_client_c_hpp

#include "nano/nano_client.h"
#include <stdint.h>


namespace rti { namespace nano { namespace xrce {


/**
 * @addtogroup nano_api_cpp_infrastructure
 * @{
 */

/**
 * @brief TODO
 * 
 */
#define XRCE_LOG_LEVEL_NONE     NANO_LOG_LEVEL_NONE
/**
 * @brief TODO
 * 
 */
#define XRCE_LOG_LEVEL_ERROR    NANO_LOG_LEVEL_ERROR
/**
 * @brief TODO
 * 
 */
#define XRCE_LOG_LEVEL_WARNING  NANO_LOG_LEVEL_WARNING
/**
 * @brief TODO
 * 
 */
#define XRCE_LOG_LEVEL_INFO     NANO_LOG_LEVEL_INFO
/**
 * @brief TODO
 * 
 */
#define XRCE_LOG_LEVEL_DEBUG    NANO_LOG_LEVEL_DEBUG
/**
 * @brief TODO
 * 
 */
#define XRCE_LOG_LEVEL_TRACE    NANO_LOG_LEVEL_TRACE
/**
 * @brief TODO
 * 
 */
#define XRCE_LOG_LEVEL_TRACE_FN NANO_LOG_LEVEL_TRACE_FN
/**
 * @brief TODO
 * 
 */
#define XRCE_LOG_LEVEL_DEFAULT  XRCE_LOG_LEVEL_ERROR

/**
 * @brief TODO
 * 
 */
#define XRCE_CLIENT_KEY_INVALID             0x0000
/**
 * @brief TODO
 * 
 */
#define XRCE_SESSION_ID_INVALID             0x0000
/**
 * @brief TODO
 * 
 */
#define XRCE_OBJECT_ID_INVALID              0x0000
/**
 * @brief TODO
 * 
 */
#define XRCE_STREAM_ID_INVALID              0x0000
/**
 * @brief TODO
 * 
 */
#define XRCE_TIMEOUT_INFINITE               NANO_TIMEOUT_INFINITE

/**
 * @brief TODO
 * 
 */
typedef NANO_MessageBuffer MessageBuffer;

/**
 * @brief TODO
 * 
 */
typedef NANO_XRCE_StreamId StreamId;

/**
 * @brief TODO
 * 
 */
typedef NANO_XRCE_RequestId RequestId;

/**
 * @brief TODO
 * 
 */
typedef NANO_XRCE_ObjectId ObjectId;

#if NANO_FEAT_SESSION_USER_STREAMS

/**
 * @brief TODO
 * 
 */
typedef NANO_XRCE_UserStreamsStorage UserStreamsStorage;

/**
 * @brief TODO
 * 
 */
typedef NANO_XRCE_DefaultUserStreamsStorage DefaultUserStreamsStorage;

#endif /* typedef NANO_XRCE_UserStreamsStorage UserStreamsStorage; */

/**
 * @brief TODO
 * 
 */
typedef NANO_XRCE_ClientRequestsStorage ClientRequestsStorage;

/**
 * @brief TODO
 * 
 */
typedef NANO_XRCE_DefaultClientRequestsStorage DefaultClientRequestsStorage;

/**
 * @brief TODO
 * 
 */
typedef NANO_XRCE_StreamStorage StreamStorage;

/**
 * @brief TODO
 * 
 */
typedef NANO_XRCE_DefaultBestEffortStreamStorage DefaultBestEffortStreamStorage;

#if NANO_FEAT_RELIABILITY

/**
 * @brief TODO
 * 
 */
typedef NANO_XRCE_DefaultReliableStreamStorage DefaultReliableStreamStorage;

/**
 * @brief TODO
 * 
 */
typedef NANO_XRCE_ReliableMessageLostReason ReliableMessageLostReason;

#endif /* NANO_FEAT_RELIABILITY */

/**
 * @brief TODO
 * 
 */
typedef NANO_XRCE_ClientTransportProperties ClientTransportProperties;

/**
 * @brief TODO
 * 
 */
typedef NANO_XRCE_ClientTransport ClientTransport;

/**
 * @brief TODO
 * 
 */
typedef NANO_XRCE_ClientProperties ClientProperties;

/**
 * @brief TODO
 * 
 */
typedef NANO_MessageBufferData  Data;

/**
 * @brief TODO
 * 
 */
typedef NANO_XRCE_DataFormat DataFormat;

/**
 * @brief TODO
 * 
 */
typedef NANO_XRCE_ObjectKind ObjectKind;

/**
 * @brief TODO
 * 
 */
typedef NANO_XRCE_ReadSpecification ReadSpecification;

/**
 * @brief TODO
 * 
 */
typedef NANO_XRCE_SeqNum SeqNum;

/**
 * @brief TODO
 * 
 */
#define XRCE_DATA_BUFFER_SIZE(mtu_) \
    (NANO_OSAPI_Memory_align_size_up(\
        (mtu_), sizeof(rti::nano::xrce::Data)) / sizeof(rti::nano::xrce::Data))

/**
 * @brief TODO
 * 
 */
class Buffer {
    public:
        /**
         * @brief Construct a new Buffer object
         * 
         * @param buffer 
         * @param len 
         */
        Buffer(Data *const buffer, const size_t len)
        {
            this->set(buffer, len);
        }

        /**
         * @brief TODO
         * 
         * @return Data* 
         */
        Data* buffer() const
        {
            return this->_buffer;
        }
        /**
         * @brief TODO
         * 
         * @return size_t 
         */
        size_t len() const
        {
            return this->_len;
        }
        /**
         * @brief TODO
         * 
         * @param buffer 
         * @param len 
         */
        void set(Data *const buffer, const size_t len)
        {
            this->_buffer = buffer;
            this->_len = len;
        }
        /**
         * @brief TODO
         * 
         * @return true 
         * @return false 
         */
        bool valid() const
        {
            return this->_buffer != NULL && this->_len > 0;
        }
protected:
        Data *_buffer;
        size_t   _len;
};


/**
 * @}
 *//* nano_api_cpp_infrastructure */

} /* namespace xrce */ } /* namespace nano */ } /* namespace rti */

#endif /* nano_client_c_hpp */