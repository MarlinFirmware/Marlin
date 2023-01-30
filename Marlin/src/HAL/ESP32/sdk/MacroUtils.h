/*****************************************************************************
*
*  PROJECT:     Eir SDK
*  FILE:        eirrepo/sdk/MacroUtils.h
*  PURPOSE:     Common macros in the SDK
*
*  Find the Eir SDK at: https://osdn.net/projects/eirrepo/
*
*****************************************************************************/

#ifndef _COMMON_MACRO_UTILITIES_
#define _COMMON_MACRO_UTILITIES_

// Basic always inline definition.
#ifndef AINLINE
#ifdef _MSC_VER
#define AINLINE __forceinline
#elif defined(__GNUC__)
#ifdef _DEBUG
#define AINLINE inline
#else
#define AINLINE inline __attribute__((always_inline))
#endif
#else
#define AINLINE inline
#endif
#endif

#ifndef LAINLINE
#ifdef _MSC_VER
#define LAINLINE [[msvc::forceinline]]
#elif defined(__linux__)
#define LAINLINE __attribute__((always_inline))
#else
#define LAINLINE
#endif
#endif

#ifdef _MSC_VER
#define PTR_NO_ALIAS __restrict
#elif defined(__GNUC__)
#define PTR_NO_ALIAS __restrict__
#else
#define PTR_NO_ALIAS
#endif

#ifndef _MSC_VER
#define abstract
#endif

#ifndef countof
#define countof(x) (sizeof(x)/sizeof(*x))
#endif

// Compatibility macros for certain compilers.
#ifdef __GNUC__
#define _GCCCOMPAT_CONSTINIT_FUNCDECL_
#else
#define _GCCCOMPAT_CONSTINIT_FUNCDECL_ constinit
#endif //_GCC_

// Fix bugs of the broken MSVC compiler that annoys me because they cannot get C++20 concept evaluation implemented properly.
// Their lexer fails anyway!
#ifdef _MSC_VER
#define _MSVC_BUGFIX(x)
#else
#define _MSVC_BUGFIX(x) x
#endif

#endif //_COMMON_MACRO_UTILITIES_
