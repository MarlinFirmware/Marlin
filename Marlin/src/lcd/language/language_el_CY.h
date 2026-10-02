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
#pragma once

/**
 * Greek (Cyprus)
 *
 * LCD Menu Messages
 * See also https://marlinfw.org/docs/development/lcd_language.html
 *
 * Substitutions are applied for the following characters when used in menu items titles:
 *
 *   $ displays an inserted string
 *   { displays  '0'....'10' for indexes 0 - 10
 *   ~ displays  '1'....'11' for indexes 0 - 10
 *   * displays 'E1'...'E11' for indexes 0 - 10 (By default. Uses LCD_FIRST_TOOL)
 *   @ displays an axis name such as XYZUVW, or E for an extruder
 */

#include "language_el.h"

namespace Language_el_CY {
  using namespace Language_el; // Inherit undefined strings from Greek (or English)

  constexpr uint8_t CHARSIZE              = 2;
  LSTR LANGUAGE                           = _UxGT("Greek (Cyprus)");

  LSTR MSG_KINEMATICS_SETTINGS            = _UxGT("Ρυθμίσεις κινηματικής");                    // Kinematics Settings
  LSTR MSG_DELTA_TOWER_ANGLE_TRIM_N       = _UxGT("@ Ρύθμιση πύργου");                         // @ Tower Trim
  LSTR MSG_DELTA_ROD_TRIM_A               = _UxGT("@ Ρύθμιση ράβδου A");                       // A Rod Trim
  LSTR MSG_DELTA_ROD_TRIM_B               = _UxGT("@ Ρύθμιση ράβδου B");                       // B Rod Trim
  LSTR MSG_DELTA_ROD_TRIM_C               = _UxGT("@ Ρύθμιση ράβδου C");                       // C Rod Trim
  LSTR MSG_SCARA_P_OFFSET                 = _UxGT("Μετατόπιση θήτα");                          // P Offset
  LSTR MSG_SCARA_T_OFFSET                 = _UxGT("Μετατόπιση ψι");                            // T Offset
  LSTR MSG_SCARA_Z_OFFSET                 = _UxGT("Μετατόπιση Z");                             // Z Offset
}
