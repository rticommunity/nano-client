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

#ifndef nano_client_object_hpp
#define nano_client_object_hpp

#include <nano/nano_client_client.hpp>

namespace rti { namespace nano { namespace xrce {

class Object
{
public:

    Object(
        Client&  client,
        const ObjectKind kind,
        const uint16_t id,
        const uint16_t parent_id = XRCE_OBJECT_ID_INVALID,
        const char *const create_repr = NULL,
        const bool ref = true,
        const uint8_t preferred_stream_id = XRCE_STREAM_ID_INVALID);
    
    const ObjectId& xrce_id() const;

    uint16_t id() const;

    const ObjectId& parent_xrce_id() const;

    uint16_t parent_id() const;

    const char * create_representation() const;

    bool is_ref() const;

    uint8_t preferred_stream_id() const;

    Client& client();

    ObjectKind kind() const;

    virtual bool create(
        const bool reuse = true,
        const bool replace = false,
        const int32_t timeout_ms = XRCE_TIMEOUT_INFINITE,
        const uint8_t stream_id = XRCE_STREAM_ID_INVALID);
    
    bool delete_from_agent(
        const int32_t timeout_ms = XRCE_TIMEOUT_INFINITE,
        const uint8_t stream_id = XRCE_STREAM_ID_INVALID);

protected:
    Client& _client;
    NANO_XRCE_RepresentationRefAndXml _create_repr;
    ObjectId _id;
    ObjectId _parent;
    StreamId _preferred_stream_id;

    StreamId request_stream(const uint8_t user_stream);
};


class Type : public Object
{
public:
    Type(
        Client&  client,
        const uint16_t id,
        const char *const create_repr = NULL,
        const bool ref = true,
        const uint8_t preferred_stream_id = XRCE_STREAM_ID_INVALID);
};

class Domain : public Object
{
public:
    Domain(
        Client&  client,
        const uint16_t id,
        const char *const create_repr = NULL,
        const bool ref = true,
        const uint8_t preferred_stream_id = XRCE_STREAM_ID_INVALID);
};

class DomainParticipant : public Object
{
public:
    DomainParticipant(
        Client&  client,
        const uint16_t id,
        const char *const create_repr = NULL,
        const bool ref = true,
        const uint8_t preferred_stream_id = XRCE_STREAM_ID_INVALID);
};


class Topic : public Object
{
public:
    Topic(
        Client&  client,
        const uint16_t id,
        const uint16_t parent_id = XRCE_OBJECT_ID_INVALID,
        const char *const create_repr = NULL,
        const bool ref = true,
        const uint8_t preferred_stream_id = XRCE_STREAM_ID_INVALID);

};

class Publisher : public Object
{
public:
    Publisher(
        Client&  client,
        const uint16_t id,
        const uint16_t parent_id = XRCE_OBJECT_ID_INVALID,
        const char *const create_repr = NULL,
        const bool ref = true,
        const uint8_t preferred_stream_id = XRCE_STREAM_ID_INVALID);

};


class Subscriber : public Object
{
public:
    Subscriber(
        Client&  client,
        const uint16_t id,
        const uint16_t parent_id = XRCE_OBJECT_ID_INVALID,
        const char *const create_repr = NULL,
        const bool ref = true,
        const uint8_t preferred_stream_id = XRCE_STREAM_ID_INVALID);

};


class DataWriter : public Object
{
public:
    DataWriter(
        Client&  client,
        const uint16_t id,
        const uint16_t parent_id = XRCE_OBJECT_ID_INVALID,
        const char *const create_repr = NULL,
        const bool ref = true,
        const uint8_t preferred_stream_id = XRCE_STREAM_ID_INVALID);

    bool write_data(
        const uint8_t *const data,
        const size_t data_len,
        const bool confirm = false,
        const int32_t timeout_ms = XRCE_TIMEOUT_INFINITE,
        const uint8_t stream_id = XRCE_STREAM_ID_INVALID);

};

class DataReader : public Object
{
public:
    DataReader(
        Client&  client,
        const uint16_t id,
        const uint16_t parent_id = XRCE_OBJECT_ID_INVALID,
        const char *const create_repr = NULL,
        const bool ref = true,
        const uint8_t preferred_stream_id = XRCE_STREAM_ID_INVALID);

    void max_samples(uint16_t val);
    uint16_t max_samples() const;

    void max_time(uint16_t val);
    uint16_t max_time() const;

    void min_interval(uint16_t val);
    uint16_t min_interval() const;

    void content_filter(const char *const val);
    const char* content_filter() const;
    
    bool read_data(
        const bool confirm = true,
        const int32_t timeout_ms = XRCE_TIMEOUT_INFINITE,
        const uint8_t stream_id = XRCE_STREAM_ID_INVALID);
    
    bool wait_for_data(
        const int32_t timeout_ms = XRCE_TIMEOUT_INFINITE);
 
    bool owns_data(const ReceivedData& data);

private:
    ReadSpecification _read_spec;
    ClientRequestToken _req;
};

} /* namespace xrce */ } /* namespace nano */ } /* namespace rti */

#endif /* nano_client_object_hpp */