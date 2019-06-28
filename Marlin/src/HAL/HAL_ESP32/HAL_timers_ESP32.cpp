/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifdef ARDUINO_ARCH_ESP32

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------

#include <stdio.h>
#include "esp_types.h"
#include "soc/timer_group_struct.h"
#include "driver/periph_ctrl.h"
#include "driver/timer.h"

#include "HAL.h"

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

static timg_dev_t *TG[2] = {&TIMERG0, &TIMERG1};

const tTimerConfig TimerConfig [NUM_HARDWARE_TIMERS] = {
  { TIMER_GROUP_0, TIMER_0, STEPPER_TIMER_PRESCALE, stepTC_Handler }, // 0 - Stepper
  { TIMER_GROUP_0, TIMER_1,    TEMP_TIMER_PRESCALE, tempTC_Handler }, // 1 - Temperature
  { TIMER_GROUP_1, TIMER_0,                      1, nullptr }, // 2
  { TIMER_GROUP_1, TIMER_1,                      1, nullptr }, // 3
};

// --------------------------------------------------------------------------
// Function prototypes
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Private functions
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Public functions
// --------------------------------------------------------------------------

void IRAM_ATTR timer_group0_isr(void *para) {
  const int timer_idx = (int)para;

  // Retrieve the interrupt status and the counter value
  // from the timer that reported the interrupt
  uint32_t intr_status = TIMERG0.int_st_timers.val;
  TIMERG0.hw_timer[timer_idx].update = 1;

  // Clear the interrupt
  if (intr_status & BIT(timer_idx)) {
    switch (timer_idx) {
      case TIMER_0: TIMERG0.int_clr_timers.t0 = 1; break;
      case TIMER_1: TIMERG0.int_clr_timers.t1 = 1; break;
    }
  }

  const tTimerConfig timer = TimerConfig[timer_idx];
  timer.fn();

  // After the alarm has been triggered
  // Enable it again so it gets triggered the next time
  TIMERG0.hw_timer[timer_idx].config.alarm_en = TIMER_ALARM_EN;
}

/**
 * Enable and initialize the timer
 * @param timer_num timer number to initialize
 * @param frequency frequency of the timer
 */
void HAL_timer_start(const uint8_t timer_num, uint32_t frequency) {
  const tTimerConfig timer = TimerConfig[timer_num];

  timer_config_t config;
  config.divider     = timer.divider;
  config.counter_dir = TIMER_COUNT_UP;
  config.counter_en  = TIMER_PAUSE;
  config.alarm_en    = TIMER_ALARM_EN;
  config.intr_type   = TIMER_INTR_LEVEL;
  config.auto_reload = true;

  // Select and initialize the timer
  timer_init(timer.group, timer.idx, &config);

  // Timer counter initial value and auto reload on alarm
  timer_set_counter_value(timer.group, timer.idx, 0x00000000ULL);

  // Configure the alam value and the interrupt on alarm
  timer_set_alarm_value(timer.group, timer.idx, (HAL_TIMER_RATE) / timer.divider / frequency - 1);

  timer_enable_intr(timer.group, timer.idx);

  // TODO need to deal with timer_group1_isr
  timer_isr_register(timer.group, timer.idx, timer_group0_isr, (void*)timer.idx, 0, nullptr);

  timer_start(timer.group, timer.idx);
}

/**
 * Set the upper value of the timer, when the timer reaches this upper value the
 * interrupt should be triggered and the counter reset
 * @param timer_num timer number to set the count to
 * @param count     threshold at which the interrupt is triggered
 */
void HAL_timer_set_compare(const uint8_t timer_num, hal_timer_t count) {
  const tTimerConfig timer = TimerConfig[timer_num];
  timer_set_alarm_value(timer.group, timer.idx, count);
}

/**
 * Get the current upper value of the timer
 * @param  timer_num timer number to get the count from
 * @return           the timer current threshold for the alarm to be triggered
 */
hal_timer_t HAL_timer_get_compare(const uint8_t timer_num) {
  const tTimerConfig timer = TimerConfig[timer_num];

  uint64_t alarm_value;
  timer_get_alarm_value(timer.group, timer.idx, &alarm_value);

  return alarm_value;
}

/**
 * Get the current counter value between 0 and the maximum count (HAL_timer_set_count)
 * @param  timer_num timer number to get the current count
 * @return           the current counter of the alarm
 */
hal_timer_t HAL_timer_get_count(const uint8_t timer_num) {
  const tTimerConfig timer = TimerConfig[timer_num];
  uint64_t counter_value;
  timer_get_counter_value(timer.group, timer.idx, &counter_value);
  return counter_value;
}

/**
 * Enable timer interrupt on the timer
 * @param timer_num timer number to enable interrupts on
 */
void HAL_timer_enable_interrupt(const uint8_t timer_num) {
  //const tTimerConfig timer = TimerConfig[timer_num];
  //timer_enable_intr(timer.group, timer.idx);
}

/**
 * Disable timer interrupt on the timer
 * @param timer_num timer number to disable interrupts on
 */
void HAL_timer_disable_interrupt(const uint8_t timer_num) {
  //const tTimerConfig timer = TimerConfig[timer_num];
  //timer_disable_intr(timer.group, timer.idx);
}

bool HAL_timer_interrupt_enabled(const uint8_t timer_num) {
  const tTimerConfig timer = TimerConfig[timer_num];
  return TG[timer.group]->int_ena.val | BIT(timer_num);
}

#endif // ARDUINO_ARCH_ESP32
