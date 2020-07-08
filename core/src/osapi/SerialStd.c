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

#if NANO_FEAT_TRANSPORT_PLUGIN_SERIAL &&\
    defined(NANO_HAVE_STD_SERIAL)

#include <fcntl.h>
#include <termios.h>
#include <errno.h>

NANO_RetCode
NANO_OSAPI_StdSerialConnection_open(
    NANO_OSAPI_SerialConnection *const self,
    const NANO_OSAPI_SerialConnectionProperties *const properties)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_u32 open_flags = 0;
    struct termios tty;
    int speed = 0;
    NANO_bool fd_open = NANO_BOOL_FALSE;
    
    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)
    NANO_PCOND(properties != NULL)

    open_flags = O_RDWR | O_NOCTTY;
    // if (properties->sync)
    // {
    //     open_flags |= O_SYNC;
    // }

    NANO_OSAPI_Memory_zero(&tty,sizeof(tty));

    self->fd = open(properties->device, open_flags);
    if (self->fd < 0)
    {
        NANO_LOG_ERROR("FAILED to open SERIAL port",
            NANO_LOG_STR("dev",properties->device)
            NANO_LOG_H32("flags",open_flags))
        goto done;
    }
    fd_open = NANO_BOOL_TRUE;

    if(!isatty(self->fd))
    {
        NANO_LOG_ERROR("device is not a SERIAL TERMINAL",
            NANO_LOG_STR("dev",properties->device)
            NANO_LOG_I32("fd", self->fd))
        goto done;
    }

    if (tcgetattr(self->fd, &tty) != 0)
    {
        NANO_LOG_ERROR("FAILED to get TTY attributes",
            NANO_LOG_I32("fd",self->fd))
        goto done;
    }

    switch (properties->speed)
    {
    case 115200:
    {
        speed = B115200;
        break;
    
    }
    case 57600:
    {
        speed = B57600;
        break;
    
    }
    case 38400:
    {
        speed = B38400;
        break;
    
    }
    case 19200:
    {
        speed = B19200;
        break;
    
    }
    case 9600:
    {
        speed = B9600;
        break;
    
    }
    case 4800:
    {
        speed = B4800;
        break;
    
    }
    case 2400:
    {
        speed = B2400;
        break;
    
    }
    default:
    {
        NANO_LOG_ERROR("unsupported SERIAL speed",
            NANO_LOG_U32("speed",properties->speed))
        break;
    }
    }

    cfsetospeed(&tty, speed);
    cfsetispeed(&tty, speed);

    // if (properties->block_time_ms > 0)
    // {
    //     /* tty.c_cc[VMIN]  = should_block ? 1 : 0; */
    //     tty.c_cc[VMIN]  = 0;
    //     tty.c_cc[VTIME] = (properties->block_time_ms / 100);
    // }

    // tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
    // // disable IGNBRK for mismatched speed tests; otherwise receive break
    // // as \000 chars
    // tty.c_iflag &= ~IGNBRK;         // disable break processing
    // tty.c_lflag = 0;                // no signaling chars, no echo,
    //                                 // no canonical processing
    // tty.c_oflag = 0;                // no remapping, no delays
    // tty.c_cc[VMIN]  = 0;            // read doesn't block
    // tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

    // tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

    // tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
    //                                 // enable reading
    // tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
    // tty.c_cflag |= properties->parity;
    // tty.c_cflag &= ~CSTOPB;
    // tty.c_cflag &= ~CRTSCTS;

    // tty.c_iflag=0;
    // tty.c_oflag=0;
    // tty.c_cflag=CS8|CREAD|CLOCAL;           // 8n1, see termios.h for more information
    // tty.c_lflag=0;
    // tty.c_cc[VMIN]=1;
    // tty.c_cc[VTIME]=5;

    //
    // Input flags - Turn off input processing
    //
    // convert break to null byte, no CR to NL translation,
    // no NL to CR translation, don't mark parity errors or breaks
    // no input parity check, don't strip high bit off,
    // no XON/XOFF software flow control
    //
    tty.c_iflag &= ~(IGNBRK | BRKINT | ICRNL |
                        INLCR | PARMRK | INPCK | ISTRIP | IXON);
    
    //
    // Output flags - Turn off output processing
    //
    // no CR to NL translation, no NL to CR-NL translation,
    // no NL to CR translation, no column 0 CR suppression,
    // no Ctrl-D suppression, no fill characters, no case mapping,
    // no local output processing
    //
    tty.c_oflag &= ~(OCRNL | ONLCR | ONLRET |
                         ONOCR | OFILL | OPOST);

#if NANO_PLATFORM != NANO_PLATFORM_DARWIN
    tty.c_oflag &= ~(OLCUC);
#endif

    
    //
    // No line processing
    //
    // echo off, echo newline off, canonical mode off, 
    // extended input processing off, signal chars off
    //
    tty.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);


    //
    // Turn off character processing
    //
    // clear current char size mask, no parity checking,
    // no output processing, force 8 bit input
    //
    tty.c_cflag &= ~(CSIZE | PARENB);
    tty.c_cflag |= CS8;

    //
    // One input byte is enough to return from read()
    // Inter-character timer off
    //
    tty.c_cc[VMIN]  = 1;
    tty.c_cc[VTIME] = 0;
    if (properties->block_time_ms > 0)
    {
        /* tty.c_cc[VMIN]  = should_block ? 1 : 0; */
        tty.c_cc[VMIN]  = 0;
        tty.c_cc[VTIME] = (properties->block_time_ms / 100);
    }

    // if (tcsetattr(self->fd, TCSANOW, &tty) != 0)
    // {
    //     NANO_LOG_ERROR("FAILED to set TTY attributes",
    //         NANO_LOG_I32("fd",self->fd))
    //     goto done;
    // }

    if(tcsetattr(self->fd, TCSAFLUSH, &tty) < 0)
    {
        NANO_LOG_ERROR("FAILED to set TTY attributes",
            NANO_LOG_I32("fd",self->fd))
        goto done;
    }

    rc = NANO_RETCODE_OK;
    
done:
    if (NANO_RETCODE_OK != rc)
    {
        if (fd_open)
        {
            close(self->fd);
            self->fd = NANO_FD_INVALID;
        }
    }

    NANO_LOG_FN_EXIT_RC(rc)

    return rc;
}

/* Signature should probably be a more "low level" write() which takes
  an array of bytes, so that transport can write messages in chunks,
   up to bytes that need encoding, which it sends individually */
NANO_RetCode
NANO_OSAPI_StdSerialConnection_write(
    NANO_OSAPI_SerialConnection *const self,
    const NANO_u8 *const data,
    const NANO_usize data_len)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    int rci = 0;
    NANO_usize tx_len = 0;
    const NANO_u8 *ptr = data;

    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)
    NANO_PCOND(data != NULL)
    NANO_PCOND(data_len > 0)

    tx_len = (self->max_tx_size > 0)?
                    ((data_len > self->max_tx_size)?
                        self->max_tx_size : data_len) :
                    data_len;

    while ((NANO_usize)(ptr - data) < data_len)
    {
        rci = write(self->fd, ptr, tx_len);
        if (rci < 0)
        {
            NANO_LOG_ERROR("FAILED to write on SERIAL device",
                NANO_LOG_I32("rc",rci))
            goto done;
        }
        ptr += rci;

#if NANO_HAVE_YIELD
        NANO_OSAPI_Scheduler_yield();
#endif /* NANO_HAVE_YIELD */
    }
    
    rc = NANO_RETCODE_OK;
    
done:
    return rc;
}

NANO_RetCode
NANO_OSAPI_StdSerialConnection_read(
    NANO_OSAPI_SerialConnection *const self,
    NANO_u8 *const data,
    NANO_usize *const data_len)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_u8 *ptr = data;
    int rci = 0;

    NANO_LOG_FN_ENTRY
    
    NANO_PCOND(self != NULL)
    NANO_PCOND(data != NULL)
    NANO_PCOND(data_len != NULL)
    NANO_PCOND(*data_len > 0)

#if 0
    rx_size = (self->max_rx_size > 0)?
                ((data_len > self->max_rx_size)?
                    self->max_rx_size : data_len):
                    data_len;
    

    while ((ptr - data) < data_len)
    {
        rci = read(self->fd, ptr, rx_size);
        if (rci < 0)
        {
            NANO_LOG_ERROR("FAILED to read from SERIAL device",
                NANO_LOG_I32("rc",rci)
                NANO_LOG_I32("errno",errno)
                NANO_LOG_STR("error",strerror(errno)))
            goto done;
        }
        ptr += rci;
    }
#else
    rci = read(self->fd, ptr, *data_len);
    if (rci < 0)
    {
        NANO_LOG_ERROR("FAILED to read from SERIAL device",
            NANO_LOG_I32("rc",rci)
            NANO_LOG_I32("errno",errno)
            NANO_LOG_STR("error",strerror(errno)))
        goto done;
    }
    *data_len = rci;
#endif
    
    rc = NANO_RETCODE_OK;
    
done:

    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

NANO_RetCode
NANO_OSAPI_StdSerialConnection_close(
    NANO_OSAPI_SerialConnection *const self)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_LOG_FN_ENTRY
    
    NANO_PCOND(self != NULL)
    
    if (0 != close(self->fd))
    {
        goto done;
    }
    self->fd = NANO_FD_INVALID;
    
    rc = NANO_RETCODE_OK;
    
done:

    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

#endif /* NANO_FEAT_TRANSPORT_PLUGIN_SERIAL &&
            defined(NANO_HAVE_STD_SERIAL) */
