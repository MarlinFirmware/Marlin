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
 *  Z-Bolt X Series board – based on Arduino Mega2560
 *  ATmega2560
 */

#define REQUIRE_MEGA2560
#include "env_validate.h"

#if HOTENDS > 4 || E_STEPPERS > 4
  #error "Z-Bolt X Series supports up to 4 hotends / E steppers."
#endif

#define BOARD_INFO_NAME "Z-Bolt X Series"

// Just a wild guess because no schematics!
#define AVR_CHIPOSCILLATOR_FREQ 16000000

//
// Servos
//
#ifndef SERVO0_PIN
  #define SERVO0_PIN                          PinB5
#endif
#ifndef SERVO3_PIN
  #define SERVO3_PIN                          PinG5
#endif

//
// Limit Switches
//
#define X_MIN_PIN                             PinE5
#ifndef X_MAX_PIN
  #define X_MAX_PIN                           PinE4
#endif
#define Y_MIN_PIN                             PinJ1
#define Y_MAX_PIN                             PinJ0
#define Z_MIN_PIN                             PinD3
#define Z_MAX_PIN                             PinD2

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                     PinC5
#endif

//
// Steppers
//
#define X_STEP_PIN                            PinF0
#define X_DIR_PIN                             PinF1
#define X_ENABLE_PIN                          PinD7
#ifndef X_CS_PIN
  #define X_CS_PIN                            -1
#endif

#define Y_STEP_PIN                            PinF6
#define Y_DIR_PIN                             PinF7
#define Y_ENABLE_PIN                          PinF2
#ifndef Y_CS_PIN
  #define Y_CS_PIN                            -1
#endif

#define Z_STEP_PIN                            PinL3
#define Z_DIR_PIN                             PinL1
#define Z_ENABLE_PIN                          PinK0
#ifndef Z_CS_PIN
  #define Z_CS_PIN                            -1
#endif

#define E0_STEP_PIN                           PinA4
#define E0_DIR_PIN                            PinA6
#define E0_ENABLE_PIN                         PinA2
#ifndef E0_CS_PIN
  #define E0_CS_PIN                           -1
#endif

#define E1_STEP_PIN                           PinC1
#define E1_DIR_PIN                            PinC3
#define E1_ENABLE_PIN                         PinC7
#ifndef E1_CS_PIN
  #define E1_CS_PIN                           -1
#endif

// Red
#define E2_STEP_PIN                           PinL7
#define E2_DIR_PIN                            PinG1
#define E2_ENABLE_PIN                         PinK3
#ifndef E2_CS_PIN
  #define E2_CS_PIN                           -1
#endif

// Black
#define E3_STEP_PIN                           PinL5
#define E3_DIR_PIN                            PinK2
#define E3_ENABLE_PIN                         PinK4
#ifndef E3_CS_PIN
  #define E3_CS_PIN                           -1
#endif

//
// Temperature Sensors
//
#define TEMP_0_PIN                            PinB7  // Analog Input
#define TEMP_1_PIN                            PinJ0  // Analog Input
#define TEMP_2_PIN                            PinE3  // Analog Input (BLACK)
#define TEMP_3_PIN                            PinH6  // Analog Input (RED)
#define TEMP_BED_PIN                          PinJ1  // Analog Input

//
// Heaters / Fans
//
#define HEATER_0_PIN                          PinB4
#define HEATER_1_PIN                          PinH4
#define HEATER_2_PIN                          PinH3
#define HEATER_3_PIN                          PinE3
#define HEATER_BED_PIN                        PinH5

#define FAN_PIN                               PinH6

//
// Misc. Functions
//
#define SDSS                                  PinB0
#define LED_PIN                               PinB7

#ifndef FILWIDTH_PIN
  #define FILWIDTH_PIN                        PinE3  // Analog Input on AUX2
#endif

// Оn the servos connector
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                      PinG5
#endif

#ifndef PS_ON_PIN
  #define PS_ON_PIN                           PinB6
#endif

#if ENABLED(CASE_LIGHT_ENABLE) && !defined(CASE_LIGHT_PIN) && !defined(SPINDLE_LASER_ENA_PIN)
  #if NUM_SERVOS <= 1                             // Prefer the servo connector
    #define CASE_LIGHT_PIN                    PinH3  // Hardware PWM
  #elif HAS_FREE_AUX2_PINS
    #define CASE_LIGHT_PIN                    PinL5  // Hardware PWM
  #endif
#endif

//
// M3/M4/M5 - Spindle/Laser Control
//
#if HAS_CUTTER && !PIN_EXISTS(SPINDLE_LASER_ENA)
  #if !defined(NUM_SERVOS) || NUM_SERVOS == 0     // Prefer the servo connector
    #define SPINDLE_LASER_ENA_PIN             PinG5  // Pullup or pulldown!
    #define SPINDLE_LASER_PWM_PIN             PinH3  // Hardware PWM
    #define SPINDLE_DIR_PIN                   PinE3
  #elif HAS_FREE_AUX2_PINS
    #define SPINDLE_LASER_ENA_PIN             PinG1  // Pullup or pulldown!
    #define SPINDLE_LASER_PWM_PIN             PinL5  // Hardware PWM
    #define SPINDLE_DIR_PIN                   PinK3
  #endif
#endif

//
// TMC SPI
//
#ifndef TMC_SPI_MOSI
  #define TMC_SPI_MOSI                        PinK4
#endif
#ifndef TMC_SPI_MISO
  #define TMC_SPI_MISO                        PinL5
#endif
#ifndef TMC_SPI_SCK
  #define TMC_SPI_SCK                         PinK2
#endif

#if HAS_TMC_UART
  /**
   * TMC220x stepper drivers
   *
   * Hardware serial communication ports.
   * If undefined software serial is used according to the pins below
   */
  //#define X_HARDWARE_SERIAL  Serial1
  //#define X2_HARDWARE_SERIAL Serial1
  //#define Y_HARDWARE_SERIAL  Serial1
  //#define Y2_HARDWARE_SERIAL Serial1
  //#define Z_HARDWARE_SERIAL  Serial1
  //#define Z2_HARDWARE_SERIAL Serial1
  //#define E0_HARDWARE_SERIAL Serial1
  //#define E1_HARDWARE_SERIAL Serial1
  //#define E2_HARDWARE_SERIAL Serial1
  //#define E3_HARDWARE_SERIAL Serial1
  //#define E4_HARDWARE_SERIAL Serial1

  #ifndef X_SERIAL_TX_PIN
    #define X_SERIAL_TX_PIN                   PinG1
  #endif
  #ifndef X_SERIAL_RX_PIN
    #define X_SERIAL_RX_PIN                   PinK1
  #endif
  #ifndef X2_SERIAL_TX_PIN
    #define X2_SERIAL_TX_PIN                  -1
  #endif
  #ifndef X2_SERIAL_RX_PIN
    #define X2_SERIAL_RX_PIN                  -1
  #endif

  #ifndef Y_SERIAL_TX_PIN
    #define Y_SERIAL_TX_PIN                   PinF5
  #endif
  #ifndef Y_SERIAL_RX_PIN
    #define Y_SERIAL_RX_PIN                   PinK2
  #endif
  #ifndef Y2_SERIAL_TX_PIN
    #define Y2_SERIAL_TX_PIN                  -1
  #endif
  #ifndef Y2_SERIAL_RX_PIN
    #define Y2_SERIAL_RX_PIN                  -1
  #endif

  #ifndef Z_SERIAL_TX_PIN
    #define Z_SERIAL_TX_PIN                   PinL7
  #endif
  #ifndef Z_SERIAL_RX_PIN
    #define Z_SERIAL_RX_PIN                   PinK3
  #endif
  #ifndef Z2_SERIAL_TX_PIN
    #define Z2_SERIAL_TX_PIN                  -1
  #endif
  #ifndef Z2_SERIAL_RX_PIN
    #define Z2_SERIAL_RX_PIN                  -1
  #endif

  #ifndef E0_SERIAL_TX_PIN
    #define E0_SERIAL_TX_PIN                  PinL5
  #endif
  #ifndef E0_SERIAL_RX_PIN
    #define E0_SERIAL_RX_PIN                  PinK4
  #endif
  #ifndef E1_SERIAL_TX_PIN
    #define E1_SERIAL_TX_PIN                  -1
  #endif
  #ifndef E1_SERIAL_RX_PIN
    #define E1_SERIAL_RX_PIN                  -1
  #endif
  #ifndef E2_SERIAL_TX_PIN
    #define E2_SERIAL_TX_PIN                  -1
  #endif
  #ifndef E2_SERIAL_RX_PIN
    #define E2_SERIAL_RX_PIN                  -1
  #endif
  #ifndef E3_SERIAL_TX_PIN
    #define E3_SERIAL_TX_PIN                  -1
  #endif
  #ifndef E3_SERIAL_RX_PIN
    #define E3_SERIAL_RX_PIN                  -1
  #endif
  #ifndef E4_SERIAL_TX_PIN
    #define E4_SERIAL_TX_PIN                  -1
  #endif
  #ifndef E4_SERIAL_RX_PIN
    #define E4_SERIAL_RX_PIN                  -1
  #endif
  #ifndef E5_SERIAL_TX_PIN
    #define E5_SERIAL_TX_PIN                  -1
  #endif
  #ifndef E5_SERIAL_RX_PIN
    #define E5_SERIAL_RX_PIN                  -1
  #endif
  #ifndef E6_SERIAL_TX_PIN
    #define E6_SERIAL_TX_PIN                  -1
  #endif
  #ifndef E6_SERIAL_RX_PIN
    #define E6_SERIAL_RX_PIN                  -1
  #endif
  #ifndef E7_SERIAL_TX_PIN
    #define E7_SERIAL_TX_PIN                  -1
  #endif
  #ifndef E7_SERIAL_RX_PIN
    #define E7_SERIAL_RX_PIN                  -1
  #endif
#endif
