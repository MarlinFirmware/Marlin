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

#include "../../inc/MarlinConfig.h"

#if ENABLED(STALLGUARD_TUNING)
#include "stallguard_tuning.h"
#include "../../module/endstops.h"

#if ENABLED(FT_MOTION)
    #include "../../module/ft_motion.h"
#endif

StallGuardTuning stallguard_tuner; // Global instance of the StallGuardTuning class

// ---------------------------------------------------------------------------
// Automatic stallguard threshold tuning procedure (StallGuard2 and StallGuard4)
// Cartesian or CoreXY printers
// Only for X and Y axes 
// ---------------------------------------------------------------------------
void StallGuardTuning::tune_axis(const AxisEnum axis) {
    tuning_success = false;
    tuning_finished = false;
    uint8_t stallguard_type;

    axis_tuned = axis;
    dist_speed_mm = motion.homing_feedrate_mm_m[axis] / 60;
    
    // For a 2 drivers axis, current will be set to the same value on both drivers
    // Only the first driver will be checked for the current:  X_CURRENT or Y_CURRENT
    // Tune at 50% of the configured current to optimize back EMF
    // Minimal current value is 400 mA
    uint16_t current;      

    #if CORE_IS_XY

        current = max(400, X_CURRENT / 2);
        stepperX.rms_current(current);
        stepperY.rms_current(current);

        stallguard_type = tmc_stallguard_version(stepperX);

        // Test if one stepper driver has both SG2 and SG4
        // Remember that stallguard_type is already set to stepperX version
        // Set the SG2-SG4 capable driver to the stallguard version of the other stepper driver 

        #if AXIS_IS_SG2_SG4(X) && !AXIS_IS_SG2_SG4(Y)
            stallguard_type = tmc_stallguard_version(stepperY);
            tmc_enable_stallguard(stepperX, stallguard_type);
            tmc_enable_stallguard(stepperY);

        #elif !AXIS_IS_SG2_SG4(X) && AXIS_IS_SG2_SG4(Y)
            tmc_enable_stallguard(stepperX);
            tmc_enable_stallguard(stepperY, stallguard_type);

        #else        
            tmc_enable_stallguard(stepperX);
            tmc_enable_stallguard(stepperY);
        #endif
    
    #else
        if (axis == X_AXIS) {
            current = max(400, X_CURRENT / 2);
            stepperX.rms_current(current);

            stallguard_type = tmc_stallguard_version(stepperX);
            
            // Enbable stallguard mode on the driver(s)
            // Test if one stepper driver has both SG2 and SG4 in case of 2 stepper drivers
            // Remember that stallguard_type is already set to stepperX version 
                
            #if X2_SENSORLESS
                stepperX2.rms_current(current);

                #if X_HAS_SG2_SG4 && !X2_HAS_SG2_SG4
                    stallguard_type = tmc_stallguard_version(stepperX2);
                    tmc_enable_stallguard(stepperX, stallguard_type);
                    tmc_enable_stallguard(stepperX2);

                #elif !X_HAS_SG2_SG4 && X2_HAS_SG2_SG4
                    tmc_enable_stallguard(stepperX);
                    tmc_enable_stallguard(stepperX2, stallguard_type);

                #else        
                    tmc_enable_stallguard(stepperX);
                    tmc_enable_stallguard(stepperX2);
                #endif
            #else
                tmc_enable_stallguard(stepperX);
            #endif
        } else {
            current = max(400, Y_CURRENT / 2);
            stepperY.rms_current(current);

            stallguard_type = tmc_stallguard_version(stepperY);
            
            #if Y2_SENSORLESS
                // Set stepper current and set the tuning value
                stepperY2.rms_current(current);

                #if Y_HAS_SG2_SG4 && !Y2_HAS_SG2_SG4
                    stallguard_type = tmc_stallguard_version(stepperY2);
                    tmc_enable_stallguard(stepperY, stallguard_type);
                    tmc_enable_stallguard(stepperY2);

                #elif !Y_HAS_SG2_SG4 && Y2_HAS_SG2_SG4
                    tmc_enable_stallguard(stepperY);
                    tmc_enable_stallguard(stepperY2, stallguard_type);

                #else        
                    tmc_enable_stallguard(stepperY);
                    tmc_enable_stallguard(stepperY2);
                #endif
            #else
                tmc_enable_stallguard(stepperY);
            #endif
        }
    #endif

    // Local helper to select the proper tuning routine based on stallguard version
    // Send logs to the terminal
    auto do_tune = [&]() {
        int16_t sg_thrs = 0;

        switch (stallguard_type)
        {
        case SG_STALLGUARD2:
            #if ANY(X_HAS_SG2, Y_HAS_SG2)
                sg_thrs = tune_sg2();
            #endif
            break;

        case SG_STALLGUARD4:
            #if ANY(X_HAS_SG4, Y_HAS_SG4)
                sg_thrs = tune_sg4();
            #endif
            break;
        }
        
        bool using_ftMotion = ENABLED(NO_STANDARD_MOTION) || TERN0(FT_MOTION, ftMotion.cfg.active);

        if (tuning_success)
            SERIAL_ECHOLN((axis?"Y":"X"), using_ftMotion ? " FTM" : " Std Motion", " SG threshold: ", sg_thrs);
        else
            SERIAL_ECHOLN((axis?"Y":"X"), using_ftMotion ? " FTM" : " Std Motion"," SG Tuning failed");

        return sg_thrs;
    };

    #if ENABLED(NO_STANDARD_MOTION)
        ftm_sg_thrs = do_tune();
        ftm_tuning_success = tuning_success;
    #else
        // If FT_MOTION is available, run first pass with FTM active
        // then second with FTM inactive. Otherwise run a single pass.
        #if ENABLED(FT_MOTION)
            // Save FTM  state
            bool old_ftm = ftMotion.cfg.active;
            // Set FTM active
            ftMotion.cfg.active = true;
            // Tune
            ftm_sg_thrs = do_tune();
            ftm_tuning_success = tuning_success;
            // Set FTM inactive
            ftMotion.cfg.active = false;
            tuning_success = false;
        #endif
        // Tune for standard motion
        std_sg_thrs = do_tune();
        std_tuning_success = tuning_success;
    #endif

    // Restore FT_MOTION cfg.active
    TERN_(FT_MOTION, ftMotion.cfg.active = old_ftm;)

    // Reset drivers
    reset_trinamic_drivers();
    tuning_finished =true;

}

#if ANY(X_HAS_SG2, Y_HAS_SG2)
// StallGuard2 tuning procedure
// For Stallguard2, SGTHRS (sensitivity) is progressively increased or decreased until SG_RESULT is in range 80-120
// SGTHRS range is -64 to 63
// The final SGTHRS possibly differs between standard motion and FT_MOTION.
// In this later case as the proposed values are in the range, you can choose one or another
int16_t StallGuardTuning::tune_sg2() { 
    int16_t sg_thrs = 0; // Indifferent value for SGT, free move
    
    // Main test loop : adapt SGTHRS until we see SG_RESULT within the prespecified range between 80 - 120 (safety margin)
    // Values are from Stallguard2 TMC datasheet (SG_RESULT before stall 0-100)
    // The first move is a free move with a SGTHRS value of 0
    // SGTHRS is then progressively increased or decreased until SG_RESULT is in the prespecified range
    while(!tuning_success) {

        // Reset sampling histogram
        memset(sampling_hist, 0, sizeof(sampling_hist));

        // Set homing threshold to sg_thrs and set motion for sampling
        sampling_motion(sg_thrs);

        // Compute median SG_RESULT
        uint16_t sg_result_test = compute_median(sizeof(sampling_hist));
        
        if (sg_result_test > 120) {
            if (++sg_thrs > 63) {
                break; // No valid SGTHRS found, tuning failed
            }
            continue;
        }

        if (sg_result_test < 80) {
            if (--sg_thrs < -64) {
                break; // No valid SGTHRS found, tuning failed
            }
            continue;
        }

        tuning_success = true;
    }
    return sg_thrs;
}
#endif  // Any HAS_SG2

#if ANY(X_HAS_SG4, Y_HAS_SG4)
// StallGuard4 sensibility threshold SGTHRS range is 0-255
// SG_RESULT range 0-510. Represents motor load. Higher SG_RESULT is, lower is the load
// Do one forward and one backward move, collect SG results in a histogram and compute median value
// Median SG_RESULT represents the value of the cruise travel regardless of the chosen motion system
// To detect a motor stall, SG_RESULT must be < SGTHRS * 2 (Stallguard4 specs)
// Take one sixth of median SG_RESULT, progressively increase SGTHRS and find the first value that will trigger the endstop during motion (in fact during accelration)
// The final SGTHRS possibly differs between standard motion and FT_MOTION.
// In this later case the best value is the lowest SG_THRS proposed
int16_t StallGuardTuning::tune_sg4() {

    // Reset sampling histogram
    memset(sampling_hist, 0, sizeof(sampling_hist));

    sampling_motion(0);

    // Compute median of SG_RESULT values sampled during the free move, divide by 2 to have a SG_THRS
    uint16_t sg_thrs_median = compute_median(sizeof(sampling_hist)) / 2;

    // Set the first SGTHRS value to 33% of median to avoid stall on acceleration or deceleration
    int16_t sg_thrs = sg_thrs_median / 3; 
    xyze_pos_t target_pos = motion.position;

    stall_detected = false;
    stall_detection_procedure_active = true; // Set the stall_detection_procedure_active to true, used in planner.endstop_triggered()
    
    // Enable endstops to detect the stall
    endstops.enable(true);

    while (!stall_detected && sg_thrs < sg_thrs_median) {
        
        // Set the test threshold and generate the motion
        set_homing_treshold(sg_thrs);

        target_pos[axis_tuned] += dist_speed_mm;
        planner.buffer_line(target_pos, dist_speed_mm, 0);
        target_pos[axis_tuned] -= dist_speed_mm;
        planner.buffer_line(target_pos, dist_speed_mm, 0);

        planner.synchronize();

        sg_thrs++;
    }
    
    // Tuning is finished, disable endstops
    endstops.enable(false);

    stall_detection_procedure_active = false; // Set the stall_detection_procedure_active to false
    if(stall_detected) {
        tuning_success = true;
        sg_thrs -=10; // Reduce the threshold by 10 to avoid false stall detection during homing
    }
    return sg_thrs;
}
#endif // Any HAS_SG4

// Forward and backward move of dist_speed_mm to sample SG_RESULT
// Set the homing threshold to the specified value for the sampling move
void StallGuardTuning::sampling_motion( const uint16_t thrs) {
    xyze_pos_t target_pos = motion.position;

    set_homing_treshold(thrs);

    target_pos[axis_tuned] += dist_speed_mm;
    planner.buffer_line(target_pos, dist_speed_mm, 0);
    target_pos[axis_tuned] -= dist_speed_mm;
    planner.buffer_line(target_pos, dist_speed_mm, 0);

    // Set tuning flag to start SG_RESULT sampling
    // Synchronize planner will allow sampling via marlin.idle() until the moves end
    sg_sampling_active = true;
    planner.synchronize();

    // End of sampling 
    sg_sampling_active = false;
}

// These function is called by marlin.idle() for sampling SG_RESULT during the tuning procedure
// SG_RESULT range is 0-1023 for StallGuard2 and 0-510 for StallGuard4
// SG_RESULT is axis related
void StallGuardTuning::sampling() {
    if (!sg_sampling_active) return; // Sample only when sampling is active

    uint16_t sg_result = 0;

    #if CORE_IS_XY
        sg_result = tmc_sg_result(stepperX);
        sampling_hist[sg_result]++;
        sg_result = tmc_sg_result(stepperY);
        sampling_hist[sg_result]++; 
    #else
        if (axis_tuned == X_AXIS) {
            sg_result = tmc_sg_result(stepperX);
            sampling_hist[sg_result]++;
            #if X2_SENSORLESS
                sg_result = tmc_sg_result(stepperX2);
                sampling_hist[sg_result]++;
            #endif
        } else {
            sg_result = tmc_sg_result(stepperY);
            sampling_hist[sg_result]++;
            #if Y2_SENSORLESS
                sg_result = tmc_sg_result(stepperY2);
                sampling_hist[sg_result]++;
            #endif
        }
    #endif
}

// Compute medain value of the sampling histogram
uint16_t StallGuardTuning::compute_median(const uint16_t hist_size) {
    uint16_t median_treshold = 0;
    uint16_t median = 0;

    for (uint16_t v = 0; v < hist_size; v++) {
        median_treshold += sampling_hist[v];
    }
    median_treshold = median_treshold / 2 ;

    int16_t cumulative = 0;

    for(int16_t v = 0; v < hist_size; v++) {
        cumulative += sampling_hist[v];
        if (cumulative > median_treshold) {
            median = v;
            break;
        }
    }
    return median;
}

void StallGuardTuning::set_homing_treshold(const uint16_t threshold) {
    #if CORE_IS_XY
        stepperX.homing_threshold(threshold);
        stepperY.homing_threshold(threshold);
    #else
        if (axis_tuned == X_AXIS) {
            stepperX.homing_threshold(threshold);
            #if X2_SENSORLESS
                stepperX2.homing_threshold(threshold);
            #endif
        } else {
            stepperY.homing_threshold(threshold);
            #if Y2_SENSORLESS
                stepperY2.homing_threshold(threshold);
            #endif
        }
    #endif
}

#endif // ENABLED(STALLGUARD_TUNING)
