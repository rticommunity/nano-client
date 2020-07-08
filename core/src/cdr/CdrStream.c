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

#include "nano/nano_core.h"

#define NANO_MEM_UNBOUNDEDSEQUENCE_MAX_LENGTH       10
#define NANO_MEM_UNBOUNDEDSTRING_MAX_LENGTH     100

#if NANO_FEAT_CDR

/* Realtek Ameba SDK defines these symbols as aliases for std int types, 
   which breaks the build, so we must undefine them. */

#ifdef u8
#undef u8
#endif /* u8 */

#ifdef u16
#undef u16
#endif /* u16 */

#ifdef u32
#undef u32
#endif /* u32 */

#ifdef u64
#undef u64
#endif /* u64 */

#define NANO_CDR_get_byte_as(p_,b_,t_) \
    ((t_)*(((NANO_u8*)(p_))+((NANO_usize)(b_))))

#define NANO_CDR_swap_2(t_,d_) \
   (NANO_CDR_get_byte_as(d_,1,t_)       | \
    NANO_CDR_get_byte_as(d_,0,t_) << 8)

#define NANO_CDR_swap_4(t_,d_) \
   (NANO_CDR_get_byte_as(d_,3,t_)       | \
    NANO_CDR_get_byte_as(d_,2,t_) << 8  | \
    NANO_CDR_get_byte_as(d_,1,t_) << 16 | \
    NANO_CDR_get_byte_as(d_,0,t_) << 24)

#define NANO_CDR_swap_8(t_,d_) \
   (NANO_CDR_get_byte_as(d_,7,t_)       | \
    NANO_CDR_get_byte_as(d_,6,t_) << 8  | \
    NANO_CDR_get_byte_as(d_,5,t_) << 16 | \
    NANO_CDR_get_byte_as(d_,4,t_) << 24 | \
    NANO_CDR_get_byte_as(d_,3,t_) << 32 | \
    NANO_CDR_get_byte_as(d_,2,t_) << 40 | \
    NANO_CDR_get_byte_as(d_,1,t_) << 48 | \
    NANO_CDR_get_byte_as(d_,0,t_) << 56)

#define NANO_CDR_get_swap_2(t_,d_,swap_) \
    ((swap_)? NANO_CDR_swap_2(t_,d_) : *((t_*)d_))

#define NANO_CDR_get_swap_4(t_,d_,swap_) \
    ((swap_)? NANO_CDR_swap_4(t_,d_) : *((t_*)d_))

#define NANO_CDR_get_swap_8(t_,d_,swap_) \
    ((swap_)? NANO_CDR_swap_8(t_,d_) : *((t_*)d_))

#define NANO_CDR_set_swap_8(t_,s_,d_,swap_) \
{\
    if (swap_) \
    {\
        NANO_CDR_u64_set_ptr_swap((d_),(s_));\
    }\
    else\
    {\
        NANO_CDR_u64_set_ptr((d_),(s_));\
    }\
}

#define NANO_CDR_set_swap_4(t_,s_,d_,swap_) \
{\
    if (swap_) \
    {\
        NANO_CDR_u32_set_ptr_swap((d_),(s_));\
    }\
    else\
    {\
        NANO_CDR_u32_set_ptr((d_),(s_));\
    }\
}

#define NANO_CDR_set_swap_2(t_,s_,d_,swap_) \
{\
    if (swap_) \
    {\
        NANO_CDR_u16_set_ptr_swap((d_),(s_));\
    }\
    else\
    {\
        NANO_CDR_u16_set_ptr((d_),(s_));\
    }\
}

#define NANO_CDR_Primitive1_deserialize_cdr(t_,s_,sr_) \
    *((t_*)NANO_CDR_Stream_head((sr_)))

#define NANO_CDR_Primitive2_deserialize_cdr(t_,s_,sr_) \
    NANO_CDR_get_swap_2(t_,\
        NANO_CDR_Stream_head(sr_),\
        NANO_CDR_Stream_needs_swap(sr_))

#define NANO_CDR_Primitive4_deserialize_cdr(t_,s_,sr_) \
    NANO_CDR_get_swap_4(t_,\
        NANO_CDR_Stream_head(sr_),\
        NANO_CDR_Stream_needs_swap(sr_))

#define NANO_CDR_Primitive8_deserialize_cdr(t_,s_,sr_) \
    NANO_CDR_get_swap_8(t_,\
        NANO_CDR_Stream_head(sr_),\
        NANO_CDR_Stream_needs_swap(sr_))

#define NANO_CDR_Primitive1_serialize_cdr(t_,s_,sr_) \
    *((t_*)NANO_CDR_Stream_head(sr_)) = *(s_)

#define NANO_CDR_Primitive2_serialize_cdr(t_,s_,sr_) \
    NANO_CDR_set_swap_2(t_,*(s_),\
        NANO_CDR_Stream_head(sr_),\
        NANO_CDR_Stream_needs_swap(sr_))

#define NANO_CDR_Primitive4_serialize_cdr(t_,s_,sr_) \
    NANO_CDR_set_swap_4(t_,*(s_),\
        NANO_CDR_Stream_head(sr_),\
        NANO_CDR_Stream_needs_swap(sr_))

#define NANO_CDR_Primitive8_serialize_cdr(t_,s_,sr_) \
    NANO_CDR_set_swap_8(t_,*(s_),\
        NANO_CDR_Stream_head(sr_),\
        NANO_CDR_Stream_needs_swap(sr_))

/******************************************************************************
 *                          Implementation Macros
 ******************************************************************************/

#define NANO_CDR_STREAM_CAN_FIT(s_,t_,sz_) \
(\
    (NANO_CDR_Stream_buffer_len((s_))) - (sz_) >= \
    ((NANO_usize)NANO_OSAPI_Memory_align(NANO_CDR_Stream_head((s_)),t_)) - \
        ((NANO_usize)NANO_CDR_Stream_buffer((s_))) \
)

#define NANO_CDR_STREAM_DESERIALIZE_PRIMITIVE_IMPL(t_,p_) \
 \
NANO_RetCode \
xrce_str_concat(NANO_CDR_Stream_deserialize_,t_)(\
    NANO_CDR_Stream *const self,\
    xrce_str_concat(NANO_,t_) *const val)\
{\
    NANO_RetCode rc = NANO_RETCODE_ERROR;\
    NANO_LOG_FN_ENTRY \
    NANO_PCOND(self != NULL && NANO_CDR_Stream_has_buffer(self) && val != NULL)\
    if (!NANO_CDR_STREAM_CAN_FIT(self, xrce_str_concat(NANO_,t_), sizeof(xrce_str_concat(NANO_,t_))))\
    {\
        /* TODO nanolog */ \
        rc = NANO_RETCODE_OUT_OF_RESOURCES;\
        goto done;\
    }\
    NANO_CDR_STREAM_ALIGN(self, xrce_str_concat(NANO_,t_));\
    *val = xrce_str_concat(xrce_str_concat(NANO_CDR_Primitive, p_),_deserialize_cdr)(\
                xrce_str_concat(NANO_,t_), val, self);\
    NANO_CHECK_RC(\
        NANO_CDR_Stream_advance(self, sizeof(xrce_str_concat(NANO_,t_))),\
        /* TODO nanolog */);\
    rc = NANO_RETCODE_OK;\
done:\
    NANO_LOG_FN_EXIT_RC(rc)\
    return rc;\
}

#define NANO_CDR_STREAM_SERIALIZE_PRIMITIVE_IMPL(t_,p_) \
 \
NANO_RetCode \
xrce_str_concat(NANO_CDR_Stream_serialize_,t_)(\
    NANO_CDR_Stream *const self,\
    const xrce_str_concat(NANO_,t_) *const val)\
{\
    NANO_RetCode rc = NANO_RETCODE_ERROR;\
    NANO_LOG_FN_ENTRY \
    NANO_PCOND(self != NULL && NANO_CDR_Stream_has_buffer(self) && val != NULL)\
    if (!NANO_CDR_STREAM_CAN_FIT(self,xrce_str_concat(NANO_,t_),sizeof(xrce_str_concat(NANO_,t_))))\
    {\
        /* TODO nanolog */\
        rc = NANO_RETCODE_OUT_OF_RESOURCES;\
        goto done;\
    }\
    NANO_CDR_STREAM_ALIGN(self, xrce_str_concat(NANO_,t_));\
    xrce_str_concat(xrce_str_concat(NANO_CDR_Primitive,p_),_serialize_cdr)(xrce_str_concat(NANO_,t_), val, self);\
    NANO_CHECK_RC(\
        NANO_CDR_Stream_advance(self,sizeof(xrce_str_concat(NANO_,t_))),\
        /* TODO nanolog */);\
    rc = NANO_RETCODE_OK;\
done:\
    NANO_LOG_FN_EXIT_RC(rc)\
    return rc;\
}

/******************************************************************************
 *                          Function Implementations
 ******************************************************************************/

NANO_RetCode
NANO_CDR_Stream_initialize(
    NANO_CDR_Stream *const self,
    NANO_u8 *const buffer,
    const NANO_usize buffer_len,
    const NANO_CDR_Endianness endianness,
    const NANO_bool owned)
{
    NANO_PCOND(self != NULL && buffer != NULL && buffer_len > 0)

    self->buffer = buffer;
    self->head = buffer;
    self->buffer_len = buffer_len;
    self->flags |= 
        (endianness == NANO_CDR_ENDIANNESS_LITTLE)? 
            NANO_CDR_STREAMFLAGS_ENDIANNESS : 0;
    self->flags |= (owned)? NANO_CDR_STREAMFLAGS_OWNED : 0;
    self->cur_alignment = NANO_u8_alignment;
    
    return NANO_RETCODE_OK;
}

#if NANO_FEAT_AGENT || 1
NANO_CDR_STREAM_DESERIALIZE_PRIMITIVE_IMPL(u8,1)
NANO_CDR_STREAM_SERIALIZE_PRIMITIVE_IMPL(u8,1)
#endif /* NANO_FEAT_AGENT */

NANO_CDR_STREAM_DESERIALIZE_PRIMITIVE_IMPL(u16,2)
NANO_CDR_STREAM_DESERIALIZE_PRIMITIVE_IMPL(u32,4)
NANO_CDR_STREAM_DESERIALIZE_PRIMITIVE_IMPL(u64,8)

NANO_CDR_STREAM_SERIALIZE_PRIMITIVE_IMPL(u16,2)
NANO_CDR_STREAM_SERIALIZE_PRIMITIVE_IMPL(u32,4)
NANO_CDR_STREAM_SERIALIZE_PRIMITIVE_IMPL(u64,8)

NANO_RetCode
NANO_CDR_Stream_deserialize_octets(
    NANO_CDR_Stream *const self,
    NANO_u8 *const val,
    const NANO_usize val_len)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;

    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL &&
               NANO_CDR_Stream_has_buffer(self) &&
               val != NULL &&
               val_len > 0)
    
    if (!NANO_CDR_STREAM_CAN_FIT(self,NANO_u8,val_len))
    {
        /* TODO nanolog */
        rc = NANO_RETCODE_OUT_OF_RESOURCES;
        goto done;
    }
    
    NANO_CDR_STREAM_ALIGN(self,NANO_u8);
    
    NANO_OSAPI_Memory_copy(val, NANO_CDR_Stream_head(self), val_len);
    
    NANO_CHECK_RC(NANO_CDR_Stream_advance(self, val_len),
        NANO_LOG_ERROR("FAILED to advance stream",
            NANO_LOG_USIZE("move_size",val_len)
            NANO_LOG_USIZE("stream_capacity", NANO_CDR_Stream_capacity(self))
            NANO_LOG_USIZE("stream_buffer_len", self->buffer_len)
            NANO_LOG_USIZE("stream_offset", NANO_CDR_Stream_offset(self))
            NANO_LOG_BOOL("stream_full",
                (self->buffer_len - val_len > NANO_CDR_Stream_offset(self)))));
    
    rc = NANO_RETCODE_OK;
    
done:
    return rc;
}

NANO_RetCode
NANO_CDR_Stream_serialize_octets(
    NANO_CDR_Stream *const self,
    const NANO_u8 *const val,
    const NANO_usize val_len)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;

    NANO_PCOND(self != NULL &&
               NANO_CDR_Stream_has_buffer(self) &&
               val != NULL &&
               val_len > 0)
    
    if (!NANO_CDR_STREAM_CAN_FIT(self,NANO_u8,val_len))
    {
        /* TODO nanolog */
        rc = NANO_RETCODE_OUT_OF_RESOURCES;
        goto done;
    }
    
    NANO_CDR_STREAM_ALIGN(self,NANO_u8);
    
    NANO_OSAPI_Memory_copy(NANO_CDR_Stream_head(self), val, val_len);

    NANO_CHECK_RC(NANO_CDR_Stream_advance(self, val_len),
        NANO_LOG_ERROR_MSG("FAILED to advance stream"));

    
    rc = NANO_RETCODE_OK;
    
done:
    return rc;
}

#if NANO_FEAT_STRING
#if 0
NANO_RetCode
NANO_CDR_Stream_deserialize_conststring(
    NANO_CDR_Stream *const self,
    const char **const str,
    const NANO_usize str_max)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_u32 str_len = 0;

    NANO_PCOND(self != NULL &&
               NANO_CDR_Stream_has_buffer(self) &&
               str != NULL &&
               *str != NULL)

    /* Deserialize string length */
    NANO_CHECK_RC(
        NANO_u32_deserialize_cdr(&str_len, self),
        /* TODO nanolog */);
    
    if (str_len > NANO_USIZE_MAX)
    {
        /* TODO nanolog */
        rc = NANO_RETCODE_OVERFLOW_DETECTED;
        goto done;
    }

    /* Check some conditions for safe deserialization:
     *
     * - The length must always include the NUL terminator, 
     *   so 0 is an invalid length.
     * - the length should be less or equal to the maximum length allowed by
     *   the type (if not unbounded).
     * - we must have enough bytes in the stream to deserialied the specified
     *   length. 
     */
    if (str_len == 0 || 
        (str_max != NANO_STRING_LENGTH_UNBOUNDED && 
            (str_len - 1) > str_max) ||
        !NANO_CDR_STREAM_CAN_FIT(self,NANO_i8,str_len))
    {
        /* TODO nanolog */
        rc = NANO_RETCODE_INVALID_ARGS;
        goto done;
    }

    NANO_CDR_STREAM_ALIGN(self, NANO_i8);
    *str = (const char *) NANO_CDR_Stream_head(self);

    /* Check that the string is properly terminated */
    if ((*str)[str_len - 1] != '\0')
    {
        /* TODO nanolog */
        rc = NANO_RETCODE_INVALID_ARGS;
        goto done;
    }

    NANO_CHECK_RC(
        NANO_CDR_Stream_advance(self, (NANO_usize)str_len),
        /* TODO nanolog */);
    
    rc = NANO_RETCODE_OK;
    
done:
    return rc;
}
#endif

NANO_RetCode
NANO_CDR_Stream_deserialize_string(
    NANO_CDR_Stream *const self,
    char *const *const str,
    const NANO_usize str_max,
    NANO_usize *const str_len_out)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_u32 str_len = 0;
    NANO_CDR_Stream sself = NANO_CDR_STREAM_INITIALIZER;

    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL &&
               NANO_CDR_Stream_has_buffer(self) &&
               (str == NULL || *str != NULL))

    sself = *self;

    /* Deserialize string length */
    NANO_CHECK_RC(
        NANO_u32_deserialize_cdr(&str_len, self),
        /* TODO nanolog */);
    
    /* Check some conditions for safe deserialization:
     *
     * - The length must always include the NUL terminator, 
     *   so 0 is an invalid length.
     * - the length should be less or equal to the maximum length allowed by
     *   the type (if not unbounded).
     * - we must have enough bytes in the stream to deserialied the specified
     *   length. 
     */
    if (str_len == 0 || 
        (str_max != NANO_STRING_LENGTH_UNBOUNDED && 
            (str_len - 1) > str_max) ||
        !NANO_CDR_STREAM_CAN_FIT(self,NANO_i8,str_len))
    {
        /* TODO nanolog */
        rc = NANO_RETCODE_INVALID_ARGS;
        goto done;
    }

    NANO_CDR_STREAM_ALIGN(self, NANO_i8);

    /* Check that the string is properly terminated */
    if (NANO_CDR_Stream_head(self)[str_len - 1] != '\0')
    {
        /* TODO nanolog */
        rc = NANO_RETCODE_INVALID_ARGS;
        goto done;
    }

    if (str != NULL)
    {
        NANO_LOG_TRACE("DESERIALIZE octets to string",
            NANO_LOG_BYTES("buffer",NANO_CDR_Stream_head(self),str_len))

        NANO_OSAPI_Memory_copy(*str,NANO_CDR_Stream_head(self),str_len);
        NANO_CHECK_RC(
            NANO_CDR_Stream_advance(self, (NANO_usize)str_len),
            /* TODO nanolog */);
        
        NANO_LOG_DEBUG("DESERIALIZED string",
            NANO_LOG_STR("value",*str))
    }

    *str_len_out = str_len - 1;

    rc = NANO_RETCODE_OK;
    
done:
    if (str == NULL)
    {
        /* restore stream state */
        *self = sself;
    }
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

NANO_RetCode
NANO_CDR_Stream_deserialize_string_alloc(
    NANO_CDR_Stream *const self,
    char **const str,
    const NANO_usize str_max)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_usize str_len = 0;
    char *str_buf = NULL;

    NANO_LOG_FN_ENTRY
    
    NANO_PCOND(self != NULL)
    NANO_PCOND(str != NULL)
    
    str_buf = *str;

#if NANO_FEAT_AGENT
    if (str_max != NANO_STRING_LENGTH_UNBOUNDED)
    {
        if (str_buf == NULL)
        {
            RTIOsapiHeap_allocateString(&str_buf, str_max);
            if (str_buf == NULL)
            {
                goto done;
            }
        }
    }
    else
    {
        NANO_bool has_buf = (str_buf == NULL);
        NANO_usize str_len_max = 0;

        str_len_max = (str_buf != NULL)? NANO_OSAPI_String_length(str_buf) : 0;

        NANO_CHECK_RC(
            NANO_CDR_Stream_deserialize_string(self, NULL, str_max, &str_len),
            NANO_LOG_ERROR_MSG("FAILED to deserialize string length"));

        if (!has_buf || str_len > str_len_max)
        {
            if (has_buf)
            {
                RTIOsapiHeap_freeString(str_buf);
            }
            RTIOsapiHeap_allocateString(&str_buf, str_len);
        }
    }


#else
    if (str_max == NANO_STRING_LENGTH_UNBOUNDED)
    {
        NANO_LOG_ERROR_MSG("UNBOUNDED strings not supported")
        goto done;
    }
    str_len = str_max;
    
    if (str_buf == NULL)
    {
        NANO_LOG_ERROR_MSG("NO BUFFER to deserialize string")
        goto done;
    }
#endif /* NANO_FEAT_AGENT */
    
    NANO_CHECK_RC(
        NANO_CDR_Stream_deserialize_string(
            self, &str_buf, str_max, &str_len),
        NANO_LOG_ERROR_MSG("FAILED to deserialize string"));

    *str = str_buf;

    NANO_LOG_TRACE("DEBUG deserialized string",
        NANO_LOG_STR("str",*str))

    
    rc = NANO_RETCODE_OK;
    
done:
    
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

NANO_RetCode
NANO_CDR_Stream_serialize_string(
    NANO_CDR_Stream *const self,
    const char *const *const str,
    const NANO_usize str_max)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_usize str_len = 0;
    NANO_u32 str_len_u = 0;

    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL &&
               NANO_CDR_Stream_has_buffer(self) &&
               str != NULL &&
               *str != NULL)

    /* Serialized string must include the NUL terminator */
    str_len = NANO_OSAPI_String_length(*str) + 1;

    if (str_len > NANO_U32_MAX)
    {
        /* TODO nanolog */
        rc = NANO_RETCODE_OVERFLOW_DETECTED;
        goto done;
    }
    if (str_max != NANO_STRING_LENGTH_UNBOUNDED && str_len > str_max)
    {
        rc = NANO_RETCODE_INVALID_ARGS;
        goto done;
    }

    str_len_u = (NANO_u32)str_len;

    /* check that string is well terminated */
    if ((*str)[str_len - 1] != '\0')
    {
        /* TODO nanolog */
        rc = NANO_RETCODE_INVALID_ARGS;
        goto done;
    }
    
    NANO_CHECK_RC(
        NANO_u32_serialize_cdr(&str_len_u, self),
        /* TODO nanolog */);
    NANO_CHECK_RC(
        NANO_Octets_serialize_cdr(*str, self, str_len),
        /* TODO nanolog */)
    
    rc = NANO_RETCODE_OK;
    
done:
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

#if NANO_FEAT_AGENT
NANO_usize
NANO_CDR_Stream_serialized_size_string(
    const NANO_CDR_Stream *const self,
    const char *const *const str,
    const NANO_usize str_max)
{
    NANO_usize res = 0;
    NANO_usize str_len = 0;
    NANO_usize str_max_v = (str_max == NANO_STRING_LENGTH_UNBOUNDED)?
                                NANO_MEM_UNBOUNDEDSTRING_MAX_LENGTH : str_max;
    
    NANO_LOG_FN_ENTRY

    UNUSED_ARG(self);

    NANO_PCOND(self != NULL &&
               NANO_CDR_Stream_has_buffer(self))

    res += sizeof(NANO_u32);

    if (str == NULL)
    {
        NANO_PCOND(NANO_USIZE_MAX - str_max_v >= res)
        /* max serialized _size */
        res += str_max_v;
    }
    else
    {
        str_len = NANO_OSAPI_String_length(*str);
        if (str_len > str_max_v)
        {
            NANO_PCOND(NANO_USIZE_MAX - str_max_v >= res)
            res += str_max_v;
        }
        else
        {
            NANO_PCOND(NANO_USIZE_MAX - str_len >= res)
            res += str_len;
        }
    }
    res += 1; /* NUL terminator */

    return res;
}
#endif

#endif /* NANO_FEAT_STRING */

#if NANO_FEAT_SEQUENCE

#if NANO_FEAT_AGENT
RTI_PRIVATE
NANO_RetCode
NANO_CDR_Stream_allocate_sequence(
    NANO_CDR_Stream *const self,
    NANO_Sequence *const seq,
    const NANO_usize seq_len)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_usize seq_max_cur = 0,
               el_size = 0;
    NANO_u8 *seq_buffer = NULL;
    char *rti_ptr = NULL;

    NANO_LOG_FN_ENTRY

    UNUSED_ARG(self);
    
    NANO_PCOND(self != NULL)
    
    seq_max_cur = NANO_Sequence_maximum(seq);
    seq_buffer = NANO_Sequence_contiguous_buffer_mut(seq);

    if (seq_buffer != NULL &&
        seq_max_cur < seq_len)
    {
        RTIOsapiHeap_freeBuffer(seq_buffer);
        seq_buffer = NULL;
    }
    if (seq_buffer == NULL)
    {
        el_size = NANO_Sequence_element_size(seq);
        RTIOsapiHeap_allocateBufferAligned(
            &rti_ptr,
            seq_len * el_size,
            RTI_OSAPI_ALIGNMENT_DEFAULT);
        if (rti_ptr == NULL)
        {
            goto done;
        }
        seq_buffer = (NANO_u8*)rti_ptr;
        NANO_Sequence_set_contiguous_buffer(
            seq, seq_buffer, seq_len, seq_len);
    }
    
    rc = NANO_RETCODE_OK;
    
done:
    
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}
NANO_RetCode
NANO_CDR_Stream_deserialize_psequence(
    NANO_CDR_Stream *const self,
    NANO_Sequence *const seq,
    const NANO_usize seq_max)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_usize seq_len = 0,
               el_size = 0;
    NANO_u32 seq_len_u = 0;

    NANO_LOG_FN_ENTRY
    
    NANO_PCOND(self != NULL &&
               NANO_CDR_Stream_has_buffer(self) &&
               seq != NULL &&
               NANO_Sequence_initialized(seq))
    
    /* Deserialize the length (in number of elements) of the sequence */
    NANO_CHECK_RC(
        NANO_u32_deserialize_cdr(&seq_len_u, self),
        /* TODO nanolog */);

    seq_len = (NANO_usize)seq_len_u;

    if (seq_max != NANO_SEQUENCE_LENGTH_UNBOUNDED &&
        seq_len > seq_max)
    {
        /* TODO nanolog */
        rc = NANO_RETCODE_INVALID_ARGS;
        goto done;
    }

    el_size = NANO_Sequence_element_size(seq);

    /* Check if seq_len is too big */
    if (NANO_USIZE_MAX / el_size < seq_len)
    {
        /* TODO nanolog */
        rc = NANO_RETCODE_INVALID_ARGS;
        goto done;
    }

#if NANO_FEAT_AGENT
    if (NANO_CDR_Stream_allow_alloc(self))
    {
        if (NANO_CDR_Stream_allocate_sequence(self, seq, seq_len))
        {
            goto done;
        }
    }
#endif /* NANO_FEAT_AGENT */

    /* Make sure that the sequence has a valid max value for its type */
    // NANO_PCOND(NANO_Sequence_maximum(seq) <= seq_max_a)

    /* Set the sequence to the deserialized length (this already 
     * checks if the length exceeds the sequence's maximum) */
    NANO_CHECK_RC(
        NANO_Sequence_set_length(seq, seq_len),
        /* TODO nanolog */);
    
    /* Deserialize sequence's buffer if present */
    if (seq_len > 0)
    {
        /* If the stream is not in native endianness, we need to swap each
         * element one by one */
        if (NANO_CDR_Stream_needs_swap(self) && el_size > sizeof(NANO_u8))
        {
            NANO_usize i = 0;
            for (i = 0; i < seq_len; i++)
            {
                /* We select what to deserialize based on the element size
                 * of the sequence */
                switch (el_size)
                {
                case sizeof(NANO_u16):
                {
                    NANO_CHECK_RC(
                        NANO_u16_deserialize_cdr(
                            (NANO_u16*) NANO_Sequence_reference_mut(seq, i),
                            self),
                        /* TODO nanolog */);
                    break;
                }
                case sizeof(NANO_u32):
                {
                    NANO_CHECK_RC(
                        NANO_u32_deserialize_cdr(
                            (NANO_u32*) NANO_Sequence_reference_mut(seq, i),
                            self),
                        /* TODO nanolog */);
                    break;
                }
                case sizeof(NANO_u64):
                {
                    NANO_CHECK_RC(
                        NANO_u64_deserialize_cdr(
                            (NANO_u64*) NANO_Sequence_reference_mut(seq, i),
                            self),
                        /* TODO nanolog */);
                    break;
                }
                default:
                {
                    /* TODO nanolog */
                    goto done;
                }
                }
            }
            
        }
        else
        {
            const NANO_usize seq_size = seq_len * el_size;
            NANO_u8 *const seq_buffer =
                NANO_Sequence_contiguous_buffer_mut(seq);

            NANO_CHECK_RC(
                NANO_Octets_deserialize_cdr(seq_buffer, self, seq_size),
                /* TODO nanolog */);
        }
    }
    
    rc = NANO_RETCODE_OK;
    
done:
    return rc;
}


NANO_RetCode
NANO_CDR_Stream_serialize_psequence(
    NANO_CDR_Stream *const self,
    const NANO_Sequence *const seq,
    const NANO_usize seq_max)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_u32 seq_len_u = 0;
    NANO_usize seq_len = 0;
    
    NANO_LOG_FN_ENTRY

    UNUSED_ARG(seq_max);
    
    NANO_PCOND(self != NULL &&
               NANO_CDR_Stream_has_buffer(self) &&
               seq != NULL &&
               NANO_Sequence_initialized(seq))
    
    seq_len = NANO_Sequence_length(seq);

    /* Serialize sequence length as a u32.
     * Check for the (unlikely) case of an overflow */
    NANO_PCOND(seq_len <= NANO_U32_MAX &&
               seq_len <= seq_max_a)

    seq_len_u = (NANO_u32)seq_len;
    
    NANO_CHECK_RC(
        NANO_u32_serialize_cdr(&seq_len_u, self),
        /* TODO nanolog */);
    
    if (seq_len_u > 0)
    {
        const NANO_usize el_size = NANO_Sequence_element_size(seq);

        if (NANO_CDR_Stream_needs_swap(self) && el_size > sizeof(NANO_u8))
        {
            NANO_usize i = 0;

            for (i = 0; i < seq_len; i++)
            {
                /* We select what to serialize based on the element size
                 * of the sequence */
                switch (el_size)
                {
                case sizeof(NANO_u16):
                {
                    NANO_CHECK_RC(
                        NANO_u16_serialize_cdr(
                            (const NANO_u16*) NANO_Sequence_reference(seq, i),
                            self),
                        /* TODO nanolog */);
                    break;
                }
                case sizeof(NANO_u32):
                {
                    NANO_CHECK_RC(
                        NANO_u32_serialize_cdr(
                            (const NANO_u32*) NANO_Sequence_reference(seq, i),
                            self),
                        /* TODO nanolog */);
                    break;
                }
                case sizeof(NANO_u64):
                {
                    NANO_CHECK_RC(
                        NANO_u64_serialize_cdr(
                            (const NANO_u64*) NANO_Sequence_reference(seq, i),
                            self),
                        /* TODO nanolog */);
                    break;
                }
                default:
                {
                    /* TODO nanolog */
                    goto done;
                }
                }
            }
        }
        else
        {
            const NANO_usize seq_size = el_size * seq_len_u;
            const NANO_u8 *seq_buffer = NANO_Sequence_contiguous_buffer(seq);

            NANO_PCOND(
               NANO_USIZE_MAX / NANO_Sequence_element_size(seq) >= seq_len)

            NANO_CHECK_RC(
                NANO_Octets_serialize_cdr(seq_buffer, self, seq_size),
                /* TODO nanolog */);
        }
    }

    rc = NANO_RETCODE_OK;
    
done:
    return rc;
}


NANO_usize
NANO_CDR_Stream_serialized_size_psequence(
    const NANO_CDR_Stream *const self,
    const NANO_Sequence *const seq,
    const NANO_usize seq_max)
{
    NANO_usize res = 0,
               seq_max_a = (seq_max == NANO_SEQUENCE_LENGTH_UNBOUNDED)?
                                NANO_MEM_UNBOUNDEDSEQUENCE_MAX_LENGTH : seq_max,
               seq_max_el = 0;
    
    UNUSED_ARG(self);

    NANO_PCOND(self != NULL && seq != NULL)

    res += sizeof(NANO_u32);
    
    seq_max_el = NANO_Sequence_maximum(seq);

    if (seq_max_el > seq_max_a)
    {
        NANO_PCOND((NANO_USIZE_MAX - res) / el_size > seq_max_a)
        res += seq_max_a * NANO_Sequence_element_size(seq);
    }
    else
    {
        NANO_PCOND((NANO_USIZE_MAX - res) / el_size > seq_max_el)
        res += seq_max_el * NANO_Sequence_element_size(seq);
    }

    return res;
}
#endif


NANO_RetCode
NANO_CDR_Stream_deserialize_sequence(
    NANO_CDR_Stream *const self,
    NANO_Sequence *const seq,
    const NANO_usize seq_max,
    NANO_CDR_Stream_DeserializeCdrFn deserialize_member)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_u32 seq_len_u = 0,
             seq_size = 0;
    NANO_usize seq_len = 0,
               deserialized_tot = 0;
    
    NANO_LOG_FN_ENTRY

    UNUSED_ARG(seq_max);
    
    NANO_PCOND(self != NULL &&
               NANO_CDR_Stream_has_buffer(self) &&
               seq != NULL &&
               NANO_Sequence_initialized(seq) &&
               deserialize_member != NULL)
    
    /* Deserialize the total serialized size of the sequence */
    NANO_CHECK_RC(
        NANO_u32_deserialize_cdr(&seq_size, self),
        /* TODO nanolog */);

    /* Deserialize the length (in number of elements) of the sequence */
    NANO_CHECK_RC(
        NANO_u32_deserialize_cdr(&seq_len_u, self),
        /* TODO nanolog */);
    
    deserialized_tot += sizeof(NANO_u32);

    seq_len = (NANO_usize)seq_len_u;

#if NANO_FEAT_AGENT
    if (NANO_CDR_Stream_allow_alloc(self))
    {
        if (NANO_CDR_Stream_allocate_sequence(self, seq, seq_len))
        {
            goto done;
        }
    }
#endif /* NANO_FEAT_AGENT */

    /* Set the sequence to the deserialized length (this already 
     * checks if the length exceeds the sequence's maximum) */
    NANO_CHECK_RC(
        NANO_Sequence_set_length(seq, seq_len),
        /* TODO nanolog */);
    
    if (seq_len > 0)
    {
        NANO_usize i = 0;

        for (i = 0; i < seq_len; i++)
        {
            const NANO_u8 *des_start = NANO_CDR_Stream_head(self);
            NANO_u8 *mem_ref = NANO_Sequence_reference_mut(seq, i);

            NANO_CHECK_RC(
                deserialize_member(mem_ref, self),
                /* TODO nanolog */);
        
            deserialized_tot += 
                (NANO_usize) (NANO_CDR_Stream_head(self) - des_start);
        }
    }

    /* Check if the total serialized size corresponds to what we
     * actually deserialized  */
    if (seq_size != deserialized_tot)
    {
        /* TODO nanolog */
        rc = NANO_RETCODE_INVALID_ARGS;
        goto done;
    }

    rc = NANO_RETCODE_OK;

done:
    return rc;
}


NANO_RetCode
NANO_CDR_Stream_serialize_sequence(
    NANO_CDR_Stream *const self,
    const NANO_Sequence *const seq,
    const NANO_usize seq_max,
    NANO_CDR_Stream_SerializeCdrFn serialize_member)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_usize seq_len = 0,
               tot_serialized = 0;
    NANO_u32 seq_len_u = 0;
    NANO_u32 *seq_size_ptr = 0;

    NANO_LOG_FN_ENTRY

    UNUSED_ARG(seq_max);
    
    NANO_PCOND(self != NULL &&
               NANO_CDR_Stream_has_buffer(self) &&
               seq != NULL &&
               NANO_Sequence_initialized(seq) &&
               serialize_member != NULL)
    
    seq_len = NANO_Sequence_length(seq);
    NANO_PCOND(seq_len <= NANO_U32_MAX &&
               seq_len <= ((seq_max == NANO_SEQUENCE_LENGTH_UNBOUNDED)?
                                NANO_MEM_UNBOUNDEDSEQUENCE_MAX_LENGTH : seq_max));
    seq_len_u = (NANO_u32)seq_len;

    if (!NANO_CDR_STREAM_CAN_FIT(self, NANO_u32, sizeof(NANO_u32)))
    {
        /* TODO nanolog */
        rc = NANO_RETCODE_OUT_OF_RESOURCES;
        goto done;
    }

    /* Align stream to serialize total size of the sequence. We only
     * retrieve a pointer to the location in the stream for now, and we
     * will set it once we have serialized all elements. */
    NANO_CDR_STREAM_ALIGN(self, NANO_u32);
    seq_size_ptr = (NANO_u32*) NANO_CDR_Stream_head(self);
    NANO_CDR_Stream_advance(self, sizeof(NANO_u32));

    NANO_CHECK_RC(
        NANO_u32_serialize_cdr(&seq_len_u,self),
        /* TODO nanolog */);
    tot_serialized += sizeof(NANO_u32);

    if (seq_len > 0)
    {
        NANO_usize i = 0;

        for (i = 0; i < seq_len; i++)
        {
            const NANO_u8 *mem_ref = NANO_Sequence_reference(seq, i),
                          *ser_start = NANO_CDR_Stream_head(self);

            NANO_CHECK_RC(
                serialize_member(mem_ref, self),
                /* TODO nanolog */);
            
            tot_serialized += 
                (NANO_usize)(NANO_CDR_Stream_head(self) - ser_start);
        }
        
    }

    NANO_PCOND(tot_serialized <= NANO_U32_MAX)

    *seq_size_ptr = (NANO_u32)tot_serialized;

    rc = NANO_RETCODE_OK;
    
done:
    return rc;
}


#if 0
NANO_usize
NANO_CDR_Stream_serialized_size_sequence(
    const NANO_CDR_Stream *const self,
    const NANO_Sequence *const seq,
    const NANO_usize seq_max,
    NANO_CDR_Stream_SerializedSizeCdrFn serialized_size_member)
{
    NANO_usize res = 0,
               seq_len = 0,
               i = 0,
               seq_max_a = (seq_max == NANO_SEQUENCE_LENGTH_UNBOUNDED)?
                        NANO_MEM_UNBOUNDEDSEQUENCE_MAX_LENGTH : seq_max;

    NANO_PCOND(seq != NULL &&
               serialized_size_member != NULL)

    res += sizeof(NANO_u32);

    seq_len = NANO_Sequence_length(seq);
    NANO_PCOND(seq_len <= seq_max_a)

    for (i = 0; i < seq_len; i++)
    {
        const NANO_u8 *mem_ref = NANO_Sequence_reference(seq, i);
        NANO_usize mem_size = serialized_size_member(mem_ref, self);
        NANO_PCOND(NANO_USIZE_MAX - mem_size <= res);
        res += mem_size;
    }
    
    return res;
}
#endif

#endif /* NANO_FEAT_SEQUENCE */

#if 0

NANO_RetCode
NANO_CDR_Stream_substream(
    const NANO_CDR_Stream *const self,
    const NANO_usize substream_size,
    const NANO_CDR_Endianness substream_endianness,
    NANO_CDR_Stream *const substream_out)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_PCOND(self != NULL && substream_size > 0 && substream_out != NULL)
    
    if (!NANO_CDR_STREAM_CAN_FIT(self, NANO_u8, substream_size))
    {
        /* TODO nanolog */
        rc = NANO_RETCODE_OUT_OF_RESOURCES;
        goto done;
    }

    substream_out->buffer = NANO_CDR_Stream_head(self);
    substream_out->head = NANO_CDR_Stream_head(self);
    substream_out->buffer_len = substream_size;
    substream_out->flags = self->flags;
    if (substream_endianness)
    {
        substream_out->flags |= NANO_CDR_STREAMFLAGS_ENDIANNESS;
    }
    else
    {
        substream_out->flags &= ~NANO_CDR_STREAMFLAGS_ENDIANNESS;
    }
    substream_out->cur_alignment = self->cur_alignment;
    
    rc = NANO_RETCODE_OK;
    
done:
    return rc;
}

void
NANO_CDR_Stream_sync_to_substream(
    NANO_CDR_Stream *const self,
    const NANO_CDR_Stream *const substream)
{
    NANO_PCOND(
        self != NULL && substream != NULL &&
        NANO_CDR_Stream_head(substream) <= self->buffer + self->buffer_len)
    
    self->head = NANO_CDR_Stream_head(substream);
    self->cur_alignment = substream->cur_alignment;
}
#endif

#endif /* NANO_FEAT_CDR */