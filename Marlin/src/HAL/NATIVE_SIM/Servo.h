/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2025 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#pragma once

// BLTouch BLTCommand definition
#include "../../../feature/bltouch.h"

 class Servo
 {
 public:
   Servo();
   uint8_t attach(int pin);           // attach the given pin to the next free channel, sets pinMode, returns channel number or INVALID_SERVO if failure
   uint8_t attach(int pin, int min, int max); // as above but also sets min and max values for writes.
   void detach();
   void write(int value);             // if value is < 200 it's treated as an angle, otherwise as pulse width in microseconds
   void writeMicroseconds(int value); // Write pulse width in microseconds
   int read();                        // returns current pulse width as an angle between 0 and 180 degrees
   int readMicroseconds();            // returns current pulse width in microseconds for this servo (was read_us() in first release)
   bool attached();                   // return true if this servo is attached, otherwise false
   int move (const BLTCommand cmd);
 private:
    uint8_t servoIndex;               // index into the channel data for this servo
    int8_t min;                       // minimum is this value times 4 added to MIN_PULSE_WIDTH
    int8_t max;                       // maximum is this value times 4 added to MAX_PULSE_WIDTH
    int value;                        // pulse width in microseconds for this servo
  };


