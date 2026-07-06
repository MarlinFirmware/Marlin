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

#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(STALLGUARD_TUNING)

#include "../../../feature/stallguard/stallguard_tuning.h"
#include "../../gcode.h"

void say_stallguard_tuning() {
  SERIAL_ECHO_START();
  SERIAL_ECHOLNPGM("M921 StallGuard Tuning");
  #if CORE_IS_XY || (X_SENSORLESS && Y_SENSORLESS)
    SERIAL_ECHOLNPGM("Tune: M921 [X|Y]");
  #elif (X_SENSORLESS && !Y_SENSORLESS)
    SERIAL_ECHOLNPGM("Tune: M921 X");
  #else
    SERIAL_ECHOLNPGM("Tune: M921 Y");
  #endif
  #if HAS_STANDARD_MOTION
    SERIAL_ECHOLNPGM("Save STD values : M921 S1");
  #endif
  #if ENABLED(FT_MOTION)
    SERIAL_ECHOLNPGM("Save FTM values : M921 S0");
  #endif
}

#if ENABLED(EDITABLE_HOMING_CURRENT)
  void sg_set_homing_current(AxisEnum axis) {
    #if X_SENSORLESS
      if (axis == X_AXIS) {
        uint16_t homing_current = max(400, X_CURRENT / 2);
        homing_current_mA.X = homing_current;
        TERN_(X2_SENSORLESS, homing_current_mA.X2 = homing_current;)
      }
    #endif

    #if Y_SENSORLESS
      if (axis == Y_AXIS) {
        uint16_t homing_current = max(400, Y_CURRENT / 2);
        homing_current_mA.X = homing_current;
        TERN_(Y2_SENSORLESS, homing_current_mA.Y2 = homing_current;)
      }
    #endif
    }
  #endif // EDITABLE_HOMING_CURRENT
  
  #if ENABLED(FT_MOTION)
    void sg_ftm_tuning_save() {
      AxisEnum axis = stallguard_tuner.tunedAxis();

      #if X_SENSORLESS
        if (axis == X_AXIS) {
          #if ENABLED(EDITABLE_HOMING_CURRENT)
            sg_set_homing_current(X_AXIS);
          #endif
          stepperX.homing_threshold(stallguard_tuner.get_ftm_threshold());
        }
      #endif
    
      #if Y_SENSORLESS
        if (axis == Y_AXIS) {
          #if ENABLED(EDITABLE_HOMING_CURRENT)
            sg_set_homing_current(Y_AXIS);
          #endif
          stepperY.homing_threshold(stallguard_tuner.get_ftm_threshold());
        }
      #endif
        queue.inject(F("M500"));
    }
  #endif // FT_MOTION

  #if HAS_STANDARD_MOTION
    void sg_std_tuning_save() {
      AxisEnum axis = stallguard_tuner.tunedAxis();

      #if X_SENSORLESS
        if (axis == X_AXIS) {
          #if ENABLED(EDITABLE_HOMING_CURRENT)
            sg_set_homing_current(X_AXIS);
          #endif
          stepperX.homing_threshold(stallguard_tuner.get_std_threshold());
        }
      #endif
    
      #if Y_SENSORLESS
        if (axis == Y_AXIS) {
          #if ENABLED(EDITABLE_HOMING_CURRENT)
            sg_set_homing_current(Y_AXIS);
          #endif
          stepperY.homing_threshold(stallguard_tuner.get_std_threshold());
        }
      #endif
        queue.inject(F("M500"));
    }
  #endif // HAS_STANDARD_MOTION

/**
 * M921: Launch StallGuard2 or StallGuard4 autotuning for X or Y axis depending on the TMC driver type.
 * For cartesian and coreXY machines
 * For axis with 2 TMC stepper drivers, both must have the same Stallguard version unless one of the drivers is a TMC2240
 * If both FT_MOTION and standard motion are enabled, tuning is done for the 2 motion systems.
 *
 * Parameters:
 *   X       - Autotune X axis stepper(s)
 *   Y       - Autotune Y axis stepper(s)
 *   S<bool> - Save tuned SGT values, 0 FTM values, 1 STD values and homing current (EDITABLE_HOMING_CURRENT must be enabled) to EEPROM.
 */

void GcodeSuite::M921() {

  bool seen_x = parser.seen('X'), seen_y = parser.seen('Y'), seen_s = parser.seen('S');
  bool has_run = false;
  if (seen_x || seen_y) {
    const AxisEnum axis = seen_x ? X_AXIS : Y_AXIS;
    if((axis == X_AXIS && X_SENSORLESS) || (axis == Y_AXIS && Y_SENSORLESS)) {
      stallguard_tuner.tune_axis(axis);
      has_run = true;
    }
    else {
      SERIAL_ECHOLN((axis?"Y":"X")," Axis is not sensorless");
      say_stallguard_tuning();
    }
  }
  else if (seen_s) {
    if(has_run) {
      // Get motion system
      bool m_system = parser.value_bool();
      if (m_system) {
        #if HAS_STANDARD_MOTION
          sg_std_tuning_save();
        #else
          SERIAL_ECHOLN("?Save with M921 S0");
        #endif
      }
      else {
        #if ENABLED(FT_MOTION)
          sg_ftm_tuning_save();
        #else
          SERIAL_ECHOLN("?Save with M921 S1");
        #endif
      }
    }
    else
      SERIAL_ECHOLN("?Tuning must be done before saving");

  }
  else
    say_stallguard_tuning();
}

#endif