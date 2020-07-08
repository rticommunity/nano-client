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

#if NANO_FEAT_LOG && NANO_FEAT_LOG_IMPL_PRINTF

#if NANO_FEAT_LOG_COLOR
#define ENABLE_TERMCOLOR
#include "nano/nano_core_log_termcolor.h"
#endif /* NANO_FEAT_LOG_COLOR */

NANO_PRIVATE
void
NANO_LOG_PrintfWriter_set_color(const NANO_LOG_Level lvl)
{
#if NANO_FEAT_LOG_COLOR
    switch (lvl)
    {
    case NANO_LOG_LEVEL_ERROR:
    {
        background_red(stdout);
        text_white(stdout);
        break;
    }
    case NANO_LOG_LEVEL_WARNING:
    {
        background_yellow(stdout);
        text_red(stdout);
        break;
    }
    case NANO_LOG_LEVEL_INFO:
    {
        background_blue(stdout);
        text_gray(stdout);
        break;
    }
    case NANO_LOG_LEVEL_DEBUG:
    {
        background_magenta(stdout);
        text_gray(stdout);
        break;
    }
    case NANO_LOG_LEVEL_TRACE:
    {
        background_cyan(stdout);
        text_gray(stdout);
        break;
    }
    case NANO_LOG_LEVEL_TRACE_FN:
    {
        background_white(stdout);
        text_gray(stdout);
        break;
    }
    default:
        break;
    }
#else
    UNUSED_ARG(lvl);
#endif /* NANO_FEAT_LOG_COLOR */
}

NANO_PRIVATE
void
NANO_LOG_PrintfWriter_reset_color()
{
#if NANO_FEAT_LOG_COLOR
    reset_colors(stdout);
#endif /* NANO_FEAT_LOG_COLOR */
}

NANO_LOG_WriterImpl NANO_LOG_WRITERIMPL = NANO_LOG_WRITERIMPL_INITIALIZER;

NANO_PRIVATE
void
NANO_LOG_PrintfWriter_write_level(const NANO_LOG_Level lvl)
{
    NANO_OSAPI_Debug_printf("[");
    NANO_LOG_PrintfWriter_set_color(lvl);
    NANO_OSAPI_Debug_printf("%s", NANO_LOG_Level_to_str(lvl));
    NANO_LOG_PrintfWriter_reset_color();
    NANO_OSAPI_Debug_printf("]");
}

#if NANO_PLATFORM == NANO_PLATFORM_WINDOWS
#define PATH_SEP         '\\'
#else
#define PATH_SEP         '/'
#endif

#if NANO_FEAT_LOG_FILENAME
NANO_PRIVATE
void 
NANO_LOG_PrintfWriter_write_file_name(
    const NANO_LOG_Level lvl,
    const char *const file,
    const NANO_usize file_line)
{
    NANO_usize i = 0;
#if !NANO_FEAT_LOG_FILENAME_FULL
    NANO_usize str_len = 0;

    str_len = NANO_OSAPI_String_length(file);

    for (i = str_len - 1;
            i > 1 && 
            (str_len - (i+1)) < NANO_LIMIT_LOG_FILENAME_MAX_LENGTH &&
            file[i-1] != PATH_SEP;
                i--) { }
#endif /* NANO_FEAT_LOG_FILENAME_FULL */
    NANO_OSAPI_Debug_printf("[");
    NANO_LOG_PrintfWriter_set_color(lvl);
    NANO_OSAPI_Debug_printf("%s:"NANO_LOG_USIZE_FMT, file + i, file_line);
    NANO_LOG_PrintfWriter_reset_color();
    NANO_OSAPI_Debug_printf("]");
}
#endif /* NANO_FEAT_LOG_FILENAME */

#if NANO_FEAT_LOG_FUNCTION
NANO_PRIVATE
void 
NANO_LOG_PrintfWriter_write_function(
    const NANO_LOG_Level lvl,
    const char *const function)
{
    NANO_OSAPI_Debug_printf("[");
    NANO_LOG_PrintfWriter_set_color(lvl);
    NANO_OSAPI_Debug_printf("%s", function);
    NANO_LOG_PrintfWriter_reset_color();
    NANO_OSAPI_Debug_printf("]");
}
#endif /* NANO_FEAT_LOG_FUNCTION */

#if NANO_FEAT_LOG_TS
NANO_PRIVATE
void 
NANO_LOG_PrintfWriter_write_header_ts_ms(
    const NANO_LOG_Level lvl,
    const NANO_u32 ts)
{
    NANO_OSAPI_Debug_printf("[");
    NANO_LOG_PrintfWriter_set_color(lvl);
    NANO_OSAPI_Debug_printf("%012u", ts);
    NANO_LOG_PrintfWriter_reset_color();
    NANO_OSAPI_Debug_printf("]");
}
#endif /* NANO_FEAT_LOG_TS */

void
NANO_LOG_PrintfWriter_begin_entry(
    NANO_LOG_Writer *const self,
    const NANO_LOG_Level lvl,
    const NANO_u32 ts,
    const char *const file,
    const NANO_usize file_line,
    const char *const function,
    const char *const header)
{
    UNUSED_ARG(ts);
    UNUSED_ARG(self);

#if NANO_FEAT_AGENT
    self->mutex_status = RTIOsapi_global_lock();
    NANO_PCOND(self->mutex_status == 0)
    if (self->mutex_status != 0)
    {
        return;
    }
#endif

    NANO_LOG_PrintfWriter_write_level(lvl);
#if NANO_FEAT_LOG_TS
    NANO_LOG_PrintfWriter_write_header_ts_ms(lvl, ts);
#endif /* NANO_FEAT_LOG_TS */
#if NANO_FEAT_LOG_FILENAME
    NANO_LOG_PrintfWriter_write_file_name(lvl, file, file_line);
#endif /* NANO_FEAT_LOG_FILENAME */
#if NANO_FEAT_LOG_FUNCTION
    NANO_LOG_PrintfWriter_write_function(lvl, function);
#endif /* NANO_FEAT_LOG_FUNCTION */
    NANO_OSAPI_Debug_printf(" %s", header);
}


void
NANO_LOG_PrintfWriter_fn_call(
    NANO_LOG_Writer *const self,
    const char *const fn)
{
    UNUSED_ARG(self);
    UNUSED_ARG(fn);

#if NANO_FEAT_LOG_COLOR
    background_green(stdout);
    text_gray(stdout);
#endif /* NANO_FEAT_LOG_COLOR */

    NANO_OSAPI_Debug_printf(">>>");

#if NANO_FEAT_LOG_COLOR
    reset_colors(stdout);
#endif /* NANO_FEAT_LOG_COLOR */
}

void
NANO_LOG_PrintfWriter_fn_return(
    NANO_LOG_Writer *const self,
    const char *const fn)
{
    UNUSED_ARG(self);
    UNUSED_ARG(fn);

#if NANO_FEAT_LOG_COLOR
    background_red(stdout);
    text_gray(stdout);
#endif /* NANO_FEAT_LOG_COLOR */

    NANO_OSAPI_Debug_printf("<<<");

#if NANO_FEAT_LOG_COLOR
    reset_colors(stdout);
#endif /* NANO_FEAT_LOG_COLOR */
}

void
NANO_LOG_PrintfWriter_end_entry(NANO_LOG_Writer *const self)
{
    UNUSED_ARG(self);
#if NANO_FEAT_AGENT
    // NANO_PCOND(self->mutex_status == 0)
    if (self->mutex_status != 0)
    {
        if (0 != RTIOsapi_global_lock())
        {
            return;
        }
        self->mutex_status = 0;
    }
#endif

#ifdef NANO_HAVE_PRINTF_CR
    NANO_OSAPI_Debug_printf("\n\r");
#else
    NANO_OSAPI_Debug_printf("\n");
#endif /* NANO_HAVE_PRINTF_CR */

#if NANO_FEAT_AGENT
    self->mutex_status = 1;
    if (0 != RTIOsapi_global_unlock())
    {
        return;
    }
#endif
}

NANO_PRIVATE
void
NANO_LOG_PrintfWriter_write_key(
    NANO_LOG_Writer *const self, const char *const key)
{
    UNUSED_ARG(self);

    NANO_OSAPI_Debug_printf(" ");
    if (key != NULL && key[0] != '\0')
    {
        NANO_OSAPI_Debug_printf("%s=", key);
    }
}

NANO_PRIVATE
void
NANO_LOG_PrintfWriter_write_null(
    NANO_LOG_Writer *const self)
{
    UNUSED_ARG(self);
    
    NANO_OSAPI_Debug_printf("<null>");
}


void
NANO_LOG_PrintfWriter_write_rc(
    NANO_LOG_Writer *const self, const char *const key, const NANO_RetCode val)
{
    const char *rc_str = NULL;

    rc_str = NANO_RetCode_as_str(val);

    NANO_LOG_PrintfWriter_write_key(self,key);
    
    NANO_OSAPI_Debug_printf("%s [%d]", rc_str, val);
}

void
NANO_LOG_PrintfWriter_write_str(
    NANO_LOG_Writer *const self,
    const char *const key,
    const char *const val)
{
    NANO_LOG_PrintfWriter_write_key(self,key);
    NANO_OSAPI_Debug_printf("\"%s\"", val);
}

void
NANO_LOG_PrintfWriter_write_ts_ms(
    NANO_LOG_Writer *const self, const char *const key, const NANO_u32 val)
{
    NANO_LOG_PrintfWriter_write_key(self,key);
    NANO_OSAPI_Debug_printf("%08u", val);
}

void
NANO_LOG_PrintfWriter_write_bool(
    NANO_LOG_Writer *const self, const char *const key, const NANO_bool val)
{
    NANO_LOG_PrintfWriter_write_key(self,key);
    NANO_OSAPI_Debug_printf("%s", (val)?"true":"false");
}

void
NANO_LOG_PrintfWriter_write_u8(
    NANO_LOG_Writer *const self, const char *const key, const NANO_u8 val)
{
    NANO_LOG_PrintfWriter_write_key(self,key);
    NANO_OSAPI_Debug_printf("%u", val);
}

void
NANO_LOG_PrintfWriter_write_u16(
    NANO_LOG_Writer *const self, const char *const key, const NANO_u16 val)
{
    NANO_LOG_PrintfWriter_write_key(self,key);
    NANO_OSAPI_Debug_printf("%u", val);
}

void
NANO_LOG_PrintfWriter_write_u32(
    NANO_LOG_Writer *const self, const char *const key, const NANO_u32 val)
{
    NANO_LOG_PrintfWriter_write_key(self,key);
    NANO_OSAPI_Debug_printf("%u", val);
}

void
NANO_LOG_PrintfWriter_write_u64(
    NANO_LOG_Writer *const self, const char *const key, const NANO_u64 val)
{
    NANO_LOG_PrintfWriter_write_key(self,key);
    NANO_OSAPI_Debug_printf("%lu", val);
}

void
NANO_LOG_PrintfWriter_write_i8(
    NANO_LOG_Writer *const self, const char *const key, const NANO_i8 val)
{
    NANO_LOG_PrintfWriter_write_key(self,key);
    NANO_OSAPI_Debug_printf("%d", val);
}

void
NANO_LOG_PrintfWriter_write_i16(
    NANO_LOG_Writer *const self, const char *const key, const NANO_i16 val)
{
    NANO_LOG_PrintfWriter_write_key(self,key);
    NANO_OSAPI_Debug_printf("%d", val);
}

void
NANO_LOG_PrintfWriter_write_i32(
    NANO_LOG_Writer *const self, const char *const key, const NANO_i32 val)
{
    NANO_LOG_PrintfWriter_write_key(self,key);
    NANO_OSAPI_Debug_printf("%d", val);
}

void
NANO_LOG_PrintfWriter_write_i64(
    NANO_LOG_Writer *const self, const char *const key, const NANO_i64 val)
{
    NANO_LOG_PrintfWriter_write_key(self,key);
    NANO_OSAPI_Debug_printf("%ld", val);
}

void
NANO_LOG_PrintfWriter_write_h8(
    NANO_LOG_Writer *const self, const char *const key, const NANO_u8 val)
{
    NANO_LOG_PrintfWriter_write_key(self,key);
    NANO_OSAPI_Debug_printf("0x%02X", val);
}

void
NANO_LOG_PrintfWriter_write_h16(
    NANO_LOG_Writer *const self, const char *const key, const NANO_u16 val)
{
    NANO_LOG_PrintfWriter_write_key(self,key);
    NANO_OSAPI_Debug_printf("0x%04X", val);
}

void
NANO_LOG_PrintfWriter_write_h32(
    NANO_LOG_Writer *const self, const char *const key, const NANO_u32 val)
{
    NANO_LOG_PrintfWriter_write_key(self,key);
    NANO_OSAPI_Debug_printf("0x%08X", val);
}

void
NANO_LOG_PrintfWriter_write_h64(
    NANO_LOG_Writer *const self, const char *const key, const NANO_u64 val)
{
    NANO_u64 val_be = NANO_u64_to_be(val);
    NANO_u32 *val_ptr = (NANO_u32*)&val_be;
    NANO_LOG_PrintfWriter_write_key(self,key);
    NANO_OSAPI_Debug_printf("0x%08X%08X", val_ptr[0], val_ptr[1]);
}

void
NANO_LOG_PrintfWriter_write_usize(
    NANO_LOG_Writer *const self, 
    const char *const key,
    const NANO_usize val)
{
    NANO_LOG_PrintfWriter_write_key(self,key);
    NANO_OSAPI_Debug_printf(NANO_LOG_USIZE_FMT, val);
}

void
NANO_LOG_PrintfWriter_write_sn(
    NANO_LOG_Writer *const self, 
    const char *const key,
    const NANO_XRCE_SeqNum val)
{
    NANO_u16 uval = 0;
    uval = NANO_XRCE_SeqNum_to_u16(&val);
    NANO_LOG_PrintfWriter_write_h16(self, key, uval);
}

void
NANO_LOG_PrintfWriter_write_clientkey(
    NANO_LOG_Writer *const self,
    const char *const key,
    const NANO_XRCE_ClientKey val)
{
    NANO_u32 uval = 0;
    uval = NANO_XRCE_ClientKey_to_u32(&val);
    NANO_LOG_PrintfWriter_write_h32(self, key, uval);
}

void
NANO_LOG_PrintfWriter_write_streamid(
    NANO_LOG_Writer *const self,
    const char *const key,
    const NANO_XRCE_StreamId val)
{
    NANO_LOG_PrintfWriter_write_h8(self, key, val);
}

void
NANO_LOG_PrintfWriter_write_sessionid(
    NANO_LOG_Writer *const self,
    const char *const key,
    const NANO_XRCE_SessionId val)
{
    NANO_LOG_PrintfWriter_write_h8(self, key, val);
}

void
NANO_LOG_PrintfWriter_write_msghdr(
    NANO_LOG_Writer *const self,
    const char *const key,
    const NANO_XRCE_MessageHeader val)
{
    NANO_LOG_PrintfWriter_write_key(self,key);
    NANO_OSAPI_Debug_printf("[");
    NANO_LOG_PrintfWriter_write_sessionid(
        self, "session", val.session_id);
    NANO_OSAPI_Debug_printf(",");
    NANO_LOG_PrintfWriter_write_streamid(
        self, "stream", val.stream_id);
    NANO_OSAPI_Debug_printf(",");
    NANO_LOG_PrintfWriter_write_sn(
        self, "sn", val.sn);
    if (NANO_XRCE_SessionId_has_client_key(val.session_id))
    {
        NANO_OSAPI_Debug_printf(",");
        NANO_LOG_PrintfWriter_write_clientkey(
            self, "key", val.client_key);
    }
    NANO_OSAPI_Debug_printf(" ]");
}

const char *
NANO_LOG_PrintfWriter_submsgid_to_str(const NANO_XRCE_SubmessageId self)
{
    switch (self)
    {
    case NANO_XRCE_SUBMESSAGEID_CREATE_CLIENT:
        return "CREATE_CLIENT";
    case NANO_XRCE_SUBMESSAGEID_CREATE:
        return "CREATE";
    case NANO_XRCE_SUBMESSAGEID_DELETE:
        return "DELETE";
    case NANO_XRCE_SUBMESSAGEID_STATUS:
        return "STATUS";
    case NANO_XRCE_SUBMESSAGEID_STATUS_AGENT:
        return "STATUS_AGENT";
    case NANO_XRCE_SUBMESSAGEID_READ_DATA:
        return "READ_DATA";
    case NANO_XRCE_SUBMESSAGEID_WRITE_DATA:
        return "WRITE_DATA";
    case NANO_XRCE_SUBMESSAGEID_DATA:
        return "DATA";
    case NANO_XRCE_SUBMESSAGEID_ACKNACK:
        return "ACKNACK";
    case NANO_XRCE_SUBMESSAGEID_HEARTBEAT:
        return "HEARTBEAT";
    case NANO_XRCE_SUBMESSAGEID_TIMESTAMP:
        return "TIMESTAMP";
    case NANO_XRCE_SUBMESSAGEID_TIMESTAMP_REPLY:
        return "TIMESTAMP_REPLY";
    case NANO_XRCE_SUBMESSAGEID_INFO:
        return "INFO";
    case NANO_XRCE_SUBMESSAGEID_GET_INFO:
        return "GET_INFO";
    case NANO_XRCE_SUBMESSAGEID_FRAGMENT:
        return "FRAGMENT";
    case NANO_XRCE_SUBMESSAGEID_RESET:
        return "RESET";
    default:
        return "UNKNOWN";
    }
}

void
NANO_LOG_PrintfWriter_write_submsghdr(
    NANO_LOG_Writer *const self,
    const char *const key,
    const NANO_XRCE_SubmessageHeader val)
{
    const char *id = NANO_LOG_PrintfWriter_submsgid_to_str(val.id);

    NANO_LOG_PrintfWriter_write_key(self,key);
    NANO_OSAPI_Debug_printf("[");
    NANO_LOG_PrintfWriter_write_str(self, "id", id);
    NANO_OSAPI_Debug_printf(",");
    NANO_LOG_PrintfWriter_write_h8(self, "flags", val.flags);
    NANO_OSAPI_Debug_printf(",");
    NANO_LOG_PrintfWriter_write_u16(self, "len", val.length);
    NANO_OSAPI_Debug_printf(" ]");
}

void
NANO_LOG_PrintfWriter_write_ptr(
    NANO_LOG_Writer *const self,
    const char *const key,
    const void *const val)
{
    NANO_LOG_PrintfWriter_write_key(self,key);
    if (val == NULL)
    {
        NANO_LOG_PrintfWriter_write_null(self);
        return;
    }
    NANO_OSAPI_Debug_printf("%p", val);
}

void
NANO_LOG_PrintfWriter_write_locator_small(
    NANO_LOG_Writer *const self,
    const char *const key,
    const NANO_XRCE_TransportLocatorSmall *const val)
{
    NANO_u16 loc = 0;
    NANO_LOG_PrintfWriter_write_key(self,key);
    if (val == NULL)
    {
        NANO_LOG_PrintfWriter_write_null(self);
        return;
    }
    NANO_u16_deserialize(&loc, val->address, NANO_BOOL_FALSE);
    NANO_OSAPI_Debug_printf("0x%04X:%u",loc, val->port);
}


void
NANO_LOG_PrintfWriter_write_locator_medium(
    NANO_LOG_Writer *const self,
    const char *const key,
    const NANO_XRCE_TransportLocatorMedium *const val)
{
    NANO_u32 loc = 0;
    NANO_LOG_PrintfWriter_write_key(self,key);
    if (val == NULL)
    {
        NANO_LOG_PrintfWriter_write_null(self);
        return;
    }
    NANO_u32_deserialize(&loc, val->address, NANO_BOOL_FALSE);
    NANO_OSAPI_Debug_printf("0x%08X:%u",loc, val->port);
}


void
NANO_LOG_PrintfWriter_write_locator_large(
    NANO_LOG_Writer *const self,
    const char *const key,
    const NANO_XRCE_TransportLocatorLarge *const val)
{
    NANO_u32 loc = 0;
    const NANO_u8 *ptr = val->address;
    NANO_usize i = 0,
                max_ints = 
                    NANO_XRCE_TRANSPORTLOCATORLARGE_ADDRESS_LEN/
                        sizeof(NANO_u32);

    NANO_LOG_PrintfWriter_write_key(self,key);
    if (val == NULL)
    {
        NANO_LOG_PrintfWriter_write_null(self);
        return;
    }

    for (i = 0; i < max_ints; i++)
    {
        if (i > 0)
        {
            NANO_OSAPI_Debug_printf(".");
        }
        NANO_u32_deserialize(
            &loc, (ptr + (i * sizeof(NANO_u32))), NANO_BOOL_FALSE);
        NANO_OSAPI_Debug_printf("0x%08X",loc);
    }

    NANO_OSAPI_Debug_printf(":%u", val->port);

}


void
NANO_LOG_PrintfWriter_write_locator_string(
    NANO_LOG_Writer *const self,
    const char *const key,
    const NANO_XRCE_TransportLocatorString *const val)
{
    NANO_LOG_PrintfWriter_write_key(self,key);
    if (val == NULL)
    {
        NANO_LOG_PrintfWriter_write_null(self);
        return;
    }
    NANO_OSAPI_Debug_printf("%s", val->value);
}


void
NANO_LOG_PrintfWriter_write_message_buffer(
        NANO_LOG_Writer *const self,
        const char *const key,
        const NANO_MessageBuffer *const val)
{
    const NANO_MessageBuffer *nxt = val;
    NANO_usize i = 0;
    const NANO_u8 *data = NULL;
    NANO_usize data_len = 0;

    NANO_LOG_PrintfWriter_write_key(self,key);
    if (val == NULL)
    {
        NANO_LOG_PrintfWriter_write_null(self);
        return;
    }
    NANO_OSAPI_Debug_printf("[");
    for (i = 0; nxt != NULL; i++)
    {
        data = NANO_MessageBuffer_data_ptr(nxt);
        data_len = NANO_MessageBuffer_data_len(nxt);

        NANO_OSAPI_Debug_printf(" MBUF[%s:"NANO_LOG_USIZE_FMT"]",
            NANO_MessageBufferType_to_str(NANO_MessageBuffer_type(nxt)),
            i);
        if (NANO_MessageBuffer_flags(nxt, NANO_MESSAGEBUFFERFLAGS_USER_0))
        {
            NANO_OSAPI_Debug_printf("[u0]");
        }
        if (NANO_MessageBuffer_flags(nxt, NANO_MESSAGEBUFFERFLAGS_USER_1))
        {
            NANO_OSAPI_Debug_printf("[u1]");
        }
        if (NANO_MessageBuffer_flags(nxt, NANO_MESSAGEBUFFERFLAGS_USER_2))
        {
            NANO_OSAPI_Debug_printf("[u2]");
        }
        if (NANO_MessageBuffer_flags(nxt, NANO_MESSAGEBUFFERFLAGS_USER_3))
        {
            NANO_OSAPI_Debug_printf("[u3]");
        }
        if (NANO_MessageBuffer_flags(nxt, NANO_MESSAGEBUFFERFLAGS_USER_4))
        {
            NANO_OSAPI_Debug_printf("[u4]");
        }
        if (NANO_MessageBuffer_flags(nxt, NANO_MESSAGEBUFFERFLAGS_USER_5))
        {
            NANO_OSAPI_Debug_printf("[u5]");
        }
        if (NANO_MessageBuffer_flags(nxt, NANO_MESSAGEBUFFERFLAGS_USER_6))
        {
            NANO_OSAPI_Debug_printf("[u6]");
        }
        if (NANO_MessageBuffer_flags_has_userdata(nxt))
        {
            NANO_OSAPI_Debug_printf("[U]");
        }
        NANO_OSAPI_Debug_printf("[%p:%s:%u]", 
            nxt,
            (NANO_MessageBuffer_flags_inline(nxt))?"I":"E",
            NANO_MessageBuffer_data_offset(nxt));
        
        NANO_LOG_PrintfWriter_write_bytes(self, "data", data, data_len);

        nxt = NANO_MessageBuffer_next(nxt);
    }
    NANO_OSAPI_Debug_printf(" ]");
    if (NANO_MessageBuffer_next_msg(val) != NULL)
    {
#if NANO_FEAT_LOG_COLOR
    background_red(stdout);
    text_gray(stdout);
#endif /* NANO_FEAT_LOG_COLOR */

        NANO_OSAPI_Debug_printf("MBUF[NEXT] =");
        NANO_LOG_PrintfWriter_write_ptr(
            self, NULL, NANO_MessageBuffer_next_msg(val));

#if NANO_FEAT_LOG_COLOR
    reset_colors(stdout);
#endif /* NANO_FEAT_LOG_COLOR */
    }
}



void
NANO_LOG_PrintfWriter_write_bytes(
        NANO_LOG_Writer *const self,
        const char *const key,
        const NANO_u8 *const val,
        const NANO_usize len)
{
    NANO_usize i = 0;
    NANO_LOG_PrintfWriter_write_key(self,key);
    NANO_OSAPI_Debug_printf("[%p, "NANO_LOG_USIZE_FMT"]", val, len);
    if (val == NULL)
    {
        return;
    }
    NANO_OSAPI_Debug_printf("{ ");
    for (i = 0; i < len; i++)
    {
        if ((i % sizeof(NANO_u32)) == 0)
        {
            if (i > 0)
            {
#if NANO_FEAT_LOG_COLOR
                reset_colors(stdout);
#endif /* NANO_FEAT_LOG_COLOR */
                NANO_OSAPI_Debug_printf(".");
            }
#if NANO_FEAT_LOG_COLOR
            background_white(text_grey(stdout));
#endif /* NANO_FEAT_LOG_COLOR */
        }
        NANO_OSAPI_Debug_printf("%02X", (NANO_u8)(val[i]));
    }
#if NANO_FEAT_LOG_COLOR
    reset_colors(stdout);
#endif /* NANO_FEAT_LOG_COLOR */
    NANO_OSAPI_Debug_printf(" }");
}


#if NANO_FEAT_REQUEST
void
NANO_LOG_PrintfWriter_write_objectid(
        NANO_LOG_Writer *const self,
        const char *const key,
        const NANO_XRCE_ObjectId val)
{
    NANO_u16 uval = 0;
    uval = NANO_XRCE_ObjectId_to_u16(&val);
    NANO_LOG_PrintfWriter_write_h16(self, key, uval);
}

void
NANO_LOG_PrintfWriter_write_requestid(
        NANO_LOG_Writer *const self,
        const char *const key,
        const NANO_XRCE_RequestId val)
{
    NANO_u16 uval = 0;
    uval = NANO_XRCE_RequestId_to_u16(&val);
    NANO_LOG_PrintfWriter_write_h16(self, key, uval);
}

#endif /* NANO_FEAT_REQUEST */

#endif /* NANO_FEAT_LOG && NANO_FEAT_LOG_IMPL_PRINTF */