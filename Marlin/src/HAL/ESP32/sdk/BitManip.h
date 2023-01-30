/*****************************************************************************
*
*  PROJECT:     Eir SDK
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        eirrepo/sdk/BitManip.h
*  PURPOSE:     Low-level bit manipulation acceleration helpers
*
*  Find the Eir SDK at: https://osdn.net/projects/eirrepo/
*  Multi Theft Auto is available from http://www.multitheftauto.com/
*
*****************************************************************************/

#ifndef _EIR_BITMANIP_HEADER_
#define _EIR_BITMANIP_HEADER_

#include "MacroUtils.h"
#include "PlatformStrategy.h"
#include "Arith.h"

#ifdef _DEBUG
#include <assert.h>
#endif

namespace eir
{

template <unsigned_integral unsignedNumberType>
AINLINE constexpr unsignedNumberType ROTL( unsignedNumberType value, platformLocalBitcountType rotBy ) noexcept
{
#ifdef _PLATFORM_HAS_FAST_BITROTATE
    if constexpr ( __platform_number_supports_brotate <unsignedNumberType>::value )
    {
        return __platform_brotate_l( value, rotBy );
    }
    else
    {
#endif
        constexpr platformLocalBitcountType bitCount = sizeof(value) * 8u;

        return ( value << rotBy ) | ( value >> ( bitCount - rotBy ) );
#ifdef _PLATFORM_HAS_FAST_BITROTATE
    }
#endif
}
template <unsigned_integral unsignedNumberType>
AINLINE constexpr unsignedNumberType ROTR( unsignedNumberType value, platformLocalBitcountType rotBy ) noexcept
{
#ifdef _PLATFORM_HAS_FAST_BITROTATE
    if constexpr ( __platform_number_supports_brotate <unsignedNumberType>::value )
    {
        return __platform_brotate_r( value, rotBy );
    }
    else
    {
#endif
        constexpr platformLocalBitcountType bitCount = sizeof(value) * 8u;

        return ( value >> rotBy ) | ( value << ( bitCount - rotBy ) );
#ifdef _PLATFORM_HAS_FAST_BITROTATE
    }
#endif
}

template <typename T>
struct is_bitfield_view : public false_type {};
template <typename T> requires ( is_bitfield_view <typename base_type <T>::type>::value )
struct is_bitfield_view <T> : public true_type {};
template <typename T>
struct plain_type_bitfield : public plain_type <T> {};
template <typename T> requires ( is_bitfield_view <T>::value && is_plain_type <T>::value == false )
struct plain_type_bitfield <T> : public plain_type_bitfield <typename plain_type <T>::type> {};

// Replaces bits inside of a single unsigned integer.
template <typename srcNumType, typename dstNumType>
    requires ( is_unsigned_integral <typename plain_type_bitfield <srcNumType>::type>::value &&
               is_unsigned_integral <typename plain_type_bitfield <dstNumType>::type>::value )
AINLINE constexpr void replace_bits( dstNumType& dst, const srcNumType& src, platformLocalBitcountType bitoff, platformLocalBitcountType bitcnt ) noexcept;

// Extracts bits from a single unsigned integer.
template <typename numberType>
    requires ( is_unsigned_integral <typename plain_type_bitfield <numberType>::type>::value )
AINLINE constexpr typename plain_type_bitfield <numberType>::type extract_bits( const numberType& val, platformLocalBitcountType bitoff, platformLocalBitcountType bitcnt ) noexcept;

// numberType has to be unsigned integer.
template <typename numberType>
struct BitfieldNumberSelectionView
{
    template <typename, typename> friend struct StaticBitfieldNumberSelectionView;

    AINLINE BitfieldNumberSelectionView( numberType *num, platformLocalBitcountType bitoff = 0u, platformLocalBitcountType bitcnt = sizeof(numberType)*8u ) noexcept
        : num( num ), bitoff( bitoff ), bitcnt( bitcnt )
    {
        return;
    }
    AINLINE BitfieldNumberSelectionView( const BitfieldNumberSelectionView& view, platformLocalBitcountType bitoff, platformLocalBitcountType bitcnt = sizeof(numberType)*8u ) noexcept
        : BitfieldNumberSelectionView( view.num, bitoff, bitcnt )
    {
        return;
    }
    AINLINE BitfieldNumberSelectionView( const BitfieldNumberSelectionView& ) = default;

    AINLINE void Write( typename plain_type <numberType>::type val ) noexcept
    {
        replace_bits( *num, val, this->bitoff, this->bitcnt );
    }

    AINLINE typename plain_type <numberType>::type Read( void ) const noexcept
    {
        return extract_bits( *num, this->bitoff, this->bitcnt );
    }

    AINLINE platformLocalBitcountType GetBitOffset( void ) const noexcept
    {
        return bitoff;
    }
    AINLINE platformLocalBitcountType GetBitCount( void ) const noexcept
    {
        return bitcnt;
    }

    // Helpers for nicety.
    AINLINE BitfieldNumberSelectionView& operator = ( typename plain_type <numberType>::type val ) noexcept
    {
        this->Write( val );
        return *this;
    }
    AINLINE operator typename plain_type <numberType>::type ( void ) const noexcept
    {
        return this->Read();
    }

    AINLINE platformLocalBitcountType GetRegionStart( void ) const noexcept
    {
        return this->bitoff;
    }
    AINLINE platformLocalBitcountType GetRegionEnd( void ) const noexcept
    {
        return this->bitoff + this->bitcnt;
    }

private:
    numberType *num;
    platformLocalBitcountType bitoff;
    platformLocalBitcountType bitcnt;
};

template <typename... args>
struct is_bitfield_view <BitfieldNumberSelectionView <args...>> : public true_type {};

template <typename insideNumberType, typename numberType>
struct StaticBitfieldNumberSelectionView
{
    static constexpr platformLocalBitcountType bitcnt = sizeof(insideNumberType)*8u;

    AINLINE StaticBitfieldNumberSelectionView( numberType *num, platformLocalBitcountType bitoff = 0u ) noexcept
        : num( num ), bitoff( bitoff )
    {
        return;
    }
    AINLINE StaticBitfieldNumberSelectionView( BitfieldNumberSelectionView <numberType> *bview ) noexcept
        : num( bview->num ), bitoff( bview->bitoff )
    {
        return;
    }
    AINLINE StaticBitfieldNumberSelectionView( const StaticBitfieldNumberSelectionView& ) = default;

    AINLINE void Write( typename plain_type <insideNumberType>::type val ) noexcept
    {
        replace_bits( *num, val, this->bitoff, bitcnt );
    }

    AINLINE typename plain_type <insideNumberType>::type Read( void ) const noexcept
    {
        return (typename plain_type <insideNumberType>::type)extract_bits( *num, this->bitoff, bitcnt );
    }

    AINLINE constexpr platformLocalBitcountType GetBitCount( void ) const noexcept
    {
        return bitcnt;
    }

    // Helpers for nicety.
    AINLINE StaticBitfieldNumberSelectionView& operator = ( typename plain_type <insideNumberType>::type val ) noexcept
    {
        this->Write( val );
        return *this;
    }
    AINLINE operator typename plain_type <insideNumberType>::type ( void ) const noexcept
    {
        return this->Read();
    }

    AINLINE platformLocalBitcountType GetRegionStart( void ) const noexcept
    {
        return this->bitoff;
    }
    AINLINE platformLocalBitcountType GetRegionEnd( void ) const noexcept
    {
        return this->bitoff + bitcnt;
    }

    template <typename T>
    AINLINE BitfieldNumberSelectionView <numberType>& SharedBitRegion( platformLocalBitcountType right_start, platformLocalBitcountType right_end ) noexcept
    {
        platformLocalBitcountType left_start = this->bitoff;
        platformLocalBitcountType left_end = left_start + bitcnt;

        return BitfieldNumberSelectionView( this->num, left_start < right_start ? left_start : right_start, left_end < right_end ? left_end : right_end );
    }

private:
    numberType *num;
    platformLocalBitcountType bitoff;
};

template <typename... args>
struct is_bitfield_view <StaticBitfieldNumberSelectionView <args...>> : public true_type {};

// Helper to abstract away bitfield type checking.
template <typename IT, typename T>
struct plain_type_bitfield <StaticBitfieldNumberSelectionView <IT, T>> : public plain_type <IT> {};
template <typename T>
struct plain_type_bitfield <BitfieldNumberSelectionView <T>> : public plain_type <T> {};

template <typename numberType>
AINLINE constexpr platformLocalBitcountType get_bitcount( const numberType& v ) noexcept
{
    if constexpr ( is_bitfield_view <numberType>::value )
    {
        return v.GetBitCount();
    }
    else
    {
        return sizeof(numberType)*8u;
    }
}

template <typename srcNumType, typename dstNumType>
    requires ( is_unsigned_integral <typename plain_type_bitfield <srcNumType>::type>::value &&
               is_unsigned_integral <typename plain_type_bitfield <dstNumType>::type>::value )
AINLINE constexpr void replace_bits( dstNumType& dst, const srcNumType& src, platformLocalBitcountType bitoff, platformLocalBitcountType bitcnt ) noexcept
{
    // bitoff between 0 and sizeof(numberType)*8u
    // bitcnt between 1 and sizeof(numberType)*8u

    platformLocalBitcountType src_bitcount = get_bitcount( src );
    platformLocalBitcountType dst_bitcount = get_bitcount( dst );

#ifdef _DEBUG
    assert( bitcnt <= dst_bitcount );
#endif

    if ( bitcnt >= dst_bitcount )
    {
        dst = (typename plain_type_bitfield <dstNumType>::type)src;
    }
    else
    {
        typedef typename plain_type_bitfield <srcNumType>::type srcFastNumType;
        typedef typename plain_type_bitfield <dstNumType>::type dstFastNumType;

        // In case of bitfield, do not modify the original value, as the bitfield is a view.
        srcFastNumType src_val = src;

        dstFastNumType dst_bitmask;
        if ( bitcnt < src_bitcount )
        {
            srcFastNumType bitmask = ((srcFastNumType)1u<<bitcnt)-1;
            src_val &= bitmask;
            dst_bitmask = (dstFastNumType)bitmask;
        }
        else
        {
            dst_bitmask = ((dstFastNumType)1u<<bitcnt)-1;
        }
        dstFastNumType tmp;
        dstFastNumType dst_val;
#ifdef _PLATFORM_HAS_FAST_BITROTATE
        if constexpr ( __platform_number_supports_brotate <dstFastNumType>::value )
        {
            dst_val = (dstFastNumType)src_val;
            tmp = __platform_brotate_r( dst, bitoff );
        }
        else
        {
#endif
            dst_bitmask <<= bitoff;
            dst_val = ((dstFastNumType)src_val << bitoff);
            tmp = dst;
#ifdef _PLATFORM_HAS_FAST_BITROTATE
        }
#endif
        tmp &= ~dst_bitmask;
        tmp |= dst_val;
#ifdef _PLATFORM_HAS_FAST_BITROTATE
        if constexpr ( __platform_number_supports_brotate <dstFastNumType>::value )
        {
            dst = __platform_brotate_l( tmp, bitoff );
        }
        else
        {
#endif
            dst = tmp;
#ifdef _PLATFORM_HAS_FAST_BITROTATE
        }
#endif
    }
}

template <typename numberType>
    requires ( is_unsigned_integral <typename plain_type_bitfield <numberType>::type>::value )
AINLINE constexpr typename plain_type_bitfield <numberType>::type extract_bits( const numberType& val, platformLocalBitcountType bitoff, platformLocalBitcountType bitcnt ) noexcept
{
    // bitoff between 0 and sizeof(numberType)*8u
    // bitcnt between 1 and sizeof(numberType)*8u

    if constexpr ( is_bitfield_view <numberType>::value )
    {
        return extract_bits( val.Read(), bitoff, bitcnt );
    }
    else
    {
        if ( bitcnt == sizeof(numberType)*8u )
        {
            return val;
        }
        else
        {
#ifdef _PLATFORM_HAS_FAST_BITEXTRACT
            if constexpr ( __platform_number_supports_bitextract <numberType>::value )
            {
                return __platform_bitextract( val, bitoff, bitcnt );
            }
            else
            {
#endif
                typedef typename no_volatile <numberType>::type fastNumberType;

                fastNumberType bitmask = ((fastNumberType)1u<<bitcnt)-1;
                fastNumberType tmp = val;
                tmp >>= bitoff;
                tmp &= bitmask;
                return tmp;
            }
#ifdef _PLATFORM_HAS_FAST_BITEXTRACT
        }
#endif
    }
}

#if defined(_PLATFORM_HAS_FAST_BITREVERSE)
AINLINE constexpr platformNativeWordType reverse_bitorder( platformNativeWordType v ) noexcept
{
    return __platform_bitreverse( v );
}
template <unsigned_integral numberType>
AINLINE constexpr numberType reverse_bitorder( numberType v ) noexcept
{
    // IDEA: implement the other lesser-sized integer versions using the native word variant and then just perform
    // a bit-shift to get the result, resulting in a two-op reverse_bitorder!

    // We assume that all numberType types are sized a power-of-two, meaning that each bigger sized number is
    // also at least double the size of platformNativeWordType.
    // This optional constexpr-if is fine because both code-graphs would compile anyway.
    if constexpr (sizeof(v) > sizeof(platformNativeWordType))
    {
        const size_t num_units = sizeof(v) / sizeof(platformNativeWordType);

        numberType result = 0u;

        for (size_t n = 0; n < num_units; n++)
        {
            platformNativeWordType buf = (platformNativeWordType)( v >> (n * (sizeof(platformNativeWordType)*8u)) );
            buf = __platform_bitreverse(buf);
            result |= (numberType)buf << ( (num_units-1) - n ) * (sizeof(platformNativeWordType)*8u);
        }
    }
    else
    {
        platformNativeWordType buf = v;
        buf = __platform_bitreverse(buf);
        return (numberType)( buf >> ((sizeof(platformNativeWordType) - sizeof(v)) * 8u) );
    }
}
#else
// Slow but generic variant.
template <unsigned_integral numberType>
constexpr AINLINE numberType reverse_bitorder( numberType v ) noexcept
{
    // MSVC cannot constant evaluate this function. Just wow.
    numberType result = 0u;

    for ( platformLocalBitcountType n = 0; n < (platformLocalBitcountType)(sizeof(numberType) * 8u); n++ )
    {
        result <<= 1;
        result |= v & 0x1;
        v >>= 1;
    }
    return result;
}
#endif

template <typename numberType> requires ( unsigned_integral <typename nospecmod_type <numberType>::type> )
struct BitIncrementalAccessor
{
private:
    using fastNumberType = typename nospecmod_type <numberType>::type;
    
public:
    AINLINE BitIncrementalAccessor( void ) noexcept
        : data( 0u ), bitoff( 0u )
    {}
    AINLINE BitIncrementalAccessor( const BitIncrementalAccessor& ) = default;
    AINLINE BitIncrementalAccessor( BitIncrementalAccessor&& ) = default;

    AINLINE BitIncrementalAccessor& operator = ( const BitIncrementalAccessor& ) = default;
    AINLINE BitIncrementalAccessor& operator = ( BitIncrementalAccessor&& ) = default;

    AINLINE void SetData( const numberType& data ) noexcept
    {
        this->data = data;
        this->bitoff = 0u;
    }

    AINLINE fastNumberType GetData( void ) const noexcept
    {
#ifdef _PLATFORM_HAS_FAST_BITROTATE
        if constexpr ( mutable_type <numberType> && __platform_number_supports_brotate <numberType>::value )
        {
            if ( this->bitoff == 0 )
            {
                return this->data;
            }
            else
            {
                return __platform_brotate_l( this->data, this->bitoff );
            }
        }
        else
        {
#endif
            return this->data;
#ifdef _PLATFORM_HAS_FAST_BITROTATE
        }
#endif
    }

    template <unsigned_integral writeNumType>
        requires ( mutable_type <numberType> && sizeof(writeNumType) <= sizeof(numberType) )
    AINLINE void WriteBits( writeNumType val, unsigned int bitcnt ) noexcept
    {
#ifdef _PLATFORM_HAS_FAST_BITROTATE
        if constexpr ( __platform_number_supports_brotate <numberType>::value )
        {
            if ( bitcnt == sizeof(numberType)*8u )
            {
                this->data = val;
            }
            else
            {
                fastNumberType data = this->data;
                fastNumberType bmask = ( (fastNumberType)1u << bitcnt ) - 1;
                data &= ~bmask;
                data |= ( val & (writeNumType)bmask );
                this->data = __platform_brotate_r( data, bitcnt );
            }
        }
        else
        {
#endif
            replace_bits( this->data, val, this->bitoff, (platformLocalBitcountType)bitcnt );
#ifdef _PLATFORM_HAS_FAST_BITROTATE
        }
#endif
        this->bitoff = ( this->bitoff + bitcnt ) % ( sizeof(numberType)*8u );
    }

    template <unsigned_integral readNumType> requires ( sizeof(readNumType) <= sizeof(numberType) )
    AINLINE readNumType ReadBits( unsigned int bitcnt ) noexcept
    {
        readNumType val;
#ifdef _PLATFORM_HAS_FAST_BITROTATE
        if constexpr ( mutable_type <numberType> && __platform_number_supports_brotate <numberType>::value )
        {
            if ( bitcnt == sizeof(numberType)*8u )
            {
                val = this->data;
            }
            else
            {
                fastNumberType data = this->data;
                fastNumberType bmask = ( (fastNumberType)1u << bitcnt ) - 1;
                val = ( data & bmask );
                this->data = __platform_brotate_r( data, bitcnt );
            }
        }
        else
        {
#endif
            val = (readNumType)extract_bits( this->data, this->bitoff, (platformLocalBitcountType)bitcnt );
#ifdef _PLATFORM_HAS_FAST_BITROTATE
        }
#endif
        this->bitoff = ( this->bitoff + bitcnt ) % ( sizeof(numberType)*8u );
        return val;
    }

private:
    numberType data;
    unsigned int bitoff;
};

} // namespace eir

#endif //_EIR_BITMANIP_HEADER_