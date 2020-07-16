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


/**
 * @file nano_client_object.hpp
 * @brief C++ XRCE Objects API
 * 
 */

#include <nano/nano_client_client.hpp>

namespace rti { namespace nano { namespace xrce {

/**
 * @addtogroup nano_api_cpp_objects
 * @{
 */

/**
 * @brief TODO
 * 
 */
class Object
{
public:

    /**
     * @brief Construct a new Object
     * 
     * @param client 
     * @param kind 
     * @param id 
     * @param parent_id 
     * @param create_repr 
     * @param ref 
     * @param preferred_stream_id 
     */
    Object(
        Client&  client,
        const ObjectKind kind,
        const uint16_t id,
        const uint16_t parent_id = XRCE_OBJECT_ID_INVALID,
        const char *const create_repr = NULL,
        const bool ref = true,
        const uint8_t preferred_stream_id = XRCE_STREAM_ID_INVALID);
    
    /**
     * @brief TODO
     * 
     * @return const ObjectId& 
     */
    const ObjectId& xrce_id() const;

    /**
     * @brief TODO
     * 
     * @return uint16_t 
     */
    uint16_t id() const;

    /**
     * @brief TODO
     * 
     * @return const ObjectId& 
     */
    const ObjectId& parent_xrce_id() const;

    /**
     * @brief TODO
     * 
     * @return uint16_t 
     */
    uint16_t parent_id() const;

    /**
     * @brief TODO
     * 
     * @return const char* 
     */
    const char * create_representation() const;

    /**
     * @brief TODO
     * 
     * @return 
     */
    bool is_ref() const;

    /**
     * @brief TODO
     * 
     * @return uint8_t 
     */
    uint8_t preferred_stream_id() const;

    /**
     * @brief TODO
     * 
     * @return Client& 
     */
    Client& client();

    /**
     * @brief TODO
     * 
     * @return ObjectKind 
     */
    ObjectKind kind() const;

    /**
     * @brief TODO
     * 
     * @param reuse 
     * @param replace 
     * @param timeout_ms 
     * @param stream_id 
     * @return 
     */
    virtual bool create(
        const bool reuse = true,
        const bool replace = false,
        const int32_t timeout_ms = XRCE_TIMEOUT_INFINITE,
        const uint8_t stream_id = XRCE_STREAM_ID_INVALID);
    
    /**
     * @brief TODO
     * 
     * @param timeout_ms 
     * @param stream_id 
     * @return 
     */
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

/**
 * @brief TODO
 * 
 */
class Type : public Object
{
public:
    /**
     * @brief Construct a new Type
     * 
     * @param client 
     * @param id 
     * @param create_repr 
     * @param ref 
     * @param preferred_stream_id 
     */
    Type(
        Client&  client,
        const uint16_t id,
        const char *const create_repr = NULL,
        const bool ref = true,
        const uint8_t preferred_stream_id = XRCE_STREAM_ID_INVALID);
};

/**
 * @brief TODO
 * 
 */
class Domain : public Object
{
public:
    /**
     * @brief Construct a new Domain
     * 
     * @param client 
     * @param id 
     * @param create_repr 
     * @param ref 
     * @param preferred_stream_id 
     */
    Domain(
        Client&  client,
        const uint16_t id,
        const char *const create_repr = NULL,
        const bool ref = true,
        const uint8_t preferred_stream_id = XRCE_STREAM_ID_INVALID);
};

/**
 * @brief TODO
 * 
 */
class DomainParticipant : public Object
{
public:
    /**
     * @brief Construct a new Domain Participant
     * 
     * @param client 
     * @param id 
     * @param create_repr 
     * @param ref 
     * @param preferred_stream_id 
     */
    DomainParticipant(
        Client&  client,
        const uint16_t id,
        const char *const create_repr = NULL,
        const bool ref = true,
        const uint8_t preferred_stream_id = XRCE_STREAM_ID_INVALID);
};

/**
 * @brief TODO
 * 
 */
class Topic : public Object
{
public:
    /**
     * @brief Construct a new Topic
     * 
     * @param client 
     * @param id 
     * @param parent_id 
     * @param create_repr 
     * @param ref 
     * @param preferred_stream_id 
     */
    Topic(
        Client&  client,
        const uint16_t id,
        const uint16_t parent_id = XRCE_OBJECT_ID_INVALID,
        const char *const create_repr = NULL,
        const bool ref = true,
        const uint8_t preferred_stream_id = XRCE_STREAM_ID_INVALID);

};

/**
 * @brief TODO
 * 
 */
class Publisher : public Object
{
public:
    /**
     * @brief Construct a new Publisher
     * 
     * @param client 
     * @param id 
     * @param parent_id 
     * @param create_repr 
     * @param ref 
     * @param preferred_stream_id 
     */
    Publisher(
        Client&  client,
        const uint16_t id,
        const uint16_t parent_id = XRCE_OBJECT_ID_INVALID,
        const char *const create_repr = NULL,
        const bool ref = true,
        const uint8_t preferred_stream_id = XRCE_STREAM_ID_INVALID);

};

/**
 * @brief TODO
 * 
 */
class Subscriber : public Object
{
public:
    /**
     * @brief Construct a new Subscriber
     * 
     * @param client 
     * @param id 
     * @param parent_id 
     * @param create_repr 
     * @param ref 
     * @param preferred_stream_id 
     */
    Subscriber(
        Client&  client,
        const uint16_t id,
        const uint16_t parent_id = XRCE_OBJECT_ID_INVALID,
        const char *const create_repr = NULL,
        const bool ref = true,
        const uint8_t preferred_stream_id = XRCE_STREAM_ID_INVALID);

};

/**
 * @brief TODO
 * 
 */
class DataWriter : public Object
{
public:
    /**
     * @brief Construct a new Data Writer
     * 
     * @param client 
     * @param id 
     * @param parent_id 
     * @param create_repr 
     * @param ref 
     * @param preferred_stream_id 
     */
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

/**
 * @brief TODO
 * 
 */
class DataReader : public Object
{
public:
    /**
     * @brief Construct a new Data Reader
     * 
     * @param client 
     * @param id 
     * @param parent_id 
     * @param create_repr 
     * @param ref 
     * @param preferred_stream_id 
     */
    DataReader(
        Client&  client,
        const uint16_t id,
        const uint16_t parent_id = XRCE_OBJECT_ID_INVALID,
        const char *const create_repr = NULL,
        const bool ref = true,
        const uint8_t preferred_stream_id = XRCE_STREAM_ID_INVALID);

    /**
     * @brief TODO
     * 
     * @param val 
     */
    void max_samples(uint16_t val);
    /**
     * @brief TODO
     * 
     * @return uint16_t 
     */
    uint16_t max_samples() const;

    /**
     * @brief TODO
     * 
     * @param val 
     */
    void max_time(uint16_t val);
    /**
     * @brief TODO
     * 
     * @return uint16_t 
     */
    uint16_t max_time() const;

    /**
     * @brief TODO
     * 
     * @param val 
     */
    void min_interval(uint16_t val);
    /**
     * @brief TODO
     * 
     * @return uint16_t 
     */
    uint16_t min_interval() const;

    /**
     * @brief TODO
     * 
     * @param val 
     */
    void content_filter(const char *const val);
    /**
     * @brief TODO
     * 
     * @return const char* 
     */
    const char* content_filter() const;
    
    /**
     * @brief TODO
     * 
     * @param confirm 
     * @param timeout_ms 
     * @param stream_id 
     * @return 
     */
    bool read_data(
        const bool confirm = true,
        const int32_t timeout_ms = XRCE_TIMEOUT_INFINITE,
        const uint8_t stream_id = XRCE_STREAM_ID_INVALID);
    
    /**
     * @brief TODO
     * 
     * @param timeout_ms 
     * @return 
     */
    bool wait_for_data(
        const int32_t timeout_ms = XRCE_TIMEOUT_INFINITE);

    /**
     * @brief TODO
     * 
     * @param data 
     * @return 
     */
    bool owns_data(const ReceivedData& data);

private:
    ReadSpecification _read_spec;
    ClientRequestToken _req;
};

/** @} *//* nano_api_cpp_objects */

} /* namespace xrce */ } /* namespace nano */ } /* namespace rti */

#endif /* nano_client_object_hpp */