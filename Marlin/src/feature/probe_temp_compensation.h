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

#include "../inc/MarlinConfig.h"

enum TempSensorID : uint8_t {
  TSI_PROBE,
  TSI_BED,
  #if ENABLED(USE_TEMP_EXT_COMPENSATION)
    TSI_EXT,
  #endif
  TSI_COUNT
};

typedef struct {
  uint8_t measurements; // Max. number of measurements to be stored (35 - 80°C)
  float   temp_res,     // Resolution in °C between measurements
          start_temp,   // Base measurement; z-offset == 0
          end_temp;
} temp_calib_t;

/**
 * Probe temperature compensation implementation.
 * Z-probes like the P.I.N.D.A V2 allow for compensation of
 * measurement errors/shifts due to changed temperature.
 */
class ProbeTempComp {
  public:

    static constexpr temp_calib_t cali_info_init[TSI_COUNT] = {
        {  10,  5,  30,  30 + 10 *  5 },       // Probe
        {  10,  5,  60,  60 + 10 *  5 },       // Bed
      #if ENABLED(USE_TEMP_EXT_COMPENSATION)
        {  20,  5, 180, 180 +  5 * 20 }        // Extruder
      #endif
    };
    static const temp_calib_t cali_info[TSI_COUNT];

    // Where to park nozzle to wait for probe cooldown
    static constexpr xyz_pos_t park_point = { PTC_PARK_POS_X, PTC_PARK_POS_Y, PTC_PARK_POS_Z };

    static constexpr int  max_bed_temp         = PTC_MAX_BED_TEMP,  // Max temperature to avoid heating errors

                          // XY coordinates of nozzle for probing the bed
                          measure_point_x      = PTC_PROBE_POS_X,   // X-coordinate to probe
                          measure_point_y      = PTC_PROBE_POS_Y,   // Y-coordinate to probe
                          //measure_point_x    = 12.0f,             // X-coordinate to probe on MK52 magnetic heatbed
                          //measure_point_y    =  7.3f,             // Y-coordinate to probe on MK52 magnetic heatbed

                          probe_calib_bed_temp = max_bed_temp,      // Bed temperature while calibrating probe
                          bed_calib_probe_temp = 30;                // Probe temperature while calibrating bed

    static int16_t *sensor_z_offsets[TSI_COUNT],
                   z_offsets_probe[cali_info_init[TSI_PROBE].measurements], // (µm)
                   z_offsets_bed[cali_info_init[TSI_BED].measurements];     // (µm)

    #if ENABLED(USE_TEMP_EXT_COMPENSATION)
      static int16_t z_offsets_ext[cali_info_init[TSI_EXT].measurements];   // (µm)
    #endif

    static inline void reset_index() { calib_idx = 0; };
    static inline uint8_t get_index() { return calib_idx; }
    static void clear_offsets(const TempSensorID tsi);
    static inline void clear_all_offsets() {
      clear_offsets(TSI_BED);
      clear_offsets(TSI_PROBE);
      #if ENABLED(USE_TEMP_EXT_COMPENSATION)
        clear_offsets(TSI_EXT);
      #endif
    }
    static bool set_offset(const TempSensorID tsi, const uint8_t idx, const int16_t offset);
    static void print_offsets();
    static void prepare_new_calibration(const float &init_meas_z);
    static void push_back_new_measurement(const TempSensorID tsi, const float &meas_z);
    static bool finish_calibration(const TempSensorID tsi);
    static void compensate_measurement(const TempSensorID tsi, const float &temp, float &meas_z);

  private:
    static uint8_t calib_idx;

    /**
     * Base value. Temperature compensation values will be deltas
     * to this value, set at first probe.
     */
    static float init_measurement;

    static float get_offset_for_temperature(const TempSensorID tsi, const float &temp);

    /**
     * Fit a linear function in measured temperature offsets
     * to allow generating values of higher temperatures.
     */
    static bool linear_regression(const TempSensorID tsi, float &k, float &d);
};

extern ProbeTempComp temp_comp;
