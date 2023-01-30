/*****************************************************************************
*
*  PROJECT:     Eir SDK
*  FILE:        eirrepo/sdk/Union.h
*  PURPOSE:     Union struct for language-driven helpers.
*
*  Find the Eir SDK at: https://osdn.net/projects/eirrepo/
*
*****************************************************************************/

#ifndef _EIR_UNION_HEADER_
#define _EIR_UNION_HEADER_

#include "MetaHelpers.h"

namespace eir
{
    
// Union data storage model.
template <typename... Ts>
struct Union
{
    AINLINE Union( void ) noexcept {}
    template <typename storeT> requires ( same_as <typename base_type <storeT>::type, Union> == false && types_find_conversion_target <typelist <Ts...>, typelist <storeT>>::value )
    AINLINE Union( storeT&& val )
    {
        if constexpr ( ( is_same_as <typename base_type <storeT>::type, Ts>::value || ... ) )
        {
            this->emplace( castforward <storeT> ( val ) );
        }
        else
        {
            this->template emplace <typename types_find_conversion_target <typelist <Ts...>, typelist <storeT>>::type>  ( castforward <storeT> ( val ) );
        }
    }
    AINLINE Union( const Union& ) = delete;

    AINLINE Union& operator = ( const Union& ) = delete;

    template <typename T> requires ( is_same_as <Ts, T>::value || ... )
    AINLINE void emplace( T&& val )
    {
        placement_constructor <T> ( this->template Get <T> (), castforward <T> ( val ) );
    }
    template <typename T> requires ( is_same_as <Ts, T>::value || ... )
    AINLINE void destroy( void ) noexcept
    {
        this->data.template Get <T> ().~T();
    }

    AINLINE auto& Get( void ) noexcept requires ( sizeof...(Ts) == 1 )
    {
        return this->data.value;
    }
    AINLINE const auto& Get( void ) const noexcept requires ( sizeof...(Ts) == 1 )
    {
        return this->data.value;
    }
    template <typename T> requires ( is_same_as <T, Ts>::value || ... )
    AINLINE T& Get( void ) noexcept
    {
        T *ptr = nullptr;

        auto lamb = [&] <typename TI, typename... UTs> ( auto& lamb, unpacked_union <TI, UTs...>& memb ) LAINLINE
        {
            if constexpr ( is_same_as <T, TI>::value )
            {
                ptr = &memb.value;
            }
            else if constexpr ( sizeof...(UTs) > 0 )
            {
                lamb( lamb, memb.next );
            }
        };

        if constexpr ( sizeof...(Ts) > 0 )
        {
            lamb( lamb, this->data );
        }

        return *ptr;
    }
    template <typename T>
    AINLINE const T& Get( void ) const noexcept
    {
        return const_cast <Union*> ( this )->template Get <T> ();
    }

    template <typename T>
    AINLINE static constexpr bool Contains( void ) noexcept
    {
        return sequence_contains <T, Ts...>::value;
    }

    AINLINE void* GetStoragePointer( void ) noexcept
    {
        return &this->data.value;
    }
    AINLINE const void* GetStoragePointer( void ) const noexcept
    {
        return &this->data.value;
    }

private:
    template <typename... UTs>
    struct unpacked_union
    {}; // for the empty case.
    template <typename UT>
    struct unpacked_union <UT>
    {
        AINLINE unpacked_union( void ) noexcept {}

        UT value;
    };
    template <typename UT, typename... UTs>
    struct unpacked_union <UT, UTs...>
    {
        AINLINE unpacked_union( void ) noexcept {}

        union
        {
            UT value;
            unpacked_union <UTs...> next;
        };
    };

    unpacked_union <Ts...> data;
};

template <typename... Ts>
struct Union <typelist <Ts...>> : public Union <Ts...> {};

template <eTypeIdentification idschema, typename... Ts>
struct Union <CustomTypeSelector <idschema, Ts...>> : public Union <Ts...> {};

} // namespace eir

#endif //_EIR_UNION_HEADER_