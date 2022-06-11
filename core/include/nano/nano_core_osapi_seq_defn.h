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

#include "nano/nano_core_osapi_seq_prelude.h"

#ifdef TSeq_has_initialize
NANO_RetCode
TSeq_initialize(TSeq *const self)
{
    return NANO_TSeqImpl_initialize(self, T, TSeqBuffer);
}
#undef TSeq_has_initialize
#endif /* TSeq_has_initialize */

#ifdef TSeq_has_initialized
NANO_bool
TSeq_initialized(const TSeq *const self)
{
    return NANO_TSeqImpl_initialized(self);
}
#undef TSeq_has_initialized
#endif /* TSeq_has_initialized */

#ifdef TSeq_has_finalize
void
TSeq_finalize(TSeq *const self)
{
    NANO_TSeqImpl_finalize(self, TSeqBuffer);
}
#undef TSeq_has_finalize
#endif /* TSeq_has_finalize */

#ifdef TSeq_has_as_seq
const NANO_Sequence*
TSeq_as_seq(const TSeq *const self)
{
    return NANO_TSeqImpl_as_seq(self);
}
#undef TSeq_has_as_seq
#endif /* TSeq_has_as_seq */

#ifdef TSeq_has_as_seq_mut
NANO_Sequence*
TSeq_as_seq_mut(TSeq *const self)
{
    return NANO_TSeqImpl_as_seq_mut(self);
}
#undef TSeq_has_as_seq_mut
#endif /* TSeq_has_as_seq_mut */

#ifdef TSeq_has_reference
const T*
TSeq_reference(const TSeq *const self, const NANO_usize i)
{
    return NANO_TSeqImpl_reference(self, i, T);
}
#undef TSeq_has_reference
#endif /* TSeq_has_reference */

#ifdef TSeq_has_reference_mut
T*
TSeq_reference_mut(TSeq *const self, const NANO_usize i)
{
    return NANO_TSeqImpl_reference_mut(self, i, T);
}
#undef TSeq_has_reference_mut
#endif /* TSeq_has_reference_mut */

#ifdef TSeq_has_contiguous_buffer
const T*
TSeq_contiguous_buffer(const TSeq *const self)
{
    return NANO_TSeqImpl_contiguous_buffer(self, T);
}
#undef TSeq_has_contiguous_buffer
#endif /* TSeq_has_contiguous_buffer */

#ifdef TSeq_has_contiguous_buffer_mut
T*
TSeq_contiguous_buffer_mut(TSeq *const self)
{
    return NANO_TSeqImpl_contiguous_buffer_mut(self, T);
}
#undef TSeq_has_contiguous_buffer_mut
#endif /* TSeq_has_contiguous_buffer_mut */

#ifdef TSeq_has_set_contiguous_buffer
NANO_RetCode
TSeq_set_contiguous_buffer(
    TSeq *const self,
    T *const buffer,
    const NANO_usize max,
    const NANO_usize len)
{
    return NANO_TSeqImpl_set_contiguous_buffer(self, buffer, max, len, TSeqBuffer);
}
#undef TSeq_has_set_contiguous_buffer
#endif /* TSeq_has_set_contiguous_buffer */

#ifdef TSeq_has_maximum
NANO_usize
TSeq_maximum(const TSeq *const self)
{
    return NANO_TSeqImpl_maximum(self);
}
#undef TSeq_has_maximum
#endif /* TSeq_has_maximum */

#ifdef TSeq_has_length
NANO_usize
TSeq_length(const TSeq *const self)
{
    return NANO_TSeqImpl_length(self);
}
#undef TSeq_has_length
#endif /* TSeq_has_length */

#ifdef TSeq_has_set_length
NANO_RetCode
TSeq_set_length(TSeq *const self, const NANO_usize len)
{
    return NANO_TSeqImpl_set_length(self, len);
}
#undef TSeq_has_set_length
#endif /* TSeq_has_set_length */

#include "nano/nano_core_osapi_seq_end.h"