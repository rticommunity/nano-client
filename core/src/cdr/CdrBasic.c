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

#if NANO_FEAT_CDR && NANO_FEAT_AGENT

#if NANO_FEAT_RESULTSTATUS && NANO_FEAT_AGENT
/******************************************************************************
 *                                ResultStatus
 ******************************************************************************/

NANO_RetCode
NANO_XRCE_ResultStatus_deserialize_cdr(
    NANO_XRCE_ResultStatus *const self,
    NANO_CDR_Stream *stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_XRCE_StatusValue_deserialize_cdr(&self->status, stream),
        /* TODO nanolog */);
    
    NANO_CHECK_RC(
        NANO_u8_deserialize_cdr(&self->implementation_status, stream),
        /* TODO nanolog */);
    
    rc = NANO_RETCODE_OK;
done:
    return rc;
}

NANO_RetCode
NANO_XRCE_ResultStatus_serialize_cdr(
    const NANO_XRCE_ResultStatus *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_XRCE_StatusValue_serialize_cdr(&self->status, stream),
        /* TODO nanolog */);
    
    NANO_CHECK_RC(
        NANO_u8_serialize_cdr(&self->implementation_status, stream),
        /* TODO nanolog */);

    rc = NANO_RETCODE_OK;
done:
    return rc;
}

#endif /* NANO_FEAT_RESULTSTATUS */

#if NANO_FEAT_PROPERTY


NANO_RetCode
NANO_Property_deserialize_cdr(
    NANO_Property *const self,
    NANO_CDR_Stream *stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_String_deserialize_cdr(&self->name.value, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE property::name"));
    
    NANO_CHECK_RC(
        NANO_String_deserialize_cdr(&self->value.value, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE property::value"));
    
    rc = NANO_RETCODE_OK;
done:
    return rc;
}

NANO_RetCode
NANO_Property_serialize_cdr(
    const NANO_Property *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_String_serialize_cdr(&self->name.value, stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE property::name"));
    
    NANO_CHECK_RC(
        NANO_String_serialize_cdr(&self->value.value, stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE property::value"));

    rc = NANO_RETCODE_OK;
done:
    return rc;
}

NANO_usize
NANO_Property_serialized_size_cdr(
    const NANO_Property *const self,
    const NANO_CDR_Stream *const stream)
{
    NANO_usize rc = 0;
    
    rc += NANO_String_serialized_size_cdr(&self->name.value, stream);
    
    rc += NANO_String_serialized_size_cdr(&self->value.value, stream);

    return rc;
}
#endif /* NANO_FEAT_PROPERTY */

#endif /* NANO_FEAT_CDR */
