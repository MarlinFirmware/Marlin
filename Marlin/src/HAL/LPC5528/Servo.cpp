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

#ifdef TARGET_LPC5528

#include "../platforms.h"

#include "../../inc/MarlinConfig.h"

#if HAS_SERVOS

#include "Servo.h"

#define MAX_SERVORS   10

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void libServo::bspPwmOut(int pin, uint32_t duty) {
  pwm_write(pin,duty);
  // TODO..
}

void libServo::bspPwmDeinit(int pin) {
  pwm_detach_pin(pin);
  // TODO..
}


/* 初始化PWM 引脚 */
int8_t libServo::attach(const int inPin) {
  // if(chCount >= MAX_SERVORS) return -1;
  if(inPin > 0 ) servo_pin = inPin;
  bool result = true;
  pwm_init(50);
  analogWriteResolution(65535);  //
  result = pwm_attach_pin(SERVO0_PIN, 65535);
 // auto result = TODO.. 这里需要初始化对饮引脚, 成功返回true, 失败返回false.
  return result;
}

/* 禁用对应引脚PWM输出 */
void libServo::detach() { bspPwmDeinit(servo_pin); }

/* 计算比较值，输出PWM */
void libServo::write(int inDegrees) {
  degrees = constrain(inDegrees, minAngle, maxAngle);
  int us = map(degrees, minAngle, maxAngle, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  int duty = map(us, 0, TauUsec, 0, MaxCompare);

  /* PWM输出↓ */
  bspPwmOut(servo_pin, duty);
}

void libServo::move(const int value) {
  constexpr uint16_t servo_delay[] = SERVO_DELAY;
  static_assert(COUNT(servo_delay) == NUM_SERVOS, "SERVO_DELAY must be an array NUM_SERVOS long.");
  if (attach(0) >= 0) {
    write(value);
    safe_delay(servo_delay[servo_pin]);
    TERN_(DEACTIVATE_SERVOS_AFTER_MOVE, detach());
  }
}

int libServo::read() { return degrees; }

#endif

#endif // TARGET_LPC5528
