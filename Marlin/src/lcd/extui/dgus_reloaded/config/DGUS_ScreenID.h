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

enum class DGUS_ScreenID : uint8_t {
  BOOT                = 0,
  HOME                = 1,
  PRINT               = 2,
  PRINT_STATUS        = 3,
  PRINT_ADJUST        = 4,
  PRINT_FINISHED      = 5,
  TEMP_MENU           = 6,
  TEMP_MANUAL         = 7,
  FAN                 = 8,
  SETTINGS_MENU       = 9,
  LEVELING_MENU       = 10,
  LEVELING_OFFSET     = 11,
  LEVELING_MANUAL     = 12,
  LEVELING_AUTOMATIC  = 13,
  LEVELING_PROBING    = 14,
  FILAMENT            = 15,
  MOVE                = 16,
  GCODE               = 17,
  SETTINGS_MENU2      = 18,
  PID                 = 19,
  VOLUME              = 20,
  BRIGHTNESS          = 21,
  INFOS               = 22,
  DEBUG               = 240,
  POWERLOSS           = 248,
  WAIT                = 249,
  KILL                = 250
};
