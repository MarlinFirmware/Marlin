/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 * Copyright (c) 2016 Bob Cousins bobcousins42@googlemail.com
 * Copyright (c) 2015-2016 Nico Tonnhofer wurstnase.reprap@gmail.com
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
 */
#pragma once

/**
 * Description: HAL for
 * Teensy4.0/4.1 (__IMXRT1062__)
 */

#include <stdint.h>

// ------------------------
// Defines
// ------------------------

#define FORCE_INLINE __attribute__((always_inline)) inline

typedef uint32_t hal_timer_t;
#define HAL_TIMER_TYPE_MAX 0xFFFFFFFF

#define TMR1_TIMER_PRESCALE 16
#define TMR2_TIMER_PRESCALE 8
#define TMR1_TIMER_PRESCALE_BITS 0b1100
#define TMR2_TIMER_PRESCALE_BITS 0b1011

#define TMR1_TIMER_RATE (F_BUS_ACTUAL / TMR1_TIMER_PRESCALE) // 132MHz / 16 = 8250kHz
#define TMR2_TIMER_RATE (F_BUS_ACTUAL / TMR2_TIMER_PRESCALE) // 132MHz / 8 = 16.5MHz

#define HAL_TIMER_RATE         (TMR1_TIMER_RATE)

#ifndef STEP_TIMER_NUM
  #define STEP_TIMER_NUM        0  // Timer Index for Stepper
#endif
#ifndef PULSE_TIMER_NUM
  #define PULSE_TIMER_NUM       STEP_TIMER_NUM
#endif
#ifndef TEMP_TIMER_NUM
  #define TEMP_TIMER_NUM        1  // Timer Index for Temperature
#endif

#define TEMP_TIMER_FREQUENCY    1000

#define STEPPER_TIMER_RATE     HAL_TIMER_RATE
#define STEPPER_TIMER_TICKS_PER_US ((STEPPER_TIMER_RATE) / 1000000)
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
  #define HAL_STEP_TIMER_ISR()  extern "C" void TMR1_isr() //void TC3_Handler()
#endif
#ifndef HAL_TEMP_TIMER_ISR
  #define HAL_TEMP_TIMER_ISR()  extern "C" void TMR2_isr() //void TC4_Handler()
#endif

void HAL_timer_start(const uint8_t timer_num, const uint32_t frequency);

FORCE_INLINE static void HAL_timer_set_compare(const uint8_t timer_num, const hal_timer_t compare) {
  switch (timer_num) {
    case 0: TMR1_COMP12 = compare; break;
    case 1: TMR2_COMP12 = compare; break;
  }
}

FORCE_INLINE static hal_timer_t HAL_timer_get_compare(const uint8_t timer_num) {
  switch (timer_num) {
    case 0: return TMR1_COMP12;
    case 1: return TMR2_COMP12;
  }
  return 0;
}

FORCE_INLINE static hal_timer_t HAL_timer_get_count(const uint8_t timer_num) {
  switch (timer_num) {
    case 0: return TMR1_CNTR2;
    case 1: return TMR2_CNTR2;
  }
  return 0;
}

void HAL_timer_enable_interrupt(const uint8_t timer_num);
void HAL_timer_disable_interrupt(const uint8_t timer_num);
bool HAL_timer_interrupt_enabled(const uint8_t timer_num);

void HAL_timer_isr_prologue(const uint8_t timer_num);
#define HAL_timer_isr_epilogue(TIMER_NUM)
