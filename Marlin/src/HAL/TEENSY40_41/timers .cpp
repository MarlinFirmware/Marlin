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

void HAL_timer_start(const uint8_t timer_num, const uint32_t frequency) {
  switch (timer_num) {
    case 0:
      TMR1_CTRL2 = 0; // stop
      TMR1_CSCTRL2 = 0;
      TMR1_LOAD2 = 0;  // start val after compare
      TMR1_COMP12 = (TMR1_TIMER_RATE) / frequency;  // count up to this val, interrupt,  and start again
      TMR1_CMPLD12 = (TMR1_TIMER_RATE) / frequency; // reload to the same value
      TMR1_SCTRL2 = TMR_SCTRL_CAPTURE_MODE(1);  //rising
      TMR1_CTRL2 = TMR_CTRL_CM(1); // Count rising edges of bus clock
      TMR1_CTRL2 |= TMR_CTRL_PCS(TMR1_TIMER_PRESCALE_BITS);
      TMR1_CTRL2 |= TMR_CTRL_SCS(1);
      TMR1_CTRL2 |= TMR_CTRL_LENGTH; // Count until compare, then re-initialize
    case 1:
      TMR2_CTRL2 = 0; // stop
      TMR2_CSCTRL2 = 0;
      TMR2_LOAD2 = 0;  // start val after compare
      TMR2_COMP12 = (TMR1_TIMER_RATE) / frequency;  // count up to this val, interrupt,  and start again
      TMR2_CMPLD12 = (TMR1_TIMER_RATE) / frequency; // reload to the same value
      TMR2_CTRL2 = TMR_CTRL_CM(1); // Count rising edges of bus clock
      TMR2_CTRL2 |= TMR_CTRL_PCS(TMR1_TIMER_PRESCALE_BITS);
      TMR2_CTRL2 |= TMR_CTRL_SCS(2);
      TMR2_CTRL2 |= TMR_CTRL_LENGTH; // Count until compare, then re-initialize
      break;
  }
}

void HAL_timer_enable_interrupt(const uint8_t timer_num) {
  switch (timer_num) {
    case 0:
      TMR1_CSCTRL2 = TMR_CSCTRL_TCF1EN;  // enable compare interrupt
      break;
    case 1:
      TMR2_CSCTRL2 = TMR_CSCTRL_TCF1EN;  // enable compare interrupt
      break;
  }
}

void HAL_timer_disable_interrupt(const uint8_t timer_num) {
  switch (timer_num) {
    case 0:
      TMR1_CSCTRL2 &= ~TMR_CSCTRL_TCF1EN;  // enable compare interrupt
      break;
    case 1:
      TMR2_CSCTRL2 &= ~TMR_CSCTRL_TCF1EN;  // enable compare interrupt
      break;
  }

  // We NEED memory barriers to ensure Interrupts are actually disabled!
  // ( https://dzone.com/articles/nvic-disabling-interrupts-on-arm-cortex-m-and-the )
  __DSB();
  __ISB();
}

bool HAL_timer_interrupt_enabled(const uint8_t timer_num) {
  switch (timer_num) {
    case 0: return NVIC_IS_ENABLED(IRQ_QTIMER1);
    case 1: return NVIC_IS_ENABLED(IRQ_QTIMER1);
  }
  return false;
}

void HAL_timer_isr_prologue(const uint8_t timer_num) {
  switch (timer_num) {
    case 0:
      TMR1_LOAD2 = 0x0000;
      TMR1_SCTRL2 &= ~TMR_SCTRL_TOF; // Clear FTM Overflow flag
      TMR1_SCTRL2 &= ~TMR_SCTRL_TCF; // Clear FTM Channel Compare flag
      break;
    case 1:
      TMR2_LOAD2 = 0x0000;
      TMR2_SCTRL2 &= ~TMR_SCTRL_TOF; // Clear FTM Overflow flag
      TMR2_SCTRL2 &= ~TMR_SCTRL_TCF; // Clear FTM Channel Compare flag
      break;
  }
}

#endif // Teensy4.0/4.1
