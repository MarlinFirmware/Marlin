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

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(MECHANICAL_GANTRY_CALIBRATION)

#include "../gcode.h"
#include "../../module/motion.h"
#include "../../module/stepper.h"
#include "../../module/endstops.h"

#if HAS_LEVELING
  #include "../../feature/bedlevel/bedlevel.h"
#endif

#define DEBUG_OUT ENABLED(DEBUG_LEVELING_FEATURE)
#include "../../core/debug_out.h"

void GcodeSuite::G34() {

  // Home before the alignment procedure
  home_if_needed();

  TERN_(HAS_LEVELING, TEMPORARY_BED_LEVELING_STATE(false));

  SET_SOFT_ENDSTOP_LOOSE(true);
  TemporaryGlobalEndstopsState unlock_z(false);

  #ifdef GANTRY_CALIBRATION_COMMANDS_PRE
    process_subcommands_now(F(GANTRY_CALIBRATION_COMMANDS_PRE));
    if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPGM("Sub Commands Processed");
  #endif

  #ifdef GANTRY_CALIBRATION_SAFE_POSITION
    // Move XY to safe position
    if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPGM("Parking XY");
    const xy_pos_t safe_pos = GANTRY_CALIBRATION_SAFE_POSITION;
    do_blocking_move_to(safe_pos, MMM_TO_MMS(GANTRY_CALIBRATION_XY_PARK_FEEDRATE));
  #endif

  const float move_distance = parser.intval('Z', GANTRY_CALIBRATION_EXTRA_HEIGHT),
              zbase = ENABLED(GANTRY_CALIBRATION_TO_MIN) ? Z_MIN_POS : Z_MAX_POS,
              zpounce = zbase - move_distance, zgrind = zbase + move_distance;

  // Move Z to pounce position
  if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPGM("Setting Z Pounce");
  do_blocking_move_to_z(zpounce, homing_feedrate(Z_AXIS));

  // Store current motor settings, then apply reduced value

  #define _REDUCE_CURRENT ANY(HAS_MOTOR_CURRENT_SPI, HAS_MOTOR_CURRENT_PWM, HAS_MOTOR_CURRENT_DAC, HAS_MOTOR_CURRENT_I2C, HAS_TRINAMIC_CONFIG)
  #if _REDUCE_CURRENT
    if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPGM("Reducing Current");
  #endif

  #if HAS_MOTOR_CURRENT_SPI
    const uint16_t target_current = parser.intval('S', GANTRY_CALIBRATION_CURRENT);
    const uint32_t previous_current = stepper.motor_current_setting[Z_AXIS];
    stepper.set_digipot_current(Z_AXIS, target_current);
  #elif HAS_MOTOR_CURRENT_PWM
    const uint16_t target_current = parser.intval('S', GANTRY_CALIBRATION_CURRENT);
    const uint32_t previous_current = stepper.motor_current_setting[1]; // Z
    stepper.set_digipot_current(1, target_current);
  #elif HAS_MOTOR_CURRENT_DAC
    const float target_current = parser.floatval('S', GANTRY_CALIBRATION_CURRENT);
    const float previous_current = dac_amps(Z_AXIS, target_current);
    stepper_dac.set_current_value(Z_AXIS, target_current);
  #elif HAS_MOTOR_CURRENT_I2C
    const uint16_t target_current = parser.intval('S', GANTRY_CALIBRATION_CURRENT);
    previous_current = dac_amps(Z_AXIS);
    digipot_i2c.set_current(Z_AXIS, target_current)
  #elif HAS_TRINAMIC_CONFIG
    const uint16_t target_current = parser.intval('S', GANTRY_CALIBRATION_CURRENT);
    static uint16_t previous_current_arr[NUM_Z_STEPPERS];
    #if AXIS_IS_TMC(Z)
      previous_current_arr[0] = stepperZ.getMilliamps();
      stepperZ.rms_current(target_current);
    #endif
    #if AXIS_IS_TMC(Z2)
      previous_current_arr[1] = stepperZ2.getMilliamps();
      stepperZ2.rms_current(target_current);
    #endif
    #if AXIS_IS_TMC(Z3)
      previous_current_arr[2] = stepperZ3.getMilliamps();
      stepperZ3.rms_current(target_current);
    #endif
    #if AXIS_IS_TMC(Z4)
      previous_current_arr[3] = stepperZ4.getMilliamps();
      stepperZ4.rms_current(target_current);
    #endif
  #endif

  // Do Final Z move to adjust
  if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPGM("Final Z Move");
  do_blocking_move_to_z(zgrind, MMM_TO_MMS(GANTRY_CALIBRATION_FEEDRATE));

  #if _REDUCE_CURRENT
    // Reset current to original values
    if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPGM("Restore Current");
  #endif

  #if HAS_MOTOR_CURRENT_SPI
    stepper.set_digipot_current(Z_AXIS, previous_current);
  #elif HAS_MOTOR_CURRENT_PWM
    stepper.set_digipot_current(1, previous_current);
  #elif HAS_MOTOR_CURRENT_DAC
    stepper_dac.set_current_value(Z_AXIS, previous_current);
  #elif HAS_MOTOR_CURRENT_I2C
    digipot_i2c.set_current(Z_AXIS, previous_current)
  #elif HAS_TRINAMIC_CONFIG
    #if AXIS_IS_TMC(Z)
      stepperZ.rms_current(previous_current_arr[0]);
    #endif
    #if AXIS_IS_TMC(Z2)
      stepperZ2.rms_current(previous_current_arr[1]);
    #endif
    #if AXIS_IS_TMC(Z3)
      stepperZ3.rms_current(previous_current_arr[2]);
    #endif
    #if AXIS_IS_TMC(Z4)
      stepperZ4.rms_current(previous_current_arr[3]);
    #endif
  #endif

  // Back off end plate, back to normal motion range
  if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPGM("Z Backoff");
  do_blocking_move_to_z(zpounce, MMM_TO_MMS(GANTRY_CALIBRATION_FEEDRATE));

  #ifdef GANTRY_CALIBRATION_COMMANDS_POST
    if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPGM("Running Post Commands");
    process_subcommands_now(F(GANTRY_CALIBRATION_COMMANDS_POST));
  #endif

  SET_SOFT_ENDSTOP_LOOSE(false);
}

#endif // MECHANICAL_GANTRY_CALIBRATION
