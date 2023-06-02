/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "LPC55S28.h"


class MarlinSerial : public HardwareSerial<RX_BUFFER_SIZE, TX_BUFFER_SIZE> {
public:

  MarlinSerial(USART_Type *UARTx) : HardwareSerial<RX_BUFFER_SIZE, TX_BUFFER_SIZE>(UARTx) { }

  void begin(uint32_t baud);

  void end() {}

  uint8_t availableForWrite() { /* flushTX(); */ return TX_BUFFER_SIZE; }

  #if ENABLED(EMERGENCY_PARSER)
    bool recv_callback(const char c) override;
  #endif
};

typedef ForwardSerial1Class<MarlinSerial> MSerialT;
extern MSerialT MSerial1;
extern MSerialT MSerial2;
extern MSerialT MSerial3;
extern MSerialT MSerial4;
extern MSerialT MSerial5;
