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

#ifdef NANO_HAVE_NET_LWIP

#include <lwip/ip_addr.h>

#include <lwip/pbuf.h>

#if NANO_FEAT_TRANSPORT_IPV4 || \
    NANO_FEAT_TRANSPORT_IPV6

#ifdef IP_ADDR4
    #define set_ip_addr(dst_, src_) \
        IP_ADDR4(&(dst_), (src_)[0], (src_)[1], (src_)[2], (src_)[3])
#elif defined(IP4_ADDR4)
    #define set_ip_addr(dst_, src_) \
        IP4_ADDR4(&(dst_), (src_)[0], (src_)[1], (src_)[2], (src_)[3])
#else
    #define set_ip_addr(dst_, src_) \
        NANO_OSAPI_Memory_copy(&(dst_).addr, (src_), 4)
#endif

#if NANO_FEAT_TRANSPORT_PLUGIN_UDPV4

NANO_PRIVATE
void
NANO_OSAPI_LwipUdpSocket_on_recv(
    void *arg,
    struct udp_pcb *pcb,
    struct pbuf *p,
    const ip_addr_t *addr,
    u16_t port);

NANO_RetCode
NANO_OSAPI_LwipUdpSocket_open(
    NANO_OSAPI_Udpv4Socket *const self,
    const NANO_u8 *const addr,
    const NANO_u16 port,
    const NANO_OSAPI_Udpv4SocketProperties *const properties)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    ip_addr_t bind_addr;
    int e = 0;
    NANO_usize i = 0;

    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)
    NANO_PCOND(addr != NULL)
    NANO_PCOND(port > 0)

    self->pcb = udp_new();
    if (self->pcb == NULL)
    {
        NANO_LOG_ERROR_MSG("FAILED to create UDP pcb")
        goto done;
    }
    
    set_ip_addr(bind_addr, addr);

    if (0 != (e = udp_bind(self->pcb, &bind_addr, port)))
    {
        NANO_LOG_ERROR("FAILED to bind UDP pcb",
            NANO_LOG_I32("err",e)
            NANO_LOG_BYTES("addr",addr,sizeof(NANO_u32))
            NANO_LOG_U16("port",port))
        goto done;
    }

    for (i = 0; i < NANO_LIMIT_LWIP_PBUF_OUT_MAX_LENGTH; i++)
    {
        NANO_LIMIT_LWIP_PBUF_OUT_MAX_LENGTH;
        /* Create a pbuf for sending messages */
        self->pbuf_out[i] = pbuf_alloc(PBUF_TRANSPORT, 0, PBUF_REF);
        if (self->pbuf_out[i] == NULL)
        {
            NANO_LOG_ERROR("FAILED to allocate output pbuf",
                NANO_LOG_USIZE("i",i+1)
                NANO_LOG_USIZE("max",NANO_LIMIT_LWIP_PBUF_OUT_MAX_LENGTH))
            goto done;
        }
    }

    self->recv_buffer = properties->recv_buffer;

    self->recv_buffer_size = properties->recv_buffer_size;

    NANO_MessageBuffer_set_external_data(
            &self->recv_msg, self->recv_buffer, 0);
    
    udp_recv(self->pcb, NANO_OSAPI_LwipUdpSocket_on_recv, self);

    rc = NANO_RETCODE_OK;
    
done:
    if (NANO_RETCODE_OK != rc)
    {
        if (self->pcb != NULL)
        {
            udp_remove(self->pcb);
            self->pcb = NULL;
        }
    }
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

NANO_RetCode
NANO_OSAPI_LwipUdpSocket_send(
    NANO_OSAPI_Udpv4Socket *const self,
    const NANO_u8 *const dest_addr,
    const NANO_u16 dest_port,
    const NANO_MessageBuffer *msg)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    ip_addr_t dst_addr;
    int e;
    NANO_MessageBuffer *nxt = NULL;
    NANO_usize i = 0;

    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)
    NANO_PCOND(dest_addr != NULL)
    NANO_PCOND(dest_port > 0)
    NANO_PCOND(msg != NULL)
    NANO_PCOND(NANO_MessageBuffer_data_ptr(msg) != NULL)

    NANO_PCOND(self->pcb != NULL)
    
    nxt = msg;
    while (nxt != NULL)
    {
        NANO_PCOND(self->pbuf_out[i] != NULL)

        self->pbuf_out[i]->payload = NANO_MessageBuffer_data_ptr(nxt);
        self->pbuf_out[i]->len = NANO_MessageBuffer_data_len(nxt);
        if (i == 0)
        {
            NANO_MessageBuffer_data_len_msg(nxt, &self->pbuf_out[i]->tot_len);
        }

        nxt = NANO_MessageBuffer_next(nxt);
        i += 1;

        if (i >= NANO_LIMIT_LWIP_PBUF_OUT_MAX_LENGTH && nxt != NULL)
        {
            NANO_LOG_ERROR("TOO MANY mbufs",
                NANO_LOG_USIZE("max", NANO_LIMIT_LWIP_PBUF_OUT_MAX_LENGTH))
            goto done;
        }
        else if (nxt != NULL)
        {
            self->pbuf_out[i - 1]->next = self->pbuf_out[i];
        }
        else
        {
            self->pbuf_out[i - 1]->next = NULL;
        }
    }

    NANO_PCOND(self->pbuf_out->tot_len == self->pbuf_out->len)

    set_ip_addr(dst_addr, dest_addr);

    NANO_LOG_DEBUG("UDP sendto",
        NANO_LOG_USIZE("size", self->pbuf_out[0]->tot_len)
        NANO_LOG_BYTES("dst_addr", dest_addr, sizeof(NANO_Ipv4Addr))
        NANO_LOG_U16("dst_port", dest_port))

    e = udp_sendto(self->pcb, self->pbuf_out[0], &dst_addr, dest_port);
    if (e < 0)
    {
        NANO_LOG_ERROR("FAILED to send UDP message",
            NANO_LOG_I32("err",e))
        goto done;
    }

#ifdef NANO_HAVE_YIELD
    NANO_OSAPI_Scheduler_yield();
#endif /* NANO_HAVE_YIELD */

    rc = NANO_RETCODE_OK;
    
done:
    if (NANO_RETCODE_OK != rc)
    {
        
    }
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}


NANO_PRIVATE
NANO_RetCode
NANO_OSAPI_LwipUdpSocket_pbuf_to_mbuf(
    struct pbuf *const p,
    NANO_MessageBuffer *const mbuf,
    const NANO_usize mbuf_size)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    struct pbuf *nxt = p;
    NANO_usize pbuf_len = 0,
               remaining = 0,
               copied_len = 0;
    NANO_u8 *ptr = NULL;
    
    NANO_LOG_FN_ENTRY
    
    remaining = mbuf_size;

    if (p->tot_len > remaining)
    {
        NANO_LOG_WARNING("received message TOO LARGE",
            NANO_LOG_USIZE("pbuf_len",p->tot_len)
            NANO_LOG_USIZE("remaining",remaining)
            NANO_LOG_USIZE("max_len",mbuf_size))
        goto done;
    }

    ptr = NANO_MessageBuffer_data_ptr(mbuf);

    /* TODO(asorbini) Adjust the end condition of this loop to conform with
       lwIP's documentation:
       "The last pbuf of a packet has a ->tot_len field that equals the ->len
        field. It can be found by traversing the list. If the last pbuf of a
        packet has a ->next field other than NULL, more packets are on the
        queue.

        Therefore, looping through a pbuf of a single packet, has an loop end
        condition (tot_len == p->len), NOT (next == NULL)."

        The condition is fine for now, because, also according to the docs:
        "CURRENTLY, PACKET QUEUES ARE NOT SUPPORTED!!!". */
    while (nxt != NULL && remaining > 0)
    {
        if (nxt->len == nxt->tot_len)
        {
            /* The last pbuf's len is equal to the total length of the message,
               so we must compute the length of the buffer to copy as the
               difference with what we've copied so far. */
            pbuf_len = nxt->len - (mbuf_size - remaining);
        }
        else
        {
            pbuf_len = nxt->len;
        }

        if (pbuf_len > remaining)
        {
            NANO_LOG_ERROR("out of socket resources",
                NANO_LOG_USIZE("needed", pbuf_len)
                NANO_LOG_USIZE("remaining", remaining)
                NANO_LOG_USIZE("max", mbuf_size))
            goto done;
        }
        NANO_OSAPI_Memory_copy(ptr, nxt->payload, pbuf_len);
        ptr += pbuf_len;
        remaining -= pbuf_len;
        nxt = nxt->next;
    }

    NANO_MessageBuffer_set_data_len(mbuf, mbuf_size - remaining);

    rc = NANO_RETCODE_OK;
    
done:

    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

NANO_RetCode
NANO_OSAPI_LwipUdpSocket_recv(
    NANO_OSAPI_Udpv4Socket *const self,
    NANO_u8 *const src_addr,
    NANO_u16 *const src_port,
    NANO_MessageBuffer *const msg,
    NANO_usize *const msg_size_out)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_usize read_len = 0,
               msg_max = 0,
               copy_len = 0;
    NANO_u8 *msg_ptr = NULL,
            *read_ptr = NULL;
    NANO_bool no_ints = NANO_BOOL_FALSE;
    NANO_usize msg_size = 0;
    
    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)
    NANO_PCOND(src_addr != NULL)
    NANO_PCOND(src_port != NULL)
    NANO_PCOND(msg != NULL)
    NANO_PCOND(msg_size_out != NULL)

    *msg_size_out = 0;

#ifdef NANO_HAVE_INTERRUPTS
    NANO_OSAPI_System_disable_interrupts();
#else
    udp_recv(self->pcb, NULL, NULL);
#endif

    if (self->pbuf_in != NULL)
    {
        NANO_CHECK_RC(
            NANO_OSAPI_LwipUdpSocket_pbuf_to_mbuf(
                self->pbuf_in, msg, NANO_MessageBuffer_data_len(msg)),
            NANO_LOG_ERROR_MSG("FAILED to convert pbuf to mbuf"));
        NANO_OSAPI_Memory_copy(
            src_addr, self->read_addr, sizeof(self->read_addr));
        *src_port = self->read_port;
        msg_size = NANO_MessageBuffer_data_len(msg);
        *msg_size_out = msg_size;
        pbuf_free(self->pbuf_in);
        self->pbuf_in = NULL;

#ifdef NANO_HAVE_INTERRUPTS
        NANO_OSAPI_System_enable_interrupts();
#else
        udp_recv(self->pcb, NANO_OSAPI_LwipUdpSocket_on_recv, self);
#endif
    }
    else
    {
#ifdef NANO_HAVE_INTERRUPTS
        NANO_OSAPI_System_enable_interrupts();
#else
        udp_recv(self->pcb, NANO_OSAPI_LwipUdpSocket_on_recv, self);
#endif
        NANO_OSAPI_Scheduler_yield();
    }


    rc = NANO_RETCODE_OK;
    
done:
    if (NANO_RETCODE_OK != rc)
    {
        
    }
    
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}


NANO_PRIVATE
void
NANO_OSAPI_LwipUdpSocket_on_recv(
    void *arg,
    struct udp_pcb *pcb,
    struct pbuf *p,
    const ip_addr_t *addr,
    u16_t port)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_OSAPI_Udpv4Socket *const self = (NANO_OSAPI_Udpv4Socket*)arg;
    NANO_u32 src_addr = 0;
    NANO_u16 src_port = 0;

    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)
    NANO_PCOND(pcb != NULL)
    NANO_PCOND(p != NULL)
    NANO_PCOND(addr != NULL)
    NANO_PCOND(port > 0)
    NANO_PCOND(self->listener != NULL)
    NANO_PCOND(self->recv_buffer != NULL)

    src_addr = ip4_addr_get_u32(addr);
    if (src_addr == 0)
    {
        goto done;
    }
    src_port = port;

    if (self->pbuf_in != NULL)
    {
        pbuf_free(self->pbuf_in);
    }
    self->read_addr = src_addr;
    self->read_port = src_port;
    self->pbuf_in = p;

    rc = NANO_RETCODE_OK;
done:
    if (NANO_RETCODE_OK != rc)
    {

    }

    NANO_LOG_FN_EXIT
}


NANO_RetCode
NANO_OSAPI_LwipUdpSocket_close(NANO_OSAPI_Udpv4Socket *const self)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_usize i = 0;

    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)

    NANO_PCOND(self->pcb != NULL)
    NANO_PCOND(self->pbuf_out != NULL)

    udp_remove(self->pcb);
    self->pcb = NULL;

    for (i = 0; i < NANO_LIMIT_LWIP_PBUF_OUT_MAX_LENGTH; i++)
    {
        pbuf_free(self->pbuf_out[i]);
        self->pbuf_out[i] = NULL;
    }
    
    rc = NANO_RETCODE_OK;
    
done:
    if (NANO_RETCODE_OK != rc)
    {
        
    }
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

#endif /* NANO_FEAT_TRANSPORT_PLUGIN_UDPV4 */



#endif /* NANO_FEAT_TRANSPORT_IPV4 || \
            NANO_FEAT_TRANSPORT_IPV6*/

#endif /* NANO_HAVE_LWIP */