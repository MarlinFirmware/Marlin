/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * HAL for Arduino Due and compatible (SAM3X8E)
 *
 * For ARDUINO_ARCH_SAM
 */

#ifndef _HAL_TIMERS_DUE_H
#define _HAL_TIMERS_DUE_H

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------

#include <stdint.h>

// --------------------------------------------------------------------------
// Defines
// --------------------------------------------------------------------------

#define FORCE_INLINE __attribute__((always_inline)) inline

typedef uint32_t hal_timer_t;
#define HAL_TIMER_TYPE_MAX 0xFFFFFFFF

#define STEP_TIMER_NUM 3  // index of timer to use for stepper
#define TEMP_TIMER_NUM 4  // index of timer to use for temperature
#define TONE_TIMER_NUM 6  // index of timer to use for beeper tones

#define HAL_TIMER_RATE         ((F_CPU) / 2)    // frequency of timers peripherals
#define STEPPER_TIMER_PRESCALE (CYCLES_PER_MICROSECOND / HAL_TICKS_PER_US)
#define HAL_STEPPER_TIMER_RATE HAL_TIMER_RATE   // frequency of stepper timer (HAL_TIMER_RATE / STEPPER_TIMER_PRESCALE)
#define HAL_TICKS_PER_US       ((HAL_STEPPER_TIMER_RATE) / 1000000) // stepper timer ticks per µs

#define TEMP_TIMER_FREQUENCY   1000 // temperature interrupt frequency
#define STEP_TIMER_MIN_INTERVAL   8 // minimum time in µs between stepper interrupts

#define ENABLE_STEPPER_DRIVER_INTERRUPT() HAL_timer_enable_interrupt(STEP_TIMER_NUM)
#define DISABLE_STEPPER_DRIVER_INTERRUPT()  HAL_timer_disable_interrupt(STEP_TIMER_NUM)
#define STEPPER_ISR_ENABLED() HAL_timer_interrupt_enabled(STEP_TIMER_NUM)

#define ENABLE_TEMPERATURE_INTERRUPT()  HAL_timer_enable_interrupt(TEMP_TIMER_NUM)
#define DISABLE_TEMPERATURE_INTERRUPT() HAL_timer_disable_interrupt(TEMP_TIMER_NUM)

#define HAL_ENABLE_ISRs() do { if (thermalManager.in_temp_isr) DISABLE_TEMPERATURE_INTERRUPT(); else ENABLE_TEMPERATURE_INTERRUPT(); ENABLE_STEPPER_DRIVER_INTERRUPT(); } while(0)

#define HAL_STEP_TIMER_ISR  void TC3_Handler()
#define HAL_TEMP_TIMER_ISR  void TC4_Handler()
#define HAL_TONE_TIMER_ISR  void TC6_Handler()

#define PULSE_TIMER_NUM STEP_TIMER_NUM
#define PULSE_TIMER_PRESCALE STEPPER_TIMER_PRESCALE

// --------------------------------------------------------------------------
// Types
// --------------------------------------------------------------------------

typedef struct {
  Tc          *pTimerRegs;
  uint16_t    channel;
  IRQn_Type   IRQ_Id;
  uint8_t     priority;
} tTimerConfig;

// --------------------------------------------------------------------------
// Public Variables
// --------------------------------------------------------------------------

extern const tTimerConfig TimerConfig[];

// --------------------------------------------------------------------------
// Public functions
// --------------------------------------------------------------------------

void HAL_timer_start(const uint8_t timer_num, const uint32_t frequency);

FORCE_INLINE static void HAL_timer_set_compare(const uint8_t timer_num, const hal_timer_t compare) {
  const tTimerConfig * const pConfig = &TimerConfig[timer_num];
  pConfig->pTimerRegs->TC_CHANNEL[pConfig->channel].TC_RC = compare;
}

FORCE_INLINE static hal_timer_t HAL_timer_get_compare(const uint8_t timer_num) {
  const tTimerConfig * const pConfig = &TimerConfig[timer_num];
  return pConfig->pTimerRegs->TC_CHANNEL[pConfig->channel].TC_RC;
}

FORCE_INLINE static hal_timer_t HAL_timer_get_count(const uint8_t timer_num) {
  const tTimerConfig * const pConfig = &TimerConfig[timer_num];
  return pConfig->pTimerRegs->TC_CHANNEL[pConfig->channel].TC_CV;
}

// if counter too high then bump up compare
FORCE_INLINE static void HAL_timer_restrain(const uint8_t timer_num, const uint16_t interval_ticks) {
  const hal_timer_t mincmp = HAL_timer_get_count(timer_num) + interval_ticks;
  if (HAL_timer_get_compare(timer_num) < mincmp) HAL_timer_set_compare(timer_num, mincmp);
}

void HAL_timer_enable_interrupt(const uint8_t timer_num);
void HAL_timer_disable_interrupt(const uint8_t timer_num);
bool HAL_timer_interrupt_enabled(const uint8_t timer_num);

//void HAL_timer_isr_prologue(const uint8_t timer_num);

FORCE_INLINE static void HAL_timer_isr_prologue(const uint8_t timer_num) {
  const tTimerConfig * const pConfig = &TimerConfig[timer_num];
  // Reading the status register clears the interrupt flag
  pConfig->pTimerRegs->TC_CHANNEL[pConfig->channel].TC_SR;
}

#endif // _HAL_TIMERS_DUE_H
