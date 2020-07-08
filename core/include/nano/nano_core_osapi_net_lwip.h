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

#ifndef nano_core_osapi_net_lwip_h
#define nano_core_osapi_net_lwip_h

#if NANO_FEAT_TRANSPORT_IPV4

#define NANO_HAVE_IPV4

#ifndef NANO_HAVE_NET_LWIP
#define NANO_HAVE_NET_LWIP
#endif /* NANO_HAVE_LWIP */

#include "lwip/init.h" /* LWIP_VERSION */

/******************************************************************************
 *                                Socket Type
 ******************************************************************************/

#if NANO_FEAT_TRANSPORT_PLUGIN_UDPV4 || \
    NANO_FEAT_TRANSPORT_PLUGIN_TCPV4 || \
    NANO_FEAT_TRANSPORT_PLUGIN_UDPV6 || \
    NANO_FEAT_TRANSPORT_PLUGIN_TCPV6

#define NANO_HAVE_MULTICAST


typedef NANO_u8 NANO_LwipSocketFlags;

#define NANO_LWIPSOCKETFLAGS_DEFAULT          (0)
#define NANO_LWIPSOCKETFLAGS_NONBLOCK         (0x01 << 0)
#define NANO_LWIPSOCKETFLAGS_REUSE            (0x01 << 1)
#define NANO_LWIPSOCKETFLAGS_MULTICAST        (0x01 << 2)

typedef struct NANODllExport NANO_OSAPI_LwipSocketPropertiesI
{
    NANO_u8 *recv_buffer;
    NANO_usize recv_buffer_size;
    NANO_LwipSocketFlags flags;

} NANO_OSAPI_LwipSocketProperties;

#define NANO_OSAPI_LWIPSOCKETPROPERTIES_INITIALIZER \
{\
    NULL,  /* recv_buffer */\
    0, /* recv_buffer_size */\
    NANO_LWIPSOCKETFLAGS_DEFAULT /* flags */\
}

#define NANO_OSAPI_Udpv4SocketProperties_set_multicast(s_) \
{\
    (s_)->flags |= NANO_SOCKETFLAGS_MULTICAST;\
}


typedef struct NANODllExport NANO_OSAPI_LwipUdpSocketI
{
    struct udp_pcb *pcb;
    struct pbuf *pbuf_out[NANO_LIMIT_LWIP_PBUF_OUT_MAX_LENGTH];
    struct pbuf *pbuf_in;
    NANO_u32 read_addr;
    NANO_u16 read_port;
    NANO_MessageBuffer recv_msg;
    NANO_usize recv_buffer_size;
    NANO_u8 *recv_buffer;
} NANO_OSAPI_LwipUdpSocket;

#define NANO_OSAPI_LWIPUDPSOCKET_INITIALIZER \
{ \
    NULL, /* pcb */ \
    { NULL },  /* pbuf_out */ \
    NULL, /* pbuf_in */\
    0, /* read_addr */\
    0, /* read_port */\
    NANO_MESSAGEBUFFER_INITIALIZER, /* recv_msg */\
    0, /* recv_buffer_size */ \
    NULL /* recv_buffer */\
}
#define NANO_OSAPI_LwipUdpSocket_is_valid(s_) \
    ((s_)->pcb != NULL)


#endif /* NANO_FEAT_TRANSPORT_PLUGIN_UDPV4 || \
          NANO_FEAT_TRANSPORT_PLUGIN_TCPV4 || \
          NANO_FEAT_TRANSPORT_PLUGIN_UDPV6 || \
          NANO_FEAT_TRANSPORT_PLUGIN_TCPV6 */


#if NANO_FEAT_TRANSPORT_PLUGIN_UDPV4
#define NANO_OSAPI_Udpv4Socket         NANO_OSAPI_LwipUdpSocket
#define NANO_OSAPI_UDPV4SOCKET_INITIALIZER \
        NANO_OSAPI_LWIPUDPSOCKET_INITIALIZER
#define NANO_OSAPI_Udpv4SocketProperties    NANO_OSAPI_LwipSocketProperties
#define NANO_OSAPI_UDPV4SOCKETPROPERTIES_INITIALIZER \
        NANO_OSAPI_LWIPSOCKETPROPERTIES_INITIALIZER
#define NANO_HAVE_UDPV4
#endif /* NANO_FEAT_TRANSPORT_PLUGIN_UDPV4 */


#if NANO_FEAT_TRANSPORT_PLUGIN_UDPV4

NANO_RetCode
NANO_OSAPI_LwipUdpSocket_open(
    NANO_OSAPI_Udpv4Socket *const self,
    const NANO_u8 *const addr,
    const NANO_u16 port,
    const NANO_OSAPI_Udpv4SocketProperties *const properties);
#define NANO_OSAPI_Udpv4Socket_open \
        NANO_OSAPI_LwipUdpSocket_open

NANO_RetCode
NANO_OSAPI_LwipUdpSocket_send(
    NANO_OSAPI_Udpv4Socket *const self,
    const NANO_u8 *const dest_addr,
    const NANO_u16 dest_port,
    const NANO_MessageBuffer *msg);
#define NANO_OSAPI_Udpv4Socket_send \
        NANO_OSAPI_LwipUdpSocket_send

NANO_RetCode
NANO_OSAPI_LwipUdpSocket_recv(
    NANO_OSAPI_Udpv4Socket *const self,
    NANO_u8 *const src_addr,
    NANO_u16 *const src_port,
    NANO_MessageBuffer *const msg,
    NANO_usize *const msg_size);
#define NANO_OSAPI_Udpv4Socket_recv \
        NANO_OSAPI_LwipUdpSocket_recv

NANO_RetCode
NANO_OSAPI_LwipUdpSocket_close(NANO_OSAPI_Udpv4Socket *const self);
#define NANO_OSAPI_Udpv4Socket_close \
        NANO_OSAPI_LwipUdpSocket_close

#define NANO_OSAPI_Udpv4Socket_is_valid \
    NANO_OSAPI_LwipUdpSocket_is_valid

#endif /* NANO_FEAT_TRANSPORT_PLUGIN_UDPV4 */

#endif /* NANO_FEAT_TRANSPORT_IPV4 */

#endif /* nano_core_osapi_net_lwip_h */