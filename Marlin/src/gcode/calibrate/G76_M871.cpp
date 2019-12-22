/**
 * G76_M871.cpp - Temperature calibration/compensation for z-probing
 */

#include "../../inc/MarlinConfig.h"

#if ENABLED(USE_TEMP_COMPENSATION)

#include "../gcode.h"
#include "../../module/motion.h"
#include "../../module/planner.h"
#include "../../module/probe.h"
#include "../../feature/bedlevel/bedlevel.h"
#include "../../module/temperature.h"
#include "../../module/probe.h"
#include "../../feature/temp_comp.h"

/**
 * G76: calibrate probe and/or bed temperature offsets
 *      Notes:
 *      - When calibrating probe, bed temperature is held constant.
 *        Compensation values are deltas to first probe measurement at probe temp. = 30°C.
 *      - When calibrating bed, probe temperature is held constant.
 *        Compensation values are deltas to first probe measurement at bed temp. = 60°C.
 *      - The hotend will not be heated at any time.
 *      - On my Prusa MK3S clone I put a piece of paper between the probe and the hotend
 *        so the hotend fan would not cool my probe constantly. Alternativly you could just
 *        make sure the fan is not running while running the calibration process.
 * 
 *      Probe calibration:
 *      - Moves probe to cooldown point.
 *      - Heats up bed to 100°C.
 *      - Moves probe to probing point (1mm above heatbed).
 *      - Waits until probe reaches target temperature (30°C).
 *      - Does a z-probing (=base value) and increases target temperature by 5°C.
 *      - Waits until probe reaches increased target temperature.
 *      - Does a z-probing (delta to base value will be a compensation value) and increases target temperature by 5°C.
 *      - Repeats last two steps until max. temperature reached or timeout (i.e. probe does not heat up any further).
 *      - Compensation values of higher temperatures will be extrapolated (using linear regression first).
 *        While this is not exact by any means it is still better than simply using the last compensation value.
 *
 *      Bed calibration:
 *      - Moves probe to cooldown point.
 *      - Heats up bed to 60°C.
 *      - Moves probe to probing point (1mm above heatbed).
 *      - Waits until probe reaches target temperature (30°C).
 *      - Does a z-probing (=base value) and increases bed temperature by 5°C.
 *      - Moves probe to cooldown point.
 *      - Waits until probe is below 30°C and bed has reached target temperature.
 *      - Moves probe to probing point and waits until it reaches target temperature (30°C).
 *      - Does a z-probing (delta to base value will be a compensation value) and increases bed temperature by 5°C.
 *      - Repeats last four points until max. bed temperature reached (110°C) or timeout.
 *      - Compensation values of higher temperatures will be extrapolated (using linear regression first).
 *        While this is not exact by any means it is still better than simply using the last compensation value.
 * 
 *      G76 [B | P]
 *      - no flag - Both calibration procedures will be run.
 *      - `B` - Run bed temperature calibration.
 *      - `P` - Run probe temperature calibration.
 */
void GcodeSuite::G76() {
    // Check if heated bed is available and z-homing is done with probe
    #if TEMP_SENSOR_BED == 0 || !(HOMING_Z_WITH_PROBE)
        return;
    #endif

    #if ENABLED(BLTOUCH)
        // Make sure any BLTouch error condition is cleared
        bltouch_command(BLTOUCH_RESET, BLTOUCH_RESET_DELAY);
        set_bltouch_deployed(false);
    #endif

    bool calib_bed = parser.seen('B');
    bool calib_probe = parser.seen('P');
    if (!calib_bed && !calib_probe) {
        calib_bed = calib_probe = true;
    }

    // Synchronize with planner
    planner.synchronize();

    // Report temperatures every second and handle heating timeouts
    unsigned long last_temp_report = millis();
    unsigned long last_probed;

    const float nozzle_offsets[3] = NOZZLE_TO_PROBE_OFFSET;

    /******************************************
     * Calibrate bed temperature offsets
     ******************************************/
    if (calib_bed) {
        // Ensure park position is reachable
        if (!position_is_reachable(TEMP_COMP_PARK_POINT_X, TEMP_COMP_PARK_POINT_Y) ||
            !(WITHIN(TEMP_COMP_PARK_POINT_Z, Z_MIN_POS - 0.001f, Z_MAX_POS + 0.001f))) {
            SERIAL_ECHOLN("Park position unreachable - aborting.");
            return;
        }
        // Ensure probe position is reachable
        destination[X_AXIS] = TEMP_COMP_MEASURE_POINT_X - nozzle_offsets[X_AXIS];
        destination[Y_AXIS] = TEMP_COMP_MEASURE_POINT_Y - nozzle_offsets[Y_AXIS];
        if (!position_is_reachable_by_probe(destination[X_AXIS], destination[Y_AXIS])) {
            SERIAL_ECHOLN("Probe position unreachable - aborting.");
            return;
        }
        
        // Home all axes
        G28(true);

        uint16_t target_bed, target_probe;
        target_bed = TEMP_COMP_BED_START_TEMP;
        target_probe = TEMP_COMP_BED_CALIB_PROBE_TEMP;
        SERIAL_ECHOLN("Waiting for printer to cool down.");
        while (thermalManager.degBed() > target_bed ||
            thermalManager.degProbe() > target_probe) {
            idle(
                #if ENABLED(ADVANCED_PAUSE_FEATURE)
                    true
                #endif
                );
            if (millis() - last_temp_report > 1000) {
                thermalManager.print_heater_states(active_extruder);
                last_temp_report = millis();
            }
        }
        
        // Disable leveling so it won't mess with us
        #if HAS_LEVELING
            set_bed_leveling_enabled(false);
        #endif

        last_probed = millis();
        bool timeout = false;
        while (true) {
            thermalManager.setTargetBed(target_bed);

            SERIAL_ECHOPAIR("Bed temp: ", target_bed); SERIAL_ECHOLNPAIR("; Probe temp: ", target_probe);

            // Park nozzle
            do_blocking_move_to(TEMP_COMP_PARK_POINT_X, TEMP_COMP_PARK_POINT_Y, TEMP_COMP_PARK_POINT_Z);

            // Wait for heatbed to reach target temp and probe to cool below target temp
            SERIAL_ECHOLN("Waiting for bed and probe to reach target temp.");
            while (fabs(thermalManager.degBed()-(float)target_bed) > 0.1 ||
                    thermalManager.degProbe() > target_probe) {
                idle(
                    #if ENABLED(ADVANCED_PAUSE_FEATURE)
                        true
                    #endif
                    );
                if (millis() - last_temp_report > 1000) {
                    thermalManager.print_heater_states(active_extruder);
                    last_temp_report = millis();
                }
                if (millis() - last_probed > 900UL * 1000UL) {
                    SERIAL_ECHOLN("Bed heating timeout.");
                    timeout = true;
                    break;
                }
            }

            if (timeout) break;

            // Move probe to probing point and wait for probe to reach target temp
            destination[X_AXIS] = TEMP_COMP_MEASURE_POINT_X;
            destination[Y_AXIS] = TEMP_COMP_MEASURE_POINT_Y;
            destination[Z_AXIS] = 0.5;
            do_blocking_move_to(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS]);
            SERIAL_ECHOLN("Waiting for probe heating.");
            while (thermalManager.degProbe() < target_probe) {
                idle(
                    #if ENABLED(ADVANCED_PAUSE_FEATURE)
                        true
                    #endif
                    );
                if (millis() - last_temp_report > 1000) {
                    thermalManager.print_heater_states(active_extruder);
                    last_temp_report = millis();
                }
            }

            // Raise nozzle before probing
            destination[Z_AXIS] = 5.0;
            do_blocking_move_to_z(destination[Z_AXIS]);

            // Do a single probe
            //setup_for_endstop_or_probe_move();
            remember_feedrate_scaling_off();
            const float measured_z = probe_at_point(destination[X_AXIS]+nozzle_offsets[X_AXIS],
                                                    destination[Y_AXIS]+nozzle_offsets[Y_AXIS],
                                                    PROBE_PT_NONE);
            restore_feedrate_and_scaling();

            if (isnan(measured_z)) {
                SERIAL_ECHOLN("Received NAN measurement - aborting.");
                break;
            } else {
                SERIAL_ECHOLNPAIR_F("Measured: ", measured_z);
            }

            if (target_bed == TEMP_COMP_BED_START_TEMP) {
                temp_comp.prepare_new_calibration(measured_z);
            } else {
                temp_comp.push_back_new_measurement(TempComp::TEMP_COMP_BED, measured_z);
            }

            target_bed += TEMP_COMP_BED_TEMP_RES;
            if (target_bed > TEMP_COMP_MAX_BED_TEMP) break;

            last_probed = millis();
        }

        SERIAL_ECHOLNPAIR("Retreived measurements: ", temp_comp.get_index());
        if (temp_comp.finish_calibration(TempComp::TEMP_COMP_BED)) {
            SERIAL_ECHOLN("Successfully calibrated bed.");
        } else {
            SERIAL_ECHOLN("Failed to calibrated bed - reset calibration values.");
        }

        // Cleanup
        thermalManager.setTargetBed(0);
        #if HAS_LEVELING
            set_bed_leveling_enabled(true);
        #endif
    } // calib_bed

    /********************************************
     * Calibrate probe temperature offsets
     ********************************************/
    if (calib_probe) {
        // Ensure park position is reachable
        if (!position_is_reachable(TEMP_COMP_PARK_POINT_X, TEMP_COMP_PARK_POINT_Y) ||
            !(WITHIN(TEMP_COMP_PARK_POINT_Z, Z_MIN_POS - 0.001f, Z_MAX_POS + 0.001f))) {
            SERIAL_ECHOLN("Park position unreachable - aborting.");
            return;
        }
        // Ensure probe position is reachable
        destination[X_AXIS] = TEMP_COMP_MEASURE_POINT_X - nozzle_offsets[X_AXIS];
        destination[Y_AXIS] = TEMP_COMP_MEASURE_POINT_Y - nozzle_offsets[Y_AXIS];
        if (!position_is_reachable_by_probe(destination[X_AXIS], destination[Y_AXIS])) {
            SERIAL_ECHOLN("Probe position unreachable - aborting.");
            return;
        }

        // Home all axes
        if (!calib_bed) G28(true);

        // Park nozzle
        do_blocking_move_to(TEMP_COMP_PARK_POINT_X, TEMP_COMP_PARK_POINT_Y, TEMP_COMP_PARK_POINT_Z);

        // Initialize temperatures
        uint16_t target_bed, target_probe;
        target_bed = TEMP_COMP_PROBE_CALIB_BED_TEMP;
        target_probe = TEMP_COMP_PROBE_START_TEMP;
        thermalManager.setTargetBed(target_bed);
        SERIAL_ECHOLN("Waiting for bed and probe temperature.");
        while (fabs(thermalManager.degBed()-(float)target_bed) > 0.1 ||
                thermalManager.degProbe() > target_probe) {
            idle(
                #if ENABLED(ADVANCED_PAUSE_FEATURE)
                    true
                #endif
                );
            if (millis() - last_temp_report > 1000) {
                thermalManager.print_heater_states(active_extruder);
                last_temp_report = millis();
            }
        }

        // Disable leveling so it won't mess with us
        #if HAS_LEVELING
        set_bed_leveling_enabled(false);
        #endif

        last_probed = millis();
        bool timeout = false;
        while (true) {
            // Move probe to probing point and wait for it to reach target temperature
            destination[X_AXIS] = TEMP_COMP_MEASURE_POINT_X;
            destination[Y_AXIS] = TEMP_COMP_MEASURE_POINT_Y;
            destination[Z_AXIS] = 0.5;
            do_blocking_move_to(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS]);
            
            SERIAL_ECHOPAIR("Bed temp: ", target_bed); SERIAL_ECHOLNPAIR("; Probe temp: ", target_probe);
            
            SERIAL_ECHOLN("Waiting for probe heating.");
            while (thermalManager.degProbe() < target_probe) {
                idle(
                    #if ENABLED(ADVANCED_PAUSE_FEATURE)
                        true
                    #endif
                    );
                if (millis() - last_temp_report > 1000) {
                    thermalManager.print_heater_states(active_extruder);
                    last_temp_report = millis();
                }
                if (millis() - last_probed > 900UL * 1000UL) {
                    SERIAL_ECHOLN("Probe heating aborted due to timeout.");
                    timeout = true;
                    break;
                }
            }

            if (timeout) break;
            
            // Raise nozzle before probing
            destination[Z_AXIS] = 5.0;
            do_blocking_move_to_z(destination[Z_AXIS]);
            
            // Do a single probe
            remember_feedrate_scaling_off();
            const float measured_z = probe_at_point(destination[X_AXIS]+nozzle_offsets[X_AXIS],
                                                    destination[Y_AXIS]+nozzle_offsets[Y_AXIS],
                                                    PROBE_PT_NONE);
            restore_feedrate_and_scaling();

            if (isnan(measured_z)) {
                SERIAL_ECHOLN("Received NAN measurement - aborting.");
                break;
            } else {
                SERIAL_ECHOLNPAIR_F("Measured: ", measured_z);
            }

            if (target_probe == TEMP_COMP_PROBE_START_TEMP) {
                temp_comp.prepare_new_calibration(measured_z);
            } else {
                temp_comp.push_back_new_measurement(TempComp::TEMP_COMP_PROBE, measured_z);
            }

            target_probe += TEMP_COMP_PROBE_TEMP_RES;
            if (target_probe > TEMP_COMP_PROBE_END_TEMP) break;

            last_probed = millis();
        }

        SERIAL_ECHOLNPAIR("Retreived measurements: ", temp_comp.get_index());
        if (temp_comp.finish_calibration(TempComp::TEMP_COMP_PROBE)) {
            SERIAL_ECHOLN("Successfully calibrated probe.");
        } else {
            SERIAL_ECHOLN("Failed to calibrated probe.");
        }

        // Cleanup
        thermalManager.setTargetBed(0);
        #if HAS_LEVELING
            set_bed_leveling_enabled(true);
        #endif

        SERIAL_ECHOLN("Final compensation values are:");
        temp_comp.print_offsets();
    } // calib_probe
}

/**
 * M871: Print/reset current temperature compensation offsets.
 *       Note: this does not effect values in EEPROM until initiating M500.
 *       M871 [ R | B | P | E ]
 *        - no flag - Print current offset values.
 *        - `R` - Set offsets to all zeros, i.e. disabling compensation.
 *        - `B` - Manually set offset for bed
 *                V[value] in um
 *                I[index]
 *        - `P` - Manually set offset for probe
 *                V[value] in um
 *                I[index]
 *        - `E` - Manually set offset for extruder
 *                V[value] in um
 *                I[index]
 */
void GcodeSuite::M871() {
    if (parser.seen('R')) {
        // Reset z-probe offsets to factory defaults
        temp_comp.clear_offsets(TempComp::TEMP_COMP_BED);
        temp_comp.clear_offsets(TempComp::TEMP_COMP_PROBE);
        temp_comp.clear_offsets(TempComp::TEMP_COMP_EXT);
        SERIAL_ECHOLN("Reset values to default.");
    } else if (parser.seen('B')) {
        // Manually set offsets for bed
        if (!parser.seenval('V')) return;
        int16_t val = parser.value_int();
        if (!parser.seenval('I')) return;
        int16_t idx = parser.value_int();

        if (idx > 0 && temp_comp.set_offset(TempComp::TEMP_COMP_BED, idx-1, val)) {
            SERIAL_ECHOLNPAIR("Set value: ", val);
        } else {
            SERIAL_ECHOLN("Invalid index, failed to set value (note: value at index 0 is constant).");
        }
    } else if (parser.seen('P')) {
        // Manually set offsets for bed
        if (!parser.seenval('V')) return;
        int16_t val = parser.value_int();
        if (!parser.seenval('I')) return;
        int16_t idx = parser.value_int();
        if (idx > 0 && temp_comp.set_offset(TempComp::TEMP_COMP_PROBE, idx-1, val)) {
            SERIAL_ECHOLNPAIR("Set value: ", val);
        } else {
            SERIAL_ECHOLN("Invalid index, failed to set value (note: value at index 0 is constant).");
        }
    } else if (parser.seen('E')) {
        // Manually set offsets for bed
        if (!parser.seenval('V')) return;
        int16_t val = parser.value_int();
        if (!parser.seenval('I')) return;
        int16_t idx = parser.value_int();
        
        if (idx > 0 && temp_comp.set_offset(TempComp::TEMP_COMP_EXT, idx-1, val)) {
            SERIAL_ECHOLNPAIR("Set value: ", val);
        } else {
            SERIAL_ECHOLN("Invalid index, failed to set value (note: value at index 0 is constant).");
        }
    } else {
        // Print current z-probe offsets. Note: values in EEPROM might differ.
        temp_comp.print_offsets();
    }
}

#endif // AUTO_BED_LEVELING_UBL