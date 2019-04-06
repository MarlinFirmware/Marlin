/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
  #error "Oops! Select 'Arduino Due' in 'Tools > Board.'"
#endif

#ifndef BOARD_NAME
  #define BOARD_NAME "RAMPS-FD v1"
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
#ifndef X_CS_PIN
  #define X_CS_PIN         68
#endif

#define Y_STEP_PIN         65
#define Y_DIR_PIN          64
#define Y_ENABLE_PIN       46
#ifndef Y_CS_PIN
  #define Y_CS_PIN         60
#endif

#define Z_STEP_PIN         67
#define Z_DIR_PIN          66
#define Z_ENABLE_PIN       44
#ifndef Z_CS_PIN
  #define Z_CS_PIN         58
#endif

#define E0_STEP_PIN        36
#define E0_DIR_PIN         28
#define E0_ENABLE_PIN      42
#ifndef E0_CS_PIN
  #define E0_CS_PIN        67
#endif

#define E1_STEP_PIN        43
#define E1_DIR_PIN         41
#define E1_ENABLE_PIN      39
#ifndef E1_CS_PIN
  #define E1_CS_PIN        61
#endif

#define E2_STEP_PIN        32
#define E2_DIR_PIN         47
#define E2_ENABLE_PIN      45
#ifndef E2_CS_PIN
  #define E2_CS_PIN        59
#endif

//
// Temperature Sensors
//
#define TEMP_0_PIN          1   // Analog Input
#define TEMP_1_PIN          2   // Analog Input
#define TEMP_2_PIN          3   // Analog Input
#define TEMP_BED_PIN        0   // Analog Input

// SPI for Max6675 or Max31855 Thermocouple
#if DISABLED(SDSUPPORT)
  #define MAX6675_SS_PIN   53
#else
  #define MAX6675_SS_PIN   49
#endif

//
// Heaters / Fans
//
#define HEATER_0_PIN        9
#define HEATER_1_PIN       10
#define HEATER_2_PIN       11
#define HEATER_BED_PIN      8

#ifndef FAN_PIN
  #define FAN_PIN          12
#endif

//
// Misc. Functions
//
#define SDSS                4
#define LED_PIN            13

//
// LCD / Controller
//
#if ENABLED(ULTRA_LCD)
  // ramps-fd lcd adaptor

  #define BEEPER_PIN       37
  #define BTN_EN1          33
  #define BTN_EN2          31
  #define BTN_ENC          35
  #define SD_DETECT_PIN    49

  #if ENABLED(NEWPANEL)
    #define LCD_PINS_RS    16
    #define LCD_PINS_ENABLE 17
    #define LCD_PINS_D4    23
    #define LCD_PINS_D5    25
    #define LCD_PINS_D6    27
    #define LCD_PINS_D7    29
  #endif

  #if ENABLED(MINIPANEL)
    #define DOGLCD_CS      25
    #define DOGLCD_A0      27
  #endif

  #if ANY(VIKI2, miniVIKI)
    #define DOGLCD_A0           16
    #define KILL_PIN            51
    #define STAT_LED_BLUE_PIN   29
    #define STAT_LED_RED_PIN    23
    #define DOGLCD_CS           17
    #define DOGLCD_SCK          76   // SCK_PIN   - These are required for DUE Hardware SPI
    #define DOGLCD_MOSI         75   // MOSI_PIN
    #define DOGLCD_MISO         74   // MISO_PIN
  #endif


#endif // ULTRA_LCD

#if HAS_DRIVER(TMC2208)
  /**
   * TMC2208 stepper drivers
   *
   * Hardware serial communication ports.
   * If undefined software serial is used according to the pins below
   */
  //#define X_HARDWARE_SERIAL  Serial1
  //#define X2_HARDWARE_SERIAL Serial1
  //#define Y_HARDWARE_SERIAL  Serial1
  //#define Y2_HARDWARE_SERIAL Serial1
  //#define Z_HARDWARE_SERIAL  Serial1
  //#define Z2_HARDWARE_SERIAL Serial1
  //#define E0_HARDWARE_SERIAL Serial1
  //#define E1_HARDWARE_SERIAL Serial1
  //#define E2_HARDWARE_SERIAL Serial1
  //#define E3_HARDWARE_SERIAL Serial1
  //#define E4_HARDWARE_SERIAL Serial1
#endif

//
// M3/M4/M5 - Spindle/Laser Control
//
#if ENABLED(SPINDLE_LASER_ENABLE) && !PIN_EXISTS(SPINDLE_LASER_ENABLE)
  #if HOTENDS < 3
    #define SPINDLE_LASER_ENABLE_PIN  45   // Use E2 ENA
    #define SPINDLE_LASER_PWM_PIN     12   // MUST BE HARDWARE PWM
    #define SPINDLE_DIR_PIN           47   // Use E2 DIR
  #endif
#endif
