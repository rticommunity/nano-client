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

void
NANO_XRCE_SeqNum_compareI(
    const NANO_XRCE_SeqNum *const self,
    const NANO_XRCE_SeqNum *const other,
    NANO_i32 *cmp_out)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_i32 sn_a = 0,
             sn_b = 0,
             diff = 0,
             factor_1 = 0,
             factor_2 = 0;
    const NANO_i32 max_diff = 1 << (NANO_XRCE_SEQNUM_SERIAL_BITS-1),
                   sn_space = (1 << (NANO_XRCE_SEQNUM_SERIAL_BITS)) - 1;
    
    NANO_LOG_FN_ENTRY
    
    NANO_PCOND(self != NULL)
    NANO_PCOND(NANO_XRCE_SEQNUM_SERIAL_BITS <= 16)
    
    sn_a = (NANO_i32)NANO_XRCE_SeqNum_to_u16(self);
    sn_b = (NANO_i32)NANO_XRCE_SeqNum_to_u16(other);

    if (sn_a == sn_b)
    {
        *cmp_out = 0;
        rc = NANO_RETCODE_OK;
        goto done;
    }

    if (sn_a > sn_b)
    {
        diff = sn_a - sn_b;
        factor_1 = 1;
        factor_2 = -1;
    }
    else /* sn_b > sn_a */
    {
        diff = sn_b - sn_a;
        factor_1 = -1;
        factor_2 = 1;
    }
    NANO_PCOND(diff > 0)

    if (diff <= max_diff)
    {
        *cmp_out = factor_1 * diff;
    }
    else
    {
        *cmp_out = factor_2 * (sn_space - diff);
    }
    
    
    rc = NANO_RETCODE_OK;
    
done:

    if (NANO_RETCODE_OK != rc)
    {
        /* handle error */
    }

    NANO_LOG_TRACE_FN("SN::cmp()",
        NANO_LOG_I32("A", sn_a)
        NANO_LOG_H32("0XA", sn_a)
        NANO_LOG_I32("B", sn_b)
        NANO_LOG_H32("0XB", sn_b)
        NANO_LOG_I32("cmp_res", *cmp_out))

    NANO_LOG_FN_EXIT_RC(rc)
    return;
}

void
NANO_XRCE_SeqNum_compare(
    const NANO_XRCE_SeqNum *const self,
    const NANO_XRCE_SeqNum *const other,
    NANO_i8 *cmp_out)
{
    NANO_i32 cmp_res = 0;
    
    NANO_LOG_FN_ENTRY
    
    NANO_PCOND(self != NULL)
    
    NANO_XRCE_SeqNum_compareI(self, other, &cmp_res);

    if (cmp_res == 0)
    {
        *cmp_out = 0;
    }
    else if (cmp_res > 0)
    {
        *cmp_out = 1;
    }
    else
    {
        *cmp_out = -1;
    }
    
    return;
}

NANO_i32
NANO_XRCE_SeqNum_distance_from(
    const NANO_XRCE_SeqNum *const self,
    const NANO_XRCE_SeqNum *const other)
{
    NANO_i32 res = 0;
    NANO_XRCE_SeqNum_compareI(self, other, &res);
    return res;
}


// NANO_i32
// NANO_XRCE_SeqNum_distance_from(
//     const NANO_XRCE_SeqNum *const self,
//     const NANO_XRCE_SeqNum *const other)
// {
//     // const NANO_i32 min = ((NANO_i32)-1) * ((NANO_i32)NANO_U16_MAX - 1),
//     //                max = NANO_U16_MAX;
//     NANO_u32 u_self = 0,
//              u_other = 0;
//     NANO_i32 diff = 0;

//     u_self = (NANO_u32)NANO_XRCE_SeqNum_to_u16(self);
//     u_other = (NANO_u32)NANO_XRCE_SeqNum_to_u16(other);

//     diff = u_self - u_other;

//     if (diff < NANO_I16_MIN)
//     {
//         const NANO_i32 orig = diff;
//         diff = (diff - (NANO_I16_MIN*2)) * -1;
//         NANO_LOG_WARNING("DISTANCE overflow MIN",
//             NANO_LOG_SN("self", *self)
//             NANO_LOG_SN("other", *other)
//             NANO_LOG_I32("distance",orig)
//             NANO_LOG_I32("return",diff))
//     }
//     else if (diff > NANO_I16_MAX)
//     {
//         const NANO_i32 orig = diff;
//         diff = (diff - (NANO_I16_MAX*2)) * -1;
//         NANO_LOG_WARNING("DISTANCE overflow MAX",
//             NANO_LOG_SN("self", *self)
//             NANO_LOG_SN("other", *other)
//             NANO_LOG_I32("distance",orig)
//             NANO_LOG_I32("return",diff))
//     }

//     return diff;
// }

/******************************************************************************
 *                                  SeqNum
 ******************************************************************************/
#if 0
NANO_RetCode
NANO_XRCE_SeqNum_add(
    const NANO_XRCE_SeqNum *const self,
    const NANO_i16 add,
    NANO_XRCE_SeqNum *const res_out)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_XRCE_SeqNum def_sn = NANO_XRCE_SEQNUM_INITIALIZER;
    NANO_u16 s1 = 0,
             s2 = 0;
    
    if (add < 0 || add > NANO_I16_MAX)
    {
        /* TODO nanolog */
        rc = NANO_RETCODE_PRECONDITION_NOT_MET;
        goto done;
    }

    s1 = NANO_XRCE_SeqNum_to_u16(self);
    
    s2 = (s1 + ((NANO_u16)add)) % NANO_U16_MAX;

    NANO_XRCE_SeqNum_from_u16(res_out, s2);
    
    rc = NANO_RETCODE_OK;
    
done:
    if (NANO_RETCODE_OK != rc)
    {
        *res_out = def_sn;
    }
    
    return rc;
}
#endif

#if 0
NANO_RetCode
NANO_XRCE_SeqNum_compare(
    const NANO_XRCE_SeqNum *const self,
    const NANO_XRCE_SeqNum *const other,
    NANO_i8 *cmp_out)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    NANO_u16 s1 = 0,
             s2 = 0,
             l = 1 << (NANO_XRCE_SEQNUM_SERIAL_BITS-1),
             d12 = 0,
             d21 = 0;

    s1 = NANO_XRCE_SeqNum_to_u16(self);
    s2 = NANO_XRCE_SeqNum_to_u16(other);

    d12 = s1 - s2;
    d21 = s2 - s1;

    if (s1 == s2)
    {
        *cmp_out = 0;
    }
    else if ((s1 < s2 && d21 < l) || (s1 > s2 && d12 > l))
    {
            *cmp_out = -1;
    }
    else if ((s1 < s2 && d21 > l) || (s1 > s2 && d12 < l))
    {
            *cmp_out = 1;
    }
    else
    {
        /* sequence numbers are incomparable */
        goto done;
    }
    
    rc = NANO_RETCODE_OK;
    
done:
    if (NANO_RETCODE_OK != rc)
    {
        *cmp_out = 0;
    }
    
    return rc;
}
#endif
