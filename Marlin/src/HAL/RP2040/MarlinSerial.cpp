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
#include "../platforms.h"

#ifdef __PLAT_RP2040__

#include "../../inc/MarlinConfig.h"
#include "MarlinSerial.h"

#if ENABLED(EMERGENCY_PARSER)
  #include "../../feature/e_parser.h"
#endif

#include <HardwareSerial.h>

// Special implementations for RP2040 mapping
arduino::UART Serial0(
#if defined(SERIAL0_TX_PIN) && defined(SERIAL0_RX_PIN)
  SERIAL0_TX_PIN, SERIAL0_RX_PIN
#else
  0, 1
#endif
);  // UART0 on pins 0/1 (or remapped)
#if defined(SERIAL1_TX_PIN) && defined(SERIAL1_RX_PIN)
  #undef Serial1 // remove conflict with Arduino core
  arduino::UART Serial1(SERIAL1_TX_PIN, SERIAL1_RX_PIN);
#endif
DefaultSerial0 MSerial0(false, Serial0);
DefaultSerial2 MSerial2(false, Serial);

#endif // __PLAT_RP2040__
