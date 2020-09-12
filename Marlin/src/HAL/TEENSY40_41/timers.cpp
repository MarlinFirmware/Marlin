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
 * Teensy4.0/4.1 (__IMXRT1062__)
 */

#ifdef __IMXRT1062__

#include "../../inc/MarlinConfig.h"

void HAL_timer_start(const uint8_t timer_num, const uint32_t frequency) {
  switch (timer_num) {
    case 0:
      CCM_CSCMR1 &= ~CCM_CSCMR1_PERCLK_CLK_SEL; // turn off 24mhz mode
      CCM_CCGR1 |= CCM_CCGR1_GPT1_BUS(CCM_CCGR_ON);

      GPT1_CR = 0;                   // disable timer
      GPT1_SR = 0x3F;                // clear all prior status
      GPT1_PR = GPT1_TIMER_PRESCALE - 1;
      GPT1_CR |= GPT_CR_CLKSRC(1);   //clock selection #1 (peripheral clock = 150 MHz)
      GPT1_CR |= GPT_CR_ENMOD;       //reset count to zero before enabling
      GPT1_CR |= GPT_CR_OM1(1);      // toggle mode
      GPT1_OCR1 = (GPT1_TIMER_RATE / frequency) -1; // Initial compare value
      GPT1_IR = GPT_IR_OF1IE;        // Compare3 value
      GPT1_CR |= GPT_CR_EN;          //enable GPT2 counting at 150 MHz

      OUT_WRITE(15, HIGH);
      attachInterruptVector(IRQ_GPT1, &stepTC_Handler);
      NVIC_SET_PRIORITY(IRQ_GPT1, 16);
      break;
    case 1:
      CCM_CSCMR1 &= ~CCM_CSCMR1_PERCLK_CLK_SEL; // turn off 24mhz mode
      CCM_CCGR0 |= CCM_CCGR0_GPT2_BUS(CCM_CCGR_ON);

      GPT2_CR = 0;                   // disable timer
      GPT2_SR = 0x3F;                // clear all prior status
      GPT2_PR = GPT2_TIMER_PRESCALE - 1;
      GPT2_CR |= GPT_CR_CLKSRC(1);   //clock selection #1 (peripheral clock = 150 MHz)
      GPT2_CR |= GPT_CR_ENMOD;       //reset count to zero before enabling
      GPT2_CR |= GPT_CR_OM1(1);      // toggle mode
      GPT2_OCR1 = (GPT2_TIMER_RATE / frequency) -1; // Initial compare value
      GPT2_IR = GPT_IR_OF1IE;        // Compare3 value
      GPT2_CR |= GPT_CR_EN;          //enable GPT2 counting at 150 MHz

      OUT_WRITE(14, HIGH);
      attachInterruptVector(IRQ_GPT2, &tempTC_Handler);
      NVIC_SET_PRIORITY(IRQ_GPT2, 32);
      break;
  }
}

void HAL_timer_enable_interrupt(const uint8_t timer_num) {
  switch (timer_num) {
    case 0:
      NVIC_ENABLE_IRQ(IRQ_GPT1);
      break;
    case 1:
      NVIC_ENABLE_IRQ(IRQ_GPT2);
      break;
  }
}

void HAL_timer_disable_interrupt(const uint8_t timer_num) {
  switch (timer_num) {
    case 0: NVIC_DISABLE_IRQ(IRQ_GPT1); break;
    case 1: NVIC_DISABLE_IRQ(IRQ_GPT2); break;
  }

  // We NEED memory barriers to ensure Interrupts are actually disabled!
  // ( https://dzone.com/articles/nvic-disabling-interrupts-on-arm-cortex-m-and-the )
  asm volatile("dsb");
}

bool HAL_timer_interrupt_enabled(const uint8_t timer_num) {
  switch (timer_num) {
    case 0: return (NVIC_IS_ENABLED(IRQ_GPT1));
    case 1: return (NVIC_IS_ENABLED(IRQ_GPT2));
  }
  return false;
}

void HAL_timer_isr_prologue(const uint8_t timer_num) {
  switch (timer_num) {
    case 0:
      GPT1_SR = GPT_IR_OF1IE;  // clear OF3 bit
      break;
    case 1:
      GPT2_SR = GPT_IR_OF1IE;  // clear OF3 bit
      break;
  }
  asm volatile("dsb");
}

#endif // __IMXRT1062__
