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

#include <stdio.h>
#include <esp_types.h>
#include <soc/timer_group_struct.h>
#include <driver/periph_ctrl.h>
#include <driver/timer.h>

#include "../../inc/MarlinConfig.h"

#if MB(MKS_TINYBEE)

  #if __has_include(<esp_arduino_version.h>)
    #include <esp_arduino_version.h>   // Only exists from arduino-esp32 2.0.0
  #endif

  /**
   * MKS TinyBee: with I2S_STEPPER_STREAM the step timer is read but never started.
   *
   * Stepping is produced by the I2S DMA stream, so Stepper::init() skips the only
   * HAL_timer_start() the step timer ever gets:
   *
   *   Marlin/src/module/stepper.cpp:3258
   *     #if DISABLED(I2S_STEPPER_STREAM)
   *       HAL_timer_start(MF_TIMER_STEP, 122);
   *
   * Nothing stopped reading it though. Marlin 2.1.x replaced the multistepping
   * heuristic with one that measures how long the ISR takes, and put the first
   * measurement at the top of block_phase_isr():
   *
   *   Marlin/src/module/stepper.cpp:2397
   *     hal_timer_t Stepper::block_phase_isr() {
   *       #if DISABLED(OLD_ADAPTIVE_MULTISTEPPING)
   *         const hal_timer_t time_spent = HAL_timer_get_count(MF_TIMER_STEP);
   *
   * In I2S mode block_phase_isr() is not driven by that timer at all, it is called
   * from stepperTask (HAL/ESP32/i2s.cpp:171) which loops over the DMA buffer
   * forever, so this read happens continuously from boot whether the printer moves
   * or not. Marlin 2.0.9.2 (the MKS fork) has no such read - block_phase_isr()
   * starts straight at "uint32_t interval" - which is why this is a 2.1.x
   * regression and not something the MKS firmware ever hit.
   *
   * START_TIMED_PULSE() (stepper.cpp:524, used from pulse_phase_isr() whenever
   * ISR_PULSE_CONTROL is on) reads the same timer, so movement adds a second
   * source once the machine is actually stepping.
   *
   * Why it only became visible on espressif32@6.1.0:
   *
   *  - arduino-esp32 1.0.x (ESP-IDF 3.3): timer_get_counter_value() just reads the
   *    register. A never-started timer reads 0 and nobody complains.
   *
   *  - arduino-esp32 2.0.x (ESP-IDF 4.4): the driver keeps a per-timer object that
   *    timer_init() allocates, and every entry point asserts on it first:
   *      TIMER_CHECK(p_timer_obj[group_num][timer_num] != NULL, TIMER_NEVER_INIT_ERROR, ESP_ERR_INVALID_ARG);
   *    So the call logs "E (...) timer_group: timer_get_counter_value(54): HW
   *    TIMER NEVER INIT ERROR", returns ESP_ERR_INVALID_ARG, and leaves the output
   *    parameter untouched. Called in a tight loop that is one error line per
   *    iteration; at 115200 baud a ~75 character line takes ~6.5 ms, which matches
   *    the observed ~7 ms spacing exactly. Because uartWriteBuf() busy-waits on
   *    the TX FIFO, the log traffic starves the loopTask and the host connection
   *    dies.
   *
   * The repair is to give the step timer a real counter. In I2S mode the timer
   * hardware (TIMERG0 timer 0) is otherwise completely idle, so it is started here
   * as a plain free-running counter - no alarm, no interrupt, no ISR registered -
   * ticking at STEPPER_TIMER_RATE, which is the unit every caller already assumes
   * (calc_timer_interval() returns STEPPER_TIMER_RATE/step_rate, ns_to_pulse_timer_ticks()
   * divides by 1e9/STEPPER_TIMER_RATE). HAL_timer_get_count() then means what its
   * name says instead of erroring out.
   *
   * Returning a constant instead would have been shorter and is wrong: AWAIT_TIMED_PULSE()
   * (stepper.cpp:525) spins on "PULSE_x_TICK_COUNT > count - start_pulse_count", so
   * a value that never advances is an infinite loop in pulse_phase_isr() as soon as
   * PULSE_HIGH_TICK_COUNT is non-zero. It happens to be 0 for TMC2209_STANDALONE
   * (MINIMUM_STEPPER_PULSE_NS 100 is below the 4 µs tick), but that is a property
   * of this config, not something to build a HAL on.
   */

  // arduino-esp32 1.0.x has no esp_arduino_version.h, hence no version macros.
  #ifdef ESP_ARDUINO_VERSION_MAJOR
    #define MARLIN_ESP32_CORE_MAJOR ESP_ARDUINO_VERSION_MAJOR
  #else
    #define MARLIN_ESP32_CORE_MAJOR 1
  #endif

  #if ENABLED(I2S_STEPPER_STREAM) && DISABLED(MARLIN_ESP32_KEEP_STOCK_STEP_TIMER)

    #define MARLIN_ESP32_I2S_STEP_COUNTER 1

    #if MARLIN_ESP32_CORE_MAJOR > 2
      // Never let this fail quietly: a HAL that silently reads a dead timer is the
      // exact bug this file exists to fix. ESP-IDF 5.x deprecated the legacy
      // driver/timer.h API this file (and stock Marlin) is written against, so the
      // free-running-counter setup has to be re-checked against gptimer first.
      #error "Untested arduino-esp32 major version. Verify that driver/timer.h still starts a counter without an alarm, then extend timers.cpp. Define MARLIN_ESP32_KEEP_STOCK_STEP_TIMER to build without the fix (expect a HW TIMER NEVER INIT ERROR flood on the serial port)."
    #endif

    #pragma message "TinyBee: I2S step timer free-running counter active"

  #endif

#endif // MARLIN_ESP32_TINYBEE_FIXES

// ------------------------
// Local defines
// ------------------------

#define NUM_HARDWARE_TIMERS 4

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

#if MB(MKS_TINYBEE)

  /**
   * Timers that have been through timer_init(), i.e. the ones the ESP-IDF driver
   * will accept. MF_TIMER_PWM is only started when the first PWM pin is attached
   * (HAL.cpp:398) and MF_TIMER_TONE only while a tone plays (Tone.cpp:41), so this
   * is not exclusively about the step timer.
   */
  static uint8_t timer_started = 0;

  #define TIMER_IS_STARTED(N) TEST(timer_started, N)

#endif

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

#if ENABLED(MARLIN_ESP32_I2S_STEP_COUNTER)

  /**
   * A tick of the step timer is 1/STEPPER_TIMER_RATE of a second to every caller,
   * so that is what the counter has to run at. STEPPER_TIMER_PRESCALE is 1 under
   * I2S_STEPPER_STREAM (timers.h:54) and describes the I2S word clock, not a
   * timer divider, so it cannot be reused here.
   */
  constexpr uint32_t step_counter_divider = (HAL_TIMER_RATE) / (STEPPER_TIMER_RATE);

  static_assert(step_counter_divider * uint32_t(STEPPER_TIMER_RATE) == uint32_t(HAL_TIMER_RATE),
    "STEPPER_TIMER_RATE must divide HAL_TIMER_RATE exactly, or the step counter drifts against the tick unit every caller assumes.");
  static_assert(step_counter_divider >= 2 && step_counter_divider <= 65536,
    "The step counter divider is outside the ESP32 timer's 2..65536 range.");

  /**
   * The pulse-timing macros read MF_TIMER_PULSE and everything else reads
   * MF_TIMER_STEP. Only the latter is given a counter below, so a build that
   * splits the two would put the pulse waits back on a dead timer.
   */
  static_assert(MF_TIMER_PULSE == MF_TIMER_STEP,
    "MF_TIMER_PULSE is no longer MF_TIMER_STEP. Give MF_TIMER_PULSE a counter too before building this.");

  static void start_step_counter() {
    const tTimerConfig timer = timer_config[MF_TIMER_STEP];

    // Zero-initialized on purpose: stock HAL_timer_start() leaves any member it
    // does not know about (clk_src on the SoCs that have it) holding stack junk.
    timer_config_t config = {};
    config.divider     = step_counter_divider;
    config.counter_dir = TIMER_COUNT_UP;
    config.counter_en  = TIMER_PAUSE;
    config.alarm_en    = TIMER_ALARM_DIS;    // No alarm and no timer_isr_register():
    config.intr_type   = TIMER_INTR_LEVEL;   // stepTC_Handler() must never run in I2S mode,
    config.auto_reload = TIMER_AUTORELOAD_DIS;  // the DMA stream owns the stepping.

    timer_init(timer.group, timer.idx, &config);
    timer_set_counter_value(timer.group, timer.idx, 0x00000000ULL);
    timer_start(timer.group, timer.idx);
  }

  /**
   * stepperTask reaches this within microseconds of i2s_init(), long before
   * anything else reads the step timer, so in practice the first call is alone
   * here. The flag is still raised before the setup rather than after: a second
   * caller then reads a counter that is mid-configuration (0, or a value that
   * jumps once) instead of running timer_init() a second time. Both callers only
   * ever use differences of this value, and both tolerate a single jump.
   */
  static void ensure_step_counter() {
    if (TIMER_IS_STARTED(MF_TIMER_STEP)) return;
    SBI(timer_started, MF_TIMER_STEP);
    start_step_counter();
  }

#endif // MARLIN_ESP32_I2S_STEP_COUNTER

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

  #if MB(MKS_TINYBEE)
    SBI(timer_started, timer_num);
  #endif
}

/**
 * Set the upper value of the timer, when the timer reaches this upper value the
 * interrupt should be triggered and the counter reset
 * @param timer_num timer number to set the compare value to
 * @param compare   threshold at which the interrupt is triggered
 */
void HAL_timer_set_compare(const uint8_t timer_num, const hal_timer_t compare) {
  const tTimerConfig timer = timer_config[timer_num];
  #if MB(MKS_TINYBEE)
    if (!TIMER_IS_STARTED(timer_num)) return;
  #endif
  timer_set_alarm_value(timer.group, timer.idx, compare);
}

/**
 * Get the current upper value of the timer
 * @param  timer_num timer number to get the count from
 * @return           the timer current threshold for the alarm to be triggered
 */
hal_timer_t HAL_timer_get_compare(const uint8_t timer_num) {
  const tTimerConfig timer = timer_config[timer_num];

  #if MB(MKS_TINYBEE)
    // Zeroed because the driver leaves it untouched when it rejects the call, and
    // stock returns that indeterminate value.
    uint64_t alarm_value = 0;
    if (TIMER_IS_STARTED(timer_num))
      timer_get_alarm_value(timer.group, timer.idx, &alarm_value);
  #else
    uint64_t alarm_value;
    timer_get_alarm_value(timer.group, timer.idx, &alarm_value);
  #endif

  return alarm_value;
}

/**
 * Get the current counter value between 0 and the maximum count (HAL_timer_set_count)
 * @param  timer_num timer number to get the current count
 * @return           the current counter of the alarm
 */
hal_timer_t HAL_timer_get_count(const uint8_t timer_num) {
  const tTimerConfig timer = timer_config[timer_num];

  #if ENABLED(MARLIN_ESP32_I2S_STEP_COUNTER)
    if (timer_num == MF_TIMER_STEP) ensure_step_counter();
  #endif

  #if MB(MKS_TINYBEE)
    uint64_t counter_value = 0;
    if (TIMER_IS_STARTED(timer_num))
      timer_get_counter_value(timer.group, timer.idx, &counter_value);
  #else
    uint64_t counter_value;
    timer_get_counter_value(timer.group, timer.idx, &counter_value);
  #endif

  return counter_value;
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

#endif // ARDUINO_ARCH_ESP32
