/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * Timers for LPC5528
 */

#ifdef TARGET_LPC5528

#include "../../inc/MarlinConfig.h"

void HAL_timer_init() {
  timer_init();
}

void HAL_timer_start(const uint8_t timer_num, const uint32_t frequency) {

  // 我们只需要初始化两个定时器即可完成脉冲输出的方式，
  // 在过去的GRBL里，是通过两个定时器来相互拉扯形成脉冲
  // 来控制电机的，在Marlin上做了改变，脉冲是通过一个定
  // 时器来产生的。

  switch (timer_num) {

    case MF_TIMER_STEP:
    timer_start(timer_num, 1000000 / frequency);                    //add TEMP_TIMER_RATE by M 2022.3.9
    NVIC_SetPriority(CTIMER0_IRQn, NVIC_EncodePriority(0, 1, 0));   // fix-wang-2022-10-31
    break;
    case MF_TIMER_TEMP:
    timer_start(timer_num, TEMP_TIMER_RATE / frequency);            //add TEMP_TIMER_RATE by M 2022.3.9
    NVIC_SetPriority(CTIMER1_IRQn, NVIC_EncodePriority(0, 2, 0));   // fix-wang-2022-10-31
    break;

    // case TMC_TIMER:
    // timer_start(timer_num, TEMP_TIMER_RATE / frequency);
    // NVIC_SetPriority(CTIMER4_IRQn, NVIC_EncodePriority(0, 4, 0));
    // break;

    // case MF_TIMER_PULSE:
    // timer_start(timer_num, PULSE_TIMER_RATE / frequency);     //add TEMP_TIMER_RATE by M 2022.3.9
    // break;

    default:
    break;
  }
}

uint32_t GetStepperTimerClkFreq(void) {
  return CLOCK_GetCTimerClkFreq(MF_TIMER_STEP);
}

#endif // TARGET_LPC5528
