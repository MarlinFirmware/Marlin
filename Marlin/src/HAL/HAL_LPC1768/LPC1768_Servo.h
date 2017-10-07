/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2017 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * The class Servo uses the PWM class to implement its functions
 *
 * The PWM1 module is only used to generate interrups at specified times. It
 * is NOT used to directly toggle pins. The ISR writes to the pin assigned to
 * that interrupt
 *
 * All PWMs use the same repetition rate - 20mS because that's the normal servo rate
 *
 */

#ifndef LPC1768_SERVO_H
#define LPC1768_SERVO_H

#include <stdint.h>

  class Servo {
    public:
      Servo();
      int8_t attach(const int pin);            // attach the given pin to the next free channel, set pinMode, return channel number (-1 on fail)
      int8_t attach(const int pin, const int min, const int max); // as above but also sets min and max values for writes.
      void detach();
      void write(int value);             // if value is < 200 it is treated as an angle, otherwise as pulse width in microseconds
      void writeMicroseconds(int value); // write pulse width in microseconds
      void move(const int value);        // attach the servo, then move to value
                                         // if value is < 200 it is treated as an angle, otherwise as pulse width in microseconds
                                         // if DEACTIVATE_SERVOS_AFTER_MOVE wait SERVO_DELAY, then detach
      int read();                        // returns current pulse width as an angle between 0 and 180 degrees
      int readMicroseconds();            // returns current pulse width in microseconds for this servo (was read_us() in first release)
      bool attached();                   // return true if this servo is attached, otherwise false

    private:
      uint8_t servoIndex;               // index into the channel data for this servo
      int min;
      int max;
  };

  #define HAL_SERVO_LIB Servo

#endif // LPC1768_SERVO_H
