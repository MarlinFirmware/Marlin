/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 * Copyright (c) 2016 Bob Cousins bobcousins42@googlemail.com
 * Copyright (c) 2015-2016 Nico Tonnhofer wurstnase.reprap@gmail.com
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * Description:
 *
 * For TARGET_LPC1768
 */

#ifdef TARGET_LPC1768

#include "../../inc/MarlinConfig.h"
#include "HAL_timers.h"

void HAL_timer_init(void) {
  SBI(LPC_SC->PCONP, 1);  // power on timer0
  LPC_TIM0->PR = (HAL_TIMER_RATE) / (HAL_STEPPER_TIMER_RATE) - 1; // Use prescaler to set frequency if needed

  SBI(LPC_SC->PCONP, 2);  // power on timer1
  LPC_TIM1->PR = (HAL_TIMER_RATE) / 1000000 - 1;
}

void HAL_timer_start(const uint8_t timer_num, const uint32_t frequency) {
  switch (timer_num) {
    case 0:
      LPC_TIM0->MCR = 3;              // Match on MR0, reset on MR0
      LPC_TIM0->MR0 = uint32_t(HAL_STEPPER_TIMER_RATE) / frequency; // Match value (period) to set frequency
      LPC_TIM0->TCR = _BV(0);       // enable
      break;
    case 1:
      LPC_TIM1->MCR = 3;
      LPC_TIM1->MR0 = uint32_t(HAL_TEMP_TIMER_RATE) / frequency;
      LPC_TIM1->TCR = _BV(0);
      break;
    default: break;
  }
}

void HAL_timer_enable_interrupt(const uint8_t timer_num) {
  switch (timer_num) {
    case 0:
      NVIC_EnableIRQ(TIMER0_IRQn); // Enable interrupt handler
      NVIC_SetPriority(TIMER0_IRQn, NVIC_EncodePriority(0, 1, 0));
      break;
    case 1:
      NVIC_EnableIRQ(TIMER1_IRQn);
      NVIC_SetPriority(TIMER1_IRQn, NVIC_EncodePriority(0, 2, 0));
      break;
  }
}

void HAL_timer_disable_interrupt(const uint8_t timer_num) {
  switch (timer_num) {
    case 0: NVIC_DisableIRQ(TIMER0_IRQn); break; // disable interrupt handler
    case 1: NVIC_DisableIRQ(TIMER1_IRQn); break;
  }
}

bool HAL_timer_interrupt_enabled(const uint8_t timer_num) {
  switch (timer_num) {
    case 0: return NVIC_GetActive(TIMER0_IRQn);
    case 1: return NVIC_GetActive(TIMER1_IRQn);
  }
  return false;
}

void HAL_timer_isr_prologue(const uint8_t timer_num) {
  switch (timer_num) {
    case 0: SBI(LPC_TIM0->IR, 0); break; // Clear the Interrupt
    case 1: SBI(LPC_TIM1->IR, 0); break;
  }
}


#endif // TARGET_LPC1768
