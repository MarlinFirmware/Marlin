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
 * RADDS
 */

#ifndef __SAM3X8E__
  #error "Oops!  Make sure you have 'Arduino Due' selected from the 'Tools -> Boards' menu."
#endif

#define BOARD_NAME         "RADDS"

//
// Servos
//
#define SERVO0_PIN          5
#define SERVO1_PIN          6
#define SERVO2_PIN         39
#define SERVO3_PIN         40

//
// Limit Switches
//
#define X_MIN_PIN          28
#define X_MAX_PIN          34
#define Y_MIN_PIN          30
#define Y_MAX_PIN          36
#define Z_MIN_PIN          32
#define Z_MAX_PIN          38

//
// Steppers
//
#define X_STEP_PIN         24
#define X_DIR_PIN          23
#define X_ENABLE_PIN       26
#ifndef X_CS_PIN
  #define X_CS_PIN         25
#endif

#define Y_STEP_PIN         17
#define Y_DIR_PIN          16
#define Y_ENABLE_PIN       22
#ifndef Y_CS_PIN
  #define Y_CS_PIN         27
#endif

#define Z_STEP_PIN          2
#define Z_DIR_PIN           3
#define Z_ENABLE_PIN       15
#ifndef Z_CS_PIN
  #define Z_CS_PIN         29
#endif

#define E0_STEP_PIN        61
#define E0_DIR_PIN         60
#define E0_ENABLE_PIN      62
#ifndef E0_CS_PIN
  #define E0_CS_PIN        31
#endif

#define E1_STEP_PIN        64
#define E1_DIR_PIN         63
#define E1_ENABLE_PIN      65
#ifndef E1_CS_PIN
  #define E1_CS_PIN        33
#endif

#define E2_STEP_PIN        51
#define E2_DIR_PIN         53
#define E2_ENABLE_PIN      49
#ifndef E2_CS_PIN
  #define E2_CS_PIN        35
#endif

// For Extension Board V2
// http://doku.radds.org/dokumentation/extension-board
//#define E3_STEP_PIN        35
//#define E3_DIR_PIN         33
//#define E3_ENABLE_PIN      37
//#ifndef E3_CS_PIN
//  #define E3_CS_PIN         6
//#endif

//#define Z2_STEP_PIN        29
//#define Z2_DIR_PIN         27
//#define Z2_ENABLE_PIN      31
//#ifndef Z2_CS_PIN
//  #define Z2_CS_PIN        39
//#endif

// Microstepping pins - Mapping not from fastio.h (?)
//#define E3_MS1_PIN         67
//#define E3_MS2_PIN         68
//#define E3_MS3_PIN         69
//#define Z2_MS1_PIN         67 // shared with E3_MS1_PIN
//#define Z2_MS2_PIN         68 // shared with E3_MS2_PIN
//#define Z2_MS3_PIN         69 // shared with E3_MS3_PIN

//
// Temperature Sensors
//
#define TEMP_0_PIN          0   // Analog Input
#define TEMP_1_PIN          1   // Analog Input
#define TEMP_2_PIN          2   // Analog Input
#define TEMP_3_PIN          3   // Analog Input
#define TEMP_4_PIN          5   // dummy so will compile when PINS_DEBUGGING is enabled
#define TEMP_BED_PIN        4   // Analog Input

// SPI for Max6675 or Max31855 Thermocouple
#if DISABLED(SDSUPPORT)
  #define MAX6675_SS        53
#else
  #define MAX6675_SS        49
#endif

//
// Heaters / Fans
//
#define HEATER_0_PIN       13
#define HEATER_1_PIN       12
#define HEATER_2_PIN       11
#define HEATER_BED_PIN      7 // BED

#define FAN_PIN             9
#define FAN1_PIN            8

//
// Misc. Functions
//
#define SDSS                4
#define PS_ON_PIN          40

#define I2C_EEPROM

//
// LCD / Controller
//
#if ENABLED(ULTRA_LCD)
  // RADDS LCD panel
  #if ENABLED(RADDS_DISPLAY)
    #define LCD_PINS_RS     42
    #define LCD_PINS_ENABLE 43
    #define LCD_PINS_D4     44
    #define LCD_PINS_D5     45
    #define LCD_PINS_D6     46
    #define LCD_PINS_D7     47

    #define BEEPER_PIN      41

    #define BTN_EN1         50
    #define BTN_EN2         52
    #define BTN_ENC         48

    #define BTN_BACK        71

    #undef SDSS
    #define SDSS            10
    #define SD_DETECT_PIN   14

  #elif ENABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER)
    #define LCD_PINS_RS     46
    #define LCD_PINS_ENABLE 47
    #define LCD_PINS_D4     44

    #define BEEPER_PIN      41

    #define BTN_EN1         50
    #define BTN_EN2         52
    #define BTN_ENC         48

  #elif ENABLED(SSD1306_OLED_I2C_CONTROLLER)
    #define BTN_EN1         50
    #define BTN_EN2         52
    #define BTN_ENC         48
    #define BEEPER_PIN      41
    #define LCD_SDSS        10
    #define SD_DETECT_PIN   14

  #elif ENABLED(SPARK_FULL_GRAPHICS)
    #define LCD_PINS_D4     29
    #define LCD_PINS_ENABLE 27
    #define LCD_PINS_RS     25

    #define BTN_EN1         35
    #define BTN_EN2         33
    #define BTN_ENC         37

  #endif // SPARK_FULL_GRAPHICS
#endif // ULTRA_LCD
