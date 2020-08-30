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

#if defined(__IMXRT1062__)

#include "../../inc/MarlinConfig.h"

/** \brief Instruction Synchronization Barrier
  Instruction Synchronization Barrier flushes the pipeline in the processor,
  so that all instructions following the ISB are fetched from cache or
  memory, after the instruction has been completed.
*/
FORCE_INLINE static void __ISB() {
  __asm__ __volatile__("isb 0xF":::"memory");
}

/** \brief Data Synchronization Barrier
  This function acts as a special kind of Data Memory Barrier.
  It completes when all explicit memory accesses before this instruction complete.
*/
FORCE_INLINE static void __DSB() {
  __asm__ __volatile__("dsb 0xF":::"memory");
}

/*
static void __attribute((naked, noinline)) stepTC_Handler() {
  GPT1_SR = GPT_SR_OF1;  // clear OF1 bit

  __DSB();
  __ISB();
}

static void __attribute((naked, noinline)) tempTC_Handler() {
  GPT2_SR = GPT_SR_OF1;  // clear OF1 bit

  __DSB();
  __ISB();
}
*/


void HAL_timer_start(const uint8_t timer_num, const uint32_t frequency) {
  switch (timer_num) {
    case 0:
      if (! NVIC_IS_ENABLED(IRQ_GPT1)) { 
        attachInterruptVector(IRQ_GPT1, &stepTC_Handler);
        NVIC_SET_PRIORITY(IRQ_GPT1, 16);
      }
      CCM_CCGR1 |= CCM_CCGR1_GPT1_BUS(CCM_CCGR_ON);
      CCM_CSCMR1 &= ~CCM_CSCMR1_PERCLK_CLK_SEL; // turn off 24mhz mode
      GPT1_CR = 0;                   // disable timer
      GPT1_PR = GPT1_TIMER_PRESCALE - 1;
      GPT1_CR |= GPT_CR_CLKSRC(1);   //clock selection #1 (peripheral clock = 150 MHz)
      GPT1_CR |= GPT_CR_ENMOD;       //reset count to zero before enabling
      GPT1_CR |= GPT_CR_OM3(1);      // toggle mode 3
      GPT1_OCR1 = (GPT1_TIMER_RATE) / frequency - 1; // Initial compare value
      GPT1_SR = 0x3F;                // clear all prior status
      GPT1_IR = GPT_IR_OF1IE;        // use first timer
      GPT1_CR = GPT_CR_EN;           // set to peripheral clock (24MHz)
      #if ENABLED(MARLIN_DEV_MODE)
        SERIAL_ECHOLNPAIR_F("GPT1CR: ", GPT1_CR);
        SERIAL_ECHOLNPAIR_F("GPT1PR: ", GPT1_PR);
        SERIAL_ECHOLNPAIR_F("GPT1OCR1: ", GPT1_OCR1);
        SERIAL_ECHOLNPAIR_F("GPT1SR: ", GPT1_SR);
        SERIAL_ECHOLNPAIR_F("GPT1IR: ", GPT1_IR);
      #endif
      break;
    case 1:
      if (! NVIC_IS_ENABLED(IRQ_GPT2)) { 
        attachInterruptVector(IRQ_GPT2, &tempTC_Handler);
        NVIC_SET_PRIORITY(IRQ_GPT2, 32);
      }
      CCM_CCGR0 |= CCM_CCGR0_GPT2_BUS(CCM_CCGR_ON);
      CCM_CSCMR1 &= ~CCM_CSCMR1_PERCLK_CLK_SEL; // turn off 24mhz mode
      GPT2_CR = 0;                   // disable timer
      GPT2_SR = 0x3F;                // clear all prior status
      GPT2_PR = GPT2_TIMER_PRESCALE; //- 1;
      GPT2_CR |= GPT_CR_CLKSRC(1);   //clock selection #1 (peripheral clock = 150 MHz)
      GPT2_CR |= GPT_CR_ENMOD;       //reset count to zero before enabling
      GPT2_CR |= GPT_CR_OM3(1);      // toggle mode 3
      GPT2_OCR1 = (GPT2_TIMER_RATE) / frequency; // Initial compare value
      GPT2_IR = GPT_IR_OF1IE;        // use first timer
      GPT2_CR = GPT_CR_EN;           // set to peripheral clock (24MHz)
      #if ENABLED(MARLIN_DEV_MODE)
        SERIAL_ECHOLNPAIR_F("GPT2CR: ", GPT2_CR);
        SERIAL_ECHOLNPAIR_F("GPT2PR: ", GPT2_PR);
        SERIAL_ECHOLNPAIR_F("GPT2OCR1: ", GPT2_OCR1);
        SERIAL_ECHOLNPAIR_F("GPT2SR: ", GPT2_SR);
        SERIAL_ECHOLNPAIR_F("GPT2IR: ", GPT2_IR);
      #endif
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
  __DSB();
  __ISB();
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
      GPT1_SR |= GPT_SR_OF1;  // clear set bit
      break;
    case 1:
      GPT2_SR |= GPT_SR_OF1;  // clear set bit
      break;
  }
}

volatile uint32_t GPT1Ticks = 0;
volatile uint32_t GPT2Ticks = 0;

void HAL_timer_isr_epilogue(const uint8_t timer_num) {
  switch (timer_num) {
    case 0:
      GPT1Ticks++;
      if (GPT1Ticks >= 1000) {
        GPT1Ticks = 0;
        OUT_WRITE(14, HIGH);
      }
      if (GPT1Ticks == 100) {
        OUT_WRITE(14, LOW);
      }
      break;
    case 1:
      if (GPT2Ticks >= 1000) {
        GPT2Ticks = 0;
        OUT_WRITE(15, HIGH);
      }
      if (GPT2Ticks == 100) {
        OUT_WRITE(15, LOW);
      }
      break;
  }
}

#endif // Teensy4.0/4.1
