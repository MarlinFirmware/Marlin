/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2025 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(STALLGUARD_TUNING)

#include "../../../feature/stallguard/stallguard_tuning.h"
#include "../../gcode.h"

void say_stallguard_tuning() {
  SERIAL_ECHO_START();
  SERIAL_ECHOLNPGM("M921 StallGuard Tuning");
  SERIAL_ECHOLNPGM("Usage: M921 [X|Y]");
}

/**
 * M921: Launch StallGuard2 or StallGuard4 autotuning for X or Y axis depending on the TMC driver type.
 * For cartesian and coreXY machines
 * Axis with 2 TMC and same Stallguard version are supported.
 *
 * Parameters:
 *   X      - Autotune X axis stepper(s)
 *   Y      - Autotune Y axis stepper(s)
 *   Todo: autosave the tuned SGT values to EEPROM and save homing current if needed.
 */

void GcodeSuite::M921() {

  bool seen_x = parser.seen('X'), seen_y = parser.seen('Y');

  if (seen_x || seen_y) {
    const AxisEnum axis = seen_x ? X_AXIS : Y_AXIS;
    stallguard_tuner.tune_axis(axis);
    if (stallguard_tuner.is_Success())
        SERIAL_ECHOLN((axis?"Y":"X")," Tuning successful: sensitivity = ", stallguard_tuner.get_treshold());
    else
        SERIAL_ECHOLN((axis?"Y":"X")," Tuning failed.");
  }
  else {
    say_stallguard_tuning();
  }
}

#endif