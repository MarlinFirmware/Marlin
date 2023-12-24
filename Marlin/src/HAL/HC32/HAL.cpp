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

#ifdef ARDUINO_ARCH_HC32

#include "HAL.h"
#include <core_hooks.h>
#include <drivers/panic/panic.h>

//
// Emergency Parser
//
#if ENABLED(EMERGENCY_PARSER)

extern "C" void core_hook_usart_rx_irq(uint8_t ch, uint8_t usart) {
  // Only handle receive on host serial ports
  if (false
    #ifdef SERIAL_PORT
      || usart != SERIAL_PORT
    #endif
    #ifdef SERIAL_PORT_2
      || usart != SERIAL_PORT_2
    #endif
    #ifdef SERIAL_PORT_3
      || usart != SERIAL_PORT_3
    #endif
  ) {
    return;
  }

  // Submit character to emergency parser
  if (MYSERIAL1.emergency_parser_enabled())
    emergency_parser.update(MYSERIAL1.emergency_state, ch);
}

#endif // EMERGENCY_PARSER
#endif // ARDUINO_ARCH_HC32
