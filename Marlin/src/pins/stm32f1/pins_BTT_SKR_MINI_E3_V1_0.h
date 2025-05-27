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

#include "pins_BTT_SKR_MINI_E3_common.h"

#define BOARD_INFO_NAME "BTT SKR Mini E3 V1.0"
#define BOARD_WEBSITE_URL "github.com/bigtreetech/BIGTREETECH-SKR-mini-E3/tree/master/hardware/BIGTREETECH-SKR-mini-E3%20V1.0"

/**
 * TMC220x stepper drivers
 * Hardware serial communication ports.
 */
#if HAS_TMC_UART
  #define X_HARDWARE_SERIAL  MSerial4
  #define Y_HARDWARE_SERIAL  MSerial4
  #define Z_HARDWARE_SERIAL  MSerial4
  #define E0_HARDWARE_SERIAL MSerial4

  // Default TMC slave addresses
  #ifndef X_SLAVE_ADDRESS
    #define X_SLAVE_ADDRESS                    0
  #endif
  #ifndef Y_SLAVE_ADDRESS
    #define Y_SLAVE_ADDRESS                    2
  #endif
  #ifndef Z_SLAVE_ADDRESS
    #define Z_SLAVE_ADDRESS                    1
  #endif
  #ifndef E0_SLAVE_ADDRESS
    #define E0_SLAVE_ADDRESS                   3
  #endif
  static_assert(X_SLAVE_ADDRESS == 0, "X_SLAVE_ADDRESS must be 0 for BOARD_BTT_SKR_MINI_E3_V1_0.");
  static_assert(Y_SLAVE_ADDRESS == 2, "Y_SLAVE_ADDRESS must be 2 for BOARD_BTT_SKR_MINI_E3_V1_0.");
  static_assert(Z_SLAVE_ADDRESS == 1, "Z_SLAVE_ADDRESS must be 1 for BOARD_BTT_SKR_MINI_E3_V1_0.");
  static_assert(E0_SLAVE_ADDRESS == 3, "E0_SLAVE_ADDRESS must be 3 for BOARD_BTT_SKR_MINI_E3_V1_0.");
#endif
