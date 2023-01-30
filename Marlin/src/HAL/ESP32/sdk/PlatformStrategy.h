/*****************************************************************************
*
*  PROJECT:     Eir SDK
*  FILE:        eirrepo/sdk/PlatformStrategy.h
*  PURPOSE:     Platform-dependant optimizations
*
*  Find the Eir SDK at: https://osdn.net/projects/eirrepo/
*
*****************************************************************************/

#ifndef _PLATFORM_STRATEGY_HEADER_
#define _PLATFORM_STRATEGY_HEADER_

// Define the preprocessor definition _PLATFORM_DEBUG_CODEGRAPHS if you want to compile your code as-if the fast intrinsics define below were
// available but just to verify the validity of the codegraphs, without actually having the performance benefits.

#include "MacroUtils.h"
#include "MetaHelpers.h"

#include <stdint.h>

#if __has_include (<type_traits>)
#include <type_traits>
#endif

#if defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_AMD64))
#include <intrin.h>
#endif

#if defined(_MSC_VER) && (defined(_M_ARM32))
#include <armintr.h>
#endif

#ifdef __cpp_lib_is_constant_evaluated
#define _PLATFORM_ISCEVAL_CONSTEXPR constexpr
#else
#define _PLATFORM_ISCEVAL_CONSTEXPR
#endif

namespace eir
{

AINLINE constexpr bool __platform_is_constant_evaluated( void ) noexcept
{
#ifdef __cpp_lib_is_constant_evaluated
#if defined(_MSC_VER) || defined(__GNUC__)
    return __builtin_is_constant_evaluated();
#elif __has_include(<type_traits>)
    return std::is_constant_evaluated();
#endif
#else
    // It is safe to return failure because then always dynamic paths are chosen which should be possible anyway.
    return false;
#endif
}

// The platform word size in bytes.
#if defined(__GNUC__) && defined(__WORDSIZE)
// Used in GCC Linux.
#define PLATFORM_WORDSIZE (__WORDSIZE/8)
#elif defined(_MSC_VER)
#if defined(_M_AMD64) || defined(_M_ARM64)
#define PLATFORM_WORDSIZE 8
#elif defined(_M_IX86) || defined(_M_ARM32)
#define PLATFORM_WORDSIZE 4
#endif
#elif defined(__GNUC__) && (defined(ARDUINO_ARCH_ESP32) || defined(__XTENSA__))
#define PLATFORM_WORDSIZE 4
#elif defined(__GNUC__) && defined(__AVR_ARCH__)
#define PLATFORM_WORDSIZE 1
#else
#error No known platform word size definition.
#endif

// For debug we want to enable code-paths that perform alignment calculation, even
// if the hardware does not require them.
#if !defined(_DEBUG) && !defined(_PLATFORM_FORCE_ALIGNED_MEMACCESS)
#if defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_AMD64)) || \
    defined(__GNUC__) && (defined(__x86__) || defined(__x86_64__))
#define _PLATFORM_SUPPORTS_UNALIGNED_MEMACCESS
#endif
#endif

#if PLATFORM_WORDSIZE <= 8 || PLATFORM_WORDSIZE <= 4
typedef uint64_t platformBitCountType;
#elif PLATFORM_WORDSIZE <= 2
typedef uint32_t platformBitCountType;
#elif PLATFORM_WORDSIZE <= 1
typedef uint16_t platformBitCountType;
#else
#error Not platformBitCountType definition selectable.
#endif

typedef uint8_t platformLocalBitcountType;

typedef
#if PLATFORM_WORDSIZE == 1
uint8_t
#elif PLATFORM_WORDSIZE == 2
uint16_t
#elif PLATFORM_WORDSIZE == 4
uint32_t
#elif PLATFORM_WORDSIZE == 8
uint64_t
#else
#error Unknown platform native word type.
#endif
platformNativeWordType;

// Define _PLATFORM_FIXED_ENDIANNESS_ENFORCE_* to override the assumed-endianness of a dynamic endianness system.
// A popular dynamic endianness system is the ARM processor. You can swap the endianness in interrupt-disabled-
// critical section code inside the embedded sector, or just decide on MCU init.
// This specification is NOT required on protected-mode, desktop environments.

#if defined(_PLATFORM_FIXED_ENDIANNESS_ENFORCE_LE) || defined(_PLATFORM_FIXED_ENDIANNESS_ENFORCE_BE) || \
    defined(_WIN32) && (defined(_M_IX86) || defined(_M_AMD64)) || \
    defined(__GNUC__) && (defined(__x86__) || defined(__x86_64__) || defined(__XTENSA__))
#define _PLATFORM_FIXED_ENDIANNESS
#endif

#ifndef _PLATFORM_DISABLE_FAST_INTRINSICS
#if defined(__GNUC__) && (defined(__arm__) || defined(__aarch64__)) || \
    defined(_MSC_VER) && defined(_M_ARM32) || \
    defined(_PLATFORM_DEBUG_CODEGRAPHS)
#define _PLATFORM_HAS_FAST_BITREVERSE

#if defined(_PLATFORM_DEBUG_CODEGRAPHS) || defined(__cpp_lib_is_constant_evaluated)
template <typename numberType>
AINLINE constexpr numberType __platform_generic_bitreverse( numberType input ) noexcept
{
    numberType result = 0u;
    for (unsigned int n = 0; n < sizeof(platformNativeWordType)*8u; n++)
    {
        result <<= 1;
        result |= (input & 1);
        input >>= 1;
    }
    return result;
}
#endif

// Reverses the bit-order in the native word.
AINLINE _PLATFORM_ISCEVAL_CONSTEXPR platformNativeWordType __platform_bitreverse( platformNativeWordType input ) noexcept
{
#if defined(__cpp_lib_is_constant_evaluated)
    if ( __platform_is_constant_evaluated() )
    {
        return __platform_generic_bitreverse( input );
    }
    else
    {
#endif
        platformNativeWordType result;
#if defined(__GNUC__) && (defined(__arm__) || defined(__aarch64__))
        // Warning: GCC does not know how to optimize this statement away, so constant-time evaluation is not possible!
        __asm__( "rbit %0,%1" : "=r" (result) : "r" (input) );
#elif defined(_MSC_VER) && (defined(_M_ARM32))
        // Mistake by MSFT: no inclusion of RBIT intrinsic for AArch64 (the instruction exists)
        result = _arm_rbit(input);
#elif defined(_PLATFORM_DEBUG_CODEGRAPHS)
        result = __platform_generic_bitreverse( input );
#endif
        return result;
#if defined(__cpp_lib_is_constant_evaluated)
    }
#endif
}

#ifndef _PLATFORM_NO_DEDUCED_OPERATIONS
#if PLATFORM_WORDSIZE > 1
#define _PLATFORM_HAS_FAST_BITREVERSE_DEDUCED_8BIT
AINLINE _PLATFORM_ISCEVAL_CONSTEXPR uint8_t __platform_bitreverse( uint8_t val ) noexcept
{
    return (uint8_t)( __platform_bitreverse( (platformNativeWordType)val ) >> ( (PLATFORM_WORDSIZE-sizeof(uint8_t)) * 8u ) );
}
#endif
#if PLATFORM_WORDSIZE > 2
#define _PLATFORM_HAS_FAST_BITREVERSE_DEDUCED_16BIT
AINLINE _PLATFORM_ISCEVAL_CONSTEXPR uint16_t __platform_bitreverse( uint16_t val ) noexcept
{
    return (uint16_t)( __platform_bitreverse( (platformNativeWordType)val ) >> ( (PLATFORM_WORDSIZE-sizeof(uint16_t)) * 8u ) );
}
#endif
#if PLATFORM_WORDSIZE > 4
#define _PLATFORM_HAS_FAST_BITREVERSE_DEDUCED_32BIT
AINLINE _PLATFORM_ISCEVAL_CONSTEXPR uint32_t __platform_bitreverse( uint32_t val ) noexcept
{
    return (uint32_t)( __platform_bitreverse( (platformNativeWordType)val ) >> ( (PLATFORM_WORDSIZE-sizeof(uint32_t)) * 8u ) );
}
#endif
#endif

template <typename T>
struct __platform_number_supports_bitreverse : false_type {};
template <>
struct __platform_number_supports_bitreverse <platformNativeWordType> : true_type {};
#ifdef _PLATFORM_HAS_FAST_BITREVERSE_DEDUCED_8BIT
template <>
struct __platform_number_supports_bitreverse <uint8_t> : true_type {};
#endif
#ifdef _PLATFORM_HAS_FAST_BITREVERSE_DEDUCED_16BIT
template <>
struct __platform_number_supports_bitreverse <uint16_t> : true_type {};
#endif
#ifdef _PLATFORM_HAS_FAST_BITREVERSE_DEDUCED_32BIT
template <>
struct __platform_number_supports_bitreverse <uint32_t> : true_type {};
#endif
#endif

#if defined(__GNUC__) && (defined(__arm__) || defined(__aarch64__)) || \
    defined(_MSC_VER) && defined(_M_ARM32) || \
    defined(_PLATFORM_DEBUG_CODEGRAPHS)
#define _PLATFORM_HAS_FAST_REV16

#if defined(_PLATFORM_DEBUG_CODEGRAPHS) || defined(__cpp_lib_is_constant_evaluated)
template <typename numberType>
AINLINE constexpr numberType __platform_generic_rev16( numberType val ) noexcept
{
    numberType result = 0u;
    for (unsigned int n = 0; n < sizeof(platformNativeWordType) / 2u; n++)
    {
        unsigned int bitidx = ( n * 16u );
        uint16_t unit = (uint16_t)( val >> bitidx );
        unit = ( ( unit & 0xFF00 ) >> 8u ) | ( ( unit & 0x00FF ) << 8u );
        result |= ( (platformNativeWordType)unit << bitidx );
    }
    return result;
}
#endif

// Reverses the byte-order of each 16bit packet inside the native word (2 packets in 32bit, 4 packets in 64bit).
// Should be used for vectorized 16bit packet reversing (more than one).
AINLINE _PLATFORM_ISCEVAL_CONSTEXPR platformNativeWordType __platform_rev16( platformNativeWordType val ) noexcept
{
#if defined(__cpp_lib_is_constant_evaluated)
    if ( __platform_is_constant_evaluated() )
    {
        return __platform_generic_rev16( val );
    }
    else
    {
#endif
        platformNativeWordType result;
#if defined(__GNUC__) && (defined(__arm__) || defined(__aarch64__))
        // Warning: GCC does not know how to optimize this statement away, so constant-time evaluation is not possible!
        __asm__( "rev16 %0,%1" : "=r" (result) : "r" (val) );
#elif defined(_MSC_VER) && defined(_M_ARM32)
        // Mistake by MSFT: no inclusion of REV16 intrinsic for AArch64 (the instruction exists)
        result = _arm_rev16(val);
#elif defined(_PLATFORM_DEBUG_CODEGRAPHS)
        result = __platform_generic_rev16( val );
#endif
        return result;
#if defined(__cpp_lib_is_constant_evaluated)
    }
#endif
}

#ifndef _PLATFORM_NO_DEDUCED_OPERATIONS
#if PLATFORM_WORDSIZE > 4
#define _PLATFORM_HAS_FAST_REV16_DEDUCED_32BIT
AINLINE _PLATFORM_ISCEVAL_CONSTEXPR uint32_t __platform_rev16( uint32_t val ) noexcept
{
    return (uint32_t)__platform_rev16( (platformNativeWordType)val );
}
#endif
#endif

template <typename T>
struct __platform_number_supports_rev16 : false_type {};
template <>
struct __platform_number_supports_rev16 <platformNativeWordType> : true_type {};
#ifdef _PLATFORM_HAS_FAST_REV16_DEDUCED_32BIT
template <>
struct __platform_number_supports_rev16 <uint32_t> : true_type {};
#endif
#endif

#if defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_AMD64)) || \
    defined(__GNUC__) && (defined(__x86_64__) || defined(__x86__)) || \
    defined(__GNUC__) && defined(__AVR_ARCH__) || \
    defined(_PLATFORM_DEBUG_CODEGRAPHS)
#define _PLATFORM_HAS_FAST_BITROTATE
#if defined(_PLATFORM_DEBUG_CODEGRAPHS) || defined(__cpp_lib_is_constant_evaluated)
template <typename numberType, bool is_left = false>
AINLINE _PLATFORM_ISCEVAL_CONSTEXPR numberType __platform_generic_brotate( numberType val, platformLocalBitcountType numrot ) noexcept
{
    platformLocalBitcountType bitcnt = (sizeof(numberType)*8u);
    platformLocalBitcountType shiftcnt = (numrot % bitcnt);

    if (shiftcnt == 0) return val;

    auto bitmask_shift_toend = (bitcnt - shiftcnt);

    if constexpr ( is_left )
    {
        return (val << shiftcnt) | (val >> bitmask_shift_toend);
    }
    else
    {
        return (val >> shiftcnt) | (val << bitmask_shift_toend);
    }
}
#endif
#if defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_AMD64)) || \
    defined(__GNUC__) && (defined(__x86__) || defined(__x86_64__)) || \
    defined(__GNUC__) && defined(__AVR_ARCH__) || \
    defined(_PLATFORM_DEBUG_CODEGRAPHS)
#define _PLATFORM_HAS_FAST_BITROTATE_8BIT
#if defined(__GNUC__) && defined(__AVR_ARCH__)
AINLINE uint8_t __platform_brotate_onebit_l( uint8_t val ) noexcept
{
    asm ( "ROL %0" : "+r" (val) );
    return val;
}
AINLINE uint8_t __platform_brotate_onebit_r( uint8_t val ) noexcept
{
    asm ( "ROR %0" : "+r" (val) );
    return val;
}
#endif
AINLINE _PLATFORM_ISCEVAL_CONSTEXPR uint8_t __platform_brotate_l( uint8_t val, platformLocalBitcountType cnt ) noexcept
{
#ifdef __cpp_lib_is_constant_evaluated
    if ( __platform_is_constant_evaluated() )
    {
        return __platform_generic_brotate <uint8_t, true> ( val, cnt );
    }
    else
    {
#endif
#if defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_AMD64))
        return _rotl8( val, cnt );
#elif defined(__GNUC__) && (defined(__x86__) || defined(__x86_64__))
        asm ( "ROL %b1,%b0" : "+rm" (val) : "ci" (cnt) );
        return val;
#elif defined(__GNUC__) && defined(__AVR_ARCH__)
        uint8_t cntmuch = (cnt % 8u);
        while (cntmuch)
        {
            val = __platform_brotate_onebit_l(val);
            cntmuch--;
        }
        return val;
#elif defined(_PLATFORM_DEBUG_CODEGRAPHS)
        return __platform_generic_brotate <uint8_t, true> ( val, cnt );
#endif
#ifdef __cpp_lib_is_constant_evaluated
    }
#endif
}
AINLINE _PLATFORM_ISCEVAL_CONSTEXPR uint8_t __platform_brotate_r( uint8_t val, platformLocalBitcountType cnt ) noexcept
{
#ifdef __cpp_lib_is_constant_evaluated
    if ( __platform_is_constant_evaluated() )
    {
        return __platform_generic_brotate <uint8_t, false> ( val, cnt );
    }
    else
    {
#endif
#if defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_AMD64))
        return _rotr8( val, cnt );
#elif defined(__GNUC__) && (defined(__x86__) || defined(__x86_64__))
        asm( "ROR %b1,%b0" : "+rm" (val) : "ci" (cnt) );
        return val;
#elif defined(__GNUC__) && defined(__AVR_ARCH__)
        uint8_t cntmuch = (cnt % 8u);
        while (cntmuch)
        {
            val = __platform_brotate_onebit_r(val);
            cntmuch--;
        }
        return val;
#elif defined(_PLATFORM_DEBUG_CODEGRAPHS)
        return __platform_generic_brotate <uint8_t, false> ( val, cnt );
#endif
#ifdef __cpp_lib_is_constant_evaluated
    }
#endif
}
#endif
#if defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_AMD64)) || \
    defined(__GNUC__) && (defined(__x86__) || defined(__x86_64__)) || \
    defined(_PLATFORM_DEBUG_CODEGRAPHS)
#define _PLATFORM_HAS_FAST_BITROTATE_16BIT
AINLINE _PLATFORM_ISCEVAL_CONSTEXPR uint16_t __platform_brotate_l( uint16_t val, platformLocalBitcountType cnt ) noexcept
{
#ifdef __cpp_lib_is_constant_evaluated
    if ( __platform_is_constant_evaluated() )
    {
        return __platform_generic_brotate <uint16_t, true> ( val, cnt );
    }
    else
    {
#endif
#if defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_AMD64))
        return _rotl16( val, cnt );
#elif defined(__GNUC__) && (defined(__x86__) || defined(__x86_64__))
        asm( "ROL %b1,%w0" : "+rm" (val) : "ci" (cnt) );
        return val;
#elif defined(_PLATFORM_DEBUG_CODEGRAPHS)
        return __platform_generic_brotate <uint16_t, true> ( val, cnt );
#endif
#ifdef __cpp_lib_is_constant_evaluated
    }
#endif
}
AINLINE _PLATFORM_ISCEVAL_CONSTEXPR uint16_t __platform_brotate_r( uint16_t val, platformLocalBitcountType cnt ) noexcept
{
#ifdef __cpp_lib_is_constant_evaluated
    if ( __platform_is_constant_evaluated() )
    {
        return __platform_generic_brotate <uint16_t, false> ( val, cnt );
    }
    else
    {
#endif
#if defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_AMD64))
        return _rotr16( val, cnt );
#elif defined(__GNUC__) && (defined(__x86__) || defined(__x86_64__))
        asm( "ROR %b1,%w0" : "+rm" (val) : "ci" (cnt) );
        return val;
#elif defined(_PLATFORM_DEBUG_CODEGRAPHS)
        return __platform_generic_brotate <uint16_t, false> ( val, cnt );
#endif
#ifdef __cpp_lib_is_constant_evaluated
    }
#endif
}
#endif
#if defined(_MSC_VER) && (defined(_M_AMD64) || defined(_M_IX386)) || \
    defined(__GNUC__) && (defined(__x86__) || defined(__x86_64__)) || \
    defined(_PLATFORM_DEBUG_CODEGRAPHS)
#define _PLATFORM_HAS_FAST_BITROTATE_32BIT
AINLINE _PLATFORM_ISCEVAL_CONSTEXPR uint32_t __platform_brotate_l( uint32_t val, platformLocalBitcountType cnt ) noexcept
{
#ifdef __cpp_lib_is_constant_evaluated
    if ( __platform_is_constant_evaluated() )
    {
        return __platform_generic_brotate <uint32_t, true> ( val, cnt );
    }
    else
    {
#endif
#if defined(_MSC_VER) && defined(_M_AMD64)
        return _rotl( val, cnt );
#elif defined(__GNUC__) && (defined(__x86__) || defined(__x86_64__))
        asm( "ROL %b1,%k0" : "+rm" (val) : "ci" (cnt) );
        return val;
#elif defined(_PLATFORM_DEBUG_CODEGRAPHS)
        return __platform_generic_brotate <uint32_t, true> ( val, cnt );
#endif
#ifdef __cpp_lib_is_constant_evaluated
    }
#endif
}
AINLINE _PLATFORM_ISCEVAL_CONSTEXPR uint32_t __platform_brotate_r( uint32_t val, platformLocalBitcountType cnt ) noexcept
{
#ifdef __cpp_lib_is_constant_evaluated
    if ( __platform_is_constant_evaluated() )
    {
        return __platform_generic_brotate <uint32_t, false> ( val, cnt );
    }
    else
    {
#endif
#if defined(_MSC_VER) && defined(_M_AMD64)
        return _rotr( val, cnt );
#elif defined(__GNUC__) && (defined(__x86__) || defined(__x86_64__))
        asm( "ROR %b1,%k0" : "+rm" (val) : "ci" (cnt) );
        return val;
#elif defined(_PLATFORM_DEBUG_CODEGRAPHS)
        return __platform_generic_brotate <uint32_t, false> ( val, cnt );
#endif
#ifdef __cpp_lib_is_constant_evaluated
    }
#endif
}
#endif
#if defined(_MSC_VER) && defined(_M_AMD64) || \
    defined(__GNUC__) && defined(__x86_64__) || \
    defined(_PLATFORM_DEBUG_CODEGRAPHS)
#define _PLATFORM_HAS_FAST_BITROTATE_64BIT
AINLINE _PLATFORM_ISCEVAL_CONSTEXPR uint64_t __platform_brotate_l( uint64_t val, platformLocalBitcountType cnt ) noexcept
{
#ifdef __cpp_lib_is_constant_evaluated
    if ( __platform_is_constant_evaluated() )
    {
        return __platform_generic_brotate <uint64_t, true> ( val, cnt );
    }
    else
    {
#endif
#if defined(_MSC_VER) && defined(_M_AMD64)
        return _rotl64( val, cnt );
#elif defined(__GNUC__) && defined(__x86_64__)
        asm( "ROL %b1,%q0" : "+rm" (val) : "ci" (cnt) );
        return val;
#elif defined(_PLATFORM_DEBUG_CODEGRAPHS)
        return __platform_generic_brotate <uint64_t, true> ( val, cnt );
#endif
#if __cpp_lib_is_constant_evaluated
    }
#endif
}
AINLINE _PLATFORM_ISCEVAL_CONSTEXPR uint64_t __platform_brotate_r( uint64_t val, platformLocalBitcountType cnt ) noexcept
{
#ifdef __cpp_lib_is_constant_evaluated
    if ( __platform_is_constant_evaluated() )
    {
        return __platform_generic_brotate <uint64_t, false> ( val, cnt );
    }
    else
    {
#endif
#if defined(_MSC_VER) && defined(_M_AMD64)
        return _rotr64( val, cnt );
#elif defined(__GNUC__) && defined(__x86_64__)
        asm( "ROR %b1,%q0" : "+rm" (val) : "ci" (cnt) );
        return val;
#elif defined(_PLATFORM_DEBUG_CODEGRAPHS)
        return __platform_generic_brotate <uint64_t, false> ( val, cnt );
#endif
#ifdef __cpp_lib_is_constant_evaluated
    }
#endif
}
#endif
template <typename numberType>
struct __platform_number_supports_brotate : public false_type {};
template <typename numberType>
struct __platform_number_supports_brotate <volatile numberType> : public __platform_number_supports_brotate <numberType> {};
template <typename numberType>
struct __platform_number_supports_brotate <const numberType> : public __platform_number_supports_brotate <numberType> {};
template <typename numberType>
struct __platform_number_supports_brotate <numberType&> : public __platform_number_supports_brotate <numberType> {};
template <typename numberType>
struct __platform_number_supports_brotate <const numberType&> : public __platform_number_supports_brotate <numberType> {};
template <typename numberType>
struct __platform_number_supports_brotate <numberType&&> : public __platform_number_supports_brotate <numberType> {};
#ifdef _PLATFORM_HAS_FAST_BITROTATE_8BIT
template <>
struct __platform_number_supports_brotate <uint8_t> : public true_type {};
#endif
#ifdef _PLATFORM_HAS_FAST_BITROTATE_16BIT
template <>
struct __platform_number_supports_brotate <uint16_t> : public true_type {};
#endif
#ifdef _PLATFORM_HAS_FAST_BITROTATE_32BIT
template <>
struct __platform_number_supports_brotate <uint32_t> : public true_type {};
#endif
#ifdef _PLATFORM_HAS_FAST_BITROTATE_64BIT
template <>
struct __platform_number_supports_brotate <uint64_t> : public true_type {};
#endif
#endif

#if defined(_MSC_VER) || \
    defined(__GNUC__) || \
    defined(_PLATFORM_DEBUG_CODEGRAPHS)
#define _PLATFORM_HAS_FAST_BYTESWAP

#if defined(__cpp_lib_is_constant_evaluated) || \
    defined(_PLATFORM_DEBUG_CODEGRAPHS)
template <typename numberType>
AINLINE constexpr numberType __platform_generic_byteswap( numberType val ) noexcept
{
    // I guess that this version could be better because it does not force use of memory, or a compiler who could
    // stil retranslate it back to register operations.
    numberType result = 0;

    for ( unsigned int n = 0; n < sizeof(numberType); n++ )
    {
        if (n > 0)
        {
            val >>= 8u;
            result <<= 8u;
        }
        result |= val & 0xFF;
    }

    return result;
}
#endif

#if defined(_MSC_VER) || \
    defined(__GNUC__) || \
    defined(_PLATFORM_DEBUG_CODEGRAPHS)
#define _PLATFORM_HAS_FAST_BYTESWAP_16BIT
AINLINE _PLATFORM_ISCEVAL_CONSTEXPR uint16_t __platform_byteswap( uint16_t val ) noexcept
{
#ifdef __cpp_lib_is_constant_evaluated
    if ( __platform_is_constant_evaluated() )
    {
        return __platform_generic_byteswap( val );
    }
    else
    {
#endif
#if defined(_MSC_VER)
        return _byteswap_ushort( val );
#elif defined(__GNUC__)
        return __builtin_bswap16( val );
#elif defined(_PLATFORM_DEBUG_CODEGRAPHS)
        return __platform_generic_byteswap( val );
#endif
#ifdef __cpp_lib_is_constant_evaluated
    }
#endif
}
#endif
#if defined(_MSC_VER) || \
    defined(__GNUC__) || \
    defined(_PLATFORM_DEBUG_CODEGRAPHS)
#define _PLATFORM_HAS_FAST_BYTESWAP_32BIT
AINLINE _PLATFORM_ISCEVAL_CONSTEXPR uint32_t __platform_byteswap( uint32_t val ) noexcept
{
#ifdef __cpp_lib_is_constant_evaluated
    if ( __platform_is_constant_evaluated() )
    {
        return __platform_generic_byteswap( val );
    }
    else
    {
#endif
#if defined(_MSC_VER)
        return _byteswap_ulong( val );
#elif defined(__GNUC__)
        return __builtin_bswap32( val );
#elif defined(_PLATFORM_DEBUG_CODEGRAPHS)
        return __platform_generic_byteswap( val );
#endif
#ifdef __cpp_lib_is_constant_evaluated
    }
#endif
}
#endif
#if defined(_MSC_VER) || \
    defined(__GNUC__) || \
    defined(_PLATFORM_DEBUG_CODEGRAPHS)
#define _PLATFORM_HAS_FAST_BYTESWAP_64BIT
AINLINE _PLATFORM_ISCEVAL_CONSTEXPR uint64_t __platform_byteswap( uint64_t val ) noexcept
{
#ifdef __cpp_lib_is_constant_evaluated
    if ( __platform_is_constant_evaluated() )
    {
        return __platform_generic_byteswap( val );
    }
    else
    {
#endif
#if defined(_MSC_VER)
        return _byteswap_uint64( val );
#elif defined(__GNUC__)
        return __builtin_bswap64( val );
#elif defined(_PLATFORM_DEBUG_CODEGRAPHS)
        return _platform_generic_byteswap( val );
#endif
#ifdef __cpp_lib_is_constant_evaluated
    }
#endif
}
#endif
template <typename numberType>
struct __platform_number_supports_byteswap : public false_type {};
template <typename numberType>
struct __platform_number_supports_byteswap <volatile numberType> : public __platform_number_supports_byteswap <numberType> {};
template <typename numberType>
struct __platform_number_supports_byteswap <const numberType> : public __platform_number_supports_byteswap <numberType> {};
template <typename numberType>
struct __platform_number_supports_byteswap <numberType&> : public __platform_number_supports_byteswap <numberType> {};
template <typename numberType>
struct __platform_number_supports_byteswap <const numberType&> : public __platform_number_supports_byteswap <numberType> {};
template <typename numberType>
struct __platform_number_supports_byteswap <numberType&&> : public __platform_number_supports_byteswap <numberType> {};
#ifdef _PLATFORM_HAS_FAST_BYTESWAP_16BIT
template <>
struct __platform_number_supports_byteswap <uint16_t> : public true_type {};
#endif
#ifdef _PLATFORM_HAS_FAST_BYTESWAP_32BIT
template <>
struct __platform_number_supports_byteswap <uint32_t> : public true_type {};
#endif
#ifdef _PLATFORM_HAS_FAST_BYTESWAP_64BIT
template <>
struct __platform_number_supports_byteswap <uint64_t> : public true_type {};
#endif
#endif

#if defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_AMD64)) || \
    defined(__GNUC__) && (defined(__x86__) || defined(__x86_64__)) || \
    defined(_PLATFORM_DEBUG_CODEGRAPHS)
#define _PLATFORM_HAS_FAST_BITEXTRACT

#if defined(__cpp_lib_is_constant_evaluated) || \
    defined(_PLATFORM_DEBUG_CODEGRAPHS)
template <typename numberType>
AINLINE constexpr numberType __platform_generic_bitextract( const numberType& val, platformLocalBitcountType bitoff, platformLocalBitcountType bitcnt ) noexcept
{
    if ( bitcnt == sizeof(numberType)*8u )
    {
        return val;
    }
    else
    {
        numberType bitmask = ((numberType)1u<<bitcnt)-1;
        numberType tmp = val;
        tmp >>= bitoff;
        tmp &= bitmask;
        return tmp;
    }
}
#endif

#if defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_AMD64)) || \
    defined(__GNUC__) && (defined(__x86__) || defined(__x86_64__)) || \
    defined(_PLATFORM_DEBUG_CODEGRAPHS)
#define _PLATFORM_HAS_FAST_BITEXTRACT_32BIT
AINLINE _PLATFORM_ISCEVAL_CONSTEXPR uint32_t __platform_bitextract( uint32_t val, platformLocalBitcountType bitoff, platformLocalBitcountType bitcnt ) noexcept
{
#ifdef __cpp_lib_is_constant_evaluated
    if ( __platform_is_constant_evaluated() )
    {
        return __platform_generic_bitextract <uint32_t> ( val, bitoff, bitcnt );
    }
    else
    {
#endif
#if defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_AMD64))
        return _bextr_u32( val, bitoff, bitcnt );
#elif defined(__GNUC__) && (defined(__x86__) || defined(__x86_64__))
        uint32_t result;
        uint16_t sourceop = ( (uint16_t)( bitoff & 0xFF ) | ( (uint16_t)( bitcnt & 0xFF ) << 8u ) );
        // AT&T syntax is undocumented and thus utter trash.
        // For the BEXTR 2-source-operands, 1-dest-operands instruction, when they say inversed dst and src operands
        // they...
        // 1) DON'T MEAN that the dst and src operand blocks are JUST swapped
        // 2) DO MEAN that all operands to registers are inversed in their order in comparison to official Intel docs
        asm ( "BEXTR %k2,%k1,%k0" : "=r" (result) : "rm" (val), "r" (sourceop) );
        return result;
#elif defined(_PLATFORM_DEBUG_CODEGRAPHS)
        return __platform_generic_bitextract <uint32_t> ( val, bitoff, bitcnt ),
#endif
#ifdef __cpp_lib_is_constant_evaluated
    }
#endif
}
#endif
#if defined(_MSC_VER) && defined(_M_AMD64) || \
    defined(__GNUC__) && defined(__x86_64__) || \
    defined(_PLATFORM_DEBUG_CODEGRAPHS)
#define _PLATFORM_HAS_FAST_BITEXTRACT_64BIT
AINLINE _PLATFORM_ISCEVAL_CONSTEXPR uint64_t __platform_bitextract( uint64_t val, platformLocalBitcountType bitoff, platformLocalBitcountType bitcnt ) noexcept
{
#ifdef __cpp_lib_is_constant_evaluated
    if ( __platform_is_constant_evaluated() )
    {
        return __platform_generic_bitextract <uint64_t> ( val, bitoff, bitcnt );
    }
    else
    {
#endif
#if defined(_MSC_VER) && defined(_M_AMD64)
        return _bextr_u64( val, bitoff, bitcnt );
#elif defined(__GNUC__) && defined(__x86_64__)
        uint64_t result;
        uint16_t sourceop = ( (uint16_t)( bitoff & 0xFF ) | ( (uint16_t)( bitcnt & 0xFF ) << 8u ) );
        asm ( "BEXTR %q2,%q1,%q0" : "=r" (result) : "rm" (val), "r" (sourceop) );
        return result;
#elif defined(_PLATFORM_DEBUG_CODEGRAPHS)
        return __platform_generic_bitextract <uint64_t> ( val, bitoff, bitcnt );
#endif
#ifdef __cpp_lib_is_constant_evaluated
    }
#endif
}
#endif
template <typename numberType>
struct __platform_number_supports_bitextract : public false_type {};
template <typename numberType>
struct __platform_number_supports_bitextract <volatile numberType> : public __platform_number_supports_bitextract <numberType> {};
template <typename numberType>
struct __platform_number_supports_bitextract <const numberType> : public __platform_number_supports_bitextract <numberType> {};
template <typename numberType>
struct __platform_number_supports_bitextract <numberType&> : public __platform_number_supports_bitextract <numberType> {};
template <typename numberType>
struct __platform_number_supports_bitextract <const numberType&> : public __platform_number_supports_bitextract <numberType> {};
template <typename numberType>
struct __platform_number_supports_bitextract <numberType&&> : public __platform_number_supports_bitextract <numberType> {};
#ifdef _PLATFORM_HAS_FAST_BITEXTRACT_32BIT
template <>
struct __platform_number_supports_bitextract <uint32_t> : public true_type {};
#endif
#ifdef _PLATFORM_HAS_FAST_BITEXTRACT_64BIT
template <>
struct __platform_number_supports_bitextract <uint64_t> : public true_type {};
#endif
#endif
#endif //_PLATFORM_DISABLE_FAST_INTRINSICS

} // namespace eir

#endif //_PLATFORM_STRATEGY_HEADER_
