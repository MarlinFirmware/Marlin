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

void refresh_zprobe_zoffset(const bool no_babystep/*=false*/) {
  static float last_zoffset = NAN;

  if (!isnan(last_zoffset)) {

    #if ENABLED(AUTO_BED_LEVELING_BILINEAR) || ENABLED(BABYSTEP_ZPROBE_OFFSET) || ENABLED(DELTA)
      const float diff = zprobe_zoffset - last_zoffset;
    #endif

    #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
      // Correct bilinear grid for new probe offset
      if (diff) {
        for (uint8_t x = 0; x < GRID_MAX_POINTS_X; x++)
          for (uint8_t y = 0; y < GRID_MAX_POINTS_Y; y++)
            z_values[x][y] -= diff;
      }
      #if ENABLED(ABL_BILINEAR_SUBDIVISION)
        bed_level_virt_interpolate();
      #endif
    #endif

    #if ENABLED(BABYSTEP_ZPROBE_OFFSET)
      if (!no_babystep && leveling_is_active())
        thermalManager.babystep_axis(Z_AXIS, -LROUND(diff * planner.axis_steps_per_mm[Z_AXIS]));
    #else
      UNUSED(no_babystep);
    #endif

    #if ENABLED(DELTA) // correct the delta_height
      home_offset[Z_AXIS] -= diff;
    #endif
  }

  last_zoffset = zprobe_zoffset;
}

void gcode_M851() {
  SERIAL_ECHO_START();
  SERIAL_ECHOPGM(MSG_ZPROBE_ZOFFSET " ");
  if (parser.seen('Z')) {
    const float value = parser.value_linear_units();
    if (WITHIN(value, Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX)) {
      zprobe_zoffset = value;
      refresh_zprobe_zoffset();
      SERIAL_ECHO(zprobe_zoffset);
    }
    else
      SERIAL_ECHOPGM(MSG_Z_MIN " " STRINGIFY(Z_PROBE_OFFSET_RANGE_MIN) " " MSG_Z_MAX " " STRINGIFY(Z_PROBE_OFFSET_RANGE_MAX));
  }
  else
    SERIAL_ECHOPAIR(": ", zprobe_zoffset);

  SERIAL_EOL();
}
