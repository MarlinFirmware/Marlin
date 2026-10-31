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
#ifdef ARDUINO_ARCH_ESP32

#include "../../inc/MarlinConfig.h"

#include <stdio.h>
#include <esp_types.h>
#if ESP_IDF_VERSION_MAJOR < 5
  #include <soc/timer_group_struct.h>
  #include <driver/periph_ctrl.h>
#endif

// ------------------------
// Local defines
// ------------------------

#define NUM_HARDWARE_TIMERS 4

#if ESP_IDF_VERSION_MAJOR >= 5

// ------------------------
// Private Variables
// ------------------------

const tTimerConfig timer_config[NUM_HARDWARE_TIMERS] = {
  { STEPPER_TIMER_PRESCALE, stepTC_Handler }, // 0 - Stepper
  {    TEMP_TIMER_PRESCALE, tempTC_Handler }, // 1 - Temperature
  {     PWM_TIMER_PRESCALE, pwmTC_Handler  }, // 2 - PWM
  {    TONE_TIMER_PRESCALE, toneTC_Handler }, // 3 - Tone
};

static gptimer_handle_t timer_handle[NUM_HARDWARE_TIMERS];
static hal_timer_t timer_alarm[NUM_HARDWARE_TIMERS];

// ------------------------
// Public functions
// ------------------------

static bool IRAM_ATTR timer_isr(gptimer_handle_t, const gptimer_alarm_event_data_t*, void *para) {
  timer_config[(int)para].fn();
  return false;
}

/**
 * Enable and initialize the timer
 * @param timer_num   timer number to initialize
 * @param frequency   frequency of the timer
 */
void HAL_timer_start(const uint8_t timer_num, const uint32_t frequency) {
  const tTimerConfig &timer = timer_config[timer_num];
  gptimer_handle_t &handle = timer_handle[timer_num];

  if (handle)
    gptimer_stop(handle); // Already set up (e.g., a new tone), so just restart it
  else {
    gptimer_config_t config = {};
    config.clk_src = GPTIMER_CLK_SRC_DEFAULT;
    config.direction = GPTIMER_COUNT_UP;
    config.resolution_hz = (HAL_TIMER_RATE) / timer.divider;
    if (gptimer_new_timer(&config, &handle) != ESP_OK) { handle = nullptr; return; }

    gptimer_event_callbacks_t callbacks = {};
    callbacks.on_alarm = timer_isr;
    gptimer_register_event_callbacks(handle, &callbacks, (void*)(uint32_t)timer_num);
    gptimer_enable(handle);
  }

  // Counter starts at 0 and reloads to 0 on each alarm
  gptimer_set_raw_count(handle, 0);
  HAL_timer_set_compare(timer_num, (HAL_TIMER_RATE) / timer.divider / frequency - 1);
  gptimer_start(handle);
}

/**
 * Set the upper value of the timer, when the timer reaches this upper value the
 * interrupt should be triggered and the counter reset
 * @param timer_num timer number to set the compare value to
 * @param compare   threshold at which the interrupt is triggered
 */
void HAL_timer_set_compare(const uint8_t timer_num, const hal_timer_t compare) {
  timer_alarm[timer_num] = compare;
  if (!timer_handle[timer_num]) return;
  gptimer_alarm_config_t alarm = {};
  alarm.alarm_count = compare;
  alarm.reload_count = 0;
  alarm.flags.auto_reload_on_alarm = true;
  gptimer_set_alarm_action(timer_handle[timer_num], &alarm);
}

/**
 * Get the current upper value of the timer
 * @param  timer_num timer number to get the count from
 * @return           the timer current threshold for the alarm to be triggered
 */
hal_timer_t HAL_timer_get_compare(const uint8_t timer_num) { return timer_alarm[timer_num]; }

/**
 * Get the current counter value between 0 and the maximum count (HAL_timer_set_count)
 * @param  timer_num timer number to get the current count
 * @return           the current counter of the alarm
 */
hal_timer_t HAL_timer_get_count(const uint8_t timer_num) {
  uint64_t counter_value = 0;
  if (timer_handle[timer_num]) gptimer_get_raw_count(timer_handle[timer_num], &counter_value);
  return counter_value;
}

/**
 * Stop the timer counting, so its interrupt no longer fires
 * @param timer_num timer number to pause
 */
void HAL_timer_pause(const uint8_t timer_num) {
  if (timer_handle[timer_num]) gptimer_stop(timer_handle[timer_num]);
}

/**
 * Resume a paused timer
 * @param timer_num timer number to resume
 */
void HAL_timer_resume(const uint8_t timer_num) {
  if (timer_handle[timer_num]) gptimer_start(timer_handle[timer_num]);
}

// Like the legacy driver code below, interrupts stay enabled while a timer runs
void HAL_timer_enable_interrupt(const uint8_t) {}
void HAL_timer_disable_interrupt(const uint8_t) {}
bool HAL_timer_interrupt_enabled(const uint8_t) { return true; }

#else // ESP_IDF_VERSION_MAJOR < 5

// ------------------------
// Private Variables
// ------------------------

static timg_dev_t *TG[2] = {&TIMERG0, &TIMERG1};

const tTimerConfig timer_config[NUM_HARDWARE_TIMERS] = {
  { TIMER_GROUP_0, TIMER_0, STEPPER_TIMER_PRESCALE, stepTC_Handler }, // 0 - Stepper
  { TIMER_GROUP_0, TIMER_1,    TEMP_TIMER_PRESCALE, tempTC_Handler }, // 1 - Temperature
  { TIMER_GROUP_1, TIMER_0,     PWM_TIMER_PRESCALE, pwmTC_Handler  }, // 2 - PWM
  { TIMER_GROUP_1, TIMER_1,    TONE_TIMER_PRESCALE, toneTC_Handler }, // 3 - Tone
};

// ------------------------
// Public functions
// ------------------------

void IRAM_ATTR timer_isr(void *para) {
  const tTimerConfig& timer = timer_config[(int)para];

  // Retrieve the interrupt status and the counter value
  // from the timer that reported the interrupt
  uint32_t intr_status = TG[timer.group]->int_st_timers.val;
  TG[timer.group]->hw_timer[timer.idx].update = 1;

  // Clear the interrupt
  if (intr_status & BIT(timer.idx)) {
    switch (timer.idx) {
      case TIMER_0: TG[timer.group]->int_clr_timers.t0 = 1; break;
      case TIMER_1: TG[timer.group]->int_clr_timers.t1 = 1; break;
      case TIMER_MAX: break;
    }
  }

  timer.fn();

  // After the alarm has been triggered
  // Enable it again so it gets triggered the next time
  TG[timer.group]->hw_timer[timer.idx].config.alarm_en = TIMER_ALARM_EN;
}

/**
 * Enable and initialize the timer
 * @param timer_num   timer number to initialize
 * @param frequency   frequency of the timer
 */
void HAL_timer_start(const uint8_t timer_num, const uint32_t frequency) {
  const tTimerConfig timer = timer_config[timer_num];

  timer_config_t config;
  config.divider     = timer.divider;
  config.counter_dir = TIMER_COUNT_UP;
  config.counter_en  = TIMER_PAUSE;
  config.alarm_en    = TIMER_ALARM_EN;
  config.intr_type   = TIMER_INTR_LEVEL;
  config.auto_reload = TIMER_AUTORELOAD_EN;

  // Select and initialize the timer
  timer_init(timer.group, timer.idx, &config);

  // Timer counter initial value and auto reload on alarm
  timer_set_counter_value(timer.group, timer.idx, 0x00000000ULL);

  // Configure the alam value and the interrupt on alarm
  timer_set_alarm_value(timer.group, timer.idx, (HAL_TIMER_RATE) / timer.divider / frequency - 1);

  timer_enable_intr(timer.group, timer.idx);

  timer_isr_register(timer.group, timer.idx, timer_isr, (void*)(uint32_t)timer_num, 0, nullptr);

  timer_start(timer.group, timer.idx);
}

/**
 * Set the upper value of the timer, when the timer reaches this upper value the
 * interrupt should be triggered and the counter reset
 * @param timer_num timer number to set the compare value to
 * @param compare   threshold at which the interrupt is triggered
 */
void HAL_timer_set_compare(const uint8_t timer_num, const hal_timer_t compare) {
  const tTimerConfig timer = timer_config[timer_num];
  timer_set_alarm_value(timer.group, timer.idx, compare);
}

/**
 * Get the current upper value of the timer
 * @param  timer_num timer number to get the count from
 * @return           the timer current threshold for the alarm to be triggered
 */
hal_timer_t HAL_timer_get_compare(const uint8_t timer_num) {
  const tTimerConfig timer = timer_config[timer_num];

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
  const tTimerConfig timer = timer_config[timer_num];
  uint64_t counter_value;
  timer_get_counter_value(timer.group, timer.idx, &counter_value);
  return counter_value;
}

/**
 * Stop the timer counting, so its interrupt no longer fires
 * @param timer_num timer number to pause
 */
void HAL_timer_pause(const uint8_t timer_num) {
  const tTimerConfig timer = timer_config[timer_num];
  timer_pause(timer.group, timer.idx);
}

/**
 * Resume a paused timer
 * @param timer_num timer number to resume
 */
void HAL_timer_resume(const uint8_t timer_num) {
  const tTimerConfig timer = timer_config[timer_num];
  timer_start(timer.group, timer.idx);
}

/**
 * Enable timer interrupt on the timer
 * @param timer_num timer number to enable interrupts on
 */
void HAL_timer_enable_interrupt(const uint8_t timer_num) {
  //const tTimerConfig timer = timer_config[timer_num];
  //timer_enable_intr(timer.group, timer.idx);
}

/**
 * Disable timer interrupt on the timer
 * @param timer_num timer number to disable interrupts on
 */
void HAL_timer_disable_interrupt(const uint8_t timer_num) {
  //const tTimerConfig timer = timer_config[timer_num];
  //timer_disable_intr(timer.group, timer.idx);
}

bool HAL_timer_interrupt_enabled(const uint8_t timer_num) {
  const tTimerConfig timer = timer_config[timer_num];
  return TG[timer.group]->int_ena.val | BIT(timer_num);
}

#endif // ESP_IDF_VERSION_MAJOR < 5

#endif // ARDUINO_ARCH_ESP32
