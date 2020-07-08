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

#include <nano/nano_client_client.hpp>

using namespace rti::nano::xrce;

ReceivedData::ReceivedData(
    const StreamId stream_id,
    const RequestId *const read_request,
    const ObjectId *const reader_id,
    const bool little_endian,
    const uint8_t *const data,
    const size_t data_len)
{
    this->_stream_id = stream_id;
    this->_read_request = NANO_XRCE_RequestId_to_u16(read_request);
    this->_reader_id = NANO_XRCE_ObjectId_to_u16(reader_id);
    this->_little_endian = little_endian;
    this->_data = data;
    this->_data_len = data_len;
}

void Client::notify_data(ReceivedData& data)
{
    if (this->_on_data != NULL)
    {
        this->_on_data(this->_on_data_listener, *this, data);
    }
}


namespace rti { namespace nano { namespace xrce {

void Client_on_data(
    NANO_XRCE_ClientListener *const listener,
    NANO_XRCE_Client *const client,
    const NANO_XRCE_StreamId stream_id,
    const NANO_XRCE_RequestId *const read_request,
    const NANO_XRCE_ObjectId *const reader,
    const NANO_XRCE_DataFormat format,
    const NANO_bool little_endian,
    const NANO_u8 *const data,
    const NANO_usize data_len)
{
    Client *const self = (Client *)listener->user_data;
    self->on_data(
        stream_id,
        read_request,
        reader,
        format,
        little_endian,
        data,
        data_len);
}

#if NANO_FEAT_RELIABILITY

void Client_on_write_complete(
    NANO_XRCE_ClientListener *const listener,
    NANO_XRCE_Client *const client,
    const NANO_XRCE_ClientRequest *const write_req)
{
    Client *const self = (Client *)listener->user_data;
    self->on_write_complete(write_req);
}

void Client_on_message_lost(
    NANO_XRCE_ClientListener *const listener,
    NANO_XRCE_Client *const client,
    const NANO_XRCE_StreamId stream_id,
    const NANO_XRCE_SeqNum lost,
    const NANO_XRCE_ReliableMessageLostReason reason)
{
    Client *const self = (Client *)listener->user_data;
    self->on_message_lost(stream_id, lost, reason);
}

#endif /* NANO_FEAT_RELIABILITY */

} /* namespace xrce */ } /* namespace nano */ } /* namespace rti */

void Client::on_data(
    const StreamId stream_id,
    const RequestId *const read_request,
    const ObjectId *const reader,
    const DataFormat format,
    const bool little_endian,
    const uint8_t *const data,
    const size_t data_len)
{
    if (format == NANO_XRCE_FORMAT_DATA)
    {
        ReceivedData recv_data(
            stream_id,
            read_request,
            reader,
            little_endian,
            data,
            data_len);
        this->notify_data(recv_data);
    }
}

#if NANO_FEAT_RELIABILITY

void Client::on_write_complete(
    const ClientRequest *const write_req)
{
    if (this->_on_write_complete != NULL)
    {
        this->_on_write_complete(
                this->_on_write_complete_listener,
                *this,
                NANO_XRCE_ObjectId_to_u16(&write_req->obj_id),
                write_req->stream_id);
    }
}

void Client::on_message_lost(
    const StreamId stream_id,
    const SeqNum lost,
    const ReliableMessageLostReason reason)
{

}

#endif /* NANO_FEAT_RELIABILITY */

Client::Client(
    Transport& transport,
    const uint32_t client_key)
: 
  _client_key(client_key),
  _initd(false),
  _transport(transport),
  _client(),
  _session_storage(),
  _client_storage(),
#if NANO_FEAT_SESSION_USER_STREAMS
  _streams_storage(),
#endif /* NANO_FEAT_SESSION_USER_STREAMS */
  _requests_storage(),
  _stream_builtin_be(*this, NANO_XRCE_STREAMID_BUILTIN_BEST_EFFORTS),
#if NANO_FEAT_RELIABILITY
  _stream_builtin_rel(*this, NANO_XRCE_STREAMID_BUILTIN_RELIABLE),
#if NANO_FEAT_PUBLISH
    _on_write_complete_listener(NULL),
    _on_write_complete(NULL),
#endif /* NANO_FEAT_PUBLISH */
#endif
  _on_data(NULL),
  _on_data_listener(NULL),
  _client_props()
{
    this->log_level(XRCE_LOG_LEVEL_DEFAULT);
    this->_session_storage.builtin_be = this->_stream_builtin_be.storage();
    this->_client_storage.session = &this->_session_storage;
    this->_client_storage.requests = &this->_requests_storage.base;
#if NANO_FEAT_SESSION_USER_STREAMS
    this->_session_storage.streams = &this->_streams_storage.base;
#endif /* NANO_FEAT_SESSION_USER_STREAMS */
#if NANO_FEAT_RELIABILITY
    this->_session_storage.builtin_rel = this->_stream_builtin_rel.storage();
    this->heartbeat_period(Client::DEFAULT_HEARTBEAT_PERIOD);
#endif
}

void Client::log_level(const uint8_t lvl) const
{
#if NANO_FEAT_LOG
    NANO_LOG_Writer_set_level(NANO_LOG_WRITER, lvl);
#endif /* NANO_FEAT_LOG */
}

void Client::include_key_in_messages(const bool include_key)
{
    this->_client_props.include_key_in_messages =
        (include_key) ? NANO_BOOL_TRUE : NANO_BOOL_FALSE;
}

#if NANO_FEAT_RELIABILITY

void Client::heartbeat_period(const uint32_t period_ms)
{
    this->_client_props.heartbeat_period.sec = period_ms / 1000;
    this->_client_props.heartbeat_period.nanosec =
        (period_ms - this->_client_props.heartbeat_period.sec * 1000) * 1000000;
}

#endif

void Client::on_data_callback(
    Client_OnDataCallback on_data,
    void *const listener)
{
    this->_on_data = on_data;
    this->_on_data_listener = listener;
}

bool Client::initialize()
{
    if (this->_initd)
    {
        return false;
    }
    
    this->_client_props.listener.on_data = Client_on_data;
#if NANO_FEAT_RELIABILITY
    this->_client_props.listener.on_message_lost = Client_on_message_lost;
    this->_client_props.listener.on_write_complete = Client_on_write_complete;
#endif
    this->_client_props.listener.user_data = this;

    this->_client_props.transport = this->_transport.c_transport();
    this->_client_props.transport_properties = this->_transport.c_properties();

    this->_client_props.storage = &this->_client_storage;

    NANO_XRCE_ClientKey_from_u32(&this->_client_props.key, this->_client_key);

    if (NANO_RETCODE_OK !=
            NANO_XRCE_Client_initialize(
                &this->_client, &this->_client_props))
    {
        return false;
    }
    
    if (!this->builtin_be_stream().enable())
    {
        return false;
    }
#if NANO_FEAT_RELIABILITY
    if (!this->builtin_rel_stream().enable())
    {
        return false;
    }
#endif

    this->_initd = true;

    return true;
}

void Client::finalize()
{
    if (this->_initd)
    {
        NANO_XRCE_Client_finalize(this->c_client());
        this->_initd = false;
    }
}

bool Client::connect(
    const int32_t timeout_ms,
    bool *const timedout)
{
    NANO_RetCode retcode = NANO_RETCODE_ERROR;
    bool rc = false;

    if (timedout != NULL)
    {
        *timedout = false;
    }

    if (!this->_initd)
    {
        if (!this->initialize())
        {
            goto done;
        }
    }

    retcode = NANO_XRCE_Client_connect(&this->_client, timeout_ms);
    if (NANO_RETCODE_OK != retcode)
    {
        if (timedout != NULL)
        {
            *timedout = (NANO_RETCODE_TIMEOUT == retcode);
        }
        goto done;
    }

    rc = true;
done:
    return rc;
}

bool Client::disconnect(
    const int32_t timeout_ms,
    bool *const timedout)
{
    NANO_RetCode retcode = NANO_RETCODE_ERROR;
    bool rc = false;

    if (timedout != NULL)
    {
        *timedout = false;
    }

    if (!this->_initd)
    {
        goto done;
    }

    retcode = 
        NANO_XRCE_Client_disconnect(
                &this->_client,
                NULL,
                Client::DEFAULT_REQUEST_STREAM,
                NANO_XRCE_REQUESTFLAGS_SYNC,
                timeout_ms);
    
    if (NANO_RETCODE_OK != retcode)
    {
        if (timedout != NULL)
        {
            *timedout = (NANO_RETCODE_TIMEOUT == retcode);
        }
        goto done;
    }

    rc = true;
done:
    return rc;
}

bool Client::connected() const
{
    if (!this->_initd)
    {
        return false;
    }
    return NANO_XRCE_Client_connected(&this->_client);
}

bool Client::register_stream(
    const uint8_t stream_id,
    StreamStorage *const storage)
{
#if NANO_FEAT_SESSION_USER_STREAMS_BESTEFFORT || \
    NANO_FEAT_SESSION_USER_STREAMS_RELIABLE
    bool rc = false;
    NANO_XRCE_Client *c_client = this->c_client();

    if (!this->_initd)
    {
        goto done;
    }

    if (NANO_RETCODE_OK !=
            NANO_XRCE_Session_register_stream(
                &c_client->session, stream_id, storage))
    {
        goto done;
    }

    rc = true;
done:
    return rc;
#else
    return false;
#endif /* NANO_FEAT_SESSION_USER_STREAMS_BESTEFFORT ||
            NANO_FEAT_SESSION_USER_STREAMS_RELIABLE */
}

#if NANO_FEAT_RELIABILITY

bool Client::enable_fragmentation_out(
    const uint8_t stream_id,
    Data *const frag_out_buffer,
    const size_t frag_out_buffer_size)
{
#if NANO_FEAT_FRAGMENT
    bool rc = false;
    NANO_XRCE_Client *client = this->c_client();
    bool has_key = NANO_XRCE_SessionId_has_client_key(this->session());
    const size_t mtu = this->_transport.c_properties()->mtu;
    const size_t transport_max_frag_payload =
        (has_key)?
            NANO_XRCE_RELIABLESTREAM_MAX_FRAGMENT_WITH_KEY_SIZE_BYTES(mtu) :
            NANO_XRCE_RELIABLESTREAM_MAX_FRAGMENT_WITHOUT_KEY_SIZE_BYTES(mtu);

    if (NANO_RETCODE_OK !=
            NANO_XRCE_Session_register_reliable_stream_fragment_storage(
                &client->session,
                stream_id,
                NANO_XRCE_MESSAGETYPE_OUTPUT_FRAGMENT,
                (NANO_u8*)frag_out_buffer,
                frag_out_buffer_size,
                transport_max_frag_payload))
    {
        goto done;
    }

    rc = true;
done:
    return rc;
#else
    return false;
#endif /* NANO_FEAT_FRAGMENT */
}

bool Client::enable_fragmentation_in(
    const uint8_t stream_id,
    Data *const frag_in_buffer,
    const size_t frag_in_buffer_size)
{
#if NANO_FEAT_FRAGMENT
    bool rc = false;
    NANO_XRCE_Client *client = this->c_client();

    if (NANO_RETCODE_OK !=
            NANO_XRCE_Session_register_reliable_stream_fragment_storage(
                &client->session,
                stream_id,
                NANO_XRCE_MESSAGETYPE_FRAGMENT_RECV_BUFFER,
                (NANO_u8*)frag_in_buffer,
                0,
                frag_in_buffer_size))
    {
        goto done;
    }

    rc = true;
done:
    return rc;
#else
    return false;
#endif /* NANO_FEAT_FRAGMENT */
}

#endif


uint8_t Client::request_stream(const uint8_t user_stream) const
{
    if (user_stream != XRCE_STREAM_ID_INVALID)
    {
        return user_stream;
    }
    else
    {
        return Client::DEFAULT_REQUEST_STREAM;
    }
}

bool Client::create_object(
    const ObjectKind obj_kind,
    const ObjectId *const obj_id,
    const ObjectId *const parent_id,
    const char *const representation,
    const bool ref_repr,
    const bool reuse,
    const bool replace,
    const int32_t timeout_ms,
    const uint8_t stream_id)
{
    bool rc = false;
    NANO_XRCE_CreateArgs args;
    NANO_XRCE_Client *client = this->c_client();
    const uint8_t sid = this->request_stream(stream_id);
    const NANO_XRCE_DomainId domain_id = 0;

    if (!this->_initd)
    {
        goto done;
    }
    
    if (representation == NULL)
    {
        goto done;
    }
    
    args.req.stream_id = sid;
    args.req.timeout_ms = timeout_ms;
    args.req.obj_id = *obj_id;

    args.req.flags |= NANO_XRCE_REQUESTFLAGS_SYNC;

    if (reuse)
    {
        args.req.flags |= NANO_XRCE_REQUESTFLAGS_CREATE_REUSE;
    }
    if (replace)
    {
        args.req.flags |= NANO_XRCE_REQUESTFLAGS_CREATE_REPLACE;
    }

    if (!ref_repr)
    {
        args.repr_fmt = NANO_XRCE_REPRESENTATION_AS_XML_STRING;
    }
    else
    {
        args.repr_fmt = NANO_XRCE_REPRESENTATION_BY_REFERENCE;
    }

    args.obj_kind = obj_kind;

    switch (obj_kind)
    {
    case NANO_XRCE_OBJK_PARTICIPANT:
    {
        args.tail_data = (NANO_u8*)&domain_id;
        args.tail_data_len = sizeof(domain_id);
        args.tail_data_align = sizeof(domain_id);
        break;
    }
    case NANO_XRCE_OBJK_TOPIC:
    {
        args.tail_data = (NANO_u8*)&parent_id;
        args.tail_data_len = sizeof(parent_id);
        args.tail_data_align = NANO_XRCE_ObjectId_alignment;
        break;
    }
    case NANO_XRCE_OBJK_PUBLISHER:
    {
        args.tail_data = (NANO_u8*)&parent_id;
        args.tail_data_len = sizeof(parent_id);
        args.tail_data_align = NANO_XRCE_ObjectId_alignment;
        break;
    }
    case NANO_XRCE_OBJK_SUBSCRIBER:
    {
        args.tail_data = (NANO_u8*)&parent_id;
        args.tail_data_len = sizeof(parent_id);
        args.tail_data_align = NANO_XRCE_ObjectId_alignment;
        break;
    }
    case NANO_XRCE_OBJK_DATAWRITER:
    {
        args.tail_data = (NANO_u8*)&parent_id;
        args.tail_data_len = sizeof(parent_id);
        args.tail_data_align = NANO_XRCE_ObjectId_alignment;
        break;
    }
    case NANO_XRCE_OBJK_DATAREADER:
    {
        args.tail_data = (NANO_u8*)&parent_id;
        args.tail_data_len = sizeof(parent_id);
        args.tail_data_align = NANO_XRCE_ObjectId_alignment;
        break;
    }
    default:
        break;
    }

    if (NANO_RETCODE_OK != 
            NANO_XRCE_Client_create_object_w_args(client, NULL, &args))
    {
        goto done;
    }

    rc = true;
done:
    return rc;
}

bool Client::delete_object(
    const ObjectId *const obj_id,
    const int32_t timeout_ms,
    const uint8_t stream_id)
{
    bool rc = false;
    NANO_XRCE_RequestArgs args;
    NANO_XRCE_Client *client = this->c_client();
    const uint8_t sid = this->request_stream(stream_id);

    if (!this->_initd)
    {
        goto done;
    }

    args.stream_id = sid;
    args.timeout_ms = timeout_ms;
    args.obj_id = *obj_id;

    if (NANO_RETCODE_OK != 
            NANO_XRCE_Client_delete_object_w_args(client, NULL, &args))
    {
        goto done;
    }

    rc = true;
done:
    return rc;
}

bool Client::read_data(
    ClientRequestToken *const request_token,
    const ObjectId *const reader_id,
    const ReadSpecification *const read_spec,
    const bool confirm,
    const int32_t timeout_ms,
    const uint8_t stream_id)
{
    bool rc = false;
    NANO_XRCE_ReadArgs sub_args;
    NANO_XRCE_Client *client = this->c_client();
    const uint8_t sid = this->request_stream(stream_id);

    sub_args.req.obj_id = *reader_id;
    sub_args.req.stream_id  = sid;
    sub_args.req.timeout_ms = timeout_ms;

    sub_args.req.flags |= NANO_XRCE_REQUESTFLAGS_SYNC;

    if (confirm)
    {
        sub_args.req.flags |= NANO_XRCE_REQUESTFLAGS_CONFIRM;

    }

    if (read_spec != NULL)
    {
        sub_args.read_spec = *read_spec;
        sub_args.read_spec.preferred_stream_id =
            this->request_stream(sub_args.read_spec.preferred_stream_id);
    }
    else
    {
        sub_args.read_spec.preferred_stream_id =
            this->request_stream(stream_id);
    }

    sub_args.read_spec.data_format = NANO_XRCE_FORMAT_DATA;

    if (NANO_RETCODE_OK !=
            NANO_XRCE_Client_read_w_args(client, request_token, &sub_args))
    {
        goto done;
    }
    
    rc = true;

done:
    return rc;
}

bool Client::write_data(
    const ObjectId *const writer_id,
    const uint8_t *const data,
    const size_t data_len,
    const bool confirm,
    const int32_t timeout_ms,
    const uint8_t stream_id)
{
    NANO_XRCE_StreamId sid = this->request_stream(stream_id);
    MessageBuffer *mbuf = this->allocate_mbuf(data, data_len, sid);
    if (mbuf == NULL)
    {
        return false;
    }
    return this->write_mbuf(
                writer_id,
                NANO_XRCE_FORMAT_DATA,
                mbuf,
                confirm,
                timeout_ms,
                stream_id);
}

bool Client::write_mbuf(
    const ObjectId *const writer_id,
    const DataFormat format,
    MessageBuffer *const mbuf,
    const bool confirm,
    const int32_t timeout_ms,
    const uint8_t stream_id)
{
    bool rc = false;
    NANO_XRCE_WriteArgs write_args;

    write_args.req.flags |= NANO_XRCE_REQUESTFLAGS_SYNC;
    if (confirm)
    {
        write_args.req.flags |= NANO_XRCE_REQUESTFLAGS_CONFIRM;
    }

    write_args.format = format;
    write_args.payload = mbuf;

    write_args.req.obj_id = *writer_id;
    write_args.req.stream_id = this->request_stream(stream_id);
    write_args.req.timeout_ms = timeout_ms;

    if (NANO_RETCODE_OK !=
            NANO_XRCE_Client_write_w_args(this->c_client(), NULL, &write_args))
    {
        goto done;
    }

    rc = true;
done:

    return rc;
}

MessageBuffer* Client::allocate_mbuf(
    const uint8_t *const data,
    const size_t data_len,
    const StreamId stream_id)
{
    return NANO_XRCE_Client_allocate_message_buffer(
                this->c_client(),
                this->request_stream(stream_id),
                data,
                data_len);
}


bool Client::release_mbuf(
    MessageBuffer *const mbuf,
    const StreamId stream_id)
{
    return NANO_XRCE_Client_release_message_buffer(
                this->c_client(),
                this->request_stream(stream_id),
                mbuf);
}


bool Client::wait_for_request(
    ClientRequestToken *const request,
    bool *const result_out,
    const int32_t timeout_ms)
{
    bool rc = false;
    NANO_XRCE_ResultStatus result_status;
    NANO_XRCE_Client *client = this->c_client();

    if (result_out != NULL)
    {
        *result_out = false;
    }

    if (NANO_RETCODE_OK !=
            NANO_XRCE_Client_wait_for_request_complete(
                client, request, timeout_ms, &result_status))
    {
        goto done;
    }

    if (result_out != NULL)
    {
        *result_out = (result_status.status == NANO_RETCODE_OK);
    }

    rc = true;
done:
    return rc;
}



bool Client::wait_for_data(
    ClientRequestToken *const request,
    const int32_t timeout_ms)
{
    bool rc = false;
    NANO_XRCE_Client *client = this->c_client();

    if (NANO_RETCODE_OK !=
            NANO_XRCE_Client_wait_for_data_received(
                client, request, timeout_ms))
    {
        goto done;
    }

    rc = true;
done:
    return rc;
}

#if NANO_FEAT_RELIABILITY

bool Client::wait_for_ack(
    const uint8_t stream_id,
    const int32_t timeout_ms)
{
    bool rc = false;
    NANO_XRCE_Client *client = this->c_client();

    if (NANO_RETCODE_OK !=
            NANO_XRCE_Session_run_until_acked(
                &client->session, stream_id, NULL, timeout_ms))
    {
        goto done;
    }

    rc = true;
done:
    return rc;
}

#endif

bool Client::run_session(
    const uint32_t max_messages,
    const int32_t timeout_ms)
{
    bool rc = false;
    NANO_XRCE_Client *client = this->c_client();

    if (NANO_RETCODE_OK !=
            NANO_XRCE_Session_run(
                &client->session, max_messages, timeout_ms))
    {
        goto done;
    }

    rc = true;
done:
    return rc;
}

void Client::request_complete(
    ClientRequestToken *const request)
{
    NANO_XRCE_Client *client = this->c_client();
    NANO_XRCE_Client_dismiss_request(client, request);
}

void Client::mark_disconnected()
{
    NANO_XRCE_Client_mark_disconnected(this->c_client());
}