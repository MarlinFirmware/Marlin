/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#if ENABLED(SPINDLE_FEATURE)
  #define _MSG_CUTTER(M) MSG_SPINDLE_##M
#else
  #define _MSG_CUTTER(M) MSG_LASER_##M
#endif
#define MSG_CUTTER(M) _MSG_CUTTER(M)

#if SPEED_POWER_MAX > 255
  #define cutter_power_t   uint16_t
  #define CUTTER_MENU_TYPE uint16_5
#else
  #define cutter_power_t   uint8_t
  #define CUTTER_MENU_TYPE uint8
#endif

class SpindleLaser {
public:
  static cutter_power_t power;

  static void init();

  static inline bool enabled() { return !!power; }

  static inline void set_power(const uint8_t pwr) { power = pwr; update_output(); }

  static inline void set_enabled(const bool enable) { set_power(enable ? 255 : 0); }

  //static bool active() { return READ(SPINDLE_LASER_ENA_PIN) == SPINDLE_LASER_ACTIVE_HIGH; }

  static void update_output();

  #if ENABLED(SPINDLE_LASER_PWM)
    static void set_ocr(const uint8_t ocr);
    static inline void set_ocr_power(const uint8_t pwr) { power = pwr; set_ocr(pwr); }
  #endif

  // Wait for spindle to spin up or spin down
  static inline void power_delay(const bool on) { safe_delay(on ? SPINDLE_LASER_POWERUP_DELAY : SPINDLE_LASER_POWERDOWN_DELAY); }

  #if ENABLED(SPINDLE_CHANGE_DIR)
    static void set_direction(const bool reverse);
  #else
    static inline void set_direction(const bool reverse) { UNUSED(reverse); }
  #endif

  static inline void disable() { set_enabled(false); }
  static inline void enable_forward() { set_direction(false); set_enabled(true); }
  static inline void enable_reverse() { set_direction(true); set_enabled(true); }

};

extern SpindleLaser cutter;
