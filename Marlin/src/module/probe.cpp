/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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
#include "motion.h"
#include "temperature.h"
#include "endstops.h"

#include "../gcode/gcode.h"
#include "../lcd/ultralcd.h"

#include "../Marlin.h"

#if HAS_LEVELING
  #include "../feature/bedlevel/bedlevel.h"
#endif

#if ENABLED(DELTA)
  #include "../module/delta.h"
#endif

#if ENABLED(BABYSTEP_ZPROBE_OFFSET)
  #include "planner.h"
#endif

float zprobe_zoffset; // Initialized by settings.load()

#if HAS_Z_SERVO_PROBE
  #include "../module/servo.h"
  const int z_servo_angle[2] = Z_SERVO_ANGLES;
#endif

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
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) {
        SERIAL_ECHOPAIR("dock_sled(", stow);
        SERIAL_CHAR(')');
        SERIAL_EOL();
      }
    #endif

    // Dock sled a bit closer to ensure proper capturing
    do_blocking_move_to_x(X_MAX_POS + SLED_DOCKING_OFFSET - ((stow) ? 1 : 0));

    #if HAS_SOLENOID_1 && DISABLED(EXT_SOLENOID)
      WRITE(SOL1_PIN, !stow); // switch solenoid
    #endif
  }

#elif ENABLED(Z_PROBE_ALLEN_KEY)

  FORCE_INLINE void do_blocking_move_to(const float (&raw)[XYZ], const float &fr_mm_s) {
    do_blocking_move_to(raw[X_AXIS], raw[Y_AXIS], raw[Z_AXIS], fr_mm_s);
  }

  void run_deploy_moves_script() {
    #if defined(Z_PROBE_ALLEN_KEY_DEPLOY_1_X) || defined(Z_PROBE_ALLEN_KEY_DEPLOY_1_Y) || defined(Z_PROBE_ALLEN_KEY_DEPLOY_1_Z)
      #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_1_X
        #define Z_PROBE_ALLEN_KEY_DEPLOY_1_X current_position[X_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_1_Y
        #define Z_PROBE_ALLEN_KEY_DEPLOY_1_Y current_position[Y_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_1_Z
        #define Z_PROBE_ALLEN_KEY_DEPLOY_1_Z current_position[Z_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_1_FEEDRATE
        #define Z_PROBE_ALLEN_KEY_DEPLOY_1_FEEDRATE 0.0
      #endif
      const float deploy_1[] = { Z_PROBE_ALLEN_KEY_DEPLOY_1_X, Z_PROBE_ALLEN_KEY_DEPLOY_1_Y, Z_PROBE_ALLEN_KEY_DEPLOY_1_Z };
      do_blocking_move_to(deploy_1, MMM_TO_MMS(Z_PROBE_ALLEN_KEY_DEPLOY_1_FEEDRATE));
    #endif
    #if defined(Z_PROBE_ALLEN_KEY_DEPLOY_2_X) || defined(Z_PROBE_ALLEN_KEY_DEPLOY_2_Y) || defined(Z_PROBE_ALLEN_KEY_DEPLOY_2_Z)
      #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_2_X
        #define Z_PROBE_ALLEN_KEY_DEPLOY_2_X current_position[X_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_2_Y
        #define Z_PROBE_ALLEN_KEY_DEPLOY_2_Y current_position[Y_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_2_Z
        #define Z_PROBE_ALLEN_KEY_DEPLOY_2_Z current_position[Z_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_2_FEEDRATE
        #define Z_PROBE_ALLEN_KEY_DEPLOY_2_FEEDRATE 0.0
      #endif
      const float deploy_2[] = { Z_PROBE_ALLEN_KEY_DEPLOY_2_X, Z_PROBE_ALLEN_KEY_DEPLOY_2_Y, Z_PROBE_ALLEN_KEY_DEPLOY_2_Z };
      do_blocking_move_to(deploy_2, MMM_TO_MMS(Z_PROBE_ALLEN_KEY_DEPLOY_2_FEEDRATE));
    #endif
    #if defined(Z_PROBE_ALLEN_KEY_DEPLOY_3_X) || defined(Z_PROBE_ALLEN_KEY_DEPLOY_3_Y) || defined(Z_PROBE_ALLEN_KEY_DEPLOY_3_Z)
      #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_3_X
        #define Z_PROBE_ALLEN_KEY_DEPLOY_3_X current_position[X_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_3_Y
        #define Z_PROBE_ALLEN_KEY_DEPLOY_3_Y current_position[Y_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_3_Z
        #define Z_PROBE_ALLEN_KEY_DEPLOY_3_Z current_position[Z_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_3_FEEDRATE
        #define Z_PROBE_ALLEN_KEY_DEPLOY_3_FEEDRATE 0.0
      #endif
      const float deploy_3[] = { Z_PROBE_ALLEN_KEY_DEPLOY_3_X, Z_PROBE_ALLEN_KEY_DEPLOY_3_Y, Z_PROBE_ALLEN_KEY_DEPLOY_3_Z };
      do_blocking_move_to(deploy_3, MMM_TO_MMS(Z_PROBE_ALLEN_KEY_DEPLOY_3_FEEDRATE));
    #endif
    #if defined(Z_PROBE_ALLEN_KEY_DEPLOY_4_X) || defined(Z_PROBE_ALLEN_KEY_DEPLOY_4_Y) || defined(Z_PROBE_ALLEN_KEY_DEPLOY_4_Z)
      #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_4_X
        #define Z_PROBE_ALLEN_KEY_DEPLOY_4_X current_position[X_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_4_Y
        #define Z_PROBE_ALLEN_KEY_DEPLOY_4_Y current_position[Y_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_4_Z
        #define Z_PROBE_ALLEN_KEY_DEPLOY_4_Z current_position[Z_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_4_FEEDRATE
        #define Z_PROBE_ALLEN_KEY_DEPLOY_4_FEEDRATE 0.0
      #endif
      const float deploy_4[] = { Z_PROBE_ALLEN_KEY_DEPLOY_4_X, Z_PROBE_ALLEN_KEY_DEPLOY_4_Y, Z_PROBE_ALLEN_KEY_DEPLOY_4_Z };
      do_blocking_move_to(deploy_4, MMM_TO_MMS(Z_PROBE_ALLEN_KEY_DEPLOY_4_FEEDRATE));
    #endif
    #if defined(Z_PROBE_ALLEN_KEY_DEPLOY_5_X) || defined(Z_PROBE_ALLEN_KEY_DEPLOY_5_Y) || defined(Z_PROBE_ALLEN_KEY_DEPLOY_5_Z)
      #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_5_X
        #define Z_PROBE_ALLEN_KEY_DEPLOY_5_X current_position[X_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_5_Y
        #define Z_PROBE_ALLEN_KEY_DEPLOY_5_Y current_position[Y_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_5_Z
        #define Z_PROBE_ALLEN_KEY_DEPLOY_5_Z current_position[Z_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_5_FEEDRATE
        #define Z_PROBE_ALLEN_KEY_DEPLOY_5_FEEDRATE 0.0
      #endif
      const float deploy_5[] = { Z_PROBE_ALLEN_KEY_DEPLOY_5_X, Z_PROBE_ALLEN_KEY_DEPLOY_5_Y, Z_PROBE_ALLEN_KEY_DEPLOY_5_Z };
      do_blocking_move_to(deploy_5, MMM_TO_MMS(Z_PROBE_ALLEN_KEY_DEPLOY_5_FEEDRATE));
    #endif
  }

  void run_stow_moves_script() {
    #if defined(Z_PROBE_ALLEN_KEY_STOW_1_X) || defined(Z_PROBE_ALLEN_KEY_STOW_1_Y) || defined(Z_PROBE_ALLEN_KEY_STOW_1_Z)
      #ifndef Z_PROBE_ALLEN_KEY_STOW_1_X
        #define Z_PROBE_ALLEN_KEY_STOW_1_X current_position[X_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_STOW_1_Y
        #define Z_PROBE_ALLEN_KEY_STOW_1_Y current_position[Y_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_STOW_1_Z
        #define Z_PROBE_ALLEN_KEY_STOW_1_Z current_position[Z_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_STOW_1_FEEDRATE
        #define Z_PROBE_ALLEN_KEY_STOW_1_FEEDRATE 0.0
      #endif
      const float stow_1[] = { Z_PROBE_ALLEN_KEY_STOW_1_X, Z_PROBE_ALLEN_KEY_STOW_1_Y, Z_PROBE_ALLEN_KEY_STOW_1_Z };
      do_blocking_move_to(stow_1, MMM_TO_MMS(Z_PROBE_ALLEN_KEY_STOW_1_FEEDRATE));
    #endif
    #if defined(Z_PROBE_ALLEN_KEY_STOW_2_X) || defined(Z_PROBE_ALLEN_KEY_STOW_2_Y) || defined(Z_PROBE_ALLEN_KEY_STOW_2_Z)
      #ifndef Z_PROBE_ALLEN_KEY_STOW_2_X
        #define Z_PROBE_ALLEN_KEY_STOW_2_X current_position[X_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_STOW_2_Y
        #define Z_PROBE_ALLEN_KEY_STOW_2_Y current_position[Y_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_STOW_2_Z
        #define Z_PROBE_ALLEN_KEY_STOW_2_Z current_position[Z_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_STOW_2_FEEDRATE
        #define Z_PROBE_ALLEN_KEY_STOW_2_FEEDRATE 0.0
      #endif
      const float stow_2[] = { Z_PROBE_ALLEN_KEY_STOW_2_X, Z_PROBE_ALLEN_KEY_STOW_2_Y, Z_PROBE_ALLEN_KEY_STOW_2_Z };
      do_blocking_move_to(stow_2, MMM_TO_MMS(Z_PROBE_ALLEN_KEY_STOW_2_FEEDRATE));
    #endif
    #if defined(Z_PROBE_ALLEN_KEY_STOW_3_X) || defined(Z_PROBE_ALLEN_KEY_STOW_3_Y) || defined(Z_PROBE_ALLEN_KEY_STOW_3_Z)
      #ifndef Z_PROBE_ALLEN_KEY_STOW_3_X
        #define Z_PROBE_ALLEN_KEY_STOW_3_X current_position[X_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_STOW_3_Y
        #define Z_PROBE_ALLEN_KEY_STOW_3_Y current_position[Y_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_STOW_3_Z
        #define Z_PROBE_ALLEN_KEY_STOW_3_Z current_position[Z_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_STOW_3_FEEDRATE
        #define Z_PROBE_ALLEN_KEY_STOW_3_FEEDRATE 0.0
      #endif
      const float stow_3[] = { Z_PROBE_ALLEN_KEY_STOW_3_X, Z_PROBE_ALLEN_KEY_STOW_3_Y, Z_PROBE_ALLEN_KEY_STOW_3_Z };
      do_blocking_move_to(stow_3, MMM_TO_MMS(Z_PROBE_ALLEN_KEY_STOW_3_FEEDRATE));
    #endif
    #if defined(Z_PROBE_ALLEN_KEY_STOW_4_X) || defined(Z_PROBE_ALLEN_KEY_STOW_4_Y) || defined(Z_PROBE_ALLEN_KEY_STOW_4_Z)
      #ifndef Z_PROBE_ALLEN_KEY_STOW_4_X
        #define Z_PROBE_ALLEN_KEY_STOW_4_X current_position[X_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_STOW_4_Y
        #define Z_PROBE_ALLEN_KEY_STOW_4_Y current_position[Y_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_STOW_4_Z
        #define Z_PROBE_ALLEN_KEY_STOW_4_Z current_position[Z_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_STOW_4_FEEDRATE
        #define Z_PROBE_ALLEN_KEY_STOW_4_FEEDRATE 0.0
      #endif
      const float stow_4[] = { Z_PROBE_ALLEN_KEY_STOW_4_X, Z_PROBE_ALLEN_KEY_STOW_4_Y, Z_PROBE_ALLEN_KEY_STOW_4_Z };
      do_blocking_move_to(stow_4, MMM_TO_MMS(Z_PROBE_ALLEN_KEY_STOW_4_FEEDRATE));
    #endif
    #if defined(Z_PROBE_ALLEN_KEY_STOW_5_X) || defined(Z_PROBE_ALLEN_KEY_STOW_5_Y) || defined(Z_PROBE_ALLEN_KEY_STOW_5_Z)
      #ifndef Z_PROBE_ALLEN_KEY_STOW_5_X
        #define Z_PROBE_ALLEN_KEY_STOW_5_X current_position[X_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_STOW_5_Y
        #define Z_PROBE_ALLEN_KEY_STOW_5_Y current_position[Y_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_STOW_5_Z
        #define Z_PROBE_ALLEN_KEY_STOW_5_Z current_position[Z_AXIS]
      #endif
      #ifndef Z_PROBE_ALLEN_KEY_STOW_5_FEEDRATE
        #define Z_PROBE_ALLEN_KEY_STOW_5_FEEDRATE 0.0
      #endif
      const float stow_5[] = { Z_PROBE_ALLEN_KEY_STOW_5_X, Z_PROBE_ALLEN_KEY_STOW_5_Y, Z_PROBE_ALLEN_KEY_STOW_5_Z };
      do_blocking_move_to(stow_5, MMM_TO_MMS(Z_PROBE_ALLEN_KEY_STOW_5_FEEDRATE));
    #endif
  }

#endif // Z_PROBE_ALLEN_KEY

#if ENABLED(PROBING_FANS_OFF)

  void fans_pause(const bool p) {
    if (p != fans_paused) {
      fans_paused = p;
      if (p)
        for (uint8_t x = 0; x < FAN_COUNT; x++) {
          paused_fanSpeeds[x] = fanSpeeds[x];
          fanSpeeds[x] = 0;
        }
      else
        for (uint8_t x = 0; x < FAN_COUNT; x++)
          fanSpeeds[x] = paused_fanSpeeds[x];
    }
  }

#endif // PROBING_FANS_OFF

#if QUIET_PROBING
  void probing_pause(const bool p) {
    #if ENABLED(PROBING_HEATERS_OFF)
      thermalManager.pause(p);
    #endif
    #if ENABLED(PROBING_FANS_OFF)
      fans_pause(p);
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

#if ENABLED(BLTOUCH)

  void bltouch_command(const int angle) {
    MOVE_SERVO(Z_PROBE_SERVO_NR, angle);  // Give the BL-Touch the command and wait
    safe_delay(BLTOUCH_DELAY);
  }

  bool set_bltouch_deployed(const bool deploy) {
    if (deploy && TEST_BLTOUCH()) {      // If BL-Touch says it's triggered
      bltouch_command(BLTOUCH_RESET);    //  try to reset it.
      bltouch_command(BLTOUCH_DEPLOY);   // Also needs to deploy and stow to
      bltouch_command(BLTOUCH_STOW);     //  clear the triggered condition.
      safe_delay(1500);                  // Wait for internal self-test to complete.
                                         //  (Measured completion time was 0.65 seconds
                                         //   after reset, deploy, and stow sequence)
      if (TEST_BLTOUCH()) {              // If it still claims to be triggered...
        SERIAL_ERROR_START();
        SERIAL_ERRORLNPGM(MSG_STOP_BLTOUCH);
        stop();                          // punt!
        return true;
      }
    }

    bltouch_command(deploy ? BLTOUCH_DEPLOY : BLTOUCH_STOW);

    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) {
        SERIAL_ECHOPAIR("set_bltouch_deployed(", deploy);
        SERIAL_CHAR(')');
        SERIAL_EOL();
      }
    #endif

    return false;
  }

#endif // BLTOUCH

/**
 * Raise Z to a minimum height to make room for a probe to move
 */
inline void do_probe_raise(const float z_raise) {
  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) {
      SERIAL_ECHOPAIR("do_probe_raise(", z_raise);
      SERIAL_CHAR(')');
      SERIAL_EOL();
    }
  #endif

  float z_dest = z_raise;
  if (zprobe_zoffset < 0) z_dest -= zprobe_zoffset;

  NOMORE(z_dest, Z_MAX_POS);

  if (z_dest > current_position[Z_AXIS])
    do_blocking_move_to_z(z_dest);
}

// returns false for ok and true for failure
bool set_probe_deployed(const bool deploy) {

  // Can be extended to servo probes, if needed.
  #if ENABLED(PROBE_IS_TRIGGERED_WHEN_STOWED_TEST)
    #if ENABLED(Z_MIN_PROBE_ENDSTOP)
      #define _TRIGGERED_WHEN_STOWED_TEST (READ(Z_MIN_PROBE_PIN) != Z_MIN_PROBE_ENDSTOP_INVERTING)
    #else
      #define _TRIGGERED_WHEN_STOWED_TEST (READ(Z_MIN_PIN) != Z_MIN_ENDSTOP_INVERTING)
    #endif
  #endif

  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) {
      DEBUG_POS("set_probe_deployed", current_position);
      SERIAL_ECHOLNPAIR("deploy: ", deploy);
    }
  #endif

  if (endstops.z_probe_enabled == deploy) return false;

  // Make room for probe to deploy (or stow)
  // Fix-mounted probe should only raise for deploy
  #if ENABLED(FIX_MOUNTED_PROBE)
    const bool deploy_stow_condition = deploy;
  #else
    constexpr bool deploy_stow_condition = true;
  #endif

  // For beds that fall when Z is powered off only raise for trusted Z
  #if ENABLED(UNKNOWN_Z_NO_RAISE)
    const bool unknown_condition = axis_known_position[Z_AXIS];
  #else
    constexpr float unknown_condition = true;
  #endif

  if (deploy_stow_condition && unknown_condition)
    do_probe_raise(max(Z_CLEARANCE_BETWEEN_PROBES, Z_CLEARANCE_DEPLOY_PROBE));

  #if ENABLED(Z_PROBE_SLED) || ENABLED(Z_PROBE_ALLEN_KEY)
    #if ENABLED(Z_PROBE_SLED)
      #define _AUE_ARGS true, false, false
    #else
      #define _AUE_ARGS
    #endif
    if (axis_unhomed_error(_AUE_ARGS)) {
      SERIAL_ERROR_START();
      SERIAL_ERRORLNPGM(MSG_STOP_UNHOMED);
      stop();
      return true;
    }
  #endif

  const float oldXpos = current_position[X_AXIS],
              oldYpos = current_position[Y_AXIS];

  #ifdef _TRIGGERED_WHEN_STOWED_TEST

    // If endstop is already false, the Z probe is deployed
    if (_TRIGGERED_WHEN_STOWED_TEST == deploy) {     // closed after the probe specific actions.
                                                     // Would a goto be less ugly?
      //while (!_TRIGGERED_WHEN_STOWED_TEST) idle(); // would offer the opportunity
                                                     // for a triggered when stowed manual probe.

      if (!deploy) endstops.enable_z_probe(false); // Switch off triggered when stowed probes early
                                                   // otherwise an Allen-Key probe can't be stowed.
  #endif

      #if ENABLED(SOLENOID_PROBE)

        #if HAS_SOLENOID_1
          WRITE(SOL1_PIN, deploy);
        #endif

      #elif ENABLED(Z_PROBE_SLED)

        dock_sled(!deploy);

      #elif HAS_Z_SERVO_PROBE && DISABLED(BLTOUCH)

        MOVE_SERVO(Z_PROBE_SERVO_NR, z_servo_angle[deploy ? 0 : 1]);

      #elif ENABLED(Z_PROBE_ALLEN_KEY)

        deploy ? run_deploy_moves_script() : run_stow_moves_script();

      #endif

  #ifdef _TRIGGERED_WHEN_STOWED_TEST
    } // _TRIGGERED_WHEN_STOWED_TEST == deploy

    if (_TRIGGERED_WHEN_STOWED_TEST == deploy) { // State hasn't changed?

      if (IsRunning()) {
        SERIAL_ERROR_START();
        SERIAL_ERRORLNPGM("Z-Probe failed");
        LCD_ALERTMESSAGEPGM("Err: ZPROBE");
      }
      stop();
      return true;

    } // _TRIGGERED_WHEN_STOWED_TEST == deploy

  #endif

  do_blocking_move_to(oldXpos, oldYpos, current_position[Z_AXIS]); // return to position before deploy
  endstops.enable_z_probe(deploy);
  return false;
}

#if Z_AFTER_PROBING
  // After probing move to a preferred Z position
  void move_z_after_probing() {
    if (current_position[Z_AXIS] != Z_AFTER_PROBING) {
      do_blocking_move_to_z(Z_AFTER_PROBING);
      current_position[Z_AXIS] = Z_AFTER_PROBING;
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
static bool do_probe_move(const float z, const float fr_mm_m) {
  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) DEBUG_POS(">>> do_probe_move", current_position);
  #endif

  // Deploy BLTouch at the start of any probe
  #if ENABLED(BLTOUCH)
    if (set_bltouch_deployed(true)) return true;
  #endif

  #if QUIET_PROBING
    probing_pause(true);
  #endif

  // Move down until probe triggered
  do_blocking_move_to_z(z, MMM_TO_MMS(fr_mm_m));

  // Check to see if the probe was triggered
  const bool probe_triggered = TEST(Endstops::endstop_hit_bits,
    #if ENABLED(Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN)
      Z_MIN
    #else
      Z_MIN_PROBE
    #endif
  );

  #if QUIET_PROBING
    probing_pause(false);
  #endif

  // Retract BLTouch immediately after a probe if it was triggered
  #if ENABLED(BLTOUCH)
    if (probe_triggered && set_bltouch_deployed(false)) return true;
  #endif

  // Clear endstop flags
  endstops.hit_on_purpose();

  // Get Z where the steppers were interrupted
  set_current_from_steppers_for_axis(Z_AXIS);

  // Tell the planner where we actually are
  SYNC_PLAN_POSITION_KINEMATIC();

  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) DEBUG_POS("<<< do_probe_move", current_position);
  #endif

  return !probe_triggered;
}

/**
 * @details Used by probe_pt to do a single Z probe at the current position.
 *          Leaves current_position[Z_AXIS] at the height where the probe triggered.
 *
 * @return The raw Z position where the probe was triggered
 */
static float run_z_probe() {

  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) DEBUG_POS(">>> run_z_probe", current_position);
  #endif

  // Double-probing does a fast probe followed by a slow probe
  #if MULTIPLE_PROBING == 2

    // Do a first probe at the fast speed
    if (do_probe_move(-10, Z_PROBE_SPEED_FAST)) return NAN;

    float first_probe_z = current_position[Z_AXIS];

    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPAIR("1st Probe Z:", first_probe_z);
    #endif

    // move up to make clearance for the probe
    do_blocking_move_to_z(current_position[Z_AXIS] + Z_CLEARANCE_BETWEEN_PROBES, MMM_TO_MMS(Z_PROBE_SPEED_FAST));

  #else

    // If the nozzle is well over the travel height then
    // move down quickly before doing the slow probe
    float z = Z_CLEARANCE_DEPLOY_PROBE + 5.0;
    if (zprobe_zoffset < 0) z -= zprobe_zoffset;

    if (current_position[Z_AXIS] > z) {
      // If we don't make it to the z position (i.e. the probe triggered), move up to make clearance for the probe
      if (!do_probe_move(z, Z_PROBE_SPEED_FAST))
        do_blocking_move_to_z(current_position[Z_AXIS] + Z_CLEARANCE_BETWEEN_PROBES, MMM_TO_MMS(Z_PROBE_SPEED_FAST));
    }
  #endif

  #if MULTIPLE_PROBING > 2
    float probes_total = 0;
    for (uint8_t p = MULTIPLE_PROBING + 1; --p;) {
  #endif

      // Move down slowly to find bed, not too far
      if (do_probe_move(-10, Z_PROBE_SPEED_SLOW)) return NAN;

  #if MULTIPLE_PROBING > 2
      probes_total += current_position[Z_AXIS];
      if (p > 1) do_blocking_move_to_z(current_position[Z_AXIS] + Z_CLEARANCE_BETWEEN_PROBES, MMM_TO_MMS(Z_PROBE_SPEED_FAST));
    }
  #endif

  #if MULTIPLE_PROBING > 2

    // Return the average value of all probes
    return probes_total * (1.0 / (MULTIPLE_PROBING));

  #elif MULTIPLE_PROBING == 2

    const float z2 = current_position[Z_AXIS];

    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) {
        SERIAL_ECHOPAIR("2nd Probe Z:", z2);
        SERIAL_ECHOLNPAIR(" Discrepancy:", first_probe_z - z2);
      }
    #endif

    // Return a weighted average of the fast and slow probes
    return (z2 * 3.0 + first_probe_z * 2.0) * 0.2;

  #else

    // Return the single probe result
    return current_position[Z_AXIS];

  #endif

  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) DEBUG_POS("<<< run_z_probe", current_position);
  #endif
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
float probe_pt(const float &rx, const float &ry, const ProbePtRaise raise_after/*=PROBE_PT_NONE*/, const uint8_t verbose_level/*=0*/, const bool probe_relative/*=true*/) {
  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) {
      SERIAL_ECHOPAIR(">>> probe_pt(", LOGICAL_X_POSITION(rx));
      SERIAL_ECHOPAIR(", ", LOGICAL_Y_POSITION(ry));
      SERIAL_ECHOPAIR(", ", raise_after == PROBE_PT_RAISE ? "raise" : raise_after == PROBE_PT_STOW ? "stow" : "none");
      SERIAL_ECHOPAIR(", ", int(verbose_level));
      SERIAL_ECHOPAIR(", ", probe_relative ? "probe" : "nozzle");
      SERIAL_ECHOLNPGM("_relative)");
      DEBUG_POS("", current_position);
    }
  #endif

  // TODO: Adapt for SCARA, where the offset rotates
  float nx = rx, ny = ry;
  if (probe_relative) {
    if (!position_is_reachable_by_probe(rx, ry)) return NAN;  // The given position is in terms of the probe
    nx -= (X_PROBE_OFFSET_FROM_EXTRUDER);                     // Get the nozzle position
    ny -= (Y_PROBE_OFFSET_FROM_EXTRUDER);
  }
  else if (!position_is_reachable(nx, ny)) return NAN;        // The given position is in terms of the nozzle

  const float nz =
    #if ENABLED(DELTA)
      // Move below clip height or xy move will be aborted by do_blocking_move_to
      min(current_position[Z_AXIS], delta_clip_start_height)
    #else
      current_position[Z_AXIS]
    #endif
  ;

  const float old_feedrate_mm_s = feedrate_mm_s;
  feedrate_mm_s = XY_PROBE_FEEDRATE_MM_S;

  // Move the probe to the starting XYZ
  do_blocking_move_to(nx, ny, nz);

  float measured_z = NAN;
  if (!DEPLOY_PROBE()) {
    measured_z = run_z_probe() + zprobe_zoffset;

    if (raise_after == PROBE_PT_RAISE)
      do_blocking_move_to_z(current_position[Z_AXIS] + Z_CLEARANCE_BETWEEN_PROBES, MMM_TO_MMS(Z_PROBE_SPEED_FAST));
    else if (raise_after == PROBE_PT_STOW)
      if (STOW_PROBE()) measured_z = NAN;
  }

  if (verbose_level > 2) {
    SERIAL_PROTOCOLPGM("Bed X: ");
    SERIAL_PROTOCOL_F(LOGICAL_X_POSITION(rx), 3);
    SERIAL_PROTOCOLPGM(" Y: ");
    SERIAL_PROTOCOL_F(LOGICAL_Y_POSITION(ry), 3);
    SERIAL_PROTOCOLPGM(" Z: ");
    SERIAL_PROTOCOL_F(measured_z, 3);
    SERIAL_EOL();
  }

  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPGM("<<< probe_pt");
  #endif

  feedrate_mm_s = old_feedrate_mm_s;

  if (isnan(measured_z)) {
    LCD_MESSAGEPGM(MSG_ERR_PROBING_FAILED);
    SERIAL_ERROR_START();
    SERIAL_ERRORLNPGM(MSG_ERR_PROBING_FAILED);
  }

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
