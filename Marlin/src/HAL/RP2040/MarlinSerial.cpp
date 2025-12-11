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

// Marlin uses: -1=USB, 0=UART0, 1=UART1
// Arduino uses: Serial=USB, Serial1=UART0, Serial2=UART1
//
// To remap Arduino's numbering to Marlin's convention, we create Serial0/Serial1
// as new UART instances with custom pins. This conflicts with the Arduino
// framework's Serial1/Serial2 objects which use the same hardware, causing
// multiple definitions of _UART0_ and _UART1_ symbols.
//
// Resolution: The build uses -Wl,--allow-multiple-definition ONLY for linking
// (not compilation) via a PlatformIO script, allowing Marlin's Serial0/Serial1
// to override the framework's Serial1/Serial2.

// Create Serial0 as UART0 with custom or default pins
arduino::UART Serial0(
#if defined(SERIAL0_TX_PIN) && defined(SERIAL0_RX_PIN)
  SERIAL0_TX_PIN, SERIAL0_RX_PIN  // Custom pins for UART0 (Marlin Serial0)
#else
  0, 1  // Default UART0 pins (GP0/GP1)
#endif
);

// Create Serial1 as UART1 with custom or default pins
#if defined(SERIAL1_TX_PIN) && defined(SERIAL1_RX_PIN)
  arduino::UART Serial1(SERIAL1_TX_PIN, SERIAL1_RX_PIN);  // Custom pins for UART1 (Marlin Serial1)
#endif

// Wrap the serial ports for Marlin
DefaultSerial0 MSerial0(false, Serial0);  // Marlin Serial0 = UART0
#if defined(SERIAL1_TX_PIN) && defined(SERIAL1_RX_PIN)
  DefaultSerial1 MSerial1(false, Serial1);  // Marlin Serial1 = UART1
#endif
DefaultSerial2 MSerial2(false, Serial);   // Marlin Serial-1 = USB

#endif // __PLAT_RP2040__
