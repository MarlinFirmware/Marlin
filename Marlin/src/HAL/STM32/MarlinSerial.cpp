/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#if defined(ARDUINO_ARCH_STM32) && !defined(STM32GENERIC)

#include "../../inc/MarlinConfig.h"
#include "MarlinSerial.h"

#if ENABLED(EMERGENCY_PARSER)
  #include "../../feature/e_parser.h"
#endif

#ifndef USART4
  #define USART4 UART4
#endif

#ifndef USART5
  #define USART5 UART5
#endif

#define DECLARE_SERIAL_PORT(ser_num) \
  void _rx_complete_irq_ ## ser_num (serial_t * obj); \
  MarlinSerial MSerial ## ser_num (USART ## ser_num, &_rx_complete_irq_ ## ser_num); \
  void _rx_complete_irq_ ## ser_num (serial_t * obj) { MSerial ## ser_num ._rx_complete_irq(obj); }

#define DECLARE_SERIAL_PORT_EXP(ser_num) DECLARE_SERIAL_PORT(ser_num)

#if defined(SERIAL_PORT) && SERIAL_PORT >= 0
  DECLARE_SERIAL_PORT_EXP(SERIAL_PORT)
#endif

#if defined(SERIAL_PORT_2) && SERIAL_PORT_2 >= 0
  DECLARE_SERIAL_PORT_EXP(SERIAL_PORT_2)
#endif

#if defined(DGUS_SERIAL_PORT) && DGUS_SERIAL_PORT >= 0
  DECLARE_SERIAL_PORT_EXP(DGUS_SERIAL_PORT)
#endif

void MarlinSerial::begin(unsigned long baud, uint8_t config) {
  HardwareSerial::begin(baud, config);
  // Replace the IRQ callback with the one we have defined
  TERN_(EMERGENCY_PARSER, _serial.rx_callback = _rx_callback);
}

// This function is Copyright (c) 2006 Nicholas Zambetti.
void MarlinSerial::_rx_complete_irq(serial_t *obj) {
  // No Parity error, read byte and store it in the buffer if there is room
  unsigned char c;

  if (uart_getc(obj, &c) == 0) {

    rx_buffer_index_t i = (unsigned int)(obj->rx_head + 1) % SERIAL_RX_BUFFER_SIZE;

    // if we should be storing the received character into the location
    // just before the tail (meaning that the head would advance to the
    // current location of the tail), we're about to overflow the buffer
    // and so we don't write the character or advance the head.
    if (i != obj->rx_tail) {
      obj->rx_buff[obj->rx_head] = c;
      obj->rx_head = i;
    }

    #if ENABLED(EMERGENCY_PARSER)
      emergency_parser.update(emergency_state, c);
    #endif
  }
}

#endif // ARDUINO_ARCH_STM32 && !STM32GENERIC
