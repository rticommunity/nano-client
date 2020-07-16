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

#ifndef nano_client_stream_hpp
#define nano_client_stream_hpp


/**
 * @file nano_client_stream.hpp
 * @brief C++ XRCE Message Streams API
 * 
 */


#include <nano/nano_client_c.hpp>

namespace rti { namespace nano { namespace xrce {

class Client;

/**
 * @addtogroup nano_api_cpp_streams
 * @{
 */

#if NANO_FEAT_RELIABILITY

#define XRCE_STREAM_FRAG_OUT_PAYLOAD_WITHOUT_KEY_SIZE(mtu_) \
    ((mtu_) - NANO_XRCE_INLINEMESSAGEHEADER_WITHOUT_KEY_SIZE)

#define XRCE_STREAM_FRAG_OUT_PAYLOAD_WITH_KEY_SIZE(mtu_) \
    ((mtu_) - NANO_XRCE_INLINEMESSAGEHEADER_WITH_KEY_SIZE)

#define XRCE_STREAM_FRAG_BUFFER_OUT_WITH_KEY_SIZE(mtu_) \
    NANO_XRCE_RELIABLESTREAM_SENDFRAGBUFFER_SIZE(\
        XRCE_STREAM_FRAG_OUT_PAYLOAD_WITH_KEY_SIZE((mtu_)),\
        NANO_LIMIT_RELIABLESTREAM_SENDQUEUE_MAX_LENGTH)

#define XRCE_STREAM_FRAG_BUFFER_OUT_WITHOUT_KEY_SIZE(mtu_) \
    NANO_XRCE_RELIABLESTREAM_SENDFRAGBUFFER_SIZE(\
        XRCE_STREAM_FRAG_OUT_PAYLOAD_WITHOUT_KEY_SIZE((mtu_)),\
        NANO_LIMIT_RELIABLESTREAM_SENDQUEUE_MAX_LENGTH)

#define XRCE_STREAM_FRAG_BUFFER_IN_SIZE(max_msg_size_) \
        XRCE_DATA_BUFFER_SIZE(max_msg_size_)

#endif

/**
 * @brief TODO
 * 
 */
class Stream
{
public:
    /**
     * @brief Construct a new Stream
     * 
     * @param client 
     * @param id 
     */
    Stream(Client& client, const uint8_t id);

    /**
     * @brief TODO
     * 
     * @return 
     */
    virtual bool enable();

    /**
     * @brief TODO
     * 
     * @return StreamId 
     */
    StreamId id() const
    {
        return this->_id;
    }

    /**
     * @brief TODO
     * 
     * @return 
     */
    bool builtin() const;

    /**
     * @brief TODO
     * 
     * @return StreamStorage* 
     */
    virtual StreamStorage *storage() = 0;

protected:
    Client& _client;

    NANO_XRCE_StreamId _id;
};

/**
 * @brief TODO
 * 
 */
class BestEffortStream : public Stream
{
public:
    /**
     * @brief Construct a new Best Effort Stream
     * 
     * @param client 
     * @param id 
     */
    BestEffortStream(Client& client, const uint8_t id);

    /**
     * @brief TODO
     * 
     * @return StreamStorage* 
     */
    StreamStorage *storage()
    {
        return &this->_def_storage.base;
    }

private:
    DefaultBestEffortStreamStorage _def_storage;
};

#if NANO_FEAT_RELIABILITY

/**
 * @brief TODO
 * 
 */
class ReliableStream : public Stream
{
public:
    /**
     * @brief Construct a new Reliable Stream
     * 
     * @param client 
     * @param id 
     */
    ReliableStream(Client& client, uint8_t id);

    /**
     * @brief TODO
     * 
     * @return 
     */
    bool enable();

    /**
     * @brief TODO
     * 
     * @return const Buffer& 
     */
    const Buffer& frag_out_buffer() const
    {
        return this->_frag_out_buffer;
    }
    /**
     * @brief TODO
     * 
     * @param buffer 
     * @param len 
     */
    void frag_out_buffer(Data *const buffer, const size_t len)
    {
        this->_frag_out_buffer.set(buffer, len);
    }
    /**
     * @brief TODO
     * 
     * @return const Buffer& 
     */
    const Buffer& frag_in_buffer() const
    {
        return this->_frag_out_buffer;
    }
    /**
     * @brief TPDP
     * 
     * @param buffer 
     * @param len 
     */
    void frag_in_buffer(Data *const buffer, const size_t len)
    {
        this->_frag_in_buffer.set(buffer, len);
    }
    /**
     * @brief TODO
     * 
     * @return StreamStorage* 
     */
    StreamStorage *storage()
    {
        return &this->_def_storage.base;
    }

private:
    DefaultReliableStreamStorage _def_storage;
    Buffer _frag_out_buffer;
    Buffer _frag_in_buffer;
};

#endif /* NANO_FEAT_RELIABILITY */

/** @} *//* nano_api_cpp_streams */

} /* namespace xrce */ } /* namespace nano */ } /* namespace rti */

#endif /* nano_client_stream_hpp */