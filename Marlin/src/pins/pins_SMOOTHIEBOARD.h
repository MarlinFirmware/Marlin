/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 * Copyright (C) 2017 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * Smoothieboard pin assignments
 */

#ifndef TARGET_LPC1768
  #error "Oops! Make sure you have the LPC1768 environment selected in your IDE."
#endif

#define BOARD_NAME          "Smoothieboard"
#define DEFAULT_WEBSITE_URL "http://smoothieware.org/smoothieboard"

#undef F_CPU
#define F_CPU 120000000

//
// Servos
//
#define SERVO0_PIN          P1_23

//
// Limit Switches
//
#define X_MIN_PIN           P1_24
#define X_MAX_PIN           P1_25
#define Y_MIN_PIN           P1_26
#define Y_MAX_PIN           P1_27
#define Z_MIN_PIN           P1_28
#define Z_MAX_PIN           P1_29

//
// Steppers
//
#define X_STEP_PIN          P2_00
#define X_DIR_PIN           P0_05
#define X_ENABLE_PIN        P0_04

#define Y_STEP_PIN          P2_01
#define Y_DIR_PIN           P0_11
#define Y_ENABLE_PIN        P0_10

#define Z_STEP_PIN          P2_02
#define Z_DIR_PIN           P0_20
#define Z_ENABLE_PIN        P0_19

#define E0_STEP_PIN         P2_03
#define E0_DIR_PIN          P0_22
#define E0_ENABLE_PIN       P0_21

#define E1_STEP_PIN         P2_08
#define E1_DIR_PIN          P2_13
#define E1_ENABLE_PIN       P4_29

//
// Temperature Sensors
// 3.3V max when defined as an analog input
//
#define TEMP_0_PIN          0   // P0.23 (T1)
#define TEMP_BED_PIN        1   // P0.24 (T2)
#define TEMP_1_PIN          2   // P0.25 (T3)
#define TEMP_2_PIN          3   // P0.26 (T4)

//
// Heaters / Fans
//
#define HEATER_BED_PIN      P2_05
#define HEATER_0_PIN        P2_07
#define HEATER_1_PIN        P1_23
#ifndef FAN_PIN
  #define FAN_PIN           P2_06
#endif
#define FAN1_PIN            P2_04

//
// Display
//
#if ENABLED(VIKI2) || ENABLED(miniVIKI)
  #define BEEPER_PIN        P1_31
  //#define DOGLCD_A0       P2_06
  #define DOGLCD_CS         P0_16

  #define BTN_EN1           P3_25
  #define BTN_EN2           P3_26
  #define BTN_ENC           P2_11

  #define SD_DETECT_PIN     P1_18
  #define SDSS              P1_21

  #define STAT_LED_RED_PIN  P1_19
  #define STAT_LED_BLUE_PIN P1_20
#endif
