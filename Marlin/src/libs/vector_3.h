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

/**
 * vector_3.cpp - Vector library for bed leveling
 * Copyright (c) 2012 Lars Brubaker.  All right reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "../core/types.h"

class matrix_3x3;

struct vector_3 {
  union {
    struct { float x, y, z; };
    float pos[3];
  };
  vector_3(const_float_t _x, const_float_t _y, const_float_t _z) : x(_x), y(_y), z(_z) {}
  vector_3(const xy_float_t   &in) { TERN_(HAS_X_AXIS, x = in.x); TERN_(HAS_Y_AXIS, y = in.y); }
  vector_3(const xyz_float_t  &in) { TERN_(HAS_X_AXIS, x = in.x); TERN_(HAS_Y_AXIS, y = in.y); TERN_(HAS_Z_AXIS, z = in.z); }
  vector_3(const xyze_float_t &in) { TERN_(HAS_X_AXIS, x = in.x); TERN_(HAS_Y_AXIS, y = in.y); TERN_(HAS_Z_AXIS, z = in.z); }
  vector_3() { x = y = z = 0; }

  // Factory method
  static vector_3 cross(const vector_3 &a, const vector_3 &b);

  // Modifiers
  void normalize();
  void apply_rotation(const matrix_3x3 &matrix);

  // Accessors
  float magnitude() const;
  vector_3 get_normal() const;

  // Operators
        float& operator[](const int n)       { return pos[n]; }
  const float& operator[](const int n) const { return pos[n]; }

  vector_3& operator*=(const float &v)  { x *= v; y *= v; z *= v; return *this; }
  vector_3 operator+(const vector_3 &v) { return vector_3(x + v.x, y + v.y, z + v.z); }
  vector_3 operator-(const vector_3 &v) { return vector_3(x - v.x, y - v.y, z - v.z); }
  vector_3 operator*(const float &v)    { return vector_3(x * v, y * v, z * v); }

  operator xy_float_t() { return xy_float_t({ TERN_(HAS_X_AXIS, x) OPTARG(HAS_Y_AXIS, y) }); }
  operator xyz_float_t() { return xyz_float_t({ TERN_(HAS_X_AXIS, x) OPTARG(HAS_Y_AXIS, y) OPTARG(HAS_Z_AXIS, z) }); }

  void debug(FSTR_P const title);
};

struct matrix_3x3 {
  vector_3 vectors[3];

  // Factory methods
  static matrix_3x3 create_from_rows(const vector_3 &row_0, const vector_3 &row_1, const vector_3 &row_2);
  static matrix_3x3 create_look_at(const vector_3 &target);
  static matrix_3x3 transpose(const matrix_3x3 &original);

  void set_to_identity();

  void debug(FSTR_P const title);

  void apply_rotation_xyz(float &x, float &y, float &z);
};
