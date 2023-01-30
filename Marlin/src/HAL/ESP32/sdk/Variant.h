/*****************************************************************************
*
*  PROJECT:     Eir SDK
*  FILE:        eirrepo/sdk/Variant.h
*  PURPOSE:     Discriminating union of types in one container, using type-id.
*
*  Find the Eir SDK at: https://osdn.net/projects/eirrepo/
*
*****************************************************************************/

#ifndef _EIR_VARIANT_HEADER_
#define _EIR_VARIANT_HEADER_

#include "MetaHelpers.h"
#include "Union.h"
#include "BitManip.h"

#ifdef _DEBUG
#include <assert.h>
#endif

namespace eir
{

namespace VariantUtils
{

// For optimizing variants that only contain POD types.
// After all, simplification of code-gen is really important in the wake of torn-apart-by-the-community compilers.
// The choice between equivalent code-graphs is the heaviest computational problem of modern-day compilers.
template <typename... Ts>
struct types_lifetime_desolate : public true_type {};
template <typename... Ts>
struct types_lifetime_desolate <typelist <Ts...>> : public types_lifetime_desolate <Ts...> {};
template <typename... Ts> requires ( ( is_pod_type <Ts>::value == false ) || ... )
struct types_lifetime_desolate <Ts...> : false_type {};

template <typename... Ts>
struct types_unsigned_integrals : false_type {};
template <typename... Ts>
struct types_unsigned_integrals <typelist <Ts...>> : types_unsigned_integrals <Ts...> {};
template <typename... Ts> requires ( is_unsigned_integral <Ts>::value && ... )
struct types_unsigned_integrals <Ts...> : true_type {};

} // namespace VariantUtils

struct invalid_value_exception {};

enum class eVariantType
{
    UNION,
    BIGGEST
};

template <typename typesel_t = TypeSelector <>>
struct CustomBasicVariant
{
    AINLINE CustomBasicVariant( void ) noexcept : typesel()
    {
        return;
    }
    // With C++20 we could make the constructor accept one of the supported types, by limiting
    // the constructor using C++20 concepts!
    AINLINE CustomBasicVariant( const CustomBasicVariant& right )
        : typesel( right.typesel )
    {
        this->typesel.Switch(
            [&] <typename T> () LAINLINE
        {
            placement_constructor <T> ( this->data.template Get <T> (), right.data.template Get <T> () );
        });
    }
    template <typename storeT> requires ( same_as <typename base_type <storeT>::type, CustomBasicVariant> == false && types_find_conversion_target <typename typesel_t::type_list, typelist <storeT>>::value )
    AINLINE CustomBasicVariant( storeT&& val )
    {
        if constexpr ( typesel_t::template Contains <typename base_type <storeT>::type> () )
        {
            this->Store( val );
        }
        else
        {
            this->Store <typename types_find_conversion_target <typename typesel_t::type_list, typelist <storeT>>::type> ( castforward <storeT> ( val ) );
        }
    }
    // Using C++20 we could generalize this to allow construction of bigger variants out of
    // smaller variants with fewer types (C++20 concepts!)
    AINLINE CustomBasicVariant( CustomBasicVariant&& right )  // TODO: noexcept using C++20 concepts!
        : typesel( castmove( right.typesel ) )
    {
        this->typesel.Switch(
            [&] <typename T> () LAINLINE
        {
            placement_constructor <T> ( this->data.template Get <T> (), castmove( right.data.template Get <T> () ) );
        });
    }

    AINLINE void Clear( void ) noexcept
    {
        if constexpr ( VariantUtils::types_lifetime_desolate <typename typesel_t::type_list>::value == false )
        {
            this->typesel.Switch(
                [&] <typename T> () LAINLINE
            {
                this->data.template Get <T> ().~T();
            });
        }
        this->typesel.Unselect();
    }

    AINLINE ~CustomBasicVariant( void )
    {
        this->Clear();
    }

    AINLINE CustomBasicVariant& operator = ( const CustomBasicVariant& right )
    {
        if constexpr ( VariantUtils::types_lifetime_desolate <typename typesel_t::type_list>::value )
        {
            this->typesel = right.typesel;
            this->typesel.Switch(
                [&] <typename T> () LAINLINE
            {
                this->data.template Get <T> () = right.data.template Get <T> ();
            });
        }
        else
        {
            if ( this->typesel.GetIndex() == right.typesel.GetIndex() )
            {
                this->typesel.Switch(
                    [&] <typename T> () LAINLINE
                {
                    this->data.template Get <T> () = right.data.template Get <T> ();
                });
            }
            else
            {
                this->Clear();
                this->typesel = right.typesel;
                this->typesel.Switch(
                    [&] <typename T> () LAINLINE
                {
                    placement_constructor <T> ( this->data.template Get <T> (), right.data.template Get <T> () );
                });
            }
        }
        return *this;
    }
    AINLINE CustomBasicVariant& operator = ( CustomBasicVariant&& right )
    {
        if constexpr ( VariantUtils::types_lifetime_desolate <typename typesel_t::type_list>::value )
        {
            this->typesel = right.typesel;
            this->typesel.Switch(
                [&] <typename T> () LAINLINE
            {
                this->data.template Get <T> () = castmove( right.data.template Get <T> () );
            });
        }
        else
        {
            if ( this->typesel.GetIndex() == right.typesel.GetIndex() )
            {
                this->typesel.Switch(
                    [&] <typename T> () LAINLINE
                {
                    this->data.template Get <T> () = castmove( right.data.template Get <T> () );
                });
            }
            else
            {
                this->Clear();
                this->typesel = right.typesel;
                this->typesel.Switch(
                    [&] <typename T> () LAINLINE
                {
                    placement_constructor <T> ( this->data.template Get <T> (), castmove( right.data.template Get <T> () ) );
                });
            }
        }
        return *this;
    }

    template <typename T> requires ( typesel_t::template Contains <typename base_type <T>::type> () )
    AINLINE void Store( T&& data )
    {
        typedef typename base_type <T>::type BT;

        if constexpr ( VariantUtils::types_lifetime_desolate <typename typesel_t::type_list>::value )
        {
            this->data.template Get <BT> () = castforward <T> ( data );
            this->typesel.template Select <BT> ();
        }
        else
        {
            if ( this->typesel.template IsSelected <BT> () )
            {
                this->data.template Get <BT> () = castforward <BT> ( data );
            }
            else
            {
                this->Clear();
                placement_constructor <BT> ( this->data.template Get <BT> (), castforward <T> ( data ) );
                this->typesel.template Select <BT> ();
            }
        }
    }
    AINLINE typename typesel_t::typesize_t GetSize( void ) const noexcept
    {
        return decltype(this->typesel)::GetSizeForIndex( this->typesel.GetIndex() );
    }

    AINLINE bool IsEmpty( void ) const noexcept
    {
        return ( this->typesel.HasSelection() == false );
    }
    template <typename T>
    AINLINE bool IsSelected( void ) const noexcept
    {
        return this->typesel.template IsSelected <T> ();
    }
    template <typename T>
    AINLINE T& Get( void )
    {
        // TODO: maybe add support to get base types of bigger types if the equal type does not exist.
        // This would still be fast because the search for a compatible type would be done at compile-time!

        if ( this->template IsSelected <T> () == false )
            throw invalid_value_exception();

        return this->data.template Get <T> ();
    }
    template <typename T>
    AINLINE const T& Get( void ) const
    {
        return const_cast <CustomBasicVariant*> ( this )->Get();
    }
    AINLINE auto& Get( void )
        requires ( typesel_t::type_list::count == 1 )
    {
        if ( this->IsEmpty() )
            throw invalid_value_exception();

        return this->data.Get();
    }
    AINLINE const auto& Get( void ) const
        requires ( typesel_t::type_list::count == 1 )
    {
        return const_cast <CustomBasicVariant*> ( this )->Get();
    }
    template <typename T>
    AINLINE T* GetOptional( void ) noexcept
    {
        if constexpr ( typesel_t::template Contains <T> () == false )
        {
            return nullptr;
        }
        else
        {
            if ( this->template IsSelected <T> () == false )
                return nullptr;

            return &this->data.template Get <T> ();
        }
    }
    template <typename T>
    AINLINE const T* GetOptional( void ) const noexcept
    {
        return const_cast <CustomBasicVariant*> ( this )->template GetOptional <T> ();
    }
    AINLINE auto* GetOptional( void ) noexcept
        requires ( typesel_t::type_list::count == 1 )
    {
        if ( this->IsEmpty() )
            return nullptr;

        return &this->data.Get();
    }
    AINLINE const auto* GetOptional( void ) const noexcept
        requires ( typesel_t::type_list::count == 1 )
    {
        if ( this->IsEmpty() )
            return nullptr;

        return &this->data.Get();
    }

    template <typename callbackType>
    AINLINE void Visit( callbackType&& cb )
    {
        this->typesel.Switch(
            [&] <typename T> () LAINLINE
        {
            cb( this->data.template Get <T> () );
        });
    }
    template <typename callbackType>
    AINLINE void Visit( callbackType&& cb ) const
    {
        this->typesel.Switch(
            [&] <typename T> () LAINLINE
        {
            cb( this->data.template Get <T> () );
        });
    }
    template <typename callbackType>
    AINLINE void VisitBitsFrom( size_t byteoff, callbackType&& cb ) const
        requires ( VariantUtils::types_unsigned_integrals <typename typesel_t::type_list>::value )
    {
#ifdef _DEBUG
        assert( byteoff < this->GetSize() );
#endif

        // This is a specialized-by-const optimization.

        typename types_find_conversion_target <typename typesel_t::type_list>::type tmp = 0u;
        typename typesel_t::typesize_t tsz = 0u;

        this->typesel.Switch(
            [&] <typename T> () LAINLINE
        {
            tmp = data.template Get <T> ();
            tsz = sizeof(T);
        });

        platformLocalBitcountType bitoff = (platformLocalBitcountType)( byteoff * 8u );
        platformLocalBitcountType bitcnt = (platformLocalBitcountType)( (tsz-byteoff)*8u );

        BitfieldNumberSelectionView <const decltype(tmp)> view( &tmp, bitoff, bitcnt );
        cb( view );
    }
    template <typename callbackType>
    AINLINE void VisitBitsFrom( size_t byteoff, callbackType&& cb )
        requires ( VariantUtils::types_unsigned_integrals <typename typesel_t::type_list>::value )
    {
#ifdef _DEBUG
        assert( byteoff < this->GetSize() );
#endif

        platformLocalBitcountType bitoff = (platformLocalBitcountType)( byteoff * 8u );

        this->typesel.Switch(
            [&] <typename T> () LAINLINE
        {
            platformLocalBitcountType bitcnt = (platformLocalBitcountType)( (sizeof(T) - byteoff)*8u );

            BitfieldNumberSelectionView view( &this->data.template Get <T> (), bitoff, bitcnt );

            cb( view );
        });
    }

    template <typename subset, typename callbackType>
        requires ( eir::typelist_subset <subset, typename typesel_t::type_list>::value )
    AINLINE bool SelectSubset( callbackType&& cb )
    {
        bool executed = subset::ForEach(
            [&] <typename T> () LAINLINE
        {
            T tmp;
            if (cb(tmp))
            {
                this->Store <T> ( castmove( tmp ) );
                return true;
            }
            return false;
        });
        if (executed == false)
        {
            this->typesel.Unselect();
        }
        return executed;
    }
    template <typename callbackType>
    AINLINE bool Select( callbackType&& cb )
    {
        return this->SelectSubset <typename typesel_t::type_list> ( castforward <callbackType> ( cb ) );
    }

    // Dangerous method! Use with caution.
    AINLINE void* GetStoragePointer( void ) noexcept
    {
        if constexpr ( typesel_t::GetCount() == 0 )
        {
            return nullptr;
        }
        else
        {
            return this->data.GetStoragePointer();
        }
    }
    AINLINE const void* GetStoragePointer( void ) const noexcept
    {
        return const_cast <CustomBasicVariant*> ( this )->GetStoragePointer();
    }

    AINLINE explicit operator bool ( void ) const noexcept
        requires ( typesel_t::template Contains <bool> () == false )
    {
        return ( this->IsEmpty() == false );
    }

    AINLINE operator auto ( void ) const
        requires ( typesel_t::type_list::count == 1 )
    {
        return this->Get();
    }

    template <typename storeT> requires ( same_as <typename base_type <storeT>::type, CustomBasicVariant> == false && types_find_conversion_target <typename typesel_t::type_list, typelist <storeT>>::value )
    AINLINE CustomBasicVariant& operator = ( storeT&& val )
    {
        if constexpr ( typesel_t::template Contains <typename base_type <storeT>::type> () )
        {
            this->Store( val );
        }
        else
        {
            this->Store <typename types_find_conversion_target <typename typesel_t::type_list, typelist <storeT>>::type> ( castforward <storeT> ( val ) );
        }
        return *this;
    }

    template <typename numberType>
        requires requires ( typesel_t TSEL, typename typesel_t::typesize_t TSZ ) { TSEL.SelectBySize( TSZ ); }
    AINLINE void StoreByBitfield( const numberType& val, typename typesel_t::typesize_t bytecnt ) noexcept
    {
        this->Select(
            [&] <typename T> ( T& data ) LAINLINE
        {
            if ( sizeof(T) == bytecnt )
            {
                data = (T)val;
                return true;
            }
            return false;
        });
    }

private:
    typesel_t typesel;
    Union <typesel_t> data;
};

template <typename typesel_t = TypeSelector <>>
struct CustomBitfieldVariant
{
private:
    typesel_t typesel;
    typename biggest_type <typename typesel_t::type_list>::type data;
public:
    template <typename T>
    using view_alias = StaticBitfieldNumberSelectionView <T, decltype(data)>;

    AINLINE CustomBitfieldVariant( void ) noexcept : typesel(), data()
    {
        return;
    }

    template <typename storeT>
    AINLINE CustomBitfieldVariant( storeT&& val ) requires ( same_as <typename base_type <storeT>::type, CustomBitfieldVariant> == false && types_find_conversion_target <typename typesel_t::type_list, typelist <storeT>>::value )
    {
        if constexpr ( typesel_t::template Contains <typename base_type <storeT>::type> () )
        {
            this->Store( val );
        }
        else
        {
            this->Store <typename types_find_conversion_target <typename typesel_t::type_list>::type, typelist <storeT>> ( val );
        }
    }
    AINLINE CustomBitfieldVariant( const CustomBitfieldVariant& ) = default;
    AINLINE CustomBitfieldVariant( CustomBitfieldVariant&& ) = default;

    AINLINE void Clear( void ) noexcept
    {
        this->data = {};
        this->typesel.Unselect();
    }

    AINLINE CustomBitfieldVariant& operator = ( const CustomBitfieldVariant& ) = default;
    AINLINE CustomBitfieldVariant& operator = ( CustomBitfieldVariant&& ) = default;

    template <typename T> requires ( typesel_t::template Contains <typename base_type <T>::type> () )
    AINLINE void Store( T&& val )
    {
        this->data = castmove( val );
        this->typesel.template Select <typename base_type <T>::type> ();
    }
    AINLINE typename typesel_t::typesize_t GetSize( void ) const noexcept
    {
        return typesel_t::GetSizeForIndex( this->typesel.GetIndex() );
    }

    AINLINE bool IsEmpty( void ) const noexcept
    {
        return ( this->typesel.HasSelection() == false );
    }
    template <typename T>
    AINLINE bool IsSelected( void ) const noexcept
    {
        return ( this->typesel.template IsSelected <T> () );
    }

    template <typename T>
    AINLINE typename conditional <(typesel_t::type_list::count == 1),
        typename sequence_get <0, typename typesel_t::type_list>::type&,
        StaticBitfieldNumberSelectionView <T, decltype(CustomBitfieldVariant::data)>
        >::type Get( void )
    {
        if ( this->template IsSelected <T> () == false )
            throw invalid_value_exception();

        if constexpr ( typesel_t::type_list::count == 1 )
        {
            return this->data;
        }
        else
        {
            return { &this->data };
        }
    }
    template <typename T>
    AINLINE typename conditional <(typesel_t::type_list::count == 1),
        typename sequence_get <0, typename typesel_t::type_list>::type,
        StaticBitfieldNumberSelectionView <T, decltype(CustomBitfieldVariant::data)>
        >::type Get( void ) const
    {
        return const_cast <CustomBitfieldVariant*> ( this )->template Get <T> ();
    }

    AINLINE auto& Get( void )
        requires ( typesel_t::type_list::count == 1 )
    {
        if ( this->IsEmpty() )
            throw invalid_value_exception();

        return this->data;
    }
    AINLINE const auto& Get( void ) const
        requires ( typesel_t::type_list::count == 1 )
    {
        if ( this->IsEmpty() )
            throw invalid_value_exception();

        return this->data;
    }

    template <typename callbackType>
    AINLINE void Visit( callbackType&& cb )
    {
        this->typesel.Switch(
            [&] <typename T> () LAINLINE
        {
            StaticBitfieldNumberSelectionView <T, decltype(this->data)> bfacc( &this->data, 0u );
            cb( bfacc );
        });
    }
    template <typename callbackType>
    AINLINE void VisitBitsFrom( size_t byteoff, callbackType&& cb )
    {
        if ( this->typesel.HasSelection() )
        {
            platformLocalBitcountType bitoff = (platformLocalBitcountType)( byteoff*8u );
            platformLocalBitcountType bitcnt = (platformLocalBitcountType)( (this->GetSize() - byteoff)*8u );

            BitfieldNumberSelectionView bfacc( &this->data, bitoff, bitcnt );
            cb( bfacc );
        }
    }
    template <typename callbackType>
    AINLINE void VisitBitsFrom( size_t byteoff, callbackType&& cb ) const
    {
        if ( this->typesel.HasSelection() )
        {
            platformLocalBitcountType bitoff = (platformLocalBitcountType)( byteoff*8u );
            platformLocalBitcountType bitcnt = (platformLocalBitcountType)( (this->GetSize() - byteoff)*8u );

            BitfieldNumberSelectionView bfacc( &this->data, bitoff, bitcnt );
            cb( bfacc );
        }
    }

    template <typename subset, typename callbackType>
        requires ( typelist_subset <subset, typename typesel_t::type_list>::value )
    AINLINE bool SelectSubset( callbackType&& cb )
    {
        bool executed = subset::ForEach(
            [&] <typename T> () LAINLINE
        {
            decltype(this->data) data = 0u;
            StaticBitfieldNumberSelectionView <T, decltype(this->data)> bfacc( &data, 0 );
            if (cb(bfacc))
            {
                this->data = data;
                this->typesel.template Select <T> ();
                return true;
            }
            return false;
        });
        if (executed == false)
        {
            this->typesel.Unselect();
        }
        return executed;
    }
    template <typename callbackType>
    AINLINE bool Select( callbackType&& cb )
    {
        return this->SelectSubset <typename typesel_t::type_list> ( castforward <callbackType> ( cb ) );
    }

    AINLINE void* GetStoragePointer( void ) noexcept
    {
        return &this->data;
    }
    AINLINE const void* GetStoragePointer( void ) const noexcept
    {
        return &this->data;
    }

    AINLINE explicit operator bool ( void ) const noexcept
    {
        return ( this->IsEmpty() == false );
    }

    AINLINE operator auto ( void ) const
        requires ( typesel_t::type_list::count == 1 )
    {
        return this->Get();
    }

    template <typename storeT> requires ( same_as <typename base_type <storeT>::type, CustomBitfieldVariant> == false && types_find_conversion_target <typename typesel_t::type_list, typelist <storeT>>::value )
    AINLINE CustomBitfieldVariant& operator = ( storeT&& val )
    {
        if constexpr ( typesel_t::template Contains <typename base_type <storeT>::type> () )
        {
            this->Store( val );
        }
        else
        {
            this->Store <typename types_find_conversion_target <typename typesel_t::type_list, typelist <storeT>>::type> ( val );
        }
        return *this;
    }

    template <typename numberType>
        requires requires ( typesel_t TSEL, typename typesel_t::typesize_t TSZ ) { TSEL.SelectBySize( TSZ ); }
    AINLINE void StoreByBitfield( const numberType& val, typename typesel_t::typesize_t bytecnt ) noexcept
    {
        this->data = val;
        this->typesel.SelectBySize( bytecnt );
    }
};

template <typename... Ts>
using Variant = CustomBasicVariant <OptTypeSelector <Ts...>>;
template <typename... Ts>
using BitfieldVariant = CustomBitfieldVariant <OptTypeSelector <Ts...>>;

} // namespace eir

#endif //_EIR_VARIANT_HEADER_
