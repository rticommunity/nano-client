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

#if NANO_FEAT_SAMPLEINFO
/******************************************************************************
 *                            SeqNumAndTimestamp
 ******************************************************************************/

NANO_RetCode
NANO_XRCE_SeqNumAndTimestamp_deserialize_cdr(
    NANO_XRCE_SeqNumAndTimestamp *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_u32_deserialize_cdr(&self->sequence_number,stream),
        /* TODO nanolog */);

    NANO_CHECK_RC(
        NANO_u32_deserialize_cdr(&self->session_time_offset,stream),
        /* TODO nanolog */);
    
    rc = NANO_RETCODE_OK;
done:
    return rc;
}

NANO_RetCode
NANO_XRCE_SeqNumAndTimestamp_serialize_cdr(
    const NANO_XRCE_SeqNumAndTimestamp *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_u32_serialize_cdr(&self->sequence_number,stream),
        /* TODO nanolog */);

    NANO_CHECK_RC(
        NANO_u32_serialize_cdr(&self->session_time_offset,stream),
        /* TODO nanolog */);

    rc = NANO_RETCODE_OK;
done:
    return rc;
}

/******************************************************************************
 *                            SampleInfoDetail
 ******************************************************************************/

NANO_RetCode
NANO_XRCE_SampleInfoDetail_deserialize_cdr(
    NANO_XRCE_SampleInfoDetail *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_XRCE_SampleInfoFormat_deserialize_cdr(&self->format, stream),
        /* TODO nanolog */);

    switch (self->format)
    {
    case NANO_XRCE_SAMPLEINFOFORMAT_FORMAT_EMPTY:
    {
        /* NOTHING to deserialize */
        break;
    }
    case NANO_XRCE_SAMPLEINFOFORMAT_FORMAT_SEQNUM:
    {
        NANO_CHECK_RC(
            NANO_u32_deserialize_cdr(&self->value.sequence_number,stream),
            /* TODO nanolog */);
        break;
    }
    case NANO_XRCE_SAMPLEINFOFORMAT_FORMAT_TIMESTAMP:
    {
        NANO_CHECK_RC(
            NANO_u32_deserialize_cdr(&self->value.session_time_offset,stream),
            /* TODO nanolog */);
        break;
    }
    case NANO_XRCE_SAMPLEINFOFORMAT_FORMAT_SEQN_TIMS:
    {
        NANO_CHECK_RC(
            NANO_XRCE_SeqNumAndTimestamp_deserialize_cdr(
                &self->value.seqnum_n_timestamp,stream),
            /* TODO nanolog */);
        break;
    }
    default:
    {
        /* TODO nanolog */
        rc = NANO_RETCODE_INVALID_ARGS;
        goto done;
    }
    }
    
    rc = NANO_RETCODE_OK;
done:
    return rc;
}

NANO_RetCode
NANO_XRCE_SampleInfoDetail_serialize_cdr(
    const NANO_XRCE_SampleInfoDetail *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_CHECK_RC(
        NANO_XRCE_SampleInfoFormat_serialize_cdr(&self->format, stream),
        /* TODO nanolog */);

    switch (self->format)
    {
    case NANO_XRCE_SAMPLEINFOFORMAT_FORMAT_EMPTY:
    {
        /* NOTHING to deserialize */
        break;
    }
    case NANO_XRCE_SAMPLEINFOFORMAT_FORMAT_SEQNUM:
    {
        NANO_CHECK_RC(
            NANO_u32_serialize_cdr(&self->value.sequence_number,stream),
            /* TODO nanolog */);
        break;
    }
    case NANO_XRCE_SAMPLEINFOFORMAT_FORMAT_TIMESTAMP:
    {
        NANO_CHECK_RC(
            NANO_u32_serialize_cdr(&self->value.session_time_offset,stream),
            /* TODO nanolog */);
        break;
    }
    case NANO_XRCE_SAMPLEINFOFORMAT_FORMAT_SEQN_TIMS:
    {
        NANO_CHECK_RC(
            NANO_XRCE_SeqNumAndTimestamp_serialize_cdr(
                &self->value.seqnum_n_timestamp,stream),
            /* TODO nanolog */);
        break;
    }
    default:
    {
        /* TODO nanolog */
        rc = NANO_RETCODE_INVALID_ARGS;
        goto done;
    }
    }

    rc = NANO_RETCODE_OK;
done:
    return rc;
}

/******************************************************************************
 *                                SampleInfo
 ******************************************************************************/

NANO_RetCode
NANO_XRCE_SampleInfo_deserialize_cdr(
    NANO_XRCE_SampleInfo *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_XRCE_SampleInfoFlags_deserialize_cdr(&self->state, stream),
        /* TODO nanolog */);

    NANO_CHECK_RC(
        NANO_XRCE_SampleInfoDetail_deserialize_cdr(&self->detail, stream),
        /* TODO nanolog */);
    
    rc = NANO_RETCODE_OK;
done:
    return rc;
}

NANO_RetCode
NANO_XRCE_SampleInfo_serialize_cdr(
    const NANO_XRCE_SampleInfo *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;

    NANO_CHECK_RC(
        NANO_XRCE_SampleInfoFlags_serialize_cdr(&self->state, stream),
        /* TODO nanolog */);

    NANO_CHECK_RC(
        NANO_XRCE_SampleInfoDetail_serialize_cdr(&self->detail, stream),
        /* TODO nanolog */);

    rc = NANO_RETCODE_OK;
done:
    return rc;
}
#endif /* NANO_FEAT_SAMPLEINFO */

#if NANO_FEAT_DATA_FORMAT_PACKED
/******************************************************************************
 *                             SampleInfoDelta
 ******************************************************************************/


NANO_RetCode
NANO_XRCE_SampleInfoDelta_deserialize_cdr(
    NANO_XRCE_SampleInfoDelta *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_XRCE_SampleInfoFlags_deserialize_cdr(&self->state, stream),
        /* TODO nanolog */);

    NANO_CHECK_RC(
        NANO_u8_deserialize_cdr(&self->seq_number_delta, stream),
        /* TODO nanolog */);

    NANO_CHECK_RC(
        NANO_XRCE_DeciSecond_deserialize_cdr(&self->timestamp_delta, stream),
        /* TODO nanolog */);
    
    rc = NANO_RETCODE_OK;
done:
    return rc;
}

NANO_RetCode
NANO_XRCE_SampleInfoDelta_serialize_cdr(
    const NANO_XRCE_SampleInfoDelta *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_XRCE_SampleInfoFlags_serialize_cdr(&self->state, stream),
        /* TODO nanolog */);

    NANO_CHECK_RC(
        NANO_u8_serialize_cdr(&self->seq_number_delta, stream),
        /* TODO nanolog */);

    NANO_CHECK_RC(
        NANO_XRCE_DeciSecond_serialize_cdr(&self->timestamp_delta, stream),
        /* TODO nanolog */);

    rc = NANO_RETCODE_OK;
done:
    return rc;
}
#endif /* NANO_FEAT_DATA_FORMAT_PACKED */

#if NANO_FEAT_DATA
#if NANO_FEAT_DATA_FORMAT_DATA
/******************************************************************************
 *                                SampleData
 ******************************************************************************/
NANO_RetCode
NANO_XRCE_SampleData_deserialize_cdr(
    NANO_XRCE_SampleData *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;

    NANO_LOG_FN_ENTRY

#if NANO_FEAT_AGENT
    self->serialized_data.value_len = NANO_CDR_Stream_capacity(stream);
    self->serialized_data.value = NANO_CDR_Stream_head(stream);
#else
    /* buffer and len must have been set before calling deserialized */
    NANO_CHECK_RC(
        NANO_XRCE_Data_deserialize_cdr(&self->serialized_data, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE data"));
#endif

    NANO_LOG_TRACE("DESERIALIZED data",
        NANO_LOG_BYTES("buffer",
            self->serialized_data.value,
            self->serialized_data.value_len))

    rc = NANO_RETCODE_OK;

    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

NANO_RetCode
NANO_XRCE_SampleData_serialize_cdr(
    const NANO_XRCE_SampleData *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_XRCE_Data_serialize_cdr(&self->serialized_data, stream),
        /* TODO nanolog */);

    rc = NANO_RETCODE_OK;
done:
    return rc;
}

NANO_usize
NANO_XRCE_SampleData_serialized_size_cdr(
    const NANO_XRCE_SampleData *const self,
    const NANO_CDR_Stream *const stream)
{
    NANO_usize res = 0;

    UNUSED_ARG(stream);

    res += NANO_XRCE_Data_serialized_size_cdr(
            NANO_MEMBER(self,serialized_data), stream);

    return res;
}
#endif /* NANO_FEAT_DATA_FORMAT_DATA */

#if NANO_FEAT_DATA_FORMAT_SAMPLE
/******************************************************************************
 *                                  Sample
 ******************************************************************************/

NANO_RetCode
NANO_XRCE_Sample_deserialize_cdr(
    NANO_XRCE_Sample *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_XRCE_SampleInfo_deserialize_cdr(&self->info, stream),
        /* TODO nanolog */);

    NANO_CHECK_RC(
        NANO_XRCE_SampleData_deserialize_cdr(&self->data, stream),
        /* TODO nanolog */);

    rc = NANO_RETCODE_OK;
done:
    return rc;
}

NANO_RetCode
NANO_XRCE_Sample_serialize_cdr(
    const NANO_XRCE_Sample *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_XRCE_SampleInfo_serialize_cdr(&self->info, stream),
        /* TODO nanolog */);

    NANO_CHECK_RC(
        NANO_XRCE_SampleData_serialize_cdr(&self->data, stream),
        /* TODO nanolog */);

    rc = NANO_RETCODE_OK;
done:
    return rc;
}

NANO_usize
NANO_XRCE_Sample_serialized_size_cdr(
    const NANO_XRCE_Sample *const self,
    const NANO_CDR_Stream *const stream)
{
    NANO_usize res = 0;

    UNUSED_ARG(self);
    UNUSED_ARG(stream);

#if 0
    res += NANO_XRCE_SampleInfo_serialized_size_cdr(
                NANO_MEMBER(self,info),stream);
    res += NANO_XRCE_SampleData_serialized_size_cdr(
                NANO_MEMBER(self,data),stream);
#endif
    return res;
}

#endif /* NANO_FEAT_DATA_FORMAT_SAMPLE */

#if NANO_FEAT_DATA_FORMAT_PACKED
/******************************************************************************
 *                              SampleDelta
 ******************************************************************************/

NANO_RetCode
NANO_XRCE_SampleDelta_deserialize_cdr(
    NANO_XRCE_SampleDelta *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_XRCE_SampleInfoDelta_deserialize_cdr(&self->info_delta, stream),
        /* TODO nanolog */);

    NANO_CHECK_RC(
        NANO_XRCE_SampleData_deserialize_cdr(&self->data, stream),
        /* TODO nanolog */);
    
    rc = NANO_RETCODE_OK;
done:
    return rc;
}

NANO_RetCode
NANO_XRCE_SampleDelta_serialize_cdr(
    const NANO_XRCE_SampleDelta *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_XRCE_SampleInfoDelta_serialize_cdr(&self->info_delta, stream),
        /* TODO nanolog */);

    NANO_CHECK_RC(
        NANO_XRCE_SampleData_serialize_cdr(&self->data, stream),
        /* TODO nanolog */);

    rc = NANO_RETCODE_OK;
done:
    return rc;
}

NANO_usize
NANO_XRCE_SampleDelta_serialized_size_cdr(
    const NANO_XRCE_SampleDelta *const self,
    const NANO_CDR_Stream *const stream)
{
    NANO_usize res = 0;

    res += NANO_XRCE_SampleInfoDelta_serialized_size_cdr(
                NANO_MEMBER(self,info_delta),stream);
    res += NANO_XRCE_SampleData_serialized_size_cdr(
                NANO_MEMBER(self,data),stream);

    return res;
}


/******************************************************************************
 *                              PackedSamples
 ******************************************************************************/

NANO_RetCode
NANO_XRCE_PackedSamples_deserialize_cdr(
    NANO_XRCE_PackedSamples *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_XRCE_SampleInfo_deserialize_cdr(&self->info, stream),
        /* TODO nanolog */);

    NANO_CHECK_RC(
        NANO_XRCE_SampleDeltaSeq_deserialize_cdr(&self->sample_delta_seq, stream),
        /* TODO nanolog */);

    rc = NANO_RETCODE_OK;
done:
    return rc;
}

NANO_RetCode
NANO_XRCE_PackedSamples_serialize_cdr(
    const NANO_XRCE_PackedSamples *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_XRCE_SampleInfo_serialize_cdr(&self->info, stream),
        /* TODO nanolog */);

    NANO_CHECK_RC(
        NANO_XRCE_SampleDeltaSeq_serialize_cdr(&self->sample_delta_seq, stream),
        /* TODO nanolog */);

    rc = NANO_RETCODE_OK;
done:
    return rc;
}
#endif /* NANO_FEAT_DATA_FORMAT_PACKED */

/******************************************************************************
 *                              DataRepresentation
 ******************************************************************************/

NANO_RetCode
NANO_XRCE_DataRepresentation_deserialize_cdr(
    NANO_XRCE_DataRepresentation *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;

    NANO_LOG_FN_ENTRY

    switch (self->format)
    {
    case NANO_XRCE_FORMAT_DATA:
    {
        NANO_CHECK_RC(
            NANO_XRCE_SampleData_deserialize_cdr(&self->value.data, stream),
            NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE sample data"));
        break;
    }
    case NANO_XRCE_FORMAT_SAMPLE:
    {
        NANO_CHECK_RC(
            NANO_XRCE_Sample_deserialize_cdr(&self->value.sample, stream),
            NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE sample"));
        break;
    }
    case NANO_XRCE_FORMAT_DATA_SEQ:
    {
        NANO_CHECK_RC(
            NANO_XRCE_SampleDataSeq_deserialize_cdr(
                &self->value.data_seq, stream),
            NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE sample data seq"));
        break;
    }
    case NANO_XRCE_FORMAT_SAMPLE_SEQ:
    {
        NANO_CHECK_RC(
            NANO_XRCE_SampleSeq_deserialize_cdr(
                &self->value.sample_seq, stream),
            NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE sample seq"));
        break;
    }
    case NANO_XRCE_FORMAT_PACKED_SAMPLES:
    {
        NANO_CHECK_RC(
            NANO_XRCE_PackedSamples_deserialize_cdr(
                &self->value.packed_samples, stream),
            NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE packed samples"));
        break;
    }
    default:
    {
        NANO_LOG_ERROR("INVALID data format",
            NANO_LOG_H8("format",self->format))
        rc = NANO_RETCODE_INVALID_ARGS;
        goto done;
    }
    }

    rc = NANO_RETCODE_OK;
done:
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

NANO_RetCode
NANO_XRCE_DataRepresentation_serialize_cdr(
    const NANO_XRCE_DataRepresentation *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_XRCE_DataFormat_serialize_cdr(&self->format, stream),
        /* TODO nanolog */);

    switch (self->format)
    {
    case NANO_XRCE_FORMAT_DATA:
    {
        NANO_CHECK_RC(
            NANO_XRCE_SampleData_serialize_cdr(&self->value.data, stream),
            /* TODO nanolog */);
        break;
    }
    case NANO_XRCE_FORMAT_SAMPLE:
    {
        NANO_CHECK_RC(
            NANO_XRCE_Sample_serialize_cdr(&self->value.sample, stream),
            /* TODO nanolog */);
        break;
    }
    case NANO_XRCE_FORMAT_DATA_SEQ:
    {
        NANO_CHECK_RC(
            NANO_XRCE_SampleDataSeq_serialize_cdr(
                &self->value.data_seq, stream),
            /* TODO nanolog */);
        break;
    }
    case NANO_XRCE_FORMAT_SAMPLE_SEQ:
    {
        NANO_CHECK_RC(
            NANO_XRCE_SampleSeq_serialize_cdr(
                &self->value.sample_seq, stream),
            /* TODO nanolog */);
        break;
    }
    case NANO_XRCE_FORMAT_PACKED_SAMPLES:
    {
        NANO_CHECK_RC(
            NANO_XRCE_PackedSamples_serialize_cdr(
                &self->value.packed_samples, stream),
            /* TODO nanolog */);
        break;
    }
    default:
    {
        /* TODO nanolog */
        rc = NANO_RETCODE_INVALID_ARGS;
        goto done;
    }
    }

    rc = NANO_RETCODE_OK;
done:
    return rc;
}

#endif /* NANO_FEAT_DATA */

#endif /* NANO_FEAT_CDR */