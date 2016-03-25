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

/**
 * MegaTronics pin assignments
 */

#ifndef __AVR_ATmega2560__
  #error Oops!  Make sure you have 'Arduino Mega' selected from the 'Tools -> Boards' menu.
#endif

#define LARGE_FLASH        true

#define X_STEP_PIN         26
#define X_DIR_PIN          28
#define X_ENABLE_PIN       24
#define X_MIN_PIN          41
#define X_MAX_PIN          37

#define Y_STEP_PIN         60 // A6
#define Y_DIR_PIN          61 // A7
#define Y_ENABLE_PIN       22
#define Y_MIN_PIN          14
#define Y_MAX_PIN          15

#define Z_STEP_PIN         54 // A0
#define Z_DIR_PIN          55 // A1
#define Z_ENABLE_PIN       56 // A2
#define Z_MIN_PIN          18
#define Z_MAX_PIN          19

#define E0_STEP_PIN        31
#define E0_DIR_PIN         32
#define E0_ENABLE_PIN      38

#define E1_STEP_PIN        34
#define E1_DIR_PIN         36
#define E1_ENABLE_PIN      30

#define SDPOWER            -1
#define SDSS               53
#define LED_PIN            13


#define FAN_PIN             7   // IO pin. Buffer needed
#define PS_ON_PIN          12
#define KILL_PIN           -1

#define HEATER_0_PIN        9
#define HEATER_1_PIN        8
#define HEATER_2_PIN       -1

#if TEMP_SENSOR_0 == -1
  #define TEMP_0_PIN        8   // ANALOG NUMBERING
#else
  #define TEMP_0_PIN       13   // ANALOG NUMBERING
#endif

#define TEMP_1_PIN         15   // ANALOG NUMBERING
#define TEMP_2_PIN         -1   // ANALOG NUMBERING
#define HEATER_BED_PIN     10   // BED
#define TEMP_BED_PIN       14   // ANALOG NUMBERING

#define BEEPER_PIN         33   // AUX-4

#if ENABLED(ULTRA_LCD) && ENABLED(NEWPANEL)

  #define LCD_PINS_RS     16
  #define LCD_PINS_ENABLE 17
  #define LCD_PINS_D4     23
  #define LCD_PINS_D5     25
  #define LCD_PINS_D6     27
  #define LCD_PINS_D7     29

  // Buttons directly attached using AUX-2
  #define BTN_EN1         59
  #define BTN_EN2         64
  #define BTN_ENC         43

  #define BLEN_C           2
  #define BLEN_B           1
  #define BLEN_A           0

  #define SD_DETECT_PIN   -1   // RAMPS doesn't use this

#endif // ULTRA_LCD && NEWPANEL
