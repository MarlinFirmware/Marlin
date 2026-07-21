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

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(RUNTIME_AXIS_DIRECTION)

#include "../gcode.h"
#include "../../module/motion.h"

/**
 * M670: Set or report runtime axis direction inversion
 *
 * Parameters:
 *   No parameters - Report current axis direction settings
 *   X Y Z [I] [J] [K] [U] [V] [W] - Specify axis to configure
 *   E<n> - Specify extruder n (0-7) to configure
 *   S<0|1> - Direction value (0=normal, 1=inverted)
 *   R - Reset all axes to compile-time defaults
 *
 * Secondary steppers (relative to primary):
 *   X2 Y2 Z2 Z3 Z4 - Specify secondary stepper (requires S parameter)
 *     Example: M670 X2 S1 means X2 motor runs opposite direction to X
 *   H - E1 direction relative to E0 for E_DUAL_STEPPER_DRIVERS (requires S parameter)
 *     Example: M670 H S1 means E1 motor runs opposite direction to E0
 *
 * Examples:
 *   M670           ; Report current settings
 *   M670 X S1      ; Invert X axis direction
 *   M670 Y S0      ; Set Y to normal direction
 *   M670 X S1 Y S0 Z S1  ; Set multiple axes
 *   M670 E0 S1     ; Invert extruder 0
 *   M670 E1 S0     ; Set extruder 1 to normal
 *   M670 X2 S1     ; X2 runs opposite to X
 *   M670 Z2 S0     ; Z2 runs same direction as Z
 *   M670 E1E0 S1   ; E1 runs opposite to E0 (dual E steppers)
 *   M670 R         ; Reset to compile-time defaults
 */
void GcodeSuite::M670() {
  // Reset to defaults if R parameter is given
  if (parser.seen('R')) {
    motion.reset_axis_direction();
    SERIAL_ECHOLNPGM("Axis directions reset to defaults");
    return;
  }

  // S parameter is required for setting values
  if (!parser.seenval('S')) {
    // No S parameter - just report current settings
    M670_report();
    return;
  }

  const bool s_val = parser.value_bool();
  bool any_axis_set = false;

  // Check for secondary stepper parameters (X2, Y2, Z2, Z3, Z4)
  #if ENABLED(X_DUAL_STEPPER_DRIVERS)
    if (parser.seenval('X') && parser.value_int() == 2) {
      motion.x2_vs_x_inverted = s_val;
      any_axis_set = true;
    }
    else
  #endif
  if (parser.seen('X')) {
    motion.axis_inverted.set(X_AXIS, s_val);
    any_axis_set = true;
  }

  #if ENABLED(Y_DUAL_STEPPER_DRIVERS)
    if (parser.seenval('Y') && parser.value_int() == 2) {
      motion.y2_vs_y_inverted = s_val;
      any_axis_set = true;
    }
    else
  #endif
  if (parser.seen('Y')) {
    motion.axis_inverted.set(Y_AXIS, s_val);
    any_axis_set = true;
  }

  // Z axis and multi-Z steppers
  #if NUM_Z_STEPPERS >= 4
    if (parser.seenval('Z') && parser.value_int() == 4) {
      motion.z4_vs_z_inverted = s_val;
      any_axis_set = true;
    }
    else
  #endif
  #if NUM_Z_STEPPERS >= 3
    if (parser.seenval('Z') && parser.value_int() == 3) {
      motion.z3_vs_z_inverted = s_val;
      any_axis_set = true;
    }
    else
  #endif
  #if NUM_Z_STEPPERS >= 2
    if (parser.seenval('Z') && parser.value_int() == 2) {
      motion.z2_vs_z_inverted = s_val;
      any_axis_set = true;
    }
    else
  #endif
  if (parser.seen('Z')) {
    motion.axis_inverted.set(Z_AXIS, s_val);
    any_axis_set = true;
  }

  // Handle remaining primary axes
  LOOP_NUM_AXES(i) {
    if (i != X_AXIS && i != Y_AXIS && i != Z_AXIS) {  // Already handled X, Y, Z above
      if (parser.seen(AXIS_CHAR(i))) {
        motion.axis_inverted.set(i, s_val);
        any_axis_set = true;
      }
    }
  }
  
  #if HAS_EXTRUDERS
    // Handle E1E0 for dual E steppers (check for 'H' parameter for "Helper extruder")
    #if ENABLED(E_DUAL_STEPPER_DRIVERS)
      if (parser.seen('H')) {
        motion.e1_vs_e0_inverted = s_val;
        any_axis_set = true;
      }
      else
    #endif
    // E parameter specifies extruder index
    if (parser.seenval('E')) {
      const int8_t target_e = parser.value_int();
      if (WITHIN(target_e, 0, E_STEPPERS - 1)) {
        motion.extruder_inverted[target_e] = s_val;
        any_axis_set = true;
      }
      else {
        SERIAL_ECHOLNPGM("?Invalid extruder index");
      }
    }
  #endif

  // If any axis was set, report the change
  if (any_axis_set) {
    SERIAL_ECHOLNPGM("Axis directions updated");
    M670_report();
  }
  else {
    SERIAL_ECHOLNPGM("?No axis specified");
  }
}

void GcodeSuite::M670_report(const bool forReplay/*=true*/) {
  TERN_(MARLIN_SMALL_BUILD, return);

  report_heading_etc(forReplay, F("Axis Direction Inversion"));
  
  // Report main axes
  #if NUM_AXES
    SERIAL_ECHO_START();
    SERIAL_ECHOPGM("  M670");
    NUM_AXIS_CODE(
      SERIAL_ECHOPGM(" X S", motion.axis_inverted[X_AXIS] ? 1 : 0),
      SERIAL_ECHOPGM(" Y S", motion.axis_inverted[Y_AXIS] ? 1 : 0),
      SERIAL_ECHOPGM(" Z S", motion.axis_inverted[Z_AXIS] ? 1 : 0),
      SERIAL_ECHOPGM(" I S", motion.axis_inverted[I_AXIS] ? 1 : 0),
      SERIAL_ECHOPGM(" J S", motion.axis_inverted[J_AXIS] ? 1 : 0),
      SERIAL_ECHOPGM(" K S", motion.axis_inverted[K_AXIS] ? 1 : 0),
      SERIAL_ECHOPGM(" U S", motion.axis_inverted[U_AXIS] ? 1 : 0),
      SERIAL_ECHOPGM(" V S", motion.axis_inverted[V_AXIS] ? 1 : 0),
      SERIAL_ECHOPGM(" W S", motion.axis_inverted[W_AXIS] ? 1 : 0)
    );
    SERIAL_EOL();
  #endif
  
  // Report secondary steppers (relative to primary)
  #if ENABLED(X_DUAL_STEPPER_DRIVERS)
    SERIAL_ECHOLNPGM("  M670 X2 S", motion.x2_vs_x_inverted ? 1 : 0, " ; X2 vs X");
  #endif
  #if ENABLED(Y_DUAL_STEPPER_DRIVERS)
    SERIAL_ECHOLNPGM("  M670 Y2 S", motion.y2_vs_y_inverted ? 1 : 0, " ; Y2 vs Y");
  #endif
  #if NUM_Z_STEPPERS >= 2
    SERIAL_ECHOLNPGM("  M670 Z2 S", motion.z2_vs_z_inverted ? 1 : 0, " ; Z2 vs Z");
  #endif
  #if NUM_Z_STEPPERS >= 3
    SERIAL_ECHOLNPGM("  M670 Z3 S", motion.z3_vs_z_inverted ? 1 : 0, " ; Z3 vs Z");
  #endif
  #if NUM_Z_STEPPERS >= 4
    SERIAL_ECHOLNPGM("  M670 Z4 S", motion.z4_vs_z_inverted ? 1 : 0, " ; Z4 vs Z");
  #endif
  #if ENABLED(E_DUAL_STEPPER_DRIVERS)
    SERIAL_ECHOLNPGM("  M670 H S", motion.e1_vs_e0_inverted ? 1 : 0, " ; E1 vs E0");
  #endif
  
  // Report each extruder on its own line
  #if HAS_EXTRUDERS
    for (uint8_t e = 0; e < E_STEPPERS; ++e) {
      SERIAL_ECHO_START();
      SERIAL_ECHOPGM("  M670 E", e, " S", motion.extruder_inverted[e] ? 1 : 0);
      SERIAL_EOL();
    }
  #endif
}

#endif // RUNTIME_AXIS_DIRECTION
