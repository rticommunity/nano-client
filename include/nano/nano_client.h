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
 ******************************************************************************/ 

/**
 * @file nano_client.h
 * @brief nano-client API.
 * 
 * This header file should be included to access the services provided by the
 * nano-client library.
 * 
 */

#ifndef nano_client_h
#define nano_client_h

#ifdef __cplusplus
extern "C" {
#endif

#include "nano/nano_client_config.h"

/******************************************************************************
 *                          Client (forward)
 ******************************************************************************/

/*e
 * @ingroup nano_api_client
 * @brief An object used to establish a session on a remote XRCE Agent and access
 * its services.
 * 
 * A NANO_XRCE_Client encapsulate all the state required by an application to
 * communicate with an XRCE Agent, and operate on the XRCE Objects it contains.
 * 
 */
typedef struct NANODllExport NANO_XRCE_ClientI NANO_XRCE_Client;

/*i
 * @brief A token object representing an XRCE request sent to the Agent.
 * 
 */
typedef struct NANODllExport NANO_XRCE_ClientRequestI NANO_XRCE_ClientRequest;

#if NANO_FEAT_SUBSCRIBE ||\
    NANO_FEAT_PUBLISH ||\
    NANO_FEAT_AGENT_DISCOVERY ||\
    NANO_FEAT_OBJECT_INFO ||\
    NANO_FEAT_RELIABILITY
#define NANO_HAVE_CLIENT_LISTENER
#else
#undef NANO_HAVE_CLIENT_LISTENER
#endif

#ifdef NANO_HAVE_CLIENT_LISTENER
/******************************************************************************
 *                              ClientListener
 ******************************************************************************/
/**
 * @addtogroup nano_api_clientlistener
 * @{
 */
/*e
 * @brief A listener object which will be notified of events occuring on a
 * NANO_XRCE_Client.
 * 
 * 
 */
typedef struct NANODllExport NANO_XRCE_ClientListenerI NANO_XRCE_ClientListener;

#if NANO_FEAT_SUBSCRIBE
/*e
 * @brief TODO
 * 
 * @param self 
 * @param client 
 * @param stream_id 
 * @param read_request 
 * @param reader 
 * @param format 
 * @param little_endian 
 * @param data 
 * @param data_len 
 */
typedef void (*NANO_XRCE_ClientListener_OnDataFn)(
    NANO_XRCE_ClientListener *const self,
    NANO_XRCE_Client *const client,
    const NANO_XRCE_StreamId stream_id,
    const NANO_XRCE_RequestId *const read_request,
    const NANO_XRCE_ObjectId *const reader,
    const NANO_XRCE_DataFormat format,
    const NANO_CDR_Endianness endianness,
    const NANO_u8 *const data,
    const NANO_usize data_len);
#endif /* NANO_FEAT_SUBSCRIBE */

#if NANO_FEAT_RELIABILITY
#if NANO_FEAT_PUBLISH
/*e
 * @brief TODO
 * 
 * @param self 
 * @param client 
 * @param write_req 
 */
typedef void (*NANO_XRCE_ClientListener_OnWriteComplete)(
    NANO_XRCE_ClientListener *const self,
    NANO_XRCE_Client *const client,
    const NANO_XRCE_ClientRequest *const write_req);
#endif /* NANO_FEAT_PUBLISH */

/*e
 * @brief TODO
 * 
 * @param self 
 * @param client 
 * @param stream_id 
 * @param lost 
 * @param reason 
 */
typedef void
    (*NANO_XRCE_ClientListener_OnMessageLostFn)(
        NANO_XRCE_ClientListener *const self,
        NANO_XRCE_Client *const client,
        const NANO_XRCE_StreamId stream_id,
        const NANO_XRCE_SeqNum lost,
        const NANO_XRCE_ReliableMessageLostReason reason);

#endif /* NANO_FEAT_RELIABILITY */

#if NANO_FEAT_OBJECT_INFO
/*e
 * @brief TODO
 * 
 * @param self 
 * @param client 
 * @param info 
 */
typedef void
    (*NANO_XRCE_ClientListener_OnInfoFn)(
        NANO_XRCE_ClientListener *const self,
        NANO_XRCE_Client *const client,
        const NANO_XRCE_ObjectInfo *const info);
#endif /* NANO_FEAT_OBJECT_INFO */

#if NANO_FEAT_AGENT_DISCOVERY
/*e
 * @brief TODO
 * 
 * @param self 
 * @param client 
 * @param agent_locator 
 * @param select 
 */
typedef void
    (*NANO_XRCE_ClientListener_OnAgentDiscoveredFn)(
        NANO_XRCE_ClientListener *const self,
        NANO_XRCE_Client *const client,
        const NANO_XRCE_TransportLocator *const agent_locator,
        NANO_bool *const select);
#endif /* NANO_FEAT_AGENT_DISCOVERY */

#if NANO_FEAT_SERVICE_CLIENT
/*e
 * @brief TODO
 *
 * @param self
 * @param client
 * @param svc_request
 * @param svc_resource
 * @param svc_flags
 * @param little_endian
 * @param data
 * @param data_len
 */
typedef void
    (*NANO_XRCE_ClientListener_OnServiceReplyFn)(
        NANO_XRCE_ClientListener *const self,
        NANO_XRCE_Client *const client,
        const NANO_XRCE_RequestId *const svc_request,
        const NANO_XRCE_ObjectId *const svc_resource,
        const NANO_bool little_endian,
        const NANO_XRCE_ServiceReplyStatus svc_status,
        const NANO_u8 *const data,
        const NANO_usize data_len,
        const NANO_u8 *const metadata,
        const NANO_usize metadata_len);
#endif /* NANO_FEAT_SERVICE_CLIENT */

/*e
 * @brief TODO
 * 
 */
struct NANO_XRCE_ClientListenerI
{
#if NANO_FEAT_SUBSCRIBE
    /*e
     * @brief TODO
     * 
     */
    NANO_XRCE_ClientListener_OnDataFn on_data;
#endif /* NANO_FEAT_SUBSCRIBE */
#if NANO_FEAT_RELIABILITY
#if NANO_FEAT_PUBLISH
    /*e
     * @brief TODO
     * 
     */
    NANO_XRCE_ClientListener_OnWriteComplete on_write_complete;
#endif /* NANO_FEAT_PUBLISH */
    /*e
     * @brief TODO
     * 
     */
    NANO_XRCE_ClientListener_OnMessageLostFn on_message_lost;
#endif /* NANO_FEAT_RELIABILITY */
#if NANO_FEAT_OBJECT_INFO
    /*e
     * @brief TODO
     * 
     */
    NANO_XRCE_ClientListener_OnInfoFn on_info;
#endif /* NANO_FEAT_OBJECT_INFO */
#if NANO_FEAT_AGENT_DISCOVERY
    /*e
     * @brief TODO
     * 
     */
    NANO_XRCE_ClientListener_OnAgentDiscoveredFn on_agent_discovered;
#endif /* NANO_FEAT_AGENT_DISCOVERY */
#if NANO_FEAT_SERVICE_CLIENT
    /*e
     * @brief TODO
     *
     */
    NANO_XRCE_ClientListener_OnServiceReplyFn on_service_reply;
#endif /* NANO_FEAT_SERVICE_CLIENT */
    /*e
     * @brief TODO
     * 
     */
    void *user_data;

#if NANO_CPP

    NANO_XRCE_ClientListenerI()
    : 
#if NANO_FEAT_SUBSCRIBE
        on_data(NULL),
#endif /* NANO_FEAT_SUBSCRIBE */
#if NANO_FEAT_RELIABILITY
#if NANO_FEAT_PUBLISH
        on_write_complete(NULL),
#endif /* NANO_FEAT_PUBLISH */
        on_message_lost(NULL),
#endif /* NANO_FEAT_RELIABILITY */
#if NANO_FEAT_OBJECT_INFO
        on_info(NULL),
#endif /* NANO_FEAT_OBJECT_INFO */
#if NANO_FEAT_AGENT_DISCOVERY
        on_agent_discovered(NULL),
#endif /* NANO_FEAT_AGENT_DISCOVERY */
        user_data(NULL)
    { }

#endif /* NANO_CPP */
};

#if NANO_FEAT_SUBSCRIBE
#define NANO_XRCE_CLIENTLISTENER_SUBSCRIBE_INITIALIZER \
        NULL,
#else
#define NANO_XRCE_CLIENTLISTENER_SUBSCRIBE_INITIALIZER
#endif /* NANO_FEAT_SUBSCRIBE */

#if NANO_FEAT_RELIABILITY
#if NANO_FEAT_PUBLISH
#define NANO_XRCE_CLIENTLISTENER_RELIABILITY_INITIALIZER \
        NULL,\
        NULL,
#else
#define NANO_XRCE_CLIENTLISTENER_RELIABILITY_INITIALIZER \
        NULL,
#endif /* NANO_FEAT_PUBLISH */
#else
#define NANO_XRCE_CLIENTLISTENER_RELIABILITY_INITIALIZER
#endif /* NANO_FEAT_RELIABILITY */

#if NANO_FEAT_OBJECT_INFO
#define NANO_XRCE_CLIENTLISTENER_INFO_INITIALIZER \
    NULL,
#else
#define NANO_XRCE_CLIENTLISTENER_INFO_INITIALIZER
#endif /* NANO_FEAT_OBJECT_INFO */

#if NANO_FEAT_AGENT_DISCOVERY
#define NANO_XRCE_CLIENTLISTENER_DISCOVERY_INITIALIZER \
    NULL,
#else
#define NANO_XRCE_CLIENTLISTENER_DISCOVERY_INITIALIZER
#endif /* NANO_FEAT_AGENT_DISCOVERY */

#if NANO_FEAT_SERVICE_CLIENT
#define NANO_XRCE_CLIENTLISTENER_SERVICE_INITIALIZER \
    NULL,
#else
#define NANO_XRCE_CLIENTLISTENER_SERVICE_INITIALIZER
#endif /* NANO_FEAT_SERVICE_CLIENT */

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_CLIENTLISTENER_INITIALIZER \
{\
    NANO_XRCE_CLIENTLISTENER_SUBSCRIBE_INITIALIZER /* subscribe */\
    NANO_XRCE_CLIENTLISTENER_RELIABILITY_INITIALIZER /* reliability */\
    NANO_XRCE_CLIENTLISTENER_INFO_INITIALIZER /* info */\
    NANO_XRCE_CLIENTLISTENER_DISCOVERY_INITIALIZER /* discovery */\
    NANO_XRCE_CLIENTLISTENER_SERVICE_INITIALIZER /* service */\
    NULL /* user_data */\
}

#if NANO_FEAT_SUBSCRIBE
void 
NANO_XRCE_ClientListener_on_data(
    NANO_XRCE_ClientListener *const self,
    NANO_XRCE_Client *const client,
    const NANO_XRCE_StreamId stream_id,
    const NANO_XRCE_RequestId *const read_request,
    const NANO_XRCE_ObjectId *const reader,
    const NANO_XRCE_DataFormat format,
    const NANO_bool little_endian,
    const NANO_u8 *const data,
    const NANO_usize data_len);

#define NANO_XRCE_ClientListener_on_data(s_,c_,st_,rr_,r_,f_,le_,d_,dl_)\
{\
    if ((s_)->on_data != NULL)\
    {\
        (s_)->on_data((s_),(c_),(st_),(rr_),(r_),(f_),(le_),(d_),(dl_));\
    }\
}
#endif /* NANO_FEAT_SUBSCRIBE */

#if NANO_FEAT_RELIABILITY
#if NANO_FEAT_PUBLISH
void 
NANO_XRCE_ClientListener_on_write_complete(
    NANO_XRCE_ClientListener *const self,
    NANO_XRCE_Client *const client,
    const NANO_XRCE_ClientRequest *const write_req);

#define NANO_XRCE_ClientListener_on_write_complete(s_,c_,r_)\
{\
    if ((s_)->on_write_complete != NULL)\
    {\
        (s_)->on_write_complete((s_),(c_),(r_));\
    }\
}
#endif /* NANO_FEAT_PUBLISH */

void 
NANO_XRCE_ClientListener_on_message_lost(
    NANO_XRCE_ClientListener *const self,
        NANO_XRCE_Client *const client,
        const NANO_XRCE_StreamId stream_id,
        const NANO_XRCE_SeqNum lost,
        const NANO_XRCE_ReliableMessageLostReason reason);

#define NANO_XRCE_ClientListener_on_message_lost(s_,c_,st_,l_,r_)\
{\
    if ((s_)->on_message_lost != NULL)\
    {\
        (s_)->on_message_lost((s_),(c_),(st_),(l_),(r_));\
    }\
}

#endif /* NANO_FEAT_RELIABILITY */

#if NANO_FEAT_OBJECT_INFO
void 
NANO_XRCE_ClientListener_on_info(
    NANO_XRCE_ClientListener *const self,
        NANO_XRCE_Client *const client,
        const NANO_XRCE_ObjectInfo *const info);

#define NANO_XRCE_ClientListener_on_info(s_,c_,i_)\
{\
    if ((s_)->on_info != NULL)\
    {\
        (s_)->on_info((s_),(c_),(i_));\
    }\
}
#endif /* NANO_FEAT_OBJECT_INFO */

#if NANO_FEAT_AGENT_DISCOVERY
void 
NANO_XRCE_ClientListener_on_agent_discovered(
    NANO_XRCE_ClientListener *const self,
    NANO_XRCE_Client *const client,
    const NANO_XRCE_TransportLocator *const agent_locator,
    NANO_bool *const select);

#define NANO_XRCE_ClientListener_on_agent_discovered(s_,c_,a_,sl_)\
{\
    if ((s_)->on_agent_discovered != NULL)\
    {\
        (s_)->on_agent_discovered((s_),(c_),(a_),(sl_));\
    }\
}
#endif /* NANO_FEAT_AGENT_DISCOVERY */

#if NANO_FEAT_SERVICE_CLIENT
void
NANO_XRCE_ClientListener_on_service_reply(
    NANO_XRCE_ClientListener *const self,
    NANO_XRCE_Client *const client,
    const NANO_XRCE_RequestId *const svc_request,
    const NANO_XRCE_ObjectId *const svc_resource,
    const NANO_bool little_endian,
    const NANO_XRCE_ServiceReplyStatus svc_status,
    const NANO_u8 *const data,
    const NANO_usize data_len,
    const NANO_u8 *const metadata,
    const NANO_usize metadata_len);

#define NANO_XRCE_ClientListener_on_service_reply(s_,c_,sreq_,sres_,sf_,le_,d_,dlen_,md_,mdlen_)\
{\
    if ((s_)->on_service_reply != NULL)\
    {\
        (s_)->on_service_reply((s_), (c_), (sreq_), (sres_), (sf_), (le_), (d_), (dlen_), (md_), (mdlen_));\
    }\
}
#endif /* NANO_FEAT_SERVICE_CLIENT */

/** @} *//* nano_api_clientlistener */

#endif /* NANO_HAVE_CLIENT_LISTENER */


/******************************************************************************
 *                                  Client
 ******************************************************************************/
/**
 * @addtogroup nano_api_clientrequests
 * @{
 */

/*e
 * @brief TODO
 * 
 */
typedef NANO_u16 NANO_XRCE_RequestFlags;

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_REQUESTFLAGS_LITTLE_ENDIAN        (0x01 << 0)
/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_REQUESTFLAGS_CREATE_REUSE         (0x01 << 1)
/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_REQUESTFLAGS_CREATE_REPLACE       (0x01 << 2)
/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_REQUESTFLAGS_CONFIRM              (0x01 << 3)
/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_REQUESTFLAGS_SYNC                 (0x01 << 4)
/*i
 * @brief TODO
 * 
 */
#define NANO_XRCE_REQUESTFLAGS_ACKED                (0x01 << 5)
/*i
 * @brief TODO
 * 
 */
#define NANO_XRCE_REQUESTFLAGS_DATA                 (0x01 << 6)
/*i
 * @brief TODO
 * 
 */
#define NANO_XRCE_REQUESTFLAGS_COMPLETE             (0x01 << 7)
/*i
 * @brief TODO
 * 
 */
#define NANO_XRCE_REQUESTFLAGS_IN_USE               (0x01 << 8)
/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_REQUESTFLAGS_CUSTOM_ENDIANNESS    (0x01 << 9)
/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_REQUESTFLAGS_ONEWAY               (0x01 << 10)
/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_REQUESTFLAGS_DEFAULT              0


struct NANODllExport NANO_XRCE_ClientRequestI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_ClientRequest *next;
    /*i
     * @brief TODO
     * 
     */
    NANO_u16 req_id;
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_RequestFlags flags;
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_ResultStatus result;
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_ObjectId obj_id;
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_SeqNum sn;
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_SubmessageFlags submsg_flags;
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_SubmessageId submsg_id;
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_StreamId stream_id;

#if NANO_CPP
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_ClientRequestI()
    : next(NULL),
      req_id(0),
      flags(NANO_XRCE_REQUESTFLAGS_DEFAULT),
      result(),
      obj_id(),
      sn(),
      submsg_flags(NANO_XRCE_SUBMESSAGEFLAGS_DEFAULT),
      submsg_id(NANO_XRCE_SUBMESSAGEID_UNKNOWN),
      stream_id(NANO_XRCE_STREAMID_NONE)
    {}
#endif /* NANO_CPP */
};

/*i
 * @brief TODO
 * 
 */
#define NANO_XRCE_CLIENTREQUEST_INITIALIZER \
{\
    NULL, /* next */\
    0, /* req_id */ \
    NANO_XRCE_REQUESTFLAGS_DEFAULT, /* flags */\
    NANO_XRCE_RESULTSTATUS_INITIALIZER, /* result */\
    NANO_XRCE_OBJECTID_INVALID, /* obj_id */\
    NANO_XRCE_SEQNUM_INITIALIZER, /* sn */\
    NANO_XRCE_SUBMESSAGEFLAGS_DEFAULT, /* submsg_flags */\
    NANO_XRCE_SUBMESSAGEID_UNKNOWN, /* submsg_id */\
    NANO_XRCE_STREAMID_NONE /* stream_id */ \
}

/*i
 * @brief TODO
 * 
 */
#define NANO_XRCE_ClientRequest_is_reliable(s_) \
    (NANO_XRCE_StreamId_is_reliable((s_)->stream_id))
/*i
 * @brief TODO
 * 
 */
#define NANO_XRCE_ClientRequest_is_acked(s_) \
    ((s_)->flags & NANO_XRCE_REQUESTFLAGS_ACKED)
/*i
 * @brief TODO
 * 
 */
#define NANO_XRCE_ClientRequest_is_complete(s_) \
    (((s_)->flags & NANO_XRCE_REQUESTFLAGS_COMPLETE) &&\
        (!NANO_XRCE_ClientRequest_is_reliable(s_) ||\
            NANO_XRCE_ClientRequest_is_acked(s_)))

/*e
 * @brief TODO
 * 
 */
typedef struct NANO_XRCE_ClientRequestTokenI
{
    /*e
     * @brief TODO
     * 
     */
    NANO_XRCE_RequestId request_id;
    /*e
     * @brief TODO
     * 
     */
    NANO_XRCE_ObjectId object_id;
    /*e
     * @brief TODO
     * 
     */
    NANO_XRCE_SeqNum sn;
    /*e
     * @brief TODO
     * 
     */
    NANO_XRCE_StreamId stream_id;
} NANO_XRCE_ClientRequestToken;

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_CLIENTREQUESTTOKEN_INITIALIZER \
{\
    NANO_XRCE_REQUESTID_INVALID, /* request_id */ \
    NANO_XRCE_OBJECTID_INVALID, /* object_id */ \
    NANO_XRCE_SEQNUM_INITIALIZER, /* sn */ \
    NANO_XRCE_STREAMID_NONE /* stream_id */ \
}

/** @} *//* nano_api_clientrequests */

/******************************************************************************
 *                       ClientRequestsStorage
 ******************************************************************************/

/**
 * @addtogroup nano_api_storage_clientreqs
 * @{
 */

/*e
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_ClientRequestsStorageI
{
    /*e
     * @brief TODO
     * 
     */
    NANO_XRCE_ClientRequest *client_requests;
    /*e
     * @brief TODO
     * 
     */
    NANO_usize client_requests_len;
    /*e
     * @brief TODO
     * 
     */
    NANO_usize client_requests_max;
} NANO_XRCE_ClientRequestsStorage;

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_CLIENTREQUESTSSTORAGE_INITIALIZER \
{\
    NULL, /* client_requests */\
    0, /* client_requests_len */\
    0 /* client_requests_max */ \
}

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_ClientRequestsStorage_initialize(s_, client_reqs_, client_reqs_max_) \
{\
    (s_)->client_requests = (client_reqs_);\
    (s_)->client_requests_max = (client_reqs_max_);\
    (s_)->client_requests_len = 0;\
}

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_CUSTOMCLIENTREQUESTSSTORAGE_TYPE_MEMBERS(max_requests_) \
    NANO_XRCE_ClientRequestsStorage base;\
    NANO_XRCE_ClientRequest client_requests[max_requests_]

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_CUSTOMCLIENTREQUESTSSTORAGE_INITIALIZER \
{\
    NANO_XRCE_CLIENTREQUESTSSTORAGE_INITIALIZER, /* base */\
    { NANO_XRCE_CLIENTREQUEST_INITIALIZER } /* client_requests */\
}

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_CUSTOMCLIENTREQUESTSSTORAGE_INITIALIZE(s_) \
{\
    NANO_XRCE_ClientRequestsStorage_initialize(&(s_)->base,\
        (s_)->client_requests,\
        sizeof((s_)->client_requests) / sizeof(NANO_XRCE_ClientRequest));\
}

/** @} *//* nano_api_storage_clientreqs */

/******************************************************************************
 * ClientStorage
 ******************************************************************************/
/**
 * @addtogroup nano_api_storage_client
 * @{
 */

/*e
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_ClientStorageI
{
    NANO_XRCE_ClientRequestsStorage *requests;
    NANO_XRCE_SessionStorage *session;
} NANO_XRCE_ClientStorage;

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_CLIENTSTORAGE_INITIALIZER \
{\
    NULL, /* requests */\
    NULL /* session */\
}

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_ClientStorage_initialize(s_, reqs_, session_) \
{\
    (s_)->requests = (reqs_);\
    (s_)->session = (session_);\
}

/******************************************************************************
 * CustomClientStorage
 ******************************************************************************/

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_CUSTOMCLIENTSTORAGE_TYPE_MEMBERS(base_type_name_,session_storage_) \
    NANO_XRCE_ClientStorage base;\
    xrce_str_concat(base_type_name_,ClientRequestsStorage) requests;\
    session_storage_ session

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_CUSTOMCLIENTSTORAGE_DEFINE_TYPES_W_SESSION(base_type_name_,max_requests_,session_storage_)\
\
typedef struct xrce_str_concat(base_type_name_,ClientRequestsStorageI)\
{\
    NANO_XRCE_CUSTOMCLIENTREQUESTSSTORAGE_TYPE_MEMBERS(max_requests_);\
    NANO_XRCE_CUSTOMSTORAGE_CPP_CONSTRUCTOR(\
        xrce_str_concat(base_type_name_,ClientRequestsStorageI),\
        NANO_XRCE_CUSTOMCLIENTREQUESTSSTORAGE_INITIALIZE(this);)\
} xrce_str_concat(base_type_name_,ClientRequestsStorage);\
\
typedef struct xrce_str_concat(base_type_name_,ClientStorageI)\
{\
    NANO_XRCE_CUSTOMCLIENTSTORAGE_TYPE_MEMBERS(base_type_name_,session_storage_);\
    NANO_XRCE_CUSTOMSTORAGE_CPP_CONSTRUCTOR(\
        xrce_str_concat(base_type_name_,ClientStorageI),\
        NANO_XRCE_CUSTOMCLIENTSTORAGE_INITIALIZE(this);)\
} xrce_str_concat(base_type_name_,ClientStorage)

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_CUSTOMCLIENTSTORAGE_DEFINE_TYPES(base_type_name_,max_requests_,builtin_rel_send_queue_len_,builtin_rel_max_payload_in_,max_user_streams_be_,max_user_streams_rel_)\
\
NANO_XRCE_CUSTOMSESSIONSTORAGE_DEFINE_TYPES(base_type_name_,builtin_rel_send_queue_len_,builtin_rel_max_payload_in_,max_user_streams_be_,max_user_streams_rel_) \
\
NANO_XRCE_CUSTOMCLIENTSTORAGE_DEFINE_TYPES_W_SESSION(base_type_name_,max_requests_,xrce_str_concat(base_type_name_,SessionStorage))

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_CUSTOMCLIENTSTORAGE_INITIALIZER \
    NANO_XRCE_CUSTOMCLIENTSTORAGE_INITIALIZER_W_SESSION(\
        NANO_XRCE_CUSTOMSESSIONSTORAGE_INITIALIZER)

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_CUSTOMCLIENTSTORAGE_INITIALIZER_W_SESSION(session_initializer_) \
{\
    NANO_XRCE_CLIENTSTORAGE_INITIALIZER, /* base */\
    NANO_XRCE_CUSTOMCLIENTREQUESTSSTORAGE_INITIALIZER, /* requests */\
    session_initializer_ /* session */\
}

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_CUSTOMCLIENTSTORAGE_INITIALIZE(s_) \
    NANO_XRCE_CUSTOMCLIENTSTORAGE_INITIALIZE_W_SESSION(s_,\
    {\
        NANO_XRCE_CUSTOMSESSIONSTORAGE_INITIALIZE(&(s_)->session);\
    })

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_CUSTOMCLIENTSTORAGE_INITIALIZE_W_SESSION(s_,session_init_) \
{\
    NANO_XRCE_CUSTOMCLIENTREQUESTSSTORAGE_INITIALIZE(&(s_)->requests);\
    session_init_ \
    NANO_XRCE_ClientStorage_initialize(&(s_)->base,\
        &(s_)->requests.base, &(s_)->session.base);\
}

/******************************************************************************
 * DefaultClientStorage
 ******************************************************************************/

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_DefaultClientStorage_initialize(s_) \
    NANO_XRCE_CUSTOMCLIENTSTORAGE_INITIALIZE(s_)


NANO_XRCE_CUSTOMCLIENTSTORAGE_DEFINE_TYPES_W_SESSION(
    NANO_XRCE_Default,
    NANO_LIMIT_CLIENT_MAX_PENDING_REQUESTS,
    NANO_XRCE_DefaultSessionStorage);

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_DEFAULTCLIENTSTORAGE_INITIALIZER \
    NANO_XRCE_CUSTOMCLIENTSTORAGE_INITIALIZER

/******************************************************************************
 * SimpleClientStorage
 ******************************************************************************/

#if NANO_FEAT_RELIABILITY
#define NANO_XRCE_SIMPLECLIENTSTORAGE_RELIABILITY_TYPE_MEMBERS(base_type_name_) \
    NANO_XRCE_StreamStorage builtin_rel;
#define NANO_XRCE_SIMPLECLIENTSTORAGE_RELIABILITY_INITIALIZER \
    NANO_XRCE_STREAMSTORAGE_INITIALIZER,
#define NANO_XRCE_SIMPLECLIENTSTORAGE_RELIABILITY_INITIALIZE(s_) \
{\
    (s_)->builtin_rel = (s_)->messages.base;\
}
#define NANO_XRCE_SIMPLECLIENTSTORAGE_RELIABILITY_INITIALIZE_ARG(s_) \
    &(s_)->builtin_rel
#else
#define NANO_XRCE_SIMPLECLIENTSTORAGE_RELIABILITY_TYPE_MEMBERS(base_type_name_) 
#define NANO_XRCE_SIMPLECLIENTSTORAGE_RELIABILITY_INITIALIZER
#define NANO_XRCE_SIMPLECLIENTSTORAGE_RELIABILITY_INITIALIZE(s_)
#define NANO_XRCE_SIMPLECLIENTSTORAGE_RELIABILITY_INITIALIZE_ARG(s_) \
    NULL
#endif /* NANO_FEAT_RELIABILITY */

#if NANO_FEAT_SESSION_USER_STREAMS
#define NANO_XRCE_SIMPLECLIENTSTORAGE_USER_STREAMS_DEFINE_TYPE(base_type_name_,max_streams_be_, max_streams_rel_)\
typedef struct xrce_str_concat(base_type_name_,UserStreamsStorageI)\
{\
    NANO_XRCE_CUSTOMUSERSTREAMSSTORAGE_TYPE_MEMBERS(max_streams_be_, max_streams_rel_);\
} xrce_str_concat(base_type_name_,UserStreamsStorage);
#define NANO_XRCE_SIMPLECLIENTSTORAGE_USER_STREAMS_TYPE_MEMBERS(base_type_name_) \
    xrce_str_concat(base_type_name_,UserStreamsStorage) streams;
#define NANO_XRCE_SIMPLECLIENTSTORAGE_USER_STREAMS_INITIALIZER \
    NANO_XRCE_CUSTOMUSERSTREAMSSTORAGE_INITIALIZER,
#define NANO_XRCE_SIMPLECLIENTSTORAGE_USER_STREAMS_INITIALIZE(s_) \
{\
    NANO_XRCE_CUSTOMUSERSTREAMSSTORAGE_INITIALIZE(&(s_)->streams);\
}
#define NANO_XRCE_SIMPLECLIENTSTORAGE_USER_STREAMS_INITIALIZE_ARG(s_) \
    &(s_)->streams.base
#else
#define NANO_XRCE_SIMPLECLIENTSTORAGE_USER_STREAMS_DEFINE_TYPE(base_type_name_,max_streams_be_, max_streams_rel_)
#define NANO_XRCE_SIMPLECLIENTSTORAGE_USER_STREAMS_TYPE_MEMBERS(base_type_name_) 
#define NANO_XRCE_SIMPLECLIENTSTORAGE_USER_STREAMS_INITIALIZER
#define NANO_XRCE_SIMPLECLIENTSTORAGE_USER_STREAMS_INITIALIZE(s_)
#define NANO_XRCE_SIMPLECLIENTSTORAGE_USER_STREAMS_INITIALIZE_ARG(s_) \
    NULL

#endif /* NANO_FEAT_SESSION_USER_STREAMS */

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_SIMPLECLIENTSTORAGE_TYPE_MEMBERS(base_type_name_) \
    NANO_XRCE_ClientStorage base;\
    NANO_XRCE_SessionStorage session;\
    NANO_XRCE_StreamStorage builtin_be;\
    NANO_XRCE_SIMPLECLIENTSTORAGE_RELIABILITY_TYPE_MEMBERS(base_type_name_) \
    xrce_str_concat(base_type_name_,ClientRequestsStorage) requests;\
    NANO_XRCE_SIMPLECLIENTSTORAGE_USER_STREAMS_TYPE_MEMBERS(base_type_name_) \
    xrce_str_concat(base_type_name_,StreamStorage) messages

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_SIMPLECLIENTSTORAGE_DEFINE_TYPES(base_type_name_,max_requests_,max_messages_,max_streams_be_,max_streams_rel_)\
\
typedef struct xrce_str_concat(base_type_name_,ClientRequestsStorageI)\
{\
    NANO_XRCE_CUSTOMCLIENTREQUESTSSTORAGE_TYPE_MEMBERS(max_requests_);\
} xrce_str_concat(base_type_name_,ClientRequestsStorage);\
\
NANO_XRCE_SIMPLECLIENTSTORAGE_USER_STREAMS_DEFINE_TYPE(base_type_name_,max_streams_be_, max_streams_rel_)\
\
typedef struct xrce_str_concat(base_type_name_,StreamStorageI)\
{\
    NANO_XRCE_CUSTOMSTREAMSTORAGE_TYPE_MEMBERS(\
        max_messages_ /* header */,\
        max_messages_ /* payload */,\
        ((max_messages_ >= 2)?(max_messages_/2):max_messages_) /* payload_tail */,\
        (max_messages_ * 2) /* payload_user */,\
        max_messages_ /* payload_in */);\
} xrce_str_concat(base_type_name_,StreamStorage);\
\
typedef struct xrce_str_concat(base_type_name_,ClientStorageI)\
{\
    NANO_XRCE_SIMPLECLIENTSTORAGE_TYPE_MEMBERS(base_type_name_);\
    NANO_XRCE_CUSTOMSTORAGE_CPP_CONSTRUCTOR(\
        xrce_str_concat(base_type_name_,ClientStorageI),\
        NANO_XRCE_SIMPLECLIENTSTORAGE_INITIALIZE(this);)\
} xrce_str_concat(base_type_name_,ClientStorage)

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_SIMPLECLIENTSTORAGE_INITIALIZER \
{\
    NANO_XRCE_CLIENTSTORAGE_INITIALIZER, /* base */\
    NANO_XRCE_SESSIONSTORAGE_INITIALIZER, /* session */\
    NANO_XRCE_STREAMSTORAGE_INITIALIZER, /* builtin_be */\
    NANO_XRCE_SIMPLECLIENTSTORAGE_RELIABILITY_INITIALIZER \
    NANO_XRCE_CUSTOMCLIENTREQUESTSSTORAGE_INITIALIZER, /* requests */\
    NANO_XRCE_SIMPLECLIENTSTORAGE_USER_STREAMS_INITIALIZER /* streams */\
    NANO_XRCE_CUSTOMSTREAMSTORAGE_INITIALIZER /* messages */\
}

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_SIMPLECLIENTSTORAGE_INITIALIZE(s_) \
{\
    NANO_XRCE_CUSTOMCLIENTREQUESTSSTORAGE_INITIALIZE(&(s_)->requests);\
    NANO_XRCE_CUSTOMSTREAMSTORAGE_INITIALIZE(&(s_)->messages);\
    (s_)->builtin_be = (s_)->messages.base;\
    NANO_XRCE_SIMPLECLIENTSTORAGE_RELIABILITY_INITIALIZE(s_)\
    NANO_XRCE_SIMPLECLIENTSTORAGE_USER_STREAMS_INITIALIZE(s_)\
    NANO_XRCE_SessionStorage_initialize(\
        &(s_)->session,\
        &(s_)->builtin_be,\
        NANO_XRCE_SIMPLECLIENTSTORAGE_RELIABILITY_INITIALIZE_ARG(s_),\
        NANO_XRCE_SIMPLECLIENTSTORAGE_USER_STREAMS_INITIALIZE_ARG(s_));\
    NANO_XRCE_ClientStorage_initialize(\
    &(s_)->base,\
        &(s_)->requests.base,\
        &(s_)->session);\
}

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_DefaultSimpleClientStorage_initialize(s_) \
    NANO_XRCE_SIMPLECLIENTSTORAGE_INITIALIZE(s_)

NANO_XRCE_SIMPLECLIENTSTORAGE_DEFINE_TYPES(
    NANO_XRCE_DefaultSimple,
    NANO_LIMIT_CLIENT_MAX_PENDING_REQUESTS,
    NANO_LIMIT_RELIABLESTREAM_SENDQUEUE_MAX_LENGTH * 2,
    NANO_LIMIT_SESSION_MAX_USER_STREAMS_BESTEFFORT,
    NANO_LIMIT_SESSION_MAX_USER_STREAMS_RELIABLE);

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_DEFAULTSIMPLECLIENTSTORAGE_INITIALIZER \
    NANO_XRCE_SIMPLECLIENTSTORAGE_INITIALIZER

/** @} *//* nano_api_storage_client */

/******************************************************************************
 *                             ClientProperties
 ******************************************************************************/

/**
 * @addtogroup nano_api_clientproperties
 * @{
 */

/*e
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_ClientPropertiesI
{
#ifdef NANO_HAVE_CLIENT_LISTENER
    /*e
     * @brief TODO
     * 
     */
    NANO_XRCE_ClientListener listener;
#endif
    /*e
     * @brief TODO
     * 
     */
    NANO_XRCE_ClientTransport *transport;
    /*e
     * @brief TODO
     * 
     */
    NANO_XRCE_ClientTransportProperties *transport_properties;
    /*e
     * @brief TODO
     * 
     */
    NANO_XRCE_ClientStorage *storage;
    /*e
     * @brief TODO
     * 
     */
    NANO_XRCE_ClientKey key;
    /*e
     * @brief TODO
     * 
     */
    NANO_bool include_key_in_messages;
#if NANO_FEAT_AGENT_DISCOVERY
    /*e
     * @brief TODO
     * 
     */
    NANO_bool enable_discovery;
#endif /* NANO_FEAT_AGENT_DISCOVERY */
#if NANO_FEAT_RELIABILITY && NANO_FEAT_SYSTIME
    /*e
     * @brief TODO
     * 
     */
    NANO_Time heartbeat_period;
    /*e
     * @brief TODO
     * 
     */
    NANO_Time acknack_period;
#endif /* NANO_FEAT_RELIABILITY && NANO_FEAT_SYSTIME */

#if NANO_CPP
    /*e
     * @brief TODO
     * 
     */
    NANO_XRCE_ClientPropertiesI():
        listener(),
        transport(),
        transport_properties(),
        storage(),
        key(),
        include_key_in_messages(
            NANO_LIMIT_CLIENT_INCLUDE_KEY_IN_MESSAGE_DEFAULT)
#if NANO_FEAT_AGENT_DISCOVERY
        ,
        enable_discovery()
#endif /* NANO_FEAT_AGENT_DISCOVERY */
#if NANO_FEAT_RELIABILITY && NANO_FEAT_SYSTIME
        ,
        heartbeat_period(
            NANO_LIMIT_SESSION_HEARTBEAT_PERIOD_DEFAULT_SEC,
            NANO_LIMIT_SESSION_HEARTBEAT_PERIOD_DEFAULT_NSEC),
        acknack_period(
            NANO_LIMIT_SESSION_ACKNACK_PERIOD_DEFAULT_SEC,
            NANO_LIMIT_SESSION_ACKNACK_PERIOD_DEFAULT_NSEC)
#endif /* NANO_FEAT_RELIABILITY && NANO_FEAT_SYSTIME */
    {

    }
#endif /*NANO_CPP */
} NANO_XRCE_ClientProperties;

#ifdef NANO_HAVE_CLIENT_LISTENER
#define NANO_XRCE_CLIENTPROPERTIES_LISTENER_INITIALIZER \
    NANO_XRCE_CLIENTLISTENER_INITIALIZER,
#else
#define NANO_XRCE_CLIENTPROPERTIES_LISTENER_INITIALIZER
#endif /* NANO_FEAT_SUBSCRIBE */

#if NANO_FEAT_AGENT_DISCOVERY
#define NANO_XRCE_CLIENTPROPERTIES_DISCOVERY_INITIALIZER \
    ,\
    NANO_BOOL_FALSE /* enable_discovery */
#else
#define NANO_XRCE_CLIENTPROPERTIES_DISCOVERY_INITIALIZER
#endif /* NANO_FEAT_AGENT_DISCOVERY */

#if NANO_FEAT_RELIABILITY && NANO_FEAT_SYSTIME
#define NANO_XRCE_CLIENTPROPERTIES_PERIOD_INITIALIZER \
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
#define NANO_XRCE_CLIENTPROPERTIES_PERIOD_INITIALIZER
#endif /* NANO_FEAT_RELIABILITY && NANO_FEAT_SYSTIME */

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_CLIENTPROPERTIES_INITIALIZER \
{\
    NANO_XRCE_CLIENTPROPERTIES_LISTENER_INITIALIZER /* listener */\
    NULL, /* transport */\
    NULL, /* transport_properties */\
    NULL, /* storage */\
    NANO_XRCE_CLIENTKEY_INVALID, /* key */\
    NANO_LIMIT_CLIENT_INCLUDE_KEY_IN_MESSAGE_DEFAULT /* include_key_in_messages */\
    NANO_XRCE_CLIENTPROPERTIES_DISCOVERY_INITIALIZER /* discovery */ \
    NANO_XRCE_CLIENTPROPERTIES_PERIOD_INITIALIZER \
}

/** @} *//* nano_api_clientproperties */

/******************************************************************************
 *                                ClientFlags
 ******************************************************************************/

/*i
 * @brief TODO
 * 
 */
typedef NANO_u8 NANO_XRCE_ClientFlags;

/*i
 * @brief TODO
 * 
 */
#define NANO_XRCE_CLIENTFLAGS_DEFAULT           (0)
/*i
 * @brief TODO
 * 
 */
#define NANO_XRCE_CLIENTFLAGS_CONNECTED         (0x01 << 0)
/*i
 * @brief TODO
 * 
 */
#define NANO_XRCE_CLIENTFLAGS_CONNECTING        (0x01 << 1)
/*i
 * @brief TODO
 * 
 */
#define NANO_XRCE_CLIENTFLAGS_DISCOVERING       (0x01 << 2)

/******************************************************************************
 *                                  Client
 ******************************************************************************/

/**
 * @addtogroup nano_api_client
 * @{
 */

/*e
 * 
 * 
 */
typedef struct NANODllExport NANO_XRCE_ClientI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_ClientStorage *storage;
#ifdef NANO_HAVE_CLIENT_LISTENER
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_ClientListener listener;
#endif /* NANO_HAVE_CLIENT_LISTENER */
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_Session session;
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_ClientFlags flags;
    /*i
     * @brief TODO
     * 
     */
    NANO_u16 next_request;
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_ClientRequest *pending_requests;

#if NANO_CPP
    /*e
     * @brief TODO
     * 
     */
    NANO_XRCE_ClientI()
    :
        storage(NULL),
#ifdef NANO_HAVE_CLIENT_LISTENER
        listener(),
#endif /* NANO_HAVE_CLIENT_LISTENER */
        session(),
        flags(NANO_XRCE_CLIENTFLAGS_DEFAULT),
        next_request(0),
        pending_requests(NULL)
    { }

#endif /* NANO_CPP */

} NANO_XRCE_Client;


#ifdef NANO_HAVE_CLIENT_LISTENER
#define NANO_XRCE_CLIENT_LISTENER_INITIALIZER \
    NANO_XRCE_CLIENTLISTENER_INITIALIZER,
#else
#define NANO_XRCE_CLIENT_LISTENER_INITIALIZER
#endif /* NANO_FEAT_SUBSCRIBE */

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_CLIENT_INITIALIZER \
{\
    NULL, /* storage */\
    NANO_XRCE_CLIENT_LISTENER_INITIALIZER /* listener */\
    NANO_XRCE_SESSION_INITIALIZER, /* session */\
    NANO_XRCE_CLIENTFLAGS_DEFAULT, /* flags */\
    0, /* next_request */\
    NULL /* pending_requests */ /* request */\
}

/*e
 * @brief TODO
 * 
 * @param self 
 * @param properties 
 * @return NANO_RetCode
 */
NANODllExport
NANO_RetCode
NANO_XRCE_Client_initialize(
    NANO_XRCE_Client *const self,
    const NANO_XRCE_ClientProperties *const properties);

/*e
 * @brief TODO
 * 
 * @param self 
 */
void
NANO_XRCE_Client_finalize(NANO_XRCE_Client *const self);

#define NANO_XRCE_Client_finalize(s_) \
{\
    NANO_XRCE_Session_finalize(&(s_)->session);\
}

/*e
 * @brief TODO
 * 
 * @param self 
 */
void
NANO_XRCE_Client_mark_connected(NANO_XRCE_Client *const self);

#define NANO_XRCE_Client_mark_connected(s_) \
{\
    (s_)->flags |= NANO_XRCE_CLIENTFLAGS_CONNECTED;\
    NANO_XRCE_Session_connected(&(s_)->session);\
}

/*e
 * @brief TODO
 * 
 * @param self 
 * @return
 */
NANODllExport
void
NANO_XRCE_Client_mark_disconnected(NANO_XRCE_Client *const self);

/*e
 * @brief TODO
 * 
 * @param self 
 * @return NANO_bool 
 */
NANO_bool
NANO_XRCE_Client_connected(const NANO_XRCE_Client *const self);

#define NANO_XRCE_Client_connected(s_) \
    ((s_)->flags & NANO_XRCE_CLIENTFLAGS_CONNECTED)

/*e
 * @brief TODO
 * 
 * @param self 
 * @return NANO_bool 
 */
NANO_bool
NANO_XRCE_Client_discovering(const NANO_XRCE_Client *const self);

#define NANO_XRCE_Client_discovering(s_) \
    ((s_)->flags & NANO_XRCE_CLIENTFLAGS_DISCOVERING)

/*e
 * @brief TODO
 * 
 * @param self 
 */
NANODllExport
void
NANO_XRCE_Client_drop_requests(NANO_XRCE_Client *const self);

/** @} *//* nano_api_client */

/**
 * @addtogroup nano_api_clientrequests
 * @{
 */
/*e
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_RequestArgsI
{
    /*e
     * @brief TODO
     * 
     */
    NANO_Timeout timeout_ms;
    /*e
     * @brief TODO
     * 
     */
    NANO_XRCE_ObjectId obj_id;
    /*e
     * @brief TODO
     * 
     */
    NANO_XRCE_StreamId stream_id;
    /*e
     * @brief TODO
     * 
     */
    NANO_XRCE_RequestFlags flags;

#if NANO_CPP
    /*e
     * @brief TODO
     * 
     */
    NANO_XRCE_RequestArgsI()
        : timeout_ms(NANO_TIMEOUT_INFINITE),
          obj_id(),
          stream_id(NANO_XRCE_STREAMID_NONE),
          flags(NANO_XRCE_REQUESTFLAGS_DEFAULT)
    {

    }
#endif /* NANO_CPP */
} NANO_XRCE_RequestArgs;

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_REQUESTARGS_INITIALIZER \
{\
    NANO_TIMEOUT_INFINITE, /* timeout_ms */\
    NANO_XRCE_OBJECTID_INVALID, /* obj_id */\
    NANO_XRCE_STREAMID_NONE, /* stream_id */\
    NANO_XRCE_REQUESTFLAGS_DEFAULT /* flags */\
}

#if NANO_FEAT_OBJECT_CREATE


/*e
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_CreateArgsI
{
    /*e
     * @brief TODO
     * 
     */
    NANO_XRCE_RequestArgs req;
    /*e
     * @brief TODO
     * 
     */
    NANO_XRCE_ObjectKind obj_kind;
    /*e
     * @brief TODO
     * 
     */
    NANO_XRCE_RepresentationFormat repr_fmt;
    /*e
     * @brief TODO
     * 
     */
    const NANO_u8 *repr;
    /*e
     * @brief TODO
     * 
     */
    NANO_usize repr_len;
    /*e
     * @brief TODO
     * 
     */
    const NANO_u8 *tail_data;
    /*e
     * @brief TODO
     * 
     */
    NANO_usize tail_data_len;
    /*e
     * @brief TODO
     * 
     */
    NANO_usize tail_data_align;

#if NANO_CPP
    /*e
     * @brief TODO
     * 
     */
    NANO_XRCE_CreateArgsI()
    {

    }
#endif /* NANO_CPP */
} NANO_XRCE_CreateArgs;

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_CREATEARGS_INITIALIZER \
{\
    NANO_XRCE_REQUESTARGS_INITIALIZER, /* req */\
    NANO_XRCE_OBJK_INVALID, /* obj_kind */\
    NANO_XRCE_REPRESENTATION_INVALID, /* repr_fmt */\
    NULL, /* repr */\
    0, /* repr_len */\
    NULL, /* tail_data */\
    0, /* tail_data_len */\
    0  /* tail_data_align */\
}

#endif /* NANO_FEAT_OBJECT_CREATE */


#if NANO_FEAT_PUBLISH

/*e
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_WriteArgsI
{
    /*e
     * @brief TODO
     * 
     */
    NANO_XRCE_RequestArgs req;
    /*e
     * @brief TODO
     * 
     */
    NANO_XRCE_DataFormat format;
    /*e
     * @brief TODO
     * 
     */
    NANO_MessageBuffer *payload;

#if NANO_CPP
    /*e
     * @brief TODO
     * 
     */
    NANO_XRCE_WriteArgsI()
        : req(),
          format(NANO_XRCE_FORMAT_INVALID),
          payload(NULL)
    {

    }
#endif /* NANO_CPP */
} NANO_XRCE_WriteArgs;

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_WRITEARGS_INITIALIZER \
{\
    NANO_XRCE_REQUESTARGS_INITIALIZER, /* req */\
    NANO_XRCE_FORMAT_INVALID, /* format */\
    NULL /* payload */\
}

#endif /* NANO_FEAT_PUBLISH */

#if NANO_FEAT_SUBSCRIBE

/*e
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_ReadArgsI
{
    /*e
     * @brief TODO
     * 
     */
    NANO_XRCE_RequestArgs req;
    /*e
     * @brief TODO
     * 
     */
    NANO_XRCE_ReadSpecification read_spec;
#if NANO_CPP
    /*e
     * @brief TODO
     * 
     */
    NANO_XRCE_ReadArgsI()
        : req(),
          read_spec()
    {

    }
#endif /* NANO_CPP */
} NANO_XRCE_ReadArgs;

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_READARGS_INITIALIZER \
{\
    NANO_XRCE_REQUESTARGS_INITIALIZER, /* req */\
    NANO_XRCE_READSPECIFICATION_INITIALIZER /* read_spec */\
}

#endif /* NANO_FEAT_SUBSCRIBE */

/** @} *//* nano_api_clientrequests */

/**
 * @addtogroup nano_api_client
 * @{
 */
/*e
 * @brief TODO
 * 
 * @param self 
 * @param timeout_ms 
 * @return NANO_RetCode
 */
NANODllExport
NANO_RetCode
NANO_XRCE_Client_connect(
    NANO_XRCE_Client *const self,
    const NANO_Timeout timeout_ms);

/*e
 * @brief TODO
 * 
 * @param self 
 * @param request_token 
 * @param request_stream 
 * @param request_flags 
 * @param request_timeout_ms 
 * @return NANO_RetCode
 */
NANODllExport
NANO_RetCode
NANO_XRCE_Client_disconnect(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientRequestToken *const request_token,
    const NANO_XRCE_StreamId request_stream,
    const NANO_XRCE_RequestFlags request_flags,
    const NANO_Timeout request_timeout_ms);

/*e
 * @brief TODO
 * 
 * @param self 
 * @param request_token 
 * @param args 
 * @return NANO_RetCode
 */
NANODllExport
NANO_RetCode
NANO_XRCE_Client_disconnect_w_args(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientRequestToken *const request_token,
    const NANO_XRCE_RequestArgs *const args);

/*e
 * @brief TODO
 * 
 * @param self 
 * @param request_token 
 */
NANODllExport
void
NANO_XRCE_Client_dismiss_request(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientRequestToken *const request_token);

#if NANO_FEAT_OBJECT_CREATE
/*e
 * @brief TODO
 * 
 * @param self 
 * @param request_token 
 * @param args 
 * @return NANO_RetCode
 */
NANODllExport
NANO_RetCode
NANO_XRCE_Client_create_object_w_args(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientRequestToken *const request_token,
    const NANO_XRCE_CreateArgs *const args);
#endif /* NANO_FEAT_OBJECT_CREATE */

#if NANO_FEAT_OBJECT_DELETE
/*e
 * @brief TODO
 * 
 * @param self 
 * @param request_token 
 * @param args 
 * @return NANO_RetCode 
 */
NANODllExport
NANO_RetCode
NANO_XRCE_Client_delete_object_w_args(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientRequestToken *const request_token,
    const NANO_XRCE_RequestArgs *const args);

#endif /* NANO_FEAT_OBJECT_DELETE */

/*e
 * @brief TODO
 * 
 * @param self 
 * @param request_token 
 * @param wait_timeout_ms 
 * @return NANO_RetCode
 */
NANODllExport
NANO_RetCode
NANO_XRCE_Client_wait_for_data_received(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientRequestToken *const request_token,
    const NANO_Timeout wait_timeout_ms);

/*e
 * @brief TODO
 * 
 * @param self 
 * @param request_token 
 * @param wait_timeout_ms 
 * @param request_result_out 
 * @return NANO_RetCode
 */
NANODllExport
NANO_RetCode
NANO_XRCE_Client_wait_for_request_complete(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientRequestToken *const request_token,
    const NANO_Timeout wait_timeout_ms,
    NANO_XRCE_ResultStatus *const request_result_out);

#if NANO_FEAT_RELIABILITY
/*e
 * @brief TODO
 * 
 * @param self 
 * @param request_token 
 * @param wait_timeout_ms 
 * @return NANO_RetCode
 */
NANODllExport
NANO_RetCode
NANO_XRCE_Client_wait_for_request_acked(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientRequestToken *const request_token,
    const NANO_Timeout wait_timeout_ms);

#endif /* NANO_FEAT_RELIABILITY */

#if NANO_FEAT_PUBLISH

#if NANO_FEAT_EXTENDED_API
/*e
 * @brief TODO
 * 
 * @param self 
 * @param stream_id 
 * @param payload 
 * @param payload_len 
 * @return NANO_MessageBuffer
 */
NANODllExport
NANO_MessageBuffer*
NANO_XRCE_Client_allocate_message_buffer(
    NANO_XRCE_Client *const self,
    const NANO_XRCE_StreamId stream_id,
    const NANO_u8 *const payload,
    const NANO_u16 payload_len);

/*e
 * @brief TODO
 * 
 * @param self 
 * @param stream_id 
 * @param mbuf 
 * @return NANO_RetCode
 */
NANODllExport
NANO_RetCode
NANO_XRCE_Client_release_message_buffer(
    NANO_XRCE_Client *const self,
    const NANO_XRCE_StreamId stream_id,
    NANO_MessageBuffer *const mbuf);

#if NANO_FEAT_DATA_FORMAT_DATA
/*e
 * @brief TODO
 * 
 * @param self 
 * @param request_token 
 * @param request_stream 
 * @param request_flags 
 * @param request_timeout_ms 
 * @param writer_id 
 * @param payload 
 * @param payload_len 
 * @return NANO_RetCode
 */
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
    const NANO_u16 payload_len);
#endif /* NANO_FEAT_DATA_FORMAT_DATA */
#endif /* NANO_FEAT_EXTENDED_API */

/*e
 * @brief TODO
 * 
 * @param self 
 * @param request_token 
 * @param args 
 * @return NANO_RetCode
 */
NANODllExport
NANO_RetCode
NANO_XRCE_Client_write_w_args(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientRequestToken *const request_token,
    const NANO_XRCE_WriteArgs *const args);

#endif /* NANO_FEAT_PUBLISH */

#if NANO_FEAT_SUBSCRIBE

#if NANO_FEAT_EXTENDED_API

/*e
 * @brief TODO
 * 
 * @param self 
 * @param request_token 
 * @param request_stream 
 * @param request_flags 
 * @param request_timeout_ms 
 * @param reader_id 
 * @param read_stream 
 * @param data_delivery_ctrl 
 * @param content_filter 
 * @return NANO_RetCode
 */
NANODllExport
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
    const char *const content_filter);

#endif /* NANO_FEAT_EXTENDED_API */

/*e
 * @brief TODO
 * 
 * @param self 
 * @param request_token 
 * @param args 
 * @return NANO_RetCode
 */
NANODllExport
NANO_RetCode
NANO_XRCE_Client_read_w_args(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientRequestToken *const request_token,
    const NANO_XRCE_ReadArgs *const args);

#endif /* NANO_FEAT_SUBSCRIBE */

#if NANO_FEAT_AGENT_DISCOVERY
/*e
 * @brief TODO
 * 
 * @param self 
 * @param locator_type 
 * @param locator 
 * @return NANO_RetCode
 */
NANODllExport
NANO_RetCode
NANO_XRCE_Client_update_locator(
    NANO_XRCE_Client *const self,
    const NANO_XRCE_ClientTransportLocatorType locator_type,
    const NANO_XRCE_TransportLocator *const locator);

/*e
 * @brief TODO
 * 
 * @param self 
 * @param request_token 
 * @param args 
 * @return NANO_RetCode
 */
NANODllExport
NANO_RetCode
NANO_XRCE_Client_announce(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientRequestToken *const request_token,
    const NANO_XRCE_RequestArgs *const args);
#endif /* NANO_FEAT_AGENT_DISCOVERY */

#if NANO_FEAT_EXTENDED_API

#if NANO_FEAT_OBJECT_CREATE
/*e
 * @brief TODO
 * 
 * @param self 
 * @param request_token 
 * @param request_stream 
 * @param request_flags 
 * @param request_timeout_ms 
 * @param object_kind 
 * @param object_id 
 * @param parent_id 
 * @param object_xml_repr 
 * @return NANO_RetCode
 */
NANODllExport
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
    const char *const object_xml_repr);

/*e
 * @brief TODO
 * 
 * @param self 
 * @param request_token 
 * @param request_stream 
 * @param request_flags 
 * @param request_timeout_ms 
 * @param object_kind 
 * @param object_id 
 * @param parent_id 
 * @param object_ref 
 * @return NANO_RetCode
 */
NANODllExport
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
    const char *const object_ref);

#if NANO_FEAT_OBJECT_KIND_PARTICIPANT
/*e
 * @brief TODO
 * 
 * @param self 
 * @param request_token 
 * @param request_stream 
 * @param request_flags 
 * @param request_timeout_ms 
 * @param object_id 
 * @param object_xml_repr 
 * @return NANO_RetCode
 */
NANO_RetCode
NANO_XRCE_Client_create_participant_xml(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientRequestToken *const request_token,
    const NANO_XRCE_StreamId request_stream,
    const NANO_XRCE_RequestFlags request_flags,
    const NANO_Timeout request_timeout_ms,
    const NANO_u16 object_id,
    const char *const object_xml_repr);

#define NANO_XRCE_Client_create_participant_xml(s_, r_tkn_, r_st_, r_f_, r_t_, o_id_, o_xml_) \
    NANO_XRCE_Client_create_object_xml(\
        (s_),\
        (r_tkn_),\
        (r_st_),\
        (r_f_),\
        (r_t_),\
        NANO_XRCE_OBJK_PARTICIPANT,\
        (o_id_),\
        0 ,\
        (o_xml_))

/*e
 * @brief TODO
 * 
 * @param self 
 * @param request_token 
 * @param request_stream 
 * @param request_flags 
 * @param request_timeout_ms 
 * @param object_id 
 * @param object_ref 
 * @return NANO_RetCode 
 */
NANO_RetCode
NANO_XRCE_Client_register_participant(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientRequestToken *const request_token,
    const NANO_XRCE_StreamId request_stream,
    const NANO_XRCE_RequestFlags request_flags,
    const NANO_Timeout request_timeout_ms,
    const NANO_u16 object_id,
    const char *const object_ref);

#define NANO_XRCE_Client_register_participant(s_, r_tkn_, r_st_, r_f_, r_t_, o_id_, o_ref_) \
    NANO_XRCE_Client_register_object(\
        (s_),\
        (r_tkn_),\
        (r_st_),\
        (r_f_),\
        (r_t_),\
        NANO_XRCE_OBJK_PARTICIPANT,\
        (o_id_),\
        0,\
        (o_ref_))

#endif /* NANO_FEAT_OBJECT_KIND_PARTICIPANT */

#if NANO_FEAT_OBJECT_KIND_TOPIC
/*e
 * @brief TODO
 * 
 * @param self 
 * @param request_token 
 * @param request_stream 
 * @param request_flags 
 * @param request_timeout_ms 
 * @param object_id 
 * @param object_xml_repr 
 * @param participant_id 
 * @return NANO_RetCode 
 */
NANO_RetCode
NANO_XRCE_Client_create_topic_xml(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientRequestToken *const request_token,
    const NANO_XRCE_StreamId request_stream,
    const NANO_XRCE_RequestFlags request_flags,
    const NANO_Timeout request_timeout_ms,
    const NANO_u16 object_id,
    const char *const object_xml_repr,
    const NANO_u16 participant_id);

#define NANO_XRCE_Client_create_topic_xml(s_, r_tkn_, r_st_, r_f_, r_t_, o_id_, o_xml_, p_id_) \
    NANO_XRCE_Client_create_object_xml(\
        (s_),\
        (r_tkn_),\
        (r_st_),\
        (r_f_),\
        (r_t_),\
        NANO_XRCE_OBJK_TOPIC,\
        (o_id_),\
        (p_id_) ,\
        (o_xml_))

/*e
 * @brief TODO
 * 
 * @param self 
 * @param request_token 
 * @param request_stream 
 * @param request_flags 
 * @param request_timeout_ms 
 * @param object_id 
 * @param object_ref 
 * @return NANO_RetCode 
 */
NANO_RetCode
NANO_XRCE_Client_register_topic(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientRequestToken *const request_token,
    const NANO_XRCE_StreamId request_stream,
    const NANO_XRCE_RequestFlags request_flags,
    const NANO_Timeout request_timeout_ms,
    const NANO_u16 object_id,
    const char *const object_ref);

#define NANO_XRCE_Client_register_topic(s_, r_tkn_, r_st_, r_f_, r_t_, o_id_, o_ref_) \
    NANO_XRCE_Client_register_object(\
        (s_),\
        (r_tkn_),\
        (r_st_),\
        (r_f_),\
        (r_t_),\
        NANO_XRCE_OBJK_TOPIC,\
        (o_id_),\
        0 ,\
        (o_ref_))

#endif /* NANO_FEAT_OBJECT_KIND_TOPIC */

#if NANO_FEAT_OBJECT_KIND_PUBLISHER
/*e
 * @brief TODO
 * 
 * @param self 
 * @param request_token 
 * @param request_stream 
 * @param request_flags 
 * @param request_timeout_ms 
 * @param object_id 
 * @param object_xml_repr 
 * @param participant_id 
 * @return NANO_RetCode 
 */
NANO_RetCode
NANO_XRCE_Client_create_publisher_xml(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientRequestToken *const request_token,
    const NANO_XRCE_StreamId request_stream,
    const NANO_XRCE_RequestFlags request_flags,
    const NANO_Timeout request_timeout_ms,
    const NANO_u16 object_id,
    const char *const object_xml_repr,
    const NANO_u16 participant_id);

#define NANO_XRCE_Client_create_publisher_xml(s_, r_tkn_, r_st_, r_f_, r_t_, o_id_, o_xml_, p_id_) \
    NANO_XRCE_Client_create_object_xml(\
        (s_),\
        (r_tkn_),\
        (r_st_),\
        (r_f_),\
        (r_t_),\
        NANO_XRCE_OBJK_PUBLISHER,\
        (o_id_),\
        (p_id_) ,\
        (o_xml_))

/*e
 * @brief TODO
 * 
 * @param self 
 * @param request_token 
 * @param request_stream 
 * @param request_flags 
 * @param request_timeout_ms 
 * @param object_id 
 * @param object_ref 
 * @return NANO_RetCode 
 */
NANO_RetCode
NANO_XRCE_Client_register_publisher(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientRequestToken *const request_token,
    const NANO_XRCE_StreamId request_stream,
    const NANO_XRCE_RequestFlags request_flags,
    const NANO_Timeout request_timeout_ms,
    const NANO_u16 object_id,
    const char *const object_ref);

#define NANO_XRCE_Client_register_publisher(s_, r_tkn_, r_st_, r_f_, r_t_, o_id_, o_ref_) \
    NANO_XRCE_Client_register_object(\
        (s_),\
        (r_tkn_),\
        (r_st_),\
        (r_f_),\
        (r_t_),\
        NANO_XRCE_OBJK_PUBLISHER,\
        (o_id_),\
        0 ,\
        (o_ref_))

#endif /* NANO_FEAT_OBJECT_KIND_PUBLISHER */

#if NANO_FEAT_OBJECT_KIND_SUBSCRIBER
/*e
 * @brief TODO
 * 
 * @param self 
 * @param request_token 
 * @param request_stream 
 * @param request_flags 
 * @param request_timeout_ms 
 * @param object_id 
 * @param object_xml_repr 
 * @param participant_id 
 * @return NANO_RetCode 
 */
NANO_RetCode
NANO_XRCE_Client_create_subscriber_xml(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientRequestToken *const request_token,
    const NANO_XRCE_StreamId request_stream,
    const NANO_XRCE_RequestFlags request_flags,
    const NANO_Timeout request_timeout_ms,
    const NANO_u16 object_id,
    const char *const object_xml_repr,
    const NANO_u16 participant_id);

#define NANO_XRCE_Client_create_subscriber_xml(s_, r_tkn_, r_st_, r_f_, r_t_, o_id_, o_xml_, p_id_) \
    NANO_XRCE_Client_create_object_xml(\
        (s_),\
        (r_tkn_),\
        (r_st_),\
        (r_f_),\
        (r_t_),\
        NANO_XRCE_OBJK_SUBSCRIBER,\
        (o_id_),\
        (p_id_) ,\
        (o_xml_))

/*e
 * @brief TODO
 * 
 * @param self 
 * @param request_token 
 * @param request_stream 
 * @param request_flags 
 * @param request_timeout_ms 
 * @param object_id 
 * @param object_ref 
 * @return NANO_RetCode 
 */
NANO_RetCode
NANO_XRCE_Client_register_subscriber(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientRequestToken *const request_token,
    const NANO_XRCE_StreamId request_stream,
    const NANO_XRCE_RequestFlags request_flags,
    const NANO_Timeout request_timeout_ms,
    const NANO_u16 object_id,
    const char *const object_ref);

#define NANO_XRCE_Client_register_subscriber(s_, r_tkn_, r_st_, r_f_, r_t_, o_id_, o_ref_) \
    NANO_XRCE_Client_register_object(\
        (s_),\
        (r_tkn_),\
        (r_st_),\
        (r_f_),\
        (r_t_),\
        NANO_XRCE_OBJK_SUBSCRIBER,\
        (o_id_),\
        0 ,\
        (o_ref_))

#endif /* NANO_FEAT_OBJECT_KIND_SUBSCRIBER */

#if NANO_FEAT_OBJECT_KIND_DATAWRITER
/*e
 * @brief TODO
 * 
 * @param self 
 * @param request_token 
 * @param request_stream 
 * @param request_flags 
 * @param request_timeout_ms 
 * @param object_id 
 * @param object_xml_repr 
 * @param publisher_id 
 * @return NANO_RetCode 
 */
NANO_RetCode
NANO_XRCE_Client_create_datawriter_xml(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientRequestToken *const request_token,
    const NANO_XRCE_StreamId request_stream,
    const NANO_XRCE_RequestFlags request_flags,
    const NANO_Timeout request_timeout_ms,
    const NANO_u16 object_id,
    const char *const object_xml_repr,
    const NANO_u16 publisher_id);

#define NANO_XRCE_Client_create_datawriter_xml(s_, r_tkn_, r_st_, r_f_, r_t_, o_id_, o_xml_, p_id_) \
    NANO_XRCE_Client_create_object_xml(\
        (s_),\
        (r_tkn_),\
        (r_st_),\
        (r_f_),\
        (r_t_),\
        NANO_XRCE_OBJK_DATAWRITER,\
        (o_id_),\
        (p_id_) ,\
        (o_xml_))

/*e
 * @brief TODO
 * 
 * @param self 
 * @param request_token 
 * @param request_stream 
 * @param request_flags 
 * @param request_timeout_ms 
 * @param object_id 
 * @param object_ref 
 * @return NANO_RetCode 
 */
NANO_RetCode
NANO_XRCE_Client_register_datawriter(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientRequestToken *const request_token,
    const NANO_XRCE_StreamId request_stream,
    const NANO_XRCE_RequestFlags request_flags,
    const NANO_Timeout request_timeout_ms,
    const NANO_u16 object_id,
    const char *const object_ref);

#define NANO_XRCE_Client_register_datawriter(s_, r_tkn_, r_st_, r_f_, r_t_, o_id_, o_ref_) \
    NANO_XRCE_Client_register_object(\
        (s_),\
        (r_tkn_),\
        (r_st_),\
        (r_f_),\
        (r_t_),\
        NANO_XRCE_OBJK_DATAWRITER,\
        (o_id_),\
        0 ,\
        (o_ref_))

#endif /* NANO_FEAT_OBJECT_KIND_DATAWRITER */

#if NANO_FEAT_OBJECT_KIND_DATAREADER
/*e
 * @brief TODO
 * 
 * @param self 
 * @param request_token 
 * @param request_stream 
 * @param request_flags 
 * @param request_timeout_ms 
 * @param object_id 
 * @param object_xml_repr 
 * @param subscriber_id 
 * @return NANO_RetCode 
 */
NANO_RetCode
NANO_XRCE_Client_create_datareader_xml(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientRequestToken *const request_token,
    const NANO_XRCE_StreamId request_stream,
    const NANO_XRCE_RequestFlags request_flags,
    const NANO_Timeout request_timeout_ms,
    const NANO_u16 object_id,
    const char *const object_xml_repr,
    const NANO_u16 subscriber_id);

#define NANO_XRCE_Client_create_datareader_xml(s_, r_tkn_, r_st_, r_f_, r_t_, o_id_, o_xml_, s_id_) \
    NANO_XRCE_Client_create_object_xml(\
        (s_),\
        (r_tkn_),\
        (r_st_),\
        (r_f_),\
        (r_t_),\
        NANO_XRCE_OBJK_DATAREADER,\
        (o_id_),\
        (s_id_) ,\
        (o_xml_))

/*e
 * @brief TODO
 * 
 * @param self 
 * @param request_token 
 * @param request_stream 
 * @param request_flags 
 * @param request_timeout_ms 
 * @param object_id 
 * @param object_ref 
 * @return NANO_RetCode 
 */
NANO_RetCode
NANO_XRCE_Client_register_datareader(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientRequestToken *const request_token,
    const NANO_XRCE_StreamId request_stream,
    const NANO_XRCE_RequestFlags request_flags,
    const NANO_Timeout request_timeout_ms,
    const NANO_u16 object_id,
    const char *const object_ref);

#define NANO_XRCE_Client_register_datareader(s_, r_tkn_, r_st_, r_f_, r_t_, o_id_, o_ref_) \
    NANO_XRCE_Client_register_object(\
        (s_),\
        (r_tkn_),\
        (r_st_),\
        (r_f_),\
        (r_t_),\
        NANO_XRCE_OBJK_DATAREADER,\
        (o_id_),\
        0 ,\
        (o_ref_))

#endif /* NANO_FEAT_OBJECT_KIND_DATAREADER */

#if NANO_FEAT_OBJECT_KIND_APPLICATION
/*e
 * @brief TODO
 * 
 * @param self 
 * @param request_token 
 * @param request_stream 
 * @param request_flags 
 * @param request_timeout_ms 
 * @param object_id 
 * @param object_xml_repr 
 * @return NANO_RetCode 
 */
NANO_RetCode
NANO_XRCE_Client_create_application_xml(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientRequestToken *const request_token,
    const NANO_XRCE_StreamId request_stream,
    const NANO_XRCE_RequestFlags request_flags,
    const NANO_Timeout request_timeout_ms,
    const NANO_u16 object_id,
    const char *const object_xml_repr);

#define NANO_XRCE_Client_create_application_xml(s_, r_tkn_, r_st_, r_f_, r_t_, o_id_, o_xml_) \
    NANO_XRCE_Client_create_object_xml(\
        (s_),\
        (r_tkn_),\
        (r_st_),\
        (r_f_),\
        (r_t_),\
        NANO_XRCE_OBJK_APPLICATION,\
        (o_id_),\
        0 ,\
        (o_xml_))

#endif /* NANO_FEAT_OBJECT_KIND_APPLICATION */

#if NANO_FEAT_OBJECT_KIND_DOMAIN
/*e
 * @brief TODO
 * 
 * @param self 
 * @param request_token 
 * @param request_stream 
 * @param request_flags 
 * @param request_timeout_ms 
 * @param object_id 
 * @param object_xml_repr 
 * @return NANO_RetCode 
 */
NANO_RetCode
NANO_XRCE_Client_create_domain_xml(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientRequestToken *const request_token,
    const NANO_XRCE_StreamId request_stream,
    const NANO_XRCE_RequestFlags request_flags,
    const NANO_Timeout request_timeout_ms,
    const NANO_u16 object_id,
    const char *const object_xml_repr);

#define NANO_XRCE_Client_create_domain_xml(s_, r_tkn_, r_st_, r_f_, r_t_, o_id_, o_xml_) \
    NANO_XRCE_Client_create_object_xml(\
        (s_),\
        (r_tkn_),\
        (r_st_),\
        (r_f_),\
        (r_t_),\
        NANO_XRCE_OBJK_TOPIC,\
        (o_id_),\
        0 ,\
        (o_xml_))

#endif /* NANO_FEAT_OBJECT_KIND_DOMAIN */

#if NANO_FEAT_OBJECT_KIND_TYPE
/*e
 * @brief TODO
 * 
 * @param self 
 * @param request_token 
 * @param request_stream 
 * @param request_flags 
 * @param request_timeout_ms 
 * @param object_id 
 * @param object_xml_repr 
 * @return NANO_RetCode 
 */
NANO_RetCode
NANO_XRCE_Client_create_type_xml(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientRequestToken *const request_token,
    const NANO_XRCE_StreamId request_stream,
    const NANO_XRCE_RequestFlags request_flags,
    const NANO_Timeout request_timeout_ms,
    const NANO_u16 object_id,
    const char *const object_xml_repr);

#define NANO_XRCE_Client_create_type_xml(s_, r_tkn_, r_st_, r_f_, r_t_, o_id_, o_xml_) \
    NANO_XRCE_Client_create_object_xml(\
        (s_),\
        (r_tkn_),\
        (r_st_),\
        (r_f_),\
        (r_t_),\
        NANO_XRCE_OBJK_TYPE,\
        (o_id_),\
        0 ,\
        (o_xml_))

#endif /* NANO_FEAT_OBJECT_KIND_TYPE */

#if NANO_FEAT_OBJECT_KIND_QOSPROFILE
/*e
 * @brief TODO
 * 
 * @param self 
 * @param request_token 
 * @param request_stream 
 * @param request_flags 
 * @param request_timeout_ms 
 * @param object_id 
 * @param object_xml_repr 
 * @return NANO_RetCode 
 */
NANO_RetCode
NANO_XRCE_Client_create_qosprofile_xml(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientRequestToken *const request_token,
    const NANO_XRCE_StreamId request_stream,
    const NANO_XRCE_RequestFlags request_flags,
    const NANO_Timeout request_timeout_ms,
    const NANO_u16 object_id,
    const char *const object_xml_repr);

#define NANO_XRCE_Client_create_qosprofile_xml(s_, r_tkn_, r_st_, r_f_, r_t_, o_id_, o_xml_) \
    NANO_XRCE_Client_create_object_xml(\
        (s_),\
        (r_tkn_),\
        (r_st_),\
        (r_f_),\
        (r_t_),\
        NANO_XRCE_OBJK_QOSPROFILE,\
        (o_id_),\
        0 ,\
        (o_xml_))

#endif /* NANO_FEAT_OBJECT_KIND_QOSPROFILE */

#endif /* NANO_FEAT_OBJECT_CREATE */

#if NANO_FEAT_FRAGMENT && NANO_FEAT_RELIABILITY
/*e
 * @brief TODO
 * 
 * @param self 
 * @param stream_id 
 * @param buffer 
 * @param buffer_size 
 * @return NANO_RetCode 
 */
NANO_RetCode
NANO_XRCE_Client_fragment_reconstruction_buffer(
    NANO_XRCE_Client *const self,
    const NANO_XRCE_StreamId stream_id,
    NANO_u8 *const buffer,
    const NANO_usize buffer_size);


#define NANO_XRCE_Client_fragment_reconstruction_buffer(s_, s_id_, b_, b_sz_) \
    NANO_XRCE_Session_register_reliable_stream_fragment_storage(\
        &(s_)->session,\
        (s_id_),\
        NANO_XRCE_MESSAGETYPE_FRAGMENT_RECV_BUFFER,\
        (NANO_u8*)(b_),\
        0,\
        (b_sz_))

#endif /* NANO_FEAT_FRAGMENT && NANO_FEAT_RELIABILITY */

#ifdef NANO_HAVE_CLIENT_LISTENER
#if NANO_FEAT_SUBSCRIBE
/*e
 * @brief TODO
 * 
 * @param self 
 * @param on_data 
 * @param listener_ctx 
 */
void
NANO_XRCE_Client_on_data(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientListener_OnDataFn on_data,
    void *const listener_ctx);

#define NANO_XRCE_Client_on_data(s_, on_data_, ctx_) \
{\
    (s_)->listener.on_data = (on_data_);\
    (s_)->listener.user_data = (ctx_);\
}

#endif /* NANO_FEAT_SUBSCRIBE */
#endif /* NANO_HAVE_CLIENT_LISTENER */

#if NANO_FEAT_SESSION_USER_STREAMS
/*e
 * @brief TODO
 * 
 * @param self 
 * @param stream_id 
 * @param storage 
 * @return NANO_RetCode 
 */
NANO_RetCode
NANO_XRCE_Client_enable_stream(
    NANO_XRCE_Client *const self,
    const NANO_XRCE_StreamId stream_id,
    NANO_XRCE_ClientStorage *const storage);

#define NANO_XRCE_Client_enable_stream(s_, s_id_, st_) \
    NANO_XRCE_Session_register_stream(\
        &(s_)->session,\
        (s_id_),\
        (st_))

#endif /* NANO_FEAT_SESSION_USER_STREAMS */

#if NANO_FEAT_ASSERT_LIVELINESS
/*e
 * @brief TODO
 * 
 * @param self 
 * @param period 
 */
void
NANO_XRCE_Client_liveliness_assertion_period(
    NANO_XRCE_Client *const self,
    const NANO_u64 period);

#define NANO_XRCE_Client_liveliness_assertion_period(s_,p_) \
{\
    (s_)->session.period_liveliness = (p_);\
}
#endif /* NANO_FEAT_ASSERT_LIVELINESS */

#if NANO_FEAT_SERVICE_CLIENT
/*e
 * @brief TODO
 *
 */
typedef struct NANODllExport NANO_XRCE_ServiceArgsI
{
    /*e
     * @brief TODO
     *
     */
    NANO_XRCE_RequestArgs req;
    /*e
     * @brief TODO
     *
     */
    NANO_XRCE_ServiceRequestFlags flags;
    /*e
     * @brief TODO
     *
     */
    NANO_u16 query_len;
    /*e
     * @brief TODO
     *
     */
    NANO_u32 data_len;
    /*e
     * @brief TODO
     *
     */
    NANO_u32 metadata_len;
    /*e
     * @brief TODO
     *
     */
    NANO_MessageBuffer *payload;

#if NANO_CPP
    /*e
     * @brief TODO
     *
     */
    NANO_XRCE_ServiceArgsI()
        : req(),
          flags(NANO_XRCE_SERVICEREQUESTFLAGS_UNKNOWN),
          query_len(0),
          data_len(0),
          metadata_len(0),
          payload(NULL)
    {

    }
#endif /* NANO_CPP */
} NANO_XRCE_ServiceArgs;

/*e
 * @brief TODO
 *
 */
#define NANO_XRCE_SERVICEARGS_INITIALIZER \
{\
    NANO_XRCE_REQUESTARGS_INITIALIZER, /* req */\
    NANO_XRCE_SERVICEREQUESTFLAGS_UNKNOWN, /* flags */\
    0, /* query_len */\
    0, /* data_len */\
    0, /* metadata_len */\
    NULL /* payload */\
}

/*e
 * @brief TODO
 * 
 * @param self 
 * @param on_data 
 * @param listener_ctx 
 */
void
NANO_XRCE_Client_on_service_reply(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientListener_OnServiceReplyFn on_svc_reply,
    void *const listener_ctx);

#define NANO_XRCE_Client_on_service_reply(s_, on_svc_reply_, ctx_) \
{\
    (s_)->listener.on_service_reply = (on_svc_reply_);\
    (s_)->listener.user_data = (ctx_);\
}


NANODllExport
NANO_RetCode
NANO_XRCE_Client_service_request(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientRequestToken *const request_token,
    const NANO_XRCE_StreamId request_stream,
    const NANO_XRCE_RequestFlags request_flags,
    const NANO_Timeout request_timeout_ms,
    const NANO_u16 service_resource_id,
    const NANO_XRCE_ServiceRequestFlags service_flags,
    const NANO_u8 *const query,
    const NANO_u16 query_len,
    const NANO_u8 *const data,
    const NANO_u32 data_len,
    const NANO_u8 *const metadata,
    const NANO_u32 metadata_len);

NANODllExport
NANO_RetCode
NANO_XRCE_Client_service_request_w_args(
    NANO_XRCE_Client *const self,
    NANO_XRCE_ClientRequestToken *const request_token,
    const NANO_XRCE_ServiceArgs * const args);
#endif /* NANO_FEAT_SERVICE_CLIENT */


#endif /* NANO_FEAT_EXTENDED_API */

/*e
 * @brief TODO
 * 
 * @param self 
 * @param max_messages 
 * @param timeout_ms 
 * @return NANO_RetCode 
 */
NANO_RetCode
NANO_XRCE_Client_run_session(
    NANO_XRCE_Client *const self,
    const NANO_usize max_messages,
    const NANO_Timeout timeout_ms);

#define NANO_XRCE_Client_run_session(s_, max_msg_, timeout_)\
    NANO_XRCE_Session_run(&(s_)->session, (max_msg_), (timeout_))

/** @} *//* nano_api_client */

#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif /* nano_client_h */
