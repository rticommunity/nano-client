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

/*i
 * @brief A typed wrapper for NANO_Sequence.
 * 
 */
typedef struct NANODllExport TSeqI
{
    NANO_Sequence base;
} TSeq;

#ifdef TSeq_has_initialize
/*i
 * @brief A typed wrapper for NANO_Sequence_initialize which will initialize
 * TSeq instances with the correct element size.
 * 
 * @param self 
 * @return 
 */
MUST_CHECK_RETURN
NANODllExport
NANO_RetCode
TSeq_initialize(TSeq *const self);
#undef TSeq_has_initialize
#endif /* TSeq_has_initialize */

#ifdef TSeq_has_initialized
/*i
 * @brief A typed wrapper for NANO_Sequence_initialized.
 * 
 * @param self 
 * @return 
 */
SHOULD_CHECK_RETURN
NANODllExport
NANO_bool
TSeq_initialized(const TSeq *const self);
#undef TSeq_has_initialized
#endif /* TSeq_has_initialized */

#ifdef TSeq_has_finalize
NANODllExport
void
TSeq_finalize(TSeq *const self);
#undef TSeq_has_finalize
#endif /* TSeq_has_finalize */


#ifdef TSeq_has_as_seq
NANODllExport
const NANO_Sequence*
TSeq_as_seq(const TSeq *const self);
#undef TSeq_has_as_seq
#endif /* TSeq_has_as_seq */

#ifdef TSeq_has_as_seq_mut
NANODllExport
NANO_Sequence*
TSeq_as_seq_mut(TSeq *const self);
#undef TSeq_has_as_seq_mut
#endif /* TSeq_has_as_seq_mut */

#ifdef TSeq_has_reference
/*i
 * @brief A typed wrapper for NANO_Sequence_reference which returns a reference
 * to a typed element.
 * 
 * @param self 
 * @param i 
 * @return NANODllExport const* TSeq_reference 
 */
NANODllExport
const T*
TSeq_reference(const TSeq *const self, const NANO_usize i);
#undef TSeq_has_reference
#endif /* TSeq_has_reference */

#ifdef TSeq_has_reference_mut
/*i
 * @brief A typed wrapper for NANO_Sequence_reference which returns a reference
 * to a typed element.
 * 
 * @param self 
 * @param i 
 * @return NANODllExport const* TSeq_reference 
 */
NANODllExport
T*
TSeq_reference_mut(TSeq *const self, const NANO_usize i);
#undef TSeq_has_reference_mut
#endif /* TSeq_has_reference_mut */

#ifdef TSeq_has_contiguous_buffer
/*i
 * @brief A typed wrapper for NANO_Sequence_contiguous_buffer which returns a
 * typed buffer rather than a bytes one.
 * 
 * @param self 
 * @return 
 */
NANODllExport
const T*
TSeq_contiguous_buffer(const TSeq *const self);
#undef TSeq_has_contiguous_buffer
#endif /* TSeq_has_contiguous_buffer */

#ifdef TSeq_has_contiguous_buffer_mut
/*i
 * @brief A typed wrapper for NANO_Sequence_contiguous_buffer which returns a
 * typed buffer rather than a bytes one.
 * 
 * @param self 
 * @return 
 */
NANODllExport
T*
TSeq_contiguous_buffer_mut(TSeq *const self);
#undef TSeq_has_contiguous_buffer_mut
#endif /* TSeq_has_contiguous_buffer_mut */

#ifdef TSeq_has_set_contiguous_buffer
/*i
 * @brief A typed wrapper for NANO_Sequence_set_contiguous_buffer which takes
 * a typed buffer rather than a bytes one.
 * 
 * @param self 
 * @param buffer 
 * @param max 
 * @param len 
 * @return 
 */
NANODllExport
NANO_RetCode
TSeq_set_contiguous_buffer(
    TSeq *const self,
    T *const buffer,
    const NANO_usize max,
    const NANO_usize len);
#undef TSeq_has_set_contiguous_buffer
#endif /* TSeq_has_set_contiguous_buffer */

#ifdef TSeq_has_maximum
/*i
 * @brief A typed wrapper for NANO_Sequence_maximum.
 * 
 * @param self 
 * @return NANODllExport TSeq_maximum 
 */
NANODllExport
NANO_usize
TSeq_maximum(const TSeq *const self);
#undef TSeq_has_maximum
#endif /* TSeq_has_maximum */

#ifdef TSeq_has_length
NANODllExport
NANO_usize
TSeq_length(const TSeq *const self);
#undef TSeq_has_length
#endif /* TSeq_has_length */

#ifdef TSeq_has_set_length
/*i
 * @brief A typed wrapper for NANO_Sequence_set_length.
 * 
 * @param self 
 * @param len 
 * @return NANODllExport TSeq_set_length 
 */
NANODllExport
NANO_RetCode
TSeq_set_length(TSeq *const self, const NANO_usize len);
#undef TSeq_has_set_length
#endif /* TSeq_has_set_length */

#ifdef TSeq_has_copy
NANODllExport
NANO_RetCode
TSeq_copy(TSeq *const self, const TSeq *const from);
#undef TSeq_has_copy
#endif /* TSeq_has_copy */

#include "nano/nano_core_osapi_seq_end.h"
