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

#define MIN_PULSE_WIDTH       544     // the shortest pulse sent to a servo
#define MAX_PULSE_WIDTH      2400     // the longest pulse sent to a servo
#define DEFAULT_PULSE_WIDTH  1500     // default pulse width when servo is attached
#define SERVO_MIN_US(v) (MIN_PULSE_WIDTH - (v) * 4) // minimum value in uS for this servo
#define SERVO_MAX_US(v) (MAX_PULSE_WIDTH - (v) * 4) // maximum value in uS for this servo

class Servo {
public:
  Servo();
  uint8_t attach(int pin);            // Attach the given pin to the next free channel, set pinMode, return channel number or INVALID_SERVO if failure
  uint8_t attach(int pin, int min, int max); // As above but also set min and max values for writes.
  void detach();
  void write(int value);              // If value is < 200 it's treated as an angle, otherwise as pulse width in microseconds
  void writeMicroseconds(int value);  // Write pulse width in microseconds
  int read();                         // Return current pulse width as an angle between 0 and 180 degrees
  int readMicroseconds();             // Return current pulse width in microseconds for this servo
  bool attached();                    // Return true if this servo is attached, otherwise false
  int move (const unsigned char cmd);
private:
  uint8_t servoIndex;                 // Index into the channel data for this servo
  int8_t min;                         // Minimum is this value times 4 added to MIN_PULSE_WIDTH
  int8_t max;                         // Maximum is this value times 4 added to MAX_PULSE_WIDTH
  int value;                          // Pulse width in microseconds for this servo
  int servo_pin = 0;                  // pin number for this servo
};
