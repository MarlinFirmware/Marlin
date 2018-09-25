/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2018 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * Fysetc F6 pin assignments
 */

#ifndef __AVR_ATmega2560__
  #error "Oops!  Make sure you have 'FYSETC_F6' selected from the 'Tools -> Boards' menu."
#endif

#if MB(FYSETC_F6_13)
  #define BOARD_NAME "FYSETC_F6_13"
#endif

//
// Limit Switches
//
#define X_MIN_PIN          63
#define X_MAX_PIN          64
#define Y_MIN_PIN          14
#define Y_MAX_PIN          15
#define Z_MIN_PIN          12
#define Z_MAX_PIN          9

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN  -1 // Z-min is recommended, of course, it can be any other idle pins.
#endif

//
// Steppers
//
#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38

#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56

#define Z_STEP_PIN         43
#define Z_DIR_PIN          48
#define Z_ENABLE_PIN       58

#define E0_STEP_PIN        26
#define E0_DIR_PIN         28
#define E0_ENABLE_PIN      24

#define E1_STEP_PIN        36
#define E1_DIR_PIN         34
#define E1_ENABLE_PIN      30

#define E2_STEP_PIN        59
#define E2_DIR_PIN         57
#define E2_ENABLE_PIN 40

/* DIAG
 * DIAG is not directly connected to the MCU. Please close
 * the jumper next to the limit switch socket when using it.
 */

#define X_CS_PIN           70
#define Y_CS_PIN           39
#define Z_CS_PIN           74

#define E0_CS_PIN          47
#define E1_CS_PIN          32
#define E2_CS_PIN          42

#define X_TMC2130_DIAG     -1
#define Y_TMC2130_DIAG     -1
#define Z_TMC2130_DIAG     -1
#define E0_TMC2130_DIAG    -1
#define E1_TMC2130_DIAG    -1
#define E2_TMC2130_DIAG    -1


#if HAS_DRIVER(TMC2208)
/**
 * Software serial
 */

 #define X_SERIAL_RX_PIN       71
 #define X_SERIAL_TX_PIN       72

 #define Y_SERIAL_RX_PIN       73
 #define Y_SERIAL_TX_PIN       75

 #define Z_SERIAL_RX_PIN       78
 #define Z_SERIAL_TX_PIN       79

 #define E0_SERIAL_RX_PIN      76
 #define E0_SERIAL_TX_PIN      77

 #define E1_SERIAL_RX_PIN      80
 #define E1_SERIAL_TX_PIN      81

 #define E2_SERIAL_RX_PIN      22
 #define E2_SERIAL_TX_PIN      83
#endif

//
// Temperature Sensors
//
#define TEMP_0_PIN         12   // Analog Input
#define TEMP_1_PIN         13   // Analog Input
#define TEMP_2_PIN         14   // Analog Input
#define TEMP_BED_PIN       15   // Analog Input

//
// Heaters / Fans
//
#define HEATER_0_PIN       5
#define HEATER_1_PIN       6
#define HEATER_2_PIN       7
#define HEATER_BED_PIN     8

#define FAN_PIN            44
#define FAN1_PIN           45
#define FAN2_PIN           46

//
// Misc. Functions
//
#define SDSS               53
#define LED_PIN            13

#if ENABLED(RGB_LED) || ENABLED(RGBW_LED)
  #define RGB_LED_R_PIN    44
  #define RGB_LED_G_PIN    45
  #define RGB_LED_B_PIN    46
  #define RGB_LED_W_PIN    -1
#endif

#ifndef FILWIDTH_PIN
  #define FILWIDTH_PIN      9   // Analog Input on endstop X+
#endif

// define digital pin 10 for the filament runout sensor. Use the digital input 10 on the servos connector
#define FIL_RUNOUT_PIN      10

// define digital pin 11 for the PS_ON control. Use the digital input 11 on the servos connector
#ifndef PS_ON_PIN
  #define PS_ON_PIN         11
#endif

//#if ENABLED(CASE_LIGHT_ENABLE) && !PIN_EXISTS(CASE_LIGHT) && !defined(SPINDLE_LASER_ENABLE_PIN)
//  #if !defined(NUM_SERVOS) || NUM_SERVOS == 0 // try to use servo connector first
//    #define CASE_LIGHT_PIN   6      // MUST BE HARDWARE PWM
//  #elif !(ENABLED(ULTRA_LCD) && ENABLED(NEWPANEL) \
//      && (ENABLED(PANEL_ONE) || ENABLED(VIKI2) || ENABLED(miniVIKI) || ENABLED(MINIPANEL) || ENABLED(REPRAPWORLD_KEYPAD)))  // try to use AUX 2
//    #define CASE_LIGHT_PIN   44     // MUST BE HARDWARE PWM
//  #endif
//#endif

//
// M3/M4/M5 - Spindle/Laser Control
//
//#if ENABLED(SPINDLE_LASER_ENABLE) && !PIN_EXISTS(SPINDLE_LASER_ENABLE)
//  #if !defined(NUM_SERVOS) || NUM_SERVOS == 0 // try to use servo connector first
//    #define SPINDLE_LASER_ENABLE_PIN  4  // Pin should have a pullup/pulldown!
//    #define SPINDLE_LASER_PWM_PIN     6  // MUST BE HARDWARE PWM
//    #define SPINDLE_DIR_PIN           5
//  #elif !(ENABLED(ULTRA_LCD) && ENABLED(NEWPANEL) \
//      && (ENABLED(PANEL_ONE) || ENABLED(VIKI2) || ENABLED(miniVIKI) || ENABLED(MINIPANEL) || ENABLED(REPRAPWORLD_KEYPAD)))  // try to use AUX 2
//    #define SPINDLE_LASER_ENABLE_PIN 40  // Pin should have a pullup/pulldown!
//    #define SPINDLE_LASER_PWM_PIN    44  // MUST BE HARDWARE PWM
//    #define SPINDLE_DIR_PIN          65
//  #endif
//#endif

//////////////////////////
// LCDs and Controllers //
//////////////////////////
#define LCD_PINS_RS         16
#define LCD_PINS_ENABLE     17
#define LCD_PINS_D4         23
#define LCD_PINS_D5         25
#define LCD_PINS_D6         27
#define LCD_PINS_D7         29
#define BEEPER_PIN          37
#define BTN_EN1             31
#define BTN_EN2             33
#define BTN_ENC             35
#define SD_DETECT_PIN       49
#define KILL_PIN            41
