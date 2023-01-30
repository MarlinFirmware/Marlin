/*****************************************************************************
*
*  PROJECT:     Eir SDK
*  FILE:        eirrepo/sdk/Endian.h
*  PURPOSE:     Endianness utilities header
*
*  Find the Eir SDK at: https://osdn.net/projects/eirrepo/
*
*****************************************************************************/

#ifndef _ENDIAN_COMPAT_HEADER_
#define _ENDIAN_COMPAT_HEADER_

#include "MacroUtils.h"
#include "PlatformStrategy.h" // for _PLATFORM_FIXED_ENDIANNESS, intrinsics

// For the inline functions.
#include <stdlib.h>
#include <stdint.h> // for (u)int8/16/32/64_t

#include <type_traits>
#if __has_include(<bit>)
#include <bit>
#endif

namespace eir
{

// Endianness compatibility definitions.
namespace endian
{

#ifdef _PLATFORM_FIXED_ENDIANNESS
#define _ENDIAN_CONSTEXPR constexpr
#else
#define _ENDIAN_CONSTEXPR
#endif

#if defined(_PLATFORM_FIXED_ENDIANNESS) && defined(__cpp_if_constexpr)
#define _ENDIAN_IF_CONSTEXPR constexpr
#endif

#if defined(__GNUC__)
#undef LITTLE_ENDIAN
#undef BIG_ENDIAN
#endif

    enum class eSpecificEndian
    {
        LITTLE_ENDIAN,
        BIG_ENDIAN,
#if defined(_PLATFORM_FIXED_ENDIANNESS_ENFORCE_LE)
        DEFAULT_ENDIAN = LITTLE_ENDIAN
#elif defined(_PLATFORM_FIXED_ENDIANNESS_ENFORCE_BE)
        DEFAULT_ENDIAN = BIG_ENDIAN
#else
        // Most of the systems are little-endian nowadays, which has advantages over big-endian.
        DEFAULT_ENDIAN = LITTLE_ENDIAN
#endif
    };

    AINLINE static _ENDIAN_CONSTEXPR eSpecificEndian get_current_endianness( void ) noexcept
    {
#ifdef _PLATFORM_FIXED_ENDIANNESS
#if defined(_PLATFORM_FIXED_ENDIANNESS_ENFORCE_LE)
        return eSpecficEndian::LITTLE_ENDIAN;
#elif defined(_PLATFORM_FIXED_ENDIANNESS_ENFORCE_BE)
        return eSpecificEndian::BIG_ENDIAN;
#else
        // This does only make sense if the platform does specify endianness.
        static_assert( std::endian::native == std::endian::little || std::endian::native == std::endian::big );

        // Thanks to new C++ features we have this one in the bag!
        return ( std::endian::native == std::endian::little ) ? eSpecificEndian::LITTLE_ENDIAN : eSpecificEndian::BIG_ENDIAN;
#endif
#else
        // Calculate the endianness dynamically at run-time.
        union
        {
            uint16_t val = 0x0102;
            uint8_t low;
        };
        return ( low == 0x02 ) ? eSpecificEndian::LITTLE_ENDIAN : eSpecificEndian::BIG_ENDIAN;
#endif
    }

    AINLINE static _ENDIAN_CONSTEXPR bool is_little_endian( void ) noexcept
    {
        return ( get_current_endianness() == eSpecificEndian::LITTLE_ENDIAN );
    }

    template <eir::unsigned_integral numberType>
    AINLINE constexpr typename base_type <numberType>::type byte_swap_fast( numberType val ) noexcept
    {
#ifdef _PLATFORM_HAS_FAST_BYTESWAP
        if constexpr ( __platform_number_supports_byteswap <numberType>::value )
        {
            return __platform_byteswap( val );
        }
        else
        {
#endif
#ifdef __cpp_lib_byteswap
            // If we don't have the fast platform byteswap implementation then we could default to the C++ standard
            // implementation which might not be as fast but still pretty fine.
            return std::byteswap( val );
#else
            if constexpr ( sizeof(numberType) == 1 )
            {
                return val;
            }
            else
            {
                // I guess that this version could be better because it does not force use of memory, or a compiler who could
                // stil retranslate it back to register operations.
                numberType result = 0;

                for ( unsigned int n = 0; n < sizeof(numberType); n++ )
                {
                    result |= val & 0xFF;
                    val >>= 8u;
                    result <<= 8u;
                }

                return result;
            }
#endif
#ifdef _PLATFORM_HAS_FAST_BYTESWAP
        }
#endif
    }

    // Aligned big_endian number type.
    template <typename numberType, bool isPacked = false>
        requires ( std::is_trivially_constructible <numberType>::value )
    struct big_endian
    {
        // Required to be default for POD handling.
        inline big_endian( void ) = default;
        inline big_endian( const big_endian& ) = default;
        inline big_endian( big_endian&& ) = default;

    private:
        AINLINE void assign_data( const numberType& right ) noexcept(nothrow_copy_assignable_with <numberType, numberType>)
        {
            if _ENDIAN_IF_CONSTEXPR ( is_little_endian() )
            {
                *(numberType*)&this->data[0] = byte_swap_fast( right );
            }
            else
            {
                *(numberType*)&this->data[0] = right;
            }
        }

    public:
        inline big_endian( const numberType& right ) noexcept(nothrow_copy_assignable_with <numberType, numberType>)
        {
            assign_data( right );
        }

        inline operator numberType ( void ) const noexcept(nothrow_constructible_from <numberType, numberType&&>)
        {
            if _ENDIAN_IF_CONSTEXPR ( is_little_endian() )
            {
                return byte_swap_fast( *(const numberType*)&this->data[0] );
            }
            else
            {
                return *(numberType*)&this->data[0];
            }
        }

        inline big_endian& operator = ( const numberType& right ) noexcept(nothrow_copy_assignable_with <numberType, numberType>)
        {
            assign_data( right );

            return *this;
        }

        inline big_endian& operator = ( const big_endian& ) = default;
        inline big_endian& operator = ( big_endian&& ) = default;

    private:
        alignas(typename conditional <!isPacked, numberType, char>::type) char data[ sizeof(numberType) ];
    };

    // Aligned little_endian number type.
    template <typename numberType, bool isPacked = false>
        requires ( std::is_trivially_constructible <numberType>::value )
    struct little_endian
    {
        // Required to be default for POD handling.
        inline little_endian( void ) = default;
        inline little_endian( const little_endian& ) = default;
        inline little_endian( little_endian&& ) = default;

    private:
        AINLINE void assign_data( const numberType& right ) noexcept(nothrow_copy_assignable_with <numberType, numberType>)
        {
            if _ENDIAN_IF_CONSTEXPR ( !is_little_endian() )
            {
                *(numberType*)&this->data[0] = byte_swap_fast( right );
            }
            else
            {
                *(numberType*)&this->data[0] = right;
            }
        }

    public:
        inline little_endian( const numberType& right ) noexcept(nothrow_copy_assignable_with <numberType, numberType>)
        {
            assign_data( right );
        }

        inline operator numberType ( void ) const noexcept(nothrow_constructible_from <numberType, numberType&&>)
        {
            if _ENDIAN_IF_CONSTEXPR ( !is_little_endian() )
            {
                return byte_swap_fast( *(const numberType*)&this->data[0] );
            }
            else
            {
                return *(numberType*)&this->data[0];
            }
        }

        inline little_endian& operator = ( const numberType& right ) noexcept(nothrow_copy_assignable_with <numberType, numberType>)
        {
            assign_data( right );

            return *this;
        }

        inline little_endian& operator = ( const little_endian& ) = default;
        inline little_endian& operator = ( little_endian&& ) = default;

    private:
        alignas(typename conditional <!isPacked, numberType, char>::type) char data[ sizeof(numberType) ];
    };

    // Shortcuts for the packed endian structs for use in packed serialization structs.
    template <typename structType>
    using p_big_endian = big_endian <structType, true>;

    template <typename structType>
    using p_little_endian = little_endian <structType, true>;
};

}; // namespace eir

// For compatibility with older code.
namespace endian = eir::endian;

#endif //_ENDIAN_COMPAT_HEADER_
