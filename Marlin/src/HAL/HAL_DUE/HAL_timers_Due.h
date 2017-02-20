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

#define HAL_TIMER_TYPE uint32_t

#define STEP_TIMER_NUM 3  // index of timer to use for stepper
#define TEMP_TIMER_NUM 4  // index of timer to use for temperature

#define HAL_TIMER_RATE         ((F_CPU) / 2.0)  // frequency of timers peripherals
#define STEPPER_TIMER_PRESCALE 1.0              // prescaler for setting stepper frequency
#define HAL_STEPPER_TIMER_RATE HAL_TIMER_RATE   // frequency of stepper timer (HAL_TIMER_RATE / STEPPER_TIMER_PRESCALE)
#define HAL_TICKS_PER_US       ((HAL_STEPPER_TIMER_RATE) / 1000000) // stepper timer ticks per us

#define TEMP_TIMER_FREQUENCY   1000 // temperature interrupt frequency

#define ENABLE_STEPPER_DRIVER_INTERRUPT() HAL_timer_enable_interrupt (STEP_TIMER_NUM)
#define DISABLE_STEPPER_DRIVER_INTERRUPT()  HAL_timer_disable_interrupt (STEP_TIMER_NUM)

#define ENABLE_TEMPERATURE_INTERRUPT()  HAL_timer_enable_interrupt (TEMP_TIMER_NUM)
#define DISABLE_TEMPERATURE_INTERRUPT() HAL_timer_disable_interrupt (TEMP_TIMER_NUM)
//
#define HAL_STEP_TIMER_ISR  void TC3_Handler()
#define HAL_TEMP_TIMER_ISR  void TC4_Handler()

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

void HAL_timer_start (uint8_t timer_num, uint32_t frequency);

static FORCE_INLINE void HAL_timer_set_count (uint8_t timer_num, uint32_t count) {
  const tTimerConfig *pConfig = &TimerConfig[timer_num];
  pConfig->pTimerRegs->TC_CHANNEL[pConfig->channel].TC_RC = count;
}

static FORCE_INLINE HAL_TIMER_TYPE HAL_timer_get_count (uint8_t timer_num) {
  const tTimerConfig *pConfig = &TimerConfig[timer_num];
  return pConfig->pTimerRegs->TC_CHANNEL[pConfig->channel].TC_RC;
}

static FORCE_INLINE uint32_t HAL_timer_get_current_count(uint8_t timer_num) {
  const tTimerConfig *pConfig = &TimerConfig[timer_num];
  return pConfig->pTimerRegs->TC_CHANNEL[pConfig->channel].TC_CV;
}

void HAL_timer_enable_interrupt(uint8_t timer_num);
void HAL_timer_disable_interrupt(uint8_t timer_num);

//void HAL_timer_isr_prologue (uint8_t timer_num);

static FORCE_INLINE void HAL_timer_isr_prologue(uint8_t timer_num) {
  const tTimerConfig *pConfig = &TimerConfig[timer_num];
  // Reading the status register clears the interrupt flag
  pConfig->pTimerRegs->TC_CHANNEL[pConfig->channel].TC_SR;
}

#endif // _HAL_TIMERS_DUE_H
