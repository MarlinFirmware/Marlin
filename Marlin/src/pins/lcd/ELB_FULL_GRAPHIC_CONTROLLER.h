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
#pragma once

/**
 * ELB_FULL_GRAPHIC_CONTROLLER — standard EXP1/EXP2 wiring.
 * Uses #ifndef guards for BEEPER_PIN, SD_DETECT_PIN, and KILL_PIN
 * so boards may pre-define board-specific overrides.
 */
#if ENABLED(ELB_FULL_GRAPHIC_CONTROLLER)

  #ifndef DOGLCD_CS
    #define DOGLCD_CS                  EXP1_08_PIN
  #endif
  #ifndef DOGLCD_A0
    #define DOGLCD_A0                  EXP1_07_PIN
  #endif

  #ifndef BEEPER_PIN
    #define BEEPER_PIN                 EXP1_05_PIN
  #endif
  #ifndef LCD_BACKLIGHT_PIN
    #define LCD_BACKLIGHT_PIN          EXP2_05_PIN
  #endif

  #ifndef BTN_EN1
    #define BTN_EN1                    EXP1_02_PIN
  #endif
  #ifndef BTN_EN2
    #define BTN_EN2                    EXP1_01_PIN
  #endif
  #ifndef BTN_ENC
    #define BTN_ENC                    EXP2_03_PIN
  #endif

  #ifndef LCD_SDSS_PIN
    #define LCD_SDSS_PIN                 SD_SS_PIN
  #endif
  #ifndef SD_DETECT_PIN
    #define SD_DETECT_PIN              EXP2_07_PIN
  #endif
  #ifndef KILL_PIN
    #define KILL_PIN                   EXP2_08_PIN
  #endif

#endif
