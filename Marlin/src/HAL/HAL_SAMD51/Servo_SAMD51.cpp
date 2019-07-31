/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * This comes from Arduino library which at the moment is buggy and uncompilable
 */

#ifdef __SAMD51__

#include "../../inc/MarlinConfig.h"

#if HAS_SERVOS

#include "../shared/Marduino.h"
#include "../shared/servo.h"
#include "../shared/servo_private.h"
#include "SAMD51.h"

static volatile int8_t currentServoIndex[_Nbr_16timers];    // index for the servo being pulsed for each timer (or -1 if refresh interval)

static inline void resetTC(Tc* TCx) {
  // Disable TCx
  TCx->COUNT16.CTRLA.reg &= ~TC_CTRLA_ENABLE;
  SYNC(TCx->COUNT16.SYNCBUSY.bit.ENABLE);

  // Reset TCx
  TCx->COUNT16.CTRLA.reg = TC_CTRLA_SWRST;
  SYNC(TCx->COUNT16.SYNCBUSY.bit.SWRST);
  SYNC(TCx->COUNT16.CTRLA.bit.SWRST);
}

// ----------------------------
// Interrupt handler for the TC
// ----------------------------
void Servo_Handler(timer16_Sequence_t timer, Tc *pTc, uint8_t channel, uint8_t intFlag);
#ifdef _useTimer1
  void HANDLER_FOR_TIMER1(void) { Servo_Handler(_timer1, TC_FOR_TIMER1, CHANNEL_FOR_TIMER1, INTFLAG_BIT_FOR_TIMER_1); }
#endif
#ifdef _useTimer2
  void HANDLER_FOR_TIMER2(void) { Servo_Handler(_timer2, TC_FOR_TIMER2, CHANNEL_FOR_TIMER2, INTFLAG_BIT_FOR_TIMER_2); }
#endif

void Servo_Handler(timer16_Sequence_t timer, Tc *tc, uint8_t channel, uint8_t intFlag) {
  if (currentServoIndex[timer] < 0) {
    tc->COUNT16.COUNT.reg = 0;
    SYNC(tc->COUNT16.SYNCBUSY.bit.COUNT);
  }
  else if (SERVO_INDEX(timer, currentServoIndex[timer]) < ServoCount && SERVO(timer, currentServoIndex[timer]).Pin.isActive)
    digitalWrite(SERVO(timer, currentServoIndex[timer]).Pin.nbr, LOW);   // pulse this channel low if activated

  // Select the next servo controlled by this timer
  currentServoIndex[timer]++;

  if (SERVO_INDEX(timer, currentServoIndex[timer]) < ServoCount && currentServoIndex[timer] < SERVOS_PER_TIMER) {
    if (SERVO(timer, currentServoIndex[timer]).Pin.isActive)     // check if activated
      digitalWrite(SERVO(timer, currentServoIndex[timer]).Pin.nbr, HIGH);   // it's an active channel so pulse it high

    // Get the counter value
    const uint16_t tcCounterValue = tc->COUNT16.COUNT.reg;
    SYNC(tc->COUNT16.SYNCBUSY.bit.COUNT);

    tc->COUNT16.CC[channel].reg = uint16_t(tcCounterValue + SERVO(timer, currentServoIndex[timer]).ticks);
         if (channel == 0) { SYNC(tc->COUNT16.SYNCBUSY.bit.CC0); }
    else if (channel == 1) { SYNC(tc->COUNT16.SYNCBUSY.bit.CC1); }
  }
  else {
    // finished all channels so wait for the refresh period to expire before starting over

    // Get the counter value
    const uint16_t tcCounterValue = tc->COUNT16.COUNT.reg;
    SYNC(tc->COUNT16.SYNCBUSY.bit.COUNT);

    if (tcCounterValue + 4UL < usToTicks(REFRESH_INTERVAL))     // allow a few ticks to ensure the next OCR1A not missed
      tc->COUNT16.CC[channel].reg = (uint16_t)usToTicks(REFRESH_INTERVAL);
    else
      tc->COUNT16.CC[channel].reg = (uint16_t)(tcCounterValue + 4UL);   // at least REFRESH_INTERVAL has elapsed

         if (channel == 0) { SYNC(tc->COUNT16.SYNCBUSY.bit.CC0); }
    else if (channel == 1) { SYNC(tc->COUNT16.SYNCBUSY.bit.CC1); }

    currentServoIndex[timer] = -1;   // this will get incremented at the end of the refresh period to start again at the first channel
  }

  // Clear the interrupt
  tc->COUNT16.INTFLAG.reg = intFlag;
}

static void _initISR(Tc *tc, uint8_t channel, IRQn_Type irqn, uint8_t intEnableBit) {
  // Select GCLK0 as timer/counter input clock source
  int idx = 30;                       // TC3
  GCLK->PCHCTRL[idx].bit.GEN = 0;     // Select GCLK0 as periph clock source
  GCLK->PCHCTRL[idx].bit.CHEN = true; // Enable peripheral
  SYNC(!GCLK->PCHCTRL[idx].bit.CHEN);

  // Reset the timer
  // TODO this is not the right thing to do if more than one channel per timer is used by the Servo library
  resetTC(tc);

  // Set timer counter mode to 16 bits
  tc->COUNT16.CTRLA.reg |= TC_CTRLA_MODE_COUNT16;

  // Set timer counter mode as normal PWM
  tc->COUNT16.WAVE.bit.WAVEGEN = TCC_WAVE_WAVEGEN_NPWM_Val;

  // Set the prescaler factor to 64 (avoid overflowing 16-bit clock counter)
  // At 120-200 MHz GCLK this is 1875-3125 ticks per millisecond
  tc->COUNT16.CTRLA.bit.PRESCALER = TCC_CTRLA_PRESCALER_DIV64_Val;

  // Count up
  tc->COUNT16.CTRLBCLR.bit.DIR = 1;
  SYNC(tc->COUNT16.SYNCBUSY.bit.CTRLB);

  // First interrupt request after 1 ms
  tc->COUNT16.CC[channel].reg = (uint16_t)usToTicks(1000UL);
       if (channel == 0) { SYNC(tc->COUNT16.SYNCBUSY.bit.CC0); }
  else if (channel == 1) { SYNC(tc->COUNT16.SYNCBUSY.bit.CC1); }

  // Configure interrupt request
  // TODO this should be changed if more than one channel per timer is used by the Servo library
  NVIC_DisableIRQ(irqn);
  NVIC_ClearPendingIRQ(irqn);
  NVIC_SetPriority(irqn, 5);
  NVIC_EnableIRQ(irqn);

  // Enable the match channel interrupt request
  tc->COUNT16.INTENSET.reg = intEnableBit;

  // Enable the timer and start it
  tc->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE;
  SYNC(tc->COUNT16.SYNCBUSY.bit.ENABLE);
}

void initISR(timer16_Sequence_t timer) {
  #ifdef _useTimer1
    if (timer == _timer1)
      _initISR(TC_FOR_TIMER1, CHANNEL_FOR_TIMER1, IRQn_FOR_TIMER1, INTENSET_BIT_FOR_TIMER_1);
  #endif
  #ifdef _useTimer2
    if (timer == _timer2)
      _initISR(TC_FOR_TIMER2, CHANNEL_FOR_TIMER2, IRQn_FOR_TIMER2, INTENSET_BIT_FOR_TIMER_2);
  #endif
}

void finISR(timer16_Sequence_t timer) {
  #ifdef _useTimer1
    // Disable the match channel interrupt request
    TC_FOR_TIMER1->COUNT16.INTENCLR.reg = INTENCLR_BIT_FOR_TIMER_1;
  #endif
  #ifdef _useTimer2
    // Disable the match channel interrupt request
    TC_FOR_TIMER2->COUNT16.INTENCLR.reg = INTENCLR_BIT_FOR_TIMER_2;
  #endif
}

#endif // HAS_SERVOS

#endif // __SAMD51__
