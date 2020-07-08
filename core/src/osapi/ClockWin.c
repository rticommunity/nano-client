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

#if NANO_PLATFORM == NANO_PLATFORM_WINDOWS && NANO_FEAT_SYSTIME

/* Copied from https://stackoverflow.com/a/31335254 */

struct timespec { long tv_sec; long tv_nsec; };

NANO_PRIVATE
int clock_gettime(int fd, struct timespec *spec)
{
    __int64 wintime;
    GetSystemTimeAsFileTime((FILETIME*)&wintime);
    wintime      -= 116444736000000000i64;      //1jan1601 to 1jan1970
    spec->tv_sec  = wintime / 10000000i64;       //seconds
    spec->tv_nsec = wintime % 10000000i64 *100;  //nano-seconds
    return 0;
}

void
NANO_OSAPI_WinClock_initialize(NANO_OSAPI_Clock *self)
{
    NANO_PCOND(self != NULL)
    self->type = 0;
}

void
NANO_OSAPI_WinClock_millis(NANO_OSAPI_Clock *self, NANO_u64 *const ms_out)
{
    struct timespec ts = { 0, 0 };
    NANO_Time t = NANO_TIME_INITIALIZER;

    NANO_PCOND(self != NULL)
    NANO_PCOND(ms_out != NULL)

    if (clock_gettime(self->type, &ts) < 0)
    {
        NANO_UNREACHABLE_CODE
    }

    t.sec = ts.tv_sec;
    t.nanosec = ts.tv_nsec;

    NANO_Time_to_millis(&t, ms_out);
}

#endif /* NANO_PLATFORM == NANO_PLATFORM_WINDOWS && 
          NANO_FEAT_SYSTIME */