/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 */
#pragma once

#include "../../inc/MarlinConfig.h"

#if HAS_SERVOS

#include <Servo.h> // AT32 Arduino core Servo class

// Forward declarations for Servo.cpp
extern uint8_t servoCount;
extern MarlinServo* servos[];

/**
 * MarlinServo wraps the AT32 Arduino core Servo class.
 */
class MarlinServo {
public:
  MarlinServo();

  int8_t attach(const pin_t pin);
  void   detach();
  bool   attached();
  void   write(servo_angle_t angle);
  void   move(servo_angle_t angle);
  servo_angle_t read();

  pin_t         pin     = -1;
  servo_angle_t angle   = 0;
  uint8_t       channel = 0;

private:
  Servo _servo; // AT32 core Servo instance
};

class libServo : public MarlinServo {
public:
  static void pause_all_servos() {
    for (int i = 0; i < NUM_SERVOS; i++)
      if (servos[i]) servos[i]->detach();
  }
  static void resume_all_servos() {
    for (int i = 0; i < NUM_SERVOS; i++)
      if (servos[i]) servos[i]->attach(-1);
  }
};

#endif // HAS_SERVOS
