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

#include <HardwareSerial.h>
#include <WString.h>

#include "../../inc/MarlinConfigPre.h"
#if ENABLED(EMERGENCY_PARSER)
  #include "../../feature/e_parser.h"
#endif
#include "../../core/serial_hook.h"

#ifndef SERIAL_PORT
  #define SERIAL_PORT 0
#endif
#ifndef RX_BUFFER_SIZE
  #define RX_BUFFER_SIZE 128
#endif
#ifndef TX_BUFFER_SIZE
  #define TX_BUFFER_SIZE 32
#endif

class MarlinSerial : public HardwareSerial<RX_BUFFER_SIZE, TX_BUFFER_SIZE> {
public:
  MarlinSerial(LPC_UART_TypeDef *UARTx) : HardwareSerial<RX_BUFFER_SIZE, TX_BUFFER_SIZE>(UARTx) { }

  void end() {}

  uint8_t availableForWrite(void) { /* flushTX(); */ return TX_BUFFER_SIZE; }

  #if ENABLED(EMERGENCY_PARSER)
    bool recv_callback(const char c) override;
  #endif
};

// On LPC176x framework, HardwareSerial does not implement the same interface as Arduino's Serial, so overloads
// of 'available' and 'read' method are not used in this multiple inheritance scenario.
// Instead, use a ForwardSerial here that adapts the interface.
typedef ForwardSerial1Class<MarlinSerial> MSerialT;
extern MSerialT MSerial0;
extern MSerialT MSerial1;
extern MSerialT MSerial2;
extern MSerialT MSerial3;

// Consequently, we can't use a RuntimeSerial either. The workaround would be to use
// a RuntimeSerial<ForwardSerial<MarlinSerial>> type here. Ignore for now until it's actually required.
#if ENABLED(SERIAL_RUNTIME_HOOK)
  #error "SERIAL_RUNTIME_HOOK is not yet supported for LPC176x."
#endif
