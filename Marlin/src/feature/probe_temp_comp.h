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
#pragma once

#include "../inc/MarlinConfig.h"

enum TempSensorID : uint8_t {
  #if ENABLED(USE_TEMP_PROBE_COMPENSATION)
    TSI_PROBE,
  #endif
  #if ENABLED(USE_TEMP_BED_COMPENSATION)
    TSI_BED,
  #endif
  #if ENABLED(USE_TEMP_EXT_COMPENSATION)
    TSI_EXT,
  #endif
  TSI_COUNT
};

typedef struct {
  uint8_t measurements;       // Max. number of measurements to be stored (35 - 80°C)
  celsius_t temp_resolution,  // Resolution in °C between measurements
            start_temp;       // Base measurement; z-offset == 0
} temp_calib_t;

/**
 * Probe temperature compensation implementation.
 * Z-probes like the P.I.N.D.A V2 allow for compensation of
 * measurement errors/shifts due to changed temperature.
 */

class ProbeTempComp {
  public:

    static constexpr temp_calib_t cali_info[TSI_COUNT] = {
      #if ENABLED(USE_TEMP_PROBE_COMPENSATION)
        { PTC_SAMPLE_COUNT, PTC_SAMPLE_RES, PTC_SAMPLE_START },   // Probe
      #endif
      #if ENABLED(USE_TEMP_BED_COMPENSATION)
        { BTC_SAMPLE_COUNT, BTC_SAMPLE_RES, BTC_SAMPLE_START },   // Bed
      #endif
      #if ENABLED(USE_TEMP_EXT_COMPENSATION)
        { ETC_SAMPLE_COUNT, ETC_SAMPLE_RES, ETC_SAMPLE_START }, // Extruder
      #endif
    };

    static int16_t *sensor_z_offsets[TSI_COUNT];
    #if ENABLED(USE_TEMP_PROBE_COMPENSATION)
      static int16_t z_offsets_probe[PTC_SAMPLE_COUNT]; // (µm)
    #endif
    #if ENABLED(USE_TEMP_BED_COMPENSATION)
      static int16_t z_offsets_bed[BTC_SAMPLE_COUNT];   // (µm)
    #endif
    #if ENABLED(USE_TEMP_EXT_COMPENSATION)
      static int16_t z_offsets_ext[ETC_SAMPLE_COUNT];   // (µm)
    #endif

    static inline void reset_index() { calib_idx = 0; };
    static inline uint8_t get_index() { return calib_idx; }
    static void reset_to_default();
    static void clear_offsets(const TempSensorID tsi);
    static inline void clear_all_offsets() {
      TERN_(USE_TEMP_PROBE_COMPENSATION, clear_offsets(TSI_PROBE));
      TERN_(USE_TEMP_BED_COMPENSATION, clear_offsets(TSI_BED));
      TERN_(USE_TEMP_EXT_COMPENSATION, clear_offsets(TSI_EXT));
    }
    static bool set_offset(const TempSensorID tsi, const uint8_t idx, const int16_t offset);
    static void print_offsets();
    static void prepare_new_calibration(const_float_t init_meas_z);
    static void push_back_new_measurement(const TempSensorID tsi, const_float_t meas_z);
    static bool finish_calibration(const TempSensorID tsi);
    static void compensate_measurement(const TempSensorID tsi, const celsius_t temp, float &meas_z);

  private:
    static uint8_t calib_idx;

    /**
     * Base value. Temperature compensation values will be deltas
     * to this value, set at first probe.
     */
    static float init_measurement;

    /**
     * Fit a linear function in measured temperature offsets
     * to allow generating values of higher temperatures.
     */
    static bool linear_regression(const TempSensorID tsi, float &k, float &d);
};

extern ProbeTempComp temp_comp;
