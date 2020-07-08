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

#ifndef nano_core_cdr_basic_h
#define nano_core_cdr_basic_h

/******************************************************************************
 *          Serialization/Deserialization (CDR) Interface
 ******************************************************************************/
/**
 * 
 * 
 * @addtogroup nano_api_cdr_support
 * @{
 */

/*e
 * @brief TODO
 * 
 */
typedef NANO_bool NANO_CDR_Endianness;

/*e
 * @brief TODO
 * 
 */
#define NANO_CDR_ENDIANNESS_LITTLE         NANO_BOOL_TRUE

/*e
 * @brief TODO
 * 
 */
#define NANO_CDR_ENDIANNESS_BIG            NANO_BOOL_FALSE

/*e
 * @brief TODO
 * 
 * @param little_endian 
 * @return NANO_bool 
 */
NANO_bool
NANO_CDR_needs_swap(const NANO_bool little_endian);

/** @} *//* nano_api_cdr_support */

/******************************************************************************
 *                              EncodingVersion
 ******************************************************************************/

/**
 * @addtogroup nano_api_cdr_support
 * @{ 
 */

typedef NANO_u8 NANO_CDR_EncodingVersion;

#define NANO_CDR_ENCODINGVERSION_UNSPECIFIED    0x00
#define NANO_CDR_ENCODINGVERSION_1              0x01
#define NANO_CDR_ENCODINGVERSION_2              0x02

#define NANO_CDR_ENCODINGVERSION_1_MAX_ALIGNMENT        ((NANO_u8)8)
#define NANO_CDR_ENCODINGVERSION_2_MAX_ALIGNMENT        ((NANO_u8)4)

#define NANO_CDR_ENCODINGVERSION_VALUE_LEN              2

/** @} *//* nano_api_cdr_support */

/******************************************************************************
 *                              EncapsulationHeader
 ******************************************************************************/

/**
 * @addtogroup nano_api_cdr_support
 * @{
 */

#define NANO_CDR_ENCAPSULATIONHEADER_VALUE_LEN              2

typedef struct NANODllExport NANO_CDR_EncapsulationHeaderI
{
    
    NANO_u8 value[NANO_CDR_ENCAPSULATIONHEADER_VALUE_LEN];

} NANO_CDR_EncapsulationHeader;


#define NANO_CDR_ENCAPSULATIONHEADER_SERIALIZED_SIZE_MAX \
    sizeof(NANO_CDR_EncapsulationHeader)

#define NANO_CDR_ENCAPSULATIONHEADER_UNKNOWN        { { 0x00, 0x00 } }
#define NANO_CDR_ENCAPSULATIONHEADER_PLAIN_CDR1_BE  { { 0x00, 0x00 } }
#define NANO_CDR_ENCAPSULATIONHEADER_PLAIN_CDR1_LE  { { 0x00, 0x01 } }
#define NANO_CDR_ENCAPSULATIONHEADER_PLAIN_CDR2_BE  { { 0x00, 0x10 } }
#define NANO_CDR_ENCAPSULATIONHEADER_PLAIN_CDR2_LE  { { 0x00, 0x11 } }

NANO_CDR_Endianness
NANO_CDR_EncapsulationHeader_endianness(NANO_CDR_EncapsulationHeader self);

#define NANO_CDR_EncapsulationHeader_endianness(s_) \
    (((s_)->value[1] & 0x01)? \
        NANO_CDR_ENDIANNESS_LITTLE : NANO_CDR_ENDIANNESS_BIG)

/*i
 * @brief The only supported versions are PLAIN_CDR2_BE (0x0010) and 
 * PLAIN_CDR2_LE (0x0011).
 * 
 * @param self 
 * @return NANO_bool 
 */
NANO_bool
NANO_CDR_EncapsulationHeader_is_valid(NANO_CDR_EncapsulationHeader self);

#define NANO_CDR_EncapsulationHeader_is_valid(s_) \
    ((s_)->value[0] == 0x00 && ((s_)->value[1] & 0x10))

/** @} *//* nano_api_cdr_support */

/******************************************************************************
 *                              DataHeader
 ******************************************************************************/

/**
 * @addtogroup nano_api_cdr_support
 * @{
 */

typedef NANO_u32 NANO_CDR_DataHeader;

/** @} *//* nano_api_cdr_support */


#if NANO_ENDIAN_LITTLE
#define NANO_CDR_ENDIANNESS_NATIVE          NANO_CDR_ENDIANNESS_LITTLE
#define NANO_CDR_ENCODING_NATIVE            NANO_CDR_ENCAPSULATIONHEADER_PLAIN_CDR2_LE
#define NANO_CDR_MAX_ALIGNMENT_NATIVE       NANO_CDR_ENCODINGVERSION_2_MAX_ALIGNMENT
#define NANO_CDR_needs_swap(le_)            (!(le_))
#else /* NANO_ENDIAN_BIG */
#define NANO_CDR_ENDIANNESS_NATIVE          NANO_CDR_ENDIANNESS_BIG
#define NANO_CDR_ENCODING_NATIVE            NANO_CDR_ENCAPSULATIONHEADER_PLAIN_CDR2_BE
#define NANO_CDR_MAX_ALIGNMENT_NATIVE       NANO_CDR_ENCODINGVERSION_2_MAX_ALIGNMENT
#define NANO_CDR_needs_swap(le_)            (le_)
#endif /* NANO_ENDIAN_LITTLE */

#if NANO_FEAT_CDR
/******************************************************************************
 *                                  Stream
 ******************************************************************************/

/**
 * @addtogroup nano_api_cdr_stream
 * @{
 */

typedef NANO_u8 NANO_CDR_StreamFlags;

#define NANO_CDR_STREAMFLAGS_DEFAULT        (0)

#define NANO_CDR_STREAMFLAGS_ENDIANNESS     (0x01 << 0)
#define NANO_CDR_STREAMFLAGS_OWNED          (0x01 << 1)
#define NANO_CDR_STREAMFLAGS_ALLOC          (0x01 << 2)

/*e
 * 
 * 
 */
struct NANO_CDR_StreamI
{
    NANO_u8 *buffer;

    NANO_u8 *head;
    
    NANO_usize buffer_len;

    NANO_u8 cur_alignment;

    NANO_CDR_StreamFlags flags;
    
};

/*e
 * @brief 
 * 
 */
#define NANO_CDR_STREAM_INITIALIZER \
{\
    NULL, /* buffer */ \
    NULL, /* head */ \
    0, /* buffer_len */ \
    NANO_u8_alignment, /* cur_alignment */ \
    NANO_CDR_STREAMFLAGS_DEFAULT /* flags */ \
}

/*e
 * @brief 
 * 
 * @param self 
 * @return NANO_CDR_Endianness 
 */
NANO_CDR_Endianness
NANO_CDR_Stream_endianness(const NANO_CDR_Stream *const self);

#define NANO_CDR_Stream_endianness(s_) \
    (((s_)->flags & NANO_CDR_STREAMFLAGS_ENDIANNESS)?\
        NANO_CDR_ENDIANNESS_LITTLE : NANO_CDR_ENDIANNESS_BIG)


NANO_bool
NANO_CDR_Stream_owned(const NANO_CDR_Stream *const self);

#define NANO_CDR_Stream_owned(s_) \
    ((s_)->flags & NANO_CDR_STREAMFLAGS_OWNED)

NANO_bool
NANO_CDR_Stream_allow_alloc(const NANO_CDR_Stream *const self);

#define NANO_CDR_Stream_allow_alloc(s_) \
    ((s_)->flags & NANO_CDR_STREAMFLAGS_ALLOC)

/*e
 * @brief TODO
 * 
 * @param self 
 * @param buffer 
 * @param buffer_len 
 * @param endianness 
 * @param owned 
 * @return NANO_RetCode 
 */
NANODllExport
NANO_RetCode
NANO_CDR_Stream_initialize(
    NANO_CDR_Stream *const self,
    NANO_u8 *const buffer,
    const NANO_usize buffer_len,
    const NANO_CDR_Endianness endianness,
    const NANO_bool owned);

/*e
 * @brief 
 * 
 * @param self 
 * @param amount 
 * @return NANO_RetCode
 */
NANODllExport
NANO_RetCode
NANO_CDR_Stream_advance(
    NANO_CDR_Stream *const self,
    NANO_usize amount);

#define NANO_CDR_Stream_advance(s_,a_) \
    (((a_) > NANO_CDR_Stream_capacity((s_)))?\
        NANO_RETCODE_OUT_OF_RESOURCES : \
        ((s_)->head += (a_), NANO_RETCODE_OK))

/*e
 * @brief TODO
 * 
 * @param self 
 * @param amount 
 * @return NANO_RetCode
 */
NANODllExport
NANO_RetCode
NANO_CDR_Stream_rewind(
    NANO_CDR_Stream *const self,
    NANO_usize amount);

#define NANO_CDR_Stream_rewind(s_,a_) \
    (((a_) > NANO_CDR_Stream_offset((s_)))?\
        NANO_RETCODE_INVALID_ARGS : \
        ((s_)->head -= (a_), NANO_RETCODE_OK))

/*e
 * @brief 
 * 
 * @param self 
 */
void
NANO_CDR_Stream_reset(
    NANO_CDR_Stream *const self);

#define NANO_CDR_Stream_reset(s_) \
{\
    (s_)->head = (s_)->buffer;\
    (s_)->cur_alignment = NANO_u8_alignment;\
}

/*e
 * @brief 
 * 
 */
#define NANO_CDR_STREAM_ALIGN(s_,t_) \
{\
    if ((s_)->cur_alignment < xrce_str_concat(t_, _alignment))\
    {\
        (s_)->head = NANO_OSAPI_Memory_align((s_)->head, t_);\
    }\
    (s_)->cur_alignment = xrce_str_concat(t_, _alignment);\
}

/*e
 * @brief 
 * 
 * @param self 
 * @return NANO_u8*
 */
NANO_u8 *
NANO_CDR_Stream_head(
    NANO_CDR_Stream *const self);

#define NANO_CDR_Stream_head(s_) \
        ((s_)->head)

/*e
 * @brief 
 * 
 * @param self 
 * @return NANO_u8*
 */
NANO_u8 *
NANO_CDR_Stream_buffer(
    NANO_CDR_Stream *const self);

#define NANO_CDR_Stream_buffer(s_) \
        ((s_)->buffer)

/*e
 * @brief 
 * 
 */
NANO_usize
NANO_CDR_Stream_buffer_len(
    const NANO_CDR_Stream *const self);

#define NANO_CDR_Stream_buffer_len(s_) \
        ((s_)->buffer_len)

/*e
 * @brief 
 * 
 * @param self 
 * @return NANO_usize 
 */
NANO_usize
NANO_CDR_Stream_offset(
    const NANO_CDR_Stream *const self);

#define NANO_CDR_Stream_offset(s_) \
    ((NANO_usize)\
        (NANO_CDR_Stream_head((s_)) - NANO_CDR_Stream_buffer((s_))))

/*e
 * @brief 
 * 
 * @param self 
 * @return NANO_usize 
 */
NANO_usize
NANO_CDR_Stream_capacity(
    const NANO_CDR_Stream *const self);

#define NANO_CDR_Stream_capacity(s_) \
    (NANO_CDR_Stream_buffer_len((s_)) - NANO_CDR_Stream_offset((s_)))

#if NANO_ENDIAN_LITTLE

#define NANO_CDR_Stream_needs_swap(sr_) \
    (!NANO_CDR_Stream_endianness(sr_))

#else /* NANO_ENDIAN_BIG */

#define NANO_CDR_Stream_needs_swap(sr_) \
    (NANO_CDR_Stream_endianness(sr_))

#endif /* NANO_ENDIAN_LITTLE */

NANO_bool
NANO_CDR_Stream_has_buffer(const NANO_CDR_Stream *self);

#define NANO_CDR_Stream_has_buffer(s_) \
        ((s_)->buffer != NULL)

#if 0
NANODllExport
NANO_RetCode
NANO_CDR_Stream_substream(
    const NANO_CDR_Stream *const self,
    const NANO_usize substream_size,
    const NANO_CDR_Endianness substream_endianness,
    NANO_CDR_Stream *const substream_out);

NANODllExport
void
NANO_CDR_Stream_sync_to_substream(
    NANO_CDR_Stream *const self,
    const NANO_CDR_Stream *const substream);
#endif

/*e
 * @brief 
 * 
 */
typedef NANO_RetCode
    (*NANO_CDR_Stream_DeserializeCdrFn)(
        void *const self,
        NANO_CDR_Stream *const stream);

/*e
 * @brief 
 * 
 */
typedef NANO_RetCode
    (*NANO_CDR_Stream_SerializeCdrFn)(
        const void *const self,
        NANO_CDR_Stream *const stream);

/*e
 * @brief 
 * 
 */
typedef NANO_usize
    (*NANO_CDR_Stream_SerializedSizeCdrFn)(
        const void *const self,
        const NANO_CDR_Stream *const stream);

/** @} *//* nano_api_data_stream */

#define NANO_CDR_STREAM_INTF_DECL(t_) \
    NANODllExport\
    NANO_RetCode\
    xrce_str_concat(t_,_deserialize_cdr)(\
        t_ *const self,\
        NANO_CDR_Stream *const stream);\
    \
    NANODllExport\
    NANO_RetCode\
    xrce_str_concat(t_,_serialize_cdr)(\
        const t_ *const self,\
        NANO_CDR_Stream *const stream);\
    \
    NANO_usize\
    xrce_str_concat(t_,_serialized_size_cdr)(\
        const t_ *const self,\
        const NANO_CDR_Stream *const stream);\

/**
 * @addtogroup nano_api_cdr_stream
 * @{
 */

/*e
 * @brief 
 * 
 * @param self 
 * @param val 
 * @return 
 */
NANODllExport
NANO_RetCode
NANO_CDR_Stream_deserialize_u8(
    NANO_CDR_Stream *const self,
    NANO_u8 *const val);

/*e
 * @brief 
 * 
 * @param self 
 * @param val 
 * @return 
 */
NANODllExport
NANO_RetCode
NANO_CDR_Stream_deserialize_u16(
    NANO_CDR_Stream *const self,
    NANO_u16 *const val);

/*e
 * @brief 
 * 
 * @param self 
 * @param val 
 * @return 
 */
NANODllExport
NANO_RetCode
NANO_CDR_Stream_deserialize_u32(
    NANO_CDR_Stream *const self,
    NANO_u32 *const val);

/*e
 * @brief 
 * 
 * @param self 
 * @param val 
 * @return 
 */
NANODllExport
NANO_RetCode
NANO_CDR_Stream_deserialize_u64(
    NANO_CDR_Stream *const self,
    NANO_u64 *const val);

/*e
 * @brief 
 * 
 * @param self 
 * @param val 
 * @return 
 */
NANO_RetCode
NANO_CDR_Stream_deserialize_i8(
    NANO_CDR_Stream *const self,
    NANO_u8 *const val);

#define NANO_CDR_Stream_deserialize_i8(s_,v_) \
    NANO_CDR_Stream_deserialize_u8((s_),(NANO_u8*)(v_))

/*e
 * @brief 
 * 
 * @param self 
 * @param val 
 * @return 
 */
NANO_RetCode
NANO_CDR_Stream_deserialize_i16(
    NANO_CDR_Stream *const self,
    NANO_i16 *const val);

#define NANO_CDR_Stream_deserialize_i16(s_,v_) \
    NANO_CDR_Stream_deserialize_u16((s_),(NANO_u16*)(v_))

/*e
 * @brief 
 * 
 * @param self 
 * @param val 
 * @return 
 */
NANO_RetCode
NANO_CDR_Stream_deserialize_i32(
    NANO_CDR_Stream *const self,
    NANO_i32 *const val);

#define NANO_CDR_Stream_deserialize_i32(s_,v_) \
    NANO_CDR_Stream_deserialize_u32((s_),(NANO_u32*)(v_))

/*e
 * @brief 
 * 
 * @param self 
 * @param val 
 * @return 
 */
NANO_RetCode
NANO_CDR_Stream_deserialize_i64(
    NANO_CDR_Stream *const self,
    NANO_i64 *const val);

#define NANO_CDR_Stream_deserialize_i64(s_,v_) \
    NANO_CDR_Stream_deserialize_u64((s_),(NANO_u64*)(v_))

/*e
 * @brief 
 * 
 * @param self 
 * @param val 
 * @return 
 */
NANODllExport
NANO_RetCode
NANO_CDR_Stream_deserialize_octets(
    NANO_CDR_Stream *const self,
    NANO_u8 *const val,
    const NANO_usize val_len);

/*e
 * @brief 
 * 
 * @param self 
 * @param val 
 * @return 
 */
NANO_RetCode
NANO_CDR_Stream_serialize_u8(
    NANO_CDR_Stream *const self,
    const NANO_u8 *const val);

/*e
 * @brief 
 * 
 * @param self 
 * @param val 
 * @return 
 */
NANODllExport
NANO_RetCode
NANO_CDR_Stream_serialize_u16(
    NANO_CDR_Stream *const self,
    const NANO_u16 *const val);

/*e
 * @brief 
 * 
 * @param self 
 * @param val 
 * @return 
 */
NANODllExport
NANO_RetCode
NANO_CDR_Stream_serialize_u32(
    NANO_CDR_Stream *const self,
    const NANO_u32 *const val);

/*e
 * @brief 
 * 
 * @param self 
 * @param val 
 * @return 
 */
NANODllExport
NANO_RetCode
NANO_CDR_Stream_serialize_u64(
    NANO_CDR_Stream *const self,
    const NANO_u64 *const val);

/*e
 * @brief 
 * 
 * @param self 
 * @param val 
 * @return 
 */
NANO_RetCode
NANO_CDR_Stream_serialize_i8(
    NANO_CDR_Stream *const self,
    NANO_u8 *const val);

#define NANO_CDR_Stream_serialize_i8(s_,v_) \
    NANO_CDR_Stream_serialize_u8((s_),(const NANO_u8*)(v_))

/*e
 * @brief 
 * 
 * @param self 
 * @param val 
 * @return 
 */
NANO_RetCode
NANO_CDR_Stream_serialize_i16(
    NANO_CDR_Stream *const self,
    const NANO_i16 *const val);

#define NANO_CDR_Stream_serialize_i16(s_,v_) \
    NANO_CDR_Stream_serialize_u16((s_),(const NANO_u16*)(v_))

/*e
 * @brief 
 * 
 * @param self 
 * @param val 
 * @return 
 */
NANO_RetCode
NANO_CDR_Stream_serialize_i32(
    NANO_CDR_Stream *const self,
    const NANO_i32 *const val);

#define NANO_CDR_Stream_serialize_i32(s_,v_) \
    NANO_CDR_Stream_serialize_u32((s_),(const NANO_u32*)(v_))

/*e
 * @brief 
 * 
 * @param self 
 * @param val 
 * @return 
 */
NANO_RetCode
NANO_CDR_Stream_serialize_i64(
    NANO_CDR_Stream *const self,
    const NANO_i64 *const val);

#define NANO_CDR_Stream_serialize_i64(s_,v_) \
    NANO_CDR_Stream_serialize_u64((s_),(const NANO_u64*)(v_))

/*e
 * @brief 
 * 
 * @param self 
 * @param val 
 * @return 
 */
NANODllExport
NANO_RetCode
NANO_CDR_Stream_serialize_octets(
    NANO_CDR_Stream *const self,
    const NANO_u8 *const val,
    const NANO_usize val_len);

/** @} *//* nano_api_cdr_stream */

/**
 * @addtogroup nano_api_cdr_stream
 * @{
 */

#if NANO_FEAT_STRING

#if 0
NANODllExport
NANO_RetCode
NANO_CDR_Stream_deserialize_conststring(
    NANO_CDR_Stream *const self,
    const char **const str,
    const NANO_usize str_max);
#endif

/*e
 * @brief 
 * 
 * @param self 
 * @param str 
 * @param str_max 
 * @param str_len 
 * @return NANODllExport 
 */
NANODllExport
NANO_RetCode
NANO_CDR_Stream_deserialize_string(
    NANO_CDR_Stream *const self,
    char *const *const str,
    const NANO_usize str_max,
    NANO_usize *const str_len);

/*e
 * @brief 
 * 
 * @param self 
 * @param str 
 * @param str_max 
 * @return 
 */
NANODllExport
NANO_RetCode
NANO_CDR_Stream_serialize_string(
    NANO_CDR_Stream *const self,
    const char *const *const str,
    const NANO_usize str_max);

/*i
 * @brief 
 * 
 * @param self 
 * @param str 
 * @param str_max 
 * @return 
 */
NANODllExport
NANO_usize
NANO_CDR_Stream_serialized_size_string(
    const NANO_CDR_Stream *const self,
    const char *const *const str,
    const NANO_usize str_max);

/*i
 * @brief 
 * 
 * @param self 
 * @param str 
 * @param str_max 
 * @return NANODllExport 
 */
NANODllExport
NANO_RetCode
NANO_CDR_Stream_deserialize_string_alloc(
    NANO_CDR_Stream *const self,
    char **const str,
    const NANO_usize str_max);

#endif /* NANO_FEAT_STRING */

/** @} *//* nano_api_cdr_stream */


/**
 * @addtogroup nano_api_cdr_stream
 * @{
 */
#if NANO_FEAT_SEQUENCE
/*e
 * @brief 
 * 
 * @param self 
 * @param seq 
 * @param seq_max 
 * @return 
 */
NANODllExport
NANO_RetCode
NANO_CDR_Stream_deserialize_psequence(
    NANO_CDR_Stream *const self,
    NANO_Sequence *const seq,
    const NANO_usize seq_max);

/*e
 * @brief 
 * 
 * @param self 
 * @param seq 
 * @param seq_max 
 * @return 
 */
NANODllExport
NANO_RetCode
NANO_CDR_Stream_serialize_psequence(
    NANO_CDR_Stream *const self,
    const NANO_Sequence *const seq,
    const NANO_usize seq_max);

/*i
 * @brief 
 * 
 * @param self 
 * @param seq 
 * @param seq_max 
 * @return 
 */
NANODllExport
NANO_usize
NANO_CDR_Stream_serialized_size_psequence(
    const NANO_CDR_Stream *const self,
    const NANO_Sequence *const seq,
    const NANO_usize seq_max);

/*e
 * @brief 
 * 
 * @param self 
 * @param seq 
 * @param seq_max 
 * @param deserialize_member 
 * @return 
 */
NANODllExport
NANO_RetCode
NANO_CDR_Stream_deserialize_sequence(
    NANO_CDR_Stream *const self,
    NANO_Sequence *const seq,
    const NANO_usize seq_max,
    NANO_CDR_Stream_DeserializeCdrFn deserialize_member);

/*e
 * @brief 
 * 
 * @param self 
 * @param seq 
 * @param seq_max 
 * @param serialize_member 
 * @return 
 */
NANODllExport
NANO_RetCode
NANO_CDR_Stream_serialize_sequence(
    NANO_CDR_Stream *const self,
    const NANO_Sequence *const seq,
    const NANO_usize seq_max,
    NANO_CDR_Stream_SerializeCdrFn serialize_member);

/*i
 * @brief 
 * 
 * @param self 
 * @param seq 
 * @param seq_max 
 * @param serialized_size_member 
 * @return 
 */
NANODllExport
NANO_usize
NANO_CDR_Stream_serialized_size_sequence(
    const NANO_CDR_Stream *const self,
    const NANO_Sequence *const seq,
    const NANO_usize seq_max,
    NANO_CDR_Stream_SerializedSizeCdrFn serialized_size_member);

#endif /* NANO_FEAT_SEQUENCE */

/** @} *//* nano_api_cdr_stream */

#endif /* NANO_FEAT_CDR */

/******************************************************************************
 *                          Implementation Macros
 ******************************************************************************/
/**
 * @addtogroup nano_api_cdr_serhelp
 * @{
 */

/*e
 * @brief 
 * 
 */
#define NANO_CDR_swap_u16(v_) \
    ((((NANO_u16)(v_) & 0xff00) >> 8) | \
     (((NANO_u16)(v_) & 0x00ff) << 8))

/*e
 * @brief 
 * 
 */
#define NANO_CDR_swap_u32(v_) \
    ((((NANO_u32)(v_) & 0xff000000) >> 24) | \
     (((NANO_u32)(v_) & 0x00ff0000) >> 8)  | \
     (((NANO_u32)(v_) & 0x0000ff00) << 8)  | \
     (((NANO_u32)(v_) & 0x000000ff) << 24))

/*e
 * @brief 
 * 
 */
#define NANO_CDR_swap_u64(v_) \
    ((((NANO_u64)(v_) & 0xff00000000000000) >> 32) | \
     (((NANO_u64)(v_) & 0x00ff000000000000) >> 24) | \
     (((NANO_u64)(v_) & 0x0000ff0000000000) >> 16) | \
     (((NANO_u64)(v_) & 0x000000ff00000000) >> 8)  | \
     (((NANO_u64)(v_) & 0x00000000ff000000) << 8)  | \
     (((NANO_u64)(v_) & 0x0000000000ff0000) << 16) | \
     (((NANO_u64)(v_) & 0x000000000000ff00) << 24) | \
     (((NANO_u64)(v_) & 0x00000000000000ff) << 32))

/*e
 * @brief 
 * 
 */
#define NANO_CDR_swap_u16_ptr(p_) \
    ((NANO_u16)*(((NANO_u8*)(p_)) + 1) | \
     (((NANO_u16)*((NANO_u8*)(p_))) << 8))

/*e
 * @brief 
 * 
 */
#define NANO_CDR_swap_u32_ptr(p_) \
    (((NANO_u32)*(((NANO_u8*)(p_)) + 3)) | \
     (((NANO_u32)*(((NANO_u8*)(p_)) + 2)) << 8) | \
     (((NANO_u32)*(((NANO_u8*)(p_)) + 1)) << 16) | \
     (((NANO_u32)*((NANO_u8*)(p_))) << 24))

/*e
 * @brief 
 * 
 */
#define NANO_CDR_swap_u64_ptr(p_) \
    ((NANO_u64)*(((NANO_u8*)(p_)) + 7) | \
     (((NANO_u64)*(((NANO_u8*)(p_)) + 6)) << 8) | \
     (((NANO_u64)*(((NANO_u8*)(p_)) + 5)) << 16) | \
     (((NANO_u64)*(((NANO_u8*)(p_)) + 4)) << 24) | \
     (((NANO_u64)*(((NANO_u8*)(p_)) + 3)) << 32) | \
     (((NANO_u64)*(((NANO_u8*)(p_)) + 2)) << 40) | \
     (((NANO_u64)*(((NANO_u8*)(p_)) + 1)) << 48) | \
     (((NANO_u64)*((NANO_u8*)(p_))) << 56))

/*e
 * @brief 
 * 
 */
#define NANO_CDR_u16_from_ptr(p_) \
    (((NANO_u16)*((NANO_u8*)(p_))) | \
    ((NANO_u16)*(((NANO_u8*)(p_)) + 1)) << 8)

/*e
 * @brief 
 * 
 */
#define NANO_CDR_u32_from_ptr(p_) \
    (((NANO_u32)*((NANO_u8*)(p_))) | \
    ((NANO_u32)*(((NANO_u8*)(p_)) + 1)) << 8 | \
    ((NANO_u32)*(((NANO_u8*)(p_)) + 2)) << 16 | \
    ((NANO_u32)*(((NANO_u8*)(p_)) + 3)) << 24)

/*e
 * @brief 
 * 
 */
#define NANO_CDR_u64_from_ptr(p_) \
    (((NANO_u64)*((NANO_u8*)(p_))) | \
    ((NANO_u64)*(((NANO_u8*)(p_)) + 1)) << 8 | \
    ((NANO_u64)*(((NANO_u8*)(p_)) + 2)) << 16 | \
    ((NANO_u64)*(((NANO_u8*)(p_)) + 3)) << 24 | \
    ((NANO_u64)*(((NANO_u8*)(p_)) + 4)) << 32 | \
    ((NANO_u64)*(((NANO_u8*)(p_)) + 5)) << 40 | \
    ((NANO_u64)*(((NANO_u8*)(p_)) + 6)) << 48 | \
    ((NANO_u64)*(((NANO_u8*)(p_)) + 7)) << 56)

/*e
 * @brief 
 * 
 */
#define NANO_CDR_u16_set_ptr(p_,v_) \
{\
    *((NANO_u8*)(p_)) = (NANO_u8)((NANO_u16)(v_) & 0x00ff);\
    *(((NANO_u8*)(p_)) + 1) = (NANO_u8)(((NANO_u16)(v_) & 0xff00) >> 8);\
}

/*e
 * @brief 
 * 
 */
#define NANO_CDR_u32_set_ptr(p_,v_) \
{\
    *((NANO_u8*)(p_)) = (NANO_u8)((NANO_u32)(v_) & 0x000000ff);\
    *(((NANO_u8*)(p_)) + 1) = (NANO_u8)(((NANO_u32)(v_) & 0x0000ff00) >> 8);\
    *(((NANO_u8*)(p_)) + 2) = (NANO_u8)(((NANO_u32)(v_) & 0x00ff0000) >> 16);\
    *(((NANO_u8*)(p_)) + 3) = (NANO_u8)(((NANO_u32)(v_) & 0xff000000) >> 24);\
}

/*e
 * @brief 
 * 
 */
#define NANO_CDR_u64_set_ptr(p_,v_) \
{\
    *((NANO_u8*)(p_)) = (NANO_u8)((NANO_u64)(v_) & 0x00000000000000ff);\
    *(((NANO_u8*)(p_)) + 1) = (NANO_u8)(((NANO_u64)(v_) & 0x000000000000ff00) >> 8);\
    *(((NANO_u8*)(p_)) + 2) = (NANO_u8)(((NANO_u64)(v_) & 0x0000000000ff0000) >> 16);\
    *(((NANO_u8*)(p_)) + 3) = (NANO_u8)(((NANO_u64)(v_) & 0x00000000ff000000) >> 24);\
    *(((NANO_u8*)(p_)) + 4) = (NANO_u8)(((NANO_u64)(v_) & 0x000000ff00000000) >> 32);\
    *(((NANO_u8*)(p_)) + 5) = (NANO_u8)(((NANO_u64)(v_) & 0x0000ff0000000000) >> 40);\
    *(((NANO_u8*)(p_)) + 6) = (NANO_u8)(((NANO_u64)(v_) & 0x00ff000000000000) >> 48);\
    *(((NANO_u8*)(p_)) + 7) = (NANO_u8)(((NANO_u64)(v_) & 0xff00000000000000) >> 56);\
}

/*e
 * @brief 
 * 
 */
#define NANO_CDR_u16_set_ptr_swap(p_,v_) \
{\
    *(((NANO_u8*)(p_)) + 1) = (NANO_u8)((NANO_u16)(v_) & 0x00ff);\
    *((NANO_u8*)(p_)) = (NANO_u8)(((NANO_u16)(v_) & 0xff00) >> 8);\
}

/*e
 * @brief 
 * 
 */
#define NANO_CDR_u32_set_ptr_swap(p_,v_) \
{\
    *(((NANO_u8*)(p_)) + 3) = (NANO_u8)((NANO_u32)(v_) & 0x000000ff);\
    *(((NANO_u8*)(p_)) + 2) = (NANO_u8)(((NANO_u32)(v_) & 0x0000ff00) >> 8);\
    *(((NANO_u8*)(p_)) + 1) = (NANO_u8)(((NANO_u32)(v_) & 0x00ff0000) >> 16);\
    *((NANO_u8*)(p_)) = (NANO_u8)(((NANO_u32)(v_) & 0xff000000) >> 24);\
}

/*e
 * @brief 
 * 
 */
#define NANO_CDR_u64_set_ptr_swap(p_,v_) \
{\
    *(((NANO_u8*)(p_)) + 7) = (NANO_u8)((NANO_u64)(v_) & 0x00000000000000ff);\
    *(((NANO_u8*)(p_)) + 6) = (NANO_u8)(((NANO_u64)(v_) & 0x000000000000ff00) >> 8);\
    *(((NANO_u8*)(p_)) + 5) = (NANO_u8)(((NANO_u64)(v_) & 0x0000000000ff0000) >> 16);\
    *(((NANO_u8*)(p_)) + 4) = (NANO_u8)(((NANO_u64)(v_) & 0x00000000ff000000) >> 24);\
    *(((NANO_u8*)(p_)) + 3) = (NANO_u8)(((NANO_u64)(v_) & 0x000000ff00000000) >> 32);\
    *(((NANO_u8*)(p_)) + 2) = (NANO_u8)(((NANO_u64)(v_) & 0x0000ff0000000000) >> 40);\
    *(((NANO_u8*)(p_)) + 1) = (NANO_u8)(((NANO_u64)(v_) & 0x00ff000000000000) >> 48);\
    *((NANO_u8*)(p_)) = (NANO_u8)(((NANO_u64)(v_) & 0xff00000000000000) >> 56);\
}

/*e
 * @brief 
 * 
 * @param self 
 * @param ptr 
 * @param little_endian 
 */
void
NANO_u64_deserialize(
    NANO_u64 *const self, 
    const NANO_u8 *const ptr,
    const NANO_bool little_endian);

#define NANO_u64_deserialize(s_, p_, le_) \
{\
    if (NANO_CDR_needs_swap(le_)) \
    {\
        *(s_) = NANO_CDR_swap_u64_ptr((p_));\
    }\
    else\
    {\
        *(s_) = NANO_CDR_u64_from_ptr((p_));\
    }\
}

/*e
 * @brief 
 * 
 * @param self 
 * @param val 
 * @param little_endian 
 */
void
NANO_u64_deserialize_aligned(
    NANO_u64 *const self, 
    const NANO_u64 *const val,
    const NANO_bool little_endian);

#define NANO_u64_deserialize_aligned(s_, v_, le_) \
{\
    if (NANO_CDR_needs_swap(le_)) \
    {\
        *(s_) = NANO_CDR_swap_u64(*(v_));\
    }\
    else\
    {\
        *(s_) = *(v_);\
    }\
}

/*e
 * @brief 
 * 
 * @param self 
 * @param ptr 
 * @param little_endian 
 */
void
NANO_u32_deserialize(
    NANO_u32 *const self, 
    const NANO_u8 *const ptr,
    const NANO_bool little_endian);

#define NANO_u32_deserialize(s_, p_, le_) \
{\
    if (NANO_CDR_needs_swap(le_)) \
    {\
        *(s_) = NANO_CDR_swap_u32_ptr((p_));\
    }\
    else\
    {\
        *(s_) = NANO_CDR_u32_from_ptr((p_));\
    }\
}

/*e
 * @brief 
 * 
 * @param self 
 * @param val 
 * @param little_endian 
 */
void
NANO_u32_deserialize_aligned(
    NANO_u32 *const self, 
    const NANO_u32 *const val,
    const NANO_bool little_endian);

#define NANO_u32_deserialize_aligned(s_, v_, le_) \
{\
    if (NANO_CDR_needs_swap(le_)) \
    {\
        *(s_) = NANO_CDR_swap_u32(*(v_));\
    }\
    else\
    {\
        *(s_) = *(v_);\
    }\
}

/*e
 * @brief 
 * 
 * @param self 
 * @param ptr 
 * @param little_endian 
 */
void
NANO_u16_deserialize(
    NANO_u16 *const self, 
    const NANO_u8 *const ptr,
    const NANO_bool little_endian);

#define NANO_u16_deserialize(s_, p_, le_) \
{\
    if (NANO_CDR_needs_swap(le_)) \
    {\
        *(s_) = NANO_CDR_swap_u16_ptr((p_));\
    }\
    else\
    {\
        *(s_) = NANO_CDR_u16_from_ptr((p_));\
    }\
}

/*e
 * @brief 
 * 
 * @param self 
 * @param val 
 * @param little_endian 
 */
void
NANO_u16_deserialize_aligned(
    NANO_u16 *const self, 
    const NANO_u16 *const val,
    const NANO_bool little_endian);

#define NANO_u16_deserialize_aligned(s_, v_, le_) \
{\
    if (NANO_CDR_needs_swap(le_)) \
    {\
        *(s_) = NANO_CDR_swap_u16(*(v_));\
    }\
    else\
    {\
        *(s_) = *(v_);\
    }\
}

/*e
 * @brief 
 * 
 * @param self 
 * @param ptr 
 * @param little_endian 
 */
void
NANO_i64_deserialize(
    NANO_i64 *const self, 
    const NANO_u8 *const ptr,
    const NANO_bool little_endian);

#define NANO_i64_deserialize(s_,p_,le_) \
    NANO_u64_deserialize((NANO_u64*)(s_),p_,le_)

/*e
 * @brief 
 * 
 * @param self 
 * @param val 
 * @param little_endian 
 */
void
NANO_i64_deserialize_aligned(
    NANO_i64 *const self, 
    const NANO_u64 *const val,
    const NANO_bool little_endian);

#define NANO_i64_deserialize_aligned(s_,p_,le_) \
    NANO_u64_deserialize((NANO_u64*)(s_),(const NANO_u64*)p_,le_)

/*e
 * @brief 
 * 
 * @param self 
 * @param ptr 
 * @param little_endian 
 */
void
NANO_i32_deserialize(
    NANO_i32 *const self, 
    const NANO_u8 *const ptr,
    const NANO_bool little_endian);

#define NANO_i32_deserialize(s_,p_,le_) \
    NANO_u32_deserialize((NANO_u32*)(s_),p_,le_)

/*e
 * @brief 
 * 
 * @param self 
 * @param val 
 * @param little_endian 
 */
void
NANO_i32_deserialize_aligned(
    NANO_i32 *const self, 
    const NANO_i32 *const val,
    const NANO_bool little_endian);

#define NANO_i32_deserialize_aligned(s_,p_,le_) \
    NANO_u32_deserialize((NANO_u32*)(s_),(const NANO_u32*)p_,le_)

/*e
 * @brief 
 * 
 * @param self 
 * @param ptr 
 * @param little_endian 
 */
void
NANO_i16_deserialize(
    NANO_i16 *const self, 
    const NANO_u8 *const ptr,
    const NANO_bool little_endian);

#define NANO_i16_deserialize(s_,p_,le_) \
    NANO_u16_deserialize((NANO_u16*)(s_),p_,le_)

/*e
 * @brief 
 * 
 * @param self 
 * @param val 
 * @param little_endian 
 */
void
NANO_i16_deserialize_aligned(
    NANO_i16 *const self, 
    const NANO_i16 *const val,
    const NANO_bool little_endian);

#define NANO_i16_deserialize_aligned(s_,p_,le_) \
    NANO_u16_deserialize((NANO_u16*)(s_),(const NANO_u16*)p_,le_)

/*e
 * @brief 
 * 
 * @param self 
 * @param ptr 
 * @param little_endian 
 */
void
NANO_u64_serialize(
    const NANO_u64 self, 
    const NANO_u8 *const ptr,
    const NANO_bool little_endian);

#define NANO_u64_serialize(s_, p_, le_) \
{\
    NANO_u64 val_ = 0;\
    if (le_) \
    {\
        val_ = NANO_u64_to_le((s_));\
    }\
    else\
    {\
        val_ = NANO_u64_to_be((s_));\
    }\
    NANO_OSAPI_Memory_copy((p_),&val_,sizeof(NANO_u64));\
}

/*e
 * @brief 
 * 
 * @param self 
 * @param ptr 
 * @param little_endian 
 */
void
NANO_u64_serialize_aligned(
    const NANO_u64 self, 
    const NANO_u64 *const ptr,
    const NANO_bool little_endian);

#define NANO_u64_serialize_aligned(s_, p_, le_) \
{\
    NANO_u64 val_ = 0;\
    if (le_) \
    {\
        val_ = NANO_u64_to_le((s_));\
    }\
    else\
    {\
        val_ = NANO_u64_to_be((s_));\
    }\
    *(p_) = (s_);\
}

/*e
 * @brief 
 * 
 * @param self 
 * @param ptr 
 * @param little_endian 
 */
void
NANO_u32_serialize(
    const NANO_u32 self, 
    const NANO_u8 *const ptr,
    const NANO_bool little_endian);

#define NANO_u32_serialize(s_, p_, le_) \
{\
    NANO_u32 val_ = 0;\
    if (le_) \
    {\
        val_ = NANO_u32_to_le((s_));\
    }\
    else\
    {\
        val_ = NANO_u32_to_be((s_));\
    }\
    NANO_OSAPI_Memory_copy((p_),&val_,sizeof(NANO_u32));\
}

/*e
 * @brief 
 * 
 * @param self 
 * @param ptr 
 * @param little_endian 
 */
void
NANO_u32_serialize_aligned(
    const NANO_u32 self, 
    const NANO_u32 *const ptr,
    const NANO_bool little_endian);

#define NANO_u32_serialize_aligned(s_, p_, le_) \
{\
    NANO_u32 val_ = 0;\
    if (le_) \
    {\
        val_ = NANO_u32_to_le((s_));\
    }\
    else\
    {\
        val_ = NANO_u32_to_be((s_));\
    }\
    *(p_) = (s_);\
}

/*e
 * @brief 
 * 
 * @param self 
 * @param ptr 
 * @param little_endian 
 */
void
NANO_u16_serialize(
    const NANO_u16 self, 
    const NANO_u8 *const ptr,
    const NANO_bool little_endian);

#define NANO_u16_serialize(s_, p_, le_) \
{\
    NANO_u16 val_ = 0;\
    if (le_) \
    {\
        val_ = NANO_u16_to_le((s_));\
    }\
    else\
    {\
        val_ = NANO_u16_to_be((s_));\
    }\
    NANO_OSAPI_Memory_copy((p_),&val_,sizeof(NANO_u16));\
}

/*e
 * @brief 
 * 
 * @param self 
 * @param ptr 
 * @param little_endian 
 */
void
NANO_u16_serialize_aligned(
    const NANO_u16 self, 
    const NANO_u16 *const ptr,
    const NANO_bool little_endian);

#define NANO_u16_serialize_aligned(s_, p_, le_) \
{\
    NANO_u64 val_ = 0;\
    if (le_) \
    {\
        val_ = NANO_u16_to_le((s_));\
    }\
    else\
    {\
        val_ = NANO_u16_to_be((s_));\
    }\
    *(p_) = (s_);\
}

/*e
 * @brief 
 * 
 * @param self 
 * @param ptr 
 * @param little_endian 
 */
void
NANO_i64_serialize(
    const NANO_i64 self, 
    const NANO_u8 *const ptr,
    const NANO_bool little_endian);

#define NANO_i64_serialize(s_,p_,le_) \
    NANO_u64_serialize((NANO_u64)(s_),(p_),(le_))

/*e
 * @brief 
 * 
 * @param self 
 * @param ptr 
 * @param little_endian 
 */
void
NANO_i64_serialize_aligned(
    const NANO_i64 self, 
    const NANO_i64 *const ptr,
    const NANO_bool little_endian);

#define NANO_i64_serialize_aligned(s_,p_,le_) \
    NANO_u64_serialize_aligned((NANO_u64)(s_),(NANO_u64*)(p_),(le_))

/*e
 * @brief 
 * 
 * @param self 
 * @param ptr 
 * @param little_endian 
 */
void
NANO_i32_serialize(
    const NANO_i32 self, 
    const NANO_u8 *const ptr,
    const NANO_bool little_endian);

#define NANO_i32_serialize(s_,p_,le_) \
    NANO_u32_serialize((NANO_u32)(s_),(p_),(le_))

/*e
 * @brief 
 * 
 * @param self 
 * @param ptr 
 * @param little_endian 
 */
void
NANO_i32_serialize_aligned(
    const NANO_i32 self, 
    const NANO_i32 *const ptr,
    const NANO_bool little_endian);

#define NANO_i32_serialize_aligned(s_,p_,le_) \
    NANO_u32_serialize_aligned((NANO_u32)(s_),(NANO_u32*)(p_),(le_))

/*e
 * @brief 
 * 
 * @param self 
 * @param ptr 
 * @param little_endian 
 */
void
NANO_i16_serialize(
    const NANO_i16 self, 
    const NANO_u8 *const ptr,
    const NANO_bool little_endian);

#define NANO_i16_serialize(s_,p_,le_) \
    NANO_u16_serialize((NANO_u16)(s_),(p_),(le_))

/*e
 * @brief 
 * 
 * @param self 
 * @param ptr 
 * @param little_endian 
 */
void
NANO_i16_serialize_aligned(
    const NANO_i16 self, 
    const NANO_i16 *const ptr,
    const NANO_bool little_endian);

#define NANO_i16_serialize_aligned(s_,p_,le_) \
    NANO_u16_serialize_aligned((NANO_u16)(s_),(NANO_u16*)(p_),(le_))

/******************************************************************************
 *                          Primitive Type Serialization
 ******************************************************************************/

#define NANO_Primitive16_alignment  ((NANO_u8)16)
#define NANO_Primitive8_alignment   ((NANO_u8)8)
#define NANO_Primitive4_alignment   ((NANO_u8)4)
#define NANO_Primitive2_alignment   ((NANO_u8)2)
#define NANO_Primitive1_alignment   ((NANO_u8)1)


#define NANO_u16_alignment \
        NANO_Primitive2_alignment

#define NANO_u32_alignment \
        NANO_Primitive4_alignment\

#define NANO_u64_alignment \
    NANO_Primitive8_alignment

#define NANO_i16_alignment \
        NANO_Primitive2_alignment

#define NANO_i32_alignment \
        NANO_Primitive4_alignment\

#define NANO_i64_alignment \
    NANO_Primitive8_alignment

#define NANO_u8_alignment \
    NANO_Primitive1_alignment

#define NANO_i8_alignment \
    NANO_Primitive1_alignment

#define NANO_bool_alignment \
        NANO_u8_alignment

#if NANO_64BIT
#define NANO_usize_alignment \
        NANO_u64_alignment
#else /* NANO_64BIT */
#define NANO_usize_alignment \
        NANO_u32_alignment
#endif /* NANO_64BIT */

#define NANO_Octets_alignment \
        NANO_u8_alignment

/** @} *//* nano_api_cdr_serhelp */


#if NANO_FEAT_CDR

/**
 * @addtogroup nano_api_cdr_serhelp
 * @{
 */

/******************************************************************************
 *                                u16
 ******************************************************************************/

NANO_CDR_STREAM_INTF_DECL(NANO_u16)

#define NANO_u16_deserialize_cdr(s_,sr_) \
    NANO_CDR_Stream_deserialize_u16((sr_),(s_))

#define NANO_u16_serialize_cdr(s_,sr_) \
    NANO_CDR_Stream_serialize_u16((sr_),(s_))

#define NANO_u16_serialized_size_cdr(s_,sr_) \
    sizeof(NANO_u16)

/******************************************************************************
 *                                u32
 ******************************************************************************/

NANO_CDR_STREAM_INTF_DECL(NANO_u32)

#define NANO_u32_deserialize_cdr(s_,sr_) \
    NANO_CDR_Stream_deserialize_u32((sr_),(s_))

#define NANO_u32_serialize_cdr(s_,sr_) \
    NANO_CDR_Stream_serialize_u32((sr_),(s_))

#define NANO_u32_serialized_size_cdr(s_,sr_) \
    sizeof(NANO_u32)

/******************************************************************************
 *                                u64
 ******************************************************************************/

NANO_CDR_STREAM_INTF_DECL(NANO_u64)

#define NANO_u64_deserialize_cdr(s_,sr_) \
    NANO_CDR_Stream_deserialize_u64((sr_),(s_))

#define NANO_u64_serialize_cdr(s_,sr_) \
    NANO_CDR_Stream_serialize_u64((sr_),(s_))

#define NANO_u64_serialized_size_cdr(s_,sr_) \
    sizeof(NANO_u64)

/******************************************************************************
 *                                i16
 ******************************************************************************/

NANO_CDR_STREAM_INTF_DECL(NANO_i16)

#define NANO_i16_deserialize_cdr(s_,sr_) \
    NANO_CDR_Stream_deserialize_i16((sr_),(s_))

#define NANO_i16_serialize_cdr(s_,sr_) \
    NANO_CDR_Stream_serialize_i16((sr_),(s_))

#define NANO_i16_serialized_size_cdr(s_,sr_) \
    sizeof(NANO_i16)

/******************************************************************************
 *                                i32
 ******************************************************************************/

NANO_CDR_STREAM_INTF_DECL(NANO_i32)

#define NANO_i32_deserialize_cdr(s_,sr_) \
    NANO_CDR_Stream_deserialize_i32((sr_),(s_))

#define NANO_i32_serialize_cdr(s_,sr_) \
    NANO_CDR_Stream_serialize_i32((sr_),(s_))

#define NANO_i32_serialized_size_cdr(s_,sr_) \
    sizeof(NANO_i32)

/******************************************************************************
 *                                i64
 ******************************************************************************/

NANO_CDR_STREAM_INTF_DECL(NANO_i64)

#define NANO_i64_deserialize_cdr(s_,sr_) \
    NANO_CDR_Stream_deserialize_i64((sr_),(s_))

#define NANO_i64_serialize_cdr(s_,sr_) \
    NANO_CDR_Stream_serialize_i64((sr_),(s_))

#define NANO_i64_serialized_size_cdr(s_,sr_) \
    sizeof(NANO_i64)

/******************************************************************************
 *                                u8
 ******************************************************************************/

NANO_CDR_STREAM_INTF_DECL(NANO_u8)

#define NANO_u8_deserialize_cdr(s_,sr_) \
    NANO_CDR_Stream_deserialize_u8((sr_),(s_))

#define NANO_u8_serialize_cdr(s_,sr_) \
    NANO_CDR_Stream_serialize_u8((sr_),(s_))

#define NANO_u8_serialized_size_cdr(s_,sr_) \
    sizeof(NANO_u8)

/******************************************************************************
 *                                i8
 ******************************************************************************/

NANO_CDR_STREAM_INTF_DECL(NANO_i8)

#define NANO_i8_deserialize_cdr(s_,sr_) \
    NANO_CDR_Stream_deserialize_i8((sr_),(s_))

#define NANO_i8_serialize_cdr(s_,sr_) \
    NANO_CDR_Stream_serialize_i8((sr_),(s_))

#define NANO_i8_serialized_size_cdr(s_,sr_) \
    sizeof(NANO_i8)

/******************************************************************************
 *                               Bool
 ******************************************************************************/

NANO_CDR_STREAM_INTF_DECL(NANO_bool)

#define NANO_bool_deserialize_cdr(s_,sr_) \
    NANO_u8_deserialize_cdr((s_) ,(sr_))

#define NANO_bool_serialize_cdr(s_,sr_) \
    NANO_u8_serialize_cdr((s_) ,(sr_))

#define NANO_bool_serialized_size_cdr(s_,sr_) \
    NANO_u8_serialized_size_cdr((s_) ,(sr_))

/******************************************************************************
 *                              Usize
 ******************************************************************************/

NANO_CDR_STREAM_INTF_DECL(NANO_usize)

#if NANO_64BIT

#define NANO_usize_deserialize_cdr(s_,sr_) \
    NANO_u64_deserialize_cdr((s_) ,(sr_))

#define NANO_usize_serialize_cdr(s_,sr_) \
    NANO_u64_serialize_cdr((s_) ,(sr_))

#define NANO_usize_serialized_size_cdr(s_,sr_) \
    NANO_u64_serialized_size_cdr((s_) ,(sr_))

#define NANO_usize_alignment \
        NANO_u64_alignment

#else /* NANO_64BIT */

#define NANO_usize_deserialize_cdr(s_,sr_) \
    NANO_u32_deserialize_cdr((s_) ,(sr_))

#define NANO_usize_serialize_cdr(s_,sr_) \
    NANO_u32_serialize_cdr((s_) ,(sr_))

#define NANO_usize_serialized_size_cdr(s_,sr_) \
    NANO_u32_serialized_size_cdr((s_) ,(sr_))

#define NANO_usize_alignment \
        NANO_u32_alignment

#endif /* NANO_64BIT */

/******************************************************************************
 *                                  Octets
 ******************************************************************************/

NANO_RetCode
NANO_Octets_deserialize_cdr(
    NANO_u8 *const self,
    NANO_CDR_Stream *const stream,
    NANO_usize value_len);

NANO_RetCode
NANO_Octets_serialize_cdr(
    const NANO_u8 *const self,
    NANO_CDR_Stream *const stream,
    NANO_usize value_len);

NANO_RetCode
NANO_Octets_serialized_size_cdr(
    const NANO_u8 *const self,
    NANO_CDR_Stream *const stream,
    NANO_usize value_len);

#define NANO_Octets_deserialize_cdr(s_, sr_, vl_) \
    NANO_CDR_Stream_deserialize_octets((sr_),(NANO_u8 *const)(s_),(vl_))

#define NANO_Octets_serialize_cdr(s_, sr_, vl_)  \
    NANO_CDR_Stream_serialize_octets((sr_),(const NANO_u8 *const)(s_),(vl_))

#define NANO_Octets_serialized_size_cdr(s_, sr_, vl_) \
        (vl_)

#if 0
NANODllExport
void
NANO_Octets_from_u16(
    NANO_u8 *const self,
    const NANO_u16 val,
    const NANO_CDR_Endianness endianness);

NANODllExport
NANO_u16
NANO_Octets_to_u16(
    const NANO_u8 *const self,
    const NANO_CDR_Endianness endianness);
#endif

/** @} *//* nano_api_cdr_serhelp */

#if NANO_FEAT_DATA
/******************************************************************************
 *                             EncapsulationHeader
 ******************************************************************************/

/*i
 * @addtogroup nano_api_cdr_support
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_CDR_EncapsulationHeader)

#define NANO_CDR_EncapsulationHeader_deserialize_cdr(s_,sr_) \
    NANO_Octets_deserialize_cdr(\
        NANO_MEMBER((s_),value),(sr_),NANO_CDR_ENCAPSULATIONHEADER_VALUE_LEN)

#define NANO_CDR_EncapsulationHeader_serialize_cdr(s_,sr_) \
    NANO_Octets_serialize_cdr(\
        NANO_MEMBER((s_),value),(sr_),NANO_CDR_ENCAPSULATIONHEADER_VALUE_LEN)

#define NANO_CDR_EncapsulationHeader_serialized_size_cdr(s_,sr_) \
    NANO_Octets_serialized_size_cdr(\
        NANO_MEMBER((s_),value),(sr_),NANO_CDR_ENCAPSULATIONHEADER_VALUE_LEN)

#define NANO_CDR_EncapsulationHeader_alignment \
    NANO_Octets_alignment

/*i @} *//* nano_api_cdr_support */

/******************************************************************************
 *                             DataHeader
 ******************************************************************************/
/*i
 * @addtogroup nano_api_cdr_support
 * @{
 */
NANO_CDR_STREAM_INTF_DECL(NANO_CDR_DataHeader)

#define NANO_CDR_DataHeader_deserialize_cdr(s_,sr_) \
    NANO_u32_deserialize_cdr((s_),(sr_))

#define NANO_CDR_DataHeader_serialize_cdr(s_,sr_) \
    NANO_u32_serialize_cdr((s_),(sr_))

#define NANO_CDR_DataHeader_serialized_size_cdr(s_,sr_) \
    NANO_u32_serialized_size_cdr((s_),(sr_))

#define NANO_CDR_DataHeader_alignment \
    NANO_u32_alignment
#endif /* NANO_FEAT_DATA */

/*i @} *//* nano_api_data_support */

#if NANO_FEAT_STRING
/******************************************************************************
 *                                  String
 ******************************************************************************/

/*i
 * @addtogroup nano_api_cdr_support
 * @{
 */

#define NANO_STRING_LENGTH_UNBOUNDED        0

NANO_CDR_STREAM_INTF_DECL(NANO_String)

#define NANO_String_deserialize_cdr_max(s_,sr_,max_) \
    NANO_CDR_Stream_deserialize_string_alloc((sr_),(s_),(max_))

#define NANO_String_serialize_cdr_max(s_,sr_,max_) \
    NANO_CDR_Stream_serialize_string((sr_),(const char *const *const)(s_),(max_))

#define NANO_String_serialized_size_cdr_max(s_,sr_,max_) \
    NANO_CDR_Stream_serialized_size_string((sr_),(const char *const *const)(s_), (max_))

#define NANO_String_deserialize_cdr(s_,sr_) \
    NANO_String_deserialize_cdr_max(\
        (s_),(sr_),NANO_STRING_LENGTH_UNBOUNDED)

#define NANO_String_serialize_cdr(s_,sr_) \
    NANO_String_serialize_cdr_max(\
        (s_),(sr_),NANO_STRING_LENGTH_UNBOUNDED)

#define NANO_String_serialized_size_cdr(s_,sr_) \
    NANO_String_serialized_size_cdr_max(\
        (s_),(sr_),NANO_STRING_LENGTH_UNBOUNDED)

#define NANO_String_alignment \
        NANO_u32_alignment

NANO_CDR_STREAM_INTF_DECL(NANO_ConstString)

#define NANO_ConstString_deserialize_cdr_max(s_,sr_,max_) \
    NANO_CDR_Stream_deserialize_conststring((sr_),(s_),(max_))

#define NANO_ConstString_serialize_cdr_max(s_,sr_,max_) \
    NANO_String_serialize_cdr_max((s_),(sr_),(max_))

#define NANO_ConstString_serialized_size_cdr_max(s_,sr_,max_) \
    NANO_String_serialized_size_cdr_max((s_),(sr_),(max_))

#define NANO_ConstString_deserialize_cdr(s_,sr_) \
    NANO_ConstString_deserialize_cdr_max(\
        (s_),(sr_),NANO_STRING_LENGTH_UNBOUNDED)

#define NANO_ConstString_serialize_cdr(s_,sr_) \
    NANO_String_serialize_cdr((s_),(sr_))

#define NANO_ConstString_serialized_size_cdr(s_,sr_) \
    NANO_String_serialized_size_cdr((s_),(sr_))

#define NANO_ConstString_alignment \
    NANO_String_alignment

/*i @} *//* nano_api_cdr_support */

#endif /* NANO_FEAT_STRING */

#endif /* NANO_FEAT_CDR */

/******************************************************************************
 *                       Conversion utilities
 ******************************************************************************/

/*e
 * @addtogroup nano_api_cdr_serhelp
 * @{
 */

/*e
 * @brief 
 * 
 * @param val 
 * @return NANO_u64 
 */
NANO_u64
NANO_u64_to_be(const NANO_u64 val);

/*e
 * @brief 
 * 
 * @param val 
 * @return NANO_u32 
 */
NANO_u32
NANO_u32_to_be(const NANO_u32 val);

/*e
 * @brief 
 * 
 * @param val 
 * @return NANO_u16 
 */
NANO_u16
NANO_u16_to_be(const NANO_u16 val);

/*e
 * @brief 
 * 
 * @param val 
 * @return NANO_u16 
 */
NANO_u16
NANO_u16_from_be(const NANO_u16 val);

/*e
 * @brief 
 * 
 * @param val 
 * @return NANO_u64 
 */
NANO_u64
NANO_u64_from_be(const NANO_u64 val);

/*e
 * @brief 
 * 
 * @param val 
 * @return NANO_u32 
 */
NANO_u32
NANO_u32_from_be(const NANO_u32 val);

/*e
 * @brief 
 * 
 * @param val 
 * @return NANO_u64 
 */
NANO_u64
NANO_u64_to_le(const NANO_u64 val);

/*e
 * @brief 
 * 
 * @param val 
 * @return NANO_u32 
 */
NANO_u32
NANO_u32_to_le(const NANO_u32 val);

/*e
 * @brief 
 * 
 * @param val 
 * @return NANO_u16 
 */
NANO_u16
NANO_u16_to_le(const NANO_u16 val);

/*e
 * @brief 
 * 
 * @param val 
 * @return NANO_u64 
 */
NANO_u64
NANO_u64_from_le(const NANO_u64 val);

/*e
 * @brief 
 * 
 * @param val 
 * @return NANO_u32 
 */
NANO_u32
NANO_u32_from_le(const NANO_u32 val);

/*e
 * @brief 
 * 
 * @param val 
 * @return NANO_u16 
 */
NANO_u16
NANO_u16_from_le(const NANO_u16 val);

/*e
 * @brief 
 * 
 * @param val 
 * @return NANO_u64 
 */
NANO_u64
NANO_u64_to_be_ptr(const NANO_u64 *val);

/*e
 * @brief 
 * 
 * @param val 
 * @return NANO_u32 
 */
NANO_u32
NANO_u32_to_be_ptr(const NANO_u32 *val);

/*e
 * @brief 
 * 
 * @param val 
 * @return NANO_u16 
 */
NANO_u16
NANO_u16_to_be_ptr(const NANO_u16 *val);

/*e
 * @brief 
 * 
 * @param val 
 * @return NANO_u16 
 */
NANO_u16
NANO_u16_from_ptr_be(const NANO_u16 *val);

/*e
 * @brief 
 * 
 * @param val 
 * @return NANO_u64 
 */
NANO_u64
NANO_u64_from_ptr_be(const NANO_u64 *val);

/*e
 * @brief 
 * 
 * @param val 
 * @return NANO_u32 
 */
NANO_u32
NANO_u32_from_ptr_be(const NANO_u32 *val);

/*e
 * @brief 
 * 
 * @param val 
 * @return NANO_u64 
 */
NANO_u64
NANO_u64_to_le_ptr(const NANO_u64 *val);

/*e
 * @brief 
 * 
 * @param val 
 * @return NANO_u32 
 */
NANO_u32
NANO_u32_to_le_ptr(const NANO_u32 *val);

/*e
 * @brief 
 * 
 * @param val 
 * @return NANO_u16 
 */
NANO_u16
NANO_u16_to_le_ptr(const NANO_u16 *val);

/*e
 * @brief 
 * 
 * @param val 
 * @return NANO_u64 
 */
NANO_u64
NANO_u64_from_ptr_le(const NANO_u64 *val);

/*e
 * @brief 
 * 
 * @param val 
 * @return NANO_u32 
 */
NANO_u32
NANO_u32_from_ptr_le(const NANO_u32 *val);

/*e
 * @brief 
 * 
 * @param val 
 * @return NANO_u16 
 */
NANO_u16
NANO_u16_from_ptr_le(const NANO_u16 *val);

/*e
 * @brief 
 * 
 * @param self 
 * @param val 
 */
void
NANO_u16_set_ptr_le(NANO_u16 *const self, const NANO_u16 val);

/*e
 * @brief 
 * 
 * @param self 
 * @param val 
 */
void
NANO_u32_set_ptr_le(NANO_u32 *const self, const NANO_u32 val);

/*e
 * @brief 
 * 
 * @param self 
 * @param val 
 */
void
NANO_u64_set_ptr_le(NANO_u64 *const self, const NANO_u64 val);

/*e
 * @brief 
 * 
 * @param self 
 * @param val 
 */
void
NANO_u16_set_ptr_be(NANO_u16 *const self, const NANO_u16 val);

/*e
 * @brief 
 * 
 * @param self 
 * @param val 
 */
void
NANO_u32_set_ptr_be(NANO_u32 *const self, const NANO_u32 val);

/*e
 * @brief 
 * 
 * @param self 
 * @param val 
 */
void
NANO_u64_set_ptr_be(NANO_u64 *const self, const NANO_u64 val);


#ifdef NANO_ENDIAN_LITTLE
#define NANO_u64_to_be(v_)          NANO_CDR_swap_u64((v_))
#define NANO_u32_to_be(v_)          NANO_CDR_swap_u32((v_))
#define NANO_u16_to_be(v_)          NANO_CDR_swap_u16((v_))
#define NANO_u64_from_be(v_)        NANO_CDR_swap_u64((v_))
#define NANO_u32_from_be(v_)        NANO_CDR_swap_u32((v_))
#define NANO_u16_from_be(v_)        NANO_CDR_swap_u16((v_))
#define NANO_u64_to_le(v_)          (v_)
#define NANO_u32_to_le(v_)          (v_)
#define NANO_u16_to_le(v_)          (v_)
#define NANO_u64_from_le(v_)        (v_)
#define NANO_u32_from_le(v_)        (v_)
#define NANO_u16_from_le(v_)        (v_)

#define NANO_u64_to_be_ptr(p_)      NANO_CDR_swap_u64_ptr((p_))
#define NANO_u32_to_be_ptr(p_)      NANO_CDR_swap_u32_ptr((p_))
#define NANO_u16_to_be_ptr(p_)      NANO_CDR_swap_u16_ptr((p_))
#define NANO_u64_from_ptr_be(p_)    NANO_CDR_swap_u64_ptr((p_))
#define NANO_u32_from_ptr_be(p_)    NANO_CDR_swap_u32_ptr((p_))
#define NANO_u16_from_ptr_be(p_)    NANO_CDR_swap_u16_ptr((p_))
#define NANO_u64_to_le_ptr(p_)      NANO_CDR_u64_from_ptr(p_)
#define NANO_u32_to_le_ptr(p_)      NANO_CDR_u32_from_ptr(p_)
#define NANO_u16_to_le_ptr(p_)      NANO_CDR_u16_from_ptr(p_)
#define NANO_u64_from_ptr_le(p_)    NANO_CDR_u64_from_ptr(p_)
#define NANO_u32_from_ptr_le(p_)    NANO_CDR_u32_from_ptr(p_)
#define NANO_u16_from_ptr_le(p_)    NANO_CDR_u16_from_ptr(p_)

#define NANO_u64_set_ptr_le(p_,v_) \
        NANO_CDR_u64_set_ptr((p_),(v_))
#define NANO_u32_set_ptr_le(p_,v_) \
        NANO_CDR_u32_set_ptr((p_),(v_))
#define NANO_u16_set_ptr_le(p_,v_) \
        NANO_CDR_u16_set_ptr((p_),(v_))

#define NANO_u64_set_ptr_be(p_,v_) \
        NANO_CDR_u64_set_ptr_swap((p_),(v_))
#define NANO_u32_set_ptr_be(p_,v_) \
        NANO_CDR_u32_set_ptr_swap((p_),(v_))
#define NANO_u16_set_ptr_be(p_,v_) \
        NANO_CDR_u16_set_ptr_swap((p_),(v_))

#else /* NANO_ENDIAN_BIG */
#define NANO_u64_to_be(v_)          (v_)
#define NANO_u32_to_be(v_)          (v_)
#define NANO_u16_to_be(v_)          (v_)
#define NANO_u64_from_be(v_)        (v_)
#define NANO_u32_from_be(v_)        (v_)
#define NANO_u16_from_be(v_)        (v_)
#define NANO_u64_to_le(v_)          NANO_CDR_swap_u64((v_))
#define NANO_u32_to_le(v_)          NANO_CDR_swap_u32((v_))
#define NANO_u16_to_le(v_)          NANO_CDR_swap_u16((v_))
#define NANO_u64_from_le(v_)        NANO_CDR_swap_u64((v_))
#define NANO_u32_from_le(v_)        NANO_CDR_swap_u32((v_))
#define NANO_u16_from_le(v_)        NANO_CDR_swap_u16((v_))

#define NANO_u64_to_be_ptr(p_)      NANO_CDR_u64_from_ptr(p_)
#define NANO_u32_to_be_ptr(p_)      NANO_CDR_u32_from_ptr(p_)
#define NANO_u16_to_be_ptr(p_)      NANO_CDR_u16_from_ptr(p_)
#define NANO_u64_from_ptr_be(p_)    NANO_CDR_u64_from_ptr(p_)
#define NANO_u32_from_ptr_be(p_)    NANO_CDR_u32_from_ptr(p_)
#define NANO_u16_from_ptr_be(p_)    NANO_CDR_u16_from_ptr(p_)
#define NANO_u64_to_le_ptr(p_)      NANO_CDR_swap_u64_ptr((p_))
#define NANO_u32_to_le_ptr(p_)      NANO_CDR_swap_u32_ptr((p_))
#define NANO_u16_to_le_ptr(p_)      NANO_CDR_swap_u16_ptr((p_))
#define NANO_u64_from_ptr_le(p_)    NANO_CDR_swap_u64_ptr((p_))
#define NANO_u32_from_ptr_le(p_)    NANO_CDR_swap_u32_ptr((p_))
#define NANO_u16_from_ptr_le(p_)    NANO_CDR_swap_u16_ptr((p_))

#define NANO_u64_set_ptr(p_,v_)     NANO_CDR_u64_set_ptr_be((p_),(v_))
#define NANO_u32_set_ptr(p_,v_)     NANO_CDR_u32_set_ptr_be((p_),(v_))
#define NANO_u16_set_ptr(p_,v_)     NANO_CDR_u16_set_ptr_be((p_),(v_))

#define NANO_u64_set_ptr_be(p_,v_) \
        NANO_CDR_u64_set_ptr((p_),(v_))
#define NANO_u32_set_ptr_be(p_,v_) \
        NANO_CDR_u32_set_ptr((p_),(v_))
#define NANO_u16_set_ptr_be(p_,v_) \
        NANO_CDR_u16_set_ptr((p_),(v_))

#define NANO_u64_set_ptr_le(p_,v_) \
        NANO_CDR_u64_set_ptr_swap((p_),(v_))
#define NANO_u32_set_ptr_le(p_,v_) \
        NANO_CDR_u32_set_ptr_swap((p_),(v_))
#define NANO_u16_set_ptr_le(p_,v_) \
        NANO_CDR_u16_set_ptr_swap((p_),(v_))

#endif /* NANO_ENDIAN_LITTLE */

/*e @} *//* nano_api_cdr_serhelp */

#if NANO_FEAT_CDR

#if NANO_FEAT_TIME
/******************************************************************************
 *                                  Time
 ******************************************************************************/

/*i
 * @addtogroup nano_api_infr_time
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_Time)

#define NANO_Time_serialized_size_cdr(s_,sr_) \
    ((NANO_usize)(\
        sizeof(NANO_i32) + \
        sizeof(NANO_u32)\
    ))

#define NANO_Time_alignment \
        NANO_i32_alignment

/*i @} *//* nano_api_infr_time */

#endif /* NANO_FEAT_TIME */

#if NANO_FEAT_SEQUENCE
/******************************************************************************
 *                               Sequence
 ******************************************************************************/
/*i
 * @addtogroup nano_api_data_stream_sequence
 * @{
 */

#define NANO_SEQUENCE_LENGTH_UNBOUNDED          0

NANO_RetCode
NANO_PrimitiveSequence_deserialize_cdr(
    NANO_Sequence *const self,
    NANO_CDR_Stream *stream,
    NANO_usize seq_max);

NANO_RetCode
NANO_PrimitiveSequence_serialize_cdr(
    NANO_Sequence *const self,
    NANO_CDR_Stream *stream,
    NANO_usize seq_max);

NANO_RetCode
NANO_PrimitiveSequence_serialized_size_cdr(
    NANO_Sequence *const self,
    NANO_CDR_Stream *stream,
    const NANO_usize seq_max);

#define NANO_PrimitiveSequence_deserialize_cdr(s_,sr_,max_) \
    NANO_CDR_Stream_deserialize_psequence((sr_),(s_),(max_))

#define NANO_PrimitiveSequence_serialize_cdr(s_,sr_,max_) \
    NANO_CDR_Stream_serialize_psequence((sr_),(s_),(max_))

#define NANO_PrimitiveSequence_serialized_size_cdr(s_,sr_,max_) \
    NANO_CDR_Stream_serialized_size_psequence((sr_),(s_),(max_))

#define NANO_PrimitiveSequence_alignment \
        NANO_u32_alignment

NANO_RetCode
NANO_Sequence_deserialize_cdr(
    NANO_Sequence *const self,
    NANO_CDR_Stream *stream,
    const NANO_usize seq_max,
    NANO_CDR_Stream_DeserializeCdrFn deserialize_member);

NANO_RetCode
NANO_Sequence_serialize_cdr(
    const NANO_Sequence *const self,
    NANO_CDR_Stream *stream,
    const NANO_usize seq_max,
    NANO_CDR_Stream_SerializeCdrFn serialize_member);

NANO_RetCode
NANO_Sequence_serialized_size_cdr(
    NANO_Sequence *const self,
    NANO_CDR_Stream *stream,
    const NANO_usize seq_max,
    NANO_CDR_Stream_SerializedSizeCdrFn serialized_size_member);

#define NANO_Sequence_deserialize_cdr(s_,sr_,max_,dm_) \
    NANO_CDR_Stream_deserialize_sequence((sr_),(s_),(max_),(dm_))

#define NANO_Sequence_serialize_cdr(s_,sr_,max_,sm_) \
    NANO_CDR_Stream_serialize_sequence((sr_),(s_),(max_),(sm_))

#define NANO_Sequence_serialized_size_cdr(s_,sr_,max_,ssm_) \
    NANO_CDR_Stream_serialized_size_sequence((sr_),(s_),(max_),(ssm_))

#define NANO_Sequence_alignment \
        NANO_u32_alignment

/*i
 * @brief Helper macro to implement typed version of
 * NANO_Sequence_deserialize_cdr.
 */
#define NANO_TSeqImpl_deserialize_cdr(s_,sr_,max_,dm_) \
    NANO_Sequence_deserialize_cdr(\
        &(s_)->base,\
        (sr_),\
        (max_),\
        (NANO_CDR_Stream_DeserializeCdrFn)(dm_))

/*i
 * @brief Helper macro to implement typed version of
 * NANO_Sequence_serialize_cdr.
 */
#define NANO_TSeqImpl_serialize_cdr(s_,sr_,max_,sm_) \
    NANO_Sequence_serialize_cdr(\
        &(s_)->base,\
        (sr_),\
        (max_),\
        (NANO_CDR_Stream_SerializeCdrFn)(sm_))

/*i
 * @brief Helper macro to implement typed version of
 * NANO_Sequence_serialized_size_cdr.
 */
#define NANO_TSeqImpl_serialized_size_cdr(s_,sr_,max_,ssm_) \
    NANO_Sequence_serialized_size_cdr(\
        &(s_)->base,\
        (sr_),\
        (max_),\
        (NANO_CDR_Stream_SerializedSizeCdrFn)(ssm_))

/*i
 * @brief Helper macro to implement typed version of
 * NANO_Sequence_deserialize_cdr for a primitive type.
 */
#define NANO_PTSeqImpl_deserialize_cdr(s_,sr_,max_) \
    NANO_PrimitiveSequence_deserialize_cdr(\
        &(s_)->base,\
        (sr_),\
        (max_))

/*i
 * @brief Helper macro to implement typed version of
 * NANO_Sequence_serialize_cdr for a primitive type.
 */
#define NANO_PTSeqImpl_serialize_cdr(s_,sr_,max_) \
    NANO_PrimitiveSequence_serialize_cdr(\
        &(s_)->base,\
        (sr_),\
        (max_))

/*i
 * @brief Helper macro to implement typed version of
 * NANO_Sequence_serialized_size_cdr for a primitive type.
 */
#define NANO_PTSeqImpl_serialized_size_cdr(s_,sr_,max_) \
    NANO_PrimitiveSequence_serialized_size_cdr(\
        &(s_)->base,\
        (sr_),\
        (max_))

/*i @} *//* nano_api_data_stream_sequence */

#endif /* NANO_FEAT_SEQUENCE */

#if NANO_FEAT_OPTIONAL
/******************************************************************************
 *                             OptionalMember
 ******************************************************************************/
/*i
 * @addtogroup nano_api_infr_opt
 * @{
 */

#define NANO_OPTIONALMEMBER_DESERIALIZE(s_,m_,sr_,dfn_,rc_) \
{\
    NANO_bool has_value_ = NANO_BOOL_FALSE;\
    (rc_) = NANO_bool_deserialize_cdr(&has_value_,(sr_));\
    if ((rc_) == NANO_RETCODE_OK)\
    {\
        if (has_value_)\
        {\
            (rc_) = dfn_(NANO_OPTIONALMEMBER_AS_REF(s_,m_),(sr_));\
        }\
        if ((rc_) == NANO_RETCODE_OK)\
        {\
            NANO_OPTIONALMEMBER_MARK_SET(s_,m_,has_value_);\
        }\
    }\
}

#define NANO_OPTIONALMEMBER_SERIALIZE(s_,m_,sr_,sfn_,rc_) \
{\
    NANO_bool has_value_ = NANO_OPTIONALMEMBER_IS_SET(s_,m_);\
    (rc_) = NANO_bool_serialize_cdr(&has_value_,(sr_));\
    if ((rc_) == NANO_RETCODE_OK)\
    {\
        if (has_value_)\
        {\
            (rc_) = sfn_(NANO_OPTIONALMEMBER_AS_REF(s_,m_),(sr_));\
        }\
    }\
}

#define NANO_OPTIONALMEMBER_SERIALIZED_SIZE(s_,m_,sr_,ssf_) \
(\
    NANO_bool_serialized_size_cdr(NANO_MEMBER(s_,xrce_str_concat(has_,m_)),(sr_)) + \
    ((NANO_OPTIONALMEMBER_IS_SET(s_,m_))? \
        ssf_(NANO_OPTIONALMEMBER_AS_REF(s_,m_),(sr_)) : 0)\
)

#define NANO_OPTIONALMEMBER_SERIALIZED_SIZE_MAX(sr_,ssf_) \
(\
    NANO_bool_serialized_size_cdr(NULL,(sr_)) + \
    ssf_(NULL,(sr_))\
)

/*i @} *//* nano_api_infr_opt */

#endif /* NANO_FEAT_OPTIONAL */


/******************************************************************************
 *                          Property && PropertySeq
 ******************************************************************************/
#if NANO_FEAT_PROPERTY
/*i
 * @addtogroup nano_api_infr_property
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_Property)

#if NANO_FEAT_TYPED_SEQUENCE
NANO_CDR_STREAM_INTF_DECL(NANO_PropertySeq)

#define NANO_PropertySeq_deserialize_cdr(s_,sr_) \
    NANO_TSeqImpl_deserialize_cdr((s_),(sr_),\
        NANO_SEQUENCE_LENGTH_UNBOUNDED,\
        NANO_Property_deserialize_cdr)

#define NANO_PropertySeq_serialize_cdr(s_,sr_) \
    NANO_TSeqImpl_serialize_cdr((s_),(sr_),\
        NANO_SEQUENCE_LENGTH_UNBOUNDED,\
        NANO_Property_serialize_cdr)

#define NANO_PropertySeq_serialized_size_cdr(s_,sr_) \
    NANO_TSeqImpl_serialized_size_cdr((s_),(sr_),\
        NANO_SEQUENCE_LENGTH_UNBOUNDED,\
        NANO_Property_serialized_size_cdr)
#endif /* NANO_FEAT_TYPED_SEQUENCE */

/*i @} *//* nano_api_infr_property */

#endif /* NANO_FEAT_PROPERTY */

#endif /* NANO_FEAT_CDR */


#endif /* nano_core_cdr_basic_h */
