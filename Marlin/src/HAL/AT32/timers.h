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
#pragma once

#include <stdint.h>
#include <libcore/timer.h>

// AT32 timer peripheral type (from at32f403a_407.h via the Arduino core)
typedef tmr_type* timer_channel_t;
typedef uint16_t  hal_timer_t;
#define HAL_TIMER_TYPE_MAX hal_timer_t(UINT16_MAX)

//
// Timer Assignments
//   TMR2 → stepper/pulse  (APB1, 240 MHz timer clock)
//   TMR4 → temperature     (APB1, 240 MHz timer clock)
//
// AT32F403A timer register names (ref: CrealityOfficial/K2_Series_Klipper src/at32/):
//   tmr_x->div    prescaler        (STM32: PSC)
//   tmr_x->pr     auto-reload      (STM32: ARR)
//   tmr_x->cval   current count    (STM32: CNT)
//   tmr_x->c1dt   compare 1        (STM32: CCR1)
//   tmr_x->iden_bit.ovfien  overflow interrupt enable
//   tmr_x->ists_bit.ovfif   overflow interrupt flag (write 0 to clear)
//

#define MF_TIMER_TEMP          TMR4
#define MF_TIMER_STEP          TMR2
#define MF_TIMER_TEMP_IRQn     TMR4_GLOBAL_IRQn
#define MF_TIMER_STEP_IRQn     TMR2_GLOBAL_IRQn
#define TEMP_TIMER_PRIORITY    2
#define STEP_TIMER_PRIORITY    0

// AT32F403A: SYSCLK 240 MHz, APB1 120 MHz, timer clock = APB1×2 = 240 MHz
#define F_PCLK1                240000000UL

// HAL_TIMER_RATE must be compile-time for constexpr stepper math
#define HAL_TIMER_RATE         F_PCLK1

#define TEMP_TIMER_PRESCALE    240UL    // 240 MHz / 240 = 1 MHz tick
#define TEMP_TIMER_RATE        1000     // 1 kHz ISR
#define TEMP_TIMER_FREQUENCY   TEMP_TIMER_RATE

#define STEPPER_TIMER_PRESCALE 120UL   // 240 MHz / 120 = 2 MHz tick
#define STEPPER_TIMER_RATE     (HAL_TIMER_RATE / STEPPER_TIMER_PRESCALE)
#define STEPPER_TIMER_TICKS_PER_US ((STEPPER_TIMER_RATE) / 1000000UL)

#define MF_TIMER_PULSE         MF_TIMER_STEP
#define PULSE_TIMER_RATE       STEPPER_TIMER_RATE
#define PULSE_TIMER_PRESCALE   STEPPER_TIMER_PRESCALE

//
// HAL timer functions
//
void HAL_timer_start(const timer_channel_t timer_ch, const uint32_t frequency);

#define MARLIN_HAL_TIMER_INLINE_ATTR __attribute__((always_inline)) inline

MARLIN_HAL_TIMER_INLINE_ATTR void HAL_timer_enable_interrupt(const timer_channel_t timer_ch) {
  timer_ch->iden_bit.ovfien = 1;
}

MARLIN_HAL_TIMER_INLINE_ATTR void HAL_timer_disable_interrupt(const timer_channel_t timer_ch) {
  timer_ch->iden_bit.ovfien = 0;
}

MARLIN_HAL_TIMER_INLINE_ATTR bool HAL_timer_interrupt_enabled(const timer_channel_t timer_ch) {
  return timer_ch->iden_bit.ovfien;
}

MARLIN_HAL_TIMER_INLINE_ATTR void HAL_timer_set_compare(const timer_channel_t timer_ch, const hal_timer_t compare) {
  timer_ch->pr = compare;
}

MARLIN_HAL_TIMER_INLINE_ATTR hal_timer_t HAL_timer_get_count(const timer_channel_t timer_ch) {
  return (hal_timer_t)timer_ch->cval;
}

MARLIN_HAL_TIMER_INLINE_ATTR void HAL_timer_isr_prologue(const timer_channel_t timer_ch) {
  timer_ch->ists = 0; // Clear all timer status flags (rc_w0 — write 0 to clear)
}

inline void HAL_timer_isr_epilogue(const timer_channel_t) {}

#define ENABLE_STEPPER_DRIVER_INTERRUPT()   HAL_timer_enable_interrupt(MF_TIMER_STEP)
#define DISABLE_STEPPER_DRIVER_INTERRUPT()  HAL_timer_disable_interrupt(MF_TIMER_STEP)
#define STEPPER_ISR_ENABLED()               HAL_timer_interrupt_enabled(MF_TIMER_STEP)

#define ENABLE_TEMPERATURE_INTERRUPT()      HAL_timer_enable_interrupt(MF_TIMER_TEMP)
#define DISABLE_TEMPERATURE_INTERRUPT()     HAL_timer_disable_interrupt(MF_TIMER_TEMP)

//
// ISR callbacks (defined in Marlin's temperature.cpp / stepper.cpp)
//
void Step_Handler();
void Temp_Handler();

#ifndef HAL_STEP_TIMER_ISR
  #define HAL_STEP_TIMER_ISR() void Step_Handler()
#endif
#ifndef HAL_TEMP_TIMER_ISR
  #define HAL_TEMP_TIMER_ISR() void Temp_Handler()
#endif
