/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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

#include "../module/stepper_indirection.h"
#include "../module/temperature.h"

uint8_t controllerFanSpeed;

void controllerfan_update() {
  static millis_t lastMotorOn = 0, // Last time a motor was turned on
                  nextMotorCheck = 0; // Last time the state was checked
  const millis_t ms = millis();
  if (ELAPSED(ms, nextMotorCheck)) {
    nextMotorCheck = ms + 2500UL; // Not a time critical function, so only check every 2.5s
    if (X_ENABLE_READ == X_ENABLE_ON || Y_ENABLE_READ == Y_ENABLE_ON || Z_ENABLE_READ == Z_ENABLE_ON
      #if HAS_HEATED_BED
        || thermalManager.soft_pwm_amount_bed > 0
      #endif
        || E0_ENABLE_READ == E_ENABLE_ON // If any of the drivers are enabled...
        #if E_STEPPERS > 1
          || E1_ENABLE_READ == E_ENABLE_ON
          #if HAS_X2_ENABLE
            || X2_ENABLE_READ == X_ENABLE_ON
          #endif
          #if E_STEPPERS > 2
            || E2_ENABLE_READ == E_ENABLE_ON
            #if E_STEPPERS > 3
              || E3_ENABLE_READ == E_ENABLE_ON
              #if E_STEPPERS > 4
                || E4_ENABLE_READ == E_ENABLE_ON
              #endif // E_STEPPERS > 4
            #endif // E_STEPPERS > 3
          #endif // E_STEPPERS > 2
        #endif // E_STEPPERS > 1
    ) {
      lastMotorOn = ms; //... set time to NOW so the fan will turn on
    }

    // Fan off if no steppers have been enabled for CONTROLLERFAN_SECS seconds
    uint8_t speed = (!lastMotorOn || ELAPSED(ms, lastMotorOn + (CONTROLLERFAN_SECS) * 1000UL)) ? 0 : CONTROLLERFAN_SPEED;
    controllerFanSpeed = speed;

    // allows digital or PWM fan output to be used (see M42 handling)
    WRITE(CONTROLLER_FAN_PIN, speed);
    analogWrite(CONTROLLER_FAN_PIN, speed);
  }
}

#endif // USE_CONTROLLER_FAN
