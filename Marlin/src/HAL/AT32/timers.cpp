/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#ifdef ARDUINO_ARCH_AT32

#include "timers.h"

/**
 * HAL_timer_start — configure a hardware timer with exact prescaler and period.
 *
 * Uses Timer_SetInterruptBase() so that the prescaler is exactly what Marlin
 * expects (STEPPER_TIMER_PRESCALE / TEMP_TIMER_PRESCALE), ensuring that
 * STEPPER_TIMER_RATE compile-time constants are valid.
 *
 * Timer_SetInterruptBase(TIMx, Period, Prescaler, callback, PreemptPrio, SubPrio)
 *   Period    = (timer_clock / prescaler / frequency) - 1
 *   Prescaler = desired prescaler - 1 (hardware loads prescaler+1)
 */
void HAL_timer_start(const timer_channel_t timer_ch, const uint32_t frequency) {
  if (timer_ch == MF_TIMER_TEMP) {
    const uint16_t period = (uint16_t)(F_PCLK1 / TEMP_TIMER_PRESCALE / frequency) - 1;
    Timer_SetInterruptBase(TMR4, period, (uint16_t)(TEMP_TIMER_PRESCALE - 1),
                           Temp_Handler, TEMP_TIMER_PRIORITY, 0);
  }
  else if (timer_ch == MF_TIMER_STEP) {
    const uint16_t period = (uint16_t)(F_PCLK1 / STEPPER_TIMER_PRESCALE / frequency) - 1;
    Timer_SetInterruptBase(TMR2, period, (uint16_t)(STEPPER_TIMER_PRESCALE - 1),
                           Step_Handler, STEP_TIMER_PRIORITY, 0);
  }
}

#endif // ARDUINO_ARCH_AT32
