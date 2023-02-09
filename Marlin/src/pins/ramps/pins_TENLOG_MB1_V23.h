/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2022 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * Tenlog MB1 V2.3 pin assignments
 * ATmega2560
 */

#define REQUIRE_MEGA2560
#include "env_validate.h"

#if HOTENDS > 2 || E_STEPPERS > 2
  #error "Tenlog supports up to 2 hotends / E steppers."
#endif

#define BOARD_INFO_NAME      "Tenlog MB1 V2.3"
#define DEFAULT_MACHINE_NAME BOARD_INFO_NAME

// Just a wild guess because no schematics!
#define AVR_CHIPOSCILLATOR_FREQ 16000000

//
// Limit Switches
//
#define X_MIN_PIN                             PinE5
#define X_MAX_PIN                             PinE4
#define Y_MIN_PIN                             PinJ1
//#define Y_MAX_PIN                           PinJ0  // Connected to "DJ" plug on extruder heads
#define Z_MIN_PIN                             PinD3
#if ENABLED(BLTOUCH)
  #define SERVO0_PIN                          PinD2
#else
  #define Z_MAX_PIN                           PinD2
#endif

//
// Steppers
//
#define X_STEP_PIN                            PinF0
#define X_DIR_PIN                             PinF1
#define X_ENABLE_PIN                          PinD7

#define X2_STEP_PIN                           PinC1
#define X2_DIR_PIN                            PinC3
#define X2_ENABLE_PIN                         PinC7

#define Y_STEP_PIN                            PinF6
#define Y_DIR_PIN                             PinF7
#define Y_ENABLE_PIN                          PinF2

#define Z_STEP_PIN                            PinL3
#define Z_DIR_PIN                             PinL1
#define Z_ENABLE_PIN                          PinK0

#define Z2_STEP_PIN                           PinK3
#define Z2_DIR_PIN                            PinK4
#define Z2_ENABLE_PIN                         PinK2

#define E0_STEP_PIN                           PinF3
#define E0_DIR_PIN                            PinF4
#define E0_ENABLE_PIN                         PinF5

#define E1_STEP_PIN                           PinA4
#define E1_DIR_PIN                            PinA6
#define E1_ENABLE_PIN                         PinA2

//
// Temperature Sensors
//
#define TEMP_0_PIN                            PinJ0  // Analog Input
#define TEMP_1_PIN                            PinB7  // Analog Input
#define TEMP_BED_PIN                          PinJ1  // Analog Input

//
// Heaters / Fans
//
#define HEATER_0_PIN                          PinB5
#define HEATER_1_PIN                          PinB4
#define HEATER_BED_PIN                        PinH5

#define FAN_PIN                               PinH6
#define FAN2_PIN                              PinE3  // Normally this would be a servo pin

//#define NUM_RUNOUT_SENSORS                  PinE0
#define FIL_RUNOUT_PIN                        PinJ0
//#define FIL_RUNOUT2_PIN                     PinD0

//
// PSU and Powerloss Recovery
//
#if ENABLED(PSU_CONTROL)
  #define PS_ON_PIN                           PinG1  // The M80/M81 PSU pin for boards v2.1-2.3
#endif

//
// Misc. Functions
//
//#define CASE_LIGHT_PIN                      PinE3
//#ifndef LED_PIN
//  #define LED_PIN                           PinB7
//#endif

#if HAS_CUTTER
  //#define SPINDLE_LASER_PWM_PIN             -1  // Hardware PWM
  //#define SPINDLE_LASER_ENA_PIN             PinG5  // Pullup!
#endif

// Use the RAMPS 1.4 Analog input 5 on the AUX2 connector
//#define FILWIDTH_PIN                        PinE3  // Analog Input

#define SDSS                                  PinB0
#define SD_DETECT_PIN                         PinL0

//
// LCD / Controller
//

//#if IS_RRD_SC

//#ifndef BEEPER_PIN
//  #define BEEPER_PIN                        -1
//#endif

#define LCD_PINS_RS                           -1
#define LCD_PINS_ENABLE                       -1
#define LCD_PINS_D4                           -1
#define LCD_PINS_D5                           -1
#define LCD_PINS_D6                           -1
#define LCD_PINS_D7                           -1

//#define BTN_EN1                             PinC6
//#define BTN_EN2                             PinC4
//#define BTN_ENC                             PinC2

//#ifndef KILL_PIN
//  #define KILL_PIN                          PinG0
//#endif

//#endif // IS_RRD_SC
