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

#include "nano/nano_client.h"

#define REQ_COMPLETE_LOG        NANO_LOG_DEBUG
#define NEW_REQ_LOG             NANO_LOG_DEBUG

#define CREATECLIENT_BUFFER \
    NANO_MESSAGEBUFFER_INLINE_SIZE(\
        NANO_XRCE_CLIENTREPRESENTATION_SERIALIZED_SIZE_MAX)

#define DELETE_BUFFER \
    NANO_MESSAGEBUFFER_INLINE_SIZE(\
        NANO_XRCE_DELETEPAYLOAD_HEADER_SERIALIZED_SIZE_MAX)

#define WRITEDATA_BUFFER \
    NANO_MESSAGEBUFFER_INLINE_SIZE(\
        NANO_XRCE_WRITEDATAPAYLOAD_HEADER_SERIALIZED_SIZE_MAX)

    

NANO_PRIVATE
void
NANO_XRCE_Client_find_pending_request(
    NANO_XRCE_Client *const self,
    const NANO_XRCE_ObjectId *const obj_id,
    const NANO_u16 req_id,
    const NANO_XRCE_StreamId stream_id,
    const NANO_XRCE_SeqNum *const sn,
    const NANO_bool not_acked,
    NANO_XRCE_ClientRequest **const req_out)
{
    NANO_XRCE_ClientRequest *req = NULL;

    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)
    NANO_PCOND(req_out != NULL)

    *req_out = NULL;

    NANO_LOG_TRACE("FIND request START",
            NANO_LOG_H16("key.req_id", req_id)
            if (obj_id == NULL)
            {
                NANO_LOG_PTR("key.obj_id", obj_id)
            }
            else
            {
                NANO_LOG_OBJID("key.obj_id", *obj_id)
            }
            if (sn == NULL)
            {
                NANO_LOG_PTR("key.sn", sn)
            }
            else
            {
                NANO_LOG_SN("key.sn", *sn)
            }
            NANO_LOG_STREAMID("key.stream", stream_id)
            NANO_LOG_BOOL("key.notacked", not_acked))

    req = self->pending_requests;
    while (req != NULL)
    {
        NANO_LOG_TRACE("FIND request CHECK",
            NANO_LOG_PTR("req",req)
            NANO_LOG_H16("req.req_id", req->req_id)
            NANO_LOG_OBJID("req.obj_id", req->obj_id)
            NANO_LOG_SN("req.sn", req->sn)
            NANO_LOG_STREAMID("req.stream", req->stream_id)
            NANO_LOG_BOOL("req.notacked",
                (req->flags & NANO_XRCE_REQUESTFLAGS_ACKED)))
        if ((req_id == 0 || req->req_id == req_id) &&
            (obj_id == NULL ||
                NANO_OSAPI_Memory_compare(
                    req->obj_id.value,
                    obj_id->value,
                    NANO_XRCE_OBJECTID_VALUE_LEN) == 0) &&
            (sn == NULL ||
                NANO_OSAPI_Memory_compare(
                    req->sn.value,
                    sn->value,
                    NANO_XRCE_SEQNUM_VALUE_LEN) == 0) &&
            (stream_id == NANO_XRCE_STREAMID_NONE ||
                req->stream_id == stream_id) &&
            (!not_acked || !(req->flags & NANO_XRCE_REQUESTFLAGS_ACKED)))
        {
            NANO_LOG_TRACE("FIND request FOUND",
                    NANO_LOG_PTR("req",req))
            break;
        }
        NANO_LOG_TRACE("FIND request IGNORED",
            NANO_LOG_PTR("req",req))
        req = req->next;
    }

    *req_out = req;

    NANO_LOG_FN_EXIT
}



void
NANO_XRCE_Client_find_pending_request_from_token(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientRequestToken *const request_token,
    NANO_XRCE_ClientRequest **const request_out);

#define NANO_XRCE_Client_find_pending_request_from_token(s_, r_tkn_, r_out_)\
{\
    NANO_u16 req_id_ = NANO_XRCE_RequestId_to_u16(&(r_tkn_)->request_id);\
    NANO_XRCE_Client_find_pending_request(\
        (s_),\
        &(r_tkn_)->object_id,\
        (req_id_),\
        (r_tkn_)->stream_id,\
        &(r_tkn_)->sn,\
        NANO_BOOL_FALSE,\
        (r_out_));\
}


void
NANO_XRCE_Client_find_unacked_request_by_sn(
    NANO_XRCE_Client *const self,
    const NANO_XRCE_StreamId stream_id,
    const NANO_XRCE_SeqNum *const sn,
    NANO_XRCE_ClientRequest **const req_out);

#define NANO_XRCE_Client_find_unacked_request_by_sn(s_, s_id_, sn_, r_out_) \
    NANO_XRCE_Client_find_pending_request(\
        (s_), NULL, 0, (s_id_), (sn_), NANO_BOOL_TRUE, (r_out_))


NANO_PRIVATE
NANO_RetCode
NANO_XRCE_Client_on_submessage(
    NANO_XRCE_SessionListener *const listener,
    NANO_XRCE_Session *const session,
    const NANO_MessageBuffer *const mbuf,
    const NANO_XRCE_MessageHeader *const msg_hdr,
    const NANO_XRCE_SubmessageHeader *const submsg_hdr,
    const NANO_u8 *const payload)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_XRCE_ClientRequest *req = NULL;
    NANO_u16 req_id = 0;
    NANO_XRCE_Client *self = NULL;

    
    NANO_LOG_FN_ENTRY

    NANO_PCOND(listener != NULL)
    NANO_PCOND(session != NULL)
    NANO_PCOND(msg_hdr != NULL)
    NANO_PCOND(submsg_hdr != NULL)
    NANO_PCOND(payload != NULL)

    UNUSED_ARG(session);
    UNUSED_ARG(mbuf);

    self = (NANO_XRCE_Client*) listener->user_data;

    NANO_LOG_DEBUG("received SUBMSG",
        NANO_LOG_MSGHDR("msg",*msg_hdr)
        NANO_LOG_SUBMSGHDR("submsg",*submsg_hdr)
        NANO_LOG_BYTES("payload",payload,submsg_hdr->length))

    switch (submsg_hdr->id)
    {
    case NANO_XRCE_SUBMESSAGEID_STATUS_AGENT:
    {
        NANO_XRCE_AgentRepresentation *ag_repr =
            (NANO_XRCE_AgentRepresentation*)payload;
        
        /* If we're not in connecting state, then we can ignore the message
         * since it is unexpected. If the message is too short, then we
         * ignore it. */
        if (!(self->flags & NANO_XRCE_CLIENTFLAGS_CONNECTING))
        {
            NANO_LOG_WARNING_MSG("STATUS_AGENT received while not connecting")
            goto done;
        }

        /* If the message is too short, then we ignore it. */
        if (submsg_hdr->length < 
                NANO_XRCE_STATUSAGENTPAYLOAD_HEADER_SERIALIZED_SIZE_MAX)
        {
            NANO_LOG_WARNING("STATUS_AGENT too short",
                NANO_LOG_U16("length",submsg_hdr->length)
                NANO_LOG_USIZE("expected",
                    NANO_XRCE_STATUSAGENTPAYLOAD_HEADER_SERIALIZED_SIZE_MAX))
            goto done;
        }

        if (!NANO_XRCE_Cookie_is_valid(&ag_repr->xrce_cookie))
        {
            NANO_LOG_WARNING_MSG("invalid XRCE cookie")
            goto done;
        }

        if (!NANO_XRCE_Version_is_compatible(&ag_repr->xrce_version))
        {
            NANO_LOG_WARNING("invalid XRCE version",
                NANO_LOG_U8("major",
                    ag_repr->xrce_version.value[NANO_XRCE_VERSION_MAJOR_BYTE])
                NANO_LOG_U8("minor",
                    ag_repr->xrce_version.value[NANO_XRCE_VERSION_MINOR_BYTE])
                NANO_LOG_U8("own_major", NANO_XRCE_VERSION_MAJOR)
                NANO_LOG_U8("own_minor", NANO_XRCE_VERSION_MINOR))
            goto done;
        }

        NANO_LOG_DEBUG_MSG("connection reply received from agent")

        NANO_XRCE_Client_mark_connected(self);

        self->flags |= NANO_XRCE_CLIENTFLAGS_CONNECTED;

        break;
    }
    case NANO_XRCE_SUBMESSAGEID_STATUS:
    {
        NANO_XRCE_StatusPayload *status = (NANO_XRCE_StatusPayload*)payload;

        if (submsg_hdr->length < 
                NANO_XRCE_STATUSPAYLOAD_SERIALIZED_SIZE_MAX)
        {
            NANO_LOG_WARNING("STATUS too short",
                NANO_LOG_U16("length",submsg_hdr->length))
            goto done;
        }

        req_id = NANO_XRCE_RequestId_to_u16(
                    &status->base.related_request.request_id);

        NANO_XRCE_Client_find_pending_request(
            self,
            &status->base.related_request.object_id,
            req_id,
            NANO_XRCE_STREAMID_NONE,
            NULL,
            NANO_BOOL_FALSE,
            &req);
        if (req == NULL)
        {
            NANO_LOG_WARNING(
                "result received for unknown request", 
                NANO_LOG_REQID("req_id",
                    status->base.related_request.request_id))
            goto done;
        }

        req->result = status->base.result;

        REQ_COMPLETE_LOG("REQUEST COMPLETE", 
            NANO_LOG_PTR("req", req)
            NANO_LOG_SN("req.sn", req->sn)
            NANO_LOG_H16("req.id", req->req_id)
            NANO_LOG_STREAMID("req.stream_id", req->stream_id)
            NANO_LOG_STR("req.kind",
                NANO_XRCE_SubmessageId_as_str(req->submsg_id))
            NANO_LOG_H8("req.flags", req->submsg_flags)
            NANO_LOG_H32("req.status",req->result.status)
            NANO_LOG_RC(req->result.implementation_status)
            NANO_LOG_SUBMSGHDR("status", *submsg_hdr))

#if NANO_FEAT_PUBLISH && NANO_FEAT_RELIABILITY
        if (req->submsg_id == NANO_XRCE_SUBMESSAGEID_WRITE_DATA &&
            NANO_XRCE_ClientRequest_is_acked(req))
        {
            NANO_XRCE_ClientListener_on_write_complete(
                    &self->listener, self, req);
        }
#endif /* NANO_FEAT_PUBLISH && NANO_FEAT_RELIABILITY */


        req->flags |= NANO_XRCE_REQUESTFLAGS_COMPLETE;
        break;
    }
#if NANO_FEAT_SUBSCRIBE
    case NANO_XRCE_SUBMESSAGEID_DATA:
    {
        NANO_XRCE_RelatedObjectRequest *rel_req = 
            (NANO_XRCE_RelatedObjectRequest*)payload;
        const NANO_u8 *data = NULL;
        NANO_XRCE_DataFormat data_fmt = NANO_XRCE_FORMAT_INVALID;
        NANO_usize data_len = 0;
        
        if (submsg_hdr->length < 
                NANO_XRCE_RELATEDOBJECTREQUEST_SERIALIZED_SIZE_MAX)
        {
            NANO_LOG_WARNING("DATA too short",
                NANO_LOG_U16("length",submsg_hdr->length))
            goto done;
        }
        req_id = NANO_XRCE_RequestId_to_u16(&rel_req->request_id);

        NANO_XRCE_Client_find_pending_request(
            self,
            &rel_req->object_id,
            req_id,
            NANO_XRCE_STREAMID_NONE,
            NULL,
            NANO_BOOL_FALSE,
            &req);
        if (req == NULL)
        {
            NANO_LOG_WARNING(
                "DATA received for unknown request", 
                NANO_LOG_SESSIONID("session",msg_hdr->session_id)
                NANO_LOG_STREAMID("stream",msg_hdr->stream_id)
                NANO_LOG_H16("req_id", req_id))
            goto done;
        }

        data_fmt = NANO_XRCE_SubmessageFlags_DATA_format(submsg_hdr->flags);
        data = payload + NANO_XRCE_RELATEDOBJECTREQUEST_SERIALIZED_SIZE_MAX;
        data_len = submsg_hdr->length - 
                        NANO_XRCE_RELATEDOBJECTREQUEST_SERIALIZED_SIZE_MAX;;

        NANO_LOG_TRACE("notifying DATA", 
            NANO_LOG_REQID("sub_id",rel_req->request_id)
            NANO_LOG_U8("format",data_fmt)
            NANO_LOG_PTR("data",data)
            NANO_LOG_USIZE("data_len",data_len)
            NANO_LOG_PTR("req", req))

        NANO_XRCE_ClientListener_on_data(
                &self->listener,
                self,
                msg_hdr->stream_id,
                &rel_req->request_id,
                &req->obj_id,
                data_fmt,
                NANO_XRCE_SubmessageFlags_is_little_endian(submsg_hdr->flags),
                data,
                data_len);
        
        /* Mark request with "DATA" flag so that someone waiting on it may 
           unblock */
        req->result.status = NANO_XRCE_STATUS_OK;
        req->flags |= NANO_XRCE_REQUESTFLAGS_DATA;

        break;
    }
#endif /* NANO_FEAT_SUBSCRIBE */
#if NANO_FEAT_OBJECT_INFO
    case NANO_XRCE_SUBMESSAGEID_INFO:
    {
        NANO_bool has_opt = NANO_BOOL_FALSE;
        const NANO_u8 *ptr = NULL,
                      *ptra = NULL;
        NANO_usize remaining = 0;
        NANO_XRCE_ObjectInfo info = NANO_XRCE_OBJECTINFO_INITIALIZER;
        const NANO_XRCE_BaseObjectReply *reply = 
            (const NANO_XRCE_BaseObjectReply *)payload;
        
        if (submsg_hdr->length < 
                NANO_XRCE_BASEOBJECTREPLY_SERIALIZED_SIZE_MAX)
        {
            NANO_LOG_WARNING("INFO too short",
                NANO_LOG_U16("length",submsg_hdr->length))
            goto done;
        }

        req_id = NANO_XRCE_RequestId_to_u16(&reply->related_request.request_id);
        
        NANO_XRCE_Client_find_pending_request(
            self,
            &reply->related_request.object_id,
            req_id,
            NANO_XRCE_STREAMID_NONE,
            NULL,
            NANO_BOOL_FALSE,
            &req);
        if (req == NULL)
        {
            NANO_LOG_WARNING(
                "INFO received for unknown request", 
                NANO_LOG_SESSIONID("session",msg_hdr->session_id)
                NANO_LOG_STREAMID("stream",msg_hdr->stream_id)
                NANO_LOG_U16("req_id", req_id))
            goto done;
        }

        remaining = submsg_hdr->length -
                        NANO_XRCE_BASEOBJECTREPLY_SERIALIZED_SIZE_MAX;
    
        NANO_LOG_DEBUG("received INFO message",
            NANO_LOG_REQID("req.id",reply->related_request.request_id)
            NANO_LOG_OBJID("req.obj_id",reply->related_request.object_id)
            NANO_LOG_H8("reply.result.status",reply->result.status)
            NANO_LOG_H8("reply.result.impl_status",reply->result.implementation_status)
            NANO_LOG_USIZE("remaining_size", remaining)
            NANO_LOG_USIZE("reply_size", NANO_XRCE_BASEOBJECTREPLY_SERIALIZED_SIZE_MAX))

        ptr = payload + NANO_XRCE_BASEOBJECTREPLY_SERIALIZED_SIZE_MAX;

#define deserialize(t_, action_) \
    ptra = NANO_OSAPI_Memory_align_ptr(ptr, sizeof(t_));\
    remaining -= ptra - ptr;\
    ptr = ptra;\
    if (sizeof(t_) > remaining) \
    {\
        NANO_LOG_ERROR("invalid message received",\
            NANO_LOG_USIZE("required_size",sizeof(t_))\
            NANO_LOG_USIZE("remaining_size",remaining))\
        req->result.status = NANO_XRCE_STATUS_ERR_INVALID_DATA;\
        goto release_info_req;\
    }\
    remaining -= sizeof(t_);\
    {\
        action_ \
    }\
    ptr += sizeof(t_)

        deserialize(NANO_bool,
            has_opt = *((NANO_bool*)ptr););

#if NANO_FEAT_OBJECT_INFO_ACTIVITY
        info.has_activity = has_opt;
        if (info.has_activity)
        {
            deserialize(NANO_bool,
                info.activity.kind = *((NANO_bool*)ptr););
            
            switch (info.activity.kind)
            {
            case NANO_XRCE_OBJK_AGENT:
            {
                NANO_u32 seq_len = 0;
                deserialize(NANO_i16,
                    NANO_i16_deserialize(
                        &info.activity.value.agent.availability,
                        ptr,
                        NANO_XRCE_SubmessageFlags_is_little_endian(
                            submsg_hdr->flags)););
                deserialize(NANO_u32,
                    NANO_u32_deserialize(
                        &seq_len,
                        ptr,
                        NANO_XRCE_SubmessageFlags_is_little_endian(
                            submsg_hdr->flags)););

#if NANO_FEAT_TYPED_SEQUENCE

                if (seq_len > 0)
                {
                    NANO_XRCE_TransportLocator agent_addr =
                            NANO_XRCE_TRANSPORTLOCATOR_INITIALIZER;
                    NANO_bool has_element = NANO_BOOL_FALSE;
                    NANO_SequenceIterator iter = 
                            NANO_SEQUENCEITERATOR_INITIALIZER;

                    NANO_XRCE_TransportLocatorSeq_initialize(
                            &info.activity.value.agent.address_seq);
                    
                    NANO_XRCE_TransportLocatorSeq_set_serialized_buffer(
                        &info.activity.value.agent.address_seq,
                        ptr,
                        remaining,
                        seq_len,
                        NANO_XRCE_SubmessageFlags_is_little_endian(
                            submsg_hdr->flags));
                    
                    NANO_PCOND(NANO_Sequence_flags(
                        &info.activity.value.agent.address_seq.base,
                        NANO_SEQUENCEFLAGS_SERIALIZED))


                    /* We must deserialize each element one by one */
                    NANO_XRCE_TransportLocatorSeq_to_iterator(
                        &info.activity.value.agent.address_seq, &iter);

                    do 
                    {
                        if (NANO_RETCODE_OK != 
                                NANO_SequenceIterator_next(
                                    &iter, &agent_addr, &has_element))
                        {
                            NANO_LOG_ERROR_MSG(
                                "failed to deserialize transport locator")
                            req->result.status = NANO_XRCE_STATUS_ERR_INVALID_DATA;
                            goto release_info_req;
                        }

                        NANO_LOG_TRACE("DESERIALIZED locator",
                            NANO_LOG_BOOL("has_element", has_element)
                            if (has_element)
                            {
                                NANO_LOG_LOCATOR("locator", &agent_addr)
                            }
                            )

#if NANO_FEAT_AGENT_DISCOVERY
                        if (has_element &&
                            NANO_XRCE_Client_discovering(self))
                        {
                            NANO_bool accepted = NANO_BOOL_TRUE;

                            NANO_XRCE_ClientListener_on_agent_discovered(
                                &self->listener, self, &agent_addr, &accepted);

                            if (accepted)
                            {
                                NANO_LOG_DEBUG("DISCOVERED agent locator",
                                    NANO_LOG_LOCATOR("agent",&agent_addr))
                                
                                if (NANO_RETCODE_OK !=
                                        NANO_XRCE_Client_update_locator(
                                            self, 
                                            NANO_XRCE_CLIENTTRANSPORTLOCATOR_AGENT,
                                            &agent_addr))
                                {
                                    NANO_LOG_ERROR("FAILED to update agent locator",
                                        NANO_LOG_LOCATOR("discovered",&agent_addr))
                                    req->result.status =
                                        NANO_XRCE_STATUS_ERR_INVALID_DATA;
                                    goto release_info_req;
                                }
                            }
                            else
                            {
                                NANO_LOG_WARNING("agent locator IGNORED",
                                    NANO_LOG_LOCATOR("agent",&agent_addr))
                            }
                        }
#endif /* NANO_FEAT_AGENT_DISCOVERY */

                    } while (has_element);

                    /* Update the sequence to it's actual serialized size */
                    NANO_XRCE_TransportLocatorSeq_set_serialized_buffer(
                        &info.activity.value.agent.address_seq,
                        ptr,
                        NANO_SequenceIterator_deserialized_size(&iter),
                        seq_len,
                        NANO_XRCE_SubmessageFlags_is_little_endian(
                            submsg_hdr->flags));

                    
                    ptr += NANO_XRCE_TransportLocatorSeq_serialized_size(
                                &info.activity.value.agent.address_seq);
                }

#else
                if (seq_len > 0)
                {
                    NANO_LOG_FEAT_MISSING(NANO_FEAT_TYPED_SEQUENCE)
                    goto done;
                }
#endif /* NANO_FEAT_TYPED_SEQUENCE */
                break;
            }
            case NANO_XRCE_OBJK_DATAREADER:
            {
                deserialize(NANO_u16,
                    NANO_i16_deserialize(
                        &info.activity.value.data_reader.highest_acked_seq_num,
                        ptr,
                        NANO_XRCE_SubmessageFlags_is_little_endian(
                            submsg_hdr->flags)););
                break;
            }
            case NANO_XRCE_OBJK_DATAWRITER:
            {
                deserialize(NANO_u64,
                    NANO_u64_deserialize(
                        &info.activity.value.data_writer.sample_seq_num,
                        ptr,
                        NANO_XRCE_SubmessageFlags_is_little_endian(
                            submsg_hdr->flags)););
                deserialize(NANO_u16,
                    NANO_u16_deserialize(
                        &info.activity.value.data_writer.stream_seq_num,
                        ptr,
                        NANO_XRCE_SubmessageFlags_is_little_endian(
                            submsg_hdr->flags)););
                break;
            }
            default:
            {
                NANO_LOG_ERROR("UNKNOWN activity kind",
                    NANO_LOG_H8("obj_kind",info.activity.kind))
                req->result.status = NANO_XRCE_STATUS_ERR_INVALID_DATA;
                goto release_info_req;
            }
            }
        }
#endif /* NANO_FEAT_OBJECT_INFO_ACTIVITY */


        deserialize(NANO_bool,
            has_opt = *((NANO_bool*)ptr););
#if NANO_FEAT_OBJECT_INFO_CONFIG
        info.has_config = has_opt;
        if (info.has_config)
        {
            NANO_LOG_NOT_IMPLEMENTED("deserialize object variant")
            req->result.status = NANO_XRCE_STATUS_ERR_INVALID_DATA;
                goto release_info_req;
        }
#else
        if (has_opt)
        {
            NANO_LOG_DEBUG("IGNORED object configuration",
                NANO_LOG_USIZE("serialized_size", remaining))
            ptr += remaining;
        }
#endif /* NANO_FEAT_OBJECT_INFO_CONFIG */

        NANO_XRCE_ClientListener_on_info(&self->listener, self, &info);

#undef deserialize

        req->result.status = NANO_XRCE_STATUS_OK;

release_info_req:

        req->flags |= NANO_XRCE_REQUESTFLAGS_COMPLETE;

        break;
    }
#endif /* NANO_FEAT_OBJECT_INFO */
    default:
    {
        NANO_LOG_DEBUG_MSG("submessage IGNORED")
        rc = NANO_RETCODE_OK;
        goto done;
    }
    }
    
    rc = NANO_RETCODE_OK;
    
done:

    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

#if NANO_FEAT_RELIABILITY
NANO_PRIVATE
void
NANO_XRCE_Client_on_send_complete(
    NANO_XRCE_SessionListener *const listener,
    NANO_XRCE_Session *const session,
    NANO_XRCE_ReliableStream *const stream,
    const NANO_XRCE_MessageHeader *const msg_hdr,
    const NANO_XRCE_SubmessageHeader *const submsg_hdr,
    NANO_MessageBuffer *const msg)
{
    NANO_XRCE_Client *self = NULL;
    NANO_XRCE_ClientRequest *req = NULL;

    NANO_LOG_FN_ENTRY

    NANO_PCOND(listener != NULL)
    NANO_PCOND(NANO_XRCE_StreamId_is_reliable(msg_hdr->stream_id))
    NANO_PCOND(msg != NULL)

    UNUSED_ARG(session);
    UNUSED_ARG(stream);
    UNUSED_ARG(msg);

    self = (NANO_XRCE_Client*)listener->user_data;
    NANO_PCOND(self != NULL &&
               &self->session == session)
    
    NANO_LOG_TRACE("RELIABLE send COMPLETE",
        NANO_LOG_MSGHDR("msg",*msg_hdr)
        NANO_LOG_SUBMSGHDR("submsg",*submsg_hdr))

    /* Check if there is a client request for the acked message */
    NANO_XRCE_Client_find_unacked_request_by_sn(
        self, msg_hdr->stream_id, &msg_hdr->sn, &req);
    if (req != NULL)
    {
        REQ_COMPLETE_LOG("request ACKED", 
            NANO_LOG_PTR("req", req)
            NANO_LOG_SN("req.sn", req->sn)
            NANO_LOG_H16("req.id", req->req_id)
            NANO_LOG_STREAMID("req.stream_id", req->stream_id)
            NANO_LOG_SUBMSGHDR("req.submsg", *submsg_hdr)
            NANO_LOG_H32("req.status",req->result.status)
            NANO_LOG_RC(req->result.implementation_status))
        req->flags |= NANO_XRCE_REQUESTFLAGS_ACKED;
    }

    switch (submsg_hdr->id)
    {
#if NANO_FEAT_PUBLISH
    case NANO_XRCE_SUBMESSAGEID_WRITE_DATA:
    {
        NANO_XRCE_DataFormat data_fmt = NANO_XRCE_FORMAT_INVALID;
        NANO_bool dismiss_on_ack = NANO_BOOL_FALSE;

        data_fmt = NANO_XRCE_SubmessageFlags_DATA_format(submsg_hdr->flags);
        if (data_fmt == NANO_XRCE_FORMAT_INVALID)
        {
            return;
        }

        NANO_PCOND(NANO_MessageBuffer_next(msg) != NULL)

#if NANO_FEAT_EPROSIMA
        /* "legacy" implementations do not support the CONFIRM flag 
            so we alwasy dismiss write requests upon ACK */
        dismiss_on_ack = NANO_BOOL_TRUE;
#else
        dismiss_on_ack =
            !NANO_XRCE_SubmessageFlags_WRITEDATA_confirm(submsg_hdr->flags);
#endif /* NANO_FEAT_EPROSIMA */
        
        if (req != NULL && dismiss_on_ack)
        {
            REQ_COMPLETE_LOG("WRITE_DATA complete",
                NANO_LOG_PTR("data",
                    NANO_MessageBuffer_data_ptr(
                        NANO_MessageBuffer_next(msg)))
                NANO_LOG_USIZE("data_len", 
                    NANO_MessageBuffer_data_len(
                        NANO_MessageBuffer_next(msg)))
                NANO_LOG_BOOL("dismiss_on_ack", dismiss_on_ack))
            
            const NANO_XRCE_ResultStatus success_rc =
                        { NANO_XRCE_STATUS_OK, NANO_RETCODE_OK };
        
            NANO_XRCE_ClientListener_on_write_complete(
                &self->listener, self, req);
            
            req->result = success_rc;
            req->flags |= NANO_XRCE_REQUESTFLAGS_COMPLETE;
        }

        break;
    }
#endif /* NANO_FEAT_PUBLISH */
    default:
    {

        break;
    }
    }
    NANO_LOG_FN_EXIT
}

NANO_PRIVATE
void
NANO_XRCE_Client_on_message_lost(
    NANO_XRCE_SessionListener *const listener,
    NANO_XRCE_Session *const session,
    NANO_XRCE_ReliableStream *const stream,
    const NANO_XRCE_SeqNum lost,
    const NANO_XRCE_ReliableMessageLostReason reason)
{
    NANO_XRCE_Client *self = NULL;
    NANO_XRCE_StreamId stream_id = NANO_XRCE_STREAMID_NONE;
    NANO_XRCE_ClientRequest *req = NULL;

    NANO_LOG_FN_ENTRY

    NANO_PCOND(listener != NULL)
    NANO_PCOND(session != NULL)
    NANO_PCOND(stream != NULL)

    UNUSED_ARG(session);
    
    self = (NANO_XRCE_Client*)listener->user_data;
    NANO_PCOND(self != NULL &&
               &self->session == session)

    stream_id = NANO_XRCE_Stream_id(&stream->base);

    NANO_LOG_DEBUG("message LOST",
        NANO_LOG_SESSIONID("session",session->id)
        NANO_LOG_STREAMID("stream",stream_id)
        NANO_LOG_SN("sn",lost)
        NANO_LOG_ENUM("reason",reason))

    NANO_XRCE_ClientListener_on_message_lost(
        &self->listener, self, stream_id, lost, reason);

    NANO_XRCE_Client_find_unacked_request_by_sn(
            self, stream_id, &lost, &req);
    if (req != NULL)
    {
        const NANO_XRCE_ResultStatus req_rc =
                { NANO_XRCE_STATUS_ERR_DDS_ERROR, NANO_RETCODE_ERROR };
        
        NANO_LOG_DEBUG("REQUEST message LOST",
            NANO_LOG_STREAMID("stream",stream_id)
            NANO_LOG_SN("req_sn", lost)
            NANO_LOG_ENUM("reason",reason))
        
        req->result = req_rc;
        req->flags |= NANO_XRCE_REQUESTFLAGS_COMPLETE;
    }

    NANO_LOG_FN_EXIT
}

#endif /* NANO_FEAT_RELIABILITY */

NANO_RetCode
NANO_XRCE_Client_initialize(
    NANO_XRCE_Client *const self,
    const NANO_XRCE_ClientProperties *const properties)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_XRCE_SessionProperties session_props = 
            NANO_XRCE_SESSIONPROPERTIES_INITIALIZER;
    
    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)
    NANO_PCOND(properties != NULL)
    NANO_PCOND(properties->transport != NULL)
    NANO_PCOND(properties->storage != NULL)

    self->flags = NANO_XRCE_CLIENTFLAGS_DEFAULT;

     self->storage = properties->storage;

#if NANO_FEAT_SUBSCRIBE
    self->listener = properties->listener;
#endif /* NANO_FEAT_SUBSCRIBE */

    session_props.key = properties->key;
    session_props.id =
        (properties->include_key_in_messages)?
        NANO_LIMIT_CLIENT_SESSION_ID_WITH_KEY_DEFAULT :
        NANO_LIMIT_CLIENT_SESSION_ID_WITHOUT_KEY_DEFAULT;
    session_props.transport = properties->transport;
    session_props.transport_properties = properties->transport_properties;
    session_props.listener.on_submsg = NANO_XRCE_Client_on_submessage;
    session_props.storage = properties->storage->session;
#if NANO_FEAT_RELIABILITY
    session_props.listener.on_send_complete = NANO_XRCE_Client_on_send_complete;
    session_props.listener.on_msg_lost = NANO_XRCE_Client_on_message_lost;

#if NANO_FEAT_SYSTIME
    session_props.acknack_period = properties->acknack_period;
    session_props.heartbeat_period = properties->heartbeat_period;
#endif /* NANO_FEAT_SYSTIME */

#endif /* NANO_FEAT_RELIABILITY */
    session_props.listener.user_data = (NANO_u8*) self;

    NANO_CHECK_RC(
        NANO_XRCE_Session_initialize(&self->session, &session_props),
        NANO_LOG_ERROR("FAILED to initialize session",
            NANO_LOG_RC(rc)));

    NANO_LOG_DEBUG_MSG("client OK")

#if NANO_FEAT_AGENT_DISCOVERY
    if (properties->enable_discovery)
    {
        NANO_CHECK_RC(
            NANO_XRCE_Client_update_locator(
                self,
                NANO_XRCE_CLIENTTRANSPORTLOCATOR_AGENT,
                NULL),
            NANO_LOG_ERROR("FAILED to enable discovery", NANO_LOG_RC(rc)));
    }
#endif

    rc = NANO_RETCODE_OK;

done:
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}


void
NANO_XRCE_Client_mark_disconnected(NANO_XRCE_Client *const self)
{
    self->flags &= ~NANO_XRCE_CLIENTFLAGS_CONNECTED;
    NANO_XRCE_Session_disconnected(&self->session);
}

NANO_PRIVATE
void
NANO_XRCE_Client_new_request(
    NANO_XRCE_Client *const self,
    const NANO_XRCE_StreamId stream_id,
    const NANO_XRCE_SubmessageId submsg_id,
    const NANO_XRCE_ObjectId *const object_id,
    const NANO_XRCE_RequestFlags request_flags,
    NANO_XRCE_ClientRequestToken *const request_token,
    NANO_XRCE_ClientRequest **const request_out)
{
    NANO_usize i = 0;
    const NANO_XRCE_ClientRequestToken def_request_token =
        NANO_XRCE_CLIENTREQUESTTOKEN_INITIALIZER;
    NANO_XRCE_ClientRequest *req = NULL,
                            *prev_req = NULL,
                            *next_req = NULL;
    NANO_u16 nxt_req_id = 0;
    NANO_XRCE_SubmessageFlags submsg_flags = NANO_XRCE_SUBMESSAGEFLAGS_DEFAULT;

    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)

    if (request_token != NULL)
    {
        *request_token = def_request_token;
    }

    if (self->storage == NULL)
    {
        /* No storage, or already allocated all available requests */
        NANO_LOG_ERROR_MSG("NO session storage provided")
        return;
    }
    if (self->storage->requests->client_requests_len == 
            self->storage->requests->client_requests_max)
    {
        /* No storage, or already allocated all available requests */
        NANO_LOG_ERROR("TOO many outstanding requests",
            NANO_LOG_USIZE("pending", self->storage->requests->client_requests_len)
            NANO_LOG_USIZE("max", self->storage->requests->client_requests_max))
        return;
    }

#if !NANO_FEAT_RELIABILITY
    if (NANO_XRCE_StreamId_is_reliable(stream_id))
    {
        NANO_LOG_FEAT_MISSING(NANO_FEAT_RELIABILITY)
        return;
    }
#endif /* !NANO_FEAT_RELIABILITY */

    if (request_flags & NANO_XRCE_REQUESTFLAGS_CREATE_REUSE)
    {
        submsg_flags |= NANO_XRCE_SUBMESSAGEFLAGS_CREATE_REUSE;
    }

    if (request_flags & NANO_XRCE_REQUESTFLAGS_CREATE_REPLACE)
    {
        submsg_flags |= NANO_XRCE_SUBMESSAGEFLAGS_CREATE_REPLACE;
    }

    if (request_flags & NANO_XRCE_REQUESTFLAGS_CONFIRM)
    {
        if (submsg_id == NANO_XRCE_SUBMESSAGEID_READ_DATA)
        {
            submsg_flags |= NANO_XRCE_SUBMESSAGEFLAGS_READDATA_CONFIRM;
        }
        else if (submsg_id == NANO_XRCE_SUBMESSAGEID_WRITE_DATA)
        {
            submsg_flags |= NANO_XRCE_SUBMESSAGEFLAGS_WRITEDATA_CONFIRM;
        }
    }

    if (request_flags & NANO_XRCE_REQUESTFLAGS_CUSTOM_ENDIANNESS)
    {
        if (request_flags & NANO_XRCE_REQUESTFLAGS_LITTLE_ENDIAN)
        {
            submsg_flags |= NANO_XRCE_SUBMESSAGEFLAGS_ENDIANNESS;
        }
        else
        {
            submsg_flags &= ~NANO_XRCE_SUBMESSAGEFLAGS_ENDIANNESS;
        }
    }

    nxt_req_id = self->next_request;
    nxt_req_id += 1;
    /* We reserve 0 as an "invalid" request id */
    if (nxt_req_id == 0)
    {
        nxt_req_id += 1;
    }

    /* Scan existing requests to make sure that the request id is not already
       in use. Pending requests are ordered by increasing request id */
    req = self->pending_requests;
    while (req != NULL)
    {
        if (nxt_req_id == req->req_id)
        {
            nxt_req_id += 1;
            /* We reserve 0 as an "invalid" request id */
            if (nxt_req_id == 0)
            {
                nxt_req_id += 1;
            }
        }
        
        prev_req = req;
        req = req->next;

        if (req != NULL && req->req_id > nxt_req_id)
        {
            /* The next requests has a higher id, so we can stop looking
               for conflicts */
            next_req = req;
            break;
        }
    }

    /* Find first free request from pool */
    for (i = 0; i < self->storage->requests->client_requests_max && req == NULL; i++)
    {
        req = self->storage->requests->client_requests + i;
        if (req->flags & NANO_XRCE_REQUESTFLAGS_IN_USE)
        {
            req = NULL;
        }
    }
    NANO_PCOND(req != NULL)
    self->storage->requests->client_requests_len += 1;
    req->req_id = nxt_req_id;
    req->flags = request_flags | NANO_XRCE_REQUESTFLAGS_IN_USE;
    req->submsg_id = submsg_id,
    req->submsg_flags = submsg_flags;
    req->stream_id = stream_id;
    if (object_id != NULL)
    {
        req->obj_id = *object_id;
    }
    
    self->next_request = nxt_req_id;

    if (prev_req != NULL)
    {
        prev_req->next = req;
    }
    else
    {
        if (self->pending_requests != NULL)
        {
            req->next = self->pending_requests;
        }
        self->pending_requests = req;
    }
    req->next = next_req;

    NEW_REQ_LOG("NEW request ",
        NANO_LOG_PTR("req", req)
        NANO_LOG_H16("req.id", req->req_id))

    if (request_token != NULL)
    {
        NANO_XRCE_RequestId_from_u16(&request_token->request_id, req->req_id);
        request_token->object_id = req->obj_id;
        request_token->sn = req->sn;
        request_token->stream_id = req->stream_id;
    }

    *request_out = req;

    req = self->pending_requests;
    while(req != NULL)
    {
        NEW_REQ_LOG("PENDING request ",
            NANO_LOG_PTR("req", req)
            NANO_LOG_H16("req.id", req->req_id))
        req = req->next;
    }

    NANO_LOG_FN_EXIT
}

void
NANO_XRCE_Client_release_request(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientRequest *const req)
{
    NANO_XRCE_ClientRequest *prev_req = NULL,
                            *pending = NULL;
    
    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)
    NANO_PCOND(req != NULL)

    NANO_LOG_DEBUG("request RETURNED",
        NANO_LOG_SESSIONID("session",self->session.id)
            NANO_LOG_STREAMID("stream",req->stream_id)
            NANO_LOG_H16("req_id", req->req_id))

#if NANO_FEAT_RELIABILITY
    if (NANO_XRCE_StreamId_is_reliable(req->stream_id) &&
        !NANO_XRCE_ClientRequest_is_acked(req))
    {
        /* Since we haven't received an ack, we might have to release
        the message sent for the request (if sent on a reliable stream) */
        NANO_XRCE_ReliableStream *rel_stream = NULL;

        rel_stream = (NANO_XRCE_ReliableStream*)
            NANO_XRCE_Session_lookup_stream(&self->session, req->stream_id);
        NANO_PCOND(rel_stream != NULL)

        NANO_XRCE_Session_dismiss_send_queue_up_to(
                &self->session, rel_stream, &req->sn);
    }
#endif /* NANO_FEAT_RELIABILITY */

    /* find request in pending_requests list */
    pending = self->pending_requests;
    while (pending != NULL && pending != req)
    {
        prev_req = pending;
        pending = pending->next;
    }
    if (pending != req)
    {
        /* WARNING unknown request */
        return;
    }

    if (prev_req != NULL)
    {
        prev_req->next = req->next;
    }
    else
    {
        self->pending_requests = req->next;
    }

    self->storage->requests->client_requests_len -= 1;
    req->flags &= ~NANO_XRCE_REQUESTFLAGS_IN_USE;

    NANO_LOG_FN_EXIT
}

void
NANO_XRCE_Client_drop_requests(NANO_XRCE_Client *const self)
{
    NANO_XRCE_ClientRequest *req = self->pending_requests,
                            *next = NULL;
    while (req != NULL)
    {
        next = req->next;
        NANO_XRCE_Client_release_request(self,req);
        req = next;
    }
}


void
NANO_XRCE_Client_dismiss_request(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientRequestToken *const request_token)
{
    NANO_XRCE_ClientRequest *req = NULL;
    const NANO_XRCE_ClientRequestToken def_token =
        NANO_XRCE_CLIENTREQUESTTOKEN_INITIALIZER;
    
    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)
    NANO_PCOND(request_token != NULL)

    NANO_XRCE_Client_find_pending_request_from_token(
        self, request_token, &req);
    if (req == NULL)
    {
        *request_token = def_token;
        return;
    }

    *request_token = def_token;
    NANO_XRCE_Client_release_request(self, req);

    NANO_LOG_FN_EXIT
}

NANO_PRIVATE
NANO_RetCode
NANO_XRCE_Client_wait_for_request(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientRequest *const req,
    const NANO_Timeout wait_timeout_ms,
    const NANO_XRCE_RequestFlags complete_flags,
    const NANO_bool dismiss_request,
    NANO_XRCE_ResultStatus *const request_result_out)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    const NANO_XRCE_ResultStatus def_result = NANO_XRCE_RESULTSTATUS_INITIALIZER;
#if NANO_FEAT_SYSTIME
    NANO_u64 ms_start = 0,
             ms_now = 0,
             ms_diff = 0;
    NANO_Timeout timeout_remaining = 0;
#endif /* NANO_FEAT_SYSTIME */
    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)
    NANO_PCOND(req != NULL)
    NANO_PCOND(self->session.transport != NULL)

    if (request_result_out != NULL)
    {
        *request_result_out = def_result;
    }

#if NANO_FEAT_SYSTIME
    if (wait_timeout_ms > 0)
    {
        NANO_OSAPI_Clock_millis(&self->session.clock, &ms_start);
    }
    timeout_remaining = wait_timeout_ms;
#endif /* NANO_FEAT_SYSTIME */

    do {
        NANO_LOG_DEBUG("WAITING for request",
            NANO_LOG_KEY("key", *NANO_XRCE_Session_key(&self->session))
            NANO_LOG_SESSIONID("id", *NANO_XRCE_Session_id(&self->session))
            NANO_LOG_STREAMID("stream", req->stream_id)
            NANO_LOG_H16("req_id", req->req_id)
            NANO_LOG_SN("req_sn", req->sn)
            NANO_LOG_PTR("req", req))

        NANO_CHECK_RC(
            NANO_XRCE_Session_run(&self->session, 1, timeout_remaining),
            if (rc == NANO_RETCODE_TIMEOUT)
            {
                NANO_LOG_DEBUG("session::run timedout", NANO_LOG_RC(rc))
            }
            else
            {
                NANO_LOG_ERROR("session::run failed", NANO_LOG_RC(rc))
            });

#if NANO_FEAT_SYSTIME
        if (wait_timeout_ms > 0)
        {
            NANO_OSAPI_Clock_millis(&self->session.clock, &ms_now);
            ms_diff = ms_now - ms_start;
            timeout_remaining =
                (ms_diff >= (NANO_u64)wait_timeout_ms)?
                    0 : (wait_timeout_ms - ((NANO_u32)ms_diff));
        }
#endif /* NANO_FEAT_SYSTIME */
    } while (!(req->flags & complete_flags) && timeout_remaining != 0);
    
    if (!(req->flags & complete_flags))
    {
        NANO_LOG_ERROR("TIMEOUT exceeded without a reply",
            NANO_LOG_I32("timeout",wait_timeout_ms)
            NANO_LOG_H16("complete_flags", complete_flags))
        
        rc = NANO_RETCODE_TIMEOUT;
        goto done;
    }

    REQ_COMPLETE_LOG("request RESULT",
        NANO_LOG_STREAMID("stream",req->stream_id)
        NANO_LOG_H16("req.id",req->req_id)
        NANO_LOG_BOOL("req.success",NANO_XRCE_ResultStatus_success(&req->result))
        NANO_LOG_H8("result.status",req->result.status)
        NANO_LOG_H8("result.impl_status",req->result.implementation_status))

    if (request_result_out != NULL)
    {
        *request_result_out = req->result;
    }

    rc = NANO_RETCODE_OK;
    
done:
    if (dismiss_request)
    {
        NANO_XRCE_Client_release_request(self, req);
    }

    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

NANO_RetCode
NANO_XRCE_Client_wait_for_data_received(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientRequestToken *const request_token,
    const NANO_Timeout wait_timeout_ms)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_XRCE_ClientRequest *req = NULL;
    
    NANO_LOG_FN_ENTRY
    
    NANO_PCOND(self != NULL)
    
    NANO_XRCE_Client_find_pending_request_from_token(
        self, request_token, &req);
    if (req == NULL)
    {
        goto done;
    }

    NANO_CHECK_RC(
        NANO_XRCE_Client_wait_for_request(
            self,
            req,
            wait_timeout_ms,
            NANO_XRCE_REQUESTFLAGS_DATA,
            NANO_BOOL_FALSE,
            NULL),
        if (rc == NANO_RETCODE_TIMEOUT)
        {
            NANO_LOG_DEBUG_MSG("wait for request TIMED OUT")
        }
        else
        {
            NANO_LOG_ERROR_MSG("FAILED to wait for request")
        }
        );
    
    rc = NANO_RETCODE_OK;
    
done:
    
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

NANODllExport
NANO_RetCode
NANO_XRCE_Client_wait_for_request_complete(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientRequestToken *const request_token,
    const NANO_Timeout wait_timeout_ms,
    NANO_XRCE_ResultStatus *const request_result_out)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_XRCE_ClientRequest *req = NULL;
    
    NANO_LOG_FN_ENTRY
    
    NANO_PCOND(self != NULL)
    
    NANO_XRCE_Client_find_pending_request_from_token(
        self, request_token, &req);
    if (req == NULL)
    {
        goto done;
    }

    NANO_CHECK_RC(
        NANO_XRCE_Client_wait_for_request(
            self,
            req,
            wait_timeout_ms,
            NANO_XRCE_REQUESTFLAGS_COMPLETE,
            NANO_BOOL_FALSE,
            request_result_out),
        NANO_LOG_ERROR_MSG("FAILED to wait for request"));
    
    rc = NANO_RETCODE_OK;
    
done:
    
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

#if NANO_FEAT_RELIABILITY

NANODllExport
NANO_RetCode
NANO_XRCE_Client_wait_for_request_acked(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientRequestToken *const request_token,
    const NANO_Timeout wait_timeout_ms)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_XRCE_ClientRequest *req = NULL;
    
    NANO_LOG_FN_ENTRY
    
    NANO_PCOND(self != NULL)

    NANO_XRCE_Client_find_pending_request_from_token(
        self, request_token, &req);
    if (req == NULL)
    {
        goto done;
    }

    NANO_CHECK_RC(
        NANO_XRCE_Session_run_until_acked(
            &self->session, req->stream_id, &req->sn, wait_timeout_ms),
        NANO_LOG_ERROR_MSG("FAILED to run session until ack"));
    
    rc = NANO_RETCODE_OK;
    
done:
    
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

#endif /* NANO_FEAT_RELIABILITY */

NANO_PRIVATE
NANO_RetCode
NANO_XRCE_Client_return_or_wait_for_request(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientRequest *const req,
    NANO_XRCE_ClientRequestToken *const request_token,
    const NANO_Timeout wait_timeout_ms)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_XRCE_ResultStatus request_result = NANO_XRCE_RESULTSTATUS_INITIALIZER;
    const NANO_bool sync = req->flags & NANO_XRCE_REQUESTFLAGS_SYNC,
        dismiss = sync && req->submsg_id != NANO_XRCE_SUBMESSAGEID_READ_DATA;
    
    NANO_LOG_FN_ENTRY
    
    NANO_PCOND(self != NULL)
    NANO_PCOND(req != NULL)

    /* update token if available */
    if (request_token != NULL)
    {
        request_token->sn = req->sn;
    }

    if (sync)
    {
        if (req->flags & NANO_XRCE_REQUESTFLAGS_CONFIRM)
        {
            rc = NANO_XRCE_Client_wait_for_request(
                    self,
                    req,
                    wait_timeout_ms,
                    NANO_XRCE_REQUESTFLAGS_COMPLETE,
                    NANO_BOOL_FALSE /* dismiss */,
                    &request_result);
            if (NANO_RETCODE_OK == rc &&
                !NANO_XRCE_ResultStatus_success(&request_result))
            {
                rc = NANO_RETCODE_ERROR;
            }
            goto done;
        }
#if NANO_FEAT_RELIABILITY
        else if (NANO_XRCE_ClientRequest_is_reliable(req))
        {
            rc = NANO_XRCE_Session_run_until_acked(
                    &self->session, req->stream_id, &req->sn, wait_timeout_ms);
            goto done;
        }
#endif /* NANO_FEAT_RELIABILITY */
    }
    
    rc = NANO_RETCODE_OK;
    
done:
    if (dismiss)
    {
        NANO_XRCE_Client_release_request(self, req);
    }

    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

NANO_RetCode
NANO_XRCE_Client_connect(
    NANO_XRCE_Client *const self,
    const NANO_Timeout timeout_ms)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    const NANO_XRCE_Cookie client_cookie = NANO_XRCE_COOKIE_DEFAULT;
    const NANO_XRCE_Version client_version = NANO_XRCE_VERSION_DEFAULT;
    const NANO_XRCE_VendorId client_vendor = NANO_XRCE_VENDORID_DEFAULT;
    /* CREATE_CLIENT payload can be set directly in the serialized buffer */
    NANO_XRCE_ClientRepresentation *cc_repr = NULL;
    NANO_MessageBuffer *msg = NULL;
    NANO_usize payload_size = 
                    /* Since we never include the property sequence, then
                       we should only allocate a buffer for the client's
                       "minimal" representation */
                    NANO_XRCE_CLIENTREPRESENTATION_SERIALIZED_SIZE_MIN;;
    NANO_u8 *ptr = NULL;
    NANO_u32 timeout_remaining = timeout_ms;
#ifdef NANO_FEAT_SYSTIME
    NANO_u64 ts_start = 0,
             ts_now = 0,
             ts_diff = 0;

    NANO_OSAPI_Clock_millis(&self->session.clock, &ts_start);
#endif /* NANO_FEAT_SYSTIME */

    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)

    if (NANO_XRCE_Client_discovering(self))
    {
        NANO_LOG_ERROR_MSG("NO AGENT discovered yet")
        rc = NANO_RETCODE_PRECONDITION_NOT_MET;
        goto done;
    }

    msg = NANO_XRCE_Session_allocate_message(
                &self->session,
                &self->session.stream_none.base,
                NANO_XRCE_MESSAGETYPE_INLINE_PAYLOAD,
                payload_size,
                NULL);
    if (msg == NULL)
    {
        NANO_LOG_ERROR("failed to allocate message",
            NANO_LOG_STREAMID("stream", NANO_XRCE_STREAMID_NONE))
        rc = NANO_RETCODE_OUT_OF_RESOURCES;
        goto done;
    }

    /* "serialize" client representation to buffer */
    cc_repr = (NANO_XRCE_ClientRepresentation*) 
                    NANO_MessageBuffer_data_ptr(msg);
    
    cc_repr->xrce_cookie = client_cookie;
    cc_repr->xrce_version = client_version;
    cc_repr->xrce_vendor_id = client_vendor;
    cc_repr->client_key = self->session.key;
    cc_repr->session_id = self->session.id;

    ptr = NANO_MessageBuffer_data_ptr(msg) + 
          NANO_OSAPI_MEMBER_OFFSET(NANO_XRCE_ClientRepresentation, session_id) +
          sizeof(NANO_XRCE_SessionId);
    /* Never serialize properties */
    *((NANO_bool*)ptr) = NANO_BOOL_FALSE;
    ptr += sizeof(NANO_bool);
#if NANO_FEAT_MTU_IN_CLIENT_REPR
    /* some legacy implementations include an MTU field */
    ptr = NANO_OSAPI_Memory_align(ptr, NANO_u16);
    *((NANO_u16*)ptr) = (NANO_u16)self->session.transport->mtu;
#endif /* NANO_FEAT_MTU_IN_CLIENT_REPR */
    
    /* Mark client as in "connecting" state */
    NANO_XRCE_Client_mark_disconnected(self);

    self->flags |= NANO_XRCE_CLIENTFLAGS_CONNECTING;

    NANO_CHECK_RC(
        NANO_XRCE_Session_send(
            &self->session, 
            &self->session.stream_none.base,
            NANO_XRCE_SUBMESSAGEID_CREATE_CLIENT,
            NANO_XRCE_SUBMESSAGEFLAGS_DEFAULT,
            msg,
            NULL),
        msg = NULL;
        NANO_LOG_ERROR("failed to send CREATE_CLIENT request",
            NANO_LOG_RC(rc)));
    msg = NULL;
    
    do
    {

        rc = NANO_XRCE_Session_run(&self->session, 1, timeout_remaining);
        if (NANO_RETCODE_OK == rc)
        {
#ifdef NANO_FEAT_SYSTIME
            if (!NANO_XRCE_Client_connected(self) && timeout_ms > 0)
            {
                NANO_OSAPI_Clock_millis(&self->session.clock, &ts_now);
                ts_diff = ts_now - ts_start;
                if (ts_diff > (NANO_u64)timeout_ms)
                {
                    rc = NANO_RETCODE_TIMEOUT;
                }
                else
                {
                    timeout_remaining = timeout_ms - ts_diff;
                }
            }
#endif /* NANO_FEAT_SYSTIME */
        }
    } while (!NANO_XRCE_Client_connected(self) && rc == NANO_RETCODE_OK);
    
done:
    self->flags &= ~NANO_XRCE_CLIENTFLAGS_CONNECTING;

    if (NANO_RETCODE_OK != rc)
    {
        NANO_XRCE_Client_mark_disconnected(self);
    }

    if (msg != NULL)
    {
        NANO_XRCE_Session_release_message(
            &self->session, &self->session.stream_none.base, msg);
    }

    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

NANO_RetCode
NANO_XRCE_Client_disconnect(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientRequestToken *const request_token,
    const NANO_XRCE_StreamId request_stream,
    const NANO_XRCE_RequestFlags request_flags,
    const NANO_Timeout request_timeout_ms)
{
    NANO_XRCE_RequestArgs args = NANO_XRCE_REQUESTARGS_INITIALIZER;
    args.timeout_ms = request_timeout_ms;
    args.flags = request_flags;
    args.stream_id = request_stream;
    return NANO_XRCE_Client_disconnect_w_args(self, request_token, &args);
}

NANO_RetCode
NANO_XRCE_Client_disconnect_w_args(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientRequestToken *const request_token,
    const NANO_XRCE_RequestArgs *const args)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    /* DELETE(CLIENT) payload can be set directly from serialized buffer */
    NANO_XRCE_DeletePayload *del_payload = NULL;
    const NANO_usize buffer_size = 
            NANO_XRCE_DELETEPAYLOAD_HEADER_SERIALIZED_SIZE_MAX;
    NANO_MessageBuffer *msg = NULL;
    NANO_XRCE_ClientRequest *req = NULL;
    NANO_XRCE_Stream *stream = NULL;
    NANO_XRCE_ObjectId client_id = NANO_XRCE_OBJECTID_INVALID;
    
    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)
    NANO_PCOND(args != NULL)

    /* Drop all requests before doing anything else */
    NANO_XRCE_Client_drop_requests(self);

    stream = NANO_XRCE_Session_lookup_stream(
                    &self->session, args->stream_id);
    if (stream == NULL)
    {
        NANO_LOG_ERROR("unknown stream for DELETE(Client)",
            NANO_LOG_SESSIONID("session",self->session.id)
            NANO_LOG_STREAMID("stream",args->stream_id))
        rc = NANO_RETCODE_INVALID_ARGS;
        goto done;
    }

    msg = NANO_XRCE_Session_allocate_message(
            &self->session,
            stream,
            NANO_XRCE_MESSAGETYPE_INLINE_PAYLOAD,
            buffer_size,
            NULL);

    if (msg == NULL)
    {
        NANO_LOG_ERROR("failed to allocate DELETE message",
            NANO_LOG_STREAMID("stream", args->stream_id))
        rc = NANO_RETCODE_OUT_OF_RESOURCES;
        goto done;
    }

    NANO_XRCE_ObjectId_set(&client_id, NANO_XRCE_OBJK_CLIENT, 0x0000);

    /* Allocate a request record */
    NANO_XRCE_Client_new_request(
            self,
            args->stream_id,
            NANO_XRCE_SUBMESSAGEID_DELETE,
            &client_id,
            args->flags | NANO_XRCE_REQUESTFLAGS_CONFIRM,
            request_token,
            &req);
    if (req == NULL)
    {
        NANO_LOG_ERROR(
            "failed to allocate DELETE(CLIENT) request",
            NANO_LOG_STREAMID("stream_id",args->stream_id))
        goto done;
    }

    del_payload = 
        (NANO_XRCE_DeletePayload*) NANO_MessageBuffer_data_ptr(msg);

    /* "serialize" DELETE payload to buffer */
    NANO_XRCE_RequestId_from_u16(
        &del_payload->base.request_id, req->req_id);
    del_payload->base.object_id = client_id;

    
    /* Send request using the session */
    NANO_CHECK_RC(
        NANO_XRCE_Session_send(
            &self->session,
            stream,
            req->submsg_id,
            req->submsg_flags,
            msg,
            &req->sn),
        msg = NULL;
        NANO_LOG_ERROR("failed to send DELETE(CLIENT) request",
            NANO_LOG_RC(rc)));
    msg = NULL;
    
    NANO_CHECK_RC(
        NANO_XRCE_Client_return_or_wait_for_request(
            self, req, request_token, args->timeout_ms),
        req = NULL;
        NANO_LOG_ERROR_MSG("FAILED to wait for request"));
    req = NULL;
    
    if (args->flags & NANO_XRCE_REQUESTFLAGS_SYNC)
    {
        NANO_XRCE_Client_mark_disconnected(self);
    }

    rc = NANO_RETCODE_OK;
    
done:

    if (msg != NULL)
    {
        NANO_XRCE_Session_release_message(&self->session, stream, msg);
    }

    if (req != NULL)
    {
        /* Dispose of unsuccessfull request */
        NANO_XRCE_Client_release_request(self, req);
    }

    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

#if NANO_FEAT_PUBLISH

#if NANO_FEAT_EXTENDED_API

NANO_MessageBuffer*
NANO_XRCE_Client_allocate_message_buffer(
    NANO_XRCE_Client *const self,
    const NANO_XRCE_StreamId stream_id,
    const NANO_u8 *const payload,
    const NANO_u16 payload_len)
{
    NANO_MessageBuffer *res = NULL;
    NANO_XRCE_Stream *stream = NULL;

    NANO_LOG_FN_ENTRY
    
    NANO_PCOND(self != NULL)
    
    stream = NANO_XRCE_Session_lookup_stream(&self->session, stream_id);
    if (stream == NULL)
    {
        NANO_LOG_ERROR("unknown stream for message buffer",
            NANO_LOG_SESSIONID("session",self->session.id)
            NANO_LOG_STREAMID("stream", stream_id))
        goto done;
    }
    
    res = NANO_XRCE_Session_allocate_message(
                &self->session,
                stream,
                NANO_XRCE_MESSAGETYPE_EXTERNAL_PAYLOAD,
                payload_len,
                payload);
    
done:
    NANO_LOG_FN_EXIT_PTR(res)
    return res;
}

NANO_RetCode
NANO_XRCE_Client_release_message_buffer(
    NANO_XRCE_Client *const self,
    const NANO_XRCE_StreamId stream_id,
    NANO_MessageBuffer *const mbuf)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_XRCE_Stream *stream = NULL;
    
    NANO_LOG_FN_ENTRY
    
    NANO_PCOND(self != NULL)

    stream = NANO_XRCE_Session_lookup_stream(&self->session, stream_id);
    if (stream == NULL)
    {
        NANO_LOG_ERROR("unknown stream for message buffer",
            NANO_LOG_SESSIONID("session",self->session.id)
            NANO_LOG_STREAMID("stream", stream_id))
        goto done;
    }

    NANO_XRCE_Session_release_message(&self->session, stream, mbuf);
    
    rc = NANO_RETCODE_OK;
    
done:
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

#endif /* NANO_FEAT_EXTENDED_API */



#if NANO_FEAT_DATA_FORMAT_DATA


#if NANO_FEAT_EXTENDED_API

NANODllExport
NANO_RetCode
NANO_XRCE_Client_write_data(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientRequestToken *const request_token,
    const NANO_XRCE_StreamId request_stream,
    const NANO_XRCE_RequestFlags request_flags,
    const NANO_Timeout request_timeout_ms,
    const NANO_u16 writer_id,
    const NANO_u8 *const payload,
    const NANO_u16 payload_len)
{
    NANO_XRCE_WriteArgs pub_args = NANO_XRCE_WRITEARGS_INITIALIZER;

    NANO_LOG_FN_ENTRY
    
    NANO_PCOND(self != NULL)
    
    pub_args.payload =
        NANO_XRCE_Client_allocate_message_buffer(
            self, request_stream, payload, payload_len);
    if (pub_args.payload == NULL)
    {
        return NANO_RETCODE_OUT_OF_RESOURCES;
    }

    pub_args.format = NANO_XRCE_FORMAT_DATA;
    pub_args.req.stream_id = request_stream;
    pub_args.req.flags = request_flags;
    pub_args.req.timeout_ms = request_timeout_ms;
    NANO_XRCE_ObjectId_from_u16(&pub_args.req.obj_id, writer_id);
    
    return NANO_XRCE_Client_write_w_args(self, request_token, &pub_args);
}

#endif /* NANO_FEAT_EXTENDED_API */

NANO_RetCode
NANO_XRCE_Client_write_w_args(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientRequestToken *const request_token,
    const NANO_XRCE_WriteArgs *const args)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    const NANO_usize buffer_size =
        NANO_XRCE_WRITEDATAPAYLOAD_HEADER_SERIALIZED_SIZE_MAX;
    NANO_MessageBuffer *wr_mbuf = NULL,
                       *payload_mbuf = NULL;
    
    /* we can set WRITE_DATA payload directly from the serialized buffer */
    NANO_XRCE_WriteDataPayload *wr_payload = NULL;
    NANO_XRCE_ClientRequest *req = NULL;
    NANO_XRCE_Stream *stream = NULL;
    NANO_bool little_endian = NANO_BOOL_FALSE;
    NANO_XRCE_ObjectId obj_id = NANO_XRCE_OBJECTID_INVALID;

    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)
    NANO_PCOND(args != NULL)
    NANO_PCOND(args->req.stream_id != NANO_XRCE_STREAMID_NONE)
    NANO_PCOND(args->payload != NULL)

    NANO_XRCE_ObjectId_combine(
        &args->req.obj_id, NANO_XRCE_OBJK_DATAWRITER, &obj_id);

    stream = NANO_XRCE_Session_lookup_stream(
                &self->session, args->req.stream_id);
    if (stream == NULL)
    {
        NANO_LOG_ERROR("unknown stream for WRITE_DATA",
            NANO_LOG_SESSIONID("session",self->session.id)
            NANO_LOG_STREAMID("stream",args->req.stream_id))
        rc = NANO_RETCODE_INVALID_ARGS;
        goto done;
    }

    /* Allocate an mbuf for the WRITE_DATA "header" */
    wr_mbuf = NANO_XRCE_Session_allocate_message(
                &self->session,
                stream,
                NANO_XRCE_MESSAGETYPE_INLINE_PAYLOAD,
                buffer_size,
                NULL);

    if (wr_mbuf == NULL)
    {
        NANO_LOG_ERROR("failed to allocate WRITE_DATA message",
            NANO_LOG_STREAMID("stream", args->req.stream_id))
        rc = NANO_RETCODE_OUT_OF_RESOURCES;
        goto done;
    }

    payload_mbuf = args->payload;
    
    NANO_PCOND(wr_mbuf != NULL)
    NANO_PCOND(payload_mbuf != NULL)

    NANO_XRCE_Client_new_request(
        self, 
        args->req.stream_id, 
        NANO_XRCE_SUBMESSAGEID_WRITE_DATA,
        &obj_id,
        args->req.flags,
        request_token,
        &req);
    if (req == NULL)
    {
        NANO_LOG_ERROR(
            "failed to allocate WRITE_DATA request", 
            NANO_LOG_STREAMID("stream_id", args->req.stream_id));
        rc = NANO_RETCODE_OUT_OF_RESOURCES;
        goto done;
    }

    little_endian = (req->submsg_flags & NANO_XRCE_SUBMESSAGEFLAGS_ENDIANNESS);

    /* Store data format in request object */
    NANO_XRCE_SubmessageFlags_DATA_set_format(
        &req->submsg_flags,
        NANO_XRCE_FORMAT_DATA,
        little_endian);
    
    /* Serialize WRITE_DATA message to buffer */
    wr_payload = (NANO_XRCE_WriteDataPayload*)
                        NANO_MessageBuffer_data_ptr(wr_mbuf);
    NANO_XRCE_RequestId_from_u16(&wr_payload->base.request_id, req->req_id);
    wr_payload->base.object_id = req->obj_id;
    
    /* Append payload to WRITE_DATA header */
    NANO_MessageBuffer_append(wr_mbuf, payload_mbuf);
    
    NANO_CHECK_RC(
        NANO_XRCE_Session_send(
            &self->session,
            stream,
            req->submsg_id,
            req->submsg_flags,
            wr_mbuf,
            &req->sn),
        wr_mbuf = NULL;
        NANO_LOG_ERROR("failed to send WRITE_DATA request", NANO_LOG_RC(rc)));
    wr_mbuf = NULL;

    NANO_LOG_DEBUG("DATA published",
        NANO_LOG_STREAMID("stream",NANO_XRCE_Stream_id(stream))
        NANO_LOG_SN("sn",req->sn))

    NANO_CHECK_RC(
        NANO_XRCE_Client_return_or_wait_for_request(
            self, req, request_token, args->req.timeout_ms),
        req = NULL;
        NANO_LOG_ERROR_MSG("FAILED to wait for request"));
    req = NULL;

    rc = NANO_RETCODE_OK;
    
done:
    
    if (wr_mbuf != NULL)
    {
        NANO_XRCE_Session_release_message(&self->session, stream, wr_mbuf);
    }
    
    if (req != NULL)
    {
        /* Release failed requests */
        NANO_XRCE_Client_release_request(self, req);
    }

    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}
#endif /* NANO_FEAT_PUBLISH */
#endif /* NANO_FEAT_DATA_FORMAT_DATA */

#if NANO_FEAT_SUBSCRIBE
NANO_RetCode
NANO_XRCE_Client_read_w_args(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientRequestToken *const request_token,
    const NANO_XRCE_ReadArgs *const args)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_MessageBuffer *rd_mbuf = NULL;
    NANO_XRCE_ReadDataPayload *rd_payload = NULL;
    NANO_XRCE_ClientRequest *req = NULL;
    NANO_u8 *ptr = NULL;
    NANO_XRCE_Stream *stream = NULL;
    NANO_XRCE_ObjectId obj_id = NANO_XRCE_OBJECTID_INVALID;
#if NANO_FEAT_CONTENT_FILTER
    NANO_MessageBuffer *cft_mbuf = NULL;
    NANO_usize cft_len = 0;
#endif /* NANO_FEAT_CONTENT_FILTER */
#if NANO_FEAT_DELIVERY_CTRL
    NANO_MessageBuffer *ctrl_mbuf = NULL;
    NANO_usize ctrl_pad = 0;
#endif /* NANO_FEAT_DELIVERY_CTRL */

    NANO_LOG_FN_ENTRY
    
    NANO_PCOND(self != NULL)
    NANO_PCOND(args != NULL)
    NANO_PCOND(args->req.stream_id != NANO_XRCE_STREAMID_NONE)
    
    NANO_XRCE_ObjectId_combine(
        &args->req.obj_id, NANO_XRCE_OBJK_DATAREADER, &obj_id);

    stream = NANO_XRCE_Session_lookup_stream(
                &self->session, args->req.stream_id);
    if (stream == NULL)
    {
        NANO_LOG_ERROR("unknown stream for READ_DATA",
            NANO_LOG_SESSIONID("session",self->session.id)
            NANO_LOG_STREAMID("stream",args->req.stream_id))
        rc = NANO_RETCODE_INVALID_ARGS;
        goto done;
    }

    NANO_XRCE_Client_new_request(
        self, 
        args->req.stream_id, 
        NANO_XRCE_SUBMESSAGEID_READ_DATA,
        &obj_id,
        args->req.flags,
        request_token,
        &req);
    if (req == NULL)
    {
        NANO_LOG_ERROR(
            "failed to allocate READ_DATA request", 
            NANO_LOG_STREAMID("stream_id", args->req.stream_id));
        rc = NANO_RETCODE_OUT_OF_RESOURCES;
        goto done;
    }

    rd_mbuf = NANO_XRCE_Session_allocate_message(
                &self->session,
                stream,
                NANO_XRCE_MESSAGETYPE_INLINE_PAYLOAD,
                NANO_XRCE_READDATAPAYLOAD_HEADER_SERIALIZED_SIZE_MAX,
                NULL);

    if (rd_mbuf == NULL)
    {
        NANO_LOG_ERROR("failed to allocate READ_DATA message",
            NANO_LOG_STREAMID("stream", args->req.stream_id))
        rc = NANO_RETCODE_OUT_OF_RESOURCES;
        goto done;
    }

#if NANO_FEAT_CONTENT_FILTER
    /* We only need a payload_user buffer to store the CFT if the user
       actually provided one, since the boolean flag is always stored
        in the inline payload buffer */
    if (args->read_spec.has_content_filter_expr)
    {
        if (args->read_spec.content_filter_expr.value == NULL)
        {
            NANO_LOG_ERROR("NULL content-filter for READ_DATA",
                NANO_LOG_STREAMID("stream", args->req.stream_id))
            goto done;
        }
        cft_len = NANO_OSAPI_String_length(
                        args->read_spec.content_filter_expr.value);
        cft_mbuf = NANO_XRCE_Session_allocate_message(
                    &self->session,
                    stream,
                    NANO_XRCE_MESSAGETYPE_EXTERNAL_PAYLOAD,
                    cft_len + 1,
                    (const NANO_u8*)
                    args->read_spec.content_filter_expr.value);

        if (cft_mbuf == NULL)
        {
            NANO_LOG_ERROR("failed to allocate READ_DATA(CFT) message",
                NANO_LOG_STREAMID("stream", args->req.stream_id))
            rc = NANO_RETCODE_OUT_OF_RESOURCES;
            goto done;
        }
    }
#endif /* NANO_FEAT_CONTENT_FILTER */

#if NANO_FEAT_DELIVERY_CTRL
#if !NANO_FEAT_CONTENT_FILTER
    /* if CFT is disabled, then the delivery_ctrl's boolean goes into
       the header payload, so we need a tail only if we actually have
       a delivery_ctrl. Otherwise, all of the delivery_ctrl goes in the
       tail (including the boolean), so we always allocate a tail. */
    if (args->read_spec.has_delivery_ctrl)
    {
#endif /* !NANO_FEAT_CONTENT_FILTER */
    ctrl_mbuf = NANO_XRCE_Session_allocate_message(
                &self->session,
                stream,
                NANO_XRCE_MESSAGETYPE_INLINE_PAYLOAD_TAIL,
                NANO_XRCE_READSPECIFICATION_TAIL_SERIALIZED_SIZED_MAX,
                NULL);

    if (ctrl_mbuf == NULL)
    {
        NANO_LOG_ERROR("failed to allocate READ_DATA(CTRL) message",
            NANO_LOG_STREAMID("stream", args->req.stream_id))
        rc = NANO_RETCODE_OUT_OF_RESOURCES;
        goto done;
    }
#if !NANO_FEAT_CONTENT_FILTER
    }
#endif /* !NANO_FEAT_CONTENT_FILTER */
#endif /* NANO_FEAT_CONTENT_FILTER */

    /* "serialize" READ_DATA payload to buffer */
    rd_payload = (NANO_XRCE_ReadDataPayload*)
                        NANO_MessageBuffer_data_ptr(rd_mbuf);
    NANO_XRCE_RequestId_from_u16(&rd_payload->base.request_id, req->req_id);
    rd_payload->base.object_id = obj_id;

    /* Serialize ReadSpecification */
    ptr = ((NANO_u8*)rd_payload) + 
            sizeof(NANO_XRCE_BaseObjectRequest);
    *((NANO_XRCE_StreamId*)ptr) = 
            args->read_spec.preferred_stream_id;
    ptr += sizeof(NANO_XRCE_StreamId);
    *((NANO_XRCE_DataFormat*)ptr) = args->read_spec.data_format;
    ptr += sizeof(NANO_XRCE_DataFormat);
    
    /* Serialize optional members of ReadSpecification */

#if NANO_FEAT_CONTENT_FILTER
    *((NANO_bool*)ptr) = args->read_spec.has_content_filter_expr;
    ptr += sizeof(NANO_bool);
    if (args->read_spec.has_content_filter_expr)
    {
        NANO_PCOND(cft_mbuf != NULL)
        ptr = NANO_OSAPI_Memory_align(ptr, NANO_u32);
        *((NANO_u32*)ptr) = (cft_len + 1);
        ptr += sizeof(NANO_u32);
        NANO_MessageBuffer_append(rd_mbuf, cft_mbuf);
#if NANO_FEAT_DELIVERY_CTRL
        /* If the CFT has a total "odd" length, then we will need to
           add a byte of padding after the delivery_ctrl's boolean */
        NANO_MessageBuffer_data_len_msg(rd_mbuf,&ctrl_pad);
        ctrl_pad  = (ctrl_pad + sizeof(NANO_bool)) % 2;
#endif /* NANO_FEAT_DELIVERY_CTRL */
    }
#else
    /* if CFT is disabled, just serialize a boolean into the inline payload */
    NANO_PCOND(!args->read_spec.has_content_filter_expr)
    *((NANO_bool*)ptr) = NANO_BOOL_FALSE;
    ptr += sizeof(NANO_bool);
#endif /* NANO_FEAT_CONTENT_FILTER */
#if NANO_FEAT_DELIVERY_CTRL
#if NANO_FEAT_CONTENT_FILTER

    NANO_MessageBuffer_set_data_len(rd_mbuf,
        (ptr - NANO_MessageBuffer_data_ptr(rd_mbuf)));

    /* If CFT is enabled, then delivery_ctrl AND its boolean go into
       the tail buffer, so swap ptr to it */
    NANO_PCOND(ctrl_mbuf != NULL)
    ptr = NANO_MessageBuffer_data_ptr(ctrl_mbuf);
    NANO_PCOND(ptr != NULL)
#endif

    *((NANO_bool*)ptr) = args->read_spec.has_delivery_ctrl;
    ptr += sizeof(NANO_bool) + ctrl_pad; 

#if !NANO_FEAT_CONTENT_FILTER
    NANO_MessageBuffer_set_data_len(rd_mbuf,
        (ptr - NANO_MessageBuffer_data_ptr(rd_mbuf)));
#endif

    if (args->read_spec.has_delivery_ctrl)
    {
#if !NANO_FEAT_CONTENT_FILTER
        NANO_PCOND(ctrl_mbuf != NULL)
        ptr = NANO_MessageBuffer_data_ptr(ctrl_mbuf);
        NANO_PCOND(ptr != NULL)
#endif
        /* because we must guarantee the overall alignment of the full XRCE
           message (by possibly adding 1 byte padding with ctrl_pad), there is
           no guarantee that ptr is correctly aligned, so each short in 
           delivery ctrl must be serialized byte by byte, to prevent the 
           compiler from generating unaligned memory accesses */
        NANO_OSAPI_Memory_copy(
            ptr, &args->read_spec.delivery_ctrl.max_samples, sizeof(NANO_u16));
        ptr += sizeof(NANO_u16);
        NANO_OSAPI_Memory_copy(
            ptr, &args->read_spec.delivery_ctrl.max_elapsed_time, sizeof(NANO_u16));
        ptr += sizeof(NANO_u16);
        NANO_OSAPI_Memory_copy(
            ptr, &args->read_spec.delivery_ctrl.max_bytes_per_second, sizeof(NANO_u16));
        ptr += sizeof(NANO_u16);
        NANO_OSAPI_Memory_copy(
            ptr, &args->read_spec.delivery_ctrl.min_pace_period, sizeof(NANO_u16));
        ptr += sizeof(NANO_u16);
    }
#if NANO_FEAT_CONTENT_FILTER
    /* Set the length of the tail buffer to the actual serialized size */
    NANO_MessageBuffer_set_data_len(ctrl_mbuf,
        (ptr - NANO_MessageBuffer_data_ptr(ctrl_mbuf)));
    if (cft_mbuf != NULL)
    {
        NANO_MessageBuffer_append(cft_mbuf, ctrl_mbuf);
    }
    else
    {
        NANO_MessageBuffer_append(rd_mbuf, ctrl_mbuf);
    }
#else
    /* The tail buffer has a fixed size which is already set at allocation */
    NANO_MessageBuffer_append(rd_mbuf, ctrl_mbuf);
#endif
#else
#if NANO_FEAT_CONTENT_FILTER
    /* If CFT is enabled, then delivery_ctrl AND its boolean go into
       the tail buffer, so swap ptr to it */
    NANO_PCOND(ctrl_mbuf != NULL)
    ptr = NANO_MessageBuffer_data_ptr(ctrl_mbuf);
    NANO_PCOND(ptr != NULL)
#endif

    NANO_PCOND(!args->read_spec.has_delivery_ctrl)
    *((NANO_bool*)ptr) = NANO_BOOL_FALSE;
    ptr += sizeof(NANO_bool);

#if NANO_FEAT_CONTENT_FILTER
    NANO_MessageBuffer_set_data_len(ctrl_mbuf, sizeof(NANO_bool));
    if (cft_mbuf != NULL)
    {
        NANO_MessageBuffer_append(cft_mbuf, ctrl_mbuf);
    }
    else
    {
        NANO_MessageBuffer_append(rd_mbuf, ctrl_mbuf);
    }
#endif
#endif /* NANO_FEAT_DELIVERY_CTRL && NANO_FEAT_OPTIONAL */

    NANO_LOG_DEBUG("READ_DATA request",
        NANO_LOG_STREAMID("req_stream",req->stream_id)
        NANO_LOG_STREAMID("sub_stream",rd_payload->read_spec.preferred_stream_id)
        NANO_LOG_REQID("req_id",rd_payload->base.request_id)
        NANO_LOG_OBJID("obj_id",rd_payload->base.object_id)
        NANO_LOG_H8("data_fmt",rd_payload->read_spec.data_format))
    
    req->obj_id = obj_id;
    
    NANO_CHECK_RC(
        NANO_XRCE_Session_send(
            &self->session, 
            stream,
            req->submsg_id,
            req->submsg_flags,
            rd_mbuf,
            &req->sn),
        rd_mbuf = NULL;
        NANO_LOG_ERROR("failed to send READ_DATA request", NANO_LOG_RC(rc)));
    rd_mbuf = NULL;
    
    NANO_CHECK_RC(
        NANO_XRCE_Client_return_or_wait_for_request(
            self, req, request_token, args->req.timeout_ms),
        req = NULL;
        NANO_LOG_ERROR_MSG("FAILED to wait for request"));
    req = NULL;
    
    rc = NANO_RETCODE_OK;
    
done:
    /* TODO properly dispose all mbufs on error */

    if (rd_mbuf != NULL)
    {
        NANO_XRCE_Session_release_message(&self->session, stream, rd_mbuf);
    }

    if (rc != NANO_RETCODE_OK && req != NULL)
    {
        NANO_XRCE_Client_release_request(self, req);
    }

    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

#endif /* NANO_FEAT_SUBSCRIBE */

#if NANO_FEAT_OBJECT_CREATE
NANO_RetCode
NANO_XRCE_Client_create_object_w_args(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientRequestToken *const request_token,
    const NANO_XRCE_CreateArgs *const args)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_XRCE_Stream *stream = NULL;
    NANO_usize buffer_size = NANO_XRCE_CREATEPAYLOAD_HEADER_SERIALIZED_SIZE_MAX,
               repr_len_tot = 0,
               tail_pad = 0;
    NANO_XRCE_CreatePayload *create_payload = NULL;
    NANO_u8 *data_ptr = NULL;
    NANO_MessageBuffer *create_mbuf = NULL,
                       *repr_mbuf = NULL;
    NANO_XRCE_ClientRequest *req = NULL;
    NANO_MessageBuffer *tail_mbuf = NULL;
    NANO_XRCE_ObjectId obj_id = NANO_XRCE_OBJECTID_INVALID;

    NANO_LOG_FN_ENTRY
    
    NANO_PCOND(self != NULL)

    NANO_PCOND(args != NULL)
    NANO_PCOND(NANO_XRCE_ObjectKind_is_valid(args->obj_kind))
    NANO_PCOND(NANO_XRCE_RepresentationFormat_is_valid(args->repr_fmt))
    NANO_PCOND(args->repr != NULL)
#if !NANO_FEAT_EPROSIMA
    NANO_PCOND(args->repr_len > 0)
#endif /* !NANO_FEAT_EPROSIMA */

    NANO_XRCE_ObjectId_combine(
        &args->req.obj_id, args->obj_kind, &obj_id);
    
    repr_len_tot = args->repr_len;
    
    if (args->repr_fmt == NANO_XRCE_REPRESENTATION_AS_XML_STRING ||
        args->repr_fmt == NANO_XRCE_REPRESENTATION_BY_REFERENCE)
    {
        repr_len_tot += 1;
    }

    NANO_LOG_DEBUG("create OBJECT",
        NANO_LOG_STREAMID("stream",args->req.stream_id)
        NANO_LOG_H8("obj_kind",args->obj_kind)
        NANO_LOG_H8("repr_fmt",args->repr_fmt)
        NANO_LOG_BYTES("repr",args->repr,repr_len_tot)
        NANO_LOG_BYTES("tail_data",args->tail_data, args->tail_data_len))

    stream = NANO_XRCE_Session_lookup_stream(
                    &self->session, args->req.stream_id);
    if (stream == NULL)
    {
        NANO_LOG_ERROR("unknown stream for CREATE",
            NANO_LOG_SESSIONID("session",self->session.id)
            NANO_LOG_STREAMID("stream",args->req.stream_id))
        rc = NANO_RETCODE_INVALID_ARGS;
        goto done;
    }

    NANO_XRCE_Client_new_request(
        self, 
        args->req.stream_id, 
        NANO_XRCE_SUBMESSAGEID_CREATE,
        &obj_id,
        args->req.flags | NANO_XRCE_REQUESTFLAGS_CONFIRM,
        request_token,
        &req);
    if (req == NULL)
    {
        NANO_LOG_ERROR(
            "failed to allocate CREATE request", 
            NANO_LOG_STREAMID("stream_id", args->req.stream_id));
        rc = NANO_RETCODE_OUT_OF_RESOURCES;
        goto done;
    }

    create_mbuf = NANO_XRCE_Session_allocate_message(
                &self->session,
                stream,
                NANO_XRCE_MESSAGETYPE_INLINE_PAYLOAD,
                buffer_size,
                NULL);

    if (create_mbuf == NULL)
    {
        NANO_LOG_ERROR("failed to allocate CREATE message",
            NANO_LOG_STREAMID("stream", args->req.stream_id))
        rc = NANO_RETCODE_OUT_OF_RESOURCES;
        goto done;
    }
    
    repr_mbuf = NANO_XRCE_Session_allocate_message(
                    &self->session,
                    stream,
                    NANO_XRCE_MESSAGETYPE_EXTERNAL_PAYLOAD,
                    repr_len_tot,
                    args->repr);
    if (repr_mbuf == NULL)
    {
        NANO_LOG_ERROR("failed to allocate message for object representation",
            NANO_LOG_STREAMID("stream", args->req.stream_id))
        rc = NANO_RETCODE_OUT_OF_RESOURCES;
        goto done;
    }

    if (args->tail_data != NULL)
    {
        tail_mbuf = NANO_XRCE_Session_allocate_message(
                &self->session,
                stream,
                NANO_XRCE_MESSAGETYPE_INLINE_PAYLOAD_TAIL,
                NANO_MESSAGEBUFFER_MIN_INLINE_DATA_SIZE_BYTES,
                NULL);

        if (tail_mbuf == NULL)
        {
            NANO_LOG_ERROR("failed to allocate CREATE tail message",
                NANO_LOG_STREAMID("stream", args->req.stream_id))
            rc = NANO_RETCODE_OUT_OF_RESOURCES;
            goto done;
        }

        tail_pad = 
            NANO_OSAPI_Memory_align_size_up(
                    repr_len_tot, args->tail_data_align) - repr_len_tot;
        NANO_PCOND(
            NANO_MessageBuffer_data_len(tail_mbuf) >= 
                args->tail_data_len + tail_pad)

        /* Serialize domain id to buffer */
        data_ptr = NANO_MessageBuffer_data_ptr(tail_mbuf) + tail_pad;
        NANO_OSAPI_Memory_copy(data_ptr, args->tail_data, args->tail_data_len);
        NANO_MessageBuffer_set_data_len(
            tail_mbuf, args->tail_data_len + tail_pad);
    }

    create_payload = (NANO_XRCE_CreatePayload*)
                        NANO_MessageBuffer_data_ptr(create_mbuf);
    NANO_XRCE_RequestId_from_u16(
        &create_payload->base.request_id, req->req_id);
    create_payload->base.object_id = obj_id;
    
    *((NANO_XRCE_ObjectKind*)
        NANO_MessageBuffer_data_ptr(create_mbuf) +
        NANO_XRCE_BASEOBJECTREQUEST_SERIALIZED_SIZE_MAX) = args->obj_kind;

    *((NANO_XRCE_RepresentationFormat*)
        NANO_MessageBuffer_data_ptr(create_mbuf) +
        NANO_XRCE_BASEOBJECTREQUEST_SERIALIZED_SIZE_MAX +
        sizeof(NANO_XRCE_ObjectKind)) = args->repr_fmt;

    data_ptr = 
        NANO_OSAPI_Memory_align(
            NANO_MessageBuffer_data_ptr(create_mbuf) +
                NANO_XRCE_BASEOBJECTREQUEST_SERIALIZED_SIZE_MAX +
                sizeof(NANO_XRCE_ObjectKind) +
                sizeof(NANO_XRCE_RepresentationFormat), NANO_u32);
    
    *((NANO_u32*)data_ptr) = repr_len_tot;

    NANO_MessageBuffer_append(create_mbuf, repr_mbuf);

    if (tail_mbuf != NULL)
    {
        NANO_MessageBuffer_append(repr_mbuf, tail_mbuf);
        tail_mbuf = NULL;
    }
    repr_mbuf = NULL;
    
    NANO_LOG_DEBUG("CREATE request info",
        NANO_LOG_STREAMID("stream", args->req.stream_id)
        NANO_LOG_U16("req_id",req->req_id)
        NANO_LOG_OBJID("obj_id",req->obj_id))

    /* Send request using the session */
    NANO_CHECK_RC(
        NANO_XRCE_Session_send(
            &self->session,
            stream,
            req->submsg_id,
            req->submsg_flags,
            create_mbuf,
            &req->sn),
        create_mbuf = NULL;
        NANO_LOG_ERROR("failed to send CREATE request",
            NANO_LOG_RC(rc)));
    create_mbuf = NULL;
    
    NANO_LOG_TRACE("CREATE request SENT",
        NANO_LOG_STREAMID("stream", args->req.stream_id)
        NANO_LOG_U16("req_id",req->req_id)
        NANO_LOG_OBJID("obj_id",req->obj_id)
        NANO_LOG_SN("req_sn",req->sn))

    req = NULL;
    NANO_CHECK_RC(
        NANO_XRCE_Client_return_or_wait_for_request(
            self, req, request_token, args->req.timeout_ms),
        req = NULL;
        NANO_LOG_ERROR_MSG("FAILED to wait for request"));
    req = NULL;
    
    rc = NANO_RETCODE_OK;
    
done:
    if (repr_mbuf != NULL)
    {
        NANO_XRCE_Session_release_message(&self->session, stream, repr_mbuf);
    }
    if (tail_mbuf != NULL)
    {
        NANO_XRCE_Session_release_message(&self->session, stream, tail_mbuf);
    }
    if (create_mbuf != NULL)
    {
        NANO_XRCE_Session_release_message(&self->session, stream, create_mbuf);
    }
    if (req != NULL)
    {
        /* Dispose of unsuccessfull request */
        NANO_XRCE_Client_release_request(self, req);
    }

    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}
#endif /* NANO_FEAT_OBJECT_CREATE */

#if NANO_FEAT_OBJECT_DELETE

NANO_RetCode
NANO_XRCE_Client_delete_object_w_args(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientRequestToken *const request_token,
    const NANO_XRCE_RequestArgs *const args)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_XRCE_DeletePayload *delete_payload = NULL;
    NANO_MessageBuffer *delete_mbuf = NULL;
    NANO_XRCE_Stream *stream = NULL;
    NANO_XRCE_ClientRequest *req = NULL;
    
    NANO_LOG_FN_ENTRY
    
    NANO_PCOND(self != NULL)
    NANO_PCOND(args != NULL)

    stream = NANO_XRCE_Session_lookup_stream(
                    &self->session, args->stream_id);
    if (stream == NULL)
    {
        NANO_LOG_ERROR("unknown stream for DELETE",
            NANO_LOG_SESSIONID("session",self->session.id)
            NANO_LOG_STREAMID("stream",args->stream_id))
        rc = NANO_RETCODE_INVALID_ARGS;
        goto done;
    }

    delete_mbuf = NANO_XRCE_Session_allocate_message(
                    &self->session,
                    stream,
                    NANO_XRCE_MESSAGETYPE_INLINE_PAYLOAD,
                    NANO_XRCE_DELETEPAYLOAD_HEADER_SERIALIZED_SIZE_MAX,
                    NULL); 

    if (delete_mbuf == NULL)
    {
        NANO_LOG_ERROR("failed to allocate DELETE message",
            NANO_LOG_STREAMID("stream", args->stream_id))
        rc = NANO_RETCODE_OUT_OF_RESOURCES;
        goto done;
    }

    NANO_XRCE_Client_new_request(
        self, 
        args->stream_id, 
        NANO_XRCE_SUBMESSAGEID_DELETE,
        &args->obj_id,
        args->flags | NANO_XRCE_REQUESTFLAGS_CONFIRM,
        request_token,
        &req);
    if (req == NULL)
    {
        NANO_LOG_ERROR(
            "failed to allocate DELETE request", 
            NANO_LOG_STREAMID("stream_id", args->stream_id));
        rc = NANO_RETCODE_OUT_OF_RESOURCES;
        goto done;
    }

    delete_payload = (NANO_XRCE_DeletePayload*)
                        NANO_MessageBuffer_data_ptr(delete_mbuf);
    NANO_XRCE_RequestId_from_u16(
        &delete_payload->base.request_id, req->req_id);
    delete_payload->base.object_id = args->obj_id;
    
    /* Send request using the session */
    NANO_CHECK_RC(
        NANO_XRCE_Session_send(
            &self->session,
            stream,
            req->submsg_id,
            req->submsg_flags,
            delete_mbuf,
            &req->sn),
        delete_mbuf = NULL;
        NANO_LOG_ERROR("failed to send DELETE request",
            NANO_LOG_RC(rc)));
    delete_mbuf = NULL;
    
    /* Wait for request to be completed */
    NANO_CHECK_RC(
        NANO_XRCE_Client_return_or_wait_for_request(
                self, req, request_token, args->timeout_ms),
        req = NULL;
        NANO_LOG_ERROR_MSG("failed to wait for request"));
    req = NULL;

    rc = NANO_RETCODE_OK;
    
done:
    if (delete_mbuf != NULL)
    {
        NANO_XRCE_Session_release_message(&self->session, stream, delete_mbuf);
    }

    /* Release failed request */
    if (req != NULL)
    {
        NANO_XRCE_Client_release_request(self, req);
    }

    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}
#endif /* NANO_FEAT_OBJECT_DELETE */

#if NANO_FEAT_AGENT_DISCOVERY
NANO_RetCode
NANO_XRCE_Client_update_locator(
    NANO_XRCE_Client *const self,
    const NANO_XRCE_ClientTransportLocatorType locator_type,
    const NANO_XRCE_TransportLocator *const locator)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;

    NANO_LOG_FN_ENTRY
    
    NANO_PCOND(self != NULL)
    NANO_PCOND(NANO_XRCE_ClientTransportLocatorType_is_valid(locator_type))

    if (locator_type == NANO_XRCE_CLIENTTRANSPORTLOCATOR_AGENT &&
        NANO_XRCE_Client_connected(self))
    {
        NANO_LOG_ERROR_MSG("cannot update AGENT locator while connected to it")
        goto done;
    }

    NANO_CHECK_RC(
        NANO_XRCE_ClientTransport_update_locator(
            self->session.transport,
            locator_type,
            locator),
        NANO_LOG_ERROR("INVALID locator",
            NANO_LOG_ENUM("type",locator_type)
            NANO_LOG_LOCATOR("locator",locator)));

    switch (locator_type)
    {
    case NANO_XRCE_CLIENTTRANSPORTLOCATOR_AGENT:
    {
        if (locator == NULL)
        {
            self->flags |= NANO_XRCE_CLIENTFLAGS_DISCOVERING;
            NANO_LOG_DEBUG_MSG("set client to DISCOVERY mode")
        }
        else
        {
            self->flags &= ~NANO_XRCE_CLIENTFLAGS_DISCOVERING;
            NANO_LOG_DEBUG("found AGENT locator",
                NANO_LOG_LOCATOR("locator", locator))
        }
        break;
    }
    case NANO_XRCE_CLIENTTRANSPORTLOCATOR_METADATA:
    {
        if (locator == NULL)
        {
            NANO_LOG_DEBUG_MSG("disabled METADATA channel")
        }
        else
        {
            NANO_LOG_DEBUG("updated METADATA locator",
                    NANO_LOG_LOCATOR("locator", locator))
        }
        break;
    }
    default:
    {
        NANO_UNREACHABLE_CODE
        goto done;
    }
    }
    
    rc = NANO_RETCODE_OK;
    
done:

    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

NANO_RetCode
NANO_XRCE_Client_announce(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientRequestToken *const request_token,
    const NANO_XRCE_RequestArgs *const args)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_XRCE_GetInfoPayload *ginfo_payload = NULL;
    NANO_MessageBuffer *ginfo_mbuf = NULL;
    NANO_XRCE_Stream *stream = NULL;
    NANO_XRCE_ClientRequest *req = NULL;
    NANO_XRCE_ObjectId agent_id = NANO_XRCE_OBJECTID_INVALID;
    NANO_u8 *ptr = NULL;

    NANO_LOG_FN_ENTRY
    
    NANO_PCOND(self != NULL)

    NANO_XRCE_ObjectId_combine(
        &agent_id, NANO_XRCE_OBJK_AGENT, &agent_id)

    stream = &self->session.stream_none.base;

    ginfo_mbuf = NANO_XRCE_Session_allocate_message(
                    &self->session,
                    stream,
                    NANO_XRCE_MESSAGETYPE_INLINE_PAYLOAD,
                    NANO_XRCE_GETINFOPAYLOAD_HEADER_SERIALIZED_SIZE_MAX,
                    NULL); 

    if (ginfo_mbuf == NULL)
    {
        NANO_LOG_ERROR_MSG("failed to allocate GET_INFO message")
        rc = NANO_RETCODE_OUT_OF_RESOURCES;
        goto done;
    }

    NANO_XRCE_Client_new_request(
        self, 
        NANO_XRCE_STREAMID_NONE,
        NANO_XRCE_SUBMESSAGEID_GET_INFO,
        &agent_id,
        args->flags,
        request_token,
        &req);
    if (req == NULL)
    {
        NANO_LOG_ERROR_MSG("failed to allocate GET_INFO request")
        rc = NANO_RETCODE_OUT_OF_RESOURCES;
        goto done;
    }

    ginfo_payload = (NANO_XRCE_GetInfoPayload*)
                        NANO_MessageBuffer_data_ptr(ginfo_mbuf);
    NANO_XRCE_RequestId_from_u16(
        &ginfo_payload->base.request_id, req->req_id);
    ginfo_payload->base.object_id = agent_id;
    ptr = NANO_MessageBuffer_data_ptr(ginfo_mbuf) + 
            NANO_XRCE_BASEOBJECTREQUEST_SERIALIZED_SIZE_MAX;
    ptr = NANO_OSAPI_Memory_align(ptr, NANO_u32);
    *((NANO_XRCE_InfoMask*)ptr) = NANO_XRCE_INFOMASK_INFO_ACTIVITY;
    ptr += sizeof(NANO_XRCE_InfoMask);

    NANO_MessageBuffer_set_data_len(ginfo_mbuf,
        ptr - NANO_MessageBuffer_data_ptr(ginfo_mbuf));

    req->obj_id = ginfo_payload->base.object_id;
    
    NANO_MessageBuffer_flags_set(
        ginfo_mbuf, NANO_XRCE_MESSAGEFLAGS_DISCOVERY);

    NANO_LOG_DEBUG("ANNOUNCING client",
        NANO_LOG_MBUF("payload", ginfo_mbuf))

    /* Send request using the session */
    NANO_CHECK_RC(
        NANO_XRCE_Session_send(
            &self->session,
            stream,
            req->submsg_id,
            req->submsg_flags,
            ginfo_mbuf,
            &req->sn),
        ginfo_mbuf = NULL;
        NANO_LOG_ERROR("failed to send GET_INFO request",
            NANO_LOG_RC(rc)));
    ginfo_mbuf = NULL;

    NANO_CHECK_RC(
        NANO_XRCE_Client_return_or_wait_for_request(
            self, req, request_token, args->timeout_ms),
        req = NULL;
        NANO_LOG_ERROR_MSG("FAILED to wait for request"));
    
    req = NULL;

    if (args->flags & NANO_XRCE_REQUESTFLAGS_SYNC)
    {
        self->flags &= ~NANO_XRCE_CLIENTFLAGS_DISCOVERING;
    }
    
    rc = NANO_RETCODE_OK;
    
done:
    if (ginfo_mbuf != NULL)
    {
        NANO_XRCE_Session_release_message(&self->session, stream, ginfo_mbuf);
    }
    if (rc != NANO_RETCODE_OK)
    {
        /* Release failed request */
        if (req != NULL)
        {
            NANO_XRCE_Client_release_request(self, req);
        }
        self->flags &= ~NANO_XRCE_CLIENTFLAGS_DISCOVERING;
    }

    NANO_LOG_FN_EXIT_RC(rc)

    return rc;
}

#endif /* NANO_FEAT_AGENT_DISCOVERY */


#if NANO_FEAT_OBJECT_CREATE && NANO_FEAT_EXTENDED_API

NANO_RetCode
NANO_XRCE_Client_create_object_xml(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientRequestToken *const request_token,
    const NANO_XRCE_StreamId request_stream,
    const NANO_XRCE_RequestFlags request_flags,
    const NANO_Timeout request_timeout_ms,
    const NANO_XRCE_ObjectKind object_kind,
    const NANO_u16 object_id,
    const NANO_u16 parent_id,
    const char *const object_xml_repr)
{
    NANO_XRCE_CreateArgs args = NANO_XRCE_CREATEARGS_INITIALIZER;
    const NANO_XRCE_DomainId domain_id = 0;
    NANO_XRCE_ObjectId parent_obj_id = NANO_XRCE_OBJECTID_INVALID;

    NANO_XRCE_ObjectId_from_u16(&parent_obj_id, parent_id);

    args.req.flags = request_flags;
    NANO_XRCE_ObjectId_set(&args.req.obj_id, object_kind, object_id);
    args.req.stream_id = request_stream;
    args.req.timeout_ms = request_timeout_ms;
    args.repr_fmt = NANO_XRCE_REPRESENTATION_AS_XML_STRING;
    args.repr = (NANO_u8*)object_xml_repr;
    args.repr_len = NANO_OSAPI_String_length(object_xml_repr);

    switch (object_kind)
    {
#if NANO_FEAT_OBJECT_KIND_PARTICIPANT
    case NANO_XRCE_OBJK_PARTICIPANT:
    {
        args.tail_data = (NANO_u8*)&domain_id;
        args.tail_data_len = sizeof(domain_id);
        args.tail_data_align = sizeof(NANO_XRCE_DomainId);
        break;
    }
#endif /* NANO_FEAT_OBJECT_KIND_PARTICIPANT */
#if NANO_FEAT_OBJECT_KIND_PUBLISHER
    case NANO_XRCE_OBJK_PUBLISHER:
    {
        args.tail_data = parent_obj_id.value;
        args.tail_data_len = sizeof(NANO_XRCE_ObjectId);
        args.tail_data_align = NANO_XRCE_ObjectId_alignment;
        break;
    }
#endif /* NANO_FEAT_OBJECT_KIND_PUBLISHER */
#if NANO_FEAT_OBJECT_KIND_SUBSCRIBER
    case NANO_XRCE_OBJK_SUBSCRIBER:
    {
        args.tail_data = parent_obj_id.value;
        args.tail_data_len = sizeof(NANO_XRCE_ObjectId);
        args.tail_data_align = NANO_XRCE_ObjectId_alignment;
        break;
    }
#endif /* NANO_FEAT_OBJECT_KIND_SUBSCRIBER */
#if NANO_FEAT_OBJECT_KIND_DATAWRITER
    case NANO_XRCE_OBJK_DATAWRITER:
    {
        args.tail_data = parent_obj_id.value;
        args.tail_data_len = sizeof(NANO_XRCE_ObjectId);
        args.tail_data_align = NANO_XRCE_ObjectId_alignment;
        break;
    }
#endif /* NANO_FEAT_OBJECT_KIND_DATAWRITER */
#if NANO_FEAT_OBJECT_KIND_DATAREADER
    case NANO_XRCE_OBJK_DATAREADER:
    {
        args.tail_data = parent_obj_id.value;
        args.tail_data_len = sizeof(NANO_XRCE_ObjectId);
        args.tail_data_align = NANO_XRCE_ObjectId_alignment;
        break;
    }
#endif /* NANO_FEAT_OBJECT_KIND_DATAREADER */
    default:
        break;
    }

    return NANO_XRCE_Client_create_object_w_args(self, request_token, &args);
}

NANO_RetCode
NANO_XRCE_Client_register_object(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientRequestToken *const request_token,
    const NANO_XRCE_StreamId request_stream,
    const NANO_XRCE_RequestFlags request_flags,
    const NANO_Timeout request_timeout_ms,
    const NANO_XRCE_ObjectKind object_kind,
    const NANO_u16 object_id,
    const NANO_u16 parent_id,
    const char *const object_ref)
{
    NANO_XRCE_CreateArgs args = NANO_XRCE_CREATEARGS_INITIALIZER;
    const NANO_XRCE_DomainId domain_id = 0;
    NANO_XRCE_ObjectId parent_obj_id = NANO_XRCE_OBJECTID_INVALID;

    NANO_XRCE_ObjectId_from_u16(&parent_obj_id, parent_id);

    args.req.flags = request_flags;
    NANO_XRCE_ObjectId_from_u16(&args.req.obj_id, object_id);
    args.req.stream_id = request_stream;
    args.req.timeout_ms = request_timeout_ms;
    args.repr_fmt = NANO_XRCE_REPRESENTATION_BY_REFERENCE;
    args.repr = (NANO_u8*)object_ref;
    args.repr_len = NANO_OSAPI_String_length(object_ref);

    switch (object_kind)
    {
#if NANO_FEAT_OBJECT_KIND_PARTICIPANT
    case NANO_XRCE_OBJK_PARTICIPANT:
    {
        args.tail_data = (NANO_u8*)&domain_id;
        args.tail_data_len = sizeof(domain_id);
        args.tail_data_align = sizeof(NANO_XRCE_DomainId);
        break;
    }
#endif /* NANO_FEAT_OBJECT_KIND_PARTICIPANT */
#if NANO_FEAT_OBJECT_KIND_PUBLISHER
    case NANO_XRCE_OBJK_PUBLISHER:
    {
        args.tail_data = parent_obj_id.value;
        args.tail_data_len = sizeof(NANO_XRCE_ObjectId);
        args.tail_data_align = NANO_XRCE_ObjectId_alignment;
        break;
    }
#endif /* NANO_FEAT_OBJECT_KIND_PUBLISHER */
#if NANO_FEAT_OBJECT_KIND_SUBSCRIBER
    case NANO_XRCE_OBJK_SUBSCRIBER:
    {
        args.tail_data = parent_obj_id.value;
        args.tail_data_len = sizeof(NANO_XRCE_ObjectId);
        args.tail_data_align = NANO_XRCE_ObjectId_alignment;
        break;
    }
#endif /* NANO_FEAT_OBJECT_KIND_SUBSCRIBER */
#if NANO_FEAT_OBJECT_KIND_DATAWRITER
    case NANO_XRCE_OBJK_DATAWRITER:
    {
        args.tail_data = parent_obj_id.value;
        args.tail_data_len = sizeof(NANO_XRCE_ObjectId);
        args.tail_data_align = NANO_XRCE_ObjectId_alignment;
        break;
    }
#endif /* NANO_FEAT_OBJECT_KIND_DATAWRITER */
#if NANO_FEAT_OBJECT_KIND_DATAREADER
    case NANO_XRCE_OBJK_DATAREADER:
    {
        args.tail_data = parent_obj_id.value;
        args.tail_data_len = sizeof(NANO_XRCE_ObjectId);
        args.tail_data_align = NANO_XRCE_ObjectId_alignment;
        break;
    }
#endif /* NANO_FEAT_OBJECT_KIND_DATAREADER */
    default:
        break;
    }

    return NANO_XRCE_Client_create_object_w_args(self, request_token, &args);
}

#endif /* NANO_FEAT_OBJECT_CREATE && NANO_FEAT_EXTENDED_API */


#if NANO_FEAT_EXTENDED_API

NANO_RetCode
NANO_XRCE_Client_read_data(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientRequestToken *const request_token,
    const NANO_XRCE_StreamId request_stream,
    const NANO_XRCE_RequestFlags request_flags,
    const NANO_Timeout request_timeout_ms,
    const NANO_u16 reader_id,
    const NANO_XRCE_StreamId read_stream,
    const NANO_XRCE_DataDeliveryControl *const data_delivery_ctrl,
    const char *const content_filter)
{
    NANO_XRCE_ReadArgs read_args = NANO_XRCE_READARGS_INITIALIZER;

    read_args.req.stream_id = request_stream;
    read_args.req.timeout_ms = request_timeout_ms;
    read_args.req.flags = request_flags;
    NANO_XRCE_ObjectId_from_u16(&read_args.req.obj_id, reader_id);

    if (content_filter != NULL)
    {
        read_args.read_spec.has_content_filter_expr = NANO_BOOL_TRUE;
        read_args.read_spec.content_filter_expr.value = (char*)content_filter;
    }
    if (data_delivery_ctrl != NULL)
    {
        read_args.read_spec.has_delivery_ctrl = NANO_BOOL_TRUE;
        read_args.read_spec.delivery_ctrl = *data_delivery_ctrl;
    }
    read_args.read_spec.data_format = NANO_XRCE_FORMAT_DATA;
    read_args.read_spec.preferred_stream_id = read_stream;

    return NANO_XRCE_Client_read_w_args(self, request_token, &read_args);
}


#endif /* NANO_FEAT_EXTENDED_API */
