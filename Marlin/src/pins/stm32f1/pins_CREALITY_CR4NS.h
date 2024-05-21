/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2024 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * Creality CREALITY_CR4NS (GD32F303RET6) board pin assignments
 * Sold as "Creality Ender-3 V3 SE CR4NS200320C13 Motherboard"
 * Preliminary support for the Professional Firmwware
 */

#include "env_validate.h"

#if HOTENDS > 1 || E_STEPPERS > 1
  #error "CR4NS200320C13 only supports one hotend / E-stepper."
#endif

// Validate stepper driver selections.
//#if !AXIS_DRIVER_TYPE_X(TMC2208) || !AXIS_DRIVER_TYPE_Y(TMC2208) || !AXIS_DRIVER_TYPE_Z(TMC2208) || !AXIS_DRIVER_TYPE_E0(TMC2208)
//  #error "This board has onboard TMC2208 drivers for X, Y, Z, and E0."
//#endif

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME      "CR4NS200320C13"
#endif
#ifndef DEFAULT_MACHINE_NAME
  #define DEFAULT_MACHINE_NAME "Ender-3 V3 SE"
#endif
#define BOARD_WEBSITE_URL      "www.creality.com"

//
// Servos
//
#ifndef SERVO0_PIN
  #define SERVO0_PIN                        PC14
#endif

#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                   PC13  // BLTouch IN
#endif

//
// Limit Switches
//
//#ifndef Z_STOP_PIN
//  #define Z_STOP_PIN                      PA15  // else PA7
//#endif

//
// Filament Runout Sensor
//
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                    PC15  // "Pulled-high"
#endif

//
// Heaters / Fans
//
#define HEATER_BED_PIN                      PB2   // HOT BED
#define FAN1_PIN                            PC1   // extruder fan
//#define FAN2_PIN                          PB1   // Controller fan FET

//
// Auto fans
//
//#ifndef CONTROLLER_FAN_PIN
//  #define CONTROLLER_FAN_PIN          FAN2_PIN
//#endif

#if HAS_TMC_UART
  // Reduce baud rate to improve software serial reliability
  #define TMC_BAUD_RATE 19200

  // Software serial
  #define X_SERIAL_TX_PIN                   PB12
  #define X_SERIAL_RX_PIN        X_SERIAL_TX_PIN

  #define Y_SERIAL_TX_PIN                   PB13
  #define Y_SERIAL_RX_PIN        Y_SERIAL_TX_PIN

  #define Z_SERIAL_TX_PIN                   PB14
  #define Z_SERIAL_RX_PIN        Z_SERIAL_TX_PIN

#endif // HAS_TMC_UART

#if ANY(RET6_12864_LCD, HAS_DWIN_E3V2, IS_DWIN_MARLINUI)

  /**
   *    LCD PIN OUT
   *        ------
   *    NC | 1  2 | NC
   *    RX | 3  4 | TX
   *    EN   5  6 | BEEP
   *     B | 7  8 | A
   *   GND | 9 10 | +5V
   *        ------
   */
  #define EXP3_01_PIN                       -1
  #define EXP3_02_PIN                       -1
  #define EXP3_03_PIN                       PA2
  #define EXP3_04_PIN                       PA3
  #define EXP3_05_PIN                       PB1
  #define EXP3_06_PIN                       -1
  #define EXP3_07_PIN                       PA12
  #define EXP3_08_PIN                       PA11

  #ifndef BEEPER_PIN
    #define BEEPER_PIN               EXP1_06_PIN  // BEEP
  #endif

  #define BTN_ENC                    EXP1_05_PIN  // EN
  #define BTN_EN1                    EXP1_08_PIN  // A
  #define BTN_EN2                    EXP1_07_PIN  // B

#endif

#include "pins_CREALITY_V4.h"
