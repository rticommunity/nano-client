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

#ifndef nano_client_client_hpp
#define nano_client_client_hpp

#include <nano/nano_client_c.hpp>
#include <nano/nano_client_transport.hpp>
#include <nano/nano_client_stream.hpp>

namespace rti { namespace nano { namespace xrce {

class ReceivedData
{
public:
    ReceivedData(
        const StreamId stream_id,
        const RequestId *const read_request,
        const ObjectId *const reader_id,
        const bool little_endian,
        const uint8_t *const data,
        const size_t data_len);

    uint8_t stream_id() const
    {
        return this->_stream_id;
    }

    uint16_t read_request() const
    {
        return this->_read_request;
    }

    uint16_t reader_id() const
    {
        return this->_reader_id;
    }

    const uint8_t* data() const
    {
        return this->_data;
    }

    uint16_t data_len() const
    {
        return this->_data_len;
    }

    bool little_endian() const
    {
        return this->_little_endian;
    }
private:

    uint8_t _stream_id;
    uint16_t _read_request;
    uint16_t _reader_id;
    const uint8_t *_data;
    uint16_t _data_len;
    bool _little_endian;

};

typedef void
    (*Client_OnDataCallback)(
        void *const listener,
        Client& client,
        const ReceivedData& data);

#if NANO_FEAT_PUBLISH
typedef void
    (*Client_OnWriteCompleteCallback)(
        void *const listener,
        Client& client,
        const uint16_t writer_id,
        const uint8_t stream_id);
#endif /* NANO_FEAT_PUBLISH */

typedef NANO_XRCE_ClientRequest ClientRequest;
typedef NANO_XRCE_ClientRequestToken ClientRequestToken;

class Client
{
public:
#if NANO_FEAT_RELIABILITY
    static const uint8_t  DEFAULT_REQUEST_STREAM    = NANO_XRCE_STREAMID_BUILTIN_RELIABLE; 
    static const uint32_t DEFAULT_ACKNACK_PERIOD    = 500 /* ms */;
    static const uint32_t DEFAULT_HEARTBEAT_PERIOD  = 500 /* ms */;
#else
    static const uint8_t  DEFAULT_REQUEST_STREAM    = NANO_XRCE_STREAMID_BUILTIN_BEST_EFFORTS; 
#endif

    Client(
        Transport& transport,
        const uint32_t client_key);

    uint32_t key() const
    {
        return this->_client_key;
    }

    uint8_t session() const
    {
        return this->_client.session.id;
    }

    void include_key_in_messages(const bool include_key);

    void log_level(const uint8_t level) const;

    bool initialize();

    void finalize();

    bool connect(
        const int32_t timeout_ms = XRCE_TIMEOUT_INFINITE,
        bool *const timedout = NULL);

    bool disconnect(
        const int32_t timeout_ms = XRCE_TIMEOUT_INFINITE,
        bool *const timedout = NULL);

    void mark_disconnected();

    bool connected() const;

    BestEffortStream& builtin_be_stream()
    {
        return this->_stream_builtin_be;
    }

#if NANO_FEAT_RELIABILITY

    void heartbeat_period(const uint32_t period_ms);

    ReliableStream& builtin_rel_stream()
    {
        return this->_stream_builtin_rel;
    }
#if NANO_FEAT_PUBLISH
    void on_write_complete_callback(
        Client_OnWriteCompleteCallback on_write_complete,
        void *const listener = NULL);
#endif /* NANO_FEAT_PUBLISH */

#endif /* NANO_FEAT_RELIABILITY */
    
    void on_data_callback(
        Client_OnDataCallback on_data,
        void *const listener = NULL);

    bool register_stream(
        const uint8_t stream_id,
        StreamStorage *const storage);
    
#if NANO_FEAT_RELIABILITY
    bool enable_fragmentation_out(
        const uint8_t stream_id,
        Data *const frag_out_buffer,
        const size_t frag_out_buffer_size);
    
    bool enable_fragmentation_in(
        const uint8_t stream_id,
        Data *const frag_in_buffer,
        const size_t frag_in_buffer_size);
#endif

    bool create_object(
        const ObjectKind obj_kind,
        const ObjectId *const obj_id,
        const ObjectId *const parent_id,
        const char *const representation,
        const bool ref_repr = true,
        const bool reuse = true,
        const bool replace = false,
        const int32_t timeout_ms = XRCE_TIMEOUT_INFINITE,
        const uint8_t stream_id = XRCE_STREAM_ID_INVALID);
    
    bool delete_object(
        const ObjectId *const obj_id,
        const int32_t timeout_ms = XRCE_TIMEOUT_INFINITE,
        const uint8_t stream_id = XRCE_STREAM_ID_INVALID);
    
    bool read_data(
        ClientRequestToken *const request_out,
        const ObjectId *const reader_id,
        const ReadSpecification *const read_spec = NULL,
        const bool confirm = true,
        const int32_t timeout_ms = XRCE_TIMEOUT_INFINITE,
        const uint8_t stream_id = XRCE_STREAM_ID_INVALID);
    
    bool write_data(
        const ObjectId *const writer_id,
        const uint8_t *const data,
        const size_t data_len,
        const bool confirm = false,
        const int32_t timeout_ms = XRCE_TIMEOUT_INFINITE,
        const uint8_t stream_id = XRCE_STREAM_ID_INVALID);
    
    bool write_mbuf(
        const ObjectId *const writer_id,
        const DataFormat format,
        MessageBuffer *const mbuf,
        const bool confirm = false,
        const int32_t timeout_ms = XRCE_TIMEOUT_INFINITE,
        const uint8_t stream_id = XRCE_STREAM_ID_INVALID);

    MessageBuffer* allocate_mbuf(
        const uint8_t *const data,
        const size_t data_len,
        const StreamId stream_id);
    
    bool release_mbuf(
        MessageBuffer *const mbuf,
        const StreamId stream_id);

    void request_complete(
        ClientRequestToken *const request_token);

    bool wait_for_request(
        ClientRequestToken *const request_token,
        bool *const result_out = NULL,
        const int32_t timeout_ms = XRCE_TIMEOUT_INFINITE);
    
    bool wait_for_data(
        ClientRequestToken *const request_token,
        const int32_t timeout_ms = XRCE_TIMEOUT_INFINITE);
    
#if NANO_FEAT_RELIABILITY

    bool wait_for_ack(
        const uint8_t stream_id = XRCE_STREAM_ID_INVALID,
        const int32_t timeout_ms = XRCE_TIMEOUT_INFINITE);
#endif
    
    bool run_session(
        const uint32_t max_messages = 0,
        const int32_t timeout_ms = 0);



protected:

    uint8_t request_stream(const uint8_t user_stream) const;

    NANO_XRCE_Client* c_client() {
        return &this->_client;
    }

    void notify_data(ReceivedData& data);

    virtual void on_data(
        const StreamId stream_id,
        const RequestId *const read_request,
        const ObjectId *const reader,
        const DataFormat format,
        const bool little_endian,
        const uint8_t *const data,
        const size_t data_len);

#if NANO_FEAT_RELIABILITY

    virtual void on_write_complete(
        const ClientRequest *const write_req);
    
    virtual void on_message_lost(
        const StreamId stream_id,
        const SeqNum lost,
        const ReliableMessageLostReason reason);

#endif /* NANO_FEAT_RELIABILITY */

private:
    uint32_t _client_key;
    bool _initd;
    Transport& _transport;
    NANO_XRCE_Client _client;
    NANO_XRCE_SessionStorage _session_storage;
    NANO_XRCE_ClientStorage _client_storage;
#if NANO_FEAT_SESSION_USER_STREAMS
    DefaultUserStreamsStorage _streams_storage;
#endif /* NANO_FEAT_SESSION_USER_STREAMS */
    DefaultClientRequestsStorage _requests_storage;
    BestEffortStream _stream_builtin_be;
#if NANO_FEAT_RELIABILITY
    ReliableStream _stream_builtin_rel;
#if NANO_FEAT_PUBLISH
    void *_on_write_complete_listener;
    Client_OnWriteCompleteCallback _on_write_complete;
#endif /* NANO_FEAT_PUBLISH */
#endif /* NANO_FEAT_RELIABILITY */
    Client_OnDataCallback _on_data;
    void *_on_data_listener;
    ClientProperties _client_props;

    friend void Client_on_data(
        NANO_XRCE_ClientListener *const listener,
        NANO_XRCE_Client *const client,
        const NANO_XRCE_StreamId stream_id,
        const NANO_XRCE_RequestId *const read_request,
        const NANO_XRCE_ObjectId *const reader,
        const NANO_XRCE_DataFormat format,
        const NANO_bool little_endian,
        const NANO_u8 *const data,
        const NANO_usize data_len);

#if NANO_FEAT_RELIABILITY

    friend void Client_on_write_complete(
        NANO_XRCE_ClientListener *const listener,
        NANO_XRCE_Client *const client,
        const NANO_XRCE_ClientRequest *const write_req);

    friend void Client_on_message_lost(
        NANO_XRCE_ClientListener *const listener,
        NANO_XRCE_Client *const client,
        const NANO_XRCE_StreamId stream_id,
        const NANO_XRCE_SeqNum lost,
        const NANO_XRCE_ReliableMessageLostReason reason);

#endif /* NANO_FEAT_RELIABILITY */
};

} /* namespace xrce */ } /* namespace nano */ } /* namespace rti */

#endif /* nano_client_client_hpp */