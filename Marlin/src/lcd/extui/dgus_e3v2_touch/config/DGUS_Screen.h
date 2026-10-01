/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * Pages of the Creality "Upgraded Touch Panel" DWIN_SET for Ender-3 (Pro) / Ender-3 V2
 */
enum class DGUS_ScreenID : uint8_t {
  BOOT              = 0,
  MAIN              = 1,
  FILE1             = 2,
  FILE2             = 3,
  FILE3             = 4,
  FILE4             = 5,
  FILE5             = 6,
  FILAMENT_RUNOUT   = 7,
  FILAMENT_INSERT   = 8,
  FINISH            = 9,
  PRINTING          = 10,
  PAUSE_CONFIRM     = 11,
  PAUSED            = 12,
  STOP_CONFIRM      = 13,
  ADJUST            = 14,
  PREPARE           = 15,
  MOVEAXIS_10       = 16,
  MOVEAXIS_1        = 17,
  MOVEAXIS_01       = 18,
  FEEDRETURN        = 19,
  CONTROL           = 20,
  TEMP              = 21,
  PLA_TEMP          = 22,
  ABS_TEMP          = 23,
  INFORMATION       = 24,
  LEVELINGMODE      = 25,
  LEVELING          = 26,
  POWERCONTINUE     = 27,
  LANGUAGE          = 28,
  NO_LEVEL          = 29,
  KEYBOARD          = 55,
  KEYBOARD_CONFIRM  = 56,
  THERMAL_RUNAWAY   = 57,
  HEATING_FAILED    = 58,
  THERMISTOR_ERROR  = 59,
  AUTOHOME          = 60,
  ABNORMAL          = 62,

  HOME              = MAIN
};
