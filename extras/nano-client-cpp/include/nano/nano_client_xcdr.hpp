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

#ifndef nano_client_xcdr_hpp
#define nano_client_xcdr_hpp

/**
 * @file nano_client_xcdr.hpp
 * @brief C++ XCDR API
 * 
 */

#include <nano/nano_client_c.hpp>

#if NANO_FEAT_CDR

namespace rti { namespace nano { namespace xcdr {

/**
 * @addtogroup nano_api_cpp_xcdr
 * @{
 */

/**
 * @brief TODO
 * 
 */
typedef rti::nano::xrce::Data Data;

/**
 * @brief TODO
 * 
 */
class Stream {
public:
    /**
     * @brief Construct a new Stream
     * 
     * @param buffer 
     * @param buffer_size 
     * @param little_endian 
     */
    Stream(
        Data *const buffer,
        const size_t buffer_size,
        const bool little_endian = NANO_CDR_ENDIANNESS_NATIVE);

    /**
     * @brief Construct a new Stream
     * 
     * @param buffer 
     * @param buffer_size 
     * @param little_endian 
     */
    Stream(
        const Data *const buffer,
        const size_t buffer_size,
        const bool little_endian = NANO_CDR_ENDIANNESS_NATIVE);

    /**
     * @brief TODO
     * 
     * @param val 
     * @return 
     */
    bool serialize(const uint8_t val);

    /**
     * @brief TODO
     * 
     * @param val 
     * @return 
     */
    bool serialize(const uint16_t val);

    /**
     * @brief TODO
     * 
     * @param val 
     * @return 
     */
    bool serialize(const uint32_t val);

    /**
     * @brief TODO
     * 
     * @param val 
     * @return 
     */
    bool serialize(const uint64_t val);

    /**
     * @brief TODO
     * 
     * @param val 
     * @return 
     */
    bool serialize(const int8_t val);

    /**
     * @brief TODO
     * 
     * @param val 
     * @return 
     */
    bool serialize(const int16_t val);

    /**
     * @brief TODO
     * 
     * @param val 
     * @return 
     */
    bool serialize(const int32_t val);

    /**
     * @brief TODO
     * 
     * @param val 
     * @return 
     */
    bool serialize(const int64_t val);

    /**
     * @brief TODO
     * 
     * @param val 
     * @return 
     */
    bool serialize(const bool val);

    /**
     * @brief TODO
     * 
     * @param val 
     * @return 
     */
    bool serialize(const float val);

    /**
     * @brief TODO
     * 
     * @param val 
     * @return 
     */
    bool serialize(const double val);

    /**
     * @brief TODO
     * 
     * @param val 
     * @param max_len 
     * @return 
     */
    bool serialize_string(
            const char *const val,
            const size_t max_len = NANO_STRING_LENGTH_UNBOUNDED);
    
    /**
     * @brief TODO
     * 
     * @param val 
     * @param val_len 
     * @return 
     */
    bool serialize_octets(
            const uint8_t *const val,
            const size_t val_len);

    /**
     * @brief TODO
     * 
     * @param val 
     * @return 
     */
    bool deserialize(uint8_t *const val);

    /**
     * @brief TODO
     * 
     * @param val 
     * @return 
     */
    bool deserialize(uint16_t *const val);

    /**
     * @brief TODO
     * 
     * @param val 
     * @return 
     */
    bool deserialize(uint32_t *const val);

    /**
     * @brief TODO
     * 
     * @param val 
     * @return 
     */
    bool deserialize(uint64_t *const val);

    /**
     * @brief TODO
     * 
     * @param val 
     * @return 
     */
    bool deserialize(int8_t *const val);

    /**
     * @brief TODO
     * 
     * @param val 
     * @return 
     */
    bool deserialize(int16_t *const val);

    /**
     * @brief TODO
     * 
     * @param val 
     * @return 
     */
    bool deserialize(int32_t *const val);

    /**
     * @brief TODO
     * 
     * @param val 
     * @return 
     */
    bool deserialize(int64_t *const val);

    /**
     * @brief TODO
     * 
     * @param val 
     * @return 
     */
    bool deserialize(bool *const val);

    /**
     * @brief TODO
     * 
     * @param val 
     * @return 
     */
    bool deserialize(float *const val);

    /**
     * @brief TODO
     * 
     * @param val 
     * @return 
     */
    bool deserialize(double *const val);

    /**
     * @brief TODO
     * 
     * @param val 
     * @param val_len 
     * @param max_len 
     * @return 
     */
    bool deserialize_string(
            char *const val,
            size_t *const val_len,
            const size_t max_len = NANO_STRING_LENGTH_UNBOUNDED);
    
    /**
     * @brief TODO
     * 
     * @param val 
     * @param val_len 
     * @return 
     */
    bool deserialize_octets(
            uint8_t *const val,
            const size_t val_len);

    /**
     * @brief TODO
     * 
     * @return 
     */
    bool read_only() const {
        return !this->_rw;
    }

    /**
     * @brief TODO
     * 
     * @return size_t 
     */
    size_t offset() const;

    /**
     * @brief TODO
     * 
     * @return size_t 
     */
    size_t capacity() const;

    /**
     * @brief TODO
     * 
     * @return size_t 
     */
    size_t buffer_len() const;

    /**
     * @brief TODO
     * 
     * @return uint8_t* 
     */
    uint8_t* head() const;

    /**
     * @brief TODO
     * 
     * @return uint8_t* 
     */
    uint8_t* buffer() const;

    /**
     * @brief TODO
     * 
     * @param amount 
     * @return 
     */
    bool advance(const size_t amount);

    /**
     * @brief TODO
     * 
     * @param amount 
     * @return 
     */
    bool rewind(const size_t amount);

    /**
     * @brief TODO
     * 
     * @param alignment 
     * @return 
     */
    bool align_to(const size_t alignment);
    
    /**
     * @brief TODO
     * 
     */
    void reset();

private:
    NANO_CDR_Stream _stream;
    bool _rw;

};

/** @} *//* nano_api_cpp_xcdr */

} /* namespace xcdr */ } /* namespace nano */ } /* namespace rti */

#endif /* NANO_FEAT_CDR */

#endif /* nano_client_xcdr_hpp */