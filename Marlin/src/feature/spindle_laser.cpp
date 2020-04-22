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

cutter_power_t SpindleLaser::power;
bool SpindleLaser::isOn;                                                       // state to determine when to apply setPower to power
#if ENABLED(SPINDLE_LASER_PWM)
cutter_setPower_t SpindleLaser::setPower = interpret_power(SPEED_POWER_MIN); // spindle/laser speed/power control in PWM, Percentage or RPM
#else
  cutter_setPower_t SpindleLaser::setPower = interpret_power(SPEED_POWER_STARTUP);   // spindle/laser speed/power control in PWM, Percentage or RPM
#endif
#if ENABLED(MARLIN_DEV_MODE)
  cutter_frequency_t SpindleLaser::frequency;                                  // setting PWM frequency; range: 2K - 50K
#endif
#define SPINDLE_LASER_PWM_OFF ((SPINDLE_LASER_PWM_INVERT) ? 255 : 0)

//
// Init the cutter to a safe OFF state
//
void SpindleLaser::init() {
  OUT_WRITE(SPINDLE_LASER_ENA_PIN, !SPINDLE_LASER_ACTIVE_HIGH); // Init spindle to off
  #if ENABLED(SPINDLE_CHANGE_DIR)
    OUT_WRITE(SPINDLE_DIR_PIN, SPINDLE_INVERT_DIR ? 255 : 0);   // Init rotation to clockwise (M3)
  #endif
  #if ENABLED(SPINDLE_LASER_PWM)
    SET_PWM(SPINDLE_LASER_PWM_PIN);
    analogWrite(pin_t(SPINDLE_LASER_PWM_PIN), SPINDLE_LASER_PWM_OFF);  // set to lowest speed
  #endif
  #if ENABLED(HAL_CAN_SET_PWM_FREQ) && defined(SPINDLE_LASER_FREQUENCY)
    set_pwm_frequency(pin_t(SPINDLE_LASER_PWM_PIN), SPINDLE_LASER_FREQUENCY);
    TERN_(MARLIN_DEV_MODE, frequency = SPINDLE_LASER_FREQUENCY);
  #endif
}

#if ENABLED(SPINDLE_LASER_PWM)

  /**
  * Set the cutter PWM directly to the given ocr value
  **/
  void SpindleLaser::set_ocr(const uint8_t ocr) {
    WRITE(SPINDLE_LASER_ENA_PIN, SPINDLE_LASER_ACTIVE_HIGH); // turn spindle on
    analogWrite(pin_t(SPINDLE_LASER_PWM_PIN), ocr ^ SPINDLE_LASER_PWM_OFF);
  }

cutter_power_t SpindleLaser::translate_power(const float pwr)
  {
    float pwrpc;
  #if CUTTER_DISPLAY_IS(PERCENT)
    pwrpc = pwr;
  #elif CUTTER_DISPLAY_IS(RPM) // RPM to percent
  #if ENABLED(CUTTER_POWER_RELATIVE)
    pwrpc = (pwr - SPEED_POWER_MIN) / (SPEED_POWER_MAX - SPEED_POWER_MIN) * 100;
  #else
    pwrpc = pwr / SPEED_POWER_MAX * 100;
  #endif
  #else
    return pwr; // PWM
  #endif

  #if ENABLED(SPINDLE_FEATURE)
  #if ENABLED(CUTTER_POWER_RELATIVE)
    constexpr float spmin = 0;
  #else
    constexpr float spmin = SPEED_POWER_MIN / SPEED_POWER_MAX * 100; // convert to percentage
  #endif
    constexpr float spmax = 100;
  #else
    constexpr float spmin = SPEED_POWER_MIN;
    constexpr float spmax = SPEED_POWER_MAX;
  #endif

  constexpr float inv_slope = RECIPROCAL(SPEED_POWER_SLOPE),
                  min_ocr = (spmin - (SPEED_POWER_INTERCEPT)) * inv_slope, // Minimum allowed
      max_ocr = (spmax - (SPEED_POWER_INTERCEPT)) * inv_slope;             // Maximum allowed
  float ocr_val;
  if (pwrpc < spmin)
    ocr_val = min_ocr; // Use minimum if set below
  else if (pwrpc > spmax)
    ocr_val = max_ocr; // Use maximum if set above
  else
    ocr_val = (pwrpc - (SPEED_POWER_INTERCEPT)) * inv_slope; // Use calculated OCR value
  return ocr_val;                                            // ...limited to Atmel PWM max
}
#endif

//
// Set cutter ON state (and PWM) to the given cutter power value
//
static cutter_power_t last_power_applied = 0;
void SpindleLaser::apply_power(const cutter_power_t inpow) {
  if (inpow == last_power_applied) return;
  last_power_applied = inpow;
  #if ENABLED(SPINDLE_LASER_PWM)
    if (enabled())
      set_ocr(translate_power(inpow));
    else {
      WRITE(SPINDLE_LASER_ENA_PIN, !SPINDLE_LASER_ACTIVE_HIGH);                           // Turn spindle off
      analogWrite(pin_t(SPINDLE_LASER_PWM_PIN), SPINDLE_LASER_PWM_OFF);                   // Only write low byte
    }
  #else
    WRITE(SPINDLE_LASER_ENA_PIN, (SPINDLE_LASER_ACTIVE_HIGH) ? enabled() : !enabled());
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
