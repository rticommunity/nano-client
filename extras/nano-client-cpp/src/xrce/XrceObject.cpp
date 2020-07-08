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

#include <nano/nano_client_object.hpp>

using namespace rti::nano::xrce;

Object::Object(
    Client&  client,
    const NANO_XRCE_ObjectKind kind,
    const uint16_t id,
    const uint16_t parent_id,
    const char *const create_repr,
    const bool ref,
    const uint8_t preferred_stream_id)
    : _client(client)
{
    NANO_XRCE_ObjectId_set(&this->_id, kind, id);

    NANO_XRCE_ObjectId_from_u16(&this->_parent, parent_id);

    if (ref)
    {
        this->_create_repr.repr.format = 
            NANO_XRCE_REPRESENTATION_BY_REFERENCE;
        this->_create_repr.repr.value.ref.value = (char*)create_repr;
    }
    else
    {
        this->_create_repr.repr.format = 
            NANO_XRCE_REPRESENTATION_AS_XML_STRING;
        this->_create_repr.repr.value.xml.value = (char*)create_repr;
    }

    this->_preferred_stream_id = preferred_stream_id;
}

const NANO_XRCE_ObjectId& Object::xrce_id() const
{
    return this->_id;
}

uint16_t Object::id() const
{
    return NANO_XRCE_ObjectId_to_u16(&this->_id);
}

const NANO_XRCE_ObjectId& Object::parent_xrce_id() const
{
    return this->_parent;
}

uint16_t Object::parent_id() const
{
    return NANO_XRCE_ObjectId_to_u16(&this->_parent);
}

const char * Object::create_representation() const
{
    switch (this->_create_repr.repr.format)
    {
    case NANO_XRCE_REPRESENTATION_BY_REFERENCE:
        return this->_create_repr.repr.value.ref.value;
    case NANO_XRCE_REPRESENTATION_AS_XML_STRING:
        return this->_create_repr.repr.value.xml.value;
    default:
        return NULL;
    }
}

bool Object::is_ref() const
{
    return
        (this->_create_repr.repr.format ==
                NANO_XRCE_REPRESENTATION_BY_REFERENCE);
}

uint8_t Object::preferred_stream_id() const
{
    return this->_preferred_stream_id;
}

Client& Object::client()
{
    return this->_client;
}

NANO_XRCE_ObjectKind Object::kind() const
{
    return NANO_XRCE_ObjectId_kind(&this->_id);
}

bool Object::create(
    const bool reuse,
    const bool replace,
    const int32_t timeout_ms,
    const uint8_t stream_id)
{
    return this->_client.create_object(
                this->kind(),
                &this->xrce_id(),
                &this->parent_xrce_id(),
                this->create_representation(),
                this->is_ref(),
                reuse,
                replace,
                timeout_ms,
                this->request_stream(stream_id));
}

bool Object::delete_from_agent(
    const int32_t timeout_ms,
    const uint8_t stream_id)
{
    return this->_client.delete_object(
                &this->xrce_id(),
                timeout_ms,
                this->request_stream(stream_id));
}

NANO_XRCE_StreamId Object::request_stream(const uint8_t user_stream)
{
    if (user_stream != XRCE_STREAM_ID_INVALID)
    {
        return user_stream;
    }
    else
    {
        return this->_preferred_stream_id;
    }
}

Type::Type(
    Client&  client,
    const uint16_t id,
    const char *const create_repr,
    const bool ref,
    const uint8_t preferred_stream_id)
    : Object(
        client,
        NANO_XRCE_OBJK_TYPE,
        id,
        XRCE_OBJECT_ID_INVALID,
        create_repr,
        ref,
        preferred_stream_id)
{
    
}

Domain::Domain(
    Client&  client,
    const uint16_t id,
    const char *const create_repr,
    const bool ref,
    const uint8_t preferred_stream_id)
    : Object(
        client,
        NANO_XRCE_OBJK_DOMAIN,
        id,
        XRCE_OBJECT_ID_INVALID,
        create_repr,
        ref,
        preferred_stream_id)
{

}

DomainParticipant::DomainParticipant(
    Client&  client,
    const uint16_t id,
    const char *const create_repr,
    const bool ref,
    const uint8_t preferred_stream_id)
: Object(
    client,
    NANO_XRCE_OBJK_PARTICIPANT,
    id,
    XRCE_OBJECT_ID_INVALID,
    create_repr,
    ref,
    preferred_stream_id)
{

}


Topic::Topic(
    Client&  client,
    const uint16_t id,
    const uint16_t parent_id,
    const char *const create_repr,
    const bool ref,
    const uint8_t preferred_stream_id)
    : Object(
        client,
        NANO_XRCE_OBJK_TOPIC,
        id,
        parent_id,
        create_repr,
        ref,
        preferred_stream_id)
{
    
}

Publisher::Publisher(
    Client&  client,
    const uint16_t id,
    const uint16_t parent_id,
    const char *const create_repr,
    const bool ref,
    const uint8_t preferred_stream_id)
    : Object(
        client,
        NANO_XRCE_OBJK_PUBLISHER,
        id,
        parent_id,
        create_repr,
        ref,
        preferred_stream_id)
{
    
}

Subscriber::Subscriber(
    Client&  client,
    const uint16_t id,
    const uint16_t parent_id,
    const char *const create_repr,
    const bool ref,
    const uint8_t preferred_stream_id)
    : Object(
        client,
        NANO_XRCE_OBJK_SUBSCRIBER,
        id,
        parent_id,
        create_repr,
        ref,
        preferred_stream_id)
{
    
}

DataWriter::DataWriter(
    Client&  client,
    const uint16_t id,
    const uint16_t parent_id,
    const char *const create_repr,
    const bool ref,
    const uint8_t preferred_stream_id)
    : Object(
        client,
        NANO_XRCE_OBJK_DATAWRITER,
        id,
        parent_id,
        create_repr,
        ref,
        preferred_stream_id)
{
    
}

bool DataWriter::write_data(
    const uint8_t *const data,
    const size_t data_len,
    const bool confirm,
    const int32_t timeout_ms,
    const uint8_t stream_id)
{
    uint8_t sid = this->request_stream(stream_id);

    if (!this->_client.write_data(
                &this->xrce_id(),
                data,
                data_len,
                confirm,
                timeout_ms,
                sid))
    {
        return false;
    }

    return true;
}

DataReader::DataReader(
    Client&  client,
    const uint16_t id,
    const uint16_t parent_id,
    const char *const create_repr,
    const bool ref,
    const uint8_t preferred_stream_id)
    : Object(
        client,
        NANO_XRCE_OBJK_DATAREADER,
        id, parent_id,
        create_repr,
        ref,
        preferred_stream_id)
{
    this->_read_spec.data_format = NANO_XRCE_FORMAT_DATA;
    this->_read_spec.preferred_stream_id =
        this->request_stream(preferred_stream_id);
    this->_read_spec.has_delivery_ctrl = NANO_BOOL_TRUE;
}

void DataReader::max_samples(uint16_t val)
{
    this->_read_spec.delivery_ctrl.max_samples = val;
}
uint16_t DataReader::max_samples() const
{
    if (this->_read_spec.has_delivery_ctrl)
    {
        return this->_read_spec.delivery_ctrl.max_samples;
    }
    return 0;
}

void DataReader::max_time(uint16_t val)
{
    this->_read_spec.delivery_ctrl.max_elapsed_time = val;
}
uint16_t DataReader::max_time() const
{
    if (this->_read_spec.has_delivery_ctrl)
    {
        return this->_read_spec.delivery_ctrl.max_elapsed_time;
    }
    return 0;
}

void DataReader::min_interval(uint16_t val)
{
    this->_read_spec.delivery_ctrl.min_pace_period = val;
}
uint16_t DataReader::min_interval() const 
{
    if (this->_read_spec.has_delivery_ctrl)
    {
        return this->_read_spec.delivery_ctrl.min_pace_period;
    }
    return 0;
}

void DataReader::content_filter(const char *const val)
{
    this->_read_spec.has_content_filter_expr = (val != NULL);
    this->_read_spec.content_filter_expr.value = (char*)val;
}
const char* DataReader::content_filter() const
{
    if (this->_read_spec.has_content_filter_expr)
    {
        return this->_read_spec.content_filter_expr.value;
    }
    return NULL;
}

bool DataReader::read_data(
    const bool confirm,
    const int32_t timeout_ms,
    const uint8_t stream_id)
{
    this->_client.request_complete(&this->_req);

    return this->_client.read_data(
                &this->_req,
                &this->xrce_id(),
                &this->_read_spec,
                confirm,
                timeout_ms,
                this->request_stream(stream_id));
}   


bool DataReader::wait_for_data(const int32_t timeout_ms)   
{
    return this->_client.wait_for_data(&this->_req, timeout_ms);
}

bool DataReader::owns_data(const ReceivedData& data)
{
    return (this->id() != data.reader_id());
}