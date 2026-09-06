/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * GMARSH X6 Rev.1 pin assignments
 * Schematic: https://github.com/gmarsh/gmarsh_x6/blob/master/armprinter_2208_1heater.pdf
 */

#include "env_validate.h"

#define BOARD_INFO_NAME "GMARSH X6 REV1"

// Ignore temp readings during development.
//#define BOGUS_TEMPERATURE_GRACE_PERIOD    2000

//
// Enable 12MHz clock output on P1.27 pin to sync TMC2208 chip clocks
//
#define LPC1768_ENABLE_CLKOUT_12M

//
// Servos
//
#define SERVO0_PIN                         P1_26  // PWM1[6]
#define SERVO1_PIN                         P1_18  // PWM1[1]

//
// Limit Switches
//
#define X_MIN_PIN                          P0_00
#define X_MAX_PIN                          P0_01
#define Y_MIN_PIN                          P0_10
#define Y_MAX_PIN                          P0_11
#define Z_MIN_PIN                          P2_13
#define Z_MAX_PIN                          P2_12

//
// Steppers
//

#define X_STEP_PIN                         P1_01
#define X_DIR_PIN                          P1_04
#define X_ENABLE_PIN                       P0_26

#define Y_STEP_PIN                         P1_10
#define Y_DIR_PIN                          P1_14
#define Y_ENABLE_PIN                       P1_08

#define Z_STEP_PIN                         P1_17
#define Z_DIR_PIN                          P4_29
#define Z_ENABLE_PIN                       P1_15

#define E0_STEP_PIN                        P0_05
#define E0_DIR_PIN                         P2_00
#define E0_ENABLE_PIN                      P4_28

#define E1_STEP_PIN                        P2_03
#define E1_DIR_PIN                         P2_04
#define E1_ENABLE_PIN                      P2_01

#define E2_STEP_PIN                        P2_07
#define E2_DIR_PIN                         P2_08
#define E2_ENABLE_PIN                      P2_05

//
// TMC2208 UART pins
//
#if HAS_TMC_UART
  #define X_SERIAL_TX_PIN                  P1_00
  #define Y_SERIAL_TX_PIN                  P1_09
  #define Z_SERIAL_TX_PIN                  P1_16
  #define E0_SERIAL_TX_PIN                 P0_04
  #define E1_SERIAL_TX_PIN                 P2_02
  #define E2_SERIAL_TX_PIN                 P2_06

  // Reduce baud rate to improve software serial reliability
  #ifndef TMC_BAUD_RATE
    #define TMC_BAUD_RATE                  19200
  #endif
#else
  #error "TMC2208 UART configuration is required for GMarsh X6."
#endif

//
// Temperature Sensors
//  3.3V max when defined as an analog input
//
#define TEMP_0_PIN                      P0_24_A1  // AD0[0] on P0_23
#define TEMP_BED_PIN                    P0_23_A0  // AD0[1] on P0_24

//
// Heaters / Fans
//
#define HEATER_BED_PIN                     P1_19  // Not a PWM pin, software PWM required
#define HEATER_0_PIN                       P3_26  // PWM1[3]
#define FAN0_PIN                           P3_25  // Part cooling fan - connected to PWM1[2]
#define E0_AUTO_FAN_PIN                    P0_27  // Extruder cooling fan

//
// Misc. Functions
//
#define LED_PIN                            P1_31
#define POWER_MONITOR_VOLTAGE_PIN       P0_25_A2

/**          ------                    ------
 *    P0_19 | 1  2 | P1_25      P0_17 | 1  2 | P0_15
 *    P0_21 | 3  4 | P0_20      P1_23 | 3  4 | P0_16
 *    P2_11   5  6 | P0_22      P1_24   5  6 | P0_18
 *    P1_29 | 7  8 | P1_28      P1_22 | 7  8 | (KILL)
 *      GND | 9 10 | 5V           GND | 9 10 | 5V
 *           ------                    ------
 *            EXP1                      EXP2
 */
#define EXP1_01_PIN                        P0_19
#define EXP1_02_PIN                        P1_25
#define EXP1_03_PIN                        P0_21  // Y+
#define EXP1_04_PIN                        P0_20
#define EXP1_05_PIN                        P2_11
#define EXP1_06_PIN                        P0_22
#define EXP1_07_PIN                        P1_29
#define EXP1_08_PIN                        P1_28

#define EXP2_01_PIN                        P0_17
#define EXP2_02_PIN                        P0_15
#define EXP2_03_PIN                        P1_23
#define EXP2_04_PIN                        P0_16
#define EXP2_05_PIN                        P1_24
#define EXP2_06_PIN                        P0_18
#define EXP2_07_PIN                        P1_22
#define EXP2_08_PIN                        -1     // RESET

//
// LCD
//
#if IS_RRD_SC

  #define BEEPER_PIN                 EXP1_01_PIN
  #define BTN_EN1                    EXP2_03_PIN
  #define BTN_EN2                    EXP2_05_PIN
  #define BTN_ENC                    EXP1_02_PIN
  #define LCD_PINS_RS                EXP1_04_PIN
  #define LCD_PINS_EN                EXP1_03_PIN
  #define LCD_PINS_D4                EXP1_05_PIN
  #define LCD_PINS_D5                EXP1_06_PIN
  #define LCD_PINS_D6                EXP1_07_PIN
  #define LCD_PINS_D7                EXP1_08_PIN

  #if ENABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER)
    #define BTN_ENC_EN               LCD_PINS_D7  // Detect the presence of the encoder
  #endif

#endif

//
// SD Support
//

#ifndef SDCARD_CONNECTION
  #if ENABLED(NO_LCD_SDCARD)
    #define SDCARD_CONNECTION            ONBOARD
  #else
    #define SDCARD_CONNECTION                LCD
  #endif
#endif

#if SD_CONNECTION_IS(LCD)
  #define SD_SCK_PIN                 EXP2_02_PIN
  #define SD_MISO_PIN                EXP2_01_PIN
  #define SD_MOSI_PIN                EXP2_06_PIN
  #define SD_SS_PIN                  EXP2_04_PIN
  #define SD_DETECT_PIN              EXP2_07_PIN
#elif SD_CONNECTION_IS(ONBOARD)
  #define SD_SCK_PIN                       P0_07
  #define SD_MISO_PIN                      P0_08
  #define SD_MOSI_PIN                      P0_09
  #define ONBOARD_SD_CS_PIN                P0_06  // Chip select for "System" SD card
  #define SD_SS_PIN            ONBOARD_SD_CS_PIN
#endif
