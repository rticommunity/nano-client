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

/******************************************************************************
 Start nanoagentd with the following command (change paths and other arguments
 according to your environment):
 
   nanoagentd -http -U -a -c resource/xml/examples/http_agent.xml

 You must also start the civetweb HTTP server using the custom start script.
 Replace the `./install` directory with the path to nano's installation 
 directory.

   PATH="./install/bin:$PATH" examples/http/server/run.sh
 
 ******************************************************************************/


#include "nano/nano_client_udpv4.h"
#include "nano/nano_core_http.h"
#include <stdlib.h>

#define SENSOR_ID               0x00000001

#define AGENT_ADDRESS           { 127, 0, 0, 1 }

#define CLIENT_KEY              0x01020304

#define SERVICE_ID              0x4C1F
#define ROOT_ID                 0x392F
#define API_ID                  0x5D2F

#define LOG_LEVEL               LEVEL_INFO

#define REQUEST_TIMEOUT         10000 /* 10s */
#define MAX_SAMPLES             10

#define TRANSPORT_MTU           128
#define RECV_BUFFER_SIZE \
    NANO_OSAPI_Memory_align_size_up(\
        TRANSPORT_MTU, sizeof(NANO_MessageBufferData))

#define FRAG_BUFFER_SIZE        (1 << 20)

#define POST_QUERY    "?key1=Foo+Bar&key2=0"

#define POST_DATA \
    "--123\r\n" \
    "Content-Disposition: form-data;name=\"foo\"\r\n" \
    "\r\n" \
    "Foo\r\n" \
    "--123\r\n" \
    "Content-Disposition: form-data; name=\"bar\"\r\n" \
    "\r\n" \
    "Bar\r\n" \
    "--123--"
#define POST_METADATA \
    "Content-Type: multipart/form-data; boundary=--123"

#define JSON_POST_DATA \
    "{\"foo\": \"Foo\", \"bar\": \"Bar\"}"
#define JSON_POST_METADATA \
    "Content-Type: application/json; charset=utf-8"


struct ReceivedReply
{
    NANO_XRCE_RequestId svc_request;
    NANO_XRCE_ObjectId svc_resource;
    NANO_XRCE_ServiceReplyStatus svc_status;
    NANO_u8 *data;
    NANO_usize data_len;
    NANO_u8 *metadata;
    NANO_usize metadata_len;
};

struct ReceivedReply last_reply;

void
print_reply(
    const NANO_XRCE_RequestId *const svc_request,
    const NANO_XRCE_ObjectId *const svc_resource,
    const NANO_XRCE_ServiceReplyStatus svc_status,
    const NANO_u8 *const data,
    const NANO_usize data_len,
    const NANO_u8 *const metadata,
    const NANO_usize metadata_len)
{
    size_t i = 0;
    printf("## ## ## ## ## ## ## ## ## ## ## ##\n");
    printf("HTTP reply:\n");
    printf("   request: 0x%04X\n", NANO_XRCE_RequestId_to_u16(svc_request));
    printf("  resource: 0x%04X\n", NANO_XRCE_ObjectId_to_u16(svc_resource));
    printf("    status: %d\n", NANO_HttpStatus_decode(svc_status));
    if (data_len > 0)
    {
        printf("      data:\n");
        printf("-- -- -- -- -- -- -- -- -- -- -- --\n");
        for (i = 0; i < data_len; i++)
        {
            printf("%c", data[i]);
        }
        printf("\n");
        printf("-- -- -- -- -- -- -- -- -- -- -- --\n");
    }
    else
    {
        printf("      data: <none>\n");
    }
    if (metadata_len > 0)
    {
        printf("  metadata:\n");
        printf("-- -- -- -- -- -- -- -- -- -- -- --\n");
        for (i = 0; i < metadata_len; i++)
        {
            printf("%c", metadata[i]);
        }
        printf("\n");
        printf("-- -- -- -- -- -- -- -- -- -- -- --\n");
    }
    else
    {
        printf("  metadata: <none>\n");
    }
    printf("## ## ## ## ## ## ## ## ## ## ## ##\n");
}

void
on_service_reply(
    NANO_XRCE_ClientListener *const self,
    NANO_XRCE_Client *const client,
    const NANO_XRCE_RequestId *const svc_request,
    const NANO_XRCE_ObjectId *const svc_resource,
    const NANO_bool little_endian,
    const NANO_XRCE_ServiceReplyStatus svc_status,
    const NANO_u8 *const data,
    const NANO_usize data_len,
    const NANO_u8 *const metadata,
    const NANO_usize metadata_len)
{
    printf("HTTP reply received for 0x%04X\n",
        NANO_XRCE_RequestId_to_u16(svc_request));

    last_reply.svc_request = *svc_request;
    last_reply.svc_resource = *svc_resource;
    last_reply.svc_status = svc_status;
    if (last_reply.data != NULL)
    {
        free(last_reply.data);
        last_reply.data = NULL;
        last_reply.data_len = 0;
    }
    if (data_len > 0)
    {
      last_reply.data = (NANO_u8*)malloc(data_len);
      last_reply.data_len = data_len;
      memcpy(last_reply.data, data, data_len);
    }
    if (last_reply.metadata != NULL)
    {
        free(last_reply.metadata);
        last_reply.metadata = NULL;
        last_reply.metadata_len = 0;
    }
    if (metadata_len > 0)
    {
      last_reply.metadata = (NANO_u8*)malloc(metadata_len);
      last_reply.metadata_len = metadata_len;
      memcpy(last_reply.metadata, metadata, metadata_len);
    }
}

int main(int argc, char const *argv[])
{
    int rc = 1;

    const NANO_u8 agent_addr[4] = AGENT_ADDRESS;

    static NANO_MessageBufferData frag_buffer[
        FRAG_BUFFER_SIZE / sizeof(NANO_MessageBufferData)] = { 0 };

    static NANO_MessageBufferData recv_buffer[
        RECV_BUFFER_SIZE / sizeof(NANO_MessageBufferData)] = { 0 };
    
    NANO_XRCE_Udpv4Client udp_client = NANO_XRCE_UDPV4CLIENT_INITIALIZER;
    NANO_XRCE_Client *const client = &udp_client.client;

    NANO_RetCode retcode = NANO_RETCODE_ERROR;
    NANO_XRCE_ClientRequestToken http_req = NANO_XRCE_CLIENTREQUESTTOKEN_INITIALIZER;
    NANO_XRCE_ResultStatus result = NANO_XRCE_RESULTSTATUS_INITIALIZER;

    NANO_LOG_SET_LEVEL(LOG_LEVEL)

    if (NANO_RETCODE_OK != 
            NANO_XRCE_Udpv4Client_initialize(
                &udp_client,
                CLIENT_KEY,
                TRANSPORT_MTU,
                (NANO_u8*)recv_buffer,
                sizeof(recv_buffer),
                agent_addr,
                0 /* default agent port */,
                NULL /* default bind address */,
                0 /* default bind port */))
    {
        printf("ERROR: failed to initialize client\n");
        goto done;
    }

    NANO_XRCE_Client_on_service_reply(
        &udp_client.client, on_service_reply, NULL);
    
    if (NANO_RETCODE_OK !=
        NANO_XRCE_Client_fragment_reconstruction_buffer(
            &udp_client.client,
            NANO_XRCE_STREAMID_BUILTIN_RELIABLE,
            (NANO_u8*)frag_buffer,
            sizeof(frag_buffer)))
    {
        printf("ERROR: failed to set frag buffer\n");
        goto done;
    }

    do
    {
        retcode = NANO_XRCE_Client_connect(client, REQUEST_TIMEOUT);
        
        if (NANO_RETCODE_OK != retcode && NANO_RETCODE_TIMEOUT != retcode)
        {
            printf("ERROR: failed to connect to agent\n");
            goto done;
        }
    } while (!NANO_XRCE_Client_connected(client));

#define print_last_reply() \
    print_reply(\
        &last_reply.svc_request,\
        &last_reply.svc_resource,\
        last_reply.svc_status,\
        last_reply.data, last_reply.data_len,\
        last_reply.metadata, last_reply.metadata_len)

    if (NANO_RETCODE_OK !=
        NANO_XRCE_Client_service_request(
            &udp_client.client,
            NULL /* token */,
            NANO_XRCE_STREAMID_BUILTIN_RELIABLE,
            NANO_XRCE_REQUESTFLAGS_CONFIRM |
              NANO_XRCE_REQUESTFLAGS_SYNC,
            REQUEST_TIMEOUT,
            ROOT_ID,
            NANO_HttpMethod_to_flags(NANO_HTTPMETHOD_GET),
            NULL, 0 /* query */,
            NULL, 0 /* data */,
            NULL, 0 /* metadata */))
    {
        printf("ERROR: failed to GET root\n");
        goto done;
    }
    print_last_reply();

    /* HTTP status codes (e.g. 404) should be propagated through XRCE */
    if (NANO_RETCODE_OK !=
        NANO_XRCE_Client_service_request(
            &udp_client.client,
            NULL /* token */,
            NANO_XRCE_STREAMID_BUILTIN_RELIABLE,
            NANO_XRCE_REQUESTFLAGS_CONFIRM |
              NANO_XRCE_REQUESTFLAGS_SYNC,
            REQUEST_TIMEOUT,
            ROOT_ID,
            NANO_HttpMethod_to_flags(NANO_HTTPMETHOD_GET),
            // NULL, 0 /* query */,
            "UNKNOWN_URL", sizeof("UNKNOWN_URL") - 1  /* query */,
            NULL, 0 /* data */,
            NULL, 0 /* metadata */))
    {
        printf("ERROR: failed to GET unknown URL\n");
        goto done;
    }
    print_last_reply();

    /* POST some data to the API endpoint with custom query.
       Requests don't have to be explicitly confirmed by the agent
       (only use SYNC). */
    if (NANO_RETCODE_OK !=
        NANO_XRCE_Client_service_request(
            &udp_client.client,
            &http_req,
            NANO_XRCE_STREAMID_BUILTIN_RELIABLE,
            NANO_XRCE_REQUESTFLAGS_SYNC,
            REQUEST_TIMEOUT,
            API_ID,
            NANO_HttpMethod_to_flags(NANO_HTTPMETHOD_POST),
            POST_QUERY, sizeof(POST_QUERY) - 1 /* query */,
            POST_DATA, sizeof(POST_DATA) - 1 /* data */,
            POST_METADATA, sizeof(POST_METADATA) - 1 /* metadata */))
    {
        printf("ERROR: failed to POST api\n");
        goto done;
    }
    print_last_reply();

    /* Requests can also be sent asynchronously */
    if (NANO_RETCODE_OK !=
        NANO_XRCE_Client_service_request(
            &udp_client.client,
            &http_req,
            NANO_XRCE_STREAMID_BUILTIN_RELIABLE,
            NANO_XRCE_REQUESTFLAGS_CONFIRM,
            REQUEST_TIMEOUT,
            API_ID,
            NANO_HttpMethod_to_flags(NANO_HTTPMETHOD_POST),
            POST_QUERY, sizeof(POST_QUERY) - 1 /* query */,
            JSON_POST_DATA, sizeof(JSON_POST_DATA) - 1 /* data */,
            JSON_POST_METADATA, sizeof(JSON_POST_METADATA) - 1 /* metadata */))
    {
        printf("ERROR: failed to POST json api\n");
        goto done;
    }

    /* Wait for a reply to the request is received */
    if (NANO_RETCODE_OK !=
        NANO_XRCE_Client_wait_for_request_complete(
            &udp_client.client, &http_req, REQUEST_TIMEOUT, &result))
    {
        printf("ERROR: failed to wait for HTTP reply");
        goto done;
    }

    if (result.status != NANO_XRCE_STATUS_OK)
    {
        printf("ERROR: failed to submit HTTP request\n");
        goto disconnect;
    }
    print_last_reply();

disconnect:

    if (NANO_RETCODE_OK !=
            NANO_XRCE_Client_disconnect(
                client,
                NULL /* req_token */,
                NANO_XRCE_STREAMID_BUILTIN_RELIABLE,
                NANO_XRCE_REQUESTFLAGS_SYNC,
                REQUEST_TIMEOUT))
    {
        printf("ERROR: failed to disconnect from Agent\n");
        goto done;
    }

    NANO_XRCE_Client_finalize(client);

    if (last_reply.data != NULL)
    {
        free(last_reply.data);
    }
    if (last_reply.metadata != NULL)
    {
        free(last_reply.metadata);
    }

    rc = 0;
done:
    return rc;
}
