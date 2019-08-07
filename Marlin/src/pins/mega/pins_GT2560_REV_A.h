/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#pragma once

/**
 * Geeetech GT2560 Revision A board pin assignments, based on the work of
 * George Robles (https://georges3dprinters.com) and
 * Richard Smith <galorin@gmail.com>
 */

#if !defined(__AVR_ATmega1280__) && !defined(__AVR_ATmega2560__)
  #error "Oops! Select 'Arduino/Genuino Mega or Mega 2560' in 'Tools > Board.'"
#endif

#ifndef BOARD_NAME
  #define BOARD_NAME "GT2560 Rev.A"
#endif
#define DEFAULT_MACHINE_NAME "Prusa i3 Pro B"

//
// Limit Switches
//
#define X_MIN_PIN          22
#define X_MAX_PIN          24
#define Y_MIN_PIN          26
#define Y_MAX_PIN          28
#define Z_MIN_PIN          30
#define Z_MAX_PIN          32

//
// Steppers
//
#define X_STEP_PIN         25
#define X_DIR_PIN          23
#define X_ENABLE_PIN       27

#define Y_STEP_PIN         31
#define Y_DIR_PIN          33
#define Y_ENABLE_PIN       29

#define Z_STEP_PIN         37
#define Z_DIR_PIN          39
#define Z_ENABLE_PIN       35

#define E0_STEP_PIN        43
#define E0_DIR_PIN         45
#define E0_ENABLE_PIN      41

#define E1_STEP_PIN        49
#define E1_DIR_PIN         47
#define E1_ENABLE_PIN      48

//
// Temperature Sensors
//
#define TEMP_0_PIN          8
#define TEMP_1_PIN          9
#define TEMP_BED_PIN       10

//
// Heaters / Fans
//
#define HEATER_0_PIN        2
#define HEATER_1_PIN        3
#define HEATER_BED_PIN      4
#ifndef FAN_PIN
  #define FAN_PIN           7
#endif

//
// Misc. Functions
//
#define SDSS               53
#define LED_PIN            13
#define PS_ON_PIN          12
#define SUICIDE_PIN        54   // Must be enabled at startup to keep power flowing
#define KILL_PIN           -1

#if HAS_SPI_LCD

  #define BEEPER_PIN       18

  #if ENABLED(NEWPANEL)

    #if ENABLED(MKS_MINI_12864)
      #define DOGLCD_A0     5
      #define DOGLCD_CS    21
      #define BTN_EN1      40
      #define BTN_EN2      42
    #else
      #define LCD_PINS_RS  20
      #define LCD_PINS_ENABLE 17
      #define LCD_PINS_D4  16
      #define LCD_PINS_D5  21
      #define LCD_PINS_D6   5
      #define LCD_PINS_D7   6
      #define BTN_EN1      42
      #define BTN_EN2      40
    #endif

    #define BTN_ENC        19
    #define SD_DETECT_PIN  38

  #else // !NEWPANEL

    #define SHIFT_CLK      38
    #define SHIFT_LD       42
    #define SHIFT_OUT      40
    #define SHIFT_EN       17

    #define LCD_PINS_RS    16
    #define LCD_PINS_ENABLE 5
    #define LCD_PINS_D4     6
    #define LCD_PINS_D5    21
    #define LCD_PINS_D6    20
    #define LCD_PINS_D7    19

    #define SD_DETECT_PIN  -1

  #endif // !NEWPANEL

#endif // HAS_SPI_LCD
