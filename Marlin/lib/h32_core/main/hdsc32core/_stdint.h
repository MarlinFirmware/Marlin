/* Copyright (C) ARM Ltd., 1999,2014 */
/* All rights reserved */

/*
 * RCS $Revision$
 * Checkin $Date$
 * Revising $Author: agrant $
 */

#ifndef __stdint_h
#define __stdint_h
#define __ARMCLIB_VERSION 5060037

#ifdef __INT64_TYPE__
/* armclang predefines '__INT64_TYPE__' and '__INT64_C_SUFFIX__' */
#define __INT64 __INT64_TYPE__
#else
/* armcc has builtin '__int64' which can be used in --strict mode */
#define __INT64 __int64
#define __INT64_C_SUFFIX__ ll
#endif
#define __PASTE2(x, y) x##y
#define __PASTE(x, y) __PASTE2(x, y)
#define __INT64_C(x) __ESCAPE__(__PASTE(x, __INT64_C_SUFFIX__))
#define __UINT64_C(x) __ESCAPE__(__PASTE(x##u, __INT64_C_SUFFIX__))
#if defined(__clang__) || (defined(__ARMCC_VERSION) && !defined(__STRICT_ANSI__))
/* armclang and non-strict armcc allow 'long long' in system headers */
#define __LONGLONG long long
#else
/* strict armcc has '__int64' */
#define __LONGLONG __int64
#endif

#ifndef __STDINT_DECLS
#define __STDINT_DECLS

#undef __CLIBNS

#ifdef __cplusplus
namespace std
{
#define __CLIBNS std::
    extern "C"
    {
#else
#define __CLIBNS
#endif /* __cplusplus */

        /*
         * 'signed' is redundant below, except for 'signed char' and if
         * the typedef is used to declare a bitfield.
         */

        /* 7.18.1.1 */

        /* exact-width signed integer types */
        typedef signed char int8_t;
        typedef signed short int int16_t;
        typedef signed int int32_t;
        typedef signed __INT64 int64_t;

        /* exact-width unsigned integer types */
        typedef unsigned char uint8_t;
        typedef unsigned short int uint16_t;
        typedef unsigned int uint32_t;
        typedef unsigned __INT64 uint64_t;

        /* 7.18.1.2 */

        /* smallest type of at least n bits */
        /* minimum-width signed integer types */
        typedef signed char int_least8_t;
        typedef signed short int int_least16_t;
        typedef signed int int_least32_t;
        typedef signed __INT64 int_least64_t;

        /* minimum-width unsigned integer types */
        typedef unsigned char uint_least8_t;
        typedef unsigned short int uint_least16_t;
        typedef unsigned int uint_least32_t;
        typedef unsigned __INT64 uint_least64_t;

        /* 7.18.1.3 */

        /* fastest minimum-width signed integer types */
        typedef signed int int_fast8_t;
        typedef signed int int_fast16_t;
        typedef signed int int_fast32_t;
        typedef signed __INT64 int_fast64_t;

        /* fastest minimum-width unsigned integer types */
        typedef unsigned int uint_fast8_t;
        typedef unsigned int uint_fast16_t;
        typedef unsigned int uint_fast32_t;
        typedef unsigned __INT64 uint_fast64_t;

        /* 7.18.1.4 integer types capable of holding object pointers */
#if __sizeof_ptr == 8
        typedef signed __INT64 intptr_t;
        typedef unsigned __INT64 uintptr_t;
#else
typedef signed int intptr_t;
typedef unsigned int uintptr_t;
#endif

        /* 7.18.1.5 greatest-width integer types */
        typedef signed __LONGLONG intmax_t;
        typedef unsigned __LONGLONG uintmax_t;

#if !defined(__cplusplus) || defined(__STDC_LIMIT_MACROS)

        /* 7.18.2.1 */

        /* minimum values of exact-width signed integer types */
#define INT8_MIN -128
#define INT16_MIN -32768
#define INT32_MIN (~0x7fffffff)                  /* -2147483648 is unsigned */
#define INT64_MIN __INT64_C(~0x7fffffffffffffff) /* -9223372036854775808 is unsigned */

        /* maximum values of exact-width signed integer types */
#define INT8_MAX 127
#define INT16_MAX 32767
#define INT32_MAX 2147483647
#define INT64_MAX __INT64_C(9223372036854775807)

        /* maximum values of exact-width unsigned integer types */
#define UINT8_MAX 255
#define UINT16_MAX 65535
#define UINT32_MAX 4294967295u
#define UINT64_MAX __UINT64_C(18446744073709551615)

        /* 7.18.2.2 */

        /* minimum values of minimum-width signed integer types */
#define INT_LEAST8_MIN -128
#define INT_LEAST16_MIN -32768
#define INT_LEAST32_MIN (~0x7fffffff)
#define INT_LEAST64_MIN __INT64_C(~0x7fffffffffffffff)

        /* maximum values of minimum-width signed integer types */
#define INT_LEAST8_MAX 127
#define INT_LEAST16_MAX 32767
#define INT_LEAST32_MAX 2147483647
#define INT_LEAST64_MAX __INT64_C(9223372036854775807)

        /* maximum values of minimum-width unsigned integer types */
#define UINT_LEAST8_MAX 255
#define UINT_LEAST16_MAX 65535
#define UINT_LEAST32_MAX 4294967295u
#define UINT_LEAST64_MAX __UINT64_C(18446744073709551615)

        /* 7.18.2.3 */

        /* minimum values of fastest minimum-width signed integer types */
#define INT_FAST8_MIN (~0x7fffffff)
#define INT_FAST16_MIN (~0x7fffffff)
#define INT_FAST32_MIN (~0x7fffffff)
#define INT_FAST64_MIN __INT64_C(~0x7fffffffffffffff)

        /* maximum values of fastest minimum-width signed integer types */
#define INT_FAST8_MAX 2147483647
#define INT_FAST16_MAX 2147483647
#define INT_FAST32_MAX 2147483647
#define INT_FAST64_MAX __INT64_C(9223372036854775807)

        /* maximum values of fastest minimum-width unsigned integer types */
#define UINT_FAST8_MAX 4294967295u
#define UINT_FAST16_MAX 4294967295u
#define UINT_FAST32_MAX 4294967295u
#define UINT_FAST64_MAX __UINT64_C(18446744073709551615)

        /* 7.18.2.4 */

        /* minimum value of pointer-holding signed integer type */
#if __sizeof_ptr == 8
#define INTPTR_MIN INT64_MIN
#else
#define INTPTR_MIN INT32_MIN
#endif

        /* maximum value of pointer-holding signed integer type */
#if __sizeof_ptr == 8
#define INTPTR_MAX INT64_MAX
#else
#define INTPTR_MAX INT32_MAX
#endif

        /* maximum value of pointer-holding unsigned integer type */
#if __sizeof_ptr == 8
#define UINTPTR_MAX UINT64_MAX
#else
#define UINTPTR_MAX UINT32_MAX
#endif

        /* 7.18.2.5 */

        /* minimum value of greatest-width signed integer type */
#define INTMAX_MIN __ESCAPE__(~0x7fffffffffffffffll)

        /* maximum value of greatest-width signed integer type */
#define INTMAX_MAX __ESCAPE__(9223372036854775807ll)

        /* maximum value of greatest-width unsigned integer type */
#define UINTMAX_MAX __ESCAPE__(18446744073709551615ull)

        /* 7.18.3 */

        /* limits of ptrdiff_t */
#if __sizeof_ptr == 8
#define PTRDIFF_MIN INT64_MIN
#define PTRDIFF_MAX INT64_MAX
#else
#define PTRDIFF_MIN INT32_MIN
#define PTRDIFF_MAX INT32_MAX
#endif

        /* limits of sig_atomic_t */
#define SIG_ATOMIC_MIN (~0x7fffffff)
#define SIG_ATOMIC_MAX 2147483647

        /* limit of size_t */
#if __sizeof_ptr == 8
#define SIZE_MAX UINT64_MAX
#else
#define SIZE_MAX UINT32_MAX
#endif

        /* limits of wchar_t */
        /* NB we have to undef and redef because they're defined in both
         * stdint.h and wchar.h */
#undef WCHAR_MIN
#undef WCHAR_MAX

#if defined(__WCHAR32) || (defined(__ARM_SIZEOF_WCHAR_T) && __ARM_SIZEOF_WCHAR_T == 4)
#define WCHAR_MIN 0
#define WCHAR_MAX 0xffffffffU
#else
#define WCHAR_MIN 0
#define WCHAR_MAX 65535
#endif

        /* limits of wint_t */
#define WINT_MIN (~0x7fffffff)
#define WINT_MAX 2147483647

#endif /* __STDC_LIMIT_MACROS */

#if !defined(__cplusplus) || defined(__STDC_CONSTANT_MACROS)

        /* 7.18.4.1 macros for minimum-width integer constants */
#define INT8_C(x) (x)
#define INT16_C(x) (x)
#define INT32_C(x) (x)
#define INT64_C(x) __INT64_C(x)

#define UINT8_C(x) (x##u)
#define UINT16_C(x) (x##u)
#define UINT32_C(x) (x##u)
#define UINT64_C(x) __UINT64_C(x)

        /* 7.18.4.2 macros for greatest-width integer constants */
#define INTMAX_C(x) __ESCAPE__(x##ll)
#define UINTMAX_C(x) __ESCAPE__(x##ull)

#endif /* __STDC_CONSTANT_MACROS */

#ifdef __cplusplus
    } /* extern "C" */
} /* namespace std */
#endif /* __cplusplus */
#endif /* __STDINT_DECLS */

#ifdef __cplusplus
#ifndef __STDINT_NO_EXPORTS
using ::std::int16_t;
using ::std::int32_t;
using ::std::int64_t;
using ::std::int8_t;
using ::std::int_fast16_t;
using ::std::int_fast32_t;
using ::std::int_fast64_t;
using ::std::int_fast8_t;
using ::std::int_least16_t;
using ::std::int_least32_t;
using ::std::int_least64_t;
using ::std::int_least8_t;
using ::std::intmax_t;
using ::std::intptr_t;
using ::std::uint16_t;
using ::std::uint32_t;
using ::std::uint64_t;
using ::std::uint8_t;
using ::std::uint_fast16_t;
using ::std::uint_fast32_t;
using ::std::uint_fast64_t;
using ::std::uint_fast8_t;
using ::std::uint_least16_t;
using ::std::uint_least32_t;
using ::std::uint_least64_t;
using ::std::uint_least8_t;
using ::std::uintmax_t;
using ::std::uintptr_t;
#endif
#endif /* __cplusplus */

#undef __INT64
#undef __LONGLONG

#endif /* __stdint_h */

/* end of stdint.h */
