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
 * Ultimaker pin assignments (Old electronics)
 * ATmega2560, ATmega1280
 */

/**
 * Rev B   3 JAN 2017
 *
 * Details on pin definitions for M3, M4 & M5 spindle control commands and for
 * the CASE_LIGHT_PIN are at the end of this file.
 *
 * This started out as an attempt to add pin definitions for M3, M4 & M5 spindle
 * control commands but quickly turned into a head scratcher as the sources for
 * the revisions provided inconsistent information.
 *
 * As best I can determine:
 *   1.5.3 boards should use the pins_ULTIMAKER.h file which means the BOARD_INFO_NAME
 *      define in this file should say 1.5.3 rather than 1.5.4
 *   This file is meant for 1.1 - 1.3 boards.
 *   The endstops for the 1.0 boards use different definitions than on the 1.1 - 1.3
 *      boards.
 *
 * I've added sections that have the 1.0 and 1.5.3 + endstop definitions so you can
 * easily switch if needed.  I've also copied over the 1.5.3 + LCD definitions.
 *
 * To be 100% sure of the board you have:
 *   1. In Configuration_adv.h enable "PINS_DEBUGGING"
 *   2. Compile & uploade
 *   3. Enter the command "M43 W1 I1".  This command will report that pin nmumber and
 *      name of any pin that changes state.
 *   4. Using a 1k (approximately) resistor pull the endstops and some of the LCD pins
 *      to ground and see what is reported.
 *   5. If the reported pin doesn't match the file then try a different board revision
 *      and repeat steps 2 - 5
 */

#define BOARD_REV_1_1_TO_1_3
//#define BOARD_REV_1_0
//#define BOARD_REV_1_5

#include "env_validate.h"

#ifdef BOARD_REV_1_1_TO_1_3
  #define BOARD_INFO_NAME       "Ultimaker 1.1-1.3"
#elif defined(BOARD_REV_1_0)
  #define BOARD_INFO_NAME       "Ultimaker 1.0"
#elif defined(BOARD_REV_1_5)
  #define BOARD_INFO_NAME       "Ultimaker 1.5"
#else
  #define BOARD_INFO_NAME       "Ultimaker 1.5.4+"
#endif
#define DEFAULT_MACHINE_NAME    "Ultimaker"
#define DEFAULT_SOURCE_CODE_URL "github.com/Ultimaker/Marlin"

//
// Limit Switches
//
#if ENABLED(BOARD_REV_1_1_TO_1_3)
  #define X_MIN_PIN                           PinJ0  // SW1
  #define X_MAX_PIN                           PinJ1  // SW2
  #define Y_MIN_PIN                           PinH0  // SW3
  #define Y_MAX_PIN                           PinH1  // SW4
  #define Z_MIN_PIN                           PinD2  // SW5
  #define Z_MAX_PIN                           PinD3  // SW6
#endif

#if ENABLED(BOARD_REV_1_0)
  #if HAS_CUTTER
    #define X_STOP_PIN                        PinB7  // SW1  (didn't change) - also has a useable hardware PWM
    #define Y_STOP_PIN                        PinB6  // SW2
    #define Z_STOP_PIN                        PinB5  // SW3
  #else
    #define X_MIN_PIN                         PinB7  // SW1
    #define X_MAX_PIN                         PinB6  // SW2
    #define Y_MIN_PIN                         PinB5  // SW3
    #define Y_MAX_PIN                         PinB4  // SW4
    #define Z_MIN_PIN                         PinH6  // SW5
    #define Z_MAX_PIN                         PinH5  // SW6
  #endif
#endif

#if ENABLED(BOARD_REV_1_5)
  #define X_MIN_PIN                           PinA0
  #define X_MAX_PIN                           PinA2
  #define Y_MIN_PIN                           PinA4
  #define Y_MAX_PIN                           PinA6
  #define Z_MIN_PIN                           PinC7
  #define Z_MAX_PIN                           PinC5
#endif

//
// Z Probe (when not Z_MIN_PIN)
//
#if !defined(Z_MIN_PROBE_PIN) && !BOTH(HAS_CUTTER, BOARD_REV_1_0)
  #define Z_MIN_PROBE_PIN              Z_MAX_PIN
#endif

//
// Steppers
//
#define X_STEP_PIN                            PinA3
#define X_DIR_PIN                             PinA1
#define X_ENABLE_PIN                          PinA5

#define Y_STEP_PIN                            PinC6
#define Y_DIR_PIN                             PinC4
#define Y_ENABLE_PIN                          PinA7

#define Z_STEP_PIN                            PinC0
#define Z_DIR_PIN                             PinG2
#define Z_ENABLE_PIN                          PinC2

#if BOTH(HAS_CUTTER, BOARD_REV_1_1_TO_1_3) && EXTRUDERS == 1
  // Move E0 to the spare and get Spindle/Laser signals from E0
  #define E0_STEP_PIN                         PinL0
  #define E0_DIR_PIN                          PinL2
  #define E0_ENABLE_PIN                       PinL1
#else
  #define E0_STEP_PIN                         PinL6
  #define E0_DIR_PIN                          PinL4
  #define E0_ENABLE_PIN                       PinG0

  #define E1_STEP_PIN                         PinL0
  #define E1_DIR_PIN                          PinL2
  #define E1_ENABLE_PIN                       PinL1
#endif

//
// Temperature Sensors
//
#define TEMP_0_PIN                            PinH5  // Analog Input
#define TEMP_1_PIN                            PinE1  // Analog Input

//
// Heaters / Fans
//
#define HEATER_0_PIN                          PinE4
//#define HEATER_1_PIN                        PinE5  // used for case light   Rev A said "1"
#define HEATER_BED_PIN                        PinG5

//
// LCD / Controller
//
#if EITHER(BOARD_REV_1_0, BOARD_REV_1_1_TO_1_3)

  #define LCD_PINS_RS                         PinA2
  #define LCD_PINS_ENABLE                     PinA0
  #define LCD_PINS_D4                         PinC1
  #define LCD_PINS_D5                         PinC3
  #define LCD_PINS_D6                         PinC5
  #define LCD_PINS_D7                         PinC7

#elif BOTH(BOARD_REV_1_5, HAS_WIRED_LCD)

  #define BEEPER_PIN                          PinD3

  #if IS_NEWPANEL

    #define LCD_PINS_RS                       PinD1
    #define LCD_PINS_ENABLE                   PinH0
    #define LCD_PINS_D4                       PinH1
    #define LCD_PINS_D5                       PinD0
    #define LCD_PINS_D6                       PinE3
    #define LCD_PINS_D7                       PinH3

    // Buttons directly attached
    #define BTN_EN1                           PinG1
    #define BTN_EN2                           PinL7
    #define BTN_ENC                           PinD2

    #define SD_DETECT_PIN                     PinD7

  #else                                           // !IS_NEWPANEL - Old style panel with shift register

    // Buttons attached to a shift register
    #define SHIFT_CLK_PIN                     PinD7
    #define SHIFT_LD_PIN                      PinL7
    #define SHIFT_OUT_PIN                     PinG1
    #define SHIFT_EN_PIN                      PinH0

    #define LCD_PINS_RS                       PinH1
    #define LCD_PINS_ENABLE                   PinE3
    #define LCD_PINS_D4                       PinH3
    #define LCD_PINS_D5                       PinD0
    #define LCD_PINS_D6                       PinD1
    #define LCD_PINS_D7                       PinD2

  #endif // !IS_NEWPANEL

#endif

//
// case light  - see spindle section for more info on available hardware PWMs
//
#if !PIN_EXISTS(CASE_LIGHT) && ENABLED(BOARD_REV_1_5)
  #define CASE_LIGHT_PIN                      PinH4  // use PWM -  MUST BE HARDWARE PWM
#endif

//
// M3/M4/M5 - Spindle/Laser Control
//
#if HAS_CUTTER
  #if EITHER(BOARD_REV_1_0, BOARD_REV_1_5)        // Use the last three SW positions
    #define SPINDLE_DIR_PIN                   PinB4  // 1.0: SW4  1.5: EXP3-6 ("10")
    #define SPINDLE_LASER_PWM_PIN             PinH6  // 1.0: SW5  1.5: EXP3-7 ( "9") .. MUST BE HARDWARE PWM
    #define SPINDLE_LASER_ENA_PIN             PinH5  // 1.0: SW6  1.5: EXP3-8 ( "8") .. Pin should have a pullup!
  #elif ENABLED(BOARD_REV_1_1_TO_1_3)
    /**
     * Only four hardware PWMs physically connected to anything on these boards:
     *
     *   HEATER_0_PIN    2  silkscreen varies - usually "PWM 1" or "HEATER1"
     *   HEATER_1_PIN    3  silkscreen varies - usually "PWM 2" or "HEATER2"
     *   HEATER_BED_PIN  4  silkscreen varies - usually "PWM 3" or "HEATED BED"
     *   E0_DIR_PIN     45
     *
     * If one of the heaters is used then special precautions will usually be needed.
     * They have an LED and resistor pullup to +24V which could damage 3.3V-5V ICs.
     */
    #if EXTRUDERS == 1
      #define SPINDLE_DIR_PIN                 PinL6
      #define SPINDLE_LASER_PWM_PIN           PinL4  // Hardware PWM
      #define SPINDLE_LASER_ENA_PIN           PinG0  // Pullup!
    #elif TEMP_SENSOR_BED == 0                    // Can't use E0 so see if HEATER_BED_PIN is available
      #undef HEATER_BED_PIN
      #define SPINDLE_DIR_PIN                 PinD7  // Probably pin 4 on 10 pin connector closest to the E0 socket
      #define SPINDLE_LASER_PWM_PIN           PinG5  // Hardware PWM - Special precautions usually needed.
      #define SPINDLE_LASER_ENA_PIN           PinG1  // Pullup! (Probably pin 6 on the 10-pin
                                            // connector closest to the E0 socket)
    #endif
  #endif
#endif

/**
 *  Where to get the spindle signals on the E0 socket
 *
 *         spindle signal     socket name       socket name
 *                                       -------
 * SPINDLE_LASER_ENA_PIN    /ENABLE  *|     |O  VMOT
 *                                 MS1  O|     |O  GND
 *                                 MS2  O|     |O  2B
 *                                 MS3  O|     |O  2A
 *                              /RESET  O|     |O  1A
 *                              /SLEEP  O|     |O  1B
 *          SPINDLE_DIR_PIN       STEP  O|     |O  VDD
 *    SPINDLE_LASER_PWM_PIN        DIR  O|     |O  GND
 *                                       -------
 *  * - pin closest to MS1, MS2 & MS3 jumpers on the board
 *
 *  Note: Socket names vary from vendor to vendor.
 */
