/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#pragma once

#define CONFIG_EXAMPLES_DIR "Sovol/SV-01/CrealityV221"

/**
 * Custom Status Screen bitmap
 *
 * Place this file in the root with your configuration files
 * and enable CUSTOM_STATUS_SCREEN_IMAGE in Configuration.h.
 *
 * Use the Marlin Bitmap Converter to make your own:
 * https://marlinfw.org/tools/u8glib/converter.html
 */

#define STATUS_LOGO_Y      8
#define STATUS_LOGO_WIDTH 32

const unsigned char status_logo_bmp[] PROGMEM = {
  B00011110,B11110111,B10011100,B00000100,
  B00111111,B01100011,B00111110,B00001100,
  B01100011,B01100011,B01100011,B00111100,
  B01100000,B01100011,B01100011,B00001100,
  B01100000,B01100011,B01100011,B00001100,
  B00111100,B01100011,B01100011,B00001100,
  B00011110,B00110110,B01100011,B00001100,
  B00000011,B00110110,B01100011,B00001100,
  B00000011,B00110110,B01100011,B00001100,
  B01100011,B00011100,B01100011,B00001100,
  B01111110,B00011100,B00111110,B00111111,
  B00111100,B00001000,B00011100,B00111111
};
