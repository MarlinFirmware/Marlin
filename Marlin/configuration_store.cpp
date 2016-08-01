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

#define EEPROM_VERSION "V24"

// Change EEPROM version if these are changed:
#define EEPROM_OFFSET 100
#define MAX_EXTRUDERS 4

/**
 * V24 EEPROM Layout:
 *
 *  100  Version (char x4)
 *  104  EEPROM Checksum (uint16_t)
 *
 *  106  M92 XYZE  planner.axis_steps_per_mm (float x4)
 *  122  M203 XYZE planner.max_feedrate_mm_s (float x4)
 *  138  M201 XYZE planner.max_acceleration_mm_per_s2 (uint32_t x4)
 *  154  M204 P    planner.acceleration (float)
 *  158  M204 R    planner.retract_acceleration (float)
 *  162  M204 T    planner.travel_acceleration (float)
 *  166  M205 S    planner.min_feedrate_mm_s (float)
 *  170  M205 T    planner.min_travel_feedrate_mm_s (float)
 *  174  M205 B    planner.min_segment_time (ulong)
 *  178  M205 X    planner.max_xy_jerk (float)
 *  182  M205 Z    planner.max_z_jerk (float)
 *  186  M205 E    planner.max_e_jerk (float)
 *  190  M206 XYZ  home_offset (float x3)
 *
 * Mesh bed leveling:
 *  202  M420 S    status (uint8)
 *  203            z_offset (float)
 *  207            mesh_num_x (uint8 as set in firmware)
 *  208            mesh_num_y (uint8 as set in firmware)
 *  209 G29 S3 XYZ z_values[][] (float x9, by default, up to float x 81)
 *
 * AUTO BED LEVELING
 *  245  M851      zprobe_zoffset (float)
 *
 * DELTA:
 *  249  M666 XYZ  endstop_adj (float x3)
 *  261  M665 R    delta_radius (float)
 *  265  M665 L    delta_diagonal_rod (float)
 *  269  M665 S    delta_segments_per_second (float)
 *  273  M665 A    delta_diagonal_rod_trim_tower_1 (float)
 *  277  M665 B    delta_diagonal_rod_trim_tower_2 (float)
 *  281  M665 C    delta_diagonal_rod_trim_tower_3 (float)
 *
 * Z_DUAL_ENDSTOPS:
 *  285  M666 Z    z_endstop_adj (float)
 *
 * ULTIPANEL:
 *  289  M145 S0 H preheatHotendTemp1 (int)
 *  291  M145 S0 B preheatBedTemp1 (int)
 *  293  M145 S0 F preheatFanSpeed1 (int)
 *  295  M145 S1 H preheatHotendTemp2 (int)
 *  297  M145 S1 B preheatBedTemp2 (int)
 *  299  M145 S1 F preheatFanSpeed2 (int)
 *
 * PIDTEMP:
 *  301  M301 E0 PIDC  Kp[0], Ki[0], Kd[0], Kc[0] (float x4)
 *  317  M301 E1 PIDC  Kp[1], Ki[1], Kd[1], Kc[1] (float x4)
 *  333  M301 E2 PIDC  Kp[2], Ki[2], Kd[2], Kc[2] (float x4)
 *  349  M301 E3 PIDC  Kp[3], Ki[3], Kd[3], Kc[3] (float x4)
 *  365  M301 L        lpq_len (int)
 *
 * PIDTEMPBED:
 *  367  M304 PID  thermalManager.bedKp, thermalManager.bedKi, thermalManager.bedKd (float x3)
 *
 * DOGLCD:
 *  379  M250 C    lcd_contrast (int)
 *
 * SCARA:
 *  381  M365 XYZ  axis_scaling (float x3)
 *
 * FWRETRACT:
 *  393  M209 S    autoretract_enabled (bool)
 *  394  M207 S    retract_length (float)
 *  398  M207 W    retract_length_swap (float)
 *  402  M207 F    retract_feedrate_mm_s (float)
 *  406  M207 Z    retract_zlift (float)
 *  410  M208 S    retract_recover_length (float)
 *  414  M208 W    retract_recover_length_swap (float)
 *  418  M208 F    retract_recover_feedrate_mm_s (float)
 *
 * Volumetric Extrusion:
 *  422  M200 D    volumetric_enabled (bool)
 *  423  M200 T D  filament_size (float x4) (T0..3)
 *
 *  439  This Slot is Available!
 *
 */
#include "Marlin.h"
#include "language.h"
#include "endstops.h"
#include "planner.h"
#include "temperature.h"
#include "ultralcd.h"
#include "configuration_store.h"

#if ENABLED(MESH_BED_LEVELING)
  #include "mesh_bed_leveling.h"
#endif

uint16_t eeprom_checksum;
const char version[4] = EEPROM_VERSION;

void _EEPROM_writeData(int &pos, uint8_t* value, uint8_t size) {
  uint8_t c;
  while (size--) {
    eeprom_write_byte((unsigned char*)pos, *value);
    c = eeprom_read_byte((unsigned char*)pos);
    if (c != *value) {
      SERIAL_ECHO_START;
      SERIAL_ECHOLNPGM(MSG_ERR_EEPROM_WRITE);
    }
    eeprom_checksum += c;
    pos++;
    value++;
  };
}
void _EEPROM_readData(int &pos, uint8_t* value, uint8_t size) {
  do {
    uint8_t c = eeprom_read_byte((unsigned char*)pos);
    *value = c;
    eeprom_checksum += c;
    pos++;
    value++;
  } while (--size);
}

/**
 * Post-process after Retrieve or Reset
 */
void Config_Postprocess() {
  // steps per s2 needs to be updated to agree with units per s2
  planner.reset_acceleration_rates();

  // Make sure delta kinematics are updated before refreshing the
  // planner position so the stepper counts will be set correctly.
  #if ENABLED(DELTA)
    recalc_delta_settings(delta_radius, delta_diagonal_rod);
  #endif

  // Refresh steps_to_mm with the reciprocal of axis_steps_per_mm
  // and init stepper.count[], planner.position[] with current_position
  planner.refresh_positioning();

  #if ENABLED(PIDTEMP)
    thermalManager.updatePID();
  #endif

  calculate_volumetric_multipliers();
}

#if ENABLED(EEPROM_SETTINGS)

  #define DUMMY_PID_VALUE 3000.0f
  #define EEPROM_START() int eeprom_index = EEPROM_OFFSET
  #define EEPROM_SKIP(VAR) eeprom_index += sizeof(VAR)
  #define EEPROM_WRITE(VAR) _EEPROM_writeData(eeprom_index, (uint8_t*)&VAR, sizeof(VAR))
  #define EEPROM_READ(VAR) _EEPROM_readData(eeprom_index, (uint8_t*)&VAR, sizeof(VAR))

/**
 * M500 - Store Configuration
 */
void Config_StoreSettings()  {
  float dummy = 0.0f;
  char ver[4] = "000";

  EEPROM_START();

  EEPROM_WRITE(ver);     // invalidate data first
  EEPROM_SKIP(eeprom_checksum); // Skip the checksum slot

  eeprom_checksum = 0; // clear before first "real data"

  EEPROM_WRITE(planner.axis_steps_per_mm);
  EEPROM_WRITE(planner.max_feedrate_mm_s);
  EEPROM_WRITE(planner.max_acceleration_mm_per_s2);
  EEPROM_WRITE(planner.acceleration);
  EEPROM_WRITE(planner.retract_acceleration);
  EEPROM_WRITE(planner.travel_acceleration);
  EEPROM_WRITE(planner.min_feedrate_mm_s);
  EEPROM_WRITE(planner.min_travel_feedrate_mm_s);
  EEPROM_WRITE(planner.min_segment_time);
  EEPROM_WRITE(planner.max_xy_jerk);
  EEPROM_WRITE(planner.max_z_jerk);
  EEPROM_WRITE(planner.max_e_jerk);
  EEPROM_WRITE(home_offset);

  #if ENABLED(MESH_BED_LEVELING)
    // Compile time test that sizeof(mbl.z_values) is as expected
    typedef char c_assert[(sizeof(mbl.z_values) == (MESH_NUM_X_POINTS) * (MESH_NUM_Y_POINTS) * sizeof(dummy)) ? 1 : -1];
    uint8_t mesh_num_x = MESH_NUM_X_POINTS,
            mesh_num_y = MESH_NUM_Y_POINTS,
            dummy_uint8 = mbl.status & _BV(MBL_STATUS_HAS_MESH_BIT);
    EEPROM_WRITE(dummy_uint8);
    EEPROM_WRITE(mbl.z_offset);
    EEPROM_WRITE(mesh_num_x);
    EEPROM_WRITE(mesh_num_y);
    EEPROM_WRITE(mbl.z_values);
  #else
    // For disabled MBL write a default mesh
    uint8_t mesh_num_x = 3,
            mesh_num_y = 3,
            dummy_uint8 = 0;
    dummy = 0.0f;
    EEPROM_WRITE(dummy_uint8);
    EEPROM_WRITE(dummy);
    EEPROM_WRITE(mesh_num_x);
    EEPROM_WRITE(mesh_num_y);
    for (uint8_t q = 0; q < mesh_num_x * mesh_num_y; q++) EEPROM_WRITE(dummy);
  #endif // MESH_BED_LEVELING

  #if !HAS_BED_PROBE
    float zprobe_zoffset = 0;
  #endif
  EEPROM_WRITE(zprobe_zoffset);

  // 9 floats for DELTA / Z_DUAL_ENDSTOPS
  #if ENABLED(DELTA)
    EEPROM_WRITE(endstop_adj);               // 3 floats
    EEPROM_WRITE(delta_radius);              // 1 float
    EEPROM_WRITE(delta_diagonal_rod);        // 1 float
    EEPROM_WRITE(delta_segments_per_second); // 1 float
    EEPROM_WRITE(delta_diagonal_rod_trim_tower_1);  // 1 float
    EEPROM_WRITE(delta_diagonal_rod_trim_tower_2);  // 1 float
    EEPROM_WRITE(delta_diagonal_rod_trim_tower_3);  // 1 float
  #elif ENABLED(Z_DUAL_ENDSTOPS)
    EEPROM_WRITE(z_endstop_adj);            // 1 float
    dummy = 0.0f;
    for (uint8_t q = 8; q--;) EEPROM_WRITE(dummy);
  #else
    dummy = 0.0f;
    for (uint8_t q = 9; q--;) EEPROM_WRITE(dummy);
  #endif

  #if DISABLED(ULTIPANEL)
    int preheatHotendTemp1 = PREHEAT_1_TEMP_HOTEND, preheatBedTemp1 = PREHEAT_1_TEMP_BED, preheatFanSpeed1 = PREHEAT_1_FAN_SPEED,
        preheatHotendTemp2 = PREHEAT_2_TEMP_HOTEND, preheatBedTemp2 = PREHEAT_2_TEMP_BED, preheatFanSpeed2 = PREHEAT_2_FAN_SPEED;
  #endif // !ULTIPANEL

  EEPROM_WRITE(preheatHotendTemp1);
  EEPROM_WRITE(preheatBedTemp1);
  EEPROM_WRITE(preheatFanSpeed1);
  EEPROM_WRITE(preheatHotendTemp2);
  EEPROM_WRITE(preheatBedTemp2);
  EEPROM_WRITE(preheatFanSpeed2);

  for (uint8_t e = 0; e < MAX_EXTRUDERS; e++) {

    #if ENABLED(PIDTEMP)
      if (e < HOTENDS) {
        EEPROM_WRITE(PID_PARAM(Kp, e));
        EEPROM_WRITE(PID_PARAM(Ki, e));
        EEPROM_WRITE(PID_PARAM(Kd, e));
        #if ENABLED(PID_EXTRUSION_SCALING)
          EEPROM_WRITE(PID_PARAM(Kc, e));
        #else
          dummy = 1.0f; // 1.0 = default kc
          EEPROM_WRITE(dummy);
        #endif
      }
      else
    #endif // !PIDTEMP
      {
        dummy = DUMMY_PID_VALUE; // When read, will not change the existing value
        EEPROM_WRITE(dummy); // Kp
        dummy = 0.0f;
        for (uint8_t q = 3; q--;) EEPROM_WRITE(dummy); // Ki, Kd, Kc
      }

  } // Hotends Loop

  #if DISABLED(PID_EXTRUSION_SCALING)
    int lpq_len = 20;
  #endif
  EEPROM_WRITE(lpq_len);

  #if DISABLED(PIDTEMPBED)
    dummy = DUMMY_PID_VALUE;
    for (uint8_t q = 3; q--;) EEPROM_WRITE(dummy);
  #else
    EEPROM_WRITE(thermalManager.bedKp);
    EEPROM_WRITE(thermalManager.bedKi);
    EEPROM_WRITE(thermalManager.bedKd);
  #endif

  #if !HAS_LCD_CONTRAST
    const int lcd_contrast = 32;
  #endif
  EEPROM_WRITE(lcd_contrast);

  #if ENABLED(SCARA)
    EEPROM_WRITE(axis_scaling); // 3 floats
  #else
    dummy = 1.0f;
    EEPROM_WRITE(dummy);
  #endif

  #if ENABLED(FWRETRACT)
    EEPROM_WRITE(autoretract_enabled);
    EEPROM_WRITE(retract_length);
    #if EXTRUDERS > 1
      EEPROM_WRITE(retract_length_swap);
    #else
      dummy = 0.0f;
      EEPROM_WRITE(dummy);
    #endif
    EEPROM_WRITE(retract_feedrate_mm_s);
    EEPROM_WRITE(retract_zlift);
    EEPROM_WRITE(retract_recover_length);
    #if EXTRUDERS > 1
      EEPROM_WRITE(retract_recover_length_swap);
    #else
      dummy = 0.0f;
      EEPROM_WRITE(dummy);
    #endif
    EEPROM_WRITE(retract_recover_feedrate_mm_s);
  #endif // FWRETRACT

  EEPROM_WRITE(volumetric_enabled);

  // Save filament sizes
  for (uint8_t q = 0; q < MAX_EXTRUDERS; q++) {
    if (q < COUNT(filament_size)) dummy = filament_size[q];
    EEPROM_WRITE(dummy);
  }

  uint16_t final_checksum = eeprom_checksum,
           eeprom_size = eeprom_index;

  eeprom_index = EEPROM_OFFSET;
  EEPROM_WRITE(version);
  EEPROM_WRITE(final_checksum);

  // Report storage size
  SERIAL_ECHO_START;
  SERIAL_ECHOPAIR("Settings Stored (", eeprom_size);
  SERIAL_ECHOLNPGM(" bytes)");
}

/**
 * M501 - Retrieve Configuration
 */
void Config_RetrieveSettings() {

  EEPROM_START();

  char stored_ver[4];
  EEPROM_READ(stored_ver);

  uint16_t stored_checksum;
  EEPROM_READ(stored_checksum);

  //  SERIAL_ECHOPAIR("Version: [", ver);
  //  SERIAL_ECHOPAIR("] Stored version: [", stored_ver);
  //  SERIAL_ECHOLNPGM("]");

  if (strncmp(version, stored_ver, 3) != 0) {
    Config_ResetDefault();
  }
  else {
    float dummy = 0;

    eeprom_checksum = 0; // clear before reading first "real data"

    // version number match
    EEPROM_READ(planner.axis_steps_per_mm);
    EEPROM_READ(planner.max_feedrate_mm_s);
    EEPROM_READ(planner.max_acceleration_mm_per_s2);

    EEPROM_READ(planner.acceleration);
    EEPROM_READ(planner.retract_acceleration);
    EEPROM_READ(planner.travel_acceleration);
    EEPROM_READ(planner.min_feedrate_mm_s);
    EEPROM_READ(planner.min_travel_feedrate_mm_s);
    EEPROM_READ(planner.min_segment_time);
    EEPROM_READ(planner.max_xy_jerk);
    EEPROM_READ(planner.max_z_jerk);
    EEPROM_READ(planner.max_e_jerk);
    EEPROM_READ(home_offset);

    uint8_t dummy_uint8 = 0, mesh_num_x = 0, mesh_num_y = 0;
    EEPROM_READ(dummy_uint8);
    EEPROM_READ(dummy);
    EEPROM_READ(mesh_num_x);
    EEPROM_READ(mesh_num_y);
    #if ENABLED(MESH_BED_LEVELING)
      mbl.status = dummy_uint8;
      mbl.z_offset = dummy;
      if (mesh_num_x == MESH_NUM_X_POINTS && mesh_num_y == MESH_NUM_Y_POINTS) {
        // EEPROM data fits the current mesh
        EEPROM_READ(mbl.z_values);
      }
      else {
        // EEPROM data is stale
        mbl.reset();
        for (uint8_t q = 0; q < mesh_num_x * mesh_num_y; q++) EEPROM_READ(dummy);
      }
    #else
      // MBL is disabled - skip the stored data
      for (uint8_t q = 0; q < mesh_num_x * mesh_num_y; q++) EEPROM_READ(dummy);
    #endif // MESH_BED_LEVELING

    #if !HAS_BED_PROBE
      float zprobe_zoffset = 0;
    #endif
    EEPROM_READ(zprobe_zoffset);

    #if ENABLED(DELTA)
      EEPROM_READ(endstop_adj);                // 3 floats
      EEPROM_READ(delta_radius);               // 1 float
      EEPROM_READ(delta_diagonal_rod);         // 1 float
      EEPROM_READ(delta_segments_per_second);  // 1 float
      EEPROM_READ(delta_diagonal_rod_trim_tower_1);  // 1 float
      EEPROM_READ(delta_diagonal_rod_trim_tower_2);  // 1 float
      EEPROM_READ(delta_diagonal_rod_trim_tower_3);  // 1 float
    #elif ENABLED(Z_DUAL_ENDSTOPS)
      EEPROM_READ(z_endstop_adj);
      dummy = 0.0f;
      for (uint8_t q=8; q--;) EEPROM_READ(dummy);
    #else
      dummy = 0.0f;
      for (uint8_t q=9; q--;) EEPROM_READ(dummy);
    #endif

    #if DISABLED(ULTIPANEL)
      int preheatHotendTemp1, preheatBedTemp1, preheatFanSpeed1,
          preheatHotendTemp2, preheatBedTemp2, preheatFanSpeed2;
    #endif

    EEPROM_READ(preheatHotendTemp1);
    EEPROM_READ(preheatBedTemp1);
    EEPROM_READ(preheatFanSpeed1);
    EEPROM_READ(preheatHotendTemp2);
    EEPROM_READ(preheatBedTemp2);
    EEPROM_READ(preheatFanSpeed2);

    #if ENABLED(PIDTEMP)
      for (uint8_t e = 0; e < MAX_EXTRUDERS; e++) {
        EEPROM_READ(dummy); // Kp
        if (e < HOTENDS && dummy != DUMMY_PID_VALUE) {
          // do not need to scale PID values as the values in EEPROM are already scaled
          PID_PARAM(Kp, e) = dummy;
          EEPROM_READ(PID_PARAM(Ki, e));
          EEPROM_READ(PID_PARAM(Kd, e));
          #if ENABLED(PID_EXTRUSION_SCALING)
            EEPROM_READ(PID_PARAM(Kc, e));
          #else
            EEPROM_READ(dummy);
          #endif
        }
        else {
          for (uint8_t q=3; q--;) EEPROM_READ(dummy); // Ki, Kd, Kc
        }
      }
    #else // !PIDTEMP
      // 4 x 4 = 16 slots for PID parameters
      for (uint8_t q = MAX_EXTRUDERS * 4; q--;) EEPROM_READ(dummy);  // Kp, Ki, Kd, Kc
    #endif // !PIDTEMP

    #if DISABLED(PID_EXTRUSION_SCALING)
      int lpq_len;
    #endif
    EEPROM_READ(lpq_len);

    #if ENABLED(PIDTEMPBED)
      EEPROM_READ(dummy); // bedKp
      if (dummy != DUMMY_PID_VALUE) {
        thermalManager.bedKp = dummy;
        EEPROM_READ(thermalManager.bedKi);
        EEPROM_READ(thermalManager.bedKd);
      }
    #else
      for (uint8_t q=3; q--;) EEPROM_READ(dummy); // bedKp, bedKi, bedKd
    #endif

    #if !HAS_LCD_CONTRAST
      int lcd_contrast;
    #endif
    EEPROM_READ(lcd_contrast);

    #if ENABLED(SCARA)
      EEPROM_READ(axis_scaling);  // 3 floats
    #else
      EEPROM_READ(dummy);
    #endif

    #if ENABLED(FWRETRACT)
      EEPROM_READ(autoretract_enabled);
      EEPROM_READ(retract_length);
      #if EXTRUDERS > 1
        EEPROM_READ(retract_length_swap);
      #else
        EEPROM_READ(dummy);
      #endif
      EEPROM_READ(retract_feedrate_mm_s);
      EEPROM_READ(retract_zlift);
      EEPROM_READ(retract_recover_length);
      #if EXTRUDERS > 1
        EEPROM_READ(retract_recover_length_swap);
      #else
        EEPROM_READ(dummy);
      #endif
      EEPROM_READ(retract_recover_feedrate_mm_s);
    #endif // FWRETRACT

    EEPROM_READ(volumetric_enabled);

    for (uint8_t q = 0; q < MAX_EXTRUDERS; q++) {
      EEPROM_READ(dummy);
      if (q < COUNT(filament_size)) filament_size[q] = dummy;
    }

    if (eeprom_checksum == stored_checksum) {
      Config_Postprocess();
      SERIAL_ECHO_START;
      SERIAL_ECHO(version);
      SERIAL_ECHOPAIR(" stored settings retrieved (", eeprom_index);
      SERIAL_ECHOLNPGM(" bytes)");
    }
    else {
      SERIAL_ERROR_START;
      SERIAL_ERRORLNPGM("EEPROM checksum mismatch");
      Config_ResetDefault();
    }
 }

  #if ENABLED(EEPROM_CHITCHAT)
    Config_PrintSettings();
  #endif
}

#endif // EEPROM_SETTINGS

/**
 * M502 - Reset Configuration
 */
void Config_ResetDefault() {
  float tmp1[] = DEFAULT_AXIS_STEPS_PER_UNIT;
  float tmp2[] = DEFAULT_MAX_FEEDRATE;
  long tmp3[] = DEFAULT_MAX_ACCELERATION;
  LOOP_XYZE(i) {
    planner.axis_steps_per_mm[i] = tmp1[i];
    planner.max_feedrate_mm_s[i] = tmp2[i];
    planner.max_acceleration_mm_per_s2[i] = tmp3[i];
    #if ENABLED(SCARA)
      if (i < COUNT(axis_scaling))
        axis_scaling[i] = 1;
    #endif
  }

  planner.acceleration = DEFAULT_ACCELERATION;
  planner.retract_acceleration = DEFAULT_RETRACT_ACCELERATION;
  planner.travel_acceleration = DEFAULT_TRAVEL_ACCELERATION;
  planner.min_feedrate_mm_s = DEFAULT_MINIMUMFEEDRATE;
  planner.min_segment_time = DEFAULT_MINSEGMENTTIME;
  planner.min_travel_feedrate_mm_s = DEFAULT_MINTRAVELFEEDRATE;
  planner.max_xy_jerk = DEFAULT_XYJERK;
  planner.max_z_jerk = DEFAULT_ZJERK;
  planner.max_e_jerk = DEFAULT_EJERK;
  home_offset[X_AXIS] = home_offset[Y_AXIS] = home_offset[Z_AXIS] = 0;

  #if ENABLED(MESH_BED_LEVELING)
    mbl.reset();
  #endif

  #if HAS_BED_PROBE
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
  #elif ENABLED(Z_DUAL_ENDSTOPS)
    z_endstop_adj = 0;
  #endif

  #if ENABLED(ULTIPANEL)
    preheatHotendTemp1 = PREHEAT_1_TEMP_HOTEND;
    preheatBedTemp1 = PREHEAT_1_TEMP_BED;
    preheatFanSpeed1 = PREHEAT_1_FAN_SPEED;
    preheatHotendTemp2 = PREHEAT_2_TEMP_HOTEND;
    preheatBedTemp2 = PREHEAT_2_TEMP_BED;
    preheatFanSpeed2 = PREHEAT_2_FAN_SPEED;
  #endif

  #if HAS_LCD_CONTRAST
    lcd_contrast = DEFAULT_LCD_CONTRAST;
  #endif

  #if ENABLED(PIDTEMP)
    #if ENABLED(PID_PARAMS_PER_HOTEND) && HOTENDS > 1
      HOTEND_LOOP()
    #else
      int e = 0; UNUSED(e); // only need to write once
    #endif
    {
      PID_PARAM(Kp, e) = DEFAULT_Kp;
      PID_PARAM(Ki, e) = scalePID_i(DEFAULT_Ki);
      PID_PARAM(Kd, e) = scalePID_d(DEFAULT_Kd);
      #if ENABLED(PID_EXTRUSION_SCALING)
        PID_PARAM(Kc, e) = DEFAULT_Kc;
      #endif
    }
    #if ENABLED(PID_EXTRUSION_SCALING)
      lpq_len = 20; // default last-position-queue size
    #endif
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
    retract_feedrate_mm_s = RETRACT_FEEDRATE;
    retract_zlift = RETRACT_ZLIFT;
    retract_recover_length = RETRACT_RECOVER_LENGTH;
    #if EXTRUDERS > 1
      retract_recover_length_swap = RETRACT_RECOVER_LENGTH_SWAP;
    #endif
    retract_recover_feedrate_mm_s = RETRACT_RECOVER_FEEDRATE;
  #endif

  volumetric_enabled = false;
  for (uint8_t q = 0; q < COUNT(filament_size); q++)
    filament_size[q] = DEFAULT_NOMINAL_FILAMENT_DIA;

  endstops.enable_globally(
    #if ENABLED(ENDSTOPS_ALWAYS_ON_DEFAULT)
      (true)
    #else
      (false)
    #endif
  );

  Config_Postprocess();

  SERIAL_ECHO_START;
  SERIAL_ECHOLNPGM("Hardcoded Default Settings Loaded");
}

#if DISABLED(DISABLE_M503)

#define CONFIG_ECHO_START do{ if (!forReplay) SERIAL_ECHO_START; }while(0)

/**
 * M503 - Print Configuration
 */
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
  SERIAL_ECHOPAIR("  M203 X", planner.max_feedrate_mm_s[X_AXIS]);
  SERIAL_ECHOPAIR(" Y", planner.max_feedrate_mm_s[Y_AXIS]);
  SERIAL_ECHOPAIR(" Z", planner.max_feedrate_mm_s[Z_AXIS]);
  SERIAL_ECHOPAIR(" E", planner.max_feedrate_mm_s[E_AXIS]);
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
  SERIAL_ECHOPAIR("  M205 S", planner.min_feedrate_mm_s);
  SERIAL_ECHOPAIR(" T", planner.min_travel_feedrate_mm_s);
  SERIAL_ECHOPAIR(" B", planner.min_segment_time);
  SERIAL_ECHOPAIR(" X", planner.max_xy_jerk);
  SERIAL_ECHOPAIR(" Z", planner.max_z_jerk);
  SERIAL_ECHOPAIR(" E", planner.max_e_jerk);
  SERIAL_EOL;

  CONFIG_ECHO_START;
  if (!forReplay) {
    SERIAL_ECHOLNPGM("Home offset (mm)");
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
    SERIAL_ECHOPAIR("  M145 S0 H", preheatHotendTemp1);
    SERIAL_ECHOPAIR(" B", preheatBedTemp1);
    SERIAL_ECHOPAIR(" F", preheatFanSpeed1);
    SERIAL_EOL;
    CONFIG_ECHO_START;
    SERIAL_ECHOPAIR("  M145 S1 H", preheatHotendTemp2);
    SERIAL_ECHOPAIR(" B", preheatBedTemp2);
    SERIAL_ECHOPAIR(" F", preheatFanSpeed2);
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
          HOTEND_LOOP() {
            CONFIG_ECHO_START;
            SERIAL_ECHOPAIR("  M301 E", e);
            SERIAL_ECHOPAIR(" P", PID_PARAM(Kp, e));
            SERIAL_ECHOPAIR(" I", unscalePID_i(PID_PARAM(Ki, e)));
            SERIAL_ECHOPAIR(" D", unscalePID_d(PID_PARAM(Kd, e)));
            #if ENABLED(PID_EXTRUSION_SCALING)
              SERIAL_ECHOPAIR(" C", PID_PARAM(Kc, e));
              if (e == 0) SERIAL_ECHOPAIR(" L", lpq_len);
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
        #if ENABLED(PID_EXTRUSION_SCALING)
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
    SERIAL_ECHOPAIR(" F", MMS_TO_MMM(retract_feedrate_mm_s));
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
    SERIAL_ECHOPAIR(" F", MMS_TO_MMM(retract_recover_feedrate_mm_s));
    SERIAL_EOL;
    CONFIG_ECHO_START;
    if (!forReplay) {
      SERIAL_ECHOLNPGM("Auto-Retract: S=0 to disable, 1 to interpret extrude-only moves as retracts or recoveries");
      CONFIG_ECHO_START;
    }
    SERIAL_ECHOPAIR("  M209 S", autoretract_enabled ? 1 : 0);
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
  #if HAS_BED_PROBE
    if (!forReplay) {
      CONFIG_ECHO_START;
      SERIAL_ECHOLNPGM("Z-Probe Offset (mm):");
    }
    CONFIG_ECHO_START;
    SERIAL_ECHOPAIR("  M851 Z", zprobe_zoffset);
    SERIAL_EOL;
  #endif
}

#endif // !DISABLE_M503
