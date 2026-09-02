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

#if ENABLED(FAN_SOFT_PWM)
  uint8_t ControllerFan::soft_pwm_speed;
#endif

void ControllerFan::setup() {
  SET_OUTPUT(CONTROLLER_FAN_PIN);
  #if PIN_EXISTS(CONTROLLER_FAN2)
    SET_OUTPUT(CONTROLLER_FAN2_PIN);
  #endif
  #if PIN_EXISTS(CONTROLLER_FAN3)
    SET_OUTPUT(CONTROLLER_FAN3_PIN);
  #endif
  #if PIN_EXISTS(CONTROLLER_FAN4)
    SET_OUTPUT(CONTROLLER_FAN4_PIN);
  #endif
  #if PIN_EXISTS(CONTROLLER_FAN5)
    SET_OUTPUT(CONTROLLER_FAN5_PIN);
  #endif
  #if PIN_EXISTS(CONTROLLER_FAN6)
    SET_OUTPUT(CONTROLLER_FAN6_PIN);
  #endif
  #if PIN_EXISTS(CONTROLLER_FAN7)
    SET_OUTPUT(CONTROLLER_FAN7_PIN);
  #endif
  #if PIN_EXISTS(CONTROLLER_FAN8)
    SET_OUTPUT(CONTROLLER_FAN8_PIN);
  #endif
  #if PIN_EXISTS(CONTROLLER_FAN9)
    SET_OUTPUT(CONTROLLER_FAN9_PIN);
  #endif
  init();
}

void ControllerFan::update() {
  const millis_t ms = millis();

  static millis_t nextFanCheck = 0;     // Last time the state was checked
  if (PENDING(ms, nextFanCheck)) return;
  nextFanCheck = ms + 2500UL; // Not a time critical function, so only check every 2.5s

  /**
   * If any triggers for the controller fan are true...
   *   - At least one stepper driver is enabled
   *   - The heated bed (MOSFET) is enabled
   *   - TEMP_SENSOR_BOARD is reporting >= CONTROLLER_FAN_MIN_BOARD_TEMP
   *   - TEMP_SENSOR_SOC is reporting >= CONTROLLER_FAN_MIN_SOC_TEMP
   */
  static millis_t lastComponentOn = 0;  // Last time a stepper, heater, etc. was turned on
  const ena_mask_t axis_mask = TERN(CONTROLLER_FAN_USE_Z_ONLY, _BV(Z_AXIS), (ena_mask_t)~TERN0(CONTROLLER_FAN_IGNORE_Z, _BV(Z_AXIS)));
  if ( (stepper.axis_enabled.bits & axis_mask)
    #if ALL(HAS_HEATED_BED, CONTROLLER_FAN_BED_HEATING)
      || thermalManager.temp_bed.soft_pwm_amount > 0
    #endif
    #ifdef CONTROLLER_FAN_MIN_BOARD_TEMP
      || thermalManager.wholeDegBoard() >= CONTROLLER_FAN_MIN_BOARD_TEMP
    #endif
    #ifdef CONTROLLER_FAN_MIN_SOC_TEMP
      || thermalManager.wholeDegSoc() >= CONTROLLER_FAN_MIN_SOC_TEMP
    #endif
  ) lastComponentOn = ms; //... set time to NOW so the fan will turn on

  /**
   * Fan Settings. Set fan > 0:
   *  - If AutoMode is on and hot components have been powered for CONTROLLERFAN_IDLE_TIME seconds.
   *  - If System is on idle and idle fan speed settings is activated.
   */
  uint8_t s = settings.auto_mode && lastComponentOn && PENDING(ms, lastComponentOn, SEC_TO_MS(settings.duration))
    ? settings.active_speed
    : settings.idle_speed;

  // Convert 1-255 to the MIN-MAX PWM range
  s = CALC_FAN_SPEED(s);

  // When the fan first starts up it can run at high power for a short period
  #if FAN_KICKSTART_TIME

    static millis_t kick_end_ms = 0;

    if (s > FAN_OFF_PWM) {                      // Is the fan turned on?
      if (!kick_end_ms) {                       // No kickstart yet?
        kick_end_ms = ms + FAN_KICKSTART_TIME;  // Set a future time at which to stop
        s = FAN_KICKSTART_POWER;                // Override the power
      }
      else if (PENDING(ms, kick_end_ms))        // Still waiting for end of kickstart time?
        s = FAN_KICKSTART_POWER;                // Override the power
    }
    else
      kick_end_ms = 0;                          // Reset kick_end_ms for kickstart on next enable

  #endif // FAN_KICKSTART_TIME

  #if ENABLED(FAN_SOFT_PWM)
    soft_pwm_speed = speed >> 1;   // Controller Fan Soft PWM uses 0-127 as 0-100% so cut the 0-255 range in half.
  #else

    #define SET_CONTROLLER_FAN(N) do { \
      if (PWM_PIN(CONTROLLER_FAN##N##_PIN)) hal.set_pwm_duty(pin_t(CONTROLLER_FAN##N##_PIN), speed); \
      else WRITE(CONTROLLER_FAN##N##_PIN, speed > 0);\
    } while (0)

    SET_CONTROLLER_FAN();
    #if PIN_EXISTS(CONTROLLER_FAN2)
      SET_CONTROLLER_FAN(2);
    #endif
    #if PIN_EXISTS(CONTROLLER_FAN3)
      SET_CONTROLLER_FAN(3);
    #endif
    #if PIN_EXISTS(CONTROLLER_FAN4)
      SET_CONTROLLER_FAN(4);
    #endif
    #if PIN_EXISTS(CONTROLLER_FAN5)
      SET_CONTROLLER_FAN(5);
    #endif
    #if PIN_EXISTS(CONTROLLER_FAN6)
      SET_CONTROLLER_FAN(6);
    #endif
    #if PIN_EXISTS(CONTROLLER_FAN7)
      SET_CONTROLLER_FAN(7);
    #endif
    #if PIN_EXISTS(CONTROLLER_FAN8)
      SET_CONTROLLER_FAN(8);
    #endif
    #if PIN_EXISTS(CONTROLLER_FAN9)
      SET_CONTROLLER_FAN(9);
    #endif
  #endif
}

#endif // USE_CONTROLLER_FAN
