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
#ifndef nan_core_cdr_agent_h
#define nan_core_cdr_agent_h
#include "nano/nano_core.h"

#if NANO_FEAT_CDR && NANO_FEAT_AGENT

#if NANO_FEAT_REQUEST
/******************************************************************************
 *                            BaseObjectRequest
 ******************************************************************************/
NANO_RetCode
NANO_XRCE_BaseObjectRequest_deserialize_cdr(
    NANO_XRCE_BaseObjectRequest *const self,
    NANO_CDR_Stream *const stream);

NANO_RetCode
NANO_XRCE_BaseObjectRequest_serialize_cdr(
    const NANO_XRCE_BaseObjectRequest *const self,
    NANO_CDR_Stream *const stream);
#endif /* NANO_FEAT_REQUEST */

#if NANO_FEAT_REPLY
/******************************************************************************
 *                            BaseObjectReply
 ******************************************************************************/
NANO_RetCode
NANO_XRCE_BaseObjectReply_deserialize_cdr(
    NANO_XRCE_BaseObjectReply *const self,
    NANO_CDR_Stream *const stream);

NANO_RetCode
NANO_XRCE_BaseObjectReply_serialize_cdr(
    const NANO_XRCE_BaseObjectReply *const self,
    NANO_CDR_Stream *const stream);
#endif /* NANO_FEAT_REPLY */

#if NANO_FEAT_DELIVERY_CTRL
/******************************************************************************
 *                            DataDeliveryControl
 ******************************************************************************/
NANO_RetCode
NANO_XRCE_DataDeliveryControl_deserialize_cdr(
    NANO_XRCE_DataDeliveryControl *const self,
    NANO_CDR_Stream *const stream);

NANO_RetCode
NANO_XRCE_DataDeliveryControl_serialize_cdr(
    const NANO_XRCE_DataDeliveryControl *const self,
    NANO_CDR_Stream *const stream);
#endif /* NANO_FEAT_DELIVERY_CTRL */

#if NANO_FEAT_SUBSCRIBE
/******************************************************************************
 *                             ReadSpecification
 ******************************************************************************/
NANO_RetCode
NANO_XRCE_ReadSpecification_deserialize_cdr(
    NANO_XRCE_ReadSpecification *const self,
    NANO_CDR_Stream *const stream);

NANO_RetCode
NANO_XRCE_ReadSpecification_serialize_cdr(
    const NANO_XRCE_ReadSpecification *const self,
    NANO_CDR_Stream *const stream);
#endif /* NANO_FEAT_SUBSCRIBE */

#if NANO_FEAT_OBJECT_CREATE
/******************************************************************************
 *                                  Create
 ******************************************************************************/

NANO_RetCode
NANO_XRCE_CreatePayload_deserialize_cdr(
    NANO_XRCE_CreatePayload *const self,
    NANO_CDR_Stream *const stream);

NANO_RetCode
NANO_XRCE_CreatePayload_serialize_cdr(
    const NANO_XRCE_CreatePayload *const self,
    NANO_CDR_Stream *const stream);

#endif /* NANO_FEAT_OBJECT_CREATE */

#if NANO_FEAT_OBJECT_INFO
/******************************************************************************
 *                                  GetInfo
 ******************************************************************************/

NANO_RetCode
NANO_XRCE_GetInfoPayload_deserialize_cdr(
    NANO_XRCE_GetInfoPayload *const self,
    NANO_CDR_Stream *const stream);

NANO_RetCode
NANO_XRCE_GetInfoPayload_serialize_cdr(
    const NANO_XRCE_GetInfoPayload *const self,
    NANO_CDR_Stream *const stream);

/******************************************************************************
 *                                Info
 ******************************************************************************/
NANO_RetCode
NANO_XRCE_InfoPayload_deserialize_cdr(
    NANO_XRCE_InfoPayload *const self,
    NANO_CDR_Stream *const stream);

NANO_RetCode
NANO_XRCE_InfoPayload_serialize_cdr(
    const NANO_XRCE_InfoPayload *const self,
    NANO_CDR_Stream *const stream);

#endif /* NANO_FEAT_OBJECT_INFO */

#if NANO_FEAT_SUBSCRIBE
/******************************************************************************
 *                              ReadData
 ******************************************************************************/
NANO_RetCode
NANO_XRCE_ReadDataPayload_deserialize_cdr(
    NANO_XRCE_ReadDataPayload *const self,
    NANO_CDR_Stream *const stream);

NANO_RetCode
NANO_XRCE_ReadDataPayload_serialize_cdr(
    const NANO_XRCE_ReadDataPayload *const self,
    NANO_CDR_Stream *const stream);

#endif /* NANO_FEAT_SUBSCRIBE */

#if NANO_FEAT_PUBLISH
/******************************************************************************
 *                              WriteData
 ******************************************************************************/
NANO_RetCode
NANO_XRCE_WriteDataPayload_deserialize_cdr(
    NANO_XRCE_WriteDataPayload *const self,
    NANO_CDR_Stream *const stream);

NANO_RetCode
NANO_XRCE_WriteDataPayload_serialize_cdr(
    const NANO_XRCE_WriteDataPayload *const self,
    NANO_CDR_Stream *const stream);
#endif /* NANO_FEAT_PUBLISH */

#if NANO_FEAT_DATA
/******************************************************************************
 *                                  Data
 ******************************************************************************/
NANO_RetCode
NANO_XRCE_DataPayload_deserialize_cdr(
    NANO_XRCE_DataPayload *const self,
    NANO_CDR_Stream *const stream);

NANO_RetCode
NANO_XRCE_DataPayload_serialize_cdr(
    const NANO_XRCE_DataPayload *const self,
    NANO_CDR_Stream *const stream);

#endif /* NANO_FEAT_DATA */

#if NANO_FEAT_RELIABILITY
/******************************************************************************
 *                                  AckNack
 ******************************************************************************/
NANO_RetCode
NANO_XRCE_AckNackPayload_deserialize_cdr(
    NANO_XRCE_AckNackPayload *const self,
    NANO_CDR_Stream *const stream);

NANO_RetCode
NANO_XRCE_AckNackPayload_serialize_cdr(
    const NANO_XRCE_AckNackPayload *const self,
    NANO_CDR_Stream *const stream);

/******************************************************************************
 *                                  Heartbeat
 ******************************************************************************/
NANO_RetCode
NANO_XRCE_HeartbeatPayload_deserialize_cdr(
    NANO_XRCE_HeartbeatPayload *const self,
    NANO_CDR_Stream *const stream);

NANO_RetCode
NANO_XRCE_HeartbeatPayload_serialize_cdr(
    const NANO_XRCE_HeartbeatPayload *const self,
    NANO_CDR_Stream *const stream);
#endif /* NANO_FEAT_RELIABILITY */

#if NANO_FEAT_TIME_SYNC
/******************************************************************************
 *                              TimestampReply
 ******************************************************************************/
NANO_RetCode
NANO_XRCE_TimestampReplyPayload_deserialize_cdr(
    NANO_XRCE_TimestampReplyPayload *const self,
    NANO_CDR_Stream *const stream);

NANO_RetCode
NANO_XRCE_TimestampReplyPayload_serialize_cdr(
    const NANO_XRCE_TimestampReplyPayload *const self,
    NANO_CDR_Stream *const stream);
#endif /* NANO_FEAT_TIME_SYNC */

#if NANO_FEAT_SERVICE_CLIENT
/******************************************************************************
 *                              ServiceRequest
 ******************************************************************************/
NANO_RetCode
NANO_XRCE_ServiceRequestPayload_deserialize_cdr(
    NANO_XRCE_ServiceRequestPayload *const self,
    NANO_CDR_Stream *const stream);

NANO_RetCode
NANO_XRCE_ServiceReplyPayload_serialize_cdr(
    const NANO_XRCE_ServiceReplyPayload *const self,
    NANO_CDR_Stream *const stream);
#endif /* NANO_FEAT_SERVICE_CLIENT */
#endif /* NANO_FEAT_CDR */
#endif /* nan_core_cdr_agent_h */