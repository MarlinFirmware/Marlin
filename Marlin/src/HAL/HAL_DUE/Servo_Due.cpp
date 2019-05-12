/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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

/*
  Copyright (c) 2013 Arduino LLC. All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/

#ifdef ARDUINO_ARCH_SAM

#include "../../inc/MarlinConfig.h"

#if HAS_SERVOS

#include "../shared/Marduino.h"
#include "../shared/servo.h"
#include "../shared/servo_private.h"

static volatile int8_t Channel[_Nbr_16timers];              // counter for the servo being pulsed for each timer (or -1 if refresh interval)

//------------------------------------------------------------------------------
/// Interrupt handler for the TC0 channel 1.
//------------------------------------------------------------------------------
void Servo_Handler(timer16_Sequence_t timer, Tc *pTc, uint8_t channel);

#ifdef _useTimer1
  void HANDLER_FOR_TIMER1(void) { Servo_Handler(_timer1, TC_FOR_TIMER1, CHANNEL_FOR_TIMER1); }
#endif
#ifdef _useTimer2
  void HANDLER_FOR_TIMER2(void) { Servo_Handler(_timer2, TC_FOR_TIMER2, CHANNEL_FOR_TIMER2); }
#endif
#ifdef _useTimer3
  void HANDLER_FOR_TIMER3(void) { Servo_Handler(_timer3, TC_FOR_TIMER3, CHANNEL_FOR_TIMER3); }
#endif
#ifdef _useTimer4
  void HANDLER_FOR_TIMER4(void) { Servo_Handler(_timer4, TC_FOR_TIMER4, CHANNEL_FOR_TIMER4); }
#endif
#ifdef _useTimer5
  void HANDLER_FOR_TIMER5(void) { Servo_Handler(_timer5, TC_FOR_TIMER5, CHANNEL_FOR_TIMER5); }
#endif

void Servo_Handler(timer16_Sequence_t timer, Tc *tc, uint8_t channel) {
  // clear interrupt
  tc->TC_CHANNEL[channel].TC_SR;
  if (Channel[timer] < 0)
    tc->TC_CHANNEL[channel].TC_CCR |= TC_CCR_SWTRG; // channel set to -1 indicated that refresh interval completed so reset the timer
  else if (SERVO_INDEX(timer, Channel[timer]) < ServoCount && SERVO(timer, Channel[timer]).Pin.isActive)
    extDigitalWrite(SERVO(timer, Channel[timer]).Pin.nbr, LOW); // pulse this channel low if activated

  Channel[timer]++;    // increment to the next channel
  if (SERVO_INDEX(timer, Channel[timer]) < ServoCount && Channel[timer] < SERVOS_PER_TIMER) {
    tc->TC_CHANNEL[channel].TC_RA = tc->TC_CHANNEL[channel].TC_CV + SERVO(timer,Channel[timer]).ticks;
    if (SERVO(timer,Channel[timer]).Pin.isActive)    // check if activated
      extDigitalWrite(SERVO(timer, Channel[timer]).Pin.nbr, HIGH); // its an active channel so pulse it high
  }
  else {
    // finished all channels so wait for the refresh period to expire before starting over
    tc->TC_CHANNEL[channel].TC_RA =
      tc->TC_CHANNEL[channel].TC_CV < usToTicks(REFRESH_INTERVAL) - 4
        ? (unsigned int)usToTicks(REFRESH_INTERVAL) // allow a few ticks to ensure the next OCR1A not missed
        : tc->TC_CHANNEL[channel].TC_CV + 4;        // at least REFRESH_INTERVAL has elapsed
    Channel[timer] = -1; // this will get incremented at the end of the refresh period to start again at the first channel
  }
}

static void _initISR(Tc *tc, uint32_t channel, uint32_t id, IRQn_Type irqn) {
  pmc_enable_periph_clk(id);
  TC_Configure(tc, channel,
    TC_CMR_TCCLKS_TIMER_CLOCK3 | // MCK/32
    TC_CMR_WAVE |                // Waveform mode
    TC_CMR_WAVSEL_UP_RC );       // Counter running up and reset when equals to RC

  /* 84MHz, MCK/32, for 1.5ms: 3937 */
  TC_SetRA(tc, channel, 2625); // 1ms

  /* Configure and enable interrupt */
  NVIC_EnableIRQ(irqn);
  // TC_IER_CPAS: RA Compare
  tc->TC_CHANNEL[channel].TC_IER = TC_IER_CPAS;

  // Enables the timer clock and performs a software reset to start the counting
  TC_Start(tc, channel);
}

void initISR(timer16_Sequence_t timer) {
  #ifdef _useTimer1
    if (timer == _timer1)
      _initISR(TC_FOR_TIMER1, CHANNEL_FOR_TIMER1, ID_TC_FOR_TIMER1, IRQn_FOR_TIMER1);
  #endif
  #ifdef _useTimer2
    if (timer == _timer2)
      _initISR(TC_FOR_TIMER2, CHANNEL_FOR_TIMER2, ID_TC_FOR_TIMER2, IRQn_FOR_TIMER2);
  #endif
  #ifdef _useTimer3
    if (timer == _timer3)
      _initISR(TC_FOR_TIMER3, CHANNEL_FOR_TIMER3, ID_TC_FOR_TIMER3, IRQn_FOR_TIMER3);
  #endif
  #ifdef _useTimer4
    if (timer == _timer4)
      _initISR(TC_FOR_TIMER4, CHANNEL_FOR_TIMER4, ID_TC_FOR_TIMER4, IRQn_FOR_TIMER4);
  #endif
  #ifdef _useTimer5
    if (timer == _timer5)
      _initISR(TC_FOR_TIMER5, CHANNEL_FOR_TIMER5, ID_TC_FOR_TIMER5, IRQn_FOR_TIMER5);
  #endif
}

void finISR(timer16_Sequence_t timer) {
  #ifdef _useTimer1
    TC_Stop(TC_FOR_TIMER1, CHANNEL_FOR_TIMER1);
  #endif
  #ifdef _useTimer2
    TC_Stop(TC_FOR_TIMER2, CHANNEL_FOR_TIMER2);
  #endif
  #ifdef _useTimer3
    TC_Stop(TC_FOR_TIMER3, CHANNEL_FOR_TIMER3);
  #endif
  #ifdef _useTimer4
    TC_Stop(TC_FOR_TIMER4, CHANNEL_FOR_TIMER4);
  #endif
  #ifdef _useTimer5
    TC_Stop(TC_FOR_TIMER5, CHANNEL_FOR_TIMER5);
  #endif
}

#endif // HAS_SERVOS

#endif // ARDUINO_ARCH_SAM
