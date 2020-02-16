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

ControllerFan fanController;

void ControllerFan::reset() {
  return init();
}

void ControllerFan::init() {
  settings_fan.controllerFan_Speed        = CONTROLLERFAN_SPEED;      // 0-255 - 255 == fullspeed; Controller fan speed on motors enabled
  settings_fan.controllerFan_Idle_Speed   = CONTROLLERFAN_IDLE_SPEED; // 0-255 - 255 == fullspeed; Controller fan Idle speed if all motors are disabled
  settings_fan.controllerFan_Duration     = CONTROLLERFAN_SECS;       // Duration in seconds for the fan to run after all motors are disabled
  settings_fan.controllerFan_AutoMode     = true;
}

bool ControllerFan::state() {
  return iFanSpeed > 0;
}

void ControllerFan::update() {
  static millis_t lastMotorOn = 0, // Last time a motor was turned on
                  nextMotorCheck = 0; // Last time the state was checked
  const millis_t ms = millis();
  if (ELAPSED(ms, nextMotorCheck)) {
    nextMotorCheck = ms + 2500UL; // Not a time critical function, so only check every 2.5s

    const bool xory = X_ENABLE_READ() == bool(X_ENABLE_ON) || Y_ENABLE_READ() == bool(Y_ENABLE_ON);

    // If any of the drivers or the bed are enabled...
    if (xory || Z_ENABLE_READ() == bool(Z_ENABLE_ON)
      #if HAS_HEATED_BED
        || thermalManager.temp_bed.soft_pwm_amount > 0
      #endif
      #if HAS_X2_ENABLE
        || X2_ENABLE_READ() == bool(X_ENABLE_ON)
      #endif
      #if HAS_Y2_ENABLE
        || Y2_ENABLE_READ() == bool(Y_ENABLE_ON)
      #endif
      #if HAS_Z2_ENABLE
        || Z2_ENABLE_READ() == bool(Z_ENABLE_ON)
      #endif
      #if HAS_Z3_ENABLE
        || Z3_ENABLE_READ() == bool(Z_ENABLE_ON)
      #endif
      #if HAS_Z4_ENABLE
        || Z4_ENABLE_READ() == bool(Z_ENABLE_ON)
      #endif
      #if E_STEPPERS
        #define _OR_ENABLED_E(N) || E##N##_ENABLE_READ() == bool(E_ENABLE_ON)
        REPEAT(E_STEPPERS, _OR_ENABLED_E)
      #endif
    ) {
      lastMotorOn = ms; //... set time to NOW so the fan will turn on
    }

    // Fan Settings - Set fan > 0:
    //        - If AutoMode in on and steppers has been enabled for CONTROLLERFAN_SECS seconds.
    //        - If System is on idle and idle fan speed settings is activated
    if( settings_fan.controllerFan_AutoMode && lastMotorOn && 
        PENDING(ms, lastMotorOn + (settings_fan.controllerFan_Duration) * 1000UL) && 
        settings_fan.controllerFan_Speed  >= CONTROLLERFAN_SPEED_MIN ) {
      iFanSpeed= settings_fan.controllerFan_Speed;
    }
    else if( settings_fan.controllerFan_Idle_Speed  >= CONTROLLERFAN_SPEED_MIN ) {
      iFanSpeed= settings_fan.controllerFan_Idle_Speed;
    } else iFanSpeed= 0; // Fan OFF

    // allows digital or PWM fan output to be used (see M42 handling)
    WRITE(CONTROLLER_FAN_PIN, iFanSpeed );
    analogWrite(pin_t(CONTROLLER_FAN_PIN), iFanSpeed );
  }
}

#endif // USE_CONTROLLER_FAN
