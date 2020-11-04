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

#if NOT_TARGET(__AVR_ATmega1280__, __AVR_ATmega2560__)
  #error "Oops! Select 'Arduino/Genuino Mega or Mega 2560' in 'Tools > Board.'"
#endif

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
#define DEFAULT_SOURCE_CODE_URL "https://github.com/Ultimaker/Marlin"

//
// Limit Switches
//
#if ENABLED(BOARD_REV_1_1_TO_1_3)
  #define X_MIN_PIN                           15  // SW1
  #define X_MAX_PIN                           14  // SW2
  #define Y_MIN_PIN                           17  // SW3
  #define Y_MAX_PIN                           16  // SW4
  #define Z_MIN_PIN                           19  // SW5
  #define Z_MAX_PIN                           18  // SW6
#endif

#if ENABLED(BOARD_REV_1_0)
  #if HAS_CUTTER
    #define X_STOP_PIN                        13  // SW1  (didn't change) - also has a useable hardware PWM
    #define Y_STOP_PIN                        12  // SW2
    #define Z_STOP_PIN                        11  // SW3
  #else
    #define X_MIN_PIN                         13  // SW1
    #define X_MAX_PIN                         12  // SW2
    #define Y_MIN_PIN                         11  // SW3
    #define Y_MAX_PIN                         10  // SW4
    #define Z_MIN_PIN                          9  // SW5
    #define Z_MAX_PIN                          8  // SW6
  #endif
#endif

#if ENABLED(BOARD_REV_1_5)
  #define X_MIN_PIN                           22
  #define X_MAX_PIN                           24
  #define Y_MIN_PIN                           26
  #define Y_MAX_PIN                           28
  #define Z_MIN_PIN                           30
  #define Z_MAX_PIN                           32
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
#define X_STEP_PIN                            25
#define X_DIR_PIN                             23
#define X_ENABLE_PIN                          27

#define Y_STEP_PIN                            31
#define Y_DIR_PIN                             33
#define Y_ENABLE_PIN                          29

#define Z_STEP_PIN                            37
#define Z_DIR_PIN                             39
#define Z_ENABLE_PIN                          35

#if BOTH(HAS_CUTTER, BOARD_REV_1_1_TO_1_3) && EXTRUDERS == 1
  // Move E0 to the spare and get Spindle/Laser signals from E0
  #define E0_STEP_PIN                         49
  #define E0_DIR_PIN                          47
  #define E0_ENABLE_PIN                       48
#else
  #define E0_STEP_PIN                         43
  #define E0_DIR_PIN                          45
  #define E0_ENABLE_PIN                       41

  #define E1_STEP_PIN                         49
  #define E1_DIR_PIN                          47
  #define E1_ENABLE_PIN                       48
#endif

//
// Temperature Sensors
//
#define TEMP_0_PIN                             8  // Analog Input
#define TEMP_1_PIN                             1  // Analog Input

//
// Heaters / Fans
//
#define HEATER_0_PIN                           2
//#define HEATER_1_PIN                         3  // used for case light   Rev A said "1"
#define HEATER_BED_PIN                         4

//
// LCD / Controller
//
#if ANY(BOARD_REV_1_0, BOARD_REV_1_1_TO_1_3)

  #define LCD_PINS_RS                         24
  #define LCD_PINS_ENABLE                     22
  #define LCD_PINS_D4                         36
  #define LCD_PINS_D5                         34
  #define LCD_PINS_D6                         32
  #define LCD_PINS_D7                         30

#elif BOTH(BOARD_REV_1_5, ULTRA_LCD)

  #define BEEPER_PIN                          18

  #if ENABLED(NEWPANEL)

    #define LCD_PINS_RS                       20
    #define LCD_PINS_ENABLE                   17
    #define LCD_PINS_D4                       16
    #define LCD_PINS_D5                       21
    #define LCD_PINS_D6                        5
    #define LCD_PINS_D7                        6

    // Buttons directly attached
    #define BTN_EN1                           40
    #define BTN_EN2                           42
    #define BTN_ENC                           19

    #define SD_DETECT_PIN                     38

  #else                                           // !NEWPANEL - Old style panel with shift register

    // Buttons attached to a shift register
    #define SHIFT_CLK                         38
    #define SHIFT_LD                          42
    #define SHIFT_OUT                         40
    #define SHIFT_EN                          17

    #define LCD_PINS_RS                       16
    #define LCD_PINS_ENABLE                    5
    #define LCD_PINS_D4                        6
    #define LCD_PINS_D5                       21
    #define LCD_PINS_D6                       20
    #define LCD_PINS_D7                       19

  #endif // !NEWPANEL

#endif

//
// case light  - see spindle section for more info on available hardware PWMs
//
#if !PIN_EXISTS(CASE_LIGHT) && ENABLED(BOARD_REV_1_5)
  #define CASE_LIGHT_PIN                       7  // use PWM -  MUST BE HARDWARE PWM
#endif

//
// M3/M4/M5 - Spindle/Laser Control
//
#if HAS_CUTTER
  #if EITHER(BOARD_REV_1_0, BOARD_REV_1_5)        // Use the last three SW positions
    #define SPINDLE_DIR_PIN                   10  // 1.0: SW4  1.5: EXP3-6 ("10")
    #define SPINDLE_LASER_PWM_PIN              9  // 1.0: SW5  1.5: EXP3-7 ( "9") .. MUST BE HARDWARE PWM
    #define SPINDLE_LASER_ENA_PIN              8  // 1.0: SW6  1.5: EXP3-8 ( "8") .. Pin should have a pullup!
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
      #define SPINDLE_DIR_PIN                 43
      #define SPINDLE_LASER_PWM_PIN           45  // Hardware PWM
      #define SPINDLE_LASER_ENA_PIN           41  // Pullup!
    #elif TEMP_SENSOR_BED == 0                    // Can't use E0 so see if HEATER_BED_PIN is available
      #undef HEATER_BED_PIN
      #define SPINDLE_DIR_PIN                 38  // Probably pin 4 on 10 pin connector closest to the E0 socket
      #define SPINDLE_LASER_PWM_PIN            4  // Hardware PWM - Special precautions usually needed.
      #define SPINDLE_LASER_ENA_PIN           40  // Pullup! (Probably pin 6 on the 10-pin
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
