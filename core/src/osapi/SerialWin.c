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

#if NANO_PLATFORM == NANO_PLATFORM_WINDOWS && \
    NANO_FEAT_TRANSPORT_PLUGIN_SERIAL

NANO_RetCode
NANO_OSAPI_WinSerialConnection_open(
    NANO_OSAPI_SerialConnection *const self,
    const NANO_OSAPI_SerialConnectionProperties *const properties)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_LOG_FN_ENTRY

    NANO_PCOND(self != NULL)
    NANO_PCOND(properties != NULL)

    

    rc = NANO_RETCODE_OK;
    
done:

    NANO_LOG_FN_EXIT_RC(rc)

    return rc;
}

NANO_RetCode
NANO_OSAPI_WinSerialConnection_write(
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

    
    
    rc = NANO_RETCODE_OK;

done:
    return rc;
}

NANO_RetCode
NANO_OSAPI_WinSerialConnection_read(
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

    

    rc = NANO_RETCODE_OK;

done:
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

NANO_RetCode
NANO_OSAPI_WinSerialConnection_close(
    NANO_OSAPI_SerialConnection *const self)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_LOG_FN_ENTRY
    
    NANO_PCOND(self != NULL)

    

    rc = NANO_RETCODE_OK;
    
done:
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

#endif /* NANO_PLATFORM == NANO_PLATFORM_WINDOWS &&
            NANO_FEAT_TRANSPORT_PLUGIN_SERIAL */