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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "../../inc/MarlinConfig.h"

#if PREHEAT_COUNT

#include "../gcode.h"
#include "../../lcd/ultralcd.h"

/**
 * M145: Set the heatup state for a material in the LCD menu
 *
 *   S<material>
 *   H<hotend temp>
 *   B<bed temp>
 *   F<fan speed>
 */
void GcodeSuite::M145() {
  const uint8_t material = (uint8_t)parser.intval('S');
  if (material >= PREHEAT_COUNT)
    SERIAL_ERROR_MSG(STR_ERR_MATERIAL_INDEX);
  else {
    preheat_t &mat = ui.material_preset[material];
    #if HAS_HOTEND
      if (parser.seenval('H'))
        mat.hotend_temp = constrain(parser.value_int(), EXTRUDE_MINTEMP, (HEATER_0_MAXTEMP) - (HOTEND_OVERSHOOT));
    #endif
    #if TEMP_SENSOR_BED != 0
      if (parser.seenval('B'))
        mat.bed_temp = constrain(parser.value_int(), BED_MINTEMP, BED_MAX_TARGET);
    #endif
    #if HAS_FAN
      if (parser.seenval('F'))
        mat.fan_speed = constrain(parser.value_int(), 0, 255);
    #endif
  }
}

#endif // PREHEAT_COUNT
