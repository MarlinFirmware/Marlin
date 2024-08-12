/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2024 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../../inc/MarlinConfig.h"

#if ENABLED(FT_MOTION)

#include "../../gcode.h"
#include "../../../module/ft_motion.h"
#include "../../../module/stepper.h"

void say_ftm_cfg() {
  #if ENABLED(FTM_UNIFIED_BWS)
    SERIAL_ECHOPGM( "M494 FTMCFG FTM_BATCH_SIZE:", (int)(FTM_BW_SIZE),
                    " FTM_WINDOW_SIZE:", (int)(FTM_BW_SIZE));
  #else
    SERIAL_ECHOPGM( "M494 FTMCFG FTM_BATCH_SIZE:", (int)(FTM_BATCH_SIZE),
                    " FTM_WINDOW_SIZE:", (int)(FTM_WINDOW_SIZE));
  #endif
  SERIAL_ECHOPGM( " FTM_FS:", (int)(FTM_FS),
                  " FTM_STEPPER_FS:", (int)(FTM_STEPPER_FS),
                  " FTM_STEPPERCMD_BUFF_SIZE:", (int)(FTM_STEPPERCMD_BUFF_SIZE),
                  " STEPPER_TIMER_RATE:", (int)(STEPPER_TIMER_RATE),
                  " FTM_ZMAX:", (int)(FTM_ZMAX),
                  " X_MAX_LENGTH:", (int)(X_MAX_LENGTH));
  #if HAS_Y_AXIS
    SERIAL_ECHOPGM( " Y_MAX_LENGTH:", (int)(Y_MAX_LENGTH));
  #endif
  SERIAL_EOL();
}

/**
 * M494: Interact with Fixed-time Motion Control's trajectory generation.
 *
 *    A<mode> Start / abort a frequency sweep profile.
 *
 *       0: None active.
 *       1: Continuous sweep on X axis.
 *       2: Continuous sweep on Y axis.
 *       99: Abort the current sweep.
 * 
 *    B<float> Start frequency.
 *    C<float> End frequency.
 *    D<float> Frequency rate.
 *    E<float> Acceleration amplitude.
 *    F<float> Step time.
 *    H<float> Step acceleration amplitude.
 *    I<float> Delay time to opening step.
 *    J<float> Delay time from opening step to sweep.
 *    K<float> Delay time from sweep to closing step.
 * 
 * With no parameters passed, M494 will report the FTM configuration and
 * variables required for autocalibration.
 * 
 */
void GcodeSuite::M494() {
  
  if (!parser.seen_any()) {
    say_ftm_cfg();
    return;
  }

  if (!ftMotion.cfg.active) {
    SERIAL_ECHOLN("M494 echo: rejected! FTM is not enabled.");
    return;
  }

  ftMotionTrajGenMode_t mode_val_seen = trajGenMode_NONE;
  bool good_cfg_received = true;

  // Parse mode parameter.
  if (parser.seenval('A')) {
    const ftMotionTrajGenMode_t newmm = (ftMotionTrajGenMode_t)parser.value_byte();
    switch (newmm) {
      default: SERIAL_ECHOLNPGM("?Invalid calibration mode [F] value."); return;
      case trajGenMode_NONE:
      case trajGenMode_SWEEPC_X:
      #if HAS_Y_AXIS
      case trajGenMode_SWEEPC_Y:
      #endif
        if ( ftMotion.traj_gen_cfg.mode ) {
          SERIAL_ECHOLN("M494 echo: rejected! wait for the previous command to complete.");
          return;
        }
        mode_val_seen = newmm;
        break;
      case trajGenMode_ABORT:
        if ( ftMotion.traj_gen_cfg.mode ) {
          ftMotion.traj_gen_cfg.mode = trajGenMode_NONE;
          stepper.quick_stop();
          return;
        }
    }
  }

  // Parse start frequency parameter.
  if (parser.seenval('B')) {
      const float val = parser.value_float();
      if (val >= 1.0f) ftMotion.traj_gen_cfg.f0 = val;
      else { good_cfg_received = false; SERIAL_ECHOLN("M494 echo: Start frequency out of range."); }
  } else good_cfg_received = false;

  // Parse end frequency parameter.
  if (parser.seenval('C')) {
      const float val = parser.value_float();
      if (val > 0.0f) ftMotion.traj_gen_cfg.f1 = val;
      else { good_cfg_received = false; SERIAL_ECHOLN("M494 echo: End frequency out of range."); }
  } else good_cfg_received = false;

  // Parse step frequency parameter.
  if (parser.seenval('D')) {
      const float val = parser.value_float();
      if (val > 0.0f) ftMotion.traj_gen_cfg.dfdt = val;
      else { good_cfg_received = false; SERIAL_ECHOLN("M494 echo: Step / delta frequency out of range."); }
  } else good_cfg_received = false;

  // Parse amplitude parameter.
  if (parser.seenval('E')) {
      const float val = parser.value_float();
      if (val > 0.0f) ftMotion.traj_gen_cfg.a = val;
      else { good_cfg_received = false; SERIAL_ECHOLN("M494 echo: Amplitude out of range."); }
  } else good_cfg_received = false;

  // Parse step time parameter.
  if (parser.seenval('F')) {
      const float val = parser.value_float();
      if (val > 0.0f) ftMotion.traj_gen_cfg.step_ti = val;
      else { good_cfg_received = false; SERIAL_ECHOLN("M494 echo: Step time out of range."); }
  } else good_cfg_received = false;

  // Parse step amplitude parameter.
  if (parser.seenval('H')) {
      const float val = parser.value_float();
      if (val > 0.0f) ftMotion.traj_gen_cfg.step_a = val;
      else { good_cfg_received = false; SERIAL_ECHOLN("M494 echo: Step amplitude out of range."); }
  } else good_cfg_received = false;

  // Parse delay parameter.
  if (parser.seenval('I')) {
      const float val = parser.value_float();
      if (val > 0.0f) ftMotion.traj_gen_cfg.dly1_ti = val;
      else { good_cfg_received = false; SERIAL_ECHOLN("M494 echo: Delay 1 out of range."); }
  } else good_cfg_received = false;

  // Parse delay parameter.
  if (parser.seenval('J')) {
      const float val = parser.value_float();
      if (val > 0.0f) ftMotion.traj_gen_cfg.dly2_ti = val;
      else { good_cfg_received = false; SERIAL_ECHOLN("M494 echo: Delay 2 out of range."); }
  } else good_cfg_received = false;

  // Parse delay parameter.
  if (parser.seenval('K')) {
      const float val = parser.value_float();
      if (val > 0.0f) ftMotion.traj_gen_cfg.dly3_ti = val;
      else { good_cfg_received = false; SERIAL_ECHOLN("M494 echo: Delay 3 out of range."); }
  } else good_cfg_received = false;

  // Validate and set.
  if ( mode_val_seen && good_cfg_received ) {

    planner.synchronize();

    const float f0_ = ftMotion.traj_gen_cfg.f0 - 1.f; // This is calculated assuming a ramp time of 1 sec.

    ftMotion.traj_gen_cfg.k1 = PI*ftMotion.traj_gen_cfg.dfdt;
    ftMotion.traj_gen_cfg.k2 = 2.f*PI*f0_;

    ftMotion.traj_gen_cfg.pcws_ti[0] = ftMotion.traj_gen_cfg.dly1_ti;
    ftMotion.traj_gen_cfg.pcws_ti[1] = ftMotion.traj_gen_cfg.pcws_ti[0] + 4 * ftMotion.traj_gen_cfg.step_ti;
    ftMotion.traj_gen_cfg.pcws_ti[2] = ftMotion.traj_gen_cfg.pcws_ti[1] + ftMotion.traj_gen_cfg.dly2_ti;
    ftMotion.traj_gen_cfg.pcws_ti[3] = ftMotion.traj_gen_cfg.pcws_ti[2] + ( ftMotion.traj_gen_cfg.f1 - f0_ ) / ftMotion.traj_gen_cfg.dfdt;
    ftMotion.traj_gen_cfg.pcws_ti[4] = ftMotion.traj_gen_cfg.pcws_ti[3] + ftMotion.traj_gen_cfg.dly3_ti;
    ftMotion.traj_gen_cfg.pcws_ti[5] = ftMotion.traj_gen_cfg.pcws_ti[4] + 4 * ftMotion.traj_gen_cfg.step_ti;

    ftMotion.traj_gen_cfg.step_a_x_0p5 = 0.5f * ftMotion.traj_gen_cfg.step_a;
    ftMotion.traj_gen_cfg.step_a_x_step_ti_x_step_ti = ftMotion.traj_gen_cfg.step_a * sq(ftMotion.traj_gen_cfg.step_ti);
    ftMotion.traj_gen_cfg.step_ti_x_2 = 2.f * ftMotion.traj_gen_cfg.step_ti;
    ftMotion.traj_gen_cfg.step_ti_x_3 = 3.f * ftMotion.traj_gen_cfg.step_ti;
    ftMotion.traj_gen_cfg.step_ti_x_4 = 4.f * ftMotion.traj_gen_cfg.step_ti;

    ftMotion.traj_gen_cfg.mode = mode_val_seen;

    ftMotion.setup_traj_gen(round(ftMotion.traj_gen_cfg.pcws_ti[5]*FTM_FS + 0.5f));

    SERIAL_ECHOLN("M494 echo: SWEEPC starting.");

    stepper.enable_all_steppers();

    // stepper.disable_all_steppers(); Leave this to the plugin or user; this can
    // be annoying on printers that require all axes homing before axis movement.
  }

}

#endif // FT_MOTION
