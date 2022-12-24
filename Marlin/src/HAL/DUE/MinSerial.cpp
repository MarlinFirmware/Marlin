/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#ifdef ARDUINO_ARCH_SAM

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(POSTMORTEM_DEBUGGING)

#include "../shared/MinSerial.h"

#include <stdarg.h>

static void TXBegin() {
  // Disable UART interrupt in NVIC
  NVIC_DisableIRQ( UART_IRQn );

  // We NEED memory barriers to ensure Interrupts are actually disabled!
  // ( https://dzone.com/articles/nvic-disabling-interrupts-on-arm-cortex-m-and-the )
  __DSB();
  __ISB();

  // Disable clock
  pmc_disable_periph_clk( ID_UART );

  // Configure PMC
  pmc_enable_periph_clk( ID_UART );

  // Disable PDC channel
  UART->UART_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;

  // Reset and disable receiver and transmitter
  UART->UART_CR = UART_CR_RSTRX | UART_CR_RSTTX | UART_CR_RXDIS | UART_CR_TXDIS;

  // Configure mode: 8bit, No parity, 1 bit stop
  UART->UART_MR = UART_MR_CHMODE_NORMAL | US_MR_CHRL_8_BIT | US_MR_NBSTOP_1_BIT | UART_MR_PAR_NO;

  // Configure baudrate (asynchronous, no oversampling) to BAUDRATE bauds
  UART->UART_BRGR = (SystemCoreClock / (BAUDRATE << 4));

  // Enable receiver and transmitter
  UART->UART_CR = UART_CR_RXEN | UART_CR_TXEN;
}

// A SW memory barrier, to ensure GCC does not overoptimize loops
#define sw_barrier() __asm__ volatile("": : :"memory");
static void TX(char c) {
  while (!(UART->UART_SR & UART_SR_TXRDY)) { WDT_Restart(WDT); sw_barrier(); };
  UART->UART_THR = c;
}

void install_min_serial() {
  HAL_min_serial_init = &TXBegin;
  HAL_min_serial_out = &TX;
}

#if DISABLED(DYNAMIC_VECTORTABLE)
extern "C" {
  __attribute__((naked)) void JumpHandler_ASM() {
    __asm__ __volatile__ (
      "b CommonHandler_ASM\n"
    );
  }
  void __attribute__((naked, alias("JumpHandler_ASM"))) HardFault_Handler();
  void __attribute__((naked, alias("JumpHandler_ASM"))) BusFault_Handler();
  void __attribute__((naked, alias("JumpHandler_ASM"))) UsageFault_Handler();
  void __attribute__((naked, alias("JumpHandler_ASM"))) MemManage_Handler();
  void __attribute__((naked, alias("JumpHandler_ASM"))) NMI_Handler();
}
#endif

#endif // POSTMORTEM_DEBUGGING
#endif // ARDUINO_ARCH_SAM
