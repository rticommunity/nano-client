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

#if NANO_FEAT_TIME

NANO_RetCode
NANO_Time_initialize(
    NANO_Time *const self,
    const NANO_i32 sec,
    const NANO_u32 nanosec)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_Time norm_t = NANO_TIME_INITIALIZER;
    
    self->sec = sec;
    self->nanosec = nanosec;

    NANO_CHECK_RC(
        NANO_Time_normalize(self,&norm_t),
        /* TODO nanolog */);
    
    *self = norm_t;

    rc = NANO_RETCODE_OK;
    
done:

    return rc;
}

NANO_RetCode
NANO_Time_normalize(const NANO_Time *const self, NANO_Time *const norm_t)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    const NANO_Time ts_zero = NANO_TIME_INITIALIZER;

    if (self == (const NANO_Time *const) norm_t)
    {
        /* TODO nanolog */
        rc = NANO_RETCODE_PRECONDITION_NOT_MET;
        goto done;
    }

    *norm_t = *self;

    if (NANO_Time_is_normalized(self))
    {
        /* Nothing to do */
    }
    else if (NANO_Time_is_infinite(self))
    {
        norm_t->sec = -1;
        norm_t->nanosec = 0;
    }
    else
    {
        NANO_u32 secs_n = self->nanosec / NANO_NSEC_IN_SEC;
        if (secs_n > 0)
        {
            /* norm_t->sec must be >= 0 because self is finite */
            norm_t->sec = (NANO_i32) (((NANO_u32)norm_t->sec) + secs_n);
            /* check if there was an overflow */
            if (self->sec >= norm_t->sec)
            {
                /* TODO nanolog */
                rc = NANO_RETCODE_OVERFLOW_DETECTED;
                goto done;
            }
            norm_t->nanosec = self->nanosec - (NANO_NSEC_IN_SEC * secs_n);
        }
        else
        {
            /* Nothing to do */
        }
    }
    
    rc = NANO_RETCODE_OK;
    
done:
    if (NANO_RETCODE_OK != rc)
    {
        *norm_t = ts_zero;
    }
    
    return rc;
}

NANO_RetCode
NANO_Time_to_sec(const NANO_Time *const self, NANO_u32 *const sec_out)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_Time norm_self = NANO_TIME_INITIALIZER;

    if (NANO_Time_is_infinite(self))
    {
        /* TODO nanolog */
        rc = NANO_RETCODE_PRECONDITION_NOT_MET;
        goto done;
    }

    NANO_CHECK_RC(
        NANO_Time_normalize(self,&norm_self),
        /* TODO nanolog */);
    
    *sec_out = ((NANO_u32)norm_self.sec);

    if (norm_self.nanosec >= NANO_NSEC_IN_SEC/2)
    {
        *sec_out += 1;
    }
    
    rc = NANO_RETCODE_OK;
    
done:
    if (NANO_RETCODE_OK != rc)
    {
        *sec_out = 0;
    }
    
    return rc;
}

void
NANO_Time_to_millis(const NANO_Time *const self, NANO_u64 *const millis_out)
{
    NANO_u64 millis_sec = 0,
             millis_nsec = 0,
             millis_nsec_rem = 0;
    
    NANO_PCOND(!NANO_Time_is_infinite(self))
    NANO_PCOND(NANO_Time_is_normalized(self))

    millis_sec = ((NANO_u64)self->sec) * NANO_MILLIS_IN_SEC;
    
    millis_nsec = ((NANO_u64)self->nanosec) / NANO_NSEC_IN_MILLIS;
    millis_nsec_rem =
        ((NANO_u64)self->nanosec) - (millis_nsec * NANO_NSEC_IN_MILLIS);

    *millis_out = millis_sec;
    *millis_out = *millis_out + millis_nsec;
    if (millis_nsec_rem >= NANO_NSEC_IN_MILLIS/((NANO_u64)2))
    {
        *millis_out =  *millis_out + 1;
    }
}

#endif /* NANO_FEAT_TIME */
