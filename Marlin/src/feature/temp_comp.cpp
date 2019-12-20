#include "temp_comp.h"
#include <math.h>

TempComp temp_comp;

int16_t TempComp::z_offsets_probe[TEMP_COMP_PROBE_MEASUREMENTS] = { 0 };
int16_t TempComp::z_offsets_bed[TEMP_COMP_BED_MEASUREMENTS] =     { 0 };
int16_t TempComp::z_offsets_ext[TEMP_COMP_EXT_MEASUREMENTS] =     { 0 };

TempComp::TempComp()
    : calib_idx(0), init_measurement(0.)
{

}

void TempComp::clear_offsets(MODULE mod) {
    int measurements;
    int16_t* data = nullptr;
    
    if (mod == TEMP_COMP_PROBE) {
        measurements = TEMP_COMP_PROBE_MEASUREMENTS;
        data = z_offsets_probe;
    } else if (mod == TEMP_COMP_BED) {
        measurements = TEMP_COMP_BED_MEASUREMENTS;
        data = z_offsets_bed;
    } else if (mod == TEMP_COMP_EXT) {
        measurements = TEMP_COMP_EXT_MEASUREMENTS;
        data = z_offsets_ext;
    } else {
        return;
    }

    for (calib_idx = 0; calib_idx < measurements; ++calib_idx)
        data[calib_idx] = 0;
    calib_idx = 0;
}

bool TempComp::set_offset(MODULE mod, uint8_t idx, int16_t offset) {
    switch (mod)
    {
    case TEMP_COMP_BED:
        if (idx >= TEMP_COMP_BED_MEASUREMENTS) return false;
        z_offsets_bed[idx] = offset;
        break;
    case TEMP_COMP_PROBE:
        if (idx >= TEMP_COMP_PROBE_MEASUREMENTS) return false;
        z_offsets_probe[idx] = offset;
        break;
    case TEMP_COMP_EXT:
        if (idx >= TEMP_COMP_EXT_MEASUREMENTS) return false;
        z_offsets_ext[idx] = offset;
        break;
    default:
        return false;
    }

    return true;
}

void TempComp::print_offsets() {
    // Print porbe offsets
    uint16_t temp = TEMP_COMP_PROBE_START_TEMP;
    for (int16_t i = -1; i < TEMP_COMP_PROBE_MEASUREMENTS; ++i) {
        if (i < 0) {
            SERIAL_ECHOPAIR("Probe temp: ", temp);
            SERIAL_ECHOLN(" deg C; Offset : 0 um");
        } else {
            SERIAL_ECHOPAIR("Probe temp: ", temp);
            SERIAL_ECHOPAIR(" deg C; Offset : ", z_offsets_probe[i]);
            SERIAL_ECHOLN(" um");
        }
        temp += TEMP_COMP_PROBE_TEMP_RES;
    }
    // Print bed offsets
    temp = TEMP_COMP_BED_START_TEMP;
    for (int16_t i = -1; i < TEMP_COMP_BED_MEASUREMENTS; ++i) {
        if (i < 0) {
            SERIAL_ECHOPAIR("Bed temp: ", temp);
            SERIAL_ECHOLN(" deg C; Offset : 0 um");
        } else {
            SERIAL_ECHOPAIR("Bed temp: ", temp);
            SERIAL_ECHOPAIR(" deg C; Offset : ", z_offsets_bed[i]);
            SERIAL_ECHOLN(" um");
        }
        temp += TEMP_COMP_BED_TEMP_RES;
    }
    // Print hotend offsets
    temp = TEMP_COMP_EXT_START_TEMP;
    for (int16_t i = -1; i < TEMP_COMP_EXT_MEASUREMENTS; ++i) {
        if (i < 0) {
            SERIAL_ECHOPAIR("Extruder temp: ", temp);
            SERIAL_ECHOLN(" deg C; Offset : 0 um");
        } else {
            SERIAL_ECHOPAIR("Extruder temp: ", temp);
            SERIAL_ECHOPAIR(" deg C; Offset : ", z_offsets_ext[i]);
            SERIAL_ECHOLN(" um");
        }
        temp += TEMP_COMP_EXT_TEMP_RES;
    }
}

void TempComp::prepare_new_calibration(float init_meas_z) {
    calib_idx = 0;
    init_measurement = init_meas_z;
}

void TempComp::push_back_new_measurement(MODULE mod, float meas_z) {
    switch (mod) {
        case TEMP_COMP_PROBE:
            if (calib_idx >= TEMP_COMP_PROBE_MEASUREMENTS) return;
            z_offsets_probe[calib_idx++] = static_cast<int16_t>(meas_z*1000. - init_measurement*1000.);
        break;
        case TEMP_COMP_BED:
            if (calib_idx >= TEMP_COMP_BED_MEASUREMENTS) return;
            z_offsets_bed[calib_idx++] = static_cast<int16_t>(meas_z*1000. - init_measurement*1000.);
        break;
        //case TEMP_COMP_EXT:
        //    if (calib_idx >= TEMP_COMP_EXT_MEASUREMENTS) return;
        //    z_offsets_ext[calib_idx++] = (int16_t)((meas_z*1000.) - (init_measurement*1000.));
        //break;
        default:
        break;
    }
}

bool TempComp::finish_calibration(MODULE mod) {
    if (calib_idx < 3) {
        SERIAL_ECHOLN("Insufficient measurements (min. 3).");
        clear_offsets(mod);
        return false;
    }
    
    int measurements, start_temp, res_temp;
    int16_t* data = nullptr;

    if (mod == TEMP_COMP_PROBE) {
        measurements = TEMP_COMP_PROBE_MEASUREMENTS;
        start_temp = TEMP_COMP_PROBE_START_TEMP;
        res_temp = TEMP_COMP_PROBE_TEMP_RES;
        data = z_offsets_probe;
    } else if (mod == TEMP_COMP_BED) {
        measurements = TEMP_COMP_BED_MEASUREMENTS;
        start_temp = TEMP_COMP_BED_START_TEMP;
        res_temp = TEMP_COMP_BED_TEMP_RES;
        data = z_offsets_bed;
    } else {
        return false;
    }

    // Extrapolate
    float k, d;
    if (calib_idx < measurements && linear_regression(mod, k, d)) {
        SERIAL_ECHOPAIR("Got ", calib_idx); SERIAL_ECHOLN(" measurements, applying linear extrapolation for higher temperatures.");
        calib_idx--;
        float temp;
        for (; calib_idx < measurements; ++calib_idx) {
            temp = static_cast<float>(start_temp + static_cast<int>(calib_idx)*res_temp);
            data[calib_idx] = static_cast<int16_t>(k*temp + d);
        }
    } else if (calib_idx < measurements) {
        // Simply use the last measured value for higher temperatures
        SERIAL_ECHOPAIR("Got ", calib_idx); SERIAL_ECHOLN(" measurements, failed to extrapolate for higher temperatures.");
        int16_t last_val = data[calib_idx];
        for (; calib_idx < measurements; ++calib_idx)
            data[calib_idx] = last_val;
    }
    

    // Sanity check
    for (calib_idx = 0; calib_idx < measurements; ++calib_idx) {
        // Restrict the max. offset
        if (abs(data[calib_idx]) > 2000) {
            SERIAL_ECHOLN("Invalid z-offset detected (max. 2mm).");
            clear_offsets(mod);
            return false;
        }
        // Restrict the max. offset difference between two probings
        if (calib_idx > 0 && abs(data[calib_idx-1]-data[calib_idx]) > 800) {
            SERIAL_ECHOLN("Invalid z-offset between two probings detected (max. 0.8mm).");
            clear_offsets(TEMP_COMP_PROBE);
            return false;
        }
    }

    return true;
}

float TempComp::compensate_measurement(MODULE mod, float temp, float meas_z) {
    // Sanity check
    switch (mod) {
        case TEMP_COMP_PROBE:
            if (temp < TEMP_COMP_PROBE_START_TEMP || temp > TEMP_COMP_PROBE_END_TEMP) return meas_z;
        break;
        case TEMP_COMP_BED:
            if (temp < TEMP_COMP_BED_START_TEMP || temp > TEMP_COMP_BED_END_TEMP) return meas_z;
        break;
        case TEMP_COMP_EXT:
            if (temp < TEMP_COMP_EXT_START_TEMP || temp > TEMP_COMP_EXT_END_TEMP) return meas_z;
        break;
        default:
            return meas_z;
        break;
    }
    
    return meas_z - get_offset_for_temperature(mod, temp);
}

float TempComp::get_offset_for_temperature(MODULE mod, float temp) {
    float start_temp, end_temp, res_temp;
    uint8_t measurements;
    const int16_t* data = nullptr;

    if (mod == TEMP_COMP_PROBE) {
        start_temp = TEMP_COMP_PROBE_START_TEMP;
        end_temp = TEMP_COMP_PROBE_END_TEMP;
        res_temp = TEMP_COMP_PROBE_TEMP_RES;
        measurements = TEMP_COMP_PROBE_MEASUREMENTS;
        data = z_offsets_probe;
    } else if (mod == TEMP_COMP_BED) {
        start_temp = TEMP_COMP_BED_START_TEMP;
        end_temp = TEMP_COMP_BED_END_TEMP;
        res_temp = TEMP_COMP_BED_TEMP_RES;
        measurements = TEMP_COMP_BED_MEASUREMENTS;
        data = z_offsets_bed;
    } else if (mod == TEMP_COMP_EXT) {
        start_temp = TEMP_COMP_EXT_START_TEMP;
        end_temp = TEMP_COMP_EXT_END_TEMP;
        res_temp = TEMP_COMP_EXT_TEMP_RES;
        measurements = TEMP_COMP_EXT_MEASUREMENTS;
        data = z_offsets_ext;
    } else {
        return 0.f;
    }
    
    if (temp <= start_temp) return 0.f;
    if (temp >= end_temp) return static_cast<float>(data[measurements-1]) / 1000.f;
    
    // Linear interpolation
    int16_t val1 = 0, val2 = data[0];
    uint8_t idx = 0;
    float meas_temp = start_temp + res_temp;
    while (meas_temp < temp) {
        if (++idx >= measurements) return static_cast<float>(val2) / 1000.f;
        meas_temp += res_temp;
        val1 = val2;
        val2 = data[idx];
    }
    const float factor = (meas_temp - temp) / static_cast<float>(res_temp);
    return (static_cast<float>(val2) - static_cast<float>(val2-val1)*factor) / 1000.f;
}

bool TempComp::linear_regression(MODULE mod, float &k, float &d) {
    if (calib_idx < 2) return false;

    float start_temp, res_temp;
    const int16_t* data;

    if (mod == TEMP_COMP_PROBE) {
        if (calib_idx > TEMP_COMP_PROBE_MEASUREMENTS) return false;
        start_temp = TEMP_COMP_PROBE_START_TEMP;
        res_temp = TEMP_COMP_PROBE_TEMP_RES;
        data = z_offsets_probe;
    } else if (mod == TEMP_COMP_BED) {
        if (calib_idx > TEMP_COMP_BED_MEASUREMENTS) return false;
        start_temp = TEMP_COMP_BED_START_TEMP;
        res_temp = TEMP_COMP_BED_TEMP_RES;
        data = z_offsets_bed;
    } else {
        return false;
    }

    float sum_x, sum_x2, sum_xy, sum_y, xi, yi, denom;
    sum_x = start_temp;
    sum_x2 = start_temp*start_temp;
    sum_xy = sum_y = 0.;

    for (uint8_t i = 0; i < calib_idx; ++i) {
        xi = start_temp + (i+1) * res_temp;
        yi = static_cast<float>(data[i]);
        sum_x += xi;
        sum_x2 += xi * xi;
        sum_xy += xi * yi;
        sum_y += yi;
    }

    denom = (static_cast<float>(calib_idx+1) * sum_x2 - sum_x * sum_x);
    if (fabs(denom) <= 10e-5) {
        // Singularity - unable to solve
        k = 0.;
        d = 0.;
        return false;
    }

    k = (static_cast<float>(calib_idx+1) * sum_xy - sum_x * sum_y) / denom;
    d = (sum_y - k * sum_x) / static_cast<float>(calib_idx+1);

    return true;
}