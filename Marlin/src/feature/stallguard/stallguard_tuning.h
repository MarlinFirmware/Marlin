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

/**
 * feature/stallguard_tuning.h - StallGuard Tuning
 * Automatically configures stall sensitivity (SGTHRS) for StallGuard capable drivers
 * by measuring SG_RESULT during a controlled movement
 */

#include "../../inc/MarlinConfigPre.h"

#include "../tmc_util.h"
#include "../../module/stepper.h"
#include "../../module/planner.h"
#include "../../module/motion.h"
#include "../../module/stepper/trinamic.h"

#define TEST_TRAVEL_MM 50.0f

// ============================================================================
// StallGuard Tuning Class (existing implementation continues below)
// ============================================================================

class StallGuardTuning {

public:
    // Stallguard tuning for a given axis (X or Y)
    void tune_axis(const AxisEnum axis);
    
    // Sampling function called in marlin.idle() during tuning
    void sampling();

    bool is_Done() { return !sg_tune_active; }
    bool is_Success() { return tuning_success; }
    int8_t get_treshold() { return sg_thrs; }

private:

    // Tune StallGuard for a given axis with StallGuard2 driver(s) (for TMC2130/2208/2660 or TMC2240 in SG2 mode)
    // Returns the optimal SGT value (-64 to 63) or -128 if tuning failed
    void tune_sg2(const float velocity);

    // Tune StallGuard for a given axis with StallGuard4 driver(s) (for TMC2209 or TMC2240 in SG4 mode)
    // Returns the optimal SGTHRS value (0-255) or -1 if tuning failed
    void tune_sg4(const float velocity);

    // Get number of TMC drivers for the specified axis
    uint8_t get_nb_drivers_for_axis(const AxisEnum axis);

    // Set Homing threshold
    void set_homing_treshold(const uint16_t threshold);

    bool sg_tune_active;                         // Flag to indicate if tuning is in progress, used to trigger SG_RESULT sampling in marlin.idle()
    static bool tuning_success;                  // Flag to indicate successful tuning

    static int16_t sg_thrs;                      // Homing threshold for SG2 (SGT value -64 to 63) or SG4 (SGTHRS value 0-255)
    static AxisEnum axis_tuned;                  // Axis to be tuned

    static uint8_t driver_count;                // Tuned axis number of drivers or number of drivers for a CoreXY printer
    static uint8_t stallguard_type;             // Type of StallGuard
    
    static uint32_t t_start;                    // Start time
    static uint16_t sampling_hist[511];         // Histogram for SG_RESULT sampling during tuning, max SG_RESULT value is 510 for SG4
    static uint16_t sampling_total;             // Total samples collected for percentile calculation
    static uint32_t sg2_sgr_sum;                // Sum of SG_RESULT values for SG2 tuning
    static uint16_t sgr_min;                    // Minimum SG_RESULT observed during tuning, used for SG2
};

extern StallGuardTuning stallguard_tuner;

