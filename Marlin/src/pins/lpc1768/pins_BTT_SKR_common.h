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

#include "env_validate.h"

// If you have the Big tree tech driver expansion module, enable HAS_BTT_EXP_MOT
// https://github.com/bigtreetech/BTT-Expansion-module/tree/master/BTT%20EXP-MOT
//#define HAS_BTT_EXP_MOT 1

#if BOTH(HAS_WIRED_LCD, HAS_BTT_EXP_MOT)
  #if EITHER(CR10_STOCKDISPLAY, ENDER2_STOCKDISPLAY)
    #define EXP_MOT_USE_EXP2_ONLY 1
  #else
    #error "You can't use both an LCD and a Motor Expansion Module on EXP1/EXP2 at the same time."
  #endif
#endif

// Ignore temp readings during development.
//#define BOGUS_TEMPERATURE_GRACE_PERIOD    2000

//
// Steppers
//
#ifndef E1_STEP_PIN
  #define E1_STEP_PIN                      P0_01
#endif
#ifndef E1_DIR_PIN
  #define E1_DIR_PIN                       P0_00
#endif
#ifndef E1_ENABLE_PIN
  #define E1_ENABLE_PIN                    P0_10
#endif

//
// Temperature Sensors
//  3.3V max when defined as an analog input
//
#ifndef TEMP_0_PIN
  #define TEMP_0_PIN                    P0_24_A1  // A1 (T1) - (68) - TEMP_0_PIN
#endif
#ifndef TEMP_1_PIN
  #define TEMP_1_PIN                    P0_25_A2  // A2 (T2) - (69) - TEMP_1_PIN
#endif
#ifndef TEMP_BED_PIN
  #define TEMP_BED_PIN                  P0_23_A0  // A0 (T0) - (67) - TEMP_BED_PIN
#endif

#if HOTENDS == 1
  #if TEMP_SENSOR_PROBE
    #define TEMP_PROBE_PIN            TEMP_1_PIN
  #elif TEMP_SENSOR_CHAMBER
    #define TEMP_CHAMBER_PIN          TEMP_1_PIN
  #endif
#endif

//
// Heaters / Fans
//
#ifndef HEATER_0_PIN
  #define HEATER_0_PIN                     P2_07
#endif
#if HOTENDS == 1
  #ifndef FAN1_PIN
    #define FAN1_PIN                       P2_04
  #endif
#else
  #ifndef HEATER_1_PIN
    #define HEATER_1_PIN                   P2_04
  #endif
#endif
#ifndef FAN_PIN
  #define FAN_PIN                          P2_03
#endif
#ifndef HEATER_BED_PIN
  #define HEATER_BED_PIN                   P2_05
#endif

//
// LCD / Controller
//
#if !defined(BEEPER_PIN) && HAS_WIRED_LCD && DISABLED(LCD_USE_I2C_BUZZER)
  #define BEEPER_PIN                       P1_30  // (37) not 5V tolerant
#endif

//
// SD Support
//
#ifndef SDCARD_CONNECTION
  #if HAS_WIRED_LCD
    #define SDCARD_CONNECTION                LCD
  #else
    #define SDCARD_CONNECTION            ONBOARD
  #endif
#endif


#define ONBOARD_SD_CS_PIN                  P0_06  // Chip select for "System" SD card

#if SD_CONNECTION_IS(LCD) && ENABLED(SKR_USE_LCD_SD_CARD_PINS_FOR_CS)
  #error "SDCARD_CONNECTION must not be 'LCD' with SKR_USE_LCD_PINS_FOR_CS."
#endif

#if SD_CONNECTION_IS(LCD)
  #define SD_SCK_PIN                       P0_15
  #define SD_MISO_PIN                      P0_17
  #define SD_MOSI_PIN                      P0_18
  #define SD_SS_PIN                  EXP2_07_PIN
  #define SD_DETECT_PIN              EXP2_04_PIN

#elif SD_CONNECTION_IS(ONBOARD)
  #undef SD_DETECT_PIN
  #define SD_DETECT_PIN                    P0_27
  #define SD_SCK_PIN                       P0_07
  #define SD_MISO_PIN                      P0_08
  #define SD_MOSI_PIN                      P0_09
  #define SD_SS_PIN            ONBOARD_SD_CS_PIN
#elif SD_CONNECTION_IS(CUSTOM_CABLE)
  #error "No custom SD drive cable defined for this board."
#endif

#if HAS_BTT_EXP_MOT

  /**              _____                                      _____
   *           NC | · · | GND                             NC | · · | GND
   *           NC | · · | 1.31 (M1EN)            (M2EN) 1.23 | · · | 1.22 (M3EN)
   * (M1STP) 0.18 | · ·   3.25 (M1DIR)           (M1RX) 1.21 | · ·   1.20 (M1DIAG)
   * (M2DIR) 0.16 | · · | 3.26 (M2STP)           (M2RX) 1.19 | · · | 1.18 (M2DIAG)
   * (M3DIR) 0.15 | · · | 0.17 (M3STP)           (M3RX) 0.28 | · · | 1.30 (M3DIAG)
   *               -----                                      -----
   *               EXP2                                       EXP1
   *
   * NB In EXP_MOT_USE_EXP2_ONLY mode EXP1 is not used and M2EN and M3EN need to be jumpered to M1EN
   */

  // M1 on Driver Expansion Module
  #define E2_STEP_PIN                EXP2_05_PIN
  #define E2_DIR_PIN                 EXP2_06_PIN
  #define E2_ENABLE_PIN              EXP2_04_PIN
  #if !EXP_MOT_USE_EXP2_ONLY
    #define E2_DIAG_PIN              EXP1_06_PIN
    #define E2_CS_PIN                EXP1_05_PIN
    #if HAS_TMC_UART
      #define E2_SERIAL_TX_PIN       EXP1_05_PIN
      #define E2_SERIAL_RX_PIN       EXP1_05_PIN
    #endif
  #endif

  // M2 on Driver Expansion Module
  #define E3_STEP_PIN                EXP2_08_PIN
  #define E3_DIR_PIN                 EXP2_07_PIN
  #if !EXP_MOT_USE_EXP2_ONLY
    #define E3_ENABLE_PIN            EXP1_03_PIN
    #define E3_DIAG_PIN              EXP1_08_PIN
    #define E3_CS_PIN                EXP1_07_PIN
    #if HAS_TMC_UART
      #define E3_SERIAL_TX_PIN       EXP1_07_PIN
      #define E3_SERIAL_RX_PIN       EXP1_07_PIN
    #endif
  #else
    #define E3_ENABLE_PIN            EXP2_04_PIN
  #endif

  // M3 on Driver Expansion Module
  #define E4_STEP_PIN                EXP2_10_PIN
  #define E4_DIR_PIN                 EXP2_09_PIN
  #if !EXP_MOT_USE_EXP2_ONLY
    #define E4_ENABLE_PIN            EXP1_04_PIN
    #define E4_DIAG_PIN              EXP1_10_PIN
    #define E4_CS_PIN                EXP1_09_PIN
    #if HAS_TMC_UART
      #define E4_SERIAL_TX_PIN       EXP1_09_PIN
      #define E4_SERIAL_RX_PIN       EXP1_09_PIN
    #endif
  #else
    #define E4_ENABLE_PIN            EXP2_04_PIN
  #endif

#endif // HAS_BTT_EXP_MOT
