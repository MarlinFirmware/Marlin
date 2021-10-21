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

enum class DGUS_Control : uint8_t {

  // PRINT
  FILE0       = 1, // RETURN_KEY_CODE
  FILE1       = 2, // RETURN_KEY_CODE
  FILE2       = 3, // RETURN_KEY_CODE
  FILE3       = 4, // RETURN_KEY_CODE
  FILE4       = 5, // RETURN_KEY_CODE
  GO_BACK     = 6, // RETURN_KEY_CODE
  SCROLL_UP   = 7, // RETURN_KEY_CODE
  SCROLL_DOWN = 8, // RETURN_KEY_CODE

  // PRINT_STATUS
  PAUSE       = 1, // POPUP_WINDOW
  RESUME      = 2, // POPUP_WINDOW

  // LEVELING_AUTOMATIC
  DISABLE     = 5, // RETURN_KEY_CODE

  // SETTINGS_MENU2
  EXTRA2      = 6, // RETURN_KEY_CODE

  // WAIT
  ABORT       = 1, // POPUP_WINDOW
  CONTINUE    = 2  // RETURN_KEY_CODE

};
