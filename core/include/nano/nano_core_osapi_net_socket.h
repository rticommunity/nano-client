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

#ifndef nano_core_osapi_net_socket_h
#define nano_core_osapi_net_socket_h

#if defined(NANO_HAVE_SOCKET)
#if NANO_FEAT_TRANSPORT_IPV4

#define NANO_HAVE_IPV4

/******************************************************************************
 *                             Helper Functions
 ******************************************************************************/
#if NANO_FEAT_TRANSPORT_RESOLVE
NANO_RetCode
NANO_OSAPI_BsdIpv4Addr_resolve(
    NANO_Ipv4Addr *const self,
    const char *const addr_str);
#define NANO_HAVE_DNS

#define NANO_OSAPI_Ipv4Addr_resolve \
        NANO_OSAPI_BsdIpv4Addr_resolve

#endif /* NANO_FEAT_TRANSPORT_RESOLVE */

#if NANO_FEAT_TRANSPORT_AUTOCFG
NANO_RetCode
NANO_OSAPI_BsdIpv4Addr_list(
    NANO_Ipv4Addr *const addrs,
    NANO_usize *const addrs_len);
#define NANO_OSAPI_Ipv4Addr_list \
        NANO_OSAPI_BsdIpv4Addr_list
#define NANO_HAVE_IFCONFIG
#endif /* #if NANO_FEAT_TRANSPORT_AUTOCFG */

#if NANO_FEAT_TRANSPORT_STRING
NANO_RetCode
NANO_OSAPI_BsdIpv4Addr_from_str(
    NANO_Ipv4Addr *const self,
    const char *const ip_str);

#define NANO_OSAPI_Ipv4Addr_from_str \
        NANO_OSAPI_BsdIpv4Addr_from_str
#endif /* NANO_FEAT_TRANSPORT_STRING */

#endif /* NANO_FEAT_TRANSPORT_IPV4 */

/******************************************************************************
 *                                Socket Type
 ******************************************************************************/

#if NANO_FEAT_TRANSPORT_PLUGIN_UDPV4 || \
    NANO_FEAT_TRANSPORT_PLUGIN_TCPV4 || \
    NANO_FEAT_TRANSPORT_PLUGIN_UDPV6 || \
    NANO_FEAT_TRANSPORT_PLUGIN_TCPV6

#if !defined(IP_MULTICAST_LOOP) || !defined(IP_MULTICAST_IF)   || \
    !defined(IP_MULTICAST_TTL)  || !defined(IP_ADD_MEMBERSHIP) || \
    !defined(IP_DROP_MEMBERSHIP)
#undef NANO_HAVE_MULTICAST
#else
#define NANO_HAVE_MULTICAST
#endif


typedef NANO_u8 NANO_SocketFlags;

#define NANO_SOCKETFLAGS_DEFAULT          (0)
#define NANO_SOCKETFLAGS_NONBLOCK         (0x01 << 0)
#define NANO_SOCKETFLAGS_REUSE            (0x01 << 1)
#define NANO_SOCKETFLAGS_MULTICAST        (0x01 << 2)

typedef struct NANODllExport NANO_OSAPI_BsdSocketI
{
    int fd;
#if NANO_CPP
    NANO_OSAPI_BsdSocketI()
    {
        this->fd = NANO_FD_INVALID;
    }
#endif /* NANO_CPP */
} NANO_OSAPI_BsdSocket;

#define NANO_OSAPI_BSDSOCKET_INITIALIZER \
{ \
    NANO_FD_INVALID, /* fd */ \
}

typedef struct NANODllExport NANO_OSAPI_BsdSocketPropertiesI
{
    NANO_usize recv_buffer_size;
    NANO_usize send_buffer_size;
    NANO_SocketFlags flags;
    NANO_u64 timeout_us;

#if NANO_CPP
    NANO_OSAPI_BsdSocketPropertiesI()
    {
        this->flags = NANO_SOCKETFLAGS_DEFAULT;
        this->recv_buffer_size = 0;
        this->send_buffer_size = 0;
        this->timeout_us = NANO_LIMIT_SOCKET_RECV_TIMEOUT_MS;
    }
#endif /* NANO_CPP */

} NANO_OSAPI_BsdSocketProperties;

#define NANO_OSAPI_BSDSOCKETPROPERTIES_INITIALIZER \
{\
    0, /* recv_buffer_size */\
    0,  /* send_buffer_size */\
    NANO_SOCKETFLAGS_DEFAULT, /* flags */\
    NANO_LIMIT_SOCKET_RECV_TIMEOUT_MS /* timeout_us */ \
}

#define NANO_OSAPI_Udpv4SocketProperties_set_multicast(s_) \
{\
    (s_)->flags |= NANO_SOCKETFLAGS_MULTICAST;\
}

#define NANO_OSAPI_BsdSocket_is_valid(s_) \
    ((s_)->fd != NANO_FD_INVALID)

#endif /* NANO_FEAT_TRANSPORT_PLUGIN_UDPV4 || \
          NANO_FEAT_TRANSPORT_PLUGIN_TCPV4 || \
          NANO_FEAT_TRANSPORT_PLUGIN_UDPV6 || \
          NANO_FEAT_TRANSPORT_PLUGIN_TCPV6 */


#if NANO_FEAT_TRANSPORT_PLUGIN_UDPV4
#define NANO_OSAPI_Udpv4Socket         NANO_OSAPI_BsdSocket
#define NANO_OSAPI_UDPV4SOCKET_INITIALIZER \
        NANO_OSAPI_BSDSOCKET_INITIALIZER
#define NANO_OSAPI_Udpv4SocketProperties    NANO_OSAPI_BsdSocketProperties
#define NANO_OSAPI_UDPV4SOCKETPROPERTIES_INITIALIZER \
        NANO_OSAPI_BSDSOCKETPROPERTIES_INITIALIZER
#define NANO_HAVE_UDPV4
#endif /* NANO_FEAT_TRANSPORT_PLUGIN_UDPV4 */

#if NANO_FEAT_TRANSPORT_PLUGIN_TCPV4
#define NANO_OSAPI_Tcpv4Socket         NANO_OSAPI_BsdSocket
#define NANO_OSAPI_TCPV4SOCKET_INITIALIZER \
        NANO_OSAPI_BSDSOCKET_INITIALIZER
#define NANO_OSAPI_Tcpv4SocketProperties    NANO_OSAPI_BsdSocketProperties
#define NANO_OSAPI_TCPV4SOCKETPROPERTIES_INITIALIZER \
        NANO_OSAPI_BSDSOCKETPROPERTIES_INITIALIZER
#define NANO_HAVE_TCPV4
#endif /* NANO_FEAT_TRANSPORT_PLUGIN_TCPV4 */


#if NANO_FEAT_TRANSPORT_PLUGIN_UDPV4

NANO_RetCode
NANO_OSAPI_BsdUdpv4Socket_open(
    NANO_OSAPI_Udpv4Socket *const self,
    const NANO_u8 *const addr,
    const NANO_u16 port,
    const NANO_OSAPI_Udpv4SocketProperties *const properties);
#define NANO_OSAPI_Udpv4Socket_open \
        NANO_OSAPI_BsdUdpv4Socket_open

NANO_RetCode
NANO_OSAPI_BsdUdpv4Socket_send(
    NANO_OSAPI_Udpv4Socket *const self,
    const NANO_u8 *const dest_addr,
    const NANO_u16 dest_port,
    const NANO_MessageBuffer *const msg);
#define NANO_OSAPI_Udpv4Socket_send \
        NANO_OSAPI_BsdUdpv4Socket_send

NANO_RetCode
NANO_OSAPI_BsdUdpv4Socket_recv(
    NANO_OSAPI_Udpv4Socket *const self,
    NANO_u8 *const src_addr,
    NANO_u16 *const src_port,
    NANO_MessageBuffer *const msg,
    NANO_usize *const msg_size);
#define NANO_OSAPI_Udpv4Socket_recv \
        NANO_OSAPI_BsdUdpv4Socket_recv

NANO_RetCode
NANO_OSAPI_BsdUdpv4Socket_close(NANO_OSAPI_Udpv4Socket *const self);
#define NANO_OSAPI_Udpv4Socket_close \
        NANO_OSAPI_BsdUdpv4Socket_close

#define NANO_OSAPI_Udpv4Socket_is_valid \
    NANO_OSAPI_BsdSocket_is_valid

#endif /* NANO_FEAT_TRANSPORT_PLUGIN_UDPV4 */

#if NANO_FEAT_TRANSPORT_PLUGIN_TCPV4
NANO_RetCode
NANO_OSAPI_BsdTcpv4Socket_open(
    NANO_OSAPI_Tcpv4Socket *const self,
    const NANO_OSAPI_Tcp4SocketProperties *const properties);
#define NANO_OSAPI_Tcpv4Socket_open \
        NANO_OSAPI_BsdTcpv4Socket_open

NANO_RetCode
NANO_OSAPI_BsdTcpv4Socket_send(
    NANO_OSAPI_Tcpv4Socket *const self,
    const NANO_MessageBuffer *msg);
#define NANO_OSAPI_Tcpv4Socket_send \
        NANO_OSAPI_BsdTcpv4Socket_send

NANO_RetCode
NANO_OSAPI_BsdTcpv4Socket_recv(
    NANO_OSAPI_Tcpv4Socket *const self,
    NANO_u8 *const msg,
    NANO_usize *const msg_len);
#define NANO_OSAPI_Tcpv4Socket_recv \
        NANO_OSAPI_BsdTcpv4Socket_recv

NANO_RetCode
NANO_OSAPI_BsdTcpv4Socket_close(NANO_OSAPI_Tcpv4Socket *const self);
#define NANO_OSAPI_Tcpv4Socket_close \
        NANO_OSAPI_BsdTcpv4Socket_close

#if NANO_FEAT_AGENT
NANO_RetCode
NANO_OSAPI_BsdTcpv4Socket_bind(
    NANO_OSAPI_Tcpv4Socket *const self,
    const NANO_Ipv4Addr addr,
    const NANO_u16 port);
#define NANO_OSAPI_Tcpv4Socket_bind \
        NANO_OSAPI_BsdTcpv4Socket_bind

NANO_RetCode
NANO_OSAPI_BsdTcpv4Socket_listen(NANO_OSAPI_Tcpv4Socket *const self);
#define NANO_OSAPI_Tcpv4Socket_listen \
        NANO_OSAPI_BsdTcpv4Socket_listen

NANO_RetCode
NANO_OSAPI_BsdTcpv4Socket_accept(
    NANO_OSAPI_Tcpv4Socket *const self,
    NANO_OSAPI_Tcpv4Socket *const new_conn);
#define NANO_OSAPI_Tcpv4Socket_accept \
        NANO_OSAPI_BsdTcpv4Socket_accept
#endif /* NANO_FEAT_AGENT */

NANO_RetCode
NANO_OSAPI_BsdTcpv4Socket_connect(
    NANO_OSAPI_Tcpv4Socket *const self,
    const NANO_Ipv4Addr addr,
    const NANO_u16 port);
#define NANO_OSAPI_Tcpv4Socket_connect \
        NANO_OSAPI_BsdTcpv4Socket_connect

#define NANO_OSAPI_Tcpv4Socket_is_valid \
    NANO_OSAPI_BsdSocket_is_valid

#endif /* NANO_FEAT_TRANSPORT_PLUGIN_TCPV4 */

#endif /* defined(NANO_HAVE_SOCKET) */


#endif /* nano_core_osapi_net_socket_h */