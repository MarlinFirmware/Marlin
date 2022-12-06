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
 * 3DRAG (and K8200 / K8400) Arduino Mega with RAMPS v1.4 pin assignments
 * ATmega2560, ATmega1280
 */

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "3Drag"
#endif

#ifndef DEFAULT_MACHINE_NAME
  #define DEFAULT_MACHINE_NAME BOARD_INFO_NAME
#endif

#ifndef DEFAULT_SOURCE_CODE_URL
  #define DEFAULT_SOURCE_CODE_URL "3dprint.elettronicain.it"
#endif

//
// Limit Switches
//
#define Z_STOP_PIN                            PinD3

//
// Steppers
//
#if HAS_CUTTER
  #define Z_DIR_PIN                           PinA6
  #define Z_ENABLE_PIN                        PinA2
  #define Z_STEP_PIN                          PinA4
#else
  #define Z_ENABLE_PIN                        PinK1
#endif

#if HAS_CUTTER && !HAS_EXTRUDERS
  #define E0_DIR_PIN                          -1
  #define E0_ENABLE_PIN                       -1
  #define E0_STEP_PIN                         -1
#endif

//
// Heaters / Fans
//
#define MOSFET_B_PIN                          PinH5
#define MOSFET_C_PIN                          PinH6
#define MOSFET_D_PIN                          PinB6

//
// Misc. Functions
//
#define SDSS                                  PinA3

#ifndef CASE_LIGHT_PIN
  #define CASE_LIGHT_PIN                      -1  // Hardware PWM but one is not available on expansion header
#endif

/**
 *  M3/M4/M5 - Spindle/Laser Control
 *
 *  If you want to control the speed of your spindle then you'll have
 *  have to sacrifce the Extruder and pull some signals off the Z stepper
 *  driver socket.
 *
 *  The following assumes:
 *   - the Z stepper driver socket is empty
 *   - the extruder driver socket has a driver board plugged into it
 *   - the Z stepper wires are attached the the extruder connector
 *
 *  If you want to keep the extruder AND don't have a LCD display then
 *  you can still control the power on/off and spindle direction.
 *
 *  Where to get spindle signals
 *
 *      stepper signal           socket name       socket name
 *                                          -------
 *       SPINDLE_LASER_ENA_PIN    /ENABLE  O|     |O  VMOT
 *                                    MS1  O|     |O  GND
 *                                    MS2  O|     |O  2B
 *                                    MS3  O|     |O  2A
 *                                 /RESET  O|     |O  1A
 *                                 /SLEEP  O|     |O  1B
 *          SPINDLE_LASER_PWM_PIN    STEP  O|     |O  VDD
 *                SPINDLE_DIR_PIN     DIR  O|     |O  GND
 *                                          -------
 *
 *  Note: Socket names vary from vendor to vendor
 */
#if HAS_CUTTER
  #if !HAS_EXTRUDERS
    #define SPINDLE_LASER_PWM_PIN             PinL3  // Hardware PWM
    #define SPINDLE_LASER_ENA_PIN             PinK0  // Pullup!
    #define SPINDLE_DIR_PIN                   PinL1
  #elif !BOTH(HAS_WIRED_LCD, IS_NEWPANEL)          // Use expansion header if no LCD in use
    #define SPINDLE_LASER_ENA_PIN             PinH1  // Pullup or pulldown!
    #define SPINDLE_DIR_PIN                   PinH0
    #if !NUM_SERVOS                               // Use servo connector if possible
      #define SPINDLE_LASER_PWM_PIN           PinH3  // Hardware PWM
    #elif HAS_FREE_AUX2_PINS
      #define SPINDLE_LASER_PWM_PIN           PinL5  // Hardware PWM
    #endif
  #endif
#endif

#include "pins_RAMPS.h"

//
// Heaters / Fans
//
#define HEATER_2_PIN                          PinH3

#undef SD_DETECT_PIN
#define SD_DETECT_PIN                         PinB0

//
// LCD / Controller
//
#if HAS_WIRED_LCD && IS_NEWPANEL
  #undef BEEPER_PIN

  // TODO: Remap EXP1/2 based on adapter
  #undef LCD_PINS_RS
  #undef LCD_PINS_ENABLE
  #undef LCD_PINS_D4
  #undef LCD_PINS_D5
  #undef LCD_PINS_D6
  #undef LCD_PINS_D7
  #define LCD_PINS_RS                         PinA5
  #define LCD_PINS_ENABLE                     PinA7
  #define LCD_PINS_D4                         PinC0
  #define LCD_PINS_D5                         PinC2
  #define LCD_PINS_D6                         PinC4
  #define LCD_PINS_D7                         PinC6

  // Buttons
  #undef BTN_EN1
  #undef BTN_EN2
  #undef BTN_ENC
  #define BTN_EN1                             PinH1
  #define BTN_EN2                             PinH0
  #define BTN_ENC                             PinA1

#else

  #define BEEPER_PIN                          PinC4

#endif // HAS_WIRED_LCD && IS_NEWPANEL

#if IS_U8GLIB_ST7920
  #define BOARD_ST7920_DELAY_1                 0
  #define BOARD_ST7920_DELAY_2               188
  #define BOARD_ST7920_DELAY_3                 0
#endif
