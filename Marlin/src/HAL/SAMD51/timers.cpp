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
 * SAMD51 HAL developed by Giuliano Zaro (AKA GMagician)
 */
#ifdef __SAMD51__

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------

#include "../../inc/MarlinConfig.h"
#include "ServoTimers.h" // for SERVO_TC

// --------------------------------------------------------------------------
// Local defines
// --------------------------------------------------------------------------

#define NUM_HARDWARE_TIMERS 9

// --------------------------------------------------------------------------
// Private Variables
// --------------------------------------------------------------------------

const tTimerConfig timer_config[NUM_HARDWARE_TIMERS] = {
  { {.pTc=TC0},  TC0_IRQn, TC_PRIORITY(0) },  // 0 - stepper (assigned priority 2)
  { {.pTc=TC1},  TC1_IRQn, TC_PRIORITY(1) },  // 1 - stepper (needed by 32 bit timers)
  { {.pTc=TC2},  TC2_IRQn, 5              },  // 2 - tone (reserved by framework and fixed assigned priority 5)
  { {.pTc=TC3},  TC3_IRQn, TC_PRIORITY(3) },  // 3 - servo (assigned priority 1)
  { {.pTc=TC4},  TC4_IRQn, TC_PRIORITY(4) },  // 4 - software serial (no interrupts used)
  { {.pTc=TC5},  TC5_IRQn, TC_PRIORITY(5) },
  { {.pTc=TC6},  TC6_IRQn, TC_PRIORITY(6) },
  { {.pTc=TC7},  TC7_IRQn, TC_PRIORITY(7) },
  { {.pRtc=RTC}, RTC_IRQn, TC_PRIORITY(8) }   // 8 - temperature (assigned priority 6)
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
  IRQn_Type irq = timer_config[timer_num].IRQ_Id;

  // Disable interrupt, just in case it was already enabled
  Disable_Irq(irq);

  if (timer_num == MF_TIMER_RTC) {
    Rtc * const rtc = timer_config[timer_num].pRtc;

    // Disable timer interrupt
    rtc->MODE0.INTENCLR.reg = RTC_MODE0_INTENCLR_CMP0;

    // RTC clock setup
    OSC32KCTRL->RTCCTRL.reg = OSC32KCTRL_RTCCTRL_RTCSEL_XOSC32K;  // External 32.768kHz oscillator

    // Stop timer, just in case, to be able to reconfigure it
    rtc->MODE0.CTRLA.bit.ENABLE = false;
    SYNC(rtc->MODE0.SYNCBUSY.bit.ENABLE);

    // Mode, reset counter on match
    rtc->MODE0.CTRLA.reg = RTC_MODE0_CTRLA_MODE_COUNT32 | RTC_MODE0_CTRLA_MATCHCLR;

    // Set compare value
    rtc->MODE0.COMP[0].reg = (32768 + frequency / 2) / frequency;
    SYNC(rtc->MODE0.SYNCBUSY.bit.COMP0);

    // Enable interrupt on compare
    rtc->MODE0.INTFLAG.reg = RTC_MODE0_INTFLAG_CMP0;    // reset pending interrupt
    rtc->MODE0.INTENSET.reg = RTC_MODE0_INTENSET_CMP0;  // enable compare 0 interrupt

    // And start timer
    rtc->MODE0.CTRLA.bit.ENABLE = true;
    SYNC(rtc->MODE0.SYNCBUSY.bit.ENABLE);
  }
  else {
    Tc * const tc = timer_config[timer_num].pTc;

    // Disable timer interrupt
    tc->COUNT32.INTENCLR.reg = TC_INTENCLR_OVF; // disable overflow interrupt

    // TCn clock setup
    const uint8_t clockID = GCLK_CLKCTRL_IDs[TCC_INST_NUM + timer_num];   // TC clock are preceded by TCC ones
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

    // Wave mode, reset counter on compare match
    tc->COUNT32.WAVE.reg = TC_WAVE_WAVEGEN_MFRQ;
    tc->COUNT32.CTRLA.reg = TC_CTRLA_MODE_COUNT32 | TC_CTRLA_PRESCALER_DIV1;
    tc->COUNT32.CTRLBCLR.reg = TC_CTRLBCLR_DIR;
    SYNC(tc->COUNT32.SYNCBUSY.bit.CTRLB);

    // Set compare value
    tc->COUNT32.CC[0].reg = (HAL_TIMER_RATE) / frequency;
    tc->COUNT32.COUNT.reg = 0;

    // Enable interrupt on compare
    tc->COUNT32.INTFLAG.reg = TC_INTFLAG_OVF;   // reset pending interrupt
    tc->COUNT32.INTENSET.reg = TC_INTENSET_OVF; // enable overflow interrupt

    // And start timer
    tc->COUNT32.CTRLA.bit.ENABLE = true;
    SYNC(tc->COUNT32.SYNCBUSY.bit.ENABLE);
  }

  // Finally, enable IRQ
  NVIC_SetPriority(irq, timer_config[timer_num].priority);
  NVIC_EnableIRQ(irq);
}

void HAL_timer_enable_interrupt(const uint8_t timer_num) {
  const IRQn_Type irq = timer_config[timer_num].IRQ_Id;
  NVIC_EnableIRQ(irq);
}

void HAL_timer_disable_interrupt(const uint8_t timer_num) {
  const IRQn_Type irq = timer_config[timer_num].IRQ_Id;
  Disable_Irq(irq);
}

// missing from CMSIS: Check if interrupt is enabled or not
static bool NVIC_GetEnabledIRQ(IRQn_Type IRQn) {
  return TEST(NVIC->ISER[uint32_t(IRQn) >> 5], uint32_t(IRQn) & 0x1F);
}

bool HAL_timer_interrupt_enabled(const uint8_t timer_num) {
  const IRQn_Type irq = timer_config[timer_num].IRQ_Id;
  return NVIC_GetEnabledIRQ(irq);
}

#endif // __SAMD51__
