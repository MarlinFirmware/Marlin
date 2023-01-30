/*****************************************************************************
*
*  PROJECT:     Eir SDK
*  FILE:        eirrepo/sdk/MetaHelpers.h
*  PURPOSE:     Memory management templates and other utils.
*
*  Find the Eir SDK at: https://osdn.net/projects/eirrepo/
*
*****************************************************************************/

#ifndef _COMMON_META_PROGRAMMING_HELPERS_
#define _COMMON_META_PROGRAMMING_HELPERS_

#if __has_include(<type_traits>)
#include <type_traits>
#endif

#if __has_include(<limits>)
#include <limits>
#endif

#include "MacroUtils.h"

#if __has_include(<new>)
#include <new>
#endif
#include <stdint.h>
#include <stdlib.h>

namespace eir
{

template <size_t...>
struct index_sequence {};

template <size_t...>
struct index_sequence_until {};
template <size_t... IDX>
struct index_sequence_until <0, IDX...> : index_sequence <IDX...> {};
template <size_t N, size_t... IDX>
struct index_sequence_until <N, IDX...> : index_sequence_until <N-1, N-1, IDX...> {};


template <typename T>
struct type_identity
{
    typedef T type;
};

struct true_type
{
    static const constexpr bool value = true;
};
struct false_type
{
    static const constexpr bool value = false;
};

template <typename T1, typename T2>
struct is_same_as : public false_type {};
template <typename T1>
struct is_same_as <T1, T1> : public true_type {};

template <typename T1, typename T2>
concept same_as = is_same_as <T1, T2>::value;

template <typename... Ts>
struct typelist
{
    static constexpr auto count = sizeof...(Ts);

    template <typename callbackType>
    AINLINE static constexpr bool ForEach( callbackType&& cb )
    {
        bool success = false;

        if constexpr ( sizeof... (Ts) > 0 )
        {
            auto lamb = [&] <typename T> () LAINLINE
            {
                if ( success == false )
                {
                    if ( cb.template operator() <T> () ) success = true;
                }
            };

            ( lamb.template operator() <Ts> (), ... );
        }

        return success;
    }
};
template <typename>
struct is_typelist : false_type {};
template <typename... Ts>
struct is_typelist <typelist <Ts...>> : true_type {};
// do not allow specialization-cv, references, pointers-to, etc..

template <typename T>
concept typelist_type = is_typelist <T>::value;

template <typename TC, typename... Ts>
struct sequence_contains : public false_type {};
template <typename TC, typename... Ts>
struct sequence_contains <TC, typelist <Ts...>> : public sequence_contains <TC, Ts...> {};
template <typename TC, typename... Ts>
struct sequence_contains <TC, TC, Ts...> : public true_type {};
template <typename TC, typename T2, typename... Ts>
struct sequence_contains <TC, T2, Ts...> : public sequence_contains <TC, Ts...> {};

template <size_t N, typename... Ts>
struct sequence_get {};
template <size_t N, typename... Ts>
struct sequence_get <N, typelist <Ts...>> : public sequence_get <N, Ts...> {};
template <typename... Ts>
struct sequence_get <0, typelist <Ts...>> : public sequence_get <0, Ts...> {};
template <typename T, typename... Ts>
struct sequence_get <0, T, Ts...>
{
    typedef T type;
};
template <size_t N, typename T, typename... Ts>
struct sequence_get <N, T, Ts...> : public sequence_get <N-1, Ts...> {};

template <typename T>
struct remove_reference
{
    typedef T type;
};
template <typename T>
struct remove_reference <T&> : public remove_reference <T> {};
template <typename T>
struct remove_reference <T&&> : public remove_reference <T> {};

template <typename T>
AINLINE constexpr typename remove_reference <T>::type&& castmove( T&& v ) noexcept { return (typename remove_reference <T>::type&&)v; }

template <typename>
struct is_any_ref : false_type {};
template <typename T>
struct is_any_ref <T&> : true_type {};
template <typename T>
struct is_any_ref <T&&> : true_type {};

// You may be wondering why I called this "castforward" instead of the equivalent "forward".
// For once I wanted to combat the malicious "using namespace std;" statement.
// On the other hand, the MSVC compiler suffers from some ambiguity look-up issue regarding "forward" templated-specifier by itself,
// causing a misdetection of "std::forward" as possible candidate, causing compilation failure.
// So to be on the safe side avoid name-collision with std-namespace things!
template <typename T>
AINLINE constexpr T&& castforward( typename remove_reference <T>::type& v ) noexcept { return static_cast <T&&> ( v ); }
template <typename T>
AINLINE constexpr T&& castforward( typename remove_reference <T>::type&& v ) noexcept { return static_cast <T&&> ( v ); }

template <typename To, typename From>
concept nothrow_move_assignable_with = requires ( To& a, From b ) { { a = castmove(b) } noexcept; };

template <typename To, typename From>
concept copy_assignable_with = requires ( To& a, const From b ) { a = b; };

template <typename To, typename From>
concept nothrow_copy_assignable_with = requires ( To& a, const From b ) { { a = b } noexcept; };

template <typename T>
struct no_volatile : type_identity <T> {};
template <typename T>
struct no_volatile <volatile T> : public no_volatile <T> {};
template <typename... Ts>
struct no_volatile <typelist <Ts...>>
    : public type_identity <typelist <typename no_volatile <Ts>::type...>> {};

template <typename>
struct is_volatile : false_type {};
template <typename T>
struct is_volatile <volatile T> : true_type {};
template <typename T>
struct is_volatile <volatile T&> : true_type {};
template <typename T>
struct is_volatile <volatile T&&> : true_type {};

template <typename T>
concept volatile_type = is_volatile <T>::value;

template <typename T>
struct no_const
{
    typedef T type;
};
template <typename T>
struct no_const <const T> : public no_const <T> {};
template <typename... Ts>
struct no_const <typelist <Ts...>>
    : public type_identity <typelist <typename no_const <Ts>::type...>> {};

template <typename>
struct is_const : false_type {};
template <typename T>
struct is_const <const T> : true_type {};
template <typename T>
struct is_const <const T&> : true_type {};
template <typename T>
struct is_const <const T&&> : true_type {};

template <typename T>
concept const_type = is_const <T>::value;
template <typename T>
concept mutable_type = ( is_const <T>::value == false );

template <typename T>
AINLINE constexpr const T& make_const_ref( T& v ) noexcept { return v; }

template <size_t N, typename... Ts> requires ( sizeof...(Ts) > 0 && N < sizeof...(Ts) )
AINLINE constexpr typename sequence_get <N, Ts...>::type&& select_from( Ts&&... args ) noexcept
{
    auto lamb = [&] <size_t IDX, typename T, typename... subTs> ( auto& lamb, T&& sel, subTs&&... next ) LAINLINE -> typename sequence_get <N, Ts...>::type&&
    {
        if constexpr ( IDX == N )
        {
            return castforward <T> ( sel );
        }
        else
        {
            return lamb.template operator() <IDX+1> ( lamb, castforward <subTs> ( next )... );
        }
    };

    return lamb.template operator() <0> ( lamb, castforward <Ts> ( args )... );
}

typedef typelist <char, signed char, signed short, signed int, signed long, signed long long> known_signed_integrals;
typedef typelist <unsigned char, unsigned short, unsigned int, unsigned long, unsigned long long> known_unsigned_integrals;

template <size_t SZ, typename...>
struct find_sizeof_equal_type : false_type {};
template <size_t SZ, typename... Ts>
struct find_sizeof_equal_type <SZ, typelist <Ts...>> : find_sizeof_equal_type <SZ, Ts...> {};
template <size_t SZ, typename T, typename... Ts> requires ( sizeof(T) == SZ )
struct find_sizeof_equal_type <SZ, T, Ts...> : type_identity <T>, true_type {};
template <size_t SZ, typename T, typename... Ts> requires ( sizeof(T) != SZ )
struct find_sizeof_equal_type <SZ, T, Ts...> : find_sizeof_equal_type <SZ, Ts...> {};

template <typename SZT, typename... Ts>
concept sizeof_equal_type = find_sizeof_equal_type <sizeof(SZT), typelist <Ts...>>::value;

#if __has_include(<type_traits>) && !defined(_EIRSDK_PREFER_NATIVE_IMPL)
template <typename T>
struct is_unsigned_integral
{
    static constexpr bool value = ( std::is_unsigned <T>::value && std::is_integral <T>::value );
};
#else
template <typename T>
struct is_unsigned_integral : sequence_contains <T, known_unsigned_integrals> {};
#endif

template <typename T>
concept unsigned_integral = is_unsigned_integral <T>::value;

#if __has_include(<type_traits>) && !defined(_EIRSDK_PREFER_NATIVE_IMPL)
template <typename T>
struct is_signed_integral
{
    static constexpr bool value = ( std::is_signed <T>::value && std::is_integral <T>::value );
};
#else
template <typename T>
struct is_signed_integral : sequence_contains <T, known_signed_integrals> {};
#endif

template <typename T>
concept signed_integral = is_signed_integral <T>::value;

template <typename T>
concept integral_type = unsigned_integral <T> || signed_integral <T>;

#if __has_include(<type_traits>) && !defined(_EIRSDK_PREFER_NATIVE_IMPL)
template <typename T>
struct make_signed_integral
{
    typedef typename std::enable_if <std::is_integral <T>::value, typename std::make_signed <T>::type>::type type;
};
#else
template <typename>
struct make_signed_integral {};
template <signed_integral T>
struct make_signed_integral <T> : type_identity <T> {};
template <>
struct make_signed_integral <unsigned char> : type_identity <signed char> {};
template <>
struct make_signed_integral <unsigned short> : type_identity <short> {};
template <>
struct make_signed_integral <unsigned int> : type_identity <int> {};
template <>
struct make_signed_integral <unsigned long> : type_identity <long> {};
template <>
struct make_signed_integral <unsigned long long> : type_identity <long long> {};
#endif

template <typename T>
struct make_signed : type_identity <T> {};
template <unsigned_integral T>
struct make_signed <T> : make_signed_integral <T> {};

#if __has_include(<type_traits>) && !defined(_EIRSDK_PREFER_NATIVE_IMPL)
template <typename T>
struct make_unsigned_integral
{
    typedef typename std::enable_if <std::is_integral <T>::value, typename std::make_unsigned <T>::type>::type type;
};
#else
template <typename>
struct make_unsigned_integral {};
template <unsigned_integral T>
struct make_unsigned_integral <T> : type_identity <T> {};
template <>
struct make_unsigned_integral <char> : type_identity <unsigned char> {};
template <>
struct make_unsigned_integral <signed char> : type_identity <unsigned char> {};
template <>
struct make_unsigned_integral <short> : type_identity <unsigned short> {};
template <>
struct make_unsigned_integral <int> : type_identity <unsigned int> {};
template <>
struct make_unsigned_integral <long long> : type_identity <unsigned long long> {};
template <>
struct make_unsigned_integral <long> : type_identity <unsigned long> {};
#endif

template <typename T>
struct make_unsigned : type_identity <T> {};
template <signed_integral T>
struct make_unsigned <T> : make_unsigned_integral <T> {};

// Converts to the underlying type, throwing away any array-spec, pointer-spec, reference, etc.
template <typename T>
struct plain_type
{
    typedef T type;
};
template <typename T> requires ( is_const <T>::value == false )
struct plain_type <volatile T> : public plain_type <T> {};
template <typename T>
struct plain_type <const T> : public plain_type <T> {};
template <typename T>
struct plain_type <T&> : public plain_type <T> {};
template <typename T>
struct plain_type <T&&> : public plain_type <T> {};
template <typename T>
struct plain_type <T*> : public plain_type <T> {};
template <typename T>
struct plain_type <T[]> : public plain_type <T> {};

// Converts to the underlying type of const/volatile/reference types.
template <typename T>
struct base_type
{
    typedef T type;
};
template <typename T> requires ( is_volatile <T>::value == false )
struct base_type <const T> : public base_type <T> {};
template <typename T>
struct base_type <volatile T> : public base_type <T> {};
template <typename T>
struct base_type <T&> : public base_type <T> {};
template <typename T>
struct base_type <T&&> : public base_type <T> {};

// Converts to the type without specification modification (const, volatile, etc).
template <typename T>
struct nospecmod_type
{
    typedef T type;
};
template <typename T> requires ( is_volatile <T>::value == false )
struct nospecmod_type <const T> : nospecmod_type <T> {};
template <typename T>
struct nospecmod_type <volatile T> : nospecmod_type <T> {};

// Checks if a type is POD.
template <typename T>
struct is_pod_type : public false_type {};
template <integral_type T>
struct is_pod_type <T> : public true_type {};
template <>
struct is_pod_type <float> : public true_type {};
template <>
struct is_pod_type <double> : public true_type {};
template <>
struct is_pod_type <bool> : public true_type {};
template <typename T>
struct is_pod_type <const T> : public is_pod_type <T> {};
template <typename T>
struct is_pod_type <volatile T> : public is_pod_type <T> {};
template <typename T>
struct is_pod_type <T&> : public is_pod_type <T> {};
template <typename T>
struct is_pod_type <T&&> : public is_pod_type <T> {};
template <typename T>
struct is_pod_type <T*> : public true_type {};

template <bool eval, typename true_type, typename false_type>
struct conditional
{
    typedef false_type type;
};
template <typename true_type, typename false_type>
struct conditional <true, true_type, false_type>
{
    typedef true_type type;
};

// Returns the first biggest type in the list.
template <typename... Ts>
struct biggest_type
{
    typedef void type;
};
template <typename BT, typename... Ts>
struct biggest_type <BT, Ts...>
{
    typedef BT type;
};
template <typename BT, typename T, typename... Ts>
struct biggest_type <BT, T, Ts...>
    : public biggest_type <typename conditional <(sizeof(BT) >= sizeof(T)), BT, T>::type, Ts...> {};

template <typename... Ts>
struct biggest_type <typelist <Ts...>> : public biggest_type <Ts...> {};

// Returns the first smallest type in the list.
template <typename... Ts>
struct smallest_type
{
    typedef void type;
};
template <typename ST, typename... Ts>
struct smallest_type <ST, Ts...>
{
    typedef ST type;
};
template <typename ST, typename T, typename... Ts>
struct smallest_type <ST, T, Ts...>
    : public smallest_type <typename conditional <(sizeof(ST) <= sizeof(T)), ST, T>::type, Ts...> {};

template <typename... Ts>
struct smallest_type <typelist <Ts...>> : public smallest_type <Ts...> {};

// Filter template for type-list operations.
template <template <typename> typename filter, typename... Ts>
struct types_filter {};
template <template <typename> typename filter, typename... Ts>
struct types_filter <filter, typelist <Ts...>> : types_filter <filter, typelist <Ts...>, typelist <>> {};
template <template <typename> typename filter, typename... Ts1, typename... Ts2, typename T>
struct types_filter <filter, typelist <T, Ts1...>, typelist <Ts2...>>
    : public types_filter <filter, typelist <Ts1...>,
        typename conditional <(filter <T>::value), typelist <Ts2..., T>, typelist <Ts2...>>::type
    >::type {};

// Represents a list of types at least as big as the posted type.
template <typename... Ts>
struct sizeof_greaterequal_types {};
template <typename... Ts, typename CT>
struct sizeof_greaterequal_types <CT, typelist <Ts...>> : sizeof_greaterequal_types <CT, typelist <Ts...>, typelist <>> {};
template <typename... Ts1, typename CT>
struct sizeof_greaterequal_types <CT, typelist <>, typelist <Ts1...>>
{
    typedef typelist <Ts1...> type;
};
template <typename... Ts1, typename CT, typename T, typename... Ts2>
struct sizeof_greaterequal_types <CT, typelist <T, Ts1...>, typelist <Ts2...>>
    : public sizeof_greaterequal_types <CT, typelist <Ts1...>,
        typename conditional <(sizeof(CT)<=sizeof(T)),
            typelist <Ts2..., T>,
            typelist <Ts2...>
        >::type
    > {};

// Represents a sorted list of types, in ascending sizeof order.
template <typename... Ts>
struct sizeof_sorted_types {};
template <typename... Ts>
struct sizeof_sorted_types <typelist <Ts...>> : public sizeof_sorted_types <typelist <Ts...>, typelist <>> {};
template <typename... Ts1, typename T, typename... Ts2>
struct sizeof_sorted_types <typelist <T, Ts1...>, typelist <Ts2...>>
    : conditional <((sizeof(T)<=sizeof(Ts1)) && ... ),
        sizeof_sorted_types <typelist <Ts1...>, typelist <Ts2..., T>>,
        sizeof_sorted_types <typelist <Ts1..., T>, typelist <Ts2...>>
    >::type {};
template <typename... Ts>
struct sizeof_sorted_types <typelist <>, typelist <Ts...>> : public type_identity <typelist <Ts...>> {};

// Reverses a list of types.
template <typename... Ts>
struct types_reverse {};
template <typename... Ts>
struct types_reverse <typelist <Ts...>> : types_reverse <typelist <Ts...>, typelist <>> {};
template <typename... Ts1, typename T, typename... Ts2>
struct types_reverse <typelist <T, Ts1...>, typelist <Ts2...>> : types_reverse <typelist <Ts1...>, typelist <T, Ts2...>> {};
template <typename... Ts>
struct types_reverse <typelist <>, typelist <Ts...>> : type_identity <typelist <Ts...>> {};

// Returns true if A is a type-list subset of B.
template <typename A, typename B> requires ( is_typelist <A>::value && is_typelist <B>::value )
struct typelist_subset : false_type {};
template <typename... Ts>
struct typelist_subset <typelist <>, typelist <Ts...>> : true_type {};
template <typename T, typename... TsA, typename... TsB> requires ( is_same_as <T, TsB>::value || ... )
struct typelist_subset <typelist <T, TsA...>, typelist <TsB...>> : public typelist_subset <typelist <TsA...>, typelist <TsB...>> {};

// Returns a typelist subtraction.
template <typename...>
struct typelist_subtract {};
template <typename... Ts1, typename... Ts2>
struct typelist_subtract <typelist <Ts1...>, typelist <Ts2...>> : typelist_subtract <typelist <Ts1...>, typelist <Ts2...>, typelist <>> {};
template <typename... Ts2, typename... Ts3>
struct typelist_subtract <typelist <>, typelist <Ts2...>, typelist <Ts3...>>
{
    typedef typelist <Ts3...> type;
};
template <typename... Ts1, typename... Ts2, typename T, typename... Ts3>
struct typelist_subtract <typelist <T, Ts1...>, typelist <Ts2...>, typelist <Ts3...>>
    : typelist_subtract <typelist <Ts1...>, typelist <Ts2...>,
        typename conditional <(same_as <T, Ts2> || ...),
            typelist <Ts3...>,
            typelist <Ts3..., T>
        >::type
    > {};

// Returns a shared type-list between input type-lists.
template <typename...>
struct typelist_shared {};
template <typename... Ts1, typename... Ts2>
struct typelist_shared <typelist <Ts1...>, typelist <Ts2...>> : typelist_shared <typelist <Ts1...>, typelist <Ts2...>, typelist <>> {};
template <typename... Ts2, typename... Ts3>
struct typelist_shared <typelist <>, typelist <Ts2...>, typelist <Ts3...>>
{
    typedef typelist <Ts3...> type;
};
template <typename... Ts1, typename T, typename... Ts2, typename... Ts3>
struct typelist_shared <typelist <T, Ts1...>, typelist <Ts2...>, typelist <Ts3...>>
    : typelist_shared <typelist <Ts1...>, typelist <Ts2...>,
        typename conditional <(same_as <T, Ts2> || ...),
            typelist <Ts3..., T>,
            typelist <Ts3...>
        >::type
    > {};

// Returns a distinct set of types.
template <typename...>
struct types_distinct {};
template <typename... Ts>
struct types_distinct <typelist <Ts...>> : types_distinct <typelist <Ts...>, typelist <>> {};
template <typename... Ts>
struct types_distinct <typelist <>, typelist <Ts...>>
{
    typedef typelist <Ts...> type;
};
template <typename T, typename... Ts1, typename... Ts2>
struct types_distinct <typelist <T, Ts1...>, typelist <Ts2...>>
    : types_distinct <typelist <Ts1...>,
        typename conditional <(same_as <T, Ts2> || ...),
            typelist <Ts2...>,
            typelist <Ts2..., T>
        >::type
    > {};

// Returns the union of type lists.
template <typename...>
struct typelist_union {};
template <typename... Ts1, typename... Ts2>
struct typelist_union <typelist <Ts1...>, typelist <Ts2...>> : types_distinct <typelist <Ts1..., Ts2...>> {};

template <typename T>
struct is_plain_type : public false_type {};
template <typename T> requires ( is_same_as <T, typename plain_type <T>::type>::value )
struct is_plain_type <T> : public true_type {};

template <typename T1, typename T2>
struct is_plain_same_as : public is_same_as <typename plain_type <T1>::type, typename plain_type <T2>::type> {};

// Friendship resistant replacement for std::constructible_from (it is buggy as shit, woof meow).
template <typename T, typename... cArgTypes>
concept constructible_from =
    requires ( cArgTypes&&... cArgs ) { new (nullptr) T( castforward <cArgTypes> ( cArgs )... ); };

template <typename T, typename... cArgTypes>
concept nothrow_constructible_from =
    requires ( cArgTypes&&... cArgs ) { { new (nullptr) T( castforward <cArgTypes> ( cArgs )... ) } noexcept; };

template <typename T, typename... argTypes>
AINLINE T& placement_constructor( T& storage, argTypes&&... args )
    noexcept(nothrow_constructible_from <T, argTypes&&...>)
    requires(constructible_from <T, argTypes&&...>)
{
    // Language bug: if the placement new function is not defined then this placement-constructor invocation will fail.
    // Typically, that placement-new function is defined with an empty body/just a return statement.
    return *::new (&storage) T( castforward <argTypes> ( args )... );
}
// WARNING: this one returns a POINTER instead of a REFERENCE!
template <typename T, typename... argTypes>
AINLINE T* placement_constructor( void *storage_ptr, argTypes&&... args )
    noexcept(nothrow_constructible_from <T, argTypes&&...>)
    requires(constructible_from <T, argTypes&&...>)
{
    return &placement_constructor( *(T*)storage_ptr, castforward <argTypes> ( args )... );
}

#if __has_include(<limits>) && !defined(_EIRSDK_PREFER_NATIVE_IMPL)
template <typename T>
struct numeric_limits
{
    static constexpr T MIN = std::numeric_limits <T>::min();
    static constexpr T MAX = std::numeric_limits <T>::max();
};
#else
template <typename T>
struct numeric_limits
{};
template <sizeof_equal_type <int8_t> T> requires ( signed_integral <T> )
struct numeric_limits <T>
{
    static constexpr T MIN = (int8_t)-128i8;
    static constexpr T MAX = 127;
};
// Unsigned integral could be generalized if we had C++20 concepts.
template <sizeof_equal_type <uint8_t> T> requires ( unsigned_integral <T> )
struct numeric_limits <T>
{
    static constexpr T MIN = 0u;
    static constexpr T MAX = 255u;
};
template <sizeof_equal_type <int16_t> T> requires ( signed_integral <T> )
struct numeric_limits <T>
{
    static constexpr T MIN = (int16_t)-0x8000i16;
    static constexpr T MAX = 0x7FFF;
};
template <sizeof_equal_type <uint16_t> T> requires ( unsigned_integral <T> )
struct numeric_limits <T>
{
    static constexpr T MIN = 0u;
    static constexpr T MAX = 0xFFFFu;
};
template <sizeof_equal_type <int32_t> T> requires ( signed_integral <T> )
struct numeric_limits <T>
{
    static constexpr T MIN = (int32_t)-0x80000000i32;
    static constexpr T MAX = 0x7FFFFFFF;
};
template <sizeof_equal_type <uint32_t> T> requires ( unsigned_integral <T> )
struct numeric_limits <T>
{
    static constexpr T MIN = 0u;
    static constexpr T MAX = 0xFFFFFFFFu;
};
template <sizeof_equal_type <int64_t> T> requires ( signed_integral <T> )
struct numeric_limits <T>
{
    static constexpr T MIN = (int64_t)-0x8000000000000000i64;
    static constexpr T MAX = 0x7FFFFFFFFFFFFFFF;
};
template <sizeof_equal_type <uint64_t> T> requires ( unsigned_integral <T> )
struct numeric_limits <T>
{
    static constexpr T MIN = 0u;
    static constexpr T MAX = 0xFFFFFFFFFFFFFFFFu;
};
#endif

template <size_t num, typename FT, typename... Ts>
struct fitting_numeric
{
    typedef FT type;
};
template <size_t num, typename FT, typename T, typename... Ts>
struct fitting_numeric <num, FT, T, Ts...>
    : public fitting_numeric <num, typename conditional <(numeric_limits <T>::MAX >= num && sizeof(FT) > sizeof(T)), T, FT>::type, Ts...> {};

template <size_t num>
struct fitting_unsigned_integral : public fitting_numeric <num, uint8_t, uint16_t, uint32_t, uint64_t> {};

template <size_t num>
struct fitting_signed_integral : public fitting_numeric <num, int8_t, int16_t, int32_t, int64_t> {};

enum class eTypeIdentification
{
    INCREMENTAL,
    BY_SIZE
};

// Configurable type selector.
template <eTypeIdentification idschema = eTypeIdentification::INCREMENTAL, typename... Ts>
struct CustomTypeSelector
{
    typedef typelist <Ts...> type_list;
    typedef typename fitting_unsigned_integral <sizeof(typename biggest_type <Ts..., uint8_t>::type)>::type typesize_t;

    typedef typename
        conditional <(idschema == eTypeIdentification::INCREMENTAL), typename fitting_unsigned_integral <sizeof...(Ts)>::type,
            typename conditional <(idschema == eTypeIdentification::BY_SIZE),
                typesize_t,
                void
            >::type
        >::type
        typeid_t;
private:
    template <size_t N, typename CT, typename... TTs>
    struct index_in_sequence
    {
        static constexpr size_t id = N;
    };
    template <size_t N, typename CT, typename... TTs>
    struct index_in_sequence <N, CT, CT, TTs...>
    {
        static constexpr size_t id = N;
    };
    template <size_t N, typename CT, typename T, typename... TTs>
    struct index_in_sequence <N, CT, T, TTs...> : public index_in_sequence <N+1, CT, TTs...> {};

    template <size_t N, typename... TTs>
    struct select_type {};
    template <typename T, typename... TTs>
    struct select_type <0, T, TTs...>
    {
        typedef T type;
    };
    template <size_t N, typename T, typename... TTs>
    struct select_type <N, T, TTs...> : public select_type <N-1, TTs...> {};

public:
    static constexpr typeid_t INVALID_INDEX =
        ( idschema == eTypeIdentification::INCREMENTAL ? sizeof...(Ts) :
        ( idschema == eTypeIdentification::BY_SIZE ? 0u :
        0u ));

    AINLINE static constexpr bool IsValidIndex( typeid_t tn ) noexcept
    {
        if constexpr ( idschema == eTypeIdentification::INCREMENTAL )
        {
            return ( tn < sizeof...( Ts ) );
        }
        else
        {
            return ( tn != INVALID_INDEX );
        }
    }

    template <typename T>
    AINLINE static constexpr typeid_t GetTypeIndex( void ) noexcept
    {
        if constexpr ( idschema == eTypeIdentification::INCREMENTAL )
        {
            return index_in_sequence <0, T, Ts...>::id;
        }
        else if constexpr ( idschema == eTypeIdentification::BY_SIZE )
        {
            if constexpr ( index_in_sequence <0, T, Ts...>::id < sizeof...(Ts) )
            {
                return sizeof( T );
            }
            else
            {
                return 0u;
            }
        }
    }

    AINLINE static constexpr typesize_t GetSizeForIndex( typeid_t tn ) noexcept
    {
        if constexpr ( idschema == eTypeIdentification::INCREMENTAL )
        {
            typeid_t curidx = 0;
            typesize_t sz = 0;

            auto lamb = [&] <typename T> ( T _ ) LAINLINE
            {
                if ( sz == 0 && curidx == tn )
                {
                    sz = sizeof(T);
                }
                curidx++;
            };

            ( lamb( Ts() ), ... );

            return sz;
        }
        else if constexpr ( idschema == eTypeIdentification::BY_SIZE )
        {
            return tn;
        }
    }

    template <typename T>
    AINLINE static constexpr bool Contains( void ) noexcept
    {
        return ( IsValidIndex( GetTypeIndex <T> () ) );
    }

    AINLINE constexpr CustomTypeSelector( void ) noexcept : tn( INVALID_INDEX ) {}
    AINLINE constexpr CustomTypeSelector( const CustomTypeSelector& ) = default;

    template <typename T>
    AINLINE void Select( void ) noexcept
    {
        this->tn = GetTypeIndex <T> ();
    }
    AINLINE void Unselect( void ) noexcept
    {
        this->tn = INVALID_INDEX;
    }

    AINLINE typeid_t GetIndex( void ) const noexcept
    {
        return this->tn;
    }

    AINLINE void SelectByIndex( typeid_t tid ) noexcept
    {
        // This is okay because if the index is >= sizeof...(Ts) then it is invalid, even if much above.
        // You have to be careful for type ids when the type id is a type's size!
        this->tn = tid;
    }
    AINLINE void SelectBySize( typesize_t tsz ) noexcept requires ( idschema == eTypeIdentification::BY_SIZE )
    {
        this->tn = tsz;
    }

    template <typename T>
    AINLINE constexpr bool IsSelected( void ) const noexcept
    {
        if constexpr ( sizeof...( Ts ) == 0 )
        {
            return false;
        }
        else
        {
            return ( this->tn == GetTypeIndex <T> () );
        }
    }
    AINLINE constexpr bool HasSelection( void ) const noexcept
    {
        return ( sizeof...( Ts ) > 0 && IsValidIndex( this->tn ) );
    }

    AINLINE static constexpr size_t GetCount( void ) noexcept
    {
        return sizeof... ( Ts );
    }

    template <typename callbackType>
    AINLINE constexpr bool Switch( callbackType&& cb ) const
    {
        bool exec = false;

        auto lambda = [&] <typename T> () LAINLINE
        {
            if ( GetTypeIndex <T> () == this->tn )
            {
                cb.template operator() <T> ();
                exec = true;
            }
        };

        ( lambda.template operator() <Ts> (), ... );

        return exec;
    }

private:
    typeid_t tn;
};

template <eTypeIdentification idschema, typename... Ts>
struct CustomTypeSelector <idschema, typelist <Ts...>> : public CustomTypeSelector <idschema, Ts...> {};

// No version for smallest_type because deprecated.
template <eTypeIdentification idschema, typename... Ts>
struct biggest_type <CustomTypeSelector <idschema, Ts...>> : public biggest_type <Ts...> {};

// Simple incremental schema type selector.
template <typename... Ts>
using TypeSelector = CustomTypeSelector <eTypeIdentification::INCREMENTAL, Ts...>;

template <typename... Ts>
struct types_distinct_sizeof : public false_type {};
template <typename... Ts>
struct types_distinct_sizeof <typelist <Ts...>> : public types_distinct_sizeof <Ts...> {};
template <typename T>
struct types_distinct_sizeof <T> : public true_type {};
template <typename T, typename... Ts>
struct types_distinct_sizeof <T, Ts...> : public conditional <((sizeof(T)!=sizeof(Ts))&&...), types_distinct_sizeof <Ts...>, false_type>::type {};

template <typename T>
struct type_identity_and_true : public type_identity <T>, true_type {};

template <typename A, typename B>
concept reasonably_convertible_to =
    ( unsigned_integral <typename base_type <A>::type> && unsigned_integral <typename base_type <B>::type> && sizeof(A)<=sizeof(B) ) ||
    ( signed_integral <typename base_type <A>::type> && signed_integral <typename base_type <B>::type> && sizeof(A)<=sizeof(B) ) ||
    ( integral_type <typename base_type <B>::type> == false && constructible_from <B, A> );

// TODO: maybe first make a search-pass for finding a truly-safe conversion target, then
// a second search-pass trying to find a reasonable one.
template <typename... Ts>
struct types_find_conversion_target : false_type {};
template <typename... Ts>
struct types_find_conversion_target <typelist <Ts...>> : public types_find_conversion_target <typelist <Ts...>, typelist <Ts...>> {};
template <typename T, typename... Ts1, typename... Ts2> requires ( constructible_from <T, Ts2> && ... )
struct types_find_conversion_target <typelist <T, Ts1...>, typelist <Ts2...>>
    : public conditional <(reasonably_convertible_to <Ts2, T> && ...),
        type_identity_and_true <T>,
        types_find_conversion_target <typelist <Ts1...>, typelist <Ts2...>>
    >::type
{};

template <typename... Ts>
using OptTypeSelector = typename conditional <(types_distinct_sizeof <Ts...>::value),
    CustomTypeSelector <eTypeIdentification::BY_SIZE, Ts...>,
    CustomTypeSelector <eTypeIdentification::INCREMENTAL, Ts...>
>::type;

} // namespace eir

// Removed (INSTANCE/PERFORM)_METHCHECK(EX) in favor of C++20 concepts! SFINAE is a retarded way-of-thinking that abuses language rules.

// Check if a class has a specific field.
#define INSTANCE_FIELDCHECK( fieldName ) \
    template <typename T> \
    concept hasField_##fieldName = requires( T t ) { t.fieldName; }

#define PERFORM_FIELDCHECK( className, fieldName ) ( hasField_##fieldName <className> )

#define INSTANCE_SUBSTRUCTCHECK( subStructName ) \
    template <typename T> \
    concept hasSubStruct_##subStructName = requires { typename T::subStructName; T::subStructName(); }

#define PERFORM_SUBSTRUCTCHECK( className, subStructName ) ( hasSubStruct_##subStructName <className> )

#define DEFINE_HEAP_REDIR_ALLOC_DEFAULTED_OBJMANIPS( allocTypeName ) \
    AINLINE allocTypeName( void ) = default; \
    AINLINE allocTypeName( allocTypeName&& ) = default; \
    AINLINE allocTypeName( const allocTypeName& ) = default; \
    AINLINE allocTypeName& operator = ( allocTypeName&& ) = default; \
    AINLINE allocTypeName& operator = ( const allocTypeName& ) = default;

// Providing the everything-purpose standard allocator pattern in the Eir SDK!
// We want a common setup where the link to the DynamicTypeSystem (DTS) is fixed to the position of the DTS.
#define DEFINE_HEAP_REDIR_ALLOC_BYSTRUCT_STDLAYOUT( allocTypeName, redirAllocTypeName ) \
    DEFINE_HEAP_REDIR_ALLOC_DEFAULTED_OBJMANIPS( allocTypeName ) \
    static inline void* Allocate( void *refMem, size_t memSize, size_t alignment ) noexcept; \
    static inline bool Resize( void *refMem, void *objMem, size_t reqNewSize ) noexcept requires ( ::eir::ResizeMemoryAllocator <redirAllocTypeName> ); \
    static inline void* Realloc( void *refMem, void *objMem, size_t reqNewSize, size_t alignment ) noexcept requires ( ::eir::ReallocMemoryAllocator <redirAllocTypeName> ); \
    static inline void Free( void *refMem, void *memPtr ) noexcept;
#define DEFINE_HEAP_REDIR_ALLOC_BYSTRUCT( allocTypeName, redirAllocTypeName ) \
    struct allocTypeName \
    { \
        DEFINE_HEAP_REDIR_ALLOC_BYSTRUCT_STDLAYOUT( allocTypeName, redirAllocTypeName ) \
    };
// Defines a heap redir allocator with resize-semantics.
#define DEFINE_HEAP_REDIR_ALLOC_IMPL_STDLAYOUT( allocTypeName ) \
    DEFINE_HEAP_REDIR_ALLOC_DEFAULTED_OBJMANIPS( allocTypeName ) \
    static inline void* Allocate( void *refMem, size_t memSize, size_t alignment ) noexcept; \
    static inline bool Resize( void *refMem, void *objMem, size_t reqNewSize ) noexcept; \
    static inline void Free( void *refMem, void *memPtr ) noexcept;
#define DEFINE_HEAP_REDIR_ALLOC_IMPL( allocTypeName ) \
    struct allocTypeName \
    { \
        DEFINE_HEAP_REDIR_ALLOC_IMPL_STDLAYOUT( allocTypeName ) \
    };

// Simple compatibility definitions for redir-allocators.
#define DEFINE_HEAP_REDIR_ALLOC_COMPATMETH( allocTypeName, compatAllocTypeName ) \
    AINLINE allocTypeName( struct compatAllocTypeName&& ) noexcept {} \
    AINLINE allocTypeName& operator = ( struct compatAllocTypeName&& ) noexcept { return *this; }

// Non-inline version of the heap allocator template.
#define DEFINE_HEAP_ALLOC( allocTypeName ) \
    struct allocTypeName \
    { \
        AINLINE allocTypeName( void ) = default; \
        AINLINE allocTypeName( allocTypeName&& ) = default; \
        AINLINE allocTypeName( const allocTypeName& ) = default; \
        AINLINE allocTypeName& operator = ( allocTypeName&& ) = default; \
        AINLINE allocTypeName& operator = ( const allocTypeName& ) = default; \
        static void* Allocate( void *refMem, size_t memSize, size_t alignment ) noexcept; \
        static bool Resize( void *refMem, void *objMem, size_t reqNewSize ) noexcept; \
        static void Free( void *refMem, void *memPtr ) noexcept; \
    };

// This thing assumes that the object pointed at by allocNode is of type "NativeHeapAllocator",
// but you may of course implement your own thing that has the same semantics.
#define IMPL_HEAP_REDIR_ALLOC( allocTypeName, hostStructTypeName, redirNode, allocNode ) \
    void* allocTypeName::Allocate( void *refMem, size_t memSize, size_t alignment ) noexcept \
    { \
        hostStructTypeName *hostStruct = LIST_GETITEM( hostStructTypeName, refMem, redirNode ); \
        return hostStruct->allocNode.Allocate( memSize, alignment ); \
    } \
    bool allocTypeName::Resize( void *refMem, void *objMem, size_t reqNewSize ) noexcept \
    { \
        hostStructTypeName *hostStruct = LIST_GETITEM( hostStructTypeName, refMem, redirNode ); \
        return hostStruct->allocNode.SetAllocationSize( objMem, reqNewSize ); \
    } \
    void allocTypeName::Free( void *refMem, void *memPtr ) noexcept \
    { \
        hostStructTypeName *hostStruct = LIST_GETITEM( hostStructTypeName, refMem, redirNode ); \
        hostStruct->allocNode.Free( memPtr ); \
    }

// Default macros for the allocator templates.
#define IMPL_HEAP_REDIR_METH_ALLOCATE_ARGS ( void *refMem, size_t memSize, size_t alignment ) noexcept
#define IMPL_HEAP_REDIR_METH_ALLOCATE_RETURN void*

#define IMPL_HEAP_REDIR_METH_RESIZE_ARGS_DIRECT ( void *refMem, void *objMem, size_t reqNewSize ) noexcept
#define IMPL_HEAP_REDIR_METH_RESIZE_ARGS(allocatorType) IMPL_HEAP_REDIR_METH_RESIZE_ARGS_DIRECT requires ( ::eir::ResizeMemoryAllocator <allocatorType> )
#define IMPL_HEAP_REDIR_METH_RESIZE_RETURN bool

#define IMPL_HEAP_REDIR_METH_REALLOC_ARGS_DIRECT ( void *refMem, void *objMem, size_t reqNewSize, size_t alignment ) noexcept
#define IMPL_HEAP_REDIR_METH_REALLOC_ARGS(allocatorType) IMPL_HEAP_REDIR_METH_REALLOC_ARGS_DIRECT requires ( eir::ReallocMemoryAllocator <allocatorType> )
#define IMPL_HEAP_REDIR_METH_REALLOC_RETURN void*

#define IMPL_HEAP_REDIR_METH_FREE_ARGS ( void *refMem, void *memPtr ) noexcept
#define IMPL_HEAP_REDIR_METH_FREE_RETURN void

// Direct allocation helpers that redirect calls to another static allocator that depends on a parent struct.
#define IMPL_HEAP_REDIR_DIRECT_ALLOC_METH_ALLOCATE_BODY( allocTypeName, hostStructTypeName, redirNode, directAllocTypeName ) \
    { \
        hostStructTypeName *hostStruct = LIST_GETITEM( hostStructTypeName, refMem, redirNode ); \
        return directAllocTypeName::Allocate( hostStruct, memSize, alignment ); \
    }

#define IMPL_HEAP_REDIR_DIRECT_ALLOC_METH_RESIZE_BODY( allocTypeName, hostStructTypeName, redirNode, directAllocTypeName ) \
    { \
        hostStructTypeName *hostStruct = LIST_GETITEM( hostStructTypeName, refMem, redirNode ); \
        return directAllocTypeName::Resize( hostStruct, objMem, reqNewSize ); \
    }

#define IMPL_HEAP_REDIR_DIRECT_ALLOC_METH_REALLOC_BODY( allocTypeName, hostStructTypeName, redirNode, directAllocTypeName ) \
    { \
        hostStructTypeName *hostStruct = LIST_GETITEM( hostStructTypeName, refMem, redirNode ); \
        return directAllocTypeName::Realloc( hostStruct, objMem, reqNewSize, alignment ); \
    }

#define IMPL_HEAP_REDIR_DIRECT_ALLOC_METH_FREE_BODY( allocTypeName, hostStructTypeName, redirNode, directAllocTypeName ) \
    { \
        hostStructTypeName *hostStruct = LIST_GETITEM( hostStructTypeName, refMem, redirNode ); \
        directAllocTypeName::Free( hostStruct, memPtr ); \
    }

// A simple redirector for allocators.
#define IMPL_HEAP_REDIR_DIRECT_ALLOC( allocTypeName, hostStructTypeName, redirNode, directAllocTypeName ) \
    IMPL_HEAP_REDIR_METH_ALLOCATE_RETURN allocTypeName::Allocate IMPL_HEAP_REDIR_METH_ALLOCATE_ARGS \
    IMPL_HEAP_REDIR_DIRECT_ALLOC_METH_ALLOCATE_BODY( allocTypeName, hostStructTypeName, redirNode, directAllocTypeName ) \
    IMPL_HEAP_REDIR_METH_RESIZE_RETURN allocTypeName::Resize IMPL_HEAP_REDIR_METH_RESIZE_ARGS(directAllocTypeName) \
    IMPL_HEAP_REDIR_DIRECT_ALLOC_METH_RESIZE_BODY( allocTypeName, hostStructTypeName, redirNode, directAllocTypeName ) \
    IMPL_HEAP_REDIR_METH_REALLOC_RETURN allocTypeName::Realloc IMPL_HEAP_REDIR_METH_REALLOC_ARGS(directAllocTypeName) \
    IMPL_HEAP_REDIR_DIRECT_ALLOC_METH_REALLOC_BODY( allocTypeName, hostStructTypeName, redirNode, directAllocTypeName ) \
    IMPL_HEAP_REDIR_METH_FREE_RETURN allocTypeName::Free IMPL_HEAP_REDIR_METH_FREE_ARGS \
    IMPL_HEAP_REDIR_DIRECT_ALLOC_METH_FREE_BODY( allocTypeName, hostStructTypeName, redirNode, directAllocTypeName )

#define IMPL_HEAP_REDIR_DIRECT_ALLOC_TEMPLATEBASE( templateBaseType, tbt_templargs, tbt_templuse, allocTypeName, hostStructTypeName, redirNode, directAllocTypeName ) \
    tbt_templargs \
    IMPL_HEAP_REDIR_METH_ALLOCATE_RETURN templateBaseType tbt_templuse::allocTypeName::Allocate IMPL_HEAP_REDIR_METH_ALLOCATE_ARGS \
    IMPL_HEAP_REDIR_DIRECT_ALLOC_METH_ALLOCATE_BODY( allocTypeName, hostStructTypeName, redirNode, directAllocTypeName ) \
    tbt_templargs \
    IMPL_HEAP_REDIR_METH_RESIZE_RETURN templateBaseType tbt_templuse::allocTypeName::Resize IMPL_HEAP_REDIR_METH_RESIZE_ARGS(directAllocTypeName) \
    IMPL_HEAP_REDIR_DIRECT_ALLOC_METH_RESIZE_BODY( allocTypeName, hostStructTypeName, redirNode, directAllocTypeName ) \
    tbt_templargs \
    IMPL_HEAP_REDIR_METH_REALLOC_RETURN templateBaseType tbt_templuse::allocTypeName::Realloc IMPL_HEAP_REDIR_METH_REALLOC_ARGS(directAllocTypeName) \
    IMPL_HEAP_REDIR_DIRECT_ALLOC_METH_REALLOC_BODY( allocTypeName, hostStructTypeName, redirNode, directAllocTypeName ) \
    tbt_templargs \
    IMPL_HEAP_REDIR_METH_FREE_RETURN templateBaseType tbt_templuse::allocTypeName::Free IMPL_HEAP_REDIR_METH_FREE_ARGS \
    IMPL_HEAP_REDIR_DIRECT_ALLOC_METH_FREE_BODY( allocTypeName, hostStructTypeName, redirNode, directAllocTypeName )

#define IMPL_HEAP_REDIR_DIRECTRPTR_ALLOC_METH_ALLOCATE_BODY( allocTypeName, hostStructTypeName, redirNode, refPtrNode, directAllocTypeName ) \
    { \
        hostStructTypeName *hostStruct = LIST_GETITEM( hostStructTypeName, refMem, redirNode ); \
        return directAllocTypeName::Allocate( hostStruct->refPtrNode, memSize, alignment ); \
    }

#define IMPL_HEAP_REDIR_DIRECTRPTR_ALLOC_METH_RESIZE_BODY( allocTypeName, hostStructTypeName, redirNode, refPtrNode, directAllocTypeName ) \
    { \
        hostStructTypeName *hostStruct = LIST_GETITEM( hostStructTypeName, refMem, redirNode ); \
        return directAllocTypeName::Resize( hostStruct->refPtrNode, objMem, reqNewSize ); \
    }

#define IMPL_HEAP_REDIR_DIRECTRPTR_ALLOC_METH_REALLOC_BODY( allocTypeName, hostStructTypeName, redirNode, refPtrNode, directAllocTypeName ) \
    { \
        hostStructTypeName *hostStruct = LIST_GETITEM( hostStructTypeName, refMem, redirNode ); \
        return directAllocTypeName::Realloc( hostStruct->refPtrNode, objMem, reqNewSize, alignment ); \
    }

#define IMPL_HEAP_REDIR_DIRECTRPTR_ALLOC_METH_FREE_BODY( allocTypeName, hostStructTypeName, redirNode, refPtrNode, directAllocTypeName ) \
    { \
        hostStructTypeName *hostStruct = LIST_GETITEM( hostStructTypeName, refMem, redirNode ); \
        directAllocTypeName::Free( hostStruct->refPtrNode, memPtr ); \
    }

#define IMPL_HEAP_REDIR_DIRECTRPTR_ALLOC( allocTypeName, hostStructTypeName, redirNode, refPtrNode, directAllocTypeName ) \
    IMPL_HEAP_REDIR_METH_ALLOCATE_RETURN allocTypeName::Allocate IMPL_HEAP_REDIR_METH_ALLOCATE_ARGS \
    IMPL_HEAP_REDIR_DIRECTRPTR_ALLOC_METH_ALLOCATE_BODY( allocTypeName, hostStructTypeName, redirNode, refPtrNode, directAllocTypeName ) \
    IMPL_HEAP_REDIR_METH_RESIZE_RETURN allocTypeName::Resize IMPL_HEAP_REDIR_METH_RESIZE_ARGS(directAllocTypeName) \
    IMPL_HEAP_REDIR_DIRECTRPTR_ALLOC_METH_RESIZE_BODY( allocTypeName, hostStructTypeName, redirNode, refPtrNode, directAllocTypeName ) \
    IMPL_HEAP_REDIR_METH_REALLOC_RETURN allocTypeName::Realloc IMPL_HEAP_REDIR_METH_REALLOC_ARGS(directAllocTypeName) \
    IMPL_HEAP_REDIR_DIRECTRPTR_ALLOC_METH_REALLOC_BODY( allocTypeName, hostStructTypeName, redirNode, refPtrNode, directAllocTypeName ) \
    IMPL_HEAP_REDIR_METH_FREE_RETURN allocTypeName::Free IMPL_HEAP_REDIR_METH_FREE_ARGS \
    IMPL_HEAP_REDIR_DIRECTRPTR_ALLOC_METH_FREE_BODY( allocTypeName, hostStructTypeName, redirNode, refPtrNode, directAllocTypeName )

#define IMPL_HEAP_REDIR_DIRECTRPTR_ALLOC_TEMPLATEBASE( templateBaseType, tbt_templargs, tbt_templuse, allocTypeName, hostStructTypeName, redirNode, refPtrNode, directAllocTypeName ) \
    tbt_templargs \
    IMPL_HEAP_REDIR_METH_ALLOCATE_RETURN templateBaseType tbt_templuse::allocTypeName::Allocate IMPL_HEAP_REDIR_METH_ALLOCATE_ARGS \
    IMPL_HEAP_REDIR_DIRECTRPTR_ALLOC_METH_ALLOCATE_BODY( allocTypeName, hostStructTypeName, redirNode, refPtrNode, directAllocTypeName ) \
    tbt_templargs \
    IMPL_HEAP_REDIR_METH_RESIZE_RETURN templateBaseType tbt_templuse::allocTypeName::Resize IMPL_HEAP_REDIR_METH_RESIZE_ARGS(directAllocTypeName) \
    IMPL_HEAP_REDIR_DIRECTRPTR_ALLOC_METH_RESIZE_BODY( allocTypeName, hostStructTypeName, redirNode, refPtrNode, directAllocTypeName ) \
    tbt_templargs \
    IMPL_HEAP_REDIR_METH_REALLOC_RETURN templateBaseType tbt_templuse::allocTypeName::Realloc IMPL_HEAP_REDIR_METH_REALLOC_ARGS(directAllocTypeName) \
    IMPL_HEAP_REDIR_DIRECTRPTR_ALLOC_METH_REALLOC_BODY( allocTypeName, hostStructTypeName, redirNode, refPtrNode, directAllocTypeName ) \
    tbt_templargs \
    IMPL_HEAP_REDIR_METH_FREE_RETURN templateBaseType tbt_templuse::allocTypeName::Free IMPL_HEAP_REDIR_METH_FREE_ARGS \
    IMPL_HEAP_REDIR_DIRECTRPTR_ALLOC_METH_FREE_BODY( allocTypeName, hostStructTypeName, redirNode, refPtrNode, directAllocTypeName )

// Similar to direct allocation but redirect calls to member allocator template instead.
#define IMPL_HEAP_REDIR_DYN_ALLOC_METH_ALLOCATE_BODY( hostStructTypeName, redirNode, dynAllocNode ) \
    { \
        hostStructTypeName *hostStruct = LIST_GETITEM( hostStructTypeName, refMem, redirNode ); \
        return hostStruct->dynAllocNode.Allocate( hostStruct, memSize, alignment ); \
    }

#define IMPL_HEAP_REDIR_DYN_ALLOC_METH_RESIZE_BODY( hostStructTypeName, redirNode, dynAllocNode ) \
    { \
        hostStructTypeName *hostStruct = LIST_GETITEM( hostStructTypeName, refMem, redirNode ); \
        return hostStruct->dynAllocNode.Resize( hostStruct, objMem, reqNewSize ); \
    }

#define IMPL_HEAP_REDIR_DYN_ALLOC_METH_REALLOC_BODY( hostStructTypeName, redirNode, dynAllocNode ) \
    { \
        hostStructTypeName *hostStruct = LIST_GETITEM( hostStructTypeName, refMem, redirNode ); \
        return hostStruct->dynAllocNode.Realloc( hostStruct, objMem, reqNewSize, alignment ); \
    }

#define IMPL_HEAP_REDIR_DYN_ALLOC_METH_FREE_BODY( hostStructTypeName, redirNode, dynAllocNode ) \
    { \
        hostStructTypeName *hostStruct = LIST_GETITEM( hostStructTypeName, refMem, redirNode ); \
        hostStruct->dynAllocNode.Free( hostStruct, memPtr ); \
    }

#define IMPL_HEAP_REDIR_DYN_ALLOC( allocTypeName, hostStructTypeName, redirNode, dynAllocNode, directAllocTypeName ) \
    IMPL_HEAP_REDIR_METH_ALLOCATE_RETURN allocTypeName::Allocate IMPL_HEAP_REDIR_METH_ALLOCATE_ARGS \
    IMPL_HEAP_REDIR_DYN_ALLOC_METH_ALLOCATE_BODY( hostStructTypeName, redirNode, dynAllocNode ) \
    IMPL_HEAP_REDIR_METH_RESIZE_RETURN allocTypeName::Resize IMPL_HEAP_REDIR_METH_RESIZE_ARGS(directAllocTypeName) \
    IMPL_HEAP_REDIR_DYN_ALLOC_METH_RESIZE_BODY( hostStructTypeName, redirNode, dynAllocNode ) \
    IMPL_HEAP_REDIR_METH_REALLOC_RETURN allocTypeName::Realloc IMPL_HEAP_REDIR_METH_REALLOC_ARGS(directAllocTypeName) \
    IMPL_HEAP_REDIR_DYN_ALLOC_METH_REALLOC_BODY( hostStructTypeName, redirNode, dynAllocNode ) \
    IMPL_HEAP_REDIR_METH_FREE_RETURN allocTypeName::Free IMPL_HEAP_REDIR_METH_FREE_ARGS \
    IMPL_HEAP_REDIR_DYN_ALLOC_METH_FREE_BODY( hostStructTypeName, redirNode, dynAllocNode )

#define IMPL_HEAP_REDIR_DYN_ALLOC_TEMPLATEBASE( templateBaseType, tbt_templargs, tbt_templuse, allocTypeName, hostStructTypeName, redirNode, dynAllocNode, directAllocTypeName ) \
    tbt_templargs \
    IMPL_HEAP_REDIR_METH_ALLOCATE_RETURN templateBaseType tbt_templuse::allocTypeName::Allocate IMPL_HEAP_REDIR_METH_ALLOCATE_ARGS \
    IMPL_HEAP_REDIR_DYN_ALLOC_METH_ALLOCATE_BODY( hostStructTypeName, redirNode, dynAllocNode ) \
    tbt_templargs \
    IMPL_HEAP_REDIR_METH_RESIZE_RETURN templateBaseType tbt_templuse::allocTypeName::Resize IMPL_HEAP_REDIR_METH_RESIZE_ARGS(directAllocTypeName) \
    IMPL_HEAP_REDIR_DYN_ALLOC_METH_RESIZE_BODY( hostStructTypeName, redirNode, dynAllocNode ) \
    tbt_templargs \
    IMPL_HEAP_REDIR_METH_REALLOC_RETURN templateBaseType tbt_templuse::allocTypeName::Realloc IMPL_HEAP_REDIR_METH_REALLOC_ARGS(directAllocTypeName) \
    IMPL_HEAP_REDIR_DYN_ALLOC_METH_REALLOC_BODY( hostStructTypeName, redirNode, dynAllocNode ) \
    tbt_templargs \
    IMPL_HEAP_REDIR_METH_FREE_RETURN templateBaseType tbt_templuse::allocTypeName::Free IMPL_HEAP_REDIR_METH_FREE_ARGS \
    IMPL_HEAP_REDIR_DYN_ALLOC_METH_FREE_BODY( hostStructTypeName, redirNode, dynAllocNode )

// Similiar to dyn-alloc but allows you to determine a separate refPtr which is on the node-way to
// the allocator itself.
#define IMPL_HEAP_REDIR_DYNRPTR_ALLOC_METH_ALLOCATE_BODY( hostStructTypeName, redirNode, refPtrNode, dynAllocRemNode ) \
    { \
        hostStructTypeName *hostStruct = LIST_GETITEM( hostStructTypeName, refMem, redirNode ); \
        auto *refObj = hostStruct->refPtrNode; \
        return refObj->dynAllocRemNode.Allocate( refObj, memSize, alignment ); \
    }

#define IMPL_HEAP_REDIR_DYNRPTR_ALLOC_METH_RESIZE_BODY( hostStructTypeName, redirNode, refPtrNode, dynAllocRemNode ) \
    { \
        hostStructTypeName *hostStruct = LIST_GETITEM( hostStructTypeName, refMem, redirNode ); \
        auto *refObj = hostStruct->refPtrNode; \
        return refObj->dynAllocRemNode.Resize( refObj, objMem, reqNewSize ); \
    }

#define IMPL_HEAP_REDIR_DYNRPTR_ALLOC_METH_REALLOC_BODY( hostStructTypeName, redirNode, refPtrNode, dynAllocRemNode ) \
    { \
        hostStructTypeName *hostStruct = LIST_GETITEM( hostStructTypeName, refMem, redirNode ); \
        auto *refObj = hostStruct->refPtrNode; \
        return refObj->dynAllocRemNode.Realloc( refObj, objMem, reqNewSize, alignment ); \
    }

#define IMPL_HEAP_REDIR_DYNRPTR_ALLOC_METH_FREE_BODY( hostStructTypeName, redirNode, refPtrNode, dynAllocRemNode ) \
    { \
        hostStructTypeName *hostStruct = LIST_GETITEM( hostStructTypeName, refMem, redirNode ); \
        auto *refObj = hostStruct->refPtrNode; \
        refObj->dynAllocRemNode.Free( refObj, memPtr ); \
    }

#define IMPL_HEAP_REDIR_DYNRPTR_ALLOC( allocTypeName, hostStructTypeName, redirNode, refPtrNode, dynAllocRemNode, directAllocTypeName ) \
    IMPL_HEAP_REDIR_METH_ALLOCATE_RETURN allocTypeName::Allocate IMPL_HEAP_REDIR_METH_ALLOCATE_ARGS \
    IMPL_HEAP_REDIR_DYNRPTR_ALLOC_METH_ALLOCATE_BODY( hostStructTypeName, redirNode, refPtrNode, dynAllocRemNode ) \
    IMPL_HEAP_REDIR_METH_RESIZE_RETURN allocTypeName::Resize IMPL_HEAP_REDIR_METH_RESIZE_ARGS(directAllocTypeName) \
    IMPL_HEAP_REDIR_DYNRPTR_ALLOC_METH_RESIZE_BODY( hostStructTypeName, redirNode, refPtrNode, dynAllocRemNode ) \
    IMPL_HEAP_REDIR_METH_REALLOC_RETURN allocTypeName::Realloc IMPL_HEAP_REDIR_METH_REALLOC_ARGS(directAllocTypeName) \
    IMPL_HEAP_REDIR_DYNRPTR_ALLOC_METH_REALLOC_BODY( hostStructTypeName, redirNode, refPtrNode, dynAllocRemNode ) \
    IMPL_HEAP_REDIR_METH_FREE_RETURN allocTypeName::Free IMPL_HEAP_REDIR_METH_FREE_ARGS \
    IMPL_HEAP_REDIR_DYNRPTR_ALLOC_METH_FREE_BODY( hostStructTypeName, redirNode, refPtrNode, dynAllocRemNode )

#define IMPL_HEAP_REDIR_DYNRPTR_ALLOC_TEMPLATEBASE( templateBaseType, tbt_templargs, tbt_templuse, allocTypeName, hostStructTypeName, redirNode, refPtrNode, dynAllocRemNode, directAllocTypeName ) \
    tbt_templargs \
    IMPL_HEAP_REDIR_METH_ALLOCATE_RETURN templateBaseType tbt_templuse::allocTypeName::Allocate IMPL_HEAP_REDIR_METH_ALLOCATE_ARGS \
    IMPL_HEAP_REDIR_DYNRPTR_ALLOC_METH_ALLOCATE_BODY( hostStructTypeName, redirNode, refPtrNode, dynAllocRemNode ) \
    tbt_templargs \
    IMPL_HEAP_REDIR_METH_RESIZE_RETURN templateBaseType tbt_templuse::allocTypeName::Resize IMPL_HEAP_REDIR_METH_RESIZE_ARGS(directAllocTypeName) \
    IMPL_HEAP_REDIR_DYNRPTR_ALLOC_METH_RESIZE_BODY( hostStructTypeName, redirNode, refPtrNode, dynAllocRemNode ) \
    tbt_templargs \
    IMPL_HEAP_REDIR_METH_REALLOC_RETURN templateBaseType tbt_templuse::allocTypeName::Realloc IMPL_HEAP_REDIR_METH_REALLOC_ARGS(directAllocTypeName) \
    IMPL_HEAP_REDIR_DYNRPTR_ALLOC_METH_REALLOC_BODY( hostStructTypeName, redirNode, refPtrNode, dynAllocRemNode ) \
    tbt_templargs \
    IMPL_HEAP_REDIR_METH_FREE_RETURN templateBaseType tbt_templuse::allocTypeName::Free IMPL_HEAP_REDIR_METH_FREE_ARGS \
    IMPL_HEAP_REDIR_DYNRPTR_ALLOC_METH_FREE_BODY( hostStructTypeName, redirNode, refPtrNode, dynAllocRemNode )

#endif //_COMMON_META_PROGRAMMING_HELPERS_
