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

#if NANO_FEAT_BUFFERPOOL

NANO_RetCode
NANO_BufferPool_initialize(
    NANO_BufferPool *const self,
    const NANO_usize el_size,
    const NANO_u8 el_align,
    NANO_u8 *buffer,
    const NANO_usize buffer_len)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_usize i = 0,
               capacity = 0;

    self->buffer = buffer;
    self->buffer_len = buffer_len;
    self->el_size = el_size;
    self->el_align = el_align;
    self->flags = NANO_BUFFERPOOLFLAGS_DEFAULT;
    self->next = 0;

    capacity = NANO_BUFFERPOOL_CAPACITY(self);

    if (capacity == 0)
    {
        rc = NANO_RETCODE_OUT_OF_RESOURCES;
        goto done;
    }

    for (i = 0; i < capacity; i++)
    {
        NANO_BufferPoolRecord *rec = NANO_BUFFERPOOL_RECORD(self, i);
        rec->next = (i + 1) % capacity;
        rec->flags = NANO_BUFFERPOOLRECORDFLAGS_DEFAULT;
    }

    rc = NANO_RETCODE_OK;
done:
    return rc;
}

NANO_u8 *
NANO_BufferPool_allocate(NANO_BufferPool *const self)
{
    NANO_u8 *res = NULL;
    NANO_BufferPoolRecord *next = NANO_BUFFERPOOL_RECORD(self, self->next);

    if (next != NULL && NANO_BufferPoolRecord_is_free(next))
    {
        NANO_BufferPoolRecord_allocated(next);
        self->next = next->next;
        res = NANO_BUFFERPOOL_BUFFER_FROM_RECORD(self, next);
    }
    return res;
}

void
NANO_BufferPool_release(
    NANO_BufferPool *const self,
    NANO_u8 *const buffer)
{
    NANO_BufferPoolRecord *rec = 
        NANO_BUFFERPOOL_RECORD_FROM_BUFFER(self, buffer);
    NANO_u16 rec_index = NANO_BUFFERPOOL_RECORD_INDEX(self, rec);

    NANO_BufferPoolRecord_released(rec);
    if (self->next != rec_index)
    {
        rec->next = self->next;
        self->next = rec_index;
    }
}

#endif /* NANO_FEAT_BUFFERPOOL */