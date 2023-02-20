/*****************************************************************************
*
*  PROJECT:     Eir SDK
*  FILE:        eirrepo/sdk/Arith.h
*  PURPOSE:     Arithmetic helpers
*
*  Find the Eir SDK at: https://osdn.net/projects/eirrepo/
*
*****************************************************************************/

#ifndef _EIRSDK_ARITH_HEADER_
#define _EIRSDK_ARITH_HEADER_

#include "MetaHelpers.h"
#include "PlatformStrategy.h"

namespace eir
{

template <typename opT, typename inputType>
AINLINE constexpr auto SIGNP_PROMOTE( inputType v ) noexcept
{
    if constexpr ( unsigned_integral <inputType> )
    {
        return (typename make_unsigned_integral <opT>::type)v;
    }
    else
    {
        return v;
    }
}

// Performs logical bitwise left-shift but keeps the sign-specification for the result type.
template <integral_type T>
AINLINE constexpr auto LSHIFT( T v, platformLocalBitcountType cnt ) noexcept
{
    return SIGNP_PROMOTE <decltype(v<<cnt)> ( v ) << cnt;
}
// Performs logical bitwise right-shift but keeps the sign-specification for the result type.
template <integral_type T>
AINLINE constexpr auto RSHIFT( T v, platformLocalBitcountType cnt ) noexcept
{
    return SIGNP_PROMOTE <decltype(v>>cnt)> ( v ) >> cnt;
}

template <integral_type LT, integral_type RT> requires ( signed_integral <LT> == signed_integral <RT> )
AINLINE constexpr auto ADD_SIGNP( LT left, RT right ) noexcept
{
    return ( SIGNP_PROMOTE <decltype(left+right)> ( left ) + SIGNP_PROMOTE <decltype(left+right)> ( right ) );
}

template <integral_type LT, integral_type RT> requires ( signed_integral <LT> == signed_integral <RT> )
AINLINE constexpr auto SUB_SIGNP( LT left, RT right ) noexcept
{
    return ( SIGNP_PROMOTE <decltype(left-right)> ( left ) - SIGNP_PROMOTE <decltype(left-right)> ( right ) );
}

} // namespace eir

#endif //_EIRSDK_ARITH_HEADER_
