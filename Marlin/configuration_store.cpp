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
 * configuration_store.cpp
 *
 * Configuration and EEPROM storage
 *
 * IMPORTANT:  Whenever there are changes made to the variables stored in EEPROM
 * in the functions below, also increment the version number. This makes sure that
 * the default values are used whenever there is a change to the data, to prevent
 * wrong data being written to the variables.
 *
 * ALSO: Variables in the Store and Retrieve sections must be in the same order.
 *       If a feature is disabled, some data must still be written that, when read,
 *       either sets a Sane Default, or results in No Change to the existing value.
 *
 */

#define EEPROM_VERSION "V23"

/**
 * V23 EEPROM Layout:
 *
 *  100  Version (char x4)
 *
 *  104  M92 XYZE  planner.axis_steps_per_mm (float x4)
 *  120  M203 XYZE planner.max_feedrate (float x4)
 *  136  M201 XYZE planner.max_acceleration_mm_per_s2 (uint32_t x4)
 *  152  M204 P    planner.acceleration (float)
 *  156  M204 R    planner.retract_acceleration (float)
 *  160  M204 T    planner.travel_acceleration (float)
 *  164  M205 S    planner.min_feedrate (float)
 *  168  M205 T    planner.min_travel_feedrate (float)
 *  172  M205 B    planner.min_segment_time (ulong)
 *  176  M205 X    planner.max_xy_jerk (float)
 *  180  M205 Z    planner.max_z_jerk (float)
 *  184  M205 E    planner.max_e_jerk (float)
 *  188  M206 XYZ  home_offset (float x3)
 *
 * Mesh bed leveling:
 *  200  M420 S    status (uint8)
 *  201            z_offset (float)
 *  205            mesh_num_x (uint8 as set in firmware)
 *  206            mesh_num_y (uint8 as set in firmware)
 *  207 G29 S3 XYZ z_values[][] (float x9, by default)
 *
 * AUTO BED LEVELING
 *  243  M851      zprobe_zoffset (float)
 *
 * DELTA:
 *  247  M666 XYZ  endstop_adj (float x3)
 *  259  M665 R    delta_radius (float)
 *  263  M665 L    delta_diagonal_rod (float)
 *  267  M665 S    delta_segments_per_second (float)
 *  271  M665 A    delta_diagonal_rod_trim_tower_1 (float)
 *  275  M665 B    delta_diagonal_rod_trim_tower_2 (float)
 *  279  M665 C    delta_diagonal_rod_trim_tower_3 (float)
 *
 * Z_DUAL_ENDSTOPS:
 *  283  M666 Z    z_endstop_adj (float)
 *
 * ULTIPANEL:
 *  287  M145 S0 H plaPreheatHotendTemp (int)
 *  289  M145 S0 B plaPreheatHPBTemp (int)
 *  291  M145 S0 F plaPreheatFanSpeed (int)
 *  293  M145 S1 H absPreheatHotendTemp (int)
 *  295  M145 S1 B absPreheatHPBTemp (int)
 *  297  M145 S1 F absPreheatFanSpeed (int)
 *
 * PIDTEMP:
 *  299  M301 E0 PIDC  Kp[0], Ki[0], Kd[0], Kc[0] (float x4)
 *  315  M301 E1 PIDC  Kp[1], Ki[1], Kd[1], Kc[1] (float x4)
 *  331  M301 E2 PIDC  Kp[2], Ki[2], Kd[2], Kc[2] (float x4)
 *  347  M301 E3 PIDC  Kp[3], Ki[3], Kd[3], Kc[3] (float x4)
 *  363  M301 L        lpq_len (int)
 *
 * PIDTEMPBED:
 *  365  M304 PID  thermalManager.bedKp, thermalManager.bedKi, thermalManager.bedKd (float x3)
 *
 * DOGLCD:
 *  377  M250 C    lcd_contrast (int)
 *
 * SCARA:
 *  379  M365 XYZ  axis_scaling (float x3)
 *
 * FWRETRACT:
 *  391  M209 S    autoretract_enabled (bool)
 *  392  M207 S    retract_length (float)
 *  396  M207 W    retract_length_swap (float)
 *  400  M207 F    retract_feedrate (float)
 *  404  M207 Z    retract_zlift (float)
 *  408  M208 S    retract_recover_length (float)
 *  412  M208 W    retract_recover_length_swap (float)
 *  416  M208 F    retract_recover_feedrate (float)
 *
 * Volumetric Extrusion:
 *  420  M200 D    volumetric_enabled (bool)
 *  421  M200 T D  filament_size (float x4) (T0..3)
 *
 *  437  This Slot is Available!
 *
 */
#include "Marlin.h"
#include "language.h"
#include "planner.h"
#include "temperature.h"
#include "ultralcd.h"
#include "configuration_store.h"

#if ENABLED(MESH_BED_LEVELING)
  #include "mesh_bed_leveling.h"
#endif

void _EEPROM_writeData(int &pos, uint8_t* value, uint8_t size) {
  uint8_t c;
  while (size--) {
    eeprom_write_byte((unsigned char*)pos, *value);
    c = eeprom_read_byte((unsigned char*)pos);
    if (c != *value) {
      SERIAL_ECHO_START;
      SERIAL_ECHOLNPGM(MSG_ERR_EEPROM_WRITE);
    }
    pos++;
    value++;
  };
}
void _EEPROM_readData(int &pos, uint8_t* value, uint8_t size) {
  do {
    *value = eeprom_read_byte((unsigned char*)pos);
    pos++;
    value++;
  } while (--size);
}
#define EEPROM_WRITE_VAR(pos, value) _EEPROM_writeData(pos, (uint8_t*)&value, sizeof(value))
#define EEPROM_READ_VAR(pos, value) _EEPROM_readData(pos, (uint8_t*)&value, sizeof(value))

/**
 * Store Configuration Settings - M500
 */

#define DUMMY_PID_VALUE 3000.0f

#define EEPROM_OFFSET 100

#if ENABLED(EEPROM_SETTINGS)

/**
 * Store Configuration Settings - M500
 */

void Config_StoreSettings()  {
  float dummy = 0.0f;
  char ver[4] = "000";
  int i = EEPROM_OFFSET;
  EEPROM_WRITE_VAR(i, ver); // invalidate data first
  EEPROM_WRITE_VAR(i, planner.axis_steps_per_mm);
  EEPROM_WRITE_VAR(i, planner.max_feedrate);
  EEPROM_WRITE_VAR(i, planner.max_acceleration_mm_per_s2);
  EEPROM_WRITE_VAR(i, planner.acceleration);
  EEPROM_WRITE_VAR(i, planner.retract_acceleration);
  EEPROM_WRITE_VAR(i, planner.travel_acceleration);
  EEPROM_WRITE_VAR(i, planner.min_feedrate);
  EEPROM_WRITE_VAR(i, planner.min_travel_feedrate);
  EEPROM_WRITE_VAR(i, planner.min_segment_time);
  EEPROM_WRITE_VAR(i, planner.max_xy_jerk);
  EEPROM_WRITE_VAR(i, planner.max_z_jerk);
  EEPROM_WRITE_VAR(i, planner.max_e_jerk);
  EEPROM_WRITE_VAR(i, home_offset);

  #if ENABLED(MESH_BED_LEVELING)
    // Compile time test that sizeof(mbl.z_values) is as expected
    typedef char c_assert[(sizeof(mbl.z_values) == (MESH_NUM_X_POINTS) * (MESH_NUM_Y_POINTS) * sizeof(dummy)) ? 1 : -1];
    uint8_t mesh_num_x = MESH_NUM_X_POINTS,
            mesh_num_y = MESH_NUM_Y_POINTS,
            dummy_uint8 = mbl.status & _BV(MBL_STATUS_HAS_MESH_BIT);
    EEPROM_WRITE_VAR(i, dummy_uint8);
    EEPROM_WRITE_VAR(i, mbl.z_offset);
    EEPROM_WRITE_VAR(i, mesh_num_x);
    EEPROM_WRITE_VAR(i, mesh_num_y);
    EEPROM_WRITE_VAR(i, mbl.z_values);
  #else
    uint8_t mesh_num_x = 3,
            mesh_num_y = 3,
            dummy_uint8 = 0;
    dummy = 0.0f;
    EEPROM_WRITE_VAR(i, dummy_uint8);
    EEPROM_WRITE_VAR(i, dummy);
    EEPROM_WRITE_VAR(i, mesh_num_x);
    EEPROM_WRITE_VAR(i, mesh_num_y);
    for (uint8_t q = 0; q < mesh_num_x * mesh_num_y; q++) EEPROM_WRITE_VAR(i, dummy);
  #endif // MESH_BED_LEVELING

  #if DISABLED(AUTO_BED_LEVELING_FEATURE)
    float zprobe_zoffset = 0;
  #endif
  EEPROM_WRITE_VAR(i, zprobe_zoffset);

  #if ENABLED(DELTA)
    EEPROM_WRITE_VAR(i, endstop_adj);               // 3 floats
    EEPROM_WRITE_VAR(i, delta_radius);              // 1 float
    EEPROM_WRITE_VAR(i, delta_diagonal_rod);        // 1 float
    EEPROM_WRITE_VAR(i, delta_segments_per_second); // 1 float
    EEPROM_WRITE_VAR(i, delta_diagonal_rod_trim_tower_1);  // 1 float
    EEPROM_WRITE_VAR(i, delta_diagonal_rod_trim_tower_2);  // 1 float
    EEPROM_WRITE_VAR(i, delta_diagonal_rod_trim_tower_3);  // 1 float
  #elif ENABLED(Z_DUAL_ENDSTOPS)
    EEPROM_WRITE_VAR(i, z_endstop_adj);            // 1 float
    dummy = 0.0f;
    for (uint8_t q = 8; q--;) EEPROM_WRITE_VAR(i, dummy);
  #else
    dummy = 0.0f;
    for (uint8_t q = 9; q--;) EEPROM_WRITE_VAR(i, dummy);
  #endif

  #if DISABLED(ULTIPANEL)
    int plaPreheatHotendTemp = PLA_PREHEAT_HOTEND_TEMP, plaPreheatHPBTemp = PLA_PREHEAT_HPB_TEMP, plaPreheatFanSpeed = PLA_PREHEAT_FAN_SPEED,
        absPreheatHotendTemp = ABS_PREHEAT_HOTEND_TEMP, absPreheatHPBTemp = ABS_PREHEAT_HPB_TEMP, absPreheatFanSpeed = ABS_PREHEAT_FAN_SPEED;
  #endif // !ULTIPANEL

  EEPROM_WRITE_VAR(i, plaPreheatHotendTemp);
  EEPROM_WRITE_VAR(i, plaPreheatHPBTemp);
  EEPROM_WRITE_VAR(i, plaPreheatFanSpeed);
  EEPROM_WRITE_VAR(i, absPreheatHotendTemp);
  EEPROM_WRITE_VAR(i, absPreheatHPBTemp);
  EEPROM_WRITE_VAR(i, absPreheatFanSpeed);

  for (uint8_t e = 0; e < 4; e++) {

    #if ENABLED(PIDTEMP)
      if (e < HOTENDS) {
        EEPROM_WRITE_VAR(i, PID_PARAM(Kp, e));
        EEPROM_WRITE_VAR(i, PID_PARAM(Ki, e));
        EEPROM_WRITE_VAR(i, PID_PARAM(Kd, e));
        #if ENABLED(PID_ADD_EXTRUSION_RATE)
          EEPROM_WRITE_VAR(i, PID_PARAM(Kc, e));
        #else
          dummy = 1.0f; // 1.0 = default kc
          EEPROM_WRITE_VAR(i, dummy);
        #endif
      }
      else
    #endif // !PIDTEMP
      {
        dummy = DUMMY_PID_VALUE; // When read, will not change the existing value
        EEPROM_WRITE_VAR(i, dummy); // Kp
        dummy = 0.0f;
        for (uint8_t q = 3; q--;) EEPROM_WRITE_VAR(i, dummy); // Ki, Kd, Kc
      }

  } // Hotends Loop

  #if DISABLED(PID_ADD_EXTRUSION_RATE)
    int lpq_len = 20;
  #endif
  EEPROM_WRITE_VAR(i, lpq_len);

  #if DISABLED(PIDTEMPBED)
    dummy = DUMMY_PID_VALUE;
    for (uint8_t q = 3; q--;) EEPROM_WRITE_VAR(i, dummy);
  #else
    EEPROM_WRITE_VAR(i, thermalManager.bedKp);
    EEPROM_WRITE_VAR(i, thermalManager.bedKi);
    EEPROM_WRITE_VAR(i, thermalManager.bedKd);
  #endif

  #if !HAS_LCD_CONTRAST
    const int lcd_contrast = 32;
  #endif
  EEPROM_WRITE_VAR(i, lcd_contrast);

  #if ENABLED(SCARA)
    EEPROM_WRITE_VAR(i, axis_scaling); // 3 floats
  #else
    dummy = 1.0f;
    EEPROM_WRITE_VAR(i, dummy);
  #endif

  #if ENABLED(FWRETRACT)
    EEPROM_WRITE_VAR(i, autoretract_enabled);
    EEPROM_WRITE_VAR(i, retract_length);
    #if EXTRUDERS > 1
      EEPROM_WRITE_VAR(i, retract_length_swap);
    #else
      dummy = 0.0f;
      EEPROM_WRITE_VAR(i, dummy);
    #endif
    EEPROM_WRITE_VAR(i, retract_feedrate);
    EEPROM_WRITE_VAR(i, retract_zlift);
    EEPROM_WRITE_VAR(i, retract_recover_length);
    #if EXTRUDERS > 1
      EEPROM_WRITE_VAR(i, retract_recover_length_swap);
    #else
      dummy = 0.0f;
      EEPROM_WRITE_VAR(i, dummy);
    #endif
    EEPROM_WRITE_VAR(i, retract_recover_feedrate);
  #endif // FWRETRACT

  EEPROM_WRITE_VAR(i, volumetric_enabled);

  // Save filament sizes
  for (uint8_t q = 0; q < 4; q++) {
    if (q < EXTRUDERS) dummy = filament_size[q];
    EEPROM_WRITE_VAR(i, dummy);
  }

  char ver2[4] = EEPROM_VERSION;
  int j = EEPROM_OFFSET;
  EEPROM_WRITE_VAR(j, ver2); // validate data

  // Report storage size
  SERIAL_ECHO_START;
  SERIAL_ECHOPAIR("Settings Stored (", i);
  SERIAL_ECHOLNPGM(" bytes)");
}

/**
 * Retrieve Configuration Settings - M501
 */

void Config_RetrieveSettings() {

  int i = EEPROM_OFFSET;
  char stored_ver[4];
  char ver[4] = EEPROM_VERSION;
  EEPROM_READ_VAR(i, stored_ver); //read stored version
  //  SERIAL_ECHOLN("Version: [" << ver << "] Stored version: [" << stored_ver << "]");

  if (strncmp(ver, stored_ver, 3) != 0) {
    Config_ResetDefault();
  }
  else {
    float dummy = 0;

    // version number match
    EEPROM_READ_VAR(i, planner.axis_steps_per_mm);
    EEPROM_READ_VAR(i, planner.max_feedrate);
    EEPROM_READ_VAR(i, planner.max_acceleration_mm_per_s2);

    // steps per sq second need to be updated to agree with the units per sq second (as they are what is used in the planner)
    planner.reset_acceleration_rates();

    EEPROM_READ_VAR(i, planner.acceleration);
    EEPROM_READ_VAR(i, planner.retract_acceleration);
    EEPROM_READ_VAR(i, planner.travel_acceleration);
    EEPROM_READ_VAR(i, planner.min_feedrate);
    EEPROM_READ_VAR(i, planner.min_travel_feedrate);
    EEPROM_READ_VAR(i, planner.min_segment_time);
    EEPROM_READ_VAR(i, planner.max_xy_jerk);
    EEPROM_READ_VAR(i, planner.max_z_jerk);
    EEPROM_READ_VAR(i, planner.max_e_jerk);
    EEPROM_READ_VAR(i, home_offset);

    uint8_t dummy_uint8 = 0, mesh_num_x = 0, mesh_num_y = 0;
    EEPROM_READ_VAR(i, dummy_uint8);
    EEPROM_READ_VAR(i, dummy);
    EEPROM_READ_VAR(i, mesh_num_x);
    EEPROM_READ_VAR(i, mesh_num_y);
    #if ENABLED(MESH_BED_LEVELING)
      mbl.status = dummy_uint8;
      mbl.z_offset = dummy;
      if (mesh_num_x == MESH_NUM_X_POINTS && mesh_num_y == MESH_NUM_Y_POINTS) {
        EEPROM_READ_VAR(i, mbl.z_values);
      } else {
        mbl.reset();
        for (uint8_t q = 0; q < mesh_num_x * mesh_num_y; q++) EEPROM_READ_VAR(i, dummy);
      }
    #else
      for (uint8_t q = 0; q < mesh_num_x * mesh_num_y; q++) EEPROM_READ_VAR(i, dummy);
    #endif // MESH_BED_LEVELING

    #if DISABLED(AUTO_BED_LEVELING_FEATURE)
      float zprobe_zoffset = 0;
    #endif
    EEPROM_READ_VAR(i, zprobe_zoffset);

    #if ENABLED(DELTA)
      EEPROM_READ_VAR(i, endstop_adj);                // 3 floats
      EEPROM_READ_VAR(i, delta_radius);               // 1 float
      EEPROM_READ_VAR(i, delta_diagonal_rod);         // 1 float
      EEPROM_READ_VAR(i, delta_segments_per_second);  // 1 float
      EEPROM_READ_VAR(i, delta_diagonal_rod_trim_tower_1);  // 1 float
      EEPROM_READ_VAR(i, delta_diagonal_rod_trim_tower_2);  // 1 float
      EEPROM_READ_VAR(i, delta_diagonal_rod_trim_tower_3);  // 1 float
      recalc_delta_settings(delta_radius, delta_diagonal_rod);
    #elif ENABLED(Z_DUAL_ENDSTOPS)
      EEPROM_READ_VAR(i, z_endstop_adj);
      dummy = 0.0f;
      for (uint8_t q=8; q--;) EEPROM_READ_VAR(i, dummy);
    #else
      dummy = 0.0f;
      for (uint8_t q=9; q--;) EEPROM_READ_VAR(i, dummy);
    #endif

    #if DISABLED(ULTIPANEL)
      int plaPreheatHotendTemp, plaPreheatHPBTemp, plaPreheatFanSpeed,
          absPreheatHotendTemp, absPreheatHPBTemp, absPreheatFanSpeed;
    #endif

    EEPROM_READ_VAR(i, plaPreheatHotendTemp);
    EEPROM_READ_VAR(i, plaPreheatHPBTemp);
    EEPROM_READ_VAR(i, plaPreheatFanSpeed);
    EEPROM_READ_VAR(i, absPreheatHotendTemp);
    EEPROM_READ_VAR(i, absPreheatHPBTemp);
    EEPROM_READ_VAR(i, absPreheatFanSpeed);

    #if ENABLED(PIDTEMP)
      for (uint8_t e = 0; e < 4; e++) { // 4 = max extruders currently supported by Marlin
        EEPROM_READ_VAR(i, dummy); // Kp
        if (e < HOTENDS && dummy != DUMMY_PID_VALUE) {
          // do not need to scale PID values as the values in EEPROM are already scaled
          PID_PARAM(Kp, e) = dummy;
          EEPROM_READ_VAR(i, PID_PARAM(Ki, e));
          EEPROM_READ_VAR(i, PID_PARAM(Kd, e));
          #if ENABLED(PID_ADD_EXTRUSION_RATE)
            EEPROM_READ_VAR(i, PID_PARAM(Kc, e));
          #else
            EEPROM_READ_VAR(i, dummy);
          #endif
        }
        else {
          for (uint8_t q=3; q--;) EEPROM_READ_VAR(i, dummy); // Ki, Kd, Kc
        }
      }
    #else // !PIDTEMP
      // 4 x 4 = 16 slots for PID parameters
      for (uint8_t q=16; q--;) EEPROM_READ_VAR(i, dummy);  // 4x Kp, Ki, Kd, Kc
    #endif // !PIDTEMP

    #if DISABLED(PID_ADD_EXTRUSION_RATE)
      int lpq_len;
    #endif
    EEPROM_READ_VAR(i, lpq_len);

    #if ENABLED(PIDTEMPBED)
      EEPROM_READ_VAR(i, dummy); // bedKp
      if (dummy != DUMMY_PID_VALUE) {
        thermalManager.bedKp = dummy;
        EEPROM_READ_VAR(i, thermalManager.bedKi);
        EEPROM_READ_VAR(i, thermalManager.bedKd);
      }
    #else
      for (uint8_t q=3; q--;) EEPROM_READ_VAR(i, dummy); // bedKp, bedKi, bedKd
    #endif

    #if !HAS_LCD_CONTRAST
      int lcd_contrast;
    #endif
    EEPROM_READ_VAR(i, lcd_contrast);

    #if ENABLED(SCARA)
      EEPROM_READ_VAR(i, axis_scaling);  // 3 floats
    #else
      EEPROM_READ_VAR(i, dummy);
    #endif

    #if ENABLED(FWRETRACT)
      EEPROM_READ_VAR(i, autoretract_enabled);
      EEPROM_READ_VAR(i, retract_length);
      #if EXTRUDERS > 1
        EEPROM_READ_VAR(i, retract_length_swap);
      #else
        EEPROM_READ_VAR(i, dummy);
      #endif
      EEPROM_READ_VAR(i, retract_feedrate);
      EEPROM_READ_VAR(i, retract_zlift);
      EEPROM_READ_VAR(i, retract_recover_length);
      #if EXTRUDERS > 1
        EEPROM_READ_VAR(i, retract_recover_length_swap);
      #else
        EEPROM_READ_VAR(i, dummy);
      #endif
      EEPROM_READ_VAR(i, retract_recover_feedrate);
    #endif // FWRETRACT

    EEPROM_READ_VAR(i, volumetric_enabled);

    for (uint8_t q = 0; q < 4; q++) {
      EEPROM_READ_VAR(i, dummy);
      if (q < EXTRUDERS) filament_size[q] = dummy;
    }

    calculate_volumetric_multipliers();
    // Call thermalManager.updatePID (similar to when we have processed M301)
    thermalManager.updatePID();

    // Report settings retrieved and length
    SERIAL_ECHO_START;
    SERIAL_ECHO(ver);
    SERIAL_ECHOPAIR(" stored settings retrieved (", i);
    SERIAL_ECHOLNPGM(" bytes)");
  }

  #if ENABLED(EEPROM_CHITCHAT)
    Config_PrintSettings();
  #endif
}

#endif // EEPROM_SETTINGS

/**
 * Reset Configuration Settings - M502
 */

void Config_ResetDefault() {
  float tmp1[] = DEFAULT_AXIS_STEPS_PER_UNIT;
  float tmp2[] = DEFAULT_MAX_FEEDRATE;
  long tmp3[] = DEFAULT_MAX_ACCELERATION;
  for (uint8_t i = 0; i < NUM_AXIS; i++) {
    planner.axis_steps_per_mm[i] = tmp1[i];
    planner.max_feedrate[i] = tmp2[i];
    planner.max_acceleration_mm_per_s2[i] = tmp3[i];
    #if ENABLED(SCARA)
      if (i < COUNT(axis_scaling))
        axis_scaling[i] = 1;
    #endif
  }

  // steps per sq second need to be updated to agree with the units per sq second
  planner.reset_acceleration_rates();

  planner.acceleration = DEFAULT_ACCELERATION;
  planner.retract_acceleration = DEFAULT_RETRACT_ACCELERATION;
  planner.travel_acceleration = DEFAULT_TRAVEL_ACCELERATION;
  planner.min_feedrate = DEFAULT_MINIMUMFEEDRATE;
  planner.min_segment_time = DEFAULT_MINSEGMENTTIME;
  planner.min_travel_feedrate = DEFAULT_MINTRAVELFEEDRATE;
  planner.max_xy_jerk = DEFAULT_XYJERK;
  planner.max_z_jerk = DEFAULT_ZJERK;
  planner.max_e_jerk = DEFAULT_EJERK;
  home_offset[X_AXIS] = home_offset[Y_AXIS] = home_offset[Z_AXIS] = 0;

  #if ENABLED(MESH_BED_LEVELING)
    mbl.reset();
  #endif

  #if ENABLED(AUTO_BED_LEVELING_FEATURE)
    zprobe_zoffset = Z_PROBE_OFFSET_FROM_EXTRUDER;
  #endif

  #if ENABLED(DELTA)
    endstop_adj[X_AXIS] = endstop_adj[Y_AXIS] = endstop_adj[Z_AXIS] = 0;
    delta_radius =  DELTA_RADIUS;
    delta_diagonal_rod =  DELTA_DIAGONAL_ROD;
    delta_segments_per_second =  DELTA_SEGMENTS_PER_SECOND;
    delta_diagonal_rod_trim_tower_1 = DELTA_DIAGONAL_ROD_TRIM_TOWER_1;
    delta_diagonal_rod_trim_tower_2 = DELTA_DIAGONAL_ROD_TRIM_TOWER_2;
    delta_diagonal_rod_trim_tower_3 = DELTA_DIAGONAL_ROD_TRIM_TOWER_3;
    recalc_delta_settings(delta_radius, delta_diagonal_rod);
  #elif ENABLED(Z_DUAL_ENDSTOPS)
    z_endstop_adj = 0;
  #endif

  #if ENABLED(ULTIPANEL)
    plaPreheatHotendTemp = PLA_PREHEAT_HOTEND_TEMP;
    plaPreheatHPBTemp = PLA_PREHEAT_HPB_TEMP;
    plaPreheatFanSpeed = PLA_PREHEAT_FAN_SPEED;
    absPreheatHotendTemp = ABS_PREHEAT_HOTEND_TEMP;
    absPreheatHPBTemp = ABS_PREHEAT_HPB_TEMP;
    absPreheatFanSpeed = ABS_PREHEAT_FAN_SPEED;
  #endif

  #if HAS_LCD_CONTRAST
    lcd_contrast = DEFAULT_LCD_CONTRAST;
  #endif

  #if ENABLED(PIDTEMP)
    #if ENABLED(PID_PARAMS_PER_HOTEND)
      for (uint8_t e = 0; e < HOTENDS; e++)
    #else
      int e = 0; UNUSED(e); // only need to write once
    #endif
    {
      PID_PARAM(Kp, e) = DEFAULT_Kp;
      PID_PARAM(Ki, e) = scalePID_i(DEFAULT_Ki);
      PID_PARAM(Kd, e) = scalePID_d(DEFAULT_Kd);
      #if ENABLED(PID_ADD_EXTRUSION_RATE)
        PID_PARAM(Kc, e) = DEFAULT_Kc;
      #endif
    }
    #if ENABLED(PID_ADD_EXTRUSION_RATE)
      lpq_len = 20; // default last-position-queue size
    #endif
    // call thermalManager.updatePID (similar to when we have processed M301)
    thermalManager.updatePID();
  #endif // PIDTEMP

  #if ENABLED(PIDTEMPBED)
    thermalManager.bedKp = DEFAULT_bedKp;
    thermalManager.bedKi = scalePID_i(DEFAULT_bedKi);
    thermalManager.bedKd = scalePID_d(DEFAULT_bedKd);
  #endif

  #if ENABLED(FWRETRACT)
    autoretract_enabled = false;
    retract_length = RETRACT_LENGTH;
    #if EXTRUDERS > 1
      retract_length_swap = RETRACT_LENGTH_SWAP;
    #endif
    retract_feedrate = RETRACT_FEEDRATE;
    retract_zlift = RETRACT_ZLIFT;
    retract_recover_length = RETRACT_RECOVER_LENGTH;
    #if EXTRUDERS > 1
      retract_recover_length_swap = RETRACT_RECOVER_LENGTH_SWAP;
    #endif
    retract_recover_feedrate = RETRACT_RECOVER_FEEDRATE;
  #endif

  volumetric_enabled = false;
  for (uint8_t q = 0; q < COUNT(filament_size); q++)
    filament_size[q] = DEFAULT_NOMINAL_FILAMENT_DIA;
  calculate_volumetric_multipliers();

  SERIAL_ECHO_START;
  SERIAL_ECHOLNPGM("Hardcoded Default Settings Loaded");
}

#if DISABLED(DISABLE_M503)

/**
 * Print Configuration Settings - M503
 */

#define CONFIG_ECHO_START do{ if (!forReplay) SERIAL_ECHO_START; }while(0)

void Config_PrintSettings(bool forReplay) {
  // Always have this function, even with EEPROM_SETTINGS disabled, the current values will be shown

  CONFIG_ECHO_START;

  if (!forReplay) {
    SERIAL_ECHOLNPGM("Steps per unit:");
    CONFIG_ECHO_START;
  }
  SERIAL_ECHOPAIR("  M92 X", planner.axis_steps_per_mm[X_AXIS]);
  SERIAL_ECHOPAIR(" Y", planner.axis_steps_per_mm[Y_AXIS]);
  SERIAL_ECHOPAIR(" Z", planner.axis_steps_per_mm[Z_AXIS]);
  SERIAL_ECHOPAIR(" E", planner.axis_steps_per_mm[E_AXIS]);
  SERIAL_EOL;

  CONFIG_ECHO_START;

  #if ENABLED(SCARA)
    if (!forReplay) {
      SERIAL_ECHOLNPGM("Scaling factors:");
      CONFIG_ECHO_START;
    }
    SERIAL_ECHOPAIR("  M365 X", axis_scaling[X_AXIS]);
    SERIAL_ECHOPAIR(" Y", axis_scaling[Y_AXIS]);
    SERIAL_ECHOPAIR(" Z", axis_scaling[Z_AXIS]);
    SERIAL_EOL;
    CONFIG_ECHO_START;
  #endif // SCARA

  if (!forReplay) {
    SERIAL_ECHOLNPGM("Maximum feedrates (mm/s):");
    CONFIG_ECHO_START;
  }
  SERIAL_ECHOPAIR("  M203 X", planner.max_feedrate[X_AXIS]);
  SERIAL_ECHOPAIR(" Y", planner.max_feedrate[Y_AXIS]);
  SERIAL_ECHOPAIR(" Z", planner.max_feedrate[Z_AXIS]);
  SERIAL_ECHOPAIR(" E", planner.max_feedrate[E_AXIS]);
  SERIAL_EOL;

  CONFIG_ECHO_START;
  if (!forReplay) {
    SERIAL_ECHOLNPGM("Maximum Acceleration (mm/s2):");
    CONFIG_ECHO_START;
  }
  SERIAL_ECHOPAIR("  M201 X", planner.max_acceleration_mm_per_s2[X_AXIS]);
  SERIAL_ECHOPAIR(" Y", planner.max_acceleration_mm_per_s2[Y_AXIS]);
  SERIAL_ECHOPAIR(" Z", planner.max_acceleration_mm_per_s2[Z_AXIS]);
  SERIAL_ECHOPAIR(" E", planner.max_acceleration_mm_per_s2[E_AXIS]);
  SERIAL_EOL;
  CONFIG_ECHO_START;
  if (!forReplay) {
    SERIAL_ECHOLNPGM("Accelerations: P=printing, R=retract and T=travel");
    CONFIG_ECHO_START;
  }
  SERIAL_ECHOPAIR("  M204 P", planner.acceleration);
  SERIAL_ECHOPAIR(" R", planner.retract_acceleration);
  SERIAL_ECHOPAIR(" T", planner.travel_acceleration);
  SERIAL_EOL;

  CONFIG_ECHO_START;
  if (!forReplay) {
    SERIAL_ECHOLNPGM("Advanced variables: S=Min feedrate (mm/s), T=Min travel feedrate (mm/s), B=minimum segment time (ms), X=maximum XY jerk (mm/s),  Z=maximum Z jerk (mm/s),  E=maximum E jerk (mm/s)");
    CONFIG_ECHO_START;
  }
  SERIAL_ECHOPAIR("  M205 S", planner.min_feedrate);
  SERIAL_ECHOPAIR(" T", planner.min_travel_feedrate);
  SERIAL_ECHOPAIR(" B", planner.min_segment_time);
  SERIAL_ECHOPAIR(" X", planner.max_xy_jerk);
  SERIAL_ECHOPAIR(" Z", planner.max_z_jerk);
  SERIAL_ECHOPAIR(" E", planner.max_e_jerk);
  SERIAL_EOL;

  CONFIG_ECHO_START;
  if (!forReplay) {
    SERIAL_ECHOLNPGM("Home offset (mm):");
    CONFIG_ECHO_START;
  }
  SERIAL_ECHOPAIR("  M206 X", home_offset[X_AXIS]);
  SERIAL_ECHOPAIR(" Y", home_offset[Y_AXIS]);
  SERIAL_ECHOPAIR(" Z", home_offset[Z_AXIS]);
  SERIAL_EOL;

  #if ENABLED(MESH_BED_LEVELING)
    if (!forReplay) {
      SERIAL_ECHOLNPGM("Mesh bed leveling:");
      CONFIG_ECHO_START;
    }
    SERIAL_ECHOPAIR("  M420 S", mbl.has_mesh() ? 1 : 0);
    SERIAL_ECHOPAIR(" X", MESH_NUM_X_POINTS);
    SERIAL_ECHOPAIR(" Y", MESH_NUM_Y_POINTS);
    SERIAL_EOL;
    for (uint8_t py = 1; py <= MESH_NUM_Y_POINTS; py++) {
      for (uint8_t px = 1; px <= MESH_NUM_X_POINTS; px++) {
        CONFIG_ECHO_START;
        SERIAL_ECHOPAIR("  G29 S3 X", px);
        SERIAL_ECHOPAIR(" Y", py);
        SERIAL_ECHOPGM(" Z");
        SERIAL_PROTOCOL_F(mbl.z_values[py-1][px-1], 5);
        SERIAL_EOL;
      }
    }
  #endif

  #if ENABLED(DELTA)
    CONFIG_ECHO_START;
    if (!forReplay) {
      SERIAL_ECHOLNPGM("Endstop adjustment (mm):");
      CONFIG_ECHO_START;
    }
    SERIAL_ECHOPAIR("  M666 X", endstop_adj[X_AXIS]);
    SERIAL_ECHOPAIR(" Y", endstop_adj[Y_AXIS]);
    SERIAL_ECHOPAIR(" Z", endstop_adj[Z_AXIS]);
    SERIAL_EOL;
    CONFIG_ECHO_START;
    if (!forReplay) {
      SERIAL_ECHOLNPGM("Delta settings: L=diagonal_rod, R=radius, S=segments_per_second, ABC=diagonal_rod_trim_tower_[123]");
      CONFIG_ECHO_START;
    }
    SERIAL_ECHOPAIR("  M665 L", delta_diagonal_rod);
    SERIAL_ECHOPAIR(" R", delta_radius);
    SERIAL_ECHOPAIR(" S", delta_segments_per_second);
    SERIAL_ECHOPAIR(" A", delta_diagonal_rod_trim_tower_1);
    SERIAL_ECHOPAIR(" B", delta_diagonal_rod_trim_tower_2);
    SERIAL_ECHOPAIR(" C", delta_diagonal_rod_trim_tower_3);
    SERIAL_EOL;
  #elif ENABLED(Z_DUAL_ENDSTOPS)
    CONFIG_ECHO_START;
    if (!forReplay) {
      SERIAL_ECHOLNPGM("Z2 Endstop adjustment (mm):");
      CONFIG_ECHO_START;
    }
    SERIAL_ECHOPAIR("  M666 Z", z_endstop_adj);
    SERIAL_EOL;
  #endif // DELTA

  #if ENABLED(ULTIPANEL)
    CONFIG_ECHO_START;
    if (!forReplay) {
      SERIAL_ECHOLNPGM("Material heatup parameters:");
      CONFIG_ECHO_START;
    }
    SERIAL_ECHOPAIR("  M145 S0 H", plaPreheatHotendTemp);
    SERIAL_ECHOPAIR(" B", plaPreheatHPBTemp);
    SERIAL_ECHOPAIR(" F", plaPreheatFanSpeed);
    SERIAL_EOL;
    CONFIG_ECHO_START;
    SERIAL_ECHOPAIR("  M145 S1 H", absPreheatHotendTemp);
    SERIAL_ECHOPAIR(" B", absPreheatHPBTemp);
    SERIAL_ECHOPAIR(" F", absPreheatFanSpeed);
    SERIAL_EOL;
  #endif // ULTIPANEL

  #if HAS_PID_HEATING

    CONFIG_ECHO_START;
    if (!forReplay) {
      SERIAL_ECHOLNPGM("PID settings:");
    }
    #if ENABLED(PIDTEMP)
      #if HOTENDS > 1
        if (forReplay) {
          for (uint8_t i = 0; i < HOTENDS; i++) {
            CONFIG_ECHO_START;
            SERIAL_ECHOPAIR("  M301 E", i);
            SERIAL_ECHOPAIR(" P", PID_PARAM(Kp, i));
            SERIAL_ECHOPAIR(" I", unscalePID_i(PID_PARAM(Ki, i)));
            SERIAL_ECHOPAIR(" D", unscalePID_d(PID_PARAM(Kd, i)));
            #if ENABLED(PID_ADD_EXTRUSION_RATE)
              SERIAL_ECHOPAIR(" C", PID_PARAM(Kc, i));
              if (i == 0) SERIAL_ECHOPAIR(" L", lpq_len);
            #endif
            SERIAL_EOL;
          }
        }
        else
      #endif // HOTENDS > 1
      // !forReplay || HOTENDS == 1
      {
        CONFIG_ECHO_START;
        SERIAL_ECHOPAIR("  M301 P", PID_PARAM(Kp, 0)); // for compatibility with hosts, only echo values for E0
        SERIAL_ECHOPAIR(" I", unscalePID_i(PID_PARAM(Ki, 0)));
        SERIAL_ECHOPAIR(" D", unscalePID_d(PID_PARAM(Kd, 0)));
        #if ENABLED(PID_ADD_EXTRUSION_RATE)
          SERIAL_ECHOPAIR(" C", PID_PARAM(Kc, 0));
          SERIAL_ECHOPAIR(" L", lpq_len);
        #endif
        SERIAL_EOL;
      }
    #endif // PIDTEMP

    #if ENABLED(PIDTEMPBED)
      CONFIG_ECHO_START;
      SERIAL_ECHOPAIR("  M304 P", thermalManager.bedKp);
      SERIAL_ECHOPAIR(" I", unscalePID_i(thermalManager.bedKi));
      SERIAL_ECHOPAIR(" D", unscalePID_d(thermalManager.bedKd));
      SERIAL_EOL;
    #endif

  #endif // PIDTEMP || PIDTEMPBED

  #if HAS_LCD_CONTRAST
    CONFIG_ECHO_START;
    if (!forReplay) {
      SERIAL_ECHOLNPGM("LCD Contrast:");
      CONFIG_ECHO_START;
    }
    SERIAL_ECHOPAIR("  M250 C", lcd_contrast);
    SERIAL_EOL;
  #endif

  #if ENABLED(FWRETRACT)

    CONFIG_ECHO_START;
    if (!forReplay) {
      SERIAL_ECHOLNPGM("Retract: S=Length (mm) F:Speed (mm/m) Z: ZLift (mm)");
      CONFIG_ECHO_START;
    }
    SERIAL_ECHOPAIR("  M207 S", retract_length);
    #if EXTRUDERS > 1
      SERIAL_ECHOPAIR(" W", retract_length_swap);
    #endif
    SERIAL_ECHOPAIR(" F", retract_feedrate * 60);
    SERIAL_ECHOPAIR(" Z", retract_zlift);
    SERIAL_EOL;
    CONFIG_ECHO_START;
    if (!forReplay) {
      SERIAL_ECHOLNPGM("Recover: S=Extra length (mm) F:Speed (mm/m)");
      CONFIG_ECHO_START;
    }
    SERIAL_ECHOPAIR("  M208 S", retract_recover_length);
    #if EXTRUDERS > 1
      SERIAL_ECHOPAIR(" W", retract_recover_length_swap);
    #endif
    SERIAL_ECHOPAIR(" F", retract_recover_feedrate * 60);
    SERIAL_EOL;
    CONFIG_ECHO_START;
    if (!forReplay) {
      SERIAL_ECHOLNPGM("Auto-Retract: S=0 to disable, 1 to interpret extrude-only moves as retracts or recoveries");
      CONFIG_ECHO_START;
    }
    SERIAL_ECHOPAIR("  M209 S", (autoretract_enabled ? 1 : 0));
    SERIAL_EOL;

  #endif // FWRETRACT

  /**
   * Volumetric extrusion M200
   */
  if (!forReplay) {
    CONFIG_ECHO_START;
    SERIAL_ECHOPGM("Filament settings:");
    if (volumetric_enabled)
      SERIAL_EOL;
    else
      SERIAL_ECHOLNPGM(" Disabled");
  }

  CONFIG_ECHO_START;
  SERIAL_ECHOPAIR("  M200 D", filament_size[0]);
  SERIAL_EOL;
  #if EXTRUDERS > 1
    CONFIG_ECHO_START;
    SERIAL_ECHOPAIR("  M200 T1 D", filament_size[1]);
    SERIAL_EOL;
    #if EXTRUDERS > 2
      CONFIG_ECHO_START;
      SERIAL_ECHOPAIR("  M200 T2 D", filament_size[2]);
      SERIAL_EOL;
      #if EXTRUDERS > 3
        CONFIG_ECHO_START;
        SERIAL_ECHOPAIR("  M200 T3 D", filament_size[3]);
        SERIAL_EOL;
      #endif
    #endif
  #endif

  if (!volumetric_enabled) {
    CONFIG_ECHO_START;
    SERIAL_ECHOLNPGM("  M200 D0");
  }

  /**
   * Auto Bed Leveling
   */
  #if ENABLED(AUTO_BED_LEVELING_FEATURE)
    #if ENABLED(CUSTOM_M_CODES)
      if (!forReplay) {
        CONFIG_ECHO_START;
        SERIAL_ECHOLNPGM("Z-Probe Offset (mm):");
      }
      CONFIG_ECHO_START;
      SERIAL_ECHOPAIR("  M" STRINGIFY(CUSTOM_M_CODE_SET_Z_PROBE_OFFSET) " Z", zprobe_zoffset);
    #else
      if (!forReplay) {
        CONFIG_ECHO_START;
        SERIAL_ECHOPAIR("Z-Probe Offset (mm):", zprobe_zoffset);
      }
    #endif
    SERIAL_EOL;
  #endif
}

#endif // !DISABLE_M503
