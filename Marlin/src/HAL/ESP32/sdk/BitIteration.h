/*****************************************************************************
*
*  PROJECT:     Eir SDK
*  FILE:        eirrepo/sdk/BitIteration.h
*  PURPOSE:     Bit-based iteration utilities
*
*  Find the Eir SDK at: https://osdn.net/projects/eirrepo/
*
*****************************************************************************/

#include "MetaHelpers.h"
#include "Tuple.h"
#include "Compare.h"
#include "PlatformStrategy.h"

#include <stdlib.h>
#include <stdint.h>

#if __has_include(<compare>)
#include <compare>
#endif

#ifdef _DEBUG
#include <assert.h>
#endif

namespace eir
{

// Type-trait.
template <typename>
struct is_bitnumberiterator : false_type {};

template <typename T>
concept bitnumberiterator_type = is_bitnumberiterator <T>::value;

// Represents an iterator over bit-arrays. Segments the bit array into "hostBitCount" slices, each referred to as
// "number" indexed in order of occurrence.
template <platformLocalBitcountType _hostBitCount>
struct BitNumberIterator
{
    static constexpr platformLocalBitcountType hostBitCount = _hostBitCount;

    AINLINE constexpr BitNumberIterator( size_t numOffset = 0, platformLocalBitcountType bitOffset = 0 ) noexcept : numOffset( numOffset ), bitOffset( bitOffset )
    {
#ifdef _DEBUG
        assert( bitOffset < hostBitCount );
#endif
    }
    AINLINE BitNumberIterator( const BitNumberIterator& ) = default;
    template <bitnumberiterator_type otherIterType> requires ( same_as <BitNumberIterator, otherIterType> == false )
    AINLINE BitNumberIterator( const otherIterType& other ) noexcept
    {
        auto bitCnt = other.getTotalBitOffset();
        this->numOffset = (size_t)( bitCnt / hostBitCount );
        this->bitOffset = ( bitCnt % hostBitCount );
    }

    AINLINE BitNumberIterator& operator = ( const BitNumberIterator& ) = default;

    template <bitnumberiterator_type otherIterType> requires ( same_as <BitNumberIterator, otherIterType> == false )
    AINLINE BitNumberIterator& operator = ( const otherIterType& other ) noexcept
    {
        auto bitCnt = other.getTotalBitOffset();
        this->numOffset = (size_t)( bitCnt / hostBitCount );
        this->bitOffset = ( bitCnt % hostBitCount );
        return *this;
    }

    AINLINE constexpr void addBits( platformBitCountType bitCount ) noexcept
    {
        platformBitCountType numBitOff = this->getTotalBitOffset();

        platformBitCountType newBitOff = numBitOff + bitCount;

        this->numOffset = (size_t)( newBitOff / hostBitCount );
        this->bitOffset = (platformLocalBitcountType)( newBitOff % hostBitCount );
    }
    AINLINE constexpr void addBytes( size_t byteCount ) noexcept
    {
        const platformLocalBitcountType hostByteCount = ( hostBitCount / 8u );

        size_t numCountByBytes = byteCount / hostByteCount;
        size_t remainderByBytes = byteCount % hostByteCount;

        this->numOffset += numCountByBytes;
        this->addBits( remainderByBytes * 8u );
    }

    AINLINE constexpr platformBitCountType distBits( const BitNumberIterator& right ) const noexcept
    {
        return ( this->getTotalBitOffset() - right.getTotalBitOffset() );
    }
    AINLINE constexpr size_t distBytes( const BitNumberIterator& right ) const noexcept
    {
        return ( this->getTotalByteOffset() - right.getTotalByteOffset() );
    }

    AINLINE constexpr size_t getNumberOffset( void ) const noexcept
    {
        return this->numOffset;
    }
    AINLINE constexpr platformLocalBitcountType getLocalBitOffset( void ) const noexcept
    {
        return this->bitOffset;
    }
    AINLINE constexpr platformBitCountType getTotalBitOffset( void ) const noexcept
    {
        return ( (platformBitCountType)this->numOffset * hostBitCount + this->bitOffset );
    }
    AINLINE constexpr size_t getTotalByteOffset( void ) const noexcept
    {
        if constexpr ( hostBitCount % 8u == 0 )
        {
            if constexpr ( hostBitCount == 8u )
            {
                return (size_t)( this->numOffset * ( hostBitCount / 8u ) );
            }
            else
            {
                return (size_t)( this->numOffset * ( hostBitCount / 8u ) + this->bitOffset / 8u );
            }
        }
        else
        {
            return (size_t)( getTotalBitOffset() / 8u );
        }
    }

    // Optimized variant.
    AINLINE constexpr BitNumberIterator& operator += ( const BitNumberIterator& right ) noexcept
    {
        this->numOffset += right.numOffset;
        this->addBits( right.bitOffset );
        return *this;
    }
    // Less-optimized variant.
    template <bitnumberiterator_type RT>
    AINLINE constexpr BitNumberIterator& operator += ( const RT& right ) noexcept
    {
        this->addBits( right.getTotalBitOffset() );
        return *this;
    }

    AINLINE constexpr BitNumberIterator operator + ( const BitNumberIterator& right ) const noexcept
    {
        BitNumberIterator res = *this;
        res += right;
        return res;
    }

    AINLINE constexpr BitNumberIterator& operator -= ( const BitNumberIterator& right ) noexcept
    {
        this->numOffset -= right.numOffset;
        platformLocalBitcountType bitOffset = this->bitOffset;
        platformLocalBitcountType subBitOffset = right.bitOffset;

        if (bitOffset < subBitOffset)
        {
            this->numOffset--;
            this->bitOffset = ( hostBitCount - ( subBitOffset - bitOffset ) );
        }
        else
        {
            this->bitOffset = ( bitOffset - subBitOffset );
        }
        return *this;
    }
    template <bitnumberiterator_type RT>
    AINLINE constexpr BitNumberIterator& operator -= ( const RT& right ) noexcept
    {
        platformBitCountType res_bitc = ( this->getTotalBitOffset() - right.getTotalBitOffset() );
        this->numOffset = (size_t)( res_bitc / hostBitCount );
        this->bitOffset = ( res_bitc % hostBitCount );
        return *this;
    }

    AINLINE constexpr BitNumberIterator operator - ( const BitNumberIterator& right ) noexcept
    {
        BitNumberIterator res = *this;
        res -= right;
        return res;
    }

    AINLINE friend constexpr bool operator == ( const BitNumberIterator& L, const BitNumberIterator& R ) noexcept
    {
        return ( ( L.numOffset == R.numOffset ) && ( L.bitOffset == R.bitOffset ) );
    }
    AINLINE static constexpr eCompResult compare( const BitNumberIterator& L, const BitNumberIterator& R ) noexcept
    {
        eCompResult numOff_cmpres = eir::DefaultValueCompare( L.numOffset, R.numOffset );

        if ( numOff_cmpres != eCompResult::EQUAL )
        {
            return numOff_cmpres;
        }

        eCompResult bitOffset_cmpres = eir::DefaultValueCompare( L.bitOffset, R.bitOffset );

        return bitOffset_cmpres;
    }
#if defined(__cpp_impl_three_way_comparison) && !defined(_DEBUG_OLDSTYLE_COMPARISONS)
    AINLINE friend constexpr auto operator <=> ( const BitNumberIterator& L, const BitNumberIterator& R ) noexcept
    {
        return eir::cmpres_to_ordering <std::strong_ordering> ( compare( L, R ) );
    }
#else
    // Required for old compilers.
    AINLINE friend constexpr bool operator != ( const BitNumberIterator& L, const BitNumberIterator& R ) noexcept
    {
        return !( operator == ( L, R ) );
    }
    AINLINE friend constexpr bool operator < ( const BitNumberIterator& L, const BitNumberIterator& R ) noexcept
    {
        return compare( L, R ) == eCompResult::LEFT_LESS;
    }
    AINLINE friend constexpr bool operator > ( const BitNumberIterator& L, const BitNumberIterator& R ) noexcept
    {
        return compare( L, R ) == eCompResult::LEFT_GREATER;
    }
    AINLINE friend constexpr bool operator <= ( const BitNumberIterator& L, const BitNumberIterator& R ) noexcept
    {
        auto res = compare( L, R );
        return ( res == eCompResult::LEFT_LESS || res == eCompResult::EQUAL );
    }
    AINLINE friend constexpr bool operator >= ( const BitNumberIterator& L, const BitNumberIterator& R ) noexcept
    {
        auto res = compare( L, R );
        return ( res == eCompResult::LEFT_GREATER || res == eCompResult::EQUAL );
    }
#endif

private:
    size_t numOffset;
    platformLocalBitcountType bitOffset;
};
template <platformLocalBitcountType S>
struct is_bitnumberiterator <BitNumberIterator <S>> : true_type {};
template <typename T> requires ( is_bitnumberiterator <typename base_type <T>::type>::value )
struct is_bitnumberiterator <T> : true_type {};

template <bitnumberiterator_type LT, bitnumberiterator_type RT> requires ( same_as <LT, RT> == false )
AINLINE constexpr bool operator == ( const LT& L, const RT& R ) noexcept
{
    return L.getTotalBitOffset() == R.getTotalBitOffset();
}
#if defined(__cpp_impl_three_way_comparison) && !defined(_DEBUG_OLDSTYLE_COMPARISONS)
template <bitnumberiterator_type LT, bitnumberiterator_type RT> requires ( same_as <LT, RT> == false )
AINLINE constexpr auto operator <=> ( const LT& L, const RT& R ) noexcept
{
    return L.getTotalBitOffset() <=> R.getTotalBitOffset();
}
#else
// Required for old compilers.
template <bitnumberiterator_type LT, bitnumberiterator_type RT> requires ( same_as <LT, RT> == false )
AINLINE constexpr bool operator != ( const LT& L, const RT& R ) noexcept
{
    return L.getTotalBitOffset() != R.getTotalBitOffset();
}
template <bitnumberiterator_type LT, bitnumberiterator_type RT> requires ( same_as <LT, RT> == false )
AINLINE constexpr bool operator < ( const LT& L, const RT& R ) noexcept
{
    return ( L.getTotalBitOffset() < R.getTotalBitOffset() );
}
template <bitnumberiterator_type LT, bitnumberiterator_type RT> requires ( same_as <LT, RT> == false )
AINLINE constexpr bool operator > ( const LT& L, const RT& R ) noexcept
{
    return ( L.getTotalBitOffset() > R.getTotalBitOffset() );
}
template <bitnumberiterator_type LT, bitnumberiterator_type RT> requires ( same_as <LT, RT> == false )
AINLINE constexpr bool operator <= ( const LT& L, const RT& R ) noexcept
{
    return ( L.getTotalBitOffset() <= R.getTotalBitOffset() );
}
template <bitnumberiterator_type LT, bitnumberiterator_type RT> requires ( same_as <LT, RT> == false )
AINLINE constexpr bool operator >= ( const LT& L, const RT& R ) noexcept
{
    return ( L.getTotalBitOffset() >= R.getTotalBitOffset() );
}
#endif

// Helper for deciding a compatible iterator based on a type.
template <typename numberType>
using BitNumberIteratorForStruct = BitNumberIterator <sizeof(numberType) * 8u>;

namespace BitIterationUtils
{

template <typename T>
concept bit_iterator_advance_min_iterType =
    tuple_type <typename base_type <T>::type> && T::count == 2 && ( bitnumberiterator_type <typename sequence_get <0, typename T::type_list>::type> && bitnumberiterator_type <typename sequence_get <1, typename T::type_list>::type> ) ||
    unsigned_integral <typename base_type <T>::type>;

} // namespace BitIterationUtils

template <BitIterationUtils::bit_iterator_advance_min_iterType... iterTypes>
AINLINE platformBitCountType bit_advance_min( const iterTypes&... iterPairs ) noexcept
{
    if constexpr ( sizeof...(iterPairs) == 0 )
    {
        return 0u;
    }
    else
    {
        platformBitCountType bitadv = 0u;
        bool has_bitadv = false;

        auto lamb = [&] <typename T> ( const T& iterPair ) LAINLINE
        {
            if constexpr ( tuple_type <T> )
            {
                if constexpr ( T::count == 2 )
                {
                    auto start = iterPair.template Get <0> ().getTotalBitOffset();
                    auto end = iterPair.template Get <1> ().getTotalBitOffset();

                    platformBitCountType adv;

                    if ( start < end )
                    {
                        adv = ( end - start );
                    }
                    else
                    {
                        adv = 0u;
                    }

                    if (has_bitadv == false || bitadv > adv)
                    {
                        bitadv = adv;
                        has_bitadv = true;
                    }
                }
            }
            else if constexpr ( unsigned_integral <T> )
            {
                if (has_bitadv == false || bitadv > iterPair)
                {
                    bitadv = iterPair;
                    has_bitadv = true;
                }
            }
        };
        ( lamb( iterPairs ), ... );

        return bitadv;
    }
}

template <bitnumberiterator_type startIterType, BitIterationUtils::bit_iterator_advance_min_iterType... iterTypes>
AINLINE startIterType bit_iterator_advance_min( const startIterType& iter, const iterTypes&... iterPairs ) noexcept
{
    if constexpr ( sizeof...(iterPairs) == 0 )
    {
        return iter;
    }
    else
    {
        startIterType end_iter = iter;

        end_iter.addBits( bit_advance_min( iterPairs... ) );

        return end_iter;
    }
}

template <BitIterationUtils::bit_iterator_advance_min_iterType... iterTypes>
AINLINE size_t bit_advance_min_bytes( const iterTypes&... iterPairs ) noexcept
{
    if constexpr ( sizeof...(iterPairs) == 0 )
    {
        return 0u;
    }
    else
    {
        size_t bytesadv = 0u;
        bool has_bytesadv = false;

        auto lamb = [&] <typename T> ( const T& iterPair ) LAINLINE
        {
            if constexpr ( tuple_type <T> )
            {
                if constexpr ( T::count == 2 )
                {
                    auto start = iterPair.template Get <0> ().getTotalByteOffset();
                    auto end = iterPair.template Get <1> ().getTotalByteOffset();

                    size_t adv;

                    if ( start < end )
                    {
                        adv = end - start;
                    }
                    else
                    {
                        adv = 0u;
                    }

                    if (has_bytesadv == false || bytesadv > adv)
                    {
                        bytesadv = adv;
                        has_bytesadv = true;
                    }
                }
            }
            else if constexpr ( unsigned_integral <T> )
            {
                if (has_bytesadv == false || bytesadv > iterPair)
                {
                    bytesadv = iterPair;
                    has_bytesadv = true;
                }
            }
        };
        ( lamb( iterPairs ), ... );

        return bytesadv;
    }
}

template <bitnumberiterator_type startIterType, BitIterationUtils::bit_iterator_advance_min_iterType... iterTypes>
AINLINE startIterType bit_iterator_advance_min_bytes( const startIterType& iter, const iterTypes&... iterPairs ) noexcept
{
    if constexpr ( sizeof...(iterPairs) == 0 )
    {
        return iter;
    }
    else
    {
        startIterType end_iter = iter;

        end_iter.addBytes( bit_advance_min_bytes( iterPairs... ) );

        return end_iter;
    }
}

} // namespace eir
