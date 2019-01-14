/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 * Copyright (C) 2017 Victor Perez
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

#ifdef __STM32F1__

#include "../../inc/MarlinConfig.h"

#if HAS_SERVOS

uint8_t ServoCount; //=0

#include "HAL_Servo_STM32F1.h"

//#include "Servo.h"

#include <boards.h>
#include <io.h>
#include <pwm.h>
#include <wirish_math.h>

/**
 * 20 millisecond period config. For a 1-based prescaler,
 *
 *    (prescaler * overflow / CYC_MSEC) msec = 1 timer cycle = 20 msec
 * => prescaler * overflow = 20 * CYC_MSEC
 *
 * This uses the smallest prescaler that allows an overflow < 2^16.
 */
#define MAX_OVERFLOW    ((1 << 16) - 1)
#define CYC_MSEC        (1000 * CYCLES_PER_MICROSECOND)
#define TAU_MSEC        20
#define TAU_USEC        (TAU_MSEC * 1000)
#define TAU_CYC         (TAU_MSEC * CYC_MSEC)
#define SERVO_PRESCALER (TAU_CYC / MAX_OVERFLOW + 1)
#define SERVO_OVERFLOW  ((uint16_t)round((double)TAU_CYC / SERVO_PRESCALER))

// Unit conversions
#define US_TO_COMPARE(us) ((uint16_t)map((us), 0, TAU_USEC, 0, SERVO_OVERFLOW))
#define COMPARE_TO_US(c)  ((uint32_t)map((c), 0, SERVO_OVERFLOW, 0, TAU_USEC))
#define ANGLE_TO_US(a)    ((uint16_t)(map((a), this->minAngle, this->maxAngle, \
                                        SERVO_DEFAULT_MIN_PW, SERVO_DEFAULT_MAX_PW)))
#define US_TO_ANGLE(us)   ((int16_t)(map((us), SERVO_DEFAULT_MIN_PW, SERVO_DEFAULT_MAX_PW,  \
                                       this->minAngle, this->maxAngle)))

libServo::libServo() {
  this->servoIndex = ServoCount < MAX_SERVOS ? ServoCount++ : INVALID_SERVO;
}

bool libServo::attach(const int32_t pin, const int32_t minAngle, const int32_t maxAngle) {
  if (this->servoIndex >= MAX_SERVOS) return false;

  this->pin = pin;
  this->minAngle = minAngle;
  this->maxAngle = maxAngle;

  timer_dev *tdev = PIN_MAP[this->pin].timer_device;
  uint8_t tchan = PIN_MAP[this->pin].timer_channel;

  pinMode(this->pin, PWM);
  pwmWrite(this->pin, 0);

  timer_pause(tdev);
  timer_set_prescaler(tdev, SERVO_PRESCALER - 1); // prescaler is 1-based
  timer_set_reload(tdev, SERVO_OVERFLOW);
  timer_generate_update(tdev);
  timer_resume(tdev);

  return true;
}

bool libServo::detach() {
  if (!this->attached()) return false;
  pwmWrite(this->pin, 0);
  return true;
}

int32_t libServo::read() const {
  if (this->attached()) {
    timer_dev *tdev = PIN_MAP[this->pin].timer_device;
    uint8_t tchan = PIN_MAP[this->pin].timer_channel;
    return US_TO_ANGLE(COMPARE_TO_US(timer_get_compare(tdev, tchan)));
  }
  return 0;
}

void libServo::move(const int32_t value) {
  constexpr uint16_t servo_delay[] = SERVO_DELAY;
  static_assert(COUNT(servo_delay) == NUM_SERVOS, "SERVO_DELAY must be an array NUM_SERVOS long.");

  if (this->attached()) {
    pwmWrite(this->pin, US_TO_COMPARE(ANGLE_TO_US(constrain(value, this->minAngle, this->maxAngle))));
    safe_delay(servo_delay[this->servoIndex]);
    #if ENABLED(DEACTIVATE_SERVOS_AFTER_MOVE)
      this->detach();
    #endif
  }
}
#endif // HAS_SERVOS

#endif // __STM32F1__
