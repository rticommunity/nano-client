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
 * @file nano_core_osapi_cc.h
 * @brief Compiler-dependent definitions.
 * 
 * This header file defines symbols which abstract compiler-specific features.
 * 
 */

#ifndef nano_core_osapi_cc_h
#define nano_core_osapi_cc_h

/*******************************************************************************
 *                   Include compiler definitions
 ******************************************************************************/

#if NANO_FEAT_OSAPI_STDINT

#include <stddef.h>
#include <stdint.h>

/**
 * @addtogroup nano_api_platform_cc_prim
 * @{
 */

/*e
 * @brief TODO
 * 
 */
typedef int8_t      NANO_i8;

/*e
 * @brief TODO
 * 
 */
typedef uint8_t     NANO_u8;

/*e
 * @brief TODO
 * 
 */
typedef int16_t     NANO_i16;

/*e
 * @brief TODO
 * 
 */
typedef uint16_t    NANO_u16;
/*e
 * @brief TODO
 * 
 */
typedef int32_t     NANO_i32;
/*e
 * @brief TODO
 * 
 */
typedef uint32_t    NANO_u32;
/*e
 * @brief TODO
 * 
 */
typedef int64_t     NANO_i64;
/*e
 * @brief TODO
 * 
 */
typedef uint64_t    NANO_u64;
/*e
 * @brief TODO
 * 
 */
typedef size_t      NANO_usize;

/** @} *//* nano_api_platform_cc_prim */

#endif /* NANO_FEAT_OSAPI_STDINT */

#include NANO_CC_DEF_H

#if defined(NANO_ENDIAN_LITTLE) && \
    defined(NANO_ENDIAN_BIG)
#error "Both NANO_ENDIAN_LITTLE and NANO_ENDIAN_BIG defined."
#elif !defined(NANO_ENDIAN_LITTLE) && \
      !defined(NANO_ENDIAN_BIG)
#error "Neither NANO_ENDIAN_LITTLE nor NANO_ENDIAN_BIG defined"
#endif /* NANO_ENDIAN_LITTLE/NANO_ENDIAN_BIG */


#if NANO_ENABLE_PRECONDITION
#ifndef NANO_assert
#error "NO implementation of NANO_assert() found when building debug libraries"
#endif /* HAVE_ASSERT */
#endif /* NANO_ENABLE_PRECONDITION */

/**
 * @addtogroup nano_api_platform_cc_prim
 * @{
 */
#ifndef NANO_I64_MAX
/*e
 * @brief The maximum value that can be represented by a NANO_i64.
 */
#define NANO_I64_MAX        9223372036854775807LL
#endif /* NANO_I64_MAX */

#ifndef NANO_I64_MIN
/*e
 * @brief The minimum value that can be represented by a NANO_i64.
 */
#define NANO_I64_MIN        (-NANO_I64_MAX - 1LL)
#endif /* NANO_I64_MIN */

#ifndef NANO_U64_MAX
/*e
 * @brief The maximum value that can be represented by a NANO_u64.
 */
#define NANO_U64_MAX        18446744073709551615ULL
#endif /* NANO_U64_MAX */

#ifndef NANO_I32_MAX
/*e
 * @brief The maximum value that can be represented by a NANO_i32.
 */
#define NANO_I32_MAX        2147483647
#endif /* NANO_I32_MAX */

#ifndef NANO_I32_MIN
/*e
 * @brief The minimum value that can be represented by a NANO_i32.
 */
#define NANO_I32_MIN        (-NANO_I32_MAX - 1)
#endif /* NANO_I32_MIN */

#ifndef NANO_U32_MAX
/*e
 * @brief The maximum value that can be represented by a NANO_u32.
 */
#define NANO_U32_MAX        4294967295U
#endif /* NANO_U32_MAX */

#ifndef NANO_I16_MAX
/*e
 * @brief The maximum value that can be represented by a NANO_i16.
 */
#define NANO_I16_MAX        32767
#endif /* NANO_I16_MAX */

#ifndef NANO_I16_MIN
/*e
 * @brief The minimum value that can be represented by a NANO_i16.
 */
#define NANO_I16_MIN        (-32768)
#endif /* NANO_I16_MIN */

#ifndef NANO_U16_MAX
/*e
 * @brief The maximum value that can be represented by a NANO_u16.
 */
#define NANO_U16_MAX        65535
#endif /* NANO_U16_MAX */

#ifndef NANO_I8_MAX
/*e
 * @brief The maximum value that can be represented by a NANO_i8.
 */
#define NANO_I8_MAX         127
#endif /* NANO_I8_MAX */

#ifndef NANO_I8_MIN
/*e
 * @brief The minimum value that can be represented by a NANO_i8.
 */
#define NANO_I8_MIN         (-128)
#endif /* NANO_I8_MIN */

#ifndef NANO_U8_MAX
/*e
 * @brief The maximum value that can be represented by a NANO_u8.
 */
#define NANO_U8_MAX         255
#endif /* NANO_U8_MAX */

/** @} *//* nano_api_platform_cc_prim */


/******************************************************************************
 *                  Compiler-independent Primitive Types
 ******************************************************************************/
/**
 * @addtogroup nano_api_platform_cc_prim
 * @{
 */
/**
 * @brief Boolean value
 */
typedef NANO_u8 NANO_bool;

/**
 * @brief "True" boolean value
 */
#define NANO_BOOL_TRUE      ((NANO_bool)1)

/**
 * @brief "False" boolean value
 */
#define NANO_BOOL_FALSE     ((NANO_bool)0)

#if defined(NANO_64BIT)

#if !NANO_FEAT_OSAPI_STDINT
typedef NANO_u64 NANO_usize;
typedef NANO_i64 NANO_isize;
#endif /* !NANO_FEAT_OSAPI_STDINT */

#ifndef NANO_USIZE_MAX
#define NANO_USIZE_MAX          NANO_U64_MAX
#endif /* NANO_USIZE_MAX */

#ifndef NANO_ISIZE_MAX
#define NANO_ISIZE_MAX          NANO_I64_MAX
#endif /* NANO_ISIZE_MAX */

#else

#if !NANO_FEAT_OSAPI_STDINT
typedef NANO_u32 NANO_usize;
typedef NANO_i32 NANO_isize;
#endif /* !NANO_FEAT_OSAPI_STDINT */

#ifndef NANO_USIZE_MAX
#define NANO_USIZE_MAX          NANO_U32_MAX
#endif /* NANO_USIZE_MAX */

#ifndef NANO_ISIZE_MAX
#define NANO_ISIZE_MAX          NANO_I32_MAX
#endif /* NANO_ISIZE_MAX */

#endif /* NANO_64BIT */

/*i
 * @brief TODO
 * 
 */
typedef const char *    NANO_ConstString;

/*i
 * @brief TODO
 * 
 */
typedef char *          NANO_String;


/** @} *//* nano_api_platform_cc_prim */



/*i
 * @addtogroup nano_api_infr_compiler
 * @{
 */
/*
 * If the compiler does not support static, override it here.
 */
#ifndef NANO_OSAPI_CC_STATIC
/*i
 * @brief A macro used to declare "private" (a.k.a. "static") elements.
 */
#define NANO_PRIVATE static
#endif /* NANO_OSAPI_CC_STATIC */

/* Some definitions based on whether we are building release or debug code */
#if NANO_ENABLE_PRECONDITION
/* This is used to prevent the compiler from issuing warnings for variables
 * not used on purpose (for example interface variables).
 */
#define PRECOND_ARG(x_)
#define NANO_PCOND(expr_)           NANO_assert(expr_);
#else /* !NANO_ENABLE_PRECONDITION */
/*i
 * @brief Avoid compiler warnings for variables that are only used when
 * NANO_ENABLE_PRECONDITION is defined.
 */
#define PRECOND_ARG(x_) (void)(x_);
#define NANO_PCOND(expr_)
#endif /* NANO_ENABLE_PRECONDITION */

/*i
 * @brief TODO
 * 
 */
#define NANO_UNREACHABLE_CODE               NANO_PCOND(NANO_BOOL_FALSE)

/*i
 * @brief Avoid compiler warnings for variables which are not used on purpose.
 */
#ifndef UNUSED_ARG
#define UNUSED_ARG(x) (void)(x)
#endif
/*i
 * @brief Avoid compiler warnings for function calls whose return value is
 * not inspected.
 */
#define IGNORE_RETVAL(x) (void)(x)

#ifndef MUST_CHECK_RETURN
/*i
 * @brief Force callers of a function to verify the function's return value or
 * generate a warning during compilation.
 */
#define MUST_CHECK_RETURN
#endif /* MUST_CHECK_RETURN */

#ifndef SHOULD_CHECK_RETURN
/*i
 * @brief Inform callers of a function that the function's return value should
 * be verified, and generate a warning during compilation if this is not the
 * case.
 */
#define SHOULD_CHECK_RETURN
#endif /* SHOULD_CHECK_RETURN */

#ifndef FUNCTION_SHOULD_TYPEDEF
/*i
 * @brief Inform implementors of an interface that a function should be
 * implemented.
 */
#define FUNCTION_SHOULD_TYPEDEF(ftdef_) \
typedef SHOULD_CHECK_RETURN ftdef_;
#endif /* FUNCTION_SHOULD_TYPEDEF */

#ifndef FUNCTION_MUST_TYPEDEF
/*i
 * @brief Inform implementors of an interface that a function must be
 * implemented.
 */
#define FUNCTION_MUST_TYPEDEF(ftdef_) \
typedef MUST_CHECK_RETURN ftdef_;
#endif /* FUNCTION_MUST_TYPEDEF */

#ifndef NANO_FUNCTION_NAME
/*i
 * @brief A symbol containing the current function's name as a string literal.
 */
#define NANO_FUNCTION_NAME "undefined"
#endif /* NANO_FUNCTION_NAME */

/** @} *//* nano_api_infr_compiler */

#endif /* nano_core_osapi_cc_h */

