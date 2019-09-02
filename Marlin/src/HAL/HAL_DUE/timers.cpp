/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

// ------------------------
// Includes
// ------------------------
#include "../../inc/MarlinConfig.h"
#include "HAL.h"

#include "timers.h"

// ------------------------
// Local defines
// ------------------------

#define NUM_HARDWARE_TIMERS 9

// ------------------------
// Private Variables
// ------------------------

const tTimerConfig TimerConfig [NUM_HARDWARE_TIMERS] = {
  { TC0, 0, TC0_IRQn,  3}, // 0 - [servo timer5]
  { TC0, 1, TC1_IRQn,  0}, // 1
  { TC0, 2, TC2_IRQn,  2}, // 2 - stepper
  { TC1, 0, TC3_IRQn,  0}, // 3
  { TC1, 1, TC4_IRQn, 15}, // 4 - temperature
  { TC1, 2, TC5_IRQn,  3}, // 5 - [servo timer3]
  { TC2, 0, TC6_IRQn, 14}, // 6 - tone
  { TC2, 1, TC7_IRQn,  0}, // 7
  { TC2, 2, TC8_IRQn,  0}, // 8
};

// ------------------------
// Public functions
// ------------------------

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

  // Disable interrupt, just in case it was already enabled
  NVIC_DisableIRQ(irq);

  // We NEED memory barriers to ensure Interrupts are actually disabled!
  // ( https://dzone.com/articles/nvic-disabling-interrupts-on-arm-cortex-m-and-the )
  __DSB();
  __ISB();

  // Disable timer interrupt
  tc->TC_CHANNEL[channel].TC_IDR = TC_IDR_CPCS;

  // Stop timer, just in case, to be able to reconfigure it
  TC_Stop(tc, channel);

  pmc_set_writeprotect(false);
  pmc_enable_periph_clk((uint32_t)irq);
  NVIC_SetPriority(irq, TimerConfig [timer_num].priority);

  // wave mode, reset counter on match with RC,
  TC_Configure(tc, channel, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK1);

  // Set compare value
  TC_SetRC(tc, channel, VARIANT_MCK / 2 / frequency);

  // And start timer
  TC_Start(tc, channel);

  // enable interrupt on RC compare
  tc->TC_CHANNEL[channel].TC_IER = TC_IER_CPCS;

  // Finally, enable IRQ
  NVIC_EnableIRQ(irq);
}

void HAL_timer_enable_interrupt(const uint8_t timer_num) {
  IRQn_Type irq = TimerConfig[timer_num].IRQ_Id;
  NVIC_EnableIRQ(irq);
}

void HAL_timer_disable_interrupt(const uint8_t timer_num) {
  IRQn_Type irq = TimerConfig[timer_num].IRQ_Id;
  NVIC_DisableIRQ(irq);

  // We NEED memory barriers to ensure Interrupts are actually disabled!
  // ( https://dzone.com/articles/nvic-disabling-interrupts-on-arm-cortex-m-and-the )
  __DSB();
  __ISB();
}

// missing from CMSIS: Check if interrupt is enabled or not
static bool NVIC_GetEnabledIRQ(IRQn_Type IRQn) {
  return (NVIC->ISER[(uint32_t)(IRQn) >> 5] & (1 << ((uint32_t)(IRQn) & 0x1F))) != 0;
}

bool HAL_timer_interrupt_enabled(const uint8_t timer_num) {
  IRQn_Type irq = TimerConfig[timer_num].IRQ_Id;
  return NVIC_GetEnabledIRQ(irq);
}

#endif // ARDUINO_ARCH_SAM
