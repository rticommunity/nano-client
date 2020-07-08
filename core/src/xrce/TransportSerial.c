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
#include "nano/nano_core_xrce_transport_serial.h"

#define SERIAL_LOG          NANO_LOG_TRACE
#define SERIAL_LOG_MSG      NANO_LOG_TRACE_MSG

#define BEGIN_FLAG          0x7E
#define ESCAPE_FLAG         0x7D
#define XOR_FLAG            0x20

#define NANO_XRCE_FRAMECHECKSEQUENCE_INIT    0xffff  /* Initial FCS value */
#define NANO_XRCE_FRAMECHECKSEQUENCE_VALID   0xf0b8  /* Good final FCS value */

#define NANO_XRCE_SerialFrameHeader_src_addr(s_) \
    (((NANO_u8*)(s_))[1])

#define NANO_XRCE_SerialFrameHeader_dst_addr(s_) \
    (((NANO_u8*)(s_))[2])

#define NANO_XRCE_SerialFrameHeader_payload_len(s_,l_) \
{\
    NANO_u8 *const f_ = (NANO_u8*)(s_);\
    NANO_u16_deserialize((l_), (f_) + 3, NANO_BOOL_TRUE);\
}

#define NANO_XRCE_SerialFrameHeader_set_payload_len(s_,l_) \
{\
    NANO_u8 *const f_ = (NANO_u8*)(s_);\
    NANO_u16_serialize((l_),(f_ + 3),NANO_BOOL_TRUE);\
}

#define NANO_XRCE_SerialFrameHeader_set_src_addr(s_,a_) \
{\
    NANO_XRCE_SerialFrameHeader_src_addr((s_)) = (a_)->address[0];\
}

#define NANO_XRCE_SerialFrameHeader_set_dst_addr(s_,a_) \
{\
    NANO_XRCE_SerialFrameHeader_dst_addr((s_)) = (a_)->address[0];\
}

/* Copied from https://tools.ietf.org/html/rfc1662 */
NANO_PRIVATE
NANO_u16 NANO_XRCE_FrameCheckSequence_TABLE[256] = 
{
      0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
      0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
      0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
      0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
      0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
      0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
      0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
      0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
      0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
      0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
      0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
      0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
      0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
      0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
      0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
      0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
      0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
      0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
      0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
      0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
      0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
      0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
      0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
      0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
      0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
      0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
      0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
      0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
      0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
      0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
      0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
      0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
};

NANO_PRIVATE
NANO_u16
NANO_XRCE_FrameCheckSequence_update(
    const NANO_u16 fcs_cur,
    const NANO_u8 *const data,
    const NANO_u16 data_len)
{
    NANO_u16 res = 0,
             len = 0;
    const NANO_u8 *ptr = NULL;
    
    for (res = fcs_cur, len = data_len, ptr = data;
            len > 0;
                len -= 1, ptr += 1)
    {
        res = (res >> 8) ^ 
                NANO_XRCE_FrameCheckSequence_TABLE[(res ^ *ptr) & 0xff];
    }

    return res;
}

#if 0
NANO_PRIVATE
NANO_bool
NANO_XRCE_FrameCheckSequence_validate(
    const NANO_u8 *const data,
    const NANO_u16 data_len)
{
    NANO_bool rc = NANO_BOOL_FALSE;
    NANO_u16 fcs = 0;
    
    NANO_LOG_FN_ENTRY
    
    fcs = NANO_XRCE_FrameCheckSequence_update(
            NANO_XRCE_FRAMECHECKSEQUENCE_INIT, data, data_len);
    
    rc = (fcs == NANO_XRCE_FRAMECHECKSEQUENCE_VALID);
    
    NANO_LOG_FN_EXIT_BOOL(rc)
    return rc;
}
#endif

NANO_PRIVATE
void
NANO_XRCE_FrameCheckSequence_compute(
    const NANO_u16 fcs,
    NANO_u8 *const fcs_bytes_out)
{
    NANO_u16 fcs_bytes = fcs;
    
    NANO_LOG_FN_ENTRY
    
    fcs_bytes ^= 0xffff; /* complement */
    fcs_bytes_out[0] = (fcs_bytes & 0x00ff);
    fcs_bytes_out[1] = ((fcs_bytes >> 8) & 0x00ff);

    NANO_LOG_FN_EXIT
}

NANO_PRIVATE
NANO_RetCode
NANO_XRCE_SerialClientTransport_write_frame(
    NANO_XRCE_SerialClientTransport *const self,
    const NANO_XRCE_TransportLocatorSmall *const src_addr,
    const NANO_XRCE_TransportLocatorSmall *const dst_addr,
    const NANO_MessageBuffer *const msg)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_u16 msg_len = 0;
    NANO_MessageBuffer mbuf = NANO_MESSAGEBUFFER_INITIALIZER;
    const NANO_MessageBuffer *cur = NULL;
    NANO_u8 *data_ptr = NULL,
            cur_byte = 0,
            *escape_seq = NULL;
    NANO_u16 data_len = 0,
             data_len_max = 0,
             fcs = NANO_XRCE_FRAMECHECKSEQUENCE_INIT;
    NANO_bool escape_byte = NANO_BOOL_FALSE,
              end_of_buf = NANO_BOOL_FALSE,
              *frame_header = NULL;
    
    NANO_LOG_FN_ENTRY
    
    NANO_PCOND(self != NULL)

    NANO_MessageBuffer_data_len_msg(msg, &msg_len);

    // NANO_LOG_DEBUG("BEGIN serial MESSAGE",
    //     NANO_LOG_USIZE("payload_len", msg_len))
    
    frame_header = (NANO_u8*)self->frame_header_out;
    
    /* Initialize frame header */
    NANO_XRCE_SerialFrameHeader_set_payload_len(frame_header, msg_len);
    NANO_XRCE_SerialFrameHeader_set_src_addr(frame_header, src_addr);
    NANO_XRCE_SerialFrameHeader_set_dst_addr(frame_header, dst_addr);

    SERIAL_LOG("WRITING frame HEADER",
        NANO_LOG_BYTES("header", frame_header, NANO_XRCE_FRAME_HEADER_SIZE))

    /* Write frame header to serial port */
    NANO_CHECK_RC(
        NANO_OSAPI_SerialConnection_write(
            &self->connection, frame_header, NANO_XRCE_FRAME_HEADER_SIZE),
        NANO_LOG_ERROR("serial::send failed: frame header",
            NANO_LOG_RC(rc)
            NANO_LOG_USIZE("size", NANO_XRCE_FRAME_HEADER_SIZE)));
    
    cur = msg;
    data_ptr = NANO_MessageBuffer_data_ptr(cur);
    NANO_PCOND(data_ptr != NULL)
    data_len_max = NANO_MessageBuffer_data_len(cur);
    data_len = 0;

    while (cur != NULL)
    {
        /* Scan current byte */
        cur_byte = data_ptr[data_len];
        data_len += 1;
        end_of_buf = (data_len >= data_len_max);

        /* Check if the current char requires escaping */
        escape_byte = (cur_byte == BEGIN_FLAG || cur_byte == ESCAPE_FLAG);

        /* Check if we should write out the current chunk of buffer */
        // flush_buffer = escape_byte || end_of_buf;

        // SERIAL_LOG("WRITE BYTE",
        //     NANO_LOG_U8("byte",cur_byte)
        //     NANO_LOG_BOOL("escape", escape_byte)
        //     NANO_LOG_BOOL("eof", end_of_buf)
        //     NANO_LOG_BYTES("chunk", data_ptr, data_len))

        if (escape_byte || end_of_buf)
        {
            /* If the current char needs escaping, then we flush up to the
               previous char (unless this is the first char in the buffer) */
            if (escape_byte)
            {
                NANO_MessageBuffer_set_external_data(
                    &mbuf, data_ptr, data_len - 1);
            }
            else
            {
                NANO_MessageBuffer_set_external_data(
                    &mbuf, data_ptr, data_len);
            }

            if (NANO_MessageBuffer_data_len(&mbuf) > 0)
            {
                SERIAL_LOG("WRITING frame CHUNK",
                    NANO_LOG_BYTES("chunk",
                        NANO_MessageBuffer_data_ptr(&mbuf),
                        NANO_MessageBuffer_data_len(&mbuf)))
                
                NANO_CHECK_RC(
                    NANO_OSAPI_SerialConnection_write(
                        &self->connection,
                        NANO_MessageBuffer_data_ptr(&mbuf),
                        NANO_MessageBuffer_data_len(&mbuf)),
                    NANO_LOG_ERROR("serial::send failed: payload",
                        NANO_LOG_RC(rc)
                        NANO_LOG_USIZE("size",data_len)));
                
                /* Update frame check sequence */
                fcs = NANO_XRCE_FrameCheckSequence_update(
                        fcs,
                        NANO_MessageBuffer_data_ptr(&mbuf),
                        NANO_MessageBuffer_data_len(&mbuf));
                
                data_len -= NANO_MessageBuffer_data_len(&mbuf);
                data_len_max -= NANO_MessageBuffer_data_len(&mbuf);
                data_ptr += NANO_MessageBuffer_data_len(&mbuf);
            }

            /* Write the escaped byte, if needed */
            if (escape_byte)
            {
                NANO_MessageBuffer_flags_set_inline(&mbuf);
                escape_seq = NANO_MessageBuffer_data_ptr(&mbuf);
                escape_seq[0] = ESCAPE_FLAG;
                escape_seq[1] = cur_byte ^ XOR_FLAG;
                NANO_MessageBuffer_set_data_len(&mbuf, 2);

                SERIAL_LOG("WRITING frame ESCAPED byte",
                    NANO_LOG_BYTES("char", escape_seq, 2))
                
                NANO_CHECK_RC(
                    NANO_OSAPI_SerialConnection_write(
                        &self->connection, escape_seq, 2),
                    NANO_LOG_ERROR("serial::send failed: payload",
                        NANO_LOG_RC(rc)
                        NANO_LOG_USIZE("size",data_len)));
                
                /* Update frame check sequence */
                fcs = NANO_XRCE_FrameCheckSequence_update(fcs, &cur_byte, 1);

                data_len -= 1;
                data_len_max -= 1;
                data_ptr += 1;
            }

            NANO_PCOND(data_len == 0)

            /* If we're not at the end of the current buffer, then move data_ptr
               ahead, and reduce data_len_max by the flushed amount */
            if (end_of_buf)
            {
                cur = NANO_MessageBuffer_next(cur);
                if (cur != NULL)
                {
                    data_ptr = NANO_MessageBuffer_data_ptr(cur);
                    data_len_max = NANO_MessageBuffer_data_len(cur);
                }
                else
                {
                    data_ptr = NULL;
                    data_len_max = 0;
                }
            }
        }
    }

    /* Write frame check sequence */
    NANO_MessageBuffer_flags_set_inline(&mbuf);
    escape_seq = NANO_MessageBuffer_data_ptr(&mbuf);
    NANO_MessageBuffer_set_data_len(&mbuf, 2);
    NANO_XRCE_FrameCheckSequence_compute(fcs, escape_seq);

    SERIAL_LOG("WRITING frame CHECK",
            NANO_LOG_BYTES("check", escape_seq, 2))

    NANO_CHECK_RC(
        NANO_OSAPI_SerialConnection_write(&self->connection, escape_seq, 2),
        NANO_LOG_ERROR("serial::send failed: payload",
            NANO_LOG_RC(rc)
            NANO_LOG_USIZE("size",data_len)));
    
    rc = NANO_RETCODE_OK;
    
done:
    
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

#if 0
NANO_PRIVATE
NANO_RetCode
NANO_XRCE_SerialClientTransport_receive_frame_payload(
    NANO_XRCE_SerialClientTransport *const self,
    const NANO_u16 payload_max,
    const NANO_u16 payload_remaining,
    const NANO_bool escape_first,
    const NANO_u8 *const chunk_start,
    const NANO_u16 chunk_max,
    NANO_u8 *const recv_start,
    NANO_u16 *const recv_len_inout,
    NANO_bool *const escape_next_out)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_usize chunk_remaining = 0,
               copy_len = 0;
    NANO_u8 *recv_ptr = NULL;
    const NANO_u8 *chunk_ptr = NULL;
    
    NANO_LOG_FN_ENTRY
    
    NANO_PCOND(self != NULL)
    NANO_PCOND(payload_max > 0)
    NANO_PCOND(payload_remaining > 0)
    NANO_PCOND(payload_max >= payload_remaining)
    NANO_PCOND(chunk_start != NULL)
    NANO_PCOND(chunk_max > 0)
    NANO_PCOND(recv_start != NULL)
    NANO_PCOND(recv_len_inout != NULL)
    NANO_PCOND(*recv_len_inout > 0)

    chunk_remaining = (chunk_max > *recv_len_inout)?
                            *recv_len_inout : chunk_max;
    if (chunk_remaining > payload_remaining)
    {
        chunk_remaining = payload_remaining;
    }
    NANO_PCOND(chunk_remaining > 0)

    recv_ptr = recv_start;
    chunk_ptr = chunk_start;

    if (escape_first)
    {
        recv_ptr[0] = chunk_ptr[0] ^ XOR_FLAG;
        chunk_remaining -= 1;
        chunk_ptr += 1;
        recv_ptr += 1;
    }

    while (chunk_remaining > 0)
    {
        copy_len = 0;

        /* Determine how many bytes can be copied, without escaping */
        while (chunk_remaining > copy_len &&
                chunk_ptr[copy_len] != ESCAPE_FLAG)
        {
            /* Make sure that the BEGIN_FLAG doesn't occur while we're 
               still reading the payload */
            if (chunk_ptr[copy_len] == BEGIN_FLAG)
            {
                goto done;
            }
            copy_len += 1;
        }

        /* Copy bytes which don't require escaping into recv buffer */
        if (copy_len > 0)
        {
            NANO_OSAPI_Memory_copy(recv_ptr, chunk_ptr, copy_len);
            chunk_remaining -= copy_len;
            chunk_ptr += copy_len;
            recv_ptr += copy_len;
            copy_len = 0;
        }

        if (chunk_remaining > 0 &&
            chunk_ptr[0] == ESCAPE_FLAG)
        {
            /* consume escape flag */
            chunk_remaining -= 1;
            chunk_ptr += 1;
            if (chunk_remaining == 0)
            {
                /* The escape flag was the last byte in the current chunk,
                   we will need to escape the first byte in the next one */
                *escape_next_out = NANO_BOOL_TRUE;
            }
            else
            {
                recv_ptr[0] = chunk_ptr[0] ^ XOR_FLAG;
                chunk_remaining -= 1;
                chunk_ptr += 1;
                recv_ptr += 1;
            }
        }
    }
    *recv_len_inout = recv_ptr - recv_start;
    
    rc = NANO_RETCODE_OK;
    
done:

    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}
#endif

#if !NANO_FEAT_AGENT
NANO_PRIVATE
#endif
NANO_RetCode
NANO_XRCE_SerialClientTransport_receive_frame(
    NANO_XRCE_SerialClientTransport *const self,
    NANO_MessageBuffer *const recv_buf,
    const NANO_Timeout timeout_ms,
    NANO_u16 *const frame_len_out,
    NANO_u8 *const src_addr_out)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_bool escape_first = NANO_BOOL_FALSE;
    NANO_MessageBuffer *cur_recv_buf = NULL;
    NANO_usize read_len = 0,
               frame_len = 0,
               data_remaining_len = 0;
    NANO_u8 *read_ptr = NULL,
            *frame_ptr = NULL,
            fcs_bytes[2] = { 0x00 },
            src_addr = 0;
    NANO_u16 frame_payload_len = 0,
             copy_len = 0,
             fcs = NANO_XRCE_FRAMECHECKSEQUENCE_INIT;

#if NANO_FEAT_SYSTIME
    NANO_u64 ts_start = 0,
             ts_now = 0,
             ts_diff = 0;
#define check_timeout() \
{\
    if (timeout_ms > 0)\
    {\
        NANO_OSAPI_Clock_millis(&self->clock, &ts_now);\
        ts_diff = ts_now - ts_start;\
        if (ts_diff > (NANO_u64)timeout_ms)\
        {\
            rc = NANO_RETCODE_TIMEOUT;\
            goto done;\
        }\
    }\
    else if (timeout_ms == 0)\
    {\
        rc = NANO_RETCODE_TIMEOUT;\
        goto done;\
    }\
}
#else
#define check_timeout()
#endif /* NANO_FEAT_SYSTIME */

    NANO_LOG_FN_ENTRY
    
    NANO_PCOND(self != NULL)
    
    *frame_len_out = 0;

#if NANO_FEAT_SYSTIME
    NANO_OSAPI_Clock_millis(&self->clock, &ts_start);
#endif /* NANO_FEAT_SYSTIME */

    read_ptr = ((NANO_u8*)self->frame_header_in);

    /* Read one byte at a time, waiting for BEGIN_FLAG */
    while (1)
    {
        SERIAL_LOG("WAITING for BEGIN_FLAG",
            NANO_LOG_TS_MS("timeout", timeout_ms))

        read_len = 1;
        NANO_CHECK_RC(
            NANO_OSAPI_SerialConnection_read(
                &self->connection, read_ptr, &read_len),
            NANO_LOG_ERROR_MSG("FAILED to read from serial port"));

        SERIAL_LOG("SERIAL read (BEGIN)",
                NANO_LOG_BYTES("buffer", read_ptr, read_len))

        if (read_len == 1)
        {
            if (read_ptr[0] == BEGIN_FLAG)
            {
                SERIAL_LOG_MSG("BEGIN serial frame")
                break;
            }
        }

        check_timeout();

    }
    NANO_PCOND(read_ptr[0] == BEGIN_FLAG)

begin:
    fcs = NANO_XRCE_FRAMECHECKSEQUENCE_INIT;
    NANO_OSAPI_Memory_zero(fcs_bytes, sizeof(fcs_bytes));
    escape_first = NANO_BOOL_FALSE;
    cur_recv_buf = recv_buf;

    data_remaining_len = NANO_MessageBuffer_data_len(cur_recv_buf);
    if (data_remaining_len == 0)
    {
        goto done;
    }

    /* Read frame header (skip BEGIN_FLAG, which we already read) */
    frame_ptr = ((NANO_u8*)self->frame_header_in) + 1;
    frame_len = NANO_XRCE_FRAME_HEADER_SIZE - 1;
    read_len = 0;

    while (frame_len > 0)
    {
        SERIAL_LOG_MSG("WAITING for frame header")

        if (read_len == 0)
        {
            /* Read some more data */
            read_len = 1;
            read_ptr = (NANO_u8*)self->read_buffer;
            NANO_CHECK_RC(
                NANO_OSAPI_SerialConnection_read(
                    &self->connection, read_ptr, &read_len),
                NANO_LOG_ERROR_MSG("FAILED to read from serial port"));
            SERIAL_LOG("SERIAL read (HEADER)",
                NANO_LOG_BYTES("buffer", read_ptr, read_len))
        }
        if (read_len > 0)
        {
            if (read_ptr[0] == BEGIN_FLAG)
            {
                goto begin;
            }

            *frame_ptr = *read_ptr;

            frame_ptr += 1;
            frame_len -= 1;
            read_ptr += 1;
            read_len = 0;
        }
        if (frame_len > 0)
        {
            check_timeout();
        }
    }
    NANO_PCOND(frame_len == 0)

    src_addr = NANO_XRCE_SerialFrameHeader_src_addr(self->frame_header_in);

    SERIAL_LOG("FRAME header RECEIVED",
        NANO_LOG_BYTES("header",
            (const NANO_u8*)self->frame_header_in,
            NANO_XRCE_FRAME_HEADER_SIZE)
        NANO_LOG_H8("src", src_addr)
        NANO_LOG_H8("dst",
            NANO_XRCE_SerialFrameHeader_dst_addr(self->frame_header_in)))

    if (NANO_XRCE_SerialFrameHeader_dst_addr(self->frame_header_in) !=
            self->bind_address.address[0])
    {
        NANO_LOG_WARNING("FRAME header IGNORED",
                NANO_LOG_H8("src", src_addr)
            NANO_LOG_H8("dst",
                NANO_XRCE_SerialFrameHeader_dst_addr(self->frame_header_in)))
        rc = NANO_RETCODE_OK;
        goto done;
    }

    /* Deserialize payload length from frame header */
    NANO_XRCE_SerialFrameHeader_payload_len(
        self->frame_header_in, &frame_payload_len);

    /* Read all payload bytes and copy them into the receive buffer(s). */
    frame_len = frame_payload_len;
    frame_ptr = NANO_MessageBuffer_data_ptr(cur_recv_buf);

    SERIAL_LOG("RECEIVING frame PAYLOAD",
            NANO_LOG_U16("payload_len", frame_payload_len))
    
    escape_first = NANO_BOOL_FALSE;
    while (frame_len > 0 && cur_recv_buf != NULL)
    {
        
        SERIAL_LOG("WAITING for FRAME PAYLOAD...",
            NANO_LOG_U16("payload_len", frame_payload_len)
            NANO_LOG_U16("remaining", frame_len))

        if (read_len == 0)
        {
            /* Read some more data */
            // read_len = (frame_len > sizeof(self->read_buffer))?
            //                 sizeof(self->read_buffer) : frame_len;
            read_len = 1;
            read_ptr = (NANO_u8*) self->read_buffer;
            NANO_CHECK_RC(
                NANO_OSAPI_SerialConnection_read(
                    &self->connection, read_ptr, &read_len),
                NANO_LOG_ERROR_MSG("FAILED to read from serial port"));
            SERIAL_LOG("SERIAL read (PAYLOAD)",
                NANO_LOG_BYTES("buffer", read_ptr, read_len))
        }

        if (read_len > 0)
        {
            if (escape_first)
            {
                escape_first = NANO_BOOL_FALSE;
                *frame_ptr = (*read_ptr) ^ XOR_FLAG;
                copy_len = 1;
            }
            else if (read_ptr[0] == BEGIN_FLAG)
            {
                goto begin;
            }
            else if (read_ptr[0] == ESCAPE_FLAG)
            {
                escape_first = NANO_BOOL_TRUE;
            }
            else
            {
                *frame_ptr = *read_ptr;
                copy_len = 1;
            }

            read_ptr += 1;
            read_len = 0;

            if (copy_len == 1)
            {
                /* Update frame check sequence */
                fcs = (fcs >> 8) ^ 
                    NANO_XRCE_FrameCheckSequence_TABLE[(fcs ^ *frame_ptr) & 0xff];
                
                SERIAL_LOG("RECEIVED byte",
                    NANO_LOG_H8("b",frame_ptr[0]))
                
                frame_ptr += 1;
                frame_len -= 1;
                data_remaining_len -= 1;
                copy_len = 0;
            }
            
        }
        
        /* If we have more frame to receive, and we used all of the current
            receive buffer, try to switch to the next buffer in the chain */
        
        if (frame_len > 0)
        {
            check_timeout();

            if (data_remaining_len == 0)
            {
                cur_recv_buf = NANO_MessageBuffer_next(cur_recv_buf);
                frame_ptr = NANO_MessageBuffer_data_ptr(cur_recv_buf);
                data_remaining_len = NANO_MessageBuffer_data_len(cur_recv_buf);
                if (data_remaining_len == 0)
                {
                    rc = NANO_RETCODE_OUT_OF_RESOURCES;
                    goto done;
                }
            }
        }

    }

    if (frame_len > 0)
    {
        /* We ran out of receive buffer before we could receive all of the
           message. This shouldn't happen. */
        rc = NANO_RETCODE_OUT_OF_RESOURCES;
        goto done;
    }

    /* Read frame check sequence */
    frame_ptr = ((NANO_u8*)self->frame_header_in);
    frame_len = sizeof(NANO_u16);

    while (frame_len > 0)
    {
        read_len = 1;
        read_ptr = (NANO_u8*) self->read_buffer;

        NANO_CHECK_RC(
            NANO_OSAPI_SerialConnection_read(
                &self->connection, read_ptr, &read_len),
            NANO_LOG_ERROR_MSG("FAILED to read from serial port"));

        SERIAL_LOG("SERIAL read (FCS)",
                NANO_LOG_BYTES("buffer", read_ptr, read_len))
        
        if (read_len > 0)
        {
            if (read_ptr[0] == BEGIN_FLAG)
            {
                goto begin;
            }
            // NANO_OSAPI_Memory_copy(
            //     frame_ptr + ((sizeof(NANO_u16)) - frame_len), read_ptr, read_len);
            frame_ptr[sizeof(NANO_u16) - frame_len] = *read_ptr;
            frame_len -= read_len;
        }


        if (frame_len > 0)
        {
            check_timeout();
        }
    }
    
    NANO_XRCE_FrameCheckSequence_compute(fcs, fcs_bytes);

    if (NANO_OSAPI_Memory_compare(frame_ptr, fcs_bytes, sizeof(NANO_u16)) != 0)
    {
        /* The computed frame check sequence is invalid, ignore message */
        NANO_LOG_WARNING("INVALID frame check sequence",
            NANO_LOG_BYTES("computed_bytes", fcs_bytes, 2)
            NANO_LOG_BYTES("recvd_bytes", frame_ptr, 2))
        
        rc = NANO_RETCODE_OK;
        goto done;
    }

    NANO_PCOND(cur_recv_buf != NULL)
    if (data_remaining_len > 0)
    {
        frame_len =
            NANO_MessageBuffer_data_len(cur_recv_buf) - data_remaining_len;
        NANO_MessageBuffer_set_data_len(cur_recv_buf, frame_len);
    }
    NANO_MessageBuffer_unlink(cur_recv_buf);

    NANO_LOG_DEBUG("SERIAL frame RECEIVED",
        NANO_LOG_H8("src", src_addr)
        NANO_LOG_MBUF("frame", recv_buf))

    if (src_addr_out != NULL)
    {
        *src_addr_out = src_addr;
    }

    *frame_len_out = frame_payload_len;
    
    rc = NANO_RETCODE_OK;
    
done:
    
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

NANO_RetCode
NANO_XRCE_SerialClientTransport_initialize(
    NANO_XRCE_ClientTransport *const transport,
    const NANO_XRCE_ClientTransportListener *const listener,
    const NANO_XRCE_ClientTransportProperties *const properties)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_XRCE_SerialClientTransport *const self =
        (NANO_XRCE_SerialClientTransport*)transport;
    NANO_XRCE_SerialClientTransportProperties *const props =
        (NANO_XRCE_SerialClientTransportProperties*)properties;
    const NANO_XRCE_TransportLocatorSmall def_agent_address =
        NANO_XRCE_TRANSPORTLOCATORSMALL_INITIALIZER;
    NANO_XRCE_TransportLocatorSmall *agent_address = NULL;
    NANO_u8 *frame_header = NULL;
    
    NANO_LOG_FN_ENTRY
    
    NANO_PCOND(transport != NULL)
    NANO_PCOND(NANO_XRCE_ClientTransport_is_valid(transport))
    NANO_PCOND(properties != NULL)
    NANO_PCOND(properties->mtu > 0)
    NANO_PCOND(
        NANO_XRCE_ClientTransportListener_is_valid(listener))
    
    self->base.mtu = properties->mtu;
    self->base.send_queue = NULL;
    self->base.send_queue_len = 0;
    self->base.listener = *listener;

    NANO_CHECK_RC(
        NANO_XRCE_ClientTransport_initialize_recv_buffer(
            &self->recv_msg,
            props->recv_buffer,
            props->recv_buffer_size,
            self->base.mtu,
            &self->recv_msg_size),
        NANO_LOG_ERROR_MSG("FAILED to initialize receive buffer"));
    
    agent_address =
        NANO_XRCE_TransportLocatorSmall_narrow(&props->base.agent_address);
    if (agent_address != NULL)
    {
        self->agent_address = *agent_address;
    }
    else
    {
        self->agent_address = def_agent_address;
    }

    self->bind_address = props->bind_address;
    
    NANO_OSAPI_Memory_zero(
        self->frame_header_in, sizeof(self->frame_header_in));
    NANO_OSAPI_Memory_zero(
        self->frame_header_out, sizeof(self->frame_header_out));

    frame_header = (NANO_u8*) self->frame_header_out;
    
    frame_header[0] = BEGIN_FLAG;
    frame_header[1] = self->bind_address.address[0];
    frame_header[2] = self->agent_address.address[0];
    
    NANO_CHECK_RC(
        NANO_OSAPI_SerialConnection_open(
            &self->connection, &props->connection),
        NANO_LOG_ERROR_MSG("FAILED to open serial connection"));

    NANO_LOG_DEBUG("Serial client transport OK",
        NANO_LOG_USIZE("mtu", self->base.mtu)
        NANO_LOG_PTR("recv_buffer",
            NANO_MessageBuffer_data_ptr(&self->recv_msg))
        NANO_LOG_USIZE("recv_buffer_size",
            NANO_MessageBuffer_data_len(&self->recv_msg))
        )

    rc = NANO_RETCODE_OK;
    
done:

    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

void
NANO_XRCE_SerialClientTransport_finalize(
    NANO_XRCE_ClientTransport *const transport)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_XRCE_SerialClientTransport *const self =
        (NANO_XRCE_SerialClientTransport*)transport;
    
    NANO_LOG_FN_ENTRY
    
    NANO_PCOND(self != NULL)
    
    NANO_CHECK_RC(
        NANO_OSAPI_SerialConnection_close(&self->connection),
        NANO_LOG_ERROR_MSG("FAILED to close serial connection"));
    
    rc = NANO_RETCODE_OK;
    
done:

    NANO_LOG_FN_EXIT_RC(rc)
    return;
}

#if !NANO_FEAT_AGENT
NANO_PRIVATE
#endif
NANO_RetCode
NANO_XRCE_SerialClientTransport_send(
    NANO_XRCE_SerialClientTransport *const self,
    const NANO_XRCE_TransportLocatorSmall *const src_addr,
    const NANO_XRCE_TransportLocatorSmall *const dst_addr,
    NANO_MessageBuffer *const msg)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_usize data_len = 0;
    
    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)
    NANO_PCOND(msg != NULL)
    
    NANO_MessageBuffer_data_len_msg(msg, &data_len);

    if (data_len > self->base.mtu)
    {
        NANO_LOG_ERROR("message exceeds MTU",
            NANO_LOG_USIZE("msg_len", data_len)
            NANO_LOG_USIZE("mtu", self->base.mtu))
        rc = NANO_RETCODE_INVALID_ARGS;
        goto done;
    }

    NANO_CHECK_RC(
        NANO_XRCE_SerialClientTransport_write_frame(
            self, src_addr, dst_addr, msg),
        NANO_LOG_ERROR("serial::send failed",
            NANO_LOG_RC(rc)));
    
    SERIAL_LOG("serial message sent", 
        NANO_LOG_MSGHDR("hdr",
            *((NANO_XRCE_MessageHeader*)NANO_MessageBuffer_data_ptr(msg)))
        NANO_LOG_SUBMSGHDR("submsg",
            *((NANO_XRCE_SubmessageHeader*)NANO_XRCE_InlineHeaderBuffer_submsgid_ptr(msg)))
        NANO_LOG_MBUF("mbuf",msg))
    
    rc = NANO_RETCODE_OK;
    
done:
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}


void
NANO_XRCE_SerialClientTransport_flush_output(
    NANO_XRCE_ClientTransport *const transport)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_XRCE_SerialClientTransport *const self =
        (NANO_XRCE_SerialClientTransport*)transport;
    NANO_MessageBuffer *msg = NULL; 

    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)
    
    while (NANO_XRCE_ClientTransport_pending_send(&self->base))
    {
        msg = NANO_XRCE_ClientTransport_next_send(&self->base);
        NANO_PCOND(msg != NULL)
        rc = NANO_XRCE_SerialClientTransport_send(
                self, &self->bind_address, &self->agent_address, msg);
        NANO_XRCE_ClientTransport_send_complete(&self->base, 0, msg, rc);
    }

    NANO_LOG_FN_EXIT
}

NANO_PRIVATE
NANO_MessageBuffer *
NANO_XRCE_SerialClientTransport_allocate_message(
    NANO_XRCE_SerialClientTransport *const self)
{
    NANO_MessageBuffer *msg = NULL;

    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)

    if (NANO_MessageBuffer_flags(
            &self->recv_msg, NANO_XRCE_MESSAGEFLAGS_OWNED_BY_TRANSPORT))
    {
        NANO_LOG_ERROR_MSG("receive buffer already in use")
        goto done;
    }

    NANO_MessageBuffer_flags_set(
        &self->recv_msg, NANO_XRCE_MESSAGEFLAGS_OWNED_BY_TRANSPORT);

    msg = &self->recv_msg;

done:
    NANO_LOG_FN_EXIT_PTR(msg)

    return msg;
}

#if !(NANO_FEAT_RELIABILITY && \
        NANO_FEAT_RELIABLESTREAM_SENDQUEUE)
NANO_PRIVATE
#endif
void
NANO_XRCE_SerialClientTransport_return(
    NANO_XRCE_ClientTransport *const transport,
    NANO_MessageBuffer *const msg)
{
    NANO_XRCE_SerialClientTransport *const self =
        (NANO_XRCE_SerialClientTransport*)transport;

    NANO_LOG_FN_ENTRY

    NANO_PCOND(transport != NULL)

    NANO_PCOND(msg == &self->recv_msg)
    UNUSED_ARG(msg);

    NANO_PCOND(NANO_MessageBuffer_flags(
        &self->recv_msg, NANO_XRCE_MESSAGEFLAGS_OWNED_BY_TRANSPORT))
    
    NANO_MessageBuffer_flags_unset(
        &self->recv_msg,
        NANO_XRCE_MESSAGEFLAGS_OWNED_BY_TRANSPORT);
    
    NANO_MessageBuffer_set_data_len(&self->recv_msg, self->recv_msg_size);
    NANO_OSAPI_Memory_zero(
        NANO_MessageBuffer_data_ptr(&self->recv_msg),
        NANO_MessageBuffer_data_len(&self->recv_msg));

    NANO_LOG_FN_EXIT
}


NANO_RetCode
NANO_XRCE_SerialClientTransport_process_input(
    NANO_XRCE_ClientTransport *const transport,
    NANO_u32 max_messages,
    NANO_Timeout timeout_ms)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_XRCE_SerialClientTransport *const self =
        (NANO_XRCE_SerialClientTransport*)transport;
    NANO_usize remaining = 0;
    NANO_u16 frame_len = 0;
    NANO_bool retained = NANO_BOOL_FALSE;
    NANO_MessageBuffer *msg = NULL; 
    NANO_Timeout remaining_timeout = timeout_ms;
#if NANO_FEAT_SYSTIME
    NANO_u64 ms_start = 0,
             ms_now = 0,
             ms_diff = 0;
#endif /* NANO_FEAT_SYSTIME */
    
    NANO_LOG_FN_ENTRY
    
    NANO_PCOND(self != NULL)
    NANO_PCOND(max_messages > 0)
    
#if NANO_FEAT_SYSTIME
    NANO_OSAPI_Clock_millis(&self->clock, &ms_start);
#endif /* NANO_FEAT_SYSTIME */

    SERIAL_LOG("process input",
        NANO_LOG_TS_MS("timeout", timeout_ms)
        NANO_LOG_TS_MS("timeout_remaining", remaining_timeout))

    for (remaining = max_messages; remaining > 0; retained = NANO_BOOL_FALSE)
    {
        msg = NANO_XRCE_SerialClientTransport_allocate_message(self);
        if (msg == NULL)
        {
            NANO_LOG_ERROR("FAILED to allocate receive message",
                NANO_LOG_RC(rc));
            goto done;
        }

        switch (NANO_XRCE_SerialClientTransport_receive_frame(
                    self, msg, remaining_timeout, &frame_len, NULL))
        {
        case NANO_RETCODE_OK:
        case NANO_RETCODE_TIMEOUT:
        {
            break;
        }
        default:
        {
            NANO_LOG_ERROR_MSG("FAILED to receive serial frame")
            goto done;
        }
        }
        NANO_PCOND(frame_len <= self->base.mtu)

        if ((frame_len >= NANO_XRCE_MESSAGE_SERIALIZED_SIZE_MIN))
        {
            SERIAL_LOG("possible XRCE(UDP) data RECEIVED",
                    NANO_LOG_USIZE("size",frame_len))

            remaining -= 1;

            NANO_XRCE_ClientTransportListener_on_message_received(
                &self->base.listener,
                transport,
                msg,
                frame_len,
                &retained);
        }
        
#if NANO_FEAT_RELIABILITY && \
    NANO_FEAT_RELIABLESTREAM_SENDQUEUE
        NANO_PCOND(!retained)
#endif /* NANO_FEAT_RELIABILITY && \
        NANO_FEAT_RELIABLESTREAM_SENDQUEUE */
        
        if (!retained)
        {
            NANO_XRCE_SerialClientTransport_return(transport, msg);
        }
        else
        {
            NANO_LOG_WARNING("MESSAGE RETAINED",
                NANO_LOG_MBUF("msg", msg))
        }
        

#if NANO_FEAT_SYSTIME
        if (timeout_ms > 0 && remaining > 0)
        {
            NANO_OSAPI_Clock_millis(&self->clock, &ms_now);
            ms_diff = ms_now - ms_start;
            if (ms_diff >= (NANO_u64)(timeout_ms))
            {
                NANO_LOG_DEBUG("TIMEOUT exceeded",
                    NANO_LOG_I32("timeout",timeout_ms)
                    NANO_LOG_U32("passed",ms_diff))
                rc = NANO_RETCODE_TIMEOUT;
                goto done;
            }
            else
            {
                remaining_timeout = timeout_ms - ms_diff;
            }
        }
#endif /* NANO_FEAT_SYSTIME */
        if (timeout_ms == 0)
        {
            break;
        }
    }
    
    rc = NANO_RETCODE_OK;
    
done:

    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

#if NANO_FEAT_TRANSPORT_UPDATE
NANO_RetCode
NANO_XRCE_SerialClientTransport_update_locator(
    NANO_XRCE_ClientTransport *const transport,
    const NANO_XRCE_ClientTransportLocatorType locator_type,
    const NANO_XRCE_TransportLocator *const locator)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_LOG_FN_ENTRY

    UNUSED_ARG(transport);
    UNUSED_ARG(locator_type);
    UNUSED_ARG(locator);
    
    /* Serial transport doesn't support updating the agent's locator */
    goto done;
    
    rc = NANO_RETCODE_OK;
    
done:

    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

#endif /* NANO_FEAT_TRANSPORT_UPDATE */
