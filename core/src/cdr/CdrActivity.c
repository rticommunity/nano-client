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

#if NANO_FEAT_OBJECT_INFO && NANO_FEAT_AGENT

/******************************************************************************
 *                             AgentActivityInfo
 ******************************************************************************/

NANO_RetCode
NANO_XRCE_AgentActivityInfo_deserialize_cdr(
    NANO_XRCE_AgentActivityInfo *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_i16_deserialize_cdr(&self->availability, stream),
        /* TODO nanolog */);

    NANO_CHECK_RC(
        NANO_XRCE_TransportLocatorSeq_deserialize_cdr(
            &self->address_seq, stream),
        /* TODO nanolog */);
    
    rc = NANO_RETCODE_OK;
done:
    return rc;
}

NANO_RetCode
NANO_XRCE_AgentActivityInfo_serialize_cdr(
    const NANO_XRCE_AgentActivityInfo *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_i16_serialize_cdr(&self->availability, stream),
        /* TODO nanolog */);

    NANO_CHECK_RC(
        NANO_XRCE_TransportLocatorSeq_serialize_cdr(
            &self->address_seq, stream),
        /* TODO nanolog */);

    rc = NANO_RETCODE_OK;
done:
    return rc;
}

/******************************************************************************
 *                          DataWriterActivityInfo
 ******************************************************************************/

NANO_RetCode
NANO_XRCE_DataWriterActivityInfo_deserialize_cdr(
    NANO_XRCE_DataWriterActivityInfo *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_u64_deserialize_cdr(&self->sample_seq_num, stream),
        /* TODO nanolog */);

    NANO_CHECK_RC(
        NANO_u16_deserialize_cdr(&self->stream_seq_num, stream),
        /* TODO nanolog */);
    
    rc = NANO_RETCODE_OK;
done:
    return rc;
}

NANO_RetCode
NANO_XRCE_DataWriterActivityInfo_serialize_cdr(
    const NANO_XRCE_DataWriterActivityInfo *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;

    NANO_CHECK_RC(
        NANO_u64_serialize_cdr(&self->sample_seq_num, stream),
        /* TODO nanolog */);

    NANO_CHECK_RC(
        NANO_u16_serialize_cdr(&self->stream_seq_num, stream),
        /* TODO nanolog */);

    rc = NANO_RETCODE_OK;
done:
    return rc;
}


/******************************************************************************
 *                          DataReaderActivityInfo
 ******************************************************************************/

NANO_RetCode
NANO_XRCE_DataReaderActivityInfo_deserialize_cdr(
    NANO_XRCE_DataReaderActivityInfo *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;

    NANO_CHECK_RC(
        NANO_u16_deserialize_cdr(&self->highest_acked_seq_num, stream),
        /* TODO nanolog */);
    
    rc = NANO_RETCODE_OK;
done:
    return rc;
}

NANO_RetCode
NANO_XRCE_DataReaderActivityInfo_serialize_cdr(
    const NANO_XRCE_DataReaderActivityInfo *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_u16_serialize_cdr(&self->highest_acked_seq_num, stream),
        /* TODO nanolog */);

    rc = NANO_RETCODE_OK;
done:
    return rc;
}

/******************************************************************************
 *                              ActivityInfoVariant
 ******************************************************************************/

NANO_RetCode
NANO_XRCE_ActivityInfoVariant_deserialize_cdr(
    NANO_XRCE_ActivityInfoVariant *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;

    NANO_CHECK_RC(
        NANO_XRCE_ObjectKind_deserialize_cdr(&self->kind, stream),
        /* TODO nanolog */);

    switch (self->kind)
    {
    case NANO_XRCE_OBJK_DATAWRITER:
    {
        NANO_CHECK_RC(
            NANO_XRCE_DataWriterActivityInfo_deserialize_cdr(
                &self->value.data_writer, stream),
            /* TODO nanolog */);
        break;
    }
    case NANO_XRCE_OBJK_DATAREADER:
    {
        NANO_CHECK_RC(
            NANO_XRCE_DataReaderActivityInfo_deserialize_cdr(
                &self->value.data_reader, stream),
            /* TODO nanolog */);
        break;
    }
    case NANO_XRCE_OBJK_AGENT:
    {
        NANO_CHECK_RC(
            NANO_XRCE_AgentActivityInfo_deserialize_cdr(
                &self->value.agent, stream),
            /* TODO nanolog */);
        break;
    }
    default:
    {
        /* TODO nanolog */
        goto done;
    }
    }

    rc = NANO_RETCODE_OK;
done:
    return rc;
}

NANO_RetCode
NANO_XRCE_ActivityInfoVariant_serialize_cdr(
    const NANO_XRCE_ActivityInfoVariant *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_XRCE_ObjectKind_serialize_cdr(&self->kind, stream),
        /* TODO nanolog */);

    switch (self->kind)
    {
    case NANO_XRCE_OBJK_DATAWRITER:
    {
        NANO_CHECK_RC(
            NANO_XRCE_DataWriterActivityInfo_serialize_cdr(
                &self->value.data_writer, stream),
            /* TODO nanolog */);
        break;
    }
    case NANO_XRCE_OBJK_DATAREADER:
    {
        NANO_CHECK_RC(
            NANO_XRCE_DataReaderActivityInfo_serialize_cdr(
                &self->value.data_reader, stream),
            /* TODO nanolog */);
        break;
    }
    case NANO_XRCE_OBJK_AGENT:
    {
        NANO_CHECK_RC(
            NANO_XRCE_AgentActivityInfo_serialize_cdr(
                &self->value.agent, stream),
            /* TODO nanolog */);
        break;
    }
    default:
    {
        /* TODO nanolog */
        goto done;
    }
    }

    rc = NANO_RETCODE_OK;
done:
    return rc;
}

/******************************************************************************
 *                                ObjectInfo
 ******************************************************************************/

NANO_RetCode
NANO_XRCE_ObjectInfo_deserialize_cdr(
    NANO_XRCE_ObjectInfo *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_OPTIONALMEMBER_DESERIALIZE(self,activity,stream,
            NANO_XRCE_ActivityInfoVariant_deserialize_cdr, rc);
    NANO_CHECK_RC(rc,/* TODO nanolog */);

    NANO_OPTIONALMEMBER_DESERIALIZE(self,config,stream,
            NANO_XRCE_ObjectVariant_deserialize_cdr, rc);
    NANO_CHECK_RC(rc,/* TODO nanolog */);
    
    rc = NANO_RETCODE_OK;
done:
    return rc;
}

NANO_RetCode
NANO_XRCE_ObjectInfo_serialize_cdr(
    const NANO_XRCE_ObjectInfo *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_OPTIONALMEMBER_SERIALIZE(self,activity,stream,
            NANO_XRCE_ActivityInfoVariant_serialize_cdr, rc);
    NANO_CHECK_RC(rc,/* TODO nanolog */);

    NANO_OPTIONALMEMBER_SERIALIZE(self,config,stream,
            NANO_XRCE_ObjectVariant_serialize_cdr, rc);
    NANO_CHECK_RC(rc,/* TODO nanolog */);

    rc = NANO_RETCODE_OK;
done:
    return rc;
}

#endif /* NANO_FEAT_OBJECT_INFO */

#endif /* NANO_FEAT_CDR */