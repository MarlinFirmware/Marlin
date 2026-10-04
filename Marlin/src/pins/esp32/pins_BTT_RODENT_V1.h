/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * BigTreeTech Rodent V1.0 / V1.1 pin assignments
 * https://github.com/bigtreetech/Rodent
 *
 * A 4-driver CNC controller. No thermistor inputs, so no hotends or bed.
 * Step/dir/enable for all four drivers go through two 74HC595s fed by the
 * I2S stepper stream, so those are expander pins (128 + bit).
 *
 * The four onboard TMC2160s are daisy-chained on VSPI with one shared CS.
 * In Configuration_adv.h set the chain positions to match the board:
 *   X_CHAIN_POS 1, Y_CHAIN_POS 2, Z_CHAIN_POS 3, E0_CHAIN_POS 4
 * An axis auto-assigned to the E socket (e.g. Z2 or I) takes position 4.
 */

#include "env_validate.h"

#if E_STEPPERS > 1
  #error "BTT Rodent has only one E stepper driver."
#elif HAS_HOTEND || TEMP_SENSOR_BED
  #error "BTT Rodent has no thermistor inputs. Set TEMP_SENSOR_0 and TEMP_SENSOR_BED to 0."
#endif

#define BOARD_INFO_NAME      "BTT Rodent V1"
#define BOARD_WEBSITE_URL    "github.com/bigtreetech/Rodent"
#define DEFAULT_MACHINE_NAME BOARD_INFO_NAME

// MAX_EXPANDER_BITS is set for this board in HAL/ESP32/inc/Conditionals_adv.h

//
// Limit Switches
//
// Optocoupler-isolated inputs with 10K pull-ups. The DIAG jumpers on the back
// connect each driver's DIAG output to the matching input for sensorless homing.
//
#define X_STOP_PIN                            35  // X-MAX / DIAGX (input only)
#define Y_STOP_PIN                            34  // Y-MAX / DIAGY (input only)
#define Z_STOP_PIN                            33  // Z-MAX / DIAGZ
// E0-MAX / DIAGE is GPIO32
// E1-MAX is GPIO39 on V1.1 and GPIO37 on V1.0 (input only)

//
// Probe
//
#define Z_MIN_PROBE_PIN                       36  // Probe (input only)

//
// I2S stepper stream
//
#ifndef I2S_STEPPER_STREAM
  #define I2S_STEPPER_STREAM
#endif
#if ENABLED(I2S_STEPPER_STREAM)
  #define I2S_WS                              17
  #define I2S_BCK                             22
  #define I2S_DATA                            21
#endif

//
// Steppers - all on the I2S expander (2 x 74HC595)
//
#define X_STEP_PIN                           130  // i2so.2
#define X_DIR_PIN                            129  // i2so.1
#define X_ENABLE_PIN                         128  // i2so.0
#ifndef X_CS_PIN
  #define X_CS_PIN                             5  // Shared by the chain
#endif

#define Y_STEP_PIN                           133  // i2so.5
#define Y_DIR_PIN                            132  // i2so.4
#define Y_ENABLE_PIN                         135  // i2so.7
#ifndef Y_CS_PIN
  #define Y_CS_PIN                      X_CS_PIN
#endif

#define Z_STEP_PIN                           138  // i2so.10
#define Z_DIR_PIN                            137  // i2so.9
#define Z_ENABLE_PIN                         136  // i2so.8
#ifndef Z_CS_PIN
  #define Z_CS_PIN                      X_CS_PIN
#endif

#define E0_STEP_PIN                          141  // i2so.13
#define E0_DIR_PIN                           140  // i2so.12
#define E0_ENABLE_PIN                        143  // i2so.15
#ifndef E0_CS_PIN
  #define E0_CS_PIN                     X_CS_PIN
#endif

//
// Integrated TMC2160 drivers
//
#if  (HAS_X_AXIS && !AXIS_DRIVER_TYPE_X(TMC2160)) \
  || (HAS_Y_AXIS && !AXIS_DRIVER_TYPE_Y(TMC2160)) \
  || (HAS_Z_AXIS && !AXIS_DRIVER_TYPE_Z(TMC2160)) \
  || (E_STEPPERS && !AXIS_DRIVER_TYPE_E0(TMC2160))
  #error "All DRIVER TYPEs must be TMC2160 for BOARD_BTT_RODENT_V1."
#endif

// SPI chain order: MOSI -> X -> Y -> Z -> E -> MISO (schematic MOT_MOSO1..3)
#if HAS_X_AXIS && X_CHAIN_POS != 1
  #error "X_CHAIN_POS must be 1 for BOARD_BTT_RODENT_V1."
#elif HAS_Y_AXIS && Y_CHAIN_POS != 2
  #error "Y_CHAIN_POS must be 2 for BOARD_BTT_RODENT_V1."
#elif HAS_Z_AXIS && Z_CHAIN_POS != 3
  #error "Z_CHAIN_POS must be 3 for BOARD_BTT_RODENT_V1."
#elif E_STEPPERS && E0_CHAIN_POS != 4
  #error "E0_CHAIN_POS must be 4 for BOARD_BTT_RODENT_V1."
#endif

// 75mΩ sense resistors per the V1.x manual. The schematic's 22mΩ is wrong.
#if HAS_X_AXIS
  static_assert(X_RSENSE == 0.075, "X_RSENSE must be 0.075 for BOARD_BTT_RODENT_V1.");
#endif
#if HAS_Y_AXIS
  static_assert(Y_RSENSE == 0.075, "Y_RSENSE must be 0.075 for BOARD_BTT_RODENT_V1.");
#endif
#if HAS_Z_AXIS
  static_assert(Z_RSENSE == 0.075, "Z_RSENSE must be 0.075 for BOARD_BTT_RODENT_V1.");
#endif
#if E_STEPPERS
  static_assert(E0_RSENSE == 0.075, "E0_RSENSE must be 0.075 for BOARD_BTT_RODENT_V1.");
#endif

//
// SPI - TMC drivers and MicroSD card share VSPI
//
#define SD_SCK_PIN                            18
#define SD_MISO_PIN                           19
#define SD_MOSI_PIN                           23
#define SD_SS_PIN                              0  // Also the BOOT strapping pin
#define USES_SHARED_SPI                           // SPI is shared by SD card with TMC SPI drivers

//
// I2C - OLED header
//
#define I2C_SDA_PIN                           27
#define I2C_SCL_PIN                           26

//
// V-MOS outputs
//
#define COOLANT_FLOOD_PIN                     12  // V-MOS 1 (HBEN)
#define COOLANT_MIST_PIN                       2  // V-MOS 2 (HE1EN)
#define FAN0_PIN                               4  // V-MOS 3 (HE0EN)

//
// M3/M4/M5 - Spindle/Laser Control
//
// SP-PWM is an op-amp output. The VR-10K trimmer sets full scale from 3V to 10V.
// The Sp-Direction and Sp-Feedback headers are direct 3.3V GPIOs, shared with
// RS485 TX (GPIO15) and RS485 DE/RE (GPIO14). RS485 RX is GPIO16.
//
#define SPINDLE_LASER_PWM_PIN                 13  // SP-PWM
#define SPINDLE_LASER_ENA_PIN                 25  // Sp-Enable
#define SPINDLE_DIR_PIN                       15  // Sp-Direction

//
// Status LEDs - active low. Board silkscreen: IISA-Q3, IISB-Q3, IISB-Q6
// i2so.3, i2so.11, i2so.14
//
