/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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

#ifdef TARGET_LPC1768

#include "../../inc/MarlinConfigPre.h"
#include "MarlinSerial.h"

#if (defined(SERIAL_PORT) && SERIAL_PORT == 0) || (defined(SERIAL_PORT_2) && SERIAL_PORT_2 == 0)
  MarlinSerial MSerial(LPC_UART0);
  extern "C" void UART0_IRQHandler(void) {
    MSerial.IRQHandler();
  }
#endif

#if (defined(SERIAL_PORT) && SERIAL_PORT == 1) || (defined(SERIAL_PORT_2) && SERIAL_PORT_2 == 1)
  MarlinSerial MSerial1((LPC_UART_TypeDef *) LPC_UART1);
  extern "C" void UART1_IRQHandler(void) {
    MSerial1.IRQHandler();
  }
#endif

#if (defined(SERIAL_PORT) && SERIAL_PORT == 2) || (defined(SERIAL_PORT_2) && SERIAL_PORT_2 == 2)
  MarlinSerial MSerial2(LPC_UART2);
  extern "C" void UART2_IRQHandler(void) {
    MSerial2.IRQHandler();
  }
#endif

#if (defined(SERIAL_PORT) && SERIAL_PORT == 3) || (defined(SERIAL_PORT_2) && SERIAL_PORT_2 == 3)
  MarlinSerial MSerial3(LPC_UART3);
  extern "C" void UART3_IRQHandler(void) {
    MSerial3.IRQHandler();
  }
#endif

#endif // TARGET_LPC1768
