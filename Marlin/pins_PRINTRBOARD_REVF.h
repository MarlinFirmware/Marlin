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
 * Printrboard pin assignments (AT90USB1286)
 * Requires the Teensyduino software with Teensy++ 2.0 selected in Arduino IDE!
 * http://www.pjrc.com/teensy/teensyduino.html
 * See http://reprap.org/wiki/Printrboard for more info
 */

/**
 *  Rev B    29MAY 2017
 *
 *  Converted all pins to Teensyduino system
 */

#ifndef __AVR_AT90USB1286__
  #error "Oops!  Make sure you have 'Teensy++ 2.0' selected from the 'Tools -> Boards' menu."
#endif

#define BOARD_NAME         "Printrboard Rev F"
#define LARGE_FLASH        true

//
// Limit Switches
//
#define X_STOP_PIN         47
#define Y_STOP_PIN         24
#define Z_STOP_PIN         36

//
// Steppers
//
#define X_STEP_PIN         28
#define X_DIR_PIN          29
#define X_ENABLE_PIN       19

#define Y_STEP_PIN         30
#define Y_DIR_PIN          31
#define Y_ENABLE_PIN       18

#define Z_STEP_PIN         32
#define Z_DIR_PIN          33
#define Z_ENABLE_PIN       17

#define E0_STEP_PIN        34
#define E0_DIR_PIN         35
#define E0_ENABLE_PIN      13

// uncomment to enable an I2C based DAC like on the Printrboard REVF
#define DAC_STEPPER_CURRENT
// Number of channels available for DAC, For Printrboar REVF there are 4
#define DAC_STEPPER_ORDER { 3, 2, 1, 0 }

#define DAC_STEPPER_SENSE    0.11
#define DAC_STEPPER_ADDRESS  0
#define DAC_STEPPER_MAX   3520
#define DAC_STEPPER_VREF     1 // internal Vref, gain 1x = 2.048V
#define DAC_STEPPER_GAIN     0
#define DAC_OR_ADDRESS    0x00

//
// Temperature Sensors
//
#define TEMP_0_PIN          1   // Analog Input (Extruder)
#define TEMP_BED_PIN        0   // Analog Input (Bed)

//
// Heaters / Fans
//
#define HEATER_0_PIN       15 // Extruder
#define HEATER_1_PIN       44
#define HEATER_2_PIN       45
#define HEATER_BED_PIN     14


#if ENABLED(FAST_PWM_FAN)
  #define FAN_PIN          16
#else
  #define FAN_PIN          22
#endif

//
// Misc. Functions
//
#define SDSS               20 
#define FILWIDTH_PIN        2 // Analog Input

//
// LCD / Controller
//
#if ENABLED(ULTRA_LCD)
  #define BEEPER_PIN -1

  #define LCD_PINS_RS 21
  #define LCD_PINS_ENABLE 20
  #define LCD_PINS_D4 35
  #define LCD_PINS_D5 34
  #define LCD_PINS_D6 33
  #define LCD_PINS_D7 32

  #define BTN_EN1   10
  #define BTN_EN2   11
  #define BTN_ENC   12 // the click

  #define SD_DETECT_PIN -1

  // encoder rotation values
  #define encrot0 0
  #define encrot1 2
  #define encrot2 3
  #define encrot3 1
#endif

#if ENABLED(VIKI2) || ENABLED(miniVIKI)
  #define BEEPER_PIN  8
  #define DOGLCD_A0  40
  #define DOGLCD_CS  41
  #define LCD_SCREEN_ROT_180

  #define BTN_EN1 2
  #define BTN_EN2 3
  #define BTN_ENC 45

  #define SDSS 43
  #define SD_DETECT_PIN -1

  #define STAT_LED_RED_PIN  24
  #define STAT_LED_BLUE_PIN  0
#endif
