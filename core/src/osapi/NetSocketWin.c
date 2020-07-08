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

#ifdef NANO_HAVE_SOCKET_WIN

#if NANO_FEAT_TRANSPORT_IPV4

#include <winsock2.h>
#include <iphlpapi.h>

#if NANO_FEAT_TRANSPORT_AUTOCFG

#if !NANO_FEAT_AGENT
#define "NANO_FEAT_TRANSPORT_AUTOCFG supported only with NANO_FEAT_AGENT"
#endif

#if NANO_FEAT_AGENT
#define MAX_ADDRS           16
#endif /* NANO_FEAT_AGENT */

typedef DWORD(WINAPI * GETADAPTERSADDRESSES_FUNC)
    (ULONG Family,
     DWORD Flags,
     PVOID Reserved,
     PIP_ADAPTER_ADDRESSES pAdapterAddresses, PULONG pOutBufLen);

NANO_RetCode
NANO_OSAPI_WinIpv4Addr_list(
    NANO_Ipv4Addr *const addrs,
    NANO_usize *const addrs_len)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_i32 interface_buffer_size = 0;
    HINSTANCE hinst_iphlpapiLib;
    GETADAPTERSADDRESSES_FUNC my_getAdaptersAddresses;
    IP_ADAPTER_ADDRESSES *interface_buffer = NULL,
                         *intf = NULL;
    char address_str[INET6_ADDRSTRLEN];
    ULONG retval = 0;
    NANO_usize j = 0,
               addrs_max = *addrs_len;

    NANO_LOG_FN_ENTRY
    
    hinst_iphlpapiLib = LoadLibrary("iphlpapi");
    if (hinst_iphlpapiLib == NULL)
    {
        NANO_LOG_ERROR_MSG("FAILED to load library iphlpapi")
        goto done;
    }

    my_getAdaptersAddresses = (GETADAPTERSADDRESSES_FUNC)
            GetProcAddress(hinst_iphlpapiLib, "GetAdaptersAddresses");
    
    if (my_getAdaptersAddresses == NULL)
    {
        NANO_LOG_ERROR_MSG("FAILED to load function iphlpapi::GetAdaptersAddresses")
        goto done;
    }

    interface_buffer_size = 0;
    retval = my_getAdaptersAddresses(AF_INET, 0, NULL,
                interface_buffer,&interface_buffer_size);
    if (retval != ERROR_BUFFER_OVERFLOW)
    {
        NANO_LOG_ERROR("iphlpapi::GetAdaptersAddresses UNEXPECTE RC",
            NANO_LOG_I32("retval", retval))
        goto done;
    }

    RTIOsapiHeap_allocateBufferAligned(
            &interface_buffer,
            interface_buffer_size,
            RTI_OSAPI_ALIGNMENT_DEFAULT);
    if (interface_buffer == NULL)
    {
        goto done;
    }

    retval = my_getAdaptersAddresses(AF_INET, 0, NULL,
                interface_buffer,&interface_buffer_size);
    if (retval != 0 || interface_buffer_size < 0)
    {
        NANO_LOG_ERROR("iphlpapi::GetAdaptersAddresses FAILED",
            NANO_LOG_I32("retval", retval)
            NANO_LOG_I32("intf_buff_size", interface_buffer_size))
        goto done;
    }

    if (interface_buffer_size > 0)
    {
        for (intf = interface_buffer;intf != NULL;intf = intf->Next)
        {
            IN_ADDR in_addr;

            NANO_OSAPI_Memory_zero(&in_addr,sizeof(in_addr));

            if (intf->IfType == IF_TYPE_TUNNEL ||
                (intf->OperStatus != IfOperStatusUp && intf->OperStatus != IfOperStatusDormant) ||
                intf->FirstUnicastAddress == NULL)
            {
                /* Ignore interface */
                continue;
            }

            if (getnameinfo(intf->FirstUnicastAddress->Address.lpSockaddr,
                            intf->FirstUnicastAddress->Address.iSockaddrLength,
                            address_str, INET6_ADDRSTRLEN, NULL, 0,
                            NI_NUMERICHOST) != 0)
            {
                NANO_LOG_ERROR_MSG("getnameinfo FAILED")
                goto done;
            }

            if (InetPton(AF_INET, address_str, &in_addr) != 1)
            {
                NANO_LOG_ERROR("InetPton FAILED",
                    NANO_LOG_STR("address_str",address_str))
                goto done;
            }

            addrs[j] = in_addr.s_addr;
            j += 1;
        }
    }

    *addrs_len = j;
    

    rc = NANO_RETCODE_OK;
    
done:
    if (interface_buffer != NULL)
    {
        RTIOsapiHeap_freeBuffer(interface_buffer);
    }

    if (hinst_iphlpapiLib != NULL)
    {
        FreeLibrary(hinst_iphlpapiLib);
    }
    
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;

}
#endif /* NANO_FEAT_TRANSPORT_AUTOCFG */

#if NANO_FEAT_TRANSPORT_STRING
NANO_RetCode
NANO_OSAPI_WinIpv4Addr_from_str(
    NANO_Ipv4Addr *const self,
    const char *const ip_str)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    int rci = 0;
    struct sockaddr_in saddr;

    NANO_LOG_FN_ENTRY

    *self = 0;

    
    rc = NANO_RETCODE_OK;
    
done:
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}
#endif /* NANO_FEAT_TRANSPORT_STRING */

#endif /* NANO_FEAT_TRANSPORT_IPV4 */

#if NANO_FEAT_TRANSPORT_PLUGIN_UDPV4

NANO_RetCode
NANO_OSAPI_WinUdpv4Socket_open(
    NANO_OSAPI_Udpv4Socket *const self,
    const NANO_u8 *const addr,
    const NANO_u16 port,
    const NANO_OSAPI_Udpv4SocketProperties *const properties)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    int rci = 0;
    struct sockaddr_in saddr;
    WSADATA wd;
#if NANO_FEAT_AGENT
    NANO_Ipv4Addr if_addrs[MAX_ADDRS] = { 0 };
    NANO_usize if_addrs_len = MAX_ADDRS;
#endif /* NANO_FEAT_AGENT */

    NANO_LOG_FN_ENTRY
    
    NANO_PCOND(self != NULL)
    NANO_PCOND(port > 0)
    NANO_PCOND(properties != NULL)

    if (!(rci = WSAStartup(0x0202, &wd)) ||  /* 2.2 success */
        !(rci = WSAStartup(0x0002, &wd)))
    {                           /* 2.0 success */

    }
    else
    {
        goto done;
    }

    /* Check version returned must not be older than 2.0 */
    if (HIBYTE(wd.wVersion) != 2)
    {
        // UDP_LOG_WINSOCK_INCOMPATIBLE(OSAPI_LOGKIND_ERROR)
        goto done;
    }

    
    self->fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (self->fd < 0)
    {
        NANO_LOG_ERROR("FAILED to create UDP socket",
            NANO_LOG_I32("fd",self->fd))
        goto done;
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
            NANO_OSAPI_WinIpv4Addr_list(if_addrs, &if_addrs_len),
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

    
    
    if (properties->timeout_us > 0) 
    {
        int timeout_msec = 1 /* round up to at least 1ms */;

        if (properties->timeout_us > (1000 * timeout_msec))
        {
            int timeout_sec = 0;
            timeout_sec = properties->timeout_us / 1000000;
            timeout_msec = 
                (timeout_sec * 1000) + 
                ((properties->timeout_us - (timeout_sec * 1000000)) / 1000);
        }
        
        NANO_LOG_INFO("SOCKET TIMEOUT",
            NANO_LOG_U32("timeout.usec",properties->timeout_us)
            NANO_LOG_U32("timeout.msec",timeout_msec))
        
        if ((rci =
                setsockopt(
                    self->fd,
                    SOL_SOCKET,
                    SO_RCVTIMEO,
                    (const char *)&timeout_msec,
                    sizeof(timeout_msec))) < 0)
        {
            NANO_LOG_ERROR("FAILED to set receive timeout",
                NANO_LOG_I32("rc",rci)
                NANO_LOG_I32("wsa",WSAGetLastError()))
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
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}


#define MAX_WSABUF_BUFFERS       8

NANO_PRIVATE
NANO_RetCode
buffer_to_wsabuf(
    const NANO_MessageBuffer *const msg,
    WSABUF *const wsabuf,
    const NANO_usize wsabuf_max,
    NANO_usize *const wsabuf_len,
    NANO_usize *const msg_size_out)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    const NANO_MessageBuffer *cur_msg = NULL;
    NANO_usize i = 0;
    WSABUF *cur_wsabuf = NULL;
    NANO_usize msg_size = 0;

    NANO_LOG_FN_ENTRY

    NANO_PCOND(wsabuf != NULL)
    NANO_PCOND(wsabuf_max > 0)
    NANO_PCOND(wsabuf_len != NULL)
    NANO_PCOND(msg != NULL)
    NANO_PCOND(NANO_MessageBuffer_data_len(msg) > 0)
    NANO_PCOND(NANO_MessageBuffer_data_ptr(msg) != NULL)

    for(i = 0, cur_msg = msg;
            cur_msg != NULL && i < wsabuf_max;
                i++, cur_msg = NANO_MessageBuffer_next(cur_msg))
    {
        cur_wsabuf = &wsabuf[i];
        cur_wsabuf->buf = NANO_MessageBuffer_data_ptr(cur_msg);
        cur_wsabuf->len = NANO_MessageBuffer_data_len(cur_msg);
        NANO_PCOND(cur_wsabuf->len > 0 && cur_wsabuf->buf != NULL)
        msg_size += cur_wsabuf->len;
    }
    if (cur_msg != NULL)
    {
        rc = NANO_RETCODE_OUT_OF_RESOURCES;
        goto done;
    }

    *wsabuf_len = i;
    if (msg_size_out != NULL)
    {
        *msg_size_out = msg_size;
    }

    rc = NANO_RETCODE_OK;
    
done:
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

NANO_PRIVATE
NANO_RetCode
wsabuf_to_buffer(
    WSABUF *const wsabuf,
    const NANO_usize wsabuf_len,
    const NANO_usize msg_size,
    NANO_MessageBuffer *const msg,
    NANO_usize *const wsabuf_size_out)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_MessageBuffer *cur_msg = NULL;
    NANO_usize i = 0,
               tot_msg_size = 0;
    WSABUF *cur_wsabuf = NULL;
    NANO_u8 *data = NULL;
    NANO_u16 data_len = 0;

    NANO_LOG_FN_ENTRY

    NANO_PCOND(wsabuf != NULL)
    NANO_PCOND(wsabuf_len > 0)
    NANO_PCOND(msg_size > 0)
    NANO_PCOND(msg != NULL)
    NANO_PCOND(NANO_MessageBuffer_data_len(msg) > 0)
    NANO_PCOND(NANO_MessageBuffer_data_ptr(msg) != NULL)

    for (i = 0, cur_msg = msg;
            cur_msg != NULL && i < wsabuf_len && tot_msg_size < msg_size;
            i++, cur_msg = NANO_MessageBuffer_next(cur_msg))
    {
        cur_wsabuf = &wsabuf[i];
        data = NANO_MessageBuffer_data_ptr(cur_msg);
        if (cur_wsabuf->buf != data)
        {
            NANO_LOG_ERROR("UNEXPECTED WSA buffer",
                NANO_LOG_PTR("wsabuf",cur_wsabuf->buf)
                NANO_LOG_PTR("data",data))
            NANO_UNREACHABLE_CODE
            goto done;
        }
        NANO_PCOND(cur_wsabuf->len <= NANO_U16_MAX &&
                   NANO_MessageBuffer_data_len(cur_msg) == cur_wsabuf->len)
        
        data_len = (msg_size - tot_msg_size > cur_wsabuf->len)?
                        cur_wsabuf->len : msg_size - tot_msg_size;

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

    *wsabuf_size_out = tot_msg_size;

    rc = NANO_RETCODE_OK;
    
done:
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

NANO_RetCode
NANO_OSAPI_WinUdpv4Socket_send(
    NANO_OSAPI_Udpv4Socket *const self,
    const NANO_u8 *const dest_addr,
    const NANO_u16 dest_port,
    const NANO_MessageBuffer *const msg)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    DWORD flags = 0;
    int rci = 0;
    WSABUF mbuf[MAX_WSABUF_BUFFERS];
    NANO_usize mbuf_len = 0,
               msg_size = 0;
    DWORD sent_size = 0;
    struct sockaddr_in ip_dst;
    int ip_len = sizeof(struct sockaddr); 

    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)
    NANO_PCOND(dest_addr != NULL)
    NANO_PCOND(dest_port > 0)
    NANO_PCOND(msg != NULL)
    
    NANO_OSAPI_Memory_zero(&ip_dst, sizeof(ip_dst));
    NANO_OSAPI_Memory_zero(&mbuf, sizeof(mbuf));

    NANO_CHECK_RC(
        buffer_to_wsabuf(msg, mbuf, MAX_WSABUF_BUFFERS, &mbuf_len, &msg_size),
        NANO_LOG_ERROR("failed to convert mbuf to wsabuf", NANO_LOG_RC(rc)));
    NANO_PCOND(msg_size > 0)

    NANO_PCOND(sizeof(ip_dst.sin_addr.s_addr) == sizeof(NANO_u32))
    NANO_OSAPI_Memory_copy(
        &ip_dst.sin_addr.s_addr, dest_addr, sizeof(ip_dst.sin_addr.s_addr));
    ip_dst.sin_port = NANO_u16_to_be(dest_port);
    ip_dst.sin_family = AF_INET;

    rci = WSASendTo((SOCKET)self->fd,
            (WSABUF*)mbuf, mbuf_len,
            &sent_size, flags,
            (struct sockaddr *)&ip_dst, ip_len, NULL, NULL);
    if (rci != 0 || sent_size != msg_size)
    {
        NANO_LOG_ERROR("WSARecvFrom failed",
            NANO_LOG_I32("rc",rci)
            NANO_LOG_I32("msg_size", msg_size)
            NANO_LOG_I32("sent_size", sent_size))
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
NANO_OSAPI_WinUdpv4Socket_recv(
    NANO_OSAPI_Udpv4Socket *const self,
    NANO_u8 *const src_addr,
    NANO_u16 *const src_port,
    NANO_MessageBuffer *const msg,
    NANO_usize *const msg_size_out)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    int flags = 0,
        rci = 0;
    WSABUF mbuf[MAX_WSABUF_BUFFERS];
    NANO_usize mbuf_len = 0,
               recv_size = 0;
    DWORD msg_size = 0;
    struct sockaddr_in ip_src;
    DWORD ip_len = sizeof(struct sockaddr); 

    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)
    NANO_PCOND(src_addr != NULL)
    NANO_PCOND(src_port != NULL)
    NANO_PCOND(msg != NULL)
    NANO_PCOND(NANO_MessageBuffer_data_len(msg) > 0)
    NANO_PCOND(NANO_MessageBuffer_data_ptr(msg) != NULL)
    NANO_PCOND(msg_size_out != NULL)

    *msg_size_out = 0;

    NANO_OSAPI_Memory_zero(&ip_src, sizeof(ip_src));
    NANO_OSAPI_Memory_zero(&mbuf, sizeof(mbuf));

    NANO_CHECK_RC(
        buffer_to_wsabuf(msg, mbuf, MAX_WSABUF_BUFFERS, &mbuf_len, NULL),
        NANO_LOG_ERROR("failed to convert mbuf to wsabuf", NANO_LOG_RC(rc)));

    rci = WSARecvFrom((SOCKET)self->fd,
            (WSABUF*)mbuf, mbuf_len,
            &msg_size, &flags,
            (struct sockaddr *)&ip_src, &ip_len, NULL, NULL);
    if ((rci != 0 && WSAGetLastError() != WSAETIMEDOUT) || msg_size < 0)
    {
        NANO_LOG_ERROR("WSARecvFrom failed",
            NANO_LOG_I32("rc",rci)
            NANO_LOG_I32("msg_size", msg_size)
            NANO_LOG_I32("wsa",WSAGetLastError()))
        goto done;
    }
    if (msg_size == 0)
    {
        rc = NANO_RETCODE_OK;
        goto done;
    }

    NANO_CHECK_RC(
        wsabuf_to_buffer(mbuf, mbuf_len, msg_size, msg, &recv_size),
        NANO_LOG_ERROR("FAILED to convert wsabuf to mbuf",
            NANO_LOG_RC(rc)));

    NANO_PCOND(ip_src.sin_family == AF_INET);
    NANO_PCOND(sizeof(NANO_u32) == sizeof(ip_src.sin_addr.s_addr))
    NANO_OSAPI_Memory_copy(src_addr, &ip_src.sin_addr.s_addr, sizeof(NANO_u32));
    *src_port = NANO_u16_from_be(ip_src.sin_port);
    *msg_size_out = (NANO_usize)msg_size;

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
NANO_OSAPI_WinUdpv4Socket_close(NANO_OSAPI_Udpv4Socket *const self)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    int rci = 0;

    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)
    
    
    
    rc = NANO_RETCODE_OK;
    
done:
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

#endif /* NANO_FEAT_TRANSPORT_PLUGIN_UDPV4 */


#endif /* NANO_HAVE_SOCKET_WIN */
