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


/**
 * @file nano_client_client.hpp
 * @brief C++ XRCE Client API
 * 
 */

#include <nano/nano_client_c.hpp>
#include <nano/nano_client_transport.hpp>
#include <nano/nano_client_stream.hpp>



namespace rti { namespace nano { namespace xrce {

/**
 * @addtogroup nano_api_cpp_client
 * @{
 */

/**
 * @brief TODO
 * 
 */
class ReceivedData
{
public:
    /**
     * @brief Construct a new Received Data object
     * 
     * @param stream_id 
     * @param read_request 
     * @param reader_id 
     * @param little_endian 
     * @param data 
     * @param data_len 
     */
    ReceivedData(
        const StreamId stream_id,
        const RequestId *const read_request,
        const ObjectId *const reader_id,
        const bool little_endian,
        const uint8_t *const data,
        const size_t data_len);

    /**
     * @brief TODO
     * 
     * @return uint8_t 
     */
    uint8_t stream_id() const
    {
        return this->_stream_id;
    }

    /**
     * @brief TODO
     * 
     * @return uint16_t 
     */
    uint16_t read_request() const
    {
        return this->_read_request;
    }

    /**
     * @brief TODO
     * 
     * @return uint16_t 
     */
    uint16_t reader_id() const
    {
        return this->_reader_id;
    }

    /**
     * @brief TODO
     * 
     * @return const uint8_t* 
     */
    const uint8_t* data() const
    {
        return this->_data;
    }

    /**
     * @brief TODO
     * 
     * @return uint16_t 
     */
    uint16_t data_len() const
    {
        return this->_data_len;
    }

    /**
     * @brief TODO
     * 
     * @return true 
     * @return false 
     */
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

/**
 * @brief TODO
 * 
 */
typedef void
    (*Client_OnDataCallback)(
        void *const listener,
        Client& client,
        const ReceivedData& data);

#if NANO_FEAT_PUBLISH
/**
 * @brief TODO
 * 
 */
typedef void
    (*Client_OnWriteCompleteCallback)(
        void *const listener,
        Client& client,
        const uint16_t writer_id,
        const uint8_t stream_id);
#endif /* NANO_FEAT_PUBLISH */

/**
 * @brief TODO
 * 
 */
typedef NANO_XRCE_ClientRequest ClientRequest;
/**
 * @brief TODO
 * 
 */
typedef NANO_XRCE_ClientRequestToken ClientRequestToken;

/**
 * @brief TODO
 * 
 */
class Client
{
public:
#if NANO_FEAT_RELIABILITY
    /**
     * @brief TODO
     * 
     */
    static const uint8_t  DEFAULT_REQUEST_STREAM    = NANO_XRCE_STREAMID_BUILTIN_RELIABLE; 
    /**
     * @brief TODO
     * 
     */
    static const uint32_t DEFAULT_HEARTBEAT_PERIOD  = 500 /* ms */;
#else
    static const uint8_t  DEFAULT_REQUEST_STREAM    = NANO_XRCE_STREAMID_BUILTIN_BEST_EFFORTS; 
#endif

    /**
     * @brief Construct a new Client object
     * 
     * @param transport 
     * @param client_key 
     */
    Client(
        Transport& transport,
        const uint32_t client_key);

    /**
     * @brief TODO
     * 
     * @return uint32_t 
     */
    uint32_t key() const
    {
        return this->_client_key;
    }

    /**
     * @brief TODO
     * 
     * @return uint8_t 
     */
    uint8_t session() const
    {
        return this->_client.session.id;
    }

    /**
     * @brief TODO
     * 
     * @param include_key 
     */
    void include_key_in_messages(const bool include_key);

    /**
     * @brief TODO
     * 
     * @param level 
     */
    void log_level(const uint8_t level) const;

    /**
     * @brief TODO
     * 
     * @return 
     */
    bool initialize();

    /**
     * @brief TODO
     * 
     */
    void finalize();

    /**
     * @brief TODO
     * 
     * @param timeout_ms 
     * @param timedout 
     * @return true 
     * @return false 
     */
    bool connect(
        const int32_t timeout_ms = XRCE_TIMEOUT_INFINITE,
        bool *const timedout = NULL);

    /**
     * @brief TODO
     * 
     * @param timeout_ms 
     * @param timedout 
     * @return true 
     * @return false 
     */
    bool disconnect(
        const int32_t timeout_ms = XRCE_TIMEOUT_INFINITE,
        bool *const timedout = NULL);

    /**
     * @brief TODO
     * 
     */
    void mark_disconnected();

    /**
     * @brief TODO
     * 
     * @return  
     */
    bool connected() const;

    /**
     * @brief TODO
     * 
     * @return BestEffortStream& 
     */
    BestEffortStream& builtin_be_stream()
    {
        return this->_stream_builtin_be;
    }

#if NANO_FEAT_RELIABILITY

    /**
     * @brief TODO
     * 
     * @param period_ms 
     */
    void heartbeat_period(const uint32_t period_ms);

    /**
     * @brief TODO
     * 
     * @return ReliableStream& 
     */
    ReliableStream& builtin_rel_stream()
    {
        return this->_stream_builtin_rel;
    }
#if NANO_FEAT_PUBLISH
    /**
     * @brief TODO
     * 
     * @param on_write_complete 
     * @param listener 
     */
    void on_write_complete_callback(
        Client_OnWriteCompleteCallback on_write_complete,
        void *const listener = NULL);
#endif /* NANO_FEAT_PUBLISH */

#endif /* NANO_FEAT_RELIABILITY */
    /**
     * @brief TODO
     * 
     * @param on_data 
     * @param listener 
     */
    void on_data_callback(
        Client_OnDataCallback on_data,
        void *const listener = NULL);

    /**
     * @brief TODO
     * 
     * @param stream_id 
     * @param storage 
     * @return true 
     * @return false 
     */
    bool register_stream(
        const uint8_t stream_id,
        StreamStorage *const storage);
    
#if NANO_FEAT_RELIABILITY
    /**
     * @brief TODO
     * 
     * @param stream_id 
     * @param frag_out_buffer 
     * @param frag_out_buffer_size 
     * @return true 
     * @return false 
     */
    bool enable_fragmentation_out(
        const uint8_t stream_id,
        Data *const frag_out_buffer,
        const size_t frag_out_buffer_size);
    
    /**
     * @brief TODO
     * 
     * @param stream_id 
     * @param frag_in_buffer 
     * @param frag_in_buffer_size 
     * @return true 
     * @return false 
     */
    bool enable_fragmentation_in(
        const uint8_t stream_id,
        Data *const frag_in_buffer,
        const size_t frag_in_buffer_size);
#endif

    /**
     * @brief Create a object
     * 
     * @param obj_kind 
     * @param obj_id 
     * @param parent_id 
     * @param representation 
     * @param ref_repr 
     * @param reuse 
     * @param replace 
     * @param timeout_ms 
     * @param stream_id 
     * @return  
     */
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
    
    /**
     * @brief TODO
     * 
     * @param obj_id 
     * @param timeout_ms 
     * @param stream_id 
     * @return 
     */
    bool delete_object(
        const ObjectId *const obj_id,
        const int32_t timeout_ms = XRCE_TIMEOUT_INFINITE,
        const uint8_t stream_id = XRCE_STREAM_ID_INVALID);
    
    /**
     * @brief TODO
     * 
     * @param request_out 
     * @param reader_id 
     * @param read_spec 
     * @param confirm 
     * @param timeout_ms 
     * @param stream_id 
     * @return 
     */
    bool read_data(
        ClientRequestToken *const request_out,
        const ObjectId *const reader_id,
        const ReadSpecification *const read_spec = NULL,
        const bool confirm = true,
        const int32_t timeout_ms = XRCE_TIMEOUT_INFINITE,
        const uint8_t stream_id = XRCE_STREAM_ID_INVALID);
    
    /**
     * @brief TODO
     * 
     * @param writer_id 
     * @param data 
     * @param data_len 
     * @param confirm 
     * @param timeout_ms 
     * @param stream_id 
     * @return 
     */
    bool write_data(
        const ObjectId *const writer_id,
        const uint8_t *const data,
        const size_t data_len,
        const bool confirm = false,
        const int32_t timeout_ms = XRCE_TIMEOUT_INFINITE,
        const uint8_t stream_id = XRCE_STREAM_ID_INVALID);
    
    /**
     * @brief TODO
     * 
     * @param writer_id 
     * @param format 
     * @param mbuf 
     * @param confirm 
     * @param timeout_ms 
     * @param stream_id 
     * @return 
     */
    bool write_mbuf(
        const ObjectId *const writer_id,
        const DataFormat format,
        MessageBuffer *const mbuf,
        const bool confirm = false,
        const int32_t timeout_ms = XRCE_TIMEOUT_INFINITE,
        const uint8_t stream_id = XRCE_STREAM_ID_INVALID);

    /**
     * @brief TODO
     * 
     * @param data 
     * @param data_len 
     * @param stream_id 
     * @return MessageBuffer* 
     */
    MessageBuffer* allocate_mbuf(
        const uint8_t *const data,
        const size_t data_len,
        const StreamId stream_id);
    
    /**
     * @brief TODO
     * 
     * @param mbuf 
     * @param stream_id 
     * @return 
     */
    bool release_mbuf(
        MessageBuffer *const mbuf,
        const StreamId stream_id);

    /**
     * @brief TODO
     * 
     * @param request_token 
     */
    void request_complete(
        ClientRequestToken *const request_token);

    /**
     * @brief TODO
     * 
     * @param request_token 
     * @param result_out 
     * @param timeout_ms 
     * @return 
     */
    bool wait_for_request(
        ClientRequestToken *const request_token,
        bool *const result_out = NULL,
        const int32_t timeout_ms = XRCE_TIMEOUT_INFINITE);
    
    /**
     * @brief TODO
     * 
     * @param request_token 
     * @param timeout_ms 
     * @return 
     */
    bool wait_for_data(
        ClientRequestToken *const request_token,
        const int32_t timeout_ms = XRCE_TIMEOUT_INFINITE);
    
#if NANO_FEAT_RELIABILITY
    /**
     * @brief TODO
     * 
     * @param stream_id 
     * @param timeout_ms 
     * @return 
     */
    bool wait_for_ack(
        const uint8_t stream_id = XRCE_STREAM_ID_INVALID,
        const int32_t timeout_ms = XRCE_TIMEOUT_INFINITE);
#endif
    /**
     * @brief TODO
     * 
     * @param max_messages 
     * @param timeout_ms 
     * @return 
     */
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

/** @} *//* nano_api_cpp_client */

} /* namespace xrce */ } /* namespace nano */ } /* namespace rti */


#endif /* nano_client_client_hpp */