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

#include <nano/nano_client_xcdr.hpp>

#if NANO_FEAT_CDR

using namespace rti::nano::xcdr;

Stream::Stream(
    Data *const buffer,
    const size_t buffer_size,
    const bool little_endian)
{
    NANO_CDR_Stream_initialize(
        &this->_stream,
        (NANO_u8*)buffer,
        buffer_size,
        (little_endian)?NANO_CDR_ENDIANNESS_LITTLE:NANO_CDR_ENDIANNESS_BIG,
        NANO_BOOL_FALSE);
    this->_rw = true;
}

Stream::Stream(
    const Data *const buffer,
    const size_t buffer_size,
    const bool little_endian)
{
    NANO_CDR_Stream_initialize(
        &this->_stream,
        (NANO_u8*)buffer,
        buffer_size,
        (little_endian)?NANO_CDR_ENDIANNESS_LITTLE:NANO_CDR_ENDIANNESS_BIG,
        NANO_BOOL_FALSE);
    this->_rw = true;
}

size_t Stream::offset() const
{
    return NANO_CDR_Stream_offset(&this->_stream);
}

size_t Stream::capacity() const
{
    return NANO_CDR_Stream_capacity(&this->_stream);
}

size_t Stream::buffer_len() const
{
    return NANO_CDR_Stream_buffer_len(&this->_stream);
}

uint8_t* Stream::head() const
{
    return NANO_CDR_Stream_head(&this->_stream);
}

uint8_t* Stream::buffer() const
{
    return NANO_CDR_Stream_buffer(&this->_stream);
}

void Stream::reset()
{
    NANO_CDR_Stream_reset(&this->_stream);
}

bool Stream::advance(const size_t amount)
{
    bool rc = false;

    if (NANO_RETCODE_OK != NANO_CDR_Stream_advance(&this->_stream, amount))
    {
        goto done;
    }

    rc = true;
done:
    return rc;
}

bool Stream::rewind(const size_t amount)
{
    bool rc = false;

    if (NANO_RETCODE_OK != NANO_CDR_Stream_rewind(&this->_stream, amount))
    {
        goto done;
    }

    rc = true;
done:
    return rc;
}

bool Stream::align_to(const size_t alignment)
{
    bool rc = false;

    if (this->_stream.cur_alignment != alignment)
    {
        NANO_u8 *head = this->head();
        NANO_u8 *a_head = NANO_OSAPI_Memory_align_ptr(head, alignment);
        if (!this->advance((a_head - head)))
        {
            goto done;
        }
        this->_stream.cur_alignment = alignment;
    }

    rc = true;
done:
    return rc;
}

bool Stream::serialize(const uint8_t val)
{
    bool rc = false;
    if (this->read_only())
    {
        goto done;
    }

    if (NANO_RETCODE_OK !=
            NANO_CDR_Stream_serialize_u8(&this->_stream, &val))
    {
        goto done;
    }

    rc = true;
done:
    return rc;
}

bool Stream::serialize(const uint16_t val)
{
    bool rc = false;
    if (this->read_only())
    {
        goto done;
    }

    if (NANO_RETCODE_OK !=
            NANO_CDR_Stream_serialize_u16(&this->_stream, &val))
    {
        goto done;
    }

    rc = true;
done:
    return rc;
}

bool Stream::serialize(const uint32_t val)
{
    bool rc = false;
    if (this->read_only())
    {
        goto done;
    }

    if (NANO_RETCODE_OK !=
            NANO_CDR_Stream_serialize_u32(
                &this->_stream, (const NANO_u32*)&val))
    {
        goto done;
    }

    rc = true;
done:
    return rc;
}

bool Stream::serialize(const uint64_t val)
{
    bool rc = false;
    if (this->read_only())
    {
        goto done;
    }

    if (NANO_RETCODE_OK !=
            NANO_CDR_Stream_serialize_u64(
                &this->_stream, (const NANO_u64*)&val))
    {
        goto done;
    }

    rc = true;
done:
    return rc;
}

bool Stream::serialize(const int8_t val)
{
    bool rc = false;
    if (this->read_only())
    {
        goto done;
    }

    if (NANO_RETCODE_OK !=
            NANO_CDR_Stream_serialize_i8(&this->_stream, &val))
    {
        goto done;
    }

    rc = true;
done:
    return rc;
}

bool Stream::serialize(const int16_t val)
{
    bool rc = false;
    if (this->read_only())
    {
        goto done;
    }

    if (NANO_RETCODE_OK !=
            NANO_CDR_Stream_serialize_i16(&this->_stream, &val))
    {
        goto done;
    }

    rc = true;
done:
    return rc;
}

bool Stream::serialize(const int32_t val)
{
    bool rc = false;
    if (this->read_only())
    {
        goto done;
    }

    if (NANO_RETCODE_OK !=
            NANO_CDR_Stream_serialize_i32(&this->_stream, &val))
    {
        goto done;
    }

    rc = true;
done:
    return rc;
}

bool Stream::serialize(const int64_t val)
{
    bool rc = false;
    if (this->read_only())
    {
        goto done;
    }

    if (NANO_RETCODE_OK !=
            NANO_CDR_Stream_serialize_i64(&this->_stream, &val))
    {
        goto done;
    }

    rc = true;
done:
    return rc;
}

bool Stream::serialize(const bool val)
{
    bool rc = false;
    const NANO_u8 as_u = val;

    if (this->read_only())
    {
        goto done;
    }

    if (NANO_RETCODE_OK !=
            NANO_CDR_Stream_serialize_u8(&this->_stream, &as_u))
    {
        goto done;
    }

    rc = true;
done:
    return rc;
}

bool Stream::serialize(const float val)
{
    bool rc = false;
    if (this->read_only())
    {
        goto done;
    }

    if (NANO_RETCODE_OK !=
            NANO_CDR_Stream_serialize_u32(&this->_stream, (NANO_u32*)&val))
    {
        goto done;
    }

    rc = true;
done:
    return rc;
}


bool Stream::serialize(const double val)
{
    bool rc = false;
    if (this->read_only())
    {
        goto done;
    }

    if (NANO_RETCODE_OK !=
            NANO_CDR_Stream_serialize_u64(&this->_stream, (NANO_u64*)&val))
    {
        goto done;
    }

    rc = true;
done:
    return rc;
}


bool Stream::serialize_string(
        const char *const val,
        const size_t max_len)
{
    bool rc = false;
    if (this->read_only())
    {
        goto done;
    }

    if (NANO_RETCODE_OK !=
            NANO_CDR_Stream_serialize_string(
                &this->_stream, &val, max_len))
    {
        goto done;
    }

    rc = true;
done:
    return rc;
}

bool Stream::serialize_octets(
        const uint8_t *const val,
        const size_t val_len)
{
    bool rc = false;
    if (this->read_only())
    {
        goto done;
    }

    if (NANO_RETCODE_OK !=
            NANO_CDR_Stream_serialize_octets(&this->_stream, val, val_len))
    {
        goto done;
    }

    rc = true;
done:
    return rc;
}

bool Stream::deserialize(uint8_t *const val)
{
    bool rc = false;

    if (NANO_RETCODE_OK !=
            NANO_CDR_Stream_deserialize_u8(&this->_stream, val))
    {
        goto done;
    }

    rc = true;
done:
    return rc;
}

bool Stream::deserialize(uint16_t *const val)
{
    bool rc = false;

    if (NANO_RETCODE_OK !=
            NANO_CDR_Stream_deserialize_u16(&this->_stream, val))
    {
        goto done;
    }

    rc = true;
done:
    return rc;
}

bool Stream::deserialize(uint32_t *const val)
{
    bool rc = false;

    if (NANO_RETCODE_OK !=
            NANO_CDR_Stream_deserialize_u32(&this->_stream, (NANO_u32*)val))
    {
        goto done;
    }

    rc = true;
done:
    return rc;
}

bool Stream::deserialize(uint64_t *const val)
{
    bool rc = false;

    if (NANO_RETCODE_OK !=
            NANO_CDR_Stream_deserialize_u64(
                &this->_stream, (NANO_u64*)val))
    {
        goto done;
    }

    rc = true;
done:
    return rc;
}

bool Stream::deserialize(int8_t *const val)
{
    bool rc = false;

    if (NANO_RETCODE_OK !=
            NANO_CDR_Stream_deserialize_i8(&this->_stream, val))
    {
        goto done;
    }

    rc = true;
done:
    return rc;
}

bool Stream::deserialize(int16_t *const val)
{
    bool rc = false;

    if (NANO_RETCODE_OK !=
            NANO_CDR_Stream_deserialize_i16(&this->_stream, val))
    {
        goto done;
    }

    rc = true;
done:
    return rc;
}

bool Stream::deserialize(int32_t *const val)
{
    bool rc = false;

    if (NANO_RETCODE_OK !=
            NANO_CDR_Stream_deserialize_i32(&this->_stream, val))
    {
        goto done;
    }

    rc = true;
done:
    return rc;
}

bool Stream::deserialize(int64_t *const val)
{
    bool rc = false;

    if (NANO_RETCODE_OK !=
            NANO_CDR_Stream_deserialize_i64(&this->_stream, val))
    {
        goto done;
    }

    rc = true;
done:
    return rc;
}

bool Stream::deserialize(bool *const val)
{
    bool rc = false;
    NANO_u8 uval = 0;

    if (NANO_RETCODE_OK !=
            NANO_CDR_Stream_deserialize_u8(&this->_stream, &uval))
    {
        goto done;
    }

    *val = (uval)?true:false;

    rc = true;
done:
    return rc;
}

bool Stream::deserialize(float *const val)
{
    bool rc = false;

    if (NANO_RETCODE_OK !=
            NANO_CDR_Stream_deserialize_u32(&this->_stream, (NANO_u32*)val))
    {
        goto done;
    }

    rc = true;
done:
    return rc;
}

bool Stream::deserialize(double *const val)
{
    bool rc = false;

    if (NANO_RETCODE_OK !=
            NANO_CDR_Stream_deserialize_u64(&this->_stream, (NANO_u64*)val))
    {
        goto done;
    }

    rc = true;
done:
    return rc;
}


bool Stream::deserialize_string(
        char *const val,
        size_t *const val_len,
        const size_t max_len)
{
    bool rc = false;
    NANO_usize str_len = 0;
    char *str = val;

    if (NANO_RETCODE_OK !=
            NANO_CDR_Stream_deserialize_string(
                &this->_stream, &str, max_len, &str_len))
    {
        goto done;
    }

    if (val_len != NULL)
    {
        *val_len = ((size_t)str_len);
    }

    rc = true;
done:
    return rc;
}


bool Stream::deserialize_octets(
        uint8_t *const val,
        const size_t val_len)
{
    bool rc = false;
    if (this->read_only())
    {
        goto done;
    }

    if (NANO_RETCODE_OK !=
            NANO_CDR_Stream_deserialize_octets(&this->_stream, val, val_len))
    {
        goto done;  
    }

    rc = true;
done:
    return rc;
}

#endif /* NANO_FEAT_CDR */