/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 * Copyright (c) 2016 Bob Cousins bobcousins42@googlemail.com
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

/**
 * HAL timers for Linux X86_64
 */

#include <stdint.h>

// ------------------------
// Defines
// ------------------------

#define FORCE_INLINE __attribute__((always_inline)) inline

typedef uint64_t hal_timer_t;
#define HAL_TIMER_TYPE_MAX 0xFFFFFFFFFFFFFFFF

#define HAL_TIMER_RATE         ((SystemCoreClock) / 4)  // frequency of timers peripherals

#ifndef STEP_TIMER_NUM
  #define STEP_TIMER_NUM        0  // Timer Index for Stepper
#endif
#ifndef PULSE_TIMER_NUM
  #define PULSE_TIMER_NUM       STEP_TIMER_NUM
#endif
#ifndef TEMP_TIMER_NUM
  #define TEMP_TIMER_NUM        1  // Timer Index for Temperature
#endif
#ifndef SYSTICK_TIMER_NUM
  #define SYSTICK_TIMER_NUM     2 // Timer Index for Systick
#endif
#define SYSTICK_TIMER_FREQUENCY 1000

#define TEMP_TIMER_RATE        1000000
#define TEMP_TIMER_FREQUENCY   1000 // temperature interrupt frequency

#define STEPPER_TIMER_RATE     HAL_TIMER_RATE   // frequency of stepper timer (HAL_TIMER_RATE / STEPPER_TIMER_PRESCALE)
#define STEPPER_TIMER_TICKS_PER_US ((STEPPER_TIMER_RATE) / 1000000) // stepper timer ticks per µs
#define STEPPER_TIMER_PRESCALE (CYCLES_PER_MICROSECOND / STEPPER_TIMER_TICKS_PER_US)

#define PULSE_TIMER_RATE       STEPPER_TIMER_RATE   // frequency of pulse timer
#define PULSE_TIMER_PRESCALE   STEPPER_TIMER_PRESCALE
#define PULSE_TIMER_TICKS_PER_US STEPPER_TIMER_TICKS_PER_US

#define ENABLE_STEPPER_DRIVER_INTERRUPT() HAL_timer_enable_interrupt(STEP_TIMER_NUM)
#define DISABLE_STEPPER_DRIVER_INTERRUPT() HAL_timer_disable_interrupt(STEP_TIMER_NUM)
#define STEPPER_ISR_ENABLED() HAL_timer_interrupt_enabled(STEP_TIMER_NUM)

#define ENABLE_TEMPERATURE_INTERRUPT() HAL_timer_enable_interrupt(TEMP_TIMER_NUM)
#define DISABLE_TEMPERATURE_INTERRUPT() HAL_timer_disable_interrupt(TEMP_TIMER_NUM)

#ifndef HAL_STEP_TIMER_ISR
  #define HAL_STEP_TIMER_ISR()  extern "C" void TIMER0_IRQHandler()
#endif
#ifndef HAL_TEMP_TIMER_ISR
  #define HAL_TEMP_TIMER_ISR()  extern "C" void TIMER1_IRQHandler()
#endif

void HAL_timer_init();
void HAL_timer_start(const uint8_t timer_num, const uint32_t frequency);

void HAL_timer_set_compare(const uint8_t timer_num, const hal_timer_t compare);
hal_timer_t HAL_timer_get_compare(const uint8_t timer_num);
hal_timer_t HAL_timer_get_count(const uint8_t timer_num);

void HAL_timer_enable_interrupt(const uint8_t timer_num);
void HAL_timer_disable_interrupt(const uint8_t timer_num);
bool HAL_timer_interrupt_enabled(const uint8_t timer_num);

#define HAL_timer_isr_prologue(TIMER_NUM)
#define HAL_timer_isr_epilogue(TIMER_NUM)
