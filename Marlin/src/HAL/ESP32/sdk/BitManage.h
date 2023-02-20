/*****************************************************************************
*
*  PROJECT:     Eir SDK
*  FILE:        eirrepo/sdk/BitManage.h
*  PURPOSE:     Low-level bit manipulation acceleration helpers
*
*  Find the Eir SDK at: https://osdn.net/projects/eirrepo/
*
*****************************************************************************/

#ifndef _BIT_MANAGE_HEADER_
#define _BIT_MANAGE_HEADER_

#include <stdint.h>
#include <stdlib.h> // for size_t

#include "MemoryRaw.h"
#include "Endian.h"
#include "MetaHelpers.h"    // for no_volatile
#include "PlatformStrategy.h"
#include "Variant.h"
#include "BitManip.h"
#include "Tuple.h"
#include "Arith.h"
#include "BitIteration.h"

// Feel the power of C++20 on the embedded side.
// Documentation Video: https://youtu.be/L_o481OL0Xw (12 Jan 2023)

namespace eir
{

#ifdef __cpp_if_constexpr
#define _BITMANAGE_IF_CONSTEXPR constexpr
#else
#define _BITMANAGE_IF_CONSTEXPR
#endif

// During code-gen analysis I have detected that the inlining of the code-graphs does not lead to great results, especially
// because we use PlatformStrategy optimizations and those values are usually not retransformable by the compilers.
#ifdef _BITMANAGE_ALWAYS_INLINE
#define BM_AINLINE AINLINE
#else
#define BM_AINLINE inline
#endif

// In comparison to the above always-inline guidelines, the "brutal" always-inline guide is applied to code-graph templates
// that are known to create a bigger mess. Thus be careful about applying this attribute!
#if defined(_BITMANAGE_ALWAYS_INLINE) && defined(_BITMANAGE_BRUTAL_ALWAYS_INLINE)
#define BM_BRUTAL_AINLINE AINLINE
#else
#define BM_BRUTAL_AINLINE inline
#endif

template <typename numberType>
struct identity_number_specificator
{
    typedef numberType type;
};

template <typename T>
struct const_number_specificator
{
    typedef const T type;
};

template <typename numberType>
struct volatile_number_specificator
{
    typedef volatile numberType type;
};

template <typename numberType>
struct const_volatile_number_specificator
{
    typedef const volatile numberType type;
};

template <typename T>
struct choose_default_number_specificator
{
    template <typename Y>
    using spec = identity_number_specificator <Y>;
};
template <typename T>
struct choose_default_number_specificator <volatile T>
{
    template <typename Y>
    using spec = volatile_number_specificator <Y>;
};
template <typename T>
struct choose_default_number_specificator <const T>
{
    template <typename Y>
    using spec = const_number_specificator <Y>;
};
template <typename T>
struct choose_default_number_specificator <const volatile T>
{
    template <typename Y>
    using spec = const_volatile_number_specificator <Y>;
};

// The default type-list of hardware-supported native number types.
typedef typelist <
#if PLATFORM_WORDSIZE >= 8
    uint64_t,
#endif
#if PLATFORM_WORDSIZE >= 4
    uint32_t,
#endif
#if PLATFORM_WORDSIZE >= 2
    uint16_t,
#endif
    uint8_t
> PlatformNumberTypeList;

typedef OptTypeSelector <PlatformNumberTypeList> PlatformNumberTypeSelector;

template <typelist_type suppTypes, typename numberType, typename callbackType>
AINLINE void SelectAlignedTypeAccess( numberType& loc, size_t bytes_left, callbackType&& cb )
{
    using filteredSpecSuppTypes = typename no_volatile <typename no_const <suppTypes>::type>::type;
    using distinctSuppTypes = typename types_distinct <filteredSpecSuppTypes>::type;
    using size_selected_types = typename sizeof_greaterequal_types <numberType, distinctSuppTypes>::type;
    using forwardsSortedSuppTypes = typename sizeof_sorted_types <size_selected_types>::type;
    using sortedSuppTypes = typename types_reverse <forwardsSortedSuppTypes>::type;

    sortedSuppTypes::ForEach(
        [&] <typename T> () LAINLINE
    {
        if (is_same_as <numberType, T>::value || (bytes_left >= sizeof(T) && (uintptr_t)&loc % alignof(T) == 0))
        {
            cb.template operator() <T> ();
            return true;
        }
        return false;
    });
}

template <
    template <typename numberType> typename numberSpecific = identity_number_specificator,
    typelist_type suppTypes = PlatformNumberTypeList,
    template <typename> typename baseVariantType = BitfieldVariant
>
struct PlatformBitCache
{
    using filteredSpecSuppTypes = typename no_volatile <typename no_const <suppTypes>::type>::type;
    using distinctSuppTypes = typename types_distinct <filteredSpecSuppTypes>::type;
    using forwardsSortedSuppTypes = typename sizeof_sorted_types <distinctSuppTypes>::type;
    using sortedSuppTypes = typename types_reverse <forwardsSortedSuppTypes>::type;

    using distinct_t = typename numberSpecific <typename conditional <(filteredSpecSuppTypes::count == 1),
        typename sequence_get <0, filteredSpecSuppTypes>::type,
        void>::type
    >::type;

    BM_AINLINE PlatformBitCache( void ) = default;
    BM_AINLINE PlatformBitCache( const PlatformBitCache& ) = default;

    BM_AINLINE PlatformBitCache& operator = ( const PlatformBitCache& ) = default;

    BM_AINLINE void Flush( void ) noexcept
    {
        if constexpr ( const_type <distinct_t> == false )
        {
            this->data.Visit(
                [this]( auto& data ) LAINLINE
                {
                    *(typename numberSpecific <typename plain_type_bitfield <decltype(data)>::type>::type*PTR_NO_ALIAS)this->data_pointer = data;
                }
            );
        }

        this->data.Clear();
    }
    BM_AINLINE void Invalidate( void ) noexcept
    {
        this->data.Clear();
    }
    BM_AINLINE bool IsInitialized( void ) const noexcept { return ( this->data.IsEmpty() == false ); }

    template <typename numberType>
        requires ( sequence_contains <typename base_type <numberType>::type, sortedSuppTypes>::value )
    BM_AINLINE void PutIntoCache( numberType& num ) noexcept
    {
        this->data.Store( num );
        this->data_pointer = &num;
    }
    template <typename numberType>
        requires ( sequence_contains <typename base_type <numberType>::type, sortedSuppTypes>::value )
    BM_AINLINE numberType GetCacheValue( void ) noexcept
    {
        return this->data.template Get <numberType> ();
    }

    template <bool no_read = false, typename cacheNumType>
        requires ( sequence_contains <typename base_type <cacheNumType>::type, sortedSuppTypes>::value )
    BM_AINLINE void CacheMemoryLocation( cacheNumType *PTR_NO_ALIAS loc, size_t bytes_left ) noexcept
    {
        SelectAlignedTypeAccess <sortedSuppTypes> ( *loc, bytes_left,
            [&] <typename T> () LAINLINE
            {
                if constexpr ( no_read )
                {
                    this->data.Store( typename base_type <cacheNumType>::type() );
                }
                else
                {
                    using acc_T = typename conditional <(is_volatile <cacheNumType>::value), volatile T, T>::type;
                    this->data.Store( *(const acc_T*PTR_NO_ALIAS)loc );
                }
            }
        );
        this->data_pointer = loc;
    }

    template <typename numberType>
        requires ( sequence_contains <typename base_type <numberType>::type, sortedSuppTypes>::value )
    BM_AINLINE bool CanSatisfy( numberType& mem_val ) noexcept
    {
        if constexpr ( sortedSuppTypes::count == 1 )
        {
            // Assume alignment guarantee.
            return ( this->data_pointer == &mem_val );
        }
        else
        {
            uintptr_t numloc = (uintptr_t)&mem_val;
            uintptr_t cachenumloc = (uintptr_t)this->data_pointer;

            auto cache_size = this->data.GetSize();

            // Structures must always be aligned but I can check for that anyway.
            return /*(numloc % alignof(numberType) == 0) &&*/ (numloc >= cachenumloc && numloc + sizeof(numberType) <= cachenumloc + cache_size);
        }
    }

    template <typename numberType, typename callbackType>
        requires ( sequence_contains <typename base_type <numberType>::type, sortedSuppTypes>::value )
    BM_AINLINE void SelectRef( distinct_t *PTR_NO_ALIAS orig_ptr, callbackType&& cb ) noexcept
    {
        if constexpr ( sortedSuppTypes::count == 1 )
        {
            cb( this->data.Get() );
        }
        else
        {
            uintptr_t cachenumloc = (uintptr_t)this->data_pointer;

            uintptr_t byteoff = ( (uintptr_t)orig_ptr - cachenumloc );

            if _ENDIAN_CONSTEXPR ( endian::get_current_endianness() == endian::eSpecificEndian::BIG_ENDIAN )
            {
                byteoff = ( this->data.GetSize() - sizeof(numberType) ) - byteoff;
            }

            this->data.VisitBitsFrom( byteoff,
                [&]( auto& ref ) LAINLINE
            {
                StaticBitfieldNumberSelectionView <numberType, typename plain_type_bitfield <decltype(ref)>::type> bfacc( &ref );
                cb( bfacc );
            });
        }
    }
    BM_AINLINE distinct_t*PTR_NO_ALIAS GetStoragePointer( void ) const noexcept
    {
        return this->data_pointer;
    }

    template <typename numberType, typename callbackType>
        requires ( sequence_contains <typename base_type <numberType>::type, sortedSuppTypes>::value )
    BM_AINLINE void SelectCachedBits( numberType& mem_val, size_t bytes_left, callbackType&& cb )
    {
#ifdef _DEBUG
        assert( bytes_left >= sizeof(numberType) );
#endif

        // All number types have a power-of-two size.
        if (this->data.IsEmpty() == false)
        {
            if ( this->template CanSatisfy <numberType> ( mem_val ) )
            {
                goto returnResult;
            }

            // We cannot use the previous cache anymore, so flush it.
            Flush();
        }

        // Attempt to fill cache memory.
        CacheMemoryLocation( &mem_val, bytes_left );

    returnResult:
        this->template SelectRef <numberType> ( &mem_val, (callbackType&&)cb );
    }

    template <typename numberType>
        requires ( sequence_contains <typename base_type <numberType>::type, sortedSuppTypes>::value )
    BM_AINLINE typename no_const <typename no_volatile <numberType>::type>::type ReadCacheInto( numberType& mem_val, size_t bytes_left = sizeof(numberType) ) noexcept
    {
        typename no_const <typename no_volatile <numberType>::type>::type result = 0u;

        this->SelectCachedBits( mem_val, bytes_left,
            [&]( auto& val ) LAINLINE
            {
                // Trim-off any bits that are too much, fill in any bits with zeroes that are missing.
                result = (numberType)val;
            }
        );

        return result;
    }

    template <mutable_type T>
    BM_AINLINE T ExtractNumberEndian( uint8_t byteoff ) const noexcept
    {
        unsigned int real_byteoff = byteoff;

        if _ENDIAN_CONSTEXPR ( endian::get_current_endianness() == endian::eSpecificEndian::BIG_ENDIAN )
        {
            real_byteoff = ( this->data.GetSize() - sizeof(T) ) - real_byteoff;
        }

        typename no_volatile <T>::type ret = 0u;
        this->data.VisitBitsFrom( real_byteoff,
            [&]( auto& ref ) LAINLINE
        {
            ret = (decltype(ret))ref;
        });
        return ret;
    }

    BM_AINLINE uint8_t GetCacheSize( void ) const noexcept
    {
        return this->data.GetSize();
    }
    BM_AINLINE distinct_t* GetOriginalDataPointer( void ) const noexcept
    {
        return this->data_pointer;
    }

private:
    typename conditional <(sortedSuppTypes::count == 1),
        Variant <sortedSuppTypes>,
        baseVariantType <sortedSuppTypes>
    >::type data;
    distinct_t *PTR_NO_ALIAS data_pointer = nullptr;
};

// Useful structure for performing the bit-ordering conversion on whole numbers outside of the BitManager class.
// You don't get the caching optimization of BitManager though so if you risk partial operations it is still preferable to use BitManager over directly this.
struct BitOrderingConverter
{
    AINLINE _ENDIAN_CONSTEXPR BitOrderingConverter( endian::eSpecificEndian storage_endianness = endian::eSpecificEndian::DEFAULT_ENDIAN, bool storage_msbfirst = false ) noexcept
    {
        bool is_diff_byteorder = ( storage_endianness != endian::get_current_endianness() );
        bool is_diff_bitorder = ( storage_msbfirst );

        this->do_reverse_bitorder = ( is_diff_bitorder );
        this->do_reverse_byteorder = ( is_diff_byteorder != is_diff_bitorder );
#if defined(_PLATFORM_HAS_FAST_REV16)
        this->do_rev16 = false;
#endif
    }
    AINLINE constexpr BitOrderingConverter( bool revbits, bool revbytes, bool rev16 = false ) noexcept
        : do_reverse_bitorder( revbits ), do_reverse_byteorder( revbytes )
#if defined(_PLATFORM_HAS_FAST_REV16)
          , do_rev16( rev16 )
#endif
    {
        return;
    }
    AINLINE constexpr BitOrderingConverter( const BitOrderingConverter& ) = default;

    template <mutable_type numberType>
    AINLINE constexpr numberType Identity( const numberType& val ) noexcept
    {
        numberType result = val;
        if (this->do_reverse_byteorder) result = endian::byte_swap_fast( result );
        if (this->do_reverse_bitorder) result = reverse_bitorder( result );
#if defined(_PLATFORM_HAS_FAST_REV16)
        if constexpr ( __platform_number_supports_rev16 <numberType>::value )
        {
            if (this->do_rev16) result = __platform_rev16( result );
        }
#endif
        return result;
    }

    AINLINE constexpr bool DoReverseBitorder( void ) const noexcept
    {
        return this->do_reverse_bitorder;
    }
    AINLINE constexpr bool DoReverseByteorder( void ) const noexcept
    {
        return this->do_reverse_byteorder;
    }

private:
    bool do_reverse_bitorder, do_reverse_byteorder;
#if defined(_PLATFORM_HAS_FAST_REV16)
    bool do_rev16;
#endif
};

// Takes a "unitNumberType" and a "containerNumberType". Returns a bitwise-operation if, performed on "containerNumberType", it's result is
// a bitwise-concatenation of "unitNumberType" entries - each solitarily assorted from LSB to MSB - in bit-index-ascending order.
struct BitVectorizedIdentityManager
{
    AINLINE _ENDIAN_CONSTEXPR BitVectorizedIdentityManager( BitOrderingConverter conv = {} ) noexcept : bitconv( conv )
    {
        return;
    }
    AINLINE _ENDIAN_CONSTEXPR BitVectorizedIdentityManager( endian::eSpecificEndian endianness, bool msbfirst = false ) noexcept : BitVectorizedIdentityManager( BitOrderingConverter( endianness, msbfirst ) )
    {
        return;
    }
    AINLINE BitVectorizedIdentityManager( const BitVectorizedIdentityManager& ) = default;

    template <mutable_type unitNumberType, mutable_type containerNumberType>
    AINLINE Variant <BitOrderingConverter> VectorTransformGetConfig( void ) noexcept
    {
        if constexpr ( is_same_as <unitNumberType, containerNumberType>::value )
        {
            return bitconv;
        }
        else
        {
#if defined(_PLATFORM_FIXED_ENDIANNESS)
            if constexpr ( sizeof(unitNumberType) == 1 )
            {
                if constexpr ( endian::get_current_endianness() == endian::eSpecificEndian::BIG_ENDIAN )
                {
                    if ( bitconv.DoReverseBitorder() )
                    {
#if defined(_PLATFORM_HAS_FAST_BITREVERSE)
                        if constexpr ( __platform_number_supports_bitreverse <containerNumberType>::value )
                        {
                            return BitOrderingConverter( true, false );
                        }
#endif
                    }
                    else
                    {
#if defined(_PLATFORM_HAS_FAST_BYTESWAP)
                        if constexpr ( __platform_number_supports_byteswap <containerNumberType>::value )
                        {
                            return BitOrderingConverter( false, true );
                        }
#endif
                    }
                }
                else
                {
                    if ( bitconv.DoReverseBitorder() )
                    {
#if defined(_PLATFORM_HAS_FAST_BITREVERSE) && defined(_PLATFORM_HAS_FAST_BYTESWAP)
                        if constexpr ( __platform_number_supports_byteswap <containerNumberType>::value && __platform_number_supports_bitreverse <containerNumberType>::value )
                        {
                            return BitOrderingConverter( true, true );
                        }
#endif
                    }
                }
            }
            if constexpr ( sizeof(unitNumberType) == 2 )
            {
                if constexpr ( endian::get_current_endianness() == endian::eSpecificEndian::LITTLE_ENDIAN )
                {
                    if ( bitconv.DoReverseByteorder() && bitconv.DoReverseBitorder() == false )
                    {
#if defined(_PLATFORM_HAS_FAST_REV16)
                        if constexpr ( __platform_number_supports_rev16 <containerNumberType>::value )
                        {
                            return BitOrderingConverter( false, false, true );
                        }
#endif
                    }
                }
                else
                {
                    if ( bitconv.DoReverseByteorder() && bitconv.DoReverseBitorder() == false )
                    {
#if defined(_PLATFORM_HAS_FAST_REV16) && defined(_PLATFORM_HAS_FAST_BYTESWAP)
                        if constexpr ( __platform_number_supports_byteswap <containerNumberType>::value && __platform_number_supports_rev16 <containerNumberType>::value )
                        {
                            return BitOrderingConverter( false, true, true );
                        }
#endif
                    }
                }
            }
            if constexpr ( endian::get_current_endianness() == endian::eSpecificEndian::LITTLE_ENDIAN )
            {
                if ( bitconv.DoReverseBitorder() == false && bitconv.DoReverseByteorder() == false )
                {
                    return BitOrderingConverter( false, false );
                }
            }
            else
            {
#if defined(_PLATFORM_HAS_FAST_BYTESWAP)
                if ( bitconv.DoReverseBitorder() == false && bitconv.DoReverseByteorder() == false )
                {
                    return BitOrderingConverter( false, true );
                }
#endif
            }
#endif
            // Unsupported transformation, related code-graphs should be deleted by compiler.
            return {};
        }
    }
    template <mutable_type hostNumberType, mutable_type numberType>
    AINLINE numberType VectorTransform( numberType val ) noexcept
    {
        if ( auto conf = VectorTransformGetConfig <hostNumberType, numberType> () )
        {
            return conf.Get().Identity( val );
        }
        // If everything else fails, do this.
        // This code path should never be taken.
        return 0u;
    }

    // This method is required to be done using lambdas because in C++ it is currently ONLY possible to specialize DOWN the code-graph
    // but NOT up (in the direction of the root of) the code-graph.
    template <mutable_type hostNumberType, mutable_type callbackType>
    BM_AINLINE void Select( hostNumberType *vec, size_t num, callbackType&& cb ) noexcept
    {
        typedef typename no_volatile <hostNumberType>::type hostFastNumberType;

        if (num == 0) return;

        // Check for support of FAST vectorized transformations.
        using required_types_for_check = typename sizeof_greaterequal_types <hostNumberType, PlatformNumberTypeList>::type;
        using sorted_types_by_size = typename sizeof_sorted_types <required_types_for_check>::type;
        using reversed_check_types = typename types_reverse <sorted_types_by_size>::type;

        reversed_check_types::ForEach(
            [&] <typename T> () LAINLINE
        {
            if ( sizeof(T) <= sizeof(hostNumberType) * num )
            {
                if ( auto cfg = this->VectorTransformGetConfig <hostFastNumberType, T> () )
                {
                    const T& val = *(const T*)vec;
                    cb( cfg.Get().Identity( val ) );
                    return true;
                }
            }
            return false;
        });
    }

private:
    BitOrderingConverter bitconv;
};

enum class eBitIdentityStorageIdentStrategy
{
    ALIASED,    // uses memory-aliasing to fetch native integrals
    BITFIELD    // uses bitfield-extraction to fetch native integrals
};

inline constexpr eBitIdentityStorageIdentStrategy get_default_bit_identity_storage_ident_strategy( void ) noexcept
{
    endian::eSpecificEndian endianness = endian::eSpecificEndian::DEFAULT_ENDIAN;
#ifdef _PLATFORM_FIXED_ENDIANNESS
    endianness = endian::get_current_endianness();
#endif

    if (endianness == endian::eSpecificEndian::LITTLE_ENDIAN)
        return eBitIdentityStorageIdentStrategy::BITFIELD;
#if !defined(_PLATFORM_HAS_FAST_BYTESWAP)
    return eBitIdentityStorageIdentStrategy::ALIASED;
#else
    return eBitIdentityStorageIdentStrategy::BITFIELD;
#endif
}

// Storage that is identity-optimized on memory request, taking boundary specification into account, allowing flushing back
// of storage into fetched.from location.
// To identity-transform a bit-array is defined as permuting it so that the bits are assorted from LSB to MSB according
// to their indices.
// hostNumberType has to be unsigned integral.
template <
    typename hostNumberType,
    template <typename> typename numberSpecificator = identity_number_specificator,
    typelist_type suppTypes = PlatformNumberTypeList,
    template <typename> typename variantType = BitfieldVariant,
    eBitIdentityStorageIdentStrategy idstrat = get_default_bit_identity_storage_ident_strategy(),
    bool write_only = false
> requires ( write_only == false || mutable_type <hostNumberType> )
struct BitIdentityStorage
{
    BM_AINLINE BitIdentityStorage( void ) = default;
    BM_AINLINE BitIdentityStorage( const BitIdentityStorage& ) = default;

private:
    using amendedSuppTypes = typename eir::typelist_union <suppTypes, eir::typelist <typename base_type <hostNumberType>::type>>::type;

    BM_AINLINE void FlushIdCache( void ) noexcept
        requires ( const_type <hostNumberType> == false )
    {
        this->idcache.Visit(
            [this]( auto& ref ) LAINLINE
            {
                typedef typename plain_type_bitfield <decltype(ref)>::type numType;

                numType data = ref;

                data = this->vecman.template VectorTransform <typename no_volatile <hostNumberType>::type> ( data );

                this->memcache.template SelectRef <numType> (
                    this->idcache_pointer,
                    [&]( auto& cref ) LAINLINE
                    {
                        cref = data;
                    }
                );
            }
        );
        this->idcache.Clear();
    }

    using hostFastNumberType = typename no_volatile <typename no_const <hostNumberType>::type>::type;

public:
    // Modelling assumption conflict: in the optimal BitManager implementation we base all of our memory requests on
    // a fixed host-type that defines that bit-layout for all succeeding memory. Not having a fixed number type across requests
    // does break the bit identity in certain operation orders.
    // => That is why I have decided to introduce the template parameter "hostNumberType".

    template <typename callbackType>
    BM_BRUTAL_AINLINE void Select( hostNumberType& num, size_t bytes_left, callbackType&& cb, bool cache_dirty )
    {
        uintptr_t numloc = (uintptr_t)&num;

        if (this->idcache.IsEmpty() == false)
        {
            // Check whether the request is inside the identity cache.
            uintptr_t cachenumloc = (uintptr_t)this->idcache_pointer;

            auto cache_size = this->idcache.GetSize();

            if (numloc >= cachenumloc && numloc + sizeof(hostFastNumberType) <= cachenumloc + cache_size)
            {
                // Request is still within cache, so return it.
                goto returnResult;
            }

            if constexpr ( const_type <hostNumberType> == false )
            {
                if ( cache_dirty )
                {
                    // Request fell outside of cache, so flush the cache.
                    this->FlushIdCache();
                }
                else
                {
                    this->idcache.Clear();
                }
            }
            else
            {
                // Just throw away const cache contents.
                this->idcache.Clear();
            }
        }

        // Invalidate the memory cache if the request is not inside of it.
        if (this->memcache.IsInitialized() == false || this->memcache.template CanSatisfy <hostNumberType> ( num ) == false)
        {
            if constexpr ( const_type <hostNumberType> == false )
            {
                if ( cache_dirty )
                {
                    this->memcache.Flush();
                }
                else
                {
                    this->memcache.Invalidate();
                }
            }
            else
            {
                this->memcache.Invalidate();
            }
            this->memcache.template CacheMemoryLocation <write_only> ( &num, bytes_left );
        }

        // Put new stuff into cache.
        {
            using required_types_for_check = typename sizeof_greaterequal_types <hostFastNumberType, amendedSuppTypes>::type;
            using sorted_types_by_size = typename sizeof_sorted_types <required_types_for_check>::type;
            using reversed_check_types = typename types_reverse <sorted_types_by_size>::type;

            // Calculate the amount of bits that are possible to read at &num location.
            auto boff = ( numloc - (uintptr_t)this->memcache.GetOriginalDataPointer() );
            auto bytecnt = ( this->memcache.GetCacheSize() - boff );

            typename biggest_type <amendedSuppTypes>::type tmp = 0u;
            uint8_t id_bytecnt = 0u;

            if constexpr ( idstrat == eBitIdentityStorageIdentStrategy::ALIASED )
            {
                // Unused.
                (void)tmp; (void)id_bytecnt;

#ifndef _PLATFORM_SUPPORTS_UNALIGNED_MEMACCESS
                // This aligned read check is only required if we use the memory-semantics-implementation of bit fetching.
                // To support every hardware in existence we check that we can perform an aligned-read onto the memloc.
                uint8_t aligncnt = 0u;
                SelectAlignedTypeAccess <reversed_check_types> ( num, bytecnt,
                    [&] <typename T> () LAINLINE
                {
                    aligncnt = alignof(T);
                });

                if (bytecnt > aligncnt)
                {
                    bytecnt = aligncnt;
                }
#endif //_PLATFORM_SUPPORTS_UNALIGNED_MEMACCESS
            }

            reversed_check_types::ForEach(
                [&] <typename T> () LAINLINE
            {
                if ( sizeof(T) <= bytecnt )
                {
                    if ( auto vcfg = this->vecman.template VectorTransformGetConfig <hostFastNumberType, T> () )
                    {
                        if constexpr ( idstrat == eBitIdentityStorageIdentStrategy::ALIASED )
                        {
                            const auto& val = *(const T*PTR_NO_ALIAS)( (const char*PTR_NO_ALIAS)this->memcache.GetStoragePointer() + boff );
                            this->idcache = vcfg.Get().Identity( val );
                        }
                        else if constexpr ( idstrat == eBitIdentityStorageIdentStrategy::BITFIELD )
                        {
                            T val = this->memcache.template ExtractNumberEndian <T> ( (uint8_t)boff );
                            tmp = vcfg.Get().Identity( val );
                            id_bytecnt = sizeof(T);
                        }
                        return true;
                    }
                }
                return false;
            });
            if constexpr ( idstrat == eBitIdentityStorageIdentStrategy::BITFIELD )
            {
                this->idcache.StoreByBitfield( tmp, id_bytecnt );
            }
            this->idcache_pointer = &num;
        }
    returnResult:
        uint8_t idbyteoff = (uint8_t)( numloc - (uintptr_t)this->idcache_pointer );

        this->idcache.VisitBitsFrom( idbyteoff, castforward <callbackType> ( cb ) );
    }

    BM_BRUTAL_AINLINE hostFastNumberType Read( hostFastNumberType& val, size_t bytes_left, bool cache_dirty ) noexcept
        requires ( write_only == false )
    {
        hostFastNumberType result = 0u;

        this->Select( val, bytes_left,
            [&]( auto& data ) LAINLINE
            {
                result = (hostFastNumberType)data;
            }, cache_dirty
        );

        return result;
    }
    BM_BRUTAL_AINLINE void Write( hostFastNumberType& val, hostFastNumberType write_to, size_t bytes_left, bool cache_dirty ) noexcept
        requires ( const_type <hostNumberType> == false )
    {
        this->Select( val, bytes_left,
            [&]( auto& data ) LAINLINE
            {
                data = write_to;
            }, cache_dirty
        );
    }

    BM_AINLINE void Flush( void ) noexcept
    {
        if constexpr ( const_type <hostNumberType> == false )
        {
            this->FlushIdCache();
            this->memcache.Flush();
        }
        else
        {
            this->Invalidate();
        }
    }
    BM_AINLINE void Invalidate( void ) noexcept
    {
        this->idcache.Clear();
        this->memcache.Invalidate();
    }

    BM_AINLINE bool IsValid( void ) const noexcept
    {
        return ( this->idcache.IsEmpty() == false );
    }

    BM_AINLINE void SetStorageProperty( endian::eSpecificEndian endianness, bool msbfirst ) noexcept
    {
        this->Flush();
        this->vecman = { endianness, msbfirst };
    }

private:
    using size_feasible_supp_types = typename sizeof_greaterequal_types <hostFastNumberType, amendedSuppTypes>::type;

    PlatformBitCache <numberSpecificator, size_feasible_supp_types, variantType> memcache;
    variantType <size_feasible_supp_types> idcache;
    typename decltype(memcache)::distinct_t *idcache_pointer;
    BitVectorizedIdentityManager vecman;
public:
    using supportedTypes = size_feasible_supp_types;
};

template <unsigned_integral unitNumberType, unsigned_integral containNumberType>
    requires ( sizeof(containNumberType) >= sizeof(unitNumberType) )
struct BitRepetitionCache
{
    static constexpr size_t count_contained = sizeof(containNumberType) / sizeof(unitNumberType);

    AINLINE BitRepetitionCache( unitNumberType val ) noexcept
    {
        BitIncrementalAccessor <containNumberType> acc;

        for ( size_t n = 0; n < count_contained; n++ )
        {
            acc.WriteBits( val, sizeof(unitNumberType)*8u );
        }

        this->data = acc.GetData();
    }
    AINLINE BitRepetitionCache( const BitRepetitionCache& ) = default;
    AINLINE BitRepetitionCache( BitRepetitionCache&& ) = default;

    AINLINE BitRepetitionCache& operator = ( const BitRepetitionCache& ) = default;
    AINLINE BitRepetitionCache& operator = ( BitRepetitionCache&& ) = default;

    AINLINE const containNumberType& GetData( void ) const noexcept
    {
        return this->data;
    }

private:
    containNumberType data;
};

#define BITMANAGER_REQUIREMENTS requires ( write_only == false || mutable_type <hostNumberType> )
#define BITMANAGER_TEMPLARGS \
template <typename hostNumberType, \
          template <typename numberType> typename numberSpecificator = choose_default_number_specificator <hostNumberType>::template spec, \
          typelist_type suppTypes = PlatformNumberTypeList, \
          template <typename> typename cacheVariantType = BitfieldVariant, \
          eBitIdentityStorageIdentStrategy idstrat = get_default_bit_identity_storage_ident_strategy(), \
          bool write_only = false \
         > BITMANAGER_REQUIREMENTS
#define BITMANAGER_TEMPLARGS_NODEF \
template <typename hostNumberType, \
          template <typename numberType> typename numberSpecificator, \
          typelist_type suppTypes, \
          template <typename> typename cacheVariantType, \
          eBitIdentityStorageIdentStrategy idstrat, \
          bool write_only \
         > BITMANAGER_REQUIREMENTS
#define BITMANAGER_TEMPLUSE <hostNumberType, numberSpecificator, suppTypes, cacheVariantType, idstrat, write_only>

// Takes a bit array as input. Allows bitwise read and write operations on the input bit array, using the
// optimized classes mentioned previously.
BITMANAGER_TEMPLARGS
struct BitManager
{
    static constexpr bool is_const_bitman = is_const <hostNumberType>::value;

    BM_AINLINE BitManager( hostNumberType *buffer, size_t bufSize, size_t numOffset = 0, platformLocalBitcountType bitOffset = 0 ) noexcept
        : buffer( buffer ), bufSize( bufSize ), iterator( numOffset, bitOffset )
    {
        this->cache.SetStorageProperty( this->storage_endianness, this->storage_msbfirst );
    }
    BM_AINLINE BitManager( const BitManager& ) = default;

    BM_AINLINE ~BitManager( void )
    {
        if constexpr ( const_type <hostNumberType> == false )
        {
            if (dirty_cache)
            {
                this->cache.Flush();
            }
        }
    }

    BM_AINLINE BitManager& operator = ( const BitManager& ) = default;

    BM_AINLINE size_t GetBufferSize( void ) const noexcept { return this->bufSize; }

private:
    typedef BitNumberIteratorForStruct <hostNumberType> iter_t;

    template <typename callbackType>
    BM_AINLINE void SelectByIndex( size_t idx, callbackType&& cb )
    {
        size_t bytes_left = sizeof(hostNumberType) * ( this->bufSize - idx );

        this->cache.Select( this->buffer[ idx ], bytes_left, (callbackType&&)cb, this->dirty_cache );
    }

public:
    // I have decided to implement a true msbfirst parameter instead of reverse-bits because the reversing of entire hostNumberType bits would conflict
    // with the reversing of bit-groups - the so called "byteswap" algorithm - in programming models.
    // Read the algorithm proof inside the methods for further details.

    BM_AINLINE void Reset( void ) noexcept
    {
        this->Flush();
        this->iterator = iter_t();
    }

private:
    template <typename numberType, bitnumberiterator_type srcIterType, bitnumberiterator_type endIterType>
        requires ( unsigned_integral <typename nospecmod_type <numberType>::type> )
    BM_AINLINE void _PutLoopImpl( const numberType& val, srcIterType& srcIter, iter_t& iter, endIterType& end_iter ) noexcept
        requires ( const_type <hostNumberType> == false )
    {
        while ( true )
        {
            size_t numOff = iter.getNumberOffset();
            platformLocalBitcountType bitOff = iter.getLocalBitOffset();

            platformBitCountType avail_bitCnt = ( end_iter - iter ).getTotalBitOffset();

            if ( avail_bitCnt == 0 ) break;

            platformLocalBitcountType src_bitOff = srcIter.getLocalBitOffset();
            platformBitCountType src_bitCount = ( srcIterType::hostBitCount - src_bitOff );

            platformLocalBitcountType bitCnt;
            bool has_data = false;

            this->SelectByIndex( numOff,
                [&]( auto& pnum ) LAINLINE
                {
                    auto numBits = SUB_SIGNP( get_bitcount( pnum ), bitOff );

                    bitCnt = (platformLocalBitcountType)eir::Minimum( numBits, avail_bitCnt, src_bitCount );

                    // TODO: may think about implementing a rolling-replace algorithm to save invocations of the
                    // ROR/ROL instructions, improving performance.

                    replace_bits( pnum, RSHIFT( val, src_bitOff ), bitOff, bitCnt );

                    has_data = true;
                }
            );
            if ( has_data )
            {
                srcIter.addBits( bitCnt );
                iter.addBits( bitCnt );

                this->dirty_cache = true;
            }
        }
        this->iterator = end_iter;
    }
public:
    // Can be used across BitManager instances if their "shared-region" is calculated and passed as end-iter.
    template <typename numberType, bool byteOptimize = true> requires ( unsigned_integral <typename nospecmod_type <numberType>::type> )
    BM_AINLINE void PutRepeatableSharedRemainder( const numberType& val, BitNumberIteratorForStruct <numberType>& srcIter, const BitNumberIteratorForStruct <numberType>& sharedIter_start, const BitNumberIteratorForStruct <numberType>& sharedIter_end ) noexcept
    {
        auto iter = this->iterator;

        decltype(iter) end_iter;

        if constexpr ( byteOptimize )
        {
            // Use this if you know that operations execute on multiples of bytes (8 bit packets).
            end_iter = bit_iterator_advance_min_bytes( iter,
                eir::Tuple( sharedIter_start, sharedIter_end )
            );
        }
        else
        {
            end_iter = bit_iterator_advance_min( iter,
                eir::Tuple( sharedIter_start, sharedIter_end )
            );
        }

        this->_PutLoopImpl( val, srcIter, iter, end_iter );
    }
    template <typename numberType, bool byteOptimize = true> requires ( unsigned_integral <typename nospecmod_type <numberType>::type> )
    BM_AINLINE void PutRepeatable( const numberType& val, BitNumberIteratorForStruct <numberType>& srcIter, const BitNumberIteratorForStruct <numberType>& srcIter_start, const BitNumberIteratorForStruct <numberType>& srcIter_end ) noexcept
    {
        auto iter = this->iterator;

        BitNumberIteratorForStruct <hostNumberType> dst_enditer( this->bufSize, 0 );

        decltype(iter) end_iter;

        if constexpr ( byteOptimize )
        {
            // Use this if you know that operations execute on multiples of bytes (8 bit packets).
            end_iter = bit_iterator_advance_min_bytes( iter,
                eir::Tuple( srcIter_start, srcIter_end ),
                ( dst_enditer - iter ).getTotalByteOffset()
            );
        }
        else
        {
            end_iter = bit_iterator_advance_min( iter,
                eir::Tuple( srcIter_start, srcIter_end ),
                ( dst_enditer - iter ).getTotalBitOffset()
            );
        }

        this->_PutLoopImpl( val, srcIter, iter, end_iter );
    }
    template <typename numberType, bool byteOptimize = true> requires ( unsigned_integral <typename nospecmod_type <numberType>::type> )
    BM_AINLINE void PutSingle( const numberType& val, BitNumberIteratorForStruct <numberType>& srcIter ) noexcept
        requires ( const_type <hostNumberType> == false )
    {
        BitNumberIteratorForStruct <numberType> src_loopiter( 0, srcIter.getLocalBitOffset() );
        BitNumberIteratorForStruct <numberType> src_enditer( 1, 0 );

        this->template PutRepeatable <numberType, byteOptimize> ( val, srcIter, src_loopiter, src_enditer );
    }
    template <typename numberType> requires ( unsigned_integral <typename nospecmod_type <numberType>::type> )
    BM_AINLINE void PutPartialBits( const numberType& val, platformLocalBitcountType putBitcnt, BitNumberIteratorForStruct <numberType>& srcIter ) noexcept
        requires ( const_type <hostNumberType> == false )
    {
        auto iter = this->iterator;

        BitNumberIteratorForStruct <numberType> src_loopiter( 0, srcIter.getLocalBitOffset() );
        BitNumberIteratorForStruct <numberType> src_enditer( 1, 0 );
        BitNumberIteratorForStruct <hostNumberType> dst_enditer( this->bufSize, 0 );

        auto end_iter = bit_iterator_advance_min( iter,
            ( src_enditer - src_loopiter ).getTotalBitOffset(),
            ( dst_enditer - iter ).getTotalBitOffset(),
            putBitcnt
        );

        this->_PutLoopImpl( val, srcIter, iter, end_iter );
    }
    template <typename numberType> requires ( unsigned_integral <typename nospecmod_type <numberType>::type> )
    BM_AINLINE void PutPartialBytes( const numberType& val, uint8_t putBytecnt, BitNumberIteratorForStruct <numberType>& srcIter ) noexcept
        requires ( const_type <hostNumberType> == false )
    {
        auto iter = this->iterator;

        BitNumberIteratorForStruct <numberType> src_loopiter( 0, srcIter.getLocalBitOffset() );
        BitNumberIteratorForStruct <numberType> src_enditer( 1, 0 );
        BitNumberIteratorForStruct <hostNumberType> dst_enditer( this->bufSize, 0 );

        auto end_iter = bit_iterator_advance_min_bytes( iter,
            ( src_enditer - src_loopiter ).getTotalByteOffset(),
            ( dst_enditer - iter ).getTotalByteOffset(),
            putBytecnt
        );

        this->_PutLoopImpl( val, srcIter, iter, end_iter );
    }
    template <typename numberType> requires ( unsigned_integral <typename nospecmod_type <numberType>::type> )
    BM_AINLINE void PutSingle( const numberType& val, BitNumberIteratorForStruct <numberType>& srcIter, endian::eSpecificEndian target_endianness, bool msbfirst = false ) noexcept
        requires ( const_type <hostNumberType> == false )
    {
        endian::eSpecificEndian prev_storage_endianness = this->storage_endianness;
        bool prev_storage_msbfirst = this->storage_msbfirst;

        this->SetDefaultStorageProperty( target_endianness, msbfirst );

        this->PutSingle( val, srcIter );

        this->storage_endianness = prev_storage_endianness;
        this->storage_msbfirst = prev_storage_msbfirst;
    }
    // Only use this function if you know that the put request will complete 100%.
    // Otherwise you should use PutEx with the iterator, proceeding until the iterator is at it's end.
    template <typename numberType> requires ( unsigned_integral <typename nospecmod_type <numberType>::type> )
    BM_AINLINE void Put( const numberType& val, endian::eSpecificEndian target_endianness = endian::eSpecificEndian::DEFAULT_ENDIAN, bool msbfirst = false ) noexcept
        requires ( const_type <hostNumberType> == false )
    {
        BitNumberIteratorForStruct <numberType> srcIter;

        this->PutSingle( val, srcIter, target_endianness, msbfirst );
    }
private:
    template <typename numberType, bitnumberiterator_type dstIterType, bitnumberiterator_type endIterType>
        requires ( write_only == false && unsigned_integral <typename nospecmod_type <numberType>::type> )
    BM_AINLINE void _FetchLoopImpl( numberType& result, dstIterType& dstIter, iter_t& iter, endIterType& end_iter ) noexcept
    {
        while ( true )
        {
            size_t numOff = iter.getNumberOffset();
            platformLocalBitcountType bitOff = iter.getLocalBitOffset();

            platformBitCountType avail_bitCnt = ( end_iter - iter ).getTotalBitOffset();

            if ( avail_bitCnt == 0 ) break;

            platformLocalBitcountType bitCnt;
            bool has_data = false;

            this->SelectByIndex( numOff,
                [&]( auto& pnum ) LAINLINE
                {
                    auto numBits = SUB_SIGNP( get_bitcount( pnum ), bitOff );

                    bitCnt = (platformLocalBitcountType)eir::Minimum( numBits, avail_bitCnt );

                    auto wbits = extract_bits( pnum, bitOff, bitCnt );

                    result |= LSHIFT( wbits, dstIter.getLocalBitOffset() );

                    has_data = true;
                }
            );
            if ( has_data )
            {
                dstIter.addBits( bitCnt );
                iter.addBits( bitCnt );
            }
        }
        this->iterator = end_iter;
    }
public:
    // Can be used across BitManager instances if their "shared-region" is calculated and passed as end-iter.
    template <typename numberType, bool byteOptimize = true, bitnumberiterator_type sharedEndIterType>
        requires ( write_only == false && unsigned_integral <typename nospecmod_type <numberType>::type> )
    BM_AINLINE void FetchSingleSharedRemainder( numberType& result, BitNumberIteratorForStruct <numberType>& dstIter, const sharedEndIterType& sharedEndIter ) noexcept
    {
        auto iter = this->iterator;

        BitNumberIteratorForStruct <numberType> dst_loopiter( 0, dstIter.getLocalBitOffset() );
        BitNumberIteratorForStruct <numberType> dst_enditer( 1, 0 );

        decltype(iter) end_iter;

        if constexpr ( byteOptimize )
        {
            // Use this if you know that operations execute on multiples of bytes (8 bit packets).
            end_iter = bit_iterator_advance_min_bytes( iter,
                ( dst_enditer - dst_loopiter ).getTotalByteOffset(),
                eir::Tuple( dstIter, sharedEndIter )
            );
        }
        else
        {
            end_iter = bit_iterator_advance_min( iter,
                ( dst_enditer - dst_loopiter ).getTotalBitOffset(),
                eir::Tuple( dstIter, sharedEndIter )
            );
        }

        this->_FetchLoopImpl( result, dstIter, iter, end_iter );
    }
    template <typename numberType, bool byteOptimize = true>
        requires ( write_only == false && unsigned_integral <typename nospecmod_type <numberType>::type> )
    BM_AINLINE void FetchSingle( numberType& result, BitNumberIteratorForStruct <numberType>& dstIter ) noexcept
    {
        auto iter = this->iterator;

        BitNumberIteratorForStruct <numberType> dst_loopiter( 0, dstIter.getLocalBitOffset() );
        BitNumberIteratorForStruct <numberType> dst_enditer( 1, 0 );
        BitNumberIteratorForStruct <hostNumberType> src_enditer( this->bufSize, 0 );

        decltype(iter) end_iter;

        if constexpr ( byteOptimize )
        {
            // Use this if you know that operations execute on multiples of bytes (8 bit packets).
            end_iter = bit_iterator_advance_min_bytes( iter,
                ( dst_enditer - dst_loopiter ).getTotalByteOffset(),
                ( src_enditer - iter ).getTotalByteOffset()
            );
        }
        else
        {
            end_iter = bit_iterator_advance_min( iter,
                ( dst_enditer - dst_loopiter ).getTotalBitOffset(),
                ( src_enditer - iter ).getTotalBitOffset()
            );
        }

        this->_FetchLoopImpl( result, dstIter, iter, end_iter );
    }
    template <typename numberType>
        requires ( write_only == false && unsigned_integral <typename nospecmod_type <numberType>::type> )
    BM_AINLINE void FetchPartialBits( numberType& result, platformLocalBitcountType fetchBitCnt, BitNumberIteratorForStruct <numberType>& dstIter ) noexcept
    {
        auto iter = this->iterator;

        BitNumberIteratorForStruct <numberType> dst_loopiter( 0, dstIter.getLocalBitOffset() );
        BitNumberIteratorForStruct <numberType> dst_enditer( 1, 0 );
        BitNumberIteratorForStruct <hostNumberType> src_enditer( this->bufSize, 0 );

        auto end_iter = bit_iterator_advance_min( iter,
            ( dst_enditer - dst_loopiter ).getTotalBitOffset(),
            ( src_enditer - iter ).getTotalBitOffset(),
            fetchBitCnt
        );

        this->_FetchLoopImpl( result, dstIter, iter, end_iter );
    }
    template <typename numberType>
        requires ( write_only == false && unsigned_integral <typename nospecmod_type <numberType>::type> )
    BM_AINLINE void FetchPartialBytes( numberType& result, uint8_t fetchByteCnt, BitNumberIteratorForStruct <numberType>& dstIter ) noexcept
    {
        auto iter = this->iterator;

        BitNumberIteratorForStruct <numberType> dst_loopiter( 0, dstIter.getLocalBitOffset() );
        BitNumberIteratorForStruct <numberType> dst_enditer( 1, 0 );
        BitNumberIteratorForStruct <hostNumberType> src_enditer( this->bufSize, 0 );

        auto end_iter = bit_iterator_advance_min_bytes( iter,
            ( dst_enditer - dst_loopiter ).getTotalByteOffset(),
            ( src_enditer - iter ).getTotalByteOffset(),
            fetchByteCnt
        );

        this->_FetchLoopImpl( result, dstIter, iter, end_iter );
    }
    template <typename numberType>
        requires ( write_only == false && unsigned_integral <typename nospecmod_type <numberType>::type> )
    BM_AINLINE void FetchSingle( numberType& result, BitNumberIteratorForStruct <numberType>& dstIter, endian::eSpecificEndian target_endianness, bool msbfirst = false ) noexcept
    {
        endian::eSpecificEndian prev_storage_endianness = this->storage_endianness;
        bool prev_storage_msbfirst = this->storage_msbfirst;

        this->SetDefaultStorageProperty( target_endianness, msbfirst );

        // Cannot throw exceptions so we are safe.
        this->FetchSingle( result, dstIter );

        this->storage_endianness = prev_storage_endianness;
        this->storage_msbfirst = prev_storage_msbfirst;
    }
    // Only use this function if you know that the fetch request will complete 100%.
    // Otherwise you should use FetchEx with the iterator, proceeding until the iterator is at it's end.
    template <typename numberType>
        requires ( write_only == false && unsigned_integral <typename nospecmod_type <numberType>::type> )
    BM_AINLINE numberType Fetch( endian::eSpecificEndian target_endianness = endian::eSpecificEndian::DEFAULT_ENDIAN, bool msbfirst = false ) noexcept
    {
        BitNumberIteratorForStruct <numberType> dstIter;

        typename no_volatile <numberType>::type result = 0;
        this->FetchSingle( result, dstIter, target_endianness, msbfirst );

        return result;
    }
    inline void Flush( void ) noexcept
    {
        if constexpr ( const_type <hostNumberType> == false )
        {
            if (this->dirty_cache)
            {
                this->cache.Flush();

                this->dirty_cache = false;
            }
            else
            {
                this->cache.Invalidate();
            }
        }
        else
        {
            this->cache.Invalidate();
        }
    }

    BM_AINLINE bool IsAtEnd( void ) const noexcept
    {
        return ( this->iterator.getNumberOffset() == this->bufSize );
    }

    BM_AINLINE void SetDefaultStorageProperty( endian::eSpecificEndian endianness, bool msbfirst ) noexcept
    {
        if (this->storage_endianness == endianness && this->storage_msbfirst == msbfirst) return;

        this->cache.SetStorageProperty( endianness, msbfirst );
        this->storage_endianness = endianness;
        this->storage_msbfirst = msbfirst;
    }

    BM_AINLINE void SetIterator( iter_t iter ) noexcept
    {
        this->iterator = castmove( iter );
    }
    BM_AINLINE iter_t GetIterator( void ) const noexcept
    {
        return this->iterator;
    }

    BM_AINLINE iter_t GetRemainderIterator( void ) const noexcept
    {
        return ( iter_t( this->bufSize, 0 ) - this->iterator );
    }

private:
    hostNumberType *buffer;
    size_t bufSize;
    iter_t iterator;

    // Endian properties of storage.
    endian::eSpecificEndian storage_endianness = endian::eSpecificEndian::DEFAULT_ENDIAN;
    bool storage_msbfirst = false;

    BitIdentityStorage <hostNumberType, numberSpecificator, suppTypes, cacheVariantType, idstrat, write_only> cache;
    bool dirty_cache = false;
public:
    using supportedTypes = typename decltype(BitManager::cache)::supportedTypes;
};
template <typename>
struct is_bitmanager : false_type {};
BITMANAGER_TEMPLARGS_NODEF
struct is_bitmanager <BitManager BITMANAGER_TEMPLUSE> : true_type {};
template <typename T> requires ( is_bitmanager <typename base_type <T>::type>::value )
struct is_bitmanager <T> : true_type {};

template <typename T>
concept bitmanager_type = is_bitmanager <T>::value;

// Namespace that contains useful code-graph templates for use with BitManager instances.
namespace BitManagerTemplates
{

// Performs an one way transfer operation, fetching data from src_bitman and pushing it into dst_bitman.
// Use the commit operation to perform the hardware send request.
template <typename commitCallbackType, bitmanager_type srcBitManType, bitmanager_type dstBitManType>
    requires ( eir::typelist_shared <typename srcBitManType::supportedTypes, typename dstBitManType::supportedTypes>::type::count > 0 && dstBitManType::is_const_bitman == false &&
               requires ( commitCallbackType cb ) { cb(); } )
BM_AINLINE void Send( srcBitManType& src_bitman, dstBitManType& dst_bitman, commitCallbackType&& cb )
    noexcept(requires(commitCallbackType cb) { { cb() } noexcept; })
{
    using txitem_t = typename eir::biggest_type <typename eir::typelist_shared <typename srcBitManType::supportedTypes, typename dstBitManType::supportedTypes>::type>::type;

    while ( src_bitman.IsAtEnd() == false )
    {
        while ( dst_bitman.IsAtEnd() == false && src_bitman.IsAtEnd() == false )
        {
            BitNumberIteratorForStruct <txitem_t> iter;
            txitem_t raw = 0u;
            src_bitman.FetchSingle( raw, iter );
            BitNumberIteratorForStruct <txitem_t> dst_iter;
            dst_bitman.PutPartialBytes( raw, (uint8_t)iter.getTotalByteOffset(), dst_iter );
        }
        dst_bitman.Flush();
        // Commit.
        cb();
        dst_bitman.SetIterator({});
    }
}

template <mutable_type dstNumberType, typename commitCallbackType, bitmanager_type srcBitManType>
    requires requires ( commitCallbackType cb, const BitNumberIteratorForStruct <typename base_type <dstNumberType>::type> iter ) { cb( iter ); }
BM_AINLINE void SendFixed( dstNumberType *txbuf, size_t txbuf_cnt, srcBitManType& src_bitMan, commitCallbackType&& cb )
    noexcept(requires ( commitCallbackType cb, const BitNumberIteratorForStruct <typename base_type <dstNumberType>::type> iter ) { { cb( iter ) } noexcept; })
{
    while ( src_bitMan.IsAtEnd() == false )
    {
        BitNumberIteratorForStruct <typename base_type <dstNumberType>::type> dst_iter;
        while ( src_bitMan.IsAtEnd() == false && dst_iter.getNumberOffset() < txbuf_cnt )
        {
            typename base_type <dstNumberType>::type tmp = 0u;
            size_t n = dst_iter.getNumberOffset();
            src_bitMan.FetchSingle( tmp, dst_iter );
            txbuf[n] = tmp;
        }
        // Commit.
        cb( make_const_ref( dst_iter ) );
    }
}

// Similar to the Send template but fills the buffer using repeated data.
template <typename putNumberType, unsigned_integral countNumberType, bitmanager_type bitmanType, typename commitCallbackType>
    requires ( bitmanType::is_const_bitman == false &&
               requires ( commitCallbackType cb ) { cb(); } )
BM_AINLINE void RepeatSendCountTo( const putNumberType& val, const countNumberType& count_to, bitmanType& dst_bitman, commitCallbackType&& cb )
    noexcept(requires ( commitCallbackType cb ) { { cb() } noexcept; })
{
    using bigtx_t = typename biggest_type <typename bitmanType::supportedTypes>::type;

    BitRepetitionCache <putNumberType, bigtx_t> repcache( val );

    BitNumberIteratorForStruct <bigtx_t> val_iter( 0, 0 );
    BitNumberIteratorForStruct <bigtx_t> val_enditer( count_to / decltype(repcache)::count_contained, 0 );

    while ( val_iter != val_enditer )
    {
        dst_bitman.PutRepeatable( repcache.GetData(), val_iter, val_iter, val_enditer );
        dst_bitman.Flush();
        // Commit.
        cb();
        dst_bitman.SetIterator({});
    }
}

// Performs an one-way receive operation, using a "kickoff"/fill callback.
// Inside the fill callback you can also send data, but this template is mainly designed for reception.
template <bitmanager_type recvBitManType, bitmanager_type txbufBitManType, typename kickoffCallbackType>
    requires ( recvBitManType::is_const_bitman == false )
BM_AINLINE void Receive( recvBitManType& recv_bitman, txbufBitManType& txbuf_bitman, kickoffCallbackType&& kickoffCB )
{
    using txitem_t = typename eir::biggest_type <typename eir::typelist_shared <typename recvBitManType::supportedTypes, typename txbufBitManType::supportedTypes>::type>::type;

    while ( recv_bitman.IsAtEnd() == false )
    {
        // Perform the hardware operation that fills the txbuf_bitman.
        auto sharedEndIter = Minimum( recv_bitman.GetRemainderIterator(), txbuf_bitman.GetRemainderIterator() );
        kickoffCB( make_const_ref( sharedEndIter ) );
        // Fetch the data from txbuf_bitman into recv_bitman.
        BitNumberIteratorForStruct <txitem_t> dst_iter;
        BitNumberIteratorForStruct <txitem_t> iter;
        while ( dst_iter != sharedEndIter )
        {
            txitem_t raw = 0u;
            txbuf_bitman.FetchSingleSharedRemainder( raw, iter, sharedEndIter );
            recv_bitman.PutRepeatableSharedRemainder( raw, dst_iter, dst_iter, iter );
        }
        txbuf_bitman.Reset();
    }
}

} // namespace BitManagerTemplates

} // namespace eir

#endif //_BIT_MANAGE_HEADER_
