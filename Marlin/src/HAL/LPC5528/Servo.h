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

/**
 * Servo support for LPC5528.
 *
 * Driven from the Arduino core's pwm API, which routes the pin to either an
 * SCTimer output or a CTIMER match output. The frame rate is set per-pin, so
 * attaching a servo retunes the timer that pin belongs to:
 *
 *  - On MKS OWL, SERVO0_PIN (P0_10) reaches CTIMER2_MAT0, which it shares with
 *    HEATER_0_PIN (P1_07) and FAN1_PIN (P1_06). While a servo is attached those
 *    two run at the 50 Hz servo frame rate. DEACTIVATE_SERVOS_AFTER_MOVE keeps
 *    that window down to the length of a deploy or stow.
 *  - HEATER_BED_PIN (P1_09) is on the SCTimer and is unaffected.
 */

#include "../../core/millis_t.h"
#include "../../core/types.h"

// Pulse widths in microseconds, and the servo frame period
#define SERVO_MIN_PULSE_WIDTH   544
#define SERVO_MAX_PULSE_WIDTH  2400
#define SERVO_PERIOD_US       20000   // 50 Hz

class libServo {
public:
  libServo();

  int8_t attach(const int inPin = 0);                 // Attach to a pin (0 = reattach the last pin)
  int8_t attach(const int inPin, const int inMin, const int inMax);
  void detach();

  void write(int inDegrees);                          // Set the angle in degrees
  void writeMicroseconds(int usec);                   // Set the pulse width directly
  void move(const int value);                         // Attach, move, optionally detach

  int read() { return degrees; }
  int readMicroseconds() { return pulse_us; }
  bool attached() { return is_attached; }

private:
  uint8_t servoIndex;                                 // Index into servo[] / SERVO_DELAY
  int  servo_pin = 0;
  int  degrees = 0;
  int  pulse_us = 0;
  int  min_us = SERVO_MIN_PULSE_WIDTH;
  int  max_us = SERVO_MAX_PULSE_WIDTH;
  bool is_attached = false;

  static uint8_t servoCount;
};

typedef libServo hal_servo_t;
