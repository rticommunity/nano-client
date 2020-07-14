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

#if defined(NANO_HAVE_SOCKET_WIN)
#if NANO_FEAT_TRANSPORT_IPV4

#include <winsock2.h>

#define NANO_HAVE_IPV4
#define NANO_HAVE_MULTICAST

/******************************************************************************
 *                             Helper Functions
 ******************************************************************************/

#if NANO_FEAT_TRANSPORT_AUTOCFG
NANO_RetCode
NANO_OSAPI_WinIpv4Addr_list(
    NANO_Ipv4Addr *const addrs,
    NANO_usize *const addrs_len);
#define NANO_OSAPI_Ipv4Addr_list \
        NANO_OSAPI_WinIpv4Addr_list
#define NANO_HAVE_IFCONFIG
#endif /* #if NANO_FEAT_TRANSPORT_AUTOCFG */

#if NANO_FEAT_TRANSPORT_STRING
NANO_RetCode
NANO_OSAPI_WinIpv4Addr_from_str(
    NANO_Ipv4Addr *const self,
    const char *const ip_str);

#define NANO_OSAPI_Ipv4Addr_from_str \
        NANO_OSAPI_WinIpv4Addr_from_str
#endif /* NANO_FEAT_TRANSPORT_STRING */

#endif /* NANO_FEAT_TRANSPORT_IPV4 */

/******************************************************************************
 *                                Socket Type
 ******************************************************************************/

#if NANO_FEAT_TRANSPORT_PLUGIN_UDPV4 || \
    NANO_FEAT_TRANSPORT_PLUGIN_TCPV4 || \
    NANO_FEAT_TRANSPORT_PLUGIN_UDPV6 || \
    NANO_FEAT_TRANSPORT_PLUGIN_TCPV6


typedef NANO_u8 NANO_SocketFlags;

#define NANO_SOCKETFLAGS_DEFAULT          (0)
#define NANO_SOCKETFLAGS_NONBLOCK         (0x01 << 0)
#define NANO_SOCKETFLAGS_REUSE            (0x01 << 1)
#define NANO_SOCKETFLAGS_MULTICAST        (0x01 << 2)

typedef struct NANODllExport NANO_OSAPI_WinSocketI
{
    SOCKET fd;
#if NANO_CPP
    NANO_OSAPI_WinSocketI()
    {
        this->fd = NANO_FD_INVALID;
    }
#endif /* NANO_CPP */
} NANO_OSAPI_WinSocket;

#define NANO_OSAPI_WINSOCKET_INITIALIZER \
{ \
    NANO_FD_INVALID, /* fd */ \
}

typedef struct NANODllExport NANO_OSAPI_WinSocketPropertiesI
{
    NANO_SocketFlags flags;
    NANO_u64 timeout_ms;

#if NANO_CPP
    NANO_OSAPI_WinSocketPropertiesI()
    {
        this->flags = NANO_SOCKETFLAGS_DEFAULT;
        this->timeout_ms = NANO_LIMIT_SOCKET_RECV_TIMEOUT_MS;
    }
#endif /* NANO_CPP */

} NANO_OSAPI_WinSocketProperties;

#define NANO_OSAPI_WINSOCKETPROPERTIES_INITIALIZER \
{\
    NANO_SOCKETFLAGS_DEFAULT, /* flags */\
    NANO_LIMIT_SOCKET_RECV_TIMEOUT_MS /* timeout_ms */ \
}

#define NANO_OSAPI_Udpv4SocketProperties_set_multicast(s_) \
{\
    (s_)->flags |= NANO_SOCKETFLAGS_MULTICAST;\
}

#define NANO_OSAPI_WinSocket_is_valid(s_) \
    ((s_)->fd != NANO_FD_INVALID)

#endif /* NANO_FEAT_TRANSPORT_PLUGIN_UDPV4 || \
          NANO_FEAT_TRANSPORT_PLUGIN_TCPV4 || \
          NANO_FEAT_TRANSPORT_PLUGIN_UDPV6 || \
          NANO_FEAT_TRANSPORT_PLUGIN_TCPV6 */


#if NANO_FEAT_TRANSPORT_PLUGIN_UDPV4
#define NANO_OSAPI_Udpv4Socket         NANO_OSAPI_WinSocket
#define NANO_OSAPI_UDPV4SOCKET_INITIALIZER \
        NANO_OSAPI_WINSOCKET_INITIALIZER
#define NANO_OSAPI_Udpv4SocketProperties    NANO_OSAPI_WinSocketProperties
#define NANO_OSAPI_UDPV4SOCKETPROPERTIES_INITIALIZER \
        NANO_OSAPI_WINSOCKETPROPERTIES_INITIALIZER
#define NANO_HAVE_UDPV4
#endif /* NANO_FEAT_TRANSPORT_PLUGIN_UDPV4 */

#if NANO_FEAT_TRANSPORT_PLUGIN_UDPV4

NANO_RetCode
NANO_OSAPI_WinUdpv4Socket_open(
    NANO_OSAPI_Udpv4Socket *const self,
    const NANO_u8 *const addr,
    const NANO_u16 port,
    const NANO_OSAPI_Udpv4SocketProperties *const properties);
#define NANO_OSAPI_Udpv4Socket_open \
        NANO_OSAPI_WinUdpv4Socket_open

NANO_RetCode
NANO_OSAPI_WinUdpv4Socket_send(
    NANO_OSAPI_Udpv4Socket *const self,
    const NANO_u8 *const dest_addr,
    const NANO_u16 dest_port,
    const NANO_MessageBuffer *const msg);
#define NANO_OSAPI_Udpv4Socket_send \
        NANO_OSAPI_WinUdpv4Socket_send

NANO_RetCode
NANO_OSAPI_WinUdpv4Socket_recv(
    NANO_OSAPI_Udpv4Socket *const self,
    NANO_u8 *const src_addr,
    NANO_u16 *const src_port,
    NANO_MessageBuffer *const msg,
    NANO_usize *const msg_size);
#define NANO_OSAPI_Udpv4Socket_recv \
        NANO_OSAPI_WinUdpv4Socket_recv

NANO_RetCode
NANO_OSAPI_WinUdpv4Socket_close(NANO_OSAPI_Udpv4Socket *const self);
#define NANO_OSAPI_Udpv4Socket_close \
        NANO_OSAPI_WinUdpv4Socket_close

#define NANO_OSAPI_Udpv4Socket_is_valid \
    NANO_OSAPI_WinSocket_is_valid

#endif /* NANO_FEAT_TRANSPORT_PLUGIN_UDPV4 */

#endif /* defined(NANO_HAVE_SOCKET) */


#endif /* nano_core_osapi_net_socket_h */