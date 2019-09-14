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

#include "../inc/MarlinConfigPre.h"
#include "../module/planner.h"

constexpr uint8_t all_on = 0xFF, all_off = 0x00;

class Backlash {
public:
  #ifdef BACKLASH_DISTANCE_MM
    #if ENABLED(BACKLASH_GCODE)
      static float distance_mm[XYZ];
    #else
      static const float distance_mm[XYZ];
      //static constexpr float distance_mm[XYZ] = BACKLASH_DISTANCE_MM; // compiler barks at this
    #endif
  #endif
  #if ENABLED(BACKLASH_GCODE)
    static uint8_t correction;
    #ifdef BACKLASH_SMOOTHING_MM
      static float smoothing_mm;
    #endif
    static inline void set_correction(const float &v) { correction = _MAX(0, _MIN(1.0, v)) * all_on; }
    static inline float get_correction() { return float(ui8_to_percent(correction)) / 100.0f; }
  #else
    static constexpr uint8_t correction = (BACKLASH_CORRECTION) * 0xFF;
    #ifdef BACKLASH_SMOOTHING_MM
      static constexpr float smoothing_mm = BACKLASH_SMOOTHING_MM;
    #endif
    static inline void set_correction(float) { }
    static inline float get_correction() { return float(ui8_to_percent(correction)) / 100.0f; }
  #endif

  #if ENABLED(MEASURE_BACKLASH_WHEN_PROBING)
    private:
      static float measured_mm[XYZ];
      static uint8_t measured_count[XYZ];
    public:
      static void measure_with_probe();
  #endif

  static inline float get_measurement(const AxisEnum a) {
    // Return the measurement averaged over all readings
    return (
      #if ENABLED(MEASURE_BACKLASH_WHEN_PROBING)
        measured_count[a] > 0 ? measured_mm[a] / measured_count[a] :
      #endif
      0
    );
    #if DISABLED(MEASURE_BACKLASH_WHEN_PROBING)
      UNUSED(a);
    #endif
  }

  static inline bool has_measurement(const AxisEnum a) {
    return (false
      #if ENABLED(MEASURE_BACKLASH_WHEN_PROBING)
        || (measured_count[a] > 0)
      #endif
    );
    #if DISABLED(MEASURE_BACKLASH_WHEN_PROBING)
      UNUSED(a);
    #endif
  }

  static inline bool has_any_measurement() {
    return has_measurement(X_AXIS) || has_measurement(Y_AXIS) || has_measurement(Z_AXIS);
  }

  void add_correction_steps(const int32_t &da, const int32_t &db, const int32_t &dc, const uint8_t dm, block_t * const block);
};

extern Backlash backlash;
