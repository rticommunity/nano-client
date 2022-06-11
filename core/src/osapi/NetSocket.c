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

#ifdef NANO_HAVE_SOCKET

#if NANO_FEAT_TRANSPORT_IPV4 || \
    NANO_FEAT_TRANSPORT_IPV6
/* Header required by symbols:
 * - close()
 * - fcntl()
 */
#include <unistd.h>
/* Header required by symbols:
 * - fcntl()
 */
#include <fcntl.h>
/* Header required by symbols:
 * - getaddrinfo()
 * - socket()
 */
#include <sys/types.h>
/* Header required by symbols:
 * - getaddrinfo()
 * - socket()
 * - AF_INET
 */
#if defined(NANO_HAVE_LWIP)
#include <lwip/sockets.h>
#else
#include <sys/socket.h>
#endif

#if NANO_FEAT_TRANSPORT_RESOLVE
/* Header required by symbols:
 * - getaddrinfo()
 */
#include <netdb.h>
#endif /* NANO_FEAT_TRANSPORT_RESOLVE */
/* Header required by symbols:
 * - inet_pton()
 */
#if defined(NANO_HAVE_LWIP)
#include <lwip/inet.h>
#else
#include <arpa/inet.h>
/* Header required by symbols:
 * - struct sockaddr_in
 */
#include <netinet/in.h> 
#endif
/* Header required by symbols:
 * - errno
 */
#include <errno.h>
#endif /* NANO_FEAT_TRANSPORT_IPV4 || \
          NANO_FEAT_TRANSPORT_IPV6 */

#if NANO_FEAT_TRANSPORT_IPV4

#if NANO_FEAT_TRANSPORT_RESOLVE
NANO_RetCode
NANO_OSAPI_BsdIpv4Addr_resolve(
    NANO_Ipv4Addr *const self,
    const char *const addr_str)
{
    return NANO_RETCODE_ERROR;
}
#endif /* NANO_FEAT_TRANSPORT_RESOLVE */

#if NANO_FEAT_TRANSPORT_AUTOCFG

#include <ifaddrs.h>

NANO_RetCode
NANO_OSAPI_BsdIpv4Addr_list(
    NANO_Ipv4Addr *const addrs,
    NANO_usize *const addrs_len)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    struct ifaddrs *sys_addrs = NULL,
                   *nxt = NULL;
    NANO_usize i = 0;
    NANO_Ipv4Addr addr = 0;
    
    NANO_LOG_FN_ENTRY
    
    if(0 != getifaddrs(&sys_addrs))
    {
        NANO_LOG_ERROR_MSG("FAILED to list network interfaces")
        goto done;
    }

    for(i = 0, nxt = sys_addrs;
            i < *addrs_len && nxt != NULL;
                nxt = nxt->ifa_next)
    {
        struct sockaddr *if_addr = nxt->ifa_addr;

        if(if_addr == NULL ||
            if_addr->sa_family != AF_INET)
        {
            continue;
        }

        addr = ((struct sockaddr_in*)if_addr)->sin_addr.s_addr;
        if (NANO_OSAPI_Ipv4Addr_is_loopback(&addr) ||
            addr == 0)
        {
            continue;
        }
        *(addrs + i) = addr;
        i++;
    }

    *addrs_len = i;

    rc = NANO_RETCODE_OK;
    
done:
    if (sys_addrs != NULL)
    {
        freeifaddrs(sys_addrs);
    }
    
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;

}
#endif /* NANO_FEAT_TRANSPORT_AUTOCFG */

#if NANO_FEAT_TRANSPORT_STRING
NANO_RetCode
NANO_OSAPI_BsdIpv4Addr_from_str(
    NANO_Ipv4Addr *const self,
    const char *const ip_str)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    int rci = 0;
    struct sockaddr_in saddr;

    NANO_LOG_FN_ENTRY

    *self = 0;

    if (1 != (rci = inet_pton(AF_INET, ip_str, &saddr.sin_addr)))
    {
        NANO_LOG_ERROR("FAILED to parse IP address from string",
            NANO_LOG_STR("str",ip_str))
        rc = NANO_RETCODE_INVALID_ARGS;
        goto done;
    }
    
    *self = saddr.sin_addr.s_addr;
    
    rc = NANO_RETCODE_OK;
    
done:
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}
#endif /* NANO_FEAT_TRANSPORT_STRING */

#endif /* NANO_FEAT_TRANSPORT_IPV4 */

#if NANO_FEAT_TRANSPORT_PLUGIN_UDPV4

#if NANO_FEAT_SYSTIME
#include "sys/time.h"
#endif /* NANO_FEAT_SYSTIME */

NANO_RetCode
NANO_OSAPI_BsdUdpv4Socket_open(
    NANO_OSAPI_Udpv4Socket *const self,
    const NANO_u8 *const addr,
    const NANO_u16 port,
    const NANO_OSAPI_Udpv4SocketProperties *const properties)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    int rci = 0;
    struct sockaddr_in saddr;
    int flags = 0;
    int YES[] = { 1 };
#if NANO_FEAT_AGENT
#define MAX_ADDRS           16
    NANO_Ipv4Addr if_addrs[MAX_ADDRS] = { 0 };
    NANO_usize if_addrs_len = MAX_ADDRS;
#endif /* NANO_FEAT_AGENT */

    NANO_LOG_FN_ENTRY
    
    NANO_PCOND(self != NULL)
    NANO_PCOND(port > 0)
    NANO_PCOND(properties != NULL)
    NANO_PCOND(self->fd == NANO_FD_INVALID)

#ifndef NANO_HAVE_MULTICAST
    if (properties->flags & NANO_SOCKETFLAGS_MULTICAST)
    {
        NANO_LOG_ERROR_MSG("multicast NOT SUPPORTED by platform")
        goto done;
    }
#endif /* NANO_HAVE_MULTICAST */


    self->fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (self->fd < 0)
    {
        NANO_LOG_ERROR("FAILED to create UDP socket",
            NANO_LOG_I32("rc",self->fd))
        goto done;
    }

#if NANO_PLATFORM_IS_POSIX
    if ((rci = fcntl(self->fd, F_SETFD, FD_CLOEXEC)) != 0)
    {
        NANO_LOG_ERROR("FAILED to set FD_CLOEXEC flags on socket",
            NANO_LOG_I32("rc",rci))
        goto done;
    }
#endif /* NANO_PLATFORM_IS_POSIX */

    if (properties->flags & NANO_SOCKETFLAGS_NONBLOCK)
    {
        flags = fcntl(self->fd,F_GETFL,0);
        if (flags == -1)
        {
            NANO_LOG_ERROR("FAILED to get current socket flags",
                NANO_LOG_I32("rc",flags))
            goto done;
        }

        if ((rci = fcntl(self->fd, F_SETFL, flags | O_NONBLOCK)) != 0)
        {
            NANO_LOG_ERROR("FAILED to set socket as non-blocking",
                NANO_LOG_I32("rc",rci))
            goto done;
        }
    }

    if (properties->flags & NANO_SOCKETFLAGS_REUSE ||
        properties->flags & NANO_SOCKETFLAGS_MULTICAST)
    {
#if NANO_PLATFORM == NANO_PLATFORM_QNX ||\
    NANO_PLATFORM == NANO_PLATFORM_DARWIN
        if ((rci = setsockopt(
                self->fd,
                SOL_SOCKET,
                SO_REUSEPORT,
                (char *)&YES,
                sizeof(int))) != 0)
#else
        if ((rci = setsockopt(
                self->fd,
                SOL_SOCKET,
                SO_REUSEADDR,
                (char *)&YES,
                sizeof(int))) != 0)
#endif
        {
            NANO_LOG_ERROR("FAILED to set reuse address on socket",
                NANO_LOG_I32("rc",rci))
            goto done;
        }
    }

    if (properties->send_buffer_size > 0)
    {
        if ((rci = setsockopt(self->fd, SOL_SOCKET, SO_SNDBUF,
                    (char *)&properties->send_buffer_size,
                    sizeof(properties->send_buffer_size))) < 0)
        {
            NANO_LOG_ERROR("FAILED to socket send buffer",
                NANO_LOG_I32("rc",rci)
                NANO_LOG_USIZE("size",properties->send_buffer_size))
            goto done;
        }
    }

    if (properties->recv_buffer_size > 0)
    {
        if ((rci = setsockopt(self->fd, SOL_SOCKET, SO_RCVBUF,
                    (char *)&properties->recv_buffer_size,
                    sizeof(properties->recv_buffer_size))) < 0)
        {
            NANO_LOG_ERROR("FAILED to socket receive buffer",
                NANO_LOG_I32("rc",rci)
                NANO_LOG_USIZE("size",properties->recv_buffer_size))
            goto done;
        }
    }

#if NANO_FEAT_AGENT

    if (properties->flags & NANO_SOCKETFLAGS_MULTICAST)
    {
        /* set the multicast loopback option,0 = disable, 1 = enable */
        NANO_u8 option = 1;
        if ((rci = setsockopt(self->fd, IPPROTO_IP, IP_MULTICAST_LOOP,
                (void*)&option, sizeof(option))) < 0)
        {
            NANO_LOG_ERROR("FAILED to enable multicast",
                NANO_LOG_I32("rc",rci))
            goto done;
        }

        NANO_CHECK_RC(
            NANO_OSAPI_BsdIpv4Addr_list(if_addrs, &if_addrs_len),
            NANO_LOG_ERROR("FAILED to list NIC addresses",
                NANO_LOG_RC(rc)));
        
        if (if_addrs_len == 0)
        {
            NANO_LOG_ERROR_MSG("NO addresses to bind multicast socket to")
            goto done;
        }
        
        NANO_LOG_INFO("BINDING multicast socket",
            NANO_LOG_I32("fd",self->fd)
            NANO_LOG_H32("address",if_addrs[0]))

        NANO_PCOND(sizeof(saddr.sin_addr.s_addr) == sizeof(NANO_u32))
        NANO_OSAPI_Memory_zero(&saddr,sizeof(saddr));
        NANO_OSAPI_Memory_copy(
            &saddr.sin_addr.s_addr, if_addrs, sizeof(saddr.sin_addr.s_addr));
        saddr.sin_family = AF_INET;

        if ((rci = setsockopt(self->fd, IPPROTO_IP, IP_MULTICAST_IF,
                    (char *)&saddr, sizeof(saddr))) < 0)
        {
            NANO_LOG_ERROR("FAILED to set multicast address",
                NANO_LOG_I32("rc",rci))
            goto done;
        }
    }
#endif /* NANO_FEAT_AGENT */

    if (properties->timeout_ms > 0) 
    {
        struct timeval timeout_tv;

        timeout_tv.tv_sec = properties->timeout_ms / 1000;
        timeout_tv.tv_usec =
            (properties->timeout_ms - (timeout_tv.tv_sec * 1000)) * 1000;
        
        NANO_LOG_INFO("SOCKET TIMEOUT",
            NANO_LOG_U32("timeout_ms",properties->timeout_ms)
            NANO_LOG_U32("tv.sec",timeout_tv.tv_sec)
            NANO_LOG_U32("tv.usec",timeout_tv.tv_usec))
        
        if ((rci =
                setsockopt(
                    self->fd,
                    SOL_SOCKET,
                    SO_RCVTIMEO,
                    &timeout_tv,
                    sizeof(timeout_tv))) < 0)
        {
            NANO_LOG_ERROR("FAILED to set receive timeout",
                NANO_LOG_I32("rc",rci))
            goto done;
        }
    }

    NANO_PCOND(sizeof(saddr.sin_addr.s_addr) == sizeof(NANO_u32))
    NANO_OSAPI_Memory_zero(&saddr,sizeof(saddr));
    NANO_OSAPI_Memory_copy(
        &saddr.sin_addr.s_addr, addr, sizeof(saddr.sin_addr.s_addr));
    saddr.sin_port = NANO_u16_to_be(port);
    saddr.sin_family = AF_INET;


    rci = bind(self->fd, (const struct sockaddr *)&saddr,sizeof(saddr));
    if (0 != rci)
    { 
        NANO_LOG_ERROR("FAILED to bind UDP socket",
                NANO_LOG_I32("rc",rci)
                NANO_LOG_BYTES("addr",addr,sizeof(NANO_u32))
                NANO_LOG_U16("port", port))
        goto done;
    }

    rc = NANO_RETCODE_OK;
    
done:
    if (NANO_RETCODE_OK != rc && self->fd > 0)
    {
        IGNORE_RETVAL(close(self->fd));
        self->fd = NANO_FD_INVALID;
    }
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

#define MAX_IOVEC_BUFFERS       8

NANO_PRIVATE
NANO_RetCode
buffer_to_iovec(
    const NANO_MessageBuffer *const msg,
    struct iovec *iov,
    const NANO_usize iov_max,
    NANO_usize *const iov_len,
    NANO_usize *const msg_size)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    const NANO_MessageBuffer *cur_msg = NULL;
    NANO_usize i = 0;
    struct iovec *cur_iov = NULL;
    NANO_usize tot_msg_size = 0;

    NANO_LOG_FN_ENTRY

    NANO_PCOND(iov != NULL)
    NANO_PCOND(iov_max > 0)
    NANO_PCOND(iov_len != NULL)
    NANO_PCOND(msg != NULL)
    NANO_PCOND(NANO_MessageBuffer_data_len(msg) > 0)
    NANO_PCOND(NANO_MessageBuffer_data_ptr(msg) != NULL)
    NANO_PCOND(msg_size != NULL)

    for(i = 0, cur_msg = msg;
            cur_msg != NULL && i < iov_max;
                i++, cur_msg = NANO_MessageBuffer_next(cur_msg))
    {
        cur_iov = &iov[i];
        cur_iov->iov_base = NANO_MessageBuffer_data_ptr(cur_msg);
        cur_iov->iov_len = NANO_MessageBuffer_data_len(cur_msg);
        NANO_PCOND(cur_iov->iov_len > 0 && cur_iov->iov_base != NULL)
        tot_msg_size += cur_iov->iov_len;

    }
    if (cur_msg != NULL)
    {
        rc = NANO_RETCODE_OUT_OF_RESOURCES;
        goto done;
    }

    *iov_len = i;
    *msg_size = tot_msg_size;

    rc = NANO_RETCODE_OK;
    
done:
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

NANO_PRIVATE
NANO_RetCode
iovec_to_buffer(
    struct iovec *iov,
    const NANO_usize iov_len,
    const NANO_usize msg_size,
    NANO_MessageBuffer *const msg,
    NANO_usize *const iov_size_out)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_MessageBuffer *cur_msg = NULL;
    NANO_usize i = 0,
               tot_msg_size = 0;
    struct iovec *cur_iov = NULL;
    NANO_u8 *data = NULL;
    NANO_u16 data_len = 0;

    NANO_LOG_FN_ENTRY

    NANO_PCOND(iov != NULL)
    NANO_PCOND(iov_len > 0)
    NANO_PCOND(msg_size > 0)
    NANO_PCOND(msg != NULL)
    NANO_PCOND(NANO_MessageBuffer_data_len(msg) > 0)
    NANO_PCOND(NANO_MessageBuffer_data_ptr(msg) != NULL)
    NANO_PCOND(iov_size_out != NULL)

    for (i = 0, cur_msg = msg;
            cur_msg != NULL && i < iov_len && tot_msg_size < msg_size;
            i++, cur_msg = NANO_MessageBuffer_next(cur_msg))
    {
        cur_iov = &iov[i];
        data = NANO_MessageBuffer_data_ptr(cur_msg);
        if (cur_iov->iov_base != data)
        {
            NANO_LOG_ERROR("UNEXPECTED iov buffer",
                NANO_LOG_PTR("iov",cur_iov->iov_base)
                NANO_LOG_PTR("data",data))
            NANO_UNREACHABLE_CODE
            goto done;
        }
        NANO_PCOND(cur_iov->iov_len <= NANO_U16_MAX &&
                   NANO_MessageBuffer_data_len(cur_msg) == cur_iov->iov_len)
        
        data_len = (msg_size - tot_msg_size > cur_iov->iov_len)?
                        cur_iov->iov_len : msg_size - tot_msg_size;

        NANO_MessageBuffer_set_data_len(cur_msg, data_len);
        tot_msg_size += data_len;
    }
    if (tot_msg_size > msg_size)
    {
        NANO_LOG_ERROR("UNEXPECTED total size",
                NANO_LOG_USIZE("total_size",tot_msg_size)
                NANO_LOG_USIZE("max_size",msg_size))
        NANO_UNREACHABLE_CODE
        goto done;
    }
    while (cur_msg != NULL)
    {
        NANO_MessageBuffer_set_data_len(cur_msg, 0);
        cur_msg = NANO_MessageBuffer_next(cur_msg);
    }

    *iov_size_out = tot_msg_size;

    rc = NANO_RETCODE_OK;
    
done:
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

NANO_RetCode
NANO_OSAPI_BsdUdpv4Socket_send(
    NANO_OSAPI_Udpv4Socket *const self,
    const NANO_u8 *const dest_addr,
    const NANO_u16 dest_port,
    const NANO_MessageBuffer *const msg)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    ssize_t rci = 0;
    struct msghdr msgh;
    struct iovec iov[MAX_IOVEC_BUFFERS];
    struct sockaddr_in daddr;
    NANO_usize iov_len = 0,
               msg_size = 0;

    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)
    NANO_PCOND(dest_addr != NULL)
    NANO_PCOND(dest_port > 0)
    NANO_PCOND(msg != NULL)

    NANO_OSAPI_Memory_zero(&daddr, sizeof(daddr));
    NANO_OSAPI_Memory_zero(&msgh, sizeof(msgh));
    NANO_OSAPI_Memory_zero(&iov, sizeof(iov));

    NANO_CHECK_RC(
        buffer_to_iovec(msg, iov, MAX_IOVEC_BUFFERS, &iov_len, &msg_size),
        NANO_LOG_ERROR("failed to convert mbuf to iovec", NANO_LOG_RC(rc)));
    NANO_PCOND(msg_size > 0)

    msgh.msg_iov = iov;
    msgh.msg_iovlen = iov_len;
    msgh.msg_name = &daddr;
    msgh.msg_namelen = sizeof(daddr);

    NANO_PCOND(sizeof(daddr.sin_addr.s_addr) == sizeof(NANO_u32))
    NANO_OSAPI_Memory_copy(
        &daddr.sin_addr.s_addr, dest_addr, sizeof(daddr.sin_addr.s_addr));
    daddr.sin_port = NANO_u16_to_be(dest_port);
    daddr.sin_family = AF_INET;

    rci = sendmsg(self->fd, &msgh, 0 /* flags */);
    if (rci < 0 || (NANO_usize)rci != msg_size)
    {
        NANO_LOG_ERROR("failed to send UDP data", 
            NANO_LOG_I32("fd",self->fd)
            NANO_LOG_I32("rc",rci)
            NANO_LOG_I32("errno",errno)
            NANO_LOG_STR("errstr",strerror(errno))
            NANO_LOG_USIZE("msg_size", msg_size))
        goto done;
    }

    NANO_LOG_TRACE("SENT UDP data",
        NANO_LOG_I32("fd",self->fd)
        NANO_LOG_I32("bytes",rci)
        NANO_LOG_PTR("msg",msg))

    rc = NANO_RETCODE_OK;
done:
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

NANO_RetCode
NANO_OSAPI_BsdUdpv4Socket_recv(
    NANO_OSAPI_Udpv4Socket *const self,
    NANO_u8 *const src_addr,
    NANO_u16 *const src_port,
    NANO_MessageBuffer *const msg,
    NANO_usize *const msg_size_out)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    struct sockaddr_in saddr;
    struct msghdr msgh;
    struct iovec iov[MAX_IOVEC_BUFFERS];
    ssize_t rci = 0;
    NANO_usize iov_len = 0,
               iov_size = 0,
               msg_size = 0;
    NANO_MessageBuffer *cur_msg = NULL;
    const NANO_u32 zero = 0;

    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)
    NANO_PCOND(src_addr != NULL)
    NANO_PCOND(src_port != NULL)
    NANO_PCOND(msg != NULL)
    NANO_PCOND(NANO_MessageBuffer_data_len(msg) > 0)
    NANO_PCOND(NANO_MessageBuffer_data_ptr(msg) != NULL)
    NANO_PCOND(msg_size_out != NULL)

    *msg_size_out = 0;

    NANO_OSAPI_Memory_zero(&saddr, sizeof(saddr));
    NANO_OSAPI_Memory_zero(&msgh, sizeof(msgh));
    NANO_OSAPI_Memory_zero(&iov, sizeof(iov));

    NANO_CHECK_RC(
        buffer_to_iovec(msg, iov, MAX_IOVEC_BUFFERS, &iov_len, &msg_size),
        NANO_LOG_ERROR("failed to convert mbuf to iovec", NANO_LOG_RC(rc)));
    NANO_PCOND(msg_size > 0)
    
    msgh.msg_iov = iov;
    msgh.msg_iovlen = iov_len;
    msgh.msg_name = &saddr;
    msgh.msg_namelen = sizeof(saddr);

    NANO_LOG_TRACE_MSG("calling recvmsg()...")

    rci = recvmsg(self->fd, &msgh, 0 /* flags */);
    if (rci == 0 || (rci < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)))
    {
        // NANO_LOG_TRACE_FN("no DATA available", 
        //     NANO_LOG_I32("fd",self->fd)
        //     NANO_LOG_I32("rc",rci)
        //     NANO_LOG_I32("errno",errno))

        /* No data available */
        cur_msg = msg;
        while (cur_msg != NULL)
        {
            NANO_MessageBuffer_set_data_len(cur_msg, 0);
            cur_msg = NANO_MessageBuffer_next(cur_msg);
        }
        NANO_OSAPI_Memory_copy(src_addr, &zero, sizeof(NANO_u32));

#ifdef NANO_HAVE_YIELD
        NANO_OSAPI_Scheduler_yield();
#endif /* NANO_HAVE_YIELD */

        *src_port = 0;
        rc = NANO_RETCODE_OK;
        goto done;
    }
    else if (rci < 0)
    {
        NANO_LOG_ERROR("FAILED to receive UDP data",
            NANO_LOG_I32("rc",rci))
        goto done;
    }
    msg_size = rci;
    NANO_CHECK_RC(
        iovec_to_buffer(iov, iov_len, msg_size, msg, &iov_size),
        NANO_LOG_ERROR("FAILED to convert iovec to mbuf",
            NANO_LOG_RC(rc)));
    
    NANO_PCOND(saddr.sin_family == AF_INET);
    NANO_PCOND(sizeof(NANO_u32) == sizeof(saddr.sin_addr.s_addr))
    NANO_OSAPI_Memory_copy(src_addr, &saddr.sin_addr.s_addr, sizeof(NANO_u32));
    *src_port = NANO_u16_from_be(saddr.sin_port);
    *msg_size_out = iov_size;

    NANO_LOG_TRACE("RECVD UDP data",
        NANO_LOG_I32("fd",self->fd)
        NANO_LOG_BYTES("src_addr", src_addr, sizeof(NANO_u32))
        NANO_LOG_U16("src_port", *src_port)
        NANO_LOG_I32("bytes", msg_size))

    rc = NANO_RETCODE_OK;
done:
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

NANO_RetCode
NANO_OSAPI_BsdUdpv4Socket_close(NANO_OSAPI_Udpv4Socket *const self)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    int rci = 0;

    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)
    
    if (0 != (rci = close(self->fd)))
    {
        NANO_LOG_ERROR("FAILED to close socket", NANO_LOG_I32("rc",rci))
        goto done;
    }

    self->fd = NANO_FD_INVALID;
    
    rc = NANO_RETCODE_OK;
    
done:
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

#endif /* NANO_FEAT_TRANSPORT_PLUGIN_UDPV4 */


#if NANO_FEAT_TRANSPORT_PLUGIN_TCPV4
NANO_RetCode
NANO_OSAPI_BsdTcpv4Socket_open(
    NANO_OSAPI_Tcpv4Socket *const self,
    const NANO_OSAPI_Tcp4SocketProperties *const properties)
{
    return NANO_RETCODE_ERROR;
}

NANO_RetCode
NANO_OSAPI_BsdTcpv4Socket_send(
    NANO_OSAPI_Tcpv4Socket *const self,
    const NANO_MessageBuffer *msg)
{
    return NANO_RETCODE_ERROR;
}

NANO_RetCode
NANO_OSAPI_BsdTcpv4Socket_recv(
    NANO_OSAPI_Tcpv4Socket *const self,
    NANO_u8 *const msg,
    NANO_usize *const msg_len)
{
    return NANO_RETCODE_ERROR;
}

NANO_RetCode
NANO_OSAPI_BsdTcpv4Socket_close(NANO_OSAPI_Tcpv4Socket *const self)
{
    return NANO_RETCODE_ERROR;
}

#if NANO_FEAT_AGENT
NANO_RetCode
NANO_OSAPI_BsdTcpv4Socket_bind(
    NANO_OSAPI_Tcpv4Socket *const self,
    const NANO_Ipv4Addr addr,
    const NANO_u16 port)
{
    return NANO_RETCODE_ERROR;
}

NANO_RetCode
NANO_OSAPI_BsdTcpv4Socket_listen(NANO_OSAPI_Tcpv4Socket *const self)
{
    return NANO_RETCODE_ERROR;
}

NANO_RetCode
NANO_OSAPI_BsdTcpv4Socket_accept(
    NANO_OSAPI_Tcpv4Socket *const self,
    NANO_OSAPI_Tcpv4Socket *const new_conn)
{
    return NANO_RETCODE_ERROR;
}
#endif /* NANO_FEAT_AGENT */

NANO_RetCode
NANO_OSAPI_BsdTcpv4Socket_connect(
    NANO_OSAPI_Tcpv4Socket *const self,
    const NANO_Ipv4Addr addr,
    const NANO_u16 port)
{
    return NANO_RETCODE_ERROR;
}


#endif /* NANO_FEAT_TRANSPORT_PLUGIN_TCPV4 */

#endif /* NANO_HAVE_SOCKET */
