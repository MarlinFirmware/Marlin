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

#ifdef __STM32F1__

#include "../../inc/MarlinConfig.h"
#include "MarlinSerial.h"
#include <libmaple/usart.h>

// Copied from ~/.platformio/packages/framework-arduinoststm32-maple/STM32F1/system/libmaple/usart_private.h
// Changed to handle Emergency Parser
FORCE_INLINE void my_usart_irq(ring_buffer *rb, ring_buffer *wb, usart_reg_map *regs, MSerialT &serial) {
 /* Handle RXNEIE and TXEIE interrupts.
  * RXNE signifies availability of a byte in DR.
  *
  * See table 198 (sec 27.4, p809) in STM document RM0008 rev 15.
  * We enable RXNEIE.
  */
  uint32_t srflags = regs->SR, cr1its = regs->CR1;

  if ((cr1its & USART_CR1_RXNEIE) && (srflags & USART_SR_RXNE)) {
    if (srflags & USART_SR_FE || srflags & USART_SR_PE ) {
      // framing error or parity error
      regs->DR; // Read and throw away the data, which also clears FE and PE
    }
    else {
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
        if (serial.emergency_parser_enabled())
          emergency_parser.update(serial.emergency_state, c);
      #endif
    }
  }
  else if (srflags & USART_SR_ORE) {
    // overrun and empty data, just do a dummy read to clear ORE
    // and prevent a raise condition where a continuous interrupt stream (due to ORE set) occurs
    // (see chapter "Overrun error" ) in STM32 reference manual
    regs->DR;
  }

  // TXE signifies readiness to send a byte to DR.
  if ((cr1its & USART_CR1_TXEIE) && (srflags & USART_SR_TXE)) {
    if (!rb_is_empty(wb))
      regs->DR=rb_remove(wb);
    else
      regs->CR1 &= ~((uint32)USART_CR1_TXEIE); // disable TXEIE
  }
}

// Not every MarlinSerial port should handle emergency parsing.
// It would not make sense to parse G-Code from TMC responses, for example.
constexpr bool serial_handles_emergency(int port) {
  return (false
    #ifdef SERIAL_PORT
      || (SERIAL_PORT) == port
    #endif
    #ifdef SERIAL_PORT_2
      || (SERIAL_PORT_2) == port
    #endif
    #ifdef LCD_SERIAL_PORT
      || (LCD_SERIAL_PORT) == port
    #endif
  );
}

#define DEFINE_HWSERIAL_MARLIN(name, n)     \
  MSerialT name(serial_handles_emergency(n),\
            USART##n,                       \
            BOARD_USART##n##_TX_PIN,        \
            BOARD_USART##n##_RX_PIN);       \
  extern "C" void __irq_usart##n(void) {    \
    my_usart_irq(USART##n->rb, USART##n->wb, USART##n##_BASE, MSerial##n); \
  }

#define DEFINE_HWSERIAL_UART_MARLIN(name, n) \
  MSerialT name(serial_handles_emergency(n), \
          UART##n,                           \
          BOARD_USART##n##_TX_PIN,           \
          BOARD_USART##n##_RX_PIN);          \
  extern "C" void __irq_usart##n(void) {     \
    my_usart_irq(UART##n->rb, UART##n->wb, UART##n##_BASE, MSerial##n); \
  }

// Instantiate all UARTs even if they are not needed
// This avoids a bunch of logic to figure out every serial
// port which may be in use on the system.
#if DISABLED(MKS_WIFI_MODULE)
  DEFINE_HWSERIAL_MARLIN(MSerial1, 1);
#endif
DEFINE_HWSERIAL_MARLIN(MSerial2, 2);
DEFINE_HWSERIAL_MARLIN(MSerial3, 3);
#if ANY(STM32_HIGH_DENSITY, STM32_XL_DENSITY)
  DEFINE_HWSERIAL_UART_MARLIN(MSerial4, 4);
  DEFINE_HWSERIAL_UART_MARLIN(MSerial5, 5);
#endif

// Check the type of each serial port by passing it to a template function.
// HardwareSerial is known to sometimes hang the controller when an error occurs,
// so this case will fail the static assert. All other classes are assumed to be ok.
template <typename T>
constexpr bool IsSerialClassAllowed(const T&) { return true; }
constexpr bool IsSerialClassAllowed(const HardwareSerial&) { return false; }

#define CHECK_CFG_SERIAL(A) static_assert(IsSerialClassAllowed(A), STRINGIFY(A) " is defined incorrectly");
#define CHECK_AXIS_SERIAL(A) static_assert(IsSerialClassAllowed(A##_HARDWARE_SERIAL), STRINGIFY(A) "_HARDWARE_SERIAL must be defined in the form MSerial1, rather than Serial1");

// If you encounter this error, replace SerialX with MSerialX, for example MSerial3.

// Non-TMC ports were already validated in HAL.h, so do not require verbose error messages.
#ifdef MYSERIAL1
  CHECK_CFG_SERIAL(MYSERIAL1);
#endif
#ifdef MYSERIAL2
  CHECK_CFG_SERIAL(MYSERIAL2);
#endif
#ifdef LCD_SERIAL
  CHECK_CFG_SERIAL(LCD_SERIAL);
#endif
#if AXIS_HAS_HW_SERIAL(X)
  CHECK_AXIS_SERIAL(X);
#endif
#if AXIS_HAS_HW_SERIAL(X2)
  CHECK_AXIS_SERIAL(X2);
#endif
#if AXIS_HAS_HW_SERIAL(Y)
  CHECK_AXIS_SERIAL(Y);
#endif
#if AXIS_HAS_HW_SERIAL(Y2)
  CHECK_AXIS_SERIAL(Y2);
#endif
#if AXIS_HAS_HW_SERIAL(Z)
  CHECK_AXIS_SERIAL(Z);
#endif
#if AXIS_HAS_HW_SERIAL(Z2)
  CHECK_AXIS_SERIAL(Z2);
#endif
#if AXIS_HAS_HW_SERIAL(Z3)
  CHECK_AXIS_SERIAL(Z3);
#endif
#if AXIS_HAS_HW_SERIAL(Z4)
  CHECK_AXIS_SERIAL(Z4);
#endif
#if AXIS_HAS_HW_SERIAL(I)
  CHECK_AXIS_SERIAL(I);
#endif
#if AXIS_HAS_HW_SERIAL(J)
  CHECK_AXIS_SERIAL(J);
#endif
#if AXIS_HAS_HW_SERIAL(K)
  CHECK_AXIS_SERIAL(K);
#endif
#if AXIS_HAS_HW_SERIAL(E0)
  CHECK_AXIS_SERIAL(E0);
#endif
#if AXIS_HAS_HW_SERIAL(E1)
  CHECK_AXIS_SERIAL(E1);
#endif
#if AXIS_HAS_HW_SERIAL(E2)
  CHECK_AXIS_SERIAL(E2);
#endif
#if AXIS_HAS_HW_SERIAL(E3)
  CHECK_AXIS_SERIAL(E3);
#endif
#if AXIS_HAS_HW_SERIAL(E4)
  CHECK_AXIS_SERIAL(E4);
#endif
#if AXIS_HAS_HW_SERIAL(E5)
  CHECK_AXIS_SERIAL(E5);
#endif
#if AXIS_HAS_HW_SERIAL(E6)
  CHECK_AXIS_SERIAL(E6);
#endif
#if AXIS_HAS_HW_SERIAL(E7)
  CHECK_AXIS_SERIAL(E7);
#endif

#endif // __STM32F1__
