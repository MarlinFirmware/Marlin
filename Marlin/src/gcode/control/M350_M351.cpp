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

#include "../../inc/MarlinConfig.h"

#if HAS_MICROSTEPS

#include "../gcode.h"
#include "../../module/stepper.h"

#if NUM_AXES == XYZ && EXTRUDERS >= 1
  #define HAS_M350_B_PARAM 1  // "5th axis" (after E0) for an original XYZEB setup.
#endif

/**
 * M350: Set axis microstepping modes. S sets mode for all drivers.
 *
 * Warning: Steps-per-unit remains unchanged.
 */
void GcodeSuite::M350() {
  if (parser.seen('S')) LOOP_DISTINCT_AXES(i) stepper.microstep_mode(i, parser.value_byte());
  LOOP_LOGICAL_AXES(i) if (parser.seenval(AXIS_CHAR(i))) stepper.microstep_mode(i, parser.value_byte());
  TERN_(HAS_M350_B_PARAM, if (parser.seenval('B')) stepper.microstep_mode(E_AXIS + 1, parser.value_byte()));
  stepper.microstep_readings();
}

/**
 * M351: Toggle MS1 MS2 pins directly with axis codes X Y Z . . . E [B]
 *       S# determines MS1, MS2 or MS3, X# sets the pin high/low.
 *
 *       Parameter 'B' sets "5th axis" (after E0) only for an original XYZEB setup.
 */
void GcodeSuite::M351() {
  const int8_t bval = TERN(HAS_M350_B_PARAM, parser.byteval('B', -1), -1); UNUSED(bval);
  if (parser.seenval('S')) switch (parser.value_byte()) {
    case 1:
      LOOP_LOGICAL_AXES(i) if (parser.seenval(AXIS_CHAR(i))) stepper.microstep_ms(i, parser.value_byte(), -1, -1);
      TERN_(HAS_M350_B_PARAM, if (bval >= 0) stepper.microstep_ms(E_AXIS + 1, bval != 0, -1, -1));
      break;
    case 2:
      LOOP_LOGICAL_AXES(i) if (parser.seenval(AXIS_CHAR(i))) stepper.microstep_ms(i, -1, parser.value_byte(), -1);
      TERN_(HAS_M350_B_PARAM, if (bval >= 0) stepper.microstep_ms(E_AXIS + 1, -1, bval != 0, -1));
      break;
    case 3:
      LOOP_LOGICAL_AXES(i) if (parser.seenval(AXIS_CHAR(i))) stepper.microstep_ms(i, -1, -1, parser.value_byte());
      TERN_(HAS_M350_B_PARAM, if (bval >= 0) stepper.microstep_ms(E_AXIS + 1, -1, -1, bval != 0));
      break;
  }
  stepper.microstep_readings();
}

#endif // HAS_MICROSTEPS
