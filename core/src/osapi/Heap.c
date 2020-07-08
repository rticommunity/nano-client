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

#if NANO_FEAT_AGENT

#if NANO_PLATFORM_IS_POSIX

#include <stdlib.h>

void*
NANO_OSAPI_PosixHeap_allocate_aligned(
    const NANO_usize size,
    const NANO_usize alignment)
{
    void *res = NULL;
    
    RTIOsapiHeap_allocateBufferAligned(&res, size, alignment);
    if (res == NULL)
    {
        goto done;
    }

    // rc = posix_memalign(&res, ssize, salign);
    // if (0 != rc)
    // {
    //     NANO_LOG_ERROR("FAILED to allocate aligned buffer",
    //         NANO_LOG_I32("rc",rc)
    //         if (rc == EINVAL)
    //         {
    //             NANO_LOG_STR("rc_str","EINVAL")
    //         }
    //         else if (rc == ENOMEM)
    //         {
    //             NANO_LOG_STR("rc_str","ENOMEM")
    //         }
    //         else
    //         {
    //             NANO_LOG_STR("rc_str","UNKNOWN")
    //         }
    //         NANO_LOG_USIZE("buffer_size",size)
    //         NANO_LOG_USIZE("alignment",alignment))
    //     goto done;
    // }

done:
    return res;
}
#endif /* NANO_PLATFORM */

#endif /* NANO_FEAT_AGENT */