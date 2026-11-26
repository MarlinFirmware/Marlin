/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * AT32 Core Hooks - Emergency Parser Integration
 *
 * This header provides the weak alias for the AT32 emergency parser hook.
 * The actual implementation is in emergency_parser.cpp to avoid include issues.
 */

#ifndef CORE_HOOKS_H
#define CORE_HOOKS_H

#ifdef ARDUINO_ARCH_AT32

// Include serial definitions for MYSERIAL1
#include "../../core/serial.h"

// Declare the actual implementation (defined in emergency_parser.cpp)
extern "C" void AT32_emergency_parser_irq(uint8_t ch, uint8_t usart);

// Use the implementation directly
#define core_hook_usart_rx_irq AT32_emergency_parser_irq

#endif // ARDUINO_ARCH_AT32

#endif // CORE_HOOKS_H
