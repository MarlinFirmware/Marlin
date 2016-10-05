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

#ifndef __AVR_AT90USB1286__
  #error "Oops!  Make sure you have 'Teensy++ 2.0' selected from the 'Tools -> Boards' menu."
#endif

#if ENABLED(AT90USBxx_TEENSYPP_ASSIGNMENTS)  // use Teensyduino Teensy++2.0 pin assignments instead of Marlin traditional.
  #error "These Printrboard assignments depend on traditional Marlin assignments, not AT90USBxx_TEENSYPP_ASSIGNMENTS in fastio.h"
#endif

#define BOARD_NAME         "Printrboard Rev F"
#define LARGE_FLASH        true

#define X_STEP_PIN          0
#define X_DIR_PIN           1
#define X_ENABLE_PIN       39

#define Y_STEP_PIN          2
#define Y_DIR_PIN           3
#define Y_ENABLE_PIN       38

#define Z_STEP_PIN          4
#define Z_DIR_PIN           5
#define Z_ENABLE_PIN       23

#define E0_STEP_PIN         6
#define E0_DIR_PIN          7
#define E0_ENABLE_PIN      19

#define HEATER_0_PIN       21  // Extruder
#define HEATER_1_PIN       46
#define HEATER_2_PIN       47
#define HEATER_BED_PIN     20

// If soft or fast PWM is off then use Teensyduino pin numbering, Marlin
// fastio pin numbering otherwise
#if ENABLED(FAN_SOFT_PWM) || ENABLED(FAST_PWM_FAN)
  #define FAN_PIN          22
#else
  #define FAN_PIN          16
#endif

#define X_STOP_PIN         35
#define Y_STOP_PIN         12
#define Z_STOP_PIN         36

#define TEMP_0_PIN          1  // Extruder / Analog pin numbering
#define TEMP_BED_PIN        0  // Bed / Analog pin numbering

#define FILWIDTH_PIN        2  // ANALOG NUMBERING

////LCD Pin Setup////

#define SDSS               20 // Teensylu pin mapping

// uncomment to enable an I2C based DAC like on the Printrboard REVF
#define DAC_STEPPER_CURRENT
// Number of channels available for DAC, For Printrboar REVF there are 4
#define DAC_STEPPER_ORDER { 3, 2, 1, 0 }

#define DAC_STEPPER_SENSE    0.11
#define DAC_STEPPER_ADDRESS  0
#define DAC_STEPPER_MAX   3520
#define DAC_STEPPER_VREF     1 //internal Vref, gain 1x = 2.048V
#define DAC_STEPPER_GAIN     0
#define DAC_OR_ADDRESS    0x00

#if ENABLED(ULTRA_LCD)
  #define BEEPER_PIN -1

  #define LCD_PINS_RS 9
  #define LCD_PINS_ENABLE 8
  #define LCD_PINS_D4 7
  #define LCD_PINS_D5 6
  #define LCD_PINS_D6 5
  #define LCD_PINS_D7 4

  #define BTN_EN1   16
  #define BTN_EN2   17
  #define BTN_ENC   18//the click

  #define SD_DETECT_PIN -1

  //encoder rotation values
  #define encrot0 0
  #define encrot1 2
  #define encrot2 3
  #define encrot3 1
#endif

#if ENABLED(VIKI2) || ENABLED(miniVIKI)
  #define BEEPER_PIN 32 //FastIO
  #define DOGLCD_A0  42 //Non-FastIO
  #define DOGLCD_CS  43 //Non-FastIO
  #define LCD_SCREEN_ROT_180

  // (FastIO Pins)
  #define BTN_EN1 26
  #define BTN_EN2 27
  #define BTN_ENC 47

  #define SDSS 45
  #define SD_DETECT_PIN -1 // FastIO (Manual says 72)

  #define STAT_LED_RED_PIN 12 //Non-FastIO
  #define STAT_LED_BLUE_PIN 10 //Non-FastIO
#endif
