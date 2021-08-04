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

#if NANO_FEAT_CDR

#if NANO_FEAT_REQUEST && NANO_FEAT_AGENT
/******************************************************************************
 *                            BaseObjectRequest
 ******************************************************************************/
NANO_RetCode
NANO_XRCE_BaseObjectRequest_deserialize_cdr(
    NANO_XRCE_BaseObjectRequest *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_LOG_FN_ENTRY
    
    NANO_CHECK_RC(
        NANO_XRCE_RequestId_deserialize_cdr(&self->request_id, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE RequestId"));

    NANO_CHECK_RC(
        NANO_XRCE_ObjectId_deserialize_cdr(&self->object_id, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE ObjectId"));
    
    rc = NANO_RETCODE_OK;
    
done:
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

NANO_RetCode
NANO_XRCE_BaseObjectRequest_serialize_cdr(
    const NANO_XRCE_BaseObjectRequest *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_XRCE_RequestId_serialize_cdr(&self->request_id, stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE RequestId"));

    NANO_CHECK_RC(
        NANO_XRCE_ObjectId_serialize_cdr(&self->object_id, stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE ObjectId"));
    
    rc = NANO_RETCODE_OK;
    
done:
    return rc;
}
#endif /* NANO_FEAT_REQUEST */

#if NANO_FEAT_REPLY && NANO_FEAT_AGENT
/******************************************************************************
 *                            BaseObjectReply
 ******************************************************************************/
NANO_RetCode
NANO_XRCE_BaseObjectReply_deserialize_cdr(
    NANO_XRCE_BaseObjectReply *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_XRCE_RelatedObjectRequest_deserialize_cdr(
                &self->related_request,stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE RelatedObjectRequest"));

    NANO_CHECK_RC(
        NANO_XRCE_ResultStatus_deserialize_cdr(&self->result, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE ResultStatus"));
    
    rc = NANO_RETCODE_OK;
    
done:
    return rc;
}

NANO_RetCode
NANO_XRCE_BaseObjectReply_serialize_cdr(
    const NANO_XRCE_BaseObjectReply *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_XRCE_RelatedObjectRequest_serialize_cdr(
                &self->related_request,stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE RelatedObjectRequest"));

    NANO_CHECK_RC(
        NANO_XRCE_ResultStatus_serialize_cdr(&self->result, stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE ResultStatus"));
    
    rc = NANO_RETCODE_OK;
    
done:
    return rc;
}
#endif /* NANO_FEAT_REPLY */

#if NANO_FEAT_DELIVERY_CTRL && NANO_FEAT_AGENT
/******************************************************************************
 *                            DataDeliveryControl
 ******************************************************************************/
NANO_RetCode
NANO_XRCE_DataDeliveryControl_deserialize_cdr(
    NANO_XRCE_DataDeliveryControl *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_u16_deserialize_cdr(&self->max_samples,stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE max_samples"));

    NANO_CHECK_RC(
        NANO_u16_deserialize_cdr(&self->max_elapsed_time,stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE max_elapsed_time"));

    NANO_CHECK_RC(
        NANO_u16_deserialize_cdr(&self->max_bytes_per_second,stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE max_bytes_per_second"));

    NANO_CHECK_RC(
        NANO_u16_deserialize_cdr(&self->min_pace_period,stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE min_pace_period"));
    
    rc = NANO_RETCODE_OK;
done:
    return rc;
}

NANO_RetCode
NANO_XRCE_DataDeliveryControl_serialize_cdr(
    const NANO_XRCE_DataDeliveryControl *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_u16_serialize_cdr(&self->max_samples,stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE max_samples"));

    NANO_CHECK_RC(
        NANO_u16_serialize_cdr(&self->max_elapsed_time,stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE max_elapsed_time"));

    NANO_CHECK_RC(
        NANO_u16_serialize_cdr(&self->max_bytes_per_second,stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE max_bytes_per_second"));

    NANO_CHECK_RC(
        NANO_u16_serialize_cdr(&self->min_pace_period,stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE min_pace_period"));

    rc = NANO_RETCODE_OK;
done:
    return rc;
}

#endif /* NANO_FEAT_DELIVERY_CTRL */

#if NANO_FEAT_SUBSCRIBE && NANO_FEAT_AGENT
/******************************************************************************
 *                             ReadSpecification
 ******************************************************************************/

NANO_RetCode
NANO_XRCE_ReadSpecification_deserialize_cdr(
    NANO_XRCE_ReadSpecification *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_XRCE_StreamId_deserialize_cdr(
                &self->preferred_stream_id,stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE StreamId"));
    
    NANO_CHECK_RC(
        NANO_XRCE_DataFormat_deserialize_cdr(
                &self->data_format,stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE DataFormat"));

#if NANO_FEAT_OPTIONAL && NANO_FEAT_CONTENT_FILTER
    NANO_OPTIONALMEMBER_DESERIALIZE(self,content_filter_expr,stream,
            NANO_XRCE_ContentFilterExpr_deserialize_cdr,rc);
    NANO_CHECK_RC(rc, NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE ContentFilterExpr"));
#endif /* NANO_FEAT_OPTIONAL && NANO_FEAT_CONTENT_FILTER */

#if NANO_FEAT_OPTIONAL && NANO_FEAT_DELIVERY_CTRL
    NANO_OPTIONALMEMBER_DESERIALIZE(self,delivery_ctrl,stream,
            NANO_XRCE_DataDeliveryControl_deserialize_cdr, rc);
    NANO_CHECK_RC(rc, NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE DataDeliveryControl"));
#endif /* NANO_FEAT_OPTIONAL && NANO_FEAT_DELIVERY_CTRL */
    
    rc = NANO_RETCODE_OK;
done:
    return rc;
}

NANO_RetCode
NANO_XRCE_ReadSpecification_serialize_cdr(
    const NANO_XRCE_ReadSpecification *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_XRCE_StreamId_serialize_cdr(
                &self->preferred_stream_id,stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE StreamId"));
    
    NANO_CHECK_RC(
        NANO_XRCE_DataFormat_serialize_cdr(
                &self->data_format,stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE DataFormat"));

#if NANO_FEAT_OPTIONAL && NANO_FEAT_CONTENT_FILTER

    NANO_OPTIONALMEMBER_SERIALIZE(self,content_filter_expr,stream,
            NANO_XRCE_ContentFilterExpr_serialize_cdr,rc);
    NANO_CHECK_RC(rc, NANO_LOG_ERROR_MSG("FAILED to SERIALIZE ContentFilterExpr"));
#endif /* NANO_FEAT_OPTIONAL && NANO_FEAT_CONTENT_FILTER */

#if NANO_FEAT_OPTIONAL && NANO_FEAT_DELIVERY_CTRL
    NANO_OPTIONALMEMBER_SERIALIZE(self,delivery_ctrl,stream,
            NANO_XRCE_DataDeliveryControl_serialize_cdr, rc);
    NANO_CHECK_RC(rc, NANO_LOG_ERROR_MSG("FAILED to SERIALIZE DataDeliveryControl"));
#endif /* NANO_FEAT_OPTIONAL && NANO_FEAT_DELIVERY_CTRL */

    rc = NANO_RETCODE_OK;
done:
    return rc;
}
#endif /* NANO_FEAT_SUBSCRIBE */

#if NANO_FEAT_OBJECT_CREATE && NANO_FEAT_AGENT
/******************************************************************************
 *                                  Create
 ******************************************************************************/

NANO_RetCode
NANO_XRCE_CreatePayload_deserialize_cdr(
    NANO_XRCE_CreatePayload *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;

    NANO_LOG_FN_ENTRY
    
    NANO_CHECK_RC(
        NANO_XRCE_BaseObjectRequest_deserialize_cdr(&self->base, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE BaseObjectReply"));

    NANO_CHECK_RC(
        NANO_XRCE_ObjectVariant_deserialize_cdr(&self->object_repr, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE ObjectVariant"));
    
    rc = NANO_RETCODE_OK;
    
done:
    return rc;
}

NANO_RetCode
NANO_XRCE_CreatePayload_serialize_cdr(
    const NANO_XRCE_CreatePayload *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_XRCE_BaseObjectRequest_serialize_cdr(&self->base, stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE BaseObjectRequest"));

    NANO_CHECK_RC(
        NANO_XRCE_ObjectVariant_serialize_cdr(&self->object_repr, stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE ObjectVariant"));
    
done:
    return rc;
}

#endif /* NANO_FEAT_OBJECT_CREATE */

#if NANO_FEAT_OBJECT_INFO && NANO_FEAT_AGENT
/******************************************************************************
 *                                  GetInfo
 ******************************************************************************/

NANO_RetCode
NANO_XRCE_GetInfoPayload_deserialize_cdr(
    NANO_XRCE_GetInfoPayload *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_XRCE_BaseObjectRequest_deserialize_cdr(&self->base, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE BaseObjectRequest"));

    NANO_CHECK_RC(
        NANO_XRCE_InfoMask_deserialize_cdr(&self->info_mask, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE InfoMask"));
    
    rc = NANO_RETCODE_OK;
    
done:
    return rc;
}

NANO_RetCode
NANO_XRCE_GetInfoPayload_serialize_cdr(
    const NANO_XRCE_GetInfoPayload *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_XRCE_BaseObjectRequest_serialize_cdr(&self->base, stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE BaseObjectRequest"));

    NANO_CHECK_RC(
        NANO_XRCE_InfoMask_serialize_cdr(&self->info_mask, stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE InfoMask"));
    
    rc = NANO_RETCODE_OK;
    
done:
    return rc;
}

/******************************************************************************
 *                                Info
 ******************************************************************************/
NANO_RetCode
NANO_XRCE_InfoPayload_deserialize_cdr(
    NANO_XRCE_InfoPayload *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_XRCE_BaseObjectReply_deserialize_cdr(&self->base, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE BaseObjectReply"));

    NANO_CHECK_RC(
        NANO_XRCE_ObjectInfo_deserialize_cdr(&self->object_info, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE ObjectInfo"));

    rc = NANO_RETCODE_OK;
    
done:
    return rc;
}

NANO_RetCode
NANO_XRCE_InfoPayload_serialize_cdr(
    const NANO_XRCE_InfoPayload *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_XRCE_BaseObjectReply_serialize_cdr(&self->base, stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE BaseObjectReply"));

    NANO_CHECK_RC(
        NANO_XRCE_ObjectInfo_serialize_cdr(&self->object_info, stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE ObjectInfo"));
    
    rc = NANO_RETCODE_OK;
    
done:
    return rc;
}

#endif /* NANO_FEAT_OBJECT_INFO */

#if NANO_FEAT_SUBSCRIBE && NANO_FEAT_AGENT
/******************************************************************************
 *                              ReadData
 ******************************************************************************/
NANO_RetCode
NANO_XRCE_ReadDataPayload_deserialize_cdr(
    NANO_XRCE_ReadDataPayload *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_XRCE_BaseObjectRequest_deserialize_cdr(&self->base, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE BaseObjectRequest"));

    NANO_CHECK_RC(
        NANO_XRCE_ReadSpecification_deserialize_cdr(&self->read_spec, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE ReadSpecification"));
    
    rc = NANO_RETCODE_OK;
    
done:
    return rc;
}

NANO_RetCode
NANO_XRCE_ReadDataPayload_serialize_cdr(
    const NANO_XRCE_ReadDataPayload *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_XRCE_BaseObjectRequest_serialize_cdr(&self->base, stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE BaseObjectRequest"));

    NANO_CHECK_RC(
        NANO_XRCE_ReadSpecification_serialize_cdr(&self->read_spec, stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE ReadSpecification"));
    
    rc = NANO_RETCODE_OK;
    
done:
    return rc;
}

#endif /* NANO_FEAT_SUBSCRIBE */

#if NANO_FEAT_PUBLISH && NANO_FEAT_AGENT
/******************************************************************************
 *                              WriteData
 ******************************************************************************/
NANO_RetCode
NANO_XRCE_WriteDataPayload_deserialize_cdr(
    NANO_XRCE_WriteDataPayload *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;

    NANO_LOG_FN_ENTRY
    
    NANO_CHECK_RC(
        NANO_XRCE_BaseObjectRequest_deserialize_cdr(&self->base, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE BaseObjectRequest"));

    /* set data's endianess from cdr stream */
    self->data_repr.little_endian =
        (NANO_CDR_Stream_endianness(stream) == NANO_CDR_ENDIANNESS_LITTLE);

    NANO_CHECK_RC(
        NANO_XRCE_DataRepresentation_deserialize_cdr(&self->data_repr, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE DataRepresentation"));
    
    rc = NANO_RETCODE_OK;
    
done:
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

NANO_RetCode
NANO_XRCE_WriteDataPayload_serialize_cdr(
    const NANO_XRCE_WriteDataPayload *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_XRCE_BaseObjectRequest_serialize_cdr(&self->base, stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE BaseObjectRequest"));

    NANO_CHECK_RC(
        NANO_XRCE_DataRepresentation_serialize_cdr(&self->data_repr, stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE DataRepresentation"));
    
    rc = NANO_RETCODE_OK;
    
done:
    return rc;
}
#endif /* NANO_FEAT_PUBLISH */

#if NANO_FEAT_DATA && NANO_FEAT_AGENT
/******************************************************************************
 *                                  Data
 ******************************************************************************/
NANO_RetCode
NANO_XRCE_DataPayload_deserialize_cdr(
    NANO_XRCE_DataPayload *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_XRCE_RelatedObjectRequest_deserialize_cdr(&self->base, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE RelatedObjectRequest"));

    NANO_CHECK_RC(
        NANO_XRCE_DataRepresentation_deserialize_cdr(&self->data_repr, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE DataRepresentation"));
    
    rc = NANO_RETCODE_OK;
    
done:
    return rc;
}

NANO_RetCode
NANO_XRCE_DataPayload_serialize_cdr(
    const NANO_XRCE_DataPayload *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_XRCE_RelatedObjectRequest_serialize_cdr(&self->base, stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE RelatedObjectRequest"));

    NANO_CHECK_RC(
        NANO_XRCE_DataRepresentation_serialize_cdr(&self->data_repr, stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE DataRepresentation"));
    
    rc = NANO_RETCODE_OK;
    
done:
    return rc;
}

#endif /* NANO_FEAT_DATA */

#if NANO_FEAT_RELIABILITY && NANO_FEAT_AGENT
/******************************************************************************
 *                                  AckNack
 ******************************************************************************/
NANO_RetCode
NANO_XRCE_AckNackPayload_deserialize_cdr(
    NANO_XRCE_AckNackPayload *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_u16_deserialize_cdr(&self->first_unacked_seq_num, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE 1st_unacked_seq_num"));
    
    NANO_CHECK_RC(
        NANO_XRCE_AckNackBitmap_deserialize_cdr(&self->nack_bitmap, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE nack_bitmap"));

    NANO_CHECK_RC(
        NANO_XRCE_StreamId_deserialize_cdr(&self->stream_id, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE stream_id"));
    
    rc = NANO_RETCODE_OK;
    
done:
    return rc;
}

NANO_RetCode
NANO_XRCE_AckNackPayload_serialize_cdr(
    const NANO_XRCE_AckNackPayload *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_u16_serialize_cdr(&self->first_unacked_seq_num, stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE first_unacked_seq_num"));
    
    NANO_CHECK_RC(
        NANO_XRCE_AckNackBitmap_serialize_cdr(&self->nack_bitmap, stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE nack_bitmap"));

    NANO_CHECK_RC(
        NANO_XRCE_StreamId_serialize_cdr(&self->stream_id, stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE stream_id"));
    
    rc = NANO_RETCODE_OK;
    
done:
    return rc;
}

/******************************************************************************
 *                                  Heartbeat
 ******************************************************************************/
NANO_RetCode
NANO_XRCE_HeartbeatPayload_deserialize_cdr(
    NANO_XRCE_HeartbeatPayload *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_u16_deserialize_cdr(&self->first_unacked_seq_num, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE 1st_unacked_seq_num"));

    NANO_CHECK_RC(
        NANO_u16_deserialize_cdr(&self->last_unacked_seq_num, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE last_unacked_seq_num"));

    NANO_CHECK_RC(
        NANO_XRCE_StreamId_deserialize_cdr(&self->stream_id, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE stream_id"));
    
    rc = NANO_RETCODE_OK;
    
done:
    return rc;
}

NANO_RetCode
NANO_XRCE_HeartbeatPayload_serialize_cdr(
    const NANO_XRCE_HeartbeatPayload *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_u16_serialize_cdr(&self->first_unacked_seq_num, stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE first_unacked_seq_num"));

    NANO_CHECK_RC(
        NANO_u16_serialize_cdr(&self->last_unacked_seq_num, stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE last_unacked_seq_num"));

    NANO_CHECK_RC(
        NANO_XRCE_StreamId_serialize_cdr(&self->stream_id, stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE stream_id"));
    
    rc = NANO_RETCODE_OK;
    
done:
    return rc;
}

#endif /* NANO_FEAT_RELIABILITY */

#if NANO_FEAT_TIME_SYNC && NANO_FEAT_AGENT
/******************************************************************************
 *                              TimestampReply
 ******************************************************************************/
NANO_RetCode
NANO_XRCE_TimestampReplyPayload_deserialize_cdr(
    NANO_XRCE_TimestampReplyPayload *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_Time_deserialize_cdr(&self->transmit_timestamp, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE transmit_timestamp"));

    NANO_CHECK_RC(
        NANO_Time_deserialize_cdr(&self->receive_timestamp, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE receive_timestamp"));

    NANO_CHECK_RC(
        NANO_Time_deserialize_cdr(&self->originate_timestamp, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE originate_timestamp"));
    
    rc = NANO_RETCODE_OK;
done:
    return rc;
}

NANO_RetCode
NANO_XRCE_TimestampReplyPayload_serialize_cdr(
    const NANO_XRCE_TimestampReplyPayload *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_CHECK_RC(
        NANO_Time_serialize_cdr(&self->transmit_timestamp, stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE transmit_timestamp"));

    NANO_CHECK_RC(
        NANO_Time_serialize_cdr(&self->receive_timestamp, stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE receive_timestamp"));

    NANO_CHECK_RC(
        NANO_Time_serialize_cdr(&self->originate_timestamp, stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE originate_timestamp"));

    rc = NANO_RETCODE_OK;
done:
    return rc;
}

#endif /* NANO_FEAT_TIME_SYNC */

#if NANO_FEAT_SERVICE_CLIENT && NANO_FEAT_AGENT
/******************************************************************************
 *                              ServiceRequest
 ******************************************************************************/
NANO_RetCode
NANO_XRCE_ServiceRequestPayload_deserialize_cdr(
    NANO_XRCE_ServiceRequestPayload *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;

    NANO_LOG_FN_ENTRY
    
    NANO_CHECK_RC(
        NANO_XRCE_BaseObjectRequest_deserialize_cdr(&self->base, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE BaseObjectRequest"));

    NANO_CHECK_RC(
      NANO_u16_deserialize_cdr(&self->flags, stream),
      NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE service request flags"));
    
    NANO_CHECK_RC(
      NANO_u16_deserialize_cdr(&self->query_len, stream),
      NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE service request query"));

    NANO_CHECK_RC(
      NANO_u32_deserialize_cdr(&self->data_len, stream),
      NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE service request data length"));
    
    NANO_CHECK_RC(
      NANO_u32_deserialize_cdr(&self->metadata_len, stream),
      NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE service request metadata length"));
    
    NANO_CHECK_RC(
      NANO_bool_deserialize_cdr(&self->has_payload, stream),
      NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE service request has_payload"));
    
    if (self->has_payload)
    {
      NANO_XRCE_BinData_initialize(&self->payload);
      NANO_XRCE_BinData_set_serialized_buffer(
          &self->payload,
          NANO_CDR_Stream_head(stream),
          NANO_CDR_Stream_capacity(stream),
          NANO_CDR_Stream_endianness(stream));
    }

    rc = NANO_RETCODE_OK;
done:
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

NANO_RetCode
NANO_XRCE_ServiceReplyPayload_serialize_cdr(
    const NANO_XRCE_ServiceReplyPayload *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_XRCE_BaseObjectReply_serialize_cdr(&self->base, stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE BaseObjectReply"));

    NANO_CHECK_RC(
      NANO_u16_serialize_cdr(&self->status, stream),
      NANO_LOG_ERROR_MSG("FAILED to SERIALIZE service reply status"));

    NANO_CHECK_RC(
      NANO_u32_serialize_cdr(&self->data_len, stream),
      NANO_LOG_ERROR_MSG("FAILED to SERIALIZE service reply data length"));
    
    NANO_CHECK_RC(
      NANO_u32_serialize_cdr(&self->metadata_len, stream),
      NANO_LOG_ERROR_MSG("FAILED to SERIALIZE service reply metadata length"));
    
    NANO_CHECK_RC(
      NANO_bool_serialize_cdr(&self->has_payload, stream),
      NANO_LOG_ERROR_MSG("FAILED to SERIALIZE service reply has_payload"));

    rc = NANO_RETCODE_OK;
done:
    return rc;
}
#endif /* NANO_FEAT_SERVICE_CLIENT */



#endif /* NANO_FEAT_CDR */