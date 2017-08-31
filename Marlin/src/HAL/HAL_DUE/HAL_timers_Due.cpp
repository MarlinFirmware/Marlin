/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * Description: HAL for Arduino Due and compatible (SAM3X8E)
 *
 * For ARDUINO_ARCH_SAM
 */

#ifdef ARDUINO_ARCH_SAM

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------

#include "../HAL.h"

#include "HAL_timers_Due.h"

// --------------------------------------------------------------------------
// Externals
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Local defines
// --------------------------------------------------------------------------

#define NUM_HARDWARE_TIMERS 9

#define PRESCALER 2
// --------------------------------------------------------------------------
// Types
// --------------------------------------------------------------------------


// --------------------------------------------------------------------------
// Public Variables
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Private Variables
// --------------------------------------------------------------------------

const tTimerConfig TimerConfig [NUM_HARDWARE_TIMERS] = {
  { TC0, 0, TC0_IRQn, 0},  // 0 - [servo timer5]
  { TC0, 1, TC1_IRQn, 0},  // 1
  { TC0, 2, TC2_IRQn, 0},  // 2
  { TC1, 0, TC3_IRQn, 2},  // 3 - stepper
  { TC1, 1, TC4_IRQn, 15}, // 4 - temperature
  { TC1, 2, TC5_IRQn, 0},  // 5 - [servo timer3]
  { TC2, 0, TC6_IRQn, 0},  // 6
  { TC2, 1, TC7_IRQn, 0},  // 7
  { TC2, 2, TC8_IRQn, 0},  // 8
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

/*
  Timer_clock1: Prescaler 2 -> 42MHz
  Timer_clock2: Prescaler 8 -> 10.5MHz
  Timer_clock3: Prescaler 32 -> 2.625MHz
  Timer_clock4: Prescaler 128 -> 656.25kHz
*/

void HAL_timer_start(const uint8_t timer_num, const uint32_t frequency) {
  Tc *tc = TimerConfig[timer_num].pTimerRegs;
  IRQn_Type irq = TimerConfig[timer_num].IRQ_Id;
  uint32_t channel = TimerConfig[timer_num].channel;

  pmc_set_writeprotect(false);
  pmc_enable_periph_clk((uint32_t)irq);
  NVIC_SetPriority(irq, TimerConfig [timer_num].priority);

  TC_Configure(tc, channel, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK1);

  TC_SetRC(tc, channel, VARIANT_MCK / 2 / frequency);
  TC_Start(tc, channel);

  // enable interrupt on RC compare
  tc->TC_CHANNEL[channel].TC_IER = TC_IER_CPCS;

  NVIC_EnableIRQ(irq);
}

void HAL_timer_enable_interrupt(const uint8_t timer_num) {
  const tTimerConfig *pConfig = &TimerConfig[timer_num];
  pConfig->pTimerRegs->TC_CHANNEL[pConfig->channel].TC_IER = TC_IER_CPCS;
}

void HAL_timer_disable_interrupt(const uint8_t timer_num) {
  const tTimerConfig *pConfig = &TimerConfig[timer_num];
  pConfig->pTimerRegs->TC_CHANNEL[pConfig->channel].TC_IDR = TC_IDR_CPCS;
}

#if 0
  void HAL_timer_set_count(const uint8_t timer_num, const uint32_t count) {
    const tTimerConfig *pConfig = &TimerConfig[timer_num];
    TC_SetRC(pConfig->pTimerRegs, pConfig->channel, count);
  }

  void HAL_timer_isr_prologue(const uint8_t timer_num) {
    const tTimerConfig *pConfig = &TimerConfig[timer_num];
    TC_GetStatus(pConfig->pTimerRegs, pConfig->channel);
  }
#endif

#endif // ARDUINO_ARCH_SAM
