/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
  servo.h - Interrupt driven Servo library for Arduino using 16 bit timers- Version 2
  Copyright (c) 2009 Michael Margolis.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

/**

  A servo is activated by creating an instance of the Servo class passing the desired pin to the attach() method.
  The servos are pulsed in the background using the value most recently written using the write() method

  Note that analogWrite of PWM on pins associated with the timer are disabled when the first servo is attached.
  Timers are seized as needed in groups of 12 servos - 24 servos use two timers, 48 servos will use four.
  The sequence used to seize timers is defined in timers.h

  The methods are:

   Servo - Class for manipulating servo motors connected to Arduino pins.

   attach(pin )  - Attaches a servo motor to an i/o pin.
   attach(pin, min, max  ) - Attaches to a pin setting min and max values in microseconds
   default min is 544, max is 2400

   write()     - Sets the servo angle in degrees.  (invalid angle that is valid as pulse in microseconds is treated as microseconds)
   writeMicroseconds() - Sets the servo pulse width in microseconds
   read()      - Gets the last written servo pulse width as an angle between 0 and 180.
   readMicroseconds() - Gets the last written servo pulse width in microseconds. (was read_us() in first release)
   attached()  - Returns true if there is a servo attached.
   detach()    - Stops an attached servos from pulsing its i/o pin.
   move(angle) - Sequence of attach(0), write(angle),
                   With DEACTIVATE_SERVOS_AFTER_MOVE wait SERVO_DELAY and detach.
 */

#ifndef servo_h
#define servo_h

#include <inttypes.h>

/**
 * Defines for 16 bit timers used with  Servo library
 *
 * If _useTimerX is defined then TimerX is a 16 bit timer on the current board
 * timer16_Sequence_t enumerates the sequence that the timers should be allocated
 * _Nbr_16timers indicates how many 16 bit timers are available.
 *
 */

// Say which 16 bit timers can be used and in what order
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  //#define _useTimer1 // Timer 1 is used by the stepper ISR
  #define _useTimer3
  #define _useTimer4
  #if !HAS_MOTOR_CURRENT_PWM
    #define _useTimer5 // Timer 5 is used for motor current PWM and can't be used for servos.
  #endif
#elif defined(__AVR_ATmega32U4__)
  #define _useTimer3
#elif defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__)
  #define _useTimer3
#elif defined(__AVR_ATmega128__) || defined(__AVR_ATmega1281__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega2561__)
  #define _useTimer3
#else
  // everything else
#endif

typedef enum {
  #if ENABLED(_useTimer1)
    _timer1,
  #endif
  #if ENABLED(_useTimer3)
    _timer3,
  #endif
  #if ENABLED(_useTimer4)
    _timer4,
  #endif
  #if ENABLED(_useTimer5)
    _timer5,
  #endif
  _Nbr_16timers
} timer16_Sequence_t;


#define Servo_VERSION           2     // software version of this library

#define MIN_PULSE_WIDTH       544     // the shortest pulse sent to a servo
#define MAX_PULSE_WIDTH      2400     // the longest pulse sent to a servo
#define DEFAULT_PULSE_WIDTH  1500     // default pulse width when servo is attached
#define REFRESH_INTERVAL    20000     // minimum time to refresh servos in microseconds

#define SERVOS_PER_TIMER       12     // the maximum number of servos controlled by one timer
#define MAX_SERVOS   (_Nbr_16timers  * SERVOS_PER_TIMER)

#define INVALID_SERVO         255     // flag indicating an invalid servo index

typedef struct {
  uint8_t nbr        : 6 ;            // a pin number from 0 to 63
  uint8_t isActive   : 1 ;            // true if this channel is enabled, pin not pulsed if false
} ServoPin_t;

typedef struct {
  ServoPin_t Pin;
  unsigned int ticks;
} ServoInfo_t;

class Servo {
  public:
    Servo();
    int8_t attach(int pin);            // attach the given pin to the next free channel, set pinMode, return channel number (-1 on fail)
    int8_t attach(int pin, int min, int max); // as above but also sets min and max values for writes.
    void detach();
    void write(int value);             // if value is < 200 it is treated as an angle, otherwise as pulse width in microseconds
    void writeMicroseconds(int value); // write pulse width in microseconds
    void move(int value);              // attach the servo, then move to value
                                       // if value is < 200 it is treated as an angle, otherwise as pulse width in microseconds
                                       // if DEACTIVATE_SERVOS_AFTER_MOVE wait SERVO_DELAY, then detach
    int read();                        // returns current pulse width as an angle between 0 and 180 degrees
    int readMicroseconds();            // returns current pulse width in microseconds for this servo (was read_us() in first release)
    bool attached();                   // return true if this servo is attached, otherwise false

  private:
    uint8_t servoIndex;               // index into the channel data for this servo
    int8_t min;                       // minimum is this value times 4 added to MIN_PULSE_WIDTH
    int8_t max;                       // maximum is this value times 4 added to MAX_PULSE_WIDTH
};

#endif
