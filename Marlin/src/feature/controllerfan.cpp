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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "../inc/MarlinConfig.h"

#if ENABLED(USE_CONTROLLER_FAN)

#include "controllerfan.h"
#include "../module/stepper.h"
#include "../module/temperature.h"

ControllerFan controllerFan;

uint8_t ControllerFan::speed;

#if ENABLED(CONTROLLER_FAN_EDITABLE)
  controllerFan_settings_t ControllerFan::settings; // {0}
 #else
   const controllerFan_settings_t &ControllerFan::settings = controllerFan_defaults;
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

    // If any triggers for the controller fan are true...
    //   - At least one stepper driver is enabled
    //   - The heated bed is enabled
    //   - TEMP_SENSOR_BOARD is reporting >= CONTROLLER_FAN_MIN_BOARD_TEMP
    const ena_mask_t axis_mask = TERN(CONTROLLER_FAN_USE_Z_ONLY, _BV(Z_AXIS), ~TERN0(CONTROLLER_FAN_IGNORE_Z, _BV(Z_AXIS)));
    if ( (stepper.axis_enabled.bits & axis_mask)
      || TERN0(HAS_HEATED_BED, thermalManager.temp_bed.soft_pwm_amount > 0)
      || TERN0(HAS_CONTROLLER_FAN_MIN_BOARD_TEMP, thermalManager.wholeDegBoard() >= CONTROLLER_FAN_MIN_BOARD_TEMP)
    ) lastMotorOn = ms; //... set time to NOW so the fan will turn on

    // Fan Settings. Set fan > 0:
    //  - If AutoMode is on and steppers have been enabled for CONTROLLERFAN_IDLE_TIME seconds.
    //  - If System is on idle and idle fan speed settings is activated.
    set_fan_speed(
      settings.auto_mode && lastMotorOn && PENDING(ms, lastMotorOn + SEC_TO_MS(settings.duration))
      ? settings.active_speed : settings.idle_speed
    );

    // Allow digital or PWM fan output (see M42 handling)
    WRITE(CONTROLLER_FAN_PIN, speed);
    analogWrite(pin_t(CONTROLLER_FAN_PIN), speed);
  }
}

#endif // USE_CONTROLLER_FAN
