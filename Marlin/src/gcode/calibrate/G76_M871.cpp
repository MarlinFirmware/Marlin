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

/**
 * G76_M871.cpp - Temperature calibration/compensation for z-probing
 */

#include "../../inc/MarlinConfig.h"

#if ENABLED(PROBE_TEMP_COMPENSATION)

#include "../gcode.h"
#include "../../module/motion.h"
#include "../../module/planner.h"
#include "../../module/probe.h"
#include "../../feature/bedlevel/bedlevel.h"
#include "../../module/temperature.h"
#include "../../module/probe.h"
#include "../../feature/probe_temp_compensation.h"

/**
 * G76: calibrate probe and/or bed temperature offsets
 *  Notes:
 *  - When calibrating probe, bed temperature is held constant.
 *    Compensation values are deltas to first probe measurement at probe temp. = 30°C.
 *  - When calibrating bed, probe temperature is held constant.
 *    Compensation values are deltas to first probe measurement at bed temp. = 60°C.
 *  - The hotend will not be heated at any time.
 *  - On my Prusa MK3S clone I put a piece of paper between the probe and the hotend
 *    so the hotend fan would not cool my probe constantly. Alternativly you could just
 *    make sure the fan is not running while running the calibration process.
 *
 *  Probe calibration:
 *  - Moves probe to cooldown point.
 *  - Heats up bed to 100°C.
 *  - Moves probe to probing point (1mm above heatbed).
 *  - Waits until probe reaches target temperature (30°C).
 *  - Does a z-probing (=base value) and increases target temperature by 5°C.
 *  - Waits until probe reaches increased target temperature.
 *  - Does a z-probing (delta to base value will be a compensation value) and increases target temperature by 5°C.
 *  - Repeats last two steps until max. temperature reached or timeout (i.e. probe does not heat up any further).
 *  - Compensation values of higher temperatures will be extrapolated (using linear regression first).
 *    While this is not exact by any means it is still better than simply using the last compensation value.
 *
 *  Bed calibration:
 *  - Moves probe to cooldown point.
 *  - Heats up bed to 60°C.
 *  - Moves probe to probing point (1mm above heatbed).
 *  - Waits until probe reaches target temperature (30°C).
 *  - Does a z-probing (=base value) and increases bed temperature by 5°C.
 *  - Moves probe to cooldown point.
 *  - Waits until probe is below 30°C and bed has reached target temperature.
 *  - Moves probe to probing point and waits until it reaches target temperature (30°C).
 *  - Does a z-probing (delta to base value will be a compensation value) and increases bed temperature by 5°C.
 *  - Repeats last four points until max. bed temperature reached (110°C) or timeout.
 *  - Compensation values of higher temperatures will be extrapolated (using linear regression first).
 *    While this is not exact by any means it is still better than simply using the last compensation value.
 *
 *  G76 [B | P]
 *  - no flag - Both calibration procedures will be run.
 *  - `B` - Run bed temperature calibration.
 *  - `P` - Run probe temperature calibration.
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

  bool do_bed_cal = parser.boolval('B'),
       do_probe_cal = parser.boolval('P');
  if (!do_bed_cal && !do_probe_cal)
    do_bed_cal = do_probe_cal = true;

  // Synchronize with planner
  planner.synchronize();

  // Report temperatures every second and handle heating timeouts
  millis_t next_temp_report = millis() + 1000;

  if (do_bed_cal || do_probe_cal) {
    // Ensure park position is reachable
    if (!position_is_reachable(ProbeTempComp::park_point.x, ProbeTempComp::park_point.y)
      || !(WITHIN(ProbeTempComp::park_point.z, Z_MIN_POS - 0.001f, Z_MAX_POS + 0.001f))
    ) {
      SERIAL_ECHOLNPGM("!Park position unreachable - aborting.");
      return;
    }
    // Ensure probe position is reachable
    destination.set(
      temp_comp.measure_point_x - probe_offset.x,
      temp_comp.measure_point_y - probe_offset.y
    );
    if (!position_is_reachable_by_probe(destination)) {
      SERIAL_ECHOLNPGM("!Probe position unreachable - aborting.");
      return;
    }

    G28(true);
  }

  /******************************************
   * Calibrate bed temperature offsets
   ******************************************/

  if (do_bed_cal) {

    uint16_t target_bed = temp_comp.cali_info_init[TSI_BED].start_temp,
             target_probe = temp_comp.bed_calib_probe_temp;

    SERIAL_ECHOLNPGM("Waiting for printer to cool down.");
    while (thermalManager.degBed() > target_bed
      || thermalManager.degProbe() > target_probe
    ) {
      idle(
        #if ENABLED(ADVANCED_PAUSE_FEATURE)
          true
        #endif
      );
      const millis_t ms = millis();
      if (ELAPSED(ms, next_temp_report)) {
        thermalManager.print_heater_states(active_extruder);
        next_temp_report = ms + 1000;
      }
    }

    // Disable leveling so it won't mess with us
    #if HAS_LEVELING
      set_bed_leveling_enabled(false);
    #endif

    bool timeout = false;
    while (true) {
      thermalManager.setTargetBed(target_bed);

      SERIAL_ECHOLNPAIR("Target Bed: ", target_bed, "; Probe: ", target_probe);

      // Park nozzle
      do_blocking_move_to(ProbeTempComp::park_point.x, ProbeTempComp::park_point.y, ProbeTempComp::park_point.z);

      // Wait for heatbed to reach target temp and probe to cool below target temp
      SERIAL_ECHOLNPGM("Waiting for bed and probe to reach target temp.");
      const millis_t probe_timeout_ms = millis() + 900UL * 1000UL;
      while (fabs(thermalManager.degBed() - float(target_bed)) > 0.1 || thermalManager.degProbe() > target_probe) {
        idle(
          #if ENABLED(ADVANCED_PAUSE_FEATURE)
            true
          #endif
        );
        const millis_t ms = millis();
        if (ELAPSED(ms, next_temp_report)) {
          thermalManager.print_heater_states(active_extruder);
          next_temp_report = ms + 1000;
        }
        if (ELAPSED(ms, probe_timeout_ms)) {
          SERIAL_ECHOLNPGM("!Bed heating timeout.");
          timeout = true;
          break;
        }
      }

      if (timeout) break;

      // Move probe to probing point and wait for probe to reach target temp
      destination.set(temp_comp.measure_point_x, temp_comp.measure_point_y, 0.5);
      do_blocking_move_to(destination.x, destination.y, destination.z);
      SERIAL_ECHOLNPGM("Waiting for probe heating.");
      while (thermalManager.degProbe() < target_probe) {
        idle(
          #if ENABLED(ADVANCED_PAUSE_FEATURE)
            true
          #endif
        );
        const millis_t ms = millis();
        if (ELAPSED(ms, next_temp_report)) {
          thermalManager.print_heater_states(active_extruder);
          next_temp_report = ms + 1000;
        }
      }

      // Raise nozzle before probing
      destination.z = 5.0;
      do_blocking_move_to_z(destination.z);

      // Do a single probe
      remember_feedrate_scaling_off();
      const float measured_z = probe_at_point(
        destination.x + probe_offset.x,
        destination.y + probe_offset.y,
        PROBE_PT_NONE
      );
      restore_feedrate_and_scaling();

      if (isnan(measured_z)) {
        SERIAL_ECHOLNPGM("!Received NAN measurement - aborting.");
        break;
      }
      else
        SERIAL_ECHOLNPAIR_F("Measured: ", measured_z);

      if (target_bed == temp_comp.cali_info_init[TSI_BED].start_temp)
        temp_comp.prepare_new_calibration(measured_z);
      else
        temp_comp.push_back_new_measurement(TSI_BED, measured_z);

      target_bed += temp_comp.cali_info_init[TSI_BED].temp_res;
      if (target_bed > temp_comp.max_bed_temp) break;
    }

    SERIAL_ECHOLNPAIR("Retrieved measurements: ", temp_comp.get_index());
    if (temp_comp.finish_calibration(TSI_BED))
      SERIAL_ECHOLNPGM("Successfully calibrated bed.");
    else
      SERIAL_ECHOLNPGM("!Failed to calibrated bed - reset calibration values.");

    // Cleanup
    thermalManager.setTargetBed(0);
    #if HAS_LEVELING
      set_bed_leveling_enabled(true);
    #endif
  } // do_bed_cal

  /********************************************
   * Calibrate probe temperature offsets
   ********************************************/

  if (do_probe_cal) {

    // Park nozzle
    do_blocking_move_to(ProbeTempComp::park_point.x, ProbeTempComp::park_point.y, ProbeTempComp::park_point.z);

    // Initialize temperatures
    uint16_t target_bed = temp_comp.probe_calib_bed_temp,
             target_probe = temp_comp.cali_info_init[TSI_PROBE].start_temp;
    thermalManager.setTargetBed(target_bed);
    SERIAL_ECHOLNPGM("Waiting for bed and probe temperature.");
    while (fabs(thermalManager.degBed() - float(target_bed)) > 0.1f
           || thermalManager.degProbe() > target_probe
    ) {
      idle(
        #if ENABLED(ADVANCED_PAUSE_FEATURE)
          true
        #endif
      );
      const millis_t ms = millis();
      if (ELAPSED(ms, next_temp_report)) {
        thermalManager.print_heater_states(active_extruder);
        next_temp_report = ms + 1000;
      }
    }

    // Disable leveling so it won't mess with us
    #if HAS_LEVELING
      set_bed_leveling_enabled(false);
    #endif

    bool timeout = false;
    while (true) {
      // Move probe to probing point and wait for it to reach target temperature
      destination.set(temp_comp.measure_point_x, temp_comp.measure_point_y, 0.5);
      do_blocking_move_to(destination);

      SERIAL_ECHOLNPAIR(
        "Bed temp: ", target_bed,
        "; Probe temp: ", target_probe,
        "  Waiting for probe heating."
      );

      const millis_t probe_timeout_ms = millis() + 900UL * 1000UL;
      while (thermalManager.degProbe() < target_probe) {
        idle(
          #if ENABLED(ADVANCED_PAUSE_FEATURE)
            true
          #endif
        );
        const millis_t ms = millis();
        if (ELAPSED(ms, next_temp_report)) {
          thermalManager.print_heater_states(active_extruder);
          next_temp_report = ms + 1000;
        }
        if (ELAPSED(ms, probe_timeout_ms)) {
          SERIAL_ECHOLNPGM("!Probe heating aborted due to timeout.");
          timeout = true;
          break;
        }
      }

      if (timeout) break;

      // Raise nozzle before probing
      destination.z = 5.0;
      do_blocking_move_to_z(destination.z);

      // Do a single probe
      remember_feedrate_scaling_off();
      const float measured_z = probe_at_point(
        destination.x + probe_offset.x,
        destination.y + probe_offset.y,
        PROBE_PT_NONE
      );
      restore_feedrate_and_scaling();

      if (isnan(measured_z)) {
        SERIAL_ECHOLNPGM("!Received NAN measurement - aborting.");
        break;
      }
      else
        SERIAL_ECHOLNPAIR_F("Measured: ", measured_z);

      if (target_probe == temp_comp.cali_info_init[TSI_PROBE].start_temp)
        temp_comp.prepare_new_calibration(measured_z);
      else
        temp_comp.push_back_new_measurement(TSI_PROBE, measured_z);

      target_probe += temp_comp.cali_info_init[TSI_PROBE].temp_res;
      if (target_probe > temp_comp.cali_info_init[TSI_PROBE].end_temp) break;
    }

    SERIAL_ECHOLNPAIR("Retrieved measurements: ", temp_comp.get_index());
    if (temp_comp.finish_calibration(TSI_PROBE))
      SERIAL_ECHOLNPGM("Successfully calibrated probe.");
    else
      SERIAL_ECHOLNPGM("!Failed to calibrated probe.");

    // Cleanup
    thermalManager.setTargetBed(0);
    #if HAS_LEVELING
      set_bed_leveling_enabled(true);
    #endif

    SERIAL_ECHOLNPGM("Final compensation values:");
    temp_comp.print_offsets();
  } // do_probe_cal
}

/**
 * M871: Report / reset temperature compensation offsets.
 *       Note: This does not affect values in EEPROM until M500.
 *
 *   M871 [ R | B | P | E ]
 *
 *    No Parameters - Print current offset values.
 *
 * Select only one of these flags:
 *    R - Reset all offsets to zero (i.e., disable compensation).
 *    B - Manually set offset for bed
 *    P - Manually set offset for probe
 *    E - Manually set offset for extruder
 *
 * With B, P, or E:
 *    I[index] - Index in the array
 *    V[value] - Adjustment in µm
 */
void GcodeSuite::M871() {

  if (parser.seen('R')) {
    // Reset z-probe offsets to factory defaults
    temp_comp.clear_all_offsets();
    SERIAL_ECHOLNPGM("Offsets reset to default.");
  }
  else if (parser.seen("BPE")) {
    if (!parser.seenval('V')) return;
    const int16_t val = parser.value_int();
    if (!parser.seenval('I')) return;
    const int16_t idx = parser.value_int();
    const TempSensorID mod = (parser.seen('B') ? TSI_BED :
                              #if ENABLED(USE_TEMP_EXT_COMPENSATION)
                                parser.seen('E') ? TSI_EXT :
                              #endif
                              TSI_PROBE
                              );
    if (idx > 0 && temp_comp.set_offset(mod, idx - 1, val))
      SERIAL_ECHOLNPAIR("Set value: ", val);
    else
      SERIAL_ECHOLNPGM("!Invalid index. Failed to set value (note: value at index 0 is constant).");

  }
  else // Print current Z-probe adjustments. Note: Values in EEPROM might differ.
    temp_comp.print_offsets();
}

#endif // PROBE_TEMP_COMPENSATION
