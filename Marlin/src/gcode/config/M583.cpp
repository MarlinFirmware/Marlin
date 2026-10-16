/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#if ENABLED(RUNTIME_AXIS_DIRECTION)

#include "../gcode.h"
#include "../../module/motion.h"
#include "../../module/planner.h"
#include "../../module/stepper.h"

#if ANY(HAS_SYNCED_X_STEPPERS, HAS_SYNCED_Y_STEPPERS, E_DUAL_STEPPER_DRIVERS) || NUM_Z_STEPPERS > 1
  #define HAS_SECONDARY_DIR 1
#endif

/**
 * M583: Set or report the stepper direction (RUNTIME_AXIS_DIRECTION)
 *
 *  X<0|1> Y<0|1> Z<0|1> ... - Normal (0) or inverted (1) axis direction
 *  E<0|1>     - Normal (0) or inverted (1) extruder direction
 *  T<index>   - Extruder index for E (Zero-based. Omit for E0.)
 *  I<index>   - Secondary stepper index (1 for X2, Y2, Z2, E1 with E_DUAL_STEPPER_DRIVERS; 2 for Z3; 3 for Z4.)
 *               The value then sets the stepper's direction relative to its primary stepper.
 *  R          - Reset to the configured INVERT_*_DIR values
 *
 * With no parameters report the current settings.
 *
 * Examples:
 *  M583 X1 Y0   ; Invert X, don't invert Y
 *  M583 Z1 I1   ; Z2 runs opposite to Z
 *  M583 E1 T1   ; Invert E1
 */
void GcodeSuite::M583() {
  if (!parser.seen_any()) return M583_report();

  // Finish queued moves before changing direction
  planner.synchronize();

  if (parser.seen_test('R'))
    motion.reset_axis_direction();
  else {
    #if HAS_SECONDARY_DIR
      const uint8_t index = parser.byteval('I');
    #else
      constexpr uint8_t index = 0;
    #endif

    LOOP_NUM_AXES(i) if (parser.seenval(AXIS_CHAR(i))) {
      const bool inv = parser.value_bool();
      if (index == 0) { motion.axis_inverted.set(i, inv); continue; }
      switch (i) {
        #if HAS_SYNCED_X_STEPPERS
          case X_AXIS: if (index == 1) { motion.x2_vs_x_inverted = inv; continue; } break;
        #endif
        #if HAS_SYNCED_Y_STEPPERS
          case Y_AXIS: if (index == 1) { motion.y2_vs_y_inverted = inv; continue; } break;
        #endif
        #if NUM_Z_STEPPERS > 1
          case Z_AXIS: switch (index) {
            case 1: motion.z2_vs_z_inverted = inv; continue;
            #if NUM_Z_STEPPERS > 2
              case 2: motion.z3_vs_z_inverted = inv; continue;
            #endif
            #if NUM_Z_STEPPERS > 3
              case 3: motion.z4_vs_z_inverted = inv; continue;
            #endif
          } break;
        #endif
        default: break;
      }
      SERIAL_ECHOLNPGM(GCODE_ERR_MSG("No stepper ", C(AXIS_CHAR(i)), " I", index));
    }

    #if HAS_EXTRUDERS
      if (parser.seenval('E')) {
        const bool inv = parser.value_bool();
        const uint8_t e = parser.byteval('T');
        #if ENABLED(E_DUAL_STEPPER_DRIVERS)
          if (index == 1)
            motion.e1_vs_e0_inverted = inv;
          else
        #endif
        if (index == 0 && e < E_STEPPERS)
          motion.extruder_inverted[e] = inv;
        else
          SERIAL_ECHOLNPGM(GCODE_ERR_MSG("No stepper E T", e, " I", index));
      }
    #endif
  }

  // DIR pins only change with the move direction, so apply the new directions now
  stepper.apply_directions();
}

void GcodeSuite::M583_report(const bool forReplay/*=true*/) {
  TERN_(MARLIN_SMALL_BUILD, return);

  report_heading(forReplay, F("Stepper Direction"));

  #if NUM_AXES
    report_echo_start(forReplay);
    SERIAL_ECHOPGM("  M583");
    LOOP_NUM_AXES(i) { SERIAL_CHAR(' ', AXIS_CHAR(i)); SERIAL_ECHO(int(motion.axis_inverted[i])); }
    SERIAL_EOL();
  #endif

  #define _SECONDARY_REPORT(A,N,V) do{ report_echo_start(forReplay); SERIAL_ECHOLNPGM("  M583 " STRINGIFY(A), int(V), " I" STRINGIFY(N)); }while(0)
  TERN_(HAS_SYNCED_X_STEPPERS, _SECONDARY_REPORT(X, 1, motion.x2_vs_x_inverted));
  TERN_(HAS_SYNCED_Y_STEPPERS, _SECONDARY_REPORT(Y, 1, motion.y2_vs_y_inverted));
  #if NUM_Z_STEPPERS > 1
    _SECONDARY_REPORT(Z, 1, motion.z2_vs_z_inverted);
  #endif
  #if NUM_Z_STEPPERS > 2
    _SECONDARY_REPORT(Z, 2, motion.z3_vs_z_inverted);
  #endif
  #if NUM_Z_STEPPERS > 3
    _SECONDARY_REPORT(Z, 3, motion.z4_vs_z_inverted);
  #endif
  TERN_(E_DUAL_STEPPER_DRIVERS, _SECONDARY_REPORT(E, 1, motion.e1_vs_e0_inverted));

  #if HAS_EXTRUDERS
    for (uint8_t e = 0; e < E_STEPPERS; ++e) {
      report_echo_start(forReplay);
      SERIAL_ECHOLNPGM("  M583 T", e, " E", int(motion.extruder_inverted[e]));
    }
  #endif
}

#endif // RUNTIME_AXIS_DIRECTION
