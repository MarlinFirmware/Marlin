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

/**
  vector_3.cpp - Vector library for bed leveling
  Copyright (c) 2012 Lars Brubaker.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include <math.h>
#include "Marlin.h"

#if HAS_ABL
#include "vector_3.h"

vector_3::vector_3() : x(0), y(0), z(0) { }

vector_3::vector_3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) { }

vector_3 vector_3::cross(vector_3 left, vector_3 right) {
  return vector_3(left.y * right.z - left.z * right.y,
                  left.z * right.x - left.x * right.z,
                  left.x * right.y - left.y * right.x);
}

vector_3 vector_3::operator+(vector_3 v) { return vector_3((x + v.x), (y + v.y), (z + v.z)); }
vector_3 vector_3::operator-(vector_3 v) { return vector_3((x - v.x), (y - v.y), (z - v.z)); }

vector_3 vector_3::get_normal() {
  vector_3 normalized = vector_3(x, y, z);
  normalized.normalize();
  return normalized;
}

float vector_3::get_length() { return SQRT(sq(x) + sq(y) + sq(z)); }

void vector_3::normalize() {
  const float inv_length = 1.0 / get_length();
  x *= inv_length;
  y *= inv_length;
  z *= inv_length;
}

void vector_3::apply_rotation(matrix_3x3 matrix) {
  const float resultX = x * matrix.matrix[3 * 0 + 0] + y * matrix.matrix[3 * 1 + 0] + z * matrix.matrix[3 * 2 + 0],
              resultY = x * matrix.matrix[3 * 0 + 1] + y * matrix.matrix[3 * 1 + 1] + z * matrix.matrix[3 * 2 + 1],
              resultZ = x * matrix.matrix[3 * 0 + 2] + y * matrix.matrix[3 * 1 + 2] + z * matrix.matrix[3 * 2 + 2];
  x = resultX;
  y = resultY;
  z = resultZ;
}

void vector_3::debug(const char * const title) {
  serialprintPGM(title);
  SERIAL_PROTOCOLPGM(" x: ");
  SERIAL_PROTOCOL_F(x, 6);
  SERIAL_PROTOCOLPGM(" y: ");
  SERIAL_PROTOCOL_F(y, 6);
  SERIAL_PROTOCOLPGM(" z: ");
  SERIAL_PROTOCOL_F(z, 6);
  SERIAL_EOL();
}

void apply_rotation_xyz(matrix_3x3 matrix, float &x, float &y, float &z) {
  vector_3 vector = vector_3(x, y, z);
  vector.apply_rotation(matrix);
  x = vector.x;
  y = vector.y;
  z = vector.z;
}

matrix_3x3 matrix_3x3::create_from_rows(vector_3 row_0, vector_3 row_1, vector_3 row_2) {
  //row_0.debug(PSTR("row_0"));
  //row_1.debug(PSTR("row_1"));
  //row_2.debug(PSTR("row_2"));
  matrix_3x3 new_matrix;
  new_matrix.matrix[0] = row_0.x; new_matrix.matrix[1] = row_0.y; new_matrix.matrix[2] = row_0.z;
  new_matrix.matrix[3] = row_1.x; new_matrix.matrix[4] = row_1.y; new_matrix.matrix[5] = row_1.z;
  new_matrix.matrix[6] = row_2.x; new_matrix.matrix[7] = row_2.y; new_matrix.matrix[8] = row_2.z;
  //new_matrix.debug(PSTR("new_matrix"));
  return new_matrix;
}

void matrix_3x3::set_to_identity() {
  matrix[0] = 1; matrix[1] = 0; matrix[2] = 0;
  matrix[3] = 0; matrix[4] = 1; matrix[5] = 0;
  matrix[6] = 0; matrix[7] = 0; matrix[8] = 1;
}

matrix_3x3 matrix_3x3::create_look_at(vector_3 target) {
  vector_3 z_row = target.get_normal();
  vector_3 x_row = vector_3(1, 0, -target.x / target.z).get_normal();
  vector_3 y_row = vector_3::cross(z_row, x_row).get_normal();

  // x_row.debug(PSTR("x_row"));
  // y_row.debug(PSTR("y_row"));
  // z_row.debug(PSTR("z_row"));

  // create the matrix already correctly transposed
  matrix_3x3 rot = matrix_3x3::create_from_rows(x_row, y_row, z_row);

  // rot.debug(PSTR("rot"));
  return rot;
}

matrix_3x3 matrix_3x3::transpose(matrix_3x3 original) {
  matrix_3x3 new_matrix;
  new_matrix.matrix[0] = original.matrix[0]; new_matrix.matrix[1] = original.matrix[3]; new_matrix.matrix[2] = original.matrix[6];
  new_matrix.matrix[3] = original.matrix[1]; new_matrix.matrix[4] = original.matrix[4]; new_matrix.matrix[5] = original.matrix[7];
  new_matrix.matrix[6] = original.matrix[2]; new_matrix.matrix[7] = original.matrix[5]; new_matrix.matrix[8] = original.matrix[8];
  return new_matrix;
}

void matrix_3x3::debug(const char * const title) {
  serialprintPGM(title);
  uint8_t count = 0;
  for (uint8_t i = 0; i < 3; i++) {
    for (uint8_t j = 0; j < 3; j++) {
      if (matrix[count] >= 0.0) SERIAL_PROTOCOLCHAR('+');
      SERIAL_PROTOCOL_F(matrix[count], 6);
      SERIAL_PROTOCOLCHAR(' ');
      count++;
    }
    SERIAL_EOL();
  }
}

#endif // HAS_ABL

