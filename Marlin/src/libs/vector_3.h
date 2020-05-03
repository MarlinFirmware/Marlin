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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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

struct vector_3 : xyz_float_t {

  vector_3(const float &_x, const float &_y, const float &_z) { set(_x, _y, _z); }
  vector_3(const xy_float_t   &in) { set(in.x, in.y); }
  vector_3(const xyz_float_t  &in) { set(in.x, in.y, in.z); }
  vector_3(const xyze_float_t &in) { set(in.x, in.y, in.z); }
  vector_3() { reset(); }

  // Factory method
  static vector_3 cross(const vector_3 &a, const vector_3 &b);

  // Modifiers
  void normalize();
  void apply_rotation(const matrix_3x3 &matrix);

  // Accessors
  float get_length() const;
  vector_3 get_normal() const;

  // Operators
  FORCE_INLINE vector_3 operator+(const vector_3 &v) const { vector_3 o = *this; o += v; return o; }
  FORCE_INLINE vector_3 operator-(const vector_3 &v) const { vector_3 o = *this; o -= v; return o; }
  FORCE_INLINE vector_3 operator*(const float    &v) const { vector_3 o = *this; o *= v; return o; }

  void debug(PGM_P const title);
};

struct matrix_3x3 {
  abc_float_t vectors[3];

  // Factory methods
  static matrix_3x3 create_from_rows(const vector_3 &row_0, const vector_3 &row_1, const vector_3 &row_2);
  static matrix_3x3 create_look_at(const vector_3 &target);
  static matrix_3x3 transpose(const matrix_3x3 &original);

  void set_to_identity();

  void debug(PGM_P const title);
};

void apply_rotation_xyz(const matrix_3x3 &rotationMatrix, float &x, float &y, float &z);
FORCE_INLINE void apply_rotation_xyz(const matrix_3x3 &rotationMatrix, xyz_pos_t &pos) {
  apply_rotation_xyz(rotationMatrix, pos.x, pos.y, pos.z);
}
