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

#include "../../inc/MarlinConfig.h"

#if ENABLED(STALLGUARD_TUNING)
#include "stallguard_tuning.h"

bool StallGuardTuning::tuning_success;
uint16_t StallGuardTuning::sampling_hist[511];
uint16_t StallGuardTuning::sampling_total;
uint32_t StallGuardTuning::sg2_sgr_sum;
uint16_t StallGuardTuning::sgr_min;
uint8_t StallGuardTuning::driver_count;
uint8_t StallGuardTuning::stallguard_type;
uint32_t StallGuardTuning::t_start;
int16_t StallGuardTuning::sg_thrs;
AxisEnum StallGuardTuning::axis_tuned;

StallGuardTuning stallguard_tuner; // Global instance of the StallGuardTuning class

// ---------------------------------------------------------------------------
// Automatic stallguard threshold tuning procedure
// Compute X_CURRENT_HOME / Y_CURRENT_HOME if needed
// Only for X and Y axes
// ---------------------------------------------------------------------------
void StallGuardTuning::tune_axis(const AxisEnum axis) {
    tuning_success = false;
    
    uint16_t saved_current_0;
    uint16_t current;

    bool restore_stealth_0;
    
    #if X2_SENSORLESS || Y2_SENSORLESS || CORE_IS_XY
        bool restore_stealth_1;
        uint16_t saved_current_1;
    #endif

    // Get number of drivers for the tuned axis 
    // 0: error, already tested in SanityCheck.h
    // 1: Tuned axis with 1 driver
    // 2: Tuned axis with 2 drivers or a coreXY machine
    driver_count = get_nb_drivers_for_axis(axis);

    axis_tuned = axis;   

    #if CORE_IS_XY
        // For a coreXY, get only current of stepperX for tuning, stallguard_type is already defined
        saved_current_0 = stepperX.rms_current();
        saved_current_1 = stepperY.rms_current();

        current = max(400, saved_current_0 / 2); // Don't start below 400mA
        stepperX.rms_current(current);
        stepperY.rms_current(current);

        // Test if one stepper driver has both SG2 and SG4
        // Remember that stallguard_type is already set to stepperX version
        // Set the SG2-SG4 capable driver to the stallguard version of the other stepper driver 

        #if AXIS_IS_SG2_SG4(X) && !AXIS_IS_SG2_SG4(Y)
            stallguard_type = tmc_stallguard_version(stepperY);
            restore_stealth_0 = tmc_enable_stallguard(stepperX, stallguard_type);
            restore_stealth_1 = tmc_enable_stallguard(stepperY);

        #elif !AXIS_IS_SG2_SG4(X) && AXIS_IS_SG2_SG4(Y)
            restore_stealth_0 = tmc_enable_stallguard(stepperX);
            restore_stealth_1 = tmc_enable_stallguard(stepperY, stallguard_type);

        #else        
            restore_stealth_0 = tmc_enable_stallguard(stepperX);
            restore_stealth_1 = tmc_enable_stallguard(stepperY);
        #endif
    
    #else
        if (axis == X_AXIS) {

            // Save actual stepper current
            // For a 2 drivers axis, current will be set to the same value on both drivers
            // Check only the first driver for the current:  X_CURRENT or Y_CURRENT
            // Tune at 50% of the configured current to optimize back EMF
            // stallguard_type is already defined
            saved_current_0 = stepperX.rms_current();
            current = max(400, saved_current_0 / 2); // Don't start below 400mA
            stepperX.rms_current(current);
            
            // Enbable stallguard mode on the driver(s), saving stealthChop state if any to restore later
            // Test if one stepper driver has both SG2 and SG4 in case of 2 stepper drivers
            // Remember that stallguard_type is already set to stepperX version 
                
            #if X2_SENSORLESS
                // Save actual stepper current and set the tuning value
                saved_current_1 = stepperX2.rms_current();
                stepperX2.rms_current(current);

                #if AXIS_IS_SG2_SG4(X) && !AXIS_IS_SG2_SG4(X2)
                    stallguard_type = tmc_stallguard_version(stepperX2);
                    restore_stealth_0 = tmc_enable_stallguard(stepperX, stallguard_type);
                    restore_stealth_1 = tmc_enable_stallguard(stepperX2);

                #elif !AXIS_IS_SG2_SG4(X) && AXIS_IS_SG2_SG4(X2)
                    restore_stealth_0 = tmc_enable_stallguard(stepperX);
                    restore_stealth_1 = tmc_enable_stallguard(stepperX2, stallguard_type);

                #else        
                    restore_stealth_0 = tmc_enable_stallguard(stepperX);
                    restore_stealth_1 = tmc_enable_stallguard(stepperX2);
                #endif
            #else
                restore_stealth_0 = tmc_enable_stallguard(stepperX);
            #endif
            } else {
            
                saved_current_0 = stepperY.rms_current();
                current = max(400, saved_current_0 / 2); // Don't start below 400mA
                stepperY.rms_current(current);
            
                #if Y2_SENSORLESS
                // Save actual stepper current and set the tuning value
                saved_current_1 = stepperY2.rms_current();
                stepperY2.rms_current(current);

                    #if AXIS_IS_SG2_SG4(Y) && !AXIS_IS_SG2_SG4(Y2)
                        stallguard_type = tmc_stallguard_version(stepperY2);
                        restore_stealth_0 = tmc_enable_stallguard(stepperY, stallguard_type);
                        restore_stealth_1 = tmc_enable_stallguard(stepperY2);

                    #elif !AXIS_IS_SG2_SG4(Y) && AXIS_IS_SG2_SG4(Y2)
                        restore_stealth_0 = tmc_enable_stallguard(stepperY);
                        restore_stealth_1 = tmc_enable_stallguard(stepperY2, stallguard_type);

                    #else        
                        restore_stealth_0 = tmc_enable_stallguard(stepperY);
                        restore_stealth_1 = tmc_enable_stallguard(stepperY2);
                    #endif
                #else
                    restore_stealth_0 = tmc_enable_stallguard(stepperY);
                #endif
            }
    #endif

    const float velocity = motion.homing_feedrate_mm_m[axis] / 60.0f; // Use homing feedrate, convert to mm/s
    
    // Launch the tuning procedure based on the stallguard version
    if (stallguard_type == SG_STALLGUARD2)
        tune_sg2(velocity);
    else 
        tune_sg4(velocity);

    // Restore both StealthChop and current initial values
    #if CORE_IS_XY
        tmc_disable_stallguard(stepperX, restore_stealth_0);
        stepperX.rms_current(saved_current_0);
        tmc_disable_stallguard(stepperY, restore_stealth_1);
        stepperY.rms_current(saved_current_1);
    #else
        if (axis == X_AXIS) {
        tmc_disable_stallguard(stepperX, restore_stealth_0);
        stepperX.rms_current(saved_current_0);
        #if X2_SENSORLESS
            tmc_disable_stallguard(stepperX2, restore_stealth_1);
            stepperX2.rms_current(saved_current_1);
        #endif
        }
        else {
            tmc_disable_stallguard(stepperY, restore_stealth_0);
            stepperY.rms_current(saved_current_0);
            #if Y2_SENSORLESS
                tmc_disable_stallguard(stepperY2, restore_stealth_1);
                stepperY2.rms_current(saved_current_1);
            #endif
        }
    #endif
}

// These function is called by marlin.idle() for sampling SG_RESULT during the tuning procedure
// SG_RESULT is sampled at 2ms interval
// This allows to get reliable values of SG_RESULT during the tuning moves
void StallGuardTuning::sampling() {
    if (!sg_tune_active) return; // Sample only when tuning is in progress

    // Sample every 2 ms
    if ((millis() - t_start) < 2) return;
    t_start += 2;

    uint16_t sg_result_0 = 0;
    #if X2_SENSORLESS || Y2_SENSORLESS || CORE_IS_XY
        uint16_t sg_result_1 = 0;
    #endif

    // SG_RESULT range is 0-1023 for StallGuard2 and 0-510 for StallGuard4
    // SG_RESULT is axis related
    #if CORE_IS_XY
        sg_result_0 = tmc_sg_result(stepperX);
        sg_result_1 = tmc_sg_result(stepperY);
        #if AXIS_HAS_STALLGUARD2(X) // StallGuard version is global for a coreXY and not related to axis
            sg2_sgr_sum += sg_result_0;
            if (sg_result_0 < sgr_min) sgr_min = sg_result_0;
            sg2_sgr_sum += sg_result_1;
            if (sg_result_1 < sgr_min) sgr_min = sg_result_1;
       #else
            sampling_hist[sg_result_0]++; // Increment histogram value for this SG_RESULT
            sampling_hist[sg_result_1]++;
        #endif 
        
    #else
        if (axis_tuned == X_AXIS) {
            sg_result_0 = tmc_sg_result(stepperX);
            #if X2_SENSORLESS
                sg_result_1 = tmc_sg_result(stepperX2);
            #endif
        } else {
            sg_result_0 = tmc_sg_result(stepperY);
            #if Y2_SENSORLESS
                sg_result_1 = tmc_sg_result(stepperY2);
            #endif
        }

        // Processing SG_RESULT depends on Stallguard version but not on axis
        if (stallguard_type == SG_STALLGUARD2) {
            sg2_sgr_sum += sg_result_0;
            if (sg_result_0 < sgr_min) sgr_min = sg_result_0;
            #if (X2_SENSORLESS || Y2_SENSORLESS)
                if (driver_count == 2) {
                    sg2_sgr_sum += sg_result_1;
                    if (sg_result_1 < sgr_min) sgr_min = sg_result_1;
                }
            #endif
        } else {
            sampling_hist[sg_result_0]++; // Increment histogram value for this SG_RESULT
            #if X2_SENSORLESS || Y2_SENSORLESS
                if (driver_count == 2) sampling_hist[sg_result_1]++;
            #endif
        }
    #endif
    sampling_total += driver_count; // Count total samples (one sample per driver)
}

// Set the parameters and generate the movement for the test
// For Stallguard2, SGTHRS (sensitivity) is progressively increased until SG_RESULT is in the predefined range 80-120
// SGTHRS range is -64 to 63
void StallGuardTuning::tune_sg2(const float velocity) { 

    xyze_pos_t target_pos = motion.position;
    int8_t direction = 1;

    sg_thrs = 0; // Indifferent value for SGT, free move
    sgr_min = 9999;
    
    // Main test loop : adapt SGT threshold until we see SG_RESULT within the prespecified range between 80 - 120 (safety margin)
    // Values are from Stallguard2 TMC datasheet (SG_RESULT before stall 0-100)
    // The first move is a free move with a SGT value of 0
    while(!tuning_success) {
        sg2_sgr_sum = 0;
        sampling_total = 0;

        set_homing_treshold(sg_thrs);

        // Forward or backward move until mean sg_result is within the prespecified range 80-120
        target_pos[axis_tuned] += TEST_TRAVEL_MM * direction;
        planner.buffer_line(target_pos, velocity, 0);

        // Set tuning flag to start SG_RESULT sampling
        // Synchronize planner will allow sampling via marlin.idle() until the moves end
        sg_tune_active = true;
        t_start = millis(); 
        planner.synchronize();

        // End of sampling 
        sg_tune_active = false; 

        uint16_t sg_result_test = sg2_sgr_sum / sampling_total; // Mean of sampled values for the move
        
        if (sg_result_test > 120 && sgr_min > 60) {
            if (++sg_thrs > 63) {
                break; // No valid SGT found, tuning failed
            }
            direction = -direction; // Change direction for another move
            continue;
        }

        if (sg_result_test < 80) {
            if (--sg_thrs < -64) {
                break; // No valid SGT found, tuning failed
            }
            direction = -direction; // Change direction for another move
            continue;
        }

        tuning_success = true;
    }
}

// StallGuard4 SGTHRS range is 0 to 255
// SG_RESULT range 0-510
// One forward and one backward move, collect SG results in a histogram
// Set SGTHRS value, at the 5% percentile of the lowest (SG_RESULT / 2)
// Values are from StallGuard4 TMC datasheet.
void StallGuardTuning::tune_sg4(const float velocity) {

    // Motion planning for tuning moves
    xyze_pos_t target_pos = motion.position;

    // Reset sampling histogram and total count
    sampling_total = 0;
    memset(sampling_hist, 0, sizeof(sampling_hist));

    // Set homing threshold to 0 : free move for cartesian or CoreXY
    set_homing_treshold(0);

    // Forward move
    target_pos[axis_tuned] += TEST_TRAVEL_MM;
    planner.buffer_line(target_pos, velocity, 0);

    // Backward move, just change axis target
    target_pos[axis_tuned] -= TEST_TRAVEL_MM;
    planner.buffer_line(target_pos, velocity, 0);

    // Set tuning flag active to start SG_RESULT sampling
    // Synchronize planner will allow sampling via marlin.idle() until the moves end
    sg_tune_active = true;
    t_start = millis(); 
    planner.synchronize();
    
    // End of sampling 
    sg_tune_active = false; 

    // Compute lower percentile
    uint16_t threshold = (uint16_t)(0.05 * sampling_total);
    uint32_t sum = 0;
    uint8_t sg_p5 = 0;

    for (uint16_t v = 0; v < 511; v++) {
        sum += sampling_hist[v];
        if (sum >= threshold) {
            sg_p5 = v;
            break;
        }
    }
    // SGTHRS final
    sg_thrs = (int8_t)round((sg_p5 / 2.0f));
    tuning_success = (sg_thrs > 0) ? true : false; // If SGTHRS is 0, it means that the 5% percentile is very low, tuning failed, otherwise tuning is successful
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

// Number of drivers per axis and get stallguard version
// Returns the number of drivers for the tuned axis or always 2 for CoreXY
uint8_t StallGuardTuning::get_nb_drivers_for_axis(const AxisEnum axis) {
    uint8_t count = 0;

    #if CORE_IS_XY
        stallguard_type = tmc_stallguard_version(stepperX);
        count = 2;
    #else
        if (axis == X_AXIS) {
            #if X_SENSORLESS
                stallguard_type = tmc_stallguard_version(stepperX);
                count = 1;
            #endif
            #if X2_SENSORLESS
                count = 2;
            #endif
        }

        if (axis == Y_AXIS) {
            #if Y_SENSORLESS
                stallguard_type = tmc_stallguard_version(stepperY);
                count = 1;
            #endif
            #if Y2_SENSORLESS
                count = 2;
            #endif
        }
    #endif
    return count;
}

#endif // ENABLED(STALLGUARD_TUNING)
