/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../gcode.h"
#include "../../module/planner.h"

void report_M92(
  #if NUM_SERIAL > 1
    const int8_t port,
  #endif
  const bool echo=true, const int8_t e=-1
) {
  if (echo) SERIAL_ECHO_START_P(port); else SERIAL_CHAR(' ');
  SERIAL_ECHOPAIR_P(port, " M92 X", LINEAR_UNIT(planner.settings.axis_steps_per_mm[X_AXIS]));
  SERIAL_ECHOPAIR_P(port, " Y", LINEAR_UNIT(planner.settings.axis_steps_per_mm[Y_AXIS]));
  SERIAL_ECHOPAIR_P(port, " Z", LINEAR_UNIT(planner.settings.axis_steps_per_mm[Z_AXIS]));
  #if DISABLED(DISTINCT_E_FACTORS)
    SERIAL_ECHOPAIR_P(port, " E", VOLUMETRIC_UNIT(planner.settings.axis_steps_per_mm[E_AXIS]));
  #endif
  SERIAL_EOL_P(port);

  #if ENABLED(DISTINCT_E_FACTORS)
    for (uint8_t i = 0; i < E_STEPPERS; i++) {
      if (e >= 0 && i != e) continue;
      if (echo) SERIAL_ECHO_START_P(port); else SERIAL_CHAR(' ');
      SERIAL_ECHOPAIR_P(port, " M92 T", (int)i);
      SERIAL_ECHOLNPAIR_P(port, " E", VOLUMETRIC_UNIT(planner.settings.axis_steps_per_mm[E_AXIS_N(i)]));
    }
  #endif
}

/**
 * M92: Set axis steps-per-unit for one or more axes, X, Y, Z, and E.
 *      (Follows the same syntax as G92)
 *
 *      With multiple extruders use T to specify which one.
 *
 *      If no argument is given print the current values.
 *
 *    With MAGIC_NUMBERS_GCODE:
 *      Use 'H' and/or 'L' to get ideal layer-height information.
 *      'H' specifies micro-steps to use. We guess if it's not supplied.
 *      'L' specifies a desired layer height. Nearest good heights are shown.
 */
void GcodeSuite::M92() {

  const int8_t target_extruder = get_target_extruder_from_command();
  if (target_extruder < 0) return;

  // No arguments? Show M92 report.
  if (!parser.seen("XYZE"
    #if ENABLED(MAGIC_NUMBERS_GCODE)
      "HL"
    #endif
  )) return report_M92(
    #if NUM_SERIAL > 1
      command_queue_port[cmd_queue_index_r],
    #endif
    true, target_extruder
  );

  LOOP_XYZE(i) {
    if (parser.seenval(axis_codes[i])) {
      if (i == E_AXIS) {
        const float value = parser.value_per_axis_units((AxisEnum)(E_AXIS_N(target_extruder)));
        if (value < 20) {
          float factor = planner.settings.axis_steps_per_mm[E_AXIS_N(target_extruder)] / value; // increase e constants if M92 E14 is given for netfab.
          #if HAS_CLASSIC_JERK && (DISABLED(JUNCTION_DEVIATION) || DISABLED(LIN_ADVANCE))
            planner.max_jerk[E_AXIS] *= factor;
          #endif
          planner.settings.max_feedrate_mm_s[E_AXIS_N(target_extruder)] *= factor;
          planner.max_acceleration_steps_per_s2[E_AXIS_N(target_extruder)] *= factor;
        }
        planner.settings.axis_steps_per_mm[E_AXIS_N(target_extruder)] = value;
      }
      else {
        planner.settings.axis_steps_per_mm[i] = parser.value_per_axis_units((AxisEnum)i);
      }
    }
  }
  planner.refresh_positioning();

  #if ENABLED(MAGIC_NUMBERS_GCODE)
    #ifndef Z_MICROSTEPS
      #define Z_MICROSTEPS 16
    #endif
    const float wanted = parser.floatval('L');
    if (parser.seen('H') || wanted) {
      const uint16_t argH = parser.ushortval('H'),
                     micro_steps = argH ? argH : Z_MICROSTEPS;
      const float z_full_step_mm = micro_steps * planner.steps_to_mm[Z_AXIS];
      SERIAL_ECHO_START();
      SERIAL_ECHOPAIR("{ micro_steps:", micro_steps);
      SERIAL_ECHOPAIR(", z_full_step_mm:", z_full_step_mm);
      if (wanted) {
        const float best = uint16_t(wanted / z_full_step_mm) * z_full_step_mm;
        SERIAL_ECHOPGM(", best:[");
        SERIAL_ECHO(best);
        if (best != wanted) { SERIAL_CHAR(','); SERIAL_ECHO(best + z_full_step_mm); }
        SERIAL_CHAR(']');
      }
      SERIAL_ECHOLNPGM(" }");
    }
  #endif
}
