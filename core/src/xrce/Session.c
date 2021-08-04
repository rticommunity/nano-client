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

#define HB_RECV_LOG     NANO_LOG_DEBUG
#define HB_SEND_LOG     NANO_LOG_DEBUG
#define ACK_RECV_LOG    NANO_LOG_DEBUG
#define ACK_SEND_LOG    NANO_LOG_DEBUG
#define RESEND_LOG      NANO_LOG_DEBUG
#define SEND_LOG        NANO_LOG_DEBUG
#define ALLOC_LOG       NANO_LOG_TRACE
#define RELEASE_LOG     NANO_LOG_TRACE
#define RECEIVE_LOG     NANO_LOG_TRACE
#define RUN_LOG         NANO_LOG_TRACE
#define RESEND_DBG_LOG  NANO_LOG_TRACE

NANO_PRIVATE
NANO_RetCode
NANO_XRCE_Session_deliver_payload(
    NANO_XRCE_Session *const self,
    NANO_XRCE_Stream *const stream,
    const NANO_MessageBuffer *const message,
    NANO_RetCode *const listener_rc_out,
    NANO_bool *const has_heartbeat_out);


#if NANO_FEAT_ASSERT_LIVELINESS

NANO_PRIVATE
NANO_RetCode
NANO_XRCE_Session_assert_periodic_liveliness(
    NANO_XRCE_Session *const self,
    const NANO_u64 ms_now);

NANO_PRIVATE
NANO_RetCode
NANO_XRCE_Session_assert_liveliness(NANO_XRCE_Session *const self);

#endif /* NANO_FEAT_ASSERT_LIVELINESS */

#if NANO_FEAT_RELIABILITY

void
NANO_XRCE_Session_stream_sendqueue_info(
    NANO_XRCE_Session *const self,
    const NANO_XRCE_ReliableStream *const stream,
    NANO_XRCE_SeqNum *const sn_next_out,
    NANO_XRCE_SeqNum *const sn_queue_head,
    NANO_XRCE_SeqNum *const sn_queue_tail,
    NANO_usize *const queue_len_out);


#define NANO_XRCE_Session_stream_sendqueue_info(s_,st_,sn_next_, q_head_, q_tail_, q_len_)\
{\
    *(sn_next_) = *NANO_XRCE_Stream_sn_next_out(&(st_)->base);\
    *(q_len_) = NANO_MessageBufferQueue_size(&(st_)->send_queue);\
    if (*(q_len_) > 0)\
    {\
        *(q_head_) = *NANO_XRCE_InlineHeaderBuffer_sn(\
                        NANO_MessageBufferQueue_head(&(st_)->send_queue));\
        *(q_tail_) = *NANO_XRCE_InlineHeaderBuffer_sn(\
                        NANO_MessageBufferQueue_tail(&(st_)->send_queue));\
    }\
    else\
    {\
        *(q_head_) = *(sn_next_);\
        *(q_tail_) = *(q_head_);\
    }\
}

#if !NANO_FEAT_AGENT
#if NANO_FEAT_SYSTIME
NANO_PRIVATE
NANO_RetCode
NANO_XRCE_Session_send_periodic_heartbeats(
    NANO_XRCE_Session *const self,
    const NANO_u64 ms_now,
    const NANO_usize min_queue_len);

#if 0
NANO_PRIVATE
NANO_RetCode
NANO_XRCE_Session_send_periodic_acknacks(
    NANO_XRCE_Session *const self,
    const NANO_u64 ms_now);
#endif

#endif /* NANO_FEAT_SYSTIME */

#endif /* !NANO_FEAT_AGENT */

NANO_PRIVATE
NANO_RetCode
NANO_XRCE_Session_send_heartbeat(
    NANO_XRCE_Session *const self,
    NANO_XRCE_ReliableStream *const stream,
    const NANO_bool force);

NANO_PRIVATE
NANO_RetCode
NANO_XRCE_Session_send_acknack(
    NANO_XRCE_Session *const self,
    NANO_XRCE_ReliableStream *const stream);


/******************************************************************************
 *                              AckNackBitmap
 ******************************************************************************/
NANO_PRIVATE
NANO_bool
NANO_XRCE_AckNackBitmap_is_lost(
    NANO_XRCE_AckNackBitmap *const self,
    const NANO_XRCE_SeqNum *const lead_sn,
    const NANO_XRCE_SeqNum *const lost_sn,
    NANO_bool *const outside_window)
{
    NANO_bool rc = NANO_BOOL_FALSE;
    NANO_i32 lost_dist = 0;
    NANO_u16 bitmap = 0,
             flag_mask = 0;

    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)
    NANO_PCOND(lead_sn != NULL)
    NANO_PCOND(lost_sn != NULL)
    NANO_PCOND(outside_window != NULL)

    *outside_window = NANO_BOOL_FALSE;

    lost_dist = NANO_XRCE_SeqNum_distance_from(lost_sn, lead_sn);
    if (lost_dist < 0 || lost_dist >= NANO_XRCE_ACKNACKBITMAP_BITS)
    {
        *outside_window = NANO_BOOL_TRUE;
        goto done;
    }

    flag_mask = 0x01 << lost_dist;
    bitmap = NANO_XRCE_AckNackBitmap_to_u16(self);
    rc = (bitmap & flag_mask) ? NANO_BOOL_TRUE : NANO_BOOL_FALSE;

done:
    NANO_LOG_FN_EXIT_RC(rc)

    return rc;
}

#endif /* NANO_FEAT_RELIABILITY */

/******************************************************************************
 * Message Allocation
 ******************************************************************************/
NANO_PRIVATE
void
NANO_XRCE_Session_release_mbuf_chain(
    NANO_XRCE_Session *const self,
    NANO_XRCE_Stream *const stream,
    NANO_MessageBuffer *const msg);


void
NANO_XRCE_Session_release_message(
    NANO_XRCE_Session *const self,
    NANO_XRCE_Stream *const stream,
    NANO_MessageBuffer *const msg)
{
    NANO_MessageBufferType buffer_type = NANO_MESSAGEBUFFERTYPE_DEFAULT;
#if NANO_FEAT_RELIABILITY
    NANO_XRCE_ReliableStream *rel_stream = (NANO_XRCE_ReliableStream *)stream;
    NANO_MessageBuffer *popped_msg = NULL;
    NANO_MessageBuffer *user_mbuf = NULL;
    NANO_XRCE_MessageHeader *msg_hdr = NULL;
    NANO_XRCE_SubmessageHeader submsg_hdr =
        NANO_XRCE_SUBMESSAGEHEADER_INITIALIZER;
#endif /* NANO_FEAT_RELIABILITY */

    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)
    NANO_PCOND(stream != NULL)
    NANO_PCOND(msg != NULL)
    NANO_PCOND(NANO_MessageBuffer_flags(
        msg, NANO_XRCE_MESSAGEFLAGS_OWNED_BY_STREAM))
    
    /* This function should be called only with messages that are not
       pending to be sent with the transport */
    NANO_PCOND(!(NANO_MessageBuffer_flags(
        msg, NANO_XRCE_MESSAGEFLAGS_PENDING_SEND)))

    buffer_type = NANO_MessageBuffer_type(msg);

    switch (buffer_type)
    {
    case NANO_XRCE_MESSAGETYPE_INLINE_HEADER:
    {

        RELEASE_LOG("RELEASE message",
            NANO_LOG_STREAMID("stream", NANO_XRCE_Stream_id(stream))
            NANO_LOG_MSGHDR("msg_hdr",
                *((NANO_XRCE_MessageHeader *)
                    NANO_XRCE_InlineHeaderBuffer_session_id_ptr(msg)))
            NANO_LOG_SUBMSGHDR("submsg_hdr",
                *((NANO_XRCE_SubmessageHeader *)
                    NANO_XRCE_InlineHeaderBuffer_submsgid_ptr(msg)))
            NANO_LOG_H32("mbuf->flags", msg->hdr.flags)
            NANO_LOG_H8("has_data",
                NANO_MessageBuffer_flags_has_userdata(msg))
            NANO_LOG_BOOL("sent",
                !(NANO_MessageBuffer_flags(
                    msg, NANO_XRCE_MESSAGEFLAGS_PENDING_SEND)))
            NANO_LOG_RC(NANO_MessageBuffer_userdata(msg)))

#if NANO_FEAT_RELIABILITY
        if (NANO_XRCE_Stream_is_reliable(stream))
        {
            /* Check if message was queued in send queue and remove it if so */
            if (msg == NANO_MessageBufferQueue_head(&rel_stream->send_queue))
            {
                NANO_MessageBufferQueue_pop_first(
                    &rel_stream->send_queue, &popped_msg);
            }
            else if (msg == NANO_MessageBufferQueue_tail(&rel_stream->send_queue))
            {
                NANO_MessageBufferQueue_pop_last(
                    &rel_stream->send_queue, &popped_msg);
            }
            else
            {
                NANO_LOG_WARNING(
                    "RELEASING unexpected message on reliable stream",
                    NANO_LOG_STREAMID("stream", NANO_XRCE_Stream_id(stream))
                    NANO_LOG_PTR("msg", msg)
                    NANO_LOG_MSGHDR("msg_hdr",
                        *((NANO_XRCE_MessageHeader *)
                            NANO_XRCE_InlineHeaderBuffer_session_id_ptr(msg)))
                    NANO_LOG_SUBMSGHDR("submsg_hdr",
                        *((NANO_XRCE_SubmessageHeader *)
                            NANO_XRCE_InlineHeaderBuffer_submsgid_ptr(msg)))
                    NANO_LOG_PTR("queue.head",
                        NANO_MessageBufferQueue_head(&rel_stream->send_queue))
                    NANO_LOG_PTR("queue.tail",
                        NANO_MessageBufferQueue_tail(&rel_stream->send_queue)))

                NANO_MessageBufferQueue_extract(&rel_stream->send_queue, msg);
                popped_msg = msg;
            }

            RELEASE_LOG(
                "RELEASED reliable message",
                NANO_LOG_KEY("session.key", self->key)
                NANO_LOG_SESSIONID("session.id", self->id)
                NANO_LOG_STREAMID("stream_id", NANO_XRCE_Stream_id(stream))
                NANO_LOG_SN("sn", *NANO_XRCE_InlineHeaderBuffer_sn(popped_msg))
                NANO_LOG_BOOL("acked",
                    NANO_MessageBuffer_flags(
                        msg, NANO_XRCE_MESSAGEFLAGS_ACKED))
                NANO_LOG_USIZE("queue_size", 
                    NANO_MessageBufferQueue_size(&rel_stream->send_queue))
                if (!NANO_MessageBufferQueue_empty(&rel_stream->send_queue)) {
                    NANO_MessageBuffer *queued =
                        NANO_MessageBufferQueue_head(&rel_stream->send_queue);
                    while (queued != NULL)
                    {
                        NANO_LOG_SN("q_sn",
                            *NANO_XRCE_InlineHeaderBuffer_sn(queued))
                        queued = NANO_MessageBuffer_next_msg(queued);
                    }
                })



            if (!NANO_MessageBuffer_flags(
                    msg, NANO_XRCE_MESSAGEFLAGS_ACKED))
            {
                
                NANO_XRCE_SessionListener_on_message_lost(
                    &self->listener,
                    self,
                    rel_stream,
                    *NANO_XRCE_InlineHeaderBuffer_sn(msg),
                    NANO_XRCE_RELIABLEMESSAGELOSTREASON_LOST_BY_LOCAL_WRITER);
            }
            else
            {
                
                msg_hdr = (NANO_XRCE_MessageHeader *)
                    NANO_MessageBuffer_data_ptr(msg);
                NANO_PCOND(msg_hdr != NULL)

                submsg_hdr.id =
                    NANO_XRCE_InlineHeaderBuffer_submsgid(msg);
                submsg_hdr.flags =
                    NANO_XRCE_InlineHeaderBuffer_submsgflags(msg);
                submsg_hdr.length =
                    NANO_XRCE_InlineHeaderBuffer_submsglen(msg);

                user_mbuf = NANO_MessageBuffer_next(msg);
                NANO_PCOND(user_mbuf != NULL);

                if (submsg_hdr.id != NANO_XRCE_SUBMESSAGEID_FRAGMENT ||
                    NANO_XRCE_SubmessageFlags_FRAGMENT_is_last(
                        submsg_hdr.flags))
                {
                    NANO_XRCE_SessionListener_on_send_complete(
                        &self->listener,
                        self,
                        (NANO_XRCE_ReliableStream *)stream,
                        msg_hdr,
                        &submsg_hdr,
                        user_mbuf);
                }
            }
        }
#endif /* NANO_FEAT_RELIABILITY */

        NANO_XRCE_Session_release_mbuf_chain(self, stream, msg);

        break;
    }
    default:
    {
        NANO_LOG_ERROR("INVALID mbuf for release",
            NANO_LOG_STR("buffer_type",
                NANO_MessageBufferType_to_str(buffer_type))
            NANO_LOG_MBUF("mbuf", msg))
        NANO_UNREACHABLE_CODE
        break;
    }
    }

    NANO_LOG_FN_EXIT
}

NANO_PRIVATE
void
NANO_XRCE_Session_release_mbuf_chain(
    NANO_XRCE_Session *const self,
    NANO_XRCE_Stream *const stream,
    NANO_MessageBuffer *const msg)
{
    NANO_MessageBuffer *nxt_mbuf = NULL,
                       *mbuf = NULL;

    UNUSED_ARG(self);
    UNUSED_ARG(stream);

    mbuf = msg;

    while (mbuf != NULL)
    {
        nxt_mbuf = NANO_MessageBuffer_next(mbuf);

        NANO_LOG_TRACE("RELEASE mbuf",
            NANO_LOG_PTR("ptr", mbuf)
            NANO_LOG_PTR("data", NANO_MessageBuffer_data_ptr(mbuf))
            NANO_LOG_USIZE("data_len", NANO_MessageBuffer_data_len(mbuf))
            NANO_LOG_STR("type",
                NANO_MessageBufferType_to_str(
                    NANO_MessageBuffer_type(mbuf))))
        
        NANO_MessageBuffer_flags_unset(mbuf,
            NANO_XRCE_MESSAGEFLAGS_PENDING_SEND |
            NANO_XRCE_MESSAGEFLAGS_ACKED |
            NANO_XRCE_MESSAGEFLAGS_OWNED_BY_STREAM |
            NANO_XRCE_MESSAGEFLAGS_DISCOVERY);
        NANO_MessageBuffer_reset_userdata(mbuf);
        NANO_MessageBuffer_unlink(mbuf);

#if NANO_FEAT_AGENT
    switch (NANO_MessageBuffer_type(mbuf))
    {
    case NANO_XRCE_MESSAGETYPE_INLINE_HEADER:
    {
        REDAFastBufferPool_returnBuffer(self->pool_header, mbuf);
        break;
    }
    case NANO_XRCE_MESSAGETYPE_INLINE_PAYLOAD:
    {
        REDAFastBufferPool_returnBuffer(self->pool_payload, mbuf);
        break;
    }
    case NANO_XRCE_MESSAGETYPE_EXTERNAL_PAYLOAD:
    {
        REDAFastBufferPool_returnBuffer(self->pool_payload_user, mbuf);
        break;
    }
    case NANO_XRCE_MESSAGETYPE_INLINE_PAYLOAD_TAIL:
    {
        REDAFastBufferPool_returnBuffer(self->pool_payload_tail, mbuf);
        break;
    }
    default:
    {
        NANO_UNREACHABLE_CODE
        break;
    }
    }
#else
    switch (NANO_MessageBuffer_type(mbuf))
    {
    case NANO_XRCE_MESSAGETYPE_INLINE_HEADER:
    {
        stream->storage->header_len -= 1;
        break;
    }
    case NANO_XRCE_MESSAGETYPE_INLINE_PAYLOAD:
    {
        stream->storage->payload_len -= 1;
        break;
    }
    case NANO_XRCE_MESSAGETYPE_EXTERNAL_PAYLOAD:
    {
        stream->storage->payload_user_len -= 1;
        break;
    }
    case NANO_XRCE_MESSAGETYPE_INLINE_PAYLOAD_TAIL:
    {
        stream->storage->payload_tail_len -= 1;
        break;
    }
    default:
    {
        NANO_UNREACHABLE_CODE
        break;
    }
    }
#endif /* NANO_FEAT_AGENT */

        mbuf = nxt_mbuf;
    }
}

#if NANO_FEAT_AGENT
NANO_PRIVATE
NANO_RetCode
NANO_XRCE_Session_initialize_storage(NANO_XRCE_Session *const self)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_usize buffer_size = 0;
    struct REDAFastBufferPoolProperty pool_props =
        REDA_FAST_BUFFER_POOL_PROPERTY_DEFAULT;

    NANO_LOG_FN_ENTRY
    
    NANO_PCOND(self != NULL)
    
    buffer_size = NANO_MESSAGEBUFFER_INLINE_SIZE(\
                    NANO_XRCE_INLINEHEADERBUFFER_INLINE_SIZE_BYTES) * 
                sizeof(NANO_MessageBufferData);
    
    NANO_LOG_DEBUG("INIT session pool HEADERS",
        NANO_LOG_USIZE("buffer_size",buffer_size))

    self->pool_header = REDAFastBufferPool_new(
        buffer_size, RTIOsapiAlignment_getDefaultAlignment(), &pool_props);
    if (self->pool_header == NULL)
    {
        goto done;
    }

    buffer_size = NANO_MESSAGEBUFFER_INLINE_SIZE(\
                    NANO_XRCE_STREAM_INLINE_BUFFER_HEADER_SIZE_BYTES) * 
                sizeof(NANO_MessageBufferData);

    NANO_LOG_DEBUG("INIT session pool PAYLOADS",
        NANO_LOG_USIZE("buffer_size",buffer_size))

    self->pool_payload = REDAFastBufferPool_new(
        buffer_size, RTIOsapiAlignment_getDefaultAlignment(), &pool_props);
    if (self->pool_payload == NULL)
    {
        goto done;
    }

    buffer_size = NANO_MESSAGEBUFFER_INLINE_SIZE(\
                    NANO_XRCE_STREAM_INLINE_BUFFER_HEADER_SIZE_BYTES) * 
                sizeof(NANO_MessageBufferData);

    NANO_LOG_DEBUG("INIT session pool PAYLOADS USER",
        NANO_LOG_USIZE("buffer_size",buffer_size))

    self->pool_payload_user = 
        REDAFastBufferPool_newForStructure(NANO_MessageBuffer, &pool_props);
    if (self->pool_payload_user == NULL)
    {
        goto done;
    }

    buffer_size = NANO_MESSAGEBUFFER_INLINE_SIZE(\
                    NANO_XRCE_STREAM_INLINE_BUFFER_TAIL_SIZE_BYTES) * 
                sizeof(NANO_MessageBufferData);

    NANO_LOG_DEBUG("INIT session pool TAIL",
        NANO_LOG_USIZE("buffer_size",buffer_size))

    self->pool_payload_tail = REDAFastBufferPool_new(
        buffer_size, RTIOsapiAlignment_getDefaultAlignment(), &pool_props);
    if (self->pool_payload_tail == NULL)
    {
        goto done;
    }

    buffer_size = 
        NANO_XRCE_CUSTOMSTREAMSTORAGE_PAYLOAD_IN_SIZE(
                NANO_LIMIT_SESSION_MAX_UNFRAGMENTED_PAYLOAD);

    NANO_LOG_DEBUG("INIT session pool PAYLOAD IN",
        NANO_LOG_USIZE("buffer_size",buffer_size))

    self->pool_payload_in = REDAFastBufferPool_new(
        buffer_size, RTIOsapiAlignment_getDefaultAlignment(), &pool_props);
    if (self->pool_payload_in == NULL)
    {
        goto done;
    }
    
    rc = NANO_RETCODE_OK;
    
done:
    if (NANO_RETCODE_OK != rc)
    {
        /* handle failure */
    }
    
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

void
NANO_XRCE_Session_finalize_storage(NANO_XRCE_Session *const self)
{
    NANO_LOG_FN_ENTRY
    
    NANO_PCOND(self != NULL)

    REDAFastBufferPool_delete(self->pool_header);
    self->pool_header = NULL;

    REDAFastBufferPool_delete(self->pool_payload);
    self->pool_payload = NULL;

    REDAFastBufferPool_delete(self->pool_payload_tail);
    self->pool_payload_tail = NULL;

    REDAFastBufferPool_delete(self->pool_payload_user);
    self->pool_payload_user = NULL;

    REDAFastBufferPool_delete(self->pool_payload_in);
    self->pool_payload_in = NULL;
    
    NANO_LOG_FN_EXIT
}


#else


NANO_PRIVATE
NANO_MessageBuffer *
NANO_XRCE_StreamStorage_next_buffer(
    NANO_XRCE_StreamStorage *const self,
    NANO_MessageBuffer *buffer_pool,
    NANO_MessageBufferData *data_pool,
    const NANO_MessageBufferType buffer_type,
    const NANO_usize data_buffer_size,
    const NANO_usize max_buffers,
    NANO_usize *const buffers_in_use)
{
    NANO_usize i = 0;
    NANO_MessageBuffer *mbuf = NULL;
    
    NANO_LOG_TRACE_FN("get NEXT buffer",
        NANO_LOG_PTR("storage", self)
        NANO_LOG_STR("buffer_type",
            NANO_MessageBufferType_to_str(buffer_type))
        NANO_LOG_PTR("buffer_pool", buffer_pool)
        NANO_LOG_PTR("data_pool", data_pool)
        NANO_LOG_USIZE("data_size", data_buffer_size)
        NANO_LOG_USIZE("max_buffers", max_buffers)
        NANO_LOG_USIZE("buffers_in_use", *buffers_in_use))

    UNUSED_ARG(self);

    if ((buffer_pool != NULL && data_pool != NULL) ||
            (*buffers_in_use == max_buffers))
    {
        goto done;
    }

    for (i = max_buffers; i > 0; i--, mbuf = NULL)
    {
        if (buffer_pool != NULL)
        {
            mbuf = buffer_pool + (i - 1);
        }
        else
        {
            mbuf = (NANO_MessageBuffer *)(data_pool + ((i - 1) * data_buffer_size));
        }

        if (!NANO_MessageBuffer_flags(
                mbuf, NANO_XRCE_MESSAGEFLAGS_OWNED_BY_STREAM))
        {
            break;
        }
    }

    if (mbuf != NULL)
    {
        NANO_PCOND(NANO_MessageBuffer_next(mbuf) == NULL)

        NANO_MessageBuffer_flags_set(
            mbuf, NANO_XRCE_MESSAGEFLAGS_OWNED_BY_STREAM);
        NANO_MessageBuffer_set_type(mbuf, buffer_type);

        if (buffer_pool == NULL)
        {
            NANO_MessageBuffer_flags_set_inline(mbuf);
        }
        *buffers_in_use += 1;
    }

    NANO_LOG_TRACE("NEXT buffer", NANO_LOG_MBUF("mbuf", mbuf))
done:
    return mbuf;
}

NANO_XRCE_InlineHeaderBuffer *
NANO_XRCE_StreamStorage_next_header_buffer(
    NANO_XRCE_StreamStorage *const self);

#define NANO_XRCE_StreamStorage_next_header_buffer(s_)  \
    ((NANO_XRCE_InlineHeaderBuffer *)                  \
         NANO_XRCE_StreamStorage_next_buffer(           \
             (s_),                                      \
             NULL,                                      \
             (s_)->header,                              \
             NANO_XRCE_MESSAGETYPE_INLINE_HEADER,       \
             NANO_XRCE_INLINEHEADERBUFFER_INLINE_SIZE,  \
             (s_)->header_max,\
             &(s_)->header_len))

NANO_MessageBuffer *
NANO_XRCE_StreamStorage_next_payload_buffer(
    NANO_XRCE_StreamStorage *const self);

NANO_MessageBuffer *
NANO_XRCE_StreamStorage_next_payload_user_buffer(
    NANO_XRCE_StreamStorage *const self);

NANO_MessageBuffer *
NANO_XRCE_StreamStorage_next_payload_tail_buffer(
    NANO_XRCE_StreamStorage *const self);

#define NANO_XRCE_StreamStorage_next_payload_buffer(s_) \
    NANO_XRCE_StreamStorage_next_buffer(                \
        (s_),                                           \
        NULL,                                           \
        (s_)->payload,                                  \
        NANO_XRCE_MESSAGETYPE_INLINE_PAYLOAD,           \
        NANO_XRCE_STREAM_INLINE_BUFFER_HEADER_SIZE,     \
        (s_)->payload_max,                              \
        &(s_)->payload_len)

#define NANO_XRCE_StreamStorage_next_payload_user_buffer(s_) \
    NANO_XRCE_StreamStorage_next_buffer(                     \
        (s_),                                                \
        (s_)->payload_user,                                  \
        NULL,                                                \
        NANO_XRCE_MESSAGETYPE_EXTERNAL_PAYLOAD,              \
        0,                                                   \
        (s_)->payload_user_max,                              \
        &(s_)->payload_user_len)

#define NANO_XRCE_StreamStorage_next_payload_tail_buffer(s_) \
    NANO_XRCE_StreamStorage_next_buffer(                     \
        (s_),                                                \
        NULL,                                                \
        (s_)->payload_tail,                                  \
        NANO_XRCE_MESSAGETYPE_INLINE_PAYLOAD_TAIL,           \
        NANO_XRCE_STREAM_INLINE_BUFFER_TAIL_SIZE,            \
        (s_)->payload_tail_max,                              \
        &(s_)->payload_tail_len)

NANO_XRCE_InlineHeaderBuffer *
NANO_XRCE_Stream_next_header_buffer(
    NANO_XRCE_Stream *const self);

#define NANO_XRCE_Stream_next_header_buffer(s_) \
    NANO_XRCE_StreamStorage_next_header_buffer((s_)->storage)

NANO_MessageBuffer *
NANO_XRCE_Stream_next_payload_buffer(
    NANO_XRCE_Stream *const self);

#define NANO_XRCE_Stream_next_payload_buffer(s_) \
    NANO_XRCE_StreamStorage_next_payload_buffer((s_)->storage)

NANO_MessageBuffer *
NANO_XRCE_Stream_next_payload_user_buffer(
    NANO_XRCE_Stream *const self);

#define NANO_XRCE_Stream_next_payload_user_buffer(s_) \
    NANO_XRCE_StreamStorage_next_payload_user_buffer((s_)->storage)

NANO_MessageBuffer *
NANO_XRCE_Stream_next_payload_tail_buffer(
    NANO_XRCE_Stream *const self);

#define NANO_XRCE_Stream_next_payload_tail_buffer(s_) \
    NANO_XRCE_StreamStorage_next_payload_tail_buffer((s_)->storage)


#endif /* NANO_FEAT_AGENT */


NANO_MessageBuffer *
NANO_XRCE_Session_allocate_message(
    NANO_XRCE_Session *const self,
    NANO_XRCE_Stream *stream,
    const NANO_MessageBufferType buffer_type,
    const NANO_usize message_size,
    const NANO_u8 *const external_payload)
{
    NANO_MessageBuffer *mbuf = NULL;
    /* assume an inline buffer was requested (since only
       one type of buffers is not inline) */
    NANO_bool inline_mbuf = NANO_BOOL_TRUE;
#if NANO_FEAT_AGENT
    const NANO_MessageBuffer def_mbuf = NANO_MESSAGEBUFFER_INITIALIZER;
#endif /* NANO_FEAT_AGENT */

    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)
    NANO_PCOND(stream != NULL)
    NANO_PCOND(buffer_type != NANO_MESSAGEBUFFERTYPE_DEFAULT)

    NANO_LOG_TRACE("ALLOCATE message",
        NANO_LOG_SESSIONID("session", self->id)
        NANO_LOG_STREAMID("stream", NANO_XRCE_Stream_id(stream))
        NANO_LOG_USIZE("message_size", message_size)
        NANO_LOG_STR("buffer_type",
            NANO_MessageBufferType_to_str(buffer_type)))

    switch (buffer_type)
    {
    case NANO_XRCE_MESSAGETYPE_INLINE_HEADER:
    {
        if (message_size != NANO_XRCE_SessionId_header_size(self->id) ||
            external_payload != NULL)
        {
            /* invalid message size for buffer type */
            NANO_LOG_ERROR("invalid HEADER buffer request",
                NANO_LOG_USIZE("req_size", message_size)
                NANO_LOG_USIZE("max_size", NANO_XRCE_SessionId_header_size(self->id))
                NANO_LOG_PTR("external_payload", external_payload))
            goto done;
        }
#if NANO_FEAT_AGENT
        mbuf = (NANO_MessageBuffer*)
            REDAFastBufferPool_getBuffer(self->pool_header);
        *mbuf = def_mbuf;
#else
        mbuf = NANO_XRCE_Stream_next_header_buffer(stream);
#endif /* NANO_FEAT_AGENT */
        break;
    }
    case NANO_XRCE_MESSAGETYPE_INLINE_PAYLOAD:
    {
        NANO_PCOND(message_size > 0)
        if (message_size == 0 ||
            message_size > NANO_XRCE_STREAM_INLINE_BUFFER_HEADER_SIZE_BYTES ||
            external_payload != NULL)
        {
            /* invalid message size for buffer type */
            goto done;
        }
#if NANO_FEAT_AGENT
        mbuf = (NANO_MessageBuffer*)
            REDAFastBufferPool_getBuffer(self->pool_payload);
        *mbuf = def_mbuf;
#else
        mbuf = NANO_XRCE_Stream_next_payload_buffer(stream);
#endif /* NANO_FEAT_AGENT */
        break;
    }
    case NANO_XRCE_MESSAGETYPE_EXTERNAL_PAYLOAD:
    {
        if (message_size == 0 || external_payload == NULL)
        {
            /* invalid message size for buffer type */
            goto done;
        }
#if NANO_FEAT_AGENT
        mbuf = (NANO_MessageBuffer*)
            REDAFastBufferPool_getBuffer(self->pool_payload_user);
        *mbuf = def_mbuf;
#else
        mbuf = NANO_XRCE_Stream_next_payload_user_buffer(stream);
#endif /* NANO_FEAT_AGENT */
        inline_mbuf = NANO_BOOL_FALSE;
        break;
    }
    case NANO_XRCE_MESSAGETYPE_INLINE_PAYLOAD_TAIL:
    {
        NANO_PCOND(message_size > 0)
        if (message_size == 0 ||
            message_size > NANO_XRCE_STREAM_INLINE_BUFFER_TAIL_SIZE_BYTES ||
            external_payload != NULL)
        {
            /* invalid message size for buffer type */
            goto done;
        }
#if NANO_FEAT_AGENT
        mbuf = (NANO_MessageBuffer*)
            REDAFastBufferPool_getBuffer(self->pool_payload_tail);
        *mbuf = def_mbuf;
#else
        mbuf = NANO_XRCE_Stream_next_payload_tail_buffer(stream);
#endif /* NANO_FEAT_AGENT */
        break;
    }
    default:
    {
        NANO_LOG_ERROR("unknown/unsupported message buffer type",
                       NANO_LOG_H8("type", buffer_type))
        NANO_UNREACHABLE_CODE
        goto done;
    }
    }

    if (mbuf == NULL)
    {
        /* all buffers already in use */
        NANO_LOG_DEBUG("ALL buffers already in use",
            NANO_LOG_SESSIONID("session", self->id)
            NANO_LOG_STREAMID("stream", NANO_XRCE_Stream_id(stream))
            NANO_LOG_STR("buffer_type",
                NANO_MessageBufferType_to_str(buffer_type)))
        goto done;
    }

    NANO_PCOND(mbuf != NULL)
    NANO_PCOND(NANO_MessageBuffer_next(mbuf) == NULL)


#if NANO_FEAT_AGENT
    /* Usually set by StreamStorage_next_buffer */
    NANO_MessageBuffer_flags_set(
        mbuf, NANO_XRCE_MESSAGEFLAGS_OWNED_BY_STREAM);
    NANO_MessageBuffer_set_type(mbuf, buffer_type);
    if (inline_mbuf)
    {
        NANO_MessageBuffer_flags_set_inline(mbuf);
    }
#endif /* NANO_FEAT_AGENT */

    if (inline_mbuf)
    {
        NANO_MessageBuffer_set_data_len(mbuf, message_size);

        NANO_OSAPI_Memory_zero(
            NANO_MessageBuffer_data_ptr(mbuf),
            NANO_MessageBuffer_data_len(mbuf));
    }
    else
    {
        NANO_MessageBuffer_set_external_data(
            mbuf,
            /* TODO avoid discarding const */
            (NANO_u8 *)external_payload,
            message_size);
    }

    ALLOC_LOG("ALLOCATED message",
        NANO_LOG_SESSIONID("session", self->id)
        NANO_LOG_STREAMID("stream", NANO_XRCE_Stream_id(stream))
        NANO_LOG_BOOL("reliable_stream",
            NANO_XRCE_Stream_is_reliable(stream))
        NANO_LOG_PTR("ptr", mbuf)
        NANO_LOG_PTR("data", NANO_MessageBuffer_data_ptr(mbuf))
        NANO_LOG_USIZE("data_len", NANO_MessageBuffer_data_len(mbuf))
        NANO_LOG_STR("buffer_type",
            NANO_MessageBufferType_to_str(buffer_type))
    )

done:
    NANO_LOG_FN_EXIT_PTR(mbuf)
    return mbuf;
}


#if NANO_FEAT_RELIABILITY
NANO_PRIVATE
NANO_RetCode
NANO_XRCE_Session_deserialize_acknack(
    NANO_XRCE_Session *const self,
    NANO_XRCE_Stream *const stream_recv,
    const NANO_XRCE_MessageHeader *const msg_hdr,
    const NANO_XRCE_SubmessageHeader *const submsg_hdr,
    const NANO_u8 *const payload,
    NANO_XRCE_AckNackPayload *const ackn,
    NANO_XRCE_ReliableStream **const rel_stream_out)
{

    NANO_XRCE_StreamId stream_recv_id = NANO_XRCE_STREAMID_NONE;
    NANO_usize deserialized_size = 0;
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_XRCE_ReliableStream *stream = NULL;

    NANO_LOG_FN_ENTRY

    UNUSED_ARG(msg_hdr);

    stream_recv_id = NANO_XRCE_Stream_id(stream_recv);

    /* ACKNACKS should only be sent over the NONE stream, ignore others */
    if (stream_recv_id != NANO_XRCE_STREAMID_NONE)
    {
        NANO_LOG_WARNING(
            "protocol error: ACKNACK sent on invalid stream",
            NANO_LOG_STREAMID("stream_id", stream_recv_id))
        rc = NANO_RETCODE_PROTOCOL_ERROR;
        goto done;
    }
    if (submsg_hdr->length < NANO_XRCE_ACKNACKPAYLOAD_SERIALIZED_SIZE_MAX)
    {
        /* message is shorter than the expected length of a serialized
         * acknack message. Ignore it. */
        NANO_LOG_DEBUG(
            "protocol error: ACKNACK payload too short",
            NANO_LOG_U16("length", submsg_hdr->length))
        rc = NANO_RETCODE_PROTOCOL_ERROR;
        goto done;
    }

    /* Deserialize AckNackPayload */
#define dptr (payload + deserialized_size)
    NANO_u16_deserialize_aligned(
        &ackn->first_unacked_seq_num,
        ((NANO_u16 *)dptr),
        NANO_XRCE_SubmessageFlags_is_little_endian(submsg_hdr->flags));
    deserialized_size += sizeof(NANO_u16);
    ackn->nack_bitmap = *((NANO_XRCE_AckNackBitmap *)dptr);
    deserialized_size += sizeof(NANO_XRCE_AckNackBitmap);
    ackn->stream_id = *((NANO_XRCE_StreamId *)dptr);
    deserialized_size += sizeof(NANO_XRCE_StreamId);
#undef dptr

    /* The stream id must signal a reliable stream */
    if (!NANO_XRCE_StreamId_is_reliable(ackn->stream_id))
    {
        NANO_LOG_DEBUG(
            "protocol error: ACKNACK sent for best-effort stream",
            NANO_LOG_STREAMID("stream_id", ackn->stream_id))
        rc = NANO_RETCODE_PROTOCOL_ERROR;
        goto done;
    }

    /* lookup stream from id */
    stream = (NANO_XRCE_ReliableStream *)
        NANO_XRCE_Session_lookup_stream(self, ackn->stream_id);
    if (stream == NULL)
    {
        /* Received an acknack for an unknown stream */
        NANO_LOG_DEBUG(
            "ACKNACK received for unknown stream",
            NANO_LOG_STREAMID("stream_id", ackn->stream_id))
        rc = NANO_RETCODE_OK;
        goto done;
    }

    *rel_stream_out = stream;

    rc = NANO_RETCODE_OK;

done:

    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

NANO_PRIVATE
NANO_RetCode
NANO_XRCE_Session_resend_message(
    NANO_XRCE_Session *const self,
    NANO_XRCE_ReliableStream *const stream,
    NANO_MessageBuffer *const queued_mbuf)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_bool extracted = NANO_BOOL_FALSE;
    NANO_MessageBuffer *nxt_mbuf = NULL;

    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)

    /* cache next message if any */
    nxt_mbuf = NANO_MessageBuffer_next_msg(queued_mbuf);
    NANO_MessageBufferQueue_extract(&stream->send_queue, queued_mbuf);
    extracted = NANO_BOOL_TRUE;
    NANO_MessageBuffer_flags_set(
        queued_mbuf, NANO_XRCE_MESSAGEFLAGS_PENDING_SEND);

    NANO_CHECK_RC(
        NANO_XRCE_ClientTransport_queue_send(self->transport, queued_mbuf),
        NANO_LOG_ERROR("transport::queue_send failed",
            NANO_LOG_MSGHDR("hdr",
                *((NANO_XRCE_MessageHeader *)
                    NANO_MessageBuffer_data_ptr(queued_mbuf)))));

    while (NANO_MessageBuffer_flags(
        queued_mbuf, NANO_XRCE_MESSAGEFLAGS_PENDING_SEND))
    {
        NANO_XRCE_ClientTransport_flush_output(self->transport);
    }
    /* the return code of the send operation is stored in
    the "user data" field of the mbuf by the on_message_sent 
    callback */
    NANO_CHECK_RC(
        (NANO_RetCode)NANO_MessageBuffer_userdata(queued_mbuf),
        NANO_LOG_ERROR("FAILED to resend message with transport",
                       NANO_LOG_RC(rc)));

    rc = NANO_RETCODE_OK;

done:
    if (extracted)
    {
        if (nxt_mbuf != NULL)
        {
            NANO_MessageBufferQueue_insert_before(
                &stream->send_queue, queued_mbuf, nxt_mbuf);
        }
        else
        {
            NANO_MessageBufferQueue_insert_last(
                &stream->send_queue, queued_mbuf);
        }
        NANO_LOG_TRACE_FN(
            "RESENT message",
            NANO_LOG_KEY("session.key", self->key)
            NANO_LOG_SESSIONID("session.id", self->id)
            NANO_LOG_STREAMID("stream_id", NANO_XRCE_Stream_id(&stream->base))
            NANO_LOG_SN("sn", *NANO_XRCE_InlineHeaderBuffer_sn(queued_mbuf))
            if (nxt_mbuf != NULL)
            {
                NANO_LOG_SN("next_sn", *NANO_XRCE_InlineHeaderBuffer_sn(nxt_mbuf))
            } 
            NANO_LOG_USIZE("queue_size", NANO_MessageBufferQueue_size(&stream->send_queue))
            if (!NANO_MessageBufferQueue_empty(&stream->send_queue))
            {
                NANO_MessageBuffer *queued =
                    NANO_MessageBufferQueue_head(&stream->send_queue);
                while (queued != NULL)
                {
                    NANO_LOG_SN("q_sn", *NANO_XRCE_InlineHeaderBuffer_sn(queued))
                    queued = NANO_MessageBuffer_next_msg(queued);
                }
            })
    }
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

#if NANO_FEAT_RELIABILITY

NANO_PRIVATE
NANO_usize
NANO_XRCE_Session_dismiss_send_queue(
    NANO_XRCE_Session *const self,
    NANO_XRCE_ReliableStream *const stream,
    const NANO_usize msg_count,
    const NANO_bool confirmed)
{
    NANO_usize i = 0;

    NANO_LOG_FN_ENTRY

    for (i = 0; i < msg_count; i++)
    {
        NANO_MessageBuffer *mbuf = NULL;

        mbuf = NANO_MessageBufferQueue_head(&stream->send_queue);
        if (mbuf == NULL)
        {
            NANO_LOG_ERROR("QUEUE is empty on dismiss",
                NANO_LOG_STREAMID("stream",
                    NANO_XRCE_Stream_id(&stream->base))
                NANO_LOG_USIZE("expected_remaining", msg_count - i))
            goto done;
        }

        if (confirmed)
        {
            NANO_MessageBuffer_flags_set(
                mbuf, NANO_XRCE_MESSAGEFLAGS_ACKED);
        }

        NANO_XRCE_Session_release_message(self, &stream->base, mbuf);
    }
done:
    NANO_LOG_FN_EXIT
    return i;
}

NANO_usize
NANO_XRCE_Session_dismiss_send_queue_up_to(
    NANO_XRCE_Session *const self,
    NANO_XRCE_ReliableStream *const stream_recv,
    const NANO_XRCE_SeqNum *const msg_sn,
    const NANO_bool confirmed)
{
    NANO_XRCE_InlineHeaderBuffer *hdr_mbuf = NULL;
    NANO_MessageBuffer *mbuf = NULL;
    NANO_i8 cmp_res = 0;
    NANO_XRCE_SeqNum *mbuf_sn = NULL;
    NANO_usize dismiss_len = 0,
               res = 0;

    NANO_LOG_FN_ENTRY

    mbuf = NANO_MessageBufferQueue_head(&stream_recv->send_queue);
    while (mbuf != NULL)
    {
        hdr_mbuf = (NANO_XRCE_InlineHeaderBuffer *)mbuf;
        mbuf = NANO_MessageBuffer_next_msg(mbuf);

        mbuf_sn = NANO_XRCE_InlineHeaderBuffer_sn(hdr_mbuf);
        NANO_XRCE_SeqNum_compare(mbuf_sn, msg_sn, &cmp_res);
        if (cmp_res > 0)
        {
            break;
        }
        dismiss_len += 1;
    }

    if (dismiss_len > 0)
    {
        res = NANO_XRCE_Session_dismiss_send_queue(
            self, stream_recv, dismiss_len, confirmed);

        if (dismiss_len != res)
        {
            NANO_LOG_ERROR("UNEXPECTED dismiss len",
                NANO_LOG_STREAMID("stream",
                    NANO_XRCE_Stream_id(&stream_recv->base))
                NANO_LOG_USIZE("expected", dismiss_len)
                NANO_LOG_USIZE("dismissed", res))
        }
    }

    NANO_LOG_FN_EXIT
    return res;
}

#endif /* NANO_FEAT_RELIABILITY */

NANO_PRIVATE
void
NANO_XRCE_Session_on_acknack(
    NANO_XRCE_Session *const self,
    NANO_XRCE_Stream *const stream_recv,
    const NANO_XRCE_MessageHeader *const msg_hdr,
    const NANO_XRCE_SubmessageHeader *const submsg_hdr,
    const NANO_u8 *const payload)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_XRCE_AckNackPayload ackn = NANO_XRCE_ACKNACKPAYLOAD_INITIALIZER;
    NANO_XRCE_ReliableStream *stream = NULL;
    NANO_XRCE_SeqNum send_queue_next = NANO_XRCE_SEQNUM_INITIALIZER,
                     send_queue_head = NANO_XRCE_SEQNUM_INITIALIZER,
                     send_queue_tail = NANO_XRCE_SEQNUM_INITIALIZER,
                     ackn_head = NANO_XRCE_SEQNUM_INITIALIZER,
                     first_unacked = NANO_XRCE_SEQNUM_INITIALIZER,
                     last_acked = NANO_XRCE_SEQNUM_INITIALIZER,
                     mbuf_sn = NANO_XRCE_SEQNUM_INITIALIZER;
    NANO_usize send_queue_len = 0,
               queue_i = 0,
               dismiss_len = 0,
               resent_msgs = 0;

    NANO_i8 cmp_res = 0;

    NANO_MessageBuffer *queued_mbuf = NULL;

    NANO_bool outside_window = NANO_BOOL_FALSE,
              in_window = NANO_BOOL_FALSE,
              msg_requested = NANO_BOOL_FALSE,
              updated_acked = NANO_BOOL_FALSE;

    NANO_LOG_FN_ENTRY

    if (!NANO_XRCE_Session_is_connected(self))
    {
        NANO_LOG_WARNING_MSG("acknack IGNORED because session isn't CONNECTED")
        rc = NANO_RETCODE_OK;
        goto done;
    }

    NANO_CHECK_RC(
        NANO_XRCE_Session_deserialize_acknack(
            self,
            stream_recv,
            msg_hdr,
            submsg_hdr,
            payload,
            &ackn,
            &stream),
        NANO_LOG_ERROR("FAILED to parse ACKNACK",
                       NANO_LOG_RC(rc)));

    if (stream == NULL)
    {
        rc = NANO_RETCODE_OK;
        goto done;
    }

    NANO_XRCE_SeqNum_from_u16(&ackn_head, ackn.first_unacked_seq_num);

    ACK_RECV_LOG("ACKNACK received",
        NANO_LOG_PTR("stream", stream)
        NANO_LOG_STREAMID("stream_id", NANO_XRCE_Stream_id(&stream->base))
        NANO_LOG_SN("stream.next_out", stream->base.next_out)
        NANO_LOG_BOOL("stream.in_ack", stream->in_acknack_recvd)
        NANO_LOG_SN("stream.in_ack_sn", stream->in_acknack_highest_sn)
        NANO_LOG_SN("ackn_head", ackn_head)
        NANO_LOG_H16("ackn_bmap",
            NANO_XRCE_AckNackBitmap_to_u16(&ackn.nack_bitmap))
        NANO_LOG_USIZE("send_queue_len",
            NANO_MessageBufferQueue_size(&stream->send_queue))
        if (NANO_MessageBufferQueue_size(&stream->send_queue) > 0)
        {
            NANO_LOG_SN("send_queue_head",
                *NANO_XRCE_InlineHeaderBuffer_sn(
                    NANO_MessageBufferQueue_head(&stream->send_queue)))
            NANO_LOG_SN("send_queue_tail",
                *NANO_XRCE_InlineHeaderBuffer_sn(
                    NANO_MessageBufferQueue_tail(&stream->send_queue)))
        })

    /* Find the first SN marked as lost in the acknack bitmap */
    first_unacked = ackn_head;
    while (!NANO_XRCE_AckNackBitmap_is_lost(
               &ackn.nack_bitmap, &ackn_head, &first_unacked, &outside_window) &&
           !outside_window)
    {
        NANO_u16 a = 0;

        NANO_XRCE_SeqNum_add(&first_unacked, 1, &first_unacked);

        a = NANO_XRCE_SeqNum_to_u16(&first_unacked);
        if (a == 0x0000)
        {
            NANO_XRCE_SeqNum_from_u16(&first_unacked, 0x0001);
        }
    }

    /* The "last acked SN" is the one before the first unacked one (duh) */
    NANO_XRCE_SeqNum_add(&first_unacked, -1, &last_acked);

    {
        NANO_u16 a = 0;

        a = NANO_XRCE_SeqNum_to_u16(&first_unacked);
        if (a == 0x0001)
        {
            NANO_XRCE_SeqNum_from_u16(&last_acked, 0xFFFF);
        }
    }
    
    /* Check if we should update the highest acked sample */
    if (stream->in_acknack_recvd)
    {
        NANO_XRCE_SeqNum_compare(
            &stream->in_acknack_highest_sn, &last_acked, &cmp_res);
        
        if (cmp_res < 0)
        {
            /* The reader is acknowledging a newer sample, so update the
            state on the stream */

            stream->in_acknack_highest_sn = last_acked;
            updated_acked = NANO_BOOL_TRUE;
        }
        else if (cmp_res > 0)
        {
            /* The highest acked SN is older than some SN we previously saw,
            so it is possible that this is a stale ACKNACK msg. Ignore it */
            NANO_LOG_ERROR("STALE ACKNACK ignored)",
                    NANO_LOG_STREAMID("stream_id",
                        NANO_XRCE_Stream_id(&stream->base))
                    NANO_LOG_SN("stream.highest_acked_sn",
                        stream->in_acknack_highest_sn)
                    NANO_LOG_SN("acknack_highest_sn", last_acked))
            goto done;
        }
    }
    else
    {
        /* this is the first ACKNACK received by this stream, so we
           just initialize the stream */
        stream->in_acknack_highest_sn = last_acked;
        stream->in_acknack_recvd = NANO_BOOL_TRUE;
        updated_acked = NANO_BOOL_TRUE;
    }

    if (updated_acked)
    {
        /* since the reader acked some SNs, go through the send queue and
           see if we can dismiss any samples */

        queued_mbuf = NANO_MessageBufferQueue_head(&stream->send_queue);
        while (queued_mbuf != NULL)
        {
            NANO_XRCE_SeqNum_compare(
                NANO_XRCE_InlineHeaderBuffer_sn(queued_mbuf),
                &last_acked,
                &cmp_res);
            
            if (cmp_res <= 0)
            {
                dismiss_len =
                    NANO_XRCE_Session_dismiss_send_queue(
                        self, stream, 1, NANO_BOOL_TRUE /* confirmed */);
                
                /* this should never happen */
                if (1 != dismiss_len)
                {
                    NANO_LOG_ERROR("FAILED to dismiss messages from send queue",
                        NANO_LOG_STREAMID("stream_id",
                            NANO_XRCE_Stream_id(&stream->base)))
                    goto done;
                }
            }
            else
            {
                /* The message is newer than the oldest acked sn, we're done
                dismissing samples. */
                break;
            }

            queued_mbuf = NANO_MessageBufferQueue_head(&stream->send_queue);
        }
    }


    NANO_XRCE_Session_stream_sendqueue_info(
        self,
        stream,
        &send_queue_next,
        &send_queue_head,
        &send_queue_tail,
        &send_queue_len);

    NANO_XRCE_SeqNum_compare(&send_queue_tail, &first_unacked, &cmp_res);

    if (send_queue_len == 0)
    {
        /* We have no samples in the queue, so the head/tail are the
           next SN that will be written by the writer */
        
        if (cmp_res < 0)
        {
            /* The reader is expecting a sample that is greater than the next
               one that will be sent by the writer. Something is not right */
            NANO_LOG_ERROR("OUT OF SYNC READER detected (waiting for FUTURE samples)",
                NANO_LOG_STREAMID("stream_id", NANO_XRCE_Stream_id(&stream->base))
                NANO_LOG_SN("next_writer_sn", send_queue_tail)
                NANO_LOG_SN("next_reader_sn", first_unacked))
            goto done;
        }
        else if (cmp_res > 0)
        {
            /* the reader is waiting for an old sample which has already been
               discarded from the writer queue.
               We can't send a HB since we don't have any samples to announce */
            NANO_LOG_WARNING("OUT OF SYNC READER detected (waiting for OLD samples on empty queue)",
                NANO_LOG_STREAMID("stream_id", NANO_XRCE_Stream_id(&stream->base))
                NANO_LOG_SN("next_writer_sn", send_queue_tail)
                NANO_LOG_SN("next_reader_sn", first_unacked))
        }
        else /* cmp_res == 0 */
        {
            /* the reader is in sync with the writer, nothing to do */
            ACK_RECV_LOG(
                "READER in SYNC",
                NANO_LOG_STREAMID("stream_id", NANO_XRCE_Stream_id(&stream->base))
                NANO_LOG_SN("stream.next_out", stream->base.next_out)
                NANO_LOG_SN("stream.ack_high_sn", stream->in_acknack_highest_sn))
        }        
    }
    else /* send_queue_len > 0 */
    {
        /* there is at least one sample in the queue. We should check if
           the reader needs to be sent a HB to "bring it up to speed", and
           then we should inspect the queue and the acknack's bitmap to
           see if we can repair any of the messages requested by the reader */
        
        if (cmp_res < 0)
        {
            /* The first NACKED message is newer than the queue's tail,
               so all messages have been received. */
            NANO_LOG_WARNING(
                "READER received ALL SAMPLES",
                NANO_LOG_STREAMID("stream_id", NANO_XRCE_Stream_id(&stream->base))
                NANO_LOG_SN("stream.next_out", stream->base.next_out)
                NANO_LOG_SN("stream.ack_high_sn", stream->in_acknack_highest_sn)
                NANO_LOG_USIZE("stream.queue_len", send_queue_len)
                NANO_LOG_SN("queue.tail", send_queue_tail)
                NANO_LOG_SN("first_unacked", first_unacked))
            
            if (send_queue_len !=
                    NANO_XRCE_Session_dismiss_send_queue(
                        self, stream, send_queue_len, NANO_BOOL_TRUE /* confirmed */))
            {
                NANO_LOG_ERROR_MSG("FAILED to dismiss send queue")
                goto done;
            }
            send_queue_len = 0;
        }
        else
        {
            NANO_XRCE_SeqNum_compare(
                &send_queue_head, &first_unacked, &cmp_res);

            if (cmp_res > 0)
            {
                /* The first NACKED sample is not available in the queue
                any longer, send a HB to bring the reader up to speed */
                NANO_CHECK_RC(
                    NANO_XRCE_Session_send_heartbeat(self, stream, NANO_BOOL_FALSE),
                    NANO_LOG_ERROR("FAILED to send HEARTBEAT",
                        NANO_LOG_SESSIONID("session", *NANO_XRCE_Session_id(self))
                        NANO_LOG_STREAMID("stream", NANO_XRCE_Stream_id(&stream->base))));
            }
            else
            {
                NANO_PCOND(cmp_res >= 0)
            }

        }
    }
    
    for (queue_i = 0,
        outside_window = NANO_BOOL_FALSE,
        queued_mbuf = NANO_MessageBufferQueue_head(&stream->send_queue);
         queued_mbuf != NULL &&
         queue_i < send_queue_len &&
         /* we want to keep scanning the send queue until we have found
            a message within the bitmap's window, and we can stop as soon as
            exited the window's rightmost side (if we're assuming
            "left to right" to go from LSB to MSB) */
         (!in_window || !outside_window) &&
         resent_msgs < NANO_LIMIT_RELIABLESTREAM_RESEND_MESSAGE_MAX;
         queue_i++,
        queued_mbuf = NANO_MessageBuffer_next_msg(queued_mbuf))
    {
        mbuf_sn = *NANO_XRCE_InlineHeaderBuffer_sn(queued_mbuf);
        msg_requested =
            NANO_XRCE_AckNackBitmap_is_lost(
                &ackn.nack_bitmap, &ackn_head, &mbuf_sn, &outside_window);
        
        if (!in_window && !outside_window)
        {
            in_window = NANO_BOOL_TRUE;
        }

        if (msg_requested)
        {
            RESEND_LOG("RESEND msg",
                NANO_LOG_STREAMID("stream_id",
                        NANO_XRCE_Stream_id(&stream->base))
                NANO_LOG_MSGHDR("hdr",
                    *((NANO_XRCE_MessageHeader *)
                        NANO_MessageBuffer_data_ptr(queued_mbuf)))
                NANO_LOG_SUBMSGHDR("submsg",
                    *((NANO_XRCE_SubmessageHeader *)
                        NANO_XRCE_InlineHeaderBuffer_submsgid_ptr(queued_mbuf)))
                NANO_LOG_USIZE("queue_size", NANO_MessageBufferQueue_size(&stream->send_queue)))

            rc = NANO_XRCE_Session_resend_message(self, stream, queued_mbuf);
            if (NANO_RETCODE_OK != rc)
            {
                NANO_LOG_ERROR("FAILED to RESEND message",
                    NANO_LOG_SESSIONID("session", *NANO_XRCE_Session_id(self))
                    NANO_LOG_STREAMID("stream", NANO_XRCE_Stream_id(&stream->base))
                    NANO_LOG_MSGHDR("hdr",
                        *((NANO_XRCE_MessageHeader *)
                            NANO_MessageBuffer_data_ptr(queued_mbuf)))
                        )
                goto done;
            }
            resent_msgs += 1;
        }
    }

done:

    NANO_LOG_FN_EXIT
    return;
}

NANO_PRIVATE
NANO_RetCode
NANO_XRCE_Session_deserialize_heartbeat(
    NANO_XRCE_Session *const self,
    NANO_XRCE_Stream *const stream_recv,
    const NANO_XRCE_MessageHeader *const msg_hdr,
    const NANO_XRCE_SubmessageHeader *const submsg_hdr,
    const NANO_u8 *const payload,
    NANO_XRCE_HeartbeatPayload *const hb,
    NANO_XRCE_ReliableStream **const rel_stream_out)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_usize deserialized_size = 0;
    NANO_XRCE_ReliableStream *stream = NULL;
    NANO_XRCE_StreamId stream_recv_id = NANO_XRCE_STREAMID_NONE;

    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)

    UNUSED_ARG(msg_hdr);

    stream_recv_id = NANO_XRCE_Stream_id(stream_recv);

    /* HEARTBEATs should only be sent over the NONE stream, ignore others */
    if (stream_recv_id != NANO_XRCE_STREAMID_NONE)
    {
        NANO_LOG_WARNING(
            "protocol error: HEARTBEAT sent on invalid stream",
            NANO_LOG_STREAMID("stream_id", stream_recv_id))
        rc = NANO_RETCODE_PROTOCOL_ERROR;
        goto done;
    }

    if (submsg_hdr->length < NANO_XRCE_HEARTBEATPAYLOAD_SERIALIZED_SIZE_MAX)
    {
        /* message is shorter than the expected length of a serialized
         * heartbeat message. Ignore it. */
        NANO_LOG_WARNING(
            "protocol error: HEARTBEAT payload too short",
            NANO_LOG_U16("length", submsg_hdr->length))
        rc = NANO_RETCODE_PROTOCOL_ERROR;
        goto done;
    }

#define dptr (payload + deserialized_size)
    /* Deserialize AckNackPayload */
    NANO_u16_deserialize_aligned(
        &hb->first_unacked_seq_num,
        ((NANO_u16 *)dptr),
        NANO_XRCE_SubmessageFlags_is_little_endian(submsg_hdr->flags));
    deserialized_size += sizeof(NANO_u16);
    NANO_u16_deserialize_aligned(
        &hb->last_unacked_seq_num,
        ((NANO_u16 *)dptr),
        NANO_XRCE_SubmessageFlags_is_little_endian(submsg_hdr->flags));
    deserialized_size += sizeof(NANO_u16);
    hb->stream_id = *((NANO_XRCE_StreamId *)dptr);
    deserialized_size += sizeof(NANO_XRCE_StreamId);
#undef dptr

    /* The stream id must signal a reliable stream */
    if (!NANO_XRCE_StreamId_is_reliable(hb->stream_id))
    {
        NANO_LOG_WARNING(
            "protocol error: HEARTBEAT sent for best-effort stream",
            NANO_LOG_STREAMID("stream_id", hb->stream_id))
        rc = NANO_RETCODE_PROTOCOL_ERROR;
        goto done;
    }

    /* lookup stream from id */
    stream = (NANO_XRCE_ReliableStream *)
        NANO_XRCE_Session_lookup_stream(self, hb->stream_id);
    if (stream == NULL)
    {
        /* Received an heartbeat for an unknown stream */
        NANO_LOG_DEBUG(
            "HEARTBEAT received for unknown stream",
            NANO_LOG_STREAMID("stream_id", hb->stream_id))
        rc = NANO_RETCODE_OK;
        goto done;
    }

    *rel_stream_out = stream;

    rc = NANO_RETCODE_OK;

done:

    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

NANO_PRIVATE
void
NANO_XRCE_Session_on_heartbeatI(
    NANO_XRCE_Session *const self,
    NANO_XRCE_ReliableStream *const stream,
    const NANO_XRCE_HeartbeatPayload *const hb,
    const NANO_bool piggyback)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_XRCE_SeqNum hb_head = NANO_XRCE_SEQNUM_INITIALIZER,
                     hb_tail = NANO_XRCE_SEQNUM_INITIALIZER;
    NANO_i32 writer_queue_len = 0;
    NANO_bool send_ack = NANO_BOOL_FALSE;

    NANO_i8 cmp_res = 0;

    NANO_LOG_FN_ENTRY

    UNUSED_ARG(piggyback);

    NANO_XRCE_SeqNum_from_u16(&hb_head, hb->first_unacked_seq_num);
    NANO_XRCE_SeqNum_from_u16(&hb_tail, hb->last_unacked_seq_num);

    NANO_XRCE_SeqNum_compare(&hb_tail, &hb_head, &cmp_res);
    if (cmp_res < 0)
    {
        writer_queue_len = 0;
    }
    else
    {
        writer_queue_len =
            NANO_XRCE_SeqNum_distance_from(&hb_tail, &hb_head) + 1;
    }
    NANO_PCOND(writer_queue_len != 0);

    /* Check that the HB "makes sense", i.e. the first SN is less or equal to
       the last SN. */
    if (writer_queue_len <= 0)
    {
        NANO_LOG_ERROR("INVALID heartbeat LENGTH",
            NANO_LOG_SESSIONID("session", self->id)
            NANO_LOG_STREAMID("stream", NANO_XRCE_Stream_id(&stream->base))
            NANO_LOG_SN("writer_head", hb_head)
            NANO_LOG_SN("writer_tail", hb_tail)
            NANO_LOG_I32("writer_len", writer_queue_len)
            NANO_LOG_USIZE("writer_len_max", NANO_XRCE_ACKNACKBITMAP_BITS)
            NANO_LOG_BOOL("piggy", piggyback))
        goto done;
    }
    
    /* Check if we should update the highest announced SN */
    if (stream->in_heartbeat_recvd)
    {
        NANO_XRCE_SeqNum_compare(
            &stream->in_heartbeat_lowest_sn, &hb_head, &cmp_res);
        
        if (cmp_res < 0)
        {
            stream->in_heartbeat_lowest_sn = hb_head;
        }

        NANO_XRCE_SeqNum_compare(
            &stream->in_heartbeat_highest_sn, &hb_tail, &cmp_res);
        
        if (cmp_res < 0)
        {
            stream->in_heartbeat_highest_sn = hb_tail;
        }
    }
    else
    {
        /* This is the first HB received by the stream, so we initialize
           the state without comparison */
        stream->in_heartbeat_lowest_sn = hb_head;
        stream->in_heartbeat_highest_sn = hb_tail;
        stream->in_heartbeat_recvd = NANO_BOOL_TRUE;
    }

    NANO_XRCE_SeqNum_compare(
        &stream->base.next_in,
        &stream->in_heartbeat_lowest_sn,
        &cmp_res);
    
    HB_RECV_LOG("HEARTBEAT received",
        NANO_LOG_PTR("stream", stream)
        NANO_LOG_STREAMID("stream_id", NANO_XRCE_Stream_id(&stream->base))
        NANO_LOG_SN("stream.next_in", stream->base.next_in)
        NANO_LOG_BOOL("stream.in_hb", stream->in_heartbeat_recvd)
        NANO_LOG_SN("stream.in_hb_sn.low", stream->in_heartbeat_lowest_sn)
        NANO_LOG_SN("stream.in_hb_sn.high", stream->in_heartbeat_highest_sn)
        NANO_LOG_I32("cmp_res", cmp_res))
    
    if (cmp_res < 0)
    {
#if NANO_FEAT_FRAGMENT
        /* Reset input FRAG buffer */
        NANO_LOG_WARNING("RESETTING recv_frag_buffer",
            NANO_LOG_SESSIONID("session", self->id)
            NANO_LOG_STREAMID("stream",
                NANO_XRCE_Stream_id(&stream->base)))
        NANO_MessageBuffer_set_data_len(&stream->recv_frag_buffer, 0);
#endif /* NANO_FEAT_FRAGMENT */

        while (cmp_res < 0)
        {
            NANO_LOG_WARNING("SAMPLES lost by writer",
                NANO_LOG_SESSIONID("session", self->id)
                NANO_LOG_STREAMID("stream",
                    NANO_XRCE_Stream_id(&stream->base))
                NANO_LOG_SN("reader_head", stream->base.next_in)
                NANO_LOG_SN("writer_head", stream->in_heartbeat_lowest_sn))
            
            NANO_XRCE_SessionListener_on_message_lost(
                &self->listener,
                self,
                stream,
                stream->base.next_in,
                NANO_XRCE_RELIABLEMESSAGELOSTREASON_LOST_BY_REMOTE_WRITER);
            
            NANO_XRCE_SeqNum_plusplus(&stream->base.next_in);
            
            NANO_XRCE_SeqNum_compare(
                &stream->base.next_in,
                &stream->in_heartbeat_lowest_sn,
                &cmp_res);
        }
    }
    else if (cmp_res >= 0)
    {
        send_ack = NANO_BOOL_TRUE;
    }
    
    if (!send_ack)
    {
        NANO_XRCE_SeqNum_compare(
            &stream->base.next_in,
            &stream->in_heartbeat_highest_sn,
            &cmp_res);
        
        send_ack = (cmp_res <= 0);
    }
    
    if (send_ack)
    {
        NANO_CHECK_RC(
            NANO_XRCE_Session_send_acknack(self, stream),
            NANO_LOG_ERROR("FAILED to send ACKNACK",
                NANO_LOG_SESSIONID("session", *NANO_XRCE_Session_id(self))
                NANO_LOG_STREAMID("stream",
                    NANO_XRCE_Stream_id(&stream->base))));
    }
    else
    {
        HB_RECV_LOG("no reply to HEARTBEAT",
            NANO_LOG_PTR("stream", stream)
            NANO_LOG_STREAMID("stream.id", NANO_XRCE_Stream_id(&stream->base))
            NANO_LOG_SN("stream.next_in", stream->base.next_in)
            NANO_LOG_BOOL("stream.first_hb", !stream->in_heartbeat_recvd)
            NANO_LOG_SN("stream.hb_high_sn", stream->in_heartbeat_highest_sn)
            NANO_LOG_SN("writer_head", hb_head)
            NANO_LOG_SN("writer_tail", hb_tail)
            NANO_LOG_USIZE("writer_len", writer_queue_len)
            NANO_LOG_BOOL("piggy", piggyback))
    }
done:
    NANO_LOG_FN_EXIT
    return;
}

NANO_PRIVATE
void
NANO_XRCE_Session_on_heartbeat(
    NANO_XRCE_Session *const self,
    NANO_XRCE_Stream *const stream_recv,
    const NANO_XRCE_MessageHeader *const msg_hdr,
    const NANO_XRCE_SubmessageHeader *const submsg_hdr,
    const NANO_u8 *const payload)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_XRCE_HeartbeatPayload hb = NANO_XRCE_HEARTBEATPAYLOAD_INITIALIZER;
    NANO_XRCE_ReliableStream *stream = NULL;;

    NANO_LOG_FN_ENTRY

    if (!NANO_XRCE_Session_is_connected(self))
    {
        NANO_LOG_WARNING_MSG("heartbeat IGNORED because session isn't CONNECTED")
        rc = NANO_RETCODE_OK;
        goto done;
    }

    NANO_CHECK_RC(
        NANO_XRCE_Session_deserialize_heartbeat(
            self, stream_recv, msg_hdr, submsg_hdr, payload, &hb, &stream),
        NANO_LOG_ERROR("FAILED to deserialize HEARTBEAT",
            NANO_LOG_SESSIONID("session", *NANO_XRCE_Session_id(self))
            NANO_LOG_STREAMID("stream", NANO_XRCE_Stream_id(stream_recv))
            NANO_LOG_BYTES("payload", payload, submsg_hdr->length)));

    if (stream == NULL)
    {
        rc = NANO_RETCODE_OK;
        goto done;
    }

    NANO_XRCE_Session_on_heartbeatI(self, stream, &hb, NANO_BOOL_FALSE);

done:

    NANO_LOG_FN_EXIT
    return;
}
#endif /* NANO_FEAT_RELIABILITY */

#if NANO_FEAT_FRAGMENT
void
NANO_XRCE_Session_on_fragment(
    NANO_XRCE_Session *const self,
    NANO_XRCE_Stream *const stream_recv,
    const NANO_XRCE_MessageHeader *const msg_hdr,
    const NANO_XRCE_SubmessageHeader *const submsg_hdr,
    const NANO_u8 *const payload,
    NANO_RetCode *const listener_rc_out,
    NANO_bool *const has_heartbeat_out)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_XRCE_StreamId stream_id = NANO_XRCE_STREAMID_NONE;
    NANO_XRCE_ReliableStream *rel_stream = NULL;
    NANO_usize frag_buffer_size = 0;
    NANO_bool final = NANO_BOOL_FALSE,
              first = NANO_BOOL_FALSE;
    NANO_u8 *ptr = NULL;
    const NANO_u8 *payload_ptr = payload;
    NANO_MessageBuffer *frag_buffer = NULL;
    const NANO_XRCE_SeqNum def_sn = NANO_XRCE_SEQNUM_INITIALIZER;
    // NANO_XRCE_SeqNum *sn = NULL;
    NANO_u16 frag_submsg_len = 0,
             cur_frag_len = 0;

    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)

    stream_id = NANO_XRCE_Stream_id(stream_recv);

    if (!NANO_XRCE_StreamId_is_reliable(stream_id))
    {
        NANO_LOG_ERROR("FRAGMENT received on best-effort stream",
            NANO_LOG_KEY("key", self->key)
            NANO_LOG_SESSIONID("session", self->id)
            NANO_LOG_STREAMID("stream", stream_id))
        rc = NANO_RETCODE_PROTOCOL_ERROR;
        goto done;
    }

    rel_stream = (NANO_XRCE_ReliableStream *)stream_recv;
    final = NANO_XRCE_SubmessageFlags_FRAGMENT_is_last(submsg_hdr->flags);
    first = NANO_XRCE_SubmessageFlags_FRAGMENT_is_first(submsg_hdr->flags);

    frag_buffer = NANO_XRCE_ReliableStream_recv_frag_buffer(rel_stream);
    ptr = NANO_MessageBuffer_data_ptr(frag_buffer);
#if NANO_FEAT_AGENT
    if (ptr == NULL)
    {
        const NANO_usize max_unfragmented_size =
            NANO_U16_MAX + sizeof(NANO_XRCE_SubmessageHeader);

        NANO_CHECK_RC(
            NANO_XRCE_Session_register_reliable_stream_fragment_storage(
                self,
                NANO_XRCE_Stream_id(stream_recv),
                NANO_XRCE_MESSAGETYPE_FRAGMENT_RECV_BUFFER,
                NULL,
                0,
                max_unfragmented_size),
            NANO_LOG_ERROR("FAILED to register reliable stream FRAG_IN storage",
                NANO_LOG_KEY("key", self->key)
                NANO_LOG_SESSIONID("session", self->id)
                NANO_LOG_STREAMID("stream", NANO_XRCE_Stream_id(stream_recv))
                NANO_LOG_USIZE("max_unfrag_size", max_unfragmented_size)));

        frag_buffer = NANO_XRCE_ReliableStream_recv_frag_buffer(rel_stream);
        ptr = NANO_MessageBuffer_data_ptr(frag_buffer);

        NANO_LOG_INFO("ALLOCATED frag_buffer",
            NANO_LOG_KEY("key", self->key)
            NANO_LOG_SESSIONID("session", self->id)
            NANO_LOG_STREAMID("stream", NANO_XRCE_Stream_id(stream_recv))
            NANO_LOG_USIZE("frag_size", max_unfragmented_size))
    }
#endif /* NANO_FEAT_AGENT */
    if (ptr == NULL)
    {
        NANO_LOG_ERROR("no buffer to receive fragments in",
            NANO_LOG_KEY("key", self->key)
            NANO_LOG_SESSIONID("session", self->id)
            NANO_LOG_STREAMID("stream", stream_id))
        goto done;
    }
    frag_buffer_size = NANO_MessageBuffer_data_len(frag_buffer);

    RECEIVE_LOG("received FRAGMENT",
        NANO_LOG_KEY("key", self->key)
        NANO_LOG_SESSIONID("session", self->id)
        NANO_LOG_STREAMID("stream", NANO_XRCE_Stream_id(stream_recv))
        NANO_LOG_USIZE("frag_size", submsg_hdr->length)
        NANO_LOG_USIZE("tot_frag_size", frag_buffer_size)
        NANO_LOG_BOOL("final", final))

    cur_frag_len = submsg_hdr->length;

    if (frag_buffer_size == 0 || first)
    {
        if (frag_buffer_size)
        {
            NANO_LOG_WARNING("DISMISSING partially reconstructed buffer",
                NANO_LOG_KEY("key", self->key)
                NANO_LOG_SESSIONID("session", self->id)
                NANO_LOG_STREAMID("stream", NANO_XRCE_Stream_id(stream_recv))
                NANO_LOG_USIZE("cur_frag_buffer_size", frag_buffer_size))
            NANO_MessageBuffer_set_data_len(frag_buffer, 0);
            rel_stream->expected_recv_frag_buffer_size = 0;
        }
        
        /* Check that fragment is big enough to contain at least 
           a submsg header */
        if (submsg_hdr->length < sizeof(NANO_XRCE_SubmessageHeader))
        {
            NANO_LOG_ERROR("INVALID first FRAGMENT: too short",
                NANO_LOG_KEY("key", self->key)
                NANO_LOG_SESSIONID("session", self->id)
                NANO_LOG_STREAMID("stream", NANO_XRCE_Stream_id(stream_recv))
                NANO_LOG_MSGHDR("msg", *msg_hdr)
                NANO_LOG_SUBMSGHDR("submsg", *submsg_hdr))
            goto done;
        }
        
        frag_submsg_len =
            NANO_u16_from_ptr_le(
                (NANO_u8*)
                    (&((NANO_XRCE_SubmessageHeader*)payload_ptr)->length));
        
        if (frag_submsg_len == 0)
        {
            NANO_LOG_ERROR("UNEXPECTED zero length FRAGMENT",
                NANO_LOG_KEY("key", self->key)
                NANO_LOG_SESSIONID("session", self->id)
                NANO_LOG_STREAMID("stream", NANO_XRCE_Stream_id(stream_recv))
                NANO_LOG_MSGHDR("msg", *msg_hdr)
                NANO_LOG_SUBMSGHDR("submsg", *submsg_hdr)
                NANO_LOG_SUBMSGHDR("frag.submsg", 
                    *((NANO_XRCE_SubmessageHeader*)
                        NANO_XRCE_InlineHeaderBuffer_submsgid_ptr(frag_buffer)))
                NANO_LOG_MBUF("frag_buffer", frag_buffer))
            goto done;
        }

        rel_stream->expected_recv_frag_buffer_size =
            frag_submsg_len + NANO_XRCE_SessionId_header_size(self->id);
        NANO_PCOND(
            rel_stream->expected_recv_frag_buffer_size <=
                    rel_stream->recv_frag_buffer_size)

        NANO_LOG_DEBUG("BEGIN recv FRAGMENT",
            NANO_LOG_KEY("key", self->key)
            NANO_LOG_SESSIONID("session", self->id)
            NANO_LOG_STREAMID("stream", NANO_XRCE_Stream_id(stream_recv))
            NANO_LOG_USIZE("expected_size",
                rel_stream->expected_recv_frag_buffer_size)
            NANO_LOG_U16("frag_submsg_len", frag_submsg_len)
            NANO_LOG_U16("cur_frag_len", cur_frag_len))

        /* Initialize the frag_in buffer with the XRCE message header */
        *NANO_XRCE_InlineHeaderBuffer_session_id_ptr(frag_buffer) = self->id;
        *NANO_XRCE_InlineHeaderBuffer_stream_id_ptr(frag_buffer) = stream_id;
        NANO_CDR_u16_set_ptr(
            NANO_XRCE_InlineHeaderBuffer_sn_ptr(frag_buffer),
            NANO_XRCE_SeqNum_to_u16(&def_sn));
        
        if (NANO_XRCE_SessionId_has_client_key(self->id))
        {
            *NANO_XRCE_InlineHeaderBuffer_clientkey_ptr(frag_buffer) =
                self->key;
        }
        
        /* Copy submessage header from received frag */
        NANO_MessageBuffer_set_data_len(frag_buffer,
            NANO_XRCE_SessionId_header_size(self->id));
        NANO_OSAPI_Memory_copy(
            NANO_XRCE_InlineHeaderBuffer_submsgid_ptr(frag_buffer),
            payload_ptr,
            sizeof(NANO_XRCE_SubmessageHeader));
        
        payload_ptr += sizeof(NANO_XRCE_SubmessageHeader);
        cur_frag_len -= sizeof(NANO_XRCE_SubmessageHeader);
        frag_buffer_size = NANO_MessageBuffer_data_len(frag_buffer);
    }

    if (rel_stream->expected_recv_frag_buffer_size == 0)
    {
        NANO_LOG_WARNING("UNEXPECTED fragment",
            NANO_LOG_KEY("key", self->key)
            NANO_LOG_SESSIONID("session", self->id)
            NANO_LOG_STREAMID("stream", NANO_XRCE_Stream_id(stream_recv))
            NANO_LOG_MSGHDR("msg", *msg_hdr)
            NANO_LOG_SUBMSGHDR("submsg", *submsg_hdr))
        goto done;
    }

    if ((rel_stream->expected_recv_frag_buffer_size - frag_buffer_size) < 
            cur_frag_len)
    {
        NANO_LOG_ERROR("not enough space in FRAGMENT buffer",
            NANO_LOG_KEY("key", self->key)
            NANO_LOG_SESSIONID("session", self->id)
            NANO_LOG_STREAMID("stream", NANO_XRCE_Stream_id(stream_recv))
            NANO_LOG_USIZE("current_size", frag_buffer_size)
            NANO_LOG_USIZE("new_fragment", cur_frag_len)
            NANO_LOG_USIZE("max", rel_stream->recv_frag_buffer_size)
            NANO_LOG_USIZE("expected", rel_stream->expected_recv_frag_buffer_size))
        rc = NANO_RETCODE_OUT_OF_RESOURCES;
        goto done;
    }

    NANO_OSAPI_Memory_copy(ptr + frag_buffer_size, payload_ptr, cur_frag_len);
    frag_buffer_size += cur_frag_len;
    NANO_MessageBuffer_set_data_len(frag_buffer, frag_buffer_size);

    if (final)
    {
        /* Copy XRCE Header to frag_buffer */
        NANO_OSAPI_Memory_copy(ptr, msg_hdr,
            /* NANO_XRCE_SessionId_header_size returns a length which 
               includes one submsg header */
            NANO_XRCE_SessionId_header_size(self->id) -
                sizeof(NANO_XRCE_SubmessageHeader));

        RECEIVE_LOG("deliver reconstructed FRAGMENTS",
            NANO_LOG_KEY("key", self->key)
            NANO_LOG_SESSIONID("session", self->id)
            NANO_LOG_STREAMID("stream", NANO_XRCE_Stream_id(stream_recv))
            NANO_LOG_MSGHDR("hdr",
                *((NANO_XRCE_MessageHeader *)
                    NANO_MessageBuffer_data_ptr(frag_buffer)))
            NANO_LOG_SUBMSGHDR("submsg",
                *((NANO_XRCE_SubmessageHeader *)
                    NANO_XRCE_InlineHeaderBuffer_submsgid_ptr(frag_buffer))))

        /* TODO Check that the reconstructed payload doesn't contain a FRAG
           message */

        // sn = NANO_XRCE_InlineHeaderBuffer_sn(frag_buffer);

        rc = NANO_XRCE_Session_deliver_payload(
                self,
                stream_recv,
                frag_buffer,
                listener_rc_out,
                has_heartbeat_out);

        // self->recvd_messages += 1;

        // NANO_XRCE_SeqNum_add(sn, 1, &rel_stream->base.next_in);
        // {
        //     /* Skip SN 0x0000 */
        //     NANO_u16 sn_u = 0;
        //     sn_u = NANO_XRCE_SeqNum_to_u16(&rel_stream->base.next_in);
        //     if (sn_u == 0)
        //     {
        //         NANO_XRCE_SeqNum_plusplus(&rel_stream->base.next_in);
        //     }
        // }
        
        /* Reset length of recv frag_buffer since the reconstructed
            message was handled successfully */
        NANO_MessageBuffer_set_data_len(frag_buffer, 0);
        rel_stream->expected_recv_frag_buffer_size = 0;
    }

    rc = NANO_RETCODE_OK;

done:
    if (NANO_RETCODE_OK != rc)
    {
        if (rel_stream != NULL)
        {
            NANO_LOG_ERROR("RESETTING recv_frag_buffer",
                NANO_LOG_KEY("key", self->key)
                NANO_LOG_SESSIONID("session", self->id)
                NANO_LOG_STREAMID("stream",
                    NANO_XRCE_Stream_id(&rel_stream->base)))
            /* Reset frag buffer on error */
            NANO_MessageBuffer_set_data_len(&rel_stream->recv_frag_buffer, 0);
            rel_stream->expected_recv_frag_buffer_size = 0;
        }
    }

    NANO_LOG_FN_EXIT
    return;
}

#endif /* NANO_FEAT_FRAGMENT */

NANO_PRIVATE
NANO_RetCode
NANO_XRCE_Session_deliver_payload(
    NANO_XRCE_Session *const self,
    NANO_XRCE_Stream *const stream,
    const NANO_MessageBuffer *const message,
    NANO_RetCode *const listener_rc_out,
    NANO_bool *const has_heartbeat_out)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    const NANO_u8 *data_ptr = NULL;
    NANO_u16 data_len = 0,
             deserialized_size = 0;
    NANO_XRCE_MessageHeader *msg_hdr = NULL;
    NANO_XRCE_SubmessageHeader submsg_hdr =
        NANO_XRCE_SUBMESSAGEHEADER_INITIALIZER;

    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)
    NANO_PCOND(stream != NULL)
    NANO_PCOND(message != NULL)

    msg_hdr = (NANO_XRCE_MessageHeader *)NANO_MessageBuffer_data_ptr(message);
    NANO_PCOND(msg_hdr != NULL);

    data_ptr = NANO_MessageBuffer_data_ptr(message) +
               NANO_XRCE_INLINEHEADERBUFFER_SUBMSG_OFFSET(self->id);
    data_len = NANO_MessageBuffer_data_len(message) -
               NANO_XRCE_INLINEHEADERBUFFER_SUBMSG_OFFSET(self->id);

    NANO_LOG_TRACE("DELIVER payload",
        NANO_LOG_KEY("key", self->key)
        NANO_LOG_SESSIONID("session", self->id)
        NANO_LOG_STREAMID("stream", NANO_XRCE_Stream_id(stream))
        NANO_LOG_MBUF("msg", message)
        NANO_LOG_BYTES("data", data_ptr, data_len))

#define dptr (data_ptr + deserialized_size)

    while (deserialized_size < data_len &&
           data_len - deserialized_size >=
               NANO_XRCE_SUBMESSAGEHEADER_SERIALIZED_SIZE_MAX)
    {
        deserialized_size = 
            NANO_OSAPI_Memory_align_size_up(
                deserialized_size, NANO_XRCE_Submessage_alignment);
        
        submsg_hdr.id = *((NANO_XRCE_SubmessageId *)dptr);
        deserialized_size += sizeof(NANO_XRCE_SubmessageId);
        submsg_hdr.flags = *((NANO_XRCE_SubmessageFlags *)dptr);
        deserialized_size += sizeof(NANO_XRCE_SubmessageFlags);
        submsg_hdr.length = NANO_u16_from_ptr_le(dptr);
        deserialized_size += sizeof(NANO_u16);

        NANO_LOG_TRACE("PARSED submessage",
            NANO_LOG_SUBMSGHDR("hdr", submsg_hdr)
            NANO_LOG_BYTES("data", dptr, submsg_hdr.length))

        if (data_len - submsg_hdr.length > deserialized_size)
        {
            /* Submessage says it's longer than the actual data */
            NANO_LOG_WARNING(
                "submessage length exceeds message size",
                NANO_LOG_KEY("key", self->key)
                NANO_LOG_SESSIONID("session", self->id)
                NANO_LOG_STREAMID("stream", NANO_XRCE_Stream_id(stream))
                NANO_LOG_U16("remaining_msg", data_len - deserialized_size)
                NANO_LOG_U16("submsg_len", submsg_hdr.length))
            goto done;
        }

        NANO_LOG_TRACE("DELIVER submessage",
            NANO_LOG_MSGHDR("msghdr", *msg_hdr)
            NANO_LOG_SUBMSGHDR("subhdr", submsg_hdr)
            NANO_LOG_BYTES("data", dptr, submsg_hdr.length))

#if NANO_FEAT_RELIABILITY
        *has_heartbeat_out =
            (NANO_XRCE_SubmessageFlags_has_heartbeat(submsg_hdr.flags)) ? NANO_BOOL_TRUE : NANO_BOOL_FALSE;
#endif /* NANO_FEAT_RELIABILITY */

        switch (submsg_hdr.id)
        {
#if NANO_FEAT_RELIABILITY
        case NANO_XRCE_SUBMESSAGEID_ACKNACK:
        {
            NANO_PCOND(stream != NULL)
            NANO_XRCE_Session_on_acknack(
                self, stream, msg_hdr, &submsg_hdr, dptr);
            *listener_rc_out = NANO_RETCODE_OK;
            break;
        }
        case NANO_XRCE_SUBMESSAGEID_HEARTBEAT:
        {
            NANO_PCOND(stream != NULL)
            NANO_XRCE_Session_on_heartbeat(
                self, stream, msg_hdr, &submsg_hdr, dptr);
            *listener_rc_out = NANO_RETCODE_OK;
            break;
        }
#endif /* NANO_FEAT_RELIABILITY */
#if NANO_FEAT_FRAGMENT
        case NANO_XRCE_SUBMESSAGEID_FRAGMENT:
        {
            NANO_PCOND(stream != NULL)
            NANO_XRCE_Session_on_fragment(
                self,
                stream,
                msg_hdr,
                &submsg_hdr,
                dptr,
                listener_rc_out,
                has_heartbeat_out);
            break;
        }
#endif /* NANO_FEAT_FRAGMENT */
        default:
        {
            *listener_rc_out =
                NANO_XRCE_SessionListener_on_submsg(
                    &self->listener, self, message,
                    msg_hdr, &submsg_hdr, dptr);
            if (*listener_rc_out != NANO_RETCODE_OK)
            {
                rc = NANO_RETCODE_OK;
                goto done;
            }
            break;
        }
        }

        deserialized_size += submsg_hdr.length;
    }

    if (deserialized_size != data_len)
    {
        NANO_LOG_WARNING("unexpected DESERIALIZED size",
            NANO_LOG_KEY("key", self->key)
            NANO_LOG_SESSIONID("session", self->id)
            NANO_LOG_STREAMID("stream", NANO_XRCE_Stream_id(stream))
            NANO_LOG_USIZE("deserialized", deserialized_size)
            NANO_LOG_USIZE("expected", data_len))
    }

#undef dptr

    rc = NANO_RETCODE_OK;

done:
    NANO_LOG_FN_EXIT_RC(rc);
    return rc;
}

#if !NANO_FEAT_AGENT
NANO_PRIVATE
#endif /* NANO_FEAT_AGENT */
NANO_bool
NANO_XRCE_Session_parse_xrce_message_header(
    NANO_XRCE_Session *const self,
    NANO_MessageBuffer *const msg,
    NANO_XRCE_SessionId *const id_out,
    NANO_XRCE_StreamId *const stream_id_out)
{
    NANO_bool rc = NANO_BOOL_FALSE;
    NANO_XRCE_SessionId sid = NANO_XRCE_SESSIONID_NONE_WITH_CLIENT;
    NANO_XRCE_StreamId stream_id = NANO_XRCE_STREAMID_NONE;
    NANO_bool expect_key = NANO_BOOL_FALSE;
    NANO_XRCE_ClientKey *key = NULL;

    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)

    *id_out = sid;
    *stream_id_out = stream_id;

    if (NANO_XRCE_SessionId_header_size(self->id) >
        NANO_MessageBuffer_data_len(msg))
    {
        /* Message is too short */
        NANO_LOG_TRACE("message too short",
            NANO_LOG_KEY("key", self->key)
            NANO_LOG_SESSIONID("session", self->id)
            NANO_LOG_USIZE("min",
                NANO_XRCE_SessionId_header_size(self->id))
            NANO_LOG_USIZE("msg", NANO_MessageBuffer_data_len(msg)))
        goto done;
    }

    sid = NANO_XRCE_InlineHeaderBuffer_session_id(msg);
    stream_id = NANO_XRCE_InlineHeaderBuffer_stream_id(msg);

    expect_key = NANO_XRCE_SessionId_has_client_key(self->id);

    if (sid != self->id &&
        ((expect_key) ? sid != NANO_XRCE_SESSIONID_NONE_WITH_CLIENT : sid != NANO_XRCE_SESSIONID_NONE_WITHOUT_CLIENT))
    {
        /* Message's session id doesn't match the session's. We might
         * accept this for CREATE_CLIENT/STATUS_AGENT messages which are
         * exchanged using the NONE_WITH[OUT]_CLIENT ids */
        NANO_LOG_TRACE("not for this session",
            NANO_LOG_KEY("key", self->key)
            NANO_LOG_SESSIONID("session", self->id)
            NANO_LOG_SESSIONID("id_msg", sid)
            NANO_LOG_BOOL("need_key",
                NANO_XRCE_SessionId_has_client_key(self->id))
            NANO_LOG_BOOL("has_key",
                    NANO_XRCE_SessionId_has_client_key(sid)))
        goto done;
    }

    if (expect_key)
    {
        key = NANO_XRCE_InlineHeaderBuffer_clientkey_ptr(msg);

        if (NANO_OSAPI_Memory_compare(
                key->value, self->key.value, sizeof(NANO_XRCE_ClientKey)) != 0)
        {
            /* The message includes a key and it doesn't match the session's */
            NANO_LOG_TRACE("invalid key",
                NANO_LOG_KEY("key", self->key)
                NANO_LOG_SESSIONID("session", self->id)
                NANO_LOG_KEY("key_msg", *key))
            goto done;
        }
    }

    *id_out = sid;
    *stream_id_out = stream_id;

    rc = NANO_BOOL_TRUE;

done:

    NANO_LOG_FN_EXIT_BOOL(rc)
    return rc;
}

#if !NANO_FEAT_AGENT
NANO_PRIVATE
#endif /* NANO_FEAT_AGENT */
NANO_RetCode
NANO_XRCE_Session_receive_message(
    NANO_XRCE_Session *const self,
    NANO_XRCE_Stream *const stream,
    NANO_MessageBuffer *const msg,
    NANO_bool *const retained_out)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR,
                 listener_rc = NANO_RETCODE_ERROR;
    NANO_XRCE_SeqNum *sn = NULL;
    NANO_i8 cmp_res = 0;
    NANO_bool deliver = NANO_BOOL_FALSE,
              reliable = NANO_BOOL_FALSE,
              has_hb = NANO_BOOL_FALSE;

    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)
    NANO_PCOND(stream != NULL)
    NANO_PCOND(msg != NULL)
    NANO_PCOND(self->transport != NULL)
    NANO_PCOND(retained_out != NULL)

    NANO_LOG_TRACE("RECV", NANO_LOG_MBUF("msg", msg))

    *retained_out = NANO_BOOL_FALSE;
    reliable = NANO_XRCE_Stream_is_reliable(stream);

#if !NANO_FEAT_RELIABILITY
    if (reliable)
    {
        NANO_LOG_FEAT_MISSING(NANO_FEAT_RELIABILITY)
        goto done;
    }
#endif /* NANO_FEAT_RELIABILITY */

    sn = NANO_XRCE_InlineHeaderBuffer_sn(msg);

    if (NANO_XRCE_Stream_id(stream) == NANO_XRCE_STREAMID_NONE)
    {
        deliver = NANO_BOOL_TRUE;
    }
    else
    {
        NANO_XRCE_SeqNum_compare(&stream->next_in, sn, &cmp_res);

        if (reliable)
        {
            deliver = (cmp_res == 0);
        }
        else
        {
            deliver = (cmp_res <= 0);
        }
    }

    if (!deliver)
    {
        RESEND_LOG("message IGNORED",
            NANO_LOG_KEY("key", self->key)
            NANO_LOG_SESSIONID("session", self->id)
            NANO_LOG_STREAMID("stream", NANO_XRCE_Stream_id(stream))
            NANO_LOG_SN("stream_sn", stream->next_in)
            NANO_LOG_SN("msg_sn", *sn)
            NANO_LOG_I8("cmp_res", cmp_res))
        rc = NANO_RETCODE_OK;
        goto done;
    }

    RECEIVE_LOG("RECEIVED message",
        NANO_LOG_KEY("key", self->key)
        NANO_LOG_SESSIONID("session", self->id)
        NANO_LOG_STREAMID("stream", NANO_XRCE_Stream_id(stream))
        NANO_LOG_MSGHDR("hdr",
            *((NANO_XRCE_MessageHeader *)NANO_MessageBuffer_data_ptr(msg)))
        NANO_LOG_SUBMSGHDR("submsg",
            *((NANO_XRCE_SubmessageHeader *)
                NANO_XRCE_InlineHeaderBuffer_submsgid_ptr(msg)))
        NANO_LOG_MBUF("mbuf", msg))

    rc = NANO_XRCE_Session_deliver_payload(
            self, stream, msg, &listener_rc, &has_hb);
    
    self->recvd_messages += 1;

    NANO_XRCE_SeqNum_add(sn, 1, &stream->next_in);
    {
        /* Skip SN 0x0000 */
        NANO_u16 sn_u = 0;
        sn_u = NANO_XRCE_SeqNum_to_u16(&stream->next_in);
        if (sn_u == 0)
        {
            NANO_XRCE_SeqNum_plusplus(&stream->next_in);
        }
    }

    NANO_LOG_TRACE("DELIVERED message",
        NANO_LOG_KEY("key", self->key)
        NANO_LOG_SESSIONID("session", self->id)
        NANO_LOG_STREAMID("stream", NANO_XRCE_Stream_id(stream))
        NANO_LOG_USIZE("recvd_messages", self->recvd_messages)
        NANO_LOG_SN("msg_sn", *sn)
        NANO_LOG_SN("next_sn", stream->next_in)
        NANO_LOG_RC(rc)
        NANO_LOG_RC(listener_rc))

    rc = NANO_RETCODE_OK;

done:
#if NANO_FEAT_RELIABILITY
    if (has_hb)
    {
        NANO_XRCE_ReliableStream *rel_stream =
            (NANO_XRCE_ReliableStream *)stream;
        NANO_XRCE_HeartbeatPayload piggy_hb =
            NANO_XRCE_HEARTBEATPAYLOAD_INITIALIZER;
        

        if (!rel_stream->in_heartbeat_recvd)
        {
            piggy_hb.first_unacked_seq_num =
                    NANO_XRCE_SeqNum_to_u16(sn);
            piggy_hb.last_unacked_seq_num =
                NANO_XRCE_SeqNum_to_u16(sn);
            piggy_hb.stream_id = NANO_XRCE_Stream_id(stream);

            NANO_XRCE_Session_on_heartbeatI(
                self, rel_stream, &piggy_hb, NANO_BOOL_TRUE);
        }
        else
        {
            NANO_XRCE_SeqNum_compare(
                &rel_stream->in_heartbeat_highest_sn, sn, &cmp_res);
            
            if (cmp_res < 0)
            {
                NANO_i32 sn_distance = 0;
                NANO_XRCE_SeqNum low_sn = NANO_XRCE_SEQNUM_INITIALIZER;

                low_sn = rel_stream->in_heartbeat_lowest_sn;
                
                /* Make sure we are announcing at most 16 samples */
                sn_distance = NANO_XRCE_SeqNum_distance_from(sn, &low_sn);

                if (sn_distance >= NANO_XRCE_ACKNACKBITMAP_BITS)
                {
                    NANO_u16 usn = 0;
                    NANO_XRCE_SeqNum_add(
                        &low_sn,
                        sn_distance -
                            (NANO_XRCE_ACKNACKBITMAP_BITS - 1),
                        &low_sn);
                    usn = NANO_XRCE_SeqNum_to_u16(&low_sn);
                    if (usn == 0)
                    {
                        NANO_XRCE_SeqNum_from_u16(&low_sn, 0xFFFF);
                    }
                }

                piggy_hb.first_unacked_seq_num =
                    NANO_XRCE_SeqNum_to_u16(&low_sn);
                piggy_hb.last_unacked_seq_num =
                    NANO_XRCE_SeqNum_to_u16(sn);

                piggy_hb.stream_id = NANO_XRCE_Stream_id(stream);

                NANO_XRCE_Session_on_heartbeatI(
                    self, rel_stream, &piggy_hb, NANO_BOOL_TRUE);
            }
        }
    }
#endif /* NANO_FEAT_RELIABILITY */
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

NANO_PRIVATE
void
NANO_XRCE_Session_on_message_sent(
    NANO_XRCE_ClientTransportListener *const listener,
    NANO_XRCE_ClientTransport *const transport,
    NANO_MessageBuffer *const msg,
    const NANO_RetCode rc_send)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_XRCE_Session *const self = (NANO_XRCE_Session *)listener->user_data;
    NANO_XRCE_StreamId stream_id = NANO_XRCE_STREAMID_NONE;
    NANO_XRCE_Stream *send_stream = NULL;

    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)

    UNUSED_ARG(transport);

    stream_id = NANO_MessageBuffer_userdata(msg);
    send_stream = NANO_XRCE_Session_lookup_stream(self, stream_id);
    if (send_stream == NULL)
    {
        goto done;
    }

    NANO_MessageBuffer_reset_userdata(msg);
    NANO_MessageBuffer_set_userdata(msg, rc_send);
    NANO_MessageBuffer_flags_unset(
        msg, NANO_XRCE_MESSAGEFLAGS_PENDING_SEND);

    rc = NANO_RETCODE_OK;

done:
    if (NANO_RETCODE_OK != rc)
    {
        /* handle failure */
    }

    NANO_LOG_FN_EXIT_RC(rc)
    return;
}

NANO_PRIVATE
void
NANO_XRCE_Session_on_message_received(
    NANO_XRCE_ClientTransportListener *const listener,
    NANO_XRCE_ClientTransport *const transport,
    NANO_MessageBuffer *const msg,
    NANO_usize msg_len,
    NANO_bool *const retained)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_XRCE_Session *self = NULL;
    NANO_XRCE_Stream *stream = NULL;
    NANO_XRCE_StreamId stream_id = NANO_XRCE_STREAMID_NONE;
    NANO_XRCE_SessionId sid = NANO_XRCE_SESSIONID_NONE_WITHOUT_CLIENT;

    NANO_LOG_FN_ENTRY

    NANO_PCOND(listener != NULL)
    NANO_PCOND(transport != NULL)
    NANO_PCOND(msg != NULL)
    NANO_PCOND(NANO_MessageBuffer_data_ptr(msg) != NULL)
    NANO_PCOND(
        NANO_OSAPI_Memory_aligned(
            NANO_MessageBuffer_data_ptr(msg), sizeof(NANO_u32)))
    NANO_PCOND(msg_len > 0)
    NANO_PCOND(NANO_MessageBuffer_data_len(msg) <= msg_len)
    NANO_PCOND(retained != NULL)

    UNUSED_ARG(transport);
    UNUSED_ARG(msg_len);

    self = (NANO_XRCE_Session *)listener->user_data;
    *retained = NANO_BOOL_FALSE;

    if (!NANO_XRCE_Session_parse_xrce_message_header(
            self, msg, &sid, &stream_id))
    {
        /* ignore invalid messages */
        rc = NANO_RETCODE_OK;
        goto done;
    }

    stream = NANO_XRCE_Session_lookup_stream(self, stream_id);
    if (stream == NULL)
    {
        NANO_LOG_WARNING("UNKNOWN stream for received message",
            NANO_LOG_KEY("session.key", self->key)
            NANO_LOG_SESSIONID("session.id", self->id)
            NANO_LOG_STREAMID("stream", stream_id))
        rc = NANO_RETCODE_OK;
        goto done;
    }

    NANO_CHECK_RC(
        NANO_XRCE_Session_receive_message(self, stream, msg, retained),
        NANO_LOG_WARNING("FAILED to receive message",
            NANO_LOG_RC(rc)
            NANO_LOG_KEY("key", self->key)
            NANO_LOG_SESSIONID("session", self->id)
            NANO_LOG_STREAMID("stream", NANO_XRCE_Stream_id(stream))));

    rc = NANO_RETCODE_OK;

done:

    NANO_LOG_FN_EXIT
    return;
}

#if NANO_FEAT_RELIABILITY && \
    NANO_FEAT_RELIABLESTREAM_RECVQUEUE
NANO_MessageBuffer *
NANO_XRCE_Session_reclaim_message(
    NANO_XRCE_ClientTransportListener *const listener,
    NANO_XRCE_ClientTransport *const transport)
{
    NANO_MessageBuffer *rc = NULL;
    NANO_XRCE_Session *self = NULL;

    NANO_LOG_FN_ENTRY

    UNUSED_ARG(self);
    UNUSED_ARG(transport);

    NANO_PCOND(listener != NULL)

    self = (NANO_XRCE_Session *)listener->user_data;
    NANO_PCOND(self != NULL)

    NANO_LOG_FN_EXIT_PTR(rc)
    return rc;
}
#endif /* NANO_FEAT_RELIABILITY && \
            NANO_FEAT_RELIABLESTREAM_RECVQUEUE */

NANO_RetCode
NANO_XRCE_Session_initialize(
    NANO_XRCE_Session *const self,
    const NANO_XRCE_SessionProperties *const properties)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_bool transport_initialized = NANO_BOOL_FALSE;
    NANO_XRCE_ClientTransportListener transport_listener =
        NANO_XRCE_CLIENTTRANSPORTLISTENER_INITIALIZER;
    NANO_XRCE_SessionId none_id = NANO_XRCE_SESSIONID_NONE_WITH_CLIENT;

    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL && properties != NULL)
    /* Validate that session_id and client_key are consistent */
    NANO_PCOND(
        !NANO_XRCE_SessionId_has_client_key(properties->id) ||
        NANO_XRCE_ClientKey_is_valid(&properties->key))
    /* Validate SessionListener */
    NANO_PCOND(NANO_XRCE_SessionListener_is_valid(&properties->listener))
    NANO_PCOND(properties->id != NANO_XRCE_SESSIONID_NONE_WITH_CLIENT &&
               properties->id != NANO_XRCE_SESSIONID_NONE_WITHOUT_CLIENT)

    self->listener = properties->listener;
    self->key = properties->key;
    self->id = properties->id;

    self->storage = properties->storage;

#if NANO_FEAT_AGENT
    NANO_CHECK_RC(
        NANO_XRCE_Session_initialize_storage(self),
        NANO_LOG_ERROR("FAILED to initialize SESSION STORAGE",
            NANO_LOG_KEY("key", self->key)
            NANO_LOG_SESSIONID("session", self->id)));
#else
    if (self->storage == NULL || self->storage->builtin_be == NULL
#if NANO_FEAT_RELIABILITY
            || self->storage->builtin_rel == NULL
#endif /* NANO_FEAT_RELIABILITY */
        )
    {
        NANO_LOG_ERROR("INVALID session STORAGE",
            NANO_LOG_KEY("key", self->key)
            NANO_LOG_SESSIONID("session", self->id)
            NANO_LOG_PTR("storage", self->storage)
            )
        goto done;
    }
#endif /* NANO_FEAT_AGENT */

    self->transport = properties->transport;

    /* Validate ClientTransport */
    NANO_PCOND(NANO_XRCE_ClientTransport_is_valid(self->transport))

    transport_listener.user_data = self;
    transport_listener.on_message_received =
        NANO_XRCE_Session_on_message_received;
    transport_listener.on_message_sent = NANO_XRCE_Session_on_message_sent;

#if NANO_FEAT_RELIABILITY && \
    NANO_FEAT_RELIABLESTREAM_RECVQUEUE
    transport_listener.reclaim_message = NANO_XRCE_Session_reclaim_message;
#endif /* NANO_FEAT_RELIABILITY && \
            NANO_FEAT_RELIABLESTREAM_RECVQUEUE */

    NANO_LOG_DEBUG("initializing transport",
        NANO_LOG_KEY("key", self->key)
        NANO_LOG_SESSIONID("session", self->id)
        NANO_LOG_PTR("transport", self->transport)
        NANO_LOG_PTR("transport->initialize", self->transport->initialize))

    NANO_CHECK_RC(
        NANO_XRCE_ClientTransport_initialize(
            self->transport, &transport_listener, properties->transport_properties),
        NANO_LOG_ERROR("transport::initialize failed", NANO_LOG_RC(rc)))
    transport_initialized = NANO_BOOL_TRUE;

    NANO_LOG_TRACE("transport INITIALIZED",
        NANO_LOG_KEY("key", self->key)
        NANO_LOG_SESSIONID("session", self->id))

    /* Check if transport's MTU is not too small */
    if (NANO_XRCE_ClientTransport_mtu(self->transport) <
        NANO_XRCE_Session_min_mtu(self))
    {
        NANO_LOG_ERROR("transport MTU too small",
            NANO_LOG_KEY("key", self->key)
            NANO_LOG_SESSIONID("session", self->id)
            NANO_LOG_USIZE("min_mtu", NANO_XRCE_Session_min_mtu(self))
            NANO_LOG_USIZE("mtu",
                NANO_XRCE_ClientTransport_mtu(self->transport)))
        goto done;
    }

    if (NANO_XRCE_SessionId_has_client_key(self->id))
    {
        none_id = NANO_XRCE_SESSIONID_NONE_WITH_CLIENT;
    }
    else
    {
        none_id = NANO_XRCE_SESSIONID_NONE_WITHOUT_CLIENT;
    }
    
    NANO_XRCE_NoneStream_initialize(&self->stream_none, none_id, self->key);
    
    NANO_XRCE_BestEffortStream_initialize(
        &self->stream_builtin_be,
        self->storage->builtin_be,
        self->id,
        self->key,
        NANO_XRCE_STREAMID_BUILTIN_BEST_EFFORTS);

#if NANO_FEAT_RELIABILITY
    NANO_XRCE_ReliableStream_initialize(
        &self->stream_builtin_rel,
        self->storage->builtin_rel,
        self->id,
        self->key,
        NANO_XRCE_STREAMID_BUILTIN_RELIABLE);

#if NANO_FEAT_SYSTIME
#if !NANO_FEAT_AGENT
    if (!(NANO_Time_is_infinite(&properties->acknack_period) ||
          NANO_Time_is_zero(&properties->acknack_period)))
    {
        NANO_Time_to_millis(&properties->acknack_period, &self->period_ack);
    }
#endif /* !NANO_FEAT_AGENT */
    if (!(NANO_Time_is_infinite(&properties->heartbeat_period) ||
          NANO_Time_is_zero(&properties->heartbeat_period)))
    {
        NANO_Time_to_millis(&properties->heartbeat_period, &self->period_hb);
    }
#endif /* NANO_FEAT_SYSTIME */

#endif /* NANO_FEAT_RELIABILITY */

    NANO_LOG_DEBUG("session INITIALIZED",
        NANO_LOG_KEY("key", self->key)
        NANO_LOG_SESSIONID("session", self->id))

    rc = NANO_RETCODE_OK;
done:
    if (NANO_RETCODE_OK != rc)
    {
        if (transport_initialized)
        {
            NANO_XRCE_ClientTransport_finalize(self->transport);
        }
    }

    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

#if NANO_FEAT_FRAGMENT
NANODllExport
    NANO_RetCode
    NANO_XRCE_Session_register_reliable_stream_fragment_storage(
        NANO_XRCE_Session *const self,
        const NANO_XRCE_StreamId stream_id,
        const NANO_MessageBufferType msg_type,
        NANO_u8 *buffer,
        const NANO_usize buffer_size,
        const NANO_usize msg_size)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_XRCE_ReliableStream *stream = NULL;
    NANO_u8 *buffer_val = NULL;

    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)
    NANO_PCOND(NANO_XRCE_StreamId_is_reliable(stream_id))
#if NANO_FEAT_AGENT
    /* When used by the agent BufferPool will allocate its own buffers */
    UNUSED_ARG(buffer);
    NANO_PCOND(buffer == NULL)
    NANO_PCOND(buffer_size == 0)
#else
    NANO_PCOND(buffer != NULL)
    // NANO_PCOND(buffer_size > 0 || msg_size > 0)
    // NANO_PCOND(msg_size <= buffer_size)
#endif /* !NANO_FEAT_AGENT */
    NANO_PCOND(msg_size > 0)
    UNUSED_ARG(buffer_size);

    stream = (NANO_XRCE_ReliableStream *)
        NANO_XRCE_Session_lookup_stream(self, stream_id);
    if (stream == NULL)
    {
        NANO_LOG_ERROR("UNKNOWN stream",
            NANO_LOG_KEY("key", self->key)
            NANO_LOG_SESSIONID("session", self->id)
            NANO_LOG_STREAMID("stream", stream_id))
        rc = NANO_RETCODE_INVALID_ARGS;
        goto done;
    }

    switch (msg_type)
    {
    case NANO_XRCE_MESSAGETYPE_FRAGMENT_RECV_BUFFER:
    {
        // NANO_PCOND(buffer_size == msg_size)
#if NANO_FEAT_AGENT
        NANO_usize alloc_size = 0;
        char *rti_ptr = NULL;

        alloc_size =
            NANO_OSAPI_Memory_align_size_up(
                msg_size, sizeof(NANO_MessageBufferData));
        /* Allocate a buffer of requested size */
        RTIOsapiHeap_allocateBufferAligned(
            &rti_ptr,
            alloc_size,
            sizeof(NANO_MessageBufferData));
        buffer_val = (NANO_u8*)rti_ptr;
        if (buffer_val == NULL)
        {
            NANO_LOG_ERROR("FAILED to allocate frag_in buffer",
                NANO_LOG_KEY("key", self->key)
                NANO_LOG_SESSIONID("session", self->id)
                NANO_LOG_STREAMID("stream", stream_id)
                NANO_LOG_USIZE("size", alloc_size)
                NANO_LOG_USIZE("alignment", sizeof(NANO_MessageBufferData)))
            goto done;
        }

        NANO_LOG_DEBUG("ALLOC'D FRAG_IN buffer",
            NANO_LOG_KEY("key", self->key)
            NANO_LOG_SESSIONID("session", self->id)
            NANO_LOG_STREAMID("stream", stream_id)
            NANO_LOG_PTR("buffer", buffer_val))
#else
        buffer_val = buffer;
#endif /* NANO_FEAT_AGENT */

        NANO_MessageBuffer_set_type(
            &stream->recv_frag_buffer,
            NANO_XRCE_MESSAGETYPE_FRAGMENT_RECV_BUFFER);
        NANO_MessageBuffer_set_external_data(
            &stream->recv_frag_buffer, buffer_val, 0);

        stream->recv_frag_buffer_size = msg_size;

        NANO_LOG_DEBUG("FRAG_IN buffer SET",
            NANO_LOG_KEY("key", self->key)
            NANO_LOG_SESSIONID("session", self->id)
            NANO_LOG_STREAMID("stream", stream_id)
            NANO_LOG_USIZE("buffer_size", stream->recv_frag_buffer_size)
            NANO_LOG_MBUF("buffer", &stream->recv_frag_buffer))

        break;
    }
    default:
        break;
    }

    rc = NANO_RETCODE_OK;

done:
#if NANO_FEAT_AGENT
    if (NANO_RETCODE_OK != rc && buffer_val != NULL)
    {
        RTIOsapiHeap_freeBuffer(buffer_val);
    }
#endif /* NANO_FEAT_AGENT */

    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}
#endif /* NANO_FEAT_FRAGMENT */

NANO_XRCE_Stream *
NANO_XRCE_Session_lookup_stream(
    NANO_XRCE_Session *const self,
    const NANO_XRCE_StreamId stream_id)
{
    NANO_XRCE_Stream *res = NULL;

    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)

    switch (stream_id)
    {
    case NANO_XRCE_STREAMID_NONE:
    {
        res = &self->stream_none.base;
        break;
    }
    case NANO_XRCE_STREAMID_BUILTIN_BEST_EFFORTS:
    {
        res = &self->stream_builtin_be.base;
        break;
    }
#if NANO_FEAT_RELIABILITY
    case NANO_XRCE_STREAMID_BUILTIN_RELIABLE:
    {
        res = &self->stream_builtin_rel.base;
        break;
    }
#endif /* NANO_FEAT_RELIABILITY */
    default:
    {
#if NANO_FEAT_SESSION_USER_STREAMS_BESTEFFORT || \
    NANO_FEAT_SESSION_USER_STREAMS_RELIABLE
        NANO_usize i = 0;

        if (NANO_XRCE_StreamId_is_reliable(stream_id))
        {
#if NANO_FEAT_RELIABILITY && \
    NANO_FEAT_SESSION_USER_STREAMS_RELIABLE
            for (i = 0;
                 self->storage != NULL &&
                 i < self->storage->streams->user_streams_rel_len;
                 i++)
            {
                res = &(self->storage->streams->user_streams_rel[i].base);
                if (NANO_XRCE_Stream_id(res) == stream_id)
                {
                    break;
                }
                res = NULL;
            }
#else
            goto done;
#endif /* NANO_FEAT_SESSION_USER_STREAMS_RELIABLE */
        }
        else
        {
#if NANO_FEAT_SESSION_USER_STREAMS_BESTEFFORT
            for (i = 0;
                 self->storage != NULL &&
                 i < self->storage->streams->user_streams_be_len;
                 i++)
            {
                res = &(self->storage->streams->user_streams_be[i].base);
                if (NANO_XRCE_Stream_id(res) == stream_id)
                {
                    goto done;
                }
            }
            res = NULL;
#else
            goto done;
#endif /* NANO_FEAT_SESSION_USER_STREAMS_BESTEFFORT */
        }
#else
        goto done;
#endif /* NANO_FEAT_SESSION_USER_STREAMS_BESTEFFORT || \
            NANO_FEAT_SESSION_USER_STREAMS_RELIABLE */
    }
    }

done:

    NANO_LOG_FN_EXIT_PTR(res)
    return res;
}

#if NANO_FEAT_SESSION_USER_STREAMS_BESTEFFORT || \
    NANO_FEAT_SESSION_USER_STREAMS_RELIABLE
NANO_RetCode
NANO_XRCE_Session_register_stream(
    NANO_XRCE_Session *const self,
    const NANO_XRCE_StreamId stream_id,
    NANO_XRCE_StreamStorage *const stream_storage)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_XRCE_Stream *stream = NULL;
    NANO_usize stream_i = 0,
               mv_len = 0;

    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)

    if (self->storage->streams == NULL)
    {
        NANO_LOG_ERROR("no session storage provided",
            NANO_LOG_KEY("key", self->key)
            NANO_LOG_SESSIONID("session", self->id)
            NANO_LOG_STREAMID("stream", stream_id))
        goto done;
    }

    stream = NANO_XRCE_Session_lookup_stream(self, stream_id);
    if (stream != NULL)
    {
        NANO_LOG_ERROR("stream ALREADY REGISTERED",
            NANO_LOG_KEY("session.key", self->key)
            NANO_LOG_SESSIONID("session.id", self->id)
            NANO_LOG_STREAMID("stream_id", stream_id))
        goto done;
    }

    if (NANO_XRCE_StreamId_is_reliable(stream_id))
    {
#if !NANO_FEAT_RELIABILITY
        NANO_LOG_FEAT_MISSING(NANO_FEAT_RELIABILITY)
        goto done;
#elif !NANO_FEAT_SESSION_USER_STREAMS_RELIABLE
        NANO_LOG_FEAT_MISSING(NANO_FEAT_SESSION_USER_STREAMS_RELIABLE)
        goto done;
#else

        if (self->storage->streams->user_streams_rel_len ==
            self->storage->streams->user_streams_rel_max)
        {
            NANO_LOG_ERROR("too many user-defined reliable streams",
                NANO_LOG_KEY("session.key", self->key)
                NANO_LOG_SESSIONID("session.id", self->id)
                NANO_LOG_USIZE("max", self->storage->streams->user_streams_rel_max))

            rc = NANO_RETCODE_OUT_OF_RESOURCES;
            goto done;
        }

        /* Find a reliable stream that isn't is use */
        for (stream_i = 0;
             stream_i < self->storage->streams->user_streams_rel_len;
             stream_i++)
        {
            stream = &(self->storage->streams->user_streams_rel[stream_i].base);
            if (NANO_XRCE_Stream_id(stream) >= stream_id)
            {
                break;
            }
        }
        stream = &(self->storage->streams->user_streams_rel[stream_i].base);

        mv_len = self->storage->streams->user_streams_rel_len - stream_i;

        if (mv_len > 0)
        {
            NANO_OSAPI_Memory_move(
                (NANO_u8 *)(self->storage->streams->user_streams_rel + stream_i + 1),
                (NANO_u8 *)(self->storage->streams->user_streams_rel + stream_i),
                sizeof(NANO_XRCE_ReliableStream) * mv_len);
        }

        NANO_XRCE_ReliableStream_initialize(
            ((NANO_XRCE_ReliableStream *)stream),
            stream_storage,
            self->id,
            self->key,
            stream_id);

        self->storage->streams->user_streams_rel_len += 1;
#endif /* NANO_FEAT_RELIABILITY */
    }
    else
    {
#if !NANO_FEAT_SESSION_USER_STREAMS_BESTEFFORT
        NANO_LOG_FEAT_MISSING(NANO_FEAT_SESSION_USER_STREAMS_BESTEFFORT)
        goto done;
#else
        if (self->storage->streams->user_streams_be_len ==
            self->storage->streams->user_streams_be_max)
        {
            NANO_LOG_ERROR("too many user-defined best-effort streams",
                NANO_LOG_KEY("session.key", self->key)
                NANO_LOG_SESSIONID("session.id", self->id)
                NANO_LOG_USIZE("max", self->storage->streams->user_streams_be_max))
            rc = NANO_RETCODE_OUT_OF_RESOURCES;
            goto done;
        }

        for (stream_i = 0; stream_i < self->storage->streams->user_streams_be_len; stream_i++)
        {
            stream = &(self->storage->streams->user_streams_be[stream_i].base);
            if (NANO_XRCE_Stream_id(stream) >= stream_id)
            {
                break;
            }
        }
        stream = &(self->storage->streams->user_streams_be[stream_i].base);

        mv_len = self->storage->streams->user_streams_be_len - stream_i;
        if (mv_len > 0)
        {
            NANO_OSAPI_Memory_move(
                (NANO_u8 *)(self->storage->streams->user_streams_be + stream_i + 1),
                (NANO_u8 *)(self->storage->streams->user_streams_be + stream_i),
                sizeof(NANO_XRCE_BestEffortStream) * mv_len);
        }

        NANO_PCOND(stream != NULL)

        NANO_XRCE_BestEffortStream_initialize(
            ((NANO_XRCE_BestEffortStream *)stream),
            stream_storage,
            self->id,
            self->key,
            stream_id);

        self->storage->streams->user_streams_be_len += 1;
#endif
    }

    NANO_LOG_INFO("user stream REGISTERED",
        NANO_LOG_KEY("session.key", self->key)
        NANO_LOG_SESSIONID("session.id", self->id)
        NANO_LOG_STREAMID("stream_id", stream_id)
        NANO_LOG_PTR("stream", stream))

    rc = NANO_RETCODE_OK;

done:
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}
#endif /* NANO_FEAT_SESSION_USER_STREAMS_BESTEFFORT || \
            NANO_FEAT_SESSION_USER_STREAMS_RELIABLE */

#if NANO_FEAT_FRAGMENT
NANO_RetCode
NANO_XRCE_Session_send_fragments(
    NANO_XRCE_Session *const self,
    NANO_XRCE_ReliableStream *stream,
    const NANO_XRCE_SubmessageId submsg_id,
    const NANO_XRCE_SubmessageFlags submsg_flags,
    NANO_MessageBuffer *const payload,
    const NANO_usize payload_len,
    const NANO_usize mtu,
    NANO_XRCE_SeqNum *const last_sn_out)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_usize tot_frags = 0,
               tot_bytes = 0,
               frag_size = 0,
               frag_i = 0,
               cur_mbuf_max = 0,
               cur_mbuf_remaining = 0,
               cur_frag_max = 0,
               cur_frag_remaining = 0,
               sent_frags = 0,
               allocd_frags = 0,
               allocd_headers = 0,
               tot_input_mbufs = 0,
               tot_mbufs = 0;
    NANO_MessageBuffer *frag = NULL,
                       *cur_mbuf = NULL,
                       *next_mbuf = NULL,
                       *next_frag_out = NULL,
                       *first_frag_mbuf = NULL,
                       *frag_pool = NULL,
                       *frag_pool_last = NULL,
                       *header = NULL,
                       *input_mbufs = NULL;
    NANO_u8 *submsg_hdr_ptr = NULL;
    NANO_XRCE_SeqNum last_sn = NANO_XRCE_SEQNUM_INITIALIZER,
                     last_sent_sn = NANO_XRCE_SEQNUM_INITIALIZER;

    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)
    NANO_PCOND(stream != NULL)
    NANO_PCOND(submsg_id != NANO_XRCE_SUBMESSAGEID_FRAGMENT)
    NANO_PCOND(payload != NULL)
    NANO_PCOND(payload_len > 0)
    NANO_PCOND(mtu > 0)
    NANO_PCOND(payload_len > mtu - NANO_XRCE_SessionId_header_size(self->id))
    NANO_PCOND(last_sn_out != NULL)

    UNUSED_ARG(mtu);

    input_mbufs = payload;

    *last_sn_out = last_sn;

    frag_size =
        NANO_XRCE_RELIABLESTREAM_MAX_FRAGMENT_PAYLOAD_SIZE_BYTES(self->id, mtu);

    tot_bytes = payload_len + NANO_XRCE_SUBMESSAGEHEADER_SERIALIZED_SIZE_MAX;

    tot_frags = tot_bytes / frag_size;
    if (tot_bytes % frag_size)
    {
        tot_frags += 1;
    }

    /* Allocate an INLINE_PAYLOAD message to store submessage header */
    first_frag_mbuf = NANO_XRCE_Session_allocate_message(
                        self,
                        &stream->base,
                        NANO_XRCE_MESSAGETYPE_INLINE_PAYLOAD,
                        NANO_XRCE_SUBMESSAGEHEADER_SERIALIZED_SIZE_MAX,
                        NULL);
    if (first_frag_mbuf == NULL)
    {
        NANO_LOG_ERROR("FAILED to allocate payload header for fragments",
            NANO_LOG_KEY("key", self->key)
            NANO_LOG_SESSIONID("session", self->id)
            NANO_LOG_STREAMID("stream", NANO_XRCE_Stream_id(&stream->base)))
        goto done;
    }

    submsg_hdr_ptr = NANO_MessageBuffer_data_ptr(first_frag_mbuf);

    /* prepare submessage header */
    submsg_hdr_ptr[0] = submsg_id;
    submsg_hdr_ptr[1] = submsg_flags;
    NANO_u16_serialize(payload_len, submsg_hdr_ptr + 2, NANO_BOOL_TRUE);
    NANO_MessageBuffer_set_data_len(
        first_frag_mbuf, sizeof(NANO_XRCE_SubmessageHeader));

    cur_mbuf = payload;
    cur_mbuf_max = NANO_MessageBuffer_data_len(cur_mbuf);
    cur_mbuf_remaining = NANO_MessageBuffer_data_len(cur_mbuf);
    cur_frag_max = frag_size - NANO_XRCE_SUBMESSAGEHEADER_SERIALIZED_SIZE_MAX;
    cur_frag_remaining = cur_frag_max;
    next_frag_out = first_frag_mbuf;
    frag = NULL;
    
    while (cur_mbuf != NULL)
    {
        if (frag == NULL)
        {
            frag = NANO_XRCE_Session_allocate_message(
                        self,
                        &stream->base,
                        NANO_XRCE_MESSAGETYPE_EXTERNAL_PAYLOAD,
                        /* we set each frag mbuf with a dummy payload, 
                            and we will set the appropriate one later */
                        frag_size, (NANO_u8*)&frag);
            if (frag == NULL)
            {
                NANO_LOG_ERROR("FAILED to allocate mbuf for fragment",
                    NANO_LOG_KEY("key", self->key)
                    NANO_LOG_SESSIONID("session", self->id)
                    NANO_LOG_STREAMID("stream",
                        NANO_XRCE_Stream_id(&stream->base)))
                goto done;
            }
            allocd_frags += 1;
        }

        next_mbuf = NANO_MessageBuffer_next(cur_mbuf);

        /* Check if the remaining payload in the current mbuf fits in the
            remaining space in this fragment */
        if (cur_mbuf_remaining <= cur_frag_remaining)
        {
            cur_frag_remaining -= cur_mbuf_remaining;
            /* Unlink cur_mbuf since we're either going to append it to
              current frag chain or we will return it to the pool */
            NANO_MessageBuffer_unlink(cur_mbuf);
            
            /* If this is the beginning of the current mbuf */
            if (cur_mbuf_remaining == cur_mbuf_max)
            {
                if (next_frag_out == NULL)
                {
                    next_frag_out = cur_mbuf;
                }
                else
                {
                    NANO_MessageBuffer *last_mbuf = next_frag_out;
                    while (NANO_MessageBuffer_next(last_mbuf) != NULL)
                    {
                        last_mbuf = NANO_MessageBuffer_next(last_mbuf);
                    }
                    NANO_MessageBuffer_append(last_mbuf, cur_mbuf);
                }
            }
            else
            {
                NANO_MessageBuffer_set_external_data(frag,
                    NANO_MessageBuffer_data_ptr(cur_mbuf) + 
                        (cur_mbuf_max - cur_mbuf_remaining),
                    cur_mbuf_remaining);
                
                if (next_frag_out == NULL)
                {
                    next_frag_out = frag;
                }
                else
                {
                    NANO_MessageBuffer *last_mbuf = next_frag_out;
                    while (NANO_MessageBuffer_next(last_mbuf) != NULL)
                    {
                        last_mbuf = NANO_MessageBuffer_next(last_mbuf);
                    }
                    NANO_MessageBuffer_append(last_mbuf, frag);
                }
                frag = NULL;

                NANO_XRCE_Session_release_mbuf_chain(
                    self, &stream->base, cur_mbuf);
            }

            cur_mbuf_remaining = 0;
            cur_mbuf = next_mbuf;
            input_mbufs = next_mbuf;
            tot_input_mbufs += 1;
            tot_mbufs += 1;

            if (cur_mbuf != NULL)
            {
                cur_mbuf_max = NANO_MessageBuffer_data_len(cur_mbuf);
                cur_mbuf_remaining = cur_mbuf_max;
            }
            else if (frag != NULL)
            {
                /* Release unused fragment */
                NANO_XRCE_Session_release_mbuf_chain(
                    self, &stream->base, frag);
                frag = NULL;
            }
        }
        else
        {
            NANO_MessageBuffer_set_external_data(frag,
                NANO_MessageBuffer_data_ptr(cur_mbuf) + 
                    (cur_mbuf_max - cur_mbuf_remaining),
                cur_frag_remaining);
            cur_mbuf_remaining -= cur_frag_remaining;
            cur_frag_remaining = 0;
            if (next_frag_out == NULL)
            {
                next_frag_out = frag;
            }
            else
            {
                NANO_MessageBuffer *last_mbuf = next_frag_out;
                while (NANO_MessageBuffer_next(last_mbuf) != NULL)
                {
                    last_mbuf = NANO_MessageBuffer_next(last_mbuf);
                }
                NANO_MessageBuffer_append(last_mbuf, frag);
            }
            frag = NULL;
            tot_mbufs += 1;
        }

        if (cur_frag_remaining == 0 || frag == NULL)
        {
            /* Allocate a header buffer */
            header = NANO_XRCE_Session_allocate_message(
                        self,
                        &stream->base,
                        NANO_XRCE_MESSAGETYPE_INLINE_HEADER, 
                        NANO_XRCE_SessionId_header_size(self->id),
                        NULL);
            if (header == NULL)
            {
                NANO_LOG_ERROR("FAILED to allocate header mbuf for fragment",
                    NANO_LOG_KEY("key", self->key)
                    NANO_LOG_SESSIONID("session", self->id)
                    NANO_LOG_STREAMID("stream",
                        NANO_XRCE_Stream_id(&stream->base)))
                goto done;
            }
            allocd_headers += 1;

            NANO_MessageBuffer_append(header, next_frag_out);

            if (frag_pool != NULL)
            {
                NANO_PCOND(frag_pool_last != NULL)
                NANO_MessageBuffer_append_msg(frag_pool_last, header);
            }
            else
            {
                frag_pool = header;
            }
            frag_pool_last = header;
            
            next_frag_out = NULL;
            header = NULL;
            cur_frag_max = frag_size;
            cur_frag_remaining = cur_frag_max;

        }
    }

    NANO_PCOND(cur_mbuf == NULL)
    NANO_PCOND(next_frag_out == NULL)
    NANO_PCOND(frag == NULL)

    frag_i = 0;
    while (frag_pool != NULL)
    {
        NANO_bool final_frag = NANO_BOOL_FALSE;
        next_mbuf = NANO_MessageBuffer_next_msg(frag_pool);
        NANO_MessageBuffer_unlink_msg(frag_pool);
        final_frag = (next_mbuf == NULL);

        if (frag_i == 0)
        {
            NANO_XRCE_Session_send_heartbeat(self, stream, NANO_BOOL_TRUE);
        }

        frag_i += 1;
        NANO_LOG_DEBUG("send FRAGMENT",
            NANO_LOG_KEY("key", self->key)
            NANO_LOG_SESSIONID("session", self->id)
            NANO_LOG_STREAMID("stream", NANO_XRCE_Stream_id(&stream->base))
            NANO_LOG_USIZE("frag_tot", tot_frags)
            NANO_LOG_USIZE("frag_i", frag_i)
            NANO_LOG_BOOL("frag_last", final_frag)
            NANO_LOG_H32("mbuf_flags", frag_pool->hdr.flags))

        NANO_CHECK_RC(
            NANO_XRCE_Session_send(
                self,
                &stream->base,
                NANO_XRCE_SUBMESSAGEID_FRAGMENT,
                NANO_XRCE_SUBMESSAGEFLAGS_DEFAULT |
                ((final_frag) ? NANO_XRCE_SUBMESSAGEFLAGS_FRAGMENT_LAST : 0) |
                ((sent_frags == 0) ? NANO_XRCE_SUBMESSAGEFLAGS_FRAGMENT_FIRST : 0),
                frag_pool,
                &last_sn),
            frag_pool = next_mbuf;
            NANO_LOG_ERROR("FAILED to send FRAGMENT",
                NANO_LOG_KEY("key", self->key)
                NANO_LOG_SESSIONID("session", self->id)
                NANO_LOG_STREAMID("stream",
                    NANO_XRCE_Stream_id(&stream->base))));
        last_sent_sn = last_sn;
        sent_frags += 1;
        frag_pool = next_mbuf;
    }
    *last_sn_out = last_sent_sn;

    rc = NANO_RETCODE_OK;
done:
    if (NANO_RETCODE_OK != rc)
    {
        NANO_usize dismissed = 0;
        NANO_LOG_ERROR("send_fragments FAILED",
            NANO_LOG_KEY("key", self->key)
            NANO_LOG_SESSIONID("session", self->id)
            NANO_LOG_STREAMID("stream",
                NANO_XRCE_Stream_id(&stream->base))
            NANO_LOG_PTR("next_frag_out", next_frag_out)
            NANO_LOG_PTR("frag", frag)
            NANO_LOG_PTR("frag_pool", frag_pool)
            NANO_LOG_PTR("input_mbufs", input_mbufs)
            NANO_LOG_USIZE("tot_frags", tot_frags)
            NANO_LOG_USIZE("allocd_frags", allocd_frags)
            NANO_LOG_USIZE("allocd_header", allocd_headers)
            NANO_LOG_USIZE("tot_input_mbufs", tot_input_mbufs)
            NANO_LOG_USIZE("tot_mbufs", tot_mbufs)
            NANO_LOG_USIZE("sent_frags", sent_frags)
            NANO_LOG_SN("last_sent_sn", last_sent_sn)
            NANO_LOG_USIZE("send_queue.len",
                NANO_MessageBufferQueue_size(&stream->send_queue))
            if (NANO_MessageBufferQueue_size(&stream->send_queue) > 0)
            {
                NANO_LOG_SN("send_queue.head",
                    *NANO_XRCE_InlineHeaderBuffer_sn(
                        NANO_MessageBufferQueue_head(&stream->send_queue)))
                NANO_LOG_SN("send_queue.tail",
                    *NANO_XRCE_InlineHeaderBuffer_sn(
                        NANO_MessageBufferQueue_tail(&stream->send_queue)))
            } 
        )

        if (next_frag_out != NULL)
        {
            NANO_XRCE_Session_release_mbuf_chain(
                self, &stream->base, next_frag_out);
            dismissed += 1;
        }
        if (frag != NULL)
        {
            NANO_XRCE_Session_release_mbuf_chain(self, &stream->base, frag);
        }
        if (frag_pool != NULL)
        {
            while (frag_pool != NULL)
            {
                frag = frag_pool;
                frag_pool = NANO_MessageBuffer_next_msg(frag);
                NANO_MessageBuffer_unlink_msg(frag);
                NANO_XRCE_Session_release_mbuf_chain(
                    self, &stream->base, frag);
                dismissed += 1;
            }
        }
        if (input_mbufs != NULL)
        {
            NANO_XRCE_Session_release_mbuf_chain(
                self, &stream->base, input_mbufs);
            dismissed += 1;
        }
        if (sent_frags > 0)
        {
            dismissed +=
                NANO_XRCE_Session_dismiss_send_queue_up_to(
                    self, stream, &last_sent_sn, NANO_BOOL_TRUE /* confirmed */);
            if (NANO_RETCODE_OK !=
                    NANO_XRCE_Session_send_heartbeat(self, stream, NANO_BOOL_TRUE))
            {
                NANO_LOG_ERROR("FAILED to send HEARTBEAT",
                    NANO_LOG_KEY("key", self->key)
                    NANO_LOG_SESSIONID("session", self->id)
                    NANO_LOG_STREAMID("stream",
                        NANO_XRCE_Stream_id(&stream->base)))
            }
        }

        NANO_LOG_ERROR("send_fragments FAILED (RELEASED)",
            NANO_LOG_KEY("key", self->key)
            NANO_LOG_SESSIONID("session", self->id)
            NANO_LOG_STREAMID("stream",
                NANO_XRCE_Stream_id(&stream->base))
            NANO_LOG_USIZE("send_queue.len",
                NANO_MessageBufferQueue_size(&stream->send_queue))
            if (NANO_MessageBufferQueue_size(&stream->send_queue) > 0)
            {
                NANO_LOG_SN("send_queue.head",
                    *NANO_XRCE_InlineHeaderBuffer_sn(
                        NANO_MessageBufferQueue_head(&stream->send_queue)))
                NANO_LOG_SN("send_queue.tail",
                    *NANO_XRCE_InlineHeaderBuffer_sn(
                        NANO_MessageBufferQueue_tail(&stream->send_queue)))
            } 
        )
    }

    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}
#endif /* NANO_FEAT_FRAGMENT */

NANO_RetCode
NANO_XRCE_Session_send(
    NANO_XRCE_Session *const self,
    NANO_XRCE_Stream *stream,
    const NANO_XRCE_SubmessageId submsg_id,
    const NANO_XRCE_SubmessageFlags submsg_flags,
    NANO_MessageBuffer *const payload,
    NANO_XRCE_SeqNum *const sn_out)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_XRCE_InlineHeaderBuffer *header = NULL;
    NANO_MessageBuffer *payload_mbuf = NULL;
    NANO_XRCE_StreamId stream_id = NANO_XRCE_STREAMID_NONE;
    NANO_XRCE_SeqNum last_sn = NANO_XRCE_SEQNUM_INITIALIZER;
    NANO_usize payload_len = 0,
               max_payload = 0;
#if NANO_FEAT_RELIABILITY
    NANO_XRCE_ReliableStream *rel_stream = (NANO_XRCE_ReliableStream *)stream;
    NANO_bool queued_payload = NANO_BOOL_FALSE;
#endif
    NANO_bool release_payload = NANO_BOOL_TRUE,
              large_payload = NANO_BOOL_FALSE,
              reliable_stream = NANO_BOOL_FALSE,
              header_payload = NANO_BOOL_FALSE;
    NANO_XRCE_SubmessageFlags send_flags = submsg_flags;

    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)
    NANO_PCOND(NANO_XRCE_SubmessageId_is_valid(submsg_id))
    NANO_PCOND(payload != NULL)
    NANO_PCOND(stream != NULL)

    /* Only send payloads that have been allocated by the session.
       This way we can "take ownership" back, and release them within
       this function (whether they've been sent successfully or not).
       Acknacks and Heartbeat payloads are allocated on the stack and
       they are not expected to be owned by the stream.
     */
    NANO_PCOND(((submsg_id == NANO_XRCE_SUBMESSAGEID_ACKNACK ||
            submsg_id == NANO_XRCE_SUBMESSAGEID_HEARTBEAT) &&
        !NANO_MessageBuffer_flags(
                   payload, NANO_XRCE_MESSAGEFLAGS_OWNED_BY_STREAM)) ||
        NANO_MessageBuffer_flags(
                   payload, NANO_XRCE_MESSAGEFLAGS_OWNED_BY_STREAM))
    
    if (sn_out != NULL)
    {
        *sn_out = last_sn;
    }

    release_payload =
        NANO_MessageBuffer_flags(
            payload, NANO_XRCE_MESSAGEFLAGS_OWNED_BY_STREAM);

    stream_id = NANO_XRCE_Stream_id(stream);
    
    reliable_stream = NANO_XRCE_StreamId_is_reliable(stream_id);

    header_payload = 
        (NANO_MessageBuffer_type(payload) ==
            NANO_XRCE_MESSAGETYPE_INLINE_HEADER);

#if !NANO_FEAT_RELIABILITY
    if (reliable_stream)
    {
        NANO_LOG_FEAT_MISSING(NANO_FEAT_RELIABILITY)
        goto done;
    }
#endif /* NANO_FEAT_RELIABILITY */

    NANO_LOG_TRACE("send MESSAGE",
        NANO_LOG_KEY("key", self->key)
        NANO_LOG_SESSIONID("session", self->id)
        NANO_LOG_STREAMID("stream", NANO_XRCE_Stream_id(stream))
        NANO_LOG_H8("submsg.id", submsg_id)
        NANO_LOG_H8("submsg.flags", submsg_flags)
        NANO_LOG_BOOL("little_endian",
            NANO_XRCE_SubmessageFlags_is_little_endian(submsg_flags))
        if (submsg_id == NANO_XRCE_SUBMESSAGEID_DATA ||
            submsg_id == NANO_XRCE_SUBMESSAGEID_WRITE_DATA)
        {
            NANO_LOG_H8("data_format",
                NANO_XRCE_SubmessageFlags_DATA_format(submsg_flags))
        }
        if (submsg_id == NANO_XRCE_SUBMESSAGEID_WRITE_DATA)
        {
            NANO_LOG_BOOL("confirm",
                NANO_XRCE_SubmessageFlags_WRITEDATA_confirm(submsg_flags))
        }
        if (submsg_id == NANO_XRCE_SUBMESSAGEID_READ_DATA)
        {
            NANO_LOG_BOOL("confirm",
                NANO_XRCE_SubmessageFlags_READDATA_confirm(submsg_flags))
        }
        if (submsg_id == NANO_XRCE_SUBMESSAGEID_CREATE)
        {
            NANO_LOG_BOOL("reuse",
                NANO_XRCE_SubmessageFlags_CREATE_reuse(submsg_flags))
            NANO_LOG_BOOL("replace",
                NANO_XRCE_SubmessageFlags_CREATE_replace(submsg_flags))
        }
        NANO_LOG_USIZE("payload_len", payload_len))


    /* Check if the message fits in the transport MTU */
    NANO_MessageBuffer_data_len_msg(payload, &payload_len);
    max_payload = 
        NANO_XRCE_ClientTransport_mtu(self->transport) -
            NANO_XRCE_SessionId_header_size(self->id);
    large_payload = (payload_len > max_payload) &&
        !(header_payload &&
            payload_len <= NANO_XRCE_ClientTransport_mtu(self->transport));

    if (large_payload)
    {
        if (header_payload)
        {
            /* Large payloads should not preallocate a header buffer,
               and instead let send_fragments() "do its thing" */
            NANO_LOG_ERROR("UNEXPECTED large header payload",
                NANO_LOG_KEY("key", self->key)
                NANO_LOG_SESSIONID("session", self->id)
                NANO_LOG_STREAMID("stream", stream_id)
                NANO_LOG_USIZE("payload_len", payload_len)
                NANO_LOG_USIZE("max_payload", max_payload))
            goto done;
        }

        if (reliable_stream)
        {
#if NANO_FEAT_FRAGMENT
            SEND_LOG("must FRAGMENT payload",
                NANO_LOG_KEY("key", self->key)
                NANO_LOG_SESSIONID("session", self->id)
                NANO_LOG_STREAMID("stream", stream_id)
                NANO_LOG_USIZE("payload_len", payload_len)
                NANO_LOG_USIZE("mtu",
                    NANO_XRCE_ClientTransport_mtu(self->transport))
                NANO_LOG_USIZE("max_unfragd", max_payload))

            /* Total length of message (payload + msg hdr + submsg hdr)
               exceeds the transport MTU so we must split the payload into
               fragments */
            /* message will be released by send_fragments() */
            release_payload = NANO_BOOL_FALSE;
            rc = NANO_XRCE_Session_send_fragments(
                    self,
                    rel_stream,
                    submsg_id,
                    submsg_flags,
                    payload,
                    payload_len,
                    NANO_XRCE_ClientTransport_mtu(self->transport),
                    &last_sn);
            if (rc != NANO_RETCODE_OK)
            {
                NANO_LOG_ERROR("FAILED to send as fragmented payload",
                    NANO_LOG_KEY("key", self->key)
                    NANO_LOG_SESSIONID("session", self->id)
                    NANO_LOG_STREAMID("stream", stream_id))
            }
            if (sn_out != NULL)
            {
                *sn_out = last_sn;
            }
            goto done;

#else
            NANO_LOG_FEAT_MISSING(NANO_FEAT_FRAGMENT)
            goto done;
#endif /* NANO_FEAT_FRAGMENT */
        }
        else
        {
            NANO_LOG_ERROR("large payloads not allowd on best-effort streams",
                NANO_LOG_KEY("key", self->key)
                NANO_LOG_SESSIONID("session", self->id)
                NANO_LOG_STREAMID("stream", stream_id)
                NANO_LOG_USIZE("payload_len", payload_len)
                NANO_LOG_USIZE("mtu",
                    NANO_XRCE_ClientTransport_mtu(self->transport))
                NANO_LOG_USIZE("max_unfragd", max_payload))
            goto done;
        }
    }

    if (header_payload)
    {
        header = payload;
        payload_mbuf = NANO_MessageBuffer_next(header);
    }
    else
    {
        payload_mbuf = payload;

        header = NANO_XRCE_Session_allocate_message(
            self,
            stream,
            NANO_XRCE_MESSAGETYPE_INLINE_HEADER,
            NANO_XRCE_SessionId_header_size(self->id),
            NULL);
        
        if (header == NULL)
        {
            NANO_LOG_ERROR("FAILED to allocate HEADER buffer",
                NANO_LOG_KEY("key", self->key)
                NANO_LOG_SESSIONID("session", self->id)
                NANO_LOG_STREAMID("stream", stream_id))
            rc = NANO_RETCODE_OUT_OF_RESOURCES;
            goto done;
        }
    }

    NANO_XRCE_InlineHeaderBuffer_initialize(
        header,
        NANO_XRCE_Stream_session_id(stream),
        self->key,
        NANO_XRCE_Stream_id(stream));

    if (reliable_stream)
    {
        /* Since we only send one submsg at a time, we always imply
           an heartbeat with every message */
        send_flags |= NANO_XRCE_SUBMESSAGEFLAGS_PIGGYBACKHB;
    }

    /* Link payload to header, update header's buffer with submessage
     * information. */
    NANO_XRCE_InlineHeaderBuffer_update_next(
        header, submsg_id, send_flags, payload_mbuf);

    /* Nothing can fail up to this point, so we WILL clean up the
       payload as part of the clean up code of this function.
       If the payload was fragmented, then it will be released by
       that code path above. */

    /* Set sequence number in stream's header */
    NANO_XRCE_InlineHeaderBuffer_set_sn(header, &stream->next_out);

    last_sn = stream->next_out;

    /* Mark msg buffer as "IN USE" to denote it's pending 
       being sent by the transport */
    NANO_MessageBuffer_flags_set(
        header, NANO_XRCE_MESSAGEFLAGS_PENDING_SEND);
    /* Store stream_id in "user data" field so that we may look up the
       stream within the on_message_sent callback */
    NANO_MessageBuffer_set_userdata(header, stream_id);

    SEND_LOG("SEND message", 
        NANO_LOG_KEY("key", self->key)
        NANO_LOG_SESSIONID("session", self->id)
        NANO_LOG_STREAMID("stream", stream_id)
        NANO_LOG_MSGHDR("msg",
            *((NANO_XRCE_MessageHeader *)
                    NANO_MessageBuffer_data_ptr(header)))
        NANO_LOG_SUBMSGHDR("submsg_hdr",
            *((NANO_XRCE_SubmessageHeader *)
                NANO_XRCE_InlineHeaderBuffer_submsgid_ptr(header)))
        )
    /* Send message to remote endpoint using the session's transport */

    NANO_CHECK_RC(
        NANO_XRCE_ClientTransport_queue_send(self->transport, header),
        NANO_LOG_ERROR("transport::queue_send failed",
            NANO_LOG_KEY("key", self->key)
            NANO_LOG_SESSIONID("session", self->id)
            NANO_LOG_STREAMID("stream", stream_id)
            NANO_LOG_RC(rc)
            NANO_LOG_MBUF("header", header)));

    while (NANO_MessageBuffer_flags(
                header, NANO_XRCE_MESSAGEFLAGS_PENDING_SEND))
    {
        NANO_XRCE_ClientTransport_flush_output(self->transport);
    }

    NANO_CHECK_RC(
        /* the return code of the send operation is stored in
           the "user data" field of the mbuf by the on_message_sent callback */
        (NANO_RetCode)NANO_MessageBuffer_userdata(header),
        NANO_LOG_ERROR("FAILED to send message with transport",
            NANO_LOG_KEY("key", self->key)
            NANO_LOG_SESSIONID("session", self->id)
            NANO_LOG_STREAMID("stream", stream_id)
            NANO_LOG_RC(rc)));

#if NANO_FEAT_RELIABILITY
    if (reliable_stream)
    {
        NANO_XRCE_ReliableStream_queue_send(rel_stream, header);
        release_payload = NANO_BOOL_FALSE;
    }
#endif /* NANO_FEAT_RELIABILITY */


    /* Increase the stream's sequence number */
    NANO_XRCE_SeqNum_plusplus(&stream->next_out);
    {
        /* Skip SN 0x0000 */
        NANO_u16 sn_u = 0;
        sn_u = NANO_XRCE_SeqNum_to_u16(&stream->next_out);
        if (sn_u == 0)
        {
            NANO_XRCE_SeqNum_plusplus(&stream->next_out);
        }
    }

    if (sn_out != NULL)
    {
        *sn_out = last_sn;
    }

    rc = NANO_RETCODE_OK;

done:

    if (release_payload)
    {
        if (header != NULL)
        {
            if (!reliable_stream
#if NANO_FEAT_RELIABILITY
                || queued_payload
#endif /* NANO_FEAT_RELIABILITY */
                )
            {
                NANO_XRCE_Session_release_message(self, stream, header);
            }
            else
            {
                NANO_XRCE_Session_release_mbuf_chain(self, stream, header);
            }
            
        }
        else
        {
            NANO_XRCE_Session_release_mbuf_chain(self, stream, payload);
        }
    }

    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

#if NANO_FEAT_RELIABILITY

NANO_PRIVATE
NANO_RetCode
NANO_XRCE_Session_run_until_ackedI(
    NANO_XRCE_Session *const self,
    NANO_XRCE_ReliableStream *const stream,
    const NANO_XRCE_SeqNum *const sn,
    const NANO_Timeout timeout_ms)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_bool queue_empty = NANO_BOOL_FALSE;
    NANO_i8 cmp_res = 0;

#if NANO_FEAT_SYSTIME
    NANO_u64 ms_start = 0,
             ms_now = 0,
             ms_diff = 0;
#endif /* NANO_FEAT_SYSTIME */
    NANO_i32 timeout_remaining = 0;

    NANO_LOG_FN_ENTRY
    
    NANO_PCOND(self != NULL)

#if NANO_FEAT_SYSTIME
    NANO_OSAPI_Clock_millis(&self->clock, &ms_start);
    ms_now = ms_start;
#endif /* NANO_FEAT_SYSTIME */

    timeout_remaining = timeout_ms;

    do
    {
        queue_empty = NANO_MessageBufferQueue_empty(&stream->send_queue);
        
        if (!queue_empty)
        {
            if (sn != NULL)
            {
                NANO_XRCE_SeqNum_compare(
                    sn,
                    NANO_XRCE_InlineHeaderBuffer_sn_ptr(
                        stream->send_queue.head),
                    &cmp_res);
                if (cmp_res < 0)
                {
                    /* The first sample in the send queue is newer than the
                       specified SN, so we're done waiting */
                    rc = NANO_RETCODE_OK;
                    goto done;
                }
            }

            NANO_LOG_DEBUG("WAITING for stream to be ACKED",
                NANO_LOG_KEY("key", self->key)
                NANO_LOG_SESSIONID("session", self->id)
                NANO_LOG_STREAMID("stream",
                    NANO_XRCE_Stream_id(&stream->base))
                NANO_LOG_USIZE("queue_len",
                    NANO_MessageBufferQueue_size(&stream->send_queue)))
            
            NANO_CHECK_RC(
                NANO_XRCE_Session_run(self, 1, timeout_remaining),
                NANO_LOG_ERROR("FAILED to run session",
                    NANO_LOG_KEY("key", self->key)
                    NANO_LOG_SESSIONID("session", self->id)
                    NANO_LOG_STREAMID("stream", NANO_XRCE_Stream_id(&stream->base))
                    NANO_LOG_RC(rc)
                    NANO_LOG_I32("timeout",timeout_ms)
                    NANO_LOG_I32("timeout_remaining",timeout_remaining)));
            
#if NANO_FEAT_SYSTIME
            if (timeout_ms > 0)
            {
                NANO_OSAPI_Clock_millis(&self->clock, &ms_now);
                ms_diff = ms_now - ms_start;
                timeout_remaining =
                    (ms_diff > (NANO_u64)timeout_ms)? 0 : timeout_ms - ms_diff;
                if (timeout_remaining == 0)
                {
                    rc = NANO_RETCODE_TIMEOUT;
                    goto done;
                }
            }
#endif /* NANO_FEAT_SYSTIME */
        }

    } while (!queue_empty);
    
    rc = NANO_RETCODE_OK;
    
done:

    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

NANO_RetCode
NANO_XRCE_Session_run_until_acked(
    NANO_XRCE_Session *const self,
    const NANO_XRCE_StreamId stream_id,
    const NANO_XRCE_SeqNum *const sn,
    const NANO_Timeout timeout_ms)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_XRCE_ReliableStream *stream = NULL;

    NANO_LOG_FN_ENTRY
    
    NANO_PCOND(self != NULL)

    if (!NANO_XRCE_StreamId_is_reliable(stream_id))
    {
        NANO_LOG_ERROR("INVALID stream id",
            NANO_LOG_KEY("key", self->key)
            NANO_LOG_SESSIONID("session", self->id)
            NANO_LOG_STREAMID("stream", stream_id))
        goto done;
    }

    stream = (NANO_XRCE_ReliableStream*)
                NANO_XRCE_Session_lookup_stream(self, stream_id);
    if (stream == NULL)
    {
        goto done;
    }

    if (NANO_RETCODE_OK !=
            NANO_XRCE_Session_run_until_ackedI(
                self, stream, sn, timeout_ms))
    {
        goto done;
    }

    rc = NANO_RETCODE_OK;
    
done:

    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}


NANO_RetCode
NANO_XRCE_Session_run_reliability(
    NANO_XRCE_Session *const self,
    const NANO_Timeout timeout_ms)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_XRCE_ReliableStream *stream = NULL;
    NANO_usize i = 0;
#if NANO_FEAT_SYSTIME
    NANO_u64 ms_start = 0,
             ms_now = 0,
             ms_diff = 0;
#endif /* NANO_FEAT_SYSTIME */
    NANO_i32 timeout_remaining = 0;

    NANO_LOG_FN_ENTRY
    
    NANO_PCOND(self != NULL)

#if NANO_FEAT_SYSTIME
    NANO_OSAPI_Clock_millis(&self->clock, &ms_start);
    ms_now = ms_start;
#endif /* NANO_FEAT_SYSTIME */

    timeout_remaining = timeout_ms;

    stream = &self->stream_builtin_rel;
    NANO_CHECK_RC(
        NANO_XRCE_Session_run_until_ackedI(
            self, stream, NULL, timeout_remaining),
        NANO_LOG_ERROR("FAILED to wait on stream",
            NANO_LOG_KEY("key", self->key)
            NANO_LOG_SESSIONID("session", self->id)
            NANO_LOG_STREAMID("stream", NANO_XRCE_Stream_id(&stream->base))));

#if NANO_FEAT_SYSTIME
    if (timeout_ms > 0)
    {
        NANO_OSAPI_Clock_millis(&self->clock, &ms_now);
        ms_diff = ms_now - ms_start;
        timeout_remaining =
            (ms_diff > (NANO_u64)timeout_ms)? 0 : timeout_ms - ms_diff;
        if (timeout_remaining == 0)
        {
            rc = NANO_RETCODE_TIMEOUT;
            goto done;
        }
    }
#endif /* NANO_FEAT_SYSTIME */
    
#if NANO_FEAT_SESSION_USER_STREAMS_RELIABLE
    for (i = 0; 
            self->storage->streams != NULL &&
            i < self->storage->streams->user_streams_rel_len;
                i++)
    {
        stream = &self->storage->streams->user_streams_rel[i];
        NANO_CHECK_RC(
            NANO_XRCE_Session_run_until_ackedI(
                self, stream, NULL, timeout_remaining),
            NANO_LOG_ERROR("FAILED to wait on stream",
                NANO_LOG_KEY("key", self->key)
                NANO_LOG_SESSIONID("session", self->id)
                NANO_LOG_STREAMID("stream",
                    NANO_XRCE_Stream_id(&stream->base))));
#if NANO_FEAT_SYSTIME
        if (timeout_ms > 0)
        {
            NANO_OSAPI_Clock_millis(&self->clock, &ms_now);
            ms_diff = ms_now - ms_start;
            timeout_remaining =
                (ms_diff > (NANO_u64)timeout_ms)? 0 : timeout_ms - ms_diff;
            if (timeout_remaining == 0)
            {
                rc = NANO_RETCODE_TIMEOUT;
                goto done;
            }
        }
#endif /* NANO_FEAT_SYSTIME */
    }
#endif /* NANO_FEAT_SESSION_USER_STREAMS_RELIABLE */
    
    rc = NANO_RETCODE_OK;
    
done:

    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}
#endif /* NANO_FEAT_RELIABILITY */


NANO_RetCode
NANO_XRCE_Session_run(
    NANO_XRCE_Session *const self,
    const NANO_usize max_messages,
    const NANO_Timeout timeout_ms)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_bool run_complete = NANO_BOOL_FALSE;
#if NANO_FEAT_SYSTIME
    NANO_u64 ms_start = 0,
             ms_now = 0,
             ms_diff = 0;
#endif /* NANO_FEAT_SYSTIME */
    NANO_Timeout timeout_remaining = 0;
    NANO_u32 timeout_transport = 0,
             min_timeout = 0;

    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)

    RUN_LOG("start SESSION::RUN",
        NANO_LOG_KEY("key", self->key)
        NANO_LOG_SESSIONID("session", self->id)
        NANO_LOG_USIZE("max", max_messages)
        NANO_LOG_TS_MS("timeout", timeout_ms))

    self->recvd_messages = 0;

    timeout_remaining = timeout_ms;

#if NANO_FEAT_SYSTIME
    NANO_OSAPI_Clock_millis(&self->clock, &ms_start);
    ms_now = ms_start;
#endif /* NANO_FEAT_SYSTIME */

    do
    {
        RUN_LOG("waiting for messages",
            NANO_LOG_KEY("key", self->key)
            NANO_LOG_SESSIONID("session", self->id)
            NANO_LOG_USIZE("rcvd_msgs", self->recvd_messages)
            NANO_LOG_USIZE("max_msgs", max_messages)
            NANO_LOG_I32("timeout", timeout_ms)
            NANO_LOG_I32("timeout_remaining", timeout_remaining))

#if NANO_FEAT_SYSTIME

#if NANO_FEAT_RELIABILITY
        rc = NANO_XRCE_Session_send_periodic_heartbeats(self, ms_now, 1);
        if (NANO_RETCODE_OK != rc)
        {
            NANO_LOG_ERROR("FAILED to send periodic heartbeats",
                NANO_LOG_KEY("key", self->key)
                NANO_LOG_SESSIONID("session", self->id))
            goto done;
        }
        rc = NANO_RETCODE_ERROR;
#if 0
        NANO_CHECK_RC(
            NANO_XRCE_Session_send_periodic_acknacks(self, ms_now),
            NANO_LOG_ERROR_MSG("FAILED to send periodic acknacks"));
#endif

#endif /* NANO_FEAT_RELIABILITY */
#if NANO_FEAT_ASSERT_LIVELINESS
        rc = NANO_XRCE_Session_assert_periodic_liveliness(self, ms_now);
        if (NANO_RETCODE_OK != rc)
        {
            NANO_LOG_ERROR("FAILED to assert periodic liveliness",
                NANO_LOG_KEY("key", self->key)
                NANO_LOG_SESSIONID("session", self->id))
            goto done;
        }
        rc = NANO_RETCODE_ERROR;
#endif /* NANO_FEAT_ASSERT_LIVELINESS */

        /* The call to ClientTransport::process_input() must always use a
           finite and non-zero timeout, otherwise we would block in that
           call and fail to send periodic heartbeats */
        timeout_transport = 0;
        if (timeout_remaining != 0)
        {
            min_timeout = 0;
#if NANO_FEAT_RELIABILITY
            min_timeout = self->period_hb;
#endif /* NANO_FEAT_RELIABILITY */
#if NANO_FEAT_ASSERT_LIVELINESS
            if (self->period_liveliness > 0 &&
                self->period_liveliness < min_timeout)
            {
                min_timeout = self->period_liveliness;
            }
#endif /* NANO_FEAT_ASSERT_LIVELINESS */

            if (min_timeout > 0 &&
                (timeout_remaining < 0 ||
                (NANO_u32)timeout_remaining > min_timeout))
            {
                timeout_transport = min_timeout;
            }
            else
            {
                timeout_transport = timeout_remaining;
            }
        }
#endif /* NANO_FEAT_SYSTIME */

        NANO_XRCE_ClientTransport_flush_output(self->transport);

        switch (rc = NANO_XRCE_ClientTransport_process_input(
                    self->transport,
                    1 /* max_messages */,
                    timeout_transport))
        {
        case NANO_RETCODE_OK:
        case NANO_RETCODE_TIMEOUT:
        {
            run_complete =
                (max_messages > 0 && self->recvd_messages >= max_messages) ||
                timeout_ms == 0;
            rc = NANO_RETCODE_ERROR;
            break;
        }
        default:
        {
            NANO_LOG_ERROR("transport::process_input failed",
                NANO_LOG_KEY("key", self->key)
                NANO_LOG_SESSIONID("session", self->id)
                NANO_LOG_RC(rc))
            goto done;
            break;
        }
        }

#if NANO_FEAT_SYSTIME
        NANO_OSAPI_Clock_millis(&self->clock, &ms_now);

        if (!run_complete && timeout_ms > 0)
        {
            ms_diff = ms_now - ms_start;

            if (ms_diff >= (NANO_u64)(timeout_ms))
            {
                NANO_LOG_DEBUG("TIMEOUT exceeded",
                    NANO_LOG_KEY("key", self->key)
                    NANO_LOG_SESSIONID("session", self->id)
                    NANO_LOG_I32("timeout", timeout_ms)
                    NANO_LOG_U32("passed", ms_diff))
                if (max_messages == 0 &&
                    self->recvd_messages > 0)
                {
                    rc = NANO_RETCODE_OK;
                }
                else
                {
                    rc = NANO_RETCODE_TIMEOUT;
                }
                goto done;
            }
            else
            {
                RUN_LOG("waiting for messages (timeout)",
                    NANO_LOG_KEY("key", self->key)
                    NANO_LOG_SESSIONID("session", self->id)
                    NANO_LOG_I32("timeout_ms", timeout_ms)
                    NANO_LOG_U32("ms_diff", ms_diff))
                
                timeout_remaining = timeout_ms - ms_diff;
            }
            NANO_PCOND(ms_diff <= (NANO_u64)NANO_U32_MAX)
        }
#endif /* NANO_FEAT_SYSTIME */
    } while (!run_complete);

    NANO_PCOND(
        max_messages == 0 ||
        self->recvd_messages == max_messages ||
        timeout_ms == 0);

    rc = NANO_RETCODE_OK;

done:
    NANO_LOG_TRACE("end SESSION::RUN",
        NANO_LOG_KEY("key", self->key)
        NANO_LOG_SESSIONID("session", self->id)
        NANO_LOG_USIZE("max", max_messages)
        NANO_LOG_TS_MS("timeout", timeout_ms))
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

#if NANO_FEAT_RELIABILITY

#if NANO_FEAT_SYSTIME
#if 0
#if !NANO_FEAT_AGENT
NANO_PRIVATE
#endif /* !NANO_FEAT_AGENT */
NANO_RetCode
NANO_XRCE_Session_send_periodic_acknacks(
    NANO_XRCE_Session *const self,
    const NANO_u64 ms_now)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_XRCE_ReliableStream *stream = NULL;
    NANO_u64 ms_diff = 0;
    NANO_i8 cmp_res = 0;
    NANO_bool send_ack = NANO_BOOL_FALSE;
    NANO_u64 ts_now = 0;

    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)

    if (!NANO_XRCE_Session_is_connected(self) || self->period_ack == 0)
    {
        /* periodic acknack is disabled */
        rc = NANO_RETCODE_OK;
        goto done;
    }

    if (ms_now == 0)
    {
        NANO_OSAPI_Clock_millis(&self->clock, &ts_now);
    }
    else
    {
        ts_now = ms_now;
    }

    if (self->ts_last_periodic_ack > 0)
    {
        NANO_PCOND(ts_now >= self->ts_last_periodic_ack)

        ms_diff = ts_now - self->ts_last_periodic_ack;

        if (ms_diff < self->period_ack)
        {
            /* too soon to send acknack */
            rc = NANO_RETCODE_OK;
            goto done;
        }
    }
    self->ts_last_periodic_ack = ts_now;
    
    stream = &(self->stream_builtin_rel);

    if (!stream->in_heartbeat_recvd)
    {
        send_ack = NANO_BOOL_TRUE;
    }
    else
    {
        NANO_XRCE_SeqNum_compare(
            &stream->base.next_in,
            &stream->in_heartbeat_highest_sn,
            &cmp_res);
        send_ack = (cmp_res <= 0);
    }

    if (send_ack)
    {
        NANO_CHECK_RC(
            NANO_XRCE_Session_send_acknack(self, stream),
            NANO_LOG_ERROR("FAILED to send periodic acknack on stream",
                NANO_LOG_STREAMID("stream_id",
                    NANO_XRCE_Stream_id(&stream->base))));
    }

#if NANO_FEAT_SESSION_USER_STREAMS_RELIABLE
    {
        NANO_usize i = 0;
        for (i = 0; i < self->user_streams_rel_len; i++)
        {
            stream = &(self->user_streams_rel[i]);
            
            if (!stream->in_heartbeat_recvd)
            {
                send_ack = NANO_BOOL_TRUE;
            }
            else
            {
                NANO_XRCE_SeqNum_compare(
                    &stream->base.next_in,
                    &stream->in_heartbeat_highest_sn,
                    &cmp_res);
                send_ack = (cmp_res <= 0);
            }

            if (send_ack)
            {
                NANO_CHECK_RC(
                    NANO_XRCE_Session_send_acknack(self, stream),
                    NANO_LOG_ERROR(
                        "FAILED to send periodic acknack on stream",
                        NANO_LOG_STREAMID("stream_id",
                            NANO_XRCE_Stream_id(&stream->base))));
            }
        }
    }
#endif /* NANO_FEAT_SESSION_USER_STREAMS_RELIABLE */

    rc = NANO_RETCODE_OK;

done:

    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}
#endif

#if !NANO_FEAT_AGENT
NANO_PRIVATE
#endif /* !NANO_FEAT_AGENT */
NANO_RetCode
NANO_XRCE_Session_send_periodic_heartbeats(
    NANO_XRCE_Session *const self,
    const NANO_u64 ms_now,
    const NANO_usize min_queue_len)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_XRCE_ReliableStream *stream = NULL;
    NANO_u64 ms_diff = 0,
             ts_now = 0;
    NANO_usize queue_len = 0;

    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)

    if (!NANO_XRCE_Session_is_connected(self) || self->period_hb == 0)
    {
        /* periodic heartbeat is disabled */
        rc = NANO_RETCODE_OK;
        goto done;
    }

    if (ms_now == 0)
    {
        NANO_OSAPI_Clock_millis(&self->clock, &ts_now);
    }
    else
    {
        ts_now = ms_now;
    }
    
    if (self->ts_last_periodic_hb > 0)
    {
        NANO_PCOND(ts_now >= self->ts_last_periodic_hb)

        ms_diff = ts_now - self->ts_last_periodic_hb;

        if (ms_diff < self->period_hb)
        {
            /* it's not time yet to send a heartbeat */
            rc = NANO_RETCODE_OK;
            goto done;
        }
    }

    self->ts_last_periodic_hb = ts_now;

    stream = &(self->stream_builtin_rel);

    queue_len = NANO_MessageBufferQueue_size(&stream->send_queue);

    if (queue_len >= min_queue_len)
    {
        NANO_CHECK_RC(
            NANO_XRCE_Session_send_heartbeat(self, stream, NANO_BOOL_FALSE),
            NANO_LOG_ERROR("FAILED to send periodic heartbeat on stream",
                NANO_LOG_KEY("key", self->key)
                NANO_LOG_SESSIONID("session", self->id)
                NANO_LOG_STREAMID("stream_id",
                    NANO_XRCE_Stream_id(&stream->base))));
    }
#if NANO_FEAT_AGENT && 0
    else
    {
        NANO_LOG_WARNING("empty send queue",
            NANO_LOG_KEY("key", self->key)
            NANO_LOG_SESSIONID("session", self->id)
            NANO_LOG_STREAMID("stream_id",
                NANO_XRCE_Stream_id(&stream->base))
            NANO_LOG_USIZE("queue_len", queue_len)
            NANO_LOG_USIZE("min_queue_len", min_queue_len))
    }
#endif

#if NANO_FEAT_SESSION_USER_STREAMS_RELIABLE
    {
        NANO_usize i = 0;
        for (i = 0;
            self->storage->streams != NULL &&
            i < self->storage->streams->user_streams_rel_len; i++)
        {
            stream = &self->storage->streams->user_streams_rel[i];
            queue_len = NANO_MessageBufferQueue_size(&stream->send_queue);

            if (queue_len >= min_queue_len)
            {
                NANO_CHECK_RC(
                    NANO_XRCE_Session_send_heartbeat(self, stream, NANO_BOOL_FALSE),
                    NANO_LOG_ERROR("FAILED to send periodic heartbeat on stream",
                        NANO_LOG_KEY("key", self->key)
                        NANO_LOG_SESSIONID("session", self->id)
                        NANO_LOG_STREAMID("stream_id",
                            NANO_XRCE_Stream_id(&stream->base))));
            }
        }
    }
#endif /* NANO_FEAT_SESSION_USER_STREAMS_RELIABLE */

    rc = NANO_RETCODE_OK;

done:

    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}
#endif /* NANO_FEAT_SYSTIME */

NANO_PRIVATE
NANO_RetCode
NANO_XRCE_Session_send_acknack(
    NANO_XRCE_Session *const self,
    NANO_XRCE_ReliableStream *const stream)
{
    NANO_MessageBufferData
        header_buffer[NANO_MESSAGEBUFFER_INLINE_SIZE(
            NANO_XRCE_INLINEHEADERBUFFER_INLINE_SIZE_BYTES)] = {0};
    NANO_MessageBufferData
        ackn_buffer[NANO_MESSAGEBUFFER_INLINE_SIZE(
            NANO_XRCE_ACKNACKPAYLOAD_SERIALIZED_SIZE_MAX)] = {0};
    NANO_MessageBuffer *ackn_mbuf = (NANO_MessageBuffer *)&ackn_buffer,
                       *ackn_header = NULL;
    NANO_XRCE_AckNackPayload *ackn = NULL;
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_XRCE_SubmessageFlags submsg_flags = NANO_XRCE_SUBMESSAGEFLAGS_DEFAULT;

    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)
    NANO_PCOND(stream != NULL)
    NANO_PCOND(NANO_XRCE_Stream_id(&stream->base) != NANO_XRCE_STREAMID_NONE)

    NANO_MessageBuffer_flags_set_inline(ackn_mbuf);
    NANO_MessageBuffer_set_data_len(ackn_mbuf,
                                    NANO_XRCE_ACKNACKPAYLOAD_SERIALIZED_SIZE_MAX);

    ackn = (NANO_XRCE_AckNackPayload *)
        NANO_MessageBuffer_data_ptr(ackn_mbuf);

    ackn->first_unacked_seq_num =
        NANO_XRCE_SeqNum_to_u16(NANO_XRCE_Stream_sn_next_in(&stream->base));
    ackn->stream_id = NANO_XRCE_Stream_id(&stream->base);

// #if NANO_FEAT_AGENT
//     /* An agent can request all available messages */
//     NANO_XRCE_AckNackBitmap_from_u16(&ackn->nack_bitmap, 0xFFFF);
// #else
//     /* Clients repair messages one at a time */
//     NANO_XRCE_AckNackBitmap_from_u16(&ackn->nack_bitmap, 0x0001);
// #endif /* NANO_FEAT_AGENT */

    /* repair messages one at a time */
    NANO_XRCE_AckNackBitmap_from_u16(&ackn->nack_bitmap, 0x0001);

    /* Allocate ackn_header from reliable stream */
    ackn_header = (NANO_MessageBuffer *)header_buffer;
    NANO_XRCE_InlineHeaderBuffer_initialize(
        ackn_header, self->id, self->key, NANO_XRCE_STREAMID_NONE);
    NANO_XRCE_InlineHeaderBuffer_update_next(
        ackn_header,
        NANO_XRCE_SUBMESSAGEID_ACKNACK,
        submsg_flags,
        ackn_mbuf);

    ACK_SEND_LOG("send ACKNACK",
        NANO_LOG_KEY("key", self->key)
        NANO_LOG_SESSIONID("session", self->id)
        NANO_LOG_STREAMID("stream", ackn->stream_id)
        NANO_LOG_SN("nack_head", *NANO_XRCE_Stream_sn_next_in(&stream->base))
        NANO_LOG_H16("nack_bitmap",
            NANO_XRCE_AckNackBitmap_to_u16(&ackn->nack_bitmap)))

    /* Mark msg buffer to denote it's pending being sent by the transport */
    NANO_MessageBuffer_flags_set(
        ackn_header, NANO_XRCE_MESSAGEFLAGS_PENDING_SEND);
    /* Store stream_id in "user data" field so that we may look up the
       stream within the on_message_sent callback */
    NANO_MessageBuffer_set_userdata(ackn_header, NANO_XRCE_STREAMID_NONE);

    /* Send message to remote endpoint using the session's transport */

    NANO_CHECK_RC(
        NANO_XRCE_ClientTransport_queue_send(self->transport, ackn_header),
        NANO_LOG_ERROR("transport::queue_send failed",
            NANO_LOG_KEY("key", self->key)
            NANO_LOG_SESSIONID("session", self->id)
            NANO_LOG_RC(rc)
            NANO_LOG_MBUF("header", ackn_header)));

    while (NANO_MessageBuffer_flags(
                ackn_header, NANO_XRCE_MESSAGEFLAGS_PENDING_SEND))
    {
        NANO_XRCE_ClientTransport_flush_output(self->transport);
    }

    NANO_LOG_DEBUG("send RESULT",
        NANO_LOG_KEY("key", self->key)
        NANO_LOG_SESSIONID("session", self->id)
        NANO_LOG_STREAMID("stream", NANO_XRCE_STREAMID_NONE)
        NANO_LOG_MSGHDR("msg",
            *((NANO_XRCE_MessageHeader *)
                    NANO_MessageBuffer_data_ptr(ackn_header)))
        NANO_LOG_RC(NANO_MessageBuffer_userdata(ackn_header))
        NANO_LOG_MBUF("buffer", ackn_header))

    NANO_CHECK_RC(
        /* the return code of the send operation is stored in
           the "user data" field of the mbuf by the on_message_sent callback */
        (NANO_RetCode)NANO_MessageBuffer_userdata(ackn_header),
        NANO_LOG_ERROR("FAILED to send ACKNACK with transport",
            NANO_LOG_KEY("key", self->key)
            NANO_LOG_SESSIONID("session", self->id)
            NANO_LOG_RC(rc)));

    rc = NANO_RETCODE_OK;

done:
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

NANO_PRIVATE
NANO_RetCode
NANO_XRCE_Session_send_heartbeat(
    NANO_XRCE_Session *const self,
    NANO_XRCE_ReliableStream *const stream,
    const NANO_bool force)
{
    NANO_MessageBufferData
        header_buffer[NANO_MESSAGEBUFFER_INLINE_SIZE(
            NANO_XRCE_INLINEHEADERBUFFER_INLINE_SIZE_BYTES)] = {0};
    NANO_MessageBufferData
        hb_data[NANO_MESSAGEBUFFER_INLINE_SIZE(
            NANO_XRCE_HEARTBEATPAYLOAD_SERIALIZED_SIZE_MAX)] = {0};
    NANO_MessageBuffer *hb_payload = (NANO_MessageBuffer *)hb_data,
                       *hb_header = NULL;
    NANO_XRCE_HeartbeatPayload *hb = NULL;
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_XRCE_SeqNum sn_queue_head = NANO_XRCE_SEQNUM_INITIALIZER,
                     sn_queue_tail = NANO_XRCE_SEQNUM_INITIALIZER,
                     sn_next_out = NANO_XRCE_SEQNUM_INITIALIZER;
    NANO_usize queue_len = 0;
    NANO_XRCE_SubmessageFlags submsg_flags = NANO_XRCE_SUBMESSAGEFLAGS_DEFAULT;

    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)
    NANO_PCOND(stream != NULL)
    NANO_PCOND(NANO_XRCE_Stream_id(&stream->base) != NANO_XRCE_STREAMID_NONE)

    NANO_MessageBuffer_flags_set_inline(hb_payload);
    NANO_MessageBuffer_set_data_len(hb_payload,
                                    NANO_XRCE_HEARTBEATPAYLOAD_SERIALIZED_SIZE_MAX);
    hb = (NANO_XRCE_HeartbeatPayload *)NANO_MessageBuffer_data_ptr(hb_payload);

    NANO_XRCE_Session_stream_sendqueue_info(
        self, 
        stream,
        &sn_next_out,
        &sn_queue_head,
        &sn_queue_tail,
        &queue_len);

    if (queue_len == 0)
    {
        if (!force)
        {
            rc = NANO_RETCODE_OK;
            goto done;
        }
        NANO_LOG_DEBUG("sending HEARTBEAT on empty send queue",
            NANO_LOG_KEY("key", self->key)
            NANO_LOG_SESSIONID("session", self->id)
            NANO_LOG_STREAMID("stream", hb->stream_id)
            NANO_LOG_SN("next_out", sn_next_out))
    }

    hb->first_unacked_seq_num = NANO_XRCE_SeqNum_to_u16(&sn_queue_head);
    hb->last_unacked_seq_num = NANO_XRCE_SeqNum_to_u16(&sn_queue_tail);
    hb->stream_id = NANO_XRCE_Stream_id(&stream->base);

    HB_SEND_LOG("send HEARTBEAT",
        NANO_LOG_KEY("key", self->key)
        NANO_LOG_SESSIONID("session", self->id)
        NANO_LOG_STREAMID("stream", hb->stream_id)
        NANO_LOG_SN("next_out", sn_next_out)
        NANO_LOG_USIZE("queue_len", queue_len)
        NANO_LOG_SN("sn_head", sn_queue_head)
        NANO_LOG_SN("sn_tail", sn_queue_tail))

    /* Allocate ackn_header from reliable stream */
    hb_header = (NANO_MessageBuffer *)header_buffer;

    NANO_XRCE_InlineHeaderBuffer_initialize(
        hb_header, self->id, self->key, NANO_XRCE_STREAMID_NONE);
    NANO_XRCE_InlineHeaderBuffer_update_next(
        hb_header,
        NANO_XRCE_SUBMESSAGEID_HEARTBEAT,
        submsg_flags,
        hb_payload);

    /* Mark msg buffer as "IN USE" to denote it's pending 
       being sent by the transport */
    NANO_MessageBuffer_flags_set(
        hb_header, NANO_XRCE_MESSAGEFLAGS_PENDING_SEND);
    /* Store stream_id in "user data" field so that we may look up the
       stream within the on_message_sent callback */
    NANO_MessageBuffer_set_userdata(hb_header, NANO_XRCE_STREAMID_NONE);

    /* Send message to remote endpoint using the session's transport */

    NANO_CHECK_RC(NANO_XRCE_ClientTransport_queue_send(
            self->transport, hb_header),
        NANO_LOG_ERROR("transport::queue_send failed",
            NANO_LOG_KEY("key", self->key)
            NANO_LOG_SESSIONID("session", self->id)
            NANO_LOG_RC(rc)));

    while (NANO_MessageBuffer_flags(
                hb_header, NANO_XRCE_MESSAGEFLAGS_PENDING_SEND))
    {
        NANO_XRCE_ClientTransport_flush_output(self->transport);
    }

    NANO_CHECK_RC(
        /* the return code of the send operation is stored in
           the "user data" field of the mbuf by the on_message_sent callback */
        (NANO_RetCode)NANO_MessageBuffer_userdata(hb_header),
        NANO_LOG_ERROR("FAILED to send HEARTBEAT with transport",
            NANO_LOG_KEY("key", self->key)
            NANO_LOG_SESSIONID("session", self->id)
            NANO_LOG_RC(rc)));

    rc = NANO_RETCODE_OK;

done:
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

#if NANO_FEAT_AGENT
void
NANO_XRCE_ReliableStream_finalize(NANO_XRCE_ReliableStream *const self)
{
    NANO_MessageBuffer *frag_buffer = NULL;
    NANO_u8 *ptr = NULL;

    frag_buffer = NANO_XRCE_ReliableStream_recv_frag_buffer(self);
    ptr = NANO_MessageBuffer_data_ptr(frag_buffer);

    if (ptr != NULL)
    {
        RTIOsapiHeap_freeBuffer(ptr);
    }
}

#endif /* NANO_FEAT_AGENT */

#endif /* NANO_FEAT_RELIABILITY */

#if NANO_FEAT_ASSERT_LIVELINESS

NANO_PRIVATE
NANO_RetCode
NANO_XRCE_Session_assert_periodic_liveliness(
    NANO_XRCE_Session *const self,
    const NANO_u64 ms_now)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_u64 ms_diff = 0,
             ts_now = 0;

    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)

    if (!NANO_XRCE_Session_is_connected(self) || self->period_liveliness == 0)
    {
        /* liveliness is disabled */
        rc = NANO_RETCODE_OK;
        goto done;
    }

    if (ms_now == 0)
    {
        NANO_OSAPI_Clock_millis(&self->clock, &ts_now);
    }
    else
    {
        ts_now = ms_now;
    }
    
    if (self->ts_last_periodic_assert > 0)
    {
        NANO_PCOND(ts_now >= self->ts_last_periodic_assert)

        ms_diff = ts_now - self->ts_last_periodic_assert;

        if (ms_diff < self->period_liveliness)
        {
            /* it's not time yet to send liveliness */
            rc = NANO_RETCODE_OK;
            goto done;
        }
    }

    self->ts_last_periodic_assert = ts_now;

    NANO_CHECK_RC(
        NANO_XRCE_Session_assert_liveliness(self),
        NANO_LOG_ERROR("FAILED to assert liveliness",
            NANO_LOG_KEY("key", self->key)
            NANO_LOG_SESSIONID("session", self->id)));

    rc = NANO_RETCODE_OK;

done:

    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}


NANO_PRIVATE
NANO_RetCode
NANO_XRCE_Session_assert_liveliness(NANO_XRCE_Session *const self)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_XRCE_GetInfoPayload *ginfo_payload = NULL;
    NANO_MessageBuffer *ginfo_mbuf = NULL;
    NANO_XRCE_Stream *stream = NULL;
    NANO_u8 *ptr = NULL;
    
    NANO_LOG_FN_ENTRY
    
    NANO_PCOND(self != NULL)

    UNUSED_ARG(stream);

    ginfo_mbuf = NANO_XRCE_Session_allocate_message(
                    self,
                    &self->stream_none.base,
                    NANO_XRCE_MESSAGETYPE_INLINE_PAYLOAD,
                    NANO_XRCE_GETINFOPAYLOAD_HEADER_SERIALIZED_SIZE_MAX,
                    NULL); 

    if (ginfo_mbuf == NULL)
    {
        NANO_LOG_ERROR("failed to allocate GET_INFO message",
            NANO_LOG_KEY("key", self->key)
            NANO_LOG_SESSIONID("session", self->id))
        rc = NANO_RETCODE_OUT_OF_RESOURCES;
        goto done;
    }
    
    ginfo_payload = (NANO_XRCE_GetInfoPayload*)
                        NANO_MessageBuffer_data_ptr(ginfo_mbuf);
    ginfo_payload->base.request_id.value[0] = self->key.value[0];
    ginfo_payload->base.request_id.value[1] = self->key.value[1];
    ginfo_payload->base.object_id.value[0] = self->key.value[2];
    ginfo_payload->base.object_id.value[1] = self->key.value[3];
    NANO_XRCE_ObjectId_combine(
        &ginfo_payload->base.object_id,
        NANO_XRCE_OBJK_CLIENT,
        &ginfo_payload->base.object_id);
    
    ptr = NANO_MessageBuffer_data_ptr(ginfo_mbuf) + 
            NANO_XRCE_BASEOBJECTREQUEST_SERIALIZED_SIZE_MAX;
    ptr = NANO_OSAPI_Memory_align(ptr, NANO_u32);
    *((NANO_XRCE_InfoMask*)ptr) = NANO_XRCE_INFOMASK_INFO_ACTIVITY;
    ptr += sizeof(NANO_XRCE_InfoMask);

    NANO_MessageBuffer_set_data_len(ginfo_mbuf,
        ptr - NANO_MessageBuffer_data_ptr(ginfo_mbuf));

    NANO_LOG_DEBUG("ASSERTING liveliness",
        NANO_LOG_KEY("key", self->key)
        NANO_LOG_SESSIONID("session", self->id))
    
    NANO_CHECK_RC(
        NANO_XRCE_Session_send(
            self,
            &self->stream_none.base,
            NANO_XRCE_SUBMESSAGEID_GET_INFO,
            NANO_XRCE_SUBMESSAGEFLAGS_DEFAULT,
            ginfo_mbuf,
            NULL),
        ginfo_mbuf = NULL;
        NANO_LOG_ERROR("failed to send GET_INFO request",
            NANO_LOG_KEY("key", self->key)
            NANO_LOG_SESSIONID("session", self->id)
            NANO_LOG_RC(rc)));
    ginfo_mbuf = NULL;
    
    rc = NANO_RETCODE_OK;
    
done:
    if (ginfo_mbuf != NULL)
    {
        NANO_XRCE_Session_release_message(
            self,
            &self->stream_none.base,
            ginfo_mbuf);
    }

    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

#endif /* NANO_FEAT_ASSERT_LIVELINESS */