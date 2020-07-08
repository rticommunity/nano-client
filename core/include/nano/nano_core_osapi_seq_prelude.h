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

#ifndef T
#error "No T defined for Sequence"
#endif /* T */

#ifndef TSeq
#error "No TSeq defined for Sequence"
#endif /* T */

#ifndef TSeqBuffer
#define TSeqBuffer xrce_str_concat(TSeq,Buffer)
#endif /* TSeqBuffer */

#ifndef TSeqBufferItem
#define TSeqBufferItem xrce_str_concat(TSeqBuffer,Item)
#endif /* TSeqBufferItem */

#ifndef TSeqI
#define TSeqI xrce_str_concat(TSeq,I)
#endif /* TSeqI */

#ifndef TSeq_API
#define TSeq_API            NANO_SEQUENCE_API_LEVEL_DEFAULT
#endif /* TSeq_API */

#ifndef TSeq_initialize
#define TSeq_initialize xrce_str_concat(TSeq,_initialize)
#endif /* TSeq_initialize */

#ifndef TSeq_initialized
#define TSeq_initialized xrce_str_concat(TSeq,_initialized)
#endif /* TSeq_initialized */

#ifndef TSeq_finalize
#define TSeq_finalize xrce_str_concat(TSeq,_finalize)
#endif /* TSeq_finalize */

#ifndef TSeq_as_seq
#define TSeq_as_seq xrce_str_concat(TSeq,_as_seq)
#endif /* TSeq_as_seq */

#ifndef TSeq_as_seq_mut
#define TSeq_as_seq_mut xrce_str_concat(TSeq,_as_seq_mut)
#endif /* TSeq_as_seq_mut */

#ifndef TSeq_reference
#define TSeq_reference xrce_str_concat(TSeq,_reference)
#endif /* TSeq_reference */

#ifndef TSeq_reference_mut
#define TSeq_reference_mut xrce_str_concat(TSeq,_reference_mut)
#endif /* TSeq_reference_mut */

#ifndef TSeq_length
#define TSeq_length xrce_str_concat(TSeq,_length)
#endif /* TSeq_length */

#ifndef TSeq_set_length
#define TSeq_set_length xrce_str_concat(TSeq,_set_length)
#endif /* TSeq_length */

#ifndef TSeq_maximum
#define TSeq_maximum xrce_str_concat(TSeq,_maximum)
#endif /* TSeq_maximum */

#ifndef TSeq_set_contiguous_buffer
#define TSeq_set_contiguous_buffer xrce_str_concat(TSeq,_set_contiguous_buffer)
#endif /* TSeq_set_contiguous_buffer */

#ifndef TSeq_contiguous_buffer
#define TSeq_contiguous_buffer xrce_str_concat(TSeq,_contiguous_buffer)
#endif /* TSeq_contiguous_buffer */

#ifndef TSeq_contiguous_buffer_mut
#define TSeq_contiguous_buffer_mut xrce_str_concat(TSeq,_contiguous_buffer_mut)
#endif /* TSeq_contiguous_buffer */

#ifndef TSeq_copy
#define TSeq_copy xrce_str_concat(TSeq,_copy)
#endif /* TSeq_copy */

#if TSeq_API == NANO_SEQUENCE_API_LEVEL_BASIC
#define TSeq_has_initialize
#define TSeq_has_initialized
#define TSeq_has_finalize
#define TSeq_has_as_seq
#define TSeq_has_as_seq_mut
#elif TSeq_API == NANO_SEQUENCE_API_LEVEL_TYPED
#define TSeq_has_initialize
#define TSeq_has_initialized
#define TSeq_has_finalize
#define TSeq_has_as_seq
#define TSeq_has_as_seq_mut
#define TSeq_has_length
#define TSeq_has_set_length
#define TSeq_has_maximum
#define TSeq_has_set_contiguous_buffer
#define TSeq_has_contiguous_buffer
#define TSeq_has_contiguous_buffer_mut
#define TSeq_has_reference
#define TSeq_has_reference_mut
#define TSeq_has_copy
#else
#error "Invalid sequence API level specified"
#endif

