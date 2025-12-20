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

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(EMERGENCY_PARSER)
  #include "../../feature/e_parser.h"
#endif

#include "../../core/serial_hook.h"

/**
 * Serial Port Configuration for RP2040 (Raspberry Pi Pico)
 *
 * Arduino-Pico Core Serial Objects:
 * - Serial: USB Serial (CDC ACM)
 * - Serial1: Hardware UART0
 * - Serial2: Hardware UART1
 * - SerialUSB: Alias for Serial (USB)
 *
 * Marlin Serial Wrappers:
 * - MSerial0: Wrapper for MarlinSerial0 (UART0), used as Serial0
 * - MSerial1: Wrapper for MarlinSerial1 (UART1), declared dynamically if used
 * - MSerial2: Wrapper for Serial (USB)
 * - USBSerial: Wrapper for SerialUSB (USB)
 *
 * How it all joins together:
 * - Configuration defines SERIAL_PORT, SERIAL_PORT_2, etc. (-1 to 1 range)
 * - shared/serial_ports.h maps these to MYSERIAL1, MYSERIAL2, etc.
 * - MYSERIAL1 uses MSerialX based on the port index
 * - USB ports (-1) use USB_SERIAL_PORT (MSerial2)
 */

// Forward declare our custom Serial objects (defined in MarlinSerial.cpp)
namespace arduino { class UART; }
extern arduino::UART MarlinSerial0;  // Always declared
extern arduino::UART MarlinSerial1;  // Custom Marlin Serial1 to avoid conflict

typedef ForwardSerial1Class< decltype(MarlinSerial0) > DefaultSerial0;
extern DefaultSerial0 MSerial0;
typedef ForwardSerial1Class< decltype(MarlinSerial1) > DefaultSerial1;
extern DefaultSerial1 MSerial1;
typedef ForwardSerial1Class< decltype(Serial) > DefaultSerial2;
extern DefaultSerial2 MSerial2;
typedef ForwardSerial1Class<decltype(SerialUSB)> USBSerialType;
extern USBSerialType USBSerial;

#define _DECLARE_SERIAL(X) \
  typedef ForwardSerial1Class<decltype(MarlinSerial##X)> DefaultSerial##X; \
  extern DefaultSerial##X MSerial##X
#define DECLARE_SERIAL(X) _DECLARE_SERIAL(X)

#define SERIAL_INDEX_MIN 0
#define SERIAL_INDEX_MAX 1
#define USB_SERIAL_PORT(...) MSerial2
#include "../shared/serial_ports.h"

#if defined(LCD_SERIAL_PORT) && ANY(HAS_DGUS_LCD, EXTENSIBLE_UI)
  #define SERIAL_GET_TX_BUFFER_FREE() LCD_SERIAL.availableForWrite()
#endif
