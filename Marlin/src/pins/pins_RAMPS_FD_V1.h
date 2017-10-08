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
 * RAMPS-FD
 *
 * No EEPROM
 * Use 4k7 thermistor tables
 */

#ifndef __SAM3X8E__
  #error "Oops!  Make sure you have 'Arduino Due' selected from the 'Tools -> Boards' menu."
#endif

#ifndef BOARD_NAME
  #define BOARD_NAME       "RAMPS-FD"
#endif

#define INVERTED_HEATER_PINS
#define INVERTED_BED_PINS
#define INVERTED_FAN_PINS

//
// Servos
//
#define SERVO0_PIN          7
#define SERVO1_PIN          6
#define SERVO2_PIN          5
#define SERVO3_PIN          3

//
// Limit Switches
//
#define X_MIN_PIN          22
#define X_MAX_PIN          30
#define Y_MIN_PIN          24
#define Y_MAX_PIN          38
#define Z_MIN_PIN          26
#define Z_MAX_PIN          34

//
// Steppers
//
#define X_STEP_PIN         63
#define X_DIR_PIN          62
#define X_ENABLE_PIN       48
#define X_CS_PIN           68

#define Y_STEP_PIN         65
#define Y_DIR_PIN          64
#define Y_ENABLE_PIN       46
#define Y_CS_PIN           60

#define Z_STEP_PIN         67
#define Z_DIR_PIN          66
#define Z_ENABLE_PIN       44
#define Z_CS_PIN           58

#define E0_STEP_PIN        36
#define E0_DIR_PIN         28
#define E0_ENABLE_PIN      42
#define E0_CS_PIN          67

#define E1_STEP_PIN        43
#define E1_DIR_PIN         41
#define E1_ENABLE_PIN      39
#define E1_CS_PIN          61

#define E2_STEP_PIN        32
#define E2_DIR_PIN         47
#define E2_ENABLE_PIN      45
#define E2_CS_PIN          59

//
// Temperature Sensors
//
#define TEMP_0_PIN          1   // Analog Input
#define TEMP_1_PIN          2   // Analog Input
#define TEMP_2_PIN          3   // Analog Input
#define TEMP_BED_PIN        0   // Analog Input

// SPI for Max6675 or Max31855 Thermocouple
#if DISABLED(SDSUPPORT)
  #define MAX6675_SS            53
#else
  #define MAX6675_SS            49
#endif

//
// Heaters / Fans
//
#define HEATER_0_PIN        9
#define HEATER_1_PIN       10
#define HEATER_2_PIN       11
#define HEATER_BED_PIN      8 // BED

#define FAN_PIN            12
#define CONTROLLER_FAN_PIN -1

//
// Misc. Functions
//
#define SDSS                4
#define LED_PIN            13

//
// LCD / Controller
//
#if ENABLED(ULTRA_LCD)
  #if ENABLED(NEWPANEL)
    // ramps-fd lcd adaptor
    #define LCD_PINS_RS         16
    #define LCD_PINS_ENABLE     17
    #define LCD_PINS_D4         23
    #define LCD_PINS_D5         25
    #define LCD_PINS_D6         27
    #define LCD_PINS_D7         29

    #if ENABLED(REPRAP_DISCOUNT_SMART_CONTROLLER)
      #define BEEPER_PIN        37

      #define BTN_EN1           33
      #define BTN_EN2           31
      #define BTN_ENC           35

      #define SD_DETECT_PIN     49
    #endif
  #endif
#endif // ULTRA_LCD
