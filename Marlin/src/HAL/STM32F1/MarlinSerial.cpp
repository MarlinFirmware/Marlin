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

#include "../../inc/MarlinConfigPre.h"
#include "MarlinSerial.h"
#include <libmaple/usart.h>

// Copied from ~/.platformio/packages/framework-arduinoststm32-maple/STM32F1/system/libmaple/usart_private.h
// Changed to handle Emergency Parser
static inline __always_inline void my_usart_irq(ring_buffer *rb, ring_buffer *wb, usart_reg_map *regs, MarlinSerial &serial) {
   /* Handle RXNEIE and TXEIE interrupts.
    * RXNE signifies availability of a byte in DR.
    *
    * See table 198 (sec 27.4, p809) in STM document RM0008 rev 15.
    * We enable RXNEIE.
    */
  if ((regs->CR1 & USART_CR1_RXNEIE) && (regs->SR & USART_SR_RXNE)) {
    uint8_t c = (uint8)regs->DR;
    #ifdef USART_SAFE_INSERT
      // If the buffer is full and the user defines USART_SAFE_INSERT,
      // ignore new bytes.
      rb_safe_insert(rb, c);
    #else
      // By default, push bytes around in the ring buffer.
      rb_push_insert(rb, c);
    #endif
    #if ENABLED(EMERGENCY_PARSER)
      emergency_parser.update(serial.emergency_state, c);
    #endif
  }
  // TXE signifies readiness to send a byte to DR.
  if ((regs->CR1 & USART_CR1_TXEIE) && (regs->SR & USART_SR_TXE)) {
    if (!rb_is_empty(wb))
      regs->DR=rb_remove(wb);
    else
      regs->CR1 &= ~((uint32)USART_CR1_TXEIE); // disable TXEIE
  }
}

#define DEFINE_HWSERIAL_MARLIN(name, n)  \
  MarlinSerial name(USART##n,            \
            BOARD_USART##n##_TX_PIN,     \
            BOARD_USART##n##_RX_PIN);    \
  extern "C" void __irq_usart##n(void) { \
    my_usart_irq(USART##n->rb, USART##n->wb, USART##n##_BASE, MSerial##n); \
  }

#define DEFINE_HWSERIAL_UART_MARLIN(name, n) \
  MarlinSerial name(UART##n,                 \
            BOARD_USART##n##_TX_PIN,         \
            BOARD_USART##n##_RX_PIN);        \
  extern "C" void __irq_usart##n(void) {     \
    my_usart_irq(USART##n->rb, USART##n->wb, USART##n##_BASE, MSerial##n); \
  }

#if SERIAL_PORT == 1 || SERIAL_PORT_2 == 1 || DGUS_SERIAL_PORT == 1
  DEFINE_HWSERIAL_MARLIN(MSerial1, 1);
#endif

#if SERIAL_PORT == 2 || SERIAL_PORT_2 == 2 || DGUS_SERIAL_PORT == 2
  DEFINE_HWSERIAL_MARLIN(MSerial2, 2);
#endif

#if SERIAL_PORT == 3 || SERIAL_PORT_2 == 3 || DGUS_SERIAL_PORT == 3
  DEFINE_HWSERIAL_MARLIN(MSerial3, 3);
#endif

#if SERIAL_PORT == 4 || SERIAL_PORT_2 == 4 || DGUS_SERIAL_PORT == 4
  DEFINE_HWSERIAL_UART_MARLIN(MSerial4, 4);
#endif

#if SERIAL_PORT == 5 || SERIAL_PORT_2 == 5 || DGUS_SERIAL_PORT == 5
  DEFINE_HWSERIAL_UART_MARLIN(MSerial5, 5);
#endif
