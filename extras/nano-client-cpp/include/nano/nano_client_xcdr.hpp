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

#include <nano/nano_client_c.hpp>

#if NANO_FEAT_CDR

namespace rti { namespace nano { namespace xcdr {

typedef rti::nano::xrce::Data Data;

class Stream {
public:
    Stream(
        Data *const buffer,
        const size_t buffer_size,
        const bool little_endian = NANO_CDR_ENDIANNESS_NATIVE);

    Stream(
        const Data *const buffer,
        const size_t buffer_size,
        const bool little_endian = NANO_CDR_ENDIANNESS_NATIVE);

    bool serialize(const uint8_t val);

    bool serialize(const uint16_t val);

    bool serialize(const uint32_t val);

    bool serialize(const uint64_t val);

    bool serialize(const int8_t val);

    bool serialize(const int16_t val);

    bool serialize(const int32_t val);

    bool serialize(const int64_t val);

    bool serialize(const bool val);

    bool serialize(const float val);

    bool serialize(const double val);

    bool serialize_string(
            const char *const val,
            const size_t max_len = NANO_STRING_LENGTH_UNBOUNDED);
    
    bool serialize_octets(
            const uint8_t *const val,
            const size_t val_len);

    bool deserialize(uint8_t *const val);

    bool deserialize(uint16_t *const val);

    bool deserialize(uint32_t *const val);

    bool deserialize(uint64_t *const val);

    bool deserialize(int8_t *const val);

    bool deserialize(int16_t *const val);

    bool deserialize(int32_t *const val);

    bool deserialize(int64_t *const val);

    bool deserialize(bool *const val);

    bool deserialize(float *const val);

    bool deserialize(double *const val);

    bool deserialize_string(
            char *const val,
            size_t *const val_len,
            const size_t max_len = NANO_STRING_LENGTH_UNBOUNDED);
    
    bool deserialize_octets(
            uint8_t *const val,
            const size_t val_len);

    bool read_only() const {
        return !this->_rw;
    }

    size_t offset() const;

    size_t capacity() const;

    size_t buffer_len() const;

    uint8_t* head() const;

    uint8_t* buffer() const;

    bool advance(const size_t amount);

    bool rewind(const size_t amount);

    bool align_to(const size_t alignment);
    
    void reset();

private:
    NANO_CDR_Stream _stream;
    bool _rw;

};

} /* namespace xcdr */ } /* namespace nano */ } /* namespace rti */

#endif /* NANO_FEAT_CDR */

#endif /* nano_client_xcdr_hpp */