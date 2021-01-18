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
#include "../../feature/probe_temp_comp.h"

#include "../../lcd/marlinui.h"
#include "../../MarlinCore.h" // for wait_for_heatup, idle(), G28_STR

#if ENABLED(PRINTJOB_TIMER_AUTOSTART)
  #include "../../module/printcounter.h"
#endif

#if ENABLED(PRINTER_EVENTS_LEDS)
  #include "../../feature/leds/leds.h"
#endif

/**
 * G76: calibrate probe and/or bed temperature offsets
 *  Notes:
 *  - When calibrating probe, bed temperature is held constant.
 *    Compensation values are deltas to first probe measurement at probe temp. = 30°C.
 *  - When calibrating bed, probe temperature is held constant.
 *    Compensation values are deltas to first probe measurement at bed temp. = 60°C.
 *  - The hotend will not be heated at any time.
 *  - On my Průša MK3S clone I put a piece of paper between the probe and the hotend
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

static void say_waiting_for()               { SERIAL_ECHOPGM("Waiting for "); }
static void say_waiting_for_probe_heating() { say_waiting_for(); SERIAL_ECHOLNPGM("probe heating."); }
static void say_successfully_calibrated()   { SERIAL_ECHOPGM("Successfully calibrated"); }
static void say_failed_to_calibrate()       { SERIAL_ECHOPGM("!Failed to calibrate"); }

void GcodeSuite::G76() {
  // Check if heated bed is available and z-homing is done with probe
  #if TEMP_SENSOR_BED == 0 || !(HOMING_Z_WITH_PROBE)
    return;
  #endif

  auto report_temps = [](millis_t &ntr, millis_t timeout=0) {
    idle_no_sleep();
    const millis_t ms = millis();
    if (ELAPSED(ms, ntr)) {
      ntr = ms + 1000;
      thermalManager.print_heater_states(active_extruder);
    }
    return (timeout && ELAPSED(ms, timeout));
  };

  auto wait_for_temps = [&](const float tb, const float tp, millis_t &ntr, const millis_t timeout=0) {
    say_waiting_for(); SERIAL_ECHOLNPGM("bed and probe temperature.");
    while (fabs(thermalManager.degBed() - tb) > 0.1f || thermalManager.degProbe() > tp)
      if (report_temps(ntr, timeout)) return true;
    return false;
  };

  auto g76_probe = [](const TempSensorID sid, uint16_t &targ, const xy_pos_t &nozpos) {
    do_z_clearance(5.0); // Raise nozzle before probing
    const float measured_z = probe.probe_at_point(nozpos, PROBE_PT_STOW, 0, false);  // verbose=0, probe_relative=false
    if (isnan(measured_z))
      SERIAL_ECHOLNPGM("!Received NAN. Aborting.");
    else {
      SERIAL_ECHOLNPAIR_F("Measured: ", measured_z);
      if (targ == cali_info_init[sid].start_temp)
        temp_comp.prepare_new_calibration(measured_z);
      else
        temp_comp.push_back_new_measurement(sid, measured_z);
      targ += cali_info_init[sid].temp_res;
    }
    return measured_z;
  };

  #if ENABLED(BLTOUCH)
    // Make sure any BLTouch error condition is cleared
    bltouch_command(BLTOUCH_RESET, BLTOUCH_RESET_DELAY);
    set_bltouch_deployed(false);
  #endif

  bool do_bed_cal = parser.boolval('B'), do_probe_cal = parser.boolval('P');
  if (!do_bed_cal && !do_probe_cal) do_bed_cal = do_probe_cal = true;

  // Synchronize with planner
  planner.synchronize();

  const xyz_pos_t parkpos = temp_comp.park_point,
            probe_pos_xyz = xyz_pos_t(temp_comp.measure_point) + xyz_pos_t({ 0.0f, 0.0f, PTC_PROBE_HEATING_OFFSET }),
              noz_pos_xyz = probe_pos_xyz - probe.offset_xy;  // Nozzle position based on probe position

  if (do_bed_cal || do_probe_cal) {
    // Ensure park position is reachable
    bool reachable = position_is_reachable(parkpos) || WITHIN(parkpos.z, Z_MIN_POS - fslop, Z_MAX_POS + fslop);
    if (!reachable)
      SERIAL_ECHOLNPGM("!Park");
    else {
      // Ensure probe position is reachable
      reachable = probe.can_reach(probe_pos_xyz);
      if (!reachable) SERIAL_ECHOLNPGM("!Probe");
    }

    if (!reachable) {
      SERIAL_ECHOLNPGM(" position unreachable - aborting.");
      return;
    }

    process_subcommands_now_P(G28_STR);
  }

  remember_feedrate_scaling_off();


  /******************************************
   * Calibrate bed temperature offsets
   ******************************************/

  // Report temperatures every second and handle heating timeouts
  millis_t next_temp_report = millis() + 1000;

  auto report_targets = [&](const uint16_t tb, const uint16_t tp) {
    SERIAL_ECHOLNPAIR("Target Bed:", tb, " Probe:", tp);
  };

  if (do_bed_cal) {

    uint16_t target_bed = cali_info_init[TSI_BED].start_temp,
             target_probe = temp_comp.bed_calib_probe_temp;

    say_waiting_for(); SERIAL_ECHOLNPGM(" cooling.");
    while (thermalManager.degBed() > target_bed || thermalManager.degProbe() > target_probe)
      report_temps(next_temp_report);

    // Disable leveling so it won't mess with us
    TERN_(HAS_LEVELING, set_bed_leveling_enabled(false));

    for (;;) {
      thermalManager.setTargetBed(target_bed);

      report_targets(target_bed, target_probe);

      // Park nozzle
      do_blocking_move_to(parkpos);

      // Wait for heatbed to reach target temp and probe to cool below target temp
      if (wait_for_temps(target_bed, target_probe, next_temp_report, millis() + MIN_TO_MS(15))) {
        SERIAL_ECHOLNPGM("!Bed heating timeout.");
        break;
      }

      // Move the nozzle to the probing point and wait for the probe to reach target temp
      do_blocking_move_to(noz_pos_xyz);
      say_waiting_for_probe_heating();
      SERIAL_EOL();
      while (thermalManager.degProbe() < target_probe)
        report_temps(next_temp_report);

      const float measured_z = g76_probe(TSI_BED, target_bed, noz_pos_xyz);
      if (isnan(measured_z) || target_bed > BED_MAX_TARGET) break;
    }

    SERIAL_ECHOLNPAIR("Retrieved measurements: ", temp_comp.get_index());
    if (temp_comp.finish_calibration(TSI_BED)) {
      say_successfully_calibrated();
      SERIAL_ECHOLNPGM(" bed.");
    }
    else {
      say_failed_to_calibrate();
      SERIAL_ECHOLNPGM(" bed. Values reset.");
    }

    // Cleanup
    thermalManager.setTargetBed(0);
    TERN_(HAS_LEVELING, set_bed_leveling_enabled(true));
  } // do_bed_cal

  /********************************************
   * Calibrate probe temperature offsets
   ********************************************/

  if (do_probe_cal) {

    // Park nozzle
    do_blocking_move_to(parkpos);

    // Initialize temperatures
    const uint16_t target_bed = temp_comp.probe_calib_bed_temp;
    thermalManager.setTargetBed(target_bed);

    uint16_t target_probe = cali_info_init[TSI_PROBE].start_temp;

    report_targets(target_bed, target_probe);

    // Wait for heatbed to reach target temp and probe to cool below target temp
    wait_for_temps(target_bed, target_probe, next_temp_report);

    // Disable leveling so it won't mess with us
    TERN_(HAS_LEVELING, set_bed_leveling_enabled(false));

    bool timeout = false;
    for (;;) {
      // Move probe to probing point and wait for it to reach target temperature
      do_blocking_move_to(noz_pos_xyz);

      say_waiting_for_probe_heating();
      SERIAL_ECHOLNPAIR(" Bed:", target_bed, " Probe:", target_probe);
      const millis_t probe_timeout_ms = millis() + 900UL * 1000UL;
      while (thermalManager.degProbe() < target_probe) {
        if (report_temps(next_temp_report, probe_timeout_ms)) {
          SERIAL_ECHOLNPGM("!Probe heating timed out.");
          timeout = true;
          break;
        }
      }
      if (timeout) break;

      const float measured_z = g76_probe(TSI_PROBE, target_probe, noz_pos_xyz);
      if (isnan(measured_z) || target_probe > cali_info_init[TSI_PROBE].end_temp) break;
    }

    SERIAL_ECHOLNPAIR("Retrieved measurements: ", temp_comp.get_index());
    if (temp_comp.finish_calibration(TSI_PROBE))
      say_successfully_calibrated();
    else
      say_failed_to_calibrate();
    SERIAL_ECHOLNPGM(" probe.");

    // Cleanup
    thermalManager.setTargetBed(0);
    TERN_(HAS_LEVELING, set_bed_leveling_enabled(true));

    SERIAL_ECHOLNPGM("Final compensation values:");
    temp_comp.print_offsets();
  } // do_probe_cal

  restore_feedrate_and_scaling();
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
    const int16_t offset_val = parser.value_int();
    if (!parser.seenval('I')) return;
    const int16_t idx = parser.value_int();
    const TempSensorID mod = (parser.seen('B') ? TSI_BED :
                                #if ENABLED(USE_TEMP_EXT_COMPENSATION)
                                  parser.seen('E') ? TSI_EXT :
                                #endif
                                TSI_PROBE
                              );
    if (idx > 0 && temp_comp.set_offset(mod, idx - 1, offset_val))
      SERIAL_ECHOLNPAIR("Set value: ", offset_val);
    else
      SERIAL_ECHOLNPGM("!Invalid index. Failed to set value (note: value at index 0 is constant).");

  }
  else // Print current Z-probe adjustments. Note: Values in EEPROM might differ.
    temp_comp.print_offsets();
}

/**
 * M192: Wait for probe temperature sensor to reach a target
 *
 * Select only one of these flags:
 *    R - Wait for heating or cooling
 *    S - Wait only for heating
 */
void GcodeSuite::M192() {
  if (DEBUGGING(DRYRUN)) return;

  const bool no_wait_for_cooling = parser.seenval('S');
  if (!no_wait_for_cooling && ! parser.seenval('R')) {
    SERIAL_ERROR_MSG("No target temperature set.");
    return;
  }

  const float target_temp = parser.value_celsius();
  ui.set_status_P(thermalManager.isProbeBelowTemp(target_temp) ? GET_TEXT(MSG_PROBE_HEATING) : GET_TEXT(MSG_PROBE_COOLING));
  thermalManager.wait_for_probe(target_temp, no_wait_for_cooling);
}

#endif // PROBE_TEMP_COMPENSATION
