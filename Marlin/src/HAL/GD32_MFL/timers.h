/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2025 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#pragma once

#include "../../inc/MarlinConfig.h"

#include <GeneralTimer.h>

// ------------------------
// Defines
// ------------------------

// Timer configuration constants
#define STEPPER_TIMER_RATE    2000000
#define TEMP_TIMER_FREQUENCY  1000

// Timer instance definitions
#define MF_TIMER_STEP     3
#define MF_TIMER_TEMP     1
#define MF_TIMER_PULSE    MF_TIMER_STEP

#define hal_timer_t         uint32_t
#define HAL_TIMER_TYPE_MAX  UINT16_MAX

extern uint32_t GetStepperTimerClkFreq();

// Timer prescaler calculations
#define STEPPER_TIMER_PRESCALE      (GetStepperTimerClkFreq() / STEPPER_TIMER_RATE) // Prescaler = 30
#define PULSE_TIMER_PRESCALE        STEPPER_TIMER_PRESCALE
#define STEPPER_TIMER_TICKS_PER_US  ((STEPPER_TIMER_RATE) / 1000000)                // Stepper timer ticks per µs
#define PULSE_TIMER_RATE            STEPPER_TIMER_RATE
#define PULSE_TIMER_TICKS_PER_US    STEPPER_TIMER_TICKS_PER_US

// Timer interrupt priorities
#define STEP_TIMER_IRQ_PRIORITY 2
#define TEMP_TIMER_IRQ_PRIORITY 14

#define ENABLE_STEPPER_DRIVER_INTERRUPT()   HAL_timer_enable_interrupt(MF_TIMER_STEP)
#define DISABLE_STEPPER_DRIVER_INTERRUPT()  HAL_timer_disable_interrupt(MF_TIMER_STEP)
#define STEPPER_ISR_ENABLED()               HAL_timer_interrupt_enabled(MF_TIMER_STEP)
#define ENABLE_TEMPERATURE_INTERRUPT()      HAL_timer_enable_interrupt(MF_TIMER_TEMP)
#define DISABLE_TEMPERATURE_INTERRUPT()     HAL_timer_disable_interrupt(MF_TIMER_TEMP)

extern void Step_Handler();
extern void Temp_Handler();

#ifndef HAL_STEP_TIMER_ISR
  #define HAL_STEP_TIMER_ISR() void Step_Handler()
#endif
#ifndef HAL_TEMP_TIMER_ISR
  #define HAL_TEMP_TIMER_ISR() void Temp_Handler()
#endif

extern GeneralTimer& Step_Timer;
extern GeneralTimer& Temp_Timer;

extern bool is_step_timer_initialized;
extern bool is_temp_timer_initialized;

// Build-time mapping between timer base and index. Used in timers.cpp and fast_pwm.cpp
static inline constexpr struct {timer::TIMER_Base base; uint8_t timer_number;} base_to_index[] = {
  { timer::TIMER_Base::TIMER0_BASE, 0 },
  { timer::TIMER_Base::TIMER1_BASE, 1 },
  { timer::TIMER_Base::TIMER2_BASE, 2 },
  { timer::TIMER_Base::TIMER3_BASE, 3 },
  { timer::TIMER_Base::TIMER4_BASE, 4 },
  { timer::TIMER_Base::TIMER5_BASE, 5 },
  { timer::TIMER_Base::TIMER6_BASE, 6 },
  { timer::TIMER_Base::TIMER7_BASE, 7 }
};

// Converts a timer base to an integer timer index.
constexpr auto timer_base_to_index(timer::TIMER_Base base) -> int {
  for (const auto& timer : base_to_index) {
    if (timer.base == base) {
      return static_cast<int>(timer.timer_number);
    }
  }
  return -1;
}

// ------------------------
// Public functions
// ------------------------

void HAL_timer_start(const uint8_t timer, const uint32_t frequency);
void HAL_timer_enable_interrupt(const uint8_t timer);
void HAL_timer_disable_interrupt(const uint8_t timer);
bool HAL_timer_interrupt_enabled(const uint8_t timer);

// Configure timer priorities for peripherals such as Software Serial or Servos.
// Exposed here to allow all timer priority information to reside in timers.cpp
void SetTimerInterruptPriorities();

// FORCE_INLINE because these are used in performance-critical situations
FORCE_INLINE bool HAL_timer_initialized(const uint8_t timer_number) {
  return (timer_number == MF_TIMER_STEP) ? is_step_timer_initialized :
         (timer_number == MF_TIMER_TEMP) ? is_temp_timer_initialized :
         false;
}

FORCE_INLINE static hal_timer_t HAL_timer_get_count(const uint8_t timer_number) {
  if (!HAL_timer_initialized(timer_number)) return 0U;

  GeneralTimer& timer = (timer_number == MF_TIMER_STEP) ? Step_Timer : Temp_Timer;

  return (timer_number == MF_TIMER_STEP || timer_number == MF_TIMER_TEMP)
         ? timer.getCounter(TimerFormat::TICK)
         : 0U;
}

FORCE_INLINE static void HAL_timer_set_compare(const uint8_t timer_number, const hal_timer_t value) {
  if (!HAL_timer_initialized(timer_number)) return;

  const auto new_value = static_cast<uint32_t>(value + 1U);
  GeneralTimer& timer = (timer_number == MF_TIMER_STEP) ? Step_Timer : Temp_Timer;

  if (timer_number == MF_TIMER_STEP || timer_number == MF_TIMER_TEMP) {
    timer.setRolloverValue(new_value, TimerFormat::TICK);
    if (value < static_cast<hal_timer_t>(timer.getCounter(TimerFormat::TICK)))
      timer.refresh();
  }
}

#define HAL_timer_isr_prologue(T) NOOP
#define HAL_timer_isr_epilogue(T) NOOP
