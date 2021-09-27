/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include <math.h>
#include <stddef.h>

#include "../inc/MarlinConfigPre.h"

class __FlashStringHelper;
typedef const __FlashStringHelper* FSTR_P;
#ifndef FPSTR
  #define FPSTR(S) (reinterpret_cast<FSTR_P>(S))
#endif
#define FTOP(S) (reinterpret_cast<const char*>(S))

//
// Conditional type assignment magic. For example...
//
// typename IF<(MYOPT==12), int, float>::type myvar;
//
template <bool, class L, class R>
struct IF { typedef R type; };
template <class L, class R>
struct IF<true, L, R> { typedef L type; };

#define LINEAR_AXIS_GANG(V...) GANG_N(LINEAR_AXES, V)
#define LINEAR_AXIS_CODE(V...) CODE_N(LINEAR_AXES, V)
#define LINEAR_AXIS_LIST(V...) LIST_N(LINEAR_AXES, V)
#define LINEAR_AXIS_ARRAY(V...) { LINEAR_AXIS_LIST(V) }
#define LINEAR_AXIS_ARGS(T...) LINEAR_AXIS_LIST(T x, T y, T z, T i, T j, T k)
#define LINEAR_AXIS_ELEM(O)    LINEAR_AXIS_LIST(O.x, O.y, O.z, O.i, O.j, O.k)
#define LINEAR_AXIS_DEFS(T,V)  LINEAR_AXIS_LIST(T x=V, T y=V, T z=V, T i=V, T j=V, T k=V)

#define LOGICAL_AXIS_GANG(E,V...) LINEAR_AXIS_GANG(V) GANG_ITEM_E(E)
#define LOGICAL_AXIS_CODE(E,V...) LINEAR_AXIS_CODE(V) CODE_ITEM_E(E)
#define LOGICAL_AXIS_LIST(E,V...) LINEAR_AXIS_LIST(V) LIST_ITEM_E(E)
#define LOGICAL_AXIS_ARRAY(E,V...) { LOGICAL_AXIS_LIST(E,V) }
#define LOGICAL_AXIS_ARGS(T...) LOGICAL_AXIS_LIST(T e, T x, T y, T z, T i, T j, T k)
#define LOGICAL_AXIS_ELEM(O)    LOGICAL_AXIS_LIST(O.e, O.x, O.y, O.z, O.i, O.j, O.k)
#define LOGICAL_AXIS_DECL(T,V)  LOGICAL_AXIS_LIST(T e=V, T x=V, T y=V, T z=V, T i=V, T j=V, T k=V)

#define LOGICAL_AXES_STRING LOGICAL_AXIS_GANG("E", "X", "Y", "Z", AXIS4_STR, AXIS5_STR, AXIS6_STR)

#if HAS_EXTRUDERS
  #define LIST_ITEM_E(N) , N
  #define CODE_ITEM_E(N) ; N
  #define GANG_ITEM_E(N) N
#else
  #define LIST_ITEM_E(N)
  #define CODE_ITEM_E(N)
  #define GANG_ITEM_E(N)
#endif

//
// Enumerated axis indices
//
//  - X_AXIS, Y_AXIS, and Z_AXIS should be used for axes in Cartesian space
//  - A_AXIS, B_AXIS, and C_AXIS should be used for Steppers, corresponding to XYZ on Cartesians
//  - X_HEAD, Y_HEAD, and Z_HEAD should be used for Steppers on Core kinematics
//
enum AxisEnum : uint8_t {

  // Linear axes may be controlled directly or indirectly
  LINEAR_AXIS_LIST(X_AXIS, Y_AXIS, Z_AXIS, I_AXIS, J_AXIS, K_AXIS)

  // Extruder axes may be considered distinctly
  #define _EN_ITEM(N) , E##N##_AXIS
  REPEAT(EXTRUDERS, _EN_ITEM)
  #undef _EN_ITEM

  // Core also keeps toolhead directions
  #if EITHER(IS_CORE, MARKFORGED_XY)
    , X_HEAD, Y_HEAD, Z_HEAD
  #endif

  // Distinct axes, including all E and Core
  , NUM_AXIS_ENUMS

  // Most of the time we refer only to the single E_AXIS
  #if HAS_EXTRUDERS
    , E_AXIS = E0_AXIS
  #endif

  // A, B, and C are for DELTA, SCARA, etc.
  , A_AXIS = X_AXIS
  #if LINEAR_AXES >= 2
    , B_AXIS = Y_AXIS
  #endif
  #if LINEAR_AXES >= 3
    , C_AXIS = Z_AXIS
  #endif

  // To refer to all or none
  , ALL_AXES_ENUM = 0xFE, NO_AXIS_ENUM = 0xFF
};

typedef IF<(NUM_AXIS_ENUMS > 8), uint16_t, uint8_t>::type axis_bits_t;

//
// Loop over axes
//
#define LOOP_ABC(VAR) LOOP_S_LE_N(VAR, A_AXIS, C_AXIS)
#define LOOP_LINEAR_AXES(VAR) LOOP_S_L_N(VAR, X_AXIS, LINEAR_AXES)
#define LOOP_LOGICAL_AXES(VAR) LOOP_S_L_N(VAR, X_AXIS, LOGICAL_AXES)
#define LOOP_DISTINCT_AXES(VAR) LOOP_S_L_N(VAR, X_AXIS, DISTINCT_AXES)

//
// feedRate_t is just a humble float
//
typedef float feedRate_t;

//
// celsius_t is the native unit of temperature. Signed to handle a disconnected thermistor value (-14).
// For more resolition (e.g., for a chocolate printer) this may later be changed to Celsius x 100
//
typedef int16_t celsius_t;
typedef float celsius_float_t;

//
// On AVR pointers are only 2 bytes so use 'const float &' for 'const float'
//
#ifdef __AVR__
  typedef const float & const_float_t;
#else
  typedef const float const_float_t;
#endif
typedef const_float_t const_feedRate_t;
typedef const_float_t const_celsius_float_t;

// Conversion macros
#define MMM_TO_MMS(MM_M) feedRate_t(static_cast<float>(MM_M) / 60.0f)
#define MMS_TO_MMM(MM_S) (static_cast<float>(MM_S) * 60.0f)

//
// Coordinates structures for XY, XYZ, XYZE...
//

// Helpers
#define _RECIP(N) ((N) ? 1.0f / static_cast<float>(N) : 0.0f)
#define _ABS(N) ((N) < 0 ? -(N) : (N))
#define _LS(N)  (N = (T)(uint32_t(N) << v))
#define _RS(N)  (N = (T)(uint32_t(N) >> v))
#define FI FORCE_INLINE

// Forward declarations
template<typename T> struct XYval;
template<typename T> struct XYZval;
template<typename T> struct XYZEval;

typedef struct XYval<bool>          xy_bool_t;
typedef struct XYZval<bool>        xyz_bool_t;
typedef struct XYZEval<bool>      xyze_bool_t;

typedef struct XYval<char>          xy_char_t;
typedef struct XYZval<char>        xyz_char_t;
typedef struct XYZEval<char>      xyze_char_t;

typedef struct XYval<unsigned char>     xy_uchar_t;
typedef struct XYZval<unsigned char>   xyz_uchar_t;
typedef struct XYZEval<unsigned char> xyze_uchar_t;

typedef struct XYval<int8_t>        xy_int8_t;
typedef struct XYZval<int8_t>      xyz_int8_t;
typedef struct XYZEval<int8_t>    xyze_int8_t;

typedef struct XYval<uint8_t>      xy_uint8_t;
typedef struct XYZval<uint8_t>    xyz_uint8_t;
typedef struct XYZEval<uint8_t>  xyze_uint8_t;

typedef struct XYval<int16_t>        xy_int_t;
typedef struct XYZval<int16_t>      xyz_int_t;
typedef struct XYZEval<int16_t>    xyze_int_t;

typedef struct XYval<uint16_t>      xy_uint_t;
typedef struct XYZval<uint16_t>    xyz_uint_t;
typedef struct XYZEval<uint16_t>  xyze_uint_t;

typedef struct XYval<int32_t>       xy_long_t;
typedef struct XYZval<int32_t>     xyz_long_t;
typedef struct XYZEval<int32_t>   xyze_long_t;

typedef struct XYval<uint32_t>     xy_ulong_t;
typedef struct XYZval<uint32_t>   xyz_ulong_t;
typedef struct XYZEval<uint32_t> xyze_ulong_t;

typedef struct XYZval<volatile int32_t>   xyz_vlong_t;
typedef struct XYZEval<volatile int32_t> xyze_vlong_t;

typedef struct XYval<float>        xy_float_t;
typedef struct XYZval<float>      xyz_float_t;
typedef struct XYZEval<float>    xyze_float_t;

typedef struct XYval<feedRate_t>     xy_feedrate_t;
typedef struct XYZval<feedRate_t>   xyz_feedrate_t;
typedef struct XYZEval<feedRate_t> xyze_feedrate_t;

typedef xy_uint8_t xy_byte_t;
typedef xyz_uint8_t xyz_byte_t;
typedef xyze_uint8_t xyze_byte_t;

typedef xyz_long_t abc_long_t;
typedef xyze_long_t abce_long_t;
typedef xyz_ulong_t abc_ulong_t;
typedef xyze_ulong_t abce_ulong_t;

typedef xy_float_t xy_pos_t;
typedef xyz_float_t xyz_pos_t;
typedef xyze_float_t xyze_pos_t;

typedef xy_float_t ab_float_t;
typedef xyz_float_t abc_float_t;
typedef xyze_float_t abce_float_t;

typedef ab_float_t ab_pos_t;
typedef abc_float_t abc_pos_t;
typedef abce_float_t abce_pos_t;

// External conversion methods
void toLogical(xy_pos_t &raw);
void toLogical(xyz_pos_t &raw);
void toLogical(xyze_pos_t &raw);
void toNative(xy_pos_t &raw);
void toNative(xyz_pos_t &raw);
void toNative(xyze_pos_t &raw);

//
// Paired XY coordinates, counters, flags, etc.
//
template<typename T>
struct XYval {
  union {
    struct { T x, y; };
    struct { T a, b; };
    T pos[2];
  };

  // Set all to 0
  FI void reset()                                       { x = y = 0; }

  // Setters taking struct types and arrays
  FI void set(const T px)                               { x = px; }
  #if HAS_Y_AXIS
    FI void set(const T px, const T py)                 { x = px; y = py; }
    FI void set(const T (&arr)[XY])                     { x = arr[0]; y = arr[1]; }
  #endif
  #if LINEAR_AXES > XY
    FI void set(const T (&arr)[LINEAR_AXES])            { x = arr[0]; y = arr[1]; }
  #endif
  #if LOGICAL_AXES > LINEAR_AXES
    FI void set(const T (&arr)[LOGICAL_AXES])           { x = arr[0]; y = arr[1]; }
    #if DISTINCT_AXES > LOGICAL_AXES
      FI void set(const T (&arr)[DISTINCT_AXES])        { x = arr[0]; y = arr[1]; }
    #endif
  #endif

  // Length reduced to one dimension
  FI T magnitude()                                const { return (T)sqrtf(x*x + y*y); }
  // Pointer to the data as a simple array
  FI operator T* ()                                     { return pos; }
  // If any element is true then it's true
  FI operator bool()                                    { return x || y; }

  // Explicit copy and copies with conversion
  FI XYval<T>           copy()                    const { return *this; }
  FI XYval<T>            ABS()                    const { return { T(_ABS(x)), T(_ABS(y)) }; }
  FI XYval<int16_t>    asInt()                          { return { int16_t(x), int16_t(y) }; }
  FI XYval<int16_t>    asInt()                    const { return { int16_t(x), int16_t(y) }; }
  FI XYval<int32_t>   asLong()                          { return { int32_t(x), int32_t(y) }; }
  FI XYval<int32_t>   asLong()                    const { return { int32_t(x), int32_t(y) }; }
  FI XYval<int32_t>   ROUNDL()                          { return { int32_t(LROUND(x)), int32_t(LROUND(y)) }; }
  FI XYval<int32_t>   ROUNDL()                    const { return { int32_t(LROUND(x)), int32_t(LROUND(y)) }; }
  FI XYval<float>    asFloat()                          { return { static_cast<float>(x), static_cast<float>(y) }; }
  FI XYval<float>    asFloat()                    const { return { static_cast<float>(x), static_cast<float>(y) }; }
  FI XYval<float> reciprocal()                    const { return {  _RECIP(x),  _RECIP(y) }; }

  // Marlin workspace shifting is done with G92 and M206
  FI XYval<float>  asLogical()                    const { XYval<float> o = asFloat(); toLogical(o); return o; }
  FI XYval<float>   asNative()                    const { XYval<float> o = asFloat(); toNative(o);  return o; }

  // Cast to a type with more fields by making a new object
  FI operator XYZval<T>()                               { return { x, y }; }
  FI operator XYZval<T>()                         const { return { x, y }; }
  FI operator XYZEval<T>()                              { return { x, y }; }
  FI operator XYZEval<T>()                        const { return { x, y }; }

  // Accessor via an AxisEnum (or any integer) [index]
  FI       T&  operator[](const int n)                  { return pos[n]; }
  FI const T&  operator[](const int n)            const { return pos[n]; }

  // Assignment operator overrides do the expected thing
  FI XYval<T>& operator= (const T v)                    { set(v,    v   ); return *this; }
  FI XYval<T>& operator= (const XYZval<T>  &rs)         { set(rs.x, rs.y); return *this; }
  FI XYval<T>& operator= (const XYZEval<T> &rs)         { set(rs.x, rs.y); return *this; }

  // Override other operators to get intuitive behaviors
  FI XYval<T>  operator+ (const XYval<T>   &rs)   const { XYval<T> ls = *this; ls.x += rs.x; ls.y += rs.y; return ls; }
  FI XYval<T>  operator+ (const XYval<T>   &rs)         { XYval<T> ls = *this; ls.x += rs.x; ls.y += rs.y; return ls; }
  FI XYval<T>  operator- (const XYval<T>   &rs)   const { XYval<T> ls = *this; ls.x -= rs.x; ls.y -= rs.y; return ls; }
  FI XYval<T>  operator- (const XYval<T>   &rs)         { XYval<T> ls = *this; ls.x -= rs.x; ls.y -= rs.y; return ls; }
  FI XYval<T>  operator* (const XYval<T>   &rs)   const { XYval<T> ls = *this; ls.x *= rs.x; ls.y *= rs.y; return ls; }
  FI XYval<T>  operator* (const XYval<T>   &rs)         { XYval<T> ls = *this; ls.x *= rs.x; ls.y *= rs.y; return ls; }
  FI XYval<T>  operator/ (const XYval<T>   &rs)   const { XYval<T> ls = *this; ls.x /= rs.x; ls.y /= rs.y; return ls; }
  FI XYval<T>  operator/ (const XYval<T>   &rs)         { XYval<T> ls = *this; ls.x /= rs.x; ls.y /= rs.y; return ls; }
  FI XYval<T>  operator+ (const XYZval<T>  &rs)   const { XYval<T> ls = *this; ls.x += rs.x; ls.y += rs.y; return ls; }
  FI XYval<T>  operator+ (const XYZval<T>  &rs)         { XYval<T> ls = *this; ls.x += rs.x; ls.y += rs.y; return ls; }
  FI XYval<T>  operator- (const XYZval<T>  &rs)   const { XYval<T> ls = *this; ls.x -= rs.x; ls.y -= rs.y; return ls; }
  FI XYval<T>  operator- (const XYZval<T>  &rs)         { XYval<T> ls = *this; ls.x -= rs.x; ls.y -= rs.y; return ls; }
  FI XYval<T>  operator* (const XYZval<T>  &rs)   const { XYval<T> ls = *this; ls.x *= rs.x; ls.y *= rs.y; return ls; }
  FI XYval<T>  operator* (const XYZval<T>  &rs)         { XYval<T> ls = *this; ls.x *= rs.x; ls.y *= rs.y; return ls; }
  FI XYval<T>  operator/ (const XYZval<T>  &rs)   const { XYval<T> ls = *this; ls.x /= rs.x; ls.y /= rs.y; return ls; }
  FI XYval<T>  operator/ (const XYZval<T>  &rs)         { XYval<T> ls = *this; ls.x /= rs.x; ls.y /= rs.y; return ls; }
  FI XYval<T>  operator+ (const XYZEval<T> &rs)   const { XYval<T> ls = *this; ls.x += rs.x; ls.y += rs.y; return ls; }
  FI XYval<T>  operator+ (const XYZEval<T> &rs)         { XYval<T> ls = *this; ls.x += rs.x; ls.y += rs.y; return ls; }
  FI XYval<T>  operator- (const XYZEval<T> &rs)   const { XYval<T> ls = *this; ls.x -= rs.x; ls.y -= rs.y; return ls; }
  FI XYval<T>  operator- (const XYZEval<T> &rs)         { XYval<T> ls = *this; ls.x -= rs.x; ls.y -= rs.y; return ls; }
  FI XYval<T>  operator* (const XYZEval<T> &rs)   const { XYval<T> ls = *this; ls.x *= rs.x; ls.y *= rs.y; return ls; }
  FI XYval<T>  operator* (const XYZEval<T> &rs)         { XYval<T> ls = *this; ls.x *= rs.x; ls.y *= rs.y; return ls; }
  FI XYval<T>  operator/ (const XYZEval<T> &rs)   const { XYval<T> ls = *this; ls.x /= rs.x; ls.y /= rs.y; return ls; }
  FI XYval<T>  operator/ (const XYZEval<T> &rs)         { XYval<T> ls = *this; ls.x /= rs.x; ls.y /= rs.y; return ls; }
  FI XYval<T>  operator* (const float &v)         const { XYval<T> ls = *this; ls.x *= v;    ls.y *= v;    return ls; }
  FI XYval<T>  operator* (const float &v)               { XYval<T> ls = *this; ls.x *= v;    ls.y *= v;    return ls; }
  FI XYval<T>  operator* (const int &v)           const { XYval<T> ls = *this; ls.x *= v;    ls.y *= v;    return ls; }
  FI XYval<T>  operator* (const int &v)                 { XYval<T> ls = *this; ls.x *= v;    ls.y *= v;    return ls; }
  FI XYval<T>  operator/ (const float &v)         const { XYval<T> ls = *this; ls.x /= v;    ls.y /= v;    return ls; }
  FI XYval<T>  operator/ (const float &v)               { XYval<T> ls = *this; ls.x /= v;    ls.y /= v;    return ls; }
  FI XYval<T>  operator/ (const int &v)           const { XYval<T> ls = *this; ls.x /= v;    ls.y /= v;    return ls; }
  FI XYval<T>  operator/ (const int &v)                 { XYval<T> ls = *this; ls.x /= v;    ls.y /= v;    return ls; }
  FI XYval<T>  operator>>(const int &v)           const { XYval<T> ls = *this; _RS(ls.x);    _RS(ls.y);    return ls; }
  FI XYval<T>  operator>>(const int &v)                 { XYval<T> ls = *this; _RS(ls.x);    _RS(ls.y);    return ls; }
  FI XYval<T>  operator<<(const int &v)           const { XYval<T> ls = *this; _LS(ls.x);    _LS(ls.y);    return ls; }
  FI XYval<T>  operator<<(const int &v)                 { XYval<T> ls = *this; _LS(ls.x);    _LS(ls.y);    return ls; }
  FI const XYval<T> operator-()                   const { XYval<T> o = *this; o.x = -x; o.y = -y; return o; }
  FI XYval<T>       operator-()                         { XYval<T> o = *this; o.x = -x; o.y = -y; return o; }

  // Modifier operators
  FI XYval<T>& operator+=(const XYval<T>   &rs)         { x += rs.x; y += rs.y; return *this; }
  FI XYval<T>& operator-=(const XYval<T>   &rs)         { x -= rs.x; y -= rs.y; return *this; }
  FI XYval<T>& operator*=(const XYval<T>   &rs)         { x *= rs.x; y *= rs.y; return *this; }
  FI XYval<T>& operator+=(const XYZval<T>  &rs)         { x += rs.x; y += rs.y; return *this; }
  FI XYval<T>& operator-=(const XYZval<T>  &rs)         { x -= rs.x; y -= rs.y; return *this; }
  FI XYval<T>& operator*=(const XYZval<T>  &rs)         { x *= rs.x; y *= rs.y; return *this; }
  FI XYval<T>& operator+=(const XYZEval<T> &rs)         { x += rs.x; y += rs.y; return *this; }
  FI XYval<T>& operator-=(const XYZEval<T> &rs)         { x -= rs.x; y -= rs.y; return *this; }
  FI XYval<T>& operator*=(const XYZEval<T> &rs)         { x *= rs.x; y *= rs.y; return *this; }
  FI XYval<T>& operator*=(const float &v)               { x *= v;    y *= v;    return *this; }
  FI XYval<T>& operator*=(const int &v)                 { x *= v;    y *= v;    return *this; }
  FI XYval<T>& operator>>=(const int &v)                { _RS(x);    _RS(y);    return *this; }
  FI XYval<T>& operator<<=(const int &v)                { _LS(x);    _LS(y);    return *this; }

  // Exact comparisons. For floats a "NEAR" operation may be better.
  FI bool      operator==(const XYval<T>   &rs)         { return x == rs.x && y == rs.y; }
  FI bool      operator==(const XYZval<T>  &rs)         { return x == rs.x && y == rs.y; }
  FI bool      operator==(const XYZEval<T> &rs)         { return x == rs.x && y == rs.y; }
  FI bool      operator==(const XYval<T>   &rs)   const { return x == rs.x && y == rs.y; }
  FI bool      operator==(const XYZval<T>  &rs)   const { return x == rs.x && y == rs.y; }
  FI bool      operator==(const XYZEval<T> &rs)   const { return x == rs.x && y == rs.y; }
  FI bool      operator!=(const XYval<T>   &rs)         { return !operator==(rs); }
  FI bool      operator!=(const XYZval<T>  &rs)         { return !operator==(rs); }
  FI bool      operator!=(const XYZEval<T> &rs)         { return !operator==(rs); }
  FI bool      operator!=(const XYval<T>   &rs)   const { return !operator==(rs); }
  FI bool      operator!=(const XYZval<T>  &rs)   const { return !operator==(rs); }
  FI bool      operator!=(const XYZEval<T> &rs)   const { return !operator==(rs); }
};

//
// Linear Axes coordinates, counters, flags, etc.
//
template<typename T>
struct XYZval {
  union {
    struct { T LINEAR_AXIS_ARGS(); };
    struct { T LINEAR_AXIS_LIST(a, b, c, u, v, w); };
    T pos[LINEAR_AXES];
  };

  // Set all to 0
  FI void reset()                                      { LINEAR_AXIS_GANG(x =, y =, z =, i =, j =, k =) 0; }

  // Setters taking struct types and arrays
  FI void set(const T px)                              { x = px; }
  FI void set(const T px, const T py)                  { x = px; y = py; }
  FI void set(const XYval<T> pxy)                      { x = pxy.x; y = pxy.y; }
  FI void set(const XYval<T> pxy, const T pz)          { LINEAR_AXIS_CODE(x = pxy.x, y = pxy.y, z = pz, NOOP, NOOP, NOOP); }
  FI void set(const T (&arr)[XY])                      { x = arr[0]; y = arr[1]; }
  #if HAS_Z_AXIS
    FI void set(const T (&arr)[LINEAR_AXES])           { LINEAR_AXIS_CODE(x = arr[0], y = arr[1], z = arr[2], i = arr[3], j = arr[4], k = arr[5]); }
    FI void set(LINEAR_AXIS_ARGS(const T))             { LINEAR_AXIS_CODE(a = x,      b = y,      c = z,      u = i,      v = j,      w = k    ); }
  #endif
  #if LOGICAL_AXES > LINEAR_AXES
    FI void set(const T (&arr)[LOGICAL_AXES])          { LINEAR_AXIS_CODE(x = arr[0], y = arr[1], z = arr[2], i = arr[3], j = arr[4], k = arr[5]); }
    FI void set(LOGICAL_AXIS_ARGS(const T))            { LINEAR_AXIS_CODE(a = x,      b = y,      c = z,      u = i,      v = j,      w = k    ); }
    #if DISTINCT_AXES > LOGICAL_AXES
      FI void set(const T (&arr)[DISTINCT_AXES])       { LINEAR_AXIS_CODE(x = arr[0], y = arr[1], z = arr[2], i = arr[3], j = arr[4], k = arr[5]); }
    #endif
  #endif
  #if LINEAR_AXES >= 4
    FI void set(const T px, const T py, const T pz)                         { x = px; y = py; z = pz; }
  #endif
  #if LINEAR_AXES >= 5
    FI void set(const T px, const T py, const T pz, const T pi)             { x = px; y = py; z = pz; i = pi; }
  #endif
  #if LINEAR_AXES >= 6
    FI void set(const T px, const T py, const T pz, const T pi, const T pj) { x = px; y = py; z = pz; i = pi; j = pj; }
  #endif

  // Length reduced to one dimension
  FI T magnitude()                               const { return (T)sqrtf(LINEAR_AXIS_GANG(x*x, + y*y, + z*z, + i*i, + j*j, + k*k)); }
  // Pointer to the data as a simple array
  FI operator T* ()                                    { return pos; }
  // If any element is true then it's true
  FI operator bool()                                   { return LINEAR_AXIS_GANG(x, || y, || z, || i, || j, || k); }

  // Explicit copy and copies with conversion
  FI XYZval<T>          copy()                   const { XYZval<T> o = *this; return o; }
  FI XYZval<T>           ABS()                   const { return LINEAR_AXIS_ARRAY(T(_ABS(x)), T(_ABS(y)), T(_ABS(z)), T(_ABS(i)), T(_ABS(j)), T(_ABS(k))); }
  FI XYZval<int16_t>   asInt()                         { return LINEAR_AXIS_ARRAY(int16_t(x), int16_t(y), int16_t(z), int16_t(i), int16_t(j), int16_t(k)); }
  FI XYZval<int16_t>   asInt()                   const { return LINEAR_AXIS_ARRAY(int16_t(x), int16_t(y), int16_t(z), int16_t(i), int16_t(j), int16_t(k)); }
  FI XYZval<int32_t>  asLong()                         { return LINEAR_AXIS_ARRAY(int32_t(x), int32_t(y), int32_t(z), int32_t(i), int32_t(j), int32_t(k)); }
  FI XYZval<int32_t>  asLong()                   const { return LINEAR_AXIS_ARRAY(int32_t(x), int32_t(y), int32_t(z), int32_t(i), int32_t(j), int32_t(k)); }
  FI XYZval<int32_t>  ROUNDL()                         { return LINEAR_AXIS_ARRAY(int32_t(LROUND(x)), int32_t(LROUND(y)), int32_t(LROUND(z)), int32_t(LROUND(i)), int32_t(LROUND(j)), int32_t(LROUND(k))); }
  FI XYZval<int32_t>  ROUNDL()                   const { return LINEAR_AXIS_ARRAY(int32_t(LROUND(x)), int32_t(LROUND(y)), int32_t(LROUND(z)), int32_t(LROUND(i)), int32_t(LROUND(j)), int32_t(LROUND(k))); }
  FI XYZval<float>   asFloat()                         { return LINEAR_AXIS_ARRAY(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), static_cast<float>(i), static_cast<float>(j), static_cast<float>(k)); }
  FI XYZval<float>   asFloat()                   const { return LINEAR_AXIS_ARRAY(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), static_cast<float>(i), static_cast<float>(j), static_cast<float>(k)); }
  FI XYZval<float> reciprocal()                  const { return LINEAR_AXIS_ARRAY(_RECIP(x),  _RECIP(y),  _RECIP(z),  _RECIP(i),  _RECIP(j),  _RECIP(k)); }

  // Marlin workspace shifting is done with G92 and M206
  FI XYZval<float> asLogical()                   const { XYZval<float> o = asFloat(); toLogical(o); return o; }
  FI XYZval<float>  asNative()                   const { XYZval<float> o = asFloat(); toNative(o);  return o; }

  // In-place cast to types having fewer fields
  FI operator       XYval<T>&()                        { return *(XYval<T>*)this; }
  FI operator const XYval<T>&()                  const { return *(const XYval<T>*)this; }

  // Cast to a type with more fields by making a new object
  FI operator       XYZEval<T>()                 const { return LINEAR_AXIS_ARRAY(x, y, z, i, j, k); }

  // Accessor via an AxisEnum (or any integer) [index]
  FI       T&   operator[](const int n)                { return pos[n]; }
  FI const T&   operator[](const int n)          const { return pos[n]; }

  // Assignment operator overrides do the expected thing
  FI XYZval<T>& operator= (const T v)                  { set(ARRAY_N_1(LINEAR_AXES, v)); return *this; }
  FI XYZval<T>& operator= (const XYval<T>   &rs)       { set(rs.x, rs.y      ); return *this; }
  FI XYZval<T>& operator= (const XYZEval<T> &rs)       { set(LINEAR_AXIS_ELEM(rs)); return *this; }

  // Override other operators to get intuitive behaviors
  FI XYZval<T>  operator+ (const XYval<T>   &rs) const { XYZval<T> ls = *this; LINEAR_AXIS_CODE(ls.x += rs.x, ls.y += rs.y, NOOP        , NOOP        , NOOP        , NOOP        ); return ls; }
  FI XYZval<T>  operator+ (const XYval<T>   &rs)       { XYZval<T> ls = *this; LINEAR_AXIS_CODE(ls.x += rs.x, ls.y += rs.y, NOOP        , NOOP        , NOOP        , NOOP        ); return ls; }
  FI XYZval<T>  operator- (const XYval<T>   &rs) const { XYZval<T> ls = *this; LINEAR_AXIS_CODE(ls.x -= rs.x, ls.y -= rs.y, NOOP        , NOOP        , NOOP        , NOOP        ); return ls; }
  FI XYZval<T>  operator- (const XYval<T>   &rs)       { XYZval<T> ls = *this; LINEAR_AXIS_CODE(ls.x -= rs.x, ls.y -= rs.y, NOOP        , NOOP        , NOOP        , NOOP        ); return ls; }
  FI XYZval<T>  operator* (const XYval<T>   &rs) const { XYZval<T> ls = *this; LINEAR_AXIS_CODE(ls.x *= rs.x, ls.y *= rs.y, NOOP        , NOOP        , NOOP        , NOOP        ); return ls; }
  FI XYZval<T>  operator* (const XYval<T>   &rs)       { XYZval<T> ls = *this; LINEAR_AXIS_CODE(ls.x *= rs.x, ls.y *= rs.y, NOOP        , NOOP        , NOOP        , NOOP        ); return ls; }
  FI XYZval<T>  operator/ (const XYval<T>   &rs) const { XYZval<T> ls = *this; LINEAR_AXIS_CODE(ls.x /= rs.x, ls.y /= rs.y, NOOP        , NOOP        , NOOP        , NOOP        ); return ls; }
  FI XYZval<T>  operator/ (const XYval<T>   &rs)       { XYZval<T> ls = *this; LINEAR_AXIS_CODE(ls.x /= rs.x, ls.y /= rs.y, NOOP        , NOOP        , NOOP        , NOOP        ); return ls; }
  FI XYZval<T>  operator+ (const XYZval<T>  &rs) const { XYZval<T> ls = *this; LINEAR_AXIS_CODE(ls.x += rs.x, ls.y += rs.y, ls.z += rs.z, ls.i += rs.i, ls.j += rs.j, ls.k += rs.k); return ls; }
  FI XYZval<T>  operator+ (const XYZval<T>  &rs)       { XYZval<T> ls = *this; LINEAR_AXIS_CODE(ls.x += rs.x, ls.y += rs.y, ls.z += rs.z, ls.i += rs.i, ls.j += rs.j, ls.k += rs.k); return ls; }
  FI XYZval<T>  operator- (const XYZval<T>  &rs) const { XYZval<T> ls = *this; LINEAR_AXIS_CODE(ls.x -= rs.x, ls.y -= rs.y, ls.z -= rs.z, ls.i -= rs.i, ls.j -= rs.j, ls.k -= rs.k); return ls; }
  FI XYZval<T>  operator- (const XYZval<T>  &rs)       { XYZval<T> ls = *this; LINEAR_AXIS_CODE(ls.x -= rs.x, ls.y -= rs.y, ls.z -= rs.z, ls.i -= rs.i, ls.j -= rs.j, ls.k -= rs.k); return ls; }
  FI XYZval<T>  operator* (const XYZval<T>  &rs) const { XYZval<T> ls = *this; LINEAR_AXIS_CODE(ls.x *= rs.x, ls.y *= rs.y, ls.z *= rs.z, ls.i *= rs.i, ls.j *= rs.j, ls.k *= rs.k); return ls; }
  FI XYZval<T>  operator* (const XYZval<T>  &rs)       { XYZval<T> ls = *this; LINEAR_AXIS_CODE(ls.x *= rs.x, ls.y *= rs.y, ls.z *= rs.z, ls.i *= rs.i, ls.j *= rs.j, ls.k *= rs.k); return ls; }
  FI XYZval<T>  operator/ (const XYZval<T>  &rs) const { XYZval<T> ls = *this; LINEAR_AXIS_CODE(ls.x /= rs.x, ls.y /= rs.y, ls.z /= rs.z, ls.i /= rs.i, ls.j /= rs.j, ls.k /= rs.k); return ls; }
  FI XYZval<T>  operator/ (const XYZval<T>  &rs)       { XYZval<T> ls = *this; LINEAR_AXIS_CODE(ls.x /= rs.x, ls.y /= rs.y, ls.z /= rs.z, ls.i /= rs.i, ls.j /= rs.j, ls.k /= rs.k); return ls; }
  FI XYZval<T>  operator+ (const XYZEval<T> &rs) const { XYZval<T> ls = *this; LINEAR_AXIS_CODE(ls.x += rs.x, ls.y += rs.y, ls.z += rs.z, ls.i += rs.i, ls.j += rs.j, ls.k += rs.k); return ls; }
  FI XYZval<T>  operator+ (const XYZEval<T> &rs)       { XYZval<T> ls = *this; LINEAR_AXIS_CODE(ls.x += rs.x, ls.y += rs.y, ls.z += rs.z, ls.i += rs.i, ls.j += rs.j, ls.k += rs.k); return ls; }
  FI XYZval<T>  operator- (const XYZEval<T> &rs) const { XYZval<T> ls = *this; LINEAR_AXIS_CODE(ls.x -= rs.x, ls.y -= rs.y, ls.z -= rs.z, ls.i -= rs.i, ls.j -= rs.j, ls.k -= rs.k); return ls; }
  FI XYZval<T>  operator- (const XYZEval<T> &rs)       { XYZval<T> ls = *this; LINEAR_AXIS_CODE(ls.x -= rs.x, ls.y -= rs.y, ls.z -= rs.z, ls.i -= rs.i, ls.j -= rs.j, ls.k -= rs.k); return ls; }
  FI XYZval<T>  operator* (const XYZEval<T> &rs) const { XYZval<T> ls = *this; LINEAR_AXIS_CODE(ls.x *= rs.x, ls.y *= rs.y, ls.z *= rs.z, ls.i *= rs.i, ls.j *= rs.j, ls.k *= rs.k); return ls; }
  FI XYZval<T>  operator* (const XYZEval<T> &rs)       { XYZval<T> ls = *this; LINEAR_AXIS_CODE(ls.x *= rs.x, ls.y *= rs.y, ls.z *= rs.z, ls.i *= rs.i, ls.j *= rs.j, ls.k *= rs.k); return ls; }
  FI XYZval<T>  operator/ (const XYZEval<T> &rs) const { XYZval<T> ls = *this; LINEAR_AXIS_CODE(ls.x /= rs.x, ls.y /= rs.y, ls.z /= rs.z, ls.i /= rs.i, ls.j /= rs.j, ls.k /= rs.k); return ls; }
  FI XYZval<T>  operator/ (const XYZEval<T> &rs)       { XYZval<T> ls = *this; LINEAR_AXIS_CODE(ls.x /= rs.x, ls.y /= rs.y, ls.z /= rs.z, ls.i /= rs.i, ls.j /= rs.j, ls.k /= rs.k); return ls; }
  FI XYZval<T>  operator* (const float &v)       const { XYZval<T> ls = *this; LINEAR_AXIS_CODE(ls.x *= v,    ls.y *= v,    ls.z *= v,    ls.i *= v,    ls.j *= v,    ls.k *= v   ); return ls; }
  FI XYZval<T>  operator* (const float &v)             { XYZval<T> ls = *this; LINEAR_AXIS_CODE(ls.x *= v,    ls.y *= v,    ls.z *= v,    ls.i *= v,    ls.j *= v,    ls.k *= v   ); return ls; }
  FI XYZval<T>  operator* (const int &v)         const { XYZval<T> ls = *this; LINEAR_AXIS_CODE(ls.x *= v,    ls.y *= v,    ls.z *= v,    ls.i *= v,    ls.j *= v,    ls.k *= v   ); return ls; }
  FI XYZval<T>  operator* (const int &v)               { XYZval<T> ls = *this; LINEAR_AXIS_CODE(ls.x *= v,    ls.y *= v,    ls.z *= v,    ls.i *= v,    ls.j *= v,    ls.k *= v   ); return ls; }
  FI XYZval<T>  operator/ (const float &v)       const { XYZval<T> ls = *this; LINEAR_AXIS_CODE(ls.x /= v,    ls.y /= v,    ls.z /= v,    ls.i /= v,    ls.j /= v,    ls.k /= v   ); return ls; }
  FI XYZval<T>  operator/ (const float &v)             { XYZval<T> ls = *this; LINEAR_AXIS_CODE(ls.x /= v,    ls.y /= v,    ls.z /= v,    ls.i /= v,    ls.j /= v,    ls.k /= v   ); return ls; }
  FI XYZval<T>  operator/ (const int &v)         const { XYZval<T> ls = *this; LINEAR_AXIS_CODE(ls.x /= v,    ls.y /= v,    ls.z /= v,    ls.i /= v,    ls.j /= v,    ls.k /= v   ); return ls; }
  FI XYZval<T>  operator/ (const int &v)               { XYZval<T> ls = *this; LINEAR_AXIS_CODE(ls.x /= v,    ls.y /= v,    ls.z /= v,    ls.i /= v,    ls.j /= v,    ls.k /= v   ); return ls; }
  FI XYZval<T>  operator>>(const int &v)         const { XYZval<T> ls = *this; LINEAR_AXIS_CODE(_RS(ls.x),    _RS(ls.y),    _RS(ls.z),    _RS(ls.i),    _RS(ls.j),    _RS(ls.k)   ); return ls; }
  FI XYZval<T>  operator>>(const int &v)               { XYZval<T> ls = *this; LINEAR_AXIS_CODE(_RS(ls.x),    _RS(ls.y),    _RS(ls.z),    _RS(ls.i),    _RS(ls.j),    _RS(ls.k)   ); return ls; }
  FI XYZval<T>  operator<<(const int &v)         const { XYZval<T> ls = *this; LINEAR_AXIS_CODE(_LS(ls.x),    _LS(ls.y),    _LS(ls.z),    _LS(ls.i),    _LS(ls.j),    _LS(ls.k)   ); return ls; }
  FI XYZval<T>  operator<<(const int &v)               { XYZval<T> ls = *this; LINEAR_AXIS_CODE(_LS(ls.x),    _LS(ls.y),    _LS(ls.z),    _LS(ls.i),    _LS(ls.j),    _LS(ls.k)   ); return ls; }
  FI const XYZval<T> operator-()                 const { XYZval<T> o = *this; LINEAR_AXIS_CODE(o.x = -x, o.y = -y, o.z = -z, o.i = -i, o.j = -j, o.k = -k); return o; }
  FI XYZval<T>       operator-()                       { XYZval<T> o = *this; LINEAR_AXIS_CODE(o.x = -x, o.y = -y, o.z = -z, o.i = -i, o.j = -j, o.k = -k); return o; }

  // Modifier operators
  FI XYZval<T>& operator+=(const XYval<T>   &rs)       { LINEAR_AXIS_CODE(x += rs.x, y += rs.y, NOOP,      NOOP,      NOOP,      NOOP     ); return *this; }
  FI XYZval<T>& operator-=(const XYval<T>   &rs)       { LINEAR_AXIS_CODE(x -= rs.x, y -= rs.y, NOOP,      NOOP,      NOOP,      NOOP     ); return *this; }
  FI XYZval<T>& operator*=(const XYval<T>   &rs)       { LINEAR_AXIS_CODE(x *= rs.x, y *= rs.y, NOOP,      NOOP,      NOOP,      NOOP     ); return *this; }
  FI XYZval<T>& operator/=(const XYval<T>   &rs)       { LINEAR_AXIS_CODE(x /= rs.x, y /= rs.y, NOOP,      NOOP,      NOOP,      NOOP     ); return *this; }
  FI XYZval<T>& operator+=(const XYZval<T>  &rs)       { LINEAR_AXIS_CODE(x += rs.x, y += rs.y, z += rs.z, i += rs.i, j += rs.j, k += rs.k); return *this; }
  FI XYZval<T>& operator-=(const XYZval<T>  &rs)       { LINEAR_AXIS_CODE(x -= rs.x, y -= rs.y, z -= rs.z, i -= rs.i, j -= rs.j, k -= rs.k); return *this; }
  FI XYZval<T>& operator*=(const XYZval<T>  &rs)       { LINEAR_AXIS_CODE(x *= rs.x, y *= rs.y, z *= rs.z, i *= rs.i, j *= rs.j, k *= rs.k); return *this; }
  FI XYZval<T>& operator/=(const XYZval<T>  &rs)       { LINEAR_AXIS_CODE(x /= rs.x, y /= rs.y, z /= rs.z, i /= rs.i, j /= rs.j, k /= rs.k); return *this; }
  FI XYZval<T>& operator+=(const XYZEval<T> &rs)       { LINEAR_AXIS_CODE(x += rs.x, y += rs.y, z += rs.z, i += rs.i, j += rs.j, k += rs.k); return *this; }
  FI XYZval<T>& operator-=(const XYZEval<T> &rs)       { LINEAR_AXIS_CODE(x -= rs.x, y -= rs.y, z -= rs.z, i -= rs.i, j -= rs.j, k -= rs.k); return *this; }
  FI XYZval<T>& operator*=(const XYZEval<T> &rs)       { LINEAR_AXIS_CODE(x *= rs.x, y *= rs.y, z *= rs.z, i *= rs.i, j *= rs.j, k *= rs.k); return *this; }
  FI XYZval<T>& operator/=(const XYZEval<T> &rs)       { LINEAR_AXIS_CODE(x /= rs.x, y /= rs.y, z /= rs.z, i /= rs.i, j /= rs.j, k /= rs.k); return *this; }
  FI XYZval<T>& operator*=(const float &v)             { LINEAR_AXIS_CODE(x *= v,    y *= v,    z *= v,    i *= v,    j *= v,    k *= v);    return *this; }
  FI XYZval<T>& operator*=(const int &v)               { LINEAR_AXIS_CODE(x *= v,    y *= v,    z *= v,    i *= v,    j *= v,    k *= v);    return *this; }
  FI XYZval<T>& operator>>=(const int &v)              { LINEAR_AXIS_CODE(_RS(x),    _RS(y),    _RS(z),    _RS(i),    _RS(j),    _RS(k));    return *this; }
  FI XYZval<T>& operator<<=(const int &v)              { LINEAR_AXIS_CODE(_LS(x),    _LS(y),    _LS(z),    _LS(i),    _LS(j),    _LS(k));    return *this; }

  // Exact comparisons. For floats a "NEAR" operation may be better.
  FI bool       operator==(const XYZEval<T> &rs)       { return true LINEAR_AXIS_GANG(&& x == rs.x, && y == rs.y, && z == rs.z, && i == rs.i, && j == rs.j, && k == rs.k); }
  FI bool       operator==(const XYZEval<T> &rs) const { return true LINEAR_AXIS_GANG(&& x == rs.x, && y == rs.y, && z == rs.z, && i == rs.i, && j == rs.j, && k == rs.k); }
  FI bool       operator!=(const XYZEval<T> &rs)       { return !operator==(rs); }
  FI bool       operator!=(const XYZEval<T> &rs) const { return !operator==(rs); }
};

//
// Logical Axes coordinates, counters, etc.
//
template<typename T>
struct XYZEval {
  union {
    struct { T LOGICAL_AXIS_ARGS(); };
    struct { T LOGICAL_AXIS_LIST(_e, a, b, c, u, v, w); };
    T pos[LOGICAL_AXES];
  };
  // Reset all to 0
  FI void reset()                     { LOGICAL_AXIS_GANG(e =, x =, y =, z =, i =, j =, k =) 0; }

  // Setters taking struct types and arrays
  FI void set(const T px)             { x = px;               }
  FI void set(const T px, const T py) { x = px;    y = py;    }
  FI void set(const XYval<T> pxy)     { x = pxy.x; y = pxy.y; }
  FI void set(const XYZval<T> pxyz)   { set(LINEAR_AXIS_ELEM(pxyz)); }
  #if HAS_Z_AXIS
    FI void set(LINEAR_AXIS_ARGS(const T))         { LINEAR_AXIS_CODE(a = x, b = y, c = z, u = i, v = j, w = k); }
  #endif
  #if LOGICAL_AXES > LINEAR_AXES
    FI void set(const XYval<T> pxy, const T pe)    { set(pxy); e = pe; }
    FI void set(const XYZval<T> pxyz, const T pe)  { set(pxyz); e = pe; }
    FI void set(LOGICAL_AXIS_ARGS(const T))        { LOGICAL_AXIS_CODE(_e = e, a = x, b = y, c = z, u = i, v = j, w = k); }
  #endif
  #if LINEAR_AXES >= 4
    FI void set(const T px, const T py, const T pz)                         { x = px; y = py; z = pz; }
  #endif
  #if LINEAR_AXES >= 5
    FI void set(const T px, const T py, const T pz, const T pi)             { x = px; y = py; z = pz; i = pi; }
  #endif
  #if LINEAR_AXES >= 6
    FI void set(const T px, const T py, const T pz, const T pi, const T pj) { x = px; y = py; z = pz; i = pi; j = pj; }
  #endif

  // Length reduced to one dimension
  FI T magnitude()                                 const { return (T)sqrtf(LOGICAL_AXIS_GANG(+ e*e, + x*x, + y*y, + z*z, + i*i, + j*j, + k*k)); }
  // Pointer to the data as a simple array
  FI operator T* ()                                      { return pos; }
  // If any element is true then it's true
  FI operator bool()                                     { return 0 LOGICAL_AXIS_GANG(|| e, || x, || y, || z, || i, || j, || k); }

  // Explicit copy and copies with conversion
  FI XYZEval<T>          copy()  const { XYZEval<T> o = *this; return o; }
  FI XYZEval<T>           ABS()  const { return LOGICAL_AXIS_ARRAY(T(_ABS(e)), T(_ABS(x)), T(_ABS(y)), T(_ABS(z)), T(_ABS(i)), T(_ABS(j)), T(_ABS(k))); }
  FI XYZEval<int16_t>   asInt()        { return LOGICAL_AXIS_ARRAY(int16_t(e), int16_t(x), int16_t(y), int16_t(z), int16_t(i), int16_t(j), int16_t(k)); }
  FI XYZEval<int16_t>   asInt()  const { return LOGICAL_AXIS_ARRAY(int16_t(e), int16_t(x), int16_t(y), int16_t(z), int16_t(i), int16_t(j), int16_t(k)); }
  FI XYZEval<int32_t>  asLong()        { return LOGICAL_AXIS_ARRAY(int32_t(e), int32_t(x), int32_t(y), int32_t(z), int32_t(i), int32_t(j), int32_t(k)); }
  FI XYZEval<int32_t>  asLong()  const { return LOGICAL_AXIS_ARRAY(int32_t(e), int32_t(x), int32_t(y), int32_t(z), int32_t(i), int32_t(j), int32_t(k)); }
  FI XYZEval<int32_t>  ROUNDL()        { return LOGICAL_AXIS_ARRAY(int32_t(LROUND(e)), int32_t(LROUND(x)), int32_t(LROUND(y)), int32_t(LROUND(z)), int32_t(LROUND(i)), int32_t(LROUND(j)), int32_t(LROUND(k))); }
  FI XYZEval<int32_t>  ROUNDL()  const { return LOGICAL_AXIS_ARRAY(int32_t(LROUND(e)), int32_t(LROUND(x)), int32_t(LROUND(y)), int32_t(LROUND(z)), int32_t(LROUND(i)), int32_t(LROUND(j)), int32_t(LROUND(k))); }
  FI XYZEval<float>   asFloat()        { return LOGICAL_AXIS_ARRAY(static_cast<float>(e), static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), static_cast<float>(i), static_cast<float>(j), static_cast<float>(k)); }
  FI XYZEval<float>   asFloat()  const { return LOGICAL_AXIS_ARRAY(static_cast<float>(e), static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), static_cast<float>(i), static_cast<float>(j), static_cast<float>(k)); }
  FI XYZEval<float> reciprocal() const { return LOGICAL_AXIS_ARRAY(_RECIP(e),  _RECIP(x),  _RECIP(y),  _RECIP(z),  _RECIP(i),  _RECIP(j),  _RECIP(k)); }

  // Marlin workspace shifting is done with G92 and M206
  FI XYZEval<float> asLogical()  const { XYZEval<float> o = asFloat(); toLogical(o); return o; }
  FI XYZEval<float>  asNative()  const { XYZEval<float> o = asFloat(); toNative(o);  return o; }

  // In-place cast to types having fewer fields
  FI operator       XYval<T>&()        { return *(XYval<T>*)this; }
  FI operator const XYval<T>&()  const { return *(const XYval<T>*)this; }
  FI operator       XYZval<T>&()       { return *(XYZval<T>*)this; }
  FI operator const XYZval<T>&() const { return *(const XYZval<T>*)this; }

  // Accessor via an AxisEnum (or any integer) [index]
  FI       T&    operator[](const int n)                  { return pos[n]; }
  FI const T&    operator[](const int n)            const { return pos[n]; }

  // Assignment operator overrides do the expected thing
  FI XYZEval<T>& operator= (const T v)                    { set(LIST_N_1(LINEAR_AXES, v)); return *this; }
  FI XYZEval<T>& operator= (const XYval<T>   &rs)         { set(rs.x, rs.y); return *this; }
  FI XYZEval<T>& operator= (const XYZval<T>  &rs)         { set(LINEAR_AXIS_ELEM(rs)); return *this; }

  // Override other operators to get intuitive behaviors
  FI XYZEval<T>  operator+ (const XYval<T>   &rs)   const { XYZEval<T> ls = *this; ls.x += rs.x; ls.y += rs.y; return ls; }
  FI XYZEval<T>  operator+ (const XYval<T>   &rs)         { XYZEval<T> ls = *this; ls.x += rs.x; ls.y += rs.y; return ls; }
  FI XYZEval<T>  operator- (const XYval<T>   &rs)   const { XYZEval<T> ls = *this; ls.x -= rs.x; ls.y -= rs.y; return ls; }
  FI XYZEval<T>  operator- (const XYval<T>   &rs)         { XYZEval<T> ls = *this; ls.x -= rs.x; ls.y -= rs.y; return ls; }
  FI XYZEval<T>  operator* (const XYval<T>   &rs)   const { XYZEval<T> ls = *this; ls.x *= rs.x; ls.y *= rs.y; return ls; }
  FI XYZEval<T>  operator* (const XYval<T>   &rs)         { XYZEval<T> ls = *this; ls.x *= rs.x; ls.y *= rs.y; return ls; }
  FI XYZEval<T>  operator/ (const XYval<T>   &rs)   const { XYZEval<T> ls = *this; ls.x /= rs.x; ls.y /= rs.y; return ls; }
  FI XYZEval<T>  operator/ (const XYval<T>   &rs)         { XYZEval<T> ls = *this; ls.x /= rs.x; ls.y /= rs.y; return ls; }
  FI XYZEval<T>  operator+ (const XYZval<T>  &rs)   const { XYZval<T>  ls = *this; LINEAR_AXIS_CODE(ls.x += rs.x, ls.y += rs.y, ls.z += rs.z, ls.i += rs.i, ls.j += rs.j, ls.k += rs.k); return ls; }
  FI XYZEval<T>  operator+ (const XYZval<T>  &rs)         { XYZval<T>  ls = *this; LINEAR_AXIS_CODE(ls.x += rs.x, ls.y += rs.y, ls.z += rs.z, ls.i += rs.i, ls.j += rs.j, ls.k += rs.k); return ls; }
  FI XYZEval<T>  operator- (const XYZval<T>  &rs)   const { XYZval<T>  ls = *this; LINEAR_AXIS_CODE(ls.x -= rs.x, ls.y -= rs.y, ls.z -= rs.z, ls.i -= rs.i, ls.j -= rs.j, ls.k -= rs.k); return ls; }
  FI XYZEval<T>  operator- (const XYZval<T>  &rs)         { XYZval<T>  ls = *this; LINEAR_AXIS_CODE(ls.x -= rs.x, ls.y -= rs.y, ls.z -= rs.z, ls.i -= rs.i, ls.j -= rs.j, ls.k -= rs.k); return ls; }
  FI XYZEval<T>  operator* (const XYZval<T>  &rs)   const { XYZval<T>  ls = *this; LINEAR_AXIS_CODE(ls.x *= rs.x, ls.y *= rs.y, ls.z *= rs.z, ls.i *= rs.i, ls.j *= rs.j, ls.k *= rs.k); return ls; }
  FI XYZEval<T>  operator* (const XYZval<T>  &rs)         { XYZval<T>  ls = *this; LINEAR_AXIS_CODE(ls.x *= rs.x, ls.y *= rs.y, ls.z *= rs.z, ls.i *= rs.i, ls.j *= rs.j, ls.k *= rs.k); return ls; }
  FI XYZEval<T>  operator/ (const XYZval<T>  &rs)   const { XYZval<T>  ls = *this; LINEAR_AXIS_CODE(ls.x /= rs.x, ls.y /= rs.y, ls.z /= rs.z, ls.i /= rs.i, ls.j /= rs.j, ls.k /= rs.k); return ls; }
  FI XYZEval<T>  operator/ (const XYZval<T>  &rs)         { XYZval<T>  ls = *this; LINEAR_AXIS_CODE(ls.x /= rs.x, ls.y /= rs.y, ls.z /= rs.z, ls.i /= rs.i, ls.j /= rs.j, ls.k /= rs.k); return ls; }
  FI XYZEval<T>  operator+ (const XYZEval<T>  &rs)  const { XYZEval<T> ls = *this; LOGICAL_AXIS_CODE(ls.e += rs.e, ls.x += rs.x, ls.y += rs.y, ls.z += rs.z, ls.i += rs.i, ls.j += rs.j, ls.k += rs.k); return ls; }
  FI XYZEval<T>  operator+ (const XYZEval<T>  &rs)        { XYZEval<T> ls = *this; LOGICAL_AXIS_CODE(ls.e += rs.e, ls.x += rs.x, ls.y += rs.y, ls.z += rs.z, ls.i += rs.i, ls.j += rs.j, ls.k += rs.k); return ls; }
  FI XYZEval<T>  operator- (const XYZEval<T>  &rs)  const { XYZEval<T> ls = *this; LOGICAL_AXIS_CODE(ls.e -= rs.e, ls.x -= rs.x, ls.y -= rs.y, ls.z -= rs.z, ls.i -= rs.i, ls.j -= rs.j, ls.k -= rs.k); return ls; }
  FI XYZEval<T>  operator- (const XYZEval<T>  &rs)        { XYZEval<T> ls = *this; LOGICAL_AXIS_CODE(ls.e -= rs.e, ls.x -= rs.x, ls.y -= rs.y, ls.z -= rs.z, ls.i -= rs.i, ls.j -= rs.j, ls.k -= rs.k); return ls; }
  FI XYZEval<T>  operator* (const XYZEval<T>  &rs)  const { XYZEval<T> ls = *this; LOGICAL_AXIS_CODE(ls.e *= rs.e, ls.x *= rs.x, ls.y *= rs.y, ls.z *= rs.z, ls.i *= rs.i, ls.j *= rs.j, ls.k *= rs.k); return ls; }
  FI XYZEval<T>  operator* (const XYZEval<T>  &rs)        { XYZEval<T> ls = *this; LOGICAL_AXIS_CODE(ls.e *= rs.e, ls.x *= rs.x, ls.y *= rs.y, ls.z *= rs.z, ls.i *= rs.i, ls.j *= rs.j, ls.k *= rs.k); return ls; }
  FI XYZEval<T>  operator/ (const XYZEval<T>  &rs)  const { XYZEval<T> ls = *this; LOGICAL_AXIS_CODE(ls.e /= rs.e, ls.x /= rs.x, ls.y /= rs.y, ls.z /= rs.z, ls.i /= rs.i, ls.j /= rs.j, ls.k /= rs.k); return ls; }
  FI XYZEval<T>  operator/ (const XYZEval<T>  &rs)        { XYZEval<T> ls = *this; LOGICAL_AXIS_CODE(ls.e /= rs.e, ls.x /= rs.x, ls.y /= rs.y, ls.z /= rs.z, ls.i /= rs.i, ls.j /= rs.j, ls.k /= rs.k); return ls; }
  FI XYZEval<T>  operator* (const float &v)         const { XYZEval<T> ls = *this; LOGICAL_AXIS_CODE(ls.e *= v,    ls.x *= v,    ls.y *= v,    ls.z *= v,    ls.i *= v,    ls.j *= v,    ls.k *= v   ); return ls; }
  FI XYZEval<T>  operator* (const float &v)               { XYZEval<T> ls = *this; LOGICAL_AXIS_CODE(ls.e *= v,    ls.x *= v,    ls.y *= v,    ls.z *= v,    ls.i *= v,    ls.j *= v,    ls.k *= v   ); return ls; }
  FI XYZEval<T>  operator* (const int &v)           const { XYZEval<T> ls = *this; LOGICAL_AXIS_CODE(ls.e *= v,    ls.x *= v,    ls.y *= v,    ls.z *= v,    ls.i *= v,    ls.j *= v,    ls.k *= v   ); return ls; }
  FI XYZEval<T>  operator* (const int &v)                 { XYZEval<T> ls = *this; LOGICAL_AXIS_CODE(ls.e *= v,    ls.x *= v,    ls.y *= v,    ls.z *= v,    ls.i *= v,    ls.j *= v,    ls.k *= v   ); return ls; }
  FI XYZEval<T>  operator/ (const float &v)         const { XYZEval<T> ls = *this; LOGICAL_AXIS_CODE(ls.e /= v,    ls.x /= v,    ls.y /= v,    ls.z /= v,    ls.i /= v,    ls.j /= v,    ls.k /= v   ); return ls; }
  FI XYZEval<T>  operator/ (const float &v)               { XYZEval<T> ls = *this; LOGICAL_AXIS_CODE(ls.e /= v,    ls.x /= v,    ls.y /= v,    ls.z /= v,    ls.i /= v,    ls.j /= v,    ls.k /= v   ); return ls; }
  FI XYZEval<T>  operator/ (const int &v)           const { XYZEval<T> ls = *this; LOGICAL_AXIS_CODE(ls.e /= v,    ls.x /= v,    ls.y /= v,    ls.z /= v,    ls.i /= v,    ls.j /= v,    ls.k /= v   ); return ls; }
  FI XYZEval<T>  operator/ (const int &v)                 { XYZEval<T> ls = *this; LOGICAL_AXIS_CODE(ls.e /= v,    ls.x /= v,    ls.y /= v,    ls.z /= v,    ls.i /= v,    ls.j /= v,    ls.k /= v   ); return ls; }
  FI XYZEval<T>  operator>>(const int &v)           const { XYZEval<T> ls = *this; LOGICAL_AXIS_CODE(_RS(ls.e),    _RS(ls.x),    _RS(ls.y),    _RS(ls.z),    _RS(ls.i),    _RS(ls.j),    _RS(ls.k)   ); return ls; }
  FI XYZEval<T>  operator>>(const int &v)                 { XYZEval<T> ls = *this; LOGICAL_AXIS_CODE(_RS(ls.e),    _RS(ls.x),    _RS(ls.y),    _RS(ls.z),    _RS(ls.i),    _RS(ls.j),    _RS(ls.k)   ); return ls; }
  FI XYZEval<T>  operator<<(const int &v)           const { XYZEval<T> ls = *this; LOGICAL_AXIS_CODE(_LS(ls.e),    _LS(ls.x),    _LS(ls.y),    _LS(ls.z),    _LS(ls.i),    _LS(ls.j),    _LS(ls.k)   ); return ls; }
  FI XYZEval<T>  operator<<(const int &v)                 { XYZEval<T> ls = *this; LOGICAL_AXIS_CODE(_LS(ls.e),    _LS(ls.x),    _LS(ls.y),    _LS(ls.z),    _LS(ls.i),    _LS(ls.j),    _LS(ls.k)   ); return ls; }
  FI const XYZEval<T> operator-()                   const { return LOGICAL_AXIS_ARRAY(-e, -x, -y, -z, -i, -j, -k); }
  FI       XYZEval<T> operator-()                         { return LOGICAL_AXIS_ARRAY(-e, -x, -y, -z, -i, -j, -k); }

  // Modifier operators
  FI XYZEval<T>& operator+=(const XYval<T>   &rs)         { x += rs.x; y += rs.y; return *this; }
  FI XYZEval<T>& operator-=(const XYval<T>   &rs)         { x -= rs.x; y -= rs.y; return *this; }
  FI XYZEval<T>& operator*=(const XYval<T>   &rs)         { x *= rs.x; y *= rs.y; return *this; }
  FI XYZEval<T>& operator/=(const XYval<T>   &rs)         { x /= rs.x; y /= rs.y; return *this; }
  FI XYZEval<T>& operator+=(const XYZval<T>  &rs)         { LINEAR_AXIS_CODE(x += rs.x, y += rs.y, z += rs.z, i += rs.i, j += rs.j, k += rs.k); return *this; }
  FI XYZEval<T>& operator-=(const XYZval<T>  &rs)         { LINEAR_AXIS_CODE(x -= rs.x, y -= rs.y, z -= rs.z, i -= rs.i, j -= rs.j, k -= rs.k); return *this; }
  FI XYZEval<T>& operator*=(const XYZval<T>  &rs)         { LINEAR_AXIS_CODE(x *= rs.x, y *= rs.y, z *= rs.z, i *= rs.i, j *= rs.j, k *= rs.k); return *this; }
  FI XYZEval<T>& operator/=(const XYZval<T>  &rs)         { LINEAR_AXIS_CODE(x /= rs.x, y /= rs.y, z /= rs.z, i /= rs.i, j /= rs.j, k /= rs.k); return *this; }
  FI XYZEval<T>& operator+=(const XYZEval<T> &rs)         { LOGICAL_AXIS_CODE(e += rs.e, x += rs.x, y += rs.y, z += rs.z, i += rs.i, j += rs.j, k += rs.k); return *this; }
  FI XYZEval<T>& operator-=(const XYZEval<T> &rs)         { LOGICAL_AXIS_CODE(e -= rs.e, x -= rs.x, y -= rs.y, z -= rs.z, i -= rs.i, j -= rs.j, k -= rs.k); return *this; }
  FI XYZEval<T>& operator*=(const XYZEval<T> &rs)         { LOGICAL_AXIS_CODE(e *= rs.e, x *= rs.x, y *= rs.y, z *= rs.z, i *= rs.i, j *= rs.j, k *= rs.k); return *this; }
  FI XYZEval<T>& operator/=(const XYZEval<T> &rs)         { LOGICAL_AXIS_CODE(e /= rs.e, x /= rs.x, y /= rs.y, z /= rs.z, i /= rs.i, j /= rs.j, k /= rs.k); return *this; }
  FI XYZEval<T>& operator*=(const T &v)                   { LOGICAL_AXIS_CODE(e *= v,    x *= v,    y *= v,    z *= v,    i *= v,    j *= v,    k *= v);    return *this; }
  FI XYZEval<T>& operator>>=(const int &v)                { LOGICAL_AXIS_CODE(_RS(e),    _RS(x),    _RS(y),    _RS(z),    _RS(i),    _RS(j),    _RS(k));    return *this; }
  FI XYZEval<T>& operator<<=(const int &v)                { LOGICAL_AXIS_CODE(_LS(e),    _LS(x),    _LS(y),    _LS(z),    _LS(i),    _LS(j),    _LS(k));    return *this; }

  // Exact comparisons. For floats a "NEAR" operation may be better.
  FI bool        operator==(const XYZval<T>  &rs)         { return true LINEAR_AXIS_GANG(&& x == rs.x, && y == rs.y, && z == rs.z, && i == rs.i, && j == rs.j, && k == rs.k); }
  FI bool        operator==(const XYZval<T>  &rs)   const { return true LINEAR_AXIS_GANG(&& x == rs.x, && y == rs.y, && z == rs.z, && i == rs.i, && j == rs.j, && k == rs.k); }
  FI bool        operator!=(const XYZval<T>  &rs)         { return !operator==(rs); }
  FI bool        operator!=(const XYZval<T>  &rs)   const { return !operator==(rs); }
};

#undef _RECIP
#undef _ABS
#undef _LS
#undef _RS
#undef FI
