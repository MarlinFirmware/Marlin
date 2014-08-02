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

#if defined(ARDUINO_ARCH_SAM)

#include <Arduino.h>
#include "Servo.h"

#define usToTicks(_us)    (( clockCyclesPerMicrosecond() * _us) / 32)     // converts microseconds to tick
#define ticksToUs(_ticks) (( (unsigned)_ticks * 32)/ clockCyclesPerMicrosecond() ) // converts from ticks back to microseconds

#define TRIM_DURATION       2                               // compensation ticks to trim adjust for digitalWrite delays

static servo_t servos[MAX_SERVOS];                          // static array of servo structures

uint8_t ServoCount = 0;                                     // the total number of attached servos

static volatile int8_t Channel[_Nbr_16timers ];             // counter for the servo being pulsed for each timer (or -1 if refresh interval)

// convenience macros
#define SERVO_INDEX_TO_TIMER(_servo_nbr) ((timer16_Sequence_t)(_servo_nbr / SERVOS_PER_TIMER)) // returns the timer controlling this servo
#define SERVO_INDEX_TO_CHANNEL(_servo_nbr) (_servo_nbr % SERVOS_PER_TIMER)       // returns the index of the servo on this timer
#define SERVO_INDEX(_timer,_channel)  ((_timer*SERVOS_PER_TIMER) + _channel)     // macro to access servo index by timer and channel
#define SERVO(_timer,_channel)  (servos[SERVO_INDEX(_timer,_channel)])            // macro to access servo class by timer and channel

#define SERVO_MIN() (MIN_PULSE_WIDTH - this->min * 4)  // minimum value in uS for this servo
#define SERVO_MAX() (MAX_PULSE_WIDTH - this->max * 4)  // maximum value in uS for this servo

/************ static functions common to all instances ***********************/

//------------------------------------------------------------------------------
/// Interrupt handler for the TC0 channel 1.
//------------------------------------------------------------------------------
void Servo_Handler(timer16_Sequence_t timer, Tc *pTc, uint8_t channel);
#if defined (_useTimer1)
void HANDLER_FOR_TIMER1(void) {
    Servo_Handler(_timer1, TC_FOR_TIMER1, CHANNEL_FOR_TIMER1);
}
#endif
#if defined (_useTimer2)
void HANDLER_FOR_TIMER2(void) {
    Servo_Handler(_timer2, TC_FOR_TIMER2, CHANNEL_FOR_TIMER2);
}
#endif
#if defined (_useTimer3)
void HANDLER_FOR_TIMER3(void) {
    Servo_Handler(_timer3, TC_FOR_TIMER3, CHANNEL_FOR_TIMER3);
}
#endif
#if defined (_useTimer4)
void HANDLER_FOR_TIMER4(void) {
    Servo_Handler(_timer4, TC_FOR_TIMER4, CHANNEL_FOR_TIMER4);
}
#endif
#if defined (_useTimer5)
void HANDLER_FOR_TIMER5(void) {
    Servo_Handler(_timer5, TC_FOR_TIMER5, CHANNEL_FOR_TIMER5);
}
#endif

void Servo_Handler(timer16_Sequence_t timer, Tc *tc, uint8_t channel)
{
    // clear interrupt
    tc->TC_CHANNEL[channel].TC_SR;
    if (Channel[timer] < 0) {
        tc->TC_CHANNEL[channel].TC_CCR |= TC_CCR_SWTRG; // channel set to -1 indicated that refresh interval completed so reset the timer
    } else {
        if (SERVO_INDEX(timer,Channel[timer]) < ServoCount && SERVO(timer,Channel[timer]).Pin.isActive == true) {
            digitalWrite(SERVO(timer,Channel[timer]).Pin.nbr, LOW); // pulse this channel low if activated
        }
    }

    Channel[timer]++;    // increment to the next channel
    if( SERVO_INDEX(timer,Channel[timer]) < ServoCount && Channel[timer] < SERVOS_PER_TIMER) {
        tc->TC_CHANNEL[channel].TC_RA = tc->TC_CHANNEL[channel].TC_CV + SERVO(timer,Channel[timer]).ticks;
        if(SERVO(timer,Channel[timer]).Pin.isActive == true) {    // check if activated
            digitalWrite( SERVO(timer,Channel[timer]).Pin.nbr,HIGH); // its an active channel so pulse it high
        }
    }
    else {
        // finished all channels so wait for the refresh period to expire before starting over
        if( (tc->TC_CHANNEL[channel].TC_CV) + 4 < usToTicks(REFRESH_INTERVAL) ) { // allow a few ticks to ensure the next OCR1A not missed
            tc->TC_CHANNEL[channel].TC_RA = (unsigned int)usToTicks(REFRESH_INTERVAL);
        }
        else {
            tc->TC_CHANNEL[channel].TC_RA = tc->TC_CHANNEL[channel].TC_CV + 4;  // at least REFRESH_INTERVAL has elapsed
        }
        Channel[timer] = -1; // this will get incremented at the end of the refresh period to start again at the first channel
    }
}

static void _initISR(Tc *tc, uint32_t channel, uint32_t id, IRQn_Type irqn)
{
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

static void initISR(timer16_Sequence_t timer)
{
#if defined (_useTimer1)
    if (timer == _timer1)
        _initISR(TC_FOR_TIMER1, CHANNEL_FOR_TIMER1, ID_TC_FOR_TIMER1, IRQn_FOR_TIMER1);
#endif
#if defined (_useTimer2)
    if (timer == _timer2)
        _initISR(TC_FOR_TIMER2, CHANNEL_FOR_TIMER2, ID_TC_FOR_TIMER2, IRQn_FOR_TIMER2);
#endif
#if defined (_useTimer3)
    if (timer == _timer3)
        _initISR(TC_FOR_TIMER3, CHANNEL_FOR_TIMER3, ID_TC_FOR_TIMER3, IRQn_FOR_TIMER3);
#endif
#if defined (_useTimer4)
    if (timer == _timer4)
        _initISR(TC_FOR_TIMER4, CHANNEL_FOR_TIMER4, ID_TC_FOR_TIMER4, IRQn_FOR_TIMER4);
#endif
#if defined (_useTimer5)
    if (timer == _timer5)
        _initISR(TC_FOR_TIMER5, CHANNEL_FOR_TIMER5, ID_TC_FOR_TIMER5, IRQn_FOR_TIMER5);
#endif
}

static void finISR(timer16_Sequence_t timer)
{
#if defined (_useTimer1)
    TC_Stop(TC_FOR_TIMER1, CHANNEL_FOR_TIMER1);
#endif
#if defined (_useTimer2)
    TC_Stop(TC_FOR_TIMER2, CHANNEL_FOR_TIMER2);
#endif
#if defined (_useTimer3)
    TC_Stop(TC_FOR_TIMER3, CHANNEL_FOR_TIMER3);
#endif
#if defined (_useTimer4)
    TC_Stop(TC_FOR_TIMER4, CHANNEL_FOR_TIMER4);
#endif
#if defined (_useTimer5)
    TC_Stop(TC_FOR_TIMER5, CHANNEL_FOR_TIMER5);
#endif
}


static boolean isTimerActive(timer16_Sequence_t timer)
{
  // returns true if any servo is active on this timer
  for(uint8_t channel=0; channel < SERVOS_PER_TIMER; channel++) {
    if(SERVO(timer,channel).Pin.isActive == true)
      return true;
  }
  return false;
}

/****************** end of static functions ******************************/

Servo::Servo()
{
  if (ServoCount < MAX_SERVOS) {
    this->servoIndex = ServoCount++;                    // assign a servo index to this instance
    servos[this->servoIndex].ticks = usToTicks(DEFAULT_PULSE_WIDTH);   // store default values
  } else {
    this->servoIndex = INVALID_SERVO;  // too many servos
  }
}

uint8_t Servo::attach(int pin)
{
  return this->attach(pin, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
}

uint8_t Servo::attach(int pin, int min, int max)
{
  timer16_Sequence_t timer;

  if (this->servoIndex < MAX_SERVOS) {
    pinMode(pin, OUTPUT);                                   // set servo pin to output
    servos[this->servoIndex].Pin.nbr = pin;
    // todo min/max check: abs(min - MIN_PULSE_WIDTH) /4 < 128
    this->min  = (MIN_PULSE_WIDTH - min)/4; //resolution of min/max is 4 uS
    this->max  = (MAX_PULSE_WIDTH - max)/4;
    // initialize the timer if it has not already been initialized
    timer = SERVO_INDEX_TO_TIMER(servoIndex);
    if (isTimerActive(timer) == false) {
      initISR(timer);
    }
    servos[this->servoIndex].Pin.isActive = true;  // this must be set after the check for isTimerActive
  }
  return this->servoIndex;
}

void Servo::detach()
{
  timer16_Sequence_t timer;

  servos[this->servoIndex].Pin.isActive = false;
  timer = SERVO_INDEX_TO_TIMER(servoIndex);
  if(isTimerActive(timer) == false) {
    finISR(timer);
  }
}

void Servo::write(int value)
{
  // treat values less than 544 as angles in degrees (valid values in microseconds are handled as microseconds)
  if (value < MIN_PULSE_WIDTH)
  {
    if (value < 0)
      value = 0;
    else if (value > 180)
      value = 180;

    value = map(value, 0, 180, SERVO_MIN(), SERVO_MAX());
  }
  writeMicroseconds(value);
}

void Servo::writeMicroseconds(int value)
{
  // calculate and store the values for the given channel
  byte channel = this->servoIndex;
  if( (channel < MAX_SERVOS) )   // ensure channel is valid
  {
    if (value < SERVO_MIN())          // ensure pulse width is valid
      value = SERVO_MIN();
    else if (value > SERVO_MAX())
      value = SERVO_MAX();

    value = value - TRIM_DURATION;
    value = usToTicks(value);  // convert to ticks after compensating for interrupt overhead
    servos[channel].ticks = value;
  }
}

int Servo::read() // return the value as degrees
{
  return map(readMicroseconds()+1, SERVO_MIN(), SERVO_MAX(), 0, 180);
}

int Servo::readMicroseconds()
{
  unsigned int pulsewidth;
  if (this->servoIndex != INVALID_SERVO)
    pulsewidth = ticksToUs(servos[this->servoIndex].ticks)  + TRIM_DURATION;
  else
    pulsewidth  = 0;

  return pulsewidth;
}

bool Servo::attached()
{
  return servos[this->servoIndex].Pin.isActive;
}

#endif // ARDUINO_ARCH_SAM

