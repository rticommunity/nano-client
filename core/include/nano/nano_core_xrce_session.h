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

#ifndef nano_core_xrce_session_h
#define nano_core_xrce_session_h

#include "nano/nano_core_config.h"

/******************************************************************************
 *                            CDR_Stream (forward)
 ******************************************************************************/
/*e
 * @addtogroup nano_api_cdr_stream
 * @{
 */
typedef struct NANODllExport NANO_CDR_StreamI NANO_CDR_Stream;
/*e @} *//* nano_api_cdr_stream */

/******************************************************************************
 *                              Session (forward)
 ******************************************************************************/
/*i
 * @addtogroup nano_api_session_session
 * @{
 */
typedef struct NANODllExport NANO_XRCE_SessionI NANO_XRCE_Session;
/*i @} *//* nano_api_session_session */

/*i
 * @addtogroup nano_api_xrce_impl
 * @{
 */

/******************************************************************************
 *                         Custom MessageBuffer Flags
 ******************************************************************************/

#define NANO_XRCE_MESSAGEFLAGS_PENDING_SEND \
            NANO_MESSAGEBUFFERFLAGS_USER_0
#define NANO_XRCE_MESSAGEFLAGS_ACKED \
            NANO_MESSAGEBUFFERFLAGS_USER_1
#define NANO_XRCE_MESSAGEFLAGS_OWNED_BY_STREAM \
            NANO_MESSAGEBUFFERFLAGS_USER_2
#define NANO_XRCE_MESSAGEFLAGS_RESERVED_BY_STREAM \
            NANO_MESSAGEBUFFERFLAGS_USER_3
#define NANO_XRCE_MESSAGEFLAGS_OWNED_BY_TRANSPORT \
            NANO_MESSAGEBUFFERFLAGS_USER_4
#define NANO_XRCE_MESSAGEFLAGS_DISCOVERY \
            NANO_MESSAGEBUFFERFLAGS_USER_5


/******************************************************************************
 *                         Custom MessageBuffer Types
 ******************************************************************************/

#define NANO_XRCE_MESSAGETYPE_INLINE_HEADER \
            NANO_MESSAGEBUFFERTYPE_USER_0
#define NANO_XRCE_MESSAGETYPE_INLINE_PAYLOAD \
            NANO_MESSAGEBUFFERTYPE_USER_1
#define NANO_XRCE_MESSAGETYPE_INLINE_PAYLOAD_TAIL \
            NANO_MESSAGEBUFFERTYPE_USER_2
#define NANO_XRCE_MESSAGETYPE_EXTERNAL_PAYLOAD \
            NANO_MESSAGEBUFFERTYPE_USER_3
#define NANO_XRCE_MESSAGETYPE_RECV_BUFFER \
            NANO_MESSAGEBUFFERTYPE_USER_4
#define NANO_XRCE_MESSAGETYPE_OUTPUT_FRAGMENT \
            NANO_MESSAGEBUFFERTYPE_USER_6
#define NANO_XRCE_MESSAGETYPE_FRAGMENT_RECV_BUFFER \
            NANO_MESSAGEBUFFERTYPE_USER_5

/*i @} *//* nano_api_xrce_impl */

/******************************************************************************
 *                          MessageHeaderBuffer
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_impl
 * @{
 */

typedef NANO_MessageBuffer NANO_XRCE_InlineHeaderBuffer;


#define NANO_XRCE_INLINEHEADERBUFFER_INLINE_SIZE \
    NANO_MESSAGEBUFFER_INLINE_SIZE(\
        NANO_XRCE_MESSAGEHEADER_SERIALIZED_SIZE_MAX + \
        NANO_XRCE_SUBMESSAGEHEADER_SERIALIZED_SIZE_MAX)

#define NANO_XRCE_INLINEHEADERBUFFER_INLINE_SIZE_BYTES \
    (NANO_XRCE_INLINEHEADERBUFFER_INLINE_SIZE * sizeof(NANO_MessageBufferData))

#define NANO_XRCE_INLINEHEADERBUFFER_SESSIONID_OFFSET         (0)
#define NANO_XRCE_INLINEHEADERBUFFER_STREAMID_OFFSET \
    (NANO_XRCE_INLINEHEADERBUFFER_SESSIONID_OFFSET +\
        sizeof(NANO_XRCE_SessionId))
#define NANO_XRCE_INLINEHEADERBUFFER_SN_OFFSET \
    (NANO_XRCE_INLINEHEADERBUFFER_STREAMID_OFFSET +\
        sizeof(NANO_XRCE_StreamId))
#define NANO_XRCE_INLINEHEADERBUFFER_CLIENTKEY_OFFSET \
    (NANO_XRCE_INLINEHEADERBUFFER_SN_OFFSET + sizeof(NANO_XRCE_SeqNum))

#define NANO_XRCE_INLINEHEADERBUFFER_SUBMSG_OFFSET(session_id_) \
    (NANO_XRCE_INLINEHEADERBUFFER_CLIENTKEY_OFFSET + \
        ((NANO_XRCE_SessionId_has_client_key((session_id_)))?\
            sizeof(NANO_XRCE_ClientKey) : 0))

#define NANO_XRCE_INLINEHEADERBUFFER_SUBMSGID_OFFSET(session_id_) \
    NANO_XRCE_INLINEHEADERBUFFER_SUBMSG_OFFSET((session_id_))

#define NANO_XRCE_INLINEHEADERBUFFER_SUBMSGFLAGS_OFFSET(session_id_) \
    (NANO_XRCE_INLINEHEADERBUFFER_SUBMSGID_OFFSET((session_id_)) + \
        sizeof(NANO_XRCE_SubmessageId))

#define NANO_XRCE_INLINEHEADERBUFFER_SUBMSGLEN_OFFSET(session_id_) \
    (NANO_XRCE_INLINEHEADERBUFFER_SUBMSGFLAGS_OFFSET((session_id_)) + \
        sizeof(NANO_XRCE_SubmessageFlags))

#define NANO_XRCE_InlineHeaderBuffer_length(s_) \
    NANO_XRCE_SessionId_header_size(\
        NANO_XRCE_InlineHeaderBuffer_session_id((s_)))

#define NANO_XRCE_InlineHeaderBuffer_session_id_ptr(s_) \
    ((NANO_XRCE_SessionId*)\
        (NANO_MessageBuffer_data_ptr(s_) + \
            NANO_XRCE_INLINEHEADERBUFFER_SESSIONID_OFFSET))

#define NANO_XRCE_InlineHeaderBuffer_session_id(s_) \
    *NANO_XRCE_InlineHeaderBuffer_session_id_ptr((s_))

#define NANO_XRCE_InlineHeaderBuffer_stream_id_ptr(s_) \
    ((NANO_XRCE_StreamId*)\
        (NANO_MessageBuffer_data_ptr(s_) + \
            NANO_XRCE_INLINEHEADERBUFFER_STREAMID_OFFSET))

#define NANO_XRCE_InlineHeaderBuffer_stream_id(s_) \
    *NANO_XRCE_InlineHeaderBuffer_stream_id_ptr((s_))

#define NANO_XRCE_InlineHeaderBuffer_sn_ptr(s_) \
    ((NANO_XRCE_SeqNum*)\
        (NANO_MessageBuffer_data_ptr(s_) + \
            NANO_XRCE_INLINEHEADERBUFFER_SN_OFFSET))

#define NANO_XRCE_InlineHeaderBuffer_sn(s_) \
    NANO_XRCE_InlineHeaderBuffer_sn_ptr((s_))

#define NANO_XRCE_InlineHeaderBuffer_has_clientkey(s_) \
    NANO_XRCE_SessionId_has_client_key(\
        NANO_XRCE_InlineHeaderBuffer_session_id((s_)))

#define NANO_XRCE_InlineHeaderBuffer_clientkey_ptr(s_) \
    ((NANO_XRCE_ClientKey*)\
        (NANO_MessageBuffer_data_ptr(s_) + \
            NANO_XRCE_INLINEHEADERBUFFER_CLIENTKEY_OFFSET))

#define NANO_XRCE_InlineHeaderBuffer_clientkey(s_) \
    NANO_XRCE_InlineHeaderBuffer_clientkey_ptr((s_))

#define NANO_XRCE_InlineHeaderBuffer_submsgid_ptr(s_) \
    ((NANO_XRCE_SubmessageId*)\
        (NANO_MessageBuffer_data_ptr(s_) + \
            NANO_XRCE_INLINEHEADERBUFFER_SUBMSGID_OFFSET(\
                NANO_XRCE_InlineHeaderBuffer_session_id((s_)))))

#define NANO_XRCE_InlineHeaderBuffer_submsgid(s_) \
    *NANO_XRCE_InlineHeaderBuffer_submsgid_ptr(s_)

#define NANO_XRCE_InlineHeaderBuffer_submsgflags_ptr(s_) \
    ((NANO_XRCE_SubmessageFlags*)\
        (NANO_MessageBuffer_data_ptr(s_) + \
            NANO_XRCE_INLINEHEADERBUFFER_SUBMSGFLAGS_OFFSET(\
                NANO_XRCE_InlineHeaderBuffer_session_id((s_)))))

#define NANO_XRCE_InlineHeaderBuffer_submsgflags(s_) \
    *NANO_XRCE_InlineHeaderBuffer_submsgflags_ptr(s_)

#define NANO_XRCE_InlineHeaderBuffer_submsglen_ptr(s_) \
    ((NANO_u16*)\
        (NANO_MessageBuffer_data_ptr(s_) + \
            NANO_XRCE_INLINEHEADERBUFFER_SUBMSGLEN_OFFSET(\
                NANO_XRCE_InlineHeaderBuffer_session_id((s_)))))

#define NANO_XRCE_InlineHeaderBuffer_submsglen(s_) \
    NANO_u16_from_le(*NANO_XRCE_InlineHeaderBuffer_submsglen_ptr(s_))

#define NANO_XRCE_InlineHeaderBuffer_set_sn(s_,sn_) \
    NANO_OSAPI_Memory_copy(\
        NANO_XRCE_InlineHeaderBuffer_sn((s_)),(sn_),sizeof(NANO_XRCE_SeqNum))

#define NANO_XRCE_InlineHeaderBuffer_set_submsg(s_,id_,flags_,len_) \
{\
    *NANO_XRCE_InlineHeaderBuffer_submsgid_ptr((s_)) = (id_);\
    *NANO_XRCE_InlineHeaderBuffer_submsgflags_ptr((s_)) = (flags_);\
    *NANO_XRCE_InlineHeaderBuffer_submsglen_ptr((s_)) = NANO_u16_to_le((len_));\
}


void
NANO_XRCE_InlineHeaderBuffer_initialize(
    NANO_XRCE_InlineHeaderBuffer *const self,
    const NANO_XRCE_SessionId session_id,
    const NANO_XRCE_ClientKey key,
    const NANO_XRCE_StreamId stream_id);

#define NANO_XRCE_InlineHeaderBuffer_initialize(s_,ss_,k_,st_) \
{\
    NANO_XRCE_SeqNum start_sn_ = NANO_XRCE_SEQNUM_INITIALIZER;\
    NANO_OSAPI_Memory_zero(\
        &(s_)->data, sizeof((s_)->data) + NANO_XRCE_SessionId_header_size((ss_)));\
    NANO_XRCE_SeqNum_plusplus(&start_sn_);\
    NANO_MessageBuffer_flags_set_inline((s_));\
    NANO_MessageBuffer_set_data_offset((s_),0);\
    *NANO_XRCE_InlineHeaderBuffer_session_id_ptr((s_)) = (ss_);\
    *NANO_XRCE_InlineHeaderBuffer_stream_id_ptr((s_)) = (st_);\
    NANO_CDR_u16_set_ptr(\
        NANO_XRCE_InlineHeaderBuffer_sn_ptr((s_)),\
        NANO_XRCE_SeqNum_to_u16(&start_sn_));\
    if (NANO_XRCE_SessionId_has_client_key((ss_)))\
    {\
        *NANO_XRCE_InlineHeaderBuffer_clientkey_ptr((s_)) = (k_);\
    }\
    NANO_MessageBuffer_set_data_len((s_),\
        NANO_XRCE_InlineHeaderBuffer_length(s_));\
}

void
NANO_XRCE_InlineHeaderBuffer_update_next(
    NANO_XRCE_InlineHeaderBuffer *const self,
    const NANO_XRCE_SubmessageId submsg_id,
    const NANO_XRCE_SubmessageFlags submsg_flags,
    NANO_MessageBuffer *const submsg_payload);

#define NANO_XRCE_InlineHeaderBuffer_update_next(s_,submsg_id_,submsg_flags_,submsg_payload_) \
{\
    NANO_usize submsg_len_ = 0;\
    NANO_MessageBuffer_data_len_msg((submsg_payload_),&submsg_len_);\
    NANO_PCOND(submsg_len_ <= NANO_U16_MAX)\
    NANO_XRCE_InlineHeaderBuffer_set_submsg(\
        (s_), (submsg_id_), (submsg_flags_), submsg_len_);\
    NANO_MessageBuffer_unlink((s_));\
    NANO_MessageBuffer_append((s_),(submsg_payload_));\
    if (NANO_MessageBuffer_flags(\
            (submsg_payload_), NANO_XRCE_MESSAGEFLAGS_DISCOVERY))\
    {\
        NANO_MessageBuffer_flags_set((s_), NANO_XRCE_MESSAGEFLAGS_DISCOVERY);\
    }\
    else\
    {\
        NANO_MessageBuffer_flags_unset((s_), NANO_XRCE_MESSAGEFLAGS_DISCOVERY);\
    }\
}

/*i @} *//* nano_api_xrce_impl */


/******************************************************************************
 *                            StreamStorage
 ******************************************************************************/

#if NANO_CPP
#define NANO_XRCE_CUSTOMSTORAGE_CPP_CONSTRUCTOR(type_name_,init_fn_) \
    type_name_()\
    {\
        init_fn_\
    }
#else
#define NANO_XRCE_CUSTOMSTORAGE_CPP_CONSTRUCTOR(type_name_,init_fn_)
#endif /* NANO_CPP */

/*e
 * @addtogroup nano_api_storage_stream
 * @{
 */

/* The maximum size of an "inline payload" depends on the enabled 
   features. At a minimum, it must be big enough to hold a CREATE_CLIENT
   submessage (14-16-20). Next biggest payloads are CREATE (12), and
   READ_DATA (8-[8+1+4+padding=16]). */

#if NANO_FEAT_CONTENT_FILTER && \
    (!NANO_FEAT_PROPERTY || \
        !NANO_FEAT_TYPED_SEQUENCE || \
        !NANO_FEAT_MTU_IN_CLIENT_REPR)
#define NANO_XRCE_STREAM_INLINE_PAYLOAD_HEADER_SIZE \
    NANO_XRCE_READDATAPAYLOAD_HEADER_SERIALIZED_SIZE_MAX
#else
/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_STREAM_INLINE_PAYLOAD_HEADER_SIZE \
    NANO_XRCE_CREATECLIENTPAYLOAD_HEADER_SERIALIZED_SIZE_MAX
#endif

/* the maximum size of the "tail payload" is either (2+padding=4) for
   CREATE messages, and (1+8+padding=12) for READ_DATA with delivery
   control (and possibly a bool and padding). */

#if NANO_FEAT_SUBSCRIBE && NANO_FEAT_DELIVERY_CTRL
/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_STREAM_INLINE_PAYLOAD_TAIL_SIZE \
    NANO_XRCE_READDATAPAYLOAD_TAIL_SERIALIZED_SIZE_MAX
#else
#define NANO_XRCE_STREAM_INLINE_PAYLOAD_TAIL_SIZE \
    NANO_XRCE_CREATEPAYLOAD_TAIL_SERIALIZED_SIZE_MAX
#endif

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_STREAM_INLINE_BUFFER_HEADER_SIZE \
    NANO_MESSAGEBUFFER_INLINE_SIZE(\
        NANO_XRCE_STREAM_INLINE_PAYLOAD_HEADER_SIZE)

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_STREAM_INLINE_BUFFER_HEADER_SIZE_BYTES \
    (NANO_XRCE_STREAM_INLINE_BUFFER_HEADER_SIZE * sizeof(NANO_MessageBufferData))

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_STREAM_INLINE_BUFFER_TAIL_SIZE \
    NANO_MESSAGEBUFFER_INLINE_SIZE(\
        NANO_XRCE_STREAM_INLINE_PAYLOAD_TAIL_SIZE)

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_STREAM_INLINE_BUFFER_TAIL_SIZE_BYTES \
    (NANO_XRCE_STREAM_INLINE_BUFFER_TAIL_SIZE * sizeof(NANO_MessageBufferData))

/*e
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_StreamStorageI
{
    /*e
     * @brief TODO
     * 
     */
    NANO_MessageBufferData *header;
    /*e
     * @brief TODO
     * 
     */
    NANO_usize header_max;
    /*e
     * @brief TODO
     * 
     */
    NANO_usize header_len;
    /*e
     * @brief TODO
     * 
     */
    NANO_MessageBufferData *payload;
    /*e
     * @brief TODO
     * 
     */
    NANO_usize payload_max;
    /*e
     * @brief TODO
     * 
     */
    NANO_usize payload_len;
    /*e
     * @brief TODO
     * 
     */
    NANO_MessageBufferData *payload_tail;
    /*e
     * @brief TODO
     * 
     */
    NANO_usize payload_tail_max;
    /*e
     * @brief TODO
     * 
     */
    NANO_usize payload_tail_len;
    /*e
     * @brief TODO
     * 
     */
    NANO_MessageBuffer *payload_user;
    /*e
     * @brief TODO
     * 
     */
    NANO_usize payload_user_max;
    /*e
     * @brief TODO
     * 
     */
    NANO_usize payload_user_len;
    /*e
     * @brief TODO
     * 
     */
    NANO_MessageBufferData *payload_in;
    /*e
     * @brief TODO
     * 
     */
    NANO_usize payload_in_max;
    /*e
     * @brief TODO
     * 
     */
    NANO_usize payload_in_len;

#if NANO_CPP

    NANO_XRCE_StreamStorageI()
    :
        header(NULL),
        header_max(0),
        header_len(0),
        payload(NULL),
        payload_max(0),
        payload_len(0),
        payload_tail(NULL),
        payload_tail_max(0),
        payload_tail_len(0),
        payload_user(NULL),
        payload_user_max(0),
        payload_user_len(0),
        payload_in(NULL),
        payload_in_max(0),
        payload_in_len(0)
    {}

#endif /* NANO_CPP */
} NANO_XRCE_StreamStorage;

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_STREAMSTORAGE_INITIALIZER \
{\
    NULL, /* header */\
    0,    /* header_max */\
    0,    /* header_len */\
    NULL, /* payload */\
    0,    /* payload_max */\
    0,    /* payload_len */\
    NULL, /* payload_tail */\
    0,    /* payload_tail_max */\
    0,    /* payload_tail_len */\
    NULL, /* payload_user */\
    0,    /* payload_user_max */\
    0,    /* payload_user_len */\
    NULL, /* payload_in */\
    0,    /* payload_in_max */\
    0     /* payload_in_len */\
}

/*e
 * @brief TODO
 * 
 * @param self 
 * @param header 
 * @param header_max 
 * @param payload 
 * @param payload_max 
 * @param payload_tail 
 * @param payload_tail_max 
 * @param payload_user 
 * @param payload_user_max 
 * @param payload_in 
 * @param payload_in_max 
 */
void
NANO_XRCE_StreamStorage_initialize(
    NANO_XRCE_StreamStorage *const self,
    NANO_MessageBufferData *const header,
    const NANO_usize header_max,
    NANO_MessageBufferData *const payload,
    const NANO_usize payload_max,
    NANO_MessageBufferData *const payload_tail,
    const NANO_usize payload_tail_max,
    NANO_MessageBuffer *const payload_user,
    const NANO_usize payload_user_max,
    NANO_MessageBufferData *const payload_in,
    const NANO_usize payload_in_max);

#if 1
#define NANO_XRCE_StreamStorage_initialize(s_,h_,hmx_,p_,pmx_,pt_,ptmx_,pu_,pumx_,fin_,finmx_) \
{\
    NANO_PCOND((s_) != NULL)\
    NANO_PCOND((h_) != NULL && (hmx_) > 0)\
    NANO_PCOND((p_) != NULL && (pmx_) > 0)\
    NANO_PCOND(((pt_) != NULL && (ptmx_) > 0) || \
                    ((pt_) == NULL && (ptmx_) == 0))\
    NANO_PCOND(((pu_) != NULL && (pumx_) > 0) || \
                    ((pu_) == NULL && (pumx_) == 0))\
    (s_)->header = (h_);\
    (s_)->header_max = (hmx_);\
    (s_)->header_len = 0;\
    (s_)->payload = (p_);\
    (s_)->payload_max = (pmx_);\
    (s_)->payload_len = 0;\
    (s_)->payload_tail = (pt_);\
    (s_)->payload_tail_max = (ptmx_);\
    (s_)->payload_tail_len = 0;\
    (s_)->payload_user = (pu_);\
    (s_)->payload_user_max = (pumx_);\
    (s_)->payload_user_len = 0;\
    (s_)->payload_in = (fin_);\
    (s_)->payload_in_max = (finmx_);\
    (s_)->payload_in_len = 0;\
}
#endif

/*i
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_NoneStreamStorageI
{
    NANO_XRCE_StreamStorage base;
    NANO_MessageBufferData
        header[1][
            NANO_MESSAGEBUFFER_INLINE_SIZE(
                NANO_XRCE_INLINEHEADERBUFFER_INLINE_SIZE_BYTES)];
    NANO_MessageBufferData 
        payload[1][
            NANO_MESSAGEBUFFER_INLINE_SIZE(
                NANO_XRCE_STREAM_INLINE_BUFFER_HEADER_SIZE_BYTES)];

#if NANO_CPP

    NANO_XRCE_NoneStreamStorageI()
    : base()
    {
        NANO_OSAPI_Memory_zero(this->header, sizeof(this->header));
        NANO_OSAPI_Memory_zero(this->payload, sizeof(this->payload));
    }

#endif /* NANO_CPP */
} NANO_XRCE_NoneStreamStorage;


#define NANO_XRCE_NONESTREAMSTORAGE_INITIALIZER \
{\
    NANO_XRCE_STREAMSTORAGE_INITIALIZER, /* base */\
    { { 0 } }, /* header */\
    { { 0 } } /* payload */\
}

void
NANO_XRCE_NoneStreamStorage_initialize(
    NANO_XRCE_NoneStreamStorage *const self,
    const NANO_XRCE_SessionId session_id,
    const NANO_XRCE_ClientKey key,
    const NANO_XRCE_StreamId stream_id);

#define NANO_XRCE_NoneStreamStorage_initialize(s_) \
    NANO_XRCE_StreamStorage_initialize(&(s_)->base,\
        (NANO_MessageBufferData*)(s_)->header, 1,\
        (NANO_MessageBufferData*)(s_)->payload, 1,\
        NULL, 0,\
        NULL, 0,\
        NULL, 0)

/** @} *//* nano_api_storage_stream */

/**
 * @addtogroup nano_api_storage_stream
 * @{
 */

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_CUSTOMSTREAMSTORAGE_PAYLOAD_IN_SIZE(max_payload_in_) \
    NANO_OSAPI_Memory_align_size_up(\
        (max_payload_in_ + \
            NANO_XRCE_SUBMESSAGEHEADER_SERIALIZED_SIZE_MAX),\
        sizeof(NANO_MessageBufferData))

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_CUSTOMSTREAMSTORAGE_INITIALIZER \
{\
    NANO_XRCE_STREAMSTORAGE_INITIALIZER, /* base */\
    { { 0 } }, /* header */\
    { { 0 } }, /* payload */\
    { { 0 } }, /* payload_tail */\
    { 0 }, /* payload_in */\
    { NANO_MESSAGEBUFFER_INITIALIZER } /* payload_user */\
}

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_CUSTOMSTREAMSTORAGE_TYPE_MEMBERS(max_header_, max_payload_, max_payload_tail_, max_payload_user_, max_payload_in_)\
    NANO_XRCE_StreamStorage base;\
    NANO_MessageBufferData\
        header[max_header_]\
              [NANO_MESSAGEBUFFER_INLINE_SIZE(\
                    NANO_XRCE_INLINEHEADERBUFFER_INLINE_SIZE_BYTES)];\
    NANO_MessageBufferData \
        payload[max_payload_]\
               [NANO_MESSAGEBUFFER_INLINE_SIZE(\
                   NANO_XRCE_STREAM_INLINE_BUFFER_HEADER_SIZE_BYTES)];\
    NANO_MessageBufferData \
        payload_tail[max_payload_tail_]\
                    [NANO_MESSAGEBUFFER_INLINE_SIZE(\
                        NANO_XRCE_STREAM_INLINE_BUFFER_TAIL_SIZE_BYTES)];\
    NANO_MessageBufferData \
        payload_in[\
            NANO_XRCE_CUSTOMSTREAMSTORAGE_PAYLOAD_IN_SIZE(max_payload_in_) /\
                    sizeof(NANO_MessageBufferData)];\
    NANO_MessageBuffer\
        payload_user[max_payload_user_]

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_CUSTOMSTREAMSTORAGE_INITIALIZE(s_) \
{\
    NANO_OSAPI_Memory_zero((void*)(s_), sizeof(*(s_)));\
    NANO_XRCE_StreamStorage_initialize(&(s_)->base,\
        (NANO_MessageBufferData*)(s_)->header,\
        (sizeof((s_)->header) / \
            (NANO_MESSAGEBUFFER_INLINE_SIZE(\
                NANO_XRCE_INLINEHEADERBUFFER_INLINE_SIZE_BYTES) * \
                sizeof(NANO_MessageBufferData))),\
        (NANO_MessageBufferData*)(s_)->payload,\
        (sizeof((s_)->payload) / \
            (NANO_MESSAGEBUFFER_INLINE_SIZE(\
                NANO_XRCE_STREAM_INLINE_BUFFER_HEADER_SIZE_BYTES) * \
                sizeof(NANO_MessageBufferData))),\
        (NANO_MessageBufferData*)(s_)->payload_tail,\
        (sizeof((s_)->payload_tail) / \
            (NANO_MESSAGEBUFFER_INLINE_SIZE(\
                NANO_XRCE_STREAM_INLINE_BUFFER_TAIL_SIZE_BYTES) * \
                sizeof(NANO_MessageBufferData))),\
        (s_)->payload_user,\
        (sizeof((s_)->payload_user) / sizeof(NANO_MessageBuffer)),\
        (s_)->payload_in,\
        sizeof((s_)->payload_in));\
}

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_SIMPLESTREAMSTORAGE_TYPE_MEMBERS(send_queue_len_,max_payload_in_) \
    NANO_XRCE_CUSTOMSTREAMSTORAGE_TYPE_MEMBERS(\
        send_queue_len_ /* header */,\
        send_queue_len_ /* payload */,\
        ((send_queue_len_ >= 2)?(send_queue_len_/2):send_queue_len_) /* payload_tail */,\
        (send_queue_len_ * 2) /* payload_user */,\
        max_payload_in_ /* payload_in */)

/*e
 * @brief 
 * 
 */
#define NANO_XRCE_SIMPLESTREAMSTORAGE_INITIALIZE(s_) \
    NANO_XRCE_CUSTOMSTREAMSTORAGE_INITIALIZE(s_)

#if NANO_FEAT_AGENT
#define NANO_XRCE_DEFAULTBESTEFFORTSTREAMSTORAGE_SIZE       16
#else
/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_DEFAULTBESTEFFORTSTREAMSTORAGE_SIZE       1
#endif /* NANO_FEAT_AGENT */

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_DefaultBestEffortStreamStorage_initialize(s_) \
    NANO_XRCE_SIMPLESTREAMSTORAGE_INITIALIZE((s_))

/*e
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_DefaultBestEffortStreamStorageI
{
    NANO_XRCE_SIMPLESTREAMSTORAGE_TYPE_MEMBERS(
        NANO_XRCE_DEFAULTBESTEFFORTSTREAMSTORAGE_SIZE, 0);
    
    NANO_XRCE_CUSTOMSTORAGE_CPP_CONSTRUCTOR(
        NANO_XRCE_DefaultBestEffortStreamStorageI,
        NANO_XRCE_DefaultBestEffortStreamStorage_initialize(this);)

} NANO_XRCE_DefaultBestEffortStreamStorage;

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_DEFAULTBESTEFFORTSTREAMSTORAGE_INITIALIZER \
    NANO_XRCE_CUSTOMSTREAMSTORAGE_INITIALIZER

/** @} *//* nano_api_storage_stream */

/**
 * @addtogroup nano_api_storage_stream
 * @{
 */

#if NANO_FEAT_RELIABILITY

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_DefaultReliableStreamStorage_initialize(s_) \
    NANO_XRCE_SIMPLESTREAMSTORAGE_INITIALIZE((s_))

/*e
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_DefaultReliableStreamStorageI
{
    NANO_XRCE_SIMPLESTREAMSTORAGE_TYPE_MEMBERS(
        NANO_LIMIT_RELIABLESTREAM_SENDQUEUE_MAX_LENGTH, 0);
    
    NANO_XRCE_CUSTOMSTORAGE_CPP_CONSTRUCTOR(
        NANO_XRCE_DefaultReliableStreamStorageI,
        NANO_XRCE_DefaultReliableStreamStorage_initialize(this);)
} NANO_XRCE_DefaultReliableStreamStorage;

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_DEFAULTRELIABLESTREAMSTORAGE_INITIALIZER \
    NANO_XRCE_CUSTOMSTREAMSTORAGE_INITIALIZER

#endif /* NANO_FEAT_RELIABILITY */

/** @} *//* nano_api_storage_stream */

/******************************************************************************
 *                                Stream
 ******************************************************************************/
/*i
 * @addtogroup nano_api_session_streams_stream
 * @{
 */
typedef struct NANODllExport NANO_XRCE_StreamI
{
    NANO_XRCE_StreamStorage *storage;
    NANO_XRCE_SeqNum next_in;
    NANO_XRCE_SeqNum next_out;
    NANO_XRCE_StreamId id;
    NANO_XRCE_SessionId session;

#if NANO_CPP

    NANO_XRCE_StreamI()
    : storage(NULL),
      next_in(),
      next_out(),
      id(NANO_XRCE_STREAMID_NONE),
      session(NANO_XRCE_SESSIONID_NONE_WITH_CLIENT)
    {}

#endif /* NANO_CPP */
} NANO_XRCE_Stream;

#define NANO_XRCE_STREAM_INITIALIZER \
{\
    NULL, /* storage */\
    NANO_XRCE_SEQNUM_INITIALIZER,  /* next_in */\
    NANO_XRCE_SEQNUM_INITIALIZER,   /* next_out */\
    NANO_XRCE_STREAMID_NONE, /* id */\
    NANO_XRCE_SESSIONID_NONE_WITH_CLIENT /* session */\
}

void
NANO_XRCE_Stream_initialize(
    NANO_XRCE_Stream *const self,
    NANO_XRCE_StreamStorage *const storage,
    const NANO_XRCE_SessionId session_id,
    const NANO_XRCE_ClientKey key,
    const NANO_XRCE_StreamId stream_id);

#define NANO_XRCE_Stream_initialize(s_,st_,ss_,k_,sid_) \
{\
    NANO_PCOND((s_) != NULL)\
    NANO_PCOND((st_) != NULL)\
    (s_)->storage = (st_);\
    (s_)->id = (sid_);\
    (s_)->session = (ss_);\
    NANO_LOG_DEBUG("STREAM initialized",\
        NANO_LOG_PTR("self",(s_))\
        NANO_LOG_PTR("storage",(s_)->storage)\
        NANO_LOG_STREAMID("stream_id", NANO_XRCE_Stream_id((s_)))\
        if ((s_)->storage != NULL)\
        {\
            NANO_LOG_USIZE("tot_headers",(s_)->storage->header_max)\
        }\
        else\
        {\
            NANO_LOG_USIZE("tot_headers",0)\
        }\
        )\
}

NANO_XRCE_StreamId
NANO_XRCE_Stream_id(const NANO_XRCE_Stream *const self);
#define NANO_XRCE_Stream_id(s_) \
    ((s_)->id)

NANO_XRCE_SessionId
NANO_XRCE_Stream_session_id(const NANO_XRCE_Stream *const self);
#define NANO_XRCE_Stream_session_id(s_) \
    ((s_)->session)

NANO_XRCE_SeqNum*
NANO_XRCE_Stream_sn_next_out(const NANO_XRCE_Stream *const self);
#define NANO_XRCE_Stream_sn_next_out(s_) \
    (&(s_)->next_out)

NANO_XRCE_SeqNum*
NANO_XRCE_Stream_sn_next_in(const NANO_XRCE_Stream *const self);
#define NANO_XRCE_Stream_sn_next_in(s_) \
    (&(s_)->next_in)

void
NANO_XRCE_Stream_update_sessionid(
    NANO_XRCE_Stream *const self,
    const NANO_XRCE_SessionId id);

#define NANO_XRCE_Stream_update_sessionid(s_,id_) \
{\
    (s_)->session = (id_);\
}

void
NANO_XRCE_Stream_reset_state(NANO_XRCE_Stream *const self);

#define NANO_XRCE_Stream_reset_state(s_) \
{\
    NANO_XRCE_SeqNum sn_zero_ = NANO_XRCE_SEQNUM_INITIALIZER;\
    (s_)->next_out = sn_zero_;\
    (s_)->next_in = sn_zero_;\
    NANO_XRCE_SeqNum_plusplus(&(s_)->next_out);\
    NANO_XRCE_SeqNum_plusplus(&(s_)->next_in);\
    NANO_LOG_DEBUG("STREAM state RESET",\
        NANO_LOG_PTR("self",(s_))\
        NANO_LOG_STREAMID("stream_id", NANO_XRCE_Stream_id((s_)))\
        NANO_LOG_SN("next_in", (s_)->next_in)\
        NANO_LOG_SN("next_out", (s_)->next_out))\
}

NANO_bool
NANO_XRCE_Stream_is_reliable(const NANO_XRCE_Stream *const self);

#define NANO_XRCE_Stream_is_reliable(s_) \
    NANO_XRCE_StreamId_is_reliable(NANO_XRCE_Stream_id((s_)))

/******************************************************************************
 *                            None Stream
 ******************************************************************************/

typedef struct NANODllExport NANO_XRCE_NoneStreamI
{
    NANO_XRCE_Stream base;
    NANO_XRCE_NoneStreamStorage storage;


#if NANO_CPP

    NANO_XRCE_NoneStreamI()
    : base(),
      storage()
    {}

#endif /* NANO_CPP */
} NANO_XRCE_NoneStream;

#define NANO_XRCE_NONESTREAM_INITIALIZER \
{\
    NANO_XRCE_STREAM_INITIALIZER, /* base */\
    NANO_XRCE_NONESTREAMSTORAGE_INITIALIZER /* storage */ \
}

void
NANO_XRCE_NoneStream_initialize(
    NANO_XRCE_NoneStream *const self,
    const NANO_XRCE_SessionId session_id,
    const NANO_XRCE_ClientKey key);

#define NANO_XRCE_NoneStream_initialize(s_,ss_,k_) \
{\
    const NANO_XRCE_NoneStream def_stream_ = NANO_XRCE_NONESTREAM_INITIALIZER;\
    NANO_PCOND((s_) != NULL)\
    *(s_) = def_stream_;\
    NANO_XRCE_NoneStreamStorage_initialize(&(s_)->storage);\
    NANO_XRCE_Stream_initialize(\
        &(s_)->base,\
        &(s_)->storage.base,\
        (ss_),\
        (k_),\
        NANO_XRCE_STREAMID_NONE);\
    NANO_XRCE_Stream_reset_state(&(s_)->base);\
}

/*i @} *//* nano_api_session_streams_stream */

/******************************************************************************
 *                            Best-Effort Stream
 ******************************************************************************/
/*i
 * @addtogroup nano_api_session_streams_stream
 * @{
 */

typedef struct NANODllExport NANO_XRCE_BestEffortStreamI
{
    NANO_XRCE_Stream base;


#if NANO_CPP

    NANO_XRCE_BestEffortStreamI()
    : base()
    {}

#endif /* NANO_CPP */
} NANO_XRCE_BestEffortStream;

#define NANO_XRCE_BESTEFFORTSTREAM_INITIALIZER \
{\
    NANO_XRCE_STREAM_INITIALIZER, /* base */\
}

void
NANO_XRCE_BestEffortStream_initialize(
    NANO_XRCE_BestEffortStream *const self,
    NANO_XRCE_StreamStorage *const storage,
    const NANO_XRCE_SessionId session_id,
    const NANO_XRCE_ClientKey key,
    const NANO_XRCE_StreamId stream_id);

#define NANO_XRCE_BestEffortStream_initialize(s_,st_,ss_,k_,sid_) \
{\
    const NANO_XRCE_BestEffortStream def_stream_ = \
        NANO_XRCE_BESTEFFORTSTREAM_INITIALIZER;\
    NANO_PCOND((s_) != NULL)\
    NANO_PCOND(!NANO_XRCE_StreamId_is_reliable((sid_)))\
    *(s_) = def_stream_;\
    NANO_XRCE_Stream_initialize(\
        &(s_)->base, (st_), (ss_), (k_), (sid_));\
    NANO_XRCE_Stream_reset_state(&(s_)->base);\
}

/*i @} *//* nano_api_session_streams_stream */

#if NANO_FEAT_RELIABILITY
/******************************************************************************
 *                           ReliableMessageStream
 ******************************************************************************/

/*e
 * @addtogroup nano_api_xrcesupport
 * @{
 */

/*e
 * @brief TODO
 * 
 */
typedef enum NANODllExport NANO_XRCE_ReliableMessageLostReasonI
{
    /*e
     * @brief  TODO
     * 
     */
    NANO_XRCE_RELIABLEMESSAGELOSTREASON_LOST_BY_LOCAL_WRITER = 0,
    /*e
     * @brief TODO
     * 
     */
    NANO_XRCE_RELIABLEMESSAGELOSTREASON_LOST_BY_REMOTE_WRITER = 1
} NANO_XRCE_ReliableMessageLostReason;

const char *
NANO_XRCE_ReliableMessageLostReason_to_str(
    const NANO_XRCE_ReliableMessageLostReason self);

#define NANO_XRCE_ReliableMessageLostReason_to_str(s_) \
    (((s_) == NANO_XRCE_RELIABLEMESSAGELOSTREASON_LOST_BY_LOCAL_WRITER)?\
        "LOST_BY_LOCAL_WRITER" : \
    ((s_) == NANO_XRCE_RELIABLEMESSAGELOSTREASON_LOST_BY_REMOTE_WRITER)?\
        "LOST_BY_REMOTE_WRITER" : \
        "UNKNOWN")

/*e @} *//* nano_api_xrcesupport */


/*i
 * @addtogroup nano_api_session_streams_stream
 * @{
 */
typedef struct NANODllExport NANO_XRCE_ReliableStreamI
{
    NANO_XRCE_Stream base;
    NANO_MessageBufferQueue send_queue;
    
    NANO_bool in_acknack_recvd;
    NANO_XRCE_SeqNum in_acknack_highest_sn;
    NANO_bool in_heartbeat_recvd;
    NANO_XRCE_SeqNum in_heartbeat_highest_sn;
    NANO_XRCE_SeqNum in_heartbeat_lowest_sn;

#if NANO_FEAT_RELIABLESTREAM_RECVQUEUE
    NANO_MessageBuffer
        *recv_queue[NANO_LIMIT_RELIABLESTREAM_RECVQUEUE_MAX_LENGTH];
    NANO_usize recv_queue_len;
#endif
#if NANO_FEAT_FRAGMENT
    NANO_MessageBuffer recv_frag_buffer;
    NANO_usize recv_frag_buffer_size;
    NANO_usize expected_recv_frag_buffer_size;
#endif /* NANO_FEAT_FRAGMENT */


#if NANO_CPP

    NANO_XRCE_ReliableStreamI()
    :
        base(),
        send_queue(),
        in_acknack_recvd(NANO_BOOL_FALSE),
        in_acknack_highest_sn(),
        in_heartbeat_recvd(NANO_BOOL_FALSE),
        in_heartbeat_highest_sn(),
        in_heartbeat_lowest_sn()
#if NANO_FEAT_RELIABLESTREAM_RECVQUEUE
        ,
        recv_queue(),
        recv_queue_len(0)
#endif
#if NANO_FEAT_FRAGMENT
        ,
        recv_frag_buffer(),
        recv_frag_buffer_size(0)
#endif /* NANO_FEAT_FRAGMENT */
    {}

#endif /* NANO_CPP */
} NANO_XRCE_ReliableStream;


#if NANO_FEAT_RELIABLESTREAM_RECVQUEUE
#define NANO_XRCE_RELIABLESTREAM_RECVQUEUE_INITIALIZER \
    , \
    { NULL },\
    0
#else
#define NANO_XRCE_RELIABLESTREAM_RECVQUEUE_INITIALIZER
#endif

#define NANO_XRCE_RELIABLESTREAM_MAX_FRAGMENT_PAYLOAD_SIZE_BYTES(sid_,mtu_) \
    ((mtu_) - NANO_XRCE_SessionId_header_size(sid_))

#define NANO_XRCE_RELIABLESTREAM_MAX_FRAGMENT_WITH_KEY_SIZE_BYTES(mtu_) \
    ((mtu_) - NANO_XRCE_INLINEMESSAGEHEADER_WITH_KEY_SIZE)

#define NANO_XRCE_RELIABLESTREAM_MAX_FRAGMENT_WITHOUT_KEY_SIZE_BYTES(mtu_) \
    ((mtu_) - NANO_XRCE_INLINEMESSAGEHEADER_WITHOUT_KEY_SIZE)


#define NANO_XRCE_RELIABLESTREAM_REQUIRED_FRAG_OUT_BUFFERS(max_user_payload_, max_frag_payload_) \
    ((max_user_payload_) / (max_frag_payload_) + \
        (((max_user_payload_) % (max_frag_payload_))? 1 : 0))

#define NANO_XRCE_RELIABLESTREAM_DATA_FORMAT_PAYLOAD_OVERHEAD \
    (NANO_XRCE_SUBMESSAGEHEADER_SERIALIZED_SIZE_MAX +\
        sizeof(NANO_XRCE_ObjectId) + \
        sizeof(NANO_XRCE_RequestId))

#define NANO_XRCE_RELIABLESTREAM_SENDFRAGBUFFER_SIZE_BYTES(frag_size_, max_frags_) \
    NANO_OSAPI_Memory_align_size_up(\
        NANO_BUFFERPOOL_SIZE(\
            NANO_MESSAGEBUFFER_INLINE_SIZE_BYTES(frag_size_),\
            sizeof(NANO_MessageBufferData),\
            (max_frags_)), sizeof(NANO_MessageBufferData))

#define NANO_XRCE_RELIABLESTREAM_SENDFRAGBUFFER_SIZE(frag_size_, max_frags_) \
    (NANO_XRCE_RELIABLESTREAM_SENDFRAGBUFFER_SIZE_BYTES(frag_size_, max_frags_)  / \
                sizeof(NANO_MessageBufferData))

#define NANO_XRCE_OUTPUT_FRAGMENT_WRITE_DATA_OVERHEAD \
    (NANO_XRCE_SUBMESSAGEHEADER_SERIALIZED_SIZE_MAX + \
    NANO_XRCE_WRITEDATAPAYLOAD_HEADER_SERIALIZED_SIZE_MAX)

#define NANO_XRCE_OUTPUT_FRAGMENTS_POOL_WITH_KEY_SIZE(mtu_, max_payload_) \
    NANO_XRCE_RELIABLESTREAM_SENDFRAGBUFFER_SIZE(\
        NANO_XRCE_RELIABLESTREAM_MAX_FRAGMENT_WITH_KEY_SIZE_BYTES(mtu_),\
        (((max_payload_) /\
            NANO_XRCE_RELIABLESTREAM_MAX_FRAGMENT_WITH_KEY_SIZE_BYTES(mtu_)) + \
        (((max_payload_) %\
            NANO_XRCE_RELIABLESTREAM_MAX_FRAGMENT_WITH_KEY_SIZE_BYTES(mtu_))? 1 : 0)))

#define NANO_XRCE_OUTPUT_FRAGMENTS_POOL_WITHOUT_KEY_SIZE(mtu_, max_payload_) \
    NANO_XRCE_RELIABLESTREAM_SENDFRAGBUFFER_SIZE(\
        NANO_XRCE_RELIABLESTREAM_MAX_FRAGMENT_WITHOUT_KEY_SIZE_BYTES(mtu_),\
        (((max_payload_) /\
            NANO_XRCE_RELIABLESTREAM_MAX_FRAGMENT_WITHOUT_KEY_SIZE_BYTES(mtu_)) + \
        (((max_payload_) %\
            NANO_XRCE_RELIABLESTREAM_MAX_FRAGMENT_WITHOUT_KEY_SIZE_BYTES(mtu_))? 1 : 0)))

#define NANO_XRCE_OUTPUT_FRAGMENTS_POOL_SIZE(session_id_,mtu_, max_payload_) \
    ((NANO_XRCE_SessionId_has_client_key(session_id_))?\
        NANO_XRCE_OUTPUT_FRAGMENTS_POOL_WITH_KEY_SIZE(mtu_,max_payload_) :\
        NANO_XRCE_OUTPUT_FRAGMENTS_POOL_WITHOUT_KEY_SIZE(mtu_,max_payload_))

#if NANO_FEAT_FRAGMENT

#define NANO_XRCE_ReliableStream_recv_frag_buffer(s_) \
    (&(s_)->recv_frag_buffer)


#define NANO_XRCE_RELIABLESTREAM_FRAGMENT_INITIALIZER \
    , \
    NANO_MESSAGEBUFFER_INITIALIZER, /* recv_frag_buffer */\
    0, /* recv_frag_buffer_size */\
    0 /* expected_recv_frag_buffer_size */
#else
#define NANO_XRCE_RELIABLESTREAM_FRAGMENT_INITIALIZER
#endif

#define NANO_XRCE_RELIABLESTREAM_INITIALIZER \
{\
    NANO_XRCE_STREAM_INITIALIZER, /* base */\
    NANO_MESSAGEBUFFERQUEUE_INITIALIZER, /* send_queue */\
    NANO_BOOL_FALSE, /* in_acknack_recvd */\
    NANO_XRCE_SEQNUM_INITIALIZER, /* in_acknack_highest_sn */\
    NANO_BOOL_FALSE, /* in_heartbeat_recvd */\
    NANO_XRCE_SEQNUM_INITIALIZER, /* in_heartbeat_lowest_sn */\
    NANO_XRCE_SEQNUM_INITIALIZER  /* in_heartbeat_highest_sn */\
    NANO_XRCE_RELIABLESTREAM_RECVQUEUE_INITIALIZER /* recv_queue */\
    NANO_XRCE_RELIABLESTREAM_FRAGMENT_INITIALIZER /* fragment state */ \
}

void
NANO_XRCE_ReliableStream_initialize(
    NANO_XRCE_ReliableStream *const self,
    NANO_XRCE_StreamStorage *const storage,
    const NANO_XRCE_SessionId session_id,
    const NANO_XRCE_ClientKey key,
    const NANO_XRCE_StreamId stream_id);

#define NANO_XRCE_ReliableStream_initialize(s_,st_,ss_,k_,sid_) \
{\
    const NANO_XRCE_ReliableStream def_stream_ = \
            NANO_XRCE_RELIABLESTREAM_INITIALIZER;\
    NANO_PCOND((s_) != NULL) \
    NANO_PCOND(NANO_XRCE_StreamId_is_reliable((sid_)))\
    NANO_PCOND((st_) != NULL)\
    *(s_) = def_stream_;\
    NANO_XRCE_Stream_initialize(\
        &(s_)->base, (st_), (ss_), (k_), (sid_));\
    NANO_XRCE_ReliableStream_reset_state((s_),NULL);\
}

#if NANO_FEAT_AGENT
void
NANO_XRCE_ReliableStream_finalize(NANO_XRCE_ReliableStream *const self);
#endif /* NANO_FEAT_AGENT */


#define NANO_XRCE_RELIABLESTREAM_SENDQUEUE_LENGTH_MAX \
    (NANO_XRCE_ACKNACKBITMAP_VALUE_LEN * 8)

void
NANO_XRCE_ReliableStream_queue_send(
    NANO_XRCE_ReliableStream *const self,
    NANO_MessageBuffer *const message);

#define NANO_XRCE_ReliableStream_queue_send(s_,m_) \
{\
    NANO_MessageBufferQueue_insert_last(&(s_)->send_queue, (m_));\
    NANO_LOG_TRACE("QUEUED on RELIABLE STREAM",\
        NANO_LOG_PTR("stream", (s_))\
        NANO_LOG_STREAMID("stream_id", NANO_XRCE_Stream_id(&(s_)->base))\
        NANO_LOG_PTR("msg", (m_))\
        NANO_LOG_MSGHDR("hdr",\
            *((NANO_XRCE_MessageHeader*)\
            NANO_XRCE_InlineHeaderBuffer_session_id_ptr((m_))))\
        NANO_LOG_SUBMSGHDR("submsg",\
            *((NANO_XRCE_SubmessageHeader*)\
            NANO_XRCE_InlineHeaderBuffer_submsgid_ptr((m_))))\
        NANO_LOG_SN("sn", *NANO_XRCE_InlineHeaderBuffer_sn((m_)))\
        NANO_LOG_USIZE("queue_size",NANO_MessageBufferQueue_size(&(s_)->send_queue))\
        if (!NANO_MessageBufferQueue_empty(&(s_)->send_queue))\
        {\
            NANO_MessageBuffer *queued_ = \
                NANO_MessageBufferQueue_head(&(s_)->send_queue);\
            while (queued_ != NULL)\
            {\
                NANO_LOG_SN("q_sn", *NANO_XRCE_InlineHeaderBuffer_sn(queued_))\
                queued_ = NANO_MessageBuffer_next_msg(queued_);\
            }\
        }\
        )\
}


#if NANO_FEAT_FRAGMENT

#define NANO_XRCE_ReliableStream_reset_state_fragment(s_) \
{\
    NANO_MessageBuffer_set_data_len(&(s_)->recv_frag_buffer, 0);\
}

#else
#define NANO_XRCE_ReliableStream_reset_state_fragment(s_)
#endif /* NANO_FEAT_FRAGMENT */

void
NANO_XRCE_ReliableStream_reset_state(
    NANO_XRCE_ReliableStream *const self,
    NANO_XRCE_Session *const paren_session);

#define NANO_XRCE_ReliableStream_reset_state(s_,ps_) \
{\
    NANO_XRCE_Session *sess_ = (ps_);\
    NANO_XRCE_SeqNum def_sn_ = NANO_XRCE_SEQNUM_INITIALIZER;\
    NANO_XRCE_InlineHeaderBuffer *mbuf_send_queue_tail_ =\
        NANO_MessageBufferQueue_tail(&(s_)->send_queue);\
    if (mbuf_send_queue_tail_ != NULL && sess_ != NULL)\
    {\
        if (0 == NANO_XRCE_Session_dismiss_send_queue_up_to(sess_, (s_), \
                    NANO_XRCE_InlineHeaderBuffer_sn(mbuf_send_queue_tail_)))\
        {\
            NANO_LOG_ERROR("FAILED to dismiss send queue",\
                NANO_LOG_KEY("session.key", *NANO_XRCE_Session_key(sess_))\
                NANO_LOG_SESSIONID("session.id", *NANO_XRCE_Session_id(sess_))\
                NANO_LOG_STREAMID("stream_id", NANO_XRCE_Stream_id(&(s_)->base)))\
        }\
    }\
    NANO_XRCE_Stream_reset_state(&(s_)->base);\
    NANO_XRCE_SeqNum_from_u16(&def_sn_, 0xFFFF);\
    (s_)->in_acknack_highest_sn = def_sn_;\
    (s_)->in_acknack_recvd = NANO_BOOL_FALSE;\
    (s_)->in_heartbeat_lowest_sn = def_sn_;\
    (s_)->in_heartbeat_highest_sn = def_sn_;\
    (s_)->in_heartbeat_recvd = NANO_BOOL_FALSE;\
    NANO_XRCE_ReliableStream_reset_state_fragment(s_);\
}

/*i @} *//* nano_api_session_streams_stream */

#endif /* NANO_FEAT_RELIABILITY */

/******************************************************************************
 *                          UserStreamsStorage
 ******************************************************************************/

#if NANO_FEAT_SESSION_USER_STREAMS

typedef struct NANO_XRCE_UserStreamsStorageI
{
#if NANO_FEAT_SESSION_USER_STREAMS_BESTEFFORT
    NANO_XRCE_BestEffortStream *user_streams_be;
    NANO_usize user_streams_be_max;
    NANO_usize user_streams_be_len;
#endif /* NANO_FEAT_SESSION_USER_STREAMS_BESTEFFORT */
#if NANO_FEAT_SESSION_USER_STREAMS_RELIABLE
    NANO_XRCE_ReliableStream *user_streams_rel;
    NANO_usize user_streams_rel_max;
    NANO_usize user_streams_rel_len;
#endif /* NANO_FEAT_SESSION_USER_STREAMS_RELIABLE */

#if NANO_CPP
    NANO_XRCE_UserStreamsStorageI()
    :
#if NANO_FEAT_SESSION_USER_STREAMS_BESTEFFORT
        user_streams_be(NULL),
        user_streams_be_max(0),
        user_streams_be_len(0)
#endif /* NANO_FEAT_SESSION_USER_STREAMS_BESTEFFORT */
#if NANO_FEAT_SESSION_USER_STREAMS_RELIABLE
#if NANO_FEAT_SESSION_USER_STREAMS_BESTEFFORT
        ,
#endif /* NANO_FEAT_SESSION_USER_STREAMS_BESTEFFORT */
        user_streams_rel(NULL),
        user_streams_rel_max(0),
        user_streams_rel_len(0)
#endif /* NANO_FEAT_SESSION_USER_STREAMS_RELIABLE */
    { }
#endif /* NANO_CPP */

} NANO_XRCE_UserStreamsStorage;

#if NANO_FEAT_SESSION_USER_STREAMS_BESTEFFORT
#define NANO_XRCE_USERSTREAMSSTORAGE_INITIALIZE_BESTEFFORT(s_,user_streams_be_, user_streams_be_max_)\
{\
    (s_)->user_streams_be = (user_streams_be_);\
    (s_)->user_streams_be_max = (user_streams_be_max_);\
    (s_)->user_streams_be_len = 0;\
}
#else
#define NANO_XRCE_USERSTREAMSSTORAGE_INITIALIZE_BESTEFFORT(s_,user_streams_be_, user_streams_be_max_)
#endif /* NANO_FEAT_SESSION_USER_STREAMS_BESTEFFORT */
#if NANO_FEAT_SESSION_USER_STREAMS_RELIABLE
#define NANO_XRCE_USERSTREAMSSTORAGE_INITIALIZE_RELIABLE(s_,user_streams_rel_, user_streams_rel_max_)\
{\
    (s_)->user_streams_rel = (user_streams_rel_);\
    (s_)->user_streams_rel_max = (user_streams_rel_max_);\
    (s_)->user_streams_rel_len = 0;\
}
#else
#define NANO_XRCE_USERSTREAMSSTORAGE_INITIALIZE_RELIABLE(s_,user_streams_rel_, user_streams_rel_max_)
#endif /* NANO_FEAT_SESSION_USER_STREAMS_RELIABLE */


#define NANO_XRCE_UserStreamsStorage_initialize(s_, user_streams_be_, user_streams_be_max_, user_streams_rel_, user_streams_rel_max_) \
{\
    NANO_XRCE_USERSTREAMSSTORAGE_INITIALIZE_BESTEFFORT(s_,user_streams_be_,user_streams_be_max_)\
    NANO_XRCE_USERSTREAMSSTORAGE_INITIALIZE_RELIABLE(s_,user_streams_rel_,user_streams_rel_max_)\
}

#if NANO_FEAT_SESSION_USER_STREAMS_BESTEFFORT
#define NANO_XRCE_USERSTREAMSSTORAGE_USER_BE_INITIALIZER \
    NULL,\
    0,\
    0
#define NANO_XRCE_CUSTOMUSERSTREAMSSTORAGE_INITIALIZE_ARGS_BESTEFFORT_POOL(s_) \
    (s_)->user_streams_be
#define NANO_XRCE_CUSTOMUSERSTREAMSSTORAGE_INITIALIZE_ARGS_BESTEFFORT_MAX(s_) \
    sizeof((s_)->user_streams_be)/sizeof(NANO_XRCE_BestEffortStream)
#define NANO_XRCE_CUSTOMUSERSTREAMSSTORAGE_TYPE_MEMBERS_BESTEFFORT(max_streams_be_) \
    ;\
    NANO_XRCE_BestEffortStream user_streams_be[max_streams_be_]
#define NANO_XRCE_CUSTOMUSERSTREAMSSTORAGE_INITIALIZE_BESTEFFORT \
    ,\
    { NANO_XRCE_BESTEFFORTSTREAM_INITIALIZER }
#else
#define NANO_XRCE_USERSTREAMSSTORAGE_USER_BE_INITIALIZER
#define NANO_XRCE_CUSTOMUSERSTREAMSSTORAGE_INITIALIZE_BESTEFFORT
#define NANO_XRCE_CUSTOMUSERSTREAMSSTORAGE_INITIALIZE_ARGS_BESTEFFORT_POOL(s_) \
    NULL
#define NANO_XRCE_CUSTOMUSERSTREAMSSTORAGE_INITIALIZE_ARGS_BESTEFFORT_MAX(s_) \
    0
#define NANO_XRCE_CUSTOMUSERSTREAMSSTORAGE_TYPE_MEMBERS_BESTEFFORT(s_)
#endif /* NANO_FEAT_SESSION_USER_STREAMS_BESTEFFORT */
#if NANO_FEAT_SESSION_USER_STREAMS_RELIABLE
#define NANO_XRCE_USERSTREAMSSTORAGE_USER_REL_INITIALIZER \
    ,\
    NULL,\
    0,\
    0
#define NANO_XRCE_CUSTOMUSERSTREAMSSTORAGE_INITIALIZE_ARGS_RELIABLE_POOL(s_) \
    (s_)->user_streams_rel
#define NANO_XRCE_CUSTOMUSERSTREAMSSTORAGE_INITIALIZE_ARGS_RELIABLE_MAX(s_) \
    sizeof((s_)->user_streams_rel)/sizeof(NANO_XRCE_ReliableStream)
#define NANO_XRCE_CUSTOMUSERSTREAMSSTORAGE_TYPE_MEMBERS_RELIABLE(max_streams_rel_) \
    ;\
    NANO_XRCE_ReliableStream user_streams_rel[max_streams_rel_]
#define NANO_XRCE_CUSTOMUSERSTREAMSSTORAGE_INITIALIZE_RELIABLE \
    ,\
    { NANO_XRCE_RELIABLESTREAM_INITIALIZER }
#else
#define NANO_XRCE_USERSTREAMSSTORAGE_USER_REL_INITIALIZER
#define NANO_XRCE_CUSTOMUSERSTREAMSSTORAGE_INITIALIZE_ARGS_RELIABLE_POOL(s_) \
    NULL
#define NANO_XRCE_CUSTOMUSERSTREAMSSTORAGE_INITIALIZE_ARGS_RELIABLE_MAX(s_) \
    0
#define NANO_XRCE_CUSTOMUSERSTREAMSSTORAGE_TYPE_MEMBERS_RELIABLE(max_streams_rel_)
#endif /* NANO_FEAT_SESSION_USER_STREAMS_RELIABLE */

#define NANO_XRCE_USERSTREAMSSTORAGE_INITIALIZER \
{\
    NANO_XRCE_USERSTREAMSSTORAGE_USER_BE_INITIALIZER \
    NANO_XRCE_USERSTREAMSSTORAGE_USER_REL_INITIALIZER \
}

#define NANO_XRCE_CUSTOMUSERSTREAMSSTORAGE_TYPE_MEMBERS(max_streams_be_, max_streams_rel_)\
    NANO_XRCE_UserStreamsStorage base\
    NANO_XRCE_CUSTOMUSERSTREAMSSTORAGE_TYPE_MEMBERS_BESTEFFORT(max_streams_be_)\
    NANO_XRCE_CUSTOMUSERSTREAMSSTORAGE_TYPE_MEMBERS_RELIABLE(max_streams_rel_)
    

#define NANO_XRCE_CUSTOMUSERSTREAMSSTORAGE_INITIALIZE(s_) \
    NANO_XRCE_UserStreamsStorage_initialize(\
        &(s_)->base,\
        NANO_XRCE_CUSTOMUSERSTREAMSSTORAGE_INITIALIZE_ARGS_BESTEFFORT_POOL(s_),\
        NANO_XRCE_CUSTOMUSERSTREAMSSTORAGE_INITIALIZE_ARGS_BESTEFFORT_MAX(s_),\
        NANO_XRCE_CUSTOMUSERSTREAMSSTORAGE_INITIALIZE_ARGS_RELIABLE_POOL(s_),\
        NANO_XRCE_CUSTOMUSERSTREAMSSTORAGE_INITIALIZE_ARGS_RELIABLE_MAX(s_))

#define NANO_XRCE_CUSTOMUSERSTREAMSSTORAGE_INITIALIZER \
{\
    NANO_XRCE_USERSTREAMSSTORAGE_INITIALIZER /* base */\
    NANO_XRCE_CUSTOMUSERSTREAMSSTORAGE_INITIALIZE_BESTEFFORT \
    NANO_XRCE_CUSTOMUSERSTREAMSSTORAGE_INITIALIZE_RELIABLE \
}


#endif /* NANO_FEAT_SESSION_USER_STREAMS */


/******************************************************************************
 * SessionStorage
 ******************************************************************************/

/*e
 * @brief TODO
 * 
 */

/*e
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_SessionStorageI
{
    /*e
     * @brief TODO
     * 
     */
    NANO_XRCE_StreamStorage *builtin_be;
#if NANO_FEAT_RELIABILITY
    /*e
     * @brief TODO
     * 
     */
    NANO_XRCE_StreamStorage *builtin_rel;
#endif /* NANO_FEAT_RELIABILITY */
#if NANO_FEAT_SESSION_USER_STREAMS
    /*e
     * @brief TODO
     * 
     */
    NANO_XRCE_UserStreamsStorage *streams;
#endif /* NANO_FEAT_SESSION_USER_STREAMS */

#if NANO_CPP
    NANO_XRCE_SessionStorageI()
    :
        builtin_be(NULL)
#if NANO_FEAT_RELIABILITY
        ,
        builtin_rel(NULL)
#endif /* NANO_FEAT_RELIABILITY */
#if NANO_FEAT_SESSION_USER_STREAMS
        ,
        streams(NULL)
#endif /* NANO_FEAT_SESSION_USER_STREAMS */
    { }
#endif /* NANO_CPP */
} NANO_XRCE_SessionStorage;

#if NANO_FEAT_RELIABILITY
#define NANO_XRCE_SESSIONSTORAGE_RELIABILITY_INITIALIZER \
    ,\
    NULL /* builtin_rel */
#define NANO_XRCE_SessionStorage_initialize_reliable(s_, builtin_rel_) \
{\
    (s_)->builtin_rel = (builtin_rel_);\
}
#else
#define NANO_XRCE_SESSIONSTORAGE_RELIABILITY_INITIALIZER
#define NANO_XRCE_SessionStorage_initialize_reliable(s_, builtin_rel_)
#endif /* NANO_FEAT_RELIABILITY */

#if NANO_FEAT_SESSION_USER_STREAMS
#define NANO_XRCE_SESSIONSTORAGE_USER_STREAMS_INITIALIZER \
    ,\
    NULL /* streams */
#define NANO_XRCE_SessionStorage_initialize_user_streams(s_, streams_) \
{\
    (s_)->streams = (streams_);\
}
#else
#define NANO_XRCE_SESSIONSTORAGE_USER_STREAMS_INITIALIZER
#define NANO_XRCE_SessionStorage_initialize_user_streams(s_, streams_)
#endif /* NANO_FEAT_SESSION_USER_STREAMS */

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_SESSIONSTORAGE_INITIALIZER \
{\
    NULL /* builtin_be */\
    NANO_XRCE_SESSIONSTORAGE_RELIABILITY_INITIALIZER \
    NANO_XRCE_SESSIONSTORAGE_USER_STREAMS_INITIALIZER \
}

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_SessionStorage_initialize(s_, builtin_be_, builtin_rel_, streams_) \
{\
    (s_)->builtin_be = (builtin_be_);\
    NANO_XRCE_SessionStorage_initialize_reliable((s_), (builtin_rel_))\
    NANO_XRCE_SessionStorage_initialize_user_streams(s_, (streams_))\
}

/******************************************************************************
 * CustomSessionStorage
 ******************************************************************************/


#if NANO_FEAT_RELIABILITY
#define NANO_XRCE_CUSTOMSESSIONSTORAGE_RELIABILITY_TYPE(type_n_,send_queue_len_,max_payload_in_) \
    typedef struct xrce_str_concat(type_n_,BuiltinReliableStreamStorageI) \
    {\
        NANO_XRCE_SIMPLESTREAMSTORAGE_TYPE_MEMBERS(send_queue_len_,max_payload_in_);\
        NANO_XRCE_CUSTOMSTORAGE_CPP_CONSTRUCTOR(\
            xrce_str_concat(type_n_,BuiltinReliableStreamStorageI),\
            NANO_XRCE_SIMPLESTREAMSTORAGE_INITIALIZE(this);)\
    } xrce_str_concat(type_n_,BuiltinReliableStreamStorage);
#define NANO_XRCE_CUSTOMSESSIONSTORAGE_RELIABILITY_INITIALIZER \
    ,\
    NANO_XRCE_CUSTOMSTREAMSTORAGE_INITIALIZER /* builtin_rel */
#define NANO_XRCE_CUSTOMSESSIONSTORAGE_RELIABILITY_TYPE_MEMBERS(type_n_) \
    ;\
    xrce_str_concat(type_n_,BuiltinReliableStreamStorage) builtin_rel
#define NANO_XRCE_CUSTOMSESSIONSTORAGE_RELIABILITY_INITIALIZE_ARG(s_) \
    &(s_)->builtin_rel.base
#define NANO_XRCE_CUSTOMSESSIONSTORAGE_RELIABILITY_INITIALIZE(s_) \
{\
    NANO_XRCE_CUSTOMSTREAMSTORAGE_INITIALIZE(&(s_)->builtin_rel);\
}
#else
#define NANO_XRCE_CUSTOMSESSIONSTORAGE_RELIABILITY_TYPE(type_n_,send_queue_len_,max_payload_in_)
#define NANO_XRCE_CUSTOMSESSIONSTORAGE_RELIABILITY_INITIALIZER
#define NANO_XRCE_CUSTOMSESSIONSTORAGE_RELIABILITY_TYPE_MEMBERS(type_n_)
#define NANO_XRCE_CUSTOMSESSIONSTORAGE_RELIABILITY_INITIALIZE_ARG(s_) \
    NULL
#define NANO_XRCE_CUSTOMSESSIONSTORAGE_RELIABILITY_INITIALIZE(s_)
#endif /* NANO_FEAT_RELIABILITY */

#if NANO_FEAT_SESSION_USER_STREAMS
#define NANO_XRCE_CUSTOMSESSIONSTORAGE_USER_STREAMS_TYPE(type_n_,max_streams_be_, max_streams_rel_) \
    typedef struct xrce_str_concat(type_n_,UserStreamsStorageI) \
    {\
        NANO_XRCE_CUSTOMUSERSTREAMSSTORAGE_TYPE_MEMBERS(max_streams_be_, max_streams_rel_);\
        NANO_XRCE_CUSTOMSTORAGE_CPP_CONSTRUCTOR(\
            xrce_str_concat(type_n_,UserStreamsStorageI),\
            NANO_XRCE_CUSTOMUSERSTREAMSSTORAGE_INITIALIZE(this);)\
    } xrce_str_concat(type_n_,UserStreamsStorage);
#define NANO_XRCE_CUSTOMSESSIONSTORAGE_USER_STREAMS_INITIALIZER \
    ,\
    NANO_XRCE_CUSTOMUSERSTREAMSSTORAGE_INITIALIZER /* streams */
#define NANO_XRCE_CUSTOMSESSIONSTORAGE_USER_STREAMS_TYPE_MEMBERS(type_n_) \
    ;\
    xrce_str_concat(type_n_,UserStreamsStorage) streams
#define NANO_XRCE_CUSTOMSESSIONSTORAGE_USER_STREAMS_INITIALIZE_ARG(s_) \
    &(s_)->streams.base
#define NANO_XRCE_CUSTOMSESSIONSTORAGE_USER_STREAMS_INITIALIZE(s_) \
{\
    NANO_XRCE_CUSTOMUSERSTREAMSSTORAGE_INITIALIZE(&(s_)->streams);\
}
#else
#define NANO_XRCE_CUSTOMSESSIONSTORAGE_USER_STREAMS_TYPE(type_n_,max_streams_be_, max_streams_rel_)
#define NANO_XRCE_CUSTOMSESSIONSTORAGE_USER_STREAMS_INITIALIZER
#define NANO_XRCE_CUSTOMSESSIONSTORAGE_USER_STREAMS_TYPE_MEMBERS(type_n_)
#define NANO_XRCE_CUSTOMSESSIONSTORAGE_USER_STREAMS_INITIALIZE_ARG(s_) \
    NULL
#define NANO_XRCE_CUSTOMSESSIONSTORAGE_USER_STREAMS_INITIALIZE(s_)
#endif /* NANO_FEAT_SESSION_USER_STREAMS */

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_CUSTOMSESSIONSTORAGE_TYPE_MEMBERS(base_type_name_) \
    NANO_XRCE_SessionStorage base;\
    NANO_XRCE_DefaultBestEffortStreamStorage builtin_be\
    NANO_XRCE_CUSTOMSESSIONSTORAGE_RELIABILITY_TYPE_MEMBERS(base_type_name_)\
    NANO_XRCE_CUSTOMSESSIONSTORAGE_USER_STREAMS_TYPE_MEMBERS(base_type_name_)\

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_CUSTOMSESSIONSTORAGE_DEFINE_TYPES(base_type_name_,builtin_rel_send_queue_len_,builtin_rel_max_payload_in_,max_user_streams_be_,max_user_streams_rel_)\
\
NANO_XRCE_CUSTOMSESSIONSTORAGE_RELIABILITY_TYPE(base_type_name_,builtin_rel_send_queue_len_,builtin_rel_max_payload_in_) \
\
NANO_XRCE_CUSTOMSESSIONSTORAGE_USER_STREAMS_TYPE(base_type_name_,max_user_streams_be_,max_user_streams_rel_) \
\
typedef struct xrce_str_concat(base_type_name_,SessionStorageI)\
{\
    NANO_XRCE_CUSTOMSESSIONSTORAGE_TYPE_MEMBERS(base_type_name_);\
    NANO_XRCE_CUSTOMSTORAGE_CPP_CONSTRUCTOR(\
        xrce_str_concat(base_type_name_,SessionStorageI),\
        NANO_XRCE_CUSTOMSESSIONSTORAGE_INITIALIZE(this);)\
} xrce_str_concat(base_type_name_,SessionStorage)

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_CUSTOMSESSIONSTORAGE_INITIALIZER \
{\
    NANO_XRCE_SESSIONSTORAGE_INITIALIZER, /* base */\
    NANO_XRCE_DEFAULTBESTEFFORTSTREAMSTORAGE_INITIALIZER /* builtin_be */\
    NANO_XRCE_CUSTOMSESSIONSTORAGE_RELIABILITY_INITIALIZER \
    NANO_XRCE_CUSTOMSESSIONSTORAGE_USER_STREAMS_INITIALIZER \
}

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_CUSTOMSESSIONSTORAGE_INITIALIZE(s_) \
{\
    NANO_XRCE_DefaultBestEffortStreamStorage_initialize(&(s_)->builtin_be);\
    NANO_XRCE_CUSTOMSESSIONSTORAGE_RELIABILITY_INITIALIZE(s_)\
    NANO_XRCE_CUSTOMSESSIONSTORAGE_USER_STREAMS_INITIALIZE(s_)\
    NANO_XRCE_SessionStorage_initialize(&(s_)->base,\
        &(s_)->builtin_be.base,\
        NANO_XRCE_CUSTOMSESSIONSTORAGE_RELIABILITY_INITIALIZE_ARG(s_),\
        NANO_XRCE_CUSTOMSESSIONSTORAGE_USER_STREAMS_INITIALIZE_ARG(s_));\
}

/******************************************************************************
 * DefaultSessionStorage
 ******************************************************************************/

NANO_XRCE_CUSTOMSESSIONSTORAGE_DEFINE_TYPES(NANO_XRCE_Default,
    NANO_XRCE_RELIABLESTREAM_SENDQUEUE_LENGTH_MAX,
    0 /* max_payload_in */,
    NANO_LIMIT_SESSION_MAX_USER_STREAMS_BESTEFFORT,
    NANO_LIMIT_SESSION_MAX_USER_STREAMS_RELIABLE);

#define NANO_XRCE_DEFAULTSESSIONSTORAGE_INITIALIZER \
    NANO_XRCE_CUSTOMSESSIONSTORAGE_INITIALIZER

#define NANO_XRCE_DefaultSessionStorage_initialize(s_) \
    NANO_XRCE_CUSTOMSESSIONSTORAGE_INITIALIZE(s_)

/** @} *//* nano_api_storage_session */

/******************************************************************************
 *                              SessionListener
 ******************************************************************************/
/*i
 * @addtogroup nano_api_session_listener
 * @{
 */

typedef struct NANO_XRCE_SessionListenerI NANO_XRCE_SessionListener;

typedef NANO_RetCode
    (*NANO_XRCE_SessionListener_OnSubmessageFn)(
        NANO_XRCE_SessionListener *const self,
        NANO_XRCE_Session *const session,
        const NANO_MessageBuffer *const mbuf,
        const NANO_XRCE_MessageHeader *const msg_hdr,
        const NANO_XRCE_SubmessageHeader *const submsg_hdr,
        const NANO_u8 *const payload);

#if NANO_FEAT_RELIABILITY

typedef void
    (*NANO_XRCE_SessionListener_OnSendCompleteFn)(
        NANO_XRCE_SessionListener *const self,
        NANO_XRCE_Session *const session,
        NANO_XRCE_ReliableStream *const stream,
        const NANO_XRCE_MessageHeader *const msg_hdr,
        const NANO_XRCE_SubmessageHeader *const submsg_hdr,
        NANO_MessageBuffer *const payload);

typedef void
    (*NANO_XRCE_SessionListener_OnMessageLostFn)(
        NANO_XRCE_SessionListener *const self,
        NANO_XRCE_Session *const session,
        NANO_XRCE_ReliableStream *const stream,
        const NANO_XRCE_SeqNum lost,
        const NANO_XRCE_ReliableMessageLostReason reason);

#endif /* NANO_FEAT_RELIABILITY */

struct NANO_XRCE_SessionListenerI
{
    NANO_u8 *user_data;
    NANO_XRCE_SessionListener_OnSubmessageFn on_submsg;
#if NANO_FEAT_RELIABILITY
    NANO_XRCE_SessionListener_OnSendCompleteFn on_send_complete;
    NANO_XRCE_SessionListener_OnMessageLostFn on_msg_lost;
#endif /* NANO_FEAT_RELIABILITY */


#if NANO_CPP

    NANO_XRCE_SessionListenerI() 
    : user_data(NULL),
      on_submsg(NULL)
#if NANO_FEAT_RELIABILITY
      ,
      on_send_complete(NULL),
      on_msg_lost(NULL)
#endif /* NANO_FEAT_RELIABILITY */
    { }

#endif /* NANO_CPP */

};

#if NANO_FEAT_RELIABILITY
#define NANO_XRCE_SESSIONLISTENER_RELIABILITY_INITIALIZER \
        ,\
        NULL,\
        NULL
#else
#define NANO_XRCE_SESSIONLISTENER_RELIABILITY_INITIALIZER
#endif /* NANO_FEAT_RELIABILITY */


#define NANO_XRCE_SESSIONLISTENER_INITIALIZER \
{\
    NULL, /* user_data */\
    NULL /* on_submsg_recvd */\
    NANO_XRCE_SESSIONLISTENER_RELIABILITY_INITIALIZER /* reliability callbacks */\
}

NANO_bool
NANO_XRCE_SessionListener_is_valid(NANO_XRCE_SessionListener *const self);

#define NANO_XRCE_SessionListener_is_valid(s_) \
    NANO_BOOL_TRUE

#define NANO_XRCE_SessionListener_on_submsg(s_,ss_,mbuf_,mh_,sh_,p_) \
    (s_)->on_submsg((s_),(ss_),(mbuf_),(mh_),(sh_),(p_))
    

#if NANO_FEAT_RELIABILITY

#define NANO_XRCE_SessionListener_on_send_complete(s_,ss_,st_,mh_,smh_,m_) \
{\
    if ((s_)->on_send_complete != NULL)\
    {\
        (s_)->on_send_complete((s_),(ss_),(st_),(mh_),(smh_),(m_));\
    }\
}

#define NANO_XRCE_SessionListener_on_message_lost(s_,ss_,st_,lo_,r_) \
{\
    if ((s_)->on_msg_lost != NULL)\
    {\
        (s_)->on_msg_lost((s_),(ss_),(st_),(lo_),(r_));\
    }\
}
#endif /* NANO_FEAT_RELIABILITY */

/*i @} *//* nano_api_session_listener */

/******************************************************************************
 *                                 Session
 ******************************************************************************/

/*i
 * @addtogroup nano_api_session_session
 * @{
 */

typedef struct NANODllExport NANO_XRCE_SessionPropertiesI
{
    NANO_XRCE_SessionListener listener;
    NANO_XRCE_ClientTransport *transport;
    NANO_XRCE_ClientTransportProperties *transport_properties;
    NANO_XRCE_SessionStorage *storage;
    NANO_XRCE_ClientKey key;
    NANO_XRCE_SessionId id;

#if NANO_FEAT_RELIABILITY && NANO_FEAT_SYSTIME
    NANO_Time heartbeat_period;
    NANO_Time acknack_period;
#endif /* NANO_FEAT_RELIABILITY && NANO_FEAT_SYSTIME */

#if NANO_CPP
    NANO_XRCE_SessionPropertiesI():
        listener(),
        transport(),
        transport_properties(),
        storage(),
        key(),
        id()
#if NANO_FEAT_RELIABILITY && NANO_FEAT_SYSTIME
        ,
        heartbeat_period(),
        acknack_period()
#endif /* NANO_FEAT_RELIABILITY && NANO_FEAT_SYSTIME */
    {
#if NANO_FEAT_RELIABILITY
        this->heartbeat_period.sec =
            NANO_LIMIT_SESSION_HEARTBEAT_PERIOD_DEFAULT_SEC;
        this->heartbeat_period.nanosec =
            NANO_LIMIT_SESSION_HEARTBEAT_PERIOD_DEFAULT_NSEC;
        this->acknack_period.sec =
            NANO_LIMIT_SESSION_ACKNACK_PERIOD_DEFAULT_SEC;
        this->acknack_period.nanosec =
            NANO_LIMIT_SESSION_ACKNACK_PERIOD_DEFAULT_NSEC;
#endif /* NANO_FEAT_RELIABILITY */
    }
#endif /*NANO_CPP */
} NANO_XRCE_SessionProperties;

#if NANO_FEAT_RELIABILITY && NANO_FEAT_SYSTIME
#define NANO_XRCE_SESSIONPROPERTIES_PERIOD_INITIALIZER \
    ,\
    {\
        NANO_LIMIT_SESSION_HEARTBEAT_PERIOD_DEFAULT_SEC,\
        NANO_LIMIT_SESSION_HEARTBEAT_PERIOD_DEFAULT_NSEC \
    }, /* heartbeat_period */\
    {\
        NANO_LIMIT_SESSION_ACKNACK_PERIOD_DEFAULT_SEC,\
        NANO_LIMIT_SESSION_ACKNACK_PERIOD_DEFAULT_NSEC \
    }, /* acknack_period */
#else
#define NANO_XRCE_SESSIONPROPERTIES_PERIOD_INITIALIZER
#endif /* NANO_FEAT_RELIABILITY && NANO_FEAT_SYSTIME */

#define NANO_XRCE_SESSIONPROPERTIES_INITIALIZER \
{\
    NANO_XRCE_SESSIONLISTENER_INITIALIZER, /* listener */\
    NULL, /* transport */\
    NULL, /* transport_properties */\
    NULL, /* storage */ \
    NANO_XRCE_CLIENTKEY_INVALID, /* key */\
    NANO_XRCE_SESSIONID_NONE_WITHOUT_CLIENT /* id */ \
    NANO_XRCE_SESSIONPROPERTIES_PERIOD_INITIALIZER \
}

/*i @} *//* nano_api_session_session */

/*i
 * @addtogroup nano_api_session_session
 * @{
 */
struct NANO_XRCE_SessionI
{
    NANO_XRCE_SessionListener listener;
    NANO_XRCE_ClientTransport *transport;
    NANO_XRCE_SessionStorage *storage;
    NANO_usize recvd_messages;
    NANO_XRCE_NoneStream stream_none;
    NANO_XRCE_BestEffortStream stream_builtin_be;
#if NANO_FEAT_RELIABILITY
    NANO_XRCE_ReliableStream stream_builtin_rel;
#endif
    NANO_XRCE_ClientKey key;
    NANO_XRCE_SessionId id;
#if NANO_FEAT_SYSTIME
    NANO_OSAPI_Clock clock;
#if NANO_FEAT_RELIABILITY
    NANO_u64 ts_last_periodic_hb;
    NANO_u64 ts_last_periodic_ack;
    NANO_u64 period_hb;
    NANO_u64 period_ack;
#endif /* NANO_FEAT_RELIABILITY */
#if NANO_FEAT_ASSERT_LIVELINESS
    NANO_u64 ts_last_periodic_assert;
    NANO_u64 period_liveliness;
#endif /* NANO_FEAT_ASSERT_LIVELINESS */
#endif /* NANO_FEAT_SYSTIME */
#if NANO_FEAT_AGENT
    struct REDAFastBufferPool *pool_header;
    struct REDAFastBufferPool *pool_payload;
    struct REDAFastBufferPool *pool_payload_tail;
    struct REDAFastBufferPool *pool_payload_user;
    struct REDAFastBufferPool *pool_payload_in;
#endif /* NANO_FEAT_FRAGMENT */

#if NANO_CPP

    NANO_XRCE_SessionI()
    : listener(),
      transport(NULL),
      storage(NULL),
      recvd_messages(0),
      stream_none(),
      stream_builtin_be(),
#if NANO_FEAT_RELIABILITY
      stream_builtin_rel(),
#endif
      key(),
      id(NANO_XRCE_SESSIONID_NONE_WITH_CLIENT)
#if NANO_FEAT_SYSTIME
      ,
      clock()
#if NANO_FEAT_RELIABILITY
      ,
      ts_last_periodic_hb(0),
      ts_last_periodic_ack(0),
      period_hb(0),
      period_ack(0)
#endif /* NANO_FEAT_RELIABILITY */
#if NANO_FEAT_ASSERT_LIVELINESS
      ,
      ts_last_periodic_assert(0),
      period_liveliness(0)
#endif /* NANO_FEAT_ASSERT_LIVELINESS */
#endif /* NANO_FEAT_SYSTIME */
#if NANO_FEAT_AGENT
      ,
      pool_header(NULL),
      pool_payload(NULL),
      pool_payload_tail(NULL),
      pool_payload_user(NULL),
      pool_payload_in(NULL)
#endif /* NANO_FEAT_FRAGMENT */
    {
        
    }

#endif /* NANO_CPP */

};

#if NANO_FEAT_RELIABILITY
#define NANO_XRCE_SESSION_RELIABILITY_INITIALIZER \
    NANO_XRCE_RELIABLESTREAM_INITIALIZER,
#else
#define NANO_XRCE_SESSION_RELIABILITY_INITIALIZER
#endif /* NANO_FEAT_RELIABILITY */

#if NANO_FEAT_ASSERT_LIVELINESS
#define NANO_XRCE_SESSION_LIVELINESS_INITIALIZER \
    ,\
    0,/* ts_last_periodic_assert */\
    0 /* period_assert_liveliness */
#else
#define NANO_XRCE_SESSION_LIVELINESS_INITIALIZER
#endif /* NANO_FEAT_ASSERT_LIVELINESS */
#if NANO_FEAT_RELIABILITY
#define NANO_XRCE_SESSION_CLOCK_RELIABILITY_INITIALIZER \
    ,\
    0, /* ts_last_periodic_hb */\
    0, /* ts_last_periodic_ack */\
    0, /* period_hb */\
    0 /* period_ack */ 
#else
#define NANO_XRCE_SESSION_CLOCK_RELIABILITY_INITIALIZER
#endif /* NANO_FEAT_RELIABILITY */

#if NANO_FEAT_SYSTIME
#define NANO_XRCE_SESSION_CLOCK_INITIALIZER \
    ,\
    NANO_OSAPI_CLOCK_INITIALIZER \
    NANO_XRCE_SESSION_CLOCK_RELIABILITY_INITIALIZER \
    NANO_XRCE_SESSION_LIVELINESS_INITIALIZER
#else
#define NANO_XRCE_SESSION_CLOCK_INITIALIZER
#endif /* NANO_FEAT_SYSTIME */

#if NANO_FEAT_AGENT
#define NANO_XRCE_SESSION_AGENT_INITIALIZER \
    ,\
    NULL, /* pool_header */\
    NULL, /* pool_payload */\
    NULL, /* pool_payload_tail */\
    NULL, /* pool_payload_user */\
    NULL /* pool_payload_in */

NANODllExport
void
NANO_XRCE_Session_finalize_storage(NANO_XRCE_Session *const self);

#else
#define NANO_XRCE_SESSION_AGENT_INITIALIZER
#endif /* NANO_FEAT_FRAGMENT */


#define NANO_XRCE_SESSION_INITIALIZER \
{\
    NANO_XRCE_SESSIONLISTENER_INITIALIZER, /* listener */\
    NULL, /* transport */\
    NULL, /* storage */\
    0, /* recvd_messages */\
    NANO_XRCE_NONESTREAM_INITIALIZER, /* stream_none */\
    NANO_XRCE_BESTEFFORTSTREAM_INITIALIZER, /* stream_builtin_be */\
    NANO_XRCE_SESSION_RELIABILITY_INITIALIZER /* reliability state */\
    NANO_XRCE_CLIENTKEY_INVALID, /* key */\
    NANO_XRCE_SESSIONID_NONE_WITH_CLIENT /* id */\
    NANO_XRCE_SESSION_CLOCK_INITIALIZER /* clock */\
    NANO_XRCE_SESSION_AGENT_INITIALIZER /* agent */\
}


NANO_usize
NANO_XRCE_Session_min_mtu(const NANO_XRCE_Session *const self);

#define NANO_XRCE_Session_min_mtu(s_) \
    NANO_XRCE_SessionId_header_size(self->id) + \
        NANO_XRCE_CREATECLIENTPAYLOAD_HEADER_SERIALIZED_SIZE_MIN


NANODllExport
NANO_RetCode
NANO_XRCE_Session_initialize(
    NANO_XRCE_Session *const self,
    const NANO_XRCE_SessionProperties *const properties);

void
NANO_XRCE_Session_finalize(NANO_XRCE_Session *const self);

#if NANO_FEAT_AGENT
#define NANO_XRCE_Session_finalize(s_) \
{\
    NANO_XRCE_Session_reset_state((s_));\
    NANO_XRCE_ClientTransport_finalize((s_)->transport);\
    (s_)->transport = NULL;\
    NANO_XRCE_ReliableStream_finalize(&(s_)->stream_builtin_rel);\
}
#else
#define NANO_XRCE_Session_finalize(s_) \
{\
    NANO_XRCE_ClientTransport_finalize((s_)->transport);\
    (s_)->transport = NULL;\
}
#endif

NANO_bool
NANO_XRCE_Session_is_connected(const NANO_XRCE_Session *const self);

#define NANO_XRCE_Session_is_connected(s_) \
    (NANO_XRCE_Stream_session_id(&(s_)->stream_none.base) == (s_)->id)

void
NANO_XRCE_Session_connected(NANO_XRCE_Session *const self);

#define NANO_XRCE_Session_connected(s_) \
{\
    NANO_XRCE_Stream_update_sessionid(&(s_)->stream_none.base, (s_)->id);\
}

void
NANO_XRCE_Session_disconnected(NANO_XRCE_Session *const self);

#define NANO_XRCE_Session_disconnected(s_) \
{\
    if (NANO_XRCE_SessionId_has_client_key((s_)->id))\
    {\
        NANO_XRCE_Stream_update_sessionid(&(s_)->stream_none.base,\
            NANO_XRCE_SESSIONID_NONE_WITH_CLIENT);\
    }\
    else\
    {\
        NANO_XRCE_Stream_update_sessionid(&(s_)->stream_none.base,\
            NANO_XRCE_SESSIONID_NONE_WITHOUT_CLIENT);\
    }\
    NANO_XRCE_Session_reset_state((s_));\
}

NANO_XRCE_ClientKey*
NANO_XRCE_Session_key(NANO_XRCE_Session *const self);

#define NANO_XRCE_Session_key(s_) \
    (&(s_)->key)


NANO_XRCE_SessionId*
NANO_XRCE_Session_id(NANO_XRCE_Session *const self);

#define NANO_XRCE_Session_id(s_) \
    (&(s_)->id)

NANODllExport
NANO_RetCode
NANO_XRCE_Session_send(
    NANO_XRCE_Session *const self,
    NANO_XRCE_Stream *stream,
    const NANO_XRCE_SubmessageId submsg_id,
    const NANO_XRCE_SubmessageFlags submsg_flags,
    NANO_MessageBuffer *const payload,
    NANO_XRCE_SeqNum *const sn_out);

NANODllExport
NANO_XRCE_Stream*
NANO_XRCE_Session_lookup_stream(
    NANO_XRCE_Session *const self,
    const NANO_XRCE_StreamId stream_id);

#if NANO_FEAT_AGENT
NANODllExport
NANO_RetCode
NANO_XRCE_Session_receive_message(
    NANO_XRCE_Session *const self,
    NANO_XRCE_Stream *const stream,
    NANO_MessageBuffer *const msg,
    NANO_bool *const retained_out);

NANODllExport
NANO_bool
NANO_XRCE_Session_parse_xrce_message_header(
    NANO_XRCE_Session *const self,
    NANO_MessageBuffer *const msg,
    NANO_XRCE_SessionId *const id_out,
    NANO_XRCE_StreamId *const stream_id_out);
#endif

#if NANO_FEAT_FRAGMENT
NANODllExport
NANO_RetCode
NANO_XRCE_Session_register_reliable_stream_fragment_storage(
    NANO_XRCE_Session *const self,
    const NANO_XRCE_StreamId stream_id,
    const NANO_MessageBufferType msg_type,
    NANO_u8 *buffer,
    const NANO_usize buffer_size,
    const NANO_usize msg_size);
#endif /* NANO_FEAT_FRAGMENT */


#if NANO_FEAT_SESSION_USER_STREAMS
NANODllExport
NANO_RetCode
NANO_XRCE_Session_register_stream(
    NANO_XRCE_Session *const self,
    const NANO_XRCE_StreamId stream_id,
    NANO_XRCE_StreamStorage *const stream_storage);
#endif /* NANO_FEAT_SESSION_USER_STREAMS */

NANODllExport
NANO_MessageBuffer*
NANO_XRCE_Session_allocate_message(
    NANO_XRCE_Session *const self,
    NANO_XRCE_Stream *stream,
    const NANO_MessageBufferType buffer_type,
    const NANO_usize message_size,
    const NANO_u8 *const external_payload);

#if NANO_FEAT_RELIABILITY

NANODllExport
NANO_usize
NANO_XRCE_Session_dismiss_send_queue_up_to(
    NANO_XRCE_Session *const self,
    NANO_XRCE_ReliableStream *const stream_recv,
    const NANO_XRCE_SeqNum *const msg_sn);

#if NANO_FEAT_AGENT
NANO_RetCode
NANO_XRCE_Session_send_periodic_heartbeats(
    NANO_XRCE_Session *const self,
    const NANO_u64 ms_now,
    const NANO_usize min_queue_len);

NANO_RetCode
NANO_XRCE_Session_send_periodic_acknacks(
    NANO_XRCE_Session *const self,
    const NANO_u64 ms_now);
#endif /* NANO_FEAT_AGENT */

NANODllExport
NANO_RetCode
NANO_XRCE_Session_run_reliability(
    NANO_XRCE_Session *const self,
    const NANO_Timeout timeout_ms);

NANO_RetCode
NANO_XRCE_Session_run_until_acked(
    NANO_XRCE_Session *const self,
    const NANO_XRCE_StreamId stream_id,
    const NANO_XRCE_SeqNum *const sn,
    const NANO_Timeout timeout_ms);

#endif /* NANO_FEAT_RELIABILITY */

NANODllExport
NANO_RetCode
NANO_XRCE_Session_run(
    NANO_XRCE_Session *const self,
    const NANO_usize max_messages,
    const NANO_Timeout timeout_ms);

void
NANO_XRCE_Session_reset_state(NANO_XRCE_Session *const self);

#if NANO_FEAT_SESSION_USER_STREAMS_BESTEFFORT
#define NANO_XRCE_Session_reset_state_user_streams_be(s_) \
{\
    NANO_usize i_ = 0;\
    for (i_ = 0;\
            (s_)->storage->streams != NULL && \
            i_ < (s_)->storage->streams->user_streams_be_len; i_++)\
    {\
        NANO_XRCE_Stream_reset_state(\
            &(s_)->storage->streams->user_streams_be[i_].base);\
    }\
}
#else
#define NANO_XRCE_Session_reset_state_user_streams_be(s_)
#endif /* NANO_FEAT_SESSION_USER_STREAMS_BESTEFFORT */

#if NANO_FEAT_SESSION_USER_STREAMS_RELIABLE
#define NANO_XRCE_Session_reset_state_user_streams_rel(s_) \
{\
    NANO_usize i_ = 0;\
    for (i_ = 0;\
            (s_)->storage->streams != NULL && \
            i_ < (s_)->storage->streams->user_streams_rel_len; i_++)\
    {\
        NANO_XRCE_ReliableStream_reset_state(\
            &(s_)->storage->streams->user_streams_rel[i_], (s_));\
    }\
}
#else
#define NANO_XRCE_Session_reset_state_user_streams_rel(s_)
#endif /* NANO_FEAT_SESSION_USER_STREAMS_RELIABLE */

#if NANO_FEAT_RELIABILITY
#define NANO_XRCE_Session_reset_state_reliability(s_) \
{\
    NANO_XRCE_ReliableStream_reset_state(&(s_)->stream_builtin_rel, (s_));\
    (s_)->ts_last_periodic_ack = 0;\
    (s_)->ts_last_periodic_hb = 0;\
}
#else
#define NANO_XRCE_Session_reset_state_reliability(s_)
#endif /* NANO_FEAT_RELIABILITY */


#define NANO_XRCE_Session_reset_state(s_) \
{\
    NANO_XRCE_Stream_reset_state(&(s_)->stream_builtin_be.base);\
    NANO_XRCE_Session_reset_state_reliability(s_);\
    NANO_XRCE_Session_reset_state_user_streams_be(s_);\
    NANO_XRCE_Session_reset_state_user_streams_rel(s_);\
}

NANO_bool
NANO_XRCE_Session_connecting(const NANO_XRCE_Session *const self);

#define NANO_XRCE_Session_connecting(s_) \
    ((s_)->id == NANO_XRCE_SESSIONID_NONE_WITH_CLIENT ||\
        (s_)->id == NANO_XRCE_SESSIONID_NONE_WITHOUT_CLIENT)

void
NANO_XRCE_Session_release_message(
    NANO_XRCE_Session *const self,
    NANO_XRCE_Stream *const stream,
    NANO_MessageBuffer *const msg);


/*i @} *//* nano_api_session_session */

#endif /* nano_core_xrce_session_h */
