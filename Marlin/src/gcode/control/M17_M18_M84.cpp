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

#include "../gcode.h"
#include "../../MarlinCore.h" // for stepper_inactive_time, disable_e_steppers
#include "../../lcd/marlinui.h"
#include "../../module/stepper.h"

#if ENABLED(AUTO_BED_LEVELING_UBL)
  #include "../../feature/bedlevel/bedlevel.h"
#endif

/**
 * M17: Enable stepper motors
 */
void GcodeSuite::M17() {
  if (parser.seen_axis()) {
    LOGICAL_AXIS_CODE(
      if (TERN0(HAS_E_STEPPER_ENABLE, parser.seen_test('E'))) enable_e_steppers(),
      if (parser.seen_test('X'))        ENABLE_AXIS_X(),
      if (parser.seen_test('Y'))        ENABLE_AXIS_Y(),
      if (parser.seen_test('Z'))        ENABLE_AXIS_Z(),
      if (parser.seen_test(AXIS4_NAME)) ENABLE_AXIS_I(),
      if (parser.seen_test(AXIS5_NAME)) ENABLE_AXIS_J(),
      if (parser.seen_test(AXIS6_NAME)) ENABLE_AXIS_K()
    );
  }
  else {
    LCD_MESSAGEPGM(MSG_NO_MOVE);
    enable_all_steppers();
  }
}

/**
 * M18, M84: Disable stepper motors
 */
void GcodeSuite::M18_M84() {
  if (parser.seenval('S')) {
    reset_stepper_timeout();
    stepper_inactive_time = parser.value_millis_from_seconds();
  }
  else {
    if (parser.seen_axis()) {
      planner.synchronize();
      LOGICAL_AXIS_CODE(
        if (TERN0(HAS_E_STEPPER_ENABLE, parser.seen_test('E'))) disable_e_steppers(),
        if (parser.seen_test('X'))        DISABLE_AXIS_X(),
        if (parser.seen_test('Y'))        DISABLE_AXIS_Y(),
        if (parser.seen_test('Z'))        DISABLE_AXIS_Z(),
        if (parser.seen_test(AXIS4_NAME)) DISABLE_AXIS_I(),
        if (parser.seen_test(AXIS5_NAME)) DISABLE_AXIS_J(),
        if (parser.seen_test(AXIS6_NAME)) DISABLE_AXIS_K()
      );
    }
    else
      planner.finish_and_disable();

    TERN_(AUTO_BED_LEVELING_UBL, ubl.steppers_were_disabled());
  }
}
