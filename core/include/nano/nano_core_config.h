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
 * @file nano_core_config.h
 * @brief Library configuration
 * 
 * This header file specifies global configuration flags which can be used
 * to control the features and behavior of the Nano Core library.
 *
 */

#ifndef nano_core_config_h
#define nano_core_config_h

#include "nano_core_user_config.h"

#ifdef DOCUMENTATION_ONLY
/*e
 * @ingroup nano_api_libcfg_headers
 * @brief Header file containing custom configuration flags to override
 * default values.
 */
#define NANO_CORE_DEF_H
/*e
 * @ingroup nano_api_libcfg_core
 * @brief Enable precondition assertion in source code.
 */
#define NANO_ENABLE_PRECONDITION
#else /* !DOCUMENTATION_ONLY */
#ifdef NANO_CORE_DEF_H
#include NANO_CORE_DEF_H
#endif /* NANO_CORE_DEF_H */
#endif /* DOCUMENTATION_ONLY */

/**
 * @addtogroup nano_api_libcfg_core
 * @{
 */

#ifndef NANO_FEAT_AGENT
/*i
 * @brief Build nanocore for use by nanoagent
 * 
 */
#define NANO_FEAT_AGENT                     0
#endif /* NANO_FEAT_AGENT */

#ifndef NANO_FEAT_TIME
/*i
 * @brief Enable support for time representation
 * 
 */
#define NANO_FEAT_TIME                      1
#endif /* NANO_FEAT_TIME */

#ifndef NANO_FEAT_SYSTIME
/*i
 * @brief Enable support for system time and clock.
 * 
 */
#define NANO_FEAT_SYSTIME                   1
#endif /* NANO_FEAT_SYSTIME */

#ifndef NANO_FEAT_TYPED_SEQUENCE
/*i
 * @brief Enable support for defining typed Sequence types
 * 
 */
#define NANO_FEAT_TYPED_SEQUENCE            1
#endif /* NANO_FEAT_TYPED_SEQUENCE */

#ifndef NANO_FEAT_SEQUENCE
/*i
 * @brief Enable support for the generic Sequence type
 * 
 */
#define NANO_FEAT_SEQUENCE                  1
#endif /* NANO_FEAT_SEQUENCE */

#ifndef NANO_FEAT_OPTIONAL
/*i
 * @brief Enable support for defining optional members in a struct.
 * 
 */
#define NANO_FEAT_OPTIONAL                  1
#endif /* NANO_FEAT_OPTIONAL */

#ifndef NANO_FEAT_BUFFERPOOL
/*i
 * @brief Enable implementation of a simple memory buffer pool.
 * 
 */
#define NANO_FEAT_BUFFERPOOL                1
#endif /* NANO_FEAT_BUFFERPOOL */

#ifndef NANO_FEAT_PROPERTY
/*i
 * @brief Enable Property type.
 * 
 */
#define NANO_FEAT_PROPERTY                  NANO_FEAT_AGENT
#endif /* NANO_FEAT_PROPERTY */

#ifndef NANO_FEAT_RESULTSTATUS
/*i
 * @brief Enable ResultStatus type
 * 
 */
#define NANO_FEAT_RESULTSTATUS              1
#endif /* NANO_FEAT_RESULTSTATUS */

#ifndef NANO_FEAT_OBJECT
/*i
 * @brief Enable support for XRCE objects
 * 
 */
#define NANO_FEAT_OBJECT                    1
#endif /* NANO_FEAT_OBJECT */

#ifndef NANO_FEAT_OBJECT_KIND_QOSPROFILE
/*i
 * @brief Enable support for XRCE QosProfile objects.
 * 
 */
#define NANO_FEAT_OBJECT_KIND_QOSPROFILE    1
#endif /* NANO_FEAT_OBJECT_KIND_QOSPROFILE */

#ifndef NANO_FEAT_OBJECT_KIND_DOMAIN
/*i
 * @brief Enable support for XRCE Domain objects.
 * 
 */
#define NANO_FEAT_OBJECT_KIND_DOMAIN        1
#endif /* NANO_FEAT_OBJECT_KIND_DOMAIN */

#ifndef NANO_FEAT_OBJECT_KIND_TYPE
/*i
 * @brief Enable support for XRCE Type objects.
 * 
 */
#define NANO_FEAT_OBJECT_KIND_TYPE          1
#endif /* NANO_FEAT_OBJECT_KIND_TYPE */

#ifndef NANO_FEAT_OBJECT_KIND_APPLICATION
/*i
 * @brief Enable support for XRCE Application objects.
 * 
 */
#define NANO_FEAT_OBJECT_KIND_APPLICATION   1
#endif /* NANO_FEAT_OBJECT_KIND_APPLICATION */

#ifndef NANO_FEAT_OBJECT_KIND_PARTICIPANT
/*i
 * @brief Enable support for XRCE DomainParticipant objects.
 * 
 */
#define NANO_FEAT_OBJECT_KIND_PARTICIPANT   1
#endif /* NANO_FEAT_OBJECT_KIND_PARTICIPANT */

#ifndef NANO_FEAT_OBJECT_KIND_TOPIC
/*i
 * @brief Enable support for XRCE Topic objects.
 * 
 */
#define NANO_FEAT_OBJECT_KIND_TOPIC         1
#endif /* NANO_FEAT_OBJECT_KIND_TOPIC */

#ifndef NANO_FEAT_OBJECT_KIND_SUBSCRIBER
/*i
 * @brief Enable support for XRCE Subscriber objects.
 * 
 */
#define NANO_FEAT_OBJECT_KIND_SUBSCRIBER    1
#endif /* NANO_FEAT_OBJECT_KIND_SUBSCRIBER */

#ifndef NANO_FEAT_OBJECT_KIND_PUBLISHER
/*i
 * @brief Enable support for XRCE Publisher objects.
 * 
 */
#define NANO_FEAT_OBJECT_KIND_PUBLISHER     1
#endif /* NANO_FEAT_OBJECT_KIND_PUBLISHER */

#ifndef NANO_FEAT_OBJECT_KIND_DATAREADER
/*i
 * @brief Enable support for XRCE DataReader objects.
 * 
 */
#define NANO_FEAT_OBJECT_KIND_DATAREADER    1
#endif /* NANO_FEAT_OBJECT_KIND_DATAREADER */

#ifndef NANO_FEAT_OBJECT_KIND_DATAWRITER
/*i
 * @brief Enable support for XRCE DataWriter objects.
 * 
 */
#define NANO_FEAT_OBJECT_KIND_DATAWRITER    1
#endif /* NANO_FEAT_OBJECT_KIND_DATAWRITER */

#ifndef NANO_FEAT_OBJECT_INFO
/*i
 * @brief Enable support for querying information about XRCE objects.
 * 
 */
#define NANO_FEAT_OBJECT_INFO          1
#endif /* NANO_FEAT_OBJECT_INFO */

#ifndef NANO_FEAT_OBJECT_INFO_ACTIVITY
/*i
 * @brief Enable support for querying state information about XRCE objects.
 * 
 */
#define NANO_FEAT_OBJECT_INFO_ACTIVITY 1
#endif /* NANO_FEAT_OBJECT_INFO_ACTIVITY */

#ifndef NANO_FEAT_OBJECT_INFO_CONFIG
/*i
 * @brief Enable support for querying configuration information about XRCE 
 * objects.
 * 
 */
#define NANO_FEAT_OBJECT_INFO_CONFIG   NANO_FEAT_AGENT
#endif /* NANO_FEAT_OBJECT_INFO_CONFIG */

#ifndef NANO_FEAT_OBJECT_CREATE
/*i
 * @brief Enable support for creating XRCE objects.
 * 
 */
#define NANO_FEAT_OBJECT_CREATE             1
#endif /* NANO_FEAT_OBJECT_CREATE */

#ifndef NANO_FEAT_OBJECT_DELETE
/*i
 * @brief Enable support for deleting XRCE objects.
 * 
 */
#define NANO_FEAT_OBJECT_DELETE             1
#endif /* NANO_FEAT_OBJECT_DELETE */

#ifndef NANO_FEAT_REQUEST
/*i
 * @brief  TODO
 * 
 */
#define NANO_FEAT_REQUEST                   1
#endif /* NANO_FEAT_REQUEST */

#ifndef NANO_FEAT_REPLY
/*i
 * @brief Enable support for replying to XRCE requests.
 * 
 */
#define NANO_FEAT_REPLY                     1
#endif /* NANO_FEAT_REPLY */

#ifndef NANO_FEAT_DATA
/*i
 * @brief Enable support for manipulating XRCE data.
 * 
 */
#define NANO_FEAT_DATA                      NANO_FEAT_AGENT
#endif /* NANO_FEAT_DATA */

#ifndef NANO_FEAT_DELIVERY_CTRL
/*i
 * @brief Enable support for specifying delivery control options when 
 * subscribing to data.
 * 
 */
#define NANO_FEAT_DELIVERY_CTRL             1
#endif /* NANO_FEAT_DELIVERY_CTRL */

#ifndef NANO_FEAT_SUBSCRIBE
/*e
 * @brief Enable support for subscribing to XRCE data.
 * 
 */
#define NANO_FEAT_SUBSCRIBE                 1
#endif /* NANO_FEAT_SUBSCRIBE */

#ifndef NANO_FEAT_PUBLISH
/*e
 * @brief Enable support for publishing XRCE data.
 * 
 */
#define NANO_FEAT_PUBLISH                   1
#endif /* NANO_FEAT_PUBLISH */

#ifndef NANO_FEAT_CONTENT_FILTER
/*i
 * @brief Enable support for specifying custom content filters when 
 * subscribing to data.
 * 
 */
#define NANO_FEAT_CONTENT_FILTER            1
#endif /* NANO_FEAT_CONTENT_FILTER */

#ifndef NANO_FEAT_SAMPLEINFO
/*i
 * @brief Enable support for representing sample metadata.
 * 
 */
#define NANO_FEAT_SAMPLEINFO                1
#endif /* NANO_FEAT_SAMPLEINFO */

#ifndef NANO_FEAT_DATA_FORMAT_PACKED
/*i
 * @brief Enable support for manipulating XRCE data in “packed” format.
 * 
 */
#define NANO_FEAT_DATA_FORMAT_PACKED        NANO_FEAT_AGENT
#endif /* NANO_FEAT_DATA_FORMAT_PACKED */

#ifndef NANO_FEAT_DATA_FORMAT_DATA
/*i
 * @brief Enable support for manipulating XRCE data in “data” format.
 * 
 */
#define NANO_FEAT_DATA_FORMAT_DATA          1
#endif /* NANO_FEAT_DATA_FORMAT_DATA */

#ifndef NANO_FEAT_DATA_FORMAT_SAMPLE
/*i
 * @brief Enable support for manipulating XRCE data in “sample” format.
 * 
 */
#define NANO_FEAT_DATA_FORMAT_SAMPLE        NANO_FEAT_AGENT
#endif /* NANO_FEAT_DATA_FORMAT_SAMPLE */

#ifndef NANO_FEAT_DATA_FORMAT_DATASEQ
/*i
 * @brief Enable support for manipulating XRCE data in “data sequence” format.
 * 
 */
#define NANO_FEAT_DATA_FORMAT_DATASEQ       NANO_FEAT_AGENT
#endif /* NANO_FEAT_DATA_FORMAT_DATASEQ */

#ifndef NANO_FEAT_DATA_FORMAT_SAMPLESEQ
/*i
 * @brief Enable support for manipulating XRCE data in “sample sequence” format.
 * 
 */
#define NANO_FEAT_DATA_FORMAT_SAMPLESEQ     NANO_FEAT_AGENT
#endif /* NANO_FEAT_DATA_FORMAT_SAMPLESEQ */

#ifndef NANO_FEAT_RELIABILITY
/*e
 * @brief Enable support for reliable exchange of XRCE messages.
 * 
 */
#define NANO_FEAT_RELIABILITY               1
#endif /* NANO_FEAT_RELIABILITY */

#ifndef NANO_FEAT_TIME_SYNC
/*i
 * @brief Enable support for time synchronization using XRCE.
 * 
 */
#define NANO_FEAT_TIME_SYNC                 1
#endif /* NANO_FEAT_TIME_SYNC */

#ifndef NANO_FEAT_FRAGMENT
/*e
 * @brief Enable support for fragmentation of reliable XRCE messages.
 * 
 */
#define NANO_FEAT_FRAGMENT                  NANO_FEAT_RELIABILITY
#endif /* NANO_FEAT_FRAGMENT */

#ifndef NANO_FEAT_STRING
/*i
 * @brief Enable additional support for handling String values.
 * 
 */
#define NANO_FEAT_STRING                    1
#endif /* NANO_FEAT_STRING */

#ifndef NANO_FEAT_CDR
/*e
 * @brief Enable implementation of CDR serialization format.
 * 
 */
#define NANO_FEAT_CDR                       1
#endif /* NANO_FEAT_CDR */

#ifndef NANO_FEAT_OSAPI_STDINT
/*e
 * @brief Use stdint.h to define integer types in OSAPI.
 * 
 */
#define NANO_FEAT_OSAPI_STDINT                       1
#endif /* NANO_FEAT_OSAPI_STDINT */


#ifndef NANO_FEAT_ASSERT_LIVELINESS
/*i
 * @brief Enable assertion of liveliness with XRCE Agent via GET_INFO messages.
 * 
 */
#define NANO_FEAT_ASSERT_LIVELINESS         1
#endif /* NANO_FEAT_ASSERT_LIVELINESS */

#ifndef NANO_FEAT_TRANSPORT_PLUGIN_UDPV4
/*e
 * @brief Enable UDPv4 transport plugin.
 */
#define NANO_FEAT_TRANSPORT_PLUGIN_UDPV4    1
#endif /* NANO_FEAT_TRANSPORT_PLUGIN_UDPV4 */

#ifndef NANO_FEAT_TRANSPORT_PLUGIN_UDPV6
/*i
 * @brief Enable UDPv6 transport plugin.
 */
#define NANO_FEAT_TRANSPORT_PLUGIN_UDPV6    0
#endif /* NANO_FEAT_TRANSPORT_UDPV6 */

#ifndef NANO_FEAT_TRANSPORT_PLUGIN_TCPV4
/*i
 * @brief Enable TCPv4 transport plugin.
 */
#define NANO_FEAT_TRANSPORT_PLUGIN_TCPV4    0
#endif /* NANO_FEAT_TRANSPORT_PLUGIN_TCPV4 */

#ifndef NANO_FEAT_TRANSPORT_PLUGIN_TCPV6
/*i
 * @brief Enable TCPv6 transport plugin.
 */
#define NANO_FEAT_TRANSPORT_PLUGIN_TCPV6    0
#endif /* NANO_FEAT_TRANSPORT_PLUGIN_TCPV6 */

#ifndef NANO_FEAT_TRANSPORT_PLUGIN_SERIAL
/*e
 * @brief Enable Serial transport plugin.
 */
#define NANO_FEAT_TRANSPORT_PLUGIN_SERIAL   1
#endif /* NANO_FEAT_TRANSPORT_PLUGIN_SERIAL */

#ifndef NANO_FEAT_TRANSPORT_RESOLVE
/*i
 * @brief Enable name resolution features in transport plugins.
 */
#define NANO_FEAT_TRANSPORT_RESOLVE         0
#endif /* NANO_FEAT_TRANSPORT_RESOLVE */

#ifndef NANO_FEAT_TRANSPORT_AUTOCFG
/*i
 * @brief Enable support for automatic configuration of transport interfaces.
 */
#define NANO_FEAT_TRANSPORT_AUTOCFG          NANO_FEAT_AGENT
#endif /* NANO_FEAT_TRANSPORT_AUTOCFG */


#ifndef NANO_FEAT_TRANSPORT_STRING
/*i
 * @brief Enable support for configuration of transport locators via string.
 */
#define NANO_FEAT_TRANSPORT_STRING             0
#endif /* NANO_FEAT_TRANSPORT_AUTOCFG */


#ifndef NANO_FEAT_TRANSPORT
/*i
 * @brief Enable transport features.
 * 
 * At least one of the available transport plugins must be enabled along
 * with this flag.
 * 
 * If unspecified, it will be automatically enabled based on transport
 * plugin selection.
 */
#define NANO_FEAT_TRANSPORT                 1
#endif /* NANO_FEAT_TRANSPORT */

#ifndef NANO_FEAT_TRANSPORT_IPV4
/*e
 * @brief Enable support for IPv4 communication.
 * 
 * If unspecified, it will be automatically enabled based on transport
 * plugin selection.
 */
#define NANO_FEAT_TRANSPORT_IPV4 \
           (NANO_FEAT_TRANSPORT_PLUGIN_UDPV4 || \
            NANO_FEAT_TRANSPORT_PLUGIN_TCPV4)
#endif /* NANO_FEAT_TRANSPORT_IPV4 */

#ifndef NANO_FEAT_TRANSPORT_IPV6
/*i
 * @brief Enable support for IPv6 communication.
 * 
 * If unspecified, it will be automatically enabled based on transport
 * plugin selection.
 */
#define NANO_FEAT_TRANSPORT_IPV6 \
           (NANO_FEAT_TRANSPORT_PLUGIN_UDPV6 || \
            NANO_FEAT_TRANSPORT_PLUGIN_TCPV6)
#endif /* NANO_FEAT_TRANSPORT_IPV6 */

#ifndef NANO_FEAT_TRANSPORT_IP
/*i
 * @brief Enable support for IP communication.
 * 
 * If unspecified, it will be automatically enabled based on transport
 * plugin selection.
 */
#define NANO_FEAT_TRANSPORT_IP \
           (NANO_FEAT_TRANSPORT_IPV4 || \
            NANO_FEAT_TRANSPORT_IPV6)
#endif /* NANO_FEAT_TRANSPORT_IP */

#ifndef NANO_FEAT_TRANSPORT_SERIAL
/*e
 * @brief Enable support for Serial communication.
 * 
 */
#define NANO_FEAT_TRANSPORT_SERIAL \
        NANO_FEAT_TRANSPORT_PLUGIN_SERIAL
#endif /* NANO_FEAT_TRANSPORT_SERIAL */

#ifndef NANO_FEAT_TRANSPORT_METADATA
/*i
 * @brief Enable transport support for exchanging metadata information.
 * 
 */
#define NANO_FEAT_TRANSPORT_METADATA                    1
#endif /* NANO_FEAT_TRANSPORT_METADATA */

#ifndef NANO_FEAT_TRANSPORT_UPDATE
/*i
 * @brief Enable support for updating the locator information in a transport.
 * 
 */
#define NANO_FEAT_TRANSPORT_UPDATE                      1
#endif /* NANO_FEAT_TRANSPORT_UPDATE */


#ifndef NANO_FEAT_LOG
/*e
 * @brief Enable the static logging interface.
 * 
 */
#define NANO_FEAT_LOG           (NANO_ENABLE_PRECONDITION || NANO_FEAT_AGENT)
#endif /* NANO_FEAT_LOG */

#ifndef NANO_FEAT_LOG_IMPL_PRINTF
/*i
 * @brief Enable implementation of logging interface on top of standard printf().
 * 
 */
#define NANO_FEAT_LOG_IMPL_PRINTF                           1
#endif /* NANO_FEAT_LOG_IMPL_PRINTF */

#ifndef NANO_FEAT_LOG_COLOR
/*e
 * @brief Enable colored logging output. Requires termcolor.h.
 * 
 */
#define NANO_FEAT_LOG_COLOR                                 NANO_PLATFORM_IS_POSIX
#endif /* NANO_FEAT_LOG */

#ifndef NANO_FEAT_LOG_FILENAME
/*e
 * @brief Include name of source file in logging messages.
 * 
 */
#define NANO_FEAT_LOG_FILENAME                              1
#endif /* NANO_FEAT_FILENAME */

#ifndef NANO_FEAT_LOG_FILENAME_FULL
/*e
 * @brief Use full name when logging the source file of a message.
 * 
 */
#define NANO_FEAT_LOG_FILENAME_FULL                         0
#endif /* NANO_FEAT_FILENAME */

#ifndef NANO_FEAT_LOG_FUNCTION
/*e
 * @brief Include function name in logging messages.
 * 
 */
#define NANO_FEAT_LOG_FUNCTION                              1
#endif /* NANO_FEAT_FUNCTION */

#ifndef NANO_FEAT_LOG_TS
/*i
 * @brief Include system timestamp in logging messages.
 * 
 */
#define NANO_FEAT_LOG_TS                                    0
#endif /* NANO_FEAT_LOG_TS */

#ifndef NANO_FEAT_LOG_DEBUG_ON_ERROR
/*i
 * @brief Increase logging verbosity when an error is logged.
 * 
 */
#define NANO_FEAT_LOG_DEBUG_ON_ERROR                        0
#endif /* NANO_FEAT_LOG_DEBUG_ON_ERROR */

#ifndef NANO_FEAT_MTU_IN_CLIENT_REPR
/*e
 * @brief XRCE protocol extension: include mtu in client representation.
 * 
 */
#define NANO_FEAT_MTU_IN_CLIENT_REPR                 1
#endif /* NANO_FEAT_MTU_IN_CLIENT_REPR */

#ifndef NANO_FEAT_ALL_BY_REF
/*i
 * @brief XRCE protocol extension: support BY_REFERENCE representation for 
 * all kinds of XRCE objects.
 * 
 */
#define NANO_FEAT_ALL_BY_REF                         1
#endif /* NANO_FEAT_ALL_BY_REF */

#ifndef NANO_FEAT_EPROSIMA
/*e
 * @brief Enable compatibility mode with Eprosima's Micro XRCE-DDS.
 * 
 */
#define NANO_FEAT_EPROSIMA                        0
#endif /* NANO_FEAT_EPROSIMA */

#ifndef NANO_FEAT_NATIVE_F128
/*i
 * @brief Enable support for native representations of 128-bit floating 
 * point values.
 * 
 */
#define NANO_FEAT_NATIVE_F128                   0
#endif /* NANO_FEAT_NATIVE_F128 */


#ifndef NANO_FEAT_HTTP_CLIENT
/*i
 * @brief Enable support for performing HTTP requests over XRCE.
 *
 */
#define NANO_FEAT_HTTP_CLIENT                   1
#endif /* NANO_FEAT_HTTP_CLIENT */

/******************************************************************************
 *                             Resource Limits
 ******************************************************************************/

#ifndef NANO_LIMIT_LOG_LEVEL_DEFAULT
/*e
 * @brief Default logging verbosity level.
 * 
 */
#define NANO_LIMIT_LOG_LEVEL_DEFAULT                     NANO_LOG_LEVEL_ERROR
#endif /* NANO_LIMIT_LOG_LEVEL_DEFAULT */

#ifndef NANO_LIMIT_LOG_LEVEL_MAX
/*i
 * @brief Highest level of logging verbosity.
 * 
 */
#define NANO_LIMIT_LOG_LEVEL_MAX                         NANO_LOG_LEVEL_TRACE_FN
#endif /* NANO_LIMIT_LOG_LEVEL_MAX */

#ifndef NANO_LIMIT_OBJECTREF_MAX_LENGTH
/*i
 * @brief Maximum length of a reference to an XRCE object.
 * 
 */
#define NANO_LIMIT_OBJECTREF_MAX_LENGTH                 128
#endif /* NANO_LIMIT_OBJECTREF_MAX_LENGTH */

#ifndef NANO_LIMIT_XMLREPRESENTATION_MAX_LENGTH
/*i
 * @brief Maximum length of an XRCE object’s XML representation.
 * 
 */
#define NANO_LIMIT_XMLREPRESENTATION_MAX_LENGTH         512
#endif /* NANO_LIMIT_XMLREPRESENTATION_MAX_LENGTH */

#ifndef NANO_LIMIT_CONTENTFILTEREXPR_MAX_LENGTH
/*i
 * @brief Maximum length of a content filter expression.
 * 
 */
#define NANO_LIMIT_CONTENTFILTEREXPR_MAX_LENGTH         256
#endif /* NANO_LIMIT_CONTENTFILTEREXPR_MAX_LENGTH */


#if  !NANO_FEAT_AGENT

#ifndef NANO_LIMIT_SESSION_MAX_UNFRAGMENTED_PAYLOAD
#define NANO_LIMIT_SESSION_MAX_UNFRAGMENTED_PAYLOAD     0
#endif /* NANO_LIMIT_AGENT_MAX_UNFRAGMENTED_PAYLOAD */

#ifndef NANO_LIMIT_RELIABLESTREAM_RECVQUEUE_MAX_LENGTH
#define NANO_LIMIT_RELIABLESTREAM_RECVQUEUE_MAX_LENGTH      0
#endif /* NANO_LIMIT_RELIABLESTREAM_RECVQUEUE_MAX_LENGTH */

#ifndef NANO_LIMIT_RELIABLESTREAM_SENDQUEUE_MAX_LENGTH
/*e
 * @brief Maximum number of unacknowledged output messages stored  by a 
 * reliable XRCE stream.
 * 
 */
#define NANO_LIMIT_RELIABLESTREAM_SENDQUEUE_MAX_LENGTH      4
#endif /* NANO_LIMIT_RELIABLESTREAM_RECVQUEUE_MAX_LENGTH */

#ifndef NANO_LIMIT_RELIABLESTREAM_RESEND_MESSAGE_MAX
/*e
 * @brief Maximum number of messages that will be resent by a
 * reliable XRCE stream upon receiving an ACKNACK requesting messages
 * in the send queue.
 * 
 */
#define NANO_LIMIT_RELIABLESTREAM_RESEND_MESSAGE_MAX \
    NANO_LIMIT_RELIABLESTREAM_SENDQUEUE_MAX_LENGTH
#endif /* NANO_LIMIT_RELIABLESTREAM_RESEND_MESSAGE_MAX */

#ifndef NANO_LIMIT_SESSION_MAX_USER_STREAMS_RELIABLE
/*e
 * @brief Maximum number of reliable “user streams” that may be registered on
 * an XRCE session.
 * 
 */
#define NANO_LIMIT_SESSION_MAX_USER_STREAMS_RELIABLE        4
#endif /* NANO_LIMIT_SESSION_MAX_USER_STREAMS_RELIABLE */

#ifndef NANO_LIMIT_SESSION_MAX_USER_STREAMS_BESTEFFORT
/*e
 * @brief Maximum number of best-effort “user streams” that may be registered
 * on an XRCE session.
 * 
 */
#define NANO_LIMIT_SESSION_MAX_USER_STREAMS_BESTEFFORT      4
#endif /* NANO_LIMIT_SESSION_MAX_USER_STREAMS_BESTEFFORT */

#else

#ifndef NANO_LIMIT_RELIABLESTREAM_RESEND_MESSAGE_MAX
#define NANO_LIMIT_RELIABLESTREAM_RESEND_MESSAGE_MAX        1
#endif /* NANO_LIMIT_RELIABLESTREAM_RESEND_MESSAGE_MAX */

#ifndef NANO_LIMIT_SESSION_MAX_UNFRAGMENTED_PAYLOAD
#define NANO_LIMIT_SESSION_MAX_UNFRAGMENTED_PAYLOAD         NANO_U16_MAX
#endif /* NANO_LIMIT_AGENT_MAX_UNFRAGMENTED_PAYLOAD */

#ifndef NANO_LIMIT_RELIABLESTREAM_RECVQUEUE_MAX_LENGTH
#define NANO_LIMIT_RELIABLESTREAM_RECVQUEUE_MAX_LENGTH      4
#endif /* NANO_LIMIT_RELIABLESTREAM_RECVQUEUE_MAX_LENGTH */

#ifndef NANO_LIMIT_RELIABLESTREAM_SENDQUEUE_MAX_LENGTH
#define NANO_LIMIT_RELIABLESTREAM_SENDQUEUE_MAX_LENGTH      32
#endif /* NANO_LIMIT_RELIABLESTREAM_RECVQUEUE_MAX_LENGTH */

#ifndef NANO_LIMIT_SESSION_MAX_USER_STREAMS_RELIABLE
#define NANO_LIMIT_SESSION_MAX_USER_STREAMS_RELIABLE        126
#endif /* NANO_LIMIT_SESSION_MAX_USER_STREAMS_RELIABLE */

#ifndef NANO_LIMIT_SESSION_MAX_USER_STREAMS_BESTEFFORT
#define NANO_LIMIT_SESSION_MAX_USER_STREAMS_BESTEFFORT      126
#endif /* NANO_LIMIT_SESSION_MAX_USER_STREAMS_BESTEFFORT */

#endif /* NANO_FEAT_AGENT */

#ifndef NANO_LIMIT_SESSION_HEARTBEAT_PERIOD_DEFAULT_SEC
/*e
 * @brief Default period for periodic HEARTBEAT messages (seconds component)
 * 
 */
#define NANO_LIMIT_SESSION_HEARTBEAT_PERIOD_DEFAULT_SEC     0
#endif /* NANO_LIMIT_SESSION_HEARTBEAT_PERIOD_DEFAULT_SEC */
#ifndef NANO_LIMIT_SESSION_HEARTBEAT_PERIOD_DEFAULT_NSEC
/*e
 * @brief Default period for periodic HEARTBEAT messages (nanoseconds component)
 * 
 */
#define NANO_LIMIT_SESSION_HEARTBEAT_PERIOD_DEFAULT_NSEC     100000000 /* 100ms */
#endif /* NANO_LIMIT_SESSION_HEARTBEAT_PERIOD_DEFAULT_SEC */

#ifndef NANO_LIMIT_SESSION_ACKNACK_PERIOD_DEFAULT_SEC
#define NANO_LIMIT_SESSION_ACKNACK_PERIOD_DEFAULT_SEC           0
#endif /* NANO_LIMIT_SESSION_ACKNACK_PERIOD_DEFAULT_SEC */
#ifndef NANO_LIMIT_SESSION_ACKNACK_PERIOD_DEFAULT_NSEC
#define NANO_LIMIT_SESSION_ACKNACK_PERIOD_DEFAULT_NSEC       100000000 /* 100ms */
#endif /* NANO_LIMIT_SESSION_ACKNACK_PERIOD_DEFAULT_NSEC */

#ifndef NANO_LIMIT_TRANSPORT_DEFAULT_MTU
/*e
 * @brief Default MTU used by a transport instance.
 * 
 */
#define NANO_LIMIT_TRANSPORT_DEFAULT_MTU                    1024
#endif /* NANO_LIMIT_TRANSPORT_DEFAULT_MTU */

#ifndef NANO_LIMIT_TRANSPORTLOCATORSEQ_MAX_LENGTH
/*i
 * @brief Maximum length of a TransportLocator sequence.
 * 
 */
#define NANO_LIMIT_TRANSPORTLOCATORSEQ_MAX_LENGTH           1
#endif /* NANO_LIMIT_TRANSPORTLOCATORSEQ_MAX_LENGTH */

#ifndef NANO_LIMIT_TRANSPORTLOCATORSTRING_ADDRESS_MAX_LENGTH
/*i
 * @brief TODO
 * 
 */
#define NANO_LIMIT_TRANSPORTLOCATORSTRING_ADDRESS_MAX_LENGTH    15
#endif /* NANO_LIMIT_TRANSPORTLOCATORSEQ_MAX_LENGTH */

#ifndef NANO_LIMIT_LOG_FILENAME_MAX_LENGTH
/*e
 * @brief Maximum length of a logged file name.
 * 
 */
#define NANO_LIMIT_LOG_FILENAME_MAX_LENGTH                  16
#endif /* NANO_LIMIT_LOG_FILENAME_MAX_LENGTH */

#ifndef NANO_LIMIT_SOCKET_RECV_TIMEOUT_MS
/*e
 * @brief Default timeout for recv() calls on BSD sockets.
 * 
 */
#define NANO_LIMIT_SOCKET_RECV_TIMEOUT_MS                   10
#endif /* NANO_LIMIT_SOCKET_RECV_TIMEOUT_MS */

#ifndef NANO_LIMIT_LWIP_PBUF_OUT_MAX_LENGTH
/*e
 * @brief Maximum number of pbuf structures used to send messages with LwIP.
 * 
 */
#define NANO_LIMIT_LWIP_PBUF_OUT_MAX_LENGTH             4
#endif /* NANO_LIMIT_LWIP_PBUF_OUT_MAX_LENGTH */

#ifndef NANO_LIMIT_SERIALCLIENTTRANSPORT_READ_BUFFER_SIZE
/*e
 * @brief Size of the internal buffer used by the Serial plugin to read in data.
 * 
 */
#define NANO_LIMIT_SERIALCLIENTTRANSPORT_READ_BUFFER_SIZE       128
#endif /* NANO_LIMIT_SERIALCLIENTTRANSPORT_READ_BUFFER_SIZE */

#ifndef NANO_LIMIT_SERIALCLIENTTRANSPORT_AGENT_ADDRESS_DEFAULT
/*e
 * @brief Default address for an Agent communicating over the Serial transport
 * 
 */
#define NANO_LIMIT_SERIALCLIENTTRANSPORT_AGENT_ADDRESS_DEFAULT \
    { { 0xFF, 0x00 }, 0 }
#endif /* NANO_LIMIT_SERIALCLIENTTRANSPORT_AGENT_ADDRESS_DEFAULT */

/******************************************************************************
 *                      Derived (aka internal) Features
 ******************************************************************************/
#define NANO_FEAT_RELIABLESTREAM_RECVQUEUE \
    (NANO_LIMIT_RELIABLESTREAM_RECVQUEUE_MAX_LENGTH > 0)

#define NANO_FEAT_RELIABLESTREAM_SENDQUEUE \
    (NANO_LIMIT_RELIABLESTREAM_SENDQUEUE_MAX_LENGTH > 0)

/*e
 * @brief Enable support for registering best-effort “user streams” on 
 * XRCE sessions.
 * 
 */
#define NANO_FEAT_SESSION_USER_STREAMS_BESTEFFORT \
    (NANO_LIMIT_SESSION_MAX_USER_STREAMS_BESTEFFORT > 0)

/*e
 * @brief Enable support for registering reliable “user streams” on XRCE
 * sessions.
 * 
 */
#define NANO_FEAT_SESSION_USER_STREAMS_RELIABLE \
    (NANO_LIMIT_SESSION_MAX_USER_STREAMS_RELIABLE > 0)


#define NANO_FEAT_SESSION_USER_STREAMS \
    (NANO_FEAT_SESSION_USER_STREAMS_BESTEFFORT ||\
        NANO_FEAT_SESSION_USER_STREAMS_RELIABLE)

/******************************************************************************
 *                            Validate configuration
 ******************************************************************************/

#if 0
#if ((NANO_FEAT_TRANSPORT_PLUGIN_UDPV4 || \
    NANO_FEAT_TRANSPORT_PLUGIN_UDPV6 || \
    NANO_FEAT_TRANSPORT_PLUGIN_TCPV4 || \
    NANO_FEAT_TRANSPORT_PLUGIN_TCPV6 || \
    NANO_FEAT_TRANSPORT_PLUGIN_SERIAL) ^ \
    NANO_FEAT_TRANSPORT)
#error "No transport implementation enabled, or transport implementation selected without enabling transport support."
#endif
#endif

#if ((NANO_FEAT_TRANSPORT_PLUGIN_UDPV4 || \
    NANO_FEAT_TRANSPORT_PLUGIN_TCPV4) && \
    !NANO_FEAT_TRANSPORT_IPV4)
#error "IPv4 support must be enabled when using UDPv4 and TCPv4 transport plugins."
#endif

#if ((NANO_FEAT_TRANSPORT_PLUGIN_UDPV6 || \
    NANO_FEAT_TRANSPORT_PLUGIN_TCPV6) && \
    !NANO_FEAT_TRANSPORT_IPV6)
#error "IPv6 support must be enabled when using UDPv6 and TCPv6 transport plugins."
#endif

/** @} *//* nano_api_libcfg_core */


/******************************************************************************
 *                              Platform Selection
 ******************************************************************************/
/**
 * @addtogroup nano_api_platimpl_id
 * @{
 */

/*e
 * @brief Identifier for an unknown platform.
 * 
 * This value is defined only as a default when automatic platform detection
 * fails. It should not be used to guard code.
 */
#define NANO_PLATFORM_UNKNOWN      0
/*e
 * @brief Identifier a custom target platform.
 * 
 */
#define NANO_PLATFORM_CUSTOM       1
/*e
 * @brief Identifier for Linux platforms.
 * 
 */
#define NANO_PLATFORM_LINUX        2
/*e
 * @brief Identifier for Darwin platforms.
 * 
 */
#define NANO_PLATFORM_DARWIN       3
/*e
 * @brief Identifier for Windows platforms.
 * 
 */
#define NANO_PLATFORM_WINDOWS      4
/*e
 * @brief Identifier for Arduino-based platforms.
 * 
 */
#define NANO_PLATFORM_ARDUINO      5

/* If a NANO_PLATFORM wasn't specified to the compiler,
   then try to detect it automatically from known macros */
#ifndef NANO_PLATFORM

#if (defined (__APPLE__) && defined (__MACH__))

#define NANO_PLATFORM         NANO_PLATFORM_DARWIN

#elif defined(__linux__)

#define NANO_PLATFORM         NANO_PLATFORM_LINUX

#elif defined(__QNXNTO__)

#define NANO_PLATFORM         NANO_PLATFORM_QNX

#elif defined(_MSC_VER) || defined(WIN32)

#define NANO_PLATFORM         NANO_PLATFORM_WINDOWS

#elif defined(ARDUINO) || defined(ESP8266) || defined(ESP32) || defined(__AVR__)

#define NANO_PLATFORM         NANO_PLATFORM_ARDUINO
#else /* unknown platform */
/*e
 * 
 * 
 * @brief The current target build platform.
 * 
 * This macro can be specified by users to select the target build platform,
 * 
 * If not explicitly set, the value will be automatically detected based on
 * available compiler features, or an error will be thrown by the preprocessor.
 * 
 */
#define NANO_PLATFORM           NANO_PLATFORM_UNKNOWN

#error "Failed to automatically detect target build platform. Please define NANO_PLATFORM (use NANO_PLATFORM_CUSTOM for a custom port)"

#endif /* detect NANO_PLATFORM */

#endif /* NANO_PLATFORM */

/*e
 * @brief A flag to indicate the target platform supports POSIX(-ish)
 * features.
 */
#define NANO_PLATFORM_IS_POSIX \
    (NANO_PLATFORM == NANO_PLATFORM_LINUX || \
    NANO_PLATFORM == NANO_PLATFORM_DARWIN || \
    NANO_PLATFORM == NANO_PLATFORM_QNX)

/** @} *//* nano_api_platimpl_id */

/******************************************************************************
 *                                Dll Support
 ******************************************************************************/

#if NANO_PLATFORM == NANO_PLATFORM_WINDOWS

    #if defined(NANO_DLL_EXPORT)
        #define NANODllExport __declspec( dllexport )
    #else
        #define NANODllExport
    #endif /* NANO_DLL_EXPORT */

    #if defined(NANO_DLL_VARIABLE) 
        #if defined(NANO_DLL_EXPORT)
            #define NANODllVariable __declspec( dllexport )
        #else
            #define NANODllVariable __declspec( dllimport )
        #endif /* NANO_DLL_EXPORT */
    #else 
        #define NANODllVariable
    #endif /* NANO_DLL_VARIABLE */

#else /* NANO_PLATFORM != NANO_PLATFORM_WINDOWS */
/*i
 * @brief Export function symbols when building Dll.
 */
#define NANODllExport
/*i
 * @brief Export variable symbols when building Dll, or import them when
 * linking user code.
 */
#define NANODllVariable
#endif /* NANO_PLATFORM == NANO_PLATFORM_WINDOWS */

/** @} *//* nanocore_api_cfg_dll */

/******************************************************************************
 *                              API Implementation
 ******************************************************************************/

#ifndef NANO_CC_DEF_H
#if defined(__STDC__) || defined(_MSC_VER) || \
    NANO_PLATFORM == NANO_PLATFORM_WINDOWS
#define NANO_CC_DEF_H "nano/nano_core_osapi_cc_stdc.h"
#else
#error "Unable to determine compiler, please define NANO_CC_DEF_H"
#endif /* detect NANO_CC_DEF_H */
#endif /* NANO_CC_DEF_H */

#ifndef xrce_str_concat
    #ifndef xxrce_str_concat
        #define xxrce_str_concat(A, B) A ## B
    #endif /* xxrce_str_concat */
    #define xrce_str_concat(A, B)   xxrce_str_concat(A,B)
#endif /* xrce_str_concat */

#ifdef __cplusplus
#define NANO_CPP                1
#else
#define NANO_CPP                0
#endif /* __cplusplus */

#if NANO_FEAT_AGENT
#include "ndds/ndds_c.h"
#endif /* NANO_FEAT_AGENT */

#endif /* nano_core_config_h */
