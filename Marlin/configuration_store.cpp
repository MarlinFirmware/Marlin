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

#define EEPROM_VERSION "V29"

// Change EEPROM version if these are changed:
#define EEPROM_OFFSET 100

/**
 * V29 EEPROM Layout:
 *
 *  100  Version                                   (char x4)
 *  104  EEPROM Checksum                           (uint16_t)
 *
 *  106            E_STEPPERS (uint8_t)
 *  107  M92 XYZE  planner.axis_steps_per_mm (float x4 ... x7)
 *  123  M203 XYZE planner.max_feedrate_mm_s (float x4 ... x7)
 *  139  M201 XYZE planner.max_acceleration_mm_per_s2 (uint32_t x4 ... x7)
 *  155  M204 P    planner.acceleration (float)
 *  159  M204 R    planner.retract_acceleration (float)
 *  163  M204 T    planner.travel_acceleration (float)
 *  167  M205 S    planner.min_feedrate_mm_s (float)
 *  171  M205 T    planner.min_travel_feedrate_mm_s (float)
 *  175  M205 B    planner.min_segment_time (ulong)
 *  179  M205 X    planner.max_jerk[X_AXIS] (float)
 *  183  M205 Y    planner.max_jerk[Y_AXIS] (float)
 *  187  M205 Z    planner.max_jerk[Z_AXIS] (float)
 *  191  M205 E    planner.max_jerk[E_AXIS] (float)
 *  195  M206 XYZ  home_offset (float x3)
 *  207  M218 XYZ  hotend_offset (float x3 per additional hotend)
 *
 * Mesh bed leveling:
 *  219  M420 S    from mbl.status (bool)
 *  220            mbl.z_offset (float)
 *  224            MESH_NUM_X_POINTS (uint8 as set in firmware)
 *  225            MESH_NUM_Y_POINTS (uint8 as set in firmware)
 *  226 G29 S3 XYZ z_values[][] (float x9, by default, up to float x 81) +288
 *
 * AUTO BED LEVELING
 *  262  M851      zprobe_zoffset (float)
 *
 * ABL_PLANAR (or placeholder):                    36 bytes
 *  266            planner.bed_level_matrix        (matrix_3x3 = float x9)
 *
 * AUTO_BED_LEVELING_BILINEAR (or placeholder):    47 bytes
 *  302            ABL_GRID_MAX_POINTS_X           (uint8_t)
 *  303            ABL_GRID_MAX_POINTS_Y           (uint8_t)
 *  304            bilinear_grid_spacing           (int x2)   from G29: (B-F)/X, (R-L)/Y
 *  308  G29 L F   bilinear_start                  (int x2)
 *  312            bed_level_grid[][]              (float x9, up to float x256) +988
 *
 * DELTA (if deltabot):                            36 bytes
 *  348  M666 XYZ  endstop_adj                     (float x3)
 *  360  M665 R    delta_radius                    (float)
 *  364  M665 L    delta_diagonal_rod              (float)
 *  368  M665 S    delta_segments_per_second       (float)
 *  372  M665 A    delta_diagonal_rod_trim_tower_1 (float)
 *  376  M665 B    delta_diagonal_rod_trim_tower_2 (float)
 *  380  M665 C    delta_diagonal_rod_trim_tower_3 (float)
 *
 * Z_DUAL_ENDSTOPS:                                4 bytes
 *  384  M666 Z    z_endstop_adj                   (float)
 *
 * ULTIPANEL:                                      6 bytes
 *  388  M145 S0 H lcd_preheat_hotend_temp         (int x2)
 *  392  M145 S0 B lcd_preheat_bed_temp            (int x2)
 *  396  M145 S0 F lcd_preheat_fan_speed           (int x2)
 *
 * PIDTEMP:                                        66 bytes
 *  400  M301 E0 PIDC  Kp[0], Ki[0], Kd[0], Kc[0]  (float x4)
 *  416  M301 E1 PIDC  Kp[1], Ki[1], Kd[1], Kc[1]  (float x4)
 *  432  M301 E2 PIDC  Kp[2], Ki[2], Kd[2], Kc[2]  (float x4)
 *  448  M301 E3 PIDC  Kp[3], Ki[3], Kd[3], Kc[3]  (float x4)
 *  464  M301 L        lpq_len                     (int)
 *
 * PIDTEMPBED:
 *  466  M304 PID  thermalManager.bedKp, thermalManager.bedKi, thermalManager.bedKd (float x3)
 *
 * DOGLCD:                                          2 bytes
 *  478  M250 C    lcd_contrast                     (int)
 *
 * FWRETRACT:                                       29 bytes
 *  480  M209 S    autoretract_enabled              (bool)
 *  481  M207 S    retract_length                   (float)
 *  485  M207 W    retract_length_swap              (float)
 *  489  M207 F    retract_feedrate_mm_s            (float)
 *  493  M207 Z    retract_zlift                    (float)
 *  497  M208 S    retract_recover_length           (float)
 *  501  M208 W    retract_recover_length_swap      (float)
 *  505  M208 F    retract_recover_feedrate_mm_s    (float)
 *
 * Volumetric Extrusion:                            17 bytes
 *  509  M200 D    volumetric_enabled               (bool)
 *  510  M200 T D  filament_size                    (float x4) (T0..3)
 *
 *  526                                Minimum end-point
 * 1847 (526 + 36 + 9 + 288 + 988)     Maximum end-point
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

#if ENABLED(ABL_BILINEAR_SUBDIVISION)
  extern void bed_level_virt_interpolate();
#endif

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

  // Software endstops depend on home_offset
  LOOP_XYZ(i) update_software_endstops((AxisEnum)i);
}

#if ENABLED(EEPROM_SETTINGS)

  uint16_t eeprom_checksum;
  const char version[4] = EEPROM_VERSION;

  bool eeprom_write_error;

  void _EEPROM_writeData(int &pos, const uint8_t* value, uint16_t size) {
    if (eeprom_write_error) return;
    while (size--) {
      uint8_t * const p = (uint8_t * const)pos;
      const uint8_t v = *value;
      // EEPROM has only ~100,000 write cycles,
      // so only write bytes that have changed!
      if (v != eeprom_read_byte(p)) {
        eeprom_write_byte(p, v);
        if (eeprom_read_byte(p) != v) {
          SERIAL_ECHO_START;
          SERIAL_ECHOLNPGM(MSG_ERR_EEPROM_WRITE);
          eeprom_write_error = true;
          return;
        }
      }
      eeprom_checksum += v;
      pos++;
      value++;
    };
  }
  bool eeprom_read_error;
  void _EEPROM_readData(int &pos, uint8_t* value, uint16_t size) {
    do {
      uint8_t c = eeprom_read_byte((unsigned char*)pos);
      if (!eeprom_read_error) *value = c;
      eeprom_checksum += c;
      pos++;
      value++;
    } while (--size);
  }

  #define DUMMY_PID_VALUE 3000.0f
  #define EEPROM_START() int eeprom_index = EEPROM_OFFSET
  #define EEPROM_SKIP(VAR) eeprom_index += sizeof(VAR)
  #define EEPROM_WRITE(VAR) _EEPROM_writeData(eeprom_index, (uint8_t*)&VAR, sizeof(VAR))
  #define EEPROM_READ(VAR) _EEPROM_readData(eeprom_index, (uint8_t*)&VAR, sizeof(VAR))
  #define EEPROM_ASSERT(TST,ERR) if () do{ SERIAL_ERROR_START; SERIAL_ERRORLNPGM(ERR); eeprom_read_error |= true; }while(0)

  /**
   * M500 - Store Configuration
   */
  void Config_StoreSettings()  {
    float dummy = 0.0f;
    char ver[4] = "000";

    EEPROM_START();

    eeprom_write_error = false;

    EEPROM_WRITE(ver);     // invalidate data first
    EEPROM_SKIP(eeprom_checksum); // Skip the checksum slot

    eeprom_checksum = 0; // clear before first "real data"

    const uint8_t esteppers = COUNT(planner.axis_steps_per_mm) - XYZ;
    EEPROM_WRITE(esteppers);

    EEPROM_WRITE(planner.axis_steps_per_mm);
    EEPROM_WRITE(planner.max_feedrate_mm_s);
    EEPROM_WRITE(planner.max_acceleration_mm_per_s2);

    EEPROM_WRITE(planner.acceleration);
    EEPROM_WRITE(planner.retract_acceleration);
    EEPROM_WRITE(planner.travel_acceleration);
    EEPROM_WRITE(planner.min_feedrate_mm_s);
    EEPROM_WRITE(planner.min_travel_feedrate_mm_s);
    EEPROM_WRITE(planner.min_segment_time);
    EEPROM_WRITE(planner.max_jerk);
    EEPROM_WRITE(home_offset);

    #if HOTENDS > 1
      // Skip hotend 0 which must be 0
      for (uint8_t e = 1; e < HOTENDS; e++)
        LOOP_XYZ(i) EEPROM_WRITE(hotend_offset[i][e]);
    #endif

    //
    // Mesh Bed Leveling
    //

    #if ENABLED(MESH_BED_LEVELING)
      // Compile time test that sizeof(mbl.z_values) is as expected
      typedef char c_assert[(sizeof(mbl.z_values) == (MESH_NUM_X_POINTS) * (MESH_NUM_Y_POINTS) * sizeof(dummy)) ? 1 : -1];
      const bool leveling_is_on = TEST(mbl.status, MBL_STATUS_HAS_MESH_BIT);
      const uint8_t mesh_num_x = MESH_NUM_X_POINTS, mesh_num_y = MESH_NUM_Y_POINTS;
      EEPROM_WRITE(leveling_is_on);
      EEPROM_WRITE(mbl.z_offset);
      EEPROM_WRITE(mesh_num_x);
      EEPROM_WRITE(mesh_num_y);
      EEPROM_WRITE(mbl.z_values);
    #else
      // For disabled MBL write a default mesh
      const bool leveling_is_on = false;
      dummy = 0.0f;
      const uint8_t mesh_num_x = 3, mesh_num_y = 3;
      EEPROM_WRITE(leveling_is_on);
      EEPROM_WRITE(dummy); // z_offset
      EEPROM_WRITE(mesh_num_x);
      EEPROM_WRITE(mesh_num_y);
      for (uint8_t q = mesh_num_x * mesh_num_y; q--;) EEPROM_WRITE(dummy);
    #endif // MESH_BED_LEVELING

    #if !HAS_BED_PROBE
      float zprobe_zoffset = 0;
    #endif
    EEPROM_WRITE(zprobe_zoffset);

    //
    // Planar Bed Leveling matrix
    //

    #if ABL_PLANAR
      EEPROM_WRITE(planner.bed_level_matrix);
    #else
      dummy = 0.0;
      for (uint8_t q = 9; q--;) EEPROM_WRITE(dummy);
    #endif

    //
    // Bilinear Auto Bed Leveling
    //

    #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
      // Compile time test that sizeof(bed_level_grid) is as expected
      typedef char c_assert[(sizeof(bed_level_grid) == (ABL_GRID_MAX_POINTS_X) * (ABL_GRID_MAX_POINTS_Y) * sizeof(dummy)) ? 1 : -1];
      const uint8_t grid_max_x = ABL_GRID_MAX_POINTS_X, grid_max_y = ABL_GRID_MAX_POINTS_Y;
      EEPROM_WRITE(grid_max_x);            // 1 byte
      EEPROM_WRITE(grid_max_y);            // 1 byte
      EEPROM_WRITE(bilinear_grid_spacing); // 2 ints
      EEPROM_WRITE(bilinear_start);        // 2 ints
      EEPROM_WRITE(bed_level_grid);        // 9-256 floats
    #else
      // For disabled Bilinear Grid write an empty 3x3 grid
      const uint8_t grid_max_x = 3, grid_max_y = 3;
      const int bilinear_start[2] = { 0 }, bilinear_grid_spacing[2] = { 0 };
      dummy = 0.0f;
      EEPROM_WRITE(grid_max_x);
      EEPROM_WRITE(grid_max_y);
      EEPROM_WRITE(bilinear_grid_spacing);
      EEPROM_WRITE(bilinear_start);
      for (uint16_t q = grid_max_x * grid_max_y; q--;) EEPROM_WRITE(dummy);
    #endif // AUTO_BED_LEVELING_BILINEAR

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
      const int lcd_preheat_hotend_temp[2] = { PREHEAT_1_TEMP_HOTEND, PREHEAT_2_TEMP_HOTEND },
                lcd_preheat_bed_temp[2] = { PREHEAT_1_TEMP_BED, PREHEAT_2_TEMP_BED },
                lcd_preheat_fan_speed[2] = { PREHEAT_1_FAN_SPEED, PREHEAT_2_FAN_SPEED };
    #endif // !ULTIPANEL

    EEPROM_WRITE(lcd_preheat_hotend_temp);
    EEPROM_WRITE(lcd_preheat_bed_temp);
    EEPROM_WRITE(lcd_preheat_fan_speed);

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

    if (!eeprom_write_error) {

      uint16_t final_checksum = eeprom_checksum,
               eeprom_size = eeprom_index;

      // Write the EEPROM header
      eeprom_index = EEPROM_OFFSET;
      EEPROM_WRITE(version);
      EEPROM_WRITE(final_checksum);

      // Report storage size
      SERIAL_ECHO_START;
      SERIAL_ECHOPAIR("Settings Stored (", eeprom_size - (EEPROM_OFFSET));
      SERIAL_ECHOLNPGM(" bytes)");
    }
  }

  /**
   * M501 - Retrieve Configuration
   */
  void Config_RetrieveSettings() {

    EEPROM_START();
    eeprom_read_error = false; // If set EEPROM_READ won't write into RAM

    char stored_ver[4];
    EEPROM_READ(stored_ver);

    uint16_t stored_checksum;
    EEPROM_READ(stored_checksum);

    //  SERIAL_ECHOPAIR("Version: [", version);
    //  SERIAL_ECHOPAIR("] Stored version: [", stored_ver);
    //  SERIAL_CHAR(']');
    //  SERIAL_EOL;

    // Version has to match or defaults are used
    if (strncmp(version, stored_ver, 3) != 0) {
      Config_ResetDefault();
    }
    else {
      float dummy = 0;

      eeprom_checksum = 0; // clear before reading first "real data"

      // Number of esteppers may change
      uint8_t esteppers;
      EEPROM_READ(esteppers);

      // Get only the number of E stepper parameters previously stored
      // Any steppers added later are set to their defaults
      const float def1[] = DEFAULT_AXIS_STEPS_PER_UNIT, def2[] = DEFAULT_MAX_FEEDRATE;
      const uint32_t def3[] = DEFAULT_MAX_ACCELERATION;
      float tmp1[XYZ + esteppers], tmp2[XYZ + esteppers];
      uint32_t tmp3[XYZ + esteppers];
      EEPROM_READ(tmp1);
      EEPROM_READ(tmp2);
      EEPROM_READ(tmp3);
      LOOP_XYZE_N(i) {
        planner.axis_steps_per_mm[i]          = i < XYZ + esteppers ? tmp1[i] : def1[i < COUNT(def1) ? i : COUNT(def1) - 1];
        planner.max_feedrate_mm_s[i]          = i < XYZ + esteppers ? tmp2[i] : def2[i < COUNT(def2) ? i : COUNT(def2) - 1];
        planner.max_acceleration_mm_per_s2[i] = i < XYZ + esteppers ? tmp3[i] : def3[i < COUNT(def3) ? i : COUNT(def3) - 1];
      }

      EEPROM_READ(planner.acceleration);
      EEPROM_READ(planner.retract_acceleration);
      EEPROM_READ(planner.travel_acceleration);
      EEPROM_READ(planner.min_feedrate_mm_s);
      EEPROM_READ(planner.min_travel_feedrate_mm_s);
      EEPROM_READ(planner.min_segment_time);
      EEPROM_READ(planner.max_jerk);
      EEPROM_READ(home_offset);

      #if HOTENDS > 1
        // Skip hotend 0 which must be 0
        for (uint8_t e = 1; e < HOTENDS; e++)
          LOOP_XYZ(i) EEPROM_READ(hotend_offset[i][e]);
      #endif

      //
      // Mesh (Manual) Bed Leveling
      //

      bool leveling_is_on;
      uint8_t mesh_num_x, mesh_num_y;
      EEPROM_READ(leveling_is_on);
      EEPROM_READ(dummy);
      EEPROM_READ(mesh_num_x);
      EEPROM_READ(mesh_num_y);

      #if ENABLED(MESH_BED_LEVELING)
        mbl.status = leveling_is_on ? _BV(MBL_STATUS_HAS_MESH_BIT) : 0;
        mbl.z_offset = dummy;
        if (mesh_num_x == MESH_NUM_X_POINTS && mesh_num_y == MESH_NUM_Y_POINTS) {
          // EEPROM data fits the current mesh
          EEPROM_READ(mbl.z_values);
        }
        else {
          // EEPROM data is stale
          mbl.reset();
          for (uint16_t q = mesh_num_x * mesh_num_y; q--;) EEPROM_READ(dummy);
        }
      #else
        // MBL is disabled - skip the stored data
        for (uint16_t q = mesh_num_x * mesh_num_y; q--;) EEPROM_READ(dummy);
      #endif // MESH_BED_LEVELING

      #if !HAS_BED_PROBE
        float zprobe_zoffset = 0;
      #endif
      EEPROM_READ(zprobe_zoffset);

      //
      // Planar Bed Leveling matrix
      //

      #if ABL_PLANAR
        EEPROM_READ(planner.bed_level_matrix);
      #else
        for (uint8_t q = 9; q--;) EEPROM_READ(dummy);
      #endif

      //
      // Bilinear Auto Bed Leveling
      //

      uint8_t grid_max_x, grid_max_y;
      EEPROM_READ(grid_max_x);                       // 1 byte
      EEPROM_READ(grid_max_y);                       // 1 byte
      #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
        if (grid_max_x == ABL_GRID_MAX_POINTS_X && grid_max_y == ABL_GRID_MAX_POINTS_Y) {
          set_bed_leveling_enabled(false);
          EEPROM_READ(bilinear_grid_spacing);        // 2 ints
          EEPROM_READ(bilinear_start);               // 2 ints
          EEPROM_READ(bed_level_grid);               // 9 to 256 floats
          #if ENABLED(ABL_BILINEAR_SUBDIVISION)
            bed_level_virt_interpolate();
          #endif
          //set_bed_leveling_enabled(leveling_is_on);
        }
        else // EEPROM data is stale
      #endif // AUTO_BED_LEVELING_BILINEAR
        {
          // Skip past disabled (or stale) Bilinear Grid data
          int bgs[2], bs[2];
          EEPROM_READ(bgs);
          EEPROM_READ(bs);
          for (uint16_t q = grid_max_x * grid_max_y; q--;) EEPROM_READ(dummy);
        }

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
        int lcd_preheat_hotend_temp[2], lcd_preheat_bed_temp[2], lcd_preheat_fan_speed[2];
      #endif

      EEPROM_READ(lcd_preheat_hotend_temp);
      EEPROM_READ(lcd_preheat_bed_temp);
      EEPROM_READ(lcd_preheat_fan_speed);

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
        if (eeprom_read_error)
          Config_ResetDefault();
        else {
          Config_Postprocess();
          SERIAL_ECHO_START;
          SERIAL_ECHO(version);
          SERIAL_ECHOPAIR(" stored settings retrieved (", eeprom_index - (EEPROM_OFFSET));
          SERIAL_ECHOLNPGM(" bytes)");
        }
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

#else // !EEPROM_SETTINGS

  void Config_StoreSettings() {
    SERIAL_ERROR_START;
    SERIAL_ERRORLNPGM("EEPROM disabled");
  }

#endif // !EEPROM_SETTINGS

/**
 * M502 - Reset Configuration
 */
void Config_ResetDefault() {
  const float tmp1[] = DEFAULT_AXIS_STEPS_PER_UNIT, tmp2[] = DEFAULT_MAX_FEEDRATE;
  const uint32_t tmp3[] = DEFAULT_MAX_ACCELERATION;
  LOOP_XYZE_N(i) {
    planner.axis_steps_per_mm[i]          = tmp1[i < COUNT(tmp1) ? i : COUNT(tmp1) - 1];
    planner.max_feedrate_mm_s[i]          = tmp2[i < COUNT(tmp2) ? i : COUNT(tmp2) - 1];
    planner.max_acceleration_mm_per_s2[i] = tmp3[i < COUNT(tmp3) ? i : COUNT(tmp3) - 1];
  }

  planner.acceleration = DEFAULT_ACCELERATION;
  planner.retract_acceleration = DEFAULT_RETRACT_ACCELERATION;
  planner.travel_acceleration = DEFAULT_TRAVEL_ACCELERATION;
  planner.min_feedrate_mm_s = DEFAULT_MINIMUMFEEDRATE;
  planner.min_segment_time = DEFAULT_MINSEGMENTTIME;
  planner.min_travel_feedrate_mm_s = DEFAULT_MINTRAVELFEEDRATE;
  planner.max_jerk[X_AXIS] = DEFAULT_XJERK;
  planner.max_jerk[Y_AXIS] = DEFAULT_YJERK;
  planner.max_jerk[Z_AXIS] = DEFAULT_ZJERK;
  planner.max_jerk[E_AXIS] = DEFAULT_EJERK;
  home_offset[X_AXIS] = home_offset[Y_AXIS] = home_offset[Z_AXIS] = 0;

  #if HOTENDS > 1
    constexpr float tmp4[XYZ][HOTENDS] = {
      HOTEND_OFFSET_X,
      HOTEND_OFFSET_Y
      #ifdef HOTEND_OFFSET_Z
        , HOTEND_OFFSET_Z
      #else
        , { 0 }
      #endif
    };
    static_assert(
      tmp4[X_AXIS][0] == 0 && tmp4[Y_AXIS][0] == 0 && tmp4[Z_AXIS][0] == 0,
      "Offsets for the first hotend must be 0.0."
    );
    LOOP_XYZ(i) HOTEND_LOOP() hotend_offset[i][e] = tmp4[i][e];
  #endif

  // Applies to all MBL and ABL
  #if PLANNER_LEVELING
    reset_bed_level();
  #endif

  #if HAS_BED_PROBE
    zprobe_zoffset = Z_PROBE_OFFSET_FROM_EXTRUDER;
  #endif

  #if ENABLED(DELTA)
    const float adj[ABC] = DELTA_ENDSTOP_ADJ;
    endstop_adj[A_AXIS] = adj[A_AXIS];
    endstop_adj[B_AXIS] = adj[B_AXIS];
    endstop_adj[C_AXIS] = adj[C_AXIS];
    delta_radius = DELTA_RADIUS;
    delta_diagonal_rod = DELTA_DIAGONAL_ROD;
    delta_segments_per_second = DELTA_SEGMENTS_PER_SECOND;
    delta_diagonal_rod_trim_tower_1 = DELTA_DIAGONAL_ROD_TRIM_TOWER_1;
    delta_diagonal_rod_trim_tower_2 = DELTA_DIAGONAL_ROD_TRIM_TOWER_2;
    delta_diagonal_rod_trim_tower_3 = DELTA_DIAGONAL_ROD_TRIM_TOWER_3;
  #elif ENABLED(Z_DUAL_ENDSTOPS)
    z_endstop_adj = 0;
  #endif

  #if ENABLED(ULTIPANEL)
    lcd_preheat_hotend_temp[0] = PREHEAT_1_TEMP_HOTEND;
    lcd_preheat_hotend_temp[1] = PREHEAT_2_TEMP_HOTEND;
    lcd_preheat_bed_temp[0] = PREHEAT_1_TEMP_BED;
    lcd_preheat_bed_temp[1] = PREHEAT_2_TEMP_BED;
    lcd_preheat_fan_speed[0] = PREHEAT_1_FAN_SPEED;
    lcd_preheat_fan_speed[1] = PREHEAT_2_FAN_SPEED;
  #endif

  #if HAS_LCD_CONTRAST
    lcd_contrast = DEFAULT_LCD_CONTRAST;
  #endif

  #if ENABLED(PIDTEMP)
    #if ENABLED(PID_PARAMS_PER_HOTEND) && HOTENDS > 1
      HOTEND_LOOP()
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

  volumetric_enabled =
  #if ENABLED(VOLUMETRIC_DEFAULT_ON)
    true
  #else
    false
  #endif
  ;
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
    #if DISABLED(DISTINCT_E_FACTORS)
      SERIAL_ECHOPAIR(" E", planner.axis_steps_per_mm[E_AXIS]);
    #endif
    SERIAL_EOL;
    #if ENABLED(DISTINCT_E_FACTORS)
      for (uint8_t i = 0; i < E_STEPPERS; i++) {
        SERIAL_ECHOPAIR("  M92 T", (int)i);
        SERIAL_ECHOLNPAIR(" E", planner.axis_steps_per_mm[E_AXIS + i]);
      }
    #endif

    CONFIG_ECHO_START;

    if (!forReplay) {
      SERIAL_ECHOLNPGM("Maximum feedrates (mm/s):");
      CONFIG_ECHO_START;
    }
    SERIAL_ECHOPAIR("  M203 X", planner.max_feedrate_mm_s[X_AXIS]);
    SERIAL_ECHOPAIR(" Y", planner.max_feedrate_mm_s[Y_AXIS]);
    SERIAL_ECHOPAIR(" Z", planner.max_feedrate_mm_s[Z_AXIS]);
    #if DISABLED(DISTINCT_E_FACTORS)
      SERIAL_ECHOPAIR(" E", planner.max_feedrate_mm_s[E_AXIS]);
    #endif
    SERIAL_EOL;
    #if ENABLED(DISTINCT_E_FACTORS)
      for (uint8_t i = 0; i < E_STEPPERS; i++) {
        SERIAL_ECHOPAIR("  M203 T", (int)i);
        SERIAL_ECHOLNPAIR(" E", planner.max_feedrate_mm_s[E_AXIS + i]);
      }
    #endif

    CONFIG_ECHO_START;
    if (!forReplay) {
      SERIAL_ECHOLNPGM("Maximum Acceleration (mm/s2):");
      CONFIG_ECHO_START;
    }
    SERIAL_ECHOPAIR("  M201 X", planner.max_acceleration_mm_per_s2[X_AXIS]);
    SERIAL_ECHOPAIR(" Y", planner.max_acceleration_mm_per_s2[Y_AXIS]);
    SERIAL_ECHOPAIR(" Z", planner.max_acceleration_mm_per_s2[Z_AXIS]);
    #if DISABLED(DISTINCT_E_FACTORS)
      SERIAL_ECHOPAIR(" E", planner.max_acceleration_mm_per_s2[E_AXIS]);
    #endif
    SERIAL_EOL;
    #if ENABLED(DISTINCT_E_FACTORS)
      for (uint8_t i = 0; i < E_STEPPERS; i++) {
        SERIAL_ECHOPAIR("  M201 T", (int)i);
        SERIAL_ECHOLNPAIR(" E", planner.max_acceleration_mm_per_s2[E_AXIS + i]);
      }
    #endif

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
    SERIAL_ECHOPAIR(" X", planner.max_jerk[X_AXIS]);
    SERIAL_ECHOPAIR(" Y", planner.max_jerk[Y_AXIS]);
    SERIAL_ECHOPAIR(" Z", planner.max_jerk[Z_AXIS]);
    SERIAL_ECHOPAIR(" E", planner.max_jerk[E_AXIS]);
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

    #if HOTENDS > 1
      CONFIG_ECHO_START;
      if (!forReplay) {
        SERIAL_ECHOLNPGM("Hotend offsets (mm)");
        CONFIG_ECHO_START;
      }
      for (uint8_t e = 1; e < HOTENDS; e++) {
        SERIAL_ECHOPAIR("  M218 T", (int)e);
        SERIAL_ECHOPAIR(" X", hotend_offset[X_AXIS][e]);
        SERIAL_ECHOPAIR(" Y", hotend_offset[Y_AXIS][e]);
        #if ENABLED(DUAL_X_CARRIAGE) || ENABLED(SWITCHING_EXTRUDER)
          SERIAL_ECHOPAIR(" Z", hotend_offset[Z_AXIS][e]);
        #endif
        SERIAL_EOL;
      }
    #endif

    #if ENABLED(MESH_BED_LEVELING)
      if (!forReplay) {
        SERIAL_ECHOLNPGM("Mesh Bed Leveling:");
        CONFIG_ECHO_START;
      }
      SERIAL_ECHOLNPAIR("  M420 S", mbl.has_mesh() ? 1 : 0);
      for (uint8_t py = 1; py <= MESH_NUM_Y_POINTS; py++) {
        for (uint8_t px = 1; px <= MESH_NUM_X_POINTS; px++) {
          CONFIG_ECHO_START;
          SERIAL_ECHOPAIR("  G29 S3 X", (int)px);
          SERIAL_ECHOPAIR(" Y", (int)py);
          SERIAL_ECHOPGM(" Z");
          SERIAL_PROTOCOL_F(mbl.z_values[py-1][px-1], 5);
          SERIAL_EOL;
        }
      }
    #elif HAS_ABL
      if (!forReplay) {
        SERIAL_ECHOLNPGM("Auto Bed Leveling:");
        CONFIG_ECHO_START;
      }
      SERIAL_ECHOLNPAIR("  M420 S", planner.abl_enabled ? 1 : 0);
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
      for (uint8_t i = 0; i < COUNT(lcd_preheat_hotend_temp); i++) {
        SERIAL_ECHOPAIR("  M145 S", (int)i);
        SERIAL_ECHOPAIR(" H", lcd_preheat_hotend_temp[i]);
        SERIAL_ECHOPAIR(" B", lcd_preheat_bed_temp[i]);
        SERIAL_ECHOPAIR(" F", lcd_preheat_fan_speed[i]);
        SERIAL_EOL;
      }
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
