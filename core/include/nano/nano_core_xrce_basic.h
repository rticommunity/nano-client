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

/**
 * @file nano_core_xrce_basic.h
 * @brief Basic XRCE Types.
 * 
 * This header file contains definitions for core types used by the XRCE
 * protocol and throughout its implementation.
 * 
 */

#ifndef nano_core_xrce_basic_h
#define nano_core_xrce_basic_h


#if NANO_FEAT_RESULTSTATUS
/******************************************************************************
 *                               StatusValue
 ******************************************************************************/
/**
 * @addtogroup nano_api_clientrequests
 * @{
 */
/*e
 * @brief An enumeration of return codes for XRCE operations.
 */
typedef NANO_u8 NANO_XRCE_StatusValue;

/*e
 * @brief An invalid return status that should never be returned.
 * 
 * This value is *non-standard*.
 */
#define NANO_XRCE_STATUS_INVALID            ((NANO_XRCE_StatusValue)0xFF)

/*e
 * @brief Return status for operations that are carried out successfully.
 */
#define NANO_XRCE_STATUS_OK                 ((NANO_XRCE_StatusValue)0x00)
/*e
 * @brief Return status for operations that are carried out successfully, and
 * which matched existing resources.
 */
#define NANO_XRCE_STATUS_OK_MATCHES         ((NANO_XRCE_StatusValue)0x01)
/*e
 * @brief Return status for operations which failed because of an error in a
 * related DDS operation.
 */
#define NANO_XRCE_STATUS_ERR_DDS_ERROR      ((NANO_XRCE_StatusValue)0x80)
/*e
 * @brief Return status for operations which failed because the request does not
 * match existing resources..
 */
#define NANO_XRCE_STATUS_ERR_MISMATCH       ((NANO_XRCE_StatusValue)0x81)
/*e
 * @brief Return status for operations which failed because the requested
 * resource already exists.
 */
#define NANO_XRCE_STATUS_ERR_ALREADY_EXISTS ((NANO_XRCE_StatusValue)0x82)
/*e
 * @brief Return status for operations which failed because of an authorization
 * issue.
 */
#define NANO_XRCE_STATUS_ERR_DENIED         ((NANO_XRCE_StatusValue)0x83)
/*e
 * @brief Return status for operations which failed because of a reference to
 * an unknown XRCE Object.
 */
#define NANO_XRCE_STATUS_ERR_UNKNOWN_REFERENCE  ((NANO_XRCE_StatusValue)0x84)
/*e
 * @brief Return status for operations which failed because of a malformed
 * request.
 */
#define NANO_XRCE_STATUS_ERR_INVALID_DATA   ((NANO_XRCE_StatusValue)0x85)
/*e
 * @brief Return status for operations which are not supported or deemed
 * invalid in any other way.
 */
#define NANO_XRCE_STATUS_ERR_INCOMPATIBLE   ((NANO_XRCE_StatusValue)0x86)
/*e
 * @brief Return status for operations which failed because of lack of
 * resources.
 */
#define NANO_XRCE_STATUS_ERR_RESOURCES      ((NANO_XRCE_StatusValue)0x87)

/*i
 * @brief Convert a NANO_RetCode to a NANO_XRCE_StatusValue.
 * 
 * @param rc the NANO_RetCode to convert.
 * @return A NANO_XRCE_StatusValue corresponding to the specified return code.
 */
NANO_XRCE_StatusValue
NANO_XRCE_StatusValue_from_rc(NANO_RetCode rc);

#define NANO_XRCE_StatusValue_from_rc(rc_) \
    ((rc_) == NANO_RETCODE_OK)? NANO_XRCE_STATUS_OK : \
    ((rc_) == NANO_RETCODE_OUT_OF_RESOURCES)? NANO_XRCE_STATUS_ERR_RESOURCES : \
    ((rc_) == NANO_RETCODE_ALREADY_EXISTS)? NANO_XRCE_STATUS_ERR_ALREADY_EXISTS : \
    ((rc_) == NANO_RETCODE_UNKNOWN_REFERENCE)? NANO_XRCE_STATUS_ERR_UNKNOWN_REFERENCE : \
    ((rc_) == NANO_RETCODE_INVALID_ARGS)? NANO_XRCE_STATUS_ERR_INVALID_DATA : \
        NANO_XRCE_STATUS_ERR_INCOMPATIBLE

/** @} *//* nano_api_clientrequests */

/******************************************************************************
 *                               ResultStatus
 ******************************************************************************/
/**
 * @addtogroup nano_api_clientrequests
 * @{
 */
/*e
 * @brief A data type to convey the outcome of an XRCE operation.
 * 
 */
typedef struct NANODllExport NANO_XRCE_ResultStatusI
{
    /*e
     * @brief  The NANO_XRCE_StatusValue representing the operation's outcome.
     */
    NANO_XRCE_StatusValue status;
    /*e
     * @brief A vendor-specific status flag for further information about
     * the operation's outcome. For all NANO operations, this field is set
     * to values of NANO_RetCode.
     */
    NANO_u8 implementation_status;
} NANO_XRCE_ResultStatus;

#define NANO_XRCE_RESULTSTATUS_INITIALIZER \
{\
    NANO_XRCE_STATUS_INVALID, /* status */ \
    NANO_RETCODE_ERROR /* implementation_status */ \
}

#define NANO_XRCE_RESULTSTATUS_SERIALIZED_SIZE_MAX \
    (sizeof(NANO_XRCE_StatusValue) + sizeof(NANO_u8))

/*e
 * @brief TODO
 * 
 * @param self 
 * @return NANO_bool 
 */
NANO_bool
NANO_XRCE_ResultStatus_success(NANO_XRCE_ResultStatus *const self);

#define NANO_XRCE_ResultStatus_success(s_) \
    ((s_)->status == NANO_XRCE_STATUS_OK ||\
        (s_)->status == NANO_XRCE_STATUS_OK_MATCHES)


NANO_bool
NANO_XRCE_ResultStatus_initialize(
    NANO_XRCE_ResultStatus *const self);

#define NANO_XRCE_ResultStatus_initialize(s_) \
    ((s_)->status = NANO_XRCE_STATUS_INVALID, \
    (s_)->implementation_status = NANO_RETCODE_ERROR,\
    NANO_BOOL_TRUE)


void
NANO_XRCE_ResultStatus_deserialize(
    NANO_XRCE_ResultStatus *const self,
    NANO_u8 *data,
    NANO_bool swap);

#define NANO_XRCE_ResultStatus_deserialize(s_,d_,sw_) \
    NANO_OSAPI_Memory_copy((s_),(d_),\
        NANO_XRCE_RESULTSTATUS_SERIALIZED_SIZE_MAX)

void
NANO_XRCE_ResultStatus_serialize(
    NANO_XRCE_ResultStatus *const self,
    NANO_u8 *data,
    NANO_bool swap);

#define NANO_XRCE_ResultStatus_serialize(s_,d_,sw_) \
    NANO_OSAPI_Memory_copy((d_),(s_),\
        NANO_XRCE_RESULTSTATUS_SERIALIZED_SIZE_MAX)

/** @} *//* nano_api_clientrequests */
#endif /* NANO_FEAT_RESULTSTATUS */

/******************************************************************************
 *                                 Cookie
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_header
 * @{
 */
/*e
 * @brief Length of a NANO_XRCE_Cookie's value, in octets.
 */
#define NANO_XRCE_COOKIE_VALUE_LEN       4

/*e
 * @brief A canary value to identify XRCE messages on the wire.
 */
typedef struct NANODllExport NANO_XRCE_CookieI
{
    /*e
     * @brief The cookie's contents.
     */
    NANO_u8 value[NANO_XRCE_COOKIE_VALUE_LEN];

} NANO_XRCE_Cookie;

NANO_bool
NANO_XRCE_Cookie_initialize(NANO_XRCE_Cookie *const self);

#define NANO_XRCE_Cookie_initialize(s_) \
    (NANO_OSAPI_Memory_zero((s_)->value, NANO_XRCE_COOKIE_VALUE_LEN),\
        NANO_BOOL_TRUE)

/*e
 * @brief Constant value used to mark and detect valid XRCE messages.
 */
#define NANO_XRCE_COOKIE_DEFAULT                 { { 0x58, 0x52, 0x43, 0x45 } }

/*e
 * @brief Check if a cookie is valid.
 * 
 * @param self the cookie to check.
 * @return NANO_BOOL_TRUE if the cookie has a valid value, NANO_BOOL_FALSE
 * otherwise.
 */
NANO_bool
NANO_XRCE_Cookie_is_valid(const NANO_XRCE_Cookie *const self);

#define NANO_XRCE_Cookie_is_valid(s_) \
    ((((s_)->value[0]) == (0x58)) && \
     (((s_)->value[1]) == (0x52)) && \
     (((s_)->value[2]) == (0x43)) && \
     (((s_)->value[3]) == (0x45)))

#define NANO_XRCE_COOKIE_VALID_U32              0x58524345

/*i @} *//* nano_api_xrce_header */

/******************************************************************************
 *                                Version
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_header
 * @{
 */
/*e
 * @brief Length of a NANO_XRCE_Version's value, in octets.
 */
#define NANO_XRCE_VERSION_VALUE_LEN       2

/*e
 * @brief A canary value to identify XRCE messages on the wire.
 */
typedef struct NANODllExport NANO_XRCE_VersionI
{
    /*e
     * @brief The version's octet components.
     */
    NANO_u8 value[NANO_XRCE_VERSION_VALUE_LEN];

} NANO_XRCE_Version;

NANO_bool
NANO_XRCE_Version_initialize(NANO_XRCE_Version *const self);


#define NANO_XRCE_Version_initialize(s_) \
    (NANO_OSAPI_Memory_zero((s_)->value, NANO_XRCE_VERSION_VALUE_LEN),\
        NANO_BOOL_TRUE)

#define NANO_XRCE_VERSION_MAJOR_BYTE    0
#define NANO_XRCE_VERSION_MINOR_BYTE    1

/*e
 * @brief Major version number of supported XRCE protocol version.
 */
#define NANO_XRCE_VERSION_MAJOR         0x01
/*e
 * @brief Minor version number of supported XRCE protocol version.
 */
#define NANO_XRCE_VERSION_MINOR         0x00

/*i
 * @brief Static initializer macro for NANO_XRCE_Version instances.
 * 
 */
#define NANO_XRCE_VERSION_INITIALIZER(maj_,min_) \
    { { (maj_), (min_) }}

/*e
 * @brief Full version identifier of supported XRCE protocol version.
 */
#define NANO_XRCE_VERSION_DEFAULT \
    NANO_XRCE_VERSION_INITIALIZER(\
        NANO_XRCE_VERSION_MAJOR, NANO_XRCE_VERSION_MINOR)

/*i
 * @brief Check if version is compatible with this implementation.
 * 
 * @param self the version to check.
 * @return NANO_BOOL_TRUE if the version is compatible, NANO_BOOL_FALSE
 * otherwise.
 */
NANO_bool
NANO_XRCE_Version_is_compatible(const NANO_XRCE_Version *const self);

#define NANO_XRCE_Version_is_compatible(s_) \
    (NANO_XRCE_Version_get_major(s_) == NANO_XRCE_VERSION_MAJOR)

/*e
 * @brief Get the "major" component of a version.
 * 
 * @param self the version to inspect.
 * @return A NANO_u8 containing the "major" component of the version.
 */
NANO_u8
NANO_XRCE_Version_get_major(const NANO_XRCE_Version *const self);

#define NANO_XRCE_Version_get_major(s_) \
    ((s_)->value[NANO_XRCE_VERSION_MAJOR_BYTE])

/*e
 * @brief Get the "minor" component of a version.
 * 
 * @param self the version to inspect.
 * @return A NANO_u8 containing the "minor" component of the version.
 */
NANO_u8
NANO_XRCE_Version_get_minor(const NANO_XRCE_Version *const self);

#define NANO_XRCE_Version_get_minor(s_) \
    ((s_)->value[NANO_XRCE_VERSION_MINOR_BYTE])


/*i @} *//* nano_api_xrce_header */

/******************************************************************************
 *                                VendorId
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_header
 * @{
 */
/*e
 * @brief Length of a NANO_XRCE_VendorId's value, in octets.
 */
#define NANO_XRCE_VENDORID_VALUE_LEN       2

/*e
 * @brief Identifier for implementors of the XRCE protocol.
 */
typedef struct NANODllExport NANO_XRCE_VendorIdI
{
    /*e
     * @brief The vendor identifier's octet components.
     */
    NANO_u8 value[NANO_XRCE_VENDORID_VALUE_LEN];

} NANO_XRCE_VendorId;


NANO_bool
NANO_XRCE_VendorId_initialize(NANO_XRCE_VendorId *const self);


#define NANO_XRCE_VendorId_initialize(s_) \
    (NANO_OSAPI_Memory_zero((s_)->value, NANO_XRCE_VENDORID_VALUE_LEN),\
        NANO_BOOL_TRUE)

/*i
 * @brief Value of first octet of an invalid XRCE implementor's identifier.
 */
#define NANO_XRCE_VENDORID_INVALID_1   0x00
/*i
 * @brief Value of second octet of an invalid XRCE implementor's identifier.
 */
#define NANO_XRCE_VENDORID_INVALID_2   0x00
/*i
 * @brief Full identifier of an invalid XRCE implementor.
 */
#define NANO_XRCE_VENDORID_INVALID \
    { { NANO_XRCE_VENDORID_INVALID_1, NANO_XRCE_VENDORID_INVALID_2 }}
/*e
 * @brief Full identifier of Nano's implementor.
 */
#define NANO_XRCE_VENDORID_DEFAULT             { {0x01, 0x01} }

/*i @} *//* nano_api_xrce_header */


#if NANO_FEAT_REQUEST
/******************************************************************************
 *                              ObjectKind
 ******************************************************************************/
/**
 * @addtogroup nano_api_obj
 * @{
 */
/*e
 * @brief A type to represent the different kinds of XRCE objects.
 */
typedef NANO_u8 NANO_XRCE_ObjectKind;

NANO_bool
NANO_XRCE_ObjectKind_initialize(
    NANO_XRCE_ObjectKind *const self);

#define NANO_XRCE_ObjectKind_initialize(s_) \
    (*(s_) = NANO_XRCE_OBJK_INVALID, NANO_BOOL_TRUE)

/*e
 * @brief NANO_XRCE_ObjectKind value to identify invalid XRCE Objects.
 */
#define NANO_XRCE_OBJK_INVALID          ((NANO_XRCE_ObjectKind)0x00)
/*e
 * @brief NANO_XRCE_ObjectKind value for XRCE DomainParticipant instances.
 */
#define NANO_XRCE_OBJK_PARTICIPANT      ((NANO_XRCE_ObjectKind)0x01)
/*e
 * @brief NANO_XRCE_ObjectKind value for XRCE Topic instances.
 */
#define NANO_XRCE_OBJK_TOPIC            ((NANO_XRCE_ObjectKind)0x02)
/*e
 * @brief NANO_XRCE_ObjectKind value for XRCE Publisher instances.
 */
#define NANO_XRCE_OBJK_PUBLISHER        ((NANO_XRCE_ObjectKind)0x03)
/*e
 * @brief NANO_XRCE_ObjectKind value for XRCE Subscriber instances.
 */
#define NANO_XRCE_OBJK_SUBSCRIBER       ((NANO_XRCE_ObjectKind)0x04)
/*e
 * @brief NANO_XRCE_ObjectKind value for XRCE DataWriter instances.
 */
#define NANO_XRCE_OBJK_DATAWRITER       ((NANO_XRCE_ObjectKind)0x05)
/*e
 * @brief NANO_XRCE_ObjectKind value for XRCE DataReader instances.
 */
#define NANO_XRCE_OBJK_DATAREADER       ((NANO_XRCE_ObjectKind)0x06)
/*e
 * @brief NANO_XRCE_ObjectKind value for XRCE Domain instances.
 */
#define NANO_XRCE_OBJK_DOMAIN           ((NANO_XRCE_ObjectKind)0x07)
/*e
 * @brief NANO_XRCE_ObjectKind value for XRCE Type instances.
 */
#define NANO_XRCE_OBJK_TYPE             ((NANO_XRCE_ObjectKind)0x0A)
/*e
 * @brief NANO_XRCE_ObjectKind value for XRCE QosProfile instances.
 */
#define NANO_XRCE_OBJK_QOSPROFILE       ((NANO_XRCE_ObjectKind)0x0B)
/*e
 * @brief NANO_XRCE_ObjectKind value for XRCE Application instances.
 */
#define NANO_XRCE_OBJK_APPLICATION      ((NANO_XRCE_ObjectKind)0x0C)
/*e
 * @brief NANO_XRCE_ObjectKind value for XRCE Agent instances.
 */
#define NANO_XRCE_OBJK_AGENT            ((NANO_XRCE_ObjectKind)0x0D)
/*e
 * @brief NANO_XRCE_ObjectKind value for XRCE Client instances.
 */
#define NANO_XRCE_OBJK_CLIENT           ((NANO_XRCE_ObjectKind)0x0E)

/*e
 * @brief Check if a NANO_XRCE_ObjectKind is valid.
 * 
 * @param self the value to check
 * @return NANO_BOOL_TRUE if the specified value is one of the known
 * constants, NANO_BOOL_FALSE otherwise.
 */
NANO_bool
NANO_XRCE_ObjectKind_is_valid(const NANO_XRCE_ObjectKind self);

#define NANO_XRCE_ObjectKind_is_valid(k_) \
    ((k_) == NANO_XRCE_OBJK_PARTICIPANT || \
    (k_) == NANO_XRCE_OBJK_TOPIC || \
    (k_) == NANO_XRCE_OBJK_PUBLISHER || \
    (k_) == NANO_XRCE_OBJK_SUBSCRIBER || \
    (k_) == NANO_XRCE_OBJK_DATAWRITER || \
    (k_) == NANO_XRCE_OBJK_DATAREADER || \
    (k_) == NANO_XRCE_OBJK_DOMAIN || \
    (k_) == NANO_XRCE_OBJK_TYPE || \
    (k_) == NANO_XRCE_OBJK_QOSPROFILE || \
    (k_) == NANO_XRCE_OBJK_APPLICATION || \
    (k_) == NANO_XRCE_OBJK_AGENT || \
    (k_) == NANO_XRCE_OBJK_CLIENT)


/** @} *//* nano_api_obj */

/******************************************************************************
 *                                ObjectId
 ******************************************************************************/
/**
 * @addtogroup nano_api_obj
 * @{
 */
/*i
 * @brief Length of a NANO_XRCE_ObjectId's value, in octets.
 */
#define NANO_XRCE_OBJECTID_VALUE_LEN       2

/*e
 * @brief Identifier for implementors of the XRCE protocol.
 */
typedef struct NANODllExport NANO_XRCE_ObjectIdI
{
    /*e
     * @brief The XRCE ObjectId's octet components.
     */
    NANO_u8 value[NANO_XRCE_OBJECTID_VALUE_LEN];

#if NANO_CPP
    NANO_XRCE_ObjectIdI()
    {
        NANO_OSAPI_Memory_zero(this->value, sizeof(this->value));
    }
#endif /* NANO_CPP */

} NANO_XRCE_ObjectId;

NANO_bool
NANO_XRCE_ObjectId_initialize(
    NANO_XRCE_ObjectId *const self);

#define NANO_XRCE_ObjectId_initialize(s_) \
    (NANO_OSAPI_Memory_zero((s_)->value, NANO_XRCE_OBJECTID_VALUE_LEN), \
        NANO_BOOL_TRUE)

/*e
 * @brief Initializer for for an invalid NANO_XRCE_ObjectId.
 */
#define NANO_XRCE_OBJECTID_INVALID \
    { { 0x00, 0x00 }}

/*i
 * @brief 
 */
#define NANO_XRCE_OBJECTID_AGENT \
    { { 0xFF, 0xFD }}

/*i
 * @brief 
 */
#define NANO_XRCE_OBJECTID_CLIENT \
    { { 0xFF, 0xFE }}

/*i
 * @brief 
 */
#define NANO_XRCE_OBJECTID_SESSION \
    { { 0xFF, 0xFF }}

/*e
 * @brief TODO
 * 
 * @param self 
 * @param kind 
 * @param base_id 
 */
void
NANO_XRCE_ObjectId_set(
    const NANO_XRCE_ObjectId *const self,
    const NANO_XRCE_ObjectKind kind,
    const NANO_u16 base_id);

#define NANO_XRCE_ObjectId_set(s_,k_,b_) \
{\
    NANO_XRCE_ObjectId_from_u16((s_),(b_));\
    NANO_XRCE_ObjectId_combine((s_),(k_),(s_));\
}


/*e
 * @brief Combine a NANO_XRCE_ObjectId with a type identifier.
 * 
 * @param self      The NANO_XRCE_ObjectId to be used as prefix.
 * @param kind      The NANO_XRCE_ObjectKind describing the type of XRCE object.
 * @param id_out    The combined NANO_XRCE_ObjectId.
 */
void
NANO_XRCE_ObjectId_combine(
    const NANO_XRCE_ObjectId *const self,
    const NANO_XRCE_ObjectKind kind,
    NANO_XRCE_ObjectId *const id_out);

#define NANO_XRCE_ObjectId_combine(s_,k_,i_) \
{\
    (i_)->value[0] = (s_)->value[0]; \
    (i_)->value[1] = ((s_)->value[1] & 0xF0) | \
                        ((NANO_XRCE_ObjectKind)(k_) & 0x0F); \
}

/*e
 * @brief Extract a NANO_XRCE_ObjectKind value from a NANO_XRCE_ObjectId.
 * 
 * @param self      The NANO_XRCE_ObjectId to analyze.
 * 
 * @return a NANO_u16 value, encoded in Big-Endian endianness.
 */
NANO_XRCE_ObjectKind
NANO_XRCE_ObjectId_kind(NANO_XRCE_ObjectId *self);

#define NANO_XRCE_ObjectId_kind(o_) \
    ((NANO_XRCE_ObjectKind)((o_)->value[1] & 0x0F))

/*e
 * @brief Convert a NANO_XRCE_ObjectId to a NANO_u16
 * 
 * @param self      The NANO_XRCE_ObjectId to convert.
 * 
 * @return a NANO_u16 value, encoded in Big-Endian endianness.
 */
NANO_u16
NANO_XRCE_ObjectId_to_u16(const NANO_XRCE_ObjectId *const self);

/*e
 * @brief Fill a NANO_XRCE_ObjectId from a NANO_u16
 * 
 * @param self      The NANO_XRCE_ObjectId to fill in
 * @param val       The value to set.
 */
void
NANO_XRCE_ObjectId_from_u16(NANO_XRCE_ObjectId *const self, NANO_u16 val);

#define NANO_XRCE_ObjectId_to_u16(s_) \
    NANO_u16_from_ptr_be((s_)->value)

#define NANO_XRCE_ObjectId_from_u16(s_,v_) \
    NANO_u16_set_ptr_be((s_)->value, (v_))

/*e
 * @brief TODO
 * 
 * @param self 
 * @param other 
 * @return NANO_i8 
 */
NANO_i8
NANO_XRCE_ObjectId_compare(
    const NANO_XRCE_ObjectId *const self,
    const NANO_XRCE_ObjectId *const other);

#define NANO_XRCE_ObjectId_compare(s_,o_) \
    NANO_OSAPI_Memory_compare(\
        (s_)->value, (o_)->value, NANO_XRCE_OBJECTID_VALUE_LEN)

/** @} *//* nano_api_obj */

#endif /* NANO_FEAT_REQUEST */

/******************************************************************************
 *                                SessionId
 ******************************************************************************/
/**
 * @addtogroup nano_api_xrcesupport
 * @{
 */

/*e
 * @brief Identifier for sessions created by a client on an agent.
 */
typedef NANO_u8 NANO_XRCE_SessionId;

NANO_bool
NANO_XRCE_SessionId_initialize(
    NANO_XRCE_SessionId *const self);

#define NANO_XRCE_SessionId_initialize(s_) \
    (*(s_) = NANO_XRCE_SESSIONID_NONE_WITH_CLIENT, NANO_BOOL_TRUE)

/*e
 * @brief Constant to indicate lack of a session, but presence of a client key.
 */
#define NANO_XRCE_SESSIONID_NONE_WITH_CLIENT         (0x00)

/*e
 * @brief Constant to indicate lack of a session, and a client key.
 */
#define NANO_XRCE_SESSIONID_NONE_WITHOUT_CLIENT      (0x80)

/*e
 * @brief Check if the session includes a client key.
 * 
 * @param id the session to inspect
 * @return NANO_BOOL_TRUE if the session includes a client key, NANO_BOOL_FALSE
 * otherwise.
 */
NANO_bool
NANO_XRCE_SessionId_has_client_key(const NANO_XRCE_SessionId id);

#define NANO_XRCE_SessionId_has_client_key(id_) \
    (!(((NANO_XRCE_SessionId)(id_)) & 0x80))

#define NANO_XRCE_INLINEMESSAGEHEADER_WITH_KEY_SIZE             12

#define NANO_XRCE_INLINEMESSAGEHEADER_WITHOUT_KEY_SIZE          8

/*e
 * @brief 
 * 
 * @param id 
 * @return NANO_u16 
 */
NANO_u16
NANO_XRCE_SessionId_header_size(const NANO_XRCE_SessionId id);
#define NANO_XRCE_SessionId_header_size(s_) \
    ((NANO_XRCE_SessionId_has_client_key(s_))? \
        NANO_XRCE_INLINEMESSAGEHEADER_WITH_KEY_SIZE : \
        NANO_XRCE_INLINEMESSAGEHEADER_WITHOUT_KEY_SIZE)


/** @} *//* nano_api_xrcesupport */


/******************************************************************************
 *                                StreamId
 ******************************************************************************/
/**
 * @addtogroup nano_api_xrcesupport
 * @{
 */

/*e
 * @brief Identifier for streams of XRCE messages.
 */
typedef NANO_u8 NANO_XRCE_StreamId;


NANO_bool
NANO_XRCE_StreamId_initialize(
    NANO_XRCE_SessionId *const self);

#define NANO_XRCE_StreamId_initialize(s_) \
    (*(s_) = NANO_XRCE_STREAMID_NONE, NANO_BOOL_TRUE)

/*e
 * @brief Constant to indicate the absence of a stream.
 */
#define NANO_XRCE_STREAMID_NONE                      (0x00)

/*e
 * @brief Constant to indicate the built-in best-effort stream.
 */
#define NANO_XRCE_STREAMID_BUILTIN_BEST_EFFORTS      (0x01)

/*e
 * @brief Constant to indicate the built-in reliable stream.
 */
#define NANO_XRCE_STREAMID_BUILTIN_RELIABLE          (0x80)

/*e
 * @brief Check if the stream is reliable.
 * 
 * @param id the stream to inspect
 * @return NANO_BOOL_TRUE if the stream is reliable, NANO_BOOL_FALSE
 * otherwise.
 */
NANO_bool
NANO_XRCE_StreamId_is_reliable(const NANO_XRCE_StreamId id);

#define NANO_XRCE_StreamId_is_reliable(id_) \
    (((NANO_XRCE_StreamId)(id_)) >= 0x80)


/*i
 * @brief Check if a stream's messages should be ordered according to their
 * sequence number.
 * 
 * @param id the stream to inspect
 * @return NANO_BOOL_TRUE if the stream is ordered, NANO_BOOL_FALSE
 * otherwise.
 */
NANO_bool
NANO_XRCE_StreamId_is_ordered(const NANO_XRCE_StreamId id);

#define NANO_XRCE_StreamId_is_ordered(id_) \
    (((NANO_XRCE_StreamId)(id_)) != NANO_XRCE_STREAMID_NONE)


/*e
 * @brief Check if the stream is a built-in one.
 * 
 * @param id the stream to inspect
 * @return NANO_BOOL_TRUE if the stream is a built-in one, NANO_BOOL_FALSE
 * otherwise.
 */
NANO_bool
NANO_XRCE_StreamId_is_builtin(const NANO_XRCE_StreamId id);

#define NANO_XRCE_StreamId_is_builtin(id_) \
    (((NANO_XRCE_StreamId)(id_)) == NANO_XRCE_STREAMID_NONE || \
        ((NANO_XRCE_StreamId)(id_)) == \
            NANO_XRCE_STREAMID_BUILTIN_BEST_EFFORTS || \
        ((NANO_XRCE_StreamId)(id_)) == \
            NANO_XRCE_STREAMID_BUILTIN_RELIABLE)

/** @} *//* nano_api_xrcesupport */


/******************************************************************************
 *                             SequenceNumber
 ******************************************************************************/
/**
 * @addtogroup nano_api_xrcesupport
 * @{
 */

/*e
 * @brief The SERIAL_BITS constant for the implementation of IETF RFC-1982.
 */
#define NANO_XRCE_SEQNUM_SERIAL_BITS     16

/*e
 * @brief Length of a NANO_XRCE_SeqNum's value, in octets.
 */
#define NANO_XRCE_SEQNUM_VALUE_LEN       (NANO_XRCE_SEQNUM_SERIAL_BITS/8)

/*e
 * @brief A sequence number type as defined by
 * [IETF RFC-1982](https://tools.ietf.org/html/rfc1982) with SERIAL_BITS
 * set to 16.
 */
typedef struct NANODllExport NANO_XRCE_SeqNumI
{
    /*e
     * @brief the value of the sequence number in octets.
     */
    NANO_u8 value[NANO_XRCE_SEQNUM_VALUE_LEN];

#if NANO_CPP

    NANO_XRCE_SeqNumI()
    {
        NANO_OSAPI_Memory_zero(this->value,sizeof(this->value));
    }

#endif /* NANO_CPP */
} NANO_XRCE_SeqNum;

NANO_bool
NANO_XRCE_SeqNum_initialize(NANO_XRCE_SeqNum *const self);

#define NANO_XRCE_SeqNum_initialize(s_) \
    (NANO_OSAPI_Memory_zero((s_),NANO_XRCE_SEQNUM_VALUE_LEN), \
        NANO_BOOL_TRUE)

/*e
 * @brief Static initializer for NANO_XRCE_SeqNum value.
 */
#define NANO_XRCE_SEQNUM_INITIALIZER \
    { { 0 } /* value */ }

/*e
 * @brief Add a 16-bit positive signed integer to a sequence numbers.
 * 
 * @param self The sequence number.
 * @param add The number to add.
 * @param res_out The resulting sequence number.
 * @return NANO_RETCODE_OK if the result was successfully computer,
 * NANO_RETCODE_INVALID_ARGS if the specified number was invalid (e.g.
 * negative).
 */
void
NANO_XRCE_SeqNum_add(
    const NANO_XRCE_SeqNum *const self,
    const NANO_i16 add,
    NANO_XRCE_SeqNum *const res_out);

#define NANO_XRCE_SeqNum_add(s_,a_,ro_) \
{\
    NANO_u16 s1_ = 0,\
             s2_ = 0;\
    NANO_PCOND((a_) < NANO_I16_MAX && (a_) > NANO_I16_MIN)\
    s1_ = NANO_XRCE_SeqNum_to_u16((s_));\
    s2_ = (NANO_u16) \
        ((((NANO_i32)s1_) + ((NANO_i32)(a_))) % ((NANO_i32)NANO_U16_MAX));\
    NANO_XRCE_SeqNum_from_u16(ro_, s2_);\
}

/*e
 * @brief TODO
 * 
 * @param self 
 */
void
NANO_XRCE_SeqNum_plusplus(const NANO_XRCE_SeqNum *const self);

#define NANO_XRCE_SeqNum_plusplus(s_) \
    NANO_XRCE_SeqNum_add((s_),1,(s_))

/*e
 * @brief Compare a sequence numbers to another.
 * 
 * Two sequence numbers, s1 and s2, and their NANO_u16 representations,
 * u1 and u2, can only be compared if:
 * 
 *   * `u1 == u2`
 *   * `u1 > u2 && (u1 - u2) == (1 << (NANO_XRCE_SEQNUM_SERIAL_BITS-1))`
 *   * `u1 < u2 && (u2 - u1) == (1 << (NANO_XRCE_SEQNUM_SERIAL_BITS-1))`
 * 
 * @param self The sequence number.
 * @param other The other sequence number
 * @param cmp_out 0 if the two sequence numbers are the same, a negative value
 * if self < other, or a positive value if self > other.
 * @return NANO_RETCODE_OK if the comparison could be sucessfully computer,
 * NANO_RETCODE_INVALID_ARGS if the two sequence numbers cannot be uniquely
 * compared according to "sequence number arithmetics".
 */
void
NANO_XRCE_SeqNum_compare(
    const NANO_XRCE_SeqNum *const self,
    const NANO_XRCE_SeqNum *const other,
    NANO_i8 *cmp_out);

/** @} *//* nano_api_xrcesupport */

/******************************************************************************
 *                                ClientKey
 ******************************************************************************/
/**
 * @addtogroup nano_api_xrcesupport
 * @{
 */

/*e
 * @brief Length of a NANO_XRCE_ClientKey's value, in octets.
 */
#define NANO_XRCE_CLIENTKEY_VALUE_LEN       4

/*e
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_ClientKeyI
{
    /*e
     * @brief The client key as octets.
     */
    NANO_u8 value[NANO_XRCE_CLIENTKEY_VALUE_LEN];

#if NANO_CPP

    NANO_XRCE_ClientKeyI()
    {
        NANO_OSAPI_Memory_zero(this->value, sizeof(this->value));
    }

#endif /* NANO_CPP */
} NANO_XRCE_ClientKey;

NANO_bool
NANO_XRCE_ClientKey_initialize(NANO_XRCE_ClientKey *const self);

#define NANO_XRCE_ClientKey_initialize(s_) \
    (NANO_XRCE_ClientKey_from_u32((s_), 0x00000000), NANO_BOOL_TRUE)

/*e
 * @brief A static initializer for an invalid client key.
 */
#define NANO_XRCE_CLIENTKEY_INVALID         { { 0x00, 0x00, 0x00, 0x00 } }

/*e
 * @brief Check if a NANO_XRCE_ClientKey contains a valid value.
 *
 * @param self The key to check.
 * @return NANO_BOOL_TRUE if the key is valid, NANO_BOOL_FALSE otherwise.
 */
NANO_bool
NANO_XRCE_ClientKey_is_valid(const NANO_XRCE_ClientKey *const self);

#define NANO_XRCE_ClientKey_is_valid(k_) \
(\
    (k_)->value[0] != 0 &&\
    (k_)->value[1] != 0 && \
    (k_)->value[2] != 0 && \
    (k_)->value[3] != 0\
)

/*i
 * @brief Copy a client key from another.
 * 
 * @param self The key to copy into.
 * @param other The key to copy from.
 */
void
NANO_XRCE_ClientKey_copy(
    NANO_XRCE_ClientKey *const self,
    const NANO_XRCE_ClientKey *const other);

#define NANO_XRCE_ClientKey_copy(s_,o_) \
{\
    (s_)->value[0] = (o_)->value[0];\
    (s_)->value[1] = (o_)->value[1];\
    (s_)->value[2] = (o_)->value[2];\
    (s_)->value[3] = (o_)->value[3];\
}

/*e
 * @brief TODO
 * 
 * @param self 
 * @param other 
 * @return NANO_i8 
 */
NANO_i8
NANO_XRCE_ClientKey_compare(
    const NANO_XRCE_ClientKey *const self,
    const NANO_XRCE_ClientKey *const other);

#define NANO_XRCE_ClientKey_compare(s_,o_) \
    ((NANO_i8)NANO_OSAPI_Memory_compare(\
                (s_)->value,(o_)->value, NANO_XRCE_CLIENTKEY_VALUE_LEN))


/** @} *//* nano_api_xrcesupport */

#if NANO_FEAT_OBJECT
/******************************************************************************
 *                          ObjectReference
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_objrepr
 * @{
 */

/* Implementation as an externally-managed string */
typedef struct NANODllExport NANO_XRCE_ObjectRefI
{
    char* value;
} NANO_XRCE_ObjectRef;

#define NANO_XRCE_OBJECTREF_INITIALIZER \
{ \
    NULL /* value */\
}

/*i
 * @brief Set the string value of a NANO_XRCE_ObjectRef instance from another
 * string.
 * 
 * @param self The reference to set.
 * @param val The string from which to set the reference.
 * @return NANO_RETCODE_OK if the reference was correctly set,
 * NANO_RETCODE_INVALID_ARGS if the input string was found to be an invalid
 * reference according to the supported grammar.
 */
NANO_RetCode
NANO_XRCE_ObjectRef_from_str(
    NANO_XRCE_ObjectRef *const self,
    const char *const val);

#define NANO_XRCE_ObjectRef_from_str(s_,v_) \
    (((v_) != NULL && \
        NANO_OSAPI_String_length((v_)) <= NANO_LIMIT_OBJECTREF_MAX_LENGTH)?\
        ((s_)->value = (v_), NANO_RETCODE_OK) : NANO_RETCODE_INVALID_ARGS)

/*i
 * @brief Access the string value contained in a NANO_XRCE_ObjectRef instance.
 * 
 * @param self The reference instance.
 * @return The string value contained in the reference object.
 */
const char*
NANO_XRCE_ObjectRef_as_str(
    const NANO_XRCE_ObjectRef *const self);

#define NANO_XRCE_ObjectRef_as_str(s_) \
    ((const char*)(s_)->value)

/*i
 * @brief Determine the length of a string reference.
 * 
 * @param self The reference to inspect.
 * @return The current length of the reference (in octets), excluding
 * the "nul" terminator.
 */
NANO_usize
NANO_XRCE_ObjectRef_length(const NANO_XRCE_ObjectRef *const self);

#define NANO_XRCE_ObjectRef_length(s_) \
    NANO_OSAPI_String_length((s_)->value)

/*i
 * @brief TODO
 * 
 * @param self 
 * @return NANO_bool 
 */
NANO_bool
NANO_XRCE_ObjectRef_is_valid(const NANO_XRCE_ObjectRef *const self);

#define NANO_XRCE_ObjectRef_is_valid(s_) \
    (NANO_XRCE_ObjectRef_length(s_) > 0 && \
        NANO_XRCE_ObjectRef_length(s_) <= NANO_LIMIT_OBJECTREF_MAX_LENGTH)

/*i @} *//* nano_api_xrce_payload_objrepr */
#endif /* NANO_FEAT_OBJECT */


#if NANO_FEAT_OBJECT_KIND_TOPIC
/******************************************************************************
 *                              TopicName
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_support_topicname
 * @{
 */
typedef struct NANODllExport NANO_XRCE_TopicNameI
{
    char *value;
} NANO_XRCE_TopicName;

#define NANO_XRCE_TOPICNAME_INITIALIZER \
{\
    NULL /* value */\
}

/*i @} *//* nano_api_xrce_payload_support_topicname */

/******************************************************************************
 *                              TypeRef
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_support_typeref
 * @{
 */
typedef struct NANODllExport NANO_XRCE_TypeRefI
{
    char *value;
} NANO_XRCE_TypeRef;

#define NANO_XRCE_TYPEREF_INITIALIZER \
{\
    NULL /* value */\
}
/*i @} *//* nano_api_xrce_payload_support_typeref */

/******************************************************************************
 *                              TypeId
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_support_typeid
 * @{
 */
typedef struct NANODllExport NANO_XRCE_TypeIdI
{
    char *value;
} NANO_XRCE_TypeId;

#define NANO_XRCE_TYPEID_INITIALIZER \
{\
    NULL /* value */\
}
/*i @} *//* nano_api_xrce_payload_support_typeid */

#endif /* NANO_FEAT_OBJECT_KIND_TOPIC */

#if NANO_FEAT_OBJECT_KIND_DATAREADER
/******************************************************************************
 *                          ContentFilterExpr
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_support_contentfilter
 * @{
 */

typedef struct NANODllExport NANO_XRCE_ContentFilterExprI
{
    char *value;
#if NANO_CPP

    NANO_XRCE_ContentFilterExprI()
    {
        value = NULL;
    }

#endif /* NANO_CPP */
} NANO_XRCE_ContentFilterExpr;

#define NANO_XRCE_CONTENTFILTEREXPR_INITIALIZER \
{\
    NULL /* value */\
}

#define NANO_XRCE_CONTENTFILTEREXPR_SERIALIZED_SIZE_MAX \
    (NANO_LIMIT_CONTENTFILTEREXPR_MAX_LENGTH + 1)

/*i @} *//* nano_api_xrce_payload_support_contentfilter */

#endif /* NANO_FEAT_OBJECT_KIND_DATAREADER */

#endif /* nano_core_xrce_basic_h */
