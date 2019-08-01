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

#define _TC_GCLK_ID(t)          TC##t##_GCLK_ID
#define TC_GCLK_ID              _TC_GCLK_ID(SERVO_TCx)

#define _SERVO_IRQn(t)          TC##t##_IRQn
#define SERVO_IRQn              _SERVO_IRQn(SERVO_TCx)

#define _SERVO_ISR_HANDLER(t)   TC##t##_Handler
#define SERVO_ISR_HANDLER       _SERVO_ISR_HANDLER(SERVO_TCx)

#define _SERVO_TC(t)            TC##t
#define SERVO_TC                _SERVO_TC(SERVO_TCx)

#define TIMER_TCCHANNEL(t)      ((t) & 1)
#define TC_COUNTER_START_VAL    0xFFFF


static bool initialized;                                    // Servo TC has been initialized
static volatile int8_t currentServoIndex[_Nbr_16timers];    // index for the servo being pulsed for each timer (or -1 if refresh interval)

FORCE_INLINE static uint16_t getTimerCount() {
  SERVO_TC->COUNT16.CTRLBSET.reg = TC_CTRLBCLR_CMD_READSYNC;
  SYNC(SERVO_TC->COUNT16.SYNCBUSY.bit.CTRLB || SERVO_TC->COUNT16.SYNCBUSY.bit.COUNT);

  return SERVO_TC->COUNT16.COUNT.reg;
}

// ----------------------------
// Interrupt handler for the TC
// ----------------------------
void SERVO_ISR_HANDLER() {
  const timer16_Sequence_t timer =
    #if !defined(_timer1)
      _timer2
    #elif !defined(_timer2)
      _timer1
    #else
      (SERVO_TC->COUNT16.INTFLAG.reg & TC_INTFLAG_MC0) ? _timer1 : _timer2
    #endif
  ;
  const uint8_t tcChannel = TIMER_TCCHANNEL(timer);

  if (currentServoIndex[timer] < 0) {
    SERVO_TC->COUNT16.COUNT.reg = TC_COUNTER_START_VAL;  // TODO need fix to handle multi channels
    SYNC(SERVO_TC->COUNT16.SYNCBUSY.bit.COUNT);
  }
  else if (SERVO_INDEX(timer, currentServoIndex[timer]) < ServoCount && SERVO(timer, currentServoIndex[timer]).Pin.isActive)
    digitalWrite(SERVO(timer, currentServoIndex[timer]).Pin.nbr, LOW);      // pulse this channel low if activated

  // Select the next servo controlled by this timer
  currentServoIndex[timer]++;

  if (SERVO_INDEX(timer, currentServoIndex[timer]) < ServoCount && currentServoIndex[timer] < SERVOS_PER_TIMER) {
    if (SERVO(timer, currentServoIndex[timer]).Pin.isActive)                // check if activated
      digitalWrite(SERVO(timer, currentServoIndex[timer]).Pin.nbr, HIGH);   // it's an active channel so pulse it high

    SERVO_TC->COUNT16.CC[channel].reg = getTimerCounter() - (uint16_t)SERVO(timer, currentServoIndex[timer]).ticks;
  }
  else {
    // finished all channels so wait for the refresh period to expire before starting over
    currentServoIndex[timer] = -1;   // this will get incremented at the end of the refresh period to start again at the first channel

    const uint16_t tcCounterValue = getTimerCount();

    if ((TC_COUNTER_START_VAL - tcCounterValue) + 4UL < usToTicks(REFRESH_INTERVAL))  // allow a few ticks to ensure the next OCR1A not missed
      SERVO_TC->COUNT16.CC[channel].reg = TC_COUNTER_START_VAL - (uint16_t)usToTicks(REFRESH_INTERVAL);
    else
      SERVO_TC->COUNT16.CC[channel].reg = (uint16_t)(tcCounterValue - 4UL);           // at least REFRESH_INTERVAL has elapsed
  }
  if (channel == 0) {
    SYNC(SERVO_TC->COUNT16.SYNCBUSY.bit.CC0); 
    // Clear the interrupt
    SERVO_TC->COUNT16.INTFLAG.reg = TC_INTFLAG_MC0;
  }
  else {
    SYNC(SERVO_TC->COUNT16.SYNCBUSY.bit.CC1); 
    // Clear the interrupt
    SERVO_TC->COUNT16.INTFLAG.reg = TC_INTFLAG_MC1;
  }
}

static void initISR(timer16_Sequence_t timer) {
  if (!initialized) {
    NVIC_DisableIRQ(SERVO_IRQn);

    // Disable the timer
    SERVO_TC->COUNT16.CTRLA.bit.ENABLE = false;
    SYNC(SERVO_TC->COUNT16.SYNCBUSY.bit.ENABLE);

    // Select GCLK0 as timer/counter input clock source
    GCLK->PCHCTRL[TC_GCLK_ID].bit.CHEN = false;
    SYNC(GCLK->PCHCTRL[TC_GCLK_ID].bit.CHEN);
    GCLK->PCHCTRL[TC_GCLK_ID].reg = GCLK_PCHCTRL_GEN_GCLK0 | GCLK_PCHCTRL_CHEN;   // 120MHz startup code programmed
    SYNC(!GCLK->PCHCTRL[TC_GCLK_ID].bit.CHEN);

    // Reset the timer
    SERVO_TC->COUNT16.CTRLA.bit.SWRST = true;
    SYNC(SERVO_TC->COUNT16.SYNCBUSY.bit.SWRST);
    SYNC(SERVO_TC->COUNT16.CTRLA.bit.SWRST);

    // Set timer counter mode to 16 bits
    SERVO_TC->COUNT16.CTRLA.reg = TC_CTRLA_MODE_COUNT16;

    // Set timer counter mode as normal PWM
    SERVO_TC->COUNT16.WAVE.bit.WAVEGEN = TCC_WAVE_WAVEGEN_NPWM_Val;

    // Set the prescaler factor
    SERVO_TC->COUNT16.CTRLA.bit.PRESCALER = TCC_CTRLA_PRESCALER_DIV64_Val;?

    // Count down
    SERVO_TC->COUNT16.CTRLBSET.reg = TC_CTRLBCLR_DIR;
    SYNC(SERVO_TC->COUNT16.SYNCBUSY.bit.CTRLB);

    // Configure interrupt request
    NVIC_ClearPendingIRQ(SERVO_IRQn);
    NVIC_SetPriority(SERVO_IRQn, 5);
    NVIC_EnableIRQ(SERVO_IRQn);

    initialized = true;
  }

  if (!SERVO_TC->COUNT16.CTRLA.bit.ENABLE) {
    // Reset the timer counter
    SERVO_TC->COUNT16.COUNT.reg = TC_COUNTER_START_VAL;
    SYNC(SERVO_TC->COUNT16.SYNCBUSY.bit.COUNT);

    // Enable the timer and start it
    SERVO_TC->COUNT16.CTRLA.bit.ENABLE = true;
    SYNC(SERVO_TC->COUNT16.SYNCBUSY.bit.ENABLE);
  }
  // First interrupt request after 1 ms
  SERVO_TC->COUNT16.CC[tcChannel].reg = getTimerCount() - (uint16_t)usToTicks(1000UL);

  if (tcChannel == 0 ) {
    SYNC(SERVO_TC->COUNT16.SYNCBUSY.bit.CC0);

    // Clear pending match interrupt
    SERVO_TC->COUNT16.INTFLAG.reg = TC_INTENSET_MC0;
    // Enable the match channel interrupt request
    SERVO_TC->COUNT16.INTENSET.reg = TC_INTENSET_MC0;
  }
  else {
    SYNC(SERVO_TC->COUNT16.SYNCBUSY.bit.CC1);

    // Clear pending match interrupt
    SERVO_TC->COUNT16.INTFLAG.reg = TC_INTENSET_MC1;
    // Enable the match channel interrupt request
    SERVO_TC->COUNT16.INTENSET.reg = TC_INTENSET_MC1;
  }
}

void finISR(timer16_Sequence_t timer) {
  // Disable the match channel interrupt request
  const uint8_t tcChannel = TIMER_TCCHANNEL(timer);
  SERVO_TC->COUNT16.INTENCLR.reg = (tcChannel == 0) ? TC_INTENCLR_MC0 : TC_INTENCLR_MC1;

  if (SERVO_TC->COUNT16.INTENCLR.reg & (TC_INTENCLR_MC0|TC_INTENCLR_MC1) == 0) {
    // Disable the timer if not used
    SERVO_TC->COUNT16.CTRLA.bit.ENABLE = false;
    SYNC(SERVO_TC->COUNT16.SYNCBUSY.bit.ENABLE);
  }
}

#endif // HAS_SERVOS

#endif // __SAMD51__
