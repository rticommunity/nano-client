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

NANO_RetCode
NANO_XRCE_ClientTransport_queue_send(
    NANO_XRCE_ClientTransport *const self,
    NANO_MessageBuffer *const msg)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)
    NANO_PCOND(msg != NULL)

    if (self->send_queue == NULL)
    {
        self->send_queue = msg;
    }
    else
    {
        NANO_MessageBuffer *nxt = NULL, *prv = NULL;
        nxt = self->send_queue;
        while (nxt != NULL)
        {
            prv = nxt;
            nxt = NANO_MessageBuffer_next_msg(nxt);
        }
        NANO_MessageBuffer_append_msg(prv, msg);
    }
    
    rc = NANO_RETCODE_OK;

    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

void
NANO_XRCE_ClientTransport_send_complete(
    NANO_XRCE_ClientTransport *const self,
    NANO_usize i,
    NANO_MessageBuffer *msg,
    NANO_RetCode rc)
{
    
    NANO_LOG_FN_ENTRY
    
    UNUSED_ARG(i);

    NANO_PCOND(self != NULL)
    NANO_PCOND(msg != NULL);

    {
        NANO_MessageBuffer *nxt = self->send_queue, *prv = NULL;
        while (nxt != NULL && nxt != msg)
        {
            prv = NULL;
            nxt = NANO_MessageBuffer_next(nxt);
        }
        
        nxt = NANO_MessageBuffer_next_msg(msg);

        if (prv != NULL)
        {
            NANO_MessageBuffer_unlink_msg(prv);
            if (nxt != NULL)
            {
                NANO_MessageBuffer_append_msg(prv, nxt);
            }
        }
        else
        {
            // NANO_PCOND(self->send_queue == msg)
            if (self->send_queue != msg)
            {
                NANO_LOG_WARNING("SOMETHING IS WRONG",
                    NANO_LOG_MBUF("DISMISS", msg)
                    NANO_LOG_PTR("send_queue", self->send_queue)
                    NANO_LOG_PTR("nxt", nxt)
                    if (self->send_queue != NULL)
                    {
                        NANO_LOG_MBUF("QUEUE", self->send_queue)
                    }
                    if (nxt != NULL)
                    {
                        NANO_LOG_MBUF("NEXT", nxt)
                    }
                    )
            }
            self->send_queue = nxt;
        }
    }

    
    /* return message upstream */
    NANO_XRCE_ClientTransportListener_on_message_sent(
        &self->listener, self, msg, rc);

    NANO_LOG_FN_EXIT
}

#if NANO_FEAT_TYPED_SEQUENCE

NANO_RetCode
NANO_XRCE_TransportLocatorSeq_deserialize_element(
    NANO_SequenceIterator *const self,
    const NANO_Sequence *const seq,
    void *const next_el)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_XRCE_TransportLocator *loc = (NANO_XRCE_TransportLocator*)next_el;
    
    NANO_PCOND(self != NULL && seq != NULL && next_el != NULL)
    
    /* Deserialize locator kind */
    if (!NANO_SequenceIterator_check_capacity(
            self, sizeof(NANO_XRCE_TransportLocatorFormat)))
    {
        goto done;
    }

    loc->format = *((NANO_XRCE_TransportLocatorFormat*)
                        NANO_SequenceIterator_head(self));

    NANO_SequenceIterator_consume(
        self, sizeof(NANO_XRCE_TransportLocatorFormat));

    switch (loc->format)
    {
    case NANO_XRCE_ADDRESS_FORMAT_LARGE:
    {
        if (!NANO_SequenceIterator_check_capacity(
                self, sizeof(loc->value.large.address)))
        {
            goto done;
        }
        NANO_OSAPI_Memory_copy(
            loc->value.large.address,
            NANO_SequenceIterator_head(self),
            sizeof(loc->value.large.address));
        NANO_SequenceIterator_consume(self, sizeof(loc->value.large.address));

        if (!NANO_SequenceIterator_check_capacity_align(
                self, sizeof(loc->value.large.port), sizeof(NANO_u32)))
        {
            goto done;
        }

        NANO_u32_deserialize_aligned(
            &loc->value.large.port,
            (NANO_u32*)NANO_SequenceIterator_head(self),
            NANO_Sequence_flags(seq, NANO_SEQUENCEFLAGS_ENDIANNESS));

        NANO_SequenceIterator_consume(self, sizeof(loc->value.large.port));
        break;
    }
    case NANO_XRCE_ADDRESS_FORMAT_MEDIUM:
    {
        if (!NANO_SequenceIterator_check_capacity(
                self, sizeof(loc->value.medium.address)))
        {
            goto done;
        }
        NANO_OSAPI_Memory_copy(
            loc->value.medium.address,
            NANO_SequenceIterator_head(self),
            sizeof(loc->value.medium.address));
        NANO_SequenceIterator_consume(self, sizeof(loc->value.medium.address));
        if (!NANO_SequenceIterator_check_capacity_align(
                self, sizeof(loc->value.medium.port), NANO_u16_alignment))
        {
            goto done;
        }
        NANO_u16_deserialize_aligned(
            &loc->value.medium.port,
            (NANO_u16*)NANO_SequenceIterator_head(self),
            NANO_Sequence_flags(seq, NANO_SEQUENCEFLAGS_ENDIANNESS));
        NANO_SequenceIterator_consume(self, sizeof(loc->value.medium.port));
        break;
    }
    case NANO_XRCE_ADDRESS_FORMAT_SMALL:
    {
        if (!NANO_SequenceIterator_check_capacity(
                self, sizeof(loc->value.small.address)))
        {
            goto done;
        }
        NANO_OSAPI_Memory_copy(
            loc->value.small.address,
            NANO_SequenceIterator_head(self),
            sizeof(loc->value.small.address));
        NANO_SequenceIterator_consume(self, sizeof(loc->value.small.address));
        if (!NANO_SequenceIterator_check_capacity(
                self, sizeof(loc->value.small.port)))
        {
            goto done;
        }
        loc->value.small.port = *NANO_SequenceIterator_head(self);
        NANO_SequenceIterator_consume(self, sizeof(loc->value.small.port));
        break;
    }
    case NANO_XRCE_ADDRESS_FORMAT_STRING:
    {
        NANO_u32 str_len = 0;
        /* Deserialize string length */
        if (!NANO_SequenceIterator_check_capacity_align(
                self, sizeof(NANO_u32), NANO_u32_alignment))
        {
            goto done;
        }
        NANO_u32_deserialize_aligned(
            &str_len,
            (NANO_u32*)NANO_SequenceIterator_head(self),
            NANO_Sequence_flags(seq, NANO_SEQUENCEFLAGS_ENDIANNESS));
        NANO_SequenceIterator_consume(self, sizeof(NANO_u32));

        if (str_len == 0 ||
            !NANO_SequenceIterator_check_capacity(self, str_len) ||
            NANO_SequenceIterator_head(self)[str_len] != '\0')
        {
            /* Invalid string (length must include terminator), or 
               string bigger than remaining buffer, or string not properly
               terminated */
            goto done;
        }

        loc->value.str.value = (char*) NANO_SequenceIterator_head(self);
        NANO_SequenceIterator_consume(self, str_len);

        break;
    }
    default:
    {
        /* unknown address format */
        goto done;
    }
    }

    rc = NANO_RETCODE_OK;
    
done:
    return rc;
}
#endif /* NANO_FEAT_TYPED_SEQUENCE */

#if NANO_FEAT_AGENT

NANO_i8
NANO_XRCE_TransportLocator_compare(
    const NANO_XRCE_TransportLocator *const left,
    const NANO_XRCE_TransportLocator *const right)
{
    NANO_i8 res = -1;
    int cmp_res = 0;
    
    NANO_LOG_FN_ENTRY
    
    NANO_PCOND(left != NULL)
    NANO_PCOND(right != NULL)
    
#define int_to_i8(v_) \
    (((v_) > 0)? 1 :\
        ((v_) < 0)? -1 : 0)

    switch (left->format)
    {
    case NANO_XRCE_ADDRESS_FORMAT_SMALL:
    {
        switch (right->format)
        {
        case NANO_XRCE_ADDRESS_FORMAT_SMALL:
        {
            const NANO_XRCE_TransportLocatorSmall *lsm = NULL,
                                                  *rsm = NULL;
            lsm = NANO_XRCE_TransportLocatorSmall_narrow(left);
            rsm = NANO_XRCE_TransportLocatorSmall_narrow(right);

            
            cmp_res = NANO_OSAPI_Memory_compare(
                    lsm->address, rsm->address, sizeof(lsm->address));
            if (cmp_res == 0)
            {
                cmp_res = REDAOrderedDataType_compareUInt(&lsm->port, &rsm->port);
            }
            res = int_to_i8(cmp_res);
            break;
        }
        default:
        {
            res = -1;
            break;
        }
        }
        break;
    }
    case NANO_XRCE_ADDRESS_FORMAT_MEDIUM:
    {
        switch (right->format)
        {
        case NANO_XRCE_ADDRESS_FORMAT_SMALL:
        {
            res = 1;
            break;
        }
        case NANO_XRCE_ADDRESS_FORMAT_MEDIUM:
        {
            const NANO_XRCE_TransportLocatorMedium *lmd = NULL,
                                                   *rmd = NULL;
            lmd = NANO_XRCE_TransportLocatorMedium_narrow(left);
            rmd = NANO_XRCE_TransportLocatorMedium_narrow(right);

            cmp_res = NANO_OSAPI_Memory_compare(
                    lmd->address,rmd->address, sizeof(lmd->address));
            if (cmp_res == 0)
            {
                cmp_res = REDAOrderedDataType_compareUInt(&lmd->port, &rmd->port);
            }
            res = int_to_i8(cmp_res);
            break;
        }
        default:
        {
            res = -1;
            break;
        }
        }
        break;
    }
    case NANO_XRCE_ADDRESS_FORMAT_LARGE:
    {
        switch (right->format)
        {
        case NANO_XRCE_ADDRESS_FORMAT_SMALL:
        case NANO_XRCE_ADDRESS_FORMAT_MEDIUM:
        {
            res = 1;
            break;
        }
        case NANO_XRCE_ADDRESS_FORMAT_LARGE:
        {
            const NANO_XRCE_TransportLocatorLarge *llg = NULL,
                                                  *rlg = NULL;
            llg = NANO_XRCE_TransportLocatorLarge_narrow(left);
            rlg = NANO_XRCE_TransportLocatorLarge_narrow(right);

            cmp_res = NANO_OSAPI_Memory_compare(
                    llg->address,rlg->address, sizeof(llg->address));
            if (cmp_res == 0)
            {
                cmp_res = REDAOrderedDataType_compareUInt(&llg->port, &rlg->port);
            }
            res = int_to_i8(cmp_res);
            break;
        }
        default:
        {
            res = -1;
            break;
        }
        }
        break;
    }
    case NANO_XRCE_ADDRESS_FORMAT_STRING:
    {
        switch (right->format)
        {
        case NANO_XRCE_ADDRESS_FORMAT_SMALL:
        case NANO_XRCE_ADDRESS_FORMAT_MEDIUM:
        case NANO_XRCE_ADDRESS_FORMAT_LARGE:
        {
            res = 1;
            break;
        }
        case NANO_XRCE_ADDRESS_FORMAT_STRING:
        {
            const NANO_XRCE_TransportLocatorString *lstr = NULL,
                                                   *rstr = NULL;
            lstr = NANO_XRCE_TransportLocatorString_narrow(left);
            rstr = NANO_XRCE_TransportLocatorString_narrow(right);

            res = REDAString_compare(lstr->value,rstr->value);
            break;
        }
        default:
        {
            res = -1;
            break;
        }
        }
        break;
    }
    default:
    {
        switch (right->format)
        {
        case NANO_XRCE_ADDRESS_FORMAT_SMALL:
        case NANO_XRCE_ADDRESS_FORMAT_MEDIUM:
        case NANO_XRCE_ADDRESS_FORMAT_LARGE:
        case NANO_XRCE_ADDRESS_FORMAT_STRING:
        {
            res = 1;
            break;
        }
        default:
        {
            res = -1;
            break;
        }
        }
        break;
    }
    }
    
    NANO_LOG_FN_EXIT_RC(res)
    return res;
}

NANO_RetCode
NANO_XRCE_TransportLocator_copy(
    NANO_XRCE_TransportLocator *const dst,
    const NANO_XRCE_TransportLocator *const src)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_LOG_FN_ENTRY
    
    NANO_PCOND(dst != NULL)
    NANO_PCOND(src != NULL)

    if (!NANO_XRCE_TransportLocatorFormat_is_valid(src->format))
    {
        goto done;
    }

    if (dst->format == NANO_XRCE_ADDRESS_FORMAT_STRING &&
        dst->value.str.value != NULL)
    {
        DDS_String_free(dst->value.str.value);
    }
    RTIOsapiMemory_zero(&dst->value, sizeof(dst->value));
    
    switch (src->format)
    {
    case NANO_XRCE_ADDRESS_FORMAT_SMALL:
    {
        dst->format = NANO_XRCE_ADDRESS_FORMAT_SMALL;
        dst->value.small = src->value.small;
        break;
    }
    case NANO_XRCE_ADDRESS_FORMAT_MEDIUM:
    {
        dst->format = NANO_XRCE_ADDRESS_FORMAT_MEDIUM;
        dst->value.medium = src->value.medium;
        break;
    }
    case NANO_XRCE_ADDRESS_FORMAT_LARGE:
    {
        dst->format = NANO_XRCE_ADDRESS_FORMAT_LARGE;
        dst->value.large = src->value.large;
        break;
    }
    case NANO_XRCE_ADDRESS_FORMAT_STRING:
    {
        dst->format = NANO_XRCE_ADDRESS_FORMAT_STRING;
        if (src->value.str.value != NULL)
        {
            dst->value.str.value = DDS_String_dup(src->value.str.value);
            if (dst->value.str.value == NULL)
            {
                goto done;
            }
        }
        break;
    }
    default:
    {
        goto done;
    }
    }
    
    rc = NANO_RETCODE_OK;
    
done:

    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

#endif /* NANO_FEAT_AGENT */


NANO_RetCode
NANO_XRCE_ClientTransport_initialize_recv_buffer(
    NANO_MessageBuffer *const msg,
    NANO_u8 *const buffer,
    const NANO_usize buffer_size,
    const NANO_usize mtu,
    NANO_u16 *abuffer_size_out)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_u8 *abuffer = NULL;
    NANO_usize abuffer_size = 0;
    
    NANO_PCOND(msg != NULL)
    NANO_PCOND(buffer != NULL)
    NANO_PCOND(buffer_size > 0)

    /* Cache aligned pointer */
    abuffer = NANO_OSAPI_Memory_align_ptr(buffer, sizeof(NANO_u32));
    /* Check that pointer was not aligned beyond specified size:
     * this is quite the "corner case", but we should check to avoid any
     * possible overflow */
    if ((NANO_usize)(abuffer - buffer) > buffer_size)
    {
        NANO_LOG_ERROR_MSG("invalid, unaligned buffer")
        rc = NANO_RETCODE_INVALID_ARGS;
        goto done;
    }
    abuffer_size = buffer_size - (abuffer - buffer);
    /* Check that there is a non-empty buffer to use */
    if (abuffer_size == 0 || abuffer_size > NANO_U16_MAX)
    {
        NANO_LOG_ERROR("invalid aligned buffer size",
            NANO_LOG_USIZE("buffer_size", buffer_size)
            NANO_LOG_USIZE("aligned_buffer_size", abuffer_size))
        rc = NANO_RETCODE_INVALID_ARGS;
        goto done;
    }
    if (abuffer_size < mtu)
    {
        NANO_LOG_ERROR("aligned buffer smaller than MTU",
            NANO_LOG_USIZE("mtu", mtu)
            NANO_LOG_USIZE("buffer_size", buffer_size)
            NANO_LOG_USIZE("aligned buffer_size", abuffer_size))
        rc = NANO_RETCODE_INVALID_ARGS;
        goto done;
    }
    else if (abuffer_size > mtu)
    {
        NANO_LOG_WARNING("buffer greater than mtu",
            NANO_LOG_USIZE("aligned buffer size",abuffer_size)
            NANO_LOG_USIZE("mtu",mtu)
            NANO_LOG_USIZE("wasted",abuffer_size - mtu))
        abuffer_size = mtu;
    }

    NANO_MessageBuffer_set_external_data(
        msg,
        abuffer,
        (NANO_u16)abuffer_size);
    NANO_MessageBuffer_set_type(msg, NANO_XRCE_MESSAGETYPE_RECV_BUFFER);
    
    NANO_PCOND(
        !NANO_MessageBuffer_flags(
            msg, NANO_XRCE_MESSAGEFLAGS_OWNED_BY_TRANSPORT))

    *abuffer_size_out = NANO_MessageBuffer_data_len(msg);
    
    rc = NANO_RETCODE_OK;
    
done:
    return rc;
}