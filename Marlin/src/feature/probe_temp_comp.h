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

// Probe temperature calibration constants
#ifndef PTC_SAMPLE_COUNT
  #define PTC_SAMPLE_COUNT 10U
#endif
#ifndef PTC_SAMPLE_RES
  #define PTC_SAMPLE_RES 5.0f
#endif
#ifndef PTC_SAMPLE_START
  #define PTC_SAMPLE_START 30.0f
#endif
#define PTC_SAMPLE_END ((PTC_SAMPLE_START) + (PTC_SAMPLE_COUNT) * (PTC_SAMPLE_RES))

// Bed temperature calibration constants
#ifndef BTC_PROBE_TEMP
  #define BTC_PROBE_TEMP 30.0f
#endif
#ifndef BTC_SAMPLE_COUNT
  #define BTC_SAMPLE_COUNT 10U
#endif
#ifndef BTC_SAMPLE_STEP
  #define BTC_SAMPLE_RES 5.0f
#endif
#ifndef BTC_SAMPLE_START
  #define BTC_SAMPLE_START 60.0f
#endif
#define BTC_SAMPLE_END ((BTC_SAMPLE_START) + (BTC_SAMPLE_COUNT) * (BTC_SAMPLE_RES))

#ifndef PTC_PROBE_HEATING_OFFSET
  #define PTC_PROBE_HEATING_OFFSET 0.5f
#endif

#ifndef PTC_PROBE_RAISE
  #define PTC_PROBE_RAISE 10.0f
#endif

static constexpr temp_calib_t cali_info_init[TSI_COUNT] = {
    {  PTC_SAMPLE_COUNT, PTC_SAMPLE_RES, PTC_SAMPLE_START, PTC_SAMPLE_END },       // Probe
    {  BTC_SAMPLE_COUNT, BTC_SAMPLE_RES, BTC_SAMPLE_START, BTC_SAMPLE_END },       // Bed
  #if ENABLED(USE_TEMP_EXT_COMPENSATION)
    {  20,  5, 180, 180 +  5 * 20 }        // Extruder
  #endif
};

class ProbeTempComp {
  public:

    static const temp_calib_t cali_info[TSI_COUNT];

    // Where to park nozzle to wait for probe cooldown
    static constexpr xyz_pos_t park_point = PTC_PARK_POS;

    // XY coordinates of nozzle for probing the bed
    static constexpr xy_pos_t measure_point    = PTC_PROBE_POS;     // Coordinates to probe
                            //measure_point    = { 12.0f, 7.3f };   // Coordinates for the MK52 magnetic heatbed

    static constexpr int  probe_calib_bed_temp = BED_MAX_TARGET,  // Bed temperature while calibrating probe
                          bed_calib_probe_temp = BTC_PROBE_TEMP;  // Probe temperature while calibrating bed

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
      TERN_(USE_TEMP_EXT_COMPENSATION, clear_offsets(TSI_EXT));
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
