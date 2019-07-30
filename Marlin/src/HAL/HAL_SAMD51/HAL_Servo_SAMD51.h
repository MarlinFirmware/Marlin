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
 *
 * This code comes from Arduino library but, at the moment this is written, such library is buggy and uncompilable
 *
 */

#pragma once

#define _useTimer1
 //#define _useTimer2   // <- TODO do not activate until the code in Servo.cpp has been changed in order
                        //         to manage more than one channel per timer on the SAMD architecture

#if defined (_useTimer1)
  #define TC_FOR_TIMER1             TC3
  #define CHANNEL_FOR_TIMER1        0
  #define INTENSET_BIT_FOR_TIMER_1  TC_INTENSET_MC0
  #define INTENCLR_BIT_FOR_TIMER_1  TC_INTENCLR_MC0
  #define INTFLAG_BIT_FOR_TIMER_1   TC_INTFLAG_MC0
  #define ID_TC_FOR_TIMER1          ID_TC3
  #define IRQn_FOR_TIMER1           TC3_IRQn
  #define HANDLER_FOR_TIMER1        TC3_Handler
#endif
#if defined (_useTimer2)
  #define TC_FOR_TIMER2             TC3
  #define CHANNEL_FOR_TIMER2        1
  #define INTENSET_BIT_FOR_TIMER_2  TC_INTENSET_MC1
  #define INTENCLR_BIT_FOR_TIMER_2  TC_INTENCLR_MC1
  #define ID_TC_FOR_TIMER2          ID_TC3
  #define IRQn_FOR_TIMER2           TC3_IRQn
  #define HANDLER_FOR_TIMER2        TC3_Handler
#endif

typedef enum {
  #if defined (_useTimer1)
    _timer1,
  #endif
  #if defined (_useTimer2)
    _timer2,
  #endif
    _Nbr_16timers
} timer16_Sequence_t;

#define Servo_VERSION           2     // software version of this library

#define MIN_PULSE_WIDTH       544     // the shortest pulse sent to a servo  
#define MAX_PULSE_WIDTH      2400     // the longest pulse sent to a servo 
#define DEFAULT_PULSE_WIDTH  1500     // default pulse width when servo is attached
#define REFRESH_INTERVAL    20000     // minumim time to refresh servos in microseconds 

#define SERVOS_PER_TIMER       12     // the maximum number of servos controlled by one timer 
#define MAX_SERVOS          (_Nbr_16timers  * SERVOS_PER_TIMER)

#define INVALID_SERVO         255     // flag indicating an invalid servo index

typedef struct {
  uint8_t nbr : 6;             // a pin number from 0 to 63
  uint8_t isActive : 1;        // true if this channel is enabled, pin not pulsed if false 
} ServoPin_t;

typedef struct {
  ServoPin_t Pin;
  volatile unsigned int ticks;
} servo_t;

class Servo {
  public:
    Servo();
    uint8_t attach(int pin);           // attach the given pin to the next free channel, sets pinMode, returns channel number or 0 if failure
    uint8_t attach(int pin, int min, int max); // as above but also sets min and max values for writes. 
    void detach();
    void write(int value);             // if value is < 200 its treated as an angle, otherwise as pulse width in microseconds 
    void writeMicroseconds(int value); // Write pulse width in microseconds 
    int read();                        // returns current pulse width as an angle between 0 and 180 degrees
    int readMicroseconds();            // returns current pulse width in microseconds for this servo (was read_us() in first release)
    void move(const int value);
    bool attached();                   // return true if this servo is attached, otherwise false 
  private:
    uint8_t servoIndex;               // index into the channel data for this servo
    int8_t min;                       // minimum is this value times 4 added to MIN_PULSE_WIDTH    
    int8_t max;                       // maximum is this value times 4 added to MAX_PULSE_WIDTH   
};

#define HAL_SERVO_LIB Servo
