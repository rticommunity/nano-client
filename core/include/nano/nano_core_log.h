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

#ifndef nano_core_log_h
#define nano_core_log_h

#if NANO_64BIT
#define NANO_LOG_SIZE_FMT        "%lu"
#define NANO_LOG_USIZE_FMT       "%lu"
#else
#define NANO_LOG_SIZE_FMT        "%u"
#define NANO_LOG_USIZE_FMT       "%u"
#endif

typedef NANO_u8 NANO_LOG_Level;

#define NANO_LOG_LEVEL_NONE          0
#define NANO_LOG_LEVEL_ERROR            1
#define NANO_LOG_LEVEL_WARNING          2
#define NANO_LOG_LEVEL_INFO             3
#define NANO_LOG_LEVEL_DEBUG            4
#define NANO_LOG_LEVEL_TRACE            5
#define NANO_LOG_LEVEL_TRACE_FN         6

const char*
NANO_LOG_Level_to_str(const NANO_LOG_Level self);

#define NANO_LOG_Level_to_str(lvl_) \
    (((lvl_) == NANO_LOG_LEVEL_ERROR)?\
        "ERR":\
    ((lvl_) == NANO_LOG_LEVEL_WARNING)?\
        "WRN":\
    ((lvl_) == NANO_LOG_LEVEL_INFO)?\
        "INF":\
    ((lvl_) == NANO_LOG_LEVEL_DEBUG)?\
        "DBG":\
    ((lvl_) == NANO_LOG_LEVEL_TRACE)?\
        "TRC":\
    ((lvl_) == NANO_LOG_LEVEL_TRACE_FN)?\
        "TFN":\
        "---")

typedef struct NANODllExport NANO_LOG_WriterI NANO_LOG_Writer;

typedef void
    (*NANO_LOG_Writer_BeginEntryFn)(
        NANO_LOG_Writer *const self,
        const NANO_LOG_Level lvl,
        const NANO_u32 ts,
        const char *const file,
        const NANO_usize file_line,
        const char *const function,
        const char *const header);

typedef void
    (*NANO_LOG_Writer_EndEntryFn)(
        NANO_LOG_Writer *const self);


typedef void
    (*NANO_LOG_Writer_FnCallFn)(
        NANO_LOG_Writer *const self,
        const char *const fn);


typedef void
    (*NANO_LOG_Writer_FnReturnFn)(
        NANO_LOG_Writer *const self,
        const char *const fn);

typedef void
    (*NANO_LOG_Writer_WriteRetCodeFn)(
        NANO_LOG_Writer *const self,
        const char *const key,
        const NANO_RetCode val);

typedef void
    (*NANO_LOG_Writer_WriteStringFn)(
        NANO_LOG_Writer *const self,
        const char *const key,
        const char *const val);

typedef void
    (*NANO_LOG_Writer_WriteTimestampMsFn)(
        NANO_LOG_Writer *const self,
        const char *const key,
        const NANO_u32 val);

typedef void
    (*NANO_LOG_Writer_WriteBoolFn)(
        NANO_LOG_Writer *const self,
        const char *const key,
        const NANO_bool val);

typedef void
    (*NANO_LOG_Writer_WriteU8Fn)(
        NANO_LOG_Writer *const self,
        const char *const key,
        const NANO_u8 val);

typedef void
    (*NANO_LOG_Writer_WriteU16Fn)(
        NANO_LOG_Writer *const self,
        const char *const key,
        const NANO_u16 val);

typedef void
    (*NANO_LOG_Writer_WriteU32Fn)(
        NANO_LOG_Writer *const self,
        const char *const key,
        const NANO_u32 val);

typedef void
    (*NANO_LOG_Writer_WriteU64Fn)(
        NANO_LOG_Writer *const self,
        const char *const key,
        const NANO_u64 val);

typedef void
    (*NANO_LOG_Writer_WriteI8Fn)(
        NANO_LOG_Writer *const self,
        const char *const key,
        const NANO_i8 val);

typedef void
    (*NANO_LOG_Writer_WriteI16Fn)(
        NANO_LOG_Writer *const self,
        const char *const key,
        const NANO_i16 val);

typedef void
    (*NANO_LOG_Writer_WriteI32Fn)(
        NANO_LOG_Writer *const self,
        const char *const key,
        const NANO_i32 val);

typedef void
    (*NANO_LOG_Writer_WriteI64Fn)(
        NANO_LOG_Writer *const self,
        const char *const key,
        const NANO_i64 val);


typedef void
    (*NANO_LOG_Writer_WriteH8Fn)(
        NANO_LOG_Writer *const self,
        const char *const key,
        const NANO_u8 val);

typedef void
    (*NANO_LOG_Writer_WriteH16Fn)(
        NANO_LOG_Writer *const self,
        const char *const key,
        const NANO_u16 val);

typedef void
    (*NANO_LOG_Writer_WriteH32Fn)(
        NANO_LOG_Writer *const self,
        const char *const key,
        const NANO_u32 val);

typedef void
    (*NANO_LOG_Writer_WriteH64Fn)(
        NANO_LOG_Writer *const self,
        const char *const key,
        const NANO_u64 val);

typedef void
    (*NANO_LOG_Writer_WriteUsizeFn)(
        NANO_LOG_Writer *const self,
        const char *const key,
        const NANO_usize val);

typedef void
    (*NANO_LOG_Writer_WriteSeqNumFn)(
        NANO_LOG_Writer *const self,
        const char *const key,
        const NANO_XRCE_SeqNum val);

typedef void
    (*NANO_LOG_Writer_WriteClientKeyFn)(
        NANO_LOG_Writer *const self,
        const char *const key,
        const NANO_XRCE_ClientKey val);

typedef void
    (*NANO_LOG_Writer_WriteStreamIdFn)(
        NANO_LOG_Writer *const self,
        const char *const key,
        const NANO_XRCE_StreamId val);

typedef void
    (*NANO_LOG_Writer_WriteSessionIdFn)(
        NANO_LOG_Writer *const self,
        const char *const key,
        const NANO_XRCE_SessionId val);

#if NANO_FEAT_REQUEST
typedef void
    (*NANO_LOG_Writer_WriteObjectIdFn)(
        NANO_LOG_Writer *const self,
        const char *const key,
        const NANO_XRCE_ObjectId val);

typedef void
    (*NANO_LOG_Writer_WriteRequestIdFn)(
        NANO_LOG_Writer *const self,
        const char *const key,
        const NANO_XRCE_RequestId val);
#endif /* NANO_FEAT_REQUEST */

typedef void
    (*NANO_LOG_Writer_WriteMessageHeaderFn)(
        NANO_LOG_Writer *const self,
        const char *const key,
        const NANO_XRCE_MessageHeader val);

typedef void
    (*NANO_LOG_Writer_WriteSubmessageHeaderFn)(
        NANO_LOG_Writer *const self,
        const char *const key,
        const NANO_XRCE_SubmessageHeader val);

typedef void
    (*NANO_LOG_Writer_WritePtrFn)(
        NANO_LOG_Writer *const self,
        const char *const key,
        const void *const val);

typedef void
    (*NANO_LOG_Writer_WriteLocatorSmallFn)(
        NANO_LOG_Writer *const self,
        const char *const key,
        const NANO_XRCE_TransportLocatorSmall *const val);

typedef void
    (*NANO_LOG_Writer_WriteLocatorMediumFn)(
        NANO_LOG_Writer *const self,
        const char *const key,
        const NANO_XRCE_TransportLocatorMedium *const val);

typedef void
    (*NANO_LOG_Writer_WriteLocatorLargeFn)(
        NANO_LOG_Writer *const self,
        const char *const key,
        const NANO_XRCE_TransportLocatorLarge *const val);

typedef void
    (*NANO_LOG_Writer_WriteLocatorStringFn)(
        NANO_LOG_Writer *const self,
        const char *const key,
        const NANO_XRCE_TransportLocatorString *const val);

typedef void
    (*NANO_LOG_Writer_WriteMessageBufferFn)(
        NANO_LOG_Writer *const self,
        const char *const key,
        const NANO_MessageBuffer *const val);

typedef void
    (*NANO_LOG_Writer_WriteBytesFn)(
        NANO_LOG_Writer *const self,
        const char *const key,
        const NANO_u8 *const val,
        const NANO_usize len);


struct NANO_LOG_WriterI
{
    NANO_LOG_Writer_BeginEntryFn begin_entry;
    NANO_LOG_Writer_EndEntryFn end_entry;
    NANO_LOG_Writer_FnCallFn fn_call;
    NANO_LOG_Writer_FnReturnFn fn_return;
    NANO_LOG_Writer_WriteRetCodeFn write_rc;
    NANO_LOG_Writer_WriteStringFn write_str;
    NANO_LOG_Writer_WriteTimestampMsFn write_ts_ms;
    NANO_LOG_Writer_WriteBoolFn write_bool;
    NANO_LOG_Writer_WriteU8Fn write_u8;
    NANO_LOG_Writer_WriteU16Fn  write_u16;
    NANO_LOG_Writer_WriteU32Fn write_u32;
    NANO_LOG_Writer_WriteU64Fn write_u64;
    NANO_LOG_Writer_WriteI8Fn write_i8;
    NANO_LOG_Writer_WriteI16Fn write_i16;
    NANO_LOG_Writer_WriteI32Fn write_i32;
    NANO_LOG_Writer_WriteI64Fn write_i64;
    NANO_LOG_Writer_WriteH8Fn write_h8;
    NANO_LOG_Writer_WriteH16Fn write_h16;
    NANO_LOG_Writer_WriteH32Fn write_h32;
    NANO_LOG_Writer_WriteH64Fn write_h64;
    NANO_LOG_Writer_WriteUsizeFn write_usize;
    NANO_LOG_Writer_WriteSeqNumFn write_sn;
    NANO_LOG_Writer_WriteClientKeyFn write_clientkey;
    NANO_LOG_Writer_WriteStreamIdFn write_streamid;
    NANO_LOG_Writer_WriteSessionIdFn write_sessionid;
    NANO_LOG_Writer_WriteMessageHeaderFn write_msghdr;
    NANO_LOG_Writer_WriteSubmessageHeaderFn write_submsghdr;
    NANO_LOG_Writer_WritePtrFn write_ptr;
    NANO_LOG_Writer_WriteLocatorSmallFn write_locator_small;
    NANO_LOG_Writer_WriteLocatorMediumFn write_locator_medium;
    NANO_LOG_Writer_WriteLocatorLargeFn write_locator_large;
    NANO_LOG_Writer_WriteLocatorStringFn write_locator_string;
    NANO_LOG_Writer_WriteMessageBufferFn write_mbuf;
    NANO_LOG_Writer_WriteBytesFn write_bytes;
#if NANO_FEAT_REQUEST
    NANO_LOG_Writer_WriteObjectIdFn write_objectid;
    NANO_LOG_Writer_WriteRequestIdFn write_requestid;
#endif /* NANO_FEAT_REQUEST */
    NANO_LOG_Level lvl;
#if NANO_FEAT_AGENT
    NANO_i32 mutex_status;
#endif /* NANO_FEAT_AGENT */
};

#if NANO_FEAT_REQUEST
#define NANO_LOG_WRITER_REQUEST_INITIALIZER \
    NULL, /* write_objectid */\
    NULL, /* write_requestid */
#else
#define NANO_LOG_WRITER_REQUEST_INITIALIZER
#endif /* NANO_FEAT_REQUEST */

#if NANO_FEAT_AGENT
#define NANO_LOG_WRITER_MUTEX_INITIALIZER \
    ,\
    1 /* mutext_status */
#else
#define NANO_LOG_WRITER_MUTEX_INITIALIZER
#endif /* NANO_FEAT_AGENT */

#define NANO_LOG_WRITER_INITIALIZER \
{\
    NULL, /* begin_entry */\
    NULL, /* end_entry */\
    NULL, /* fn_call */\
    NULL, /* fn_return */\
    NULL, /* write_rc */\
    NULL, /* write_str */\
    NULL, /* write_ts_ms */\
    NULL, /* write_bool */\
    NULL, /* write_u8 */\
    NULL, /* write_u16 */\
    NULL, /* write_u32 */\
    NULL, /* write_u64 */\
    NULL, /* write_i8 */\
    NULL, /* write_i16 */\
    NULL, /* write_i32 */\
    NULL, /* write_i64 */\
    NULL, /* write_h8 */\
    NULL, /* write_h16 */\
    NULL, /* write_h32 */\
    NULL, /* write_h64 */\
    NULL, /* write_usize */\
    NULL, /* write_sn */\
    NULL, /* write_clientkey */\
    NULL, /* write_streamid */\
    NULL, /* write_sessionid */\
    NULL, /* write_msghdr */\
    NULL, /* write_submsghdr */\
    NULL, /* write_ptr */\
    NULL, /* write_locator_small */\
    NULL, /* write_locator_medium */\
    NULL, /* write_locator_large */\
    NULL, /* write_locator_string */\
    NULL, /* write_mbuf */\
    NULL, /* write_bytes */\
    NANO_LOG_WRITER_REQUEST_INITIALIZER /* request */\
    NANO_LIMIT_LOG_LEVEL_DEFAULT /* lvl */\
    NANO_LOG_WRITER_MUTEX_INITIALIZER /* mutex */\
}

void
NANO_LOG_Writer_set_level(
    NANO_LOG_Writer *const self,
    const NANO_LOG_Level lvl);

#define NANO_LOG_Writer_set_level(s_, l_) \
{\
    (s_)->lvl = (l_);\
}

NANO_LOG_Level
NANO_LOG_Writer_level(
    NANO_LOG_Writer *const self);

#define NANO_LOG_Writer_level(s_) \
    ((s_)->lvl)

void
NANO_LOG_Writer_begin_entry(
    NANO_LOG_Writer *const self,
    const NANO_LOG_Level lvl,
    const NANO_u32 ts,
    const char *const file,
    const NANO_usize file_line,
    const char *const function,
    const char *const header);

#define NANO_LOG_Writer_begin_entry(s_,l_,t_,f_,fl_,fn_,h_) \
{\
    if ((s_)->begin_entry != NULL)\
    {\
        (s_)->begin_entry((s_),(l_),(t_),(f_),(fl_),(fn_),(h_));\
    }\
}

void
NANO_LOG_Writer_end_entry(
    NANO_LOG_Writer *const self);

#define NANO_LOG_Writer_end_entry(s_) \
{\
    if ((s_)->end_entry != NULL)\
    {\
        (s_)->end_entry((s_));\
    }\
}

void
NANO_LOG_Writer_fn_call(
    NANO_LOG_Writer *const self,
    const char *const fn);

#define NANO_LOG_Writer_fn_call(s_,fn_) \
{\
    if ((s_)->fn_call != NULL)\
    {\
        (s_)->fn_call((s_),(fn_));\
    }\
}

void
NANO_LOG_Writer_fn_return(
    NANO_LOG_Writer *const self,
    const char *const fn);

#define NANO_LOG_Writer_fn_return(s_,fn_) \
{\
    if ((s_)->fn_return != NULL)\
    {\
        (s_)->fn_return((s_),(fn_));\
    }\
}

void
NANO_LOG_Writer_write_rc(
    NANO_LOG_Writer *const self,
    const char *const key,
    const NANO_RetCode val);

#define NANO_LOG_Writer_write_rc(s_,k_,v_) \
{\
    if ((s_)->write_rc != NULL)\
    {\
        (s_)->write_rc((s_),(k_),(v_));\
    }\
}

void
NANO_LOG_Writer_write_string(
    NANO_LOG_Writer *const self,
    const char *const key,
    const char *const val);

#define NANO_LOG_Writer_write_string(s_,k_,v_) \
{\
    if ((s_)->write_str != NULL)\
    {\
        (s_)->write_str((s_),(k_),(v_));\
    }\
}

void
NANO_LOG_Writer_write_timestamp_ms(
    NANO_LOG_Writer *const self,
    const char *const key,
    const NANO_u32 val);

#define NANO_LOG_Writer_write_timestamp_ms(s_,k_,v_) \
{\
    if ((s_)->write_ts_ms != NULL)\
    {\
        (s_)->write_ts_ms((s_),(k_),(v_));\
    }\
}

void
NANO_LOG_Writer_write_bool(
    NANO_LOG_Writer *const self,
    const char *const key,
    const NANO_bool val);

#define NANO_LOG_Writer_write_bool(s_,k_,v_) \
{\
    if ((s_)->write_bool != NULL)\
    {\
        (s_)->write_bool((s_),(k_),(v_));\
    }\
}

void
NANO_LOG_Writer_write_u8(
    NANO_LOG_Writer *const self,
    const char *const key,
    const NANO_u8 val);

#define NANO_LOG_Writer_write_u8(s_,k_,v_) \
{\
    if ((s_)->write_u8 != NULL)\
    {\
        (s_)->write_u8((s_),(k_),(v_));\
    }\
}

void
NANO_LOG_Writer_write_u16(
    NANO_LOG_Writer *const self,
    const char *const key,
    const NANO_u16 val);

#define NANO_LOG_Writer_write_u16(s_,k_,v_) \
{\
    if ((s_)->write_u16 != NULL)\
    {\
        (s_)->write_u16((s_),(k_),(v_));\
    }\
}

void
NANO_LOG_Writer_write_u32(
    NANO_LOG_Writer *const self,
    const char *const key,
    const NANO_u32 val);

#define NANO_LOG_Writer_write_u32(s_,k_,v_) \
{\
    if ((s_)->write_u32 != NULL)\
    {\
        (s_)->write_u32((s_),(k_),(v_));\
    }\
}

void
NANO_LOG_Writer_write_u64(
    NANO_LOG_Writer *const self,
    const char *const key,
    const NANO_u64 val);

#define NANO_LOG_Writer_write_u64(s_,k_,v_) \
{\
    if ((s_)->write_u64 != NULL)\
    {\
        (s_)->write_u64((s_),(k_),(v_));\
    }\
}


void
NANO_LOG_Writer_write_h8(
    NANO_LOG_Writer *const self,
    const char *const key,
    const NANO_u8 val);

#define NANO_LOG_Writer_write_h8(s_,k_,v_) \
{\
    if ((s_)->write_h8 != NULL)\
    {\
        (s_)->write_h8((s_),(k_),(v_));\
    }\
}

void
NANO_LOG_Writer_write_h16(
    NANO_LOG_Writer *const self,
    const char *const key,
    const NANO_u16 val);

#define NANO_LOG_Writer_write_h16(s_,k_,v_) \
{\
    if ((s_)->write_h16 != NULL)\
    {\
        (s_)->write_h16((s_),(k_),(v_));\
    }\
}

void
NANO_LOG_Writer_write_h32(
    NANO_LOG_Writer *const self,
    const char *const key,
    const NANO_u32 val);

#define NANO_LOG_Writer_write_h32(s_,k_,v_) \
{\
    if ((s_)->write_h32 != NULL)\
    {\
        (s_)->write_h32((s_),(k_),(v_));\
    }\
}

void
NANO_LOG_Writer_write_h64(
    NANO_LOG_Writer *const self,
    const char *const key,
    const NANO_u64 val);

#define NANO_LOG_Writer_write_h64(s_,k_,v_) \
{\
    if ((s_)->write_h64 != NULL)\
    {\
        (s_)->write_h64((s_),(k_),(v_));\
    }\
}


void
NANO_LOG_Writer_write_i8(
    NANO_LOG_Writer *const self,
    const char *const key,
    const NANO_u8 val);

#define NANO_LOG_Writer_write_i8(s_,k_,v_) \
{\
    if ((s_)->write_i8 != NULL)\
    {\
        (s_)->write_i8((s_),(k_),(v_));\
    }\
}

void
NANO_LOG_Writer_write_i16(
    NANO_LOG_Writer *const self,
    const char *const key,
    const NANO_i16 val);

#define NANO_LOG_Writer_write_i16(s_,k_,v_) \
{\
    if ((s_)->write_i16 != NULL)\
    {\
        (s_)->write_i16((s_),(k_),(v_));\
    }\
}

void
NANO_LOG_Writer_write_i32(
    NANO_LOG_Writer *const self,
    const char *const key,
    const NANO_i32 val);

#define NANO_LOG_Writer_write_i32(s_,k_,v_) \
{\
    if ((s_)->write_i32 != NULL)\
    {\
        (s_)->write_i32((s_),(k_),(v_));\
    }\
}

void
NANO_LOG_Writer_write_i64(
    NANO_LOG_Writer *const self,
    const char *const key,
    const NANO_i64 val);


#define NANO_LOG_Writer_write_i64(s_,k_,v_) \
{\
    if ((s_)->write_i64 != NULL)\
    {\
        (s_)->write_i64((s_),(k_),(v_));\
    }\
}

void
NANO_LOG_Writer_write_usize(
    NANO_LOG_Writer *const self,
    const char *const key,
    const NANO_usize val);


#define NANO_LOG_Writer_write_usize(s_,k_,v_) \
{\
    if ((s_)->write_usize != NULL)\
    {\
        (s_)->write_usize((s_),(k_),(v_));\
    }\
}

void
NANO_LOG_Writer_write_sn(
    NANO_LOG_Writer *const self,
    const char *const key,
    const NANO_XRCE_SeqNum val);


#define NANO_LOG_Writer_write_sn(s_,k_,v_) \
{\
    if ((s_)->write_sn != NULL)\
    {\
        (s_)->write_sn((s_),(k_),(v_));\
    }\
}

void
NANO_LOG_Writer_write_clientkey(
    NANO_LOG_Writer *const self,
    const char *const key,
    const NANO_XRCE_ClientKey val);


#define NANO_LOG_Writer_write_clientkey(s_,k_,v_) \
{\
    if ((s_)->write_clientkey != NULL)\
    {\
        (s_)->write_clientkey((s_),(k_),(v_));\
    }\
}

void
NANO_LOG_Writer_write_streamid(
    NANO_LOG_Writer *const self,
    const char *const key,
    const NANO_XRCE_StreamId val);


#define NANO_LOG_Writer_write_streamid(s_,k_,v_) \
{\
    if ((s_)->write_streamid != NULL)\
    {\
        (s_)->write_streamid((s_),(k_),(v_));\
    }\
}

void
NANO_LOG_Writer_write_sessionid(
    NANO_LOG_Writer *const self,
    const char *const key,
    const NANO_XRCE_SessionId val);


#define NANO_LOG_Writer_write_sessionid(s_,k_,v_) \
{\
    if ((s_)->write_sessionid != NULL)\
    {\
        (s_)->write_sessionid((s_),(k_),(v_));\
    }\
}

void
NANO_LOG_Writer_write_message_header(
    NANO_LOG_Writer *const self,
    const char *const key,
    const NANO_XRCE_MessageHeader *const val);


#define NANO_LOG_Writer_write_message_header(s_,k_,v_) \
{\
    if ((s_)->write_msghdr != NULL)\
    {\
        (s_)->write_msghdr((s_),(k_),(v_));\
    }\
}

void
NANO_LOG_Writer_write_submessage_header(
    NANO_LOG_Writer *const self,
    const char *const key,
    const NANO_XRCE_SubmessageHeader *const val);


#define NANO_LOG_Writer_write_submessage_header(s_,k_,v_) \
{\
    if ((s_)->write_submsghdr != NULL)\
    {\
        (s_)->write_submsghdr((s_),(k_),(v_));\
    }\
}

void
NANO_LOG_Writer_write_ptr(
    NANO_LOG_Writer *const self,
    const char *const key,
    const void *const val);


#define NANO_LOG_Writer_write_ptr(s_,k_,v_) \
{\
    if ((s_)->write_ptr != NULL)\
    {\
        (s_)->write_ptr((s_),(k_),(v_));\
    }\
}

void
NANO_LOG_Writer_write_locator(
        NANO_LOG_Writer *const self,
        const char *const key,
        const NANO_XRCE_TransportLocator *const val);

#define NANO_LOG_Writer_write_locator(s_,k_,v_) \
{\
    switch ((v_)->format)\
    {\
    case NANO_XRCE_ADDRESS_FORMAT_SMALL:\
    {\
        NANO_LOG_Writer_write_locator_small((s_),(k_),&(v_)->value.small);\
        break;\
    }\
    case NANO_XRCE_ADDRESS_FORMAT_MEDIUM:\
    {\
        NANO_LOG_Writer_write_locator_medium((s_),(k_),&(v_)->value.medium);\
        break;\
    }\
    case NANO_XRCE_ADDRESS_FORMAT_LARGE:\
    {\
        NANO_LOG_Writer_write_locator_large((s_),(k_),&(v_)->value.large);\
        break;\
    }\
    case NANO_XRCE_ADDRESS_FORMAT_STRING:\
    {\
        NANO_LOG_Writer_write_locator_string((s_),(k_),&(v_)->value.str);\
        break;\
    }\
    default:\
    {\
        NANO_LOG_Writer_write_string((s_),(k_),"invalid locator");\
        break;\
    }\
    }\
}


void
NANO_LOG_Writer_write_locator_small(
        NANO_LOG_Writer *const self,
        const char *const key,
        const NANO_XRCE_TransportLocatorSmall *const val);

#define NANO_LOG_Writer_write_locator_small(s_,k_,v_) \
{\
    if ((s_)->write_locator_small != NULL)\
    {\
        (s_)->write_locator_small((s_),(k_),(v_));\
    }\
}

void
NANO_LOG_Writer_write_locator_medium(
        NANO_LOG_Writer *const self,
        const char *const key,
        const NANO_XRCE_TransportLocatorMedium *const val);

#define NANO_LOG_Writer_write_locator_medium(s_,k_,v_) \
{\
    if ((s_)->write_locator_medium != NULL)\
    {\
        (s_)->write_locator_medium((s_),(k_),(v_));\
    }\
}

void
NANO_LOG_Writer_write_locator_large(
        NANO_LOG_Writer *const self,
        const char *const key,
        const NANO_XRCE_TransportLocatorLarge *const val);

#define NANO_LOG_Writer_write_locator_large(s_,k_,v_) \
{\
    if ((s_)->write_locator_large != NULL)\
    {\
        (s_)->write_locator_large((s_),(k_),(v_));\
    }\
}

void
NANO_LOG_Writer_write_locator_string(
        NANO_LOG_Writer *const self,
        const char *const key,
        const NANO_XRCE_TransportLocatorString *const val);

#define NANO_LOG_Writer_write_locator_string(s_,k_,v_) \
{\
    if ((s_)->write_locator_string != NULL)\
    {\
        (s_)->write_locator_string((s_),(k_),(v_));\
    }\
}

void
NANO_LOG_Writer_write_message_buffer(
        NANO_LOG_Writer *const self,
        const char *const key,
        const NANO_MessageBuffer *const val);

#define NANO_LOG_Writer_write_message_buffer(s_,k_,v_) \
{\
    if ((s_)->write_mbuf != NULL)\
    {\
        (s_)->write_mbuf((s_),(k_),(v_));\
    }\
}

void
NANO_LOG_Writer_write_bytes(
        NANO_LOG_Writer *const self,
        const char *const key,
        const NANO_u8 *const val,
        const NANO_usize len);

#define NANO_LOG_Writer_write_bytes(s_,k_,v_,l_) \
{\
    if ((s_)->write_bytes != NULL)\
    {\
        (s_)->write_bytes((s_),(k_),(v_),(l_));\
    }\
}

#if NANO_FEAT_REQUEST
void
NANO_LOG_Writer_write_objectid(
        NANO_LOG_Writer *const self,
        const char *const key,
        const NANO_XRCE_ObjectId val);

#define NANO_LOG_Writer_write_objectid(s_,k_,v_) \
{\
    if ((s_)->write_objectid != NULL)\
    {\
        (s_)->write_objectid((s_),(k_),(v_));\
    }\
}

void
NANO_LOG_Writer_write_requestid(
        NANO_LOG_Writer *const self,
        const char *const key,
        const NANO_XRCE_RequestId val);

#define NANO_LOG_Writer_write_requestid(s_,k_,v_) \
{\
    if ((s_)->write_requestid != NULL)\
    {\
        (s_)->write_requestid((s_),(k_),(v_));\
    }\
}

#endif /* NANO_FEAT_REQUEST */



/******************************************************************************
 *                        Conditional definitions
 ******************************************************************************/
#if NANO_FEAT_LOG

/******************************************************************************
 *                          Log Implementation
 ******************************************************************************/
#ifndef NANO_LOG_DEF_H
#if NANO_FEAT_LOG_IMPL_PRINTF
#define NANO_LOG_DEF_H      "nano/nano_core_log_impl_printf.h"
#endif /* NANO_FEAT_LOG_IMPL_PRINTF */
#endif /* NANO_LOG_DEF_H */

#ifdef NANO_LOG_DEF_H

#include NANO_LOG_DEF_H

#ifndef NANO_LOG_WriterImpl
#error "no implementation of log writer specified"
#endif /* NANO_LOG_WriterImpl */

#ifndef NANO_LOG_WRITERIMPL_INITIALIZER
#error "no initializer for log writer implementation specified"
#endif /* NANO_LOG_WRITERIMPL_INITIALIZER */

#else

#warning "log enabled without a writer implementation"

#define NANO_LOG_WriterImpl                 NANO_LOG_Writer
#define NANO_LOG_WRITERIMPL_INITIALIZER     NANO_LOG_WRITER_INITIALIZER

#endif /* NANO_LOG_DEF_H */

extern NANO_LOG_WriterImpl NANO_LOG_WRITERIMPL;

#ifndef NANO_LOG_WRITER
#define NANO_LOG_WRITER     &NANO_LOG_WRITERIMPL.base
#endif /* NANO_LOG_WRITER */

#define NANO_LOG_LEVEL \
    NANO_LOG_Writer_level(NANO_LOG_WRITER)

#define NANO_LOG_LEVEL_ENABLED(lvl_,action_) \
{\
    if (xrce_str_concat(NANO_LOG_,lvl_) <= NANO_LOG_LEVEL)\
    {\
        action_\
    }\
}

#if NANO_FEAT_LOG_DEBUG_ON_ERROR
#define NANO_LOG_ON_ERROR \
    NANO_LOG_SET_LEVEL(LEVEL_TRACE_FN)
#else
#define NANO_LOG_ON_ERROR
#endif /* NANO_FEAT_LOG_DEBUG_ON_ERROR */

#if NANO_LIMIT_LOG_LEVEL_MAX >= NANO_LOG_LEVEL_ERROR
#define NANO_LOG_ERROR(msg_, entries_) \
NANO_LOG_LEVEL_ENABLED(LEVEL_ERROR,\
    NANO_LOG_ENTRY_BEGIN(LEVEL_ERROR, msg_)\
    entries_\
    NANO_LOG_ENTRY_END\
    NANO_LOG_ON_ERROR)
#else
#define NANO_LOG_ERROR(msg_, entries_)
#endif /* NANO_LIMIT_LOG_LEVEL_MAX >= NANO_LOG_LEVEL_ERROR */

#if NANO_LIMIT_LOG_LEVEL_MAX >= NANO_LOG_LEVEL_WARNING
#define NANO_LOG_WARNING(msg_, entries_) \
NANO_LOG_LEVEL_ENABLED(LEVEL_WARNING,\
    NANO_LOG_ENTRY_BEGIN(LEVEL_WARNING, msg_)\
    entries_\
    NANO_LOG_ENTRY_END)
#else
#define NANO_LOG_WARNING(msg_, entries_)
#endif /* NANO_LIMIT_LOG_LEVEL_MAX >= NANO_LOG_LEVEL_WARNING */

#if NANO_LIMIT_LOG_LEVEL_MAX >= NANO_LOG_LEVEL_INFO
#define NANO_LOG_INFO(msg_, entries_) \
NANO_LOG_LEVEL_ENABLED(LEVEL_INFO,\
    NANO_LOG_ENTRY_BEGIN(LEVEL_INFO, msg_)\
    entries_\
    NANO_LOG_ENTRY_END)
#else
#define NANO_LOG_INFO(msg_, entries_)
#endif /* NANO_LIMIT_LOG_LEVEL_MAX >= NANO_LOG_LEVEL_INFO */

#if NANO_LIMIT_LOG_LEVEL_MAX >= NANO_LOG_LEVEL_DEBUG
#define NANO_LOG_DEBUG(msg_, entries_) \
NANO_LOG_LEVEL_ENABLED(LEVEL_DEBUG,\
    NANO_LOG_ENTRY_BEGIN(LEVEL_DEBUG, msg_)\
    entries_\
    NANO_LOG_ENTRY_END)
#else
#define NANO_LOG_DEBUG(msg_, entries_)
#endif /* NANO_LIMIT_LOG_LEVEL_MAX >= NANO_LOG_LEVEL_DEBUG */

#if NANO_LIMIT_LOG_LEVEL_MAX >= NANO_LOG_LEVEL_TRACE
#define NANO_LOG_TRACE(msg_, entries_) \
NANO_LOG_LEVEL_ENABLED(LEVEL_TRACE,\
    NANO_LOG_ENTRY_BEGIN(LEVEL_TRACE, msg_)\
    entries_\
    NANO_LOG_ENTRY_END)
#else
#define NANO_LOG_TRACE(msg_, entries_)
#endif /* NANO_LIMIT_LOG_LEVEL_MAX >= NANO_LOG_LEVEL_TRACE */


#if NANO_LIMIT_LOG_LEVEL_MAX >= NANO_LOG_LEVEL_TRACE_FN
#define NANO_LOG_TRACE_FN(msg_, entries_) \
NANO_LOG_LEVEL_ENABLED(LEVEL_TRACE_FN,\
    NANO_LOG_ENTRY_BEGIN(LEVEL_TRACE_FN, msg_)\
    entries_\
    NANO_LOG_ENTRY_END)
#else
#define NANO_LOG_TRACE_FN(msg_, entries_)
#endif /* NANO_LIMIT_LOG_LEVEL_MAX >= NANO_LOG_LEVEL_TRACE_FN */

#define NANO_LOG_ERROR_MSG(msg_) \
        NANO_LOG_ERROR(msg_,)

#define NANO_LOG_WARNING_MSG(msg_) \
        NANO_LOG_WARNING(msg_,)

#define NANO_LOG_INFO_MSG(msg_) \
        NANO_LOG_INFO(msg_,)

#define NANO_LOG_DEBUG_MSG(msg_) \
        NANO_LOG_DEBUG(msg_,)

#define NANO_LOG_TRACE_MSG(msg_) \
        NANO_LOG_TRACE(msg_,)

#define NANO_LOG_TRACE_FN_MSG(msg_) \
        NANO_LOG_TRACE_FN(msg_,)

#define NANO_LOG_SET_LEVEL(lvl_) \
    NANO_LOG_Writer_set_level(NANO_LOG_WRITER, xrce_str_concat(NANO_LOG_,lvl_));

#define NANO_LOG_FN_ENTRY \
    NANO_LOG_TRACE_FN("", NANO_LOG_FN_CALL)

#define NANO_LOG_FN_EXIT \
    NANO_LOG_TRACE_FN("", NANO_LOG_FN_RETURN)

#define NANO_LOG_FN_EXIT_RC(rc_) \
    NANO_LOG_TRACE_FN("",\
        NANO_LOG_FN_RETURN\
        NANO_LOG_RC(rc_))

#define NANO_LOG_FN_EXIT_PTR(ptr_) \
    NANO_LOG_TRACE_FN("",\
        NANO_LOG_FN_RETURN\
        NANO_LOG_PTR("res",(ptr_))) \

#define NANO_LOG_FN_EXIT_BOOL(bool_) \
    NANO_LOG_TRACE_FN("",\
        NANO_LOG_FN_RETURN\
        NANO_LOG_BOOL("res",(bool_))) \

#define NANO_LOG_FEAT_MISSING(feat_) \
    NANO_LOG_ERROR("required feature disabled",\
            NANO_LOG_STR("flag",#feat_))

#define NANO_LOG_NOT_IMPLEMENTED(feature_) \
    NANO_LOG_ERROR("NOT IMPLEMENTED",\
        NANO_LOG_STR("feature", feature_))

#define NANO_LOG_ENTRY_BEGIN(lvl_,msg_) \
    NANO_LOG_Writer_begin_entry(\
        NANO_LOG_WRITER,\
        xrce_str_concat(NANO_LOG_,lvl_),\
        0 /* ts */,\
        __FILE__, __LINE__, NANO_FUNCTION_NAME,\
        (msg_));

#define NANO_LOG_ENTRY_END \
    NANO_LOG_Writer_end_entry(NANO_LOG_WRITER);

#define NANO_LOG_FN_CALL \
    NANO_LOG_Writer_fn_call(NANO_LOG_WRITER, NANO_FUNCTION_NAME);

#define NANO_LOG_FN_RETURN \
    NANO_LOG_Writer_fn_return(NANO_LOG_WRITER, NANO_FUNCTION_NAME);

#define NANO_LOG_RC(rc_) \
    NANO_LOG_Writer_write_rc(NANO_LOG_WRITER,"rc", (rc_));

#define NANO_LOG_STR(key_,val_) \
    NANO_LOG_Writer_write_string(NANO_LOG_WRITER, (key_), (val_));

#define NANO_LOG_TS_MS(key_,ts_) \
    NANO_LOG_Writer_write_timestamp_ms(NANO_LOG_WRITER, (key_), (ts_));

#define NANO_LOG_BOOL(key_,val_) \
    NANO_LOG_Writer_write_bool(NANO_LOG_WRITER, (key_), (val_));

#define NANO_LOG_U8(key_,val_) \
    NANO_LOG_Writer_write_u8(NANO_LOG_WRITER, (key_), (val_));

#define NANO_LOG_U16(key_,val_) \
    NANO_LOG_Writer_write_u16(NANO_LOG_WRITER, (key_), (val_));

#define NANO_LOG_U32(key_,val_) \
    NANO_LOG_Writer_write_u32(NANO_LOG_WRITER, (key_), (val_));

#define NANO_LOG_U64(key_,val_) \
    NANO_LOG_Writer_write_u64(NANO_LOG_WRITER, (key_), (val_));

#define NANO_LOG_I8(key_,val_) \
    NANO_LOG_Writer_write_i8(NANO_LOG_WRITER, (key_), (val_));

#define NANO_LOG_I16(key_,val_) \
    NANO_LOG_Writer_write_i16(NANO_LOG_WRITER, (key_), (val_));

#define NANO_LOG_I32(key_,val_) \
    NANO_LOG_Writer_write_i32(NANO_LOG_WRITER, (key_), (val_));

#define NANO_LOG_I64(key_,val_) \
    NANO_LOG_Writer_write_i64(NANO_LOG_WRITER, (key_), (val_));

#define NANO_LOG_H8(key_,val_) \
    NANO_LOG_Writer_write_h8(NANO_LOG_WRITER, (key_), (val_));

#define NANO_LOG_H16(key_,val_) \
    NANO_LOG_Writer_write_h16(NANO_LOG_WRITER, (key_), (val_));

#define NANO_LOG_H32(key_,val_) \
    NANO_LOG_Writer_write_h32(NANO_LOG_WRITER, (key_), (val_));

#define NANO_LOG_H64(key_,val_) \
    NANO_LOG_Writer_write_h64(NANO_LOG_WRITER, (key_), (val_));

#define NANO_LOG_USIZE(key_,val_) \
    NANO_LOG_Writer_write_usize(NANO_LOG_WRITER, (key_), (val_));

#define NANO_LOG_ENUM(key_,val_) \
    NANO_LOG_I32((key_),(val_))

#define NANO_LOG_SN(key_,val_) \
    NANO_LOG_Writer_write_sn(NANO_LOG_WRITER, (key_), (val_));

#define NANO_LOG_KEY(key_,val_) \
    NANO_LOG_Writer_write_clientkey(NANO_LOG_WRITER, (key_), (val_));

#define NANO_LOG_STREAMID(key_,val_) \
    NANO_LOG_Writer_write_streamid(NANO_LOG_WRITER, (key_), (val_));

#define NANO_LOG_SESSIONID(key_,val_) \
    NANO_LOG_Writer_write_sessionid(NANO_LOG_WRITER, (key_), (val_));

#define NANO_LOG_MSGHDR(key_,val_) \
    NANO_LOG_Writer_write_message_header(NANO_LOG_WRITER, (key_), (val_));

#define NANO_LOG_SUBMSGHDR(key_,val_) \
    NANO_LOG_Writer_write_submessage_header(NANO_LOG_WRITER, (key_), (val_));

#define NANO_LOG_PTR(key_,val_) \
    NANO_LOG_Writer_write_ptr(NANO_LOG_WRITER, (key_), (val_));

#define NANO_LOG_LOCATOR(key_,val_) \
    NANO_LOG_Writer_write_locator(NANO_LOG_WRITER, (key_), (val_));

#define NANO_LOG_LOCATOR_SMALL(key_,val_) \
    NANO_LOG_Writer_write_locator_small(NANO_LOG_WRITER, (key_), (val_));

#define NANO_LOG_LOCATOR_MEDIUM(key_,val_) \
    NANO_LOG_Writer_write_locator_medium(NANO_LOG_WRITER, (key_), (val_));

#define NANO_LOG_LOCATOR_LARGE(key_,val_) \
    NANO_LOG_Writer_write_locator_large(NANO_LOG_WRITER, (key_), (val_));

#define NANO_LOG_LOCATOR_STRING(key_,val_) \
    NANO_LOG_Writer_write_locator_string(NANO_LOG_WRITER, (key_), (val_));

#define NANO_LOG_MBUF(key_,val_) \
    NANO_LOG_Writer_write_message_buffer(NANO_LOG_WRITER, (key_), (val_));

#define NANO_LOG_BYTES(key_,val_, len_) \
    NANO_LOG_Writer_write_bytes(NANO_LOG_WRITER, (key_), (val_), (len_));

#if NANO_FEAT_REQUEST
#define NANO_LOG_OBJID(key_,val_) \
    NANO_LOG_Writer_write_objectid(NANO_LOG_WRITER, (key_), (val_));
#define NANO_LOG_REQID(key_,val_) \
    NANO_LOG_Writer_write_requestid(NANO_LOG_WRITER, (key_), (val_));
#endif /* NANO_FEAT_REQUEST */

#else

#define NANO_LOG_LEVEL  (0)
#define NANO_LOG_LEVEL_ENABLED(lvl_,action_)
#define NANO_LOG_SET_LEVEL(lvl_)
#define NANO_LOG_ERROR(msg_, entries_)
#define NANO_LOG_WARNING(msg_, entries_)
#define NANO_LOG_INFO(msg_, entries_)
#define NANO_LOG_DEBUG(msg_, entries_)
#define NANO_LOG_TRACE(msg_, entries_)
#define NANO_LOG_TRACE_FN(msg_, entries_)
#define NANO_LOG_ERROR_MSG(msg_)
#define NANO_LOG_WARNING_MSG(msg_)
#define NANO_LOG_INFO_MSG(msg_)
#define NANO_LOG_DEBUG_MSG(msg_)
#define NANO_LOG_TRACE_MSG(msg_)
#define NANO_LOG_TRACE_FN_MSG(msg_)
#define NANO_LOG_FN_ENTRY
#define NANO_LOG_FN_EXIT
#define NANO_LOG_FN_EXIT_RC(rc_)
#define NANO_LOG_FN_EXIT_PTR(ptr_)
#define NANO_LOG_FN_EXIT_BOOL(bool_)
#define NANO_LOG_FEAT_MISSING(feat_)
#define NANO_LOG_NOT_IMPLEMENTED(feature_)
#define NANO_LOG_ENTRY_BEGIN(lvl_,msg_)
#define NANO_LOG_ENTRY_END
#define NANO_LOG_RC(rc_)
#define NANO_LOG_STR(key_,val_)
#define NANO_LOG_TS_MS(key_,ts_)
#define NANO_LOG_BOOL(key_,val_)
#define NANO_LOG_U8(key_,val_)
#define NANO_LOG_U16(key_,val_)
#define NANO_LOG_U32(key_,val_)
#define NANO_LOG_U32(key_,val_)
#define NANO_LOG_I8(key_,val_)
#define NANO_LOG_I16(key_,val_)
#define NANO_LOG_I32(key_,val_)
#define NANO_LOG_I64(key_,val_)
#define NANO_LOG_H8(key_,val_)
#define NANO_LOG_H16(key_,val_)
#define NANO_LOG_H32(key_,val_)
#define NANO_LOG_H64(key_,val_)
#define NANO_LOG_USIZE(key_,val_)
#define NANO_LOG_ENUM(key_,val_)
#define NANO_LOG_SN(key_,val_)
#define NANO_LOG_KEY(key_,val_)
#define NANO_LOG_STREAMID(key_,val_)
#define NANO_LOG_SESSIONID(key_,val_)
#define NANO_LOG_MSGHDR(key_,val_)
#define NANO_LOG_SUBMSGHDR(key_,val_)
#define NANO_LOG_PTR(key_,val_)
#define NANO_LOG_LOCATOR(key_,val_)
#define NANO_LOG_LOCATOR_SMALL(key_,val_)
#define NANO_LOG_LOCATOR_MEDIUM(key_,val_)
#define NANO_LOG_LOCATOR_LARGE(key_,val_)
#define NANO_LOG_LOCATOR_STRING(key_,val_)
#define NANO_LOG_MBUF(key_,val_)
#define NANO_LOG_BYTES(key_,val_, len_)

#if NANO_FEAT_REQUEST
#define NANO_LOG_OBJID(key_,val_)
#define NANO_LOG_REQID(key_,val_)
#endif /* NANO_FEAT_REQUEST */

#endif /* NANO_FEAT_LOG */

#endif /* nano_core_log_h */
