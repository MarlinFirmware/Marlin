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
 * Arduino Mega or Due with RAMPS Duo pin assignments
 *
 * Applies to the following boards:
 *
 *  RAMPS_DUO_EFB (Hotend, Fan, Bed)
 *  RAMPS_DUO_EEB (Hotend0, Hotend1, Bed)
 *  RAMPS_DUO_EFF (Hotend, Fan0, Fan1)
 *  RAMPS_DUO_EEF (Hotend0, Hotend1, Fan)
 *  RAMPS_DUO_SF  (Spindle, Controller Fan)
 *
 *  Differences between
 *  RAMPS_14 | RAMPS_DUO
 *    A9/D63 | A12/D66
 *   A10/D64 | A13/D67
 *   A11/D65 | A14/D68
 *   A12/D66 | A15/D69
 *       A13 | A9
 *       A14 | A10
 *       A15 | A11
 */

#define BOARD_INFO_NAME "RAMPS Duo"

//
// Temperature Sensors
//
#define TEMP_0_PIN                             9  // Analog Input
#define TEMP_1_PIN                            11  // Analog Input
#define TEMP_BED_PIN                          10  // Analog Input

#define ALLOW_SAM3X8E
#include "../ramps/pins_RAMPS.h"

// SPI for MAX Thermocouple
#undef TEMP_0_CS_PIN
#if !HAS_MEDIA
  #define TEMP_0_CS_PIN                       69  // Don't use 53 if using Display/SD card
#else
  #define TEMP_0_CS_PIN                       69  // Don't use 49 (SD_DETECT_PIN)
#endif

//
// LCD / Controller
//
#if HAS_WIRED_LCD

  #if ALL(IS_NEWPANEL, PANEL_ONE)
    #undef LCD_PINS_D4
    #define LCD_PINS_D4                       68

    #undef LCD_PINS_D5
    #define LCD_PINS_D5                       69

    #undef LCD_PINS_D7
    #define LCD_PINS_D7                       67
  #endif

  #if IS_NEWPANEL

    #if ENABLED(REPRAPWORLD_GRAPHICAL_LCD)

      #undef BTN_EN1
      #define BTN_EN1                         67

      #undef BTN_ENC
      #define BTN_ENC                         66

    #elif ENABLED(MINIPANEL)

      #undef DOGLCD_CS
      #define DOGLCD_CS                       69

      #undef LCD_BACKLIGHT_PIN
      #define LCD_BACKLIGHT_PIN               68  // backlight LED on A14/D68

      #undef KILL_PIN
      #define KILL_PIN                        67

      #undef BTN_EN2
      #define BTN_EN2                         66

    #else

      #if IS_RRW_KEYPAD
        #undef BTN_EN1
        #define BTN_EN1                       67  // encoder

        #undef BTN_ENC
        #define BTN_ENC                       66  // enter button
      #elif ENABLED(PANEL_ONE)
        #undef BTN_EN2
        #define BTN_EN2                       66  // AUX2 PIN 4
      #endif
    #endif

    #if ENABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER)
      #define BTN_ENC_EN             LCD_PINS_D7  // Detect the presence of the encoder
    #endif

  #endif // IS_NEWPANEL

#endif // HAS_WIRED_LCD
