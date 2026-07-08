/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

// ============================================================================
// StallGuard Tuning Class (existing implementation continues below)
// ============================================================================

class StallGuardTuning {

public:
    // Stallguard tuning for a given axis (X or Y)
    void tune_axis(const AxisEnum axis);

    bool isDone() { return tuning_finished; }

    AxisEnum tunedAxis() { return axis_tuned; }

    #if ENABLED(FT_MOTION)
        bool sg_ftmSuccess() { return ftm_tuning_success; }
        int16_t get_ftm_threshold() { return ftm_sg_thrs; }
    #endif
    #if HAS_STANDARD_MOTION
        bool sg_stdSuccess() { return std_tuning_success; }
        int16_t get_std_threshold() { return std_sg_thrs; }
    #endif

    #if ANY(X_HAS_SG4, Y_HAS_SG4)
        bool is_stall_detection_active() { return stall_detection_procedure_active; }
        void setStallDetected(bool detected) { stall_detected = detected; }
    #endif

    // Sampling function called in marlin.idle() during tuning
    void sampling();

private:
    
    #if ANY(X_HAS_SG2, Y_HAS_SG2)
        // Tune StallGuard for a given axis with StallGuard2 driver(s) (for TMC2130/2208/2660 or TMC2240 in SG2 mode)
        // Returns the optimal SGT value (-64 to 63) or -128 if tuning failed
        int16_t tune_sg2();
    #endif

    #if ANY(X_HAS_SG4, Y_HAS_SG4)
        // Tune StallGuard for a given axis with StallGuard4 driver(s) (for TMC2209 or TMC2240 in SG4 mode)
        // Returns the optimal SGTHRS value (0-255) or -1 if tuning failed
        int16_t tune_sg4();
    #endif

    // Generate a movement for StallGuard sampling with threshold set to thrs and velocity in mm/s
    void sampling_motion( const uint16_t thrs);

    // Median of the sampling histogram, used to determine the optimal threshold for StallGuard
    uint16_t compute_median(const uint16_t hist_size);

    // Set Homing threshold
    void set_homing_treshold(const uint16_t threshold);

    bool sg_sampling_active;                   // Flag to indicate sampling is active, used to trigger SG_RESULT sampling in marlin.idle()

    bool tuning_success;                       // Flag to indicate successful tuning (global)
    bool tuning_finished = true;               // Flag to indicate end of tuning procedure, init to true for MarlinUI menu

    #if ANY(X_HAS_SG4, Y_HAS_SG4)
        bool stall_detection_procedure_active;
        bool stall_detected;
    #endif
    
    #if HAS_STANDARD_MOTION
        int16_t std_sg_thrs;                   // Homing threshold for standard motion system SG2 (SGT value -64 to 63) or SG4 (SGTHRS value 0-255)
        bool std_tuning_success;
    #endif

    #if ENABLED(FT_MOTION)
        int16_t ftm_sg_thrs;                   // Homing threshold for Fixed Time Motion system SG2 (SGT value -64 to 63) or SG4 (SGTHRS value 0-255)
        bool ftm_tuning_success;
    #endif

    AxisEnum axis_tuned;                      // Axis to be tuned
    float dist_speed_mm;                      // Test travel distance and speed equals homing_feedrate / 60
    
    // Histogram for SG_RESULT sampling during tuning, max SG_RESULT value is 510 for SG4, 1023 for SG2};
    #if ANY(X_HAS_SG2, Y_HAS_SG2)
        uint16_t sampling_hist[1024];
    #else
        uint16_t sampling_hist[511];
    #endif
};

extern StallGuardTuning stallguard_tuner;

