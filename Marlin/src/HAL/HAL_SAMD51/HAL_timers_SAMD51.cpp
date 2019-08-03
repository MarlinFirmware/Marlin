/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 * SAMD51 HAL developed by Giuliano Zaro (AKA GMagician)
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

#ifdef __SAMD51__

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------
#include "../../inc/MarlinConfig.h"
#include "HAL_timers_SAMD51.h"

// --------------------------------------------------------------------------
// Local defines
// --------------------------------------------------------------------------

#define NUM_HARDWARE_TIMERS 8

// --------------------------------------------------------------------------
// Private Variables
// --------------------------------------------------------------------------

const tTimerConfig TimerConfig[NUM_HARDWARE_TIMERS] = {
  { TC0, TC0_IRQn, TC_PRIORITY(0) },
  { TC1, TC1_IRQn, TC_PRIORITY(1) },
  { TC2, TC2_IRQn, TC_PRIORITY(2) },  // Reserved by framework tone function
  { TC3, TC3_IRQn, TC_PRIORITY(3) },  // Reserved by servo library
  { TC4, TC4_IRQn, TC_PRIORITY(4) },
  { TC5, TC5_IRQn, TC_PRIORITY(5) },
  { TC6, TC6_IRQn, TC_PRIORITY(6) },
  { TC7, TC7_IRQn, TC_PRIORITY(7) }
};

// --------------------------------------------------------------------------
// Private functions
// --------------------------------------------------------------------------

FORCE_INLINE void Disable_Irq(IRQn_Type irq) {
  NVIC_DisableIRQ(irq);

  // We NEED memory barriers to ensure Interrupts are actually disabled!
  // ( https://dzone.com/articles/nvic-disabling-interrupts-on-arm-cortex-m-and-the )
  __DSB();
  __ISB();
}

// --------------------------------------------------------------------------
// Public functions
// --------------------------------------------------------------------------

void HAL_timer_start(const uint8_t timer_num, const uint32_t frequency) {
  Tc * const tc = TimerConfig[timer_num].pTimer;
  IRQn_Type irq = TimerConfig[timer_num].IRQ_Id;

  // Disable interrupt, just in case it was already enabled
  Disable_Irq(irq);

  // Disable timer interrupt
  tc->COUNT32.INTENCLR.reg = TC_INTENCLR_OVF; // disable overflow interrupt

  // TCn clock setup
  const uint8_t clockID = GCLK_CLKCTRL_IDs[TCC_INST_NUM + timer_num];
  GCLK->PCHCTRL[clockID].bit.CHEN = false;
  SYNC(GCLK->PCHCTRL[clockID].bit.CHEN);
  GCLK->PCHCTRL[clockID].reg = GCLK_PCHCTRL_GEN_GCLK0 | GCLK_PCHCTRL_CHEN;   // 120MHz startup code programmed
  SYNC(!GCLK->PCHCTRL[clockID].bit.CHEN);

  // Stop timer, just in case, to be able to reconfigure it
  tc->COUNT32.CTRLA.bit.ENABLE = false;
  SYNC(tc->COUNT32.SYNCBUSY.bit.ENABLE);

  // Reset timer
  tc->COUNT32.CTRLA.bit.SWRST = true;
  SYNC(tc->COUNT32.SYNCBUSY.bit.SWRST);

  NVIC_SetPriority(irq, TimerConfig[timer_num].priority);

  // Wave mode, reset counter on overflow on 0 (I use count down to prevent double buffer use)
  tc->COUNT32.WAVE.reg = TC_WAVE_WAVEGEN_MFRQ;
  tc->COUNT32.CTRLA.reg = TC_CTRLA_MODE_COUNT32 | TC_CTRLA_PRESCALER_DIV1;
  tc->COUNT32.CTRLBSET.reg = TC_CTRLBCLR_DIR;
  SYNC(tc->COUNT32.SYNCBUSY.bit.CTRLB);

  // Set compare value
  tc->COUNT32.COUNT.reg = tc->COUNT32.CC[0].reg = HAL_TIMER_RATE / frequency;

  // And start timer
  tc->COUNT32.CTRLA.bit.ENABLE = true;
  SYNC(tc->COUNT32.SYNCBUSY.bit.ENABLE);

  // Enable interrupt on RC compare
  tc->COUNT32.INTENSET.reg = TC_INTENCLR_OVF; // enable overflow interrupt

  // Finally, enable IRQ
  NVIC_EnableIRQ(irq);
}

void HAL_timer_enable_interrupt(const uint8_t timer_num) {
  IRQn_Type irq = TimerConfig[timer_num].IRQ_Id;
  NVIC_EnableIRQ(irq);
}

void HAL_timer_disable_interrupt(const uint8_t timer_num) {
  IRQn_Type irq = TimerConfig[timer_num].IRQ_Id;
  Disable_Irq(irq);
}

// missing from CMSIS: Check if interrupt is enabled or not
static bool NVIC_GetEnabledIRQ(IRQn_Type IRQn) {
  return (NVIC->ISER[(uint32_t)(IRQn) >> 5] & (1 << ((uint32_t)(IRQn) & 0x1F))) != 0;
}

bool HAL_timer_interrupt_enabled(const uint8_t timer_num) {
  IRQn_Type irq = TimerConfig[timer_num].IRQ_Id;
  return NVIC_GetEnabledIRQ(irq);
}

#endif // __SAMD51__
