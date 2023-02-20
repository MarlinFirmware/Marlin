/*****************************************************************************
*
*  PROJECT:     Eir SDK
*  FILE:        eirrepo/sdk/Tuple.h
*  PURPOSE:     Parameterized specialiazed list concept implementation
*
*  Find the Eir SDK at: https://osdn.net/projects/eirrepo/
*
*****************************************************************************/

#ifndef _EIRSDK_TUPLE_HEADER_
#define _EIRSDK_TUPLE_HEADER_

#include "MacroUtils.h"
#include "MetaHelpers.h"

namespace eir
{

template <typename... types>
struct Tuple
{
private:
    template <typename...>
    struct tuple_storage {};
    template <typename T>
    struct tuple_storage <T>
    {
        template <typename c_T> requires ( constructible_from <T, c_T> )
        AINLINE tuple_storage( c_T&& value ) noexcept(nothrow_constructible_from <T, c_T>)
            : value( castforward <c_T> ( value ) )
        {}
        AINLINE tuple_storage( const tuple_storage& ) = default;
        AINLINE tuple_storage( tuple_storage&& ) = default;

        T value;
    };
    template <typename T, typename... Ts>
    struct tuple_storage <T, Ts...>
    {
        template <typename c_T, typename... c_Ts> requires ( constructible_from <T, c_T> && constructible_from <tuple_storage <Ts...>, c_Ts...> )
        AINLINE tuple_storage( c_T&& value, c_Ts&&... others ) noexcept(nothrow_constructible_from <T, c_T> && nothrow_constructible_from <tuple_storage <Ts...>, c_Ts...>)
            : value( castforward <c_T> ( value ) ), next( castforward <c_Ts> ( others )... )
        {}
        AINLINE tuple_storage( const tuple_storage& ) = default;
        AINLINE tuple_storage( tuple_storage&& ) = default;

        T value;
        tuple_storage <Ts...> next;
    };

    tuple_storage <types...> data;

public:
    static constexpr size_t count = sizeof...(types);
    using type_list = typelist <types...>;

    AINLINE Tuple( types... args ) noexcept(nothrow_constructible_from <tuple_storage <types...>, types...>)
        : data( castmove( args )... )
    {}
    template <typename... c_types> requires ( constructible_from <tuple_storage <types...>, c_types...> )
    AINLINE Tuple( c_types&&... args ) noexcept(nothrow_constructible_from <tuple_storage <types...>, c_types...>)
        : data( castforward <c_types> ( args )... )
    {}
    AINLINE Tuple( const Tuple& ) = default;
    AINLINE Tuple( Tuple&& ) = default;

    AINLINE Tuple& operator = ( const Tuple& ) = default;
    AINLINE Tuple& operator = ( Tuple&& ) = default;

    template <size_t N> requires ( N < sizeof...(types) )
    AINLINE auto& Get( void ) noexcept
    {
        auto lamb = [] <size_t idx> ( auto& lamb, auto& data ) LAINLINE -> auto&
        {
            if constexpr ( idx == 0 )
            {
                return data.value;
            }
            else
            {
                return lamb.template operator() <idx-1> ( lamb, data.next );
            }
        };

        return lamb.template operator() <N> ( lamb, this->data );
    }

    template <size_t N> requires ( N < sizeof...(types) )
    AINLINE const auto& Get( void ) const noexcept
    {
        auto lamb = [] <size_t idx> ( auto& lamb, auto& data ) LAINLINE -> const auto&
        {
            if constexpr ( idx == 0 )
            {
                return data.value;
            }
            else
            {
                return lamb.template operator() <idx-1> ( lamb, data.next );
            }
        };

        return lamb.template operator() <N> ( lamb, this->data );
    }
};
template <typename>
struct is_tuple : false_type {};
template <typename... Ts>
struct is_tuple <Tuple <Ts...>> : true_type {};
template <typename T> requires ( is_tuple <typename base_type <T>::type>::value )
struct is_tuple <T> : true_type {};

template <typename T>
concept tuple_type = is_tuple <T>::value;

template <typename T, typename... Ts> requires ( constructible_from <T, Ts...> )
AINLINE T construct_from_tuple( const Tuple <Ts...>& tuple ) noexcept(nothrow_constructible_from <T, Ts...>)
{
    auto lamb = [&] <size_t... IDX> ( index_sequence <IDX...> ) LAINLINE
    {
        return T( tuple.template Get <IDX> ()... );
    };

    return lamb( index_sequence_until <sizeof...(Ts)> () );
}

template <typename T, typename... Ts> requires ( constructible_from <T, Ts...> )
AINLINE T construct_from_tuple( Tuple <Ts...>&& tuple ) noexcept(nothrow_constructible_from <T, Ts...>)
{
    auto lamb = [&] <size_t... IDX> ( index_sequence <IDX...> ) LAINLINE
    {
        return T( castmove( tuple.template Get <IDX> () )... );
    };

    return lamb( index_sequence_until <sizeof...(Ts)> () );
}

} // namespace eir

#endif //_EIRSDK_TUPLE_HEADER_
