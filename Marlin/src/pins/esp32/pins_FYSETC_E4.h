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
 * FYSETC E4 pin assignments
 *
 * 3D printer control board based on the ESP32 microcontroller.
 * Supports 4 stepper drivers, heated bed, single hotend.
 */

#include "env_validate.h"

#if HAS_MULTI_HOTEND || E_STEPPERS > 1
  #error "FYSETC E4 only supports 1 hotend / E stepper."
#endif

#if !AXIS_DRIVER_TYPE_X(TMC2209) || !AXIS_DRIVER_TYPE_Y(TMC2209) || !AXIS_DRIVER_TYPE_Z(TMC2209) || !AXIS_DRIVER_TYPE_E0(TMC2209)
  #error "This board has onboard TMC2209 drivers for X, Y, Z, and E0."
#endif

#define BOARD_INFO_NAME       "FYSETC_E4"

#include "pins_ESPA_common.h"

#if HAS_TMC_UART
  //
  // TMC2208/TMC2209 stepper drivers
  //
  #define X_HARDWARE_SERIAL              Serial1
  #define Y_HARDWARE_SERIAL              Serial1
  #define Z_HARDWARE_SERIAL              Serial1
  #define E0_HARDWARE_SERIAL             Serial1
  #ifndef TMC_BAUD_RATE
    #define TMC_BAUD_RATE                 115200
  #endif

  // Default TMC slave addresses
  #ifndef X_SLAVE_ADDRESS
    #define X_SLAVE_ADDRESS                    1
  #endif
  #ifndef Y_SLAVE_ADDRESS
    #define Y_SLAVE_ADDRESS                    3
  #endif
  #ifndef Z_SLAVE_ADDRESS
    #define Z_SLAVE_ADDRESS                    0
  #endif
  #ifndef E0_SLAVE_ADDRESS
    #define E0_SLAVE_ADDRESS                   2
  #endif
  static_assert(X_SLAVE_ADDRESS == 1, "X_SLAVE_ADDRESS must be 1 for BOARD_FYSETC_E4.");
  static_assert(Y_SLAVE_ADDRESS == 3, "Y_SLAVE_ADDRESS must be 3 for BOARD_FYSETC_E4.");
  static_assert(Z_SLAVE_ADDRESS == 0, "Z_SLAVE_ADDRESS must be 0 for BOARD_FYSETC_E4.");
  static_assert(E0_SLAVE_ADDRESS == 2, "E0_SLAVE_ADDRESS must be 2 for BOARD_FYSETC_E4.");
#endif

/**
 * Hardware serial pins
 *
 * Override these pins in Configuration.h or Configuration_adv.h if needed.
 *
 * Note: Serial2 can be defined using HARDWARE_SERIAL2_RX and HARDWARE_SERIAL2_TX
 *       but MRR ESPA does not have enough spare pins for such reassignment.
 */
#ifndef HARDWARE_SERIAL1_RX
  #define HARDWARE_SERIAL1_RX                 21
#endif
#ifndef HARDWARE_SERIAL1_TX
  #define HARDWARE_SERIAL1_TX                 22
#endif
