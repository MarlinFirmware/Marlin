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

/************************************************
 * Rambo pin assignments MODIFIED FOR Scoovo X9H
 ************************************************/

#ifndef __AVR_ATmega2560__
  #error "Oops!  Make sure you have 'Arduino Mega 2560' selected from the 'Tools -> Boards' menu."
#endif

#define BOARD_NAME "Scoovo X9H"

#define LARGE_FLASH true

//
// Servos
//
#define SERVO0_PIN 22 // Motor header MX1
#define SERVO1_PIN 23 // Motor header MX2
#define SERVO2_PIN 24 // Motor header MX3
#define SERVO3_PIN  5 // PWM header pin 5

//
// Limit Switches
//
#define X_MIN_PIN           12
#define X_MAX_PIN           24
#define Y_MIN_PIN           11
#define Y_MAX_PIN           23
#define Z_MIN_PIN           10
#define Z_MAX_PIN           30

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN   30
#endif

//
// Steppers
//
#define X_STEP_PIN          37
#define X_DIR_PIN           48
#define X_ENABLE_PIN        29

#define Y_STEP_PIN          36
#define Y_DIR_PIN           49
#define Y_ENABLE_PIN        28

#define Z_STEP_PIN          35
#define Z_DIR_PIN           47
#define Z_ENABLE_PIN        27

#define E0_STEP_PIN         34
#define E0_DIR_PIN          43
#define E0_ENABLE_PIN       26

#define E1_STEP_PIN         33
#define E1_DIR_PIN          42
#define E1_ENABLE_PIN       25

// Microstepping pins - Mapping not from fastio.h (?)
#define X_MS1_PIN           40
#define X_MS2_PIN           41
#define Y_MS1_PIN           69
#define Y_MS2_PIN           39
#define Z_MS1_PIN           68
#define Z_MS2_PIN           67
#define E0_MS1_PIN          65
#define E0_MS2_PIN          66
#define E1_MS1_PIN          63
#define E1_MS2_PIN          64

#define DIGIPOTSS_PIN       38
#define DIGIPOT_CHANNELS {4,5,3,0,1} // X Y Z E0 E1 digipot channels to stepper driver mapping

//
// Temperature Sensors
//
#define TEMP_0_PIN           0   // Analog Input
#define TEMP_BED_PIN         7   // Analog Input

//
// Heaters / Fans
//
#define HEATER_0_PIN         9
#define HEATER_1_PIN         7
#define HEATER_BED_PIN       3

#define FAN_PIN              8
#define FAN1_PIN             6
#define FAN2_PIN             2

//
// Misc. Functions
//
#define SDSS                53
#define LED_PIN             13
#define PS_ON_PIN            4

#ifndef FILWIDTH_PIN
  #define FILWIDTH_PIN       3   // Analog Input
#endif

//
// LCD / Controller
//
#define LCD_PINS_RS         70 // Ext2_5
#define LCD_PINS_ENABLE     71 // Ext2_7
#define LCD_PINS_D4         72 // Ext2_9 ?
#define LCD_PINS_D5         73 // Ext2_11 ?
#define LCD_PINS_D6         74 // Ext2_13
#define LCD_PINS_D7         75 // Ext2_15 ?
#define BEEPER_PIN          -1

#define BTN_HOME            80 // Ext_16
#define BTN_CENTER          81 // Ext_14
#define BTN_ENC             BTN_CENTER
#define BTN_RIGHT           82 // Ext_12
#define BTN_LEFT            83 // Ext_10
#define BTN_UP              84 // Ext2_8
#define BTN_DOWN            85 // Ext2_6

#define HOME_PIN            BTN_HOME

#if ENABLED(VIKI2) || ENABLED(miniVIKI)
  #define BEEPER_PIN        44
  // Pins for DOGM SPI LCD Support
  #define DOGLCD_A0         70
  #define DOGLCD_CS         71
  #define LCD_SCREEN_ROT_180

  #define SD_DETECT_PIN     -1 // Pin 72 if using easy adapter board

  #define STAT_LED_RED_PIN  22
  #define STAT_LED_BLUE_PIN 32
#endif // VIKI2/miniVIKI
