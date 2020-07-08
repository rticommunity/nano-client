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
    defined(NANO_HAVE_ARDUINO_SERIAL)

NANO_RetCode
NANO_OSAPI_ArduinoSerialConnection_open(
    NANO_OSAPI_SerialConnection *const self,
    const NANO_OSAPI_SerialConnectionProperties *const properties)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)
    NANO_PCOND(properties != NULL)

    self->port = properties->port;
    self->block_timeout_ms = properties->block_timeout_ms;

    if (properties->init)
    {
        if (0 != NANO_ARDUINO_SerialPort_begin(self->port, properties->speed))
        {
            goto done;
        }

        if (0 != NANO_ARDUINO_SerialPort_wait_for_available(
                        self->port, properties->init_delay))
        {
            goto done;
        }
    }
    if (self->block_timeout_ms > 0)
    {
        if (0 != NANO_ARDUINO_SerialPort_set_timeout(
                        self->port, self->block_timeout_ms))
        {
            goto done;
        }
    }



    rc = NANO_RETCODE_OK;
    
done:

    NANO_LOG_FN_EXIT_RC(rc)

    return rc;
}

NANO_RetCode
NANO_OSAPI_ArduinoSerialConnection_write(
    NANO_OSAPI_SerialConnection *const self,
    const NANO_u8 *const data,
    const NANO_usize data_len)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    int rci = 0;
    
    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)
    NANO_PCOND(data != NULL)
    NANO_PCOND(data_len > 0)

    rci = NANO_ARDUINO_SerialPort_write(
            self->port, (const char*)data, data_len);

    if (rci != data_len)
    {
        NANO_LOG_ERROR("SERIAL write FAILED",
            NANO_LOG_USIZE("data_len", data_len)
            NANO_LOG_I32("rc", rci))
        goto done;
    }
    
    rc = NANO_RETCODE_OK;

done:
    return rc;
}

NANO_RetCode
NANO_OSAPI_ArduinoSerialConnection_read(
    NANO_OSAPI_SerialConnection *const self,
    NANO_u8 *const data,
    NANO_usize *const data_len)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    int rci = 0;
    
    NANO_LOG_FN_ENTRY
    
    NANO_PCOND(self != NULL)
    NANO_PCOND(data != NULL)
    NANO_PCOND(data_len != NULL)
    NANO_PCOND(*data_len > 0)

    rci = NANO_ARDUINO_SerialPort_read(
            self->port, (char*)data, *data_len, self->block_timeout_ms);

    if (rci < 0)
    {
        NANO_LOG_ERROR("SERIAL read FAILED",
            NANO_LOG_USIZE("data_len", *data_len)
            NANO_LOG_I32("rc", rci))
        goto done;
    }
    *data_len = (NANO_usize)rci;

    rc = NANO_RETCODE_OK;

done:
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

NANO_RetCode
NANO_OSAPI_ArduinoSerialConnection_close(
    NANO_OSAPI_SerialConnection *const self)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_LOG_FN_ENTRY
    
    NANO_PCOND(self != NULL)

    if (0 != NANO_ARDUINO_SerialPort_end(self->port))
    {
        goto done;
    }

    rc = NANO_RETCODE_OK;
    
done:
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

#endif /* NANO_FEAT_TRANSPORT_PLUGIN_SERIAL &&
            defined(NANO_HAVE_STD_SERIAL) */