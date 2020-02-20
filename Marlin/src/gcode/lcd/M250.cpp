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

#if HAS_LCD_CONTRAST || TFT_HAS_COLOR

#include "../gcode.h"
#include "../../lcd/ultralcd.h"
#include "../../lcd/dogm/ultralcd_DOGM.h"

/**
 * M250: Read and optionally set the LCD contrast
 */
void GcodeSuite::M250() {
  #if HAS_LCD_CONTRAST
    if (parser.seen('C')) ui.set_contrast(parser.value_int());
    SERIAL_ECHOLNPAIR("LCD Contrast: ", ui.contrast);
  #endif // HAS_LCD_CONTRAST

//
// Warning this component is still pretty stupid
//

  #if TFT_HAS_COLOR
    if (parser.seen('B'))
    {
      bg_color = parser.value_ushort();
      SERIAL_ECHOLNPAIR("TFT Background Color: ", bg_color);
    }

    if (parser.seen('F'))
    {
      ui_color = parser.value_ushort();
      SERIAL_ECHOLNPAIR("TFT Foreground Color: ", ui_color);
    }

        if (parser.seen('P'))
    {
      switchColorPreset(parser.value_byte());
      SERIAL_ECHOLN("Switched Color Preset: ");
    }
  #endif // TFT_HAS_COLOR

}

#endif // HAS_LCD_CONTRAST || TFT_HAS_COLOR
