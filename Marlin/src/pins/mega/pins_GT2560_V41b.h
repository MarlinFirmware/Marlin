/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * Geeetech GT2560 V4.1b Pins
 * Schematic (4.1B): https://green-candy.osdn.jp/external/MarlinFW/board_schematics/Geeetech%20GT2560%20V4.x%20+%20A20/GT2560V4.1BSCHA20T.pdf
 * Origin: https://www.geeetech.com/download.html?spm=a2g0s.imconversation.0.0.22d23e5fXlQBWv&download_id=45
 * ATmega2560
*/

#define ALLOW_MEGA1280
#include "env_validate.h"

#if HOTENDS > 3 || E_STEPPERS > 3
  #error "GT2560 supports up to 3 hotends / E steppers."
#endif

#define BOARD_INFO_NAME "GT2560 4.1b"

// LIMIT SWITCHES CONNECTORS

/*
 * X-Y-Z AXIS MAX LIMIT SWITCHES CONNECTOR (H6)
 *        ---------
 *       | 1  4  7 |  V5 32 Z0_MAX GND
 *       | 2  5  8 |  V5 26 Y_MAX  GND
 *       | 3  6  9 |  V5 22 X_MAX  GND
 *        ---------
 *           H6
 *
 * X AXIS MIN LIMIT SWITCH CONN
 *         ---
 *        | 1 | V5
 *        | 2 | 24 X_MIN
 *        | 3 | GND
 *         ---
 *          J3
 *
 * Y AXIS MIN LIMIT SWITCH CONN
 *         ---
 *        | 1 | V5
 *        | 2 | 28 Y_MIN
 *        | 3 | GND
 *         ---
 *          J4
 *
 * Z AXIS MIN LIMIT SWITCHES CONN
 *
 *         ---
 *        | 1 | V5
 *        | 2 | PE7 Z1_MIN
 *        | 3 | GND
 *         ---
 *          J5
 *
 *
 *         ---
 *        | 1 | V5
 *        | 2 | 30 Z0_MIN
 *        | 3 | GND
 *         ---
 *         J6
 *
 *  All limit switches signals have external 10k pull-up resistors
*/

#define X_MIN                                 24
#define X_MAX                                 22
#define Y_MIN                                 28
#define Y_MAX                                 26
#define Z0_MIN                                30
#define Z_MIN1                            Z0_MIN
#define Z1_MIN                               PE7
#define Z0_MAX                                32

#ifndef X_STOP_PIN
  #ifndef X_MIN_PIN
    #define X_MIN_PIN                      X_MIN
  #endif
  #ifndef X_MAX_PIN
    #define X_MAX_PIN                      X_MAX
  #endif
#endif
#ifndef Y_STOP_PIN
  #ifndef Y_MIN_PIN
    #define Y_MIN_PIN                      Y_MIN
  #endif
  #ifndef Y_MAX_PIN
    #define Y_MAX_PIN                      Y_MAX
  #endif
#endif
#ifndef Z_STOP_PIN
  #ifndef Z_MIN_PIN
    #define Z_MIN_PIN                     Z0_MIN
  #endif
  #ifndef Z_MAX_PIN
    #define Z_MAX_PIN                     Z0_MAX
  #endif
#endif

/*
 * filament runout sensors connections
 *
 * filament 1
 *         ---
 *        | 1 | V5
 *        | 2 | 66 F_DET0
 *        | 3 | GND
 *         ---
 *         J12
 *
 * filament 2
 *         ---
 *        | 1 | V5
 *        | 2 | 67 F_DET1
 *        | 3 | GND
 *         ---
 *         J13
 *
 * filament 3
 *         ---
 *        | 1 | V5
 *        | 2 | 54 F_DET2
 *        | 3 | GND
 *         ---
 *         J14
 */

#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                  F_DET0
#endif
#ifndef FIL_RUNOUT2_PIN
  #define FIL_RUNOUT2_PIN                 F_DET1
#endif
#ifndef FIL_RUNOUT3_PIN
  #define FIL_RUNOUT3_PIN                 F_DET2
#endif

//
// Power Recovery
//
#define POWER_LOSS_PIN                        69  // Pin to detect power loss
#define POWER_LOSS_STATE                     LOW

//
// Steppers
//
#define X_STEP_PIN                            37
#define X_DIR_PIN                             39
#define X_ENABLE_PIN                          35

#define Y_STEP_PIN                            31
#define Y_DIR_PIN                             33
#define Y_ENABLE_PIN                          29

#define Z_STEP_PIN                            25
#define Z_DIR_PIN                             23
#define Z_ENABLE_PIN                          27

#define E0_STEP_PIN                           46
#define E0_DIR_PIN                            44
#define E0_ENABLE_PIN                         12

#define E1_STEP_PIN                           49
#define E1_DIR_PIN                            47
#define E1_ENABLE_PIN                         48

#define E2_STEP_PIN                           43
#define E2_DIR_PIN                            45
#define E2_ENABLE_PIN                         41

/**           PRINT HEAD CONNECTOR
 *                         ------
 *   (PWM8_FAN0) FAN_E0 9 | 1  2 | 9 FAN_E0 24V PWM FROM (PWM8_FAN0)
 *               (T0) A11 | 3  4 | A11 (T0) Extruder Temp Sensor
 *                    GND | 5  6 | 30 Z_MIN1 same of (Z0_MIN)
 *                     V5 | 7  8 | 11 (PB5) servo for BL_TOUCH/3D_TOUCH
 *       (PB4_HE2) HE2 19 | 9 10 | GND
 *       (PB4_HE2) HE2 19 |11 12 | 19 HE2 24V PWM OUT FOR FIRST EXTRUDER (PB4_HE2)
 *                    V24 |13 14 | V24
 *                    V24 |15 16 | V24
 *                         ------
 *                           H3
 */

#define PWM8_FAN0                              9
#define T0                                    11  // Analog input
#define PB5                                   11  // digital out to servo BL_TOUCH/3D_TOUCH
#define SERVO0_PIN                           PB5  // BL_TOUCH/3D_TOUCH
#define PB4_HE2                               10  // analog PWM out to the first Extruder

//
// Z Probe PIN6 HEADER H3 (PRINT HEAD HEADER CONN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                 Z_MIN1
#endif

//
// Temperature Sensors
//
#define TEMP_0_PIN                            T0  // Analog Input
#define TEMP_1_PIN                             9  // Analog Input
#define TEMP_2_PIN                             8  // Analog Input
#define TEMP_BED_PIN                          10  // Analog Input

//
// Heaters / Fans
//
#define HEATER_0_PIN                     PB4_HE2
#define HEATER_1_PIN                           3
#define HEATER_2_PIN                           2
#define HEATER_BED_PIN                         4
#define FAN0_PIN                       PWM8_FAN0
#define FAN1_PIN                               8
#define FAN2_PIN                               7

//
// Misc. Functions
//
#define SD_DETECT_PIN                         38
#define SDSS                                  53
#define LED_PIN                               13  // Use 6 (case light) for external LED. 13 is internal (yellow) LED.
#define PS_ON_PIN                             12

#if NUM_RUNOUT_SENSORS < 3
  #define SUICIDE_PIN                         54  // This pin must be enabled at boot to keep power flowing
#endif

#ifndef CASE_LIGHT_PIN
  #define CASE_LIGHT_PIN                       6  // 21
#endif

/**           LCD CONNECTOR
 *                ------
 *            V5 | 1  2 | GND
 *   (LCM_D7) 36 | 3  4 |  5 (LCM_D6)
 *   (LCM_D5) 21 | 5  6 | GND
 *   (LCM_D4) 16 | 7  8 | 17 (LCM_EN)
 * (EC_PRESS) 19 | 9 10 | GND
 *       (RESET) |11 12 | 19 (BEEP)
 *                ------
 *                  H2
 *
 */

#define LCM_D4                                16  // used as BTN_EN1 for YHCB2004 LCD Module
#define LCM_D5                                21  // YHCB2004_SCK_PIN
#define LCM_D6                                 5  // YHCB2004_SS_PIN
#define LCM_D7                                36  // YHCB2004_MOSI_PIN
#define LCM_EN                                17  // BTN_EN2
#define EC_PRESS                              19  // BTN_ENC
#define BEEP                                  18

#define BEEPER_PIN                          BEEP
#define LCM_RS                                20  // pin named and connected to 10k pull-up resistor but unused
#if ENABLED(YHCB2004)
 #define YHCB2004_SS_PIN                  LCM_D6
 #define YHCB2004_SCK_PIN                 LCM_D5
 #define YHCB2004_MOSI_PIN                LCM_D7
 #define YHCB2004_MISO_PIN                   LCM_RS      // unused pin on V4.1b board
#endif

/*  Cannot use HAS_WIRED_LCD because V4.1b board has not LCD_PINS_RS wired to display connector
#elif HAS_WIRED_LCD
  #ifndef LCD_PINS_RS
    #define LCD_PINS_RS                       20
  #endif
  #ifndef LCD_PINS_EN
    #define LCD_PINS_EN                       17
  #endif
  #ifndef LCD_PINS_D4
    #define LCD_PINS_D4                       16
  #endif
  #ifndef LCD_PINS_D5
    #define LCD_PINS_D5                       21
  #endif
  #ifndef LCD_PINS_D6
    #define LCD_PINS_D6                        5
  #endif
  #ifndef LCD_PINS_D7
    #define LCD_PINS_D7                       36
  #endif
#endif
*/

#if ENABLED(YHCB2004)
  #ifndef BTN_EN1
    #define BTN_EN1                       LCM_D4
  #endif
  #ifndef BTN_EN2
    #define BTN_EN2                       LCM_EN
  #endif
  #ifndef BTN_ENC
    #define BTN_ENC                     EC_PRESS
  #endif
#elif IS_NEWPANEL
  #ifndef BTN_EN1
    #define BTN_EN1                           42
  #endif
  #ifndef BTN_EN2
    #define BTN_EN2                           40
  #endif
  #ifndef BTN_ENC
    #define BTN_ENC                           19
  #endif
#endif
