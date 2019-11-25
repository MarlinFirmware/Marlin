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

#include "../inc/MarlinConfig.h"

#if ABL_PLANAR || ENABLED(AUTO_BED_LEVELING_UBL)

#include "vector_3.h"

#include <math.h>

/**
 *  vector_3
 */

vector_3 vector_3::cross(const vector_3 &left, const vector_3 &right) {
  const xyz_float_t &lv = left, &rv = right;
  return vector_3(lv.y * rv.z - lv.z * rv.y,      // YZ cross
                  lv.z * rv.x - lv.x * rv.z,      // ZX cross
                  lv.x * rv.y - lv.y * rv.x);     // XY cross
}

vector_3 vector_3::get_normal() const {
  vector_3 normalized = *this;
  normalized.normalize();
  return normalized;
}

void vector_3::normalize() {
  *this *= RSQRT(sq(x) + sq(y) + sq(z));
}

// Apply a rotation to the matrix
void vector_3::apply_rotation(const matrix_3x3 &matrix) {
  const float _x = x, _y = y, _z = z;
  *this = { matrix.vectors[0][0] * _x + matrix.vectors[1][0] * _y + matrix.vectors[2][0] * _z,
            matrix.vectors[0][1] * _x + matrix.vectors[1][1] * _y + matrix.vectors[2][1] * _z,
            matrix.vectors[0][2] * _x + matrix.vectors[1][2] * _y + matrix.vectors[2][2] * _z };
}

void vector_3::debug(PGM_P const title) {
  serialprintPGM(title);
  SERIAL_ECHOPAIR_F(" X", x, 6);
  SERIAL_ECHOPAIR_F(" Y", y, 6);
  SERIAL_ECHOLNPAIR_F(" Z", z, 6);
}

/**
 *  matrix_3x3
 */

void apply_rotation_xyz(const matrix_3x3 &matrix, float &_x, float &_y, float &_z) {
  vector_3 vec = vector_3(_x, _y, _z); vec.apply_rotation(matrix);
  _x = vec.x; _y = vec.y; _z = vec.z;
}

// Reset to identity. No rotate or translate.
void matrix_3x3::set_to_identity() {
  for (uint8_t i = 0; i < 3; i++)
    for (uint8_t j = 0; j < 3; j++)
      vectors[i][j] = float(i == j);
}

// Create a matrix from 3 vector_3 inputs
matrix_3x3 matrix_3x3::create_from_rows(const vector_3 &row_0, const vector_3 &row_1, const vector_3 &row_2) {
  //row_0.debug(PSTR("row_0"));
  //row_1.debug(PSTR("row_1"));
  //row_2.debug(PSTR("row_2"));
  matrix_3x3 new_matrix;
  new_matrix.vectors[0] = row_0;
  new_matrix.vectors[1] = row_1;
  new_matrix.vectors[2] = row_2;
  //new_matrix.debug(PSTR("new_matrix"));
  return new_matrix;
}

// Create a matrix rotated to point towards a target
matrix_3x3 matrix_3x3::create_look_at(const vector_3 &target) {
  const vector_3 z_row = target.get_normal(),
                 x_row = vector_3(1, 0, -target.x / target.z).get_normal(),
                 y_row = vector_3::cross(z_row, x_row).get_normal();

  // x_row.debug(PSTR("x_row"));
  // y_row.debug(PSTR("y_row"));
  // z_row.debug(PSTR("z_row"));

  // create the matrix already correctly transposed
  matrix_3x3 rot = matrix_3x3::create_from_rows(x_row, y_row, z_row);

  // rot.debug(PSTR("rot"));
  return rot;
}

// Get a transposed copy of the matrix
matrix_3x3 matrix_3x3::transpose(const matrix_3x3 &original) {
  matrix_3x3 new_matrix;
  for (uint8_t i = 0; i < 3; i++)
    for (uint8_t j = 0; j < 3; j++)
      new_matrix.vectors[i][j] = original.vectors[j][i];
  return new_matrix;
}

void matrix_3x3::debug(PGM_P const title) {
  if (title != nullptr) {
    serialprintPGM(title);
    SERIAL_EOL();
  }
  for (uint8_t i = 0; i < 3; i++) {
    for (uint8_t j = 0; j < 3; j++) {
      if (vectors[i][j] >= 0.0) SERIAL_CHAR('+');
      SERIAL_ECHO_F(vectors[i][j], 6);
      SERIAL_CHAR(' ');
    }
    SERIAL_EOL();
  }
}

#endif // HAS_ABL_OR_UBL
