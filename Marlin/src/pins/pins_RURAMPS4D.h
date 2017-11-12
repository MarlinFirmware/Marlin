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
 * Ported sys0724 & Vynt
 */

/**
 * Arduino Mega? or Due with RuRAMPS4DUE pin assignments
 *
 * Applies to the following boards:
 *  RURAMPS4DUE      (Hotend0, Hotend1, Hotend2, Fan0, Fan1, Bed)
 *
 *  Differences between
 *     RADDS | RuRAMPS4DUE
 *       A13 | A9/D63 (shares the same pin with AUX2_4PIN)
 *       A14 | A10/D64 (shares the same pin with AUX2_5PIN)
 *       A15 | NC
 */

#ifndef __SAM3X8E__
  #error "Oops!  Make sure you have 'Arduino Due' selected from the 'Tools -> Boards' menu."
#endif

#ifndef BOARD_NAME
  #define BOARD_NAME "RuRAMPS4Due"
#endif

#undef INVERTED_HEATER_PINS
#undef INVERTED_BED_PINS
#undef INVERTED_FAN_PINS

#define I2C_EEPROM
//#define SPI_EEPROM

//
// Servos
//
#define SERVO0_PIN          5
#define SERVO1_PIN          3
#define SERVO2_PIN         -1
#define SERVO3_PIN          1

//
// Limit Switches
//
#define X_MIN_PIN          45
#define X_MAX_PIN          39
#define Y_MIN_PIN          46
#define Y_MAX_PIN          41
#define Z_MIN_PIN          49
#define Z_MAX_PIN          43

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN  43
#endif

//
// Steppers
//
#define X_STEP_PIN         37
#define X_DIR_PIN          36
#define X_ENABLE_PIN       38
#define X_CS_PIN           -1

#define Y_STEP_PIN         32
#define Y_DIR_PIN          35
#define Y_ENABLE_PIN       34
#define Y_CS_PIN           -1

#define Z_STEP_PIN         30
#define Z_DIR_PIN          2
#define Z_ENABLE_PIN       33
#define Z_CS_PIN           -1

#define E0_STEP_PIN        29
#define E0_DIR_PIN         28
#define E0_ENABLE_PIN      31
#define E0_CS_PIN          -1

#define E1_STEP_PIN        22
#define E1_DIR_PIN         24
#define E1_ENABLE_PIN      26
#define E1_CS_PIN          -1

#define E2_STEP_PIN        25 //14 //sys0724
#define E2_DIR_PIN         23 //15
#define E2_ENABLE_PIN      27 //61
#define E2_CS_PIN          -1

// For Extension Board V2
// http://doku.radds.org/dokumentation/extension-board
//#define E3_STEP_PIN        35
//#define E3_DIR_PIN         33
//#define E3_ENABLE_PIN      37
//#define E3_CS_PIN           6

//#define Z2_STEP_PIN        29
//#define Z2_DIR_PIN         27
//#define Z2_ENABLE_PIN      31
//#define Z2_CS_PIN          39

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
#define TEMP_0_PIN          0  // ANALOG NUMBERING #54
#define TEMP_1_PIN          1  // ANALOG NUMBERING #55
#define TEMP_2_PIN          2  // ANALOG NUMBERING #56
#define TEMP_3_PIN          3  // ANALOG NUMBERING #57
#define TEMP_BED_PIN        4  // ANALOG NUMBERING #58

/*
// SPI for Max6675 or Max31855 Thermocouple
#if DISABLED(SDSUPPORT)
  #define MAX6675_SS       53
#else
  #define MAX6675_SS       49
#endif
*/

//
// Heaters / Fans
//
#define HEATER_0_PIN       13
#define HEATER_1_PIN       12
#define HEATER_2_PIN       11
#define HEATER_BED_PIN      7    // BED H1

#define FAN_PIN	            9
#define FAN1_PIN            8

//
// Misc. Functions
//
#define SDSS                4   // 4,10,52 if using HW SPI.
#define LED_PIN            13

//#define FIL_RUNOUT_PIN    Y_MIN_PIN

//
// LCD / Controller
//
#if ENABLED(ULTRA_LCD)
  // RADDS LCD panel
  #if ENABLED(RADDS_DISPLAY) || ENABLED(REPRAP_DISCOUNT_SMART_CONTROLLER)
    #define LCD_PINS_RS     63
    #define LCD_PINS_ENABLE 64
    #define LCD_PINS_D4     48
    #define LCD_PINS_D5     50
    #define LCD_PINS_D6     52
    #define LCD_PINS_D7     53

    #define BEEPER_PIN      62

    #define BTN_EN1         44
    #define BTN_EN2         42
    #define BTN_ENC         40

    #define SD_DETECT_PIN   51

  #elif ENABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER)
    #define LCD_PINS_RS     52   //?
    #define LCD_PINS_ENABLE 53   //?
    #define LCD_PINS_D4     48   //?
    #define LCD_PINS_D5     50   //?
    #define LCD_PINS_D6     52   //?
    #define LCD_PINS_D7     53   //?

    #define BEEPER_PIN      62

    #define BTN_EN1         44   //!
    #define BTN_EN2         42   //!
    #define BTN_ENC         40
    #define SD_DETECT_PIN   51

  #elif ENABLED(SSD1306_OLED_I2C_CONTROLLER)

    #define BTN_EN1         44   //?
    #define BTN_EN2         42   //?
    #define BTN_ENC         40   //?
    #define SD_DETECT_PIN   51   //?

    #define BEEPER_PIN      62   //?
    #define LCD_SDSS        10   //?

  #elif ENABLED(SPARK_FULL_GRAPHICS)   //!!! - check!

    #define LCD_PINS_D4     29
    #define LCD_PINS_ENABLE 27
    #define LCD_PINS_RS     25
    #define BTN_EN1         35
    #define BTN_EN2         33
    #define BTN_ENC         37

  #endif // SPARK_FULL_GRAPHICS

#endif // ULTRA_LCD
