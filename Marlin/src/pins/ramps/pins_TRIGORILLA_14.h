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
// Limit Switches
//
//#define ANYCUBIC_4_MAX_PRO_ENDSTOPS

#define X_MIN_PIN                              3

#if ENABLED(ANYCUBIC_4_MAX_PRO_ENDSTOPS)
  #define X_MAX_PIN                           43
#else
  #define X_MAX_PIN                           43
#endif

#if ENABLED(ANYCUBIC_4_MAX_PRO_ENDSTOPS)
  #define Y_STOP_PIN                          19
#else
  #define Y_STOP_PIN                          42
#endif

#define Z_STOP_PIN                            18

//
// Z Probe (when not Z_MIN_PIN)
//
#define Z_MIN_PROBE_PIN                        2

#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                      19
#endif

//
// Heaters / Fans
//
#define TG_HEATER_BED_PIN                      8
#define TG_HEATER_0_PIN                       10
#define TG_HEATER_1_PIN                       45  // Anycubic Kossel: Unused

#define TG_FAN0_PIN                            9  // Anycubic Kossel: Usually the part cooling fan
#define TG_FAN1_PIN                            7  // Anycubic Kossel: Unused
#define TG_FAN2_PIN                           44  // Anycubic Kossel: Hotend fan

#define CONTROLLER_FAN_PIN           TG_FAN1_PIN

#define BEEPER_PIN                            31
#define SD_DETECT_PIN                         49

// Remap MOSFET pins to common usages:

#define RAMPS_D10_PIN            TG_HEATER_0_PIN  // HEATER_0_PIN is always RAMPS_D10_PIN in pins_RAMPS.h

#if HAS_MULTI_HOTEND                              // EEF and EEB
  #define RAMPS_D9_PIN           TG_HEATER_1_PIN
  #if !TEMP_SENSOR_BED
    // EEF
    #define RAMPS_D8_PIN             TG_FAN0_PIN
  #else
    // EEB
    #define RAMPS_D8_PIN       TG_HEATER_BED_PIN
    #define FAN_PIN                  TG_FAN0_PIN  // Override pin 4 in pins_RAMPS.h
  #endif
#elif TEMP_SENSOR_BED
  // EFB (Anycubic Kossel default)
  #define RAMPS_D9_PIN               TG_FAN0_PIN
  #define RAMPS_D8_PIN         TG_HEATER_BED_PIN
#else
  // EFF
  #define RAMPS_D9_PIN               TG_FAN1_PIN
  #define RAMPS_D8_PIN               TG_FAN0_PIN
#endif

#if HAS_MULTI_HOTEND || TEMP_SENSOR_BED           // EEF, EEB, EFB
  #define FAN1_PIN                   TG_FAN1_PIN
#endif
#define FAN2_PIN                     TG_FAN2_PIN

#ifndef E0_AUTO_FAN_PIN
  #define E0_AUTO_FAN_PIN            TG_FAN2_PIN  // Used in Anycubic Kossel example config
#endif

#include "pins_RAMPS.h"

//
// AnyCubic made the following changes to 1.1.0-RC8
// If these are appropriate for your LCD let us know.
//
#if 0 && HAS_SPI_LCD

  // LCD Display output pins
  #if BOTH(NEWPANEL, PANEL_ONE)
    #undef LCD_PINS_D6
    #define LCD_PINS_D6                       57
  #endif

  // LCD Display input pins
  #if ENABLED(NEWPANEL)
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

#endif // HAS_SPI_LCD
