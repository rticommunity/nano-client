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

typedef rti::nano::xrce::MessageBuffer                  XrceMessageBuffer;

typedef rti::nano::xrce::Data                           XrceData;
typedef rti::nano::xrce::Buffer                         XrceBuffer;
typedef rti::nano::xrce::StreamId                       XrceStreamId;
typedef rti::nano::xrce::RequestId                      XrceRequestId;
typedef rti::nano::xrce::ObjectId                       XrceObjectId;
typedef rti::nano::xrce::StreamStorage                  XrceStreamStorage;
typedef rti::nano::xrce::DefaultBestEffortStreamStorage XrceDefaultBestEffortStreamStorage;
typedef rti::nano::xrce::ClientTransportProperties      XrceClientTransportProperties;
typedef rti::nano::xrce::ClientTransport                XrceClientTransport;
typedef rti::nano::xrce::ClientProperties               XrceClientProperties;
typedef rti::nano::xrce::DataFormat                     XrceDataFormat;
typedef rti::nano::xrce::ObjectKind                     XrceObjectKind;
typedef rti::nano::xrce::ReadSpecification              XrceReadSpecification;
typedef rti::nano::xrce::SeqNum                         XrceSeqNum;

#if NANO_FEAT_RELIABILITY
typedef rti::nano::xrce::ReliableStream                 XrceReliableStream;
typedef rti::nano::xrce::DefaultReliableStreamStorage   XrceDefaultReliableStreamStorage;
typedef rti::nano::xrce::ReliableMessageLostReason      ReliableMessageLostReason;
#endif

typedef rti::nano::xrce::ReceivedData                   XrceReceivedData;
typedef rti::nano::xrce::Client                         XrceClient;
typedef rti::nano::xrce::Stream                         XrceStream;
typedef rti::nano::xrce::BestEffortStream               XrceBestEffortStream;

typedef rti::nano::xrce::Object                         XrceObject;
typedef rti::nano::xrce::DomainParticipant              XrceDomainParticipant;
typedef rti::nano::xrce::Type                           XrceType;
typedef rti::nano::xrce::Domain                         XrceDomain;
typedef rti::nano::xrce::Topic                          XrceTopic;
typedef rti::nano::xrce::Publisher                      XrcePublisher;
typedef rti::nano::xrce::Subscriber                     XrceSubscriber;
typedef rti::nano::xrce::DataWriter                     XrceDataWriter;
typedef rti::nano::xrce::DataReader                     XrceDataReader;

typedef rti::nano::xrce::Transport                      XrceTransport;

#if NANO_FEAT_TRANSPORT_PLUGIN_SERIAL
typedef rti::nano::xrce::SerialTransport                XrceSerialTransport;
#endif /* NANO_FEAT_TRANSPORT_PLUGIN_SERIAL */

#if NANO_FEAT_TRANSPORT_PLUGIN_UDPV4
typedef rti::nano::xrce::UdpTransport                   XrceUdpTransport;
#endif /* NANO_FEAT_TRANSPORT_PLUGIN_UDPV4 */

#if NANO_FEAT_CDR
typedef rti::nano::xcdr::Stream                         XcdrStream;
#endif /* NANO_FEAT_CDR */

#endif /* nano_client_arduino_h */
