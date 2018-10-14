/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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
#ifndef _DEP_TYPES_H_
  #define _DEP_TYPES_H_
#else
  #error "dep_types.h should only be included from MarlinConfig.h"
#endif

#if ENABLED(DISTINCT_E_FACTORS) && E_STEPPERS > 1
  #define E_AXIS_N(E) (E_AXIS + E)
  #define E_FIELDS LIST_N(E_STEPPERS, e, e1, e2, e3, e4, e5)
#else
  #undef DISTINCT_E_FACTORS
  #define E_AXIS_N(E) E_AXIS
  #define E_FIELDS e
#endif

// Forward declarations
template<typename T> union Tabce;
template<typename T> union Txyze;
template<typename T> union Tabc;
template<typename T> union Txyz;
template<typename T> union Txy;

//
// Position structures
// TODO: Parent template taking a type and a templated union
//

//
// ABC
//
template<typename T>
union Tabc {
  T axis[ABC];
  struct { T a, b, c; };
  Tabc<T>()                                   { a = b = c = 0; }
  Tabc<T>(Tabce<T> &s)                        { *this = s; }
  Tabc<T>& operator=(const Tabce<T> &s)       { memcpy(this, &s, sizeof(Tabc<T>)); return *this; }
  T& operator[](const AxisEnum a)             { return axis[uint8_t(a)]; }
  const T& operator[](const AxisEnum a) const { return axis[uint8_t(a)]; }
};

typedef Tabc<uint32_t> abc32u_t;
typedef Tabc<uint16_t> abc16u_t;
typedef Tabc<uint8_t>  abc8u_t;
typedef Tabc<int32_t>  abc32_t;
typedef Tabc<int16_t>  abc16_t;
typedef Tabc<int8_t>   abc8_t;
typedef Tabc<float>    abc_t;
typedef Tabc<bool>     abc_bool_t;

typedef Tabc<volatile int32_t>  abc32v_t;

//
// ABCE
//
template<typename T>
union Tabce {
  T axis[ABCE];
  struct { T a, b, c, e; };
  Tabce<T>()                                  { a = b = c = e = 0; }
  Tabce<T>(Tabc<T> &s)                        { *this = s; e = 0; }
  Tabce<T>& operator=(const Tabc<T> &s)       { memcpy(this, &s, sizeof(Tabc<T>)); return *this; }
  T& operator[](const AxisEnum a)             { return axis[uint8_t(a)]; }
  const T& operator[](const AxisEnum a) const { return axis[uint8_t(a)]; }
};

typedef Tabce<uint32_t> abce32u_t;
typedef Tabce<uint16_t> abce16u_t;
typedef Tabce<uint8_t>  abce8u_t;
typedef Tabce<int32_t>  abce32_t;
typedef Tabce<int16_t>  abce16_t;
typedef Tabce<int8_t>   abce8_t;
typedef Tabce<float>    abce_t;
typedef Tabce<bool>     abce_bool_t;

typedef Tabce<volatile int32_t> abce32v_t;

//
// XY
//
template<typename T>
union Txy {
  T axis[2];
  struct { T x, y; };
  Txy<T>()                                    { x = y = 0; }
  Txy<T>(Txyz<T> &s)                          { *this = s; }
  Txy<T>(Txyze<T> &s)                         { *this = s; }
  Txy<T>& operator=(const Txyz<T> &s)         { memcpy(this, &s, sizeof(Txy<T>)); return *this; }
  Txy<T>& operator=(const Txyze<T> &s)        { memcpy(this, &s, sizeof(Txy<T>)); return *this; }
  T& operator[](const AxisEnum a)             { return axis[uint8_t(a)]; }
  const T& operator[](const AxisEnum a) const { return axis[uint8_t(a)]; }
};

typedef Txy<uint32_t> xy32u_t;
typedef Txy<uint16_t> xy16u_t;
typedef Txy<uint8_t>  xy8u_t;
typedef Txy<int32_t>  xy32_t;
typedef Txy<int16_t>  xy16_t;
typedef Txy<int8_t>   xy8_t;
typedef Txy<float>    xy_t;
typedef Txy<bool>     xy_bool_t;

//
// XYZ
//
template<typename T>
union Txyz {
  T axis[XYZ];
  struct{ T x, y, z; };
  Txyz<T>()                                   { x = y = z = 0; }
  Txyz<T>(Txy<T> &s)                          { *this = s; z = 0; }
  Txyz<T>(Txyze<T> &s)                        { *this = s; }
  Txyz<T>& operator=(const Txyze<T> &s)       { memcpy(this, &s, sizeof(Txyz<T>)); return *this; }
  T& operator[](const AxisEnum a)             { return axis[uint8_t(a)]; }
  const T& operator[](const AxisEnum a) const { return axis[uint8_t(a)]; }
};

typedef Txyz<uint32_t> xyz32u_t;
typedef Txyz<uint16_t> xyz16u_t;
typedef Txyz<uint8_t>  xyz8u_t;
typedef Txyz<int32_t>  xyz32_t;
typedef Txyz<int16_t>  xyz16_t;
typedef Txyz<int8_t>   xyz8_t;
typedef Txyz<float>    xyz_t;
typedef Txyz<bool>     xyz_bool_t;

//
// XYZE
//
template<typename T>
union Txyze {
  T axis[XYZE];
  struct { T x, y, z, e; };
  Txyze<T>()                                  { x = y = z = e = 0; }
  Txyze<T>(Txy<T> &s)                         { *this = s; z = 0; e = 0; }
  Txyze<T>(Txyz<T> &s)                        { *this = s; e = 0; }
  Txyze<T>& operator=(const Txy<T> &s)        { memcpy(this, &s, sizeof(Txy<T>)); return *this; }
  Txyze<T>& operator=(const Txyz<T> &s)       { memcpy(this, &s, sizeof(Txyz<T>)); return *this; }
  T& operator[](const AxisEnum a)             { return axis[uint8_t(a)]; }
  const T& operator[](const AxisEnum a) const { return axis[uint8_t(a)]; }
};

typedef Txyze<uint32_t> xyze32u_t;
typedef Txyze<uint16_t> xyze16u_t;
typedef Txyze<uint8_t>  xyze8u_t;
typedef Txyze<int32_t>  xyze32_t;
typedef Txyze<int16_t>  xyze16_t;
typedef Txyze<int8_t>   xyze8_t;
typedef Txyze<float>    xyze_t;
typedef Txyze<bool>     xyze_bool_t;

//
// XYZE_N
//
template<typename T>
union T_per_axis {
  T axis[XYZE_N];
  struct { T a, b, c, E_FIELDS; };
  T& E(const uint8_t n) { return axis[E_AXIS_N(n)]; }
  T& operator[](const AxisEnum a)             { return axis[uint8_t(a)]; }
  const T& operator[](const AxisEnum a) const { return axis[uint8_t(a)]; }
};

typedef T_per_axis<float> per_axis_t;
typedef T_per_axis<uint32_t> per_axis32_t;
