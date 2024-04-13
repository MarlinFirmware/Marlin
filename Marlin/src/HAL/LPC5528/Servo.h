/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

class libServo {

private:
  uint32_t         minAngle = 0;                // 最小角度
  uint32_t         maxAngle = 180;              // 最大角度
  uint32_t         minPluseWidth = 544;         // 输出最小脉宽值
  uint32_t         MaxPluseWidth = 2400;        // 输出最大脉宽值
  uint32_t         TauMsec = 20;
  uint32_t         TauUsec = (TauMsec * 1000);
   uint32_t        MaxCompare = 65535-1;        // PWM 范围0-（65535 -1）

  int servo_pin;
  int degrees;

public:
  int8_t  attach(const int inPin);
  void    detach(void);
  void    write(int inDegrees);
  void    move(const int value);
  int     read(void);

  void    bspPwmOut(int pin, uint32_t duty);
  void    bspPwmDeinit(int pin);
};
