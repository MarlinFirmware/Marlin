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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "../inc/MarlinConfig.h"

#if ENABLED(USE_CONTROLLER_FAN)

#include "controllerfan.h"
#include "../module/stepper/indirection.h"
#include "../module/temperature.h"

ControllerFan controllerFan;

uint8_t ControllerFan::speed;

#if ENABLED(CONTROLLER_FAN_EDITABLE)
  controllerFan_settings_t ControllerFan::settings; // {0}
#endif

void ControllerFan::setup() {
  SET_OUTPUT(CONTROLLER_FAN_PIN);
  init();
}

void ControllerFan::set_fan_speed(const uint8_t s) {
  speed = s < (CONTROLLERFAN_SPEED_MIN) ? 0 : s; // Fan OFF below minimum
}

void ControllerFan::update() {
  static millis_t lastMotorOn = 0,    // Last time a motor was turned on
                  nextMotorCheck = 0; // Last time the state was checked
  const millis_t ms = millis();
  if (ELAPSED(ms, nextMotorCheck)) {
    nextMotorCheck = ms + 2500UL; // Not a time critical function, so only check every 2.5s

    #define MOTOR_IS_ON(A,B) (A##_ENABLE_READ() == bool(B##_ENABLE_ON))
    #define _OR_ENABLED_E(N) || MOTOR_IS_ON(E##N,E)

    const bool motor_on = MOTOR_IS_ON(Z,Z)
      #if HAS_Z2_ENABLE
        || MOTOR_IS_ON(Z2,Z)
      #endif
      #if HAS_Z3_ENABLE
        || MOTOR_IS_ON(Z3,Z)
      #endif
      #if HAS_Z4_ENABLE
        || MOTOR_IS_ON(Z4,Z)
      #endif
      || (DISABLED(CONTROLLER_FAN_USE_Z_ONLY) && (
          MOTOR_IS_ON(X,X) || MOTOR_IS_ON(Y,Y)
          #if HAS_X2_ENABLE
            || MOTOR_IS_ON(X2,X)
          #endif
          #if HAS_Y2_ENABLE
            || MOTOR_IS_ON(Y2,Y)
          #endif
          #if E_STEPPERS
            REPEAT(E_STEPPERS, _OR_ENABLED_E)
          #endif
        )
      )
    ;

    // If any of the drivers or the heated bed are enabled...
    if (motor_on
      #if HAS_HEATED_BED
        || thermalManager.temp_bed.soft_pwm_amount > 0
      #endif
    ) lastMotorOn = ms; //... set time to NOW so the fan will turn on

    // Fan Settings. Set fan > 0:
    //  - If AutoMode is on and steppers have been enabled for CONTROLLERFAN_IDLE_TIME seconds.
    //  - If System is on idle and idle fan speed settings is activated.
    set_fan_speed(
      settings.auto_mode && lastMotorOn && PENDING(ms, lastMotorOn + settings.duration * 1000UL)
      ? settings.active_speed : settings.idle_speed
    );

    // Allow digital or PWM fan output (see M42 handling)
    WRITE(CONTROLLER_FAN_PIN, speed);
    analogWrite(pin_t(CONTROLLER_FAN_PIN), speed);
  }
}

#endif // USE_CONTROLLER_FAN
