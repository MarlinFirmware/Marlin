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
#include "../../module/temperature.h"

#if NUM_SERIAL > 1
  #include "../../gcode/queue.h"
#endif

/**
 * M105: Read hot end and bed temperature
 */
void GcodeSuite::M105() {

  const int8_t target_extruder = get_target_extruder_from_command();
  if (target_extruder < 0) return;

  #if HAS_TEMP_SENSOR
    SERIAL_ECHOPGM(MSG_OK);
    thermalManager.print_heater_states(target_extruder);
  #else // !HAS_TEMP_SENSOR
    SERIAL_ERROR_MSG(MSG_ERR_NO_THERMISTORS);
  #endif

  SERIAL_EOL();
}
