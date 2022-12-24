/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2022 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * SAMD21 HAL developed by Bart Meijer (brupje)
 * Based on SAMD51 HAL by Giuliano Zaro (AKA GMagician)
 */
#ifdef __SAMD21__

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
  { {.pTcc=TCC0}, TimerType::tcc,  TCC0_IRQn, TC_PRIORITY(0) },  // 0 - stepper (assigned priority 2)
  { {.pTcc=TCC1}, TimerType::tcc,  TCC1_IRQn, TC_PRIORITY(1) },  // 1 - stepper (needed by 32 bit timers)
  { {.pTcc=TCC2}, TimerType::tcc,  TCC2_IRQn, 5              },  // 2 - tone (reserved by framework and fixed assigned priority 5)
  { {.pTc=TC3},   TimerType::tc,   TC3_IRQn, TC_PRIORITY(3) },  // 3 - servo (assigned priority 1)
  { {.pTc=TC4},   TimerType::tc,   TC4_IRQn, TC_PRIORITY(4) },  // 4 - software serial (no interrupts used)
  { {.pTc=TC5},   TimerType::tc,   TC5_IRQn, TC_PRIORITY(5) },
  { {.pTc=TC6},   TimerType::tc,   TC6_IRQn, TC_PRIORITY(6) },
  { {.pTc=TC7},   TimerType::tc,   TC7_IRQn, TC_PRIORITY(7) },
  { {.pRtc=RTC},  TimerType::rtc,   RTC_IRQn, TC_PRIORITY(8) }   // 8 - temperature (assigned priority 6)
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

static bool tcIsSyncing(Tc * tc) {
  return tc->COUNT32.STATUS.reg & TC_STATUS_SYNCBUSY;
}

static void tcReset( Tc * tc) {
  tc->COUNT32.CTRLA.reg = TC_CTRLA_SWRST;
  while (tcIsSyncing(tc)) {}
  while (tc->COUNT32.CTRLA.bit.SWRST) {}
}

// --------------------------------------------------------------------------
// Public functions
// --------------------------------------------------------------------------

void HAL_timer_start(const uint8_t timer_num, const uint32_t frequency) {
  IRQn_Type irq = timer_config[timer_num].IRQ_Id;

  // Disable interrupt, just in case it was already enabled
  NVIC_DisableIRQ(irq);
  NVIC_ClearPendingIRQ(irq);

  if (timer_num == MF_TIMER_RTC) {

    // https://github.com/arduino-libraries/RTCZero
    Rtc * const rtc = timer_config[timer_num].pRtc;
    PM->APBAMASK.reg |= PM_APBAMASK_RTC;

    GCLK->CLKCTRL.reg = (uint32_t)((GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK4 | (RTC_GCLK_ID << GCLK_CLKCTRL_ID_Pos)));
    while (GCLK->STATUS.bit.SYNCBUSY) {}

    GCLK->GENCTRL.reg = (GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_OSCULP32K | GCLK_GENCTRL_ID(4) | GCLK_GENCTRL_DIVSEL );
    while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY) {}

    GCLK->GENDIV.reg = GCLK_GENDIV_ID(4);
    GCLK->GENDIV.bit.DIV=4;
    while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY) {}

    // Disable timer interrupt
    rtc->MODE0.INTENCLR.reg = RTC_MODE0_INTENCLR_CMP0;
    SYNC(rtc->MODE0.STATUS.bit.SYNCBUSY);

    while(rtc->MODE0.STATUS.bit.SYNCBUSY) {}

    // Stop timer, just in case, to be able to reconfigure it
    rtc->MODE0.CTRL.reg =
      RTC_MODE0_CTRL_MODE_COUNT32 |           // Mode 0 = 32-bits counter
      RTC_MODE0_CTRL_PRESCALER_DIV1024;       // Divisor = 1024

    while(rtc->MODE0.STATUS.bit.SYNCBUSY) {}

    // Mode, reset counter on match
    rtc->MODE0.CTRL.reg = RTC_MODE0_CTRL_MODE_COUNT32 | RTC_MODE0_CTRL_MATCHCLR;

    // Set compare value
    rtc->MODE0.COMP[0].reg = (32768 + frequency / 2) / frequency;
    SYNC(rtc->MODE0.STATUS.bit.SYNCBUSY);

    // Enable interrupt on compare
    rtc->MODE0.INTFLAG.reg = RTC_MODE0_INTFLAG_CMP0;    // reset pending interrupt
    rtc->MODE0.INTENSET.reg = RTC_MODE0_INTENSET_CMP0;  // enable compare 0 interrupt

    // And start timer
    rtc->MODE0.CTRL.bit.ENABLE = true;
    SYNC(rtc->MODE0.STATUS.bit.SYNCBUSY);

  }
  else if (timer_config[timer_num].type==TimerType::tcc) {
    
    Tcc * const tc = timer_config[timer_num].pTcc;

    PM->APBCMASK.reg |= PM_APBCMASK_TCC0;
     GCLK->CLKCTRL.reg =(GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID(TCC0_GCLK_ID));
    SYNC (GCLK->STATUS.bit.SYNCBUSY);

    tc->CTRLA.reg = TCC_CTRLA_SWRST;
    SYNC (tc->SYNCBUSY.reg & TCC_SYNCBUSY_SWRST) {}

    SYNC (tc->CTRLA.bit.SWRST);

    tc->CTRLA.reg &= ~(TCC_CTRLA_ENABLE);       // disable TC module

    tc->CTRLA.reg |= TCC_WAVE_WAVEGEN_MFRQ;
    tc->CTRLA.reg |= TCC_CTRLA_PRESCALER_DIV2;
    tc->CC[0].reg = (HAL_TIMER_RATE) / frequency;
    tc->INTENSET.reg = TCC_INTFLAG_MC0;
    tc->CTRLA.reg |= TCC_CTRLA_ENABLE;
    tc->INTFLAG.reg = 0xFF;
    SYNC ( tc->STATUS.reg & TC_STATUS_SYNCBUSY);

  }
  else {
    Tc * const tc = timer_config[timer_num].pTc;

    // Disable timer interrupt
    tc->COUNT32.INTENCLR.reg = TC_INTENCLR_OVF; // disable overflow interrupt

    // TCn clock setup
    GCLK->CLKCTRL.reg = (uint16_t) (GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0  | GCLK_CLKCTRL_ID(GCM_TC4_TC5)) ;
    SYNC (GCLK->STATUS.bit.SYNCBUSY);

    tcReset(tc); // reset TC

    // Set Timer counter 5 Mode to 16 bits, it will become a 16bit counter ('mode1' in the datasheet)
    tc->COUNT32.CTRLA.reg |= TC_CTRLA_MODE_COUNT32;
    // Set TC waveform generation mode to 'match frequency'
    tc->COUNT32.CTRLA.reg |= TC_CTRLA_WAVEGEN_MFRQ;
    //set prescaler
    //the clock normally counts at the GCLK_TC frequency, but we can set it to divide that frequency to slow it down
    //you can use different prescaler divisons here like TC_CTRLA_PRESCALER_DIV1 to get a different range
    tc->COUNT32.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV1 | TC_CTRLA_ENABLE; //it will divide GCLK_TC frequency by 1024
    //set the compare-capture register.
    //The counter will count up to this value (it's a 16bit counter so we use uint16_t)
    //this is how we fine-tune the frequency, make it count to a lower or higher value
    //system clock should be 1MHz (8MHz/8) at Reset by default
    tc->COUNT32.CC[0].reg = (uint16_t) (HAL_TIMER_RATE / frequency);
    while (tcIsSyncing(tc)) {}

    // Enable the TC interrupt request
    tc->COUNT32.INTENSET.bit.MC0 = 1;
    while (tcIsSyncing(tc)) {}
  }

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

#endif // __SAMD21__
