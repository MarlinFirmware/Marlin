/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include <Servo.h>

#define SIMPLE_REATTACH

// Inherit and expand on core Servo library
class libServo : public Servo {
  typedef Servo super;
  public:
    int8_t attach(const int pin);
    int8_t attach(const int pin, const int min, const int max);
    void move(const int value);
    // Re-attach to the given pin without resetting min/max (assumes nothing else needs the pin)
    #ifdef SIMPLE_REATTACH
      int8_t reattach() { return attach(0); }
    #else
      int8_t reattach();
    #endif
  private:
    #ifndef SIMPLE_REATTACH
      int inputmin, inputmax;
    #endif
    //uint16_t min_ticks, max_ticks;
    uint8_t servoIndex; // Index into the channel data for this servo
};
