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
#pragma once

#include <Arduino.h>

class Servo {
  static const int MIN_ANGLE =   0,
                   MAX_ANGLE = 180,
                   MIN_PULSE_WIDTH =  544,  // Shortest pulse sent to a servo
                   MAX_PULSE_WIDTH = 2400,  // Longest pulse sent to a servo
                   TAU_MSEC = 20,
                   TAU_USEC = (TAU_MSEC * 1000),
                   MAX_COMPARE = ((1 << 16) - 1), // 65535
                   CHANNEL_MAX_NUM = 16;

public:
  Servo();
  int8_t attach(const int pin);   // attach the given pin to the next free channel, set pinMode, return channel number (-1 on fail)
  void detach();
  void write(int degrees);        // set angle
  void move(const int degrees);   // attach the servo, then move to value
  int read();                     // returns current pulse width as an angle between 0 and 180 degrees

private:
  static int channel_next_free;
  int channel;
  int pin;
  int degrees;
};
