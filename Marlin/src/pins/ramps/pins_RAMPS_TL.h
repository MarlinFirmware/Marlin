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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma once

#ifdef TARGET_LPC1768
  #error "Oops! Set MOTHERBOARD to an LPC1768-based board when building for LPC1768."
#elif defined(__STM32F1__)
  #error "Oops! Set MOTHERBOARD to an STM32F1-based board when building for STM32F1."
#endif

#if NONE(IS_RAMPS_SMART, IS_RAMPS_DUO, IS_RAMPS4DUE, TARGET_LPC1768)
  #if !defined(__AVR_ATmega1280__) && !defined(__AVR_ATmega2560__)
    #error "Oops! Select 'Arduino/Genuino Mega or Mega 2560' in 'Tools > Board.'"
  #endif
#endif

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "RAMPS 1.4"
#endif

#define X_MIN_PIN                              3  // IGHMC, 3 in Marlin2 pins_RAMPS.h if X_STOP_PIN not defined
#define X_MAX_PIN                              2  // Ori 2 By zyf   - IGHMC,  2 in Marlin2 pins_RAMPS.h if X_STOP_PIN not defined
#define Y_MIN_PIN                             14  // Ori 14 by zyf  - IGHMC, 14 in Marlin2 pins_RAMPS.h if X_STOP_PIN not defined
//#define Y_MAX_PIN                           -1  //15              - IGHMC, 15 in Marlin2 pins_RAMPS.h if X_STOP_PIN not defined (nb: not used by ZYF)
#define Z_MIN_PIN                             18  // IGHMC, same in Marlin2 pins_RAMPS.h if Z_STOP_PIN not defined
#define Z_MAX_PIN                             19  // IGHMC, same in Marlin2 pins_RAMPS.h  not used in zyf

//
// Steppers, IGHMC zyf doesn't define any *_CS_PIN but equally
// they don't seem to be used elsewhere in Marlin.
// TODO: leave the CS_pins from Marlin and check they don't conflict with zyf
//
#define X_STEP_PIN                            54
#define X_DIR_PIN                             55
#define X_ENABLE_PIN                          38
#ifndef X_CS_PIN
  #define X_CS_PIN                            53  // SDSS
#endif

#define X2_STEP_PIN                           36
#define X2_DIR_PIN                            34
#define X2_ENABLE_PIN                         30

#define Y_STEP_PIN                            60
#define Y_DIR_PIN                             61
#define Y_ENABLE_PIN                          56
#ifndef Y_CS_PIN
  #define Y_CS_PIN                            49  // SD_DETECT_PIN
#endif

#define Z_STEP_PIN                            46
#define Z_DIR_PIN                             48
#define Z_ENABLE_PIN                          62
#ifndef Z_CS_PIN
  #define Z_CS_PIN                            40  // PS_ON_PIN
#endif

// IGHMC in Marlin2 pins the Z2 pins are chosen by _EPIN macro
// but their dual z stepper removes an extruder driver so zyf
// defined ZYF_DUAL_Z in configuration_adv

#define Z2_STEP_PIN                           65
#define Z2_DIR_PIN                            66
#define Z2_ENABLE_PIN                         64
#define Z2_MIN_PIN                            19

#ifdef P2P1                                       // IGHMC, p2p1 defined by ZYF in configuration_zyf, does no p2p1 refer to other machines?)
  #define E1_STEP_PIN                         26  // IGHMC, 36 Marlin2 pins_RAMPS.h
  #define E1_DIR_PIN                          28  // IGHMC, 34 Marlin2 pins_RAMPS.h
  #define E1_ENABLE_PIN                       24  // IGHMC, 30 Marlin2 pins_RAMPS.h

  #define E0_STEP_PIN                         57  // IGHMC, 26 Marlin2 pins_RAMPS.h
  #define E0_DIR_PIN                          58  // IGHMC, 28 Marlin2 pins_RAMPS.h
  #define E0_ENABLE_PIN                       59  // IGHMC, 24 Marlin2 pins_RAMPS.h
#else                                             // IGHMC, the zyf config should preventS this because P2P1 always there is it to re-use firmware for other printers?
  #define E0_STEP_PIN                         26  // IGHMC, same as Marlin2 pins_RAMPS.h
  #define E0_DIR_PIN                          28  // IGHMC, same as Marlin2 pins_RAMPS.h
  #define E0_ENABLE_PIN                       24  // IGHMC, same as Marlin2 pins_RAMPS.h

  #define E1_STEP_PIN                         57  // IGHMC, 36 Marlin2 pins_RAMPS.h
  #define E1_DIR_PIN                          58  // IGHMC, 34 Marlin2 pins_RAMPS.h
  #define E1_ENABLE_PIN                       59  // IGHMC, 24 Marlin2 pins_RAMPS.h
#endif

#define SDPOWER_PIN                           -1  // IGHMC SD_POWER_PIN in Marlin2, defined in Pins.h
#define SDSS                                  53  // IGHMC, same as Marlin2 pins_RAMPS.h
#define LED_PIN                               13  // IGHMC, same as Marlin2 pins_RAMPS.h

#define FAN_PIN                                9  // (Sprinter config)           // IGHMC Ramps_d9_pin in pins_RAMPS.h, may be equivalent in this case but marlin2 has logic to choose multiple fan pins
#define FAN2_Control
#define FAN2_PIN                               5  // IGHMC copied from configuration_zyh, TODO: check this fits with Marlin handling of fans which uses logic rather than explicit naming

#if ENABLED(PSU_CONTROL)
  #define PS_ON_PIN                           40  // zyf 40    //PF1                           // IGHMC, it's 12 Marlin2 pins_RAMPS.h  (?PF1)
#endif

#ifdef POWER_LOSS_RECOVERY                        // IGHMC from configuration_zyf, power loss handled diffently in Marlin2 see configuration_adv
  #define POWER_LOSS_DETECT_PIN               32  // zyf 32   //PF2
#else
  #define POWER_LOSS_DETECT_PIN               -1
#endif

#define ENGRAVE_PIN                           35  // IGHMC not used in Marlin2, appears in zyf Marlin_main against 'M2000' possibly in readiness for an engraver tool?
                                                  //  and in configuration_zyf where it is defined as 37
#ifdef P2P1
  #define HEATER_1_PIN                        10  // IGHMC, RAMPS_D9_PIN (sames as fan_pin??) Marlin2 pins_RAMPS.h
  #define HEATER_0_PIN                        11  // IGHMC, RAMPS_D10_PIN (sames as fan_pin??) Marlin2 pins_RAMPS.h
#else                                             // IGHMC, zyf switches them round for dual extruder??
  #define HEATER_0_PIN                        10  // IGHMC, RAMPS_D9_PIN (sames as fan_pin??) Marlin2 pins_RAMPS.h
  #define HEATER_1_PIN                        11  // IGHMC, RAMPS_D10_PIN (sames as fan_pin??) Marlin2 pins_RAMPS.h
#endif

#define HEATER_2_PIN                          -1  // IGHMC same as marlin2 pins.h but marlin2 defines with a longer set starting half way through (eg: heater 3, 4,5 etc)
#define TEMP_2_PIN                            -1  // Analog Input

#ifdef P2P1
  #define TEMP_1_PIN                          13  // Analog Input               - IGHMC, 15 Marlin2 pins_RAMPS.h
  #define TEMP_0_PIN                          15  // Analog Input  // 15 by zyf - IGHMC, 13 Marlin2 pins_RAMPS.h
#else                                             // IGHMC, zyf switches them round for dual extruder??
  #define TEMP_0_PIN                          13  // Analog Input               - IGHMC, 15 Marlin2 pins_RAMPS.h
  #define TEMP_1_PIN                          15  // Analog Input  // 15 by zyf - IGHMC, 13 Marlin2 pins_RAMPS.h
#endif

#define HEATER_BED_PIN                         8  //  by zyf  // BED            - IGHMC,      Ramps_d8_pin in pins_RAMPS.h   may be equivalent in this case but marlin2 has logic to choose multiple HEATER_0_PIN pin
#define TEMP_BED_PIN                          14  // Analog Input  // by zyf    - IGHMC, same as Marlin2 pins_RAMPS.h

#ifndef FAN_PIN
  #if EITHER(IS_RAMPS_EFB, IS_RAMPS_EFF)          // Hotend, Fan, Bed or Hotend, Fan, Fan
    #define FAN_PIN                 RAMPS_D9_PIN
  #elif EITHER(IS_RAMPS_EEF, IS_RAMPS_SF)         // Hotend, Hotend, Fan or Spindle, Fan
    #define FAN_PIN                 RAMPS_D8_PIN
  #elif ENABLED(IS_RAMPS_EEB)                     // Hotend, Hotend, Bed
    #define FAN_PIN                            4  // IO pin. Buffer needed
  #else                                           // Non-specific are "EFB" (i.e., "EFBF" or "EFBE")
    #define FAN_PIN                 RAMPS_D9_PIN
  #endif
#endif

#ifndef FILWIDTH_PIN
  #define FILWIDTH_PIN                         5  // Analog Input on AUX2
#endif

#define BEEPER_PIN                            -1
#define LCD_PINS_RS                           -1
#define LCD_PINS_ENABLE                       -1
#define LCD_PINS_D4                           -1
#define LCD_PINS_D5                           -1
#define LCD_PINS_D6                           -1
#define LCD_PINS_D7                           -1

#define SD_DETECT_PIN                         49  // Usually 49 but depends on display controller option
