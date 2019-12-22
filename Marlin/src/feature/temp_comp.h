#pragma once

#include "../inc/MarlinConfig.h"

/* Heaters */
#define TEMP_COMP_MAX_BED_TEMP TEMP_COMPENSATION_MAX_BED_TEMP // To avoid heating errors we use this max temperature

/* Probe calibration */
#define TEMP_COMP_PROBE_START_TEMP          30    // Base measurement; z-offset == 0
#define TEMP_COMP_PROBE_TEMP_RES             5    // Resolution in °C between measurements
#define TEMP_COMP_PROBE_MEASUREMENTS        10    // Max. number of measurements to be stored (35 - 80°C)
#define TEMP_COMP_PROBE_END_TEMP     (TEMP_COMP_PROBE_MEASUREMENTS*TEMP_COMP_PROBE_TEMP_RES+TEMP_COMP_PROBE_START_TEMP)

/* Bed calibration */
#define TEMP_COMP_BED_START_TEMP            60    // Base measurement; z-offset == 0
#define TEMP_COMP_BED_TEMP_RES               5    // Resolution in °C between measurements
#define TEMP_COMP_BED_MEASUREMENTS          10    // Max. number of measurements to be stored (65 - 110°C)
#define TEMP_COMP_BED_END_TEMP       (TEMP_COMP_BED_MEASUREMENTS*TEMP_COMP_BED_TEMP_RES+TEMP_COMP_BED_START_TEMP)

/* Extruder/hotend calibration */
// Note: calibrating this cannot be automated, therefore we need to set it manually -> use less measurements
#define TEMP_COMP_EXT_START_TEMP           180    // Base measurement; z-offset == 0
#define TEMP_COMP_EXT_TEMP_RES              20    // Resolution in °C between measurements
#define TEMP_COMP_EXT_MEASUREMENTS           5    // Max. number of measurements to be stored (200 - 280°C)
#define TEMP_COMP_EXT_END_TEMP     (TEMP_COMP_EXT_MEASUREMENTS*TEMP_COMP_EXT_TEMP_RES+TEMP_COMP_EXT_START_TEMP)

/* XY coordinates of nozzel for probing the bed */
//#define TEMP_COMP_MEASURE_POINT_X      12.0F // X-coordinate on MK52 magnetic heatbed to be probed
//#define TEMP_COMP_MEASURE_POINT_Y       7.3F // Y-oordinate on MK52 magnetic heatbed to be probed
#define TEMP_COMP_MEASURE_POINT_X       TEMP_COMPENSATION_PROBE_POS_X // X-coordinate on heatbed to be probed
#define TEMP_COMP_MEASURE_POINT_Y       TEMP_COMPENSATION_PROBE_POS_Y // Y-oordinate on heatbed to be probed

/* XYZ coordinates to park nozzle to wait for probe cooldown */
#define TEMP_COMP_PARK_POINT_X       TEMP_COMPENSATION_PARK_POS_X // X-coordinate to park print head
#define TEMP_COMP_PARK_POINT_Y       TEMP_COMPENSATION_PARK_POS_Y // Y-oordinate to park print head
#define TEMP_COMP_PARK_POINT_Z       TEMP_COMPENSATION_PARK_POS_Z // Z-oordinate to park print head

#define TEMP_COMP_PROBE_CALIB_BED_TEMP TEMP_COMP_MAX_BED_TEMP // Bed temperature while calibrating probe
#define TEMP_COMP_BED_CALIB_PROBE_TEMP                     30 // Probe temperature while calibrating bed

/**
 * Probe temperature compensation implementation.
 * Z-probes like the P.I.N.D.A V2 allow for compensation of 
 * measurement errors/shifts due to changed temperature.
 */
class TempComp {
  public:
    enum MODULE {
        TEMP_COMP_PROBE,
        TEMP_COMP_BED,
        TEMP_COMP_EXT
    };
    static int16_t z_offsets_probe[TEMP_COMP_PROBE_MEASUREMENTS]; // [um]
    static int16_t z_offsets_bed[TEMP_COMP_BED_MEASUREMENTS];     // [um]
    static int16_t z_offsets_ext[TEMP_COMP_EXT_MEASUREMENTS];     // [um]
    

    TempComp();
    void reset_index() { calib_idx = 0; };
    uint8_t get_index() { return calib_idx; }
    void clear_offsets(MODULE mod);
    bool set_offset(MODULE mod, uint8_t idx, int16_t offset);
    void print_offsets();
    void prepare_new_calibration(float init_meas_z);
    void push_back_new_measurement(MODULE mod, float meas_z);
    bool finish_calibration(MODULE mod);
    float compensate_measurement(MODULE mod, float temp, float meas_z);
  
  private:
    uint8_t calib_idx;
    /**
     * Base value. Temperature compensation values will be deltas
     * to this value, set at first probe.
     */
    float init_measurement;

    float get_offset_for_temperature(MODULE mod, float temp);
    /**
     * Fit a linear function in measured temperature offsets
     * to allow generating values of higher temperatures.
     */
    bool linear_regression(MODULE mod, float &k, float &d);
}; // class TempComp

extern TempComp temp_comp;