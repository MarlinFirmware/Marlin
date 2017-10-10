/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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

#ifdef ARDUINO_ARCH_ESP32

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------

#include "../HAL.h"

#include "HAL_timers_ESP32.h"

// --------------------------------------------------------------------------
// Externals
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Local defines
// --------------------------------------------------------------------------

#define NUM_HARDWARE_TIMERS 4

// --------------------------------------------------------------------------
// Types
// --------------------------------------------------------------------------


// --------------------------------------------------------------------------
// Public Variables
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Private Variables
// --------------------------------------------------------------------------

hw_timer_t *timers[NUM_HARDWARE_TIMERS];

// --------------------------------------------------------------------------
// Function prototypes
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Private functions
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Public functions
// --------------------------------------------------------------------------

/**
 * Enable and initialize the timer
 * @param timer_num timer number to initialize
 * @param frequency frequency of the timer
 */
void HAL_timer_start (uint8_t timer_num, uint32_t frequency) {
  switch (timer_num) {
    case STEP_TIMER_NUM:
      timers[timer_num] = timerBegin(timer_num, STEP_TIMER_PRESCALE, true);
      timerAttachInterrupt(timers[timer_num], &stepTC_Handler, true);
      timerAlarmWrite(timers[timer_num], STEP_TIMER_RATE / frequency, false);
      timerAlarmEnable(timers[timer_num]);
      break;
    case TEMP_TIMER_NUM:
      timers[timer_num] = timerBegin(timer_num, TEMP_TIMER_PRESCALE, true);
      timerAttachInterrupt(timers[timer_num], &tempTC_Handler, true);
      timerAlarmWrite(timers[timer_num], TEMP_TIMER_RATE / frequency, false);
      timerAlarmEnable(timers[timer_num]);
      break;
  }
}

/**
 * Set the upper value of the timer, when the timer reaches this upper value the
 * interrupt should be triggered and the counter reset
 * @param timer_num timer number to set the count to
 * @param count     threshold at which the interrupt is triggered
 */
void HAL_timer_set_count(uint8_t timer_num, HAL_TIMER_TYPE count) {
  timerAlarmWrite(timers[timer_num], count, false);
}

/**
 * Get the current upper value of the timer
 * @param  timer_num timer number to get the count from
 * @return           the timer current threshold for the alarm to be triggered
 */
HAL_TIMER_TYPE HAL_timer_get_count (uint8_t timer_num) {
  return timerAlarmRead(timers[timer_num]);
}

/**
 * Get the current counter value between 0 and the maximum count (HAL_timer_set_count)
 * @param  timer_num timer number to get the current count
 * @return           the current counter of the alarm
 */
HAL_TIMER_TYPE HAL_timer_get_current_count(uint8_t timer_num) {
  return timerRead(timers[timer_num]);
}

/**
 * Enable timer interrupt on the timer
 * @param timer_num timer number to enable interrupts on
 */
void HAL_timer_enable_interrupt (uint8_t timer_num) {
  timerAlarmEnable(timers[timer_num]);
}

/**
 * Disable timer interrupt on the timer
 * @param timer_num timer number to disable interrupts on
 */
void HAL_timer_disable_interrupt (uint8_t timer_num) {
  timerAlarmDisable(timers[timer_num]);
}

/**
 * Function executed before the ISR
 * @param timer_num timer number which was triggered
 */
void HAL_timer_isr_prologue(uint8_t timer_num) {
  timerWrite(timers[timer_num], 0);
}

#endif // ARDUINO_ARCH_ESP32
