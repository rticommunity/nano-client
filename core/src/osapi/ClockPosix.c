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


#if NANO_PLATFORM_IS_POSIX && \
    NANO_FEAT_SYSTIME

#include <errno.h>

void
NANO_OSAPI_PosixClock_initialize(NANO_OSAPI_Clock *self)
{
    NANO_PCOND(self != NULL)
#ifdef NANO_HAVE_CLOCKID
#ifdef NANO_HAVE_CLOCK_MONOTONIC
    self->type = CLOCK_MONOTONIC;
    if (clock_getres(self->type,&self->resolution) < 0 )
    {
        NANO_PCOND(errno == EINVAL)
#endif /* NANO_HAVE_CLOCK_MONOTONIC */
        self->type = CLOCK_REALTIME;
        if (clock_getres(self->type,&self->resolution) < 0)
        {
            NANO_UNREACHABLE_CODE
        }
#ifdef NANO_HAVE_CLOCK_MONOTONIC
    }
#endif /* NANO_HAVE_CLOCK_MONOTONIC */
#else
    UNUSED_ARG(self);
    /* Nothing to do */
#endif /* NANO_HAVE_CLOCKID */
}

void
NANO_OSAPI_PosixClock_millis(NANO_OSAPI_Clock *self, NANO_u64 *const ms_out)
{
    NANO_Time t = NANO_TIME_INITIALIZER;
#ifdef NANO_HAVE_CLOCKID
    struct timespec ts = { 0, 0 };

    NANO_PCOND(self != NULL)
    NANO_PCOND(ms_out != NULL)

    if (clock_gettime(self->type, &ts) < 0)
    {
        NANO_UNREACHABLE_CODE
    }

    t.sec = ts.tv_sec;
    t.nanosec = ts.tv_nsec;
#else

#ifdef NANO_HAVE_GETTIMEOFDAY
    struct timeval tv;

    UNUSED_ARG(self);

    if (gettimeofday(&tv, NULL) != 0)
    {
        NANO_UNREACHABLE_CODE
    }
    
    t.sec = tv.tv_sec;
    t.nanosec = tv.tv_usec * 1000;
#endif

#endif /* NANO_HAVE_CLOCKID */

    NANO_Time_to_millis(&t, ms_out);
}

#endif /* NANO_PLATFORM_IS_POSIX && 
          NANO_FEAT_SYSTIME */