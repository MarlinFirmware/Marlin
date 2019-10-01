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
 * probe.cpp
 */

#include "../inc/MarlinConfig.h"

#if HAS_BED_PROBE

#include "probe.h"

#include "../libs/buzzer.h"
#include "motion.h"
#include "temperature.h"
#include "endstops.h"

#include "../gcode/gcode.h"
#include "../lcd/ultralcd.h"

#include "../Marlin.h" // for stop(), disable_e_steppers, wait_for_user

#if HAS_LEVELING
  #include "../feature/bedlevel/bedlevel.h"
#endif

#if ENABLED(DELTA)
  #include "delta.h"
#endif

#if ENABLED(BABYSTEP_ZPROBE_OFFSET)
  #include "planner.h"
#endif

#if ENABLED(MEASURE_BACKLASH_WHEN_PROBING)
  #include "../feature/backlash.h"
#endif

xyz_pos_t probe_offset; // Initialized by settings.load()

#if ENABLED(BLTOUCH)
  #include "../feature/bltouch.h"
#endif

#if ENABLED(HOST_PROMPT_SUPPORT)
  #include "../feature/host_actions.h" // for PROMPT_USER_CONTINUE
#endif

#if HAS_Z_SERVO_PROBE
  #include "servo.h"
#endif

#if ENABLED(SENSORLESS_PROBING)
  #include "stepper.h"
  #include "../feature/tmc_util.h"
#endif

#if QUIET_PROBING
  #include "stepper/indirection.h"
#endif

#if ENABLED(EXTENSIBLE_UI)
  #include "../lcd/extensible_ui/ui_api.h"
#endif

#define DEBUG_OUT ENABLED(DEBUG_LEVELING_FEATURE)
#include "../core/debug_out.h"

#if ENABLED(Z_PROBE_SLED)

  #ifndef SLED_DOCKING_OFFSET
    #define SLED_DOCKING_OFFSET 0
  #endif

  /**
   * Method to dock/undock a sled designed by Charles Bell.
   *
   * stow[in]     If false, move to MAX_X and engage the solenoid
   *              If true, move to MAX_X and release the solenoid
   */
  static void dock_sled(bool stow) {
    if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPAIR("dock_sled(", stow, ")");

    // Dock sled a bit closer to ensure proper capturing
    do_blocking_move_to_x(X_MAX_POS + SLED_DOCKING_OFFSET - ((stow) ? 1 : 0));

    #if HAS_SOLENOID_1 && DISABLED(EXT_SOLENOID)
      WRITE(SOL1_PIN, !stow); // switch solenoid
    #endif
  }

#elif ENABLED(TOUCH_MI_PROBE)

  // Move to the magnet to unlock the probe
  void run_deploy_moves_script() {
    #if TOUCH_MI_DEPLOY_XPOS > X_MAX_BED
      TemporaryGlobalEndstopsState unlock_x(false);
    #endif
    #if TOUCH_MI_DEPLOY_YPOS > Y_MAX_BED
      TemporaryGlobalEndstopsState unlock_y(false);
    #endif

    #if ENABLED(TOUCH_MI_MANUAL_DEPLOY)

      const screenFunc_t prev_screen = ui.currentScreen;
      LCD_MESSAGEPGM(MSG_MANUAL_DEPLOY_TOUCHMI);
      ui.return_to_status();

      KEEPALIVE_STATE(PAUSED_FOR_USER);
      wait_for_user = true; // LCD click or M108 will clear this
      #if ENABLED(HOST_PROMPT_SUPPORT)
        host_prompt_do(PROMPT_USER_CONTINUE, PSTR("Deploy TouchMI probe."), PSTR("Continue"));
      #endif
      while (wait_for_user) idle();
      ui.reset_status();
      ui.goto_screen(prev_screen);

    #elif defined(TOUCH_MI_DEPLOY_XPOS) && defined(TOUCH_MI_DEPLOY_YPOS)
      do_blocking_move_to_xy(TOUCH_MI_DEPLOY_XPOS, TOUCH_MI_DEPLOY_YPOS);
    #elif defined(TOUCH_MI_DEPLOY_XPOS)
      do_blocking_move_to_x(TOUCH_MI_DEPLOY_XPOS);
    #elif defined(TOUCH_MI_DEPLOY_YPOS)
      do_blocking_move_to_y(TOUCH_MI_DEPLOY_YPOS);
    #endif
  }

  // Move down to the bed to stow the probe
  void run_stow_moves_script() {
    const xyz_pos_t oldpos = current_position;
    endstops.enable_z_probe(false);
    do_blocking_move_to_z(TOUCH_MI_RETRACT_Z, MMM_TO_MMS(HOMING_FEEDRATE_Z));
    do_blocking_move_to(oldpos, MMM_TO_MMS(HOMING_FEEDRATE_Z));
  }

#elif ENABLED(Z_PROBE_ALLEN_KEY)

  void run_deploy_moves_script() {
    #ifdef Z_PROBE_ALLEN_KEY_DEPLOY_1
      #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_1_FEEDRATE
        #define Z_PROBE_ALLEN_KEY_DEPLOY_1_FEEDRATE 0.0
      #endif
      constexpr xyz_pos_t deploy_1 = Z_PROBE_ALLEN_KEY_DEPLOY_1;
      do_blocking_move_to(deploy_1, MMM_TO_MMS(Z_PROBE_ALLEN_KEY_DEPLOY_1_FEEDRATE));
    #endif
    #ifdef Z_PROBE_ALLEN_KEY_DEPLOY_2
      #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_2_FEEDRATE
        #define Z_PROBE_ALLEN_KEY_DEPLOY_2_FEEDRATE 0.0
      #endif
      constexpr xyz_pos_t deploy_2 = Z_PROBE_ALLEN_KEY_DEPLOY_2;
      do_blocking_move_to(deploy_2, MMM_TO_MMS(Z_PROBE_ALLEN_KEY_DEPLOY_2_FEEDRATE));
    #endif
    #ifdef Z_PROBE_ALLEN_KEY_DEPLOY_3
      #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_3_FEEDRATE
        #define Z_PROBE_ALLEN_KEY_DEPLOY_3_FEEDRATE 0.0
      #endif
      constexpr xyz_pos_t deploy_3 = Z_PROBE_ALLEN_KEY_DEPLOY_3;
      do_blocking_move_to(deploy_3, MMM_TO_MMS(Z_PROBE_ALLEN_KEY_DEPLOY_3_FEEDRATE));
    #endif
    #ifdef Z_PROBE_ALLEN_KEY_DEPLOY_4
      #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_4_FEEDRATE
        #define Z_PROBE_ALLEN_KEY_DEPLOY_4_FEEDRATE 0.0
      #endif
      constexpr xyz_pos_t deploy_4 = Z_PROBE_ALLEN_KEY_DEPLOY_4;
      do_blocking_move_to(deploy_4, MMM_TO_MMS(Z_PROBE_ALLEN_KEY_DEPLOY_4_FEEDRATE));
    #endif
    #ifdef Z_PROBE_ALLEN_KEY_DEPLOY_5
      #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_5_FEEDRATE
        #define Z_PROBE_ALLEN_KEY_DEPLOY_5_FEEDRATE 0.0
      #endif
      constexpr xyz_pos_t deploy_5 = Z_PROBE_ALLEN_KEY_DEPLOY_5;
      do_blocking_move_to(deploy_5, MMM_TO_MMS(Z_PROBE_ALLEN_KEY_DEPLOY_5_FEEDRATE));
    #endif
  }

  void run_stow_moves_script() {
    #ifdef Z_PROBE_ALLEN_KEY_STOW_1
      #ifndef Z_PROBE_ALLEN_KEY_STOW_1_FEEDRATE
        #define Z_PROBE_ALLEN_KEY_STOW_1_FEEDRATE 0.0
      #endif
      constexpr xyz_pos_t stow_1 = Z_PROBE_ALLEN_KEY_STOW_1;
      do_blocking_move_to(stow_1, MMM_TO_MMS(Z_PROBE_ALLEN_KEY_STOW_1_FEEDRATE));
    #endif
    #ifdef Z_PROBE_ALLEN_KEY_STOW_2
      #ifndef Z_PROBE_ALLEN_KEY_STOW_2_FEEDRATE
        #define Z_PROBE_ALLEN_KEY_STOW_2_FEEDRATE 0.0
      #endif
      constexpr xyz_pos_t stow_2 = Z_PROBE_ALLEN_KEY_STOW_2;
      do_blocking_move_to(stow_2, MMM_TO_MMS(Z_PROBE_ALLEN_KEY_STOW_2_FEEDRATE));
    #endif
    #ifdef Z_PROBE_ALLEN_KEY_STOW_3
      #ifndef Z_PROBE_ALLEN_KEY_STOW_3_FEEDRATE
        #define Z_PROBE_ALLEN_KEY_STOW_3_FEEDRATE 0.0
      #endif
      constexpr xyz_pos_t stow_3 = Z_PROBE_ALLEN_KEY_STOW_3;
      do_blocking_move_to(stow_3, MMM_TO_MMS(Z_PROBE_ALLEN_KEY_STOW_3_FEEDRATE));
    #endif
    #ifdef Z_PROBE_ALLEN_KEY_STOW_4
      #ifndef Z_PROBE_ALLEN_KEY_STOW_4_FEEDRATE
        #define Z_PROBE_ALLEN_KEY_STOW_4_FEEDRATE 0.0
      #endif
      constexpr xyz_pos_t stow_4 = Z_PROBE_ALLEN_KEY_STOW_4;
      do_blocking_move_to(stow_4, MMM_TO_MMS(Z_PROBE_ALLEN_KEY_STOW_4_FEEDRATE));
    #endif
    #ifdef Z_PROBE_ALLEN_KEY_STOW_5
      #ifndef Z_PROBE_ALLEN_KEY_STOW_5_FEEDRATE
        #define Z_PROBE_ALLEN_KEY_STOW_5_FEEDRATE 0.0
      #endif
      constexpr xyz_pos_t stow_5 = Z_PROBE_ALLEN_KEY_STOW_5;
      do_blocking_move_to(stow_5, MMM_TO_MMS(Z_PROBE_ALLEN_KEY_STOW_5_FEEDRATE));
    #endif
  }

#endif // Z_PROBE_ALLEN_KEY

#if QUIET_PROBING
  void probing_pause(const bool p) {
    #if ENABLED(PROBING_HEATERS_OFF)
      thermalManager.pause(p);
    #endif
    #if ENABLED(PROBING_FANS_OFF)
      thermalManager.set_fans_paused(p);
    #endif
    #if ENABLED(PROBING_STEPPERS_OFF)
      disable_e_steppers();
      #if NONE(DELTA, HOME_AFTER_DEACTIVATE)
        disable_X(); disable_Y();
      #endif
    #endif
    if (p) safe_delay(
      #if DELAY_BEFORE_PROBING > 25
        DELAY_BEFORE_PROBING
      #else
        25
      #endif
    );
  }
#endif // QUIET_PROBING

/**
 * Raise Z to a minimum height to make room for a probe to move
 */
inline void do_probe_raise(const float z_raise) {
  if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPAIR("do_probe_raise(", z_raise, ")");

  float z_dest = z_raise;
  if (probe_offset.z < 0) z_dest -= probe_offset.z;

  NOMORE(z_dest, Z_MAX_POS);

  if (z_dest > current_position.z)
    do_blocking_move_to_z(z_dest);
}

FORCE_INLINE void probe_specific_action(const bool deploy) {
  #if ENABLED(PAUSE_BEFORE_DEPLOY_STOW)
    do {
      #if ENABLED(PAUSE_PROBE_DEPLOY_WHEN_TRIGGERED)
        if (deploy == (READ(Z_MIN_PROBE_PIN) == Z_MIN_PROBE_ENDSTOP_INVERTING)) break;
      #endif

      BUZZ(100, 659);
      BUZZ(100, 698);

      PGM_P const ds_str = deploy ? PSTR(MSG_MANUAL_DEPLOY) : PSTR(MSG_MANUAL_STOW);
      ui.return_to_status();       // To display the new status message
      ui.set_status_P(ds_str, 99);
      serialprintPGM(ds_str);
      SERIAL_EOL();

      KEEPALIVE_STATE(PAUSED_FOR_USER);
      wait_for_user = true;
      #if ENABLED(HOST_PROMPT_SUPPORT)
        host_prompt_do(PROMPT_USER_CONTINUE, PSTR("Stow Probe"), PSTR("Continue"));
      #endif
      #if ENABLED(EXTENSIBLE_UI)
        ExtUI::onUserConfirmRequired(PSTR("Stow Probe"));
      #endif
      while (wait_for_user) idle();
      ui.reset_status();

    } while(
      #if ENABLED(PAUSE_PROBE_DEPLOY_WHEN_TRIGGERED)
        true
      #else
        false
      #endif
    );

  #endif // PAUSE_BEFORE_DEPLOY_STOW

  #if ENABLED(SOLENOID_PROBE)

    #if HAS_SOLENOID_1
      WRITE(SOL1_PIN, deploy);
    #endif

  #elif ENABLED(Z_PROBE_SLED)

    dock_sled(!deploy);

  #elif HAS_Z_SERVO_PROBE

    #if DISABLED(BLTOUCH)
      MOVE_SERVO(Z_PROBE_SERVO_NR, servo_angles[Z_PROBE_SERVO_NR][deploy ? 0 : 1]);
    #elif ENABLED(BLTOUCH_HS_MODE)
      // In HIGH SPEED MODE, use the normal retractable probe logic in this code
      // i.e. no intermediate STOWs and DEPLOYs in between individual probe actions
      if (deploy) bltouch.deploy(); else bltouch.stow();
    #endif

  #elif EITHER(TOUCH_MI_PROBE, Z_PROBE_ALLEN_KEY)

    deploy ? run_deploy_moves_script() : run_stow_moves_script();

  #elif ENABLED(RACK_AND_PINION_PROBE)

    do_blocking_move_to_x(deploy ? Z_PROBE_DEPLOY_X : Z_PROBE_RETRACT_X);

  #elif DISABLED(PAUSE_BEFORE_DEPLOY_STOW)

    UNUSED(deploy);

  #endif
}

// returns false for ok and true for failure
bool set_probe_deployed(const bool deploy) {

  if (DEBUGGING(LEVELING)) {
    DEBUG_POS("set_probe_deployed", current_position);
    DEBUG_ECHOLNPAIR("deploy: ", deploy);
  }

  if (endstops.z_probe_enabled == deploy) return false;

  // Make room for probe to deploy (or stow)
  // Fix-mounted probe should only raise for deploy
  // unless PAUSE_BEFORE_DEPLOY_STOW is enabled
  #if ENABLED(FIX_MOUNTED_PROBE) && DISABLED(PAUSE_BEFORE_DEPLOY_STOW)
    const bool deploy_stow_condition = deploy;
  #else
    constexpr bool deploy_stow_condition = true;
  #endif

  // For beds that fall when Z is powered off only raise for trusted Z
  #if ENABLED(UNKNOWN_Z_NO_RAISE)
    const bool unknown_condition = TEST(axis_known_position, Z_AXIS);
  #else
    constexpr float unknown_condition = true;
  #endif

  if (deploy_stow_condition && unknown_condition)
    do_probe_raise(_MAX(Z_CLEARANCE_BETWEEN_PROBES, Z_CLEARANCE_DEPLOY_PROBE));

  #if EITHER(Z_PROBE_SLED, Z_PROBE_ALLEN_KEY)
    if (axis_unhomed_error(
      #if ENABLED(Z_PROBE_SLED)
        _BV(X_AXIS)
      #endif
    )) {
      SERIAL_ERROR_MSG(MSG_STOP_UNHOMED);
      stop();
      return true;
    }
  #endif

  const xy_pos_t old_xy = current_position;

  #if ENABLED(PROBE_TRIGGERED_WHEN_STOWED_TEST)
    #if USES_Z_MIN_PROBE_ENDSTOP
      #define PROBE_STOWED() (READ(Z_MIN_PROBE_PIN) != Z_MIN_PROBE_ENDSTOP_INVERTING)
    #else
      #define PROBE_STOWED() (READ(Z_MIN_PIN) != Z_MIN_ENDSTOP_INVERTING)
    #endif
  #endif

  #ifdef PROBE_STOWED

    // Only deploy/stow if needed
    if (PROBE_STOWED() == deploy) {
      if (!deploy) endstops.enable_z_probe(false); // Switch off triggered when stowed probes early
                                                   // otherwise an Allen-Key probe can't be stowed.
      probe_specific_action(deploy);
    }

    if (PROBE_STOWED() == deploy) {                // Unchanged after deploy/stow action?
      if (IsRunning()) {
        SERIAL_ERROR_MSG("Z-Probe failed");
        LCD_ALERTMESSAGEPGM("Err: ZPROBE");
      }
      stop();
      return true;
    }

  #else

    probe_specific_action(deploy);

  #endif

  do_blocking_move_to(old_xy);
  endstops.enable_z_probe(deploy);
  return false;
}

#ifdef Z_AFTER_PROBING
  // After probing move to a preferred Z position
  void move_z_after_probing() {
    if (current_position.z != Z_AFTER_PROBING) {
      do_blocking_move_to_z(Z_AFTER_PROBING);
      current_position.z = Z_AFTER_PROBING;
    }
  }
#endif

/**
 * @brief Used by run_z_probe to do a single Z probe move.
 *
 * @param  z        Z destination
 * @param  fr_mm_s  Feedrate in mm/s
 * @return true to indicate an error
 */

#if HAS_HEATED_BED && ENABLED(WAIT_FOR_BED_HEATER)
  const char msg_wait_for_bed_heating[25] PROGMEM = "Wait for bed heating...\n";
#endif

static bool do_probe_move(const float z, const feedRate_t fr_mm_s) {
  if (DEBUGGING(LEVELING)) DEBUG_POS(">>> do_probe_move", current_position);

  #if HAS_HEATED_BED && ENABLED(WAIT_FOR_BED_HEATER)
    // Wait for bed to heat back up between probing points
    if (thermalManager.isHeatingBed()) {
      serialprintPGM(msg_wait_for_bed_heating);
      LCD_MESSAGEPGM(MSG_BED_HEATING);
      thermalManager.wait_for_bed();
      ui.reset_status();
    }
  #endif

  #if ENABLED(BLTOUCH) && DISABLED(BLTOUCH_HS_MODE)
    if (bltouch.deploy()) return true; // DEPLOY in LOW SPEED MODE on every probe action
  #endif

  // Disable stealthChop if used. Enable diag1 pin on driver.
  #if ENABLED(SENSORLESS_PROBING)
    sensorless_t stealth_states { false };
    #if ENABLED(DELTA)
      stealth_states.x = tmc_enable_stallguard(stepperX);
      stealth_states.y = tmc_enable_stallguard(stepperY);
    #endif
    stealth_states.z = tmc_enable_stallguard(stepperZ);
    endstops.enable(true);
  #endif

  #if QUIET_PROBING
    probing_pause(true);
  #endif

  // Move down until the probe is triggered
  do_blocking_move_to_z(z, fr_mm_s);

  // Check to see if the probe was triggered
  const bool probe_triggered =
    #if BOTH(DELTA, SENSORLESS_PROBING)
      endstops.trigger_state() & (_BV(X_MIN) | _BV(Y_MIN) | _BV(Z_MIN))
    #else
      TEST(endstops.trigger_state(),
        #if ENABLED(Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN)
          Z_MIN
        #else
          Z_MIN_PROBE
        #endif
      )
    #endif
  ;

  #if QUIET_PROBING
    probing_pause(false);
  #endif

  // Re-enable stealthChop if used. Disable diag1 pin on driver.
  #if ENABLED(SENSORLESS_PROBING)
    endstops.not_homing();
    #if ENABLED(DELTA)
      tmc_disable_stallguard(stepperX, stealth_states.x);
      tmc_disable_stallguard(stepperY, stealth_states.y);
    #endif
    tmc_disable_stallguard(stepperZ, stealth_states.z);
  #endif

  #if ENABLED(BLTOUCH) && DISABLED(BLTOUCH_HS_MODE)
    if (probe_triggered && bltouch.stow()) return true; // STOW in LOW SPEED MODE on trigger on every probe action
  #endif

  // Clear endstop flags
  endstops.hit_on_purpose();

  // Get Z where the steppers were interrupted
  set_current_from_steppers_for_axis(Z_AXIS);

  // Tell the planner where we actually are
  sync_plan_position();

  if (DEBUGGING(LEVELING)) DEBUG_POS("<<< do_probe_move", current_position);

  return !probe_triggered;
}

/**
 * @brief Probe at the current XY (possibly more than once) to find the bed Z.
 *
 * @details Used by probe_at_point to get the bed Z height at the current XY.
 *          Leaves current_position.z at the height where the probe triggered.
 *
 * @return The Z position of the bed at the current XY or NAN on error.
 */
static float run_z_probe() {

  if (DEBUGGING(LEVELING)) DEBUG_POS(">>> run_z_probe", current_position);

  // Stop the probe before it goes too low to prevent damage.
  // If Z isn't known then probe to -10mm.
  const float z_probe_low_point = TEST(axis_known_position, Z_AXIS) ? -probe_offset.z + Z_PROBE_LOW_POINT : -10.0;

  // Double-probing does a fast probe followed by a slow probe
  #if TOTAL_PROBING == 2

    // Do a first probe at the fast speed
    if (do_probe_move(z_probe_low_point, MMM_TO_MMS(Z_PROBE_SPEED_FAST))) {
      if (DEBUGGING(LEVELING)) {
        DEBUG_ECHOLNPGM("FAST Probe fail!");
        DEBUG_POS("<<< run_z_probe", current_position);
      }
      return NAN;
    }

    const float first_probe_z = current_position.z;

    if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPAIR("1st Probe Z:", first_probe_z);

    // Raise to give the probe clearance
    do_blocking_move_to_z(current_position.z + Z_CLEARANCE_MULTI_PROBE, MMM_TO_MMS(Z_PROBE_SPEED_FAST));

  #elif Z_PROBE_SPEED_FAST != Z_PROBE_SPEED_SLOW

    // If the nozzle is well over the travel height then
    // move down quickly before doing the slow probe
    const float z = Z_CLEARANCE_DEPLOY_PROBE + 5.0 + (probe_offset.z < 0 ? -probe_offset.z : 0);
    if (current_position.z > z) {
      // Probe down fast. If the probe never triggered, raise for probe clearance
      if (!do_probe_move(z, MMM_TO_MMS(Z_PROBE_SPEED_FAST)))
        do_blocking_move_to_z(current_position.z + Z_CLEARANCE_BETWEEN_PROBES, MMM_TO_MMS(Z_PROBE_SPEED_FAST));
    }
  #endif

  #ifdef EXTRA_PROBING
    float probes[TOTAL_PROBING];
  #endif

  #if TOTAL_PROBING > 2
    float probes_total = 0;
    for (
      #if EXTRA_PROBING
        uint8_t p = 0; p < TOTAL_PROBING; p++
      #else
        uint8_t p = TOTAL_PROBING; p--;
      #endif
    )
  #endif
    {
      // Probe downward slowly to find the bed
      if (do_probe_move(z_probe_low_point, MMM_TO_MMS(Z_PROBE_SPEED_SLOW))) {
        if (DEBUGGING(LEVELING)) {
          DEBUG_ECHOLNPGM("SLOW Probe fail!");
          DEBUG_POS("<<< run_z_probe", current_position);
        }
        return NAN;
      }

      #if ENABLED(MEASURE_BACKLASH_WHEN_PROBING)
        backlash.measure_with_probe();
      #endif

      const float z = current_position.z;

      #if EXTRA_PROBING
        // Insert Z measurement into probes[]. Keep it sorted ascending.
        for (uint8_t i = 0; i <= p; i++) {                            // Iterate the saved Zs to insert the new Z
          if (i == p || probes[i] > z) {                              // Last index or new Z is smaller than this Z
            for (int8_t m = p; --m >= i;) probes[m + 1] = probes[m];  // Shift items down after the insertion point
            probes[i] = z;                                            // Insert the new Z measurement
            break;                                                    // Only one to insert. Done!
          }
        }
      #elif TOTAL_PROBING > 2
        probes_total += z;
      #else
        UNUSED(z);
      #endif

      #if TOTAL_PROBING > 2
        // Small Z raise after all but the last probe
        if (p
          #if EXTRA_PROBING
            < TOTAL_PROBING - 1
          #endif
        ) do_blocking_move_to_z(z + Z_CLEARANCE_MULTI_PROBE, MMM_TO_MMS(Z_PROBE_SPEED_FAST));
      #endif
    }

  #if TOTAL_PROBING > 2

    #if EXTRA_PROBING
      // Take the center value (or average the two middle values) as the median
      static constexpr int PHALF = (TOTAL_PROBING - 1) / 2;
      const float middle = probes[PHALF],
                  median = ((TOTAL_PROBING) & 1) ? middle : (middle + probes[PHALF + 1]) * 0.5f;

      // Remove values farthest from the median
      uint8_t min_avg_idx = 0, max_avg_idx = TOTAL_PROBING - 1;
      for (uint8_t i = EXTRA_PROBING; i--;)
        if (ABS(probes[max_avg_idx] - median) > ABS(probes[min_avg_idx] - median))
          max_avg_idx--; else min_avg_idx++;

      // Return the average value of all remaining probes.
      for (uint8_t i = min_avg_idx; i <= max_avg_idx; i++)
        probes_total += probes[i];

    #endif

    const float measured_z = probes_total * RECIPROCAL(MULTIPLE_PROBING);

  #elif TOTAL_PROBING == 2

    const float z2 = current_position.z;

    if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPAIR("2nd Probe Z:", z2, " Discrepancy:", first_probe_z - z2);

    // Return a weighted average of the fast and slow probes
    const float measured_z = (z2 * 3.0 + first_probe_z * 2.0) * 0.2;

  #else

    // Return the single probe result
    const float measured_z = current_position.z;

  #endif

  if (DEBUGGING(LEVELING)) DEBUG_POS("<<< run_z_probe", current_position);

  return measured_z;
}

/**
 * - Move to the given XY
 * - Deploy the probe, if not already deployed
 * - Probe the bed, get the Z position
 * - Depending on the 'stow' flag
 *   - Stow the probe, or
 *   - Raise to the BETWEEN height
 * - Return the probed Z position
 */
float probe_at_point(const float &rx, const float &ry, const ProbePtRaise raise_after/*=PROBE_PT_NONE*/, const uint8_t verbose_level/*=0*/, const bool probe_relative/*=true*/) {
  if (DEBUGGING(LEVELING)) {
    DEBUG_ECHOLNPAIR(
      ">>> probe_at_point(", LOGICAL_X_POSITION(rx), ", ", LOGICAL_Y_POSITION(ry),
      ", ", raise_after == PROBE_PT_RAISE ? "raise" : raise_after == PROBE_PT_STOW ? "stow" : "none",
      ", ", int(verbose_level),
      ", ", probe_relative ? "probe" : "nozzle", "_relative)"
    );
    DEBUG_POS("", current_position);
  }

  // TODO: Adapt for SCARA, where the offset rotates
  xyz_pos_t npos = { rx, ry };
  if (probe_relative) {
    if (!position_is_reachable_by_probe(npos)) return NAN;  // The given position is in terms of the probe
    npos -= probe_offset;                                   // Get the nozzle position
  }
  else if (!position_is_reachable(npos)) return NAN;        // The given position is in terms of the nozzle

  npos.z =
    #if ENABLED(DELTA)
      // Move below clip height or xy move will be aborted by do_blocking_move_to
      _MIN(current_position.z, delta_clip_start_height)
    #else
      current_position.z
    #endif
  ;

  const float old_feedrate_mm_s = feedrate_mm_s;
  feedrate_mm_s = XY_PROBE_FEEDRATE_MM_S;

  // Move the probe to the starting XYZ
  do_blocking_move_to(npos);

  float measured_z = NAN;
  if (!DEPLOY_PROBE()) {
    measured_z = run_z_probe() + probe_offset.z;

    const bool big_raise = raise_after == PROBE_PT_BIG_RAISE;
    if (big_raise || raise_after == PROBE_PT_RAISE)
      do_blocking_move_to_z(current_position.z + (big_raise ? 25 : Z_CLEARANCE_BETWEEN_PROBES), MMM_TO_MMS(Z_PROBE_SPEED_FAST));
    else if (raise_after == PROBE_PT_STOW)
      if (STOW_PROBE()) measured_z = NAN;
  }

  if (verbose_level > 2) {
    SERIAL_ECHOPAIR_F("Bed X: ", LOGICAL_X_POSITION(rx), 3);
    SERIAL_ECHOPAIR_F(" Y: ", LOGICAL_Y_POSITION(ry), 3);
    SERIAL_ECHOLNPAIR_F(" Z: ", measured_z, 3);
  }

  feedrate_mm_s = old_feedrate_mm_s;

  if (isnan(measured_z)) {
    STOW_PROBE();
    LCD_MESSAGEPGM(MSG_ERR_PROBING_FAILED);
    SERIAL_ERROR_MSG(MSG_ERR_PROBING_FAILED);
  }

  if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPGM("<<< probe_at_point");

  return measured_z;
}

#if HAS_Z_SERVO_PROBE

  void servo_probe_init() {
    /**
     * Set position of Z Servo Endstop
     *
     * The servo might be deployed and positioned too low to stow
     * when starting up the machine or rebooting the board.
     * There's no way to know where the nozzle is positioned until
     * homing has been done - no homing with z-probe without init!
     *
     */
    STOW_Z_SERVO();
  }

#endif // HAS_Z_SERVO_PROBE

#endif // HAS_BED_PROBE
