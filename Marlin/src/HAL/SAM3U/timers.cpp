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

/**
 * HAL Timers for Atmel SAM3U (AT91SAM3U4E)
 */

#ifdef __SAM3U4E__

#include "../../inc/MarlinConfig.h"
#include "HAL.h"

// ------------------------
// Local defines
// ------------------------

#define NUM_HARDWARE_TIMERS 3

// ------------------------
// Private Variables
// ------------------------

/**
 * The SAM3U's one TC block gives each of its three channels its own peripheral
 * ID and its own NVIC line, so ID_TCn == TCn_IRQn == channel n.
 *
 * Priorities: the stepper must pre-empt everything, temperature can wait.
 */
const tTimerConfig timer_config[NUM_HARDWARE_TIMERS] = {
  { 0, TC0_IRQn,  2 }, // 0 - stepper
  { 1, TC1_IRQn, 15 }, // 1 - temperature
  { 2, TC2_IRQn, 14 }, // 2 - tone
};

// ------------------------
// Public functions
// ------------------------

/**
 * TC channel clock sources (MCK = 96MHz):
 *   TIMER_CLOCK1: MCK/2   -> 48MHz
 *   TIMER_CLOCK2: MCK/8   -> 12MHz
 *   TIMER_CLOCK3: MCK/32  ->  3MHz  <- HAL_TIMER_PRESCALER
 *   TIMER_CLOCK4: MCK/128 -> 750kHz
 */
void HAL_timer_start(const uint8_t timer_num, const uint32_t frequency) {
  const tTimerConfig * const pConfig = &timer_config[timer_num];
  const uint8_t channel = pConfig->channel;
  const IRQn_Type irq = pConfig->IRQ_Id;

  // Disable the interrupt in case it was already running
  NVIC_DisableIRQ(irq);

  // Memory barriers to be sure the NVIC really is quiet before we reconfigure
  // ( https://dzone.com/articles/nvic-disabling-interrupts-on-arm-cortex-m-and-the )
  __DSB();
  __ISB();

  TC0->TC_CHANNEL[channel].TC_IDR = TC_IDR_CPCS;

  // Stop the channel while it is reprogrammed
  TC0->TC_CHANNEL[channel].TC_CCR = TC_CCR_CLKDIS;

  // Each TC channel has its own peripheral clock, and ID_TCn == channel n
  pmc_enable_periph_clk(ID_TC0 + channel);

  NVIC_SetPriority(irq, pConfig->priority);

  // Waveform mode, counter resets on RC compare
  TC0->TC_CHANNEL[channel].TC_CMR = TC_CMR_WAVE
                                  | TC_CMR_WAVSEL_UP_RC
                                  | TC_CMR_TCCLKS_TIMER_CLOCK3;   // MCK/32

  // Compare value. Clamped because the counter is only 16 bits - a frequency
  // below HAL_TIMER_RATE / 65536 (~46Hz) cannot be represented.
  const uint32_t compare = (HAL_TIMER_RATE) / frequency;
  TC0->TC_CHANNEL[channel].TC_RC = _MIN(compare, uint32_t(HAL_TIMER_TYPE_MAX));

  // Start the channel
  TC0->TC_CHANNEL[channel].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;

  // Interrupt on RC compare
  TC0->TC_CHANNEL[channel].TC_IER = TC_IER_CPCS;

  NVIC_EnableIRQ(irq);
}

void HAL_timer_enable_interrupt(const uint8_t timer_num) {
  NVIC_EnableIRQ(timer_config[timer_num].IRQ_Id);
}

void HAL_timer_disable_interrupt(const uint8_t timer_num) {
  NVIC_DisableIRQ(timer_config[timer_num].IRQ_Id);

  // We NEED memory barriers to ensure interrupts are actually disabled!
  __DSB();
  __ISB();
}

// Missing from CMSIS: check whether an interrupt is enabled
static bool NVIC_GetEnabledIRQ(const IRQn_Type IRQn) {
  return TEST(NVIC->ISER[uint32_t(IRQn) >> 5], uint32_t(IRQn) & 0x1F);
}

bool HAL_timer_interrupt_enabled(const uint8_t timer_num) {
  return NVIC_GetEnabledIRQ(timer_config[timer_num].IRQ_Id);
}

#endif // __SAM3U4E__
