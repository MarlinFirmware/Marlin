/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include <HardwareSerial.h>
#include <WString.h>

#include "../../inc/MarlinConfigPre.h"
#if ENABLED(EMERGENCY_PARSER)
  #include "../../feature/emergency_parser.h"
#endif

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
  MarlinSerial(LPC_UART_TypeDef *UARTx) :
    HardwareSerial<RX_BUFFER_SIZE, TX_BUFFER_SIZE>(UARTx)
    #if ENABLED(EMERGENCY_PARSER)
       , emergency_state(EmergencyParser::State::EP_RESET)
    #endif
    {
    }

  void end() { }

  #if ENABLED(EMERGENCY_PARSER)
    bool recv_callback(const char c) override {
      emergency_parser.update(emergency_state, c);
      return true; // do not discard character
    }

    EmergencyParser::State emergency_state;
  #endif
};

extern MarlinSerial MSerial;
extern MarlinSerial MSerial1;
extern MarlinSerial MSerial2;
extern MarlinSerial MSerial3;
