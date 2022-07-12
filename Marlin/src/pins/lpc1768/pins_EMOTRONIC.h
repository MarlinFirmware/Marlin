/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2022 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * eMotion-Tech eMotronic pin assignments
 * 
 * Board pins<->features assignments are based on the 
 * Micro-Delta Rework printer default connections.
 */

#include "env_validate.h"

#define BOARD_INFO_NAME   "eMotronic"
#define BOARD_WEBSITE_URL "www.reprap-france.com/article/lemotronic-quesaco"


//
// Limit Switches
//
#define _S0_PIN                            P1_22  // (S0)
#define _S1_PIN                            P1_23  // (S1)
#define _S2_PIN                            P1_24  // (S2)
#define _S3_PIN                            P0_26  // (S3)
#define _S4_PIN                            P0_26  // (S4: optocoupled)

#define X_MAX_PIN                          _S0_PIN
#define Y_MAX_PIN                          _S1_PIN
#define Z_MAX_PIN                          _S2_PIN
#define Z_MIN_PIN                          _S4_PIN // manual probe


//
// Steppers
//
#define _M1_STEP_PIN                       P2_00
#define _M1_DIR_PIN                        P3_26
#define _M1_ENABLE_PIN                     P3_25

#define _M2_STEP_PIN                       P2_01
#define _M2_DIR_PIN                        P0_22
#define _M2_ENABLE_PIN                     P0_21

#define _M3_STEP_PIN                       P2_02
#define _M3_DIR_PIN                        P4_28
#define _M3_ENABLE_PIN                     P4_29

#define _M4_STEP_PIN                       P2_03
#define _M4_DIR_PIN                        P0_04
#define _M4_ENABLE_PIN                     P0_05 // the doc wrongly says 0.3

#define _M5_STEP_PIN                       P2_08
#define _M5_DIR_PIN                        P0_20
#define _M5_ENABLE_PIN                     P0_19


#define X_STEP_PIN                         _M3_STEP_PIN
#define X_DIR_PIN                          _M3_DIR_PIN
#define X_ENABLE_PIN                       _M3_ENABLE_PIN

#define Y_STEP_PIN                         _M2_STEP_PIN
#define Y_DIR_PIN                          _M2_DIR_PIN
#define Y_ENABLE_PIN                       _M2_ENABLE_PIN

#define Z_STEP_PIN                         _M1_STEP_PIN
#define Z_DIR_PIN                          _M1_DIR_PIN
#define Z_ENABLE_PIN                       _M1_ENABLE_PIN

#define E0_STEP_PIN                        _M4_STEP_PIN
#define E0_DIR_PIN                         _M4_DIR_PIN
#define E0_ENABLE_PIN                      _M4_ENABLE_PIN

#define E1_STEP_PIN                        _M5_STEP_PIN
#define E1_DIR_PIN                         _M5_DIR_PIN
#define E1_ENABLE_PIN                      _M5_ENABLE_PIN


//
// Temperature Sensors
// 3.3V max when defined as an analog input
//
#define _TH0_PIN                           P0_23_A0  // (TH0)
#define _TH1_PIN                           P0_24_A1  // (TH1)
#define _TH2_PIN                           P0_25_A2  // (TH2)

#define TEMP_0_PIN                         _TH1_PIN
#define TEMP_1_PIN                         _TH0_PIN
#define TEMP_BED_PIN                       _TH2_PIN


//
// Heaters / Fans
//
#define _H0_PIN                            P2_04  // (H0: 15A)
#define _H1_PIN                            P2_05  // (H1: 10A shared)
#define _H2_PIN                            P2_06  // (H2: 10A shared)
#define _H3_PIN                            P2_07  // (H3: 10A shared)

#define _FAN0_PIN                          P2_11  // (FAN0: 1A)
#define _FAN1_PIN                          P2_13  // (FAN1: 1A)

#define HEATER_BED_PIN                     _H1_PIN
#define HEATER_0_PIN                       _H2_PIN
#define HEATER_1_PIN                       _H3_PIN

#define FAN_PIN                            _FAN0_PIN  // part cooling on Micro-Delta Rework
#define FAN1_PIN                           _FAN1_PIN  // extruder fan on Micro-Delta Rework


//
// Extension ports
//
#define _EXT1_01                           P0_03  // RX0
#define _EXT1_02                           P0_02  // TX0
#define _EXT1_03                           P1_30
#define _EXT1_04                           P1_31
#define _EXT1_05                           P0_17  // MISO0
#define _EXT1_06                           P0_18  // MOSI0
#define _EXT1_07                           P0_15  // SCK0
#define _EXT1_08                           P0_16  // SSEL0
#define _EXT1_09                           P0_27  // SDA0
#define _EXT1_10                           P0_28  // SCL0

#define _EXT2_01                           P0_10
#define _EXT2_02                           P0_11
#define _EXT2_03                           P0_00  // SDA1
#define _EXT2_04                           P0_01  // SCL1
#define _EXT2_05                           P1_28
#define _EXT2_06                           P1_29
#define _EXT2_07                           P1_26
#define _EXT2_08                           P1_27
#define _EXT2_09                           -1 /* GND */
#define _EXT2_10                           -1 /* 5V */


//
// SD Support
//
#ifndef SDCARD_CONNECTION
  #define SDCARD_CONNECTION              ONBOARD
#endif

#if SD_CONNECTION_IS(ONBOARD)
  #define SD_SCK_PIN                       P0_07
  #define SD_MISO_PIN                      P0_08
  #define SD_MOSI_PIN                      P0_09
  #define SD_SS_PIN                        P0_06
#elif SD_CONNECTION_IS(LCD)
  #define SD_SCK_PIN                       _EXT1_07
  #define SD_MISO_PIN                      _EXT1_05
  #define SD_MOSI_PIN                      _EXT1_06
  #define SD_SS_PIN                        _EXT1_08
  #define SD_DETECT_PIN                    _EXT1_04
#elif SD_CONNECTION_IS(CUSTOM_CABLE)
  #error "No custom SD drive cable defined for this board."
#endif


//
// LCD / Controller
//
#if ENABLED(EMOTION_TECH_LCD)
  #define BEEPER_PIN                       _EXT2_01
  #define DOGLCD_A0                        _EXT2_06
  #define DOGLCD_CS                        _EXT2_04
  #define DOGLCD_SCK                       _EXT1_07
  #define DOGLCD_MOSI                      _EXT1_06

  #define BTN_EN1                          _EXT2_07
  #define BTN_EN2                          _EXT2_05
  #define BTN_ENC                          _EXT2_02
#endif
