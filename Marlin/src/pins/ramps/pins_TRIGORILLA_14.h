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
 * Arduino Mega with RAMPS v1.4 for Anycubic
 */

#define BOARD_INFO_NAME "Anycubic RAMPS 1.4"

//
// Servos
//
#if MB(TRIGORILLA_14_11)
  #define SERVO0_PIN                           5
  #define SERVO1_PIN                           4
  #define SERVO2_PIN                          11
  #define SERVO3_PIN                           6
#endif

//
// PWM FETS
//
#if EITHER(FET_ORDER_EEF, FET_ORDER_EEB)
  #define MOSFET_B_PIN                        45  // HEATER1
#elif FET_ORDER_EFB
  #define MOSFET_B_PIN                         9  // FAN0
#else
  #define MOSFET_B_PIN                         7  // FAN1
#endif

#if FET_ORDER_EEB
  #define MOSFET_C_PIN                         8  // BED
#elif FET_ORDER_EFB
  #if DISABLED(ANYCUBIC_LCD_CHIRON)
    #define MOSFET_C_PIN                       8
  #else
    #define MOSFET_C_PIN                      45
  #endif
#else                                             // EEF, EFF
  #define MOSFET_C_PIN                         9
#endif

#if FET_ORDER_EEB
  #define FAN_PIN                              9  // Override pin 4 in pins_RAMPS.h
#endif

//
// Heaters / Fans
//
#if ANY(FET_ORDER_EEF, FET_ORDER_EEB, FET_ORDER_EFB)
  #define FAN1_PIN                             7
#endif
#define FAN2_PIN                              44
#ifndef E0_AUTO_FAN_PIN
  #define E0_AUTO_FAN_PIN                     44  // Used in Anycubic Kossel example config
#endif
#if ENABLED(ANYCUBIC_LCD_I3MEGA)
  #define CONTROLLER_FAN_PIN                   7
#endif

//
// AnyCubic standard pin mappings
//
//  On most printers, endstops are NOT all wired to the appropriate pins on the Trigorilla board.
//  For instance, on a Chiron, Y axis goes to an aux connector.
//  There are also other things that have been wired in creative ways.
//  To enable PIN definitions for a specific printer model, #define the appropriate symbol after
//  MOTHERBOARD in Configuration.h

//
// Limit Switches
//
//#define ANYCUBIC_4_MAX_PRO_ENDSTOPS

#if ENABLED(ANYCUBIC_4_MAX_PRO_ENDSTOPS)
  #define X_MAX_PIN                           43
  #define Y_STOP_PIN                          19
#elif EITHER(ANYCUBIC_LCD_CHIRON, ANYCUBIC_LCD_I3MEGA)
  #define Y_STOP_PIN                          42
  #define Z2_MIN_PIN                          43
  #ifndef Z_MIN_PROBE_PIN
    #define Z_MIN_PROBE_PIN                    2
  #endif
  #ifndef FIL_RUNOUT_PIN
    #if ENABLED(ANYCUBIC_LCD_CHIRON)
      #define FIL_RUNOUT_PIN                  33
    #else
      #define FIL_RUNOUT_PIN                  19
    #endif
  #endif
  #define BEEPER_PIN                          31
  #define SD_DETECT_PIN                       49
#endif

#include "pins_RAMPS.h"

//
// AnyCubic made the following changes to 1.1.0-RC8
// If these are appropriate for your LCD let us know.
//
#if 0 && HAS_WIRED_LCD

  // LCD Display output pins
  #if BOTH(IS_NEWPANEL, PANEL_ONE)
    #undef LCD_PINS_D6
    #define LCD_PINS_D6                       57
  #endif

  // LCD Display input pins
  #if IS_NEWPANEL
    #if ANY(VIKI2, miniVIKI)
      #undef DOGLCD_A0
      #define DOGLCD_A0                       23
    #elif ENABLED(ELB_FULL_GRAPHIC_CONTROLLER)
      #undef BEEPER_PIN
      #define BEEPER_PIN                      33
      #undef LCD_BACKLIGHT_PIN
      #define LCD_BACKLIGHT_PIN               67
    #endif
  #elif ENABLED(MINIPANEL)
    #undef BEEPER_PIN
    #define BEEPER_PIN                        33
    #undef DOGLCD_A0
    #define DOGLCD_A0                         42
  #endif

#endif // HAS_WIRED_LCD
