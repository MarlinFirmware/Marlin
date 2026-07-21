/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2024 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../inc/MarlinConfig.h"

#if ENABLED(RUNTIME_HOMING_DIRECTION)

#include "../gcode.h"
#include "../../module/motion.h"

/**
 * M671: Set or report runtime homing direction
 *
 * Main axes (requires S parameter with value -1 or 1):
 *   X Y Z I J K U V W - Specify axis letter, S-1 = home to MIN, S1 = home to MAX
 *     Example: M671 X S-1 means X homes to MIN endstop
 *     Example: M671 Z S1 means Z homes to MAX endstop
 *
 * Special commands:
 *   R - Reset all axes to compile-time defaults
 *   (no parameters) - Report current homing directions
 *
 * Examples:
 *   M671              Report current homing direction settings
 *   M671 X S-1        Set X to home towards MIN endstop
 *   M671 Z S1         Set Z to home towards MAX endstop
 *   M671 X S-1 Y S-1  Set multiple axes at once
 *   M671 R            Reset all to compile-time defaults
 */
void GcodeSuite::M671() {
  bool report = true;

  // Handle reset command
  if (parser.seen('R')) {
    motion.reset_axis_home_dir();
    report = false;
    SERIAL_ECHOLNPGM("Homing directions reset to defaults");
  }

  // Handle individual axis parameters
  #if HAS_X_AXIS
    if (parser.seenval('X')) {
      const int8_t dir = parser.value_byte();
      if (dir == -1 || dir == 1) {
        motion.axis_home_dir[X_AXIS] = dir;
        report = false;
      }
      else SERIAL_ECHOLNPGM("?Invalid X direction (use -1 or 1)");
    }
  #endif

  #if HAS_Y_AXIS
    if (parser.seenval('Y')) {
      const int8_t dir = parser.value_byte();
      if (dir == -1 || dir == 1) {
        motion.axis_home_dir[Y_AXIS] = dir;
        report = false;
      }
      else SERIAL_ECHOLNPGM("?Invalid Y direction (use -1 or 1)");
    }
  #endif

  #if HAS_Z_AXIS
    if (parser.seenval('Z')) {
      const int8_t dir = parser.value_byte();
      #if ENABLED(DELTA)
        // Delta printers always home Z to MAX (direction 1)
        if (dir == 1) {
          motion.axis_home_dir[Z_AXIS] = dir;
          report = false;
        }
        else SERIAL_ECHOLNPGM("?Delta Z must home to MAX (use S1 only)");
      #else
        if (dir == -1 || dir == 1) {
          motion.axis_home_dir[Z_AXIS] = dir;
          report = false;
        }
        else SERIAL_ECHOLNPGM("?Invalid Z direction (use -1 or 1)");
      #endif
    }
  #endif

  #if HAS_I_AXIS
    if (parser.seenval('I')) {
      const int8_t dir = parser.value_byte();
      if (dir == -1 || dir == 1) {
        motion.axis_home_dir[I_AXIS] = dir;
        report = false;
      }
      else SERIAL_ECHOLNPGM("?Invalid I direction (use -1 or 1)");
    }
  #endif

  #if HAS_J_AXIS
    if (parser.seenval('J')) {
      const int8_t dir = parser.value_byte();
      if (dir == -1 || dir == 1) {
        motion.axis_home_dir[J_AXIS] = dir;
        report = false;
      }
      else SERIAL_ECHOLNPGM("?Invalid J direction (use -1 or 1)");
    }
  #endif

  #if HAS_K_AXIS
    if (parser.seenval('K')) {
      const int8_t dir = parser.value_byte();
      if (dir == -1 || dir == 1) {
        motion.axis_home_dir[K_AXIS] = dir;
        report = false;
      }
      else SERIAL_ECHOLNPGM("?Invalid K direction (use -1 or 1)");
    }
  #endif

  #if HAS_U_AXIS
    if (parser.seenval('U')) {
      const int8_t dir = parser.value_byte();
      if (dir == -1 || dir == 1) {
        motion.axis_home_dir[U_AXIS] = dir;
        report = false;
      }
      else SERIAL_ECHOLNPGM("?Invalid U direction (use -1 or 1)");
    }
  #endif

  #if HAS_V_AXIS
    if (parser.seenval('V')) {
      const int8_t dir = parser.value_byte();
      if (dir == -1 || dir == 1) {
        motion.axis_home_dir[V_AXIS] = dir;
        report = false;
      }
      else SERIAL_ECHOLNPGM("?Invalid V direction (use -1 or 1)");
    }
  #endif

  #if HAS_W_AXIS
    if (parser.seenval('W')) {
      const int8_t dir = parser.value_byte();
      if (dir == -1 || dir == 1) {
        motion.axis_home_dir[W_AXIS] = dir;
        report = false;
      }
      else SERIAL_ECHOLNPGM("?Invalid W direction (use -1 or 1)");
    }
  #endif

  if (report) M671_report();
}

void GcodeSuite::M671_report(const bool forReplay/*=true*/) {
  TERN_(MARLIN_SMALL_BUILD, return);

  report_heading_etc(forReplay, F("Homing Direction"));

  // Report all axes on one line
  #if NUM_AXES
    SERIAL_ECHO_START();
    SERIAL_ECHOPGM("  M671");
    NUM_AXIS_CODE(
      SERIAL_ECHOPGM(" X S", motion.axis_home_dir[X_AXIS]),
      SERIAL_ECHOPGM(" Y S", motion.axis_home_dir[Y_AXIS]),
      SERIAL_ECHOPGM(" Z S", motion.axis_home_dir[Z_AXIS]),
      SERIAL_ECHOPGM(" I S", motion.axis_home_dir[I_AXIS]),
      SERIAL_ECHOPGM(" J S", motion.axis_home_dir[J_AXIS]),
      SERIAL_ECHOPGM(" K S", motion.axis_home_dir[K_AXIS]),
      SERIAL_ECHOPGM(" U S", motion.axis_home_dir[U_AXIS]),
      SERIAL_ECHOPGM(" V S", motion.axis_home_dir[V_AXIS]),
      SERIAL_ECHOPGM(" W S", motion.axis_home_dir[W_AXIS])
    );
    SERIAL_ECHOLNPGM(" ; -1=MIN, 1=MAX");
  #endif
}

#endif // RUNTIME_HOMING_DIRECTION
