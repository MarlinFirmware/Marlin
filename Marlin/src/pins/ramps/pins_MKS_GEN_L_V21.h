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
 * MKS GEN L V2 – Arduino Mega2560 with RAMPS v1.4 pin assignments
 * Schematic: https://green-candy.osdn.jp/external/MarlinFW/board_schematics/RAMPS/MKS%20GEN%20L%20V2.1/MKS%20GEN_L%20V2.1_001%20SCH.pdf
 * Origin: https://github.com/makerbase-mks/MKS-GEN_L/blob/master/hardware/MKS%20Gen_L%20V2.1_001/MKS%20GEN_L%20V2.1_001%20SCH.pdf
 * ATmega2560
 */

#if HOTENDS > 2 || E_STEPPERS > 2
  #error "MKS GEN L V2.1 supports up to 2 hotends / E steppers."
#endif

#define BOARD_INFO_NAME "MKS GEN L V2.1"

//
// Heaters / Fans
//
// Power outputs EFBF or EFBE
#define MOSFET_D_PIN                           7

//
// CS Pins wired to avoid conflict with the LCD
// See https://www.thingiverse.com/asset:66604
//

#ifndef X_CS_PIN
  #define X_CS_PIN                            63
#endif
#ifndef Y_CS_PIN
  #define Y_CS_PIN                            64
#endif
#ifndef Z_CS_PIN
  #define Z_CS_PIN                            65
#endif
#ifndef E0_CS_PIN
  #define E0_CS_PIN                           66
#endif
#ifndef E1_CS_PIN
  #define E1_CS_PIN                           12
#endif

// TMC2130 Diag Pins (currently just for reference)
#define X_DIAG_PIN                             3
#define Y_DIAG_PIN                            14
#define Z_DIAG_PIN                            18
#define E0_DIAG_PIN                            2
#define E1_DIAG_PIN                           15

#ifndef SERVO1_PIN
  #define SERVO1_PIN                          21
#endif
#ifndef SERVO2_PIN
  #define SERVO2_PIN                          39
#endif
#ifndef SERVO3_PIN
  #define SERVO3_PIN                          32
#endif

#ifndef E1_SERIAL_TX_PIN
  #define E1_SERIAL_TX_PIN                    20
#endif
#ifndef E1_SERIAL_RX_PIN
  #define E1_SERIAL_RX_PIN                    12
#endif

#include "pins_RAMPS.h"
