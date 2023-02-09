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
 * AZTEEG_X3 Arduino Mega with RAMPS v1.4 pin assignments
 * Schematic: https://green-candy.osdn.jp/external/MarlinFW/board_schematics/AZTEEG_X3/AZTEEG%20X3%20PUB%20v1.12.pdf
 * Origin: http://files.panucatt.com/datasheets/azteegx3_designfiles.zip
 * ATmega2560
 */

#define REQUIRE_MEGA2560
#include "env_validate.h"

#if HOTENDS > 2 || E_STEPPERS > 2
  #error "Azteeg X3 supports up to 2 hotends / E steppers."
#endif

#if ENABLED(CASE_LIGHT_ENABLE) && !PIN_EXISTS(CASE_LIGHT)
  #define CASE_LIGHT_PIN                      PinH3  // Define before RAMPS pins include
#endif
#define BOARD_INFO_NAME "Azteeg X3"

#define AVR_CHIPOSCILLATOR_FREQ 16000000

//
// Servos
//
#define SERVO0_PIN                            PinL5  // SERVO1 port
#define SERVO1_PIN                            PinF1  // SERVO2 port

#include "pins_RAMPS_13.h" // ... RAMPS

//
// LCD / Controller
//
#undef STAT_LED_RED_PIN
#undef STAT_LED_BLUE_PIN

#if EITHER(VIKI2, miniVIKI)

  #undef DOGLCD_A0
  #undef DOGLCD_CS
  #undef BTN_ENC
  #define DOGLCD_A0                           PinC6
  #define DOGLCD_CS                           PinC5
  #define BTN_ENC                             PinB6

  #define STAT_LED_RED_PIN                    PinK2
  #define STAT_LED_BLUE_PIN                   PinK1

#else

  #define STAT_LED_RED_PIN                    PinH3
  #define STAT_LED_BLUE_PIN                   PinB5

#endif

//
// Misc
//
#if ENABLED(CASE_LIGHT_ENABLE) && PINS_EXIST(CASE_LIGHT, STAT_LED_RED) && STAT_LED_RED_PIN == CASE_LIGHT_PIN
  #undef STAT_LED_RED_PIN
#endif

//
// M3/M4/M5 - Spindle/Laser Control
//
#undef SPINDLE_LASER_PWM_PIN                      // Definitions in pins_RAMPS.h are no good with the AzteegX3 board
#undef SPINDLE_LASER_ENA_PIN
#undef SPINDLE_DIR_PIN

#if HAS_CUTTER
  #undef SDA                                      // use EXP3 header
  #undef SCL
  #if SERVO0_PIN == PinH4
    #undef SERVO0_PIN
    #define SERVO0_PIN                        PinB5
  #endif
  #define SPINDLE_LASER_PWM_PIN               PinH4  // Hardware PWM
  #define SPINDLE_LASER_ENA_PIN               PinD1  // Pullup!
  #define SPINDLE_DIR_PIN                     PinD0
#endif
