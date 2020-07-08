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

#include <nano/nano_client.hpp>

using namespace rti::nano::xrce;

Stream::Stream(Client& client, const uint8_t id)
    : _client(client), _id(id)
{
    
}

bool Stream::builtin() const
{
    return NANO_XRCE_StreamId_is_builtin(this->_id);
}

bool Stream::enable()
{
    if (!this->builtin())
    {
        if (!this->_client.register_stream(this->_id, this->storage()))
        {
            return false;
        }
    }
    return true;
}

BestEffortStream::BestEffortStream(
    Client& client,
    const uint8_t id)
        : Stream(client, id)
{
    NANO_XRCE_DefaultBestEffortStreamStorage_initialize(&this->_def_storage);
}

#if NANO_FEAT_RELIABILITY

ReliableStream::ReliableStream(
    Client& client,
    const uint8_t id)
    : Stream(client, id),
        _frag_out_buffer(NULL, 0),
        _frag_in_buffer(NULL, 0)
{
    NANO_XRCE_DefaultReliableStreamStorage_initialize(&this->_def_storage);
}

bool ReliableStream::enable()
{
    bool rc = false;

    if (!Stream::enable())
    {
        goto done;
    }

    if (this->_frag_out_buffer.valid())
    {
        if (!this->_client.enable_fragmentation_out(
                this->_id,
                this->_frag_out_buffer.buffer(),
                this->_frag_out_buffer.len()))
        {
            goto done;
        }
    }
    if (this->_frag_in_buffer.valid())
    {
        if (!this->_client.enable_fragmentation_in(
                this->_id,
                this->_frag_out_buffer.buffer(),
                this->_frag_out_buffer.len()))
        {
            goto done;
        }
    }

    rc = true;

done:

    return rc;
}

#endif /* NANO_FEAT_RELIABILITY */