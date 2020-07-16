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

#ifndef nano_client_arduino_h
#define nano_client_arduino_h

#include <nano/nano_client.hpp>

/**
 * @addtogroup nano_api_arduino
 * @{
 */

/**
 * @brief TODO
 * 
 */
typedef rti::nano::xrce::MessageBuffer                  XrceMessageBuffer;

/**
 * @brief TODO
 * 
 */
typedef rti::nano::xrce::Data                           XrceData;
/**
 * @brief TODO
 * 
 */
typedef rti::nano::xrce::Buffer                         XrceBuffer;
/**
 * @brief TODO
 * 
 */
typedef rti::nano::xrce::StreamId                       XrceStreamId;
/**
 * @brief TODO
 * 
 */
typedef rti::nano::xrce::RequestId                      XrceRequestId;
/**
 * @brief TODO
 * 
 */
typedef rti::nano::xrce::ObjectId                       XrceObjectId;
/**
 * @brief TODO
 * 
 */
typedef rti::nano::xrce::StreamStorage                  XrceStreamStorage;
/**
 * @brief TODO
 * 
 */
typedef rti::nano::xrce::DefaultBestEffortStreamStorage XrceDefaultBestEffortStreamStorage;
/**
 * @brief TODO
 * 
 */
typedef rti::nano::xrce::ClientTransportProperties      XrceClientTransportProperties;
/**
 * @brief TODO
 * 
 */
typedef rti::nano::xrce::ClientTransport                XrceClientTransport;
/**
 * @brief TODO
 * 
 */
typedef rti::nano::xrce::ClientProperties               XrceClientProperties;
/**
 * @brief TODO
 * 
 */
typedef rti::nano::xrce::DataFormat                     XrceDataFormat;
/**
 * @brief TODO
 * 
 */
typedef rti::nano::xrce::ObjectKind                     XrceObjectKind;
/**
 * @brief TODO
 * 
 */
typedef rti::nano::xrce::ReadSpecification              XrceReadSpecification;
/**
 * @brief TODO
 * 
 */
typedef rti::nano::xrce::SeqNum                         XrceSeqNum;

#if NANO_FEAT_RELIABILITY
/**
 * @brief TODO
 * 
 */
typedef rti::nano::xrce::ReliableStream                 XrceReliableStream;
/**
 * @brief TODO
 * 
 */
typedef rti::nano::xrce::DefaultReliableStreamStorage   XrceDefaultReliableStreamStorage;

typedef rti::nano::xrce::ReliableMessageLostReason      ReliableMessageLostReason;
#endif

/**
 * @brief TODO
 * 
 */
typedef rti::nano::xrce::Object                         XrceObject;
/**
 * @brief TODO
 * 
 */
typedef rti::nano::xrce::DomainParticipant              XrceDomainParticipant;
/**
 * @brief TODO
 * 
 */
typedef rti::nano::xrce::Type                           XrceType;
/**
 * @brief TODO
 * 
 */
typedef rti::nano::xrce::Domain                         XrceDomain;
/**
 * @brief TODO
 * 
 */
typedef rti::nano::xrce::Topic                          XrceTopic;
/**
 * @brief TODO
 * 
 */
typedef rti::nano::xrce::Publisher                      XrcePublisher;
/**
 * @brief TODO
 * 
 */
typedef rti::nano::xrce::Subscriber                     XrceSubscriber;
/**
 * @brief TODO
 * 
 */
typedef rti::nano::xrce::DataWriter                     XrceDataWriter;
/**
 * @brief TODO
 * 
 */
typedef rti::nano::xrce::DataReader                     XrceDataReader;

/**
 * @brief TODO
 * 
 */
typedef rti::nano::xrce::Transport                      XrceTransport;

#if NANO_FEAT_TRANSPORT_PLUGIN_SERIAL
/**
 * @brief TODO
 * 
 */
typedef rti::nano::xrce::SerialTransport                XrceSerialTransport;
#endif /* NANO_FEAT_TRANSPORT_PLUGIN_SERIAL */

#if NANO_FEAT_TRANSPORT_PLUGIN_UDPV4
/**
 * @brief TODO
 * 
 */
typedef rti::nano::xrce::UdpTransport                   XrceUdpTransport;
#endif /* NANO_FEAT_TRANSPORT_PLUGIN_UDPV4 */

#if NANO_FEAT_CDR
/**
 * @brief TODO
 * 
 */
typedef rti::nano::xcdr::Stream                         XcdrStream;
#endif /* NANO_FEAT_CDR */

/** @} *//* nano_api_arduino */

#endif /* nano_client_arduino_h */
