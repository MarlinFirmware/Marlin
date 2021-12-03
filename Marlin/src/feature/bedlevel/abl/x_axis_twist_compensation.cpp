/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#include "../../../inc/MarlinConfig.h"

#if ENABLED(X_AXIS_TWIST_COMPENSATION)

#include "../bedlevel.h"

xatc_points xatc_z_values;
float xatc_spacing, xatc_start;

void print_xatc_points() {
  SERIAL_ECHOLNPGM(" X-Twist Correction:");
  LOOP_L_N(x, XATC_MAX_POINTS) {
    SERIAL_CHAR(' ');
    if (!isnan(xatc_z_values[x])) {
      if (xatc_z_values[x] >= 0) SERIAL_CHAR('+');
      SERIAL_ECHO_F(xatc_z_values[x], 3);
    }
    else {
      LOOP_L_N(i, 6)
        SERIAL_CHAR(i ? '=' : ' ');
    }
  }
  SERIAL_EOL();
}

float lerp(const_float_t t, const_float_t a, const_float_t b) { return  a + t * (b - a); }

float x_axis_twist_compensation(const xy_pos_t &raw) {
  float t = (raw.x - xatc_start) / xatc_spacing;
  int i = FLOOR(t);
  LIMIT(i, 0, XATC_MAX_POINTS - 2);
  t -= i;
  return lerp(t, xatc_z_values[i], xatc_z_values[i + 1]);
}

#endif // X_AXIS_TWIST_COMPENSATION
