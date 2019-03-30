/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * Formbot Raptor 2 pin assignments
 */

#define DEFAULT_MACHINE_NAME "Formbot Raptor2"
#define BOARD_NAME           "Formbot Raptor2"

#define FAN_PIN             6

#include "pins_FORMBOT_RAPTOR.h"

#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN   22
#endif

#define GREEDY_PANEL ANY(PANEL_ONE, VIKI2, miniVIKI, MINIPANEL, REPRAPWORLD_KEYPAD)

//
// M3/M4/M5 - Spindle/Laser Control
//
#if ENABLED(SPINDLE_LASER_ENABLE) && !PIN_EXISTS(SPINDLE_LASER_ENABLE)
  #if !NUM_SERVOS                       // Try to use servo connector first
    #define SPINDLE_LASER_ENABLE_PIN  6 // Pin should have a pullup/pulldown!
    #define SPINDLE_LASER_PWM_PIN     4 // MUST BE HARDWARE PWM
    #define SPINDLE_DIR_PIN           5
  #elif !GREEDY_PANEL                   // Try to use AUX2
    #define SPINDLE_LASER_ENABLE_PIN 40 // Pin should have a pullup/pulldown!
    #define SPINDLE_LASER_PWM_PIN    44 // MUST BE HARDWARE PWM
    #define SPINDLE_DIR_PIN          65
  #endif
#endif

#if ENABLED(CASE_LIGHT_ENABLE) && !PIN_EXISTS(CASE_LIGHT)
  #if NUM_SERVOS <= 1                   // Try to use servo connector first
    #define CASE_LIGHT_PIN  6           // MUST BE HARDWARE PWM
  #elif !GREEDY_PANEL                   // Try to use AUX2
    #define CASE_LIGHT_PIN 44           // MUST BE HARDWARE PWM
  #endif
#endif

#undef GREEDY_PANEL

#if ENABLED(CASE_LIGHT_ENABLE) && PIN_EXISTS(CASE_LIGHT) && (CASE_LIGHT_PIN == SPINDLE_LASER_ENABLE_PIN || CASE_LIGHT_PIN == SPINDLE_LASER_PWM_PIN)
  #error "CASE_LIGHT_PIN conflicts with a Spindle / Laser pin."
#endif
