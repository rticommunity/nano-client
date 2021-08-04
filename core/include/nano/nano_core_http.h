/******************************************************************************
 *
 * (c) 2020 Copyright, Real-Time Innovations, Inc. (RTI) All rights reserved.
 *
 * RTI grants Licensee a license to use, modify, compile, and create derivative
 * works of the Software solely in combination with RTI Connext DDS. Licensee
 * may redistribute copies of the Software provided that all such copies are
 * subject to this License. The Software is provided "as is", with no warranty
 * of any type, including any warranty for fitness for any purpose. RTI is
 * under no obligation to maintain or support the Software. RTI shall not be
 * liable for any incidental or consequential damages arising out of the use or
 * inability to use the Software. For purposes of clarity, nothing in this
 * License prevents Licensee from using alternate versions of DDS, provided
 * that Licensee may not combine or link such alternate versions of DDS with
 * the Software.
 *
 ******************************************************************************/

#ifndef nano_core_http_h
#define nano_core_http_h

#include "nano/nano_core.h"

typedef NANO_u8 NANO_HttpMethod;
#define NANO_HTTPMETHOD_UNKNOWN     ((NANO_HttpMethod)0x00)
#define NANO_HTTPMETHOD_GET         ((NANO_HttpMethod)0x01)
#define NANO_HTTPMETHOD_HEAD        ((NANO_HttpMethod)0x02)
#define NANO_HTTPMETHOD_POST        ((NANO_HttpMethod)0x03)
#define NANO_HTTPMETHOD_PUT         ((NANO_HttpMethod)0x04)
#define NANO_HTTPMETHOD_DELETE      ((NANO_HttpMethod)0x05)
#define NANO_HTTPMETHOD_CONNECT     ((NANO_HttpMethod)0x06)
#define NANO_HTTPMETHOD_OPTIONS     ((NANO_HttpMethod)0x07)
#define NANO_HTTPMETHOD_TRACE       ((NANO_HttpMethod)0x08)
#define NANO_HTTPMETHOD_PATCH       ((NANO_HttpMethod)0x09)

/* Encode method in the 4 MSB bits. */
NANO_HttpMethod
NANO_HttpMethod_from_flags(const NANO_XRCE_ServiceRequestFlags flags);
#define NANO_HttpMethod_from_flags(f_)\
    ((NANO_HttpMethod)(((NANO_XRCE_ServiceRequestFlags)(f_)) >> 12))

NANO_XRCE_ServiceRequestFlags
NANO_HttpMethod_to_flags(const NANO_HttpMethod method);
#define NANO_HttpMethod_to_flags(m_)\
    ((((NANO_XRCE_ServiceRequestFlags)(m_)) & 0x000F) << 12)

const char *
NANO_HttpMethod_to_str(const NANO_HttpMethod method);
#define NANO_HttpMethod_to_str(m_) \
(\
    ((m_) == NANO_HTTPMETHOD_GET)?\
        "GET":\
    ((m_) == NANO_HTTPMETHOD_HEAD)?\
        "HEAD":\
    ((m_) == NANO_HTTPMETHOD_POST)?\
        "POST":\
    ((m_) == NANO_HTTPMETHOD_PUT)?\
        "PUT":\
    ((m_) == NANO_HTTPMETHOD_DELETE)?\
        "DELETE":\
    ((m_) == NANO_HTTPMETHOD_CONNECT)?\
        "CONNECT":\
    ((m_) == NANO_HTTPMETHOD_OPTIONS)?\
        "OPTIONS":\
    ((m_) == NANO_HTTPMETHOD_TRACE)?\
        "TRACE":\
    ((m_) == NANO_HTTPMETHOD_PATCH)?\
        "PATCH":\
        "UNKNOWN" \
)

/* HTTP status codes go from 100 to 599, so only 10 bytes are needed to
   represent them. We encode them in the 10 LSB bits */
typedef NANO_i32 NANO_HttpStatus;

NANO_XRCE_ServiceReplyStatus
NANO_HttpStatus_encode(const NANO_HttpStatus status);
#define NANO_HttpStatus_encode(s_) \
  ((NANO_XRCE_ServiceReplyStatus)(((NANO_HttpStatus)(s_)) & 0x000003FF))

NANO_HttpStatus
NANO_HttpStatus_decode(const NANO_XRCE_ServiceReplyStatus status);
#define NANO_HttpStatus_decode(s_) \
  ((NANO_HttpStatus)(((NANO_XRCE_ServiceReplyStatus)(s_)) & 0x000003FF))

#endif /* nano_core_http_h */