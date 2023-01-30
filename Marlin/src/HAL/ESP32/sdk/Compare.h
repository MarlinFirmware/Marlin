/*****************************************************************************
*
*  PROJECT:     Eir SDK
*  FILE:        eirrepo/sdk/Compare.h
*  PURPOSE:     Comparison handling helpers.
*
*  Find the Eir SDK at: https://osdn.net/projects/eirrepo/
*
*****************************************************************************/

#ifndef _EIRSDK_COMPARE_HEADER_
#define _EIRSDK_COMPARE_HEADER_

#include "MacroUtils.h"
#include "MetaHelpers.h"

#if __has_include(<compare>)
#include <compare>
#endif

namespace eir
{

// Comparison results.
enum class eCompResult
{
    LEFT_LESS,
    EQUAL,
    LEFT_GREATER
};

#if defined(__cpp_impl_three_way_comparison)
template <typename orderingTypeName = std::weak_ordering>
static AINLINE orderingTypeName cmpres_to_ordering( eCompResult res )
{
    switch( res )
    {
    case eCompResult::LEFT_LESS:
        return orderingTypeName::less;
    case eCompResult::LEFT_GREATER:
        return orderingTypeName::greater;
    default: break;
    }

    return orderingTypeName::equivalent;
}
#endif

AINLINE eCompResult flip_comp_result( eCompResult res ) noexcept
{
    if ( res == eCompResult::LEFT_LESS )
    {
        return eCompResult::LEFT_GREATER;
    }
    else if ( res == eCompResult::LEFT_GREATER )
    {
        return eCompResult::LEFT_LESS;
    }

    return eCompResult::EQUAL;
}

#define EIR_VALCMP( left, right ) \
    { \
        auto cmpRes = eir::DefaultValueCompare( left, right ); \
        if ( cmpRes != eir::eCompResult::EQUAL ) \
        { \
            return cmpRes; \
        } \
    }
#define EIR_VALCMP_LT( _cmpVal ) \
    { \
        eir::eCompResult cmpVal = _cmpVal; \
        if ( cmpVal != eir::eCompResult::EQUAL ) \
        { \
            return ( cmpVal == eir::eCompResult::LEFT_LESS ); \
        } \
    }

template <typename A, typename B = A>
concept less_than_comparable = requires ( A a, B b ) { { a < b } -> same_as <bool>; };
template <typename A, typename B = A>
concept nothrow_less_than_comparable = requires ( A a, B b ) { { a < b } noexcept -> same_as <bool>; };

template <typename A, typename B = A>
concept greater_than_comparable = requires ( A a, B b ) { { a > b } -> same_as <bool>; };
template <typename A, typename B = A>
concept nothrow_greater_than_comparable = requires ( A a, B b ) { { a > b } noexcept -> same_as <bool>; };

template <typename A, typename B = A>
concept equality_comparable = requires ( A a, B b ) { { a == b } -> same_as <bool>; };
template <typename A, typename B = A>
concept nothrow_equality_comparable = requires ( A a, B b ) { { a == b } noexcept -> same_as <bool>; };

template <typename LT, typename RT = LT>
concept three_way_comparable =
#if defined(__cpp_impl_three_way_comparison)
    requires( LT l, RT r ) { l <=> r; };
#else
    less_than_comparable <LT, RT> && equality_comparable <LT, RT> && greater_than_comparable <LT, RT>;
#endif
template <typename LT, typename RT = LT>
concept nothrow_three_way_comparable =
#if defined(__cpp_impl_three_way_comparison)
    requires( LT l, RT r ) { { l <=> r } noexcept; };
#else
    nothrow_less_than_comparable <LT, RT> && nothrow_equality_comparable <LT, RT> && nothrow_greater_than_comparable <LT, RT>;
#endif

// Basic function.
template <typename leftType, typename rightType> requires ( three_way_comparable <leftType, rightType> )
AINLINE eCompResult DefaultValueCompare( const leftType& left, const rightType& right ) noexcept(nothrow_three_way_comparable <leftType, rightType>)
{
#if defined(__cpp_impl_three_way_comparison)
    auto wo = ( left <=> right );

    if ( std::is_lt( wo ) )
    {
        return eCompResult::LEFT_LESS;
    }
    else if ( std::is_gt( wo ) )
    {
        return eCompResult::LEFT_GREATER;
    }
#else
    if ( left < right )
    {
        return eCompResult::LEFT_LESS;
    }
    else if ( left > right )
    {
        return eCompResult::LEFT_GREATER;
    }
#endif

    // TODO: think about the case of comparing floats of the "partial ordering" scenario (NaN, etc).

    return eCompResult::EQUAL;
}

namespace CompareUtils
{

template <typename... Ts>
struct select_minimum_target_type : types_find_conversion_target <typelist <Ts...>> {};
template <integral_type... Ts>
struct select_minimum_target_type <Ts...> : smallest_type <Ts...> {};

} // namespace CompareUtils

template <typename...>
struct types_less_than_comparable : true_type {};
template <typename... Ts>
struct types_less_than_comparable <typelist <Ts...>> : types_less_than_comparable <Ts...> {};
template <typename T, typename... Ts>
struct types_less_than_comparable <T, Ts...>
    : conditional <less_than_comparable <T> && (less_than_comparable <T, Ts> && ...),
        types_less_than_comparable <Ts...>,
        false_type
    >::type {};

template <typename...>
struct types_nothrow_less_than_comparable : true_type {};
template <typename... Ts>
struct types_nothrow_less_than_comparable <typelist <Ts...>> : types_nothrow_less_than_comparable <Ts...> {};
template <typename T, typename... Ts>
struct types_nothrow_less_than_comparable <T, Ts...>
    : conditional <nothrow_less_than_comparable <T> && (nothrow_less_than_comparable <T, Ts> && ...),
        types_nothrow_less_than_comparable <Ts...>,
        false_type
    >::type {};

template <typename... numberTypes>
    requires ( sizeof...(numberTypes) > 0 && types_find_conversion_target <typelist <typename base_type <numberTypes>::type...>>::value && types_less_than_comparable <typename base_type <numberTypes>::type...>::value )
AINLINE auto Minimum( numberTypes&&... values )
    noexcept(types_nothrow_less_than_comparable <typename base_type <numberTypes>::type...>::value)
{
    typename types_find_conversion_target <typelist <typename base_type <numberTypes>::type...>>::type result =
        select_from <0> ( castforward <numberTypes> ( values )... );
        
    auto metalamb = [&] <size_t... IDX> ( index_sequence <IDX...> ) LAINLINE
    {
        auto lamb = [&] <size_t N, typename T> ( T&& val ) LAINLINE
        {
            if constexpr ( N > 0 )
            {
                if ( val < result )
                {
                    result = castforward <T> ( val );
                }
            }
        };
        ( lamb.template operator() <IDX> ( castforward <numberTypes> ( values ) ), ... );
    };
    metalamb( index_sequence_until <sizeof...(values)> () );

    return (typename CompareUtils::select_minimum_target_type <typename base_type <numberTypes>::type...>::type)result;
}

template <typename... numberTypes>
    requires ( sizeof...(numberTypes) > 0 && types_find_conversion_target <typelist <typename base_type <numberTypes>::type...>>::value && types_less_than_comparable <typename base_type <numberTypes>::type...>::value )
AINLINE auto Maximum( numberTypes&&... values )
    noexcept(types_nothrow_less_than_comparable <typename base_type <numberTypes>::type...>::value)
{
    typename types_find_conversion_target <typelist <typename base_type <numberTypes>::type...>>::type result =
        select_from <0> ( castforward <numberTypes> ( values )... );

    auto metalamb = [&] <size_t... IDX> ( index_sequence <IDX...> ) LAINLINE
    {
        auto lamb = [&] <size_t N, typename T> ( T&& val ) LAINLINE
        {
            if constexpr ( N > 0 )
            {
                if ( result < val )
                {
                    result = castforward <T> ( val );
                }
            }
        };
        ( lamb.template operator() <IDX> ( castforward <numberTypes> ( values ) ), ... );
    };
    metalamb( index_sequence_until <sizeof...(values)> () );

    return result;
}

} // namespace eir

#endif //_EIRSDK_COMPARE_HEADER_