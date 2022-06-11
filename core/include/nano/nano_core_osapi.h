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

/**
 * @file nano_core_osapi.h
 * @brief Operating System services abstraction layer.
 * 
 * This header file defines a wrapper layer for services typically provided by
 * the Operating System. The API can be ported to different environments,
 * including those lacking an Operating System altogether.
 */

#ifndef nano_core_osapi_h
#define nano_core_osapi_h

#include "nano/nano_core_config.h"

/*****************************************************************************
 *                  Compiler-specific Definitions
 *****************************************************************************/

#include "nano/nano_core_osapi_cc.h"


/******************************************************************************
 *                                  Retcode
 ******************************************************************************/
/**
 * @addtogroup nano_api_infr_rc
 * @{
 */
/*e
 * @brief An enumeration of return codes to convery the outcome for all
 * NANO operations.
 */
typedef enum NANO_RetCodeI
{
    /*e
     * @brief The operation succeeded.
     */
    NANO_RETCODE_OK     = 0x00,
    /*e
     * @brief The operation failed because of an unspecified error.
     */
    NANO_RETCODE_ERROR  = 0x01,
    /*e
     * @brief The operation failed because of insufficient resources.
     */
    NANO_RETCODE_OUT_OF_RESOURCES  = 0x02,
    /*e
     * @brief The operation failed because the resource already exists.
     */
    NANO_RETCODE_ALREADY_EXISTS  = 0x03,
    /*e
     * @brief The operation failed because the referenced resource does not
     * exist.
     */
    NANO_RETCODE_UNKNOWN_REFERENCE  = 0x04,
    /*e
     * @brief The operation failed because it was passed invalid arguments.
     */
    NANO_RETCODE_INVALID_ARGS  = 0x05,
    /*e
     * @brief The operation failed because a precondition was not met.
     */
    NANO_RETCODE_PRECONDITION_NOT_MET  = 0x06,
    /*e
     * @brief The operation failed because a numerical overflow occurred.
     */
    NANO_RETCODE_OVERFLOW_DETECTED  = 0x07,
    /*e
     * @brief A timeout occurred while performing an operation.
     */
    NANO_RETCODE_TIMEOUT  = 0x08,

    /*e
     * @brief The operation failed because it detected a violation of
     * an established protocol.
     */
    NANO_RETCODE_PROTOCOL_ERROR = 0x09,

    /*e
     * @brief The operation failed and it should be tried again at a later time.
     */
    NANO_RETCODE_TRY_AGAIN = 0x10
} NANO_RetCode;

/*i
 * @brief Convert a boolean value to a NANO_RetCode.
 * 
 * Boolean "true" is assumed to mean the operation succeeded.
 * 
 * @param rc the boolean value to convert.
 * @return A NANO_RetCode corresponding to the specified boolean.
 */
NANO_RetCode
NANO_RetCode_from_bool(NANO_bool rc);

#define NANO_RetCode_from_bool(rc_) \
    (((NANO_bool)(rc_))? NANO_RETCODE_OK : NANO_RETCODE_ERROR)

/*i
 * @brief Convert an int value to a NANO_RetCode.
 * 
 * The operation is assumed to have succeded if rc is 0.
 * 
 * @param rc the int value to convert.
 * @return A NANO_RetCode corresponding to the specified int.
 */
NANO_RetCode
NANO_RetCode_from_int(NANO_i32 rc);

#define NANO_RetCode_from_int(rc_) \
    (((NANO_i32)(rc_) == 0)? NANO_RETCODE_OK : NANO_RETCODE_ERROR)

/*i
 * @brief TODO
 * 
 * @param rc 
 * @return const char* 
 */
const char*
NANO_RetCode_as_str(NANO_RetCode rc);

#define NANO_RetCode_as_str(rc_) \
    (((rc_) == NANO_RETCODE_OK)?\
        "OK":\
    ((rc_) == NANO_RETCODE_ERROR)?\
        "ERROR":\
    ((rc_) == NANO_RETCODE_OUT_OF_RESOURCES)?\
        "OUT_OF_RESOURCES":\
    ((rc_) == NANO_RETCODE_ALREADY_EXISTS)?\
        "ALREADY_EXISTS":\
    ((rc_) == NANO_RETCODE_UNKNOWN_REFERENCE)?\
        "UNKNOWN_REFERENCE":\
    ((rc_) == NANO_RETCODE_INVALID_ARGS)?\
        "INVALID_ARGS":\
    ((rc_) == NANO_RETCODE_PRECONDITION_NOT_MET)?\
        "PRECONDITION_NOT_MET":\
    ((rc_) == NANO_RETCODE_OVERFLOW_DETECTED)?\
        "OVERFLOW_DETECTED":\
    ((rc_) == NANO_RETCODE_TIMEOUT)?\
        "TIMEOUT":\
        "UNKNOWN")


#define NANO_CHECK_RC(expr_,log_) \
{ \
    if (NANO_RETCODE_OK != (rc = (expr_))) \
    { \
        log_ \
        goto done; \
    } \
    rc = NANO_RETCODE_ERROR; \
}


#define NANO_CHECK_BOOL(expr_,log_) \
{ \
    if (!(expr_)) \
    { \
        log_ \
        goto done; \
    } \
}


/** @} *//* nano_api_infr_rc */


/*****************************************************************************
 *                        OS-specific Definitions
 *****************************************************************************/

#ifdef DOCUMENTATION_ONLY
/******************************************************************************
 *                         String Helper Functions
 ******************************************************************************/

/**
 * @addtogroup nano_api_platform_sys_str
 * @{
 */

/*e
 * @brief Retrieve the length of a nul-terminated string.
 * 
 * @param self The string to inspect
 * @return The length of the string, excluding the "nul" character.
 */
NANO_usize
NANO_OSAPI_String_length(const char *self);

/** @} *//* nano_api_platform_sys_str */

/******************************************************************************
 *         Memory Allocation and Manipulation Helper Functions
 ******************************************************************************/
/**
 * @addtogroup nano_api_platform_sys_mem
 * @{
 */

/*e
 * @brief Copy memory from one address to another.
 * 
 * @param dest The destination address.
 * @param src The source address.
 * @param size The amount of memory to copy.
 * @return A pointer to the destination memory.
 */
void*
NANO_OSAPI_Memory_copy(
    void *const dest,
    const void *const src,
    const NANO_usize size);

/*e
 * @brief Set memory to a constant value.
 * 
 * @param addr The address of the memory to set.
 * @param val The constant to which the memory will be set.
 * @param size The amonut of memory to set.
 */
void
NANO_OSAPI_Memory_set(
    void *addr,
    NANO_i32 val,
    NANO_usize size);

/*e
 * @brief Compare two memory areas.
 * 
 * @param a1 The first memory area to compare.
 * @param a2 The second memory area to compare.
 * @param size The amount of memory to compare.
 * @return 0 if `a1[0-size] == a2[0-size]`, a value < 0 if
 * ``a1[0-size] < a2[0-size]`, a value > 0 if ``a1[0-size] > a2[0-size]`.
 */
NANO_i32
NANO_OSAPI_Memory_compare(
    const void *const a1,
    const void *const a2,
    const NANO_usize size);

/*e
 * @brief Initialize a memory area with zeroes.
 * 
 * @param addr The memory to initialize.
 * @param size The amount of memory to initialize.
 */
void
NANO_OSAPI_Memory_zero(
    const void* const addr,
    const NANO_usize size);

/*e
 * @brief Move a chunck of memory from one address to another.
 * 
 * @param dest Destination address.
 * @param src Source address.
 * @param size Amount of memory to move.
 * @return void* A pointer to the destination address.
 */
void*
NANO_OSAPI_Memory_move(
    void* const dest,
    const void* const src,
    const NANO_usize size);

/** @} *//* nano_api_platform_sys_mem */

/**
 * @addtogroup nano_api_platform_sys_clock
 * @{
 */

/*e
 * @brief 
 * 
 */
typedef struct NANO_OSAPI_FooClockI {
    int bar;
} NANO_OSAPI_FooClock;

/*e
 * @brief
 * 
 */
#define NANO_OSAPI_Clock        NANO_OSAPI_FooClock

/*e
 * @brief
 * 
 */
NANO_RetCode
NANO_OSAPI_Clock_initialize(NANO_OSAPI_Clock *self);

/*e
 * @brief
 * 
 */
void
NANO_OSAPI_Clock_millis(NANO_OSAPI_Clock *self, NANO_u64 *const ms_out);

/** @} *//* nano_api_platform_sys_clock */


/**
 * @addtogroup nano_api_libcfg_headers
 * @{
 */

/**
 * @brief Header file containing definitions for the target build platform.
 */
#define NANO_OS_DEF_H

/** @} *//* nano_api_libcfg_headers */

#else /* !DOCUMENTATION_ONLY */

#ifndef NANO_OS_DEF_H
#if NANO_PLATFORM_IS_POSIX
    #define NANO_OS_DEF_H "nano/nano_core_osapi_platform_posix.h"
#elif NANO_PLATFORM == NANO_PLATFORM_WINDOWS
    #define NANO_OS_DEF_H "nano/nano_core_osapi_platform_win.h"
#elif NANO_PLATFORM == NANO_PLATFORM_ARDUINO
    #define NANO_OS_DEF_H "nano/nano_core_osapi_platform_arduino.h"
#else
    #error "Please define NANO_OS_DEF_H for your custom platform"
#endif /* NANO_PLATFORM */
#endif /* NANO_OS_DEF_H */

#include NANO_OS_DEF_H

#ifndef NANO_OSAPI_String_length
#error "Required function not defined: NANO_OSAPI_String_length"
#endif /* NANO_OSAPI_String_length */

#ifndef NANO_OSAPI_Memory_copy
#error "Required function not defined: NANO_OSAPI_Memory_copy"
#endif /* NANO_OSAPI_Memory_copy */
#ifndef NANO_OSAPI_Memory_set
#error "Required function not defined: NANO_OSAPI_Memory_set"
#endif /* NANO_OSAPI_Memory_set */
#ifndef NANO_OSAPI_Memory_compare
#error "Required function not defined: NANO_OSAPI_Memory_compare"
#endif /* NANO_OSAPI_Memory_compare */
#ifndef NANO_OSAPI_Memory_zero
#error "Required function not defined: NANO_OSAPI_Memory_zero"
#endif /* NANO_OSAPI_Memory_zero */
#ifndef NANO_OSAPI_Memory_move
#error "Required function not defined: NANO_OSAPI_Memory_move"
#endif /* NANO_OSAPI_Memory_move */

#if NANO_FEAT_SYSTIME

#ifndef NANO_OSAPI_Clock
#error "Required type not defined: NANO_OSAPI_Clock"
#endif /* NANO_OSAPI_Clock */
#ifndef NANO_OSAPI_CLOCK_INITIALIZER
#error "Required macro not defined: NANO_OSAPI_CLOCK_INITIALIZER"
#endif /* NANO_OSAPI_Clock */
#ifndef NANO_OSAPI_Clock_initialize
#error "Required function not defined: NANO_OSAPI_Clock_initialize"
#endif /* NANO_OSAPI_Clock_initialize */
#ifndef NANO_OSAPI_Clock_millis
#error "Required function not defined: NANO_OSAPI_Clock_millis"
#endif /* NANO_OSAPI_Clock_millis */
#endif /* NANO_FEAT_SYSTIME */

#endif /* DOCUMENTATION_ONLY */

/******************************************************************************
 *                              MessageBuffer
 ******************************************************************************/

/**
 * @addtogroup nano_api_infr_mbuf
 * @{
 */

#define NANO_MESSAGEBUFFERFLAGS_TYPE            (0xE0000000)
#define NANO_MESSAGEBUFFERFLAGS_INLINE          (0x10000000)
#define NANO_MESSAGEBUFFERFLAGS_HAS_USERDATA    (0x08000000)
#define NANO_MESSAGEBUFFERFLAGS_USER_0          (0x04000000)
#define NANO_MESSAGEBUFFERFLAGS_USER_1          (0x02000000)
#define NANO_MESSAGEBUFFERFLAGS_USER_2          (0x01000000)
#define NANO_MESSAGEBUFFERFLAGS_USER_3          (0x00800000)
#define NANO_MESSAGEBUFFERFLAGS_USER_4          (0x00400000)
#define NANO_MESSAGEBUFFERFLAGS_USER_5          (0x00200000)
#define NANO_MESSAGEBUFFERFLAGS_USER_6          (0x00100000)
#define NANO_MESSAGEBUFFERFLAGS_UNUSED          (0x000F0000)
#define NANO_MESSAGEBUFFERFLAGS_USERDATA        (0x0000FFFF)
#define NANO_MESSAGEBUFFERFLAGS_DEFAULT         (0)

typedef NANO_u8 NANO_MessageBufferType;

#define NANO_MESSAGEBUFFERTYPE_DEFAULT          (0x00)
#define NANO_MESSAGEBUFFERTYPE_USER_0           (0x01)
#define NANO_MESSAGEBUFFERTYPE_USER_1           (0x02)
#define NANO_MESSAGEBUFFERTYPE_USER_2           (0x03)
#define NANO_MESSAGEBUFFERTYPE_USER_3           (0x04)
#define NANO_MESSAGEBUFFERTYPE_USER_4           (0x05)
#define NANO_MESSAGEBUFFERTYPE_USER_5           (0x06)
#define NANO_MESSAGEBUFFERTYPE_USER_6           (0x07)
#define NANO_MESSAGEBUFFERTYPE_USER_7           (0x08)


#define NANO_MessageBufferType_to_str(s_) \
    (((s_) == NANO_MESSAGEBUFFERTYPE_DEFAULT)?\
        "DEFAULT":\
    ((s_) == NANO_MESSAGEBUFFERTYPE_USER_0)?\
        "USER_0":\
    ((s_) == NANO_MESSAGEBUFFERTYPE_USER_1)?\
        "USER_1":\
    ((s_) == NANO_MESSAGEBUFFERTYPE_USER_2)?\
        "USER_2":\
    ((s_) == NANO_MESSAGEBUFFERTYPE_USER_3)?\
        "USER_3":\
    ((s_) == NANO_MESSAGEBUFFERTYPE_USER_4)?\
        "USER_4":\
    ((s_) == NANO_MESSAGEBUFFERTYPE_USER_5)?\
        "USER_5":\
    ((s_) == NANO_MESSAGEBUFFERTYPE_USER_6)?\
        "USER_6":\
    ((s_) == NANO_MESSAGEBUFFERTYPE_USER_7)?\
        "USER_7":\
        "UNKNOWN")

/*e
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_MessageBufferI NANO_MessageBuffer;

/*e
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_MessageBufferHeaderI
{
    /*e
     * @brief TODO
     * 
     */
    NANO_u32 flags;
    /*e
     * @brief TODO
     * 
     */
    NANO_u32 data;
    /*e
     * @brief TODO
     * 
     */
    NANO_MessageBuffer *next;
    /*e
     * @brief TODO
     * 
     */
    NANO_MessageBuffer *next_msg;

#if NANO_CPP
    /*e
     * @brief TODO
     * 
     */
    NANO_MessageBufferHeaderI()
    : 
        flags(NANO_MESSAGEBUFFERFLAGS_DEFAULT),
        data(0),
        next(NULL),
        next_msg(NULL)
    {}

#endif /* NANO_CPP */
} NANO_MessageBufferHeader;

/*e
 * @brief TODO
 * 
 */
#define NANO_MESSAGEBUFFERHEADER_INITIALIZER \
{\
    NANO_MESSAGEBUFFERFLAGS_DEFAULT, /* flags */ \
    0, /* data */\
    NULL, /* next */\
    NULL /* next_msg */\
}

/*e
 * @brief TODO
 * 
 */
struct NANODllExport NANO_MessageBufferI
{
    /*e
     * @brief TODO
     * 
     */
    NANO_MessageBufferHeader hdr;
    /*e
     * @brief TODO
     * 
     */
    NANO_u8 *data;
#if NANO_CPP
    /*e
     * @brief TODO
     * 
     */
    NANO_MessageBufferI()
    : 
        hdr(),
        data(NULL)
    {}

#endif /* NANO_CPP */
};

/*e
 * @brief TODO
 * 
 */
#define NANO_MESSAGEBUFFER_INITIALIZER \
{\
    NANO_MESSAGEBUFFERHEADER_INITIALIZER, /* hdr */ \
    NULL /* data */\
}

#define NANO_MESSAGEBUFFER_MIN_INLINE_DATA_SIZE \
    (sizeof(NANO_u8*)/sizeof(NANO_MessageBufferData))

#define NANO_MESSAGEBUFFER_MIN_INLINE_DATA_SIZE_BYTES \
    NANO_MESSAGEBUFFER_MIN_INLINE_DATA_SIZE * sizeof(NANO_MessageBufferData)

#define NANO_MESSAGEBUFFER_DATA_SIZE(tot_size_) \
    ((tot_size_) - NANO_OSAPI_MEMBER_OFFSET(NANO_MessageBuffer, data))

/*e
 * @brief Enable the INLINE flag.
 * 
 */
#define NANO_MessageBuffer_flags_set_inline(s_) \
{\
    NANO_MessageBuffer_flags_set((s_),NANO_MESSAGEBUFFERFLAGS_INLINE);\
}

/*i
 * @brief Enable one or more flags.
 * 
 */
#define NANO_MessageBuffer_flags_set(s_,f_) \
{\
    (s_)->hdr.flags |= (f_);\
}

/*i
 * @brief Disable one or more flags.
 * 
 */
#define NANO_MessageBuffer_flags_unset(s_,f_) \
{\
    (s_)->hdr.flags &= ~(f_);\
}

/*i
 * @brief Check if one or more flags are set.
 * 
 */
#define NANO_MessageBuffer_flags(s_,f_) \
    (((s_)->hdr.flags & (f_))? NANO_BOOL_TRUE : NANO_BOOL_FALSE)

/*i
 * @brief Set the data offset field.
 * 
 */
#define NANO_MessageBuffer_set_data_offset(s_,o_) \
{\
    (s_)->hdr.data &= 0x0000ffff;\
    (s_)->hdr.data |= (((NANO_u32)(o_)) << 16);\
}

/*e
 * @brief Set the data length field.
 * 
 */
#define NANO_MessageBuffer_set_data_len(s_,l_) \
{\
    (s_)->hdr.data &= 0xffff0000;\
    (s_)->hdr.data |= (((NANO_u32)(l_)) & 0x0000ffff);\
}

/*e
 * @brief Store the specified pointer to the data buffer, its length in the
 * data length field, and disable the INLINE flag.
 * 
 */
#define NANO_MessageBuffer_set_external_data(s_,data_,data_len_) \
{\
    (s_)->hdr.flags &= ~NANO_MESSAGEBUFFERFLAGS_INLINE;\
    (s_)->data = (data_);\
    NANO_MessageBuffer_set_data_len((s_),(data_len_));\
}

/*i
 * @brief Return the value of the data offset field.
 * 
 */
#define NANO_MessageBuffer_data_offset(s_) \
    ((NANO_u16)(((s_)->hdr.data & 0xffff0000) >> 16))

/*e
 * @brief Maximum length of a payload stored in a NANO_MessageBuffer.
 * 
 */
#define NANO_MESSAGEBUFFER_DATA_LEN_MAX         ((NANO_usize)NANO_U16_MAX)


/*e
 * @brief Return the total sum of the data length field of all the 
 * NANO_MessageBuffer in the current chain.
 * 
 */
#define NANO_MessageBuffer_data_len_msg(s_,len_) \
{\
    const NANO_MessageBuffer *nxt_mbuf_ = (s_);\
    *(len_) = 0;\
    while (nxt_mbuf_ != NULL)\
    {\
        *(len_) += NANO_MessageBuffer_data_len(nxt_mbuf_);\
        nxt_mbuf_ = NANO_MessageBuffer_next(nxt_mbuf_);\
    }\
}

/*e
 * @brief Return the value of the data length field.
 * 
 */
#define NANO_MessageBuffer_data_len(s_) \
    ((NANO_u16)(((s_)->hdr.data & 0x0000ffff)))

#define NANO_MESSAGEBUFFERFLAGS_TYPE_OFFSET     (29)

/*i
 * @brief Retrieve the value of the type field.
 * 
 */
#define NANO_MessageBuffer_type(s_) \
    ((NANO_u8)\
        (((s_)->hdr.flags & NANO_MESSAGEBUFFERFLAGS_TYPE) >> \
            NANO_MESSAGEBUFFERFLAGS_TYPE_OFFSET))

/*i
 * @brief Set the value of the type field.
 * 
 */
#define NANO_MessageBuffer_set_type(s_,t_) \
{\
    NANO_u32 tbits_ = ((((NANO_u32)(t_)) & \
            (NANO_MESSAGEBUFFERFLAGS_TYPE >> \
                    NANO_MESSAGEBUFFERFLAGS_TYPE_OFFSET)) << \
                            NANO_MESSAGEBUFFERFLAGS_TYPE_OFFSET);\
    (s_)->hdr.flags &= ~NANO_MESSAGEBUFFERFLAGS_TYPE;\
    (s_)->hdr.flags |= tbits_;\
}

/*i
 * @brief Set the value of the userdata field and enable the HAS_USERDATA flag.
 * 
 */
#define NANO_MessageBuffer_set_userdata(s_,d_) \
{\
    NANO_u32 data_ = (d_);\
    (s_)->hdr.flags &= ~NANO_MESSAGEBUFFERFLAGS_USERDATA;\
    (s_)->hdr.flags |= (data_ & NANO_MESSAGEBUFFERFLAGS_USERDATA);\
    NANO_MessageBuffer_flags_set((s_),NANO_MESSAGEBUFFERFLAGS_HAS_USERDATA);\
}

/*i
 * @brief Reset the value of the userdata field and disable the HAS_USERDATA
 * flag.
 * 
 */
#define NANO_MessageBuffer_reset_userdata(s_) \
{\
    (s_)->hdr.flags &= ~NANO_MESSAGEBUFFERFLAGS_USERDATA;\
    NANO_MessageBuffer_flags_unset((s_),NANO_MESSAGEBUFFERFLAGS_HAS_USERDATA);\
}

/*i
 * @brief Get the 16-bit value of the userdata field.
 * 
 */
#define NANO_MessageBuffer_userdata(s_) \
    ((NANO_MessageBuffer_flags_has_userdata((s_)))?\
        ((NANO_u16)((s_)->hdr.flags & NANO_MESSAGEBUFFERFLAGS_USERDATA)):0)

/*e
 * @brief Check if the INLINE flag is set.
 * 
 */
#define NANO_MessageBuffer_flags_inline(s_) \
    ((s_)->hdr.flags & NANO_MESSAGEBUFFERFLAGS_INLINE)

/*i
 * @brief Check if the HAS_USERDATA flag is set.
 * 
 */
#define NANO_MessageBuffer_flags_has_userdata(s_) \
    ((s_)->hdr.flags & NANO_MESSAGEBUFFERFLAGS_HAS_USERDATA)

#define NANO_MessageBuffer_inline_data_head_ptr(s_) \
    (((NANO_u8*)(s_)) + NANO_OSAPI_MEMBER_OFFSET(NANO_MessageBuffer,data))

#define NANO_MessageBuffer_inline_data_ptr(s_) \
    (NANO_MessageBuffer_inline_data_head_ptr((s_)) + \
            NANO_MessageBuffer_data_offset((s_)))

/*i
 * @brief Get a pointer to the start of the data buffer.
 * 
 */
#define NANO_MessageBuffer_data_head_ptr(s_) \
    ((NANO_MessageBuffer_flags_inline((s_)))? \
        NANO_MessageBuffer_inline_data_head_ptr((s_)) : \
        (s_)->data)

/*e
 * @brief Get a pointer to the pointer to the data buffer, offset by the value
 * of the offset field.
 * 
 */
#define NANO_MessageBuffer_data_ptr(s_) \
   (NANO_MessageBuffer_data_head_ptr((s_)) + \
        NANO_MessageBuffer_data_offset((s_)))

/*e
 * @brief Get a pointer to the next NANO_MessageBuffer in the current chain.
 * 
 */
#define NANO_MessageBuffer_next(s_) \
    ((s_)->hdr.next)

/*e
 * @brief Get a pointer to the first NANO_MessageBuffer in the next chain.
 * 
 */
#define NANO_MessageBuffer_next_msg(s_) \
    ((s_)->hdr.next_msg)

/*e
 * @brief Reset the link to the next NANO_MessageBuffer in the current chain.
 * 
 */
#define NANO_MessageBuffer_unlink(s_) \
{\
    (s_)->hdr.next = NULL;\
}

/*e
 * @brief Reset the link to the first NANO_MessageBuffer in the next chain.
 * 
 */
#define NANO_MessageBuffer_unlink_msg(s_) \
{\
    (s_)->hdr.next_msg = NULL;\
}

/*e
 * @brief Store the specified NANO_MessageBuffer as the next in the
 * current chain.
 * 
 */
#define NANO_MessageBuffer_append(s_,next_) \
{\
    (s_)->hdr.next = (next_);\
}

/*e
 * @brief Store the specified NANO_MessageBuffer as the first of the next chain.
 * 
 */
#define NANO_MessageBuffer_append_msg(s_,next_) \
{\
    (s_)->hdr.next_msg = (next_);\
}

#if NANO_64BIT && !DOCUMENTATION_ONLY
typedef NANO_u64 NANO_MessageBufferData;
#define NANO_MessageBufferData_alignment    NANO_u64_alignment
#else
/*e
 * @brief TODO
 * 
 */
typedef NANO_u32 NANO_MessageBufferData;
#define NANO_MessageBufferData_alignment    NANO_u32_alignment
#endif /* NANO_64BIT */

/*e
 * @brief TODO
 * 
 */
#define NANO_MESSAGEBUFFER_INLINE_SIZE(size_) \
    (NANO_MESSAGEBUFFER_INLINE_SIZE_BYTES((size_)) / \
        sizeof(NANO_MessageBufferData))

/*e
 * @brief TODO
 * 
 */
#define NANO_MESSAGEBUFFER_INLINE_SIZE_BYTES(size_) \
    (((size_) < (NANO_U16_MAX - sizeof(NANO_MessageBufferData)))?\
        NANO_OSAPI_Memory_align_size_up(\
            (sizeof(NANO_MessageBufferHeader) + (size_)), \
            sizeof(NANO_MessageBufferData)):\
        (size_))

/*e
 * @brief TODO
 * 
 */
#define NANO_MESSAGEBUFFER_MAX_INLINE_PAYLOAD(buffer_size_) \
    ((buffer_size_) - sizeof(NANO_MessageBufferHeader))


/******************************************************************************
 *                         MessageBufferQueue
 ******************************************************************************/

/*i
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_MessageBufferQueueI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_MessageBuffer *head;
    /*i
     * @brief TODO
     * 
     */
    NANO_MessageBuffer *tail;
    /*i
     * @brief TODO
     * 
     */
    NANO_usize len;


#if NANO_CPP
    /*i
     * @brief TODO
     * 
     */
    NANO_MessageBufferQueueI()
    :
        head(NULL),
        tail(NULL),
        len(0)
    { }

#endif /* NANO_CPP */
} NANO_MessageBufferQueue;

/*i
 * @brief TODO
 * 
 */
#define NANO_MESSAGEBUFFERQUEUE_INITIALIZER \
{\
    NULL, /* head */\
    NULL, /* tail */\
    0 /* len */\
}

/*i
 * @brief Check if the list is empty
 * 
 */
#define NANO_MessageBufferQueue_empty(s_) \
    ((s_)->head == NULL)

/*i
 * @brief Remove the first NANO_MessageBuffer from the list.
 * 
 */
#define NANO_MessageBufferQueue_pop_first(s_,head_) \
{\
    *(head_) = (s_)->head;\
    if ((s_)->head != NULL)\
    {\
        if ((s_)->tail == (s_)->head)\
        {\
            NANO_MessageBufferQueue_drop((s_));\
        }\
        else\
        {\
            (s_)->head = NANO_MessageBuffer_next_msg((s_)->head);\
            NANO_PCOND((s_)->len > 1 && (s_)->head != NULL)\
            (s_)->len -= 1;\
            NANO_MessageBuffer_unlink_msg(*(head_));\
        }\
    }\
}

/*i
 * @brief Access the first NANO_MessageBuffer in the list.
 * 
 */
#define NANO_MessageBufferQueue_head(s_) \
    ((s_)->head)

/*i
 * @brief Access the last NANO_MessageBuffer in the list.
 * 
 */
#define NANO_MessageBufferQueue_tail(s_) \
    ((s_)->tail)

/*i
 * @brief Retrieve the total number of NANO_MessageBuffer currently stored in
 * the list
 * 
 */
#define NANO_MessageBufferQueue_size(s_) \
    ((s_)->len)

/*i
 * @brief Append a NANO_MessageBuffer at the end of the list.
 * 
 */
#define NANO_MessageBufferQueue_insert_last(s_,msg_) \
{\
    if ((s_)->tail != NULL)\
    {\
        NANO_MessageBuffer_append_msg((s_)->tail, (msg_));\
        (s_)->tail = (msg_);\
    }\
    else\
    {\
        (s_)->head = (msg_);\
        (s_)->tail = (msg_);\
    }\
    (s_)->len += 1;\
}

/*i
 * @brief Append a NANO_MessageBuffer at the beginning of the list.
 * 
 */
#define NANO_MessageBufferQueue_insert_first(s_,msg_) \
{\
    if ((s_)->head != NULL)\
    {\
        NANO_MessageBuffer_append_msg((msg_),(s_)->head);\
        (s_)->head = (msg_);\
    }\
    else\
    {\
        (s_)->head = (msg_);\
        (s_)->tail = (msg_);\
    }\
    (s_)->len += 1;\
}

/*i
 * @brief Extract a NANO_MessageBuffer from the list.
 * 
 */
#define NANO_MessageBufferQueue_extract(s_,msg_) \
{\
    NANO_MessageBuffer *nxt_ = NULL,\
                       *prv_ = NULL;\
    NANO_PCOND((s_)->head != NULL && (s_)->tail != NULL)\
    nxt_ = NANO_MessageBuffer_next_msg((msg_));\
    if ((msg_) != (s_)->head) \
    {\
        prv_ = (s_)->head;\
        while (prv_ != NULL && NANO_MessageBuffer_next_msg(prv_) != (msg_))\
        {\
            prv_ = NANO_MessageBuffer_next_msg(prv_);\
        }\
        NANO_PCOND(prv_ != NULL)\
        NANO_MessageBuffer_append_msg(prv_, nxt_);\
    }\
    else\
    {\
        (s_)->head = (nxt_);\
    }\
    if ((msg_) == (s_)->tail) \
    {\
        NANO_PCOND(nxt_ == NULL)\
        (s_)->tail = prv_;\
    }\
    NANO_MessageBuffer_unlink_msg((msg_));\
    NANO_PCOND((s_)->len > 0)\
    (s_)->len -= 1;\
}

/*i
 * @brief Insert a NANO_MessageBuffer before another one already in the list.
 * 
 */
#define NANO_MessageBufferQueue_insert_before(s_,msg_,next_) \
{\
    NANO_MessageBuffer *prv_ = NULL;\
    NANO_PCOND((s_)->head != NULL && (s_)->tail != NULL)\
    NANO_PCOND(NANO_MessageBuffer_next_msg((msg_)) == NULL ||\
        NANO_MessageBuffer_next_msg((msg_)) == (next_))\
    if ((next_) != (s_)->head) \
    {\
        prv_ = (s_)->head;\
        while (prv_ != NULL && NANO_MessageBuffer_next_msg(prv_) != (next_))\
        {\
            prv_ = NANO_MessageBuffer_next_msg(prv_);\
        }\
        NANO_PCOND(prv_ != NULL)\
        NANO_MessageBuffer_append_msg(prv_, (msg_));\
    }\
    else\
    {\
        (s_)->head = (msg_);\
    }\
    NANO_MessageBuffer_append_msg((msg_), (next_));\
    (s_)->len += 1;\
}

/*i
 * @brief Insert a NANO_MessageBuffer after another one already in the list.
 * 
 */
#define NANO_MessageBufferQueue_insert_after(s_,msg_,after_) \
{\
    NANO_MessageBuffer *nxt_ = NULL;\
    NANO_PCOND((s_)->head != NULL && (s_)->tail != NULL)\
    NANO_PCOND(NANO_MessageBuffer_next_msg((msg_)) == NULL)\
    nxt_ = NANO_MessageBuffer_next_msg((after_));\
    NANO_MessageBuffer_append_msg((after_),(msg_));\
    NANO_MessageBUffer_append_msg((msg_), nxt_);\
    if ((after_) == (s_)->tail) \
    {\
        (s_)->tail = (msg_);\
    }\
    (s_)->len += 1;\
}

/*i
 * @brief Remove the last NANO_MessageBuffer from the list.
 * 
 */
#define NANO_MessageBufferQueue_pop_last(s_,tail_) \
{\
    *(tail_) = (s_)->tail;\
    if ((s_)->tail != NULL)\
    {\
        if ((s_)->head == (s_)->tail)\
        {\
            NANO_MessageBufferQueue_drop((s_));\
        }\
        else\
        {\
            NANO_MessageBuffer *nxt_ = (s_)->head;\
            while (NANO_MessageBuffer_next_msg(nxt_) != (s_)->tail)\
            {\
                nxt_ = NANO_MessageBuffer_next_msg(nxt_);\
            }\
            (s_)->tail = nxt_;\
            (s_)->len -= 1;\
            NANO_MessageBuffer_unlink_msg(nxt_);\
        }\
    }\
}

/*i
 * @brief Drop all NANO_MessageBuffer from the list.
 * 
 */
#define NANO_MessageBufferQueue_drop(s_) \
{\
    NANO_MessageBuffer *cur_ = (s_)->head,\
                       *nxt_ = NULL;\
    while (cur_ != NULL) \
    {\
        nxt_ = NANO_MessageBuffer_next_msg(cur_);\
        NANO_MessageBuffer_unlink_msg(cur_);\
        cur_ = nxt_;\
    }\
    (s_)->head = NULL;\
    (s_)->tail = NULL;\
    (s_)->len = 0;\
}

/** @} *//* nano_api_infr_mbuf */

/******************************************************************************
 *                            Time Representation
 ******************************************************************************/

/**
 * @addtogroup nano_api_infr_time
 * @{
 */

/*e
 * @brief TODO
 * 
 */
typedef NANO_i32 NANO_Timeout;

/*e
 * @brief TODO
 * 
 */
#define NANO_TIMEOUT_INFINITE           (-1)

#if NANO_FEAT_TIME
/*e
 * @brief A type to represent time-stamps and time durations.
 * 
 * Time is represented as a "seconds" component and a fractional, "nanoseconds"
 * one. The "nanoseconds" part should always be less than 1s.
 */
typedef struct NANODllExport NANO_TimeI {
    /**
     * @brief TODO
     * 
     */
    NANO_i32    sec;
    /**
     * @brief TODO
     * 
     */
    NANO_u32    nanosec;

#if NANO_CPP

    NANO_TimeI()
    : sec(0),
      nanosec(0)
    {}

    NANO_TimeI(const NANO_i32 s, const NANO_u32 ns)
    : sec(s),
      nanosec(ns)
    {}

#endif /* NANO_CPP */
} NANO_Time;

#define NANO_TIME_SERIALIZED_SIZE_MAX \
    (sizeof(NANO_i32) + \
        sizeof(NANO_u32))

/*i
 * @brief Constant defining the number of nanoseconds in a second.
 * 
 * Used for conversion purposes.
 */
#define NANO_NSEC_IN_SEC        ((NANO_u32)1000000000)

/*i
 * @brief Constant defining the number of milliseconds in a second.
 * 
 * Used for conversion purposes.
 */
#define NANO_MILLIS_IN_SEC      ((NANO_u64)1000)


/*i
 * @brief Constant defining the maximum number of milliseconds that can
 * be represented by a NANO_Time instance.
 */
#define NANO_MILLIS_MAX         ((NANO_u64)2147483648000)

/*i
 * @brief Constant defining the number of nanoseconds in a millisecond.
 * 
 * Used for conversion purposes.
 */
#define NANO_NSEC_IN_MILLIS \
    (((NANO_u64)NANO_NSEC_IN_SEC)/NANO_MILLIS_IN_SEC)

/*e
 * @brief A macro that can be used to initialize static values of NANO_Time.
 */
#define NANO_TIME_INITIALIZER   { 0, 0 }

/*e
 * @brief TODO
 * 
 */
#define NANO_TIME_INITIALIZER_INFINITE   { -1, 0 }

/*e
 * @brief Initialize a NANO_Time and make sure it is normalized.
 * 
 * @param self the instance to initialize.
 * @param sec the "seconds" component of the new instance.
 * @param nanosec the "nanoseconds" component of the new instance.
 * @return NANO_RETCODE_OK if the instance was initialized successfully, 
 * an error code from NANO_Time_normalize otherwise.
 */
NANODllExport
NANO_RetCode
NANO_Time_initialize(
    NANO_Time *const self,
    const NANO_i32 sec,
    const NANO_u32 nanosec);

/*i
 * @brief Compare a NANO_Time instance with another
 * 
 * All "infinte" instances are considered equal and greater than any "finite"
 * ones.
 * 
 * @param self the instance to check.
 * @param other the instance to compare.
 * @return 0 if the two instances are equal, a positive value if self is greater
 * than other, and a negative value otherwise.
 */
NANO_i8
NANO_Time_compare(const NANO_Time *const self, const NANO_Time *const other);

#define NANO_Time_compare(s_,o_) \
    ((NANO_Time_is_infinite((s_)))? \
        ((NANO_Time_is_infinite((o_)))? \
            /* both infinite */ 0 : /* s infinite, o finite */ 1 ) : \
        ((NANO_Time_is_infinite((o_)))? \
            /* s finite, o infinite */ -1 : \
            /* both finite */ \
            (((s_)->sec > (o_)->sec)? \
                /* s > o */ 1 : \
                (((s_)->sec < (o_)->sec)? \
                    /* s < o */ -1 : \
                    /* s.sec == o.sec */ \
                    (((s_)->nanosec > (o_)->nanosec)? \
                        /* s > o */ 1 : \
                        (((s_)->nanosec < (o_)->nanosec)? \
                            /* s < o */ -1 : \
                            /* s.nanosec == o.nanosec */ 0 \
                            ) \
                        ) \
                    ) \
                ) \
            ) \
        )

/*i
 * @brief Check if a NANO_Time instance is equal to another
 * 
 * Comparison is performed according to the semantics defined by
 * NANO_Time_compare.
 * 
 * @param self the instance to check.
 * @param other the instance to compare.
 * @return NANO_TRUE is the instances are equal, NANO_FALSE otherwise.
 */
NANODllExport
NANO_bool
NANO_Time_is_equal(const NANO_Time *const self, const NANO_Time *const other);

#define NANO_Time_is_equal(s_,o_) \
    (0 == NANO_Time_compare((s_),(o_)))

/*i
 * @brief Check if a NANO_Time instance is normalized.
 * 
 * @param self the instance to check.
 * @return NANO_TRUE is the instance is normalized, NANO_FALSE otherwise.
 */
NANO_bool
NANO_Time_is_normalized(const NANO_Time *const self);

#define NANO_Time_is_normalized(t_) \
    ((NANO_Time_is_infinite((t_)))? \
        ((t_)->sec == -1 && \
            (t_)->nanosec == 0) : \
        ((t_)->nanosec / NANO_NSEC_IN_SEC) == 0)

/*e
 * @brief Check if a NANO_Time instance is infinite
 * 
 * @param self the instance to check.
 * @return NANO_TRUE is the instance is infinite, NANO_FALSE otherwise.
 */
NANO_bool
NANO_Time_is_infinite(const NANO_Time *const self);

#define NANO_Time_is_infinite(t_) \
    ((t_)->sec < 0)

/*e
 * @brief Check if a NANO_Time instance is zero
 * 
 * @param self the instance to check.
 * @return NANO_TRUE is the instance is zero, NANO_FALSE otherwise.
 */
NANO_bool
NANO_Time_is_zero(const NANO_Time *const self);

#define NANO_Time_is_zero(t_) \
    (((t_)->sec == 0) && ((t_)->nanosec == 0))

/*e
 * @brief Generate a normalized version of a NANO_Time instance.
 * 
 * The source and destination instances must not be the same.
 * 
 * @param self  The instance to normalize.
 * @param norm_t_out  A normalized representation of the input instance.
 * @return NANO_RETCODE_OK if the instance was successfully normalized,
 * NANO_RETCODE_OVERFLOW_DETECTED if an overflow occurred.
 */
NANODllExport
NANO_RetCode
NANO_Time_normalize(const NANO_Time *const self, NANO_Time *const norm_t_out);

/*e
 * @brief Convert a NANO_Time instance to a seconds representation.
 * 
 * The returned value will be rounded up if the instance has a "nanoseconds"
 * component.
 * 
 * The instance must contain a "finite" time.
 * 
 * @param self The instance to convert.
 * @param sec_out The instance's time in seconds (rounded up)
 * @return NANO_RETCODE_OK if the instance was successfully converted,
 * NANO_PRECONDITION_NOT_MET if the instance does not contain a finite time.
 */
NANODllExport
NANO_RetCode
NANO_Time_to_sec(const NANO_Time *const self, NANO_u32 *const sec_out);

/*e
 * @brief Convert a NANO_Time instance to a milliseconds representation.
 * 
 * The returned value will be rounded up if the instance has a "nanoseconds"
 * component.
 * 
 * The instance must contain a "finite" time.
 * 
 * @param self The instance to convert.
 * @param millis_out The instance's time in seconds (rounded up)
 */
NANODllExport
void
NANO_Time_to_millis(const NANO_Time *const self, NANO_u64 *const millis_out);

void
NANO_Time_from_millis(const NANO_Time *const self, NANO_u64 millis);

#define NANO_Time_from_millis(s_,m_) \
{\
    (s_)->sec = (m_) / 1000;\
    (s_)->nanosec = ((m_) - ((s_)->sec * 1000)) * 1000000;\
}

#endif /* NANO_FEAT_TIME */

/** @} *//* nano_api_infr_time */

/******************************************************************************
 *                     OS-independent Network API
 ******************************************************************************/
/**
 * @addtogroup nano_api_infr_ipv4
 * @{
 */

/*e
 * @brief A data type to repreent IPv4 addresses.
 * 
 * The address is stored in network endianness (i.e. big-endian).
 * 
 */
typedef NANO_u32 NANO_Ipv4Addr;

/*e
 * @brief Generate an IPv4 address (as a 32-bit unsigned integer in network
 * endianness) from individual octet components.
 * 
 * @param self The IP address to set.
 * @param b0  The first (most significant) octet of the address.
 * @param b1  The second octet of the address.
 * @param b2  The third octet of the address.
 * @param b3  The fourth (least significant) octet of the address.
 */
NANODllExport
void
NANO_OSAPI_Ipv4Addr_from_octets(
    NANO_Ipv4Addr *const self,
    const NANO_u8 b0,
    const NANO_u8 b1,
    const NANO_u8 b2,
    const NANO_u8 b3);

#define NANO_OSAPI_Ipv4Addr_from_octets(s_,b0_,b1_,b2_,b3_) \
{\
   *(s_) = ((NANO_Ipv4Addr)\
                (((NANO_u32)b0_)  | \
                 ((NANO_u32)b1_) << 8 | \
                 ((NANO_u32)b2_) << 16 | \
                 ((NANO_u32)b3_) << 24));\
}

/*e
 * @brief 
 * 
 * @param self 
 * @return NANO_bool 
 */
NANO_bool
NANO_OSAPI_Ipv4Addr_is_multicast(const NANO_Ipv4Addr * const self);

#define NANO_OSAPI_Ipv4Addr_is_multicast(s_) \
    ((((NANO_u8*)(s_))[0] & 0xf0) == 0xE0)

/*e
 * @brief TODO
 * 
 * @param self 
 * @return NANO_bool 
 */
NANO_bool
NANO_OSAPI_Ipv4Addr_is_loopback(const NANO_Ipv4Addr * const self);

#define NANO_OSAPI_Ipv4Addr_is_loopback(s_) \
    (((NANO_u8*)(s_))[0] == 0x7F)

/** @} *//* nano_api_infr_ipv4 */

/*****************************************************************************
 *                        OS-specific Network API
 *****************************************************************************/

#ifdef DOCUMENTATION_ONLY

/**
 * @addtogroup nano_api_platimpl_net
 * @{
 */

/*e
 * @ingroup nanocore_api_cfg_impl_flags
 * @brief Feature flag that should be defined if the target network stack
 * supports DNS name resolution.
 */
#define NANO_HAVE_DNS

/*e
 * @ingroup nanocore_api_cfg_impl_flags
 * @brief Feature flag that should be defined if the target network stack
 * supports listing of available network interfaces.
 */
#define NANO_HAVE_IFCONFIG

/*e
 * @ingroup nanocore_api_cfg_impl_flags
 * @brief Feature flag that should be defined if the target network stack
 * supports IPv4 communications.
 */
#define NANO_HAVE_IPV4

/*e
 * @ingroup nanocore_api_cfg_impl_flags
 * @brief Feature flag that should be defined if the target network stack
 * supports UDPv4 communication.
 */
#define NANO_HAVE_UDPV4

/*i
 * @brief Convert an IPv4 address in dotted notation into a 32-bit unsigned
 * integer in network endianness (i.e. big-endian).
 * 
 * @param self   The IP address to set.
 * @param ip_str The IPv4 address string to convert.
 * @return NANO_RETCODE_OK if the string was correctly converted.
 */
NANO_RetCode
NANO_OSAPI_Ipv4Addr_from_str(
    NANO_Ipv4Addr *const self,
    const char *const ip_str);

/*i
 * @brief Resolve a DNS domain name to an IPv4 address.
 * 
 * @param self   The IP address to set.
 * @param addr_str The DNS address string to convert.
 * @return NANO_RETCODE_OK if the string was correctly converted.
 */
NANO_RetCode
NANO_OSAPI_Ipv4Addr_resolve(
    NANO_Ipv4Addr *const self,
    const char *const addr_str);

/*i
 * @brief Generate a list of IPv4 addresses for the host's available
 * network interfaces.
 * 
 * @param addrs     An array of Ipv4Addr values which will be filled with
 *                  addresses from the host's NICs.
 * @param addrs_len The maximum number of addresses that can be stored in
 *                  `addrs`, and the actual number of addresses that were
 *                  filled in upon success.
 * @return NANO_RETCODE_OK if one or more addresses were successfully stored
 * in the provided array.
 */
NANO_RetCode
NANO_OSAPI_Ipv4Addr_list(
    NANO_Ipv4Addr *const addrs,
    NANO_usize *const addrs_len);

/** @} *//* nano_api_platform_net */

/**
 * @addtogroup nano_api_platform_net_udp
 * @{
 */


/*e
 * @brief A representation of a BSD-like UDPv4 socket for fictional OS "Foo".
 */
typedef struct NANODllExport NANO_OSAPI_FooUdpv4SocketI
{
    /*e
     * @brief TODO
     * 
     */
    int fd;
} NANO_OSAPI_FooUdpv4Socket;

/*e
 * @brief A static initializer for instances of NANO_OSAPI_FooUdpv4Socket.
 * 
 */
#define NANO_OSAPI_FOOUDPV4SOCKET_INITIALIZER \
{\
    -1, /* fd */ \
}

/*e
 * @brief 
 * 
 */
typedef struct NANODllExport NANO_OSAPI_FooUdpv4SocketPropertiesI
{
    /*e
     * @brief TODO
     * 
     */
    NANO_usize send_buffer_size;
} NANO_OSAPI_FooUdpv4SocketProperties;

/*e
 * @brief 
 * 
 */
#define NANO_OSPAI_FOOUDPV4SOCKETPROPERTIES_INITIALIZER \
{\
    0 /* send_buffer_size */ \
}

/*e
 * @brief An alias for the OS-specific UDPv4 socket representation.
 */
#define NANO_OSAPI_Udpv4Socket              NANO_OSAPI_FooUdpv4Socket

/*e
 * @brief TODO
 * 
 */
#define NANO_OSAPI_Udpv4SocketProperties    NANO_OSAPI_FooUdpv4SocketProperties

/*e
 * @brief TODO
 * 
 */
#define NANO_OSAPI_UDPV4SOCKETPROPERTIES_INITIALIZER \
        NANO_OSPAI_FOOUDPV4SOCKETPROPERTIES_INITIALIZER

/*e
 * @brief An alias for the OS-specific static initializer for UDPv4 socket
 * representations.
 */
#define NANO_OSAPI_UDPV4SOCKET_INITIALIZER \
        NANO_OSAPI_FOOUDPV4SOCKET_INITIALIZER

/*e
 * @brief Initialize and open a UDPv4 socket.
 * 
 * @param self The socket to initialize.
 * @param addr Address for the new socket (in network order).
 * @param port UDP port for the new socket.
 * @param properties
 * @return NANO_RETCODE_OK if the socket was correctly initialized.
 */
NANO_RetCode
NANO_OSAPI_Udpv4Socket_open(
    NANO_OSAPI_Udpv4Socket *const self,
    const NANO_u8 *const addr,
    const NANO_u16 port,
    const NANO_OSAPI_Udpv4SocketProperties *const properties);

/*e
 * @brief Send a message over a UDPv4 socket.
 * 
 * @param self The socket that will be used to write the message.
 * @param dest_addr Address where the message will be sent to.
 * @param dest_port UDP port where the message will be sent to.
 * @param msg   Buffer to be written out in a UDP message.
 * @return NANO_RETCODE_OK if the message was correctly sent.
 */
NANO_RetCode
NANO_OSAPI_Udpv4Socket_send(
    NANO_OSAPI_Udpv4Socket *const self,
    const NANO_u8 *const dest_addr,
    const NANO_u16 dest_port,
    const NANO_MessageBuffer *msg);


/*e
 * @brief Receive a message over a UDPv4 socket.
 * 
 * @param self The socket that will be used to read the message.
 * @param src_addr Source address for the message.
 * @param src_port source UDP port for the message.
 * @param msg A buffer where the incoming message will be copied to.
 * @param msg_size Size of the received message in bytes.
 * the received message.
 * @return NANO_RETCODE_OK if a message was correctly received.
 */
NANO_RetCode
NANO_OSAPI_Udpv4Socket_recv(
    NANO_OSAPI_Udpv4Socket *const self,
    NANO_u8 *const src_addr,
    NANO_u16 *const src_port,
    NANO_MessageBuffer *msg,
    NANO_usize *const msg_size);

/*e
 * @brief Close an existing UDPv4 socket.
 * 
 * @param self The socket to close
 * @return NANO_RETCODE_OK if socket was correctly closed.
 */
NANO_RetCode
NANO_OSAPI_Udpv4Socket_close(NANO_OSAPI_Udpv4Socket *const self);

/** @} *//* nano_api_platform_net_udp */

/**
 * @addtogroup nano_api_platform_serial
 * @{
 */

/*e
 * @ingroup nanocore_api_cfg_impl_flags
 * @brief Feature flag that should be defined if the target network stack
 * supports serial communication.
 */
#define NANO_HAVE_SERIAL

/** @} *//* nano_api_platform_serial*/

/**
 * @addtogroup nano_api_platform_serial_conn
 * @{
 */

/*e
 * @brief A representation of a serial connection for OS "Foo".
 */
typedef struct NANODllExport NANO_OSAPI_FooSerialConnectionI
{
    /*e
     * @brief TODO
     * 
     */
    int fd;
} NANO_OSAPI_FooSerialConnection;


/*e
 * @brief A static initializer for instances of
 * NANO_OSAPI_FooSerialConnection.
 * 
 */
#define NANO_OSAPI_FOOSERIALCONNECTION_INITIALIZER \
{\
    -1, /* fd */ \
}

/*e
 * @brief An alias for the OS-specific serial connection representation.
 */
#define NANO_OSAPI_SerialConnection        NANO_OSAPI_FooSerialConnection

/*e
 * @brief An alias for the OS-specific static initializer for UDPv4 socket
 * representations.
 */
#define NANO_OSAPI_SERIALCONNECTION_INITIALIZER \
        NANO_OSAPI_FOOSERIALCONNECTION_INITIALIZER

/*e
 * @brief Initialize a serial connection.
 * 
 * @param self The connection to initialize.
 * @param properties
 * @return NANO_RETCODE_OK if the connection was correctly initialized.
 */
NANO_RetCode
NANO_OSAPI_SerialConnection_open(
    NANO_OSAPI_SerialConnection *const self,
    const NANO_OSAPI_SerialConnectionProperties *const properties);

/*e
 * @brief Send a message over a serial connection.
 * 
 * @param self The connection that will be used to write the message.
 * @param data   Buffer to be written out.
 * @param data_len Length of the buffer to be written out.
 * @return NANO_RETCODE_OK if the message was correctly sent.
 */
NANO_RetCode
NANO_OSAPI_SerialConnection_write(
    NANO_OSAPI_SerialConnection *const self,
    const NANO_u8 *const data,
    const NANO_usize data_len);


/*e
 * @brief Receive a message over a serial connection.
 * 
 * @param self The connection that will be used to read the message.
 * @param data A buffer where the incoming message will be copied to.
 * @param data_len Total length of the supplied buffer, and actual length of
 * the received message.
 * @return NANO_RETCODE_OK if a message was correctly received.
 */
NANO_RetCode
NANO_OSAPI_SerialConnection_read(
    NANO_OSAPI_SerialConnection *const self,
    NANO_u8 *const data,
    NANO_usize *const data_len);

/*e
 * @brief Close an existing serial connection.
 * 
 * @param self The serial connection to close.
 * @return NANO_RETCODE_OK if connection was correctly closed.
 */
NANO_RetCode
NANO_OSAPI_SerialConnection_close(NANO_OSAPI_SerialConnection *const self);

/** @} *//* nano_api_platform_net_serial_conn */

/*e
 * @ingroup nano_api_libcfg_headers
 * @brief Header file containing definitions for the target network stack.
 */
#define NANO_IPNET_DEF_H

/*e
 * @ingroup nano_api_libcfg_headers
 * @brief Header file containing definitions for the target serial stack.
 */
#define NANO_SERIAL_DEF_H

#else /* !DOCUMENTATION_ONLY */

#ifndef NANO_IPNET_DEF_H
#if defined(NANO_HAVE_SOCKET)
    /* An implementation for platforms which supports BSD sockets */
    #define NANO_IPNET_DEF_H "nano/nano_core_osapi_net_socket.h"
    #include NANO_IPNET_DEF_H
#elif defined(NANO_HAVE_LWIP)
    /* An implekentation using Lwip network stack */
    #define NANO_IPNET_DEF_H "nano/nano_core_osapi_net_lwip.h"
    #include NANO_IPNET_DEF_H
#elif defined(NANO_HAVE_SOCKET_WIN)
    #define NANO_IPNET_DEF_H "nano/nano_core_osapi_net_win.h"
    #include NANO_IPNET_DEF_H
#elif 0
    #warning "Please provide a network stack for your custom platform by defining NANO_IPNET_DEF_H"
#endif /* NANO_PLATFORM */
#else
    #include NANO_IPNET_DEF_H
#endif /* NANO_IPNET_DEF_H */


#if NANO_FEAT_TRANSPORT_IPV4 && \
    !defined(NANO_HAVE_IPV4)
#error "Selected network stack doesn't support IPv4 communications"
#endif

#if NANO_FEAT_TRANSPORT_IPV6 && \
    !defined(NANO_HAVE_IPV6)
#error "Selected network stack doesn't support IPv6 communications"
#endif

#if NANO_FEAT_TRANSPORT_RESOLVE && \
    NANO_FEAT_TRANSPORT_IP && \
    defined(NANO_HAVE_DNS)

#if NANO_FEAT_TRANSPORT_IPV4
#ifndef NANO_OSAPI_Ipv4Addr_resolve
#error "Required function not defined: NANO_OSAPI_Ipv4Addr_resolve"
#endif /* NANO_OSAPI_Ipv4Addr_resolve */
#endif /* NANO_FEAT_TRANSPORT_IPV4 */

#endif /* NANO_FEAT_TRANSPORT_RESOLVE && \
          NANO_FEAT_TRANSPORT_IP && \
          defined(NANO_HAVE_DNS) */

#if NANO_FEAT_TRANSPORT_AUTOCFG && \
    NANO_FEAT_TRANSPORT_IP && \
    defined(NANO_HAVE_IFCONFIG)

#if NANO_FEAT_TRANSPORT_IPV4
#ifndef NANO_OSAPI_Ipv4Addr_list
#error "Required function not defined: NANO_OSAPI_Ipv4Addr_list"
#endif /* NANO_OSAPI_Ipv4Addr_list */
#endif /* NANO_FEAT_TRANSPORT_IPV4 */

#endif /* NANO_FEAT_TRANSPORT_AUTOCFG && \
          NANO_FEAT_TRANSPORT_IP && \
          defined(NANO_HAVE_IFCONFIG) */

#if NANO_FEAT_TRANSPORT_IPV4
#if NANO_FEAT_TRANSPORT_STRING
#ifndef NANO_OSAPI_Ipv4Addr_from_str
#error "Required function not defined: NANO_OSAPI_Ipv4Addr_from_str"
#endif /* NANO_OSAPI_Ipv4Addr_from_str */
#endif /* NANO_FEAT_TRANSPORT_STRING */
#endif /* NANO_FEAT_TRANSPORT_IPV4 */

#ifdef NANO_HAVE_UDPV4

#if NANO_FEAT_AGENT
#ifndef NANO_HAVE_MULTICAST
#error "Target platform doesn't support multicast (required by Agent)"
#endif /* NANO_HAVE_MULTICAST */
#endif /* NANO_FEAT_AGENT */

#ifndef NANO_OSAPI_Udpv4SocketProperties
#error "Required type not defined: NANO_OSAPI_Udpv4SocketProperties"
#endif /* NANO_OSAPI_Udpv4SocketProperties */
#ifndef NANO_OSAPI_UDPV4SOCKETPROPERTIES_INITIALIZER
#error "Required macro not defined: NANO_OSAPI_UDPV4SOCKETPROPERTIES_INITIALIZER"
#endif /* NANO_OSAPI_UDPV4SOCKETPROPERTIES_INITIALIZER */
#ifndef NANO_OSAPI_Udpv4SocketProperties_set_multicast
#error "Required function not defined: NANO_OSAPI_Udpv4SocketProperties_set_multicast"
#endif /* NANO_OSAPI_Udpv4SocketProperties_set_multicast */
#ifndef NANO_OSAPI_Udpv4Socket
#error "Required type not defined: NANO_OSAPI_Udpv4Socket"
#endif /* NANO_OSAPI_Udpv4Socket */
#ifndef NANO_OSAPI_UDPV4SOCKET_INITIALIZER
#error "Required macro not defined: NANO_OSAPI_UDPV4SOCKET_INITIALIZER"
#endif /* NANO_OSAPI_UDPV4SOCKET_INITIALIZER */
#ifndef NANO_OSAPI_Udpv4Socket_open
#error "Required function not defined: NANO_OSAPI_Udpv4Socket_open"
#endif /* NANO_OSAPI_Udpv4Socket_open */
#ifndef NANO_OSAPI_Udpv4Socket_send
#error "Required function not defined: NANO_OSAPI_Udpv4Socket_send"
#endif /* NANO_OSAPI_Udpv4Socket_send */
#ifndef NANO_OSAPI_Udpv4Socket_recv
#error "Required function not defined: NANO_OSAPI_Udpv4Socket_recv"
#endif /* NANO_OSAPI_Udpv4Socket_recv */
#ifndef NANO_OSAPI_Udpv4Socket_close
#error "Required function not defined: NANO_OSAPI_Udpv4Socket_close"
#endif /* NANO_OSAPI_Udpv4Socket_close */
#ifndef NANO_OSAPI_Udpv4Socket_is_valid
#error "Required function not defined: NANO_OSAPI_Udpv4Socket_is_valid"
#endif /* NANO_OSAPI_Udpv4Socket_is_valid */
#else /* !NANO_HAVE_UDPV4 */
#if NANO_FEAT_TRANSPORT_PLUGIN_UDPV4
#error "Selected network stack doesn't support UDPv4 communication."
#endif /* NANO_FEAT_TRANSPORT_PLUGIN_UDPV4 */
#endif /* NANO_HAVE_UDPV4 */


#if NANO_FEAT_TRANSPORT_PLUGIN_SERIAL

#ifndef NANO_SERIAL_DEF_H
#if defined(NANO_HAVE_STD_SERIAL)
    /* An implementation for platforms which supports termios.h */
    #define NANO_SERIAL_DEF_H "nano/nano_core_osapi_serial_std.h"
#elif defined(NANO_HAVE_ARDUINO_SERIAL)
    /* An implementation using Arduino's Serial API */
    #define NANO_SERIAL_DEF_H "nano/nano_core_osapi_serial_arduino.h"
#elif defined(NANO_HAVE_WIN_SERIAL)
    /* An implementation using Win32 API */
    #define NANO_SERIAL_DEF_H "nano/nano_core_osapi_serial_win.h"
#else
    #error "Please select a serial stack by defining NANO_SERIAL_DEF_H"
#endif /* NANO_PLATFORM */
#endif /* NANO_IPNET_DEF_H */

#include NANO_SERIAL_DEF_H

#ifndef NANO_OSAPI_SerialConnection
#error "Required type not defined: NANO_OSAPI_SerialConnection"
#endif /* NANO_OSAPI_SerialConnection */
#ifndef NANO_OSAPI_SERIALCONNECTION_INITIALIZER
#error "Required macro not defined: NANO_OSAPI_SERIALCONNECTION_INITIALIZER"
#endif /* NANO_OSAPI_SERIALCONNECTION_INITIALIZER */
#ifndef NANO_OSAPI_SerialConnectionProperties
#error "Required type not defined: NANO_OSAPI_SerialConnectionProperties"
#endif /* NANO_OSAPI_SerialConnectionProperties */
#ifndef NANO_OSAPI_SERIALCONNECTIONPROPERTIES_INITIALIZER
#error "Required macro not defined: NANO_OSAPI_SERIALCONNECTIONPROPERTIES_INITIALIZER"
#endif /* NANO_OSAPI_SERIALCONNECTIONPROPERTIES_INITIALIZER */
#ifndef NANO_OSAPI_SerialConnection_open
#error "Required function not defined: NANO_OSAPI_SerialConnection_open"
#endif /* NANO_OSAPI_SerialConnection_open */
#ifndef NANO_OSAPI_SerialConnection_close
#error "Required function not defined: NANO_OSAPI_SerialConnection_close"
#endif /* NANO_OSAPI_SerialConnection_close */
#ifndef NANO_OSAPI_SerialConnection_write
#error "Required function not defined: NANO_OSAPI_SerialConnection_write"
#endif /* NANO_OSAPI_SerialConnection_send */
#ifndef NANO_OSAPI_SerialConnection_read
#error "Required function not defined: NANO_OSAPI_SerialConnection_read"
#endif /* NANO_OSAPI_SerialConnection_recv */

#endif /* NANO_FEAT_TRANSPORT_PLUGIN_SERIAL */

#endif /* DOCUMENTATION_ONLY */

/******************************************************************************
 *                              Memory Helpers
 ******************************************************************************/
/**
 * @addtogroup nano_api_infr_memory
 * @{
 */
/*i
 * @brief Determine the memory offset of the member of a struct type.
 */
#define NANO_OSAPI_MEMBER_OFFSET(type_,member_) \
        ((NANO_u8*)(&((type_*)NULL)->member_) - (NANO_u8*)NULL)

/*i
 * @brief Determine the memory offset of the member of a struct type from
 * the start of the struct.
 */
#define NANO_OSAPI_BASE_FROM_MEMBER(ptr_,type_,member_) \
        (type_*)((NANO_u8*)(ptr_) - NANO_OSAPI_MEMBER_OFFSET(type_,member_))


#define NANO_MEMBER(s_,m_) \
    (((s_) == NULL)? NULL : &(s_)->m_)

/*e
 * @brief Increase a "size" value to the next greater or equal value that
 * is also properly align at the specified boundary.
 * 
 * E.g.:
 * 
 * - size = 1, alignment = 8, return = 8
 * - size = 4, alignment = 2, return = 4
 * 
 * @param size The value to align.
 * @param alignment The desired alignment.
 * @return A value that is greater or equal to size and it is aligned at the
 * specified boundary.
 */
NANO_usize
NANO_OSAPI_Memory_align_size_up(NANO_usize size, NANO_usize alignment);
    
#define NANO_OSAPI_Memory_align_size_up(s_, a_) \
    ((NANO_usize) \
        ((((NANO_usize)(s_)) + (((NANO_usize)(a_)) - ((NANO_usize)1U))) & \
        ~(((NANO_usize)(a_)) - ((NANO_usize)1U)))\
    )


const NANO_u8*
NANO_OSAPI_Memory_align(const NANO_u8 *ptr, NANO_u8 alignment);

#define NANO_OSAPI_Memory_align(p_,t_) \
    NANO_OSAPI_Memory_align_ptr((p_),xrce_str_concat(t_,_alignment))


/*e
 * @brief TODO
 * 
 * @param ptr 
 * @param alignment 
 * @return const NANO_u8* 
 */
const NANO_u8*
NANO_OSAPI_Memory_align_ptr(const NANO_u8 *ptr, NANO_u8 alignment);

#define NANO_OSAPI_Memory_align_ptr(p_,a_) \
((NANO_u8*) \
    (\
        ((NANO_usize)(p_) + (NANO_usize)(a_) - (NANO_usize)1U) & \
        ~((NANO_usize)(a_) - (NANO_usize)1U) \
    ) \
)

NANO_bool
NANO_OSAPI_Memory_aligned(const NANO_u8 *ptr, NANO_u8 alignment);

#define NANO_OSAPI_Memory_aligned(p_,a_) \
    (NANO_OSAPI_Memory_align_ptr((p_),(a_)) == ((NANO_u8*)(p_)))



/** @} *//* nano_api_infr_memory */

/******************************************************************************
 *                                  Sequence
 ******************************************************************************/
#if NANO_FEAT_SEQUENCE
/*i
 * @addtogroup nano_api_infr_seq
 * @{
 */


typedef NANO_u8 NANO_SequenceFlags;

#define NANO_SEQUENCEFLAGS_DEFAULT          (0)
#define NANO_SEQUENCEFLAGS_SERIALIZED       (0x01 << 0)
#define NANO_SEQUENCEFLAGS_ENDIANNESS       (0x01 << 1)

/*i
 * @brief A dynamic container of homogenous elements.
 * 
 * A sequence provides an interface to manage a (possibly) dynamic buffer
 * of elements of the same type. Buffers have a "maximum" number of elements
 * that can be stored, and a "length" which indicates the number of elements
 * currently stored within them.
 * 
 * A sequence provides methods to inspect the size of its buffer, and to
 * access elements within the buffer by their index.
 * 
 * Nano Core tries to avoid any dynamic allocation, and this is reflected by
 * the sequence API: users must provide a buffer to the sequence
 * using NANO_Sequence_set_contiguous_buffer.
 * 
 * For example, to define (and test) a sequence of octets:
 * 
 * ```c
 * NANO_u8 buffer[1024],
 *         *b_ref = NULL;
 * NANO_Sequence seq = NANO_SEQUENCE_INITIALIZER;
 * assert_false(NANO_Sequence_initialized(seq));
 * assert_true(0 == NANO_Sequence_maximum(seq));
 * assert_true(0 == NANO_Sequence_length(seq));
 * assert_true(0 == NANO_Sequence_contiguous_buffer(seq));
 * assert_true(
 *     NANO_RETCODE_OK == NANO_Sequence_initialize(&seq,1));
 * assert_false(NANO_Sequence_initialized(seq));
 * assert_true(
 *     NANO_RETCODE_OK == 
 *         NANO_Sequence_set_contiguous_buffer(&seq, buffer, 1024, 0));
 * assert_true(1024 == NANO_Sequence_maximum(&seq));
 * assert_true(0 == NANO_Sequence_length(&seq));
 * assert_true(buffer == NANO_Sequence_contiguous_buffer(&seq));
 * 
 * assert_true(NULL == NANO_Sequence_reference(&seq,0));
 * assert_true(
 *     NANO_RETCODE_OK == NANO_Sequence_set_length(&seq,1));
 * b_ref = NANO_Sequence_reference(&seq,0);
 * assert_true(b_ref = &buffer[0])
 * ```
 */
typedef struct NANODllExport NANO_SequenceI
{
    const NANO_u8* buffer;
    NANO_usize len;
    NANO_usize max;
    NANO_usize el_size;
    NANO_u8 flags;
} NANO_Sequence;

/*i
 * @brief A static initializer for NANO_Sequence instances.
 */
#define NANO_SEQUENCE_INITIALIZER \
{\
    NULL, /* buffer */ \
    0, /* len */ \
    0, /* max */ \
    0, /* el_size */ \
    0  /* flags */ \
}

/*i
 * @brief A constant identifying the "basic" API for sequences.
 * 
 * Sequences defined with this API level will include the following
 * operations:
 * 
 * - TSeq_initialize
 * - TSeq_initialized
 * - TSeq_as_seq
 * - TSeq_as_seq_mut
 * 
 */
#define NANO_SEQUENCE_API_LEVEL_BASIC           0

/*i
 * @brief A constant identifying the "typed" API for sequences.
 * 
 * Sequences defined with this API level will include all operations defined
 * by the "basic" level, and the following ones:
 * 
 * - TSeq_reference
 * - TSeq_reference_mut
 * - TSeq_length
 * - TSeq_set_length
 * - TSeq_maximum
 * - TSeq_set_contiguous_buffer
 * - TSeq_contiguous_buffer
 * - TSeq_contiguous_buffer_mut
 * 
 */
#define NANO_SEQUENCE_API_LEVEL_TYPED           1

#define NANO_SEQUENCE_API_LEVEL_DEFAULT         NANO_SEQUENCE_API_LEVEL_BASIC

/*i
 * @brief Initialize a NANO_Sequence instance.
 * 
 * The sequence must have not been already initialized.
 * 
 * @param self The sequence to initialize.
 * @param el_size The size of each element in the buffer.
 * @return NANO_RETCODE_OK if the sequence was successfully initialized,
 * NANO_RETCODE_INVALID_ARGS if the supplied arguments are invalid.
 */
NANODllExport
void
NANO_Sequence_initialize(
    NANO_Sequence *const self,
    const NANO_usize el_size);

#define NANO_Sequence_initialize(s_,es_) \
{\
    NANO_PCOND((es_) == 0 || (es_) & 0x01)\
    (s_)->buffer = NULL;\
    (s_)->len = 0;\
    (s_)->max = 0;\
    (s_)->el_size = (es_);\
}

/*i
 * @brief Check if a sequence has been initialized.
 * 
 * @param self The sequence to inspect.
 * @return NANO_BOOL_TRUE if the sequence has been initialized, NANO_BOOL_FALSE
 * otherwise.
 */
NANO_bool
NANO_Sequence_initialized(const NANO_Sequence *const self);

#define NANO_Sequence_initialized(s_) \
    (NANO_Sequence_element_size(s_) > 0)

/*i
 * @brief Set the current length of a sequence.
 * 
 * The specified length must less or equal to the current maximum.
 * 
 * @param self The sequence to modify.
 * @param len The new length of the sequence.
 * @return NANO_RETCODE_OK if the length was set successfully,
 * NANO_RETCODE_INVALID_ARGS if the length exceeds the sequence's maximum.
 */
NANO_RetCode
NANO_Sequence_set_length(NANO_Sequence *const self, const NANO_usize len);

#define NANO_Sequence_set_length(s_,l_) \
    ((!NANO_Sequence_initialized((s_)))? \
        NANO_RETCODE_PRECONDITION_NOT_MET : \
        ((NANO_Sequence_maximum((s_)) < (l_))? \
            NANO_RETCODE_INVALID_ARGS : \
            ((s_)->len = (l_), NANO_RETCODE_OK)\
        ) \
    )
/*i
 * @brief Retrieve the current length of a sequence.
 * 
 * @param self The sequence to inspect.
 * @return The total number of elements currently stored in the sequence's
 * buffer. This is an upper bound on the indices that can be passed to
 * NANO_Sequence_reference.
 */
NANO_usize
NANO_Sequence_length(const NANO_Sequence *const self);

#define NANO_Sequence_length(s_) \
    ((NANO_Sequence_initialized(s_))?(s_)->len:0)

/*i
 * @brief Retrieve the size in bytes of each element stored by a sequence.
 * 
 * @param self 
 * @return NANO_usize 
 */
NANO_usize
NANO_Sequence_element_size(const NANO_Sequence *const self);

#define NANO_Sequence_element_size(s_) \
    ((s_)->el_size)

/*i
 * @brief Assign or retake the sequence’s backing buffer, and modify the 
 * sequence’s maximum.
 * 
 * @param self The sequence to modify.
 * @param buffer The sequence's buffer.
 * @param max The maximum number of elements that can be stored in the buffer.
 * @param len The current number of valid elements in the buffer.
 * @return NANO_RETCODE_OK if the buffer was successfully set,
 * NANO_RETCODE_INVALID_ARGS if the specified arguments were invalid.
 */
NANODllExport
NANO_RetCode
NANO_Sequence_set_contiguous_buffer(
    NANO_Sequence *const self,
    NANO_u8 *const buffer,
    const NANO_usize max,
    const NANO_usize len);


#define NANO_Sequence_set_contiguous_buffer(s_, b_, m_, l_) \
{\
    NANO_PCOND((s_) != NULL)\
    NANO_PCOND(NANO_Sequence_initialized((s_)))\
    NANO_PCOND((b_) == NULL || (m_) > 0)\
    NANO_PCOND((b_) != NULL || (m_) == 0)\
    NANO_PCOND((l_) <= (m_))\
    (s_)->buffer = (const NANO_u8*)(b_);\
    (s_)->max = (m_);\
    (s_)->len = (l_);\
    NANO_Sequence_flags_unset((s_),\
        NANO_SEQUENCEFLAGS_SERIALIZED | NANO_SEQUENCEFLAGS_ENDIANNESS);\
}

/*i
 * @brief Similar to NANO_Sequence_set_contiguous_buffer but marks the sequence
 * as having a buffer with “serialized” elements.
 * 
 * The provided maximum is interpreted and stored as the “total serialized
 * size” of the buffer in bytes, rather than the maximum number of elements
 * that may be stored in the buffer.
 * 
 * @param self 
 * @param buffer 
 * @param max_serialized_size 
 * @param el_count 
 * @param little_endian 
 */
void
NANO_Sequence_set_serialized_buffer(
    NANO_Sequence *const self,
    NANO_u8 *const buffer,
    const NANO_usize max_serialized_size,
    const NANO_usize el_count,
    const NANO_bool little_endian);

#define NANO_Sequence_set_serialized_buffer(s_,b_,sz_,cnt_,le_) \
{\
    NANO_PCOND((s_) != NULL)\
    NANO_PCOND(NANO_Sequence_initialized((s_)))\
    NANO_PCOND((b_) == NULL || (sz_) > 0)\
    NANO_PCOND((b_) != NULL || (sz_) == 0)\
    NANO_PCOND((sz_) == 0 || (cnt_) > 0)\
    (s_)->buffer = (const NANO_u8*)(b_);\
    (s_)->max = (sz_);\
    (s_)->len = (cnt_);\
    NANO_Sequence_flags_set((s_), NANO_SEQUENCEFLAGS_SERIALIZED);\
    if ((le_)) \
    {\
        NANO_Sequence_flags_set((s_),NANO_SEQUENCEFLAGS_ENDIANNESS);\
    }\
}

#define NANO_Sequence_contiguous_bufferI(s_) \
    ((NANO_Sequence_initialized((s_)) && \
        !NANO_Sequence_flags((s_),NANO_SEQUENCEFLAGS_SERIALIZED))?\
            (s_)->buffer : NULL)

/*i
 * @brief Get a sequence's buffer in read mode.
 * 
 * @param self The sequence to inspect.
 * @return A pointer to the sequence's buffer or NULL if the sequence currently
 * has no buffer.
 */
const NANO_u8*
NANO_Sequence_contiguous_buffer(const NANO_Sequence *const self);

#define NANO_Sequence_contiguous_buffer(s_) \
    ((const NANO_u8*)NANO_Sequence_contiguous_bufferI(s_))

/*i
 * @brief Get a sequence's buffer in write mode.
 * 
 * @param self The sequence to inspect.
 * @return A pointer to the sequence's buffer or NULL if the sequence currently
 * has no buffer.
 */
NANO_u8*
NANO_Sequence_contiguous_buffer_mut(NANO_Sequence *const self);

#define NANO_Sequence_contiguous_buffer_mut(s_) \
    ((NANO_u8*)NANO_Sequence_contiguous_bufferI(s_))

/*i
 * @brief Retrieve the current maximum of a sequence.
 * 
 * @param self The sequence to inspect.
 * @return The total number of elements that can be stored in the sequence's
 * buffer. This is an upper bound to the length that can be passed to
 * NANO_Sequence_set_length.
 */
NANO_usize
NANO_Sequence_maximum(const NANO_Sequence *const self);

#define NANO_Sequence_maximum(s_) \
    ((NANO_Sequence_initialized(s_) && \
        !NANO_Sequence_flags((s_),NANO_SEQUENCEFLAGS_SERIALIZED))?(s_)->max:0)

NANO_usize
NANO_Sequence_serialized_size(const NANO_Sequence *const self);

#define NANO_Sequence_serialized_size(s_) \
    ((NANO_Sequence_initialized(s_) && \
        NANO_Sequence_flags((s_),NANO_SEQUENCEFLAGS_SERIALIZED))?(s_)->max:0)


#define NANO_Sequence_referenceI(s_, i_) \
   ((const NANO_u8*) \
    (\
        (NANO_Sequence_flags((s_),NANO_SEQUENCEFLAGS_SERIALIZED) || \
            ((i_)+1) > NANO_Sequence_length((s_)))? \
            NULL : \
            ((s_)->buffer + ((s_)->el_size * (i_)))\
    )\
   )

/*i
 * @brief Access an element from a sequence's buffer in read mode.
 * 
 * @param self The sequence to access.
 * @param i Index of the element to access
 * @return A pointer to the requested element, or NULL if an error occurred.
 */
const NANO_u8*
NANO_Sequence_reference(const NANO_Sequence *const self, const NANO_usize i);

#define NANO_Sequence_reference(s_,i_) \
    ((const NANO_u8*)NANO_Sequence_referenceI((s_),(i_)))

/*i
 * @brief Access an element from a sequence's buffer in read/write mode.
 * 
 * @param self The sequence to access.
 * @param i Index of the element to access
 * @return A pointer to the requested element, or NULL if an error occurred.
 */
NANO_u8*
NANO_Sequence_reference_mut(
    NANO_Sequence *const self,
    const NANO_usize i);

#define NANO_Sequence_reference_mut(s_,i_) \
    ((NANO_u8*)NANO_Sequence_referenceI((s_),(i_)))

/*i
 * @brief Check if one or more flags have been set on a sequence.
 * 
 * @param self 
 * @param flags 
 * @return NANO_bool 
 */
NANO_bool
NANO_Sequence_flags(
    NANO_Sequence *const self,
    const NANO_SequenceFlags flags);

#define NANO_Sequence_flags(s_,f_) \
    ((s_)->flags & (f_))

/*i
 * @brief Enable one or more flags on a sequence.
 * 
 * @param self 
 * @param flags 
 */
void
NANO_Sequence_flags_set(
    NANO_Sequence *const self,
    const NANO_SequenceFlags flags);

#define NANO_Sequence_flags_set(s_,f_) \
{\
    (s_)->flags |= (f_);\
}

/*i
 * @brief Disable one or more flags on a sequence.
 * 
 * @param self 
 * @param flags 
 */
void
NANO_Sequence_flags_unset(
    NANO_Sequence *const self,
    const NANO_SequenceFlags flags);

#define NANO_Sequence_flags_unset(s_,f_) \
{\
    (s_)->flags &= ~(f_);\
}

/*i @} *//* nano_api_infr_seq */

#if NANO_FEAT_TYPED_SEQUENCE

/*i
 * @addtogroup nano_api_infr_seq_impl
 * @{
 */

/*i
 * @brief TODO
 * 
 */
#define NANO_TSEQIMPL_INITIALIZER(el_) \
{\
    {\
        NULL, /* buffer */ \
        0, /* len */ \
        0, /* max */ \
        sizeof(el_), /* el_size */ \
        0 /* flags */\
    }, /* base */\
}

/*i
 * @brief Helper macro to implement typed version of
 * NANO_Sequence_initialize.
 */
#define NANO_TSeqImpl_initialize(s_, el_) \
    NANO_Sequence_initialize(&(s_)->base,sizeof(el_))
/*i
 * @brief Helper macro to implement typed version of
 * NANO_Sequence_set_contiguous_buffer.
 */
#define NANO_TSeqImpl_set_contiguous_buffer(s_,b_,m_,l_, tinit_) \
{\
    const el_ def_el_ = tinit_;\
    NANO_RetCode rc_ = NANO_RETCODE_ERROR;\
    rc_ = NANO_Sequence_set_contiguous_buffer(\
                &(s_)->base,(NANO_u8*)(b_),(m_),(l_));\
    if (rc_ == NANO_RETCODE_OK) \
    {\
        NANO_usize i_ = 0;\
        for (i_ = 0; i_ < (m_); i_++)\
        {\
            el_ *el_ref_ = ((el_*)(b_)) + (i_);\
            *el_ref_ = def_el_;\
        }\
    }\
    rc_;\
}

#define NANO_TSeqImpl_set_contiguous_buffer_primitive(s_,b_,m_,l_) \
{\
    NANO_RetCode rc_ = NANO_RETCODE_ERROR;\
    rc_ = NANO_Sequence_set_contiguous_buffer(\
                &(s_)->base,(NANO_u8*)(b_),(m_),(l_));\
    if (rc_ == NANO_RETCODE_OK) \
    {\
        NANO_OSAPI_Memory_zero((b_),(m_)*(s_)->base.el_size);\
    }\
    rc_;\
}

#define NANO_TSeqImpl_finalize(s_) \
    NANO_Sequence_set_contiguous_buffer(&(s_)->base,NULL,0,0)



#define NANO_TSeqImpl_set_serialized_buffer(s_,b_,sz_,cnt_,le_) \
    NANO_Sequence_set_serialized_buffer(&(s_)->base, (b_), (sz_), (cnt_), (le_))


#define NANO_TSeqImpl_to_iterator(s_,dfn_,i_) \
    NANO_Sequence_to_iterator(&(s_)->base,(dfn_),(i_))


/*i
 * @brief Helper macro to implement typed version of
 * NANO_Sequence_initialized.
 */
#define NANO_TSeqImpl_initialized(s_) \
    NANO_Sequence_initialized(&(s_)->base)

/*i
 * @brief Helper macro to implement typed version of
 * NANO_Sequence_as_seq.
 */
#define NANO_TSeqImpl_as_seq(s_) \
    (NANO_Sequence_initialized(&(s_)->base)?\
        &(s_)->base : NULL)

/*i
 * @brief Helper macro to implement typed version of
 * NANO_Sequence_as_seq_mut.
 */
#define NANO_TSeqImpl_as_seq_mut(s_) \
    (NANO_Sequence_initialized(&(s_)->base)?\
        &(s_)->base : NULL)

/*i
 * @brief Helper macro to implement typed version of
 * NANO_Sequence_reference.
 */
#define NANO_TSeqImpl_reference(s_,i_,el_) \
    ((const el_*)NANO_Sequence_reference(&(s_)->base,(i_)))

/*i
 * @brief Helper macro to implement typed version of
 * NANO_Sequence_reference_mut.
 */
#define NANO_TSeqImpl_reference_mut(s_,i_,el_) \
    ((el_*)NANO_Sequence_reference_mut(&(s_)->base,(i_)))

/*i
 * @brief Helper macro to implement typed version of
 * NANO_Sequence_contiguous_buffer.
 */
#define NANO_TSeqImpl_contiguous_buffer(s_,el_) \
    ((const el_*)NANO_Sequence_contiguous_buffer(&(s_)->base))

/*i
 * @brief Helper macro to implement typed version of
 * NANO_Sequence_contiguous_buffer_mut.
 */
#define NANO_TSeqImpl_contiguous_buffer_mut(s_,el_) \
    ((el_*)NANO_Sequence_contiguous_buffer_mut(&(s_)->base))

/*i
 * @brief Helper macro to implement typed version of
 * NANO_Sequence_maximum.
 */
#define NANO_TSeqImpl_maximum(s_) \
    NANO_Sequence_maximum(&(s_)->base)

/*i
 * @brief Helper macro to implement typed version of
 * NANO_Sequence_serialized_size.
 */
#define NANO_TSeqImpl_serialized_size(s_) \
    NANO_Sequence_serialized_size(&(s_)->base)

/*i
 * @brief Helper macro to implement typed version of
 * NANO_Sequence_length.
 */
#define NANO_TSeqImpl_length(s_) \
    NANO_Sequence_length(&(s_)->base)

/*i
 * @brief Helper macro to implement typed version of
 * NANO_Sequence_set_length.
 */
#define NANO_TSeqImpl_set_length(s_,l_) \
    NANO_Sequence_set_length(&(s_)->base,(l_))

/*i @} *//* nano_api_infr_seq_impl */

#endif /* NANO_FEAT_TYPED_SEQUENCE */

/*i
 * @addtogroup nano_api_infr_seq_iter
 * @{
 */

typedef struct NANODllExport NANO_SequenceIteratorI
    NANO_SequenceIterator;

typedef NANO_RetCode
    (*NANO_SequenceIterator_DeserializeElFn)(
        NANO_SequenceIterator *const self,
        const NANO_Sequence *const seq,
        void *const next_el);


struct NANO_SequenceIteratorI
{
    const NANO_Sequence *seq;
    const NANO_u8 *head;
    NANO_usize el_count;
    NANO_SequenceIterator_DeserializeElFn deserialize_el;
};

#define NANO_SEQUENCEITERATOR_INITIALIZER \
{\
    NULL, /* seq */\
    NULL, /* head */\
    0, /* el_count */\
    NULL /* deserialize_el */\
}

/*i
 * @brief Retrieve the total number of elements deserialized so far by the
 * iterator.
 * 
 * @param self 
 * @return NANO_usize 
 */
NANO_usize
NANO_SequenceIterator_deserialized_count(
    const NANO_SequenceIterator *const self);

#define NANO_SequenceIterator_deserialized_count(s_) \
    ((s_)->el_count)

/*i
 * @brief Retrieve the total amount of space (in bytes) in the sequence’s
 * buffer deserialized so far by the iterator.
 * 
 * @param self 
 * @return NANO_usize 
 */
NANO_usize
NANO_SequenceIterator_deserialized_size(
    const NANO_SequenceIterator *const self);

#define NANO_SequenceIterator_deserialized_size(s_) \
    (((s_)->seq != NULL)?\
      ((NANO_usize) ((s_)->head - (s_)->seq->buffer)):0)

/*i
 * @brief Retrieve the total amount of space (in bytes) left to deserialize in
 * the sequence’s buffer.
 * 
 * @param self 
 * @return NANO_usize 
 */
NANO_usize
NANO_SequenceIterator_remaining(
    const NANO_SequenceIterator *const self);

#define NANO_SequenceIterator_remaining(s_) \
    (((s_)->seq != NULL)?\
    ((NANO_usize)\
        ((s_)->seq->max - NANO_SequenceIterator_deserialized_size(s_))):0)

/*i
 * @brief Check if there is at least the specified amount of space remaining
 * in the sequence’s buffer.
 * 
 * @param self 
 * @param deserialize_size 
 * @return NANO_bool 
 */
NANO_bool
NANO_SequenceIterator_check_capacity(
    NANO_SequenceIterator *const self,
    const NANO_usize deserialize_size);

#define NANO_SequenceIterator_check_capacity(s_, sz_) \
    (NANO_SequenceIterator_remaining((s_)) >= (sz_))

/*i
 * @brief Advance the iterator’s cursor of the specified amount, “consuming”
 * the sequence’s buffer.
 * 
 * @param self 
 * @param deserialized_size 
 */
void
NANO_SequenceIterator_consume(
    NANO_SequenceIterator *const self,
    const NANO_usize deserialized_size);

#define NANO_SequenceIterator_consume(s_, sz_) \
{\
    NANO_PCOND(NANO_SequenceIterator_check_capacity((s_),(sz_)))\
    (s_)->head = (s_)->head + (sz_);\
}

/*i
 * @brief Check if there is at least the specified amount of space remaining
 * in the sequence’s buffer, after possibly advancing the iterator’s cursor to
 * be at the specified alignment.
 * 
 * @param self 
 * @param deserialize_size 
 * @param alignment 
 * @return NANO_bool 
 */
NANO_bool
NANO_SequenceIterator_check_capacity_align(
    NANO_SequenceIterator *const self,
    const NANO_usize deserialize_size,
    const NANO_usize alignment);

#define NANO_SequenceIterator_check_capacity_align(s_, dsz_, a_) \
    (NANO_SequenceIterator_align((s_),(a_)) && \
        NANO_SequenceIterator_check_capacity((s_),(dsz_)))

/*i
 * @brief Retrieve the iterator’s cursor.
 * 
 * @param self 
 * @return NANO_u8* 
 */
NANO_u8*
NANO_SequenceIterator_head(const NANO_SequenceIterator *const self);

#define NANO_SequenceIterator_head(s_) \
    ((s_)->head)


/*i
 * @brief Advance the iterator’s cursor until it’s address is at the requested
 * alignment.
 * 
 * @param self 
 * @param align 
 * @return NANODllExport NANO_SequenceIterator_align 
 */
NANODllExport
NANO_bool
NANO_SequenceIterator_align(
    NANO_SequenceIterator *const self, 
    const NANO_usize align);


/*i
 * @brief Invoke the iterator’s element deserialization function and try to
 * deserialize the next element in the sequence in user provided memory. 
 * 
 * @param self 
 * @param next 
 * @param has_next
 * @return 
 */
NANODllExport
NANO_RetCode
NANO_SequenceIterator_next(
    NANO_SequenceIterator *const self,
    void *const next,
    NANO_bool *const has_next);

NANO_RetCode
NANO_SequenceIterator_deserialize_element(
    NANO_SequenceIterator *const self,
    void *const next_el);

#define NANO_SequenceIterator_deserialize_element(s_,nxt_) \
    ((s_)->deserialize_el((s_), (s_)->seq, (nxt_)))

/*i
 * @brief Initialize a NANO_SequenceIterator from a sequence using the
 * specified element deserialization function.
 * 
 * @param self 
 * @param deserialize_el 
 * @param iter 
 */
void
NANO_Sequence_to_iterator(
    const NANO_Sequence *const self,
    NANO_SequenceIterator_DeserializeElFn deserialize_el,
    NANO_SequenceIterator *const iter);

#define NANO_Sequence_to_iterator(s_,dfn_,i_)\
{\
    NANO_PCOND(NANO_Sequence_flags((s_),NANO_SEQUENCEFLAGS_SERIALIZED))\
    (i_)->seq = (s_);\
    (i_)->head = (s_)->buffer;\
    (i_)->deserialize_el = (dfn_);\
}


/*i @} *//* nano_api_infr_seq_iter */

#endif /* NANO_FEAT_SEQUENCE */

/******************************************************************************
 *                                  OptionalMember
 ******************************************************************************/
#if NANO_FEAT_OPTIONAL

/*i
 * @addtogroup nano_api_infr_opt
 * @{
 */

/*i
 * @brief TODO
 * 
 */
#define NANO_OPTIONALMEMBER_DEFN(t_,f_) \
    t_ f_;\
    NANO_bool xrce_str_concat(has_,f_)

/*i
 * @brief TODO
 * 
 */
#define NANO_OPTIONALMEMBER_INITIALIZER(i_) \
    i_, \
    NANO_BOOL_FALSE


/*i
 * @brief TODO
 * 
 */
#define NANO_OPTIONALMEMBER_INITIALIZER_CPP(f_, i_) \
    f_(i_),\
    xrce_str_concat(has_,f_)(NANO_BOOL_FALSE)

/*i
 * @brief TODO
 * 
 */
#define NANO_OPTIONALMEMBER_IS_SET(s_,m_) \
    ((s_)->xrce_str_concat(has_,m_))

#define NANO_OPTIONALMEMBER_MARK_SET(s_,m_,set_) \
{\
    (s_)->xrce_str_concat(has_,m_) = (set_)? NANO_BOOL_TRUE : NANO_BOOL_FALSE; \
}

/*i
 * @brief TODO
 * 
 */
#define NANO_OPTIONALMEMBER_SET(s_,m_,v_) \
{\
    (s_)->m_ = (v_); \
    NANO_OPTIONALMEMBER_MARK_SET(s_,m_,NANO_BOOL_TRUE);\
}

/*i
 * @brief TODO
 * 
 */
#define NANO_OPTIONALMEMBER_CLEAR(s_,m_) \
    NANO_OPTIONALMEMBER_MARK_SET(s_,m_,NANO_BOOL_FALSE)

/*i
 * @brief TODO
 * 
 */
#define NANO_OPTIONALMEMBER_AS_REF(s_,m_) \
    (&((s_)->m_))

/*i
 * @brief TODO
 * 
 */
#define NANO_OPTIONALMEMBER_GET(s_,m_) \
    ((s_)->m_)

/*i @} *//* nano_api_infr_opt */

#endif /* NANO_FEAT_OPTIONAL */

/******************************************************************************
 *                              BufferPool
 ******************************************************************************/
#if NANO_FEAT_BUFFERPOOL

/*i
 * @addtogroup nano_api_infr_bufpool
 * @{
 */

typedef struct NANODllExport NANO_BufferHeaderI
{
    NANO_u16 next;
    NANO_u8 flags;
    NANO_u8 user_data;
} NANO_BufferPoolRecord;

#define NANO_BUFFERPOOLRECORD_INITIALIZERS \
{\
    0, /* next */ \
    0, /* flags */ \
    0 /* user_data */ \
}

#define NANO_BUFFERPOOLRECORD_ALIGN     ((NANO_usize)2)


#define NANO_BUFFERPOOLRECORDFLAGS_DEFAULT \
    NANO_BUFFERPOOLRECORDFLAGS_FREE

#define NANO_BUFFERPOOLRECORDFLAGS_FREE     (1 << 0)

NANO_bool
NANO_BufferPoolRecord_is_free(NANO_BufferPoolRecord *const self);

#define NANO_BufferPoolRecord_is_free(s_) \
    ((s_)->flags & NANO_BUFFERPOOLRECORDFLAGS_FREE)

void
NANO_BufferPoolRecord_allocated(NANO_BufferPoolRecord *const self);

#define NANO_BufferPoolRecord_allocated(s_) \
{\
    (s_)->flags &= ~NANO_BUFFERPOOLRECORDFLAGS_FREE;\
}

void
NANO_BufferPoolRecord_released(NANO_BufferPoolRecord *const self);

#define NANO_BufferPoolRecord_released(s_) \
{\
    (s_)->flags |= NANO_BUFFERPOOLRECORDFLAGS_FREE;\
}

typedef struct NANODllExport NANO_BufferPoolI
{
    NANO_u8 *buffer;
    NANO_usize buffer_len;
    NANO_usize el_size;
    NANO_u16 next;
    NANO_u8 el_align;
    NANO_u8 flags;


#if NANO_CPP

    NANO_BufferPoolI() {}

#endif /* NANO_CPP */
} NANO_BufferPool;

#define NANO_BUFFERPOOL_INITIALIZER \
{\
    NULL, /* buffer */\
    0, /* buffer_len */\
    0, /* el_size */\
    0, /* next */\
    0, /* el_align */\
    0  /* flags */\
}

/*i
 * @brief Initialize a NANO_BufferPool on a contiguous memory area to manage
 * as many buffers of the specified size and alignment that fit in it.
 * 
 * @param self 
 * @param el_size 
 * @param el_align 
 * @param buffer 
 * @param buffer_len 
 * @return
 */
NANODllExport
NANO_RetCode
NANO_BufferPool_initialize(
    NANO_BufferPool *const self,
    const NANO_usize el_size,
    const NANO_u8 el_align,
    NANO_u8 *buffer,
    const NANO_usize buffer_len);

/*i
 * @brief Allocate a buffer from the pool if available.
 * 
 * @param self 
 * @return
 */
NANODllExport
NANO_u8 *
NANO_BufferPool_allocate(NANO_BufferPool *const self);

/**
 * @brief Return a buffer to the pool.
 * 
 * @param self 
 * @param buffer 
 * @return NANODllExport NANO_BufferPool_release 
 */
NANODllExport
void
NANO_BufferPool_release(
    NANO_BufferPool *const self,
    NANO_u8 *const buffer);


#define NANO_BUFFERPOOLFLAGS_DEFAULT        (0x00)

#define NANO_SIZE_MAX(a_,b_) \
(\
    (((NANO_usize)(a_)) > ((NANO_usize)(b_)))?\
        (a_) : (b_)\
)

#define NANO_BUFFERPOOLRECORD_SIZE(el_size_,el_align_) \
    NANO_OSAPI_Memory_align_size_up(\
        (el_size_) + \
            NANO_SIZE_MAX((el_align_),sizeof(NANO_BufferPoolRecord)),\
        (el_align_))

#define NANO_BUFFERPOOL_SIZE(el_size_,el_align_,el_n_) \
    (((NANO_usize)(el_n_))*NANO_BUFFERPOOLRECORD_SIZE((el_size_),(el_align_)))

#define NANO_BUFFERPOOL_LENGTH(el_size_,el_align_,el_n_) \
    (NANO_BUFFERPOOL_SIZE((el_size_),(el_align_),(el_n_))/((NANO_usize)el_align_))

#define NANO_BUFFERPOOL_RECORD(s_, i_) \
    ((NANO_BufferPoolRecord*)\
            (NANO_BUFFERPOOL_BUFFER_HEAD(s_) + \
                (NANO_BUFFERPOOL_RECORD_SIZE(s_) * ((NANO_usize)i_))))

#define NANO_BUFFERPOOL_RECORD_FROM_BUFFER(s_, b_) \
    (NANO_BufferPoolRecord*)(((NANO_usize)(b_)) - \
        NANO_BUFFERPOOL_RECORD_HEADER_SIZE(s_))

#define NANO_BUFFERPOOL_BUFFER_FROM_RECORD(s_, r_) \
    ((NANO_u8*)r_) + NANO_BUFFERPOOL_RECORD_HEADER_SIZE(s_)

#define NANO_BUFFERPOOL_RECORD_INDEX(s_, r_) \
(\
    (((NANO_u8*)r_) == NANO_BUFFERPOOL_BUFFER_HEAD(s_) && 0) ?\
    0 : \
    ((\
        (((NANO_u8*)r_) - NANO_BUFFERPOOL_BUFFER_HEAD(s_)) / \
            NANO_BUFFERPOOL_RECORD_SIZE(s_) \
    ) - (NANO_usize)0) \
)

#define NANO_BUFFERPOOL_RECORD_ALIGN(s_) \
    NANO_SIZE_MAX(((NANO_usize)(s_)->el_align), NANO_BUFFERPOOLRECORD_ALIGN)

#define NANO_BUFFERPOOL_RECORD_HEADER_SIZE(s_) \
    NANO_SIZE_MAX(((NANO_usize)(s_)->el_align) , sizeof(NANO_BufferPoolRecord))

#define NANO_BUFFERPOOL_RECORD_SIZE(s_) \
    NANO_OSAPI_Memory_align_size_up(\
        NANO_BUFFERPOOL_RECORD_HEADER_SIZE(s_) + ((NANO_usize)(s_)->el_size),\
        NANO_BUFFERPOOL_RECORD_ALIGN(s_))

#define NANO_BUFFERPOOL_BUFFER_HEAD(s_) \
    ((NANO_u8*)NANO_OSAPI_Memory_align_size_up(\
        (s_)->buffer,\
        NANO_BUFFERPOOL_RECORD_ALIGN(s_)))

#define NANO_BUFFERPOOL_BUFFER_LENGTH(s_) \
    ((((NANO_usize)(s_)->buffer) + (s_)->buffer_len) - \
        (NANO_usize)NANO_BUFFERPOOL_BUFFER_HEAD(s_))

#define NANO_BUFFERPOOL_CAPACITY(s_) \
    (NANO_BUFFERPOOL_BUFFER_LENGTH(s_) / NANO_BUFFERPOOL_RECORD_SIZE(s_))

/*i @} *//* nano_api_infr_bufpool */

#endif /* NANO_FEAT_BUFFERPOOL */

/******************************************************************************
 *                              PropertyName
 ******************************************************************************/
#if NANO_FEAT_PROPERTY

/*i
 * @addtogroup nano_api_infr_property
 * @{
 */


typedef struct NANODllExport NANO_PropertyNameI
{
    char *value;
} NANO_PropertyName;

#define NANO_PROPERTYNAME_INITIALIZER \
{\
    NULL /* value */\
}


/******************************************************************************
 *                              PropertyValue
 ******************************************************************************/

typedef struct NANODllExport NANO_PropertyValueI
{
    char *value;
} NANO_PropertyValue;

#define NANO_PROPERTYVALUE_INITIALIZER \
{\
    NULL /* value */\
}

/******************************************************************************
 *                                Property
 ******************************************************************************/

typedef struct NANODllExport NANO_PropertyI
{
    NANO_PropertyName name;
    NANO_PropertyValue value;
} NANO_Property;

#define NANO_PROPERTY_INITIALIZER \
{\
    NANO_PROPERTYNAME_INITIALIZER, /* name */\
    NANO_PROPERTYVALUE_INITIALIZER /* value */\
}


/******************************************************************************
 *                                PropertySeq
 ******************************************************************************/
#if NANO_FEAT_TYPED_SEQUENCE

#define T           NANO_Property
#define TSeq        NANO_PropertySeq
#define TSeq_API    NANO_SEQUENCE_API_LEVEL_TYPED


#include "nano/nano_core_osapi_seq_decl.h"

#define NANO_PROPERTYSEQ_INITIALIZER \
        NANO_TSEQIMPL_INITIALIZER(NANO_Property)

#define NANO_PropertySeq_initialize(s_) \
    NANO_TSeqImpl_initialize(s_, NANO_Property)

#define NANO_PropertySeq_finalize(s_) \
    NANO_TSeqImpl_finalize(s_)

#define NANO_PropertySeq_set_contiguous_buffer(s_,b_,m_,l_) \
    NANO_TSeqImpl_set_contiguous_buffer(s_, b_, m_, l_, NANO_PROPERTY_INITIALIZER)

#define NANO_PropertySeq_initialized(s_) \
    NANO_TSeqImpl_initialized(s_)

#define NANO_PropertySeq_as_seq(s_) \
    NANO_TSeqImpl_as_seq(s_)
    
#define NANO_PropertySeq_as_seq_mut(s_) \
    NANO_TSeqImpl_as_seq_mut(s_)
    
#define NANO_PropertySeq_reference(s_,i_) \
    NANO_TSeqImpl_reference(s_,i_, NANO_Property)
    
#define NANO_PropertySeq_reference_mut(s_,i_) \
    NANO_TSeqImpl_reference_mut(s_,i_, NANO_Property)

#define NANO_PropertySeq_contiguous_buffer(s_) \
    NANO_TSeqImpl_contiguous_buffer(s_, NANO_Property)
    
#define NANO_PropertySeq_contiguous_buffer_mut(s_) \
    NANO_TSeqImpl_contiguous_buffer_mut(s_, NANO_Property)
    
#define NANO_PropertySeq_maximum(s_) \
    NANO_TSeqImpl_maximum(s_)
    
#define NANO_PropertySeq_length(s_) \
    NANO_TSeqImpl_length(s_)
    
#define NANO_PropertySeq_set_length(s_, l_) \
    NANO_TSeqImpl_set_length(s_,l_)

#endif /* NANO_FEAT_TYPED_SEQUENCE */

/*i @} *//* nano_api_infr_property */

#endif /* NANO_FEAT_PROPERTY */

#endif /* nano_core_osapi_h */
