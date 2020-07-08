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
 * @file nano_core_xrce_transport.h
 * @brief XRCE Transport.
 * 
 */

#ifndef nano_core_xrce_transport_h
#define nano_core_xrce_transport_h

#if NANO_FEAT_TRANSPORT

/******************************************************************************
 *                          TransportLocatorFormat
 ******************************************************************************/

/**
 * @addtogroup nano_api_client_transport_locator
 * @{
 */

/*e
 * @brief An enumeration of all possible formats of transport address.
 */
typedef NANO_u8 NANO_XRCE_TransportLocatorFormat;

/*e
 * @brief Identifier for locators of type NANO_XRCE_TransportLocatorSmall.
 */
#define NANO_XRCE_ADDRESS_FORMAT_SMALL \
    ((NANO_XRCE_TransportLocatorFormat)0)
/*e
 * @brief Identifier for locators of type NANO_XRCE_TransportLocatorMedium.
 */
#define NANO_XRCE_ADDRESS_FORMAT_MEDIUM \
    ((NANO_XRCE_TransportLocatorFormat)1)
/*e
 * @brief Identifier for locators of type NANO_XRCE_TransportLocatorLarge.
 */
#define NANO_XRCE_ADDRESS_FORMAT_LARGE \
    ((NANO_XRCE_TransportLocatorFormat)2)

/*e
 * @brief Identifier for locators of type NANO_XRCE_TransportLocatorString.
 */
#define NANO_XRCE_ADDRESS_FORMAT_STRING \
    ((NANO_XRCE_TransportLocatorFormat)3)

/*e
 * @brief An initializer for NANO_XRCE_TransportLocatorFormat values, which
 * initializes them to an invalid value.
 */
#define NANO_XRCE_ADDRESS_FORMAT_INVALID \
    ((NANO_XRCE_TransportLocatorFormat)0xFF)

/*e
 * @brief Check if a transport locator format is one of the supported ones.
 * 
 * @param self The transport locator format to check.
 * @return NANO_BOOL_TRUE if the format is supported, NANO_BOOL_FALSE
 * otherwise.
 */
NANO_bool
NANO_XRCE_TransportLocatorFormat_is_valid(
    const NANO_XRCE_TransportLocatorFormat self);

#define NANO_XRCE_TransportLocatorFormat_is_valid(s_) \
   ((s_) == NANO_XRCE_ADDRESS_FORMAT_SMALL || \
    (s_) == NANO_XRCE_ADDRESS_FORMAT_MEDIUM || \
    (s_) == NANO_XRCE_ADDRESS_FORMAT_LARGE || \
    (s_) == NANO_XRCE_ADDRESS_FORMAT_STRING)

/** @} *//* nano_api_client_transport_locator */

/******************************************************************************
 *                              TransportLocator
 ******************************************************************************/

/**
 * @addtogroup nano_api_client_transport_locator
 * @{
 */

/*e
 * @brief Basic representation of a transport address.
 * 
 * This type only provides access to the format of the address, so that a
 * locator can be cast to one of the concrete types.
 * 
 * This strategy is preferred over defining a union type for locators.
 * 
 */
typedef struct NANODllExport NANO_XRCE_TransportLocatorI
    NANO_XRCE_TransportLocator;

/** @} *//* nano_api_client_transport_locator */

/******************************************************************************
 *                          TransportLocatorSmall
 ******************************************************************************/

/**
 * @addtogroup nano_api_client_transport_locator_small
 * @{
 */

/*e
 * @brief Size of the address of a NANO_XRCE_TransportLocatorSmall.
 */
#define NANO_XRCE_TRANSPORTLOCATORSMALL_ADDRESS_LEN         2

/*e
 * @brief The smallest type of transport address.
 */
typedef struct NANODllExport NANO_XRCE_TransportLocatorSmallI
{
    /*e
     * @brief The locator's address.
     */
    NANO_u8 address[NANO_XRCE_TRANSPORTLOCATORSMALL_ADDRESS_LEN];
    /*e
     * @brief The locator's port.
     */
    NANO_u8 port;

#if NANO_CPP
    NANO_XRCE_TransportLocatorSmallI()
    : address(),
      port(0)
    {
        NANO_OSAPI_Memory_zero(address, sizeof(address));
    }
#endif

} NANO_XRCE_TransportLocatorSmall;


#define NANO_XRCE_TRANSPORTLOCATORSMALL_SERIALIZED_SIZE_MAX \
    (/* locator format */sizeof(NANO_bool) + \
        NANO_XRCE_TRANSPORTLOCATORSTRING_ADDRESS_LEN + \
        sizeof(NANO_u8)) /* total = 4 */

/*e
 * @brief A static initializer for NANO_XRCE_TransportLocatorSmall intances.
 */
#define NANO_XRCE_TRANSPORTLOCATORSMALL_INITIALIZER \
{ \
    { 0, 0 }, /* address */ \
    0, /* port */ \
}

/*e
 * @brief Narrow a NANO_XRCE_TransportLocator to a
 * NANO_XRCE_TransportLocatorSmall.
 * 
 * @param self The locator to narrow
 * @return The locator converted to a NANO_XRCE_TransportLocatorSmall, or NULL
 * if the locator has format other than NANO_XRCE_ADDRESS_FORMAT_SMALL.
 */
const NANO_XRCE_TransportLocatorSmall*
NANO_XRCE_TransportLocatorSmall_narrow(
    const NANO_XRCE_TransportLocator *const self);

#define NANO_XRCE_TransportLocatorSmall_narrow(s_) \
    (((s_)->format == NANO_XRCE_ADDRESS_FORMAT_SMALL)?\
        (&(s_)->value.small) : NULL)

/*e
 * @brief Convert a NANO_XRCE_TransportLocatorSmall to a
 * NANO_XRCE_TransportLocator.
 * 
 * @param self The locator to convert
 * @param locator_out The NANO_XRCE_TransportLocator to set.
 */
void
NANO_XRCE_TransportLocatorSmall_to_locator(
    const NANO_XRCE_TransportLocatorSmall *const self,
    NANO_XRCE_TransportLocator *const locator_out);

#define NANO_XRCE_TransportLocatorSmall_to_locator(s_,l_) \
{\
    (l_)->format = NANO_XRCE_ADDRESS_FORMAT_SMALL;\
    (l_)->value.small = *(s_);\
}


/** @} *//* nano_api_client_transport_locator_small */

/******************************************************************************
 *                        TransportLocatorMedium
 ******************************************************************************/

/**
 * @addtogroup nano_api_client_transport_locator_med
 * @{
 */

/*e
 * @brief Size of the address of a NANO_XRCE_TransportLocatorMedium.
 */
#define NANO_XRCE_TRANSPORTLOCATORMEDIUM_ADDRESS_LEN         4

/*e
 * @brief The intermediate type of transport address.
 */
typedef struct NANODllExport NANO_XRCE_TransportLocatorMediumI
{
    /*e
     * @brief The locator's address.
     */
    NANO_u8 address[NANO_XRCE_TRANSPORTLOCATORMEDIUM_ADDRESS_LEN];
    /*e
     * @brief The locator's port.
     */
    NANO_u16 port;

#if NANO_CPP
    NANO_XRCE_TransportLocatorMediumI()
    : address(),
      port(0)
    {
        NANO_OSAPI_Memory_zero(address, sizeof(address));
    }
#endif

} NANO_XRCE_TransportLocatorMedium;


#define NANO_XRCE_TRANSPORTLOCATORMEDIUM_SERIALIZED_SIZE_MAX \
    NANO_OSAPI_Memory_align_size_up(\
        (/* locator format */sizeof(NANO_bool) + \
        NANO_XRCE_TRANSPORTLOCATORMEDIUM_ADDRESS_LEN + \
        sizeof(NANO_u16)),\
        sizeof(NANO_u16)) /* total = 8 (pad = 1) */

/*e
 * @brief A static initializer for NANO_XRCE_TransportLocatorMedium intances.
 */
#define NANO_XRCE_TRANSPORTLOCATORMEDIUM_INITIALIZER \
{ \
    { 0, 0, 0, 0 }, /* address */ \
    0, /* port */ \
}

/*e
 * @brief Narrow a NANO_XRCE_TransportLocator to a
 * NANO_XRCE_TransportLocatorMedium.
 * 
 * @param self The locator to narrow
 * @return The locator converted to a NANO_XRCE_TransportLocatorMedium, or NULL
 * if the locator has format other than NANO_XRCE_ADDRESS_FORMAT_MEDIUM.
 */
const NANO_XRCE_TransportLocatorMedium*
NANO_XRCE_TransportLocatorMedium_narrow(
    const NANO_XRCE_TransportLocator *const self);

#define NANO_XRCE_TransportLocatorMedium_narrow(s_) \
    (((s_)->format == NANO_XRCE_ADDRESS_FORMAT_MEDIUM)?\
        (&(s_)->value.medium) : NULL)

/*e
 * @brief Convert a NANO_XRCE_TransportLocatorMedium to a
 * NANO_XRCE_TransportLocator.
 * 
 * @param self The locator to convert
 * @param locator_out The NANO_XRCE_TransportLocator to set.
 */
void
NANO_XRCE_TransportLocatorMedium_to_locator(
    const NANO_XRCE_TransportLocatorMedium *const self,
    NANO_XRCE_TransportLocator *const locator_out);

#define NANO_XRCE_TransportLocatorMedium_to_locator(s_,l_) \
{\
    (l_)->format = NANO_XRCE_ADDRESS_FORMAT_MEDIUM;\
    (l_)->value.medium = *(s_);\
}


#if NANO_FEAT_TRANSPORT_IPV4
/*e
 * @brief Convert the address of a NANO_XRCE_TransportLocatorMedium to an
 * Ipv4Addr.
 * 
 * @param self  the locator to convert.
 * @return a NANO_Ipv4Addr.
 */
NANO_Ipv4Addr
NANO_XRCE_TransportLocatorMedium_address_to_ipv4addr(
    const NANO_XRCE_TransportLocatorMedium *const self);

#define NANO_XRCE_TransportLocatorMedium_address_to_ipv4addr(s_) \
    NANO_CDR_u32_from_ptr((s_)->address)

/*e
 * @brief Set the address of a NANO_XRCE_TransportLocatorMedium from an
 * Ipv4Addr.
 * 
 * @param self  the locator to convert.
 * @param val the value from which to set the locator.
 */
NANODllExport
void
NANO_XRCE_TransportLocatorMedium_address_from_ipv4addr(
    NANO_XRCE_TransportLocatorMedium *const self,
    const NANO_Ipv4Addr val,
    const NANO_u16 port);

#define NANO_XRCE_TransportLocatorMedium_address_from_ipv4addr(s_,v_,p_) \
{\
    NANO_CDR_u32_set_ptr((s_)->address, (v_));\
    (s_)->port = (p_);\
}

void
NANO_XRCE_TransportLocatorMedium_address_from_ipv4addr_octets(
    NANO_XRCE_TransportLocatorMedium *const self,
    const NANO_u8 b0,
    const NANO_u8 b1,
    const NANO_u8 b2,
    const NANO_u8 b3,
    const NANO_u16 port);

#define NANO_XRCE_TransportLocatorMedium_address_from_ipv4addr_octets(s_,b0_,b1_,b2_,b3_,p_) \
{\
    NANO_Ipv4Addr ip_ = 0;\
    NANO_OSAPI_Ipv4Addr_from_octets(&ip_,(b0_),(b1_),(b2_),(b3_));\
    NANO_XRCE_TransportLocatorMedium_address_from_ipv4addr((s_),(ip_),(p_));\
}


#endif /* NANO_FEAT_TRANSPORT_IPV4 */

/** @} *//* nano_api_client_transport_locator_med */

/******************************************************************************
 *                          TransportLocatorLarge
 ******************************************************************************/

/**
 * @addtogroup nano_api_client_transport_locator_lrg
 * @{
 */

/*e
 * @brief Size of the address of a NANO_XRCE_TransportLocatorLarge.
 */
#define NANO_XRCE_TRANSPORTLOCATORLARGE_ADDRESS_LEN         16

/*e
 * @brief The largest type of transport address.
 */
typedef struct NANODllExport NANO_XRCE_TransportLocatorLargeI
{
    /*e
     * @brief The locator's address.
     */
    NANO_u8 address[NANO_XRCE_TRANSPORTLOCATORLARGE_ADDRESS_LEN];
    /*e
     * @brief The locator's port.
     */
    NANO_u32 port;

#if NANO_CPP
    NANO_XRCE_TransportLocatorLargeI()
    : address(),
      port(0)
    {
        NANO_OSAPI_Memory_zero(address, sizeof(address));
    }
#endif

} NANO_XRCE_TransportLocatorLarge;


#define NANO_XRCE_TRANSPORTLOCATORLARGE_SERIALIZED_SIZE_MAX \
    NANO_OSAPI_Memory_align_size_up(\
        (/* locator format */sizeof(NANO_bool) + \
        NANO_XRCE_TRANSPORTLOCATORLARGE_ADDRESS_LEN + \
        sizeof(NANO_u32)),\
        sizeof(NANO_u32)) /* total = 24 (pad = 3) */

/*e
 * @brief A static initializer for NANO_XRCE_TransportLocatorLarge intances.
 */
#define NANO_XRCE_TRANSPORTLOCATORLARGE_INITIALIZER \
{ \
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, /* address */ \
    0, /* port */ \
}

/*e
 * @brief Narrow a NANO_XRCE_TransportLocator to a
 * NANO_XRCE_TransportLocatorLarge.
 * 
 * @param self The locator to narrow
 * @return The locator converted to a NANO_XRCE_TransportLocatorLarge, or NULL
 * if the locator has format other than NANO_XRCE_ADDRESS_FORMAT_LARGE.
 */
const NANO_XRCE_TransportLocatorLarge*
NANO_XRCE_TransportLocatorLarge_narrow(
    const NANO_XRCE_TransportLocator *const self);

#define NANO_XRCE_TransportLocatorLarge_narrow(s_) \
    (((s_)->format == NANO_XRCE_ADDRESS_FORMAT_LARGE)?\
        (&(s_)->value.large) : NULL)

/*e
 * @brief Convert a NANO_XRCE_TransportLocatorLarge to a
 * NANO_XRCE_TransportLocator.
 * 
 * @param self The locator to convert
 * @param locator_out The NANO_XRCE_TransportLocator to set.
 */
void
NANO_XRCE_TransportLocatorLarge_to_locator(
    const NANO_XRCE_TransportLocatorLarge *const self,
    NANO_XRCE_TransportLocator *const locator_out);

#define NANO_XRCE_TransportLocatorLarge_to_locator(s_,l_) \
{\
    (l_)->format = NANO_XRCE_ADDRESS_FORMAT_LARGE;\
    (l_)->value.large = *(s_);\
}


/** @} *//* nano_api_client_transport_locator_lrg */

/******************************************************************************
 *                        TransportLocatorString
 ******************************************************************************/

/**
 * @addtogroup nano_api_client_transport_locator_str
 * @{
 */

/*e
 * @brief The most generic type of transport address, which represents the
 * address as a string.
 */
typedef struct NANODllExport NANO_XRCE_TransportLocatorStringI
{
    /*e
     * @brief The locator's value.
     */
    char *value;

#if NANO_CPP
    NANO_XRCE_TransportLocatorStringI()
    : value(NULL)
    { }
#endif
} NANO_XRCE_TransportLocatorString;

#define NANO_XRCE_TRANSPORTLOCATORSTRING_SERIALIZED_SIZE_MAX \
    (NANO_OSAPI_Memory_align_size_up(\
        (/* locator format */sizeof(NANO_bool) + \
        sizeof(NANO_u32)),\
        sizeof(NANO_u32)) + \
    NANO_LIMIT_TRANSPORTLOCATORSTRING_ADDRESS_MAX_LENGTH + 1)
    /* total = 9 + STR_MAX (pad = 3) */
    /* if STR_MAX > 15, then LOCSTRING_MAX > LOCLARG_MAX */

/*e
 * @brief A static initializer for NANO_XRCE_TransportLocatorString intances.
 */
#define NANO_XRCE_TRANSPORTLOCATORSTRING_INITIALIZER \
{ \
    NULL, /* value */ \
}

/*e
 * @brief Narrow a NANO_XRCE_TransportLocator to a
 * NANO_XRCE_TransportLocatorString.
 * 
 * @param self The locator to narrow
 * @return The locator converted to a NANO_XRCE_TransportLocatorString, or NULL
 * if the locator has format other than NANO_XRCE_ADDRESS_FORMAT_STRING.
 */
const NANO_XRCE_TransportLocatorString*
NANO_XRCE_TransportLocatorString_narrow(
    const NANO_XRCE_TransportLocator *const self);

#define NANO_XRCE_TransportLocatorString_narrow(s_) \
    (((s_)->format == NANO_XRCE_ADDRESS_FORMAT_STRING)?\
        (&(s_)->value.str) : NULL)

/*e
 * @brief Convert a NANO_XRCE_TransportLocatorString to a
 * NANO_XRCE_TransportLocator.
 * 
 * @param self The locator to convert
 * @param locator_out The NANO_XRCE_TransportLocator to set.
 */
void
NANO_XRCE_TransportLocatorString_to_locator(
    const NANO_XRCE_TransportLocatorString *const self,
    NANO_XRCE_TransportLocator *const locator_out);

#define NANO_XRCE_TransportLocatorString_to_locator(s_,l_) \
{\
    (l_)->format = NANO_XRCE_ADDRESS_FORMAT_STRING;\
    (l_)->value.str = *(s_);\
}


/** @} *//* nano_api_client_transport_locator_str */


/******************************************************************************
 *                              TransportLocator
 ******************************************************************************/

/**
 * @addtogroup nano_api_client_transport_locator
 * @{
 */


/*e
 * @brief A union of all possible types of locators.
 */
typedef union NANODllExport NANO_XRCE_TransportLocatorValueI
{
    /*e
     * @brief The value as a NANO_XRCE_TransportLocatorLarge.
     */
    NANO_XRCE_TransportLocatorLarge large;
    /*e
     * @brief The value as a NANO_XRCE_TransportLocatorMedium.
     */
    NANO_XRCE_TransportLocatorMedium medium;
    /*e
     * @brief The value as a NANO_XRCE_TransportLocatorSmall.
     */
    NANO_XRCE_TransportLocatorSmall small;
    /*e
     * @brief The value as a NANO_XRCE_TransportLocatorString.
     */
    NANO_XRCE_TransportLocatorString str;

#if NANO_CPP
    NANO_XRCE_TransportLocatorValueI()
    {
    }
#endif
} NANO_XRCE_TransportLocatorValue;

/*e
 * @brief Static initializer for instances of NANO_XRCE_TransportLocatorValue.
 */
#define NANO_XRCE_TRANSPORTLOCATORVALUE_INITIALIZER \
{\
    NANO_XRCE_TRANSPORTLOCATORLARGE_INITIALIZER /* large */ \
}

/*e
 * 
 * 
 */
struct NANO_XRCE_TransportLocatorI
{
    /*e
     * @brief The format identifier for this type of transport locator.
     */
    NANO_XRCE_TransportLocatorFormat format;
    /*e
     * @brief The value of the locator as a union of all possible types.
     * 
     */
    NANO_XRCE_TransportLocatorValue value;

#if NANO_CPP
    NANO_XRCE_TransportLocatorI()
    {
        this->format = NANO_XRCE_ADDRESS_FORMAT_INVALID;
    }
#endif /* NANO_CPP */
};

#if NANO_LIMIT_TRANSPORTLOCATORSTRING_ADDRESS_MAX_LENGTH > 15
#define NANO_XRCE_TRANSPORTLOCATOR_SERIALIZED_SIZE_MAX \
    NANO_LIMIT_TRANSPORTLOCATORSTRING_ADDRESS_MAX_LENGTH
#else
#define NANO_XRCE_TRANSPORTLOCATOR_SERIALIZED_SIZE_MAX \
    NANO_LIMIT_TRANSPORTLOCATORLARGE_ADDRESS_MAX_LENGTH
#endif

/*e
 * @brief A static initializer for NANO_XRCE_TransportLocator intances.
 * 
 */
#define NANO_XRCE_TRANSPORTLOCATOR_INITIALIZER \
{ \
    NANO_XRCE_ADDRESS_FORMAT_INVALID, /* format */ \
    NANO_XRCE_TRANSPORTLOCATORVALUE_INITIALIZER /* value */ \
}



void
NANO_XRCE_TransportLocator_address_from_ipv4addr(
    NANO_XRCE_TransportLocator *const self,
    const NANO_Ipv4Addr val,
    const NANO_u16 port);

#define NANO_XRCE_TransportLocator_address_from_ipv4addr(s_,v_,p_) \
{\
    NANO_XRCE_TransportLocatorMedium_address_from_ipv4addr(\
        &(s_)->value.medium, (v_), (p_));\
    (s_)->format = NANO_XRCE_ADDRESS_FORMAT_MEDIUM;\
}

void
NANO_XRCE_TransportLocator_address_from_ipv4addr_octets(
    NANO_XRCE_TransportLocator *const self,
    const NANO_u8 b0,
    const NANO_u8 b1,
    const NANO_u8 b2,
    const NANO_u8 b3,
    const NANO_u16 port);

#define NANO_XRCE_TransportLocator_address_from_ipv4addr_octets(s_,b0_,b1_,b2_,b3_,p_) \
{\
    NANO_XRCE_TransportLocatorMedium_address_from_ipv4addr_octets(\
        &(s_)->value.medium,(b0_),(b1_),(b2_),(b3_),(p_));\
    (s_)->format = NANO_XRCE_ADDRESS_FORMAT_MEDIUM;\
}


#if NANO_FEAT_AGENT

NANO_i8
NANO_XRCE_TransportLocator_compare(
    const NANO_XRCE_TransportLocator *const left,
    const NANO_XRCE_TransportLocator *const right);

NANO_RetCode
NANO_XRCE_TransportLocator_copy(
    NANO_XRCE_TransportLocator *const dst,
    const NANO_XRCE_TransportLocator *const src);

#endif /* NANO_FEAT_AGENT */



/*e
 * @brief Check if a transport locator is valid
 * 
 * @param self The transport locator to check.
 * @return NANO_BOOL_TRUE if the locator is valid, NANO_BOOL_FALSE
 * otherwise.
 */
NANO_bool
NANO_XRCE_TransportLocator_is_valid(
    const NANO_XRCE_TransportLocator *const self);

#define NANO_XRCE_TransportLocator_is_valid(s_) \
   NANO_XRCE_TransportLocatorFormat_is_valid((s_)->format)


/** @} *//* nano_api_client_transport_locator */


#if NANO_FEAT_TYPED_SEQUENCE
/******************************************************************************
 *                          TransportLocatorSeq
 ******************************************************************************/


/**
 * @addtogroup nano_api_client_transport_locator_seq
 * @{
 */

NANO_RetCode
NANO_XRCE_TransportLocatorSeq_deserialize_element(
    NANO_SequenceIterator *const self,
    const NANO_Sequence *const seq,
    void *const next_el);

#define T           NANO_XRCE_TransportLocator
#define TSeq        NANO_XRCE_TransportLocatorSeq
#define TSeq_API    NANO_SEQUENCE_API_LEVEL_TYPED

#include "nano/nano_core_osapi_seq_decl.h"

#define NANO_XRCE_TRANSPORTLOCATORSEQ_INITIALIZER \
        NANO_TSEQIMPL_INITIALIZER(NANO_XRCE_TransportLocator)

#define NANO_XRCE_TransportLocatorSeq_initialize(s_) \
    NANO_TSeqImpl_initialize(s_,sizeof(NANO_XRCE_TransportLocator))

#define NANO_XRCE_TransportLocatorSeq_set_contiguous_buffer(s_,b_,m_,l_) \
    NANO_TSeqImpl_set_contiguous_buffer(\
        s_, b_, m_, l_, NANO_XRCE_TRANSPORTLOCATOR_INITIALIZER)

#define NANO_XRCE_TransportLocatorSeq_finalize(s_) \
    NANO_TSeqImpl_finalize(s_)

#define NANO_XRCE_TransportLocatorSeq_set_serialized_buffer(s_,b_,sz_,cnt_,le_) \
    NANO_TSeqImpl_set_serialized_buffer(s_, b_, sz_, cnt_, le_)

#define NANO_XRCE_TransportLocatorSeq_to_iterator(s_,i_) \
    NANO_TSeqImpl_to_iterator(\
        (s_),\
        NANO_XRCE_TransportLocatorSeq_deserialize_element,\
        (i_))


#define NANO_XRCE_TransportLocatorSeq_initialized(s_) \
    NANO_TSeqImpl_initialized(s_)

#define NANO_XRCE_TransportLocatorSeq_as_seq(s_) \
    NANO_TSeqImpl_as_seq(s_)
    
#define NANO_XRCE_TransportLocatorSeq_as_seq_mut(s_) \
    NANO_TSeqImpl_as_seq_mut(s_)
    
#define NANO_XRCE_TransportLocatorSeq_reference(s_,i_) \
    NANO_TSeqImpl_reference(s_,i_, NANO_XRCE_TransportLocator)
    
#define NANO_XRCE_TransportLocatorSeq_reference_mut(s_,i_) \
    NANO_TSeqImpl_reference_mut(s_,i_, NANO_XRCE_TransportLocator)

#define NANO_XRCE_TransportLocatorSeq_contiguous_buffer(s_) \
    NANO_TSeqImpl_contiguous_buffer(s_, NANO_XRCE_TransportLocator)
    
#define NANO_XRCE_TransportLocatorSeq_contiguous_buffer_mut(s_) \
    NANO_TSeqImpl_contiguous_buffer_mut(s_, NANO_XRCE_TransportLocator)
    
#define NANO_XRCE_TransportLocatorSeq_maximum(s_) \
    NANO_TSeqImpl_maximum(s_)

#define NANO_XRCE_TransportLocatorSeq_serialized_size(s_) \
    NANO_TSeqImpl_serialized_size(s_)
    
#define NANO_XRCE_TransportLocatorSeq_length(s_) \
    NANO_TSeqImpl_length(s_)
    
#define NANO_XRCE_TransportLocatorSeq_set_length(s_, l_) \
    NANO_TSeqImpl_set_length(s_,l_)


/** @} *//* nano_api_client_transport_locator_seq */

#endif /* NANO_FEAT_TYPED_SEQUENCE */

/******************************************************************************
 *                          ClientTransportListener
 ******************************************************************************/
/**
 * @addtogroup nano_api_client_transport_listener
 * @{
 */

/*e
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_ClientTransportListenerI
    NANO_XRCE_ClientTransportListener;

/*e
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_ClientTransportI
    NANO_XRCE_ClientTransport;

/*e
 * @brief TODO
 * 
 * @param self 
 * @param transport 
 * @param msg 
 * @param msg_len 
 * @param retained 
 */
typedef void
    (*NANO_XRCE_ClientTransportListener_OnMessageReceivedFn)(
        NANO_XRCE_ClientTransportListener *const self,
        NANO_XRCE_ClientTransport *const transport,
        NANO_MessageBuffer *const msg,
        NANO_usize msg_len,
        NANO_bool *const retained);

/*e
 * @brief TODO
 * 
 * @param self 
 * @param transport 
 * @return NANO_MessageBuffer* 
 */
typedef NANO_MessageBuffer*
    (*NANO_XRCE_ClientTransportListener_ReclaimMessageFn)(
        NANO_XRCE_ClientTransportListener *const self,
        NANO_XRCE_ClientTransport *const transport);

/*e
 * @brief TODO
 * 
 * @param self 
 * @param transport 
 * @param msg 
 * @param rc 
 */
typedef void
    (*NANO_XRCE_ClientTransportListener_OnMessageSentFn)(
        NANO_XRCE_ClientTransportListener *const self,
        NANO_XRCE_ClientTransport *const transport,
        NANO_MessageBuffer *const msg,
        const NANO_RetCode rc);

/*e
 * 
 * 
 */
struct NANO_XRCE_ClientTransportListenerI
{
    /*e
     * @brief 
     * 
     */
    NANO_XRCE_ClientTransportListener_OnMessageReceivedFn on_message_received;

    /*e
     * @brief 
     * 
     */
    NANO_XRCE_ClientTransportListener_ReclaimMessageFn reclaim_message;

    /*e
     * @brief 
     * 
     */
    NANO_XRCE_ClientTransportListener_OnMessageSentFn on_message_sent;

    /*e
     * @brief 
     * 
     */
    void *user_data;

#if NANO_CPP
    NANO_XRCE_ClientTransportListenerI()
    :
        on_message_received(NULL),
        reclaim_message(NULL),
        on_message_sent(NULL),
        user_data(NULL)
    { }
#endif /* NANO_CPP */
};

/*e
 * @brief
 * 
 */
#define NANO_XRCE_CLIENTTRANSPORTLISTENER_INITIALIZER \
{ \
    NULL, /* on_message_received */ \
    NULL, /* reclaim_message*/ \
    NULL, /* on_message_sent */\
    NULL /* user_data */\
}

NANO_bool
NANO_XRCE_ClientTransportListener_is_valid(
    const NANO_XRCE_ClientTransportListener *const self);

#define NANO_XRCE_ClientTransportListener_is_valid(s_) \
   ((s_)->on_message_received != NULL)

void 
NANO_XRCE_ClientTransportListener_on_message_received(
    NANO_XRCE_ClientTransportListener *const self,
    NANO_XRCE_ClientTransport *const transport,
    NANO_MessageBuffer *const msg,
    NANO_usize msg_len,
    NANO_bool *const retained);

#define NANO_XRCE_ClientTransportListener_on_message_received(s_,t_,m_,ml_,r_) \
    ((s_)->on_message_received((s_),(t_),(m_),(ml_),(r_)))

void 
NANO_XRCE_ClientTransportListener_on_message_sent(
    NANO_XRCE_ClientTransportListener *const self,
    NANO_XRCE_ClientTransport *const transport,
    NANO_MessageBuffer *const msg,
    const NANO_RetCode rc);

#define NANO_XRCE_ClientTransportListener_on_message_sent(s_,t_,m_,rc_) \
    ((s_)->on_message_sent((s_),(t_),(m_),(rc_)))

NANO_MessageBuffer *
NANO_XRCE_ClientTransportListener_reclaim_message(
    NANO_XRCE_ClientTransportListener *const self,
    NANO_XRCE_ClientTransport *const transport);

#define NANO_XRCE_ClientTransportListener_reclaim_message(s_,t_) \
    ((s_)->reclaim_message((s_),(t_)))

/** @} *//* nano_api_client_transport_listener */

/******************************************************************************
 *                    ClientTransportProperties
 ******************************************************************************/

/**
 * @addtogroup nano_api_client_transport_props
 * @{
 */

/*e
 * @brief 
 * 
 */
typedef struct NANODllExport NANO_XRCE_ClientTransportPropertiesI
{
    /*e
     * @brief Locator of the server to which this client transport should
     * connect.
     * 
     * If unspecified (i.e. the locator is invalid), the transport 
     * implementation must have some other mean to determine the server to
     * which it should connrect (e.g. the address might be hard-coded).
     */
    NANO_XRCE_TransportLocator agent_address;
    /*e
     * @brief Maximum Transport Unit of the transport.
     * 
     * This value determines the maximum size of an XRCE Message that can be
     * sent by a transport.
     * 
     * Bigger messages must be fragmented so that they fit within the MTU.
     */
    NANO_usize mtu;

#if NANO_FEAT_TRANSPORT_METADATA
    
    /*e
     * @brief TODO
     * 
     */
    NANO_XRCE_TransportLocator metadata_address;

#endif /* NANO_FEAT_TRANSPORT_METADATA */

#if NANO_CPP
    NANO_XRCE_ClientTransportPropertiesI():
        agent_address(),
        mtu(NANO_LIMIT_TRANSPORT_DEFAULT_MTU)
#if NANO_FEAT_TRANSPORT_METADATA
        ,
        metadata_address()
#endif
    {
        
    }
#endif /* NANO_CPP */

} NANO_XRCE_ClientTransportProperties;

#if NANO_FEAT_TRANSPORT_METADATA
#define NANO_XRCE_CLIENTTRANSPORTPROPERTIES_METADATA_INITIALIZER \
    ,\
    NANO_XRCE_TRANSPORTLOCATOR_INITIALIZER
#else
#define NANO_XRCE_CLIENTTRANSPORTPROPERTIES_METADATA_INITIALIZER
#endif /* NANO_FEAT_TRANSPORT_METADATA */

/*e
 * @brief Static initializer for instances of
 * NANO_XRCE_ClientTransportProperties.
 * 
 */
#define NANO_XRCE_CLIENTTRANSPORTPROPERTIES_INITIALIZER \
{ \
    NANO_XRCE_TRANSPORTLOCATOR_INITIALIZER, /* server_addr */ \
    NANO_LIMIT_TRANSPORT_DEFAULT_MTU /* mtu */ \
    NANO_XRCE_CLIENTTRANSPORTPROPERTIES_METADATA_INITIALIZER /* metadata */\
}

/** @} *//* nano_api_client_transport_props */


/******************************************************************************
 *                              ClientTransport
 ******************************************************************************/

/**
 * @addtogroup nano_api_client_transport_transport
 * @{
 */

/*e
 * @brief Initialize a client transport.
 * 
 * The transport is expected to establish a connection to the server during
 * this call.
 * 
 * @param self The transport to initialize.
 * @param properties Optional properties.
 * @return NANO_RETCODE_OK if the transport was successfully initialized.
 */
typedef NANO_RetCode
    (*NANO_XRCE_ClientTransport_InitializeFn)(
        NANO_XRCE_ClientTransport *const self,
        const NANO_XRCE_ClientTransportListener *const listener,
        const NANO_XRCE_ClientTransportProperties *const properties);

/*e
 * @brief Finalize a client transport.
 * 
 * The transport is expected to close its connection to the server during
 * this call.
 * 
 * @param self The transport to finalize.
 * @return NANO_RETCODE_OK if the transport was successfully finalized.
 */
typedef void
    (*NANO_XRCE_ClientTransport_FinalizeFn)(
        NANO_XRCE_ClientTransport *const self);

/*e
 * @brief 
 * 
 * @param self 
 * @param max_messages 
 * @param timeout_ms 
 * @return NANO_RetCode 
 */
typedef NANO_RetCode
    (*NANO_XRCE_ClientTransport_ProcessInputFn)(
        NANO_XRCE_ClientTransport *const self,
        NANO_u32 max_messages,
        NANO_Timeout timeout_ms);

/*e
 * @brief 
 * 
 * @param self 
 */
typedef void
    (*NANO_XRCE_ClientTransport_FlushOutputFn)(
        NANO_XRCE_ClientTransport *const self);

#if NANO_FEAT_RELIABILITY && \
    NANO_FEAT_RELIABLESTREAM_RECVQUEUE
typedef void
    (*NANO_XRCE_ClientTransport_ReturnMessageFn)(
        NANO_XRCE_ClientTransport *const self,
        NANO_MessageBuffer *const msg);
#endif /* NANO_FEAT_RELIABILITY && \
            NANO_FEAT_RELIABLESTREAM_RECVQUEUE */

#if NANO_FEAT_TRANSPORT_UPDATE

/*e
 * @brief 
 * 
 */
typedef enum NANODllExport NANO_XRCE_ClientTransportLocatorTypeI
{
    /*e
     * @brief 
     * 
     */
    NANO_XRCE_CLIENTTRANSPORTLOCATOR_AGENT,
    /*e
     * @brief 
     * 
     */
    NANO_XRCE_CLIENTTRANSPORTLOCATOR_METADATA
} NANO_XRCE_ClientTransportLocatorType;

NANO_bool
NANO_XRCE_ClientTransportLocatorType_is_valid(
    const NANO_XRCE_ClientTransportLocatorType self);

#define NANO_XRCE_ClientTransportLocatorType_is_valid(s_) \
    ((s_) == NANO_XRCE_CLIENTTRANSPORTLOCATOR_AGENT || \
        (s_) == NANO_XRCE_CLIENTTRANSPORTLOCATOR_METADATA)

/*e
 * @brief 
 * 
 */
typedef NANO_RetCode
    (*NANO_XRCE_ClientTransport_UpdateLocatorFn)(
        NANO_XRCE_ClientTransport *const self,
        const NANO_XRCE_ClientTransportLocatorType locator_type,
        const NANO_XRCE_TransportLocator *const locator);
#endif /* NANO_FEAT_TRANSPORT_UPDATE */

/*e
 * 
 * 
 */
struct NANO_XRCE_ClientTransportI
{
    /*e
     * @brief Initialize a new transport instance.
     */
    NANO_XRCE_ClientTransport_InitializeFn initialize;
    /*e
     * @brief Finalize an existing instance.
     */
    NANO_XRCE_ClientTransport_FinalizeFn finalize;
    /*e
     * @brief Ask transport to process its inputs.
     */
    NANO_XRCE_ClientTransport_ProcessInputFn process_input;
    /*e
     * @brief Ask transport to lush its outputs.
     */
    NANO_XRCE_ClientTransport_FlushOutputFn flush_output;

#if NANO_FEAT_RELIABILITY && \
        NANO_FEAT_RELIABLESTREAM_RECVQUEUE
    /*i
     * @brief Return a message to the transport.
     */
    NANO_XRCE_ClientTransport_ReturnMessageFn return_msg;
#endif

#if NANO_FEAT_TRANSPORT_UPDATE
    /*e
     * @brief 
     * 
     */
    NANO_XRCE_ClientTransport_UpdateLocatorFn update_locator;
#endif /* NANO_FEAT_TRANSPORT_UPDATE */

    /*e
     * @brief 
     * 
     */
    NANO_XRCE_ClientTransportListener listener;
    /*e
     * @brief 
     * 
     */
    NANO_usize mtu;
    /*e
     * @brief 
     * 
     */
    NANO_usize send_queue_len;
    /*e
     * @brief 
     * 
     */
    NANO_MessageBuffer *send_queue;

#if NANO_CPP
    NANO_XRCE_ClientTransportI()
    :
        initialize(NULL),
        finalize(NULL),
        flush_output(NULL),
#if NANO_FEAT_RELIABILITY && \
        NANO_FEAT_RELIABLESTREAM_RECVQUEUE
        return_msg(NULL),
#endif
#if NANO_FEAT_TRANSPORT_UPDATE
        update_locator(NULL),
#endif /* NANO_FEAT_TRANSPORT_UPDATE */
        listener(),
        mtu(NANO_LIMIT_TRANSPORT_DEFAULT_MTU),
        send_queue_len(0),
        send_queue(NULL)
    { }
#endif /* NANO_CPP */

};


NANO_bool
NANO_XRCE_ClientTransport_is_valid_update(
    const NANO_XRCE_ClientTransport *const self);

#if NANO_FEAT_TRANSPORT_UPDATE

#define NANO_XRCE_CLIENTTRANSPORT_UPDATE_INITIALIZER \
    NULL, /* update_locator */
#define NANO_XRCE_ClientTransport_is_valid_update(s_) \
    ((s_)->update_locator != NULL)
#else

#define NANO_XRCE_CLIENTTRANSPORT_UPDATE_INITIALIZER
#define NANO_XRCE_ClientTransport_is_valid_update(s_) \
    NANO_BOOL_TRUE
#endif /* NANO_FEAT_TRANSPORT_UPDATE */


#define NANO_XRCE_CLIENTTRANSPORT_COMMON_INITIALIZER \
    NANO_XRCE_CLIENTTRANSPORTLISTENER_INITIALIZER, /* listener */\
    NANO_LIMIT_TRANSPORT_DEFAULT_MTU, /* mtu */\
    0, /* send_queue_len */\
    NULL /* send_queue */


NANO_bool
NANO_XRCE_ClientTransport_is_valid_reliability(
    const NANO_XRCE_ClientTransport *const self);

#if NANO_FEAT_RELIABILITY && \
        NANO_FEAT_RELIABLESTREAM_RECVQUEUE
#define NANO_XRCE_CLIENTTRANSPORT_RELIABILITY_INITIALIZER \
    NULL, /* return_msg */
#define NANO_XRCE_ClientTransport_is_valid_reliability(s_) \
    ((s_)->return_msg != NULL)
#else
#define NANO_XRCE_CLIENTTRANSPORT_RELIABILITY_INITIALIZER
#define NANO_XRCE_ClientTransport_is_valid_reliability(s_) \
    NANO_BOOL_TRUE
#endif /* NANO_FEAT_RELIABILITY */


/*e
 * @brief Static initializer for instances of NANO_XRCE_ClientTransport.
 */
#define NANO_XRCE_CLIENTTRANSPORT_INITIALIZER \
{\
    NULL, /* initialize */ \
    NULL, /* finalize */ \
    NULL, /* process_input */ \
    NULL, /* flush_output */ \
    NANO_XRCE_CLIENTTRANSPORT_RELIABILITY_INITIALIZER /* reliability methods */ \
    NANO_XRCE_CLIENTTRANSPORT_UPDATE_INITIALIZER /* update methods */\
    NANO_XRCE_CLIENTTRANSPORT_COMMON_INITIALIZER /* common fields */\
}

/*e
 * @brief 
 * 
 * @param self 
 * @return NANO_bool 
 */
NANO_bool
NANO_XRCE_ClientTransport_is_valid(
    const NANO_XRCE_ClientTransport *const self);

#define NANO_XRCE_ClientTransport_is_valid(s_) \
   ((s_)->initialize != NULL && \
    (s_)->finalize != NULL && \
    (s_)->process_input != NULL && \
    (s_)->flush_output != NULL && \
    NANO_XRCE_ClientTransport_is_valid_reliability(s_) && \
    NANO_XRCE_ClientTransport_is_valid_update(s_))

NANO_RetCode
NANO_XRCE_ClientTransport_initialize(
    NANO_XRCE_ClientTransport *const self,
    const NANO_XRCE_ClientTransportListener *const listener,
    const NANO_XRCE_ClientTransportProperties *const properties);

#define NANO_XRCE_ClientTransport_initialize(s_,l_,p_) \
    ((s_)->initialize((s_),(l_),(p_)))

void
NANO_XRCE_ClientTransport_finalize(
    NANO_XRCE_ClientTransport *const self);

#define NANO_XRCE_ClientTransport_finalize(s_) \
    ((s_)->finalize((s_)))

NANO_usize
NANO_XRCE_ClientTransport_mtu(NANO_XRCE_ClientTransport *const self);

#define NANO_XRCE_ClientTransport_mtu(s_) \
    ((s_)->mtu)

NANO_RetCode
NANO_XRCE_ClientTransport_process_input(
    NANO_XRCE_ClientTransport *const self,
    NANO_u32 max_messages,
    NANO_Timeout timeout_ms);

#define NANO_XRCE_ClientTransport_process_input(s_,m_,t_) \
    ((s_)->process_input((s_),(m_),(t_)))

void
NANO_XRCE_ClientTransport_flush_output(
    NANO_XRCE_ClientTransport *const self);

#define NANO_XRCE_ClientTransport_flush_output(s_) \
    ((s_)->flush_output((s_)))

#if NANO_FEAT_RELIABILITY && \
        NANO_FEAT_RELIABLESTREAM_RECVQUEUE
NANO_RetCode
NANO_XRCE_ClientTransport_return(
    NANO_XRCE_ClientTransport *const self,
    NANO_MessageBuffer *const msg);

#define NANO_XRCE_ClientTransport_return(s_,m_) \
    ((s_)->return_msg((s_),(m_)))
#endif /* NANO_FEAT_RELIABILITY && \
            NANO_FEAT_RELIABLESTREAM_RECVQUEUE */

#if NANO_FEAT_TRANSPORT_UPDATE
NANO_RetCode
NANO_XRCE_ClientTransport_update_locator(
    NANO_XRCE_ClientTransport *const self,
    const NANO_XRCE_ClientTransportLocatorType locator_type,
    const NANO_XRCE_TransportLocator *const locator);

#define NANO_XRCE_ClientTransport_update_locator(s_,t_,l_) \
    ((s_)->update_locator((s_),(t_),(l_)))

#endif /* NANO_FEAT_TRANSPORT_UPDATE */

NANODllExport
NANO_RetCode
NANO_XRCE_ClientTransport_queue_send(
    NANO_XRCE_ClientTransport *const self,
    NANO_MessageBuffer *const msg);

/*e
 * @brief 
 * 
 * @param self 
 * @param send_i 
 * @param msg 
 * @param rc 
 */
NANODllExport
void
NANO_XRCE_ClientTransport_send_complete(
    NANO_XRCE_ClientTransport *const self,
    NANO_usize send_i,
    NANO_MessageBuffer *msg,
    NANO_RetCode rc);

/*e
 * @brief 
 * 
 * @param self 
 * @return NANO_MessageBuffer* 
 */
NANO_MessageBuffer*
NANO_XRCE_ClientTransport_next_send(
    NANO_XRCE_ClientTransport *const self);

/*e
 * @brief 
 * 
 * @param self 
 * @return NANO_bool 
 */
NANO_bool
NANO_XRCE_ClientTransport_pending_send(
    NANO_XRCE_ClientTransport *const self);

#define NANO_XRCE_ClientTransport_next_send(s_) \
    ((s_)->send_queue)

#define NANO_XRCE_ClientTransport_pending_send(s_) \
    ((s_)->send_queue != NULL)

/*e
 * @brief 
 * 
 * @param msg 
 * @param buffer 
 * @param buffer_size 
 * @param mtu 
 * @param abuffer_size_out 
 * @return NANO_RetCode
 */
NANODllExport
NANO_RetCode
NANO_XRCE_ClientTransport_initialize_recv_buffer(
    NANO_MessageBuffer *const msg,
    NANO_u8 *const buffer,
    const NANO_usize buffer_size,
    const NANO_usize mtu,
    NANO_u16 *abuffer_size_out);

/** @} *//* nano_api_client_transport_transport */

#endif /* NANO_FEAT_TRANSPORT */

#endif /* nano_core_xrce_transport_h */
