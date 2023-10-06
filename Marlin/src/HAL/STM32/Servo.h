/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 * Copyright (c) 2017 Victor Perez
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

#include <Servo.h>

#include "../../core/millis_t.h"

// Inherit and expand on the official library
class libServo {
  public:
    libServo();
    int8_t attach(const int pin = 0); // pin == 0 uses value from previous call
    int8_t attach(const int pin, const int min, const int max);
    void detach() { stm32_servo.detach(); }
    int read() { return stm32_servo.read(); }
    void move(const int value);

    void pause();
    void resume();

    static void pause_all_servos();
    static void resume_all_servos();
    static void setInterruptPriority(uint32_t preemptPriority, uint32_t subPriority);

  private:
    Servo stm32_servo;

    int servo_pin = 0;
    millis_t delay = 0;

    bool was_attached_before_pause;
    int value_before_pause;
};
