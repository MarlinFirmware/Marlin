/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * Arduino Mega with RAMPS v1.4 for Anycubic
 *
 * Defines RAMPS_D44_PIN 44
 * Defines FAN_PIN RAMPS_D44_PIN or -1
 */

#if !defined(__AVR_ATmega1280__) && !defined(__AVR_ATmega2560__)
  #error "Oops!  Make sure you have 'Arduino Mega' selected from the 'Tools -> Boards' menu."
#endif

#ifndef BOARD_NAME
  #define BOARD_NAME "RAMPS 1.4 for Anycubic"
#endif
#define IS_RAMPS_13
#define IS_RAMPS_EFB
#define RAMPS_D44_PIN 44

#include "pins_RAMPS.h"

#ifdef FAN_PIN
  #undef FAN_PIN
#endif
#define FAN_PIN RAMPS_D44_PIN

#ifdef ORIG_E0_AUTO_FAN_PIN
  #undef ORIG_E0_AUTO_FAN_PIN
#endif
#define ORIG_E0_AUTO_FAN_PIN  RAMPS_D44_PIN

#ifdef E1_STEP_PIN
  #undef E1_STEP_PIN
#endif
#ifdef E1_DIR_PIN
  #undef E1_DIR_PIN
#endif
#ifdef E1_ENABLE_PIN
  #undef E1_ENABLE_PIN
#endif
#ifdef E1_CS_PIN
  #undef E1_CS_PIN
#endif
