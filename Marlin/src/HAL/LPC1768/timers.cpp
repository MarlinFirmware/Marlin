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
 * Description:
 *
 * Timers for LPC1768
 */

#ifdef TARGET_LPC1768

#include "../../inc/MarlinConfig.h"

void HAL_timer_init() {
  SBI(LPC_SC->PCONP, SBIT_TIMER0);  // Power ON Timer 0
  LPC_TIM0->PR = (HAL_TIMER_RATE) / (STEPPER_TIMER_RATE) - 1; // Use prescaler to set frequency if needed

  SBI(LPC_SC->PCONP, SBIT_TIMER1);  // Power ON Timer 1
  LPC_TIM1->PR = (HAL_TIMER_RATE) / 1000000 - 1;
}

void HAL_timer_start(const uint8_t timer_num, const uint32_t frequency) {
  switch (timer_num) {
    case MF_TIMER_STEP:
      LPC_TIM0->MCR = _BV(SBIT_MR0I) | _BV(SBIT_MR0R); // Match on MR0, reset on MR0, interrupts when NVIC enables them
      LPC_TIM0->MR0 = uint32_t(STEPPER_TIMER_RATE) / frequency; // Match value (period) to set frequency
      LPC_TIM0->TCR = _BV(SBIT_CNTEN); // Counter Enable

      NVIC_SetPriority(TIMER0_IRQn, NVIC_EncodePriority(0, 1, 0));
      NVIC_EnableIRQ(TIMER0_IRQn);
      break;

    case MF_TIMER_TEMP:
      LPC_TIM1->MCR = _BV(SBIT_MR0I) | _BV(SBIT_MR0R); // Match on MR0, reset on MR0, interrupts when NVIC enables them
      LPC_TIM1->MR0 = uint32_t(TEMP_TIMER_RATE) / frequency;
      LPC_TIM1->TCR = _BV(SBIT_CNTEN); // Counter Enable

      NVIC_SetPriority(TIMER1_IRQn, NVIC_EncodePriority(0, 2, 0));
      NVIC_EnableIRQ(TIMER1_IRQn);
      break;

    default: break;
  }
}

#endif // TARGET_LPC1768
