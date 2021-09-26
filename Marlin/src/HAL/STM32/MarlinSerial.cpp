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
#include "../platforms.h"

#ifdef HAL_STM32

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
  MSerialT MSerial ## ser_num (true, USART ## ser_num, &_rx_complete_irq_ ## ser_num); \
  void _rx_complete_irq_ ## ser_num (serial_t * obj) { MSerial ## ser_num ._rx_complete_irq(obj); }

#if USING_HW_SERIAL1
  DECLARE_SERIAL_PORT(1)
#endif
#if USING_HW_SERIAL2
  DECLARE_SERIAL_PORT(2)
#endif
#if USING_HW_SERIAL3
  DECLARE_SERIAL_PORT(3)
#endif
#if USING_HW_SERIAL4
  DECLARE_SERIAL_PORT(4)
#endif
#if USING_HW_SERIAL5
  DECLARE_SERIAL_PORT(5)
#endif
#if USING_HW_SERIAL6
  DECLARE_SERIAL_PORT(6)
#endif
#if USING_HW_SERIAL7
  DECLARE_SERIAL_PORT(7)
#endif
#if USING_HW_SERIAL8
  DECLARE_SERIAL_PORT(8)
#endif
#if USING_HW_SERIAL9
  DECLARE_SERIAL_PORT(9)
#endif
#if USING_HW_SERIAL10
  DECLARE_SERIAL_PORT(10)
#endif
#if USING_HW_SERIALLP1
  DECLARE_SERIAL_PORT(LP1)
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
      emergency_parser.update(static_cast<MSerialT*>(this)->emergency_state, c);
    #endif
  }
}

#endif // HAL_STM32
