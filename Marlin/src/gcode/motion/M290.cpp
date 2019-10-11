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

#include "../../inc/MarlinConfig.h"

#if ENABLED(BABYSTEPPING)

#include "../gcode.h"
#include "../../feature/babystep.h"
#include "../../module/probe.h"
#include "../../module/temperature.h"
#include "../../module/planner.h"

#if ENABLED(BABYSTEP_ZPROBE_OFFSET)
  #include "../../core/serial.h"
#endif

#if ENABLED(MESH_BED_LEVELING)
  #include "../../feature/bedlevel/bedlevel.h"
#endif

#if ENABLED(BABYSTEP_ZPROBE_OFFSET)

  FORCE_INLINE void apply_offset_steps(const uint8_t axis, const float &offs) {
    if (axis == Z_AXIS && parser.boolval('P', true)) {
      #if ENABLED(BABYSTEP_ZPROBE_OFFSET)
        if (true
          #if ENABLED(BABYSTEP_HOTEND_Z_OFFSET)
            && active_extruder == 0
          #endif
        ) {
          NOLESS(offs, Z_PROBE_OFFSET_RANGE_MIN - zprobe_zoffset);
          NOMORE(offs, Z_PROBE_OFFSET_RANGE_MAX - zprobe_zoffset);
        }
        #if ENABLED(BABYSTEP_HOTEND_Z_OFFSET)
          else if (offs > hotend_offset[active_extruder].z + hotend_offset_limit.z)
            offs = -hotend_offset_limit.z - zprobe_zoffset;
          else if (offs < hotend_offset[active_extruder].z - hotend_offset_limit.z)
            offs = hotend_offset_limit.z - hotend_offset[active_extruder].z;
        #endif
        mod_zprobe_zoffset(offs);
      #endif
    }
    #if ENABLED(BABYSTEP_XY) && EXTRUDERS > 1
      else if (axis == X_AXIS) {
        if (offs > hotend_offset[active_extruder].x + hotend_offset_limit.x)
          offs = -hotend_offset_limit.x - zprobe_zoffset;
        else if (offs < hotend_offset[active_extruder].x - hotend_offset_limit.x)
          offs = hotend_offset_limit.x - hotend_offset[active_extruder].x;
      }
      else if (axis == Y_AXIS) {
        if (offs > hotend_offset[active_extruder].y + hotend_offset_limit.y)
          offs = -hotend_offset_limit.y - zprobe_zoffset;
        else if (offs < hotend_offset[active_extruder].y - hotend_offset_limit.y)
          offs = hotend_offset_limit.y - hotend_offset[active_extruder].y;
      }
    #endif
    babystep.add_mm((AxisEnum)axis, offs);
  }

  FORCE_INLINE void mod_zprobe_zoffset(const float &offs) {
    if (true
      #if ENABLED(BABYSTEP_HOTEND_Z_OFFSET)
        && active_extruder == 0
      #endif
    ) {
      probe_offset.z += offs;
      SERIAL_ECHO_START();
      SERIAL_ECHOLNPAIR(MSG_PROBE_OFFSET MSG_Z ": ", probe_offset.z);
    }
    else {
      #if ENABLED(BABYSTEP_HOTEND_Z_OFFSET)
        hotend_offset[active_extruder].z -= offs;
        SERIAL_ECHO_START();
        SERIAL_ECHOLNPAIR(MSG_PROBE_OFFSET MSG_Z ": ", hotend_offset[active_extruder].z);
      #endif
    }
  }

#endif

/**
 * M290: Babystepping
 *
 * Send 'R' or no parameters for a report.
 *
 *  X<linear> - Distance to step X
 *  Y<linear> - Distance to step Y
 *  Z<linear> - Distance to step Z
 *  S<linear> - Distance to step Z (alias for Z)
 *
 * With BABYSTEP_ZPROBE_OFFSET:
 *  P0 - Don't adjust the Z probe offset
 */
void GcodeSuite::M290() {
  #if ENABLED(BABYSTEP_XY)
    for (uint8_t a = X_AXIS; a <= Z_AXIS; a++)
      if (parser.seenval(axis_codes[a]) || (a == Z_AXIS && parser.seenval('S'))) {
        const float offs = constrain(parser.value_axis_units((AxisEnum)a), -2, 2);
        apply_offset_steps(a, offs);
        #if ENABLED(BABYSTEP_ZPROBE_OFFSET)
          if (a == Z_AXIS && parser.boolval('P', true)) mod_zprobe_zoffset(offs);
        #endif
      }
  #else
    if (parser.seenval('Z') || parser.seenval('S')) {
      const float offs = constrain(parser.value_axis_units(Z_AXIS), -2, 2);
      apply_offset_steps(Z_AXIS, offs);
      #if ENABLED(BABYSTEP_ZPROBE_OFFSET)
        if (parser.boolval('P', true)) mod_zprobe_zoffset(offs);
      #endif
    }
  #endif

  if (!parser.seen("XYZ") || parser.seen('R')) {
    SERIAL_ECHO_START();

    #if ENABLED(BABYSTEP_ZPROBE_OFFSET)
      SERIAL_ECHOLNPAIR(MSG_PROBE_OFFSET " " MSG_Z, probe_offset.z);
    #endif

    #if ENABLED(BABYSTEP_HOTEND_Z_OFFSET)
    {
      SERIAL_ECHOLNPAIR("Hotend ", int(active_extruder), "Offset"
        #if ENABLED(BABYSTEP_XY)
          " X", hotend_offset[active_extruder].x,
          " Y", hotend_offset[active_extruder].y,
        #endif
        " Z", hotend_offset[active_extruder].z
      );
    }
    #endif

    #if ENABLED(MESH_BED_LEVELING)
      SERIAL_ECHOLNPAIR("MBL Adjust Z", mbl.z_offset);
    #endif

    #if ENABLED(BABYSTEP_DISPLAY_TOTAL)
    {
      SERIAL_ECHOLNPAIR("Babystep"
        #if ENABLED(BABYSTEP_XY)
          " X", babystep.axis_total[X_AXIS],
          " Y", babystep.axis_total[Y_AXIS],
        #endif
        " Z", babystep.axis_total[Z_AXIS]
      );
    }
    #endif
  }
}

#endif // BABYSTEPPING
