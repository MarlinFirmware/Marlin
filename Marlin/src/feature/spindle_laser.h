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
#pragma once

/**
 * feature/spindle_laser.h
 * Support for Laser Power or Spindle Power & Direction
 */

#include "../inc/MarlinConfig.h"

#include "spindle_laser_types.h"

#if ENABLED(LASER_POWER_INLINE)
  #include "../module/planner.h"
#endif

#ifndef SPEED_POWER_INTERCEPT
  #define SPEED_POWER_INTERCEPT 0
#endif
#define SPEED_POWER_FLOOR TERN(CUTTER_POWER_RELATIVE, SPEED_POWER_MIN, 0)

#define _MAP(N,S1,S2,D1,D2) ((N)*_MAX((D2)-(D1),0)/_MAX((S2)-(S1),1)+(D1))

class SpindleLaser {
public:
  static constexpr float
    min_pct = 100.0f * TERN(CUTTER_POWER_RELATIVE, 0, (SPEED_POWER_MIN)) / (SPEED_POWER_MAX),
    max_pct = TERN(SPINDLE_FEATURE, 100, SPEED_POWER_MAX);
  static constexpr float pct_to_ocr(const float pct) {
    return (pct - (SPEED_POWER_INTERCEPT)) * RECIPROCAL(SPEED_POWER_SLOPE);
  }
  static constexpr float min_ocr() { return pct_to_ocr(min_pct); }  // Minimum allowed
  static constexpr float max_ocr() { return pct_to_ocr(max_pct); }  // Maximum allowed

  static constexpr float upower_to_percent(const float upwr) {
    return _MAP(upwr, SPEED_POWER_FLOOR, SPEED_POWER_MAX, 0, 100);
  }

  // Convert a given speed/power from the native unit
  // to the display/edit unit: PWM, Percent, or RPM (rel/abs).
  static constexpr
  cutter_power_t dpower_to_upower(const float dpwr) {
    return (
      #if CUTTER_DISPLAY_IS(RPM)              // RPM is also the native unit
        dpwr
      #elif CUTTER_DISPLAY_IS(PERCENT)        // Percent to anything
        _MAP(dpwr, 0, 100, 0, SPEED_POWER_MAX)
      #else                                   // PWM / OCR to allowed range
        _MAP(dpwr, 0, 255, SPEED_POWER_FLOOR, SPEED_POWER_MAX)
      #endif
    );
  }

  // Convert a given display/edit value to native unit,
  // which can also be PWM, Percent, or RPM (rel/abs).
  static constexpr
  cutter_displayPower_t upower_to_dpower(const float upwr) {
    return (
      #if CUTTER_DISPLAY_IS(RPM)              // RPM is also the native unit
        upwr
      #elif CUTTER_DISPLAY_IS(PERCENT)        // Anything to percent
        _MAP(upwr, 0, SPEED_POWER_MAX, 0, 100)
      #else                                   // Allowed range to PWM / OCR
        _MAP(upwr, SPEED_POWER_MIN, SPEED_POWER_MAX, 0, 255)
      #endif
    );
  }

  static constexpr cutter_displayPower_t dpower_min() { return upower_to_dpower(SPEED_POWER_MIN); }
  static constexpr cutter_displayPower_t dpower_max() { return upower_to_dpower(SPEED_POWER_MAX); }

  static bool isOn;                           // State to determine when to apply power to OCR
  static cutter_power_t power;

  #if ENABLED(MARLIN_DEV_MODE)
    static cutter_frequency_t frequency;      // Set PWM frequency; range: 2K-50K
  #endif

  static cutter_displayPower_t displayPower;  // Power as displayed in PWM, Percentage or RPM

  static void init();

  // Modifying this function should update everywhere
  static inline bool enabled(const cutter_power_t upwr) { return upwr > 0; }
  static inline bool enabled() { return enabled(power); }
  #if ENABLED(MARLIN_DEV_MODE)
    static inline void refresh_frequency() { set_pwm_frequency(pin_t(SPINDLE_LASER_PWM_PIN), frequency); }
  #endif
  static void apply_power(const cutter_power_t upwr);

  FORCE_INLINE static void refresh() { apply_power(power); }
  FORCE_INLINE static void set_power(const cutter_power_t upwr) { power = upwr; refresh(); }

  static inline void set_enabled(const bool enable) { set_power(enable ? (power ?: SPEED_POWER_STARTUP) : 0); }

  #if ENABLED(SPINDLE_LASER_PWM)

    static void set_ocr(const uint8_t ocr);
    static inline void set_ocr_power(const uint8_t ocr) { power = ocr; set_ocr(ocr); }

    // Used to update output for power->OCR translation
    static inline constexpr
    uint8_t upower_to_ocr(const float &upwr) {
      #if CUTTER_DISPLAY_IS(PWM255)
        return upwr;
      #else
        return (upower_to_percent(upwr) < min_pct) ? min_ocr()  // Use minimum if set below
             : (upower_to_percent(upwr) > max_pct) ? max_ocr()  // Use maximum if set above
             : pct_to_ocr(upower_to_percent(upwr)); // Calculate OCR value
      #endif
    }

  #endif // SPINDLE_LASER_PWM

  // Wait for spindle to spin up or spin down
  static inline void power_delay(const bool on) {
    #if DISABLED(LASER_POWER_INLINE)
      safe_delay(on ? SPINDLE_LASER_POWERUP_DELAY : SPINDLE_LASER_POWERDOWN_DELAY);
    #endif
  }

  #if ENABLED(SPINDLE_CHANGE_DIR)
    static void set_direction(const bool reverse);
  #else
    static inline void set_direction(const bool) {}
  #endif

  static inline void disable() { isOn = false; set_enabled(false); }

  #if HAS_LCD_MENU

    static inline void enable_with_dir(const bool reverse) {
      isOn = true;
      const cutter_power_t upwr = dpower_to_upower(displayPower);
      if (upwr)
        power = upwr;
      else
        displayPower = upower_to_dpower(SPEED_POWER_STARTUP);
      set_direction(reverse);
      set_enabled(true);
    }
    FORCE_INLINE static void enable_forward() { enable_with_dir(false); }
    FORCE_INLINE static void enable_reverse() { enable_with_dir(true); }

    #if ENABLED(SPINDLE_LASER_PWM)
      static inline void update_from_dpower() { if (isOn) power = dpower_to_upower(displayPower); }
    #endif

  #endif

  #if ENABLED(LASER_POWER_INLINE)
    /**
     * Inline power adds extra fields to the planner block
     * to handle laser power and scale to movement speed.
     */

    // Force disengage planner power control
    static inline void inline_disable() {
      isOn = false;
      planner.laser.status = 0;
      planner.laser.power = 0;
    }

    // Inline modes of all other functions; all enable planner inline power control
    static inline void set_inline_enabled(const bool enable) {
      if (enable)
        inline_power(SPEED_POWER_STARTUP);
      else
        TERN(SPINDLE_LASER_PWM, inline_ocr_power, inline_power)(0);
    }

    // Set the power for subsequent movement blocks
    static void inline_power(const cutter_power_t upwr) {
      #if ENABLED(SPINDLE_LASER_PWM)
        inline_ocr_power(upower_to_ocr(upwr));
      #else
        planner.laser.status = enabled(upwr) ? 0x03 : 0x01;
        planner.laser.power = upwr;
      #endif
    }

    static inline void inline_direction(const bool) { /* never */ }

    #if ENABLED(SPINDLE_LASER_PWM)
      static inline void inline_ocr_power(const uint8_t ocrpwr) {
        planner.laser.status = ocrpwr ? 0x03 : 0x01;
        planner.laser.power = ocrpwr;
      }
    #endif
  #endif

  static inline void kill() {
    TERN_(LASER_POWER_INLINE, inline_disable());
    disable();
  }
};

extern SpindleLaser cutter;
