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

#include "../gcode.h"
#include "../../Marlin.h" // for stepper_inactive_time
#include "../../lcd/ultralcd.h"
#include "../../module/stepper.h"

#if ENABLED(AUTO_BED_LEVELING_UBL) && ENABLED(ULTRA_LCD)
  #include "../../feature/bedlevel/bedlevel.h"
#endif

/**
 * M17: Enable power on all stepper motors
 */
void GcodeSuite::M17() {
  LCD_MESSAGEPGM(MSG_NO_MOVE);
  enable_all_steppers();
}

/**
 * M18, M84: Disable stepper motors
 */
void GcodeSuite::M18_M84() {
  if (parser.seenval('S')) {
    stepper_inactive_time = parser.value_millis_from_seconds();
  }
  else {
    bool all_axis = !(parser.seen('X') || parser.seen('Y') || parser.seen('Z') || parser.seen('E'));
    if (all_axis) {
      planner.finish_and_disable();
    }
    else {
      planner.synchronize();
      if (parser.seen('X')) disable_X();
      if (parser.seen('Y')) disable_Y();
      if (parser.seen('Z')) disable_Z();
      // Only disable on boards that have separate ENABLE_PINS or another method for disabling the driver
      #if (E0_ENABLE_PIN != X_ENABLE_PIN && E1_ENABLE_PIN != Y_ENABLE_PIN) || AXIS_DRIVER_TYPE(E0, TMC2660) || AXIS_DRIVER_TYPE(E1, TMC2660) || AXIS_DRIVER_TYPE(E2, TMC2660) || AXIS_DRIVER_TYPE(E3, TMC2660) || AXIS_DRIVER_TYPE(E4, TMC2660) || AXIS_DRIVER_TYPE(E5, TMC2660)
        if (parser.seen('E')) disable_e_steppers();
      #endif
    }

    #if HAS_LCD_MENU && ENABLED(AUTO_BED_LEVELING_UBL)
      if (ubl.lcd_map_control) ubl.lcd_map_control = defer_return_to_status = false;
    #endif
  }
}
