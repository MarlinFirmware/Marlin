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

/**
 * HAL Timers for Teensy 4.0 (IMXRT1062DVL6A) / 4.1 (IMXRT1062DVJ6A)
 */

#ifdef __IMXRT1062__

#include "../../inc/MarlinConfig.h"

void HAL_timer_start(const uint8_t timer_num, const uint32_t frequency) {
  switch (timer_num) {

    //
    // Step Timer – GPT1 - Compare Interrupt OCR1 - Reset Mode
    //
    case MF_TIMER_STEP:
      // 24MHz mode off – Use peripheral clock (150MHz)
      CCM_CSCMR1 &= ~CCM_CSCMR1_PERCLK_CLK_SEL;
      // Enable GPT1 clock gating
      CCM_CCGR1 |= CCM_CCGR1_GPT1_BUS(CCM_CCGR_ON);

      // Disable timer, clear all status bits
      GPT1_CR = 0;                      // Disable timer
      GPT1_SR = 0x3F;                   // Clear all prior status

      // Prescaler = 2 => 75MHz counting clock
      GPT1_PR = GPT1_TIMER_PRESCALE - 1;

      GPT1_CR = GPT_CR_CLKSRC(1)        // Clock selection #1 (peripheral clock = 150 MHz)
              | GPT_CR_ENMOD            // Reset count to zero before enabling
              | GPT_CR_OM2(TERN(MARLIN_DEV_MODE, 1, 0)); // 0 = edge compare, 1 = toggle

      // Compare value – the number of clocks between edges
      GPT1_OCR1 = (GPT1_TIMER_RATE / frequency) - 1;

      // Enable compare‑event interrupt
      GPT1_IR = GPT_IR_OF1IE;           // OF1 interrupt enabled

      // Pull Pin 15 HIGH (logic‑high is the “idle” state)
      TERN_(MARLIN_DEV_MODE, OUT_WRITE(15, HIGH));

      // Attach and enable Stepper IRQ
      // Note: UART priority is 16
      attachInterruptVector(IRQ_GPT1, &stepTC_Handler);
      NVIC_SET_PRIORITY(IRQ_GPT1, 16);  // Priority 16 (higher than Temp Timer)

      // Start GPT1 counting at 150 MHz
      GPT1_CR |= GPT_CR_EN;

      break;

    //
    // Temperature Timer – GPT2 - Compare Interrupt OCR1 - Reset Mode
    //
    case MF_TIMER_TEMP:
      // 24MHz mode off – Use peripheral clock (150MHz)
      CCM_CSCMR1 &= ~CCM_CSCMR1_PERCLK_CLK_SEL;
      // Enable GPT2 clock gating
      CCM_CCGR0 |= CCM_CCGR0_GPT2_BUS(CCM_CCGR_ON);

      // Disable timer, clear all status bits
      GPT2_CR = 0;                      // Disable timer
      GPT2_SR = 0x3F;                   // Clear all prior status

      // Prescaler = 10 => 15MHz counting clock
      GPT2_PR = GPT2_TIMER_PRESCALE - 1;

      GPT2_CR = GPT_CR_CLKSRC(1)        // Clock selection #1 (peripheral clock = 150 MHz)
              | GPT_CR_ENMOD            // and reset count to zero before enabling
              | GPT_CR_OM2(TERN(MARLIN_DEV_MODE, 1, 0)); // 0 = edge compare, 1 = toggle

      // Compare value – the number of clocks between edges
      GPT2_OCR1 = (GPT2_TIMER_RATE / frequency) - 1;

      // Enable compare‑event interrupt
      GPT2_IR = GPT_IR_OF1IE;           // OF1 interrupt enabled

      // Pull Pin 14 HIGH (logic‑high is the “idle” state)
      TERN_(MARLIN_DEV_MODE, OUT_WRITE(14, HIGH));

      // Attach Temperature ISR
      attachInterruptVector(IRQ_GPT2, &tempTC_Handler);
      NVIC_SET_PRIORITY(IRQ_GPT2, 32);  // Priority 32 (lower than Step Timer)

      // Start GPT2 counting at 150 MHz
      GPT2_CR |= GPT_CR_EN;

      break;
  }
}

void HAL_timer_enable_interrupt(const uint8_t timer_num) {
  switch (timer_num) {
    case MF_TIMER_STEP: NVIC_ENABLE_IRQ(IRQ_GPT1); break;
    case MF_TIMER_TEMP: NVIC_ENABLE_IRQ(IRQ_GPT2); break;
  }
}

void HAL_timer_disable_interrupt(const uint8_t timer_num) {
  switch (timer_num) {
    case MF_TIMER_STEP: NVIC_DISABLE_IRQ(IRQ_GPT1); break;
    case MF_TIMER_TEMP: NVIC_DISABLE_IRQ(IRQ_GPT2); break;
  }

  // Ensure the CPU actually stops servicing the IRQ
  // We NEED memory barriers to ensure Interrupts are actually disabled!
  // ( https://dzone.com/articles/nvic-disabling-interrupts-on-arm-cortex-m-and-the )
  asm volatile("dsb");
}

bool HAL_timer_interrupt_enabled(const uint8_t timer_num) {
  switch (timer_num) {
    case MF_TIMER_STEP: return (NVIC_IS_ENABLED(IRQ_GPT1));
    case MF_TIMER_TEMP: return (NVIC_IS_ENABLED(IRQ_GPT2));
  }
  return false;
}

void HAL_timer_isr_prologue(const uint8_t timer_num) {
  switch (timer_num) {
    case MF_TIMER_STEP: GPT1_SR = GPT_IR_OF1IE; break;   // clear OF1
    case MF_TIMER_TEMP: GPT2_SR = GPT_IR_OF1IE; break;
  }
  asm volatile("dsb");
}

#endif // __IMXRT1062__
