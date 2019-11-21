/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include <math.h>
#include <stddef.h>

#include "millis_t.h"

class __FlashStringHelper;
typedef const __FlashStringHelper *progmem_str;

//
// Enumerated axis indices
//
//  - X_AXIS, Y_AXIS, and Z_AXIS should be used for axes in Cartesian space
//  - A_AXIS, B_AXIS, and C_AXIS should be used for Steppers, corresponding to XYZ on Cartesians
//  - X_HEAD, Y_HEAD, and Z_HEAD should be used for Steppers on Core kinematics
//
enum AxisEnum : uint8_t {
  X_AXIS   = 0, A_AXIS = 0,
  Y_AXIS   = 1, B_AXIS = 1,
  Z_AXIS   = 2, C_AXIS = 2,
  E_AXIS   = 3,
  X_HEAD   = 4, Y_HEAD = 5, Z_HEAD = 6,
  E0_AXIS  = 3,
  E1_AXIS  = 4,
  E2_AXIS  = 5,
  E3_AXIS  = 6,
  E4_AXIS  = 7,
  E5_AXIS  = 8,
  ALL_AXES = 0xFE, NO_AXIS = 0xFF
};

//
// Loop over XYZE axes
//

#define LOOP_S_LE_N(VAR, S, N) for (uint8_t VAR=(S); VAR<=(N); VAR++)
#define LOOP_S_L_N(VAR, S, N) for (uint8_t VAR=(S); VAR<(N); VAR++)
#define LOOP_LE_N(VAR, N) LOOP_S_LE_N(VAR, 0, N)
#define LOOP_L_N(VAR, N) LOOP_S_L_N(VAR, 0, N)

#define LOOP_XYZ(VAR) LOOP_S_LE_N(VAR, X_AXIS, Z_AXIS)
#define LOOP_XYZE(VAR) LOOP_S_LE_N(VAR, X_AXIS, E_AXIS)
#define LOOP_XYZE_N(VAR) LOOP_S_L_N(VAR, X_AXIS, XYZE_N)
#define LOOP_ABC(VAR) LOOP_S_LE_N(VAR, A_AXIS, C_AXIS)
#define LOOP_ABCE(VAR) LOOP_S_LE_N(VAR, A_AXIS, E_AXIS)
#define LOOP_ABCE_N(VAR) LOOP_S_L_N(VAR, A_AXIS, XYZE_N)

//
// Conditional type assignment magic. For example...
//
// typename IF<(MYOPT==12), int, float>::type myvar;
//
template <bool, class L, class R>
struct IF { typedef R type; };
template <class L, class R>
struct IF<true, L, R> { typedef L type; };

//
// feedRate_t is just a humble float
//
typedef float feedRate_t;

// Conversion macros
#define MMM_TO_MMS(MM_M) feedRate_t(float(MM_M) / 60.0f)
#define MMS_TO_MMM(MM_S) (float(MM_S) * 60.0f)
#define MMS_SCALED(V)    ((V) * 0.01f * feedrate_percentage)

//
// Coordinates structures for XY, XYZ, XYZE...
//

// Helpers
#define _RECIP(N) ((N) ? 1.0f / float(N) : 0.0f)
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
// XY coordinates, counters, etc.
//
template<typename T>
struct XYval {
  union {
    struct { T x, y; };
    struct { T a, b; };
    T pos[2];
  };
  FI void set(const T px)                               { x = px; }
  FI void set(const T px, const T py)                   { x = px; y = py; }
  FI void reset()                                       { x = y = 0; }
  FI T magnitude()                                const { return (T)sqrtf(x*x + y*y); }
  FI operator T* ()                                     { return pos; }
  FI operator bool()                                    { return x || y; }
  FI XYval<T>           copy()                    const { return *this; }
  FI XYval<T>            ABS()                    const { return { T(_ABS(x)), T(_ABS(y)) }; }
  FI XYval<int16_t>    asInt()                          { return { int16_t(x), int16_t(y) }; }
  FI XYval<int16_t>    asInt()                    const { return { int16_t(x), int16_t(y) }; }
  FI XYval<int32_t>   asLong()                          { return { int32_t(x), int32_t(y) }; }
  FI XYval<int32_t>   asLong()                    const { return { int32_t(x), int32_t(y) }; }
  FI XYval<float>    asFloat()                          { return {   float(x),   float(y) }; }
  FI XYval<float>    asFloat()                    const { return {   float(x),   float(y) }; }
  FI XYval<float> reciprocal()                    const { return {  _RECIP(x),  _RECIP(y) }; }
  FI XYval<float>  asLogical()                    const { XYval<float> o = asFloat(); toLogical(o); return o; }
  FI XYval<float>   asNative()                    const { XYval<float> o = asFloat(); toNative(o);  return o; }
  FI operator XYZval<T>()                               { return { x, y }; }
  FI operator XYZval<T>()                         const { return { x, y }; }
  FI operator XYZEval<T>()                              { return { x, y }; }
  FI operator XYZEval<T>()                        const { return { x, y }; }
  FI       T&  operator[](const int i)                  { return pos[i]; }
  FI const T&  operator[](const int i)            const { return pos[i]; }
  FI XYval<T>& operator= (const T v)                    { set(v,    v   ); return *this; }
  FI XYval<T>& operator= (const XYZval<T>  &rs)         { set(rs.x, rs.y); return *this; }
  FI XYval<T>& operator= (const XYZEval<T> &rs)         { set(rs.x, rs.y); return *this; }
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
  FI XYval<T>       operator-()                         { XYval<T> o = *this; o.x = -x; o.y = -y; return o; }
  FI const XYval<T> operator-()                   const { XYval<T> o = *this; o.x = -x; o.y = -y; return o; }
};

//
// XYZ coordinates, counters, etc.
//
template<typename T>
struct XYZval {
  union {
    struct { T x, y, z; };
    struct { T a, b, c; };
    T pos[3];
  };
  FI void set(const T px)                              { x = px; }
  FI void set(const T px, const T py)                  { x = px; y = py; }
  FI void set(const T px, const T py, const T pz)      { x = px; y = py; z = pz; }
  FI void set(const XYval<T> pxy, const T pz)          { x = pxy.x; y = pxy.y; z = pz; }
  FI void reset()                                      { x = y = z = 0; }
  FI T magnitude()                               const { return (T)sqrtf(x*x + y*y + z*z); }
  FI operator T* ()                                    { return pos; }
  FI operator bool()                                   { return z || x || y; }
  FI XYZval<T>          copy()                   const { XYZval<T> o = *this; return o; }
  FI XYZval<T>           ABS()                   const { return { T(_ABS(x)), T(_ABS(y)), T(_ABS(z)) }; }
  FI XYZval<int16_t>   asInt()                         { return { int16_t(x), int16_t(y), int16_t(z) }; }
  FI XYZval<int16_t>   asInt()                   const { return { int16_t(x), int16_t(y), int16_t(z) }; }
  FI XYZval<int32_t>  asLong()                         { return { int32_t(x), int32_t(y), int32_t(z) }; }
  FI XYZval<int32_t>  asLong()                   const { return { int32_t(x), int32_t(y), int32_t(z) }; }
  FI XYZval<float>   asFloat()                         { return {   float(x),   float(y),   float(z) }; }
  FI XYZval<float>   asFloat()                   const { return {   float(x),   float(y),   float(z) }; }
  FI XYZval<float> reciprocal()                  const { return {  _RECIP(x),  _RECIP(y),  _RECIP(z) }; }
  FI XYZval<float> asLogical()                   const { XYZval<float> o = asFloat(); toLogical(o); return o; }
  FI XYZval<float>  asNative()                   const { XYZval<float> o = asFloat(); toNative(o);  return o; }
  FI operator       XYval<T>&()                        { return *(XYval<T>*)this; }
  FI operator const XYval<T>&()                  const { return *(const XYval<T>*)this; }
  FI operator       XYZEval<T>()                 const { return { x, y, z }; }
  FI       T&   operator[](const int i)                { return pos[i]; }
  FI const T&   operator[](const int i)          const { return pos[i]; }
  FI XYZval<T>& operator= (const T v)                  { set(v,    v,    v   ); return *this; }
  FI XYZval<T>& operator= (const XYval<T>   &rs)       { set(rs.x, rs.y      ); return *this; }
  FI XYZval<T>& operator= (const XYZEval<T> &rs)       { set(rs.x, rs.y, rs.z); return *this; }
  FI XYZval<T>  operator+ (const XYval<T>   &rs) const { XYZval<T> ls = *this; ls.x += rs.x; ls.y += rs.y;               return ls; }
  FI XYZval<T>  operator+ (const XYval<T>   &rs)       { XYZval<T> ls = *this; ls.x += rs.x; ls.y += rs.y;               return ls; }
  FI XYZval<T>  operator- (const XYval<T>   &rs) const { XYZval<T> ls = *this; ls.x -= rs.x; ls.y -= rs.y;               return ls; }
  FI XYZval<T>  operator- (const XYval<T>   &rs)       { XYZval<T> ls = *this; ls.x -= rs.x; ls.y -= rs.y;               return ls; }
  FI XYZval<T>  operator* (const XYval<T>   &rs) const { XYZval<T> ls = *this; ls.x *= rs.x; ls.y *= rs.y;               return ls; }
  FI XYZval<T>  operator* (const XYval<T>   &rs)       { XYZval<T> ls = *this; ls.x *= rs.x; ls.y *= rs.y;               return ls; }
  FI XYZval<T>  operator/ (const XYval<T>   &rs) const { XYZval<T> ls = *this; ls.x /= rs.x; ls.y /= rs.y;               return ls; }
  FI XYZval<T>  operator/ (const XYval<T>   &rs)       { XYZval<T> ls = *this; ls.x /= rs.x; ls.y /= rs.y;               return ls; }
  FI XYZval<T>  operator+ (const XYZval<T>  &rs) const { XYZval<T> ls = *this; ls.x += rs.x; ls.y += rs.y; ls.z += rs.z; return ls; }
  FI XYZval<T>  operator+ (const XYZval<T>  &rs)       { XYZval<T> ls = *this; ls.x += rs.x; ls.y += rs.y; ls.z += rs.z; return ls; }
  FI XYZval<T>  operator- (const XYZval<T>  &rs) const { XYZval<T> ls = *this; ls.x -= rs.x; ls.y -= rs.y; ls.z -= rs.z; return ls; }
  FI XYZval<T>  operator- (const XYZval<T>  &rs)       { XYZval<T> ls = *this; ls.x -= rs.x; ls.y -= rs.y; ls.z -= rs.z; return ls; }
  FI XYZval<T>  operator* (const XYZval<T>  &rs) const { XYZval<T> ls = *this; ls.x *= rs.x; ls.y *= rs.y; ls.z *= rs.z; return ls; }
  FI XYZval<T>  operator* (const XYZval<T>  &rs)       { XYZval<T> ls = *this; ls.x *= rs.x; ls.y *= rs.y; ls.z *= rs.z; return ls; }
  FI XYZval<T>  operator/ (const XYZval<T>  &rs) const { XYZval<T> ls = *this; ls.x /= rs.x; ls.y /= rs.y; ls.z /= rs.z; return ls; }
  FI XYZval<T>  operator/ (const XYZval<T>  &rs)       { XYZval<T> ls = *this; ls.x /= rs.x; ls.y /= rs.y; ls.z /= rs.z; return ls; }
  FI XYZval<T>  operator+ (const XYZEval<T> &rs) const { XYZval<T> ls = *this; ls.x += rs.x; ls.y += rs.y; ls.z += rs.z; return ls; }
  FI XYZval<T>  operator+ (const XYZEval<T> &rs)       { XYZval<T> ls = *this; ls.x += rs.x; ls.y += rs.y; ls.z += rs.z; return ls; }
  FI XYZval<T>  operator- (const XYZEval<T> &rs) const { XYZval<T> ls = *this; ls.x -= rs.x; ls.y -= rs.y; ls.z -= rs.z; return ls; }
  FI XYZval<T>  operator- (const XYZEval<T> &rs)       { XYZval<T> ls = *this; ls.x -= rs.x; ls.y -= rs.y; ls.z -= rs.z; return ls; }
  FI XYZval<T>  operator* (const XYZEval<T> &rs) const { XYZval<T> ls = *this; ls.x *= rs.x; ls.y *= rs.y; ls.z *= rs.z; return ls; }
  FI XYZval<T>  operator* (const XYZEval<T> &rs)       { XYZval<T> ls = *this; ls.x *= rs.x; ls.y *= rs.y; ls.z *= rs.z; return ls; }
  FI XYZval<T>  operator/ (const XYZEval<T> &rs) const { XYZval<T> ls = *this; ls.x /= rs.x; ls.y /= rs.y; ls.z /= rs.z; return ls; }
  FI XYZval<T>  operator/ (const XYZEval<T> &rs)       { XYZval<T> ls = *this; ls.x /= rs.x; ls.y /= rs.y; ls.z /= rs.z; return ls; }
  FI XYZval<T>  operator* (const float &v)       const { XYZval<T> ls = *this; ls.x *= v;    ls.y *= v;    ls.z *= z;    return ls; }
  FI XYZval<T>  operator* (const float &v)             { XYZval<T> ls = *this; ls.x *= v;    ls.y *= v;    ls.z *= z;    return ls; }
  FI XYZval<T>  operator* (const int &v)         const { XYZval<T> ls = *this; ls.x *= v;    ls.y *= v;    ls.z *= z;    return ls; }
  FI XYZval<T>  operator* (const int &v)               { XYZval<T> ls = *this; ls.x *= v;    ls.y *= v;    ls.z *= z;    return ls; }
  FI XYZval<T>  operator/ (const float &v)       const { XYZval<T> ls = *this; ls.x /= v;    ls.y /= v;    ls.z /= z;    return ls; }
  FI XYZval<T>  operator/ (const float &v)             { XYZval<T> ls = *this; ls.x /= v;    ls.y /= v;    ls.z /= z;    return ls; }
  FI XYZval<T>  operator/ (const int &v)         const { XYZval<T> ls = *this; ls.x /= v;    ls.y /= v;    ls.z /= z;    return ls; }
  FI XYZval<T>  operator/ (const int &v)               { XYZval<T> ls = *this; ls.x /= v;    ls.y /= v;    ls.z /= z;    return ls; }
  FI XYZval<T>  operator>>(const int &v)         const { XYZval<T> ls = *this; _RS(ls.x); _RS(ls.y); _RS(ls.z); return ls; }
  FI XYZval<T>  operator>>(const int &v)               { XYZval<T> ls = *this; _RS(ls.x); _RS(ls.y); _RS(ls.z); return ls; }
  FI XYZval<T>  operator<<(const int &v)         const { XYZval<T> ls = *this; _LS(ls.x); _LS(ls.y); _LS(ls.z); return ls; }
  FI XYZval<T>  operator<<(const int &v)               { XYZval<T> ls = *this; _LS(ls.x); _LS(ls.y); _LS(ls.z); return ls; }
  FI XYZval<T>& operator+=(const XYval<T>   &rs)       { x += rs.x; y += rs.y;            return *this; }
  FI XYZval<T>& operator-=(const XYval<T>   &rs)       { x -= rs.x; y -= rs.y;            return *this; }
  FI XYZval<T>& operator*=(const XYval<T>   &rs)       { x *= rs.x; y *= rs.y;            return *this; }
  FI XYZval<T>& operator/=(const XYval<T>   &rs)       { x /= rs.x; y /= rs.y;            return *this; }
  FI XYZval<T>& operator+=(const XYZval<T>  &rs)       { x += rs.x; y += rs.y; z += rs.z; return *this; }
  FI XYZval<T>& operator-=(const XYZval<T>  &rs)       { x -= rs.x; y -= rs.y; z -= rs.z; return *this; }
  FI XYZval<T>& operator*=(const XYZval<T>  &rs)       { x *= rs.x; y *= rs.y; z *= rs.z; return *this; }
  FI XYZval<T>& operator/=(const XYZval<T>  &rs)       { x /= rs.x; y /= rs.y; z /= rs.z; return *this; }
  FI XYZval<T>& operator+=(const XYZEval<T> &rs)       { x += rs.x; y += rs.y; z += rs.z; return *this; }
  FI XYZval<T>& operator-=(const XYZEval<T> &rs)       { x -= rs.x; y -= rs.y; z -= rs.z; return *this; }
  FI XYZval<T>& operator*=(const XYZEval<T> &rs)       { x *= rs.x; y *= rs.y; z *= rs.z; return *this; }
  FI XYZval<T>& operator/=(const XYZEval<T> &rs)       { x /= rs.x; y /= rs.y; z /= rs.z; return *this; }
  FI XYZval<T>& operator*=(const float &v)             { x *= v;    y *= v;    z *= v;    return *this; }
  FI XYZval<T>& operator*=(const int &v)               { x *= v;    y *= v;    z *= v;    return *this; }
  FI XYZval<T>& operator>>=(const int &v)              { _RS(x);   _RS(y);   _RS(z);   return *this; }
  FI XYZval<T>& operator<<=(const int &v)              { _LS(x);   _LS(y);   _LS(z);   return *this; }
  FI bool       operator==(const XYZEval<T> &rs)       { return x == rs.x && y == rs.y && z == rs.z; }
  FI bool       operator!=(const XYZEval<T> &rs)       { return !operator==(rs); }
  FI bool       operator==(const XYZEval<T> &rs) const { return x == rs.x && y == rs.y && z == rs.z; }
  FI bool       operator!=(const XYZEval<T> &rs) const { return !operator==(rs); }
  FI XYZval<T>       operator-()                       { XYZval<T> o = *this; o.x = -x; o.y = -y; o.z = -z; return o; }
  FI const XYZval<T> operator-()                 const { XYZval<T> o = *this; o.x = -x; o.y = -y; o.z = -z; return o; }
};

//
// XYZE coordinates, counters, etc.
//
template<typename T>
struct XYZEval {
  union {
    struct{ T x, y, z, e; };
    struct{ T a, b, c; };
    T pos[4];
  };
  FI void reset()                                             { x = y = z = e = 0; }
  FI T magnitude()                                      const { return (T)sqrtf(x*x + y*y + z*z + e*e); }
  FI operator T* ()                                           { return pos; }
  FI operator bool()                                          { return e || z || x || y; }
  FI void set(const T px)                                     { x = px;                                        }
  FI void set(const T px, const T py)                         { x = px;     y = py;                            }
  FI void set(const T px, const T py, const T pz)             { x = px;     y = py;     z = pz;                }
  FI void set(const T px, const T py, const T pz, const T pe) { x = px;     y = py;     z = pz;     e = pe;    }
  FI void set(const XYval<T> pxy)                             { x = pxy.x;  y = pxy.y;                         }
  FI void set(const XYval<T> pxy, const T pz)                 { x = pxy.x;  y = pxy.y;  z = pz;                }
  FI void set(const XYZval<T> pxyz)                           { x = pxyz.x; y = pxyz.y; z = pxyz.z;            }
  FI void set(const XYval<T> pxy, const T pz, const T pe)     { x = pxy.x;  y = pxy.y;  z = pz;     e = pe;    }
  FI void set(const XYval<T> pxy, const XYval<T> pze)         { x = pxy.x;  y = pxy.y;  z = pze.z;  e = pze.e; }
  FI void set(const XYZval<T> pxyz, const T pe)               { x = pxyz.x; y = pxyz.y; z = pxyz.z; e = pe;    }
  FI XYZEval<T>          copy()                         const { return *this; }
  FI XYZEval<T>           ABS()                         const { return { T(_ABS(x)), T(_ABS(y)), T(_ABS(z)), T(_ABS(e)) }; }
  FI XYZEval<int16_t>   asInt()                               { return { int16_t(x), int16_t(y), int16_t(z), int16_t(e) }; }
  FI XYZEval<int16_t>   asInt()                         const { return { int16_t(x), int16_t(y), int16_t(z), int16_t(e) }; }
  FI XYZEval<int32_t>  asLong()                         const { return { int32_t(x), int32_t(y), int32_t(z), int32_t(e) }; }
  FI XYZEval<int32_t>  asLong()                               { return { int32_t(x), int32_t(y), int32_t(z), int32_t(e) }; }
  FI XYZEval<float>   asFloat()                               { return {   float(x),   float(y),   float(z),   float(e) }; }
  FI XYZEval<float>   asFloat()                         const { return {   float(x),   float(y),   float(z),   float(e) }; }
  FI XYZEval<float> reciprocal()                        const { return {  _RECIP(x),  _RECIP(y),  _RECIP(z),  _RECIP(e) }; }
  FI XYZEval<float> asLogical()                         const { XYZEval<float> o = asFloat(); toLogical(o); return o; }
  FI XYZEval<float>  asNative()                         const { XYZEval<float> o = asFloat(); toNative(o);  return o; }
  FI operator       XYval<T>&()                               { return *(XYval<T>*)this; }
  FI operator const XYval<T>&()                         const { return *(const XYval<T>*)this; }
  FI operator       XYZval<T>&()                              { return *(XYZval<T>*)this; }
  FI operator const XYZval<T>&()                        const { return *(const XYZval<T>*)this; }
  FI       T&    operator[](const int i)                      { return pos[i]; }
  FI const T&    operator[](const int i)                const { return pos[i]; }
  FI XYZEval<T>& operator= (const T v)                        { set(v, v, v, v); return *this; }
  FI XYZEval<T>& operator= (const XYval<T>   &rs)             { set(rs.x, rs.y); return *this; }
  FI XYZEval<T>& operator= (const XYZval<T>  &rs)             { set(rs.x, rs.y, rs.z); return *this; }
  FI XYZEval<T>  operator+ (const XYval<T>   &rs)       const { XYZEval<T> ls = *this; ls.x += rs.x; ls.y += rs.y;                             return ls; }
  FI XYZEval<T>  operator+ (const XYval<T>   &rs)             { XYZEval<T> ls = *this; ls.x += rs.x; ls.y += rs.y;                             return ls; }
  FI XYZEval<T>  operator- (const XYval<T>   &rs)       const { XYZEval<T> ls = *this; ls.x -= rs.x; ls.y -= rs.y;                             return ls; }
  FI XYZEval<T>  operator- (const XYval<T>   &rs)             { XYZEval<T> ls = *this; ls.x -= rs.x; ls.y -= rs.y;                             return ls; }
  FI XYZEval<T>  operator* (const XYval<T>   &rs)       const { XYZEval<T> ls = *this; ls.x *= rs.x; ls.y *= rs.y;                             return ls; }
  FI XYZEval<T>  operator* (const XYval<T>   &rs)             { XYZEval<T> ls = *this; ls.x *= rs.x; ls.y *= rs.y;                             return ls; }
  FI XYZEval<T>  operator/ (const XYval<T>   &rs)       const { XYZEval<T> ls = *this; ls.x /= rs.x; ls.y /= rs.y;                             return ls; }
  FI XYZEval<T>  operator/ (const XYval<T>   &rs)             { XYZEval<T> ls = *this; ls.x /= rs.x; ls.y /= rs.y;                             return ls; }
  FI XYZEval<T>  operator+ (const XYZval<T>  &rs)       const { XYZEval<T> ls = *this; ls.x += rs.x; ls.y += rs.y; ls.z += rs.z;               return ls; }
  FI XYZEval<T>  operator+ (const XYZval<T>  &rs)             { XYZEval<T> ls = *this; ls.x += rs.x; ls.y += rs.y; ls.z += rs.z;               return ls; }
  FI XYZEval<T>  operator- (const XYZval<T>  &rs)       const { XYZEval<T> ls = *this; ls.x -= rs.x; ls.y -= rs.y; ls.z -= rs.z;               return ls; }
  FI XYZEval<T>  operator- (const XYZval<T>  &rs)             { XYZEval<T> ls = *this; ls.x -= rs.x; ls.y -= rs.y; ls.z -= rs.z;               return ls; }
  FI XYZEval<T>  operator* (const XYZval<T>  &rs)       const { XYZEval<T> ls = *this; ls.x *= rs.x; ls.y *= rs.y; ls.z *= rs.z;               return ls; }
  FI XYZEval<T>  operator* (const XYZval<T>  &rs)             { XYZEval<T> ls = *this; ls.x *= rs.x; ls.y *= rs.y; ls.z *= rs.z;               return ls; }
  FI XYZEval<T>  operator/ (const XYZval<T>  &rs)       const { XYZEval<T> ls = *this; ls.x /= rs.x; ls.y /= rs.y; ls.z /= rs.z;               return ls; }
  FI XYZEval<T>  operator/ (const XYZval<T>  &rs)             { XYZEval<T> ls = *this; ls.x /= rs.x; ls.y /= rs.y; ls.z /= rs.z;               return ls; }
  FI XYZEval<T>  operator+ (const XYZEval<T> &rs)       const { XYZEval<T> ls = *this; ls.x += rs.x; ls.y += rs.y; ls.z += rs.z; ls.e += rs.e; return ls; }
  FI XYZEval<T>  operator+ (const XYZEval<T> &rs)             { XYZEval<T> ls = *this; ls.x += rs.x; ls.y += rs.y; ls.z += rs.z; ls.e += rs.e; return ls; }
  FI XYZEval<T>  operator- (const XYZEval<T> &rs)       const { XYZEval<T> ls = *this; ls.x -= rs.x; ls.y -= rs.y; ls.z -= rs.z; ls.e -= rs.e; return ls; }
  FI XYZEval<T>  operator- (const XYZEval<T> &rs)             { XYZEval<T> ls = *this; ls.x -= rs.x; ls.y -= rs.y; ls.z -= rs.z; ls.e -= rs.e; return ls; }
  FI XYZEval<T>  operator* (const XYZEval<T> &rs)       const { XYZEval<T> ls = *this; ls.x *= rs.x; ls.y *= rs.y; ls.z *= rs.z; ls.e *= rs.e; return ls; }
  FI XYZEval<T>  operator* (const XYZEval<T> &rs)             { XYZEval<T> ls = *this; ls.x *= rs.x; ls.y *= rs.y; ls.z *= rs.z; ls.e *= rs.e; return ls; }
  FI XYZEval<T>  operator/ (const XYZEval<T> &rs)       const { XYZEval<T> ls = *this; ls.x /= rs.x; ls.y /= rs.y; ls.z /= rs.z; ls.e /= rs.e; return ls; }
  FI XYZEval<T>  operator/ (const XYZEval<T> &rs)             { XYZEval<T> ls = *this; ls.x /= rs.x; ls.y /= rs.y; ls.z /= rs.z; ls.e /= rs.e; return ls; }
  FI XYZEval<T>  operator* (const float &v)             const { XYZEval<T> ls = *this; ls.x *= v;    ls.y *= v;    ls.z *= v;    ls.e *= v;    return ls; }
  FI XYZEval<T>  operator* (const float &v)                   { XYZEval<T> ls = *this; ls.x *= v;    ls.y *= v;    ls.z *= v;    ls.e *= v;    return ls; }
  FI XYZEval<T>  operator* (const int &v)               const { XYZEval<T> ls = *this; ls.x *= v;    ls.y *= v;    ls.z *= v;    ls.e *= v;    return ls; }
  FI XYZEval<T>  operator* (const int &v)                     { XYZEval<T> ls = *this; ls.x *= v;    ls.y *= v;    ls.z *= v;    ls.e *= v;    return ls; }
  FI XYZEval<T>  operator/ (const float &v)             const { XYZEval<T> ls = *this; ls.x /= v;    ls.y /= v;    ls.z /= v;    ls.e /= v;    return ls; }
  FI XYZEval<T>  operator/ (const float &v)                   { XYZEval<T> ls = *this; ls.x /= v;    ls.y /= v;    ls.z /= v;    ls.e /= v;    return ls; }
  FI XYZEval<T>  operator/ (const int &v)               const { XYZEval<T> ls = *this; ls.x /= v;    ls.y /= v;    ls.z /= v;    ls.e /= v;    return ls; }
  FI XYZEval<T>  operator/ (const int &v)                     { XYZEval<T> ls = *this; ls.x /= v;    ls.y /= v;    ls.z /= v;    ls.e /= v;    return ls; }
  FI XYZEval<T>  operator>>(const int &v)               const { XYZEval<T> ls = *this; _RS(ls.x);    _RS(ls.y);    _RS(ls.z);    _RS(ls.e);    return ls; }
  FI XYZEval<T>  operator>>(const int &v)                     { XYZEval<T> ls = *this; _RS(ls.x);    _RS(ls.y);    _RS(ls.z);    _RS(ls.e);    return ls; }
  FI XYZEval<T>  operator<<(const int &v)               const { XYZEval<T> ls = *this; _LS(ls.x);    _LS(ls.y);    _LS(ls.z);    _LS(ls.e);    return ls; }
  FI XYZEval<T>  operator<<(const int &v)                     { XYZEval<T> ls = *this; _LS(ls.x);    _LS(ls.y);    _LS(ls.z);    _LS(ls.e);    return ls; }
  FI XYZEval<T>& operator+=(const XYval<T>   &rs)             { x += rs.x; y += rs.y;                       return *this; }
  FI XYZEval<T>& operator-=(const XYval<T>   &rs)             { x -= rs.x; y -= rs.y;                       return *this; }
  FI XYZEval<T>& operator*=(const XYval<T>   &rs)             { x *= rs.x; y *= rs.y;                       return *this; }
  FI XYZEval<T>& operator/=(const XYval<T>   &rs)             { x /= rs.x; y /= rs.y;                       return *this; }
  FI XYZEval<T>& operator+=(const XYZval<T>  &rs)             { x += rs.x; y += rs.y; z += rs.z;            return *this; }
  FI XYZEval<T>& operator-=(const XYZval<T>  &rs)             { x -= rs.x; y -= rs.y; z -= rs.z;            return *this; }
  FI XYZEval<T>& operator*=(const XYZval<T>  &rs)             { x *= rs.x; y *= rs.y; z *= rs.z;            return *this; }
  FI XYZEval<T>& operator/=(const XYZval<T>  &rs)             { x /= rs.x; y /= rs.y; z /= rs.z;            return *this; }
  FI XYZEval<T>& operator+=(const XYZEval<T> &rs)             { x += rs.x; y += rs.y; z += rs.z; e += rs.e; return *this; }
  FI XYZEval<T>& operator-=(const XYZEval<T> &rs)             { x -= rs.x; y -= rs.y; z -= rs.z; e -= rs.e; return *this; }
  FI XYZEval<T>& operator*=(const XYZEval<T> &rs)             { x *= rs.x; y *= rs.y; z *= rs.z; e *= rs.e; return *this; }
  FI XYZEval<T>& operator/=(const XYZEval<T> &rs)             { x /= rs.x; y /= rs.y; z /= rs.z; e /= rs.e; return *this; }
  FI XYZEval<T>& operator*=(const T &v)                       { x *= v;    y *= v;    z *= v;    e *= v;    return *this; }
  FI XYZEval<T>& operator>>=(const int &v)                    { _RS(x);    _RS(y);    _RS(z);    _RS(e);    return *this; }
  FI XYZEval<T>& operator<<=(const int &v)                    { _LS(x);    _LS(y);    _LS(z);    _LS(e);    return *this; }
  FI bool        operator==(const XYZval<T>  &rs)             { return x == rs.x && y == rs.y && z == rs.z; }
  FI bool        operator!=(const XYZval<T>  &rs)             { return !operator==(rs); }
  FI bool        operator==(const XYZval<T>  &rs)       const { return x == rs.x && y == rs.y && z == rs.z; }
  FI bool        operator!=(const XYZval<T>  &rs)       const { return !operator==(rs); }
  FI       XYZEval<T> operator-()                             { return { -x, -y, -z, -e }; }
  FI const XYZEval<T> operator-()                       const { return { -x, -y, -z, -e }; }
};

#undef _RECIP
#undef _ABS
#undef _LS
#undef _RS
#undef FI

const xyze_char_t axis_codes { 'X', 'Y', 'Z', 'E' };
