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

#if NANO_FEAT_SEQUENCE

#if 0
void
NANO_Sequence_set_contiguous_buffer(
    NANO_Sequence *const self,
    NANO_u8 *const buffer,
    const NANO_usize max,
    const NANO_usize len)
{
    NANO_PCOND(self != NULL)
    NANO_PCOND(NANO_Sequence_initialized(self))
    NANO_PCOND((buffer == NULL && (max != 0 || len != 0)) ||
                (buffer != NULL && (max == 0 || len > max)))
    self->buffer = buffer;
    self->max = max;
    self->len = len;
}


NANO_RetCode
NANO_Sequence_copy(
    NANO_Sequence *const self,
    const NANO_Sequence *const from)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    if ((const NANO_Sequence*)self == from ||
        self->el_size != from->el_size)
    {
        rc = NANO_RETCODE_INVALID_ARGS;
        goto done;
    }

    *self = *from;
    
    rc = NANO_RETCODE_OK;
    
done:
    return rc;
}

#endif

#define NANO_SequenceIterator_ended(s_) \
    (NANO_SequenceIterator_deserialized_count((s_)) == \
            NANO_Sequence_length((s_)->seq) || \
        NANO_SequenceIterator_remaining((s_)) == 0)

NANO_RetCode
NANO_SequenceIterator_next(
    NANO_SequenceIterator *const self,
    void *const next,
    NANO_bool *const has_next)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;

    NANO_PCOND(self != NULL)
    NANO_PCOND(next != NULL)
    NANO_PCOND(has_next != NULL)
    
    *has_next = NANO_BOOL_FALSE;

    if (NANO_SequenceIterator_ended(self))
    {
        rc = NANO_RETCODE_OK;
        goto done;
    }

    NANO_CHECK_RC(
        NANO_SequenceIterator_deserialize_element(self, next),
        NANO_LOG_ERROR_MSG("FAILED to deserialize next element"));
    
    self->el_count += 1;

    *has_next = NANO_BOOL_TRUE;
    
    rc = NANO_RETCODE_OK;
    
done:
    return rc;
}

NANO_bool
NANO_SequenceIterator_align(
    NANO_SequenceIterator *const self, 
    const NANO_usize align)
{
    NANO_bool rc = NANO_BOOL_FALSE;
    NANO_u8 *ahead = NULL;
    NANO_usize adiff = 0;

    NANO_PCOND(self != NULL)
    NANO_PCOND(align > 0)

    ahead = NANO_OSAPI_Memory_align_ptr(self->head, align);
    adiff = (ahead - self->head);

    if (!NANO_SequenceIterator_check_capacity(self, adiff))
    {
        goto done;
    }

    NANO_SequenceIterator_consume(self, adiff);
    
    
    rc = NANO_BOOL_TRUE;
    
done:
    return rc;
}

#endif /* NANO_FEAT_SEQUENCE */