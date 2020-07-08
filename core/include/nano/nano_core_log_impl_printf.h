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

#ifndef nano_core_log_impl_printf
#define nano_core_log_impl_printf

#if NANO_FEAT_LOG && NANO_FEAT_LOG_IMPL_PRINTF

#ifndef NANO_HAVE_PRINTF
#error "target platform does not implement printf()"
#endif /* NANO_HAVE_PRINTF */

/******************************************************************************
 *                          Log Implementation
 ******************************************************************************/

void
NANO_LOG_PrintfWriter_begin_entry(
    NANO_LOG_Writer *const self,
    const NANO_LOG_Level lvl,
    const NANO_u32 ts,
    const char *const file,
    const NANO_usize file_line,
    const char *const function,
    const char *const header);

void
NANO_LOG_PrintfWriter_end_entry(NANO_LOG_Writer *const self);


void
NANO_LOG_PrintfWriter_fn_call(
    NANO_LOG_Writer *const self,
    const char *const fn);

void
NANO_LOG_PrintfWriter_fn_return(
    NANO_LOG_Writer *const self,
    const char *const fn);

void
NANO_LOG_PrintfWriter_write_rc(
    NANO_LOG_Writer *const self, const char *const key, const NANO_RetCode val);

void
NANO_LOG_PrintfWriter_write_str(
    NANO_LOG_Writer *const self,
    const char *const key,
    const char *const val);

void
NANO_LOG_PrintfWriter_write_ts_ms(
    NANO_LOG_Writer *const self, const char *const key, const NANO_u32 val);

void
NANO_LOG_PrintfWriter_write_bool(
    NANO_LOG_Writer *const self, const char *const key, const NANO_bool val);

void
NANO_LOG_PrintfWriter_write_u8(
    NANO_LOG_Writer *const self, const char *const key, const NANO_u8 val);

void
NANO_LOG_PrintfWriter_write_u16(
    NANO_LOG_Writer *const self, const char *const key, const NANO_u16 val);

void
NANO_LOG_PrintfWriter_write_u32(
    NANO_LOG_Writer *const self, const char *const key, const NANO_u32 val);

void
NANO_LOG_PrintfWriter_write_u64(
    NANO_LOG_Writer *const self, const char *const key, const NANO_u64 val);

void
NANO_LOG_PrintfWriter_write_i8(
    NANO_LOG_Writer *const self, const char *const key, const NANO_i8 val);

void
NANO_LOG_PrintfWriter_write_i16(
    NANO_LOG_Writer *const self, const char *const key, const NANO_i16 val);

void
NANO_LOG_PrintfWriter_write_i32(
    NANO_LOG_Writer *const self, const char *const key, const NANO_i32 val);

void
NANO_LOG_PrintfWriter_write_i64(
    NANO_LOG_Writer *const self, const char *const key, const NANO_i64 val);

void
NANO_LOG_PrintfWriter_write_h8(
    NANO_LOG_Writer *const self, const char *const key, const NANO_u8 val);

void
NANO_LOG_PrintfWriter_write_h16(
    NANO_LOG_Writer *const self, const char *const key, const NANO_u16 val);

void
NANO_LOG_PrintfWriter_write_h32(
    NANO_LOG_Writer *const self, const char *const key, const NANO_u32 val);

void
NANO_LOG_PrintfWriter_write_h64(
    NANO_LOG_Writer *const self, const char *const key, const NANO_u64 val);

void
NANO_LOG_PrintfWriter_write_usize(
    NANO_LOG_Writer *const self, const char *const key, const NANO_usize val);

void
NANO_LOG_PrintfWriter_write_sn(
    NANO_LOG_Writer *const self, const char *const key, const NANO_XRCE_SeqNum val);

void
NANO_LOG_PrintfWriter_write_clientkey(
    NANO_LOG_Writer *const self,
    const char *const key,
    const NANO_XRCE_ClientKey val);

void
NANO_LOG_PrintfWriter_write_streamid(
    NANO_LOG_Writer *const self,
    const char *const key,
    const NANO_XRCE_StreamId val);

void
NANO_LOG_PrintfWriter_write_sessionid(
    NANO_LOG_Writer *const self,
    const char *const key,
    const NANO_XRCE_SessionId val);

void
NANO_LOG_PrintfWriter_write_msghdr(
    NANO_LOG_Writer *const self,
    const char *const key,
    const NANO_XRCE_MessageHeader val);

void
NANO_LOG_PrintfWriter_write_submsghdr(
    NANO_LOG_Writer *const self,
    const char *const key,
    const NANO_XRCE_SubmessageHeader val);


void
NANO_LOG_PrintfWriter_write_ptr(
    NANO_LOG_Writer *const self,
    const char *const key,
    const void *const val);

void
NANO_LOG_PrintfWriter_write_locator_small(
    NANO_LOG_Writer *const self,
    const char *const key,
    const NANO_XRCE_TransportLocatorSmall *const val);

void
NANO_LOG_PrintfWriter_write_locator_medium(
    NANO_LOG_Writer *const self,
    const char *const key,
    const NANO_XRCE_TransportLocatorMedium *const val);

void
NANO_LOG_PrintfWriter_write_locator_large(
    NANO_LOG_Writer *const self,
    const char *const key,
    const NANO_XRCE_TransportLocatorLarge *const val);

void
NANO_LOG_PrintfWriter_write_locator_string(
    NANO_LOG_Writer *const self,
    const char *const key,
    const NANO_XRCE_TransportLocatorString *const val);

void
NANO_LOG_PrintfWriter_write_message_buffer(
        NANO_LOG_Writer *const self,
        const char *const key,
        const NANO_MessageBuffer *const val);

void
NANO_LOG_PrintfWriter_write_bytes(
        NANO_LOG_Writer *const self,
        const char *const key,
        const NANO_u8 *const val,
        const NANO_usize len);

#if NANO_FEAT_REQUEST
void
NANO_LOG_PrintfWriter_write_objectid(
        NANO_LOG_Writer *const self,
        const char *const key,
        const NANO_XRCE_ObjectId val);

void
NANO_LOG_PrintfWriter_write_requestid(
        NANO_LOG_Writer *const self,
        const char *const key,
        const NANO_XRCE_RequestId val);
#endif /* NANO_FEAT_REQUEST */


#if NANO_FEAT_REQUEST
#define NANO_LOG_PRINTFWRITER_REQUEST_INITIALIZER \
    NANO_LOG_PrintfWriter_write_objectid,\
    NANO_LOG_PrintfWriter_write_requestid,
#else
#define NANO_LOG_PRINTFWRITER_REQUEST_INITIALIZER
#endif /* NANO_FEAT_REQUEST */


typedef struct NANODllExport NANO_LOG_PrintfWriterI
{
    NANO_LOG_Writer base;
} NANO_LOG_PrintfWriter;

#define NANO_LOG_PRINTFWRITER_INITIALIZER \
{\
    {\
        NANO_LOG_PrintfWriter_begin_entry, /* begin_entry */\
        NANO_LOG_PrintfWriter_end_entry, /* end_entry */\
        NANO_LOG_PrintfWriter_fn_call, /* fn_call */\
        NANO_LOG_PrintfWriter_fn_return, /* fn_return */\
        NANO_LOG_PrintfWriter_write_rc, /* write_rc */\
        NANO_LOG_PrintfWriter_write_str, /* write_str */\
        NANO_LOG_PrintfWriter_write_ts_ms, /* write_ts_ms */\
        NANO_LOG_PrintfWriter_write_bool, /* write_bool */\
        NANO_LOG_PrintfWriter_write_u8, /* write_u8 */\
        NANO_LOG_PrintfWriter_write_u16, /* write_u16 */\
        NANO_LOG_PrintfWriter_write_u32, /* write_u32 */\
        NANO_LOG_PrintfWriter_write_u64, /* write_u64 */\
        NANO_LOG_PrintfWriter_write_i8, /* write_i8 */\
        NANO_LOG_PrintfWriter_write_i16, /* write_i16 */\
        NANO_LOG_PrintfWriter_write_i32, /* write_i32 */\
        NANO_LOG_PrintfWriter_write_i64, /* write_i64 */\
        NANO_LOG_PrintfWriter_write_h8, /* write_h8 */\
        NANO_LOG_PrintfWriter_write_h16, /* write_h16 */\
        NANO_LOG_PrintfWriter_write_h32, /* write_h32 */\
        NANO_LOG_PrintfWriter_write_h64, /* write_h64 */\
        NANO_LOG_PrintfWriter_write_usize, /* write_usize */\
        NANO_LOG_PrintfWriter_write_sn, /* write_sn */\
        NANO_LOG_PrintfWriter_write_clientkey, /* write_clientkey */\
        NANO_LOG_PrintfWriter_write_streamid, /* write_streamid */\
        NANO_LOG_PrintfWriter_write_sessionid, /* write_sessionid */\
        NANO_LOG_PrintfWriter_write_msghdr, /* write_msghdr */\
        NANO_LOG_PrintfWriter_write_submsghdr,  /* write_submsghdr */\
        NANO_LOG_PrintfWriter_write_ptr,  /* write_ptr */\
        NANO_LOG_PrintfWriter_write_locator_small, /* write_locator_small */\
        NANO_LOG_PrintfWriter_write_locator_medium, /* write_locator_medium */\
        NANO_LOG_PrintfWriter_write_locator_large, /* write_locator_large */\
        NANO_LOG_PrintfWriter_write_locator_string, /* write_locator_string */\
        NANO_LOG_PrintfWriter_write_message_buffer, /* write_mbuf */\
        NANO_LOG_PrintfWriter_write_bytes, /* write_bytes */\
        NANO_LOG_PRINTFWRITER_REQUEST_INITIALIZER /* request */\
        NANO_LIMIT_LOG_LEVEL_DEFAULT  /* lvl */\
        NANO_LOG_WRITER_MUTEX_INITIALIZER \
    }\
}

#define NANO_LOG_WriterImpl                 NANO_LOG_PrintfWriter
#define NANO_LOG_WRITERIMPL_INITIALIZER     NANO_LOG_PRINTFWRITER_INITIALIZER

#endif /* NANO_FEAT_LOG  && NANO_FEAT_LOG_IMPL_PRINTF*/

#endif /* nano_core_log_impl_printf */