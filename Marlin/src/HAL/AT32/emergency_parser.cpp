/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * AT32 Emergency Parser Integration
 *
 * This file provides the AT32-specific implementation of the emergency parser
 * interrupt hook that feeds serial characters to the emergency parser.
 */

#ifdef ARDUINO_ARCH_AT32

#include "../../MarlinCore.h"
#include "../../core/serial.h"
#include "core_hooks.h"  // For core_hook_usart_rx_irq weak alias

// Emergency parser integration for AT32
// The AT32 core's USART IRQ handler calls core_hook_usart_rx_irq()
// which is aliased to AT32_emergency_parser_irq() in core_hooks.h

void AT32_emergency_parser_irq(uint8_t ch, uint8_t usart) {
  // Only handle receive on host serial ports
  if (usart != SERIAL_PORT
    #ifdef SERIAL_PORT_2
      && usart != SERIAL_PORT_2
    #endif
    #ifdef SERIAL_PORT_3
      && usart != SERIAL_PORT_3
    #endif
  ) {
    return;
  }

  // Submit character to emergency parser
  if (MYSERIAL1.emergency_parser_enabled())
    emergency_parser.update(MYSERIAL1.emergency_state, ch);
}

#endif // ARDUINO_ARCH_AT32
