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

/**
 * feature/spindle_laser.cpp
 */

#include "../inc/MarlinConfig.h"

#if HAS_CUTTER

#include "spindle_laser.h"

SpindleLaser cutter;
uint8_t SpindleLaser::power;
bool SpindleLaser::isReady;                                           // Ready to apply power setting from the UI to OCR
cutter_power_t SpindleLaser::menuPower,                               // Power set via LCD menu in PWM, PERCENT, or RPM
               SpindleLaser::unitPower;                               // LCD status power in PWM, PERCENT, or RPM

#if ENABLED(MARLIN_DEV_MODE)
  cutter_frequency_t SpindleLaser::frequency;                         // setting PWM frequency; range: 2K - 50K
#endif
#define SPINDLE_LASER_PWM_OFF ((SPINDLE_LASER_PWM_INVERT) ? 255 : 0)

//
// Init the cutter to a safe OFF state
//
void SpindleLaser::init() {
  OUT_WRITE(SPINDLE_LASER_ENA_PIN, !SPINDLE_LASER_ACTIVE_HIGH);       // Init spindle to off
  #if ENABLED(SPINDLE_CHANGE_DIR)
    OUT_WRITE(SPINDLE_DIR_PIN, SPINDLE_INVERT_DIR ? 255 : 0);         // Init rotation to clockwise (M3)
  #endif
  #if ENABLED(SPINDLE_LASER_PWM)
    SET_PWM(SPINDLE_LASER_PWM_PIN);
    analogWrite(pin_t(SPINDLE_LASER_PWM_PIN), SPINDLE_LASER_PWM_OFF); // set to lowest speed
  #endif
  #if ENABLED(HAL_CAN_SET_PWM_FREQ) && defined(SPINDLE_LASER_FREQUENCY)
    set_pwm_frequency(pin_t(SPINDLE_LASER_PWM_PIN), SPINDLE_LASER_FREQUENCY);
    TERN_(MARLIN_DEV_MODE, frequency = SPINDLE_LASER_FREQUENCY);
  #endif
}

#if ENABLED(SPINDLE_LASER_PWM)
  /**
   * Set the cutter PWM directly to the given ocr value
   */
  void SpindleLaser::set_ocr(const uint8_t ocr) {
    WRITE(SPINDLE_LASER_ENA_PIN, SPINDLE_LASER_ACTIVE_HIGH);        // turn spindle on
    analogWrite(pin_t(SPINDLE_LASER_PWM_PIN), ocr ^ SPINDLE_LASER_PWM_OFF);
  }
  void SpindleLaser::ocr_off() {
    WRITE(SPINDLE_LASER_ENA_PIN, !SPINDLE_LASER_ACTIVE_HIGH);       // Turn spindle off
    analogWrite(pin_t(SPINDLE_LASER_PWM_PIN), SPINDLE_LASER_PWM_OFF); // Only write low byte
  }
#endif

//
// Set cutter ON/OFF state (and PWM) to the given cutter power value
//
void SpindleLaser::apply_power(const uint8_t opwr) {
  static uint8_t last_power_applied = 0;
  if (opwr == last_power_applied) return;
  last_power_applied = opwr;
  power = opwr;
  #if ENABLED(SPINDLE_LASER_PWM)
    if (cutter.unitPower == 0 && CUTTER_UNIT_IS(RPM)) {
      ocr_off();
      isReady = false;
    }
    else if (enabled() || ENABLED(CUTTER_POWER_RELATIVE)) {
      set_ocr(power);
      isReady = true;
    }
    else {
      ocr_off();
      isReady = false;
    }
  #else
    WRITE(SPINDLE_LASER_ENA_PIN, enabled() == SPINDLE_LASER_ACTIVE_HIGH);
    isReady = true;
  #endif
}

#if ENABLED(SPINDLE_CHANGE_DIR)
  //
  // Set the spindle direction and apply immediately
  // Stop on direction change if SPINDLE_STOP_ON_DIR_CHANGE is enabled
  //
  void SpindleLaser::set_direction(const bool reverse) {
    const bool dir_state = (reverse == SPINDLE_INVERT_DIR); // Forward (M3) HIGH when not inverted
    if (TERN0(SPINDLE_STOP_ON_DIR_CHANGE, enabled()) && READ(SPINDLE_DIR_PIN) != dir_state) disable();
    WRITE(SPINDLE_DIR_PIN, dir_state);
  }
#endif

#endif // HAS_CUTTER
