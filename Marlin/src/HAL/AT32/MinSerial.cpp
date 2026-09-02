/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#ifdef ARDUINO_ARCH_AT32

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(POSTMORTEM_DEBUGGING)

#include "../shared/MinSerial.h"

/**
 * Minimal AT32 USART register layout for postmortem output.
 *
 * AT32F403A USART register offsets (from datasheet):
 *   sts   +0x00  status (tdbe = bit 7: TX data buffer empty)
 *   dt    +0x04  data register
 *   brt   +0x08  baud rate
 *   ctrl1 +0x0C  control 1 (TE = bit 3, UEN = bit 13)
 *   ctrl2 +0x10  control 2
 *   ctrl3 +0x14  control 3
 */
struct AT32USARTMin {
  volatile uint32_t sts;
  volatile uint32_t dt;
  volatile uint32_t brt;
  volatile uint32_t ctrl1;
};

static const uintptr_t usart_base[] = {
  0x40013800, // USART1  (APB2)
  0x40004400, // USART2  (APB1)
  0x40004800, // USART3  (APB1)
  0x40004C00, // UART4   (APB1)
  0x40005000, // UART5   (APB1)
};

#if WITHIN(SERIAL_PORT, 1, 5)
  static AT32USARTMin* const regs = (AT32USARTMin*)usart_base[SERIAL_PORT - 1];
#endif

static void TXBegin() {
  #if WITHIN(SERIAL_PORT, 1, 5)
    // Disable USART interrupt in NVIC so we can poll safely
    const IRQn_Type irqs[] = {
      USART1_IRQn, USART2_IRQn, USART3_IRQn, UART4_IRQn, UART5_IRQn
    };
    NVIC_DisableIRQ(irqs[SERIAL_PORT - 1]);
    __DSB(); __ISB();
    // Re-enable TX only: TE | UEN (preserve baud rate register)
    uint32_t brt = regs->brt;
    regs->ctrl1 = 0;
    regs->ctrl2 = 0;
    regs->ctrl3 = 0;
    regs->brt   = brt;
    regs->ctrl1 = (1U << 3) | (1U << 13); // TE | UEN
  #endif
}

#define sw_barrier() __asm__ volatile("": : :"memory")

static void TX(char c) {
  #if WITHIN(SERIAL_PORT, 1, 5)
    constexpr uint32_t TDBE = (1U << 7);
    while (!(regs->sts & TDBE)) { sw_barrier(); }
    regs->dt = (uint32_t)(uint8_t)c;
  #endif
}

void install_min_serial() {
  HAL_min_serial_init = &TXBegin;
  HAL_min_serial_out  = &TX;
}

#endif // POSTMORTEM_DEBUGGING
#endif // ARDUINO_ARCH_AT32
