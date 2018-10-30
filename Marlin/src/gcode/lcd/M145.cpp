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

#include "../../inc/MarlinConfig.h"

#if HAS_LCD_MENU

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
  if (material >= COUNT(lcd_preheat_hotend_temp)) {
    SERIAL_ERROR_START();
    SERIAL_ERRORLNPGM(MSG_ERR_MATERIAL_INDEX);
  }
  else {
    int v;
    if (parser.seenval('H')) {
      v = parser.value_int();
      lcd_preheat_hotend_temp[material] = constrain(v, EXTRUDE_MINTEMP, HEATER_0_MAXTEMP - 15);
    }
    if (parser.seenval('F')) {
      v = parser.value_int();
      lcd_preheat_fan_speed[material] = (uint8_t)constrain(v, 0, 255);
    }
    #if TEMP_SENSOR_BED != 0
      if (parser.seenval('B')) {
        v = parser.value_int();
        lcd_preheat_bed_temp[material] = constrain(v, BED_MINTEMP, BED_MAXTEMP - 15);
      }
    #endif
  }
}

#endif // HAS_LCD_MENU
