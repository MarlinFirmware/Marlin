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
 * Dreammaker Overlord v1.1 pin assignments
 * ATmega2560
 */

#if HOTENDS > 2 || E_STEPPERS > 2
  #error "Overlord supports up to 2 hotends / E steppers."
#endif

#include "env_validate.h"

#define BOARD_INFO_NAME         "OVERLORD"
#define DEFAULT_MACHINE_NAME    BOARD_INFO_NAME

//
// Limit Switches
//
#define X_STOP_PIN                            PinA2
#define Y_STOP_PIN                            PinA6
#define Z_MIN_PIN                             PinL3
#define Z_MAX_PIN                             PinC5

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                     PinL3  // JP4, Tfeed1
#endif

#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                      PinL5  // JP3, Tfeed2
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

#define E0_STEP_PIN                           PinL6
#define E0_DIR_PIN                            PinL4
#define E0_ENABLE_PIN                         PinG0

#define E1_STEP_PIN                           PinL0
#define E1_DIR_PIN                            PinL2
#define E1_ENABLE_PIN                         PinL1

//
// Temperature Sensors
//
#define TEMP_0_PIN                            PinH5  // Analog Input
#define TEMP_1_PIN                            PinH6  // Analog Input - Redundant temp sensor
#define TEMP_2_PIN                            PinB6  // Analog Input
#define TEMP_3_PIN                            PinJ1  // Analog Input
#define TEMP_BED_PIN                          PinB4  // Analog Input

//
// Heaters / Fans
//
#define HEATER_0_PIN                          PinE4
#define HEATER_1_PIN                          PinE5
#define HEATER_BED_PIN                        PinG5

#define FAN_PIN                               PinH4  // material cooling fan

//
// SD Card
//
#define SDSS                                  PinB0
#define SD_DETECT_PIN                         PinD7

//
// Misc. Functions
//
#define LED_PIN                               PinB7  // On PCB status led
#define PS_ON_PIN                             PinB6  // For stepper/heater/fan power. Active HIGH.
#define POWER_LOSS_PIN                        PinC3  // Power check - whether hotends/steppers/fans have power

#if ENABLED(BATTERY_STATUS_AVAILABLE)
  #undef BATTERY_STATUS_PIN
  #define BATTERY_STATUS_PIN                  PinA4  // Status of power loss battery, whether it is charged (low) or charging (high)
#endif
#if ENABLED(INPUT_VOLTAGE_AVAILABLE)
  #undef VOLTAGE_DETECTION_PIN
  #define VOLTAGE_DETECTION_PIN               PinB5  // Analog Input - ADC Voltage level of main input
#endif

//
// LCD / Controller
//
#if HAS_WIRED_LCD
  // OVERLORD OLED pins
  #define LCD_PINS_RS                         PinD1
  #define LCD_PINS_D5                         PinD0
  #define LCD_PINS_ENABLE                     PinJ0
  #define LCD_PINS_D4                         PinJ1
  #define LCD_PINS_D6                         PinE3
  #define LCD_PINS_D7                         PinH3
  #ifndef LCD_RESET_PIN
    #define LCD_RESET_PIN                     PinE3  // LCD_PINS_D6
  #endif
#endif

#if IS_NEWPANEL
  #define BTN_ENC                             PinH1  // Enter Pin
  #define BTN_UP                              PinD2  // Button UP Pin
  #define BTN_DOWN                            PinH0  // Button DOWN Pin
#endif

// Additional connectors/pins on the Overlord V1.X board
#define PCB_VERSION_PIN                       PinA0
#define APPROACH_PIN                          PinB5  // JP7, Tpd
#define GATE_PIN                              PinC1  // Threshold, JP6, Tg
