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

#if NANO_FEAT_CDR
#if NANO_FEAT_TRANSPORT && NANO_FEAT_AGENT

NANO_RetCode
NANO_XRCE_TransportLocator_deserialize_cdr(
    NANO_XRCE_TransportLocator *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    UNUSED_ARG(self);
    UNUSED_ARG(stream);
    
    rc = NANO_RETCODE_OK;
    return rc;
}

NANO_RetCode
NANO_XRCE_TransportLocator_serialize_cdr(
    const NANO_XRCE_TransportLocator *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;

    UNUSED_ARG(self);
    UNUSED_ARG(stream);

    rc = NANO_RETCODE_OK;

    return rc;
}

#endif /* NANO_FEAT_TRANSPORT */
#endif /* NANO_FEAT_CDR */