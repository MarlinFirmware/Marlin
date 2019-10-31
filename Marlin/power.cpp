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

/**
 * power.cpp - power control
 */

#include "MarlinConfig.h"

#if ENABLED(AUTO_POWER_CONTROL)

#include "power.h"
#include "temperature.h"
#include "stepper_indirection.h"

Power powerManager;

millis_t Power::lastPowerOn;

bool Power::is_power_needed() {
  #if ENABLED(AUTO_POWER_FANS)
    for (uint8_t i = 0; i < FAN_COUNT; i++) if (fanSpeeds[i] > 0) return true;
  #endif

  #if ENABLED(AUTO_POWER_E_FANS)
    HOTEND_LOOP() if (thermalManager.autofan_speed[e] > 0) return true;
  #endif

  #if ENABLED(AUTO_POWER_CONTROLLERFAN) && HAS_CONTROLLER_FAN && ENABLED(USE_CONTROLLER_FAN)
    if (controllerFanSpeed > 0) return true;
  #endif

  // If any of the drivers or the bed are enabled...
  if (X_ENABLE_READ == X_ENABLE_ON || Y_ENABLE_READ == Y_ENABLE_ON || Z_ENABLE_READ == Z_ENABLE_ON
    #if HAS_HEATED_BED
      || thermalManager.soft_pwm_amount_bed > 0
    #endif
      #if HAS_X2_ENABLE
        || X2_ENABLE_READ == X_ENABLE_ON
      #endif
      #if HAS_Y2_ENABLE
        || Y2_ENABLE_READ == Y_ENABLE_ON
      #endif
      #if HAS_Z2_ENABLE
        || Z2_ENABLE_READ == Z_ENABLE_ON
      #endif
      || E0_ENABLE_READ == E_ENABLE_ON
      #if E_STEPPERS > 1
        || E1_ENABLE_READ == E_ENABLE_ON
        #if E_STEPPERS > 2
            || E2_ENABLE_READ == E_ENABLE_ON
          #if E_STEPPERS > 3
              || E3_ENABLE_READ == E_ENABLE_ON
            #if E_STEPPERS > 4
                || E4_ENABLE_READ == E_ENABLE_ON
            #endif
          #endif
        #endif
      #endif
  ) return true;

  HOTEND_LOOP() if (thermalManager.degTargetHotend(e) > 0) return true;
  #if HAS_HEATED_BED
    if (thermalManager.degTargetBed() > 0) return true;
  #endif

  return false;
}

void Power::check() {
  static millis_t nextPowerCheck = 0;
  millis_t ms = millis();
  if (ELAPSED(ms, nextPowerCheck)) {
    nextPowerCheck = ms + 2500UL;
    if (is_power_needed())
      power_on();
    else if (!lastPowerOn || ELAPSED(ms, lastPowerOn + (POWER_TIMEOUT) * 1000UL))
      power_off();
  }
}

void Power::power_on() {
  lastPowerOn = millis();
  if (!powersupply_on) {
    PSU_PIN_ON();

    #if HAS_TRINAMIC
      delay(100); // Wait for power to settle
      restore_stepper_drivers();
    #endif
  }
}

void Power::power_off() {
  if (powersupply_on) PSU_PIN_OFF();
}

#endif // AUTO_POWER_CONTROL
