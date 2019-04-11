/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#pragma once

#include "../inc/MarlinConfigPre.h"

#if ENABLED(BACKLASH_COMPENSATION)
  #include "../module/planner.h"

  class Backlash {
    public:
      #if ENABLED(BACKLASH_GCODE)
        static uint8_t backlash_correction;
        #ifdef BACKLASH_DISTANCE_MM
          static float backlash_distance_mm[XYZ];
        #endif
        #ifdef BACKLASH_SMOOTHING_MM
          static float backlash_smoothing_mm;
        #endif

        void set_correction(float v) {backlash_correction = MAX(0, MIN(1.0, v)) * all_on;}
      #else
        static constexpr uint8_t backlash_correction = (BACKLASH_CORRECTION) * 0xFF;
        #ifdef BACKLASH_DISTANCE_MM
          static constexpr float backlash_distance_mm[XYZ] = BACKLASH_DISTANCE_MM;
        #endif
        #ifdef BACKLASH_SMOOTHING_MM
          static constexpr float backlash_smoothing_mm = BACKLASH_SMOOTHING_MM;
        #endif

        void set_correction(float) {}
      #endif

      float get_correction() {return float(ui8_to_percent(backlash_correction)) / 100;}


      #if ENABLED(MEASURE_BACKLASH_WHEN_PROBING)
        private:
          static float backlash_measured_mm[];
          static uint8_t backlash_measured_num[];

        public:
          static void measure_with_probe();
      #endif

      static float get_measurement(const uint8_t e) {
        // Returns the measurement averaged over all readings
       return
         #if ENABLED(MEASURE_BACKLASH_WHEN_PROBING)
           backlash_measured_num[e] > 0 ? backlash_measured_mm[e] / backlash_measured_num[e] :
         #endif
         0;
      }

      static bool has_measurement(const uint8_t e) {
        return
          #if ENABLED(MEASURE_BACKLASH_WHEN_PROBING)
            backlash_measured_num[e] > 0;
          #else
            false;
          #endif
      }

      void add_correction_steps(const int32_t &da, const int32_t &db, const int32_t &dc, const uint8_t dm, block_t * const block);
  };

  extern Backlash backlash;
#endif // ENABLED(BACKLASH_COMPENSATION)
