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

#include "../HAL.h"
#include "HAL_timers.h"

void HAL_timer_init(void) {
  LPC_SC->PCONP |= (0x1 << 0x1);  // power on timer0
  LPC_TIM0->PR = ((HAL_TIMER_RATE / HAL_STEPPER_TIMER_RATE) - 1); // Use prescaler to set frequency if needed

  LPC_SC->PCONP |= (0x1 << 0x2);  // power on timer1
  LPC_TIM1->PR = ((HAL_TIMER_RATE / 1000000) - 1);
}

void HAL_timer_start(uint8_t timer_num, uint32_t frequency) {
  switch(timer_num) {
  case 0:
    LPC_TIM0->MCR = 3;              // Match on MR0, reset on MR0
    LPC_TIM0->MR0 = (uint32_t)(HAL_STEPPER_TIMER_RATE / frequency); // Match value (period) to set frequency
    LPC_TIM0->TCR = (1 << 0);       // enable
    break;
  case 1:
    LPC_TIM1->MCR = 3;
    LPC_TIM1->MR0 = (uint32_t)(HAL_TEMP_TIMER_RATE / frequency);;
    LPC_TIM1->TCR = (1 << 0);
    break;
  default:
    return;
  }
}

void HAL_timer_enable_interrupt (uint8_t timer_num) {
  switch(timer_num) {
  case 0:
    NVIC_EnableIRQ(TIMER0_IRQn);     // Enable interrupt handler
    NVIC_SetPriority(TIMER0_IRQn, NVIC_EncodePriority(0, 1, 0));
    break;
  case 1:
    NVIC_EnableIRQ(TIMER1_IRQn);
    NVIC_SetPriority(TIMER1_IRQn, NVIC_EncodePriority(0, 2, 0));
    break;
  }
}

void HAL_timer_disable_interrupt (uint8_t timer_num) {
  switch(timer_num) {
  case 0:
    NVIC_DisableIRQ(TIMER0_IRQn);     // disable interrupt handler
    break;
  case 1:
    NVIC_DisableIRQ(TIMER1_IRQn);
    break;
  }
}

void HAL_timer_isr_prologue (uint8_t timer_num) {
  switch(timer_num) {
  case 0:
    LPC_TIM0->IR |= 1; //Clear the Interrupt
    break;
  case 1:
    LPC_TIM1->IR |= 1;
    break;
  }
}


#endif // TARGET_LPC1768
