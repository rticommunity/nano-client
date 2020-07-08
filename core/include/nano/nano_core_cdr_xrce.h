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

#ifndef nano_core_cdr_xrce_h
#define nano_core_cdr_xrce_h


/******************************************************************************
 *                                  SeqNum
 ******************************************************************************/

/**
 * @addtogroup nano_api_xrcesupport
 * @{
 */

/*e
 * @brief Set a sequence number from a 16-bit unsigned integer in native
 * endianness.
 * 
 * @param self The sequence number to set.
 * @param val The 16-bit integer from which to set the sequence number.
 */
void
NANO_XRCE_SeqNum_from_u16(NANO_XRCE_SeqNum *const self, const NANO_u16 val);

/*e
 * @brief Convert a sequence number into a 16-bit unsigned integer.
 * 
 * @param self The sequence number to convert.
 * @return A NANO_u16 representing the sequence number in native endianness.
 */
NANO_u16
NANO_XRCE_SeqNum_to_u16(const NANO_XRCE_SeqNum *const self);


#define NANO_XRCE_SeqNum_to_u16(s_) \
    NANO_u16_from_ptr_le((s_)->value)


#define NANO_XRCE_SeqNum_from_u16(s_,v_) \
    NANO_u16_set_ptr_le((s_)->value,(v_))


NANO_i32
NANO_XRCE_SeqNum_distance_from(
    const NANO_XRCE_SeqNum *const self,
    const NANO_XRCE_SeqNum *const other);

/** @} *//* nano_api_client_req */


/**
 * @addtogroup nano_api_xrcesupport
 * @{
 */
/*e
 * @brief Set a client key from a NANO_u32 in native endianness.
 * 
 * @param self The key to set.
 * @param val The value to set the key from.
 */
void
NANO_XRCE_ClientKey_from_u32(
    NANO_XRCE_ClientKey *const self,
    const NANO_u32 val);

/*e
 * @brief Transform a client key into a NANO_u32 in native endianness.
 * 
 * @param self The key to transform.
 * @return A NANO_u32 representing the value of the client key.
 */
NANO_u32
NANO_XRCE_ClientKey_to_u32(NANO_XRCE_ClientKey *const self);

#define NANO_XRCE_ClientKey_to_u32(s_) \
    NANO_u32_from_ptr_be((s_)->value)

#define NANO_XRCE_ClientKey_from_u32(s_,v_) \
    NANO_u32_set_ptr_be((s_)->value, (v_))

/** @} *//* nano_api_xrcesupport */

#define NANO_XRCE_ObjectId_alignment \
    NANO_Octets_alignment


#if NANO_FEAT_RELIABILITY
/*i
 * @addtogroup nano_api_xrce_payload_support_acknackbmap
 * @{
 */
NANO_u16
NANO_XRCE_AckNackBitmap_to_u16(
    const NANO_XRCE_AckNackBitmap *const self);

#define NANO_XRCE_AckNackBitmap_to_u16(s_) \
    NANO_u16_from_ptr_le((s_)->value)

void
NANO_XRCE_AckNackBitmap_from_u16(
    NANO_XRCE_AckNackBitmap *const self,
    const NANO_u16 val);

#define NANO_XRCE_AckNackBitmap_from_u16(s_,v_) \
    NANO_u16_set_ptr_le((s_)->value,(v_))

/*i @} *//* nano_api_xrce_payload_support_acknackbmap */

#endif /* NANO_FEAT_RELIABILITY */

/**
 * @addtogroup nano_api_clientrequests
 * @{
 */

/*e
 * @brief 
 * 
 * @param self 
 * @return NANO_u16 
 */
NANO_u16
NANO_XRCE_RequestId_to_u16(const NANO_XRCE_RequestId *const self);

#define NANO_XRCE_RequestId_to_u16(s_) \
    NANO_u16_from_ptr_le((s_)->value)

/*e
 * @brief 
 * 
 * @param self 
 * @param val 
 */
void
NANO_XRCE_RequestId_from_u16(NANO_XRCE_RequestId *const self, NANO_u16 val);

#define NANO_XRCE_RequestId_from_u16(s_,v_) \
    NANO_u16_set_ptr_le((s_)->value,(v_))

/** @} *//* nano_api_clientrequests */

#if NANO_FEAT_CDR

#if NANO_FEAT_RESULTSTATUS
/******************************************************************************
 *                              StatusValue
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrcesupport
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_StatusValue)

#define NANO_XRCE_StatusValue_deserialize_cdr(s_,sr_) \
    NANO_u8_deserialize_cdr(s_,sr_)

#define NANO_XRCE_StatusValue_serialize_cdr(s_,sr_) \
    NANO_u8_serialize_cdr(s_,sr_)

#define NANO_XRCE_StatusValue_serialized_size_cdr(s_,sr_) \
    NANO_u8_serialized_size_cdr(s_,sr_)

#define NANO_XRCE_StatusValue_alignment \
    NANO_u8_alignment

/*i @} *//* nano_api_client_req */

/******************************************************************************
 *                              ResultStatus
 ******************************************************************************/

/*i
 * @addtogroup nano_api_clientrequests
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_ResultStatus)

#define NANO_ResultStatus_serialized_size_cdr(s_,sr_)   \
(\
    NANO_XRCE_StatusValue_serialized_size_cdr(NANO_MEMBER((s_),status),(sr_)) + \
    NANO_u8_serialized_size_cdr(NANO_MEMBER((s_),implementation_status),(sr_)) \
)

#define NANO_ResultStatus_alignment \
    NANO_XRCE_StatusValue_alignment

/*i @} *//* nano_api_client_req */

#endif /* NANO_FEAT_RESULTSTATUS */

/******************************************************************************
 *                              Cookie
 ******************************************************************************/

/*i 
 * @addtogroup nano_api_xrce_header
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_Cookie)

#define NANO_XRCE_Cookie_deserialize_cdr(s_,sr_) \
    NANO_Octets_deserialize_cdr(\
        NANO_MEMBER((s_),value),\
        (sr_),\
        NANO_XRCE_COOKIE_VALUE_LEN)

#define NANO_XRCE_Cookie_serialize_cdr(s_,sr_) \
    NANO_Octets_serialize_cdr(\
        NANO_MEMBER((s_),value),\
        (sr_),\
        NANO_XRCE_COOKIE_VALUE_LEN)

#define NANO_XRCE_Cookie_serialized_size_cdr(s_,sr_) \
    NANO_Octets_serialized_size_cdr(\
        NANO_MEMBER((s_),value),\
        (sr_),\
        NANO_XRCE_COOKIE_VALUE_LEN)

#define NANO_XRCE_Cookie_alignment \
    NANO_Octets_alignment


NANO_u32
NANO_XRCE_Cookie_to_u32(NANO_XRCE_Cookie *const self);

#define NANO_XRCE_Cookie_to_u32(s_) \
    NANO_u32_from_ptr_be((s_)->value)

void
NANO_XRCE_Cookie_from_u32(NANO_XRCE_Cookie *const self, const NANO_u32 val);

#define NANO_XRCE_Cookie_from_u32(s_,v_) \
    NANO_u32_set_ptr_be((s_)->value,(v_))

/*i @} *//* nano_api_xrce_header */

/******************************************************************************
 *                              Version
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_header
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_Version)

#define NANO_XRCE_Version_deserialize_cdr(s_,sr_) \
    NANO_Octets_deserialize_cdr(\
        NANO_MEMBER((s_),value),\
        (sr_),\
        NANO_XRCE_VERSION_VALUE_LEN)

#define NANO_XRCE_Version_serialize_cdr(s_,sr_) \
    NANO_Octets_serialize_cdr(\
        NANO_MEMBER((s_),value),\
        (sr_),\
        NANO_XRCE_VERSION_VALUE_LEN)

#define NANO_XRCE_Version_serialized_size_cdr(s_,sr_) \
    NANO_Octets_serialized_size_cdr(\
        NANO_MEMBER((s_),value),\
        (sr_),\
        NANO_XRCE_VERSION_VALUE_LEN)

#define NANO_XRCE_Version_alignment \
    NANO_Octets_alignment


NANO_u16
NANO_XRCE_Version_to_u16(NANO_XRCE_Version *const self);

#define NANO_XRCE_Version_to_u16(s_) \
    NANO_CDR_u16_from_ptr_be((s_)->value)

void
NANO_XRCE_Version_from_u16(NANO_XRCE_Version *const self, const NANO_u16 val);

#define NANO_XRCE_Version_from_u16(s_,v_) \
    NANO_CDR_u16_set_ptr_be((s_)->value, (v_))

/*i @} *//* nano_api_xrce_header */

/******************************************************************************
 *                              VendorId
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_header
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_VendorId)

#define NANO_XRCE_VendorId_deserialize_cdr(s_,sr_) \
    NANO_Octets_deserialize_cdr(\
        NANO_MEMBER((s_),value),\
        (sr_),\
        NANO_XRCE_VENDORID_VALUE_LEN)

#define NANO_XRCE_VendorId_serialize_cdr(s_,sr_) \
    NANO_Octets_serialize_cdr(\
        NANO_MEMBER((s_),value),\
        (sr_),\
        NANO_XRCE_VENDORID_VALUE_LEN)

#define NANO_XRCE_VendorId_serialized_size_cdr(s_,sr_) \
    NANO_Octets_serialized_size_cdr(\
        NANO_MEMBER((s_),value),\
        (sr_),\
        NANO_XRCE_VENDORID_VALUE_LEN)

#define NANO_XRCE_VendorId_alignment \
    NANO_Octets_alignment


NANO_u16
NANO_XRCE_VendorId_to_u16(NANO_XRCE_VendorId *const self);

#define NANO_XRCE_VendorId_to_u16(s_) \
    NANO_CDR_u16_set_ptr_be((s_)->value, (v_))

void
NANO_XRCE_VendorId_from_u16(NANO_XRCE_VendorId *const self, const NANO_u16 val);

#define NANO_XRCE_VendorId_from_u16(s_,v_) \
    NANO_CDR_u16_from_ptr_be((s_)->value)


/*i @} *//* nano_api_xrce_header */

#if NANO_FEAT_OBJECT
/******************************************************************************
 *                              ObjectKind
 ******************************************************************************/

/*i
 * @addtogroup nano_api_client_obj
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_ObjectKind)

#define NANO_XRCE_ObjectKind_deserialize_cdr(s_,sr_) \
    NANO_u8_deserialize_cdr(s_,sr_)

#define NANO_XRCE_ObjectKind_serialize_cdr(s_,sr_) \
    NANO_u8_serialize_cdr(s_,sr_)

#define NANO_XRCE_ObjectKind_serialized_size_cdr(s_,sr_) \
    NANO_u8_serialized_size_cdr(s_,sr_)

#define NANO_XRCE_ObjectKind_alignment \
    NANO_u8_alignment

/*i @} *//* nano_api_client_obj */

/******************************************************************************
 *                                  ObjectId
 ******************************************************************************/

/*i
 * @addtogroup nano_api_client_obj
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_ObjectId)

#define NANO_XRCE_ObjectId_deserialize_cdr(s_,sr_) \
    NANO_Octets_deserialize_cdr(\
        NANO_MEMBER((s_),value),\
        (sr_),\
        NANO_XRCE_OBJECTID_VALUE_LEN)

#define NANO_XRCE_ObjectId_serialize_cdr(s_,sr_) \
    NANO_Octets_serialize_cdr(\
        NANO_MEMBER((s_),value),\
        (sr_),\
        NANO_XRCE_OBJECTID_VALUE_LEN)

#define NANO_XRCE_ObjectId_serialized_size_cdr(s_,sr_) \
    NANO_Octets_serialized_size_cdr(\
        NANO_MEMBER((s_),value),\
        (sr_),\
        NANO_XRCE_OBJECTID_VALUE_LEN)

/*i @} *//* nano_api_client_obj */

#endif /* NANO_FEAT_OBJECT */

/******************************************************************************
 *                                 SessionId
 ******************************************************************************/

/*i
 * @addtogroup nano_api_client_client
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_SessionId)

#define NANO_XRCE_SessionId_deserialize_cdr(s_,sr_) \
    NANO_u8_deserialize_cdr(s_,sr_)

#define NANO_XRCE_SessionId_serialize_cdr(s_,sr_) \
    NANO_u8_serialize_cdr(s_,sr_)

#define NANO_XRCE_SessionId_serialized_size_cdr(s_,sr_) \
    NANO_u8_serialized_size_cdr(s_,sr_)

#define NANO_XRCE_SessionId_alignment \
    NANO_u8_alignment


/*i @} *//* nano_api_client_client */

/******************************************************************************
 *                                  StreamId
 ******************************************************************************/

/*i
 * @addtogroup nano_api_client_req
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_StreamId)

#define NANO_XRCE_StreamId_deserialize_cdr(s_,sr_) \
    NANO_u8_deserialize_cdr(s_,sr_)

#define NANO_XRCE_StreamId_serialize_cdr(s_,sr_) \
    NANO_u8_serialize_cdr(s_,sr_)

#define NANO_XRCE_StreamId_serialized_size_cdr(s_,sr_) \
    NANO_u8_serialized_size_cdr(s_,sr_)

#define NANO_XRCE_StreamId_alignment \
    NANO_u8_alignment

/*i @} *//* nano_api_client_req */

/******************************************************************************
 *                                  SeqNum
 ******************************************************************************/

/*i
 * @addtogroup nano_api_client_req
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_SeqNum)

#define NANO_XRCE_SeqNum_deserialize_cdr(s_,sr_) \
    NANO_Octets_deserialize_cdr(\
        NANO_MEMBER((s_),value),\
        (sr_),\
        NANO_XRCE_SEQNUM_VALUE_LEN)

#define NANO_XRCE_SeqNum_serialize_cdr(s_,sr_) \
    NANO_Octets_serialize_cdr(\
        NANO_MEMBER((s_),value),\
        (sr_),\
        NANO_XRCE_SEQNUM_VALUE_LEN)

#define NANO_XRCE_SeqNum_serialized_size_cdr(s_,sr_) \
    NANO_Octets_serialized_size_cdr(\
        NANO_MEMBER((s_),value),\
        (sr_),\
        NANO_XRCE_SEQNUM_VALUE_LEN)

#define NANO_XRCE_SeqNum_alignment \
    NANO_Octets_alignment

/*i @} *//* nano_api_client_req */

/******************************************************************************
 *                                ClientKey
 ******************************************************************************/
/*i
 * @addtogroup nano_api_client_client
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_ClientKey)

#define NANO_XRCE_ClientKey_deserialize_cdr(s_,sr_) \
    NANO_Octets_deserialize_cdr(\
        NANO_MEMBER((s_),value),\
        (sr_),\
        NANO_XRCE_CLIENTKEY_VALUE_LEN)

#define NANO_XRCE_ClientKey_serialize_cdr(s_,sr_) \
    NANO_Octets_serialize_cdr(\
        NANO_MEMBER((s_),value),\
        (sr_),\
        NANO_XRCE_CLIENTKEY_VALUE_LEN)

#define NANO_XRCE_ClientKey_serialized_size_cdr(s_,sr_) \
    NANO_Octets_serialized_size_cdr(\
        NANO_MEMBER((s_),value),\
        (sr_),\
        NANO_XRCE_CLIENTKEY_VALUE_LEN)

#define NANO_XRCE_ClientKey_alignment \
    NANO_Octets_alignment

/*i @} *//* nano_api_client_client */

#if NANO_FEAT_OBJECT
/******************************************************************************
 *                                ObjectRef
 ******************************************************************************/

/*i
 * @addtogroup nano_api_client_obj
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_ObjectRef)

#define NANO_XRCE_ObjectRef_deserialize_cdr(s_,sr_) \
    NANO_String_deserialize_cdr_max(\
        NANO_MEMBER((s_),value),\
        (sr_),\
        NANO_LIMIT_OBJECTREF_MAX_LENGTH)

#define NANO_XRCE_ObjectRef_serialize_cdr(s_,sr_) \
    NANO_String_serialize_cdr_max(\
        NANO_MEMBER((s_),value),\
        (sr_),\
        NANO_LIMIT_OBJECTREF_MAX_LENGTH)

#define NANO_XRCE_ObjectRef_serialized_size_cdr(s_,sr_) \
    NANO_String_serialized_size_cdr_max(\
        NANO_MEMBER((s_),value),\
        (sr_),\
        NANO_LIMIT_OBJECTREF_MAX_LENGTH)

#define NANO_XRCE_ObjectRef_alignment \
    NANO_String_alignment

/*i @} *//* nano_api_client_obj */

/******************************************************************************
 *                          RepresentationFormat
 ******************************************************************************/

/*i
 * @addtogroup nano_api_client_obj
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_RepresentationFormat)

#define NANO_XRCE_RepresentationFormat_deserialize_cdr(s_,sr_) \
    NANO_u8_deserialize_cdr(s_,sr_)

#define NANO_XRCE_RepresentationFormat_serialize_cdr(s_,sr_) \
    NANO_u8_serialize_cdr(s_,sr_)

#define NANO_XRCE_RepresentationFormat_serialized_size_cdr(s_,sr_) \
    NANO_u8_serialized_size_cdr(s_,sr_)

#define NANO_XRCE_RepresentationFormat_alignment \
    NANO_u8_alignment

/*i @} *//* nano_api_client_obj */

/******************************************************************************
 *                      RefRepresentationFormat
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_objrepr_common
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_RefRepresentationFormat)

#define NANO_XRCE_RefRepresentationFormat_deserialize_cdr(s_,sr_) \
    NANO_XRCE_ObjectRef_deserialize_cdr((s_), (sr_))

#define NANO_XRCE_RefRepresentationFormat_serialize_cdr(s_,sr_) \
    NANO_XRCE_ObjectRef_serialize_cdr((s_), (sr_))

#define NANO_XRCE_RefRepresentationFormat_serialized_size_cdr(s_,sr_) \
    NANO_XRCE_ObjectRef_serialized_size_cdr((s_), (sr_))

#define NANO_XRCE_RefRepresentationFormat_alignment \
    NANO_XRCE_ObjectRef_alignment

/*i @} *//* nano_api_xrce_payload_objrepr_common */

/******************************************************************************
 *                      XmlRepresentationFormat
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_objrepr_common
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_XmlRepresentationFormat)

#define NANO_XRCE_XmlRepresentationFormat_deserialize_cdr(s_,sr_) \
    NANO_String_deserialize_cdr_max(\
        NANO_MEMBER((s_),value), (sr_), NANO_STRING_LENGTH_UNBOUNDED)

#define NANO_XRCE_XmlRepresentationFormat_serialize_cdr(s_,sr_) \
    NANO_String_serialize_cdr_max(\
        NANO_MEMBER((s_),value), (sr_), NANO_STRING_LENGTH_UNBOUNDED)

#define NANO_XRCE_XmlRepresentationFormat_serialized_size_cdr(s_,sr_) \
    NANO_String_serialized_size_cdr_max(\
        NANO_MEMBER((s_),value), (sr_), NANO_STRING_LENGTH_UNBOUNDED)

#define NANO_XRCE_XmlRepresentationFormat_alignment \
    NANO_String_alignment

/*i @} *//* nano_api_xrce_payload_objrepr_common */

/******************************************************************************
 *                      BinRepresentationFormat
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_objrepr_common
 * @{
 */

#if NANO_FEAT_TYPED_SEQUENCE

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_BinData)

#define NANO_XRCE_BinData_deserialize_cdr(s_,sr_) \
    NANO_PTSeqImpl_deserialize_cdr((s_),(sr_),\
        NANO_SEQUENCE_LENGTH_UNBOUNDED)

#define NANO_XRCE_BinData_serialize_cdr(s_,sr_) \
    NANO_PTSeqImpl_serialize_cdr((s_),(sr_),\
        NANO_SEQUENCE_LENGTH_UNBOUNDED)

#define NANO_XRCE_BinData_serialized_size_cdr(s_,sr_) \
    NANO_PTSeqImpl_serialized_size_cdr((s_),\
        NANO_SEQUENCE_LENGTH_UNBOUNDED)
#define NANO_XRCE_BinData_alignment \
    NANO_PrimitiveSequence_alignment

#endif /* NANO_FEAT_TYPED_SEQUENCE */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_BinRepresentationFormat)

#if NANO_FEAT_TYPED_SEQUENCE

#define NANO_XRCE_BinRepresentationFormat_deserialize_cdr(s_,sr_) \
    NANO_XRCE_BinData_deserialize_cdr((s_),(sr_))

#define NANO_XRCE_BinRepresentationFormat_serialize_cdr(s_,sr_) \
    NANO_XRCE_BinData_serialize_cdr((s_),(sr_))

#define NANO_XRCE_BinRepresentationFormat_serialized_size_cdr(s_,sr_) \
    NANO_XRCE_BinData_serialized_size_cdr((s_),(sr_))

#define NANO_XRCE_BinRepresentationFormat_alignment \
    NANO_XRCE_BinData_alignment

#else

#define NANO_XRCE_BinRepresentationFormat_deserialize_cdr(s_,sr_)
    
#define NANO_XRCE_BinRepresentationFormat_serialize_cdr(s_,sr_)

#define NANO_XRCE_BinRepresentationFormat_serialized_size_cdr(s_,sr_)

#define NANO_XRCE_BinRepresentationFormat_alignment

#endif /* NANO_FEAT_TYPED_SEQUENCE */

/*i @} *//* nano_api_xrce_payload_objrepr_common */

/******************************************************************************
 *                          RepresentationFormats
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_objrepr_common
 * @{
 */

NANODllExport
NANO_RetCode
NANO_XRCE_RepresentationFormats_serialize_cdr(
    const NANO_XRCE_RepresentationFormat *const format,
    const NANO_XRCE_RefRepresentationFormat *const ref,
    const NANO_XRCE_XmlRepresentationFormat *const xml,
    const NANO_XRCE_BinRepresentationFormat *const bin,
    NANO_CDR_Stream *const stream);

NANODllExport
NANO_RetCode
NANO_XRCE_RepresentationFormats_deserialize_cdr(
    NANO_XRCE_RepresentationFormat *const format,
    NANO_XRCE_RefRepresentationFormat *const ref,
    NANO_XRCE_XmlRepresentationFormat *const xml,
    NANO_XRCE_BinRepresentationFormat *const bin,
    NANO_CDR_Stream *const stream);

/******************************************************************************
 *                          Representation3Formats
 ******************************************************************************/

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_Representation3Formats)

#define NANO_XRCE_Representation3Formats_deserialize_cdr(s_,sr_) \
    NANO_XRCE_RepresentationFormats_deserialize_cdr(\
        NANO_MEMBER((s_),format),\
        NANO_MEMBER((s_),value.ref),\
        NANO_MEMBER((s_),value.xml),\
        NANO_MEMBER((s_),value.bin),\
        (sr_))

#define NANO_XRCE_Representation3Formats_serialize_cdr(s_,sr_) \
    NANO_XRCE_RepresentationFormats_serialize_cdr(\
        NANO_MEMBER((s_),format),\
        NANO_MEMBER((s_),value.ref),\
        NANO_MEMBER((s_),value.xml),\
        NANO_MEMBER((s_),value.bin),\
        (sr_))

#define NANO_XRCE_Representation3Formats_serialized_size_cdr(s_,sr_) \
(\
    NANO_XRCE_RepresentationFormat_serialized_size_cdr(\
            NANO_MEMBER((s_),format),(sr_)) + \
    (((s_) == NULL)? \
        ((NANO_XRCE_OBJECTREF_MAX_LENGTH > NANO_MEM_BINREF_MAX_LENGTH &&\
            NANO_XRCE_OBJECTREF_MAX_LENGTH > NANO_MEM_XMLREF_MAX_LENGTH)?\
            NANO_XRCE_RefRepresentationFormat_serialized_size_cdr(NULL,(sr_)) :\
            ((NANO_MEM_XMLREF_MAX_LENGTH > NANO_MEM_BINREF_MAX_LENGTH &&\
                NANO_MEM_XMLREF_MAX_LENGTH > NANO_XRCE_OBJECTREF_MAX_LENGTH)?\
                NANO_XRCE_XmlRepresentationFormat_serialized_size_cdr(NULL,(sr_)) : \
                NANO_XRCE_BinRepresentationFormat_serialized_size_cdr(NULL,(sr_)))) \
        : \
    (((s_)->format == NANO_XRCE_REPRESENTATION_BY_REFERENCE)?\
        NANO_XRCE_RefRepresentationFormat_serialized_size_cdr(&(s_)->value.ref,(sr_)) : \
        ((s_)->format == NANO_XRCE_REPRESENTATION_AS_XML_STRING)? \
            NANO_XRCE_XmlRepresentationFormat_serialized_size_cdr(&(s_)->value.xml,(sr_)) : \
            ((s_)->format == NANO_XRCE_REPRESENTATION_IN_BINARY)? \
                NANO_XRCE_BinRepresentationFormat_serialized_size_cdr(&(s_)->value.bin,(sr_)) : \
                0) \
    )\
)

#define NANO_XRCE_Representation3Formats_alignment \
    NANO_XRCE_RepresentationFormat_alignment

/******************************************************************************
 *                              Representation3
 ******************************************************************************/

#define NANO_XRCE_Representation3_as_bin_mut(s_) \
    NANO_XRCE_Representation3Formats_as_bin_mut(&(s_)->repr)

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_Representation3)

#define NANO_XRCE_Representation3_deserialize_cdr(s_,sr_) \
    NANO_XRCE_Representation3Formats_deserialize_cdr(\
        NANO_MEMBER((s_),repr),(sr_))

#define NANO_XRCE_Representation3_serialize_cdr(s_,sr_) \
    NANO_XRCE_Representation3Formats_serialize_cdr(\
        NANO_MEMBER((s_),repr),(sr_))

#define NANO_XRCE_Representation3_serialized_size_cdr(s_,sr_) \
    NANO_XRCE_Representation3Formats_serialized_size_cdr(\
        NANO_MEMBER((s_),repr),(sr_))

#define NANO_XRCE_Representation3_alignment \
    NANO_XRCE_Representation3Formats_alignment


/*i @} *//* nano_api_xrce_payload_objrepr_common */

/******************************************************************************
 *                      RepresentationRefAndXmlFormats
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_objrepr_common
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_RepresentationRefAndXmlFormats)


#define NANO_XRCE_RepresentationRefAndXmlFormats_deserialize_cdr(s_,sr_) \
    NANO_XRCE_RepresentationFormats_deserialize_cdr(\
        NANO_MEMBER((s_),format),\
        NANO_MEMBER((s_),value.ref),\
        NANO_MEMBER((s_),value.xml),\
        NULL,\
        (sr_))

#define NANO_XRCE_RepresentationRefAndXmlFormats_serialize_cdr(s_,sr_) \
    NANO_XRCE_RepresentationFormats_serialize_cdr(\
        NANO_MEMBER((s_),format),\
        NANO_MEMBER((s_),value.ref),\
        NANO_MEMBER((s_),value.xml),\
        NULL,\
        (sr_))

#define NANO_XRCE_RepresentationRefAndXmlFormats_serialized_size_cdr(s_,sr_) \
(\
    NANO_XRCE_RepresentationFormat_serialized_size_cdr(\
            NANO_MEMBER((s_),format),(sr_)) + \
    (((s_) == NULL)? \
        ((NANO_XRCE_OBJECTREF_MAX_LENGTH > NANO_MEM_XMLREF_MAX_LENGTH)?\
            NANO_XRCE_RefRepresentationFormat_serialized_size_cdr(NULL,(sr_)) :\
            NANO_XRCE_XmlRepresentationFormat_serialized_size_cdr(NULL,(sr_))) \
        : \
    (((s_)->format == NANO_XRCE_REPRESENTATION_BY_REFERENCE)?\
        NANO_XRCE_RefRepresentationFormat_serialized_size_cdr(&(s_)->value.ref,(sr_)) : \
        ((s_)->format == NANO_XRCE_REPRESENTATION_AS_XML_STRING)? \
            NANO_XRCE_XmlRepresentationFormat_serialized_size_cdr(&(s_)->value.xml,(sr_)) : \
            0) \
    )\
)

#define NANO_XRCE_RepresentationRefAndXmlFormats_alignment \
    NANO_XRCE_RepresentationFormat_alignment

/******************************************************************************
 *                          RepresentationRefAndXml
 ******************************************************************************/

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_RepresentationRefAndXml)

#define NANO_XRCE_RepresentationRefAndXml_deserialize_cdr(s_,sr_) \
    NANO_XRCE_RepresentationRefAndXmlFormats_deserialize_cdr(\
        NANO_MEMBER((s_),repr),(sr_))

#define NANO_XRCE_RepresentationRefAndXml_serialize_cdr(s_,sr_) \
    NANO_XRCE_RepresentationRefAndXmlFormats_serialize_cdr(\
        NANO_MEMBER((s_),repr),(sr_))

#define NANO_XRCE_RepresentationRefAndXml_serialized_size_cdr(s_,sr_) \
    NANO_XRCE_RepresentationRefAndXmlFormats_serialized_size_cdr(\
        NANO_MEMBER((s_),repr),(sr_))

#define NANO_XRCE_RepresentationRefAndXml_alignment \
    NANO_XRCE_RepresentationRefAndXmlFormats_alignment

/*i @} *//* nano_api_xrce_payload_objrepr_common */

/******************************************************************************
 *                      RepresentationBinAndXmlFormats
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_objrepr_common
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_RepresentationBinAndXmlFormats)

#if NANO_FEAT_ALL_BY_REF

#define NANO_XRCE_RepresentationBinAndXmlFormats_deserialize_cdr(s_,sr_) \
    NANO_XRCE_RepresentationFormats_deserialize_cdr(\
        NANO_MEMBER((s_),format),\
        NANO_MEMBER((s_),value.ref),\
        NANO_MEMBER((s_),value.xml),\
        NANO_MEMBER((s_),value.bin),\
        (sr_))

#define NANO_XRCE_RepresentationBinAndXmlFormats_serialize_cdr(s_,sr_) \
    NANO_XRCE_RepresentationFormats_serialize_cdr(\
        NANO_MEMBER((s_),format),\
        NANO_MEMBER((s_),value.ref),\
        NANO_MEMBER((s_),value.xml),\
        NANO_MEMBER((s_),value.bin),\
        (sr_))


#else
#define NANO_XRCE_RepresentationBinAndXmlFormats_deserialize_cdr(s_,sr_) \
    NANO_XRCE_RepresentationFormats_deserialize_cdr(\
        NANO_MEMBER((s_),format),\
        NULL,\
        NANO_MEMBER((s_),value.xml),\
        NANO_MEMBER((s_),value.bin),\
        (sr_))

#define NANO_XRCE_RepresentationBinAndXmlFormats_serialize_cdr(s_,sr_) \
    NANO_XRCE_RepresentationFormats_serialize_cdr(\
        NANO_MEMBER((s_),format),\
        NULL,\
        NANO_MEMBER((s_),value.xml),\
        NANO_MEMBER((s_),value.bin),\
        (sr_))

#endif /* NANO_FEAT_ALL_BY_REF */

/* TODO differantiate this implementation based on NANO_FEAT_ALL_BY_REF */
#define NANO_XRCE_RepresentationBinAndXmlFormats_serialized_size_cdr(s_,sr_) \
(\
    NANO_XRCE_RepresentationFormat_serialized_size_cdr(\
            NANO_MEMBER((s_),format),(sr_)) + \
    (((s_) == NULL)? \
        ((NANO_XRCE_BINREF_MAX_LENGTH > NANO_MEM_XMLREF_MAX_LENGTH)?\
            NANO_XRCE_BinRepresentationFormat_serialized_size_cdr(NULL,(sr_)) :\
            NANO_XRCE_XmlRepresentationFormat_serialized_size_cdr(NULL,(sr_))) \
        : \
    (((s_)->format == NANO_XRCE_REPRESENTATION_IN_BINARY)?\
        NANO_XRCE_BinRepresentationFormat_serialized_size_cdr(&(s_)->value.bin,(sr_)) : \
        ((s_)->format == NANO_XRCE_REPRESENTATION_AS_XML_STRING)? \
            NANO_XRCE_XmlRepresentationFormat_serialized_size_cdr(&(s_)->value.xml,(sr_)) : \
            0) \
    )\
)

#define NANO_XRCE_RepresentationBinAndXmlFormats_alignment \
    NANO_XRCE_RepresentationFormat_alignment

/******************************************************************************
 *                        RepresentationBinAndXml
 ******************************************************************************/

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_RepresentationBinAndXml)

#define NANO_XRCE_RepresentationBinAndXml_deserialize_cdr(s_,sr_) \
    NANO_XRCE_RepresentationBinAndXmlFormats_deserialize_cdr(\
        NANO_MEMBER((s_),repr),(sr_))

#define NANO_XRCE_RepresentationBinAndXml_serialize_cdr(s_,sr_) \
    NANO_XRCE_RepresentationBinAndXmlFormats_serialize_cdr(\
        NANO_MEMBER((s_),repr),(sr_))

#define NANO_XRCE_RepresentationBinAndXml_serialized_size_cdr(s_,sr_) \
    NANO_XRCE_RepresentationBinAndXmlFormats_serialized_size_cdr(\
        NANO_MEMBER((s_),repr),(sr_))

#define NANO_XRCE_RepresentationBinAndXml_alignment \
    NANO_XRCE_RepresentationBinAndXmlFormats_alignment

#endif /* NANO_FEAT_OBJECT */

#if NANO_FEAT_OBJECT_KIND_QOSPROFILE
/******************************************************************************
 *                          QosProfileRepresentation
 ******************************************************************************/

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_QosProfileRepresentation)

#define NANO_XRCE_QosProfileRepresentation_deserialize_cdr(s_,sr_) \
    NANO_XRCE_RepresentationRefAndXml_deserialize_cdr(NANO_MEMBER((s_),base),(sr_))

#define NANO_XRCE_QosProfileRepresentation_serialize_cdr(s_,sr_) \
    NANO_XRCE_RepresentationRefAndXml_serialize_cdr(NANO_MEMBER((s_),base),(sr_))

#define NANO_XRCE_QosProfileRepresentation_serialized_size_cdr(s_,sr_) \
    NANO_XRCE_RepresentationRefAndXml_serialized_size_cdr(NANO_MEMBER((s_),base),(sr_))

#endif /* NANO_FEAT_OBJECT_KIND_QOSPROFILE */

/*i @} *//* nano_api_xrce_payload_objrepr_common */

#if NANO_FEAT_OBJECT_KIND_TYPE
/******************************************************************************
 *                            TypeRepresentation
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_objrepr_type
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_TypeRepresentation)

#define NANO_XRCE_TypeRepresentation_deserialize_cdr(s_,sr_) \
    NANO_XRCE_RepresentationRefAndXml_deserialize_cdr(\
        NANO_MEMBER((s_),base),(sr_))

#define NANO_XRCE_TypeRepresentation_serialize_cdr(s_,sr_) \
    NANO_XRCE_RepresentationRefAndXml_serialize_cdr(\
        NANO_MEMBER((s_),base),(sr_))

#define NANO_XRCE_TypeRepresentation_serialized_size_cdr(s_,sr_) \
    NANO_XRCE_RepresentationRefAndXml_serialized_size_cdr(\
        NANO_MEMBER((s_),base),(sr_))

/*i @} *//* nano_api_xrce_payload_objrepr_type */

#endif /* NANO_FEAT_OBJECT_KIND_TYPE */

#if NANO_FEAT_OBJECT_KIND_DOMAIN
/******************************************************************************
 *                          DomainRepresentation
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_objrepr_domain
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_DomainRepresentation)

#define NANO_XRCE_DomainRepresentation_deserialize_cdr(s_,sr_) \
    NANO_XRCE_RepresentationRefAndXml_deserialize_cdr(\
        NANO_MEMBER((s_),base),(sr_))

#define NANO_XRCE_DomainRepresentation_serialize_cdr(s_,sr_) \
    NANO_XRCE_RepresentationRefAndXml_serialize_cdr(\
        NANO_MEMBER((s_),base),(sr_))

#define NANO_XRCE_DomainRepresentation_serialized_size_cdr(s_,sr_) \
    NANO_XRCE_RepresentationRefAndXml_serialized_size_cdr(\
        NANO_MEMBER((s_),base),(sr_))
#endif /* NANO_FEAT_OBJECT_KIND_DOMAIN */

/*i @} *//* nano_api_xrce_payload_objrepr_domain */

#if NANO_FEAT_OBJECT_KIND_APPLICATION
/******************************************************************************
 *                      ApplicationRepresentation
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_objrepr_app
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_ApplicationRepresentation)

#define NANO_XRCE_ApplicationRepresentation_deserialize_cdr(s_,sr_) \
    NANO_XRCE_RepresentationRefAndXml_deserialize_cdr(\
        NANO_MEMBER((s_),base),(sr_))

#define NANO_XRCE_ApplicationRepresentation_serialize_cdr(s_,sr_) \
    NANO_XRCE_RepresentationRefAndXml_serialize_cdr(\
        NANO_MEMBER((s_),base),(sr_))

#define NANO_XRCE_ApplicationRepresentation_serialized_size_cdr(s_,sr_) \
    NANO_XRCE_RepresentationRefAndXml_serialized_size_cdr(\
        NANO_MEMBER((s_),base),(sr_))

/*i @} *//* nano_api_xrce_payload_objrepr_app */

#endif /* NANO_FEAT_OBJECT_KIND_APPLICATION */


#if NANO_FEAT_OBJECT_KIND_SUBSCRIBER && \
    NANO_FEAT_OBJECT_KIND_PUBLISHER && \
    NANO_FEAT_OBJECT_KIND_DATAWRITER && \
    NANO_FEAT_OBJECT_KIND_DATAREADER && \
    NANO_FEAT_OBJECT_KIND_TOPIC
/******************************************************************************
 *                      ContainedBinAndXmlObject
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_objrepr_common
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_ContainedBinAndXmlObjectRepresentation)

#define NANO_XRCE_ContainedBinAndXmlObjectRepresentation_serialized_size_cdr(s_,sr_)\
(\
    NANO_XRCE_RepresentationBinAndXml_serialized_size_cdr(\
        NANO_MEMBER((s_),base),(sr_)) + \
    NANO_XRCE_ObjectId_serialized_size_cdr(NANO_MEMBER((s_),parent_id),(sr_))\
)

/*i @} *//* nano_api_xrce_payload_objrepr_common */

#endif /* NANO_FEAT_OBJECT_KIND_SUBSCRIBER && \
          NANO_FEAT_OBJECT_KIND_PUBLISHER && \
          NANO_FEAT_OBJECT_KIND_DATAWRITER && \
          NANO_FEAT_OBJECT_KIND_DATAREADER && \
          NANO_FEAT_OBJECT_KIND_TOPIC */

#if NANO_FEAT_OBJECT_KIND_SUBSCRIBER || \
    NANO_FEAT_OBJECT_KIND_PUBLISHER
/******************************************************************************
 *                              EntityName
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_support_entityname
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_EntityName)

#define NANO_XRCE_EntityName_deserialize_cdr(s_,sr_) \
    NANO_String_deserialize_cdr_max(NANO_MEMBER((s_),value),(sr_),\
        NANO_STRING_LENGTH_UNBOUNDED)

#define NANO_XRCE_EntityName_serialize_cdr(s_,sr_) \
    NANO_String_serialize_cdr_max(NANO_MEMBER((s_),value),(sr_),\
        NANO_STRING_LENGTH_UNBOUNDED)

#define NANO_XRCE_EntityName_serialized_size_cdr(s_,sr_) \
    NANO_String_serialized_size_cdr_max(NANO_MEMBER((s_),value),(sr_),\
        NANO_STRING_LENGTH_UNBOUNDED)

/*i @} *//* nano_api_xrce_payload_support_entityname */

/******************************************************************************
 *                              Partition
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_support_partition
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_Partition)

/******************************************************************************
 *                              PartitionSeq
 ******************************************************************************/
#if NANO_FEAT_TYPED_SEQUENCE
NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_PartitionSeq)

#define NANO_XRCE_PartitionSeq_deserialize_cdr(s_,sr_) \
    NANO_TSeqImpl_deserialize_cdr((s_),(sr_),\
        NANO_SEQUENCE_LENGTH_UNBOUNDED,\
        NANO_XRCE_Partition_deserialize_cdr)

#define NANO_XRCE_PartitionSeq_serialize_cdr(s_,sr_) \
    NANO_TSeqImpl_serialize_cdr((s_),(sr_),\
        NANO_SEQUENCE_LENGTH_UNBOUNDED,\
        NANO_XRCE_Partition_serialize_cdr)

#define NANO_XRCE_PartitionSeq_serialized_size_cdr(s_,sr_) \
    NANO_TSeqImpl_serialized_size_cdr((s_),(sr_),\
        NANO_SEQUENCE_LENGTH_UNBOUNDED,\
        NANO_XRCE_Partition_serialized_size_cdr)

/*i @} *//* nano_api_xrce_payload_support_partition */

#endif /* NANO_FEAT_TYPED_SEQUENCE */
/******************************************************************************
 *                              GroupData
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_support_groupdata
 * @{
 */

#if NANO_FEAT_TYPED_SEQUENCE
NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_GroupData)

#define NANO_XRCE_GroupData_deserialize_cdr(s_,sr_) \
    NANO_PTSeqImpl_deserialize_cdr((s_),(sr_),\
        NANO_SEQUENCE_LENGTH_UNBOUNDED)

#define NANO_XRCE_GroupData_serialize_cdr(s_,sr_) \
    NANO_PTSeqImpl_serialize_cdr((s_),(sr_),\
        NANO_SEQUENCE_LENGTH_UNBOUNDED)

#define NANO_XRCE_GroupData_serialized_size_cdr(s_,sr_) \
    NANO_PTSeqImpl_serialized_size_cdr((s_),(sr_),\
        NANO_SEQUENCE_LENGTH_UNBOUNDED)

#endif /* NANO_FEAT_TYPED_SEQUENCE */
#endif /* NANO_FEAT_OBJECT_KIND_SUBSCRIBER || \
            NANO_FEAT_OBJECT_KIND_PUBLISHER */

/*i @} *//* nano_api_xrce_payload_support_groupdata */

#if NANO_FEAT_OBJECT_KIND_DATAWRITER || \
    NANO_FEAT_OBJECT_KIND_DATAREADER
/******************************************************************************
 *                              UserData
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_support_userdata
 * @{
 */

#if NANO_FEAT_TYPED_SEQUENCE
NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_UserData)

#define NANO_XRCE_UserData_deserialize_cdr(s_,sr_) \
    NANO_PTSeqImpl_deserialize_cdr((s_),(sr_),\
        NANO_SEQUENCE_LENGTH_UNBOUNDED)

#define NANO_XRCE_UserData_serialize_cdr(s_,sr_) \
    NANO_PTSeqImpl_serialize_cdr((s_),(sr_),\
        NANO_SEQUENCE_LENGTH_UNBOUNDED)

#define NANO_XRCE_UserData_serialized_size_cdr(s_,sr_) \
    NANO_PTSeqImpl_serialized_size_cdr((s_),(sr_),\
        NANO_SEQUENCE_LENGTH_UNBOUNDED)

/*i @} *//* nano_api_xrce_payload_support_userdata */

#endif /* NANO_FEAT_TYPED_SEQUENCE */
#endif /* NANO_FEAT_OBJECT_KIND_DATAWRITER || \
            NANO_FEAT_OBJECT_KIND_DATAREADER */

#if NANO_FEAT_OBJECT_KIND_SUBSCRIBER || \
    NANO_FEAT_OBJECT_KIND_PUBLISHER
/******************************************************************************
 *                      EndpointContainerQosBinary
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_support_containerqos
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_EndpointContainerQosBinary)

#if NANO_FEAT_OPTIONAL
#define NANO_XRCE_EndpointContainerQosBinary_serialized_size_cdr(s_,sr_) \
(\
    ((s_) == NULL)?\
       (NANO_OPTIONALMEMBER_SERIALIZED_SIZE_MAX(\
            (sr_),NANO_XRCE_PartitionSeq_serialized_size_cdr) + \
        NANO_OPTIONALMEMBER_SERIALIZED_SIZE_MAX(\
            (sr_),NANO_XRCE_GroupData_serialized_size_cdr)) \
        : \
       (NANO_OPTIONALMEMBER_SERIALIZED_SIZE((s_),partitions,(sr),\
            NANO_XRCE_PartitionSeq_serialized_size_cdr) + \
        NANO_OPTIONALMEMBER_SERIALIZED_SIZE((s_),group_data,(sr),\
            NANO_XRCE_GroupData_serialized_size_cdr))\
)
#endif /* NANO_FEAT_OPTIONAL */


/*i @} *//* nano_api_xrce_payload_support_containerqos */


/******************************************************************************
 *                       EndpointContainerBinary
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_objrepr_common
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_EndpointContainerBinary)

#if NANO_FEAT_OPTIONAL
#define NANO_XRCE_EndpointContainerBinary_serialized_size_cdr(s_,sr_) \
(\
    ((s_) == NULL)?\
       (NANO_OPTIONALMEMBER_SERIALIZED_SIZE_MAX((sr_),\
            NANO_XRCE_EntityName_serialized_size_cdr) + \
        NANO_OPTIONALMEMBER_SERIALIZED_SIZE_MAX((sr_),\
            NANO_XRCE_EndpointContainerQosBinary_serialized_size_cdr)) \
        : \
       (NANO_OPTIONALMEMBER_SERIALIZED_SIZE((s_),partitions,(sr),\
            NANO_XRCE_EntityName_serialized_size_cdr) + \
        NANO_OPTIONALMEMBER_SERIALIZED_SIZE((s_),group_data,(sr),\
            NANO_XRCE_EndpointContainerQosBinary_serialized_size_cdr))\
)
#endif /* NANO_FEAT_OPTIONAL */


/*i @} *//* nano_api_xrce_payload_objrepr_common */


#endif /* NANO_FEAT_OBJECT_KIND_SUBSCRIBER || \
            NANO_FEAT_OBJECT_KIND_PUBLISHER */

#if NANO_FEAT_OBJECT_KIND_PUBLISHER
/******************************************************************************
 *                      PublisherRepresentation
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_objrepr_pub
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_PublisherRepresentation)

#define NANO_XRCE_PublisherRepresentation_deserialize_cdr(s_,sr_) \
    NANO_XRCE_ContainedBinAndXmlObjectRepresentation_deserialize_cdr((s_),(sr_))

#define NANO_XRCE_PublisherRepresentation_serialize_cdr(s_,sr_) \
    NANO_XRCE_ContainedBinAndXmlObjectRepresentation_serialize_cdr((s_),(sr_))

#define NANO_XRCE_PublisherRepresentation_serialized_size_cdr(s_,sr_) \
    NANO_XRCE_ContainedBinAndXmlObjectRepresentation_serialized_size_cdr((s_),(sr_))

/*i @} *//* nano_api_xrce_payload_objrepr_pub */

/******************************************************************************
 *                          PublisherQosBinary
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_support_pubqos
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_PublisherQosBinary)

#define NANO_XRCE_PublisherQosBinary_deserialize_cdr(s_,sr_) \
    NANO_XRCE_EndpointContainerQosBinary_deserialize_cdr((s_),(sr_))

#define NANO_XRCE_PublisherQosBinary_serialize_cdr(s_,sr_) \
    NANO_XRCE_EndpointContainerQosBinary_serialize_cdr((s_),(sr_))

#define NANO_XRCE_PublisherQosBinary_serialized_size_cdr(s_,sr_) \
    NANO_XRCE_EndpointContainerQosBinary_serialized_size_cdr((s_),(sr_))

/*i @} *//* nano_api_xrce_payload_support_pubqos */

/******************************************************************************
 *                             PublisherBinary
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_objrepr_pub
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_PublisherBinary)

#define NANO_XRCE_PublisherBinary_deserialize_cdr(s_,sr_) \
    NANO_XRCE_EndpointContainerBinary_deserialize_cdr((s_),(sr_))

#define NANO_XRCE_PublisherBinary_serialize_cdr(s_,sr_) \
    NANO_XRCE_EndpointContainerBinary_serialize_cdr((s_),(sr_))

#define NANO_XRCE_PublisherBinary_serialized_size_cdr(s_,sr_) \
    NANO_XRCE_EndpointContainerBinary_serialized_size_cdr((s_),(sr_))

/*i @} *//* nano_api_xrce_payload_objrepr_pub */

#endif /* NANO_FEAT_OBJECT_KIND_PUBLISHER */

#if NANO_FEAT_OBJECT_KIND_SUBSCRIBER
/******************************************************************************
 *                          SubscriberRepresentation
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_objrepr_sub
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_SubscriberRepresentation)

#define NANO_XRCE_SubscriberRepresentation_deserialize_cdr(s_,sr_) \
    NANO_XRCE_ContainedBinAndXmlObjectRepresentation_deserialize_cdr((s_),(sr_))

#define NANO_XRCE_SubscriberRepresentation_serialize_cdr(s_,sr_) \
    NANO_XRCE_ContainedBinAndXmlObjectRepresentation_serialize_cdr((s_),(sr_))

#define NANO_XRCE_SubscriberRepresentation_serialized_size_cdr(s_,sr_) \
    NANO_XRCE_ContainedBinAndXmlObjectRepresentation_serialized_size_cdr((s_),(sr_))

/*i @} *//* nano_api_xrce_payload_objrepr_sub */

/******************************************************************************
 *                          SubscriberQosBinary
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_support_subqos
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_SubscriberQosBinary)

#define NANO_XRCE_SubscriberQosBinary_deserialize_cdr(s_,sr_) \
    NANO_XRCE_EndpointContainerQosBinary_deserialize_cdr((s_),(sr_))

#define NANO_XRCE_SubscriberQosBinary_serialize_cdr(s_,sr_) \
    NANO_XRCE_EndpointContainerQosBinary_serialize_cdr((s_),(sr_))

#define NANO_XRCE_SubscriberQosBinary_serialized_size_cdr(s_,sr_) \
    NANO_XRCE_EndpointContainerQosBinary_serialized_size_cdr((s_),(sr_))

/*i @} *//* nano_api_xrce_payload_support_subqos */

/******************************************************************************
 *                          SubscriberBinary
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_objrepr_sub
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_SubscriberBinary)

#define NANO_XRCE_SubscriberBinary_deserialize_cdr(s_,sr_) \
    NANO_XRCE_EndpointContainerBinary_deserialize_cdr((s_),(sr_))

#define NANO_XRCE_SubscriberBinary_serialize_cdr(s_,sr_) \
    NANO_XRCE_EndpointContainerBinary_serialize_cdr((s_),(sr_))

#define NANO_XRCE_SubscriberBinary_serialized_size_cdr(s_,sr_) \
    NANO_XRCE_EndpointContainerBinary_serialized_size_cdr((s_),(sr_))

/*i @} *//* nano_api_xrce_payload_objrepr_sub */

#endif /* NANO_FEAT_OBJECT_KIND_SUBSCRIBER */

#if NANO_FEAT_OBJECT_KIND_TOPIC
/******************************************************************************
 *                              TopicName
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_support_topicname
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_TopicName)

#define NANO_XRCE_TopicName_deserialize_cdr(s_,sr_) \
    NANO_String_deserialize_cdr_max(\
        NANO_MEMBER((s_),value), (sr_), NANO_STRING_LENGTH_UNBOUNDED)

#define NANO_XRCE_TopicName_serialize_cdr(s_,sr_) \
    NANO_String_serialize_cdr_max(\
        NANO_MEMBER((s_),value), (sr_), NANO_STRING_LENGTH_UNBOUNDED)

#define NANO_XRCE_TopicName_serialized_size_cdr(s_,sr_) \
    NANO_String_serialized_size_cdr_max(\
        NANO_MEMBER((s_),value), (sr_), NANO_STRING_LENGTH_UNBOUNDED)

#define NANO_XRCE_TopicName_alignment \
    NANO_String_alignment

/*i @} *//* nano_api_xrce_payload_support_topicname */

/******************************************************************************
 *                              TypeRef
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_support_typeref
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_TypeRef)

#define NANO_XRCE_TypeRef_deserialize_cdr(s_,sr_) \
    NANO_String_deserialize_cdr_max(\
        NANO_MEMBER((s_),value), (sr_), NANO_STRING_LENGTH_UNBOUNDED)

#define NANO_XRCE_TypeRef_serialize_cdr(s_,sr_) \
    NANO_String_serialize_cdr_max(\
        NANO_MEMBER((s_),value), (sr_), NANO_STRING_LENGTH_UNBOUNDED)

#define NANO_XRCE_TypeRef_serialized_size_cdr(s_,sr_) \
    NANO_String_serialized_size_cdr_max(\
        NANO_MEMBER((s_),value), (sr_), NANO_STRING_LENGTH_UNBOUNDED)

#define NANO_XRCE_TypeRef_alignment \
    NANO_String_alignment

/*i @} *//* nano_api_xrce_payload_support_typeref */

/******************************************************************************
 *                              TypeId
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_support_typeid
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_TypeId)

#define NANO_XRCE_TypeId_deserialize_cdr(s_,sr_) \
    NANO_String_deserialize_cdr_max(\
        NANO_MEMBER((s_),value), (sr_), NANO_STRING_LENGTH_UNBOUNDED)

#define NANO_XRCE_TypeId_serialize_cdr(s_,sr_) \
    NANO_String_serialize_cdr_max(\
        NANO_MEMBER((s_),value), (sr_), NANO_STRING_LENGTH_UNBOUNDED)

#define NANO_XRCE_TypeId_serialized_size_cdr(s_,sr_) \
    NANO_String_serialized_size_cdr_max(\
        NANO_MEMBER((s_),value), (sr_), NANO_STRING_LENGTH_UNBOUNDED)

#define NANO_XRCE_TypeId_alignment \
    NANO_String_alignment

/*i @} *//* nano_api_xrce_payload_support_typeid */

#endif /* NANO_FEAT_OBJECT_KIND_TOPIC */


#if NANO_FEAT_CONTENT_FILTER
/******************************************************************************
 *                              ContentFilterExpr
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_support_contentfilter
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_ContentFilterExpr)

#define NANO_XRCE_ContentFilterExpr_deserialize_cdr(s_,sr_) \
    NANO_String_deserialize_cdr_max(\
        NANO_MEMBER((s_),value), (sr_), NANO_STRING_LENGTH_UNBOUNDED)

#define NANO_XRCE_ContentFilterExpr_serialize_cdr(s_,sr_) \
    NANO_String_serialize_cdr_max(\
        NANO_MEMBER((s_),value), (sr_), NANO_STRING_LENGTH_UNBOUNDED)

#define NANO_XRCE_ContentFilterExpr_serialized_size_cdr(s_,sr_) \
    NANO_String_serialized_size_cdr_max(\
        NANO_MEMBER((s_),value), (sr_), NANO_STRING_LENGTH_UNBOUNDED)

#define NANO_XRCE_ContentFilterExpr_alignment \
    NANO_String_alignment

/*i @} *//* nano_api_xrce_payload_support_contentfilter */

#endif /* NANO_FEAT_CONTENT_FILTER */

#if NANO_FEAT_OBJECT_KIND_DATAWRITER && \
    NANO_FEAT_OBJECT_KIND_DATAREADER
/******************************************************************************
 *                        EndpointQosFlags
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_support_endpointqos
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_EndpointQosFlags)

#define NANO_XRCE_EndpointQosFlags_deserialize_cdr(s_,sr_) \
    NANO_u16_deserialize_cdr((s_),(sr_))

#define NANO_XRCE_EndpointQosFlags_serialize_cdr(s_,sr_) \
    NANO_u16_serialize_cdr((s_),(sr_))

#define NANO_XRCE_EndpointQosFlags_serialized_size_cdr(s_,sr_) \
    NANO_u16_serialized_size_cdr((s_),(sr_))

#define NANO_XRCE_EndpointQosFlags_alignment \
    NANO_u16_alignment

/*i @} *//* nano_api_xrce_payload_support_endpointqos */

/******************************************************************************
 *                              EndpointQosBinary
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_support_endpointqos
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_EndpointQosBinary)

#define NANO_XRCE_EndpointQosBinary_serialized_size_cdr(s_,sr_) \
(\
    NANO_XRCE_EndpointQosFlags_serialized_size_cdr(NANO_MEMBER((s_),qos_flags),(sr_)) + \
    ((s_) == NULL)?\
       (NANO_OPTIONALMEMBER_SERIALIZED_SIZE_MAX(\
            (sr_),NANO_u16_serialized_size_cdr) + \
        NANO_OPTIONALMEMBER_SERIALIZED_SIZE_MAX(\
            (sr_),NANO_u32_serialized_size_cdr) + \
        NANO_OPTIONALMEMBER_SERIALIZED_SIZE_MAX(\
            (sr_),NANO_u32_serialized_size_cdr) + \
        NANO_OPTIONALMEMBER_SERIALIZED_SIZE_MAX(\
            (sr_),NANO_XRCE_UserData_serialized_size_cdr)) \
        : \
       (NANO_OPTIONALMEMBER_SERIALIZED_SIZE((s_),history_depth,(sr),\
                NANO_u16_serialized_size_cdr) +\
        NANO_OPTIONALMEMBER_SERIALIZED_SIZE((s_),deadline_msec,(sr),\
                NANO_u32_serialized_size_cdr) +\
        NANO_OPTIONALMEMBER_SERIALIZED_SIZE((s_),lifespan_msec,(sr),\
                NANO_u32_serialized_size_cdr) +\
        NANO_OPTIONALMEMBER_SERIALIZED_SIZE((s_),user_data,(sr),\
                NANO_XRCE_UserData_serialized_size_cdr)) \
)

/*i @} *//* nano_api_xrce_payload_support_endpointqos */

#endif /*NANO_FEAT_OBJECT_KIND_DATAWRITER && \
            NANO_FEAT_OBJECT_KIND_DATAREADER */


#if NANO_FEAT_OBJECT_KIND_DATAWRITER
/******************************************************************************
 *                          DataWriterRepresentation
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_objrepr_dw
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_DataWriterRepresentation)

#define NANO_XRCE_DataWriterRepresentation_deserialize_cdr(s_,sr_) \
    NANO_XRCE_ContainedBinAndXmlObjectRepresentation_deserialize_cdr((s_),(sr_))

#define NANO_XRCE_DataWriterRepresentation_serialize_cdr(s_,sr_) \
    NANO_XRCE_ContainedBinAndXmlObjectRepresentation_serialize_cdr((s_),(sr_))

#define NANO_XRCE_DataWriterRepresentation_serialized_size_cdr(s_,sr_) \
    NANO_XRCE_ContainedBinAndXmlObjectRepresentation_serialized_size_cdr((s_),(sr_))

/*i @} *//* nano_api_xrce_payload_objrepr_dw */


/******************************************************************************
 *                          DataWriterQosBinary
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_support_dwqos
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_DataWriterQosBinary)

#define NANO_XRCE_DataWriterQosBinary_serialized_size_cdr(s_,sr_) \
(\
    NANO_XRCE_EndpointQosBinary_serialized_size_cdr(\
        NANO_MEMBER((s_),base),(sr_)) + \
    (((s_) == NULL)?\
        NANO_OPTIONALMEMBER_SERIALIZED_SIZE_MAX(\
            (sr_),NANO_u32_serialized_size_cdr) : \
        NANO_OPTIONALMEMBER_SERIALIZED_SIZE(s_),ownership_strength,(sr),\
            NANO_u32_serialized_size_cdr))\
)

/*i @} *//* nano_api_xrce_payload_support_dwqos */

/******************************************************************************
 *                          DataWriterBinary
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_objrepr_dw
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_DataWriterBinary)

#define NANO_XRCE_DataWriterBinary_serialized_size_cdr(s_,sr_) \
(\
    NANO_TopicName_serialized_size_cdr(NANO_MEMBER((s_),topic_name),(sr_)) + \
    (((s_) == NULL)?\
        NANO_OPTIONALMEMBER_SERIALIZED_SIZE_MAX(\
            (sr_),NANO_TopicName_serialized_size_cdr) : \
        NANO_OPTIONALMEMBER_SERIALIZED_SIZE((s_),qos,(sr),\
            NANO_XRCE_DataWriterQosBinary_serialized_size_cdr))\
)

/*i @} *//* nano_api_xrce_payload_objrepr_dw */

#endif /* NANO_FEAT_OBJECT_KIND_DATAWRITER */

#if NANO_FEAT_OBJECT_KIND_DATAREADER
/******************************************************************************
 *                        DataReaderRepresentation
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_objrepr_dr
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_DataReaderRepresentation)

#define NANO_XRCE_DataReaderRepresentation_deserialize_cdr(s_,sr_) \
    NANO_XRCE_ContainedBinAndXmlObjectRepresentation_deserialize_cdr((s_),(sr_))

#define NANO_XRCE_DataReaderRepresentation_serialize_cdr(s_,sr_) \
    NANO_XRCE_ContainedBinAndXmlObjectRepresentation_serialize_cdr((s_),(sr_))

#define NANO_XRCE_DataReaderRepresentation_serialized_size_cdr(s_,sr_) \
    NANO_XRCE_ContainedBinAndXmlObjectRepresentation_serialized_size_cdr((s_),(sr_))

/*i @} *//* nano_api_xrce_payload_objrepr_dr */

/******************************************************************************
 *                          DataReaderQosBinary
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_support_drqos
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_DataReaderQosBinary)

#define NANO_XRCE_DataReaderQosBinary_serialized_size_cdr(s_,sr_) \
(\
    NANO_XRCE_EndpointQosBinary_serialized_size_cdr(\
        NANO_MEMBER((s_),base),(sr_)) + \
    (((s_) == NULL)?\
        NANO_OPTIONALMEMBER_SERIALIZED_SIZE_MAX(\
            (sr_),NANO_u32_serialized_size_cdr) + \
        NANO_OPTIONALMEMBER_SERIALIZED_SIZE_MAX(\
            (sr_),NANO_u32_serialized_size_cdr) : \
        NANO_OPTIONALMEMBER_SERIALIZED_SIZE((s_),timebased_filter_msec,(sr),\
            NANO_u32_serialized_size_cdr) + \
        NANO_OPTIONALMEMBER_SERIALIZED_SIZE((s_),contentbased_filter,(sr),\
            NANO_XRCE_ContentFilterExpr_serialized_size_cdr))\
)

/*i @} *//* nano_api_xrce_payload_support_drqos */

/******************************************************************************
 *                            DataReaderBinary
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_objrepr_dr
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_DataReaderBinary)

#define NANO_XRCE_DataReaderBinary_serialized_size_cdr(s_,sr_) \
(\
    NANO_TopicName_serialized_size_cdr(NANO_MEMBER((s_),topic_name),(sr_)) + \
    (((s_) == NULL)?\
        NANO_OPTIONALMEMBER_SERIALIZED_SIZE_MAX(\
            (sr_),NANO_TopicName_serialized_size_cdr) : \
        NANO_OPTIONALMEMBER_SERIALIZED_SIZE((s_),qos,(sr),\
            NANO_XRCE_DataReaderQosBinary_serialized_size_cdr))\
)

/*i @} *//* nano_api_xrce_payload_objrepr_dr */

#endif /* NANO_FEAT_OBJECT_KIND_DATAREADER */

#if NANO_FEAT_OBJECT_KIND_PARTICIPANT
/******************************************************************************
 *                                DomainId
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_support_domainid
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_DomainId)

#define NANO_XRCE_DomainId_deserialize_cdr(s_,sr_) \
    NANO_i16_deserialize_cdr((s_),(sr_))

#define NANO_XRCE_DomainId_serialize_cdr(s_,sr_) \
    NANO_i16_serialize_cdr((s_),(sr_))

#define NANO_XRCE_DomainId_serialized_size_cdr(s_,sr_) \
    NANO_i16_serialized_size_cdr((s_),(sr_))

#define NANO_XRCE_DomainId_alignment \
    NANO_i16_alignment

/*i @} *//* nano_api_xrce_payload_support_domainid */

/******************************************************************************
 *                  DomainParticipantRepresentation
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_objrepr_dp
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_DomainParticipantRepresentation)

#define NANO_XRCE_DomainParticipantRepresentation_serialized_size_cdr(s_,sr_) \
(\
    NANO_XRCE_Representation3_serialized_size_cdr(NANO_MEMBER((s_),base),(sr_)) + \
    NANO_XRCE_DomainId_serialized_size_cdr(NANO_MEMBER((s_),domain_id),(sr_))\
)

/*i @} *//* nano_api_xrce_payload_objrepr_dp */

/******************************************************************************
 *                      DomainParticipantBinary
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_objrepr_dp
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_DomainParticipantBinary)

#define NANO_XRCE_DomainParticipantBinary_serialized_size_cdr(s_,sr_) \
(\
    ((s_) == NULL)?\
        NANO_OPTIONALMEMBER_SERIALIZED_SIZE_MAX((sr_),\
            (sr_),NANO_XRCE_ObjectRef_serialized_size_cdr) + \
        NANO_OPTIONALMEMBER_SERIALIZED_SIZE_MAX(\
            (sr_),NANO_XRCE_ObjectRef_serialized_size_cdr) : \
        NANO_OPTIONALMEMBER_SERIALIZED_SIZE((s_),domain_ref,(sr),\
            NANO_XRCE_ObjectRef_serialized_size_cdr) + \
        NANO_OPTIONALMEMBER_SERIALIZED_SIZE((s_),qos_profile_ref,(sr),\
            NANO_XRCE_ObjectRef_serialized_size_cdr)\
)

/*i @} *//* nano_api_xrce_payload_objrepr_dp */

#endif /* NANO_FEAT_OBJECT_KIND_PARTICIPANT */

#if NANO_FEAT_OBJECT_KIND_TOPIC
/******************************************************************************
 *                      TopicRepresentation
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_objrepr_topic
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_TopicRepresentation)

#define NANO_XRCE_TopicRepresentation_serialized_size_cdr(s_,sr_) \
(\
    NANO_XRCE_Representation3_serialized_size_cdr(NANO_MEMBER((s_),base),(sr_)) + \
    NANO_XRCE_ObjectId_serialized_size_cdr(NANO_MEMBER((s_),participant_id),(sr_))\
)

/*i @} *//* nano_api_xrce_payload_objrepr_topic */

/******************************************************************************
 *                          TopicBinary
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_objrepr_topic
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_TopicBinary)

#define NANO_XRCE_TopicBinary_serialized_size_cdr(s_,sr_) \
(\
    ((s_) == NULL)?\
        NANO_TopicString_serialized_size_cdr(NULL,(sr_)) + \
        NANO_OPTIONALMEMBER_SERIALIZED_SIZE_MAX(\
            (sr_),NANO_XRCE_TypeRef_serialized_size_cdr) + \
        NANO_OPTIONALMEMBER_SERIALIZED_SIZE_MAX(\
            (sr_),NANO_XRCE_TypeId_serialized_size_cdr) \
        : \
        NANO_TopicString_serialized_size_cdr(\
            NANO_MEMBER((s_),topic_name),(sr_)) + \
        NANO_OPTIONALMEMBER_SERIALIZED_SIZE((s_),typeref,(sr),\
            NANO_XRCE_TypeRef_serialized_size_cdr) + \
        NANO_OPTIONALMEMBER_SERIALIZED_SIZE((s_),type_id,(sr),\
            NANO_XRCE_TypeId_serialized_size_cdr)\
)

#define NANO_XRCE_TopicBinary_alignment \
    NANO_String_alignment

/*i @} *//* nano_api_xrce_payload_objrepr_topic */

#endif /* NANO_FEAT_OBJECT_KIND_TOPIC */

/******************************************************************************
 *                      ClientRepresentation
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_objrepr_client
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_ClientRepresentation)

#define NANO_XRCE_ClientRepresentation_serialized_size_cdr(s_,sr_) \
(\
    NANO_XRCE_Cookie_serialized_size_cdr(NANO_MEMBER((s_),xrce_cookie),(sr_)) + \
    NANO_XRCE_Version_serialized_size_cdr(NANO_MEMBER((s_),xrce_version),(sr_)) + \
    NANO_XRCE_VendorId_serialized_size_cdr(NANO_MEMBER((s_),xrce_vendor_id),(sr_)) + \
    NANO_XRCE_ClientKey_serialized_size_cdr(NANO_MEMBER((s_),client_key),(sr_)) + \
    NANO_XRCE_SessionId_serialized_size_cdr(NANO_MEMBER((s_),session_id),(sr_))\
)

#define NANO_XRCE_ClientRepresentation_alignment \
    NANO_XRCE_Cookie_alignment

/*i @} *//* nano_api_xrce_payload_objrepr_client */

/******************************************************************************
 *                          AgentRepresentation
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_objrepr_agent
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_AgentRepresentation)

#define NANO_XRCE_AgentRepresentation_serialized_size_cdr(s_,sr_) \
(\
    NANO_XRCE_Cookie_serialized_size_cdr(NANO_MEMBER((s_),xrce_cookie),(sr_)) + \
    NANO_XRCE_Version_serialized_size_cdr(NANO_MEMBER((s_),xrce_version),(sr_)) + \
    NANO_XRCE_VendorId_serialized_size_cdr(NANO_MEMBER((s_),xrce_vendor_id),(sr_))\
)

#define NANO_XRCE_AgentRepresentation_alignment \
    NANO_XRCE_Cookie_alignment

/*i @} *//* nano_api_xrce_payload_objrepr_agent */


#if NANO_FEAT_OBJECT
/******************************************************************************
 *                              ObjectVariant
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_objrepr_common
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_ObjectVariant)

#define NANO_XRCE_ObjectVariant_serialized_size_cdr(s_,sr_) \
(\
    NANO_XRCE_ObjectKind_serialized_size_cdr(NANO_MEMBER((s_),kind),(sr_)) + \
    ((s_) == NULL)?\
    (\
        NANO_XRCE_TopicRepresentation_serialized_size_cdr(NULL,(sr_)) \
    )\
    :\
    (\
        ((s_)->kind == NANO_XRCE_OBJK_PARTICIPANT)?\
            NANO_XRCE_DomainParticipantRepresentation_serialized_size_cdr(&(s_)->value.participant,(sr_)) : \
        ((s_)->kind == NANO_XRCE_OBJK_TOPIC)?\
            NANO_XRCE_TopicRepresentation_serialized_size_cdr(&(s_)->value.topic,(sr_)) : \
        ((s_)->kind == NANO_XRCE_OBJK_PUBLISHER)?\
            NANO_XRCE_PublisherRepresentation_serialized_size_cdr(&(s_)->value.publisher,(sr_)) : \
        ((s_)->kind == NANO_XRCE_OBJK_SUBSCRIBER)?\
            NANO_XRCE_SubscriberRepresentation_serialized_size_cdr(&(s_)->value.subscriber,(sr_)) : \
        ((s_)->kind == NANO_XRCE_OBJK_DATAWRITER)?\
            NANO_XRCE_DataWriterRepresentation_serialized_size_cdr(&(s_)->value.data_writer,(sr_)) : \
        ((s_)->kind == NANO_XRCE_OBJK_DATAREADER)?\
            NANO_XRCE_DataReaderRepresentation_serialized_size_cdr(&(s_)->value.data_reader,(sr_)) : \
        ((s_)->kind == NANO_XRCE_OBJK_DOMAIN)?\
            NANO_XRCE_DomainRepresentation_serialized_size_cdr(&(s_)->value.domain,(sr_)) : \
        ((s_)->kind == NANO_XRCE_OBJK_QOSPROFILE)?\
            NANO_XRCE_QosProfileRepresentation_serialized_size_cdr(&(s_)->value.qos_profile,(sr_)) : \
        ((s_)->kind == NANO_XRCE_OBJK_TYPE)?\
            NANO_XRCE_TypeRepresentation_serialized_size_cdr(&(s_)->value.type,(sr_)) : \
        ((s_)->kind == NANO_XRCE_OBJK_APPLICATION)?\
            NANO_XRCE_ApplicationRepresentation_serialized_size_cdr(&(s_)->value.application,(sr_)) : \
        ((s_)->kind == NANO_XRCE_OBJK_AGENT)?\
            NANO_XRCE_AgentRepresentation_serialized_size_cdr(&(s_)->value.agent,(sr_)) : \
        ((s_)->kind == NANO_XRCE_OBJK_CLIENT)?\
            NANO_XRCE_ClientRepresentation_serialized_size_cdr(&(s_)->value.client,(sr_)) : \
            0 \
    )\
)

/*i @} *//* nano_api_xrce_payload_objrepr_common */

#endif /* NANO_FEAT_OBJECT */

#if NANO_FEAT_TRANSPORT
/******************************************************************************
 *                          TransportLocator
 ******************************************************************************/

/*i
 * @addtogroup nano_api_client_transport_locator
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_TransportLocator)

/*i @} *//* nano_api_client_transport_locator */

#if NANO_FEAT_TYPED_SEQUENCE
/******************************************************************************
 *                         TransportLocatorSeq
 ******************************************************************************/

/*i
 * @addtogroup nano_api_client_transport_locator_seq
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_TransportLocatorSeq)

#define NANO_XRCE_TransportLocatorSeq_deserialize_cdr(s_,sr_) \
    NANO_TSeqImpl_deserialize_cdr((s_),(sr_),\
        NANO_SEQUENCE_LENGTH_UNBOUNDED,\
        NANO_XRCE_TransportLocator_deserialize_cdr)

#define NANO_XRCE_TransportLocatorSeq_serialize_cdr(s_,sr_) \
    NANO_TSeqImpl_serialize_cdr((s_),(sr_),\
        NANO_SEQUENCE_LENGTH_UNBOUNDED,\
        NANO_XRCE_TransportLocator_serialize_cdr)

#define NANO_XRCE_TransportLocatorSeq_serialized_size_cdr(s_,sr_) \
    NANO_TSeqImpl_serialized_size_cdr((s_),(sr_),\
        NANO_SEQUENCE_LENGTH_UNBOUNDED,\
        NANO_XRCE_TransportLocator_serialized_size_cdr)

/*i @} *//* nano_api_client_transport_locator_seq */

#endif /* NANO_FEAT_TYPED_SEQUENCE */
#endif /* NANO_FEAT_TRANSPORT */

#if NANO_FEAT_OBJECT_INFO
/******************************************************************************
 *                              InfoMask
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_support_infomask
 * @{
 */

/* TODO map InfoMask to u32 instead of u8 */
NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_InfoMask)

#define NANO_XRCE_InfoMask_deserialize_cdr(s_,sr_) \
    NANO_u32_deserialize_cdr(s_,sr_)

#define NANO_XRCE_InfoMask_serialize_cdr(s_,sr_) \
    NANO_u32_serialize_cdr(s_,sr_)

#define NANO_XRCE_InfoMask_serialized_size_cdr(s_,sr_) \
    NANO_u32_serialized_size_cdr(s_,sr_)

#define NANO_XRCE_InfoMask_alignment \
    NANO_u32_alignment

/*i @} *//* nano_api_xrce_payload_support_infomask */

/******************************************************************************
 *                          AgentActivityInfo
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_activity_agent
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_AgentActivityInfo)

#define NANO_XRCE_AgentActivityInfo_serialized_size_cdr(s_,sr_) \
(\
    NANO_i16_serialized_size_cdr(NANO_MEMBER((s_),availability),(sr_)) + \
    NANO_XRCE_TransportLocatorSeq_serialized_size_cdr(NANO_MEMBER((s_),address_seq),(sr_))\
)

/*i @} *//* nano_api_xrce_payload_activity_agent */

#if NANO_FEAT_OBJECT_KIND_DATAWRITER
/******************************************************************************
 *                        DataWriterActivityInfo
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_activity_dw
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_DataWriterActivityInfo)

#define NANO_XRCE_DataWriterActivityInfo_serialized_size_cdr(s_,sr_) \
(\
    NANO_u32_serialized_size_cdr(NANO_MEMBER((s_),sample_seq_num),(sr_)) + \
    NANO_u16_serialized_size_cdr(NANO_MEMBER((s_),stream_seq_num),(sr_)) \
)

/*i @} *//* nano_api_xrce_payload_activity_dw */

#endif /* NANO_FEAT_OBJECT_KIND_DATAWRITER */

#if NANO_FEAT_OBJECT_KIND_DATAREADER
/******************************************************************************
 *                        DataReaderActivityInfo
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_activity_dr
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_DataReaderActivityInfo)

#define NANO_XRCE_DataReaderActivityInfo_serialized_size_cdr(s_,sr_) \
(\
    NANO_u16_serialized_size_cdr(NANO_MEMBER((s_),highest_acked_seq_num),(sr_)) \
)

/*i @} *//* nano_api_xrce_payload_activity_dr */

#endif /* NANO_FEAT_OBJECT_KIND_DATAREADER */

/******************************************************************************
 *                          ActivityInfoVariant
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_activity
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_ActivityInfoVariant)

#define NANO_XRCE_ActivityInfoVariant_serialized_size_cdr(s_,sr_) \
(\
    NANO_XRCE_ObjectKind_serialized_size_cdr(NANO_MEMBER((s_),kind),(sr_)) + \
    ((s_) == NULL)?\
    (\
        NANO_XRCE_DataWriterActivityInfo_serialized_size_cdr(&(s_)->value.data_writer,(sr_)) \
    )\
    : \
    (\
        ((s_)->kind == NANO_XRCE_OBJK_DATAWRITER)?\
            NANO_XRCE_DataWriterActivityInfo_serialized_size_cdr(&(s_)->value.data_writer,(sr_)) : \
        ((s_)->kind == NANO_XRCE_OBJK_DATAREADER)?\
            NANO_XRCE_DataReaderActivityInfo_serialized_size_cdr(&(s_)->value.data_reader,(sr_)) : \
        ((s_)->kind == NANO_XRCE_OBJK_AGENT)?\
            NANO_XRCE_AgentActivityInfo_serialized_size_cdr(&(s_)->value.agent,(sr_)) : \
            0 \
    )\
)

/*i @} *//* nano_api_xrce_payload_activity */

/******************************************************************************
 *                              ObjectInfo
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_activity
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_ObjectInfo)

#define NANO_XRCE_ObjectInfo_serialized_size_cdr(s_,sr_) \
(\
    ((s_) == NULL)?\
    (\
        NANO_OPTIONALMEMBER_SERIALIZED_SIZE_MAX((sr_),\
            NANO_XRCE_ActivityInfoVariant_serialized_size_cdr) + \
        NANO_OPTIONALMEMBER_SERIALIZED_SIZE_MAX((sr_),\
            NANO_XRCE_ObjectVariant_serialized_size_cdr)\
    )\
    :\
    (\
        NANO_OPTIONALMEMBER_SERIALIZED_SIZE((s_),activity,(sr),\
            NANO_XRCE_ActivityInfoVariant_serialized_size_cdr) + \
        NANO_OPTIONALMEMBER_SERIALIZED_SIZE((s_),config,(sr),\
            NANO_XRCE_ObjectVariant_serialized_size_cdr)\
    )\
)

/*i @} *//* nano_api_xrce_payload_activity */

#endif /* NANO_FEAT_DISCOVERY */

#if NANO_FEAT_REQUEST
/******************************************************************************
 *                              RequestId
 ******************************************************************************/

/*i
 * @addtogroup nano_api_client_req
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_RequestId)

#define NANO_XRCE_RequestId_deserialize_cdr(s_,sr_) \
    NANO_Octets_deserialize_cdr(\
        NANO_MEMBER((s_),value),\
        (sr_),\
        NANO_XRCE_REQUESTID_VALUE_LEN)

#define NANO_XRCE_RequestId_serialize_cdr(s_,sr_) \
    NANO_Octets_serialize_cdr(\
        NANO_MEMBER((s_),value),\
        (sr_),\
        NANO_XRCE_REQUESTID_VALUE_LEN)

#define NANO_XRCE_RequestId_serialized_size_cdr(s_,sr_) \
    NANO_Octets_serialized_size_cdr(\
        NANO_MEMBER((s_),value),\
        (sr_),\
        NANO_XRCE_REQUESTID_VALUE_LEN)

#define NANO_XRCE_RequestId_alignment \
    NANO_Octets_alignment

/*i @} *//* nano_api_client_req */

/******************************************************************************
 *                          BaseObjectRequest
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_req_common
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_BaseObjectRequest)

#define NANO_XRCE_BaseObjectRequest_serialized_size_cdr(s_,sr_) \
(\
    NANO_XRCE_RequestId_serialized_size_cdr(NANO_MEMBER((s_),request_id),(sr_)) + \
    NANO_XRCE_ObjectId_serialized_size_cdr(NANO_MEMBER((s_),object_id),(sr_))\
)

#define NANO_XRCE_BaseObjectRequest_alignment \
    NANO_XRCE_RequestId_alignment

/*i @} *//* nano_api_xrce_payload_req_common */

/******************************************************************************
 *                          RelatedObjectRequest
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_req_common
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_RelatedObjectRequest)

#define NANO_XRCE_RelatedObjectRequest_deserialize_cdr(s_,sr_) \
    NANO_XRCE_BaseObjectRequest_deserialize_cdr((s_),(sr_))

#define NANO_XRCE_RelatedObjectRequest_serialize_cdr(s_,sr_) \
    NANO_XRCE_BaseObjectRequest_serialize_cdr((s_),(sr_))

#define NANO_XRCE_RelatedObjectRequest_serialized_size_cdr(s_,sr_) \
    NANO_XRCE_BaseObjectRequest_serialized_size_cdr((s_),(sr_))

#define NANO_XRCE_RelatedObjectRequest_alignment \
    NANO_XRCE_BaseObjectRequest_alignment

/*i @} *//* nano_api_xrce_payload_req_common */

#endif /* NANO_FEAT_REQUEST */

#if NANO_FEAT_REPLY
/******************************************************************************
 *                          BaseObjectReply
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_req_common
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_BaseObjectReply)

#define NANO_XRCE_BaseObjectReply_serialized_size_cdr(s_,sr_) \
(\
    NANO_XRCE_RelatedObjectRequest_serialized_size_cdr(NANO_MEMBER((s_),related_request),(sr_)) + \
    NANO_XRCE_ResultStatus_serialized_size_cdr(NANO_MEMBER((s_),result),(sr_)) \
)

#define NANO_XRCE_BaseObjectReply_alignment \
    NANO_XRCE_RelatedObjectRequest_alignment

/*i @} *//* nano_api_xrce_payload_req_common */

#endif /* NANO_FEAT_REPLY */

#if NANO_FEAT_PUBLISH || NANO_FEAT_SUBSCRIBE
/******************************************************************************
 *                              DataFormat
 ******************************************************************************/

/*i
 * @addtogroup nano_api_client_req
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_DataFormat)

#define NANO_XRCE_DataFormat_deserialize_cdr(s_,sr_) \
    NANO_u8_deserialize_cdr(s_,sr_)

#define NANO_XRCE_DataFormat_serialize_cdr(s_,sr_) \
    NANO_u8_serialize_cdr(s_,sr_)

#define NANO_XRCE_DataFormat_serialized_size_cdr(s_,sr_) \
    NANO_u8_serialized_size_cdr(s_,sr_)

#define NANO_XRCE_DataFormat_alignment \
    NANO_u8_alignment

/*i @} *//* nano_api_client_req */

#endif /* NANO_FEAT_PUBLISH || NANO_FEAT_SUBSCRIBE */

#if NANO_FEAT_DELIVERY_CTRL
/******************************************************************************
 *                           DataDeliveryControl
 ******************************************************************************/

/*i
 * @addtogroup nano_api_client_sub
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_DataDeliveryControl)

#define NANO_XRCE_DataDeliveryControl_serialized_size_cdr(s_,sr_) \
(\
    NANO_u16_serialized_size_cdr(NANO_MEMBER((s_),max_samples),(sr_)) + \
    NANO_u16_serialized_size_cdr(NANO_MEMBER((s_),max_elapsed_time),(sr_)) + \
    NANO_u16_serialized_size_cdr(NANO_MEMBER((s_),max_bytes_per_second),(sr_)) + \
    NANO_u16_serialized_size_cdr(NANO_MEMBER((s_),min_pace_period),(sr_)) \
)

/*i @} *//* nano_api_client_sub */

#endif /* NANO_FEAT_DELIVERY_CTRL */

#if NANO_FEAT_SUBSCRIBE
/******************************************************************************
 *                            ReadSpecification
 ******************************************************************************/

/*i
 * @addtogroup nano_api_client_sub
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_ReadSpecification)

#define NANO_XRCE_ReadSpecification_serialized_size_cdr(s_,sr_) \
(\
    NANO_XRCE_StreamId_serialized_size_cdr(\
        NANO_MEMBER((s_),preferred_stream_id),(sr_)) + \
    NANO_XRCE_DataFormat_serialized_size_cdr(\
        NANO_MEMBER((s_),data_format),(sr_)) + \
    ((s_) == NULL)?\
    (\
        NANO_OPTIONALMEMBER_SERIALIZED_SIZE_MAX((sr_),\
            NANO_XRCE_ContentFilterExpr_serialized_size_cdr) + \
        NANO_OPTIONALMEMBER_SERIALIZED_SIZE_MAX((sr_)\
            NANO_XRCE_DataDeliveryControl_serialized_size_cdr)\
    )\
    : \
    (\
        NANO_OPTIONALMEMBER_SERIALIZED_SIZE((s_),content_filter_expr,(sr),\
            NANO_XRCE_ContentFilterExpr_serialized_size_cdr) + \
        NANO_OPTIONALMEMBER_SERIALIZED_SIZE((s_),delivery_ctrl,(sr),\
            NANO_XRCE_DataDeliveryControl_serialized_size_cdr)\
    )\
)

#define NANO_XRCE_ReadSpecification_alignment \
    NANO_XRCE_StreamId_alignment

/*i @} *//* nano_api_client_sub */

#endif /* NANO_FEAT_SUBSCRIBE */

#if NANO_FEAT_SAMPLEINFO
/******************************************************************************
 *                          SampleInfoFlags
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_support_sampleinfoflags
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_SampleInfoFlags)

#define NANO_XRCE_SampleInfoFlags_deserialize_cdr(s_,sr_) \
    NANO_u8_deserialize_cdr(s_,sr_)

#define NANO_XRCE_SampleInfoFlags_serialize_cdr(s_,sr_) \
    NANO_u8_serialize_cdr(s_,sr_)

#define NANO_XRCE_SampleInfoFlags_serialized_size_cdr(s_,sr_) \
    NANO_u8_serialized_size_cdr(s_,sr_)

#define NANO_XRCE_SampleInfoFlags_alignment \
    NANO_u8_alignment

/*i @} *//* nano_api_xrce_payload_support_sampleinfoflags */

/******************************************************************************
 *                            SampleInfoFormat
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_support_sampleinfofmt
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_SampleInfoFormat)

#define NANO_XRCE_SampleInfoFormat_deserialize_cdr(s_,sr_) \
    NANO_u8_deserialize_cdr(s_,sr_)

#define NANO_XRCE_SampleInfoFormat_serialize_cdr(s_,sr_) \
    NANO_u8_serialize_cdr(s_,sr_)

#define NANO_XRCE_SampleInfoFormat_serialized_size_cdr(s_,sr_) \
    NANO_u8_serialized_size_cdr(s_,sr_)

#define NANO_XRCE_SampleInfoFormat_alignment \
    NANO_u8_alignment

/*i @} *//* nano_api_xrce_payload_support_sampleinfofmt */

/******************************************************************************
 *                      SeqNumAndTimestamp
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_support_seqnumandts
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_SeqNumAndTimestamp)

#define NANO_XRCE_SeqNumAndTimestamp_serialized_size_cdr(s_,sr_) \
(\
    NANO_u32_serialized_size_cdr(NANO_MEMBER((s_),sequence_number),(sr_)) + \
    NANO_u32_serialized_size_cdr(NANO_MEMBER((s_),sequence_number),(sr_)) \
)

#define NANO_XRCE_SeqNumAndTimestamp_alignment \
    NANO_u32_alignment

/*i @} *//* nano_api_xrce_payload_support_seqnumandts */

/******************************************************************************
 *                      SampleInfoDetail
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_support_sampleinfodetail
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_SampleInfoDetail)

#define NANO_XRCE_SampleInfoDetail_serialized_size_cdr(s_,sr_) \
(\
    NANO_XRCE_SampleInfoFormat_serialized_size_cdr(NANO_MEMBER((s_),format)) + \
    ((s_) == NULL)?\
    (\
        NANO_XRCE_SeqNumAndTimestamp_serialized_size_cdr(NULL,(sr)) \
    )\
    :\
    (\
        ((s_)->format == NANO_XRCE_SAMPLEINFOFORMAT_FORMAT_SEQNUM)?\
            NANO_u32_serialized_size_cdr(\
                NANO_MEMBER((s_),value.sequence_number),(sr_)) : \
        ((s_)->format == NANO_XRCE_SAMPLEINFOFORMAT_FORMAT_TIMESTAMP)?\
            NANO_u32_serialized_size_cdr(\
                NANO_MEMBER((s_),value.session_time_offset),(sr_)) : \
        ((s_)->format == NANO_XRCE_SAMPLEINFOFORMAT_FORMAT_SEQN_TIMS)?\
            NANO_XRCE_SeqNumAndTimestamp_serialized_size_cdr(\
                NANO_MEMBER((s_),value.seqnum_n_timestamp),(sr_)) : \
            0 \
    )\
)

#define NANO_XRCE_SampleInfoDetail_alignment \
    NANO_XRCE_SampleInfoFormat_alignment

/*i @} *//* nano_api_xrce_payload_support_sampleinfodetail */

/******************************************************************************
 *                              SampleInfo
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_data_sampleinfo
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_SampleInfo)

#define NANO_XRCE_SampleInfo_serialized_size_cdr(s_,sr_) \
(\
    NANO_XRCE_SampleInfoFlags_serialized_size_cdr(NANO_MEMBER((s_),state),(sr_)) + \
    NANO_XRCE_SampleInfoDetail_serialized_size_cdr(NANO_MEMBER((s_),detail),(sr_)) \
)

#define NANO_XRCE_SampleInfo_alignment \
    NANO_XRCE_SampleInfoFlags_alignment

/*i @} *//* nano_api_xrce_payload_data_sampleinfo */

#endif /* NANO_FEAT_SAMPLEINFO */

#if NANO_FEAT_DATA_FORMAT_PACKED
/******************************************************************************
 *                            DeciSecond
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_support_decisecond
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_DeciSecond)

#define NANO_XRCE_DeciSecond_deserialize_cdr(s_,sr_) \
    NANO_u16_deserialize_cdr(s_,sr_)

#define NANO_XRCE_DeciSecond_serialize_cdr(s_,sr_) \
    NANO_u16_serialize_cdr(s_,sr_)

#define NANO_XRCE_DeciSecond_serialized_size_cdr(s_,sr_) \
    NANO_u16_serialized_size_cdr(s_,sr_)

#define NANO_XRCE_DeciSecond_alignment \
    NANO_u16_alignment

/*i @} *//* nano_api_xrce_payload_support_decisecond */

/******************************************************************************
 *                          SampleInfoDelta
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_support_sampleinfodelta
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_SampleInfoDelta)

#define NANO_XRCE_SampleInfoDelta_serialized_size_cdr(s_,sr_) \
(\
    NANO_XRCE_SampleInfoFlags_serialized_size_cdr(NANO_MEMBER((s_),state),(sr_)) + \
    NANO_u8_serialized_size_cdr(NANO_MEMBER((s_),seq_number_delta),(sr_)) + \
    NANO_XRCE_DeciSecond_serialized_size_cdr(NANO_MEMBER((s_),timestamp_delta),(sr_)) \
)

/*i @} *//* nano_api_xrce_payload_support_sampleinfodelta */

#endif /* NANO_FEAT_DATA_FORMAT_PACKED */

#if NANO_FEAT_DATA_FORMAT_DATA
/******************************************************************************
 *                                Data
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_data_sampledata
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_Data)

#if 0
#define NANO_XRCE_Data_deserialize_cdr(s_,sr_) \
    NANO_PTSeqImpl_deserialize_cdr((s_),(sr_),\
        NANO_SADB_MAX_LENGTH(NANO_XRCE_DataBuffer))

#define NANO_XRCE_Data_serialize_cdr(s_,sr_) \
    NANO_PTSeqImpl_serialize_cdr((s_),(sr_),\
        NANO_SADB_MAX_LENGTH(NANO_XRCE_DataBuffer))

#define NANO_XRCE_Data_serialized_size_cdr(s_,sr_) \
    NANO_PTSeqImpl_serialized_size_cdr((s_),(sr_),\
        NANO_SADB_MAX_LENGTH(NANO_XRCE_DataBuffer))
#else
#define NANO_XRCE_Data_deserialize_cdr(s_,sr_) \
    NANO_Octets_deserialize_cdr(\
        NANO_MEMBER((s_),value), (sr_), (s_)->value_len)

#define NANO_XRCE_Data_serialize_cdr(s_,sr_) \
    NANO_Octets_serialize_cdr(\
        NANO_MEMBER((s_),value), (sr_), (s_)->value_len)

#define NANO_XRCE_Data_serialized_size_cdr(s_,sr_) \
    NANO_Octets_serialized_size_cdr(\
        NANO_MEMBER((s_),value), (sr_), (s_)->value_len)
#endif

/******************************************************************************
 *                             SampleData
 ******************************************************************************/

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_SampleData)

/*i @} *//* nano_api_xrce_payload_data_sampledata */

#endif /* NANO_FEAT_DATA_FORMAT_DATA */

#if NANO_FEAT_DATA_FORMAT_DATASEQ
/******************************************************************************
 *                            SampleDataSeq
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_data_sampledataseq
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_SampleDataSeq)

#define NANO_XRCE_SampleDataSeq_deserialize_cdr(s_,sr_) \
    NANO_TSeqImpl_deserialize_cdr((s_),(sr_),\
        NANO_SEQUENCE_LENGTH_UNBOUNDED,\
        NANO_XRCE_SampleData_deserialize_cdr)

#define NANO_XRCE_SampleDataSeq_serialize_cdr(s_,sr_) \
    NANO_TSeqImpl_serialize_cdr((s_),(sr_),\
        NANO_SEQUENCE_LENGTH_UNBOUNDED,\
        NANO_XRCE_SampleData_deserialize_cdr)

#define NANO_XRCE_SampleDataSeq_serialized_size_cdr(s_,sr_) \
    NANO_TSeqImpl_serialized_size_cdr((s_),(sr_),\
        NANO_SEQUENCE_LENGTH_UNBOUNDED,\
        NANO_XRCE_SampleData_deserialize_cdr)

#define NANO_XRCE_SampleDataSeq_alignment \
    NANO_Sequence_alignment

/*i @} *//* nano_api_xrce_payload_data_sampledataseq */

#endif /* NANO_FEAT_DATA_FORMAT_DATASEQ */

#if NANO_FEAT_DATA_FORMAT_SAMPLE
/******************************************************************************
 *                              Sample
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_data_sample
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_Sample)

/*i @} *//* nano_api_xrce_payload_data_sample */

#endif /* NANO_FEAT_DATA_FORMAT_SAMPLE */

#if NANO_FEAT_DATA_FORMAT_SAMPLESEQ
/******************************************************************************
 *                            SampleSeq
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_data_sampleseq
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_SampleSeq)

#define NANO_XRCE_SampleSeq_deserialize_cdr(s_,sr_) \
    NANO_TSeqImpl_deserialize_cdr((s_),(sr_),\
        NANO_SEQUENCE_LENGTH_UNBOUNDED,\
        NANO_XRCE_Sample_deserialize_cdr)

#define NANO_XRCE_SampleSeq_serialize_cdr(s_,sr_) \
    NANO_TSeqImpl_serialize_cdr((s_),(sr_),\
        NANO_SEQUENCE_LENGTH_UNBOUNDED,\
        NANO_XRCE_Sample_deserialize_cdr)

#define NANO_XRCE_SampleSeq_serialized_size_cdr(s_,sr_) \
    NANO_TSeqImpl_serialized_size_cdr((s_),(sr_),\
        NANO_SEQUENCE_LENGTH_UNBOUNDED,\
        NANO_XRCE_Sample_deserialize_cdr)

#define NANO_XRCE_SampleSeq_alignment \
    NANO_Sequence_alignment

/*i @} *//* nano_api_xrce_payload_data_sampleseq */

#endif /* NANO_FEAT_DATA_FORMAT_SAMPLE */

#if NANO_FEAT_DATA_FORMAT_PACKED
/******************************************************************************
 *                           SampleDelta
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_support_sampledelta
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_SampleDelta)

/*i @} *//* nano_api_xrce_payload_support_sampledelta */

/******************************************************************************
 *                          SampleDeltaSeq
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_support_sampledeltaseq
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_SampleDeltaSeq)

#define NANO_XRCE_SampleDeltaSeq_deserialize_cdr(s_,sr_) \
    NANO_TSeqImpl_deserialize_cdr((s_),(sr_),\
        NANO_SEQUENCE_LENGTH_UNBOUNDED,\
        NANO_XRCE_SampleDelta_deserialize_cdr)

#define NANO_XRCE_SampleDeltaSeq_serialize_cdr(s_,sr_) \
    NANO_TSeqImpl_serialize_cdr((s_),(sr_),\
        NANO_SEQUENCE_LENGTH_UNBOUNDED,\
        NANO_XRCE_SampleDelta_deserialize_cdr)

#define NANO_XRCE_SampleDeltaSeq_serialized_size_cdr(s_,sr_) \
    NANO_TSeqImpl_serialized_size_cdr((s_),(sr_),\
        NANO_SEQUENCE_LENGTH_UNBOUNDED,\
        NANO_XRCE_SampleDelta_deserialize_cdr)

#define NANO_XRCE_SampleDeltaSeq_alignment \
    NANO_Sequence_alignment

/*i @} *//* nano_api_xrce_payload_support_sampledeltaseq */

/******************************************************************************
 *                          PackedSamples
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_data_packed
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_PackedSamples)

#define NANO_XRCE_PackedSamples_serialized_size_cdr(s_,sr_) \
(\
    NANO_XRCE_SampleInfo_serialized_size_cdr(NANO_MEMBER((s_),info),(sr_)) + \
    NANO_XRCE_SampleDeltaSeq_serialized_size_cdr(NANO_MEMBER((s_),sample_delta_seq),(sr_)) \
)

#define NANO_XRCE_PackedSamples_alignment \
    NANO_XRCE_SampleInfo_alignment

/*i @} *//* nano_api_xrce_payload_data_packed */

#endif /* NANO_FEAT_DATA_FORMAT_PACKED */

#if NANO_FEAT_DATA
/******************************************************************************
 *                       DataRepresentation
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_data
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_DataRepresentation)

#define NANO_XRCE_DataRepresentation_serialized_size_cdr(s_,sr_) \
(\
    NANO_XRCE_DataFormat_serialized_size_cdr(NANO_MEMBER((s_),format),(sr_)) + \
    ((s_) == NULL)?\
    (\
        NANO_XRCE_SampleSeq_serialized_size_cdr(NULL,(sr_)) \
    )\
    :\
    (\
        ((s_)->format == NANO_XRCE_FORMAT_DATA)?\
            NANO_XRCE_SampleData_serialized_size_cdr(NANO_MEMBER((s_),value.data),(sr_)) : \
        ((s_)->format == NANO_XRCE_FORMAT_SAMPLE)?\
            NANO_XRCE_Sample_serialized_size_cdr(NANO_MEMBER((s_),value.sample),(sr_)) : \
        ((s_)->format == NANO_XRCE_FORMAT_DATA_SEQ)?\
            NANO_XRCE_SampleDataSeq_serialized_size_cdr(NANO_MEMBER((s_),value.data_seq),(sr_)) : \
        ((s_)->format == NANO_XRCE_FORMAT_SAMPLE_SEQ)?\
            NANO_XRCE_SampleSeq_serialized_size_cdr(NANO_MEMBER((s_),value.sample_seq),(sr_)) : \
        ((s_)->format == NANO_XRCE_FORMAT_PACKED_SAMPLES)?\
            NANO_XRCE_PackedSamples_serialized_size_cdr(NANO_MEMBER((s_),value.packed_samples),(sr_)) : \
        0 \
    )\
)

#define NANO_XRCE_DataRepresentation_alignment \
    NANO_XRCE_DataFormat_alignment

/*i @} *//* nano_api_xrce_payload_data */

#endif /* NANO_FEAT_DATA */

/******************************************************************************
 *                        CreateClientPayload
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_req_createclient
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_CreateClientPayload)

#define NANO_XRCE_CreateClientPayload_deserialize_cdr(s_,sr_) \
    NANO_XRCE_ClientRepresentation_deserialize_cdr(NANO_MEMBER((s_),client_repr),(sr_))

#define NANO_XRCE_CreateClientPayload_serialize_cdr(s_,sr_) \
    NANO_XRCE_ClientRepresentation_serialize_cdr(NANO_MEMBER((s_),client_repr),(sr_))

#define NANO_XRCE_CreateClientPayload_serialized_size_cdr(s_,sr_) \
    NANO_XRCE_ClientRepresentation_serialized_size_cdr(NANO_MEMBER((s_),client_repr),(sr_))

#define NANO_XRCE_CreateClientPayload_alignment \
    NANO_XRCE_ClientRepresentation_alignment

/*i @} *//* nano_api_xrce_payload_req_createclient */

#if NANO_FEAT_OBJECT_CREATE
/******************************************************************************
 *                              CreatePayload
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_req_create
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_CreatePayload)

#define NANO_XRCE_CreatePayload_serialized_size_cdr(s_,sr_) \
(\
    NANO_XRCE_BaseObjectRequest_serialized_size_cdr(NANO_MEMBER((s_),base),(sr_)) + \
    NANO_XRCE_ObjectVariant_serialized_size_cdr(NANO_MEMBER((s_),object_repr),(sr_)) \
)

#define NANO_XRCE_CreatePayload_alignment \
    NANO_XRCE_BaseObjectRequest_alignment

/*i @} *//* nano_api_xrce_payload_req_create */

#endif /* NANO_FEAT_OBJECT_CREATE */

#if NANO_FEAT_OBJECT_INFO
/******************************************************************************
 *                              GetInfoPayload
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_req_getinfo
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_GetInfoPayload)

#define NANO_XRCE_GetInfoPayload_serialized_size_cdr(s_,sr_) \
(\
    NANO_XRCE_BaseObjectRequest_serialized_size_cdr(NANO_MEMBER((s_),base),(sr_)) + \
    NANO_XRCE_InfoMask_alignment \
)

#define NANO_XRCE_GetInfoPayload_alignment \
    NANO_XRCE_BaseObjectRequest_alignment

/*i @} *//* nano_api_xrce_payload_req_getinfo */

#endif /* NANO_FEAT_OBJECT_INFO */

#if NANO_FEAT_OBJECT_DELETE
/******************************************************************************
 *                              DeletePayload
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_req_delete
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_DeletePayload)

#define NANO_XRCE_DeletePayload_deserialize_cdr(s_,sr_) \
    NANO_XRCE_BaseObjectRequest_deserialize_cdr(NANO_MEMBER((s_),base),(sr_))

#define NANO_XRCE_DeletePayload_serialize_cdr(s_,sr_) \
    NANO_XRCE_BaseObjectRequest_serialize_cdr(NANO_MEMBER((s_),base),(sr_))

#define NANO_XRCE_DeletePayload_serialized_size_cdr(s_,sr_) \
    NANO_XRCE_BaseObjectRequest_serialized_size_cdr(NANO_MEMBER((s_),base),(sr_))

/*i @} *//* nano_api_xrce_payload_req_delete */

#endif /* NANO_FEAT_OBJECT_DELETE */

/******************************************************************************
 *                          StatusAgentPayload
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_req_statusagent
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_StatusAgentPayload)

#define NANO_XRCE_StatusAgentPayload_deserialize_cdr(s_,sr_) \
    NANO_XRCE_AgentRepresentation_deserialize_cdr(NANO_MEMBER((s_),agent_info),(sr_))

#define NANO_XRCE_StatusAgentPayload_serialize_cdr(s_,sr_) \
    NANO_XRCE_AgentRepresentation_serialize_cdr(NANO_MEMBER((s_),agent_info),(sr_))

#define NANO_XRCE_StatusAgentPayload_serialized_size_cdr(s_,sr_) \
    NANO_XRCE_AgentRepresentation_serialized_size_cdr(NANO_MEMBER((s_),agent_info),(sr_))

/*i @} *//* nano_api_xrce_payload_req_statusagent */

#if NANO_FEAT_OBJECT_CREATE || \
    NANO_FEAT_OBJECT_DELETE || \
    NANO_FEAT_OBJECT_INFO || \
    NANO_FEAT_SUBSCRIBE || \
    NANO_FEAT_PUBLISH

/******************************************************************************
 *                            StatusPayload
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_req_status
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_StatusPayload)

#define NANO_XRCE_StatusPayload_deserialize_cdr(s_,sr_) \
    NANO_XRCE_BaseObjectReply_deserialize_cdr(NANO_MEMBER((s_),base),(sr_))

#define NANO_XRCE_StatusPayload_serialize_cdr(s_,sr_) \
    NANO_XRCE_BaseObjectReply_serialize_cdr(NANO_MEMBER((s_),base),(sr_))

#define NANO_XRCE_StatusPayload_serialized_size_cdr(s_,sr_) \
    NANO_XRCE_BaseObjectReply_serialized_size_cdr(NANO_MEMBER((s_),base),(sr_))

/*i @} *//* nano_api_xrce_payload_req_status */

#endif /* NANO_FEAT_OBJECT_CREATE || \
            NANO_FEAT_OBJECT_DELETE || \
            NANO_FEAT_OBJECT_INFO || \
            NANO_FEAT_SUBSCRIBE || \
            NANO_FEAT_PUBLISH */

#if NANO_FEAT_OBJECT_INFO
/******************************************************************************
 *                          InfoPayload
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_req_info
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_InfoPayload)

#define NANO_XRCE_InfoPayload_serialized_size_cdr(s_,sr_) \
(\
    NANO_XRCE_BaseObjectReply_serialized_size_cdr(NANO_MEMBER((s_),base),(sr_)) + \
    NANO_XRCE_ObjectInfo_serialized_size_cdr(NANO_MEMBER((s_),object_info),(sr_)) \
)

/*i @} *//* nano_api_xrce_payload_req_info */

#endif /* NANO_FEAT_OBJECT_INFO */

#if NANO_FEAT_SUBSCRIBE
/******************************************************************************
 *                      ReadDataPayload
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_req_readdata
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_ReadDataPayload)

#define NANO_XRCE_ReadDataPayload_serialized_size_cdr(s_,sr_) \
(\
    NANO_XRCE_BaseObjectRequest_serialized_size_cdr(NANO_MEMBER((s_),base),(sr_)) + \
    NANO_XRCE_ReadSpecification_serialized_size_cdr(NANO_MEMBER((s_),read_spec),(sr_)) \
)

/*i @} *//* nano_api_xrce_payload_req_readdata */

#endif /* NANO_FEAT_SUBSCRIBE */

#if NANO_FEAT_PUBLISH
/******************************************************************************
 *                          WriteData
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_req_writedata
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_WriteDataPayload)

#define NANO_XRCE_WriteDataPayload_serialized_size_cdr(s_,sr_) \
(\
    NANO_XRCE_BaseObjectRequest_serialized_size_cdr(NANO_MEMBER((s_),base),(sr_)) + \
    NANO_XRCE_DataRepresentation_serialized_size_cdr(NANO_MEMBER((s_),data_repr),(sr_)) \
)

/*i @} *//* nano_api_xrce_payload_req_writedata */

#endif /* NANO_FEAT_PUBLISH */


#if NANO_FEAT_DATA
/******************************************************************************
 *                          DataPayload
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_req_data
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_DataPayload)

#define NANO_XRCE_DataPayload_serialized_size_cdr(s_,sr_) \
(\
    NANO_XRCE_RelatedObjectRequest_serialized_size_cdr(NANO_MEMBER((s_),base),(sr_)) + \
    NANO_XRCE_DataRepresentation_serialized_size_cdr(NANO_MEMBER((s_),data_repr),(sr_)) \
)

/*i @} *//* nano_api_xrce_payload_req_data */

#endif /* NANO_FEAT_DATA */

#if NANO_FEAT_RELIABILITY
/******************************************************************************
 *                          AckNackBitmap
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_support_acknackbmap
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_AckNackBitmap)


#define NANO_XRCE_AckNackBitmap_deserialize_cdr(s_,sr_) \
    NANO_Octets_deserialize_cdr(\
        NANO_MEMBER((s_),value),\
        (sr_),\
        NANO_XRCE_ACKNACKBITMAP_VALUE_LEN)

#define NANO_XRCE_AckNackBitmap_serialize_cdr(s_,sr_) \
    NANO_Octets_serialize_cdr(\
        NANO_MEMBER((s_),value),\
        (sr_),\
        NANO_XRCE_ACKNACKBITMAP_VALUE_LEN)

#define NANO_XRCE_AckNackBitmap_serialized_size_cdr(s_,sr_) \
    NANO_Octets_serialized_size_cdr(\
        NANO_MEMBER((s_),value),\
        (sr_),\
        NANO_XRCE_ACKNACKBITMAP_VALUE_LEN)

#define NANO_XRCE_AckNackBitmap_alignment \
    NANO_Octets_alignment

/*i @} *//* nano_api_xrce_payload_support_acknackbmap */


/******************************************************************************
 *                         AckNackPayload
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_req_acknack
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_AckNackPayload)

#define NANO_XRCE_AckNackPayload_serialized_size_cdr(s_,sr_) \
(\
    NANO_u16_serialized_size_cdr(NANO_MEMBER((s_),first_unacked_seq_num),(sr_)) + \
    NANO_XRCE_AckNackBitmap_serialized_size_cdr(NANO_MEMBER((s_),nack_bitmap),(sr_)) + \
    NANO_XRCE_StreamId_serialized_size_cdr(NANO_MEMBER((s_),member),(sr_)) \
)

/*i @} *//* nano_api_xrce_payload_req_acknack */

/******************************************************************************
 *                          HeartbeatPayload
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_req_heartbeat
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_HeartbeatPayload)

#define NANO_XRCE_HeartbeatPayload_serialized_size_cdr(s_,sr_) \
(\
    NANO_u16_serialized_size_cdr(NANO_MEMBER((s_),first_unacked_seq_num),(sr_)) + \
    NANO_u16_serialized_size_cdr(NANO_MEMBER((s_),last_unacked_seq_num),(sr_)) + \
    NANO_XRCE_StreamId_serialized_size_cdr(NANO_MEMBER((s_),stream_id),(sr_)) \
)

/*i @} *//* nano_api_xrce_payload_req_heartbeat */

#endif /* NANO_FEAT_RELIABILITY */

#if NANO_FEAT_TIME_SYNC
/******************************************************************************
 *                          TimestampPayload
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_req_timestamp
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_TimestampPayload)

#define NANO_XRCE_TimestampPayload_deserialize_cdr(s_,sr_) \
    NANO_Time_deserialize_cdr(NANO_MEMBER((s_),transmit_timestamp),(sr_))

#define NANO_XRCE_TimestampPayload_serialize_cdr(s_,sr_) \
    NANO_Time_serialize_cdr(NANO_MEMBER((s_),transmit_timestamp),(sr_))

#define NANO_XRCE_TimestampPayload_serialized_size_cdr(s_,sr_) \
    NANO_Time_serialized_size_cdr(NANO_MEMBER((s_),transmit_timestamp),(sr_))

/*i @} *//* nano_api_xrce_payload_req_timestamp */

/******************************************************************************
 *                         TimestampReplyPayload
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_req_timestampreply
 * @{
 */

NANO_CDR_STREAM_INTF_DECL(NANO_XRCE_TimestampReplyPayload)

#define NANO_XRCE_TimestampReplyPayload_serialized_size_cdr(s_,sr_) \
(\
    NANO_Time_serialized_size_cdr(NANO_MEMBER((s_),transmit_timestamp),(sr_)) + \
    NANO_Time_serialized_size_cdr(NANO_MEMBER((s_),receive_timestamp),(sr_)) + \
    NANO_Time_serialized_size_cdr(NANO_MEMBER((s_),originate_timestamp),(sr_)) \
)
#endif /* NANO_FEAT_TIME_SYNC */

/*i @} *//* nano_api_xrce_payload_req_timestampreply */

#endif /* NANO_FEAT_CDR */

#endif /* nano_core_cdr_xrce_h */