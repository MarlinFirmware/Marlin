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

#ifdef ARDUINO_ARCH_HC32

#include "timers.h"
#include <core_debug.h>

/**
 * Timer0 Unit 2 Channel A is used for Temperature interrupts
 */
Timer0 temp_timer(&TIMER02A_config, &Temp_Handler);

/**
 * Timer0 Unit 2 Channel B is used for Step interrupts
 */
Timer0 step_timer(&TIMER02B_config, &Step_Handler);

void HAL_timer_start(const timer_channel_t timer_num, const uint32_t frequency) {
  if (timer_num == TEMP_TIMER_NUM) {
    CORE_DEBUG_PRINTF("HAL_timer_start: temp timer, f=%ld\n", long(frequency));
    timer_num->start(frequency, TEMP_TIMER_PRESCALE);
    timer_num->setCallbackPriority(TEMP_TIMER_PRIORITY);
  }
  else if (timer_num == STEP_TIMER_NUM) {
    CORE_DEBUG_PRINTF("HAL_timer_start: step timer, f=%ld\n", long(frequency));
    timer_num->start(frequency, STEPPER_TIMER_PRESCALE);
    timer_num->setCallbackPriority(STEP_TIMER_PRIORITY);
  }
  else {
    CORE_ASSERT_FAIL("HAL_timer_start: invalid timer_num")
  }
}

#endif // ARDUINO_ARCH_HC32
