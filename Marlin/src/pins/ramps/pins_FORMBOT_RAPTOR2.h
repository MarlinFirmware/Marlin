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
 * Formbot Raptor 2 pin assignments
 * ATmega2560
 */

#define BOARD_INFO_NAME      "Formbot Raptor2"
#define DEFAULT_MACHINE_NAME BOARD_INFO_NAME

#define FAN0_PIN                               6

#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                      22
#endif

#include "pins_FORMBOT_RAPTOR.h"

#define GREEDY_PANEL ANY(PANEL_ONE, VIKI2, miniVIKI, MINIPANEL, REPRAPWORLD_KEYPAD)

//
// M3/M4/M5 - Spindle/Laser Control
//
#if HAS_CUTTER && !PIN_EXISTS(SPINDLE_LASER_ENA)
  #if !NUM_SERVOS                                 // Try to use servo connector first
    #define SPINDLE_LASER_PWM_PIN              4  // Hardware PWM
    #define SPINDLE_LASER_ENA_PIN              6  // Pullup or pulldown!
    #define SPINDLE_DIR_PIN                    5
  #elif !GREEDY_PANEL                             // Try to use AUX2
    #define SPINDLE_LASER_PWM_PIN             44  // Hardware PWM
    #define SPINDLE_LASER_ENA_PIN              4  // Pullup or pulldown!
    #define SPINDLE_DIR_PIN                   65
  #endif
#endif

#if ENABLED(CASE_LIGHT_ENABLE) && !PIN_EXISTS(CASE_LIGHT)
  #if NUM_SERVOS <= 1                             // Try to use servo connector first
    #define CASE_LIGHT_PIN                     6  // Hardware PWM
  #elif !GREEDY_PANEL                             // Try to use AUX2
    #define CASE_LIGHT_PIN                    44  // Hardware PWM
  #endif
#endif

#undef GREEDY_PANEL

#if ENABLED(CASE_LIGHT_ENABLE) && PIN_EXISTS(CASE_LIGHT) && (CASE_LIGHT_PIN == SPINDLE_LASER_ENA_PIN || CASE_LIGHT_PIN == SPINDLE_LASER_PWM_PIN)
  #error "CASE_LIGHT_PIN conflicts with a Spindle / Laser pin."
#endif

#if HAS_MARLINUI_U8GLIB
  #define BOARD_ST7920_DELAY_1               125
  #define BOARD_ST7920_DELAY_2               125
  #define BOARD_ST7920_DELAY_3               125
#endif
