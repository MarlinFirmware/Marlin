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

class SpindleLaser {
public:
  static bool isOn;                             //  state to determine when to apply setPower to power
  static cutter_power_t power;
  static cutter_setPower_t setPower;            //  spindle/laser menu set power; in PWM, Percentage or RPM
  #if ENABLED(MARLIN_DEV_MODE)
    static cutter_frequency_t frequency;        //  set PWM frequency; range: 2K-50K
  #endif

  static cutter_setPower_t interpret_power(const float pwr) {     // convert speed/power to configured PWM, Percentage or RPM in relative or normal range
    #if CUTTER_DISPLAY_IS(PERCENT)
      return (pwr / SPEED_POWER_MAX) * 100;                                               // to percent
    #elif CUTTER_DISPLAY_IS(RPM)                                                          // to RPM is unaltered
      return pwr;
    #else                                                                                 // to PWM
      #if ENABLED(CUTTER_POWER_RELATIVE)
        return (pwr - SPEED_POWER_MIN) / (SPEED_POWER_MAX - SPEED_POWER_MIN) * 255;     // using rpm range as relative percentage
      #else
        return (pwr / SPEED_POWER_MAX) * 255;
      #endif
    #endif
  }
  /**
  * Translate speed/power --> percentage --> PWM value
  **/
  static cutter_power_t translate_power(const float pwr) {
    float pwrpc;
    #if CUTTER_DISPLAY_IS(PERCENT)
      pwrpc = pwr;
    #elif CUTTER_DISPLAY_IS(RPM)            // RPM to percent
     #if ENABLED(CUTTER_POWER_RELATIVE)
        pwrpc = (pwr - SPEED_POWER_MIN) / (SPEED_POWER_MAX - SPEED_POWER_MIN) * 100;
      #else
        pwrpc = pwr / SPEED_POWER_MAX * 100;
      #endif
    #else
      return pwr;                           // PWM
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
                    min_ocr = (spmin - (SPEED_POWER_INTERCEPT)) * inv_slope,         // Minimum allowed
                    max_ocr = (spmax - (SPEED_POWER_INTERCEPT)) * inv_slope;         // Maximum allowed
    float ocr_val;
    if (pwrpc < spmin) ocr_val = min_ocr;                                           // Use minimum if set below
    else if (pwrpc > spmax) ocr_val = max_ocr;                                      // Use maximum if set above
    else ocr_val = (pwrpc - (SPEED_POWER_INTERCEPT)) * inv_slope;                   // Use calculated OCR value
    return ocr_val;                                                                 // ...limited to Atmel PWM max
  }

  static void init();

  // Modifying this function should update everywhere
  static inline bool enabled(const cutter_power_t pwr) { return pwr > 0; }
  static inline bool enabled() { return enabled(power); }
  #if ENABLED(MARLIN_DEV_MODE)
    static inline void refresh_frequency() { set_pwm_frequency(pin_t(SPINDLE_LASER_PWM_PIN), frequency); }
  #endif
  static void apply_power(const cutter_power_t inpow);

  FORCE_INLINE static void refresh() { apply_power(power); }
  FORCE_INLINE static void set_power(const cutter_power_t pwr) { power = pwr; refresh(); }

  static inline void set_enabled(const bool enable) { set_power(enable ? (power ?: interpret_power(SPEED_POWER_STARTUP)) : 0); }

  #if ENABLED(SPINDLE_LASER_PWM)
    static void set_ocr(const uint8_t ocr);
    static inline void set_ocr_power(const uint8_t pwr) { power = pwr; set_ocr(pwr); }
    // static uint8_t translate_power(const cutter_power_t pwr); // Used by update output for power->OCR translation
  #endif

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
    static inline void enable_forward() { isOn = true; setPower ? (power = setPower) : (setPower = interpret_power(SPEED_POWER_STARTUP)); set_direction(false); set_enabled(true); }
    static inline void enable_reverse() { isOn = true; setPower ? (power = setPower) : (setPower = interpret_power(SPEED_POWER_STARTUP)); set_direction(true); set_enabled(true); }
  #endif

  #if ENABLED(LASER_POWER_INLINE)
    // Force disengage planner power control
    static inline void inline_disable() { planner.laser.status = 0; planner.laser.power = 0; isOn = false;}

    // Inline modes of all other functions; all enable planner inline power control
    static inline void inline_enabled(const bool enable) { enable ? inline_power(SPEED_POWER_STARTUP) : inline_ocr_power(0); }

    static void inline_power(const cutter_power_t pwr) {
      #if ENABLED(SPINDLE_LASER_PWM)
        inline_ocr_power(translate_power(pwr));
      #else
        planner.laser.status = enabled(pwr) ? 0x03 : 0x01;
        planner.laser.power = pwr;
      #endif
    }

    static inline void inline_direction(const bool reverse) { UNUSED(reverse); } // TODO is this ever going to be needed

    #if ENABLED(SPINDLE_LASER_PWM)
      static inline void inline_ocr_power(const uint8_t pwr) {
        planner.laser.status = pwr ? 0x03 : 0x01;
        planner.laser.power = pwr;
      }
    #endif
  #endif

  static inline void kill() {
    TERN_(LASER_POWER_INLINE, inline_disable());
    disable();
  }
};

extern SpindleLaser cutter;
