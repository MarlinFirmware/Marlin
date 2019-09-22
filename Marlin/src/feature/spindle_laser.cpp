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

cutter_power_t SpindleLaser::power; // = 0

#define SPINDLE_LASER_PWM_OFF ((SPINDLE_LASER_PWM_INVERT) ? 255 : 0)

void SpindleLaser::init() {
  OUT_WRITE(SPINDLE_LASER_ENA_PIN, !SPINDLE_LASER_ACTIVE_HIGH); // Init spindle to off
  #if ENABLED(SPINDLE_CHANGE_DIR)
    OUT_WRITE(SPINDLE_DIR_PIN, SPINDLE_INVERT_DIR ? 255 : 0);   // Init rotation to clockwise (M3)
  #endif
  #if ENABLED(SPINDLE_LASER_PWM)
    SET_PWM(SPINDLE_LASER_PWM_PIN);
    analogWrite(pin_t(SPINDLE_LASER_PWM_PIN), SPINDLE_LASER_PWM_OFF);  // set to lowest speed
  #endif
  #if ENABLED(HAL_PWM_FREQ)
    #ifdef SPINDLE_LASER_FREQUENCY
      set_pwm_frequency(pin_t(SPINDLE_LASER_PWM_PIN), SPINDLE_LASER_FREQUENCY);
    #endif
  #endif
}

#if ENABLED(SPINDLE_LASER_PWM)

  /**
   * ocr_val_mode() is used for debugging and to get the points needed to compute the RPM vs ocr_val line
   *
   * it accepts inputs of 0-255
   */
  void SpindleLaser::set_ocr(const uint8_t ocr) {
    WRITE(SPINDLE_LASER_ENA_PIN, SPINDLE_LASER_ACTIVE_HIGH); // turn spindle on (active low)
    analogWrite(pin_t(SPINDLE_LASER_PWM_PIN), ocr ^ SPINDLE_LASER_PWM_OFF);
  }

  uint8_t SpindleLaser::translate_power(const cutter_power_t pwr) {
      constexpr float inv_slope = RECIPROCAL(SPEED_POWER_SLOPE),
                      min_ocr = (SPEED_POWER_MIN - (SPEED_POWER_INTERCEPT)) * inv_slope,  // Minimum allowed
                      max_ocr = (SPEED_POWER_MAX - (SPEED_POWER_INTERCEPT)) * inv_slope;  // Maximum allowed
      int16_t ocr_val;
           if (pwr <= SPEED_POWER_MIN) ocr_val = min_ocr;                               // Use minimum if set below
      else if (pwr >= SPEED_POWER_MAX) ocr_val = max_ocr;                               // Use maximum if set above
      else ocr_val = (pwr - (SPEED_POWER_INTERCEPT)) * inv_slope;                       // Use calculated OCR value
      return ocr_val & 0xFF;                                                              // ...limited to Atmel PWM max
  }

#endif

void SpindleLaser::apply_power(const cutter_power_t inpow) {
  static cutter_power_t last_power_applied = 0;
  if (inpow == last_power_applied) return;
  last_power_applied = inpow;
  #if ENABLED(SPINDLE_LASER_PWM)
    if (ena) {
      set_ocr(translate_power(power));
    }
    else {
      WRITE(SPINDLE_LASER_ENA_PIN, !SPINDLE_LASER_ACTIVE_HIGH);   // Turn spindle off (active low)
      analogWrite(pin_t(SPINDLE_LASER_PWM_PIN), SPINDLE_LASER_PWM_OFF);  // Only write low byte
    }
  #else
    WRITE(SPINDLE_LASER_ENA_PIN, (SPINDLE_LASER_ACTIVE_HIGH) ? enabled() : !enabled());
  #endif
}

#if ENABLED(SPINDLE_CHANGE_DIR)

  void SpindleLaser::set_direction(const bool reverse) {
    const bool dir_state = (reverse == SPINDLE_INVERT_DIR); // Forward (M3) HIGH when not inverted
    #if ENABLED(SPINDLE_STOP_ON_DIR_CHANGE)
      if (enabled() && READ(SPINDLE_DIR_PIN) != dir_state) disable();
    #endif
    WRITE(SPINDLE_DIR_PIN, dir_state);
  }

#endif

#endif // HAS_CUTTER
