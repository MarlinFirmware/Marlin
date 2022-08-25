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
 * MKS Robin nano (STM32F103VET6) board pin assignments
 * https://github.com/makerbase-mks/MKS-Robin-Nano-V1.X/tree/master/hardware
 */

#define ALLOW_STM32DUINO
#include "env_validate.h"

#define BOARD_INFO_NAME "MKS Robin Nano"

//
// Release PB4 (Y_ENABLE_PIN) from JTAG NRST role
//
#define DISABLE_JTAG

//
// Thermocouples
//
//#define TEMP_0_CS_PIN                     PE5   // TC1 - CS1
//#define TEMP_0_CS_PIN                     PE6   // TC2 - CS2

//#define LED_PIN                           PB2

#include "pins_MKS_ROBIN_NANO_common.h"

#if HAS_TFT_LVGL_UI && FAN1_PIN != PB0 && HEATER_1_PIN != PB0
  #define BOARD_INIT() OUT_WRITE(PB0, LOW)
#endif

#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                    PA4   // MT_DET
#endif

#if HAS_TMC220x
  /**
   * TMC2208/TMC2209 stepper drivers
   */
  //
  // Software serial
  //
  #define TMC_BAUD_RATE 19200

  #if HAS_TMC220x
  /**
   * TMC2208/TMC2209 stepper drivers
   *
   * Hardware serial communication ports.
   * If undefined software serial is used according to the pins below
   */
  //#define X_HARDWARE_SERIAL  Serial1
  //#define Y_HARDWARE_SERIAL  Serial1
  //#define Z_HARDWARE_SERIAL  Serial1
  //#define E0_HARDWARE_SERIAL Serial1
  //#define E1_HARDWARE_SERIAL Serial1

  //
  // Software serial
  //

  // #ifndef X_SERIAL_TX_PIN
  //   #define X_SERIAL_TX_PIN  PA6 //E1_STEP_PIN
  // #endif
  // #ifndef X_SERIAL_RX_PIN
  //   #define X_SERIAL_RX_PIN  PA1 //E1_DIR_PIN
  // #endif

  // #ifndef Y_SERIAL_TX_PIN
  //   #define Y_SERIAL_TX_PIN  PA6 //E1_STEP_PIN
  // #endif
  // #ifndef Y_SERIAL_RX_PIN
  //   #define Y_SERIAL_RX_PIN  PA1 //E1_DIR_PIN
  // #endif

  // #ifndef Z_SERIAL_TX_PIN
  //   #define Z_SERIAL_TX_PIN  PA6 //E1_STEP_PIN
  // #endif
  // #ifndef Z_SERIAL_RX_PIN
  //   #define Z_SERIAL_RX_PIN  PA1 //E1_DIR_PIN
  // #endif

  #ifndef E0_SERIAL_TX_PIN
    #define E0_SERIAL_TX_PIN PA6 //E1_STEP_PIN
  #endif
  #ifndef E0_SERIAL_RX_PIN
    #define E0_SERIAL_RX_PIN PA1 //E1_DIR_PIN
  #endif
#endif

#endif