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
 * @file nano_core_osapi_cc_stdc.h
 * @brief Compiler-dependent definitions for standard C compilers (and Windows).
 * 
 * This header file defines compiler-specific symbols for compilers which
 * support the standard C library, and the Windows C compiler.
 * 
 */

#ifndef nano_core_osapi_cc_stdc_h
#define nano_core_osapi_cc_stdc_h

#if !defined(__STDC__) && !defined(_MSC_VER) && \
    NANO_PLATFORM != NANO_PLATFORM_WINDOWS
#error "osapi_cc_stdc.h used used with a non-standard C compiler (or MSVC)"
#endif

#include <limits.h>
#include <stddef.h>


/*******************************************************************************
 * Define types for for MS VS. The supported types can be found here:
 *
 * https://msdn.microsoft.com/en-us/library/s3f49ktz.aspx
 ******************************************************************************/
#if defined(_MSC_VER) || defined(NANO_WIN32)

#if !NANO_FEAT_OSAPI_STDINT
typedef signed char         NANO_i8;
typedef unsigned char       NANO_u8;
typedef signed short        NANO_i16;
typedef unsigned short      NANO_u16;
typedef signed int          NANO_i32;
typedef unsigned int        NANO_u32;
typedef signed long long    NANO_i64;
typedef unsigned long long  NANO_u64;
#endif /* NANO_FEAT_OSAPI_STDINT */

#define NANO_HAVE_F32
typedef float               NANO_f32;

#define NANO_HAVE_F64
typedef double              NANO_f64;

#if NANO_FEAT_NATIVE_F128
#error "Native f128 is not supported on Win32."
#else
#define NANO_HAVE_F128
typedef struct NANO_f128
{
    char bytes[16];
} NANO_f128;
#endif

#else /* checks when __STDC__ is defined */

/*******************************************************************************
 *                   Define/Check STDC compiler integer mappings
 ******************************************************************************/
#if !NANO_FEAT_OSAPI_STDINT
#if SCHAR_MAX == 127
typedef signed char     NANO_i8;
typedef unsigned char   NANO_u8;
#else
#error "Invalid SCHAR_MAX found, failed to define NANO_i8/u8."
#endif /* SCHAR_MAX */

#if SHRT_MAX == 32767
typedef signed short    NANO_i16;
typedef unsigned short  NANO_u16;
#else
#error "Invalid SHRT_MAX found, failed to define NANO_i16/u16."
#endif /* SHRT_MAX */

#if INT_MAX == 2147483647
typedef signed int      NANO_i32;
typedef unsigned int    NANO_u32;
#elif LONG_MAX == 2147483647
typedef signed long int      NANO_i32;
typedef unsigned long int    NANO_u32;
#else
#error "Invalid INT_MAX found, failed to define NANO_i32/u32."
#endif /* INT_MAX */

#ifdef LLONG_MAX
#if LLONG_MAX == 9223372036854775807LL
typedef signed long long int    NANO_i64;
typedef unsigned long long int  NANO_u64;
#else
#error "Invalid LLONG_MAX found, failed to define NANO_i64/u64."
#endif /* LLONG_MAX has unknown size */
#elif defined(__LONG_LONG_MAX__)
#if __LONG_LONG_MAX__ == 9223372036854775807LL
typedef signed long long int    NANO_i64;
typedef unsigned long long int  NANO_u64;
#else /* __LONG_LONG_MAX__ has unknown size */
#error "Invalid __LONG_LONG_MAX__ found, failed to define NANO_i64/u64."
#endif
#else /* no size defined */
#error "Failed to define NANO_i64/u64."
#endif
#endif /* !NANO_FEAT_OSAPI_STDINT */
/*******************************************************************************
 *                  Define compiler floating point mappings
 ******************************************************************************/
#if (__SIZEOF_FLOAT__ == 4) || \
    (__FLT_MAX_EXP__ == 128) || \
    (FLT_MAX_EXP == 128) || \
    defined(__VOS__)
#define NANO_HAVE_F32
typedef float NANO_f32;
#endif

#if (__SIZEOF_DOUBLE__ == 8) || \
    (__DBL_MAX_EXP__ == 1024) || \
    (DBL_MAX_EXP == 1024) || \
    defined(__VOS__)
#define NANO_HAVE_F64
typedef double      NANO_f64;
#elif (__SIZEOF_LONG_LONG__ == 8)
/* Some compilers define double as 4 bytes and long long as 8 bytes */
typedef long long   NANO_f64;
#endif

#if NANO_FEAT_NATIVE_F128
#if __GNUC__ > 4 || (__GNUC__ == 4 && (__GNUC_MINOR__ > 2))
#define NANO_HAVE_F128
typedef __float128 NANO_f128;
#else
#error "Invalid __SIZEOF_DOUBLE__ found, failed to define NANO_f128."
#endif
#else
#define NANO_HAVE_F128
typedef struct NANO_f128
{
    char bytes[16];
} NANO_f128;
#endif

#endif /* __STDC__ */

/*******************************************************************************
 *                          Define CPU endian
 ******************************************************************************/
#if !defined(NANO_ENDIAN_LITTLE) && \
    !defined(NANO_ENDIAN_BIG)
/*********************************************************
 *    Determine Endianness using GCC-specific macros
 *********************************************************/
#ifdef __GNUC__
/* 
 * Determine Endianness from macro __BYTE_ORDER__ 
 */
#if defined(__BYTE_ORDER__) && \
    ((__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__) || \
    defined(__LITTLE_ENDIAN__) || \
    (__BYTE_ORDER == __LITTLE_ENDIAN))
#define NANO_ENDIAN_LITTLE 1
#undef  NANO_ENDIAN_BIG
#elif defined(__BYTE_ORDER__) && \
      ((__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__) || \
      defined(__BIG_ENDIAN__) || \
      (__BYTE_ORDER == __BIG_ENDIAN))
#define NANO_ENDIAN_BIG    1
#undef  NANO_ENDIAN_LITTLE
/* 
 * Determine Endianness from architecture-specific macros 
 */
#elif i386 || i686 || __i686 || __i686__ || __i386 || __i386__ || __x86_64 || \
     __x86_64__ || __LITTLE_ENDIAN__ || _LITTLE_ENDIAN || __MIPSEL
#define NANO_ENDIAN_LITTLE 1
#undef  NANO_ENDIAN_BIG
/* 
 * One last check for big-endian macros
 */
#elif __BIG_ENDIAN__ || _BIG_ENDIAN || __MIPSEB || \
    (defined(BYTE_ORDER) && defined(BIG_ENDIAN) && \
        BYTE_ORDER == BIG_ENDIAN)
#define NANO_ENDIAN_BIG    1
#undef  NANO_ENDIAN_LITTLE
#elif (defined(BYTE_ORDER) && defined(LITTLE_ENDIAN) && \
        BYTE_ORDER == LITTLE_ENDIAN)
#define NANO_ENDIAN_LITTLE  1
#undef  NANO_ENDIAN_BIG
#else
#error "Unable to determine GNU byte-order"
#endif /* __BYTE_ORDER__ */

/*********************************************************
 *  Determine Endianness using Windows-specific macros
 *********************************************************/
#elif defined(_MSC_VER)
#if defined(_M_IX86) || defined(_M_X64)
#define NANO_ENDIAN_LITTLE 1
#undef  NANO_ENDIAN_BIG
#else
#error "Unable to determine MSC byte-order"
#endif
/*********************************************************
 *  Failed to automatically determine endianness
 *********************************************************/
#else
#error "Unable to determine endianness."
#endif /* !_M_IX86 */
#endif

/* Try to determine the ptr size unless it has been set by this flag */
#ifndef NANO_64BIT

#if defined(_MSC_VER)
#if _WIN64
#define NANO_64BIT 1
#endif /* _MSC_VER */
#elif defined(__GNUC__)
#if defined(__SIZEOF_PTRDIFF_T__)
#if ( __SIZEOF_PTRDIFF_T__ == 8 )
#define NANO_64BIT 1
#elif ( __SIZEOF_PTRDIFF_T__ != 4 )
#warning "Size of PTR DIFF not 4 or 8 "
#endif /* __SIZEOF_PTRDIFF_T__ */
#elif __x86_64__ || __x86_64
#define NANO_64BIT 1
#elif i386 || i686 || __i686 || __i686__ || __i386 || __i386__ || _ARCH_PPC || \
      __mips__ || mips || __arm__ || PPC
    /* 32 bits */
#else
#error "GCC - Unable to determine whether architecture is 32 or 64 bit"
#endif
#else
#error "Unable to determine 32 bit or 64 architecture"
#endif

#endif /* NANO_64BIT */

/*******************************************************************************
 *                   Define various compiler helpers
 ******************************************************************************/

#if NANO_PLATFORM == NANO_PLATFORM_WINDOWS
#define NANO_OSAPI_Memory_get_alignment_of(testType) __alignof(testType)
#else
#define NANO_OSAPI_Memory_get_alignment_of(testType) \
    offsetof (struct { char c; testType member; }, member)
#endif /* NANO_PLATFORM */

/* Function attributes */
#if defined(__GNUC__) && (__GNUC__ >= 4)
#define MUST_CHECK_RETURN __attribute__((warn_unused_result))
#define SHOULD_CHECK_RETURN
#define FUNCTION_SHOULD_TYPEDEF(ftdef_) \
typedef SHOULD_CHECK_RETURN ftdef_;
#define FUNCTION_MUST_TYPEDEF(ftdef_) \
typedef MUST_CHECK_RETURN ftdef_;
#elif defined(_MSC_VER) || defined(WIN32)
#if _MSC_VER >= 1900
#define MUST_CHECK_RETURN _Check_return_
#else
#include <CodeAnalysis\SourceAnnotations.h>
#define MUST_CHECK_RETURN [returnvalue:SA_Post(MustCheck=SA_Yes)]
#endif
#define SHOULD_CHECK_RETURN
#define FUNCTION_SHOULD_TYPEDEF(ftdef_) \
typedef ftdef_;
#define FUNCTION_MUST_TYPEDEF(ftdef_) \
typedef ftdef_;
#else
#define SHOULD_CHECK_RETURN
#define FUNCTION_SHOULD_TYPEDEF(ftdef_) \
typedef ftdef_;
#define FUNCTION_MUST_TYPEDEF(ftdef_) \
typedef ftdef_;
#endif

/* In case it is defined elsewhere */
#ifndef NANO_FUNCTION_NAME
/* MS VC/VC++ is not 100% C99 compliant, some versions support __func__ */
#ifdef _MSC_VER
#if (_MSC_VER >= 1900)
#define  NANO_FUNCTION_NAME __func__
#else
#define NANO_FUNCTION_NAME __FUNCTION__
#endif /* _MSC_VER >= 1900 */
#elif defined (__STDC__)
/* func was introduced in C99 */
#if __STDC_VERSION__ >= 199901L
#define NANO_FUNCTION_NAME __func__
#else
/* Fallback */
#define NANO_FUNCTION_NAME __FUNCTION__
#endif /* __STDC_VERSION__ */
#endif /* __STDC__ */
#endif /* NANO_FUNCTION_NAME */

#if NANO_ENABLE_PRECONDITION
/******************************************************************************
 *                              Assert
 ******************************************************************************/
#include <assert.h>

#define NANO_assert(expr_)     assert(expr_)

#endif /* NANO_ENABLE_PRECONDITION */

#define NANO_FD_INVALID       -1

#endif /* nano_core_osapi_cc_stdc_h */

