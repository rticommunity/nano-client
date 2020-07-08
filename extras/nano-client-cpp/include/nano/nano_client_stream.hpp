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

#include <nano/nano_client_c.hpp>

namespace rti { namespace nano { namespace xrce {

class Client;

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

class Stream
{
public:
    Stream(Client& client, const uint8_t id);

    virtual bool enable();

    StreamId id() const
    {
        return this->_id;
    }

    bool builtin() const;

    virtual StreamStorage *storage() = 0;

protected:
    Client& _client;

    NANO_XRCE_StreamId _id;
};

class BestEffortStream : public Stream
{
public:
    BestEffortStream(Client& client, const uint8_t id);

    StreamStorage *storage()
    {
        return &this->_def_storage.base;
    }

private:
    DefaultBestEffortStreamStorage _def_storage;
};

#if NANO_FEAT_RELIABILITY

class ReliableStream : public Stream
{
public:
    ReliableStream(Client& client, uint8_t id);

    bool enable();

    const Buffer& frag_out_buffer() const
    {
        return this->_frag_out_buffer;
    }
    void frag_out_buffer(Data *const buffer, const size_t len)
    {
        this->_frag_out_buffer.set(buffer, len);
    }

    const Buffer& frag_in_buffer() const
    {
        return this->_frag_out_buffer;
    }
    void frag_in_buffer(Data *const buffer, const size_t len)
    {
        this->_frag_in_buffer.set(buffer, len);
    }

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

} /* namespace xrce */ } /* namespace nano */ } /* namespace rti */

#endif /* nano_client_stream_hpp */