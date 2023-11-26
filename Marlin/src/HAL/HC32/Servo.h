/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../inc/MarlinConfigPre.h"

#include <Servo.h>

/**
 * return last written value in servo.read instead of calculated value
 */
#define OPTIMISTIC_SERVO_READ

/**
 * @brief servo lib wrapper for marlin
 */
class MarlinServo {
public:
  MarlinServo();

  /**
   * @brief attach the pin to the servo, set pin mode, return channel number
   * @param pin pin to attach to
   * @return channel number, -1 if failed
   */
  int8_t attach(const pin_t apin);

  /**
   * @brief detach servo
   */
  void detach();

  /**
   * @brief is servo attached?
   */
  bool attached();

  /**
   * @brief set servo angle
   * @param angle new angle
   */
  void write(servo_angle_t angle);

  /**
   * @brief attach servo, move to angle, delay then detach
   * @param angle angle to move to
   */
  void move(servo_angle_t angle);

  /**
   * @brief read current angle
   * @return current angle betwwne 0 and 180 degrees
   */
  servo_angle_t read();

private:
  /**
   * @brief internal servo object, provided by arduino core
   */
  Servo servo;

  /**
   * @brief virtual servo channel
   */
  uint8_t channel;

  /**
   * @brief pin the servo attached to last
   */
  pin_t pin;

  /**
   * @brief last known servo angle
   */
  servo_angle_t angle;
};

// Alias for marlin HAL
typedef MarlinServo hal_servo_t;
