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

#define EEPROM_VERSION "V31"

// Change EEPROM version if these are changed:
#define EEPROM_OFFSET 100

/**
 * V31 EEPROM Layout:
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
 *  224            GRID_MAX_POINTS_X (uint8 as set in firmware)
 *  225            GRID_MAX_POINTS_Y (uint8 as set in firmware)
 *  226 G29 S3 XYZ z_values[][] (float x9, by default, up to float x 81) +288
 *
 * AUTO BED LEVELING
 *  262  M851      zprobe_zoffset (float)
 *
 * ABL_PLANAR (or placeholder):                    36 bytes
 *  266            planner.bed_level_matrix        (matrix_3x3 = float x9)
 *
 * AUTO_BED_LEVELING_BILINEAR (or placeholder):    47 bytes
 *  302            GRID_MAX_POINTS_X               (uint8_t)
 *  303            GRID_MAX_POINTS_Y               (uint8_t)
 *  304            bilinear_grid_spacing           (int x2)   from G29: (B-F)/X, (R-L)/Y
 *  308  G29 L F   bilinear_start                  (int x2)
 *  312            bed_level_grid[][]              (float x9, up to float x256) +988
 *
 * DELTA (if deltabot):                             48 bytes
 *  348  M666 XYZ  endstop_adj                      (float x3)
 *  360  M665 R    delta_radius                     (float)
 *  364  M665 L    delta_diagonal_rod               (float)
 *  368  M665 S    delta_segments_per_second        (float)
 *  372  M665 A    delta_diagonal_rod_trim[A]       (float)
 *  376  M665 B    delta_diagonal_rod_trim[B]       (float)
 *  380  M665 C    delta_diagonal_rod_trim[C]       (float)
 *  384  M665 I    delta_tower_angle_trim[A]        (float)
 *  388  M665 J    delta_tower_angle_trim[B]        (float)
 *  392  M665 K    delta_tower_angle_trim[C]        (float)
 *
 * Z_DUAL_ENDSTOPS (if not deltabot):              48 bytes
 *  348  M666 Z    z_endstop_adj                   (float)
 *  ---            dummy data                      (float x11)
 *
 * ULTIPANEL:                                      6 bytes
 *  396  M145 S0 H lcd_preheat_hotend_temp         (int x2)
 *  400  M145 S0 B lcd_preheat_bed_temp            (int x2)
 *  404  M145 S0 F lcd_preheat_fan_speed           (int x2)
 *
 * PIDTEMP:                                        66 bytes
 *  408  M301 E0 PIDC  Kp[0], Ki[0], Kd[0], Kc[0]  (float x4)
 *  424  M301 E1 PIDC  Kp[1], Ki[1], Kd[1], Kc[1]  (float x4)
 *  440  M301 E2 PIDC  Kp[2], Ki[2], Kd[2], Kc[2]  (float x4)
 *  456  M301 E3 PIDC  Kp[3], Ki[3], Kd[3], Kc[3]  (float x4)
 *  472  M301 L        lpq_len                     (int)
 *
 * PIDTEMPBED:                                      12 bytes
 *  474  M304 PID  thermalManager.bedKp, .bedKi, .bedKd (float x3)
 *
 * DOGLCD:                                          2 bytes
 *  486  M250 C    lcd_contrast                     (int)
 *
 * FWRETRACT:                                       29 bytes
 *  488  M209 S    autoretract_enabled              (bool)
 *  489  M207 S    retract_length                   (float)
 *  493  M207 W    retract_length_swap              (float)
 *  497  M207 F    retract_feedrate_mm_s            (float)
 *  501  M207 Z    retract_zlift                    (float)
 *  505  M208 S    retract_recover_length           (float)
 *  509  M208 W    retract_recover_length_swap      (float)
 *  513  M208 F    retract_recover_feedrate_mm_s    (float)
 *
 * Volumetric Extrusion:                            17 bytes
 *  517  M200 D    volumetric_enabled               (bool)
 *  518  M200 T D  filament_size                    (float x4) (T0..3)
 *
 * TMC2130 Stepper Current:                         20 bytes
 *  534  M906 X    stepperX current                 (uint16_t)
 *  536  M906 Y    stepperY current                 (uint16_t)
 *  538  M906 Z    stepperZ current                 (uint16_t)
 *  540  M906 X2   stepperX2 current                (uint16_t)
 *  542  M906 Y2   stepperY2 current                (uint16_t)
 *  544  M906 Z2   stepperZ2 current                (uint16_t)
 *  546  M906 E0   stepperE0 current                (uint16_t)
 *  548  M906 E1   stepperE1 current                (uint16_t)
 *  550  M906 E2   stepperE2 current                (uint16_t)
 *  552  M906 E3   stepperE3 current                (uint16_t)
 *
 *  554                                Minimum end-point
 * 1875 (554 + 36 + 9 + 288 + 988)     Maximum end-point
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

#if ENABLED(HAVE_TMC2130)
  #include "stepper_indirection.h"
#endif

#if ENABLED(AUTO_BED_LEVELING_UBL)
  #include "ubl.h"
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

  #if DISABLED(NO_WORKSPACE_OFFSETS) || ENABLED(DUAL_X_CARRIAGE) || ENABLED(DELTA)
    // Software endstops depend on home_offset
    LOOP_XYZ(i) update_software_endstops((AxisEnum)i);
  #endif
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
  bool Config_StoreSettings() {
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
    #if ENABLED(NO_WORKSPACE_OFFSETS)
      float home_offset[XYZ] = { 0 };
    #endif
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
      typedef char c_assert[(sizeof(mbl.z_values) == (GRID_MAX_POINTS_X) * (GRID_MAX_POINTS_Y) * sizeof(dummy)) ? 1 : -1];
      const bool leveling_is_on = TEST(mbl.status, MBL_STATUS_HAS_MESH_BIT);
      const uint8_t mesh_num_x = GRID_MAX_POINTS_X, mesh_num_y = GRID_MAX_POINTS_Y;
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
      typedef char c_assert[(sizeof(bed_level_grid) == (GRID_MAX_POINTS_X) * (GRID_MAX_POINTS_Y) * sizeof(dummy)) ? 1 : -1];
      const uint8_t grid_max_x = GRID_MAX_POINTS_X, grid_max_y = GRID_MAX_POINTS_Y;
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
      EEPROM_WRITE(delta_diagonal_rod_trim);   // 3 floats
      EEPROM_WRITE(delta_tower_angle_trim);    // 3 floats
    #elif ENABLED(Z_DUAL_ENDSTOPS)
      EEPROM_WRITE(z_endstop_adj);             // 1 float
      dummy = 0.0f;
      for (uint8_t q = 11; q--;) EEPROM_WRITE(dummy);
    #else
      dummy = 0.0f;
      for (uint8_t q = 12; q--;) EEPROM_WRITE(dummy);
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

    // Save TCM2130 Configuration, and placeholder values
    uint16_t val;
    #if ENABLED(HAVE_TMC2130)
      #if ENABLED(X_IS_TMC2130)
        val = stepperX.getCurrent();
      #else
        val = 0;
      #endif
      EEPROM_WRITE(val);
      #if ENABLED(Y_IS_TMC2130)
        val = stepperY.getCurrent();
      #else
        val = 0;
      #endif
      EEPROM_WRITE(val);
      #if ENABLED(Z_IS_TMC2130)
        val = stepperZ.getCurrent();
      #else
        val = 0;
      #endif
      EEPROM_WRITE(val);
      #if ENABLED(X2_IS_TMC2130)
        val = stepperX2.getCurrent();
      #else
        val = 0;
      #endif
      EEPROM_WRITE(val);
      #if ENABLED(Y2_IS_TMC2130)
        val = stepperY2.getCurrent();
      #else
        val = 0;
      #endif
      EEPROM_WRITE(val);
      #if ENABLED(Z2_IS_TMC2130)
        val = stepperZ2.getCurrent();
      #else
        val = 0;
      #endif
      EEPROM_WRITE(val);
      #if ENABLED(E0_IS_TMC2130)
        val = stepperE0.getCurrent();
      #else
        val = 0;
      #endif
      EEPROM_WRITE(val);
      #if ENABLED(E1_IS_TMC2130)
        val = stepperE1.getCurrent();
      #else
        val = 0;
      #endif
      EEPROM_WRITE(val);
      #if ENABLED(E2_IS_TMC2130)
        val = stepperE2.getCurrent();
      #else
        val = 0;
      #endif
      EEPROM_WRITE(val);
      #if ENABLED(E3_IS_TMC2130)
        val = stepperE3.getCurrent();
      #else
        val = 0;
      #endif
      EEPROM_WRITE(val);
    #else
      val = 0;
      for (uint8_t q = 0; q < 10; ++q) EEPROM_WRITE(val);
    #endif

    if (!eeprom_write_error) {

      const uint16_t final_checksum = eeprom_checksum,
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

    #if ENABLED(AUTO_BED_LEVELING_UBL)
      ubl.store_state();
      if (ubl.state.eeprom_storage_slot >= 0)
        ubl.store_mesh(ubl.state.eeprom_storage_slot);
    #endif

    return !eeprom_write_error;
  }

  /**
   * M501 - Retrieve Configuration
   */
  bool Config_RetrieveSettings() {

    EEPROM_START();
    eeprom_read_error = false; // If set EEPROM_READ won't write into RAM

    char stored_ver[4];
    EEPROM_READ(stored_ver);

    uint16_t stored_checksum;
    EEPROM_READ(stored_checksum);

    // Version has to match or defaults are used
    if (strncmp(version, stored_ver, 3) != 0) {
      if (stored_ver[0] != 'V') {
        stored_ver[0] = '?';
        stored_ver[1] = '\0';
      }
      SERIAL_ECHO_START;
      SERIAL_ECHOPGM("EEPROM version mismatch ");
      SERIAL_ECHOPAIR("(EEPROM=", stored_ver);
      SERIAL_ECHOLNPGM(" Marlin=" EEPROM_VERSION ")");
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

      #if ENABLED(NO_WORKSPACE_OFFSETS)
        float home_offset[XYZ];
      #endif
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
        if (mesh_num_x == GRID_MAX_POINTS_X && mesh_num_y == GRID_MAX_POINTS_Y) {
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
        if (grid_max_x == GRID_MAX_POINTS_X && grid_max_y == GRID_MAX_POINTS_Y) {
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
        EEPROM_READ(endstop_adj);               // 3 floats
        EEPROM_READ(delta_radius);              // 1 float
        EEPROM_READ(delta_diagonal_rod);        // 1 float
        EEPROM_READ(delta_segments_per_second); // 1 float
        EEPROM_READ(delta_diagonal_rod_trim);   // 3 floats
        EEPROM_READ(delta_tower_angle_trim);    // 3 floats
      #elif ENABLED(Z_DUAL_ENDSTOPS)
        EEPROM_READ(z_endstop_adj);
        dummy = 0.0f;
        for (uint8_t q=11; q--;) EEPROM_READ(dummy);
      #else
        dummy = 0.0f;
        for (uint8_t q=12; q--;) EEPROM_READ(dummy);
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

      uint16_t val;
      #if ENABLED(HAVE_TMC2130)
        EEPROM_READ(val);
        #if ENABLED(X_IS_TMC2130)
          stepperX.setCurrent(val, R_SENSE, HOLD_MULTIPLIER);
        #endif
        EEPROM_READ(val);
        #if ENABLED(Y_IS_TMC2130)
          stepperY.setCurrent(val, R_SENSE, HOLD_MULTIPLIER);
        #endif
        EEPROM_READ(val);
        #if ENABLED(Z_IS_TMC2130)
          stepperZ.setCurrent(val, R_SENSE, HOLD_MULTIPLIER);
        #endif
        EEPROM_READ(val);
        #if ENABLED(X2_IS_TMC2130)
          stepperX2.setCurrent(val, R_SENSE, HOLD_MULTIPLIER);
        #endif
        EEPROM_READ(val);
        #if ENABLED(Y2_IS_TMC2130)
          stepperY2.setCurrent(val, R_SENSE, HOLD_MULTIPLIER);
        #endif
        EEPROM_READ(val);
        #if ENABLED(Z2_IS_TMC2130)
          stepperZ2.setCurrent(val, R_SENSE, HOLD_MULTIPLIER);
        #endif
        EEPROM_READ(val);
        #if ENABLED(E0_IS_TMC2130)
          stepperE0.setCurrent(val, R_SENSE, HOLD_MULTIPLIER);
        #endif
        EEPROM_READ(val);
        #if ENABLED(E1_IS_TMC2130)
          stepperE1.setCurrent(val, R_SENSE, HOLD_MULTIPLIER);
        #endif
        EEPROM_READ(val);
        #if ENABLED(E2_IS_TMC2130)
          stepperE2.setCurrent(val, R_SENSE, HOLD_MULTIPLIER);
        #endif
        EEPROM_READ(val);
        #if ENABLED(E3_IS_TMC2130)
          stepperE3.setCurrent(val, R_SENSE, HOLD_MULTIPLIER);
        #endif
      #else
        for (uint8_t q = 0; q < 10; q++) EEPROM_READ(val);
      #endif

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

      #if ENABLED(AUTO_BED_LEVELING_UBL)
        ubl.eeprom_start = (eeprom_index + 32) & 0xFFF8; // Pad the end of configuration data so it
                                                         // can float up or down a little bit without
                                                         // disrupting the Unified Bed Leveling data
        ubl.load_state();

        SERIAL_ECHOPGM(" UBL ");
        if (!ubl.state.active) SERIAL_ECHO("not ");
        SERIAL_ECHOLNPGM("active!");

        if (!ubl.sanity_check()) {
          int tmp_mesh;                                // We want to preserve whether the UBL System is Active
          bool tmp_active;                             // If it is, we want to preserve the Mesh that is being used.
          tmp_mesh = ubl.state.eeprom_storage_slot;
          tmp_active = ubl.state.active;
          SERIAL_ECHOLNPGM("\nInitializing Bed Leveling State to current firmware settings.\n");
          ubl.state = ubl.pre_initialized;             // Initialize with the pre_initialized data structure
          ubl.state.eeprom_storage_slot = tmp_mesh;    // But then restore some data we don't want mangled
          ubl.state.active = tmp_active;
        }
        else {
          SERIAL_PROTOCOLPGM("?Unable to enable Unified Bed Leveling.\n");
          ubl.state = ubl.pre_initialized;
          ubl.reset();
          ubl.store_state();
        }

        if (ubl.state.eeprom_storage_slot >= 0) {
          ubl.load_mesh(ubl.state.eeprom_storage_slot);
          SERIAL_ECHOPAIR("Mesh ", ubl.state.eeprom_storage_slot);
          SERIAL_ECHOLNPGM(" loaded from storage.");
        }
        else {
          ubl.reset();
          SERIAL_ECHOLNPGM("UBL System reset()");
        }
      #endif
    }
    #if ENABLED(EEPROM_CHITCHAT)
      Config_PrintSettings();
    #endif

    return !eeprom_read_error;
  }

#else // !EEPROM_SETTINGS

  bool Config_StoreSettings() {
    SERIAL_ERROR_START;
    SERIAL_ERRORLNPGM("EEPROM disabled");
    return false;
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
  #if DISABLED(NO_WORKSPACE_OFFSETS)
    ZERO(home_offset);
  #endif

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
    const float adj[ABC] = DELTA_ENDSTOP_ADJ,
                drt[ABC] = { DELTA_DIAGONAL_ROD_TRIM_TOWER_1, DELTA_DIAGONAL_ROD_TRIM_TOWER_2, DELTA_DIAGONAL_ROD_TRIM_TOWER_3 },
                dta[ABC] = { DELTA_TOWER_ANGLE_TRIM_1, DELTA_TOWER_ANGLE_TRIM_2, DELTA_TOWER_ANGLE_TRIM_3 };
    COPY(endstop_adj, adj);
    delta_radius = DELTA_RADIUS;
    delta_diagonal_rod = DELTA_DIAGONAL_ROD;
    delta_segments_per_second = DELTA_SEGMENTS_PER_SECOND;
    COPY(delta_diagonal_rod_trim, drt);
    COPY(delta_tower_angle_trim, dta);
  #elif ENABLED(Z_DUAL_ENDSTOPS)
    #if defined(Z_DUAL_ENDSTOPS_ADJUSTMENT)
      float z_endstop_adj = Z_DUAL_ENDSTOPS_ADJUSTMENT;
    #else
      float z_endstop_adj = 0;
    #endif
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

  #if ENABLED(HAVE_TMC2130)
    #if ENABLED(X_IS_TMC2130)
      stepperX.setCurrent(X_MAX_CURRENT, R_SENSE, HOLD_MULTIPLIER);
    #endif
    #if ENABLED(Y_IS_TMC2130)
      stepperY.setCurrent(Y_MAX_CURRENT, R_SENSE, HOLD_MULTIPLIER);
    #endif
    #if ENABLED(Z_IS_TMC2130)
      stepperZ.setCurrent(Z_MAX_CURRENT, R_SENSE, HOLD_MULTIPLIER);
    #endif
    #if ENABLED(X2_IS_TMC2130)
      stepperX2.setCurrent(X2_MAX_CURRENT, R_SENSE, HOLD_MULTIPLIER);
    #endif
    #if ENABLED(Y2_IS_TMC2130)
      stepperY2.setCurrent(Y2_MAX_CURRENT, R_SENSE, HOLD_MULTIPLIER);
    #endif
    #if ENABLED(Z2_IS_TMC2130)
      stepperZ2.setCurrent(Z2_MAX_CURRENT, R_SENSE, HOLD_MULTIPLIER);
    #endif
    #if ENABLED(E0_IS_TMC2130)
      stepperE0.setCurrent(E0_MAX_CURRENT, R_SENSE, HOLD_MULTIPLIER);
    #endif
    #if ENABLED(E1_IS_TMC2130)
      stepperE1.setCurrent(E1_MAX_CURRENT, R_SENSE, HOLD_MULTIPLIER);
    #endif
    #if ENABLED(E2_IS_TMC2130)
      stepperE2.setCurrent(E2_MAX_CURRENT, R_SENSE, HOLD_MULTIPLIER);
    #endif
    #if ENABLED(E3_IS_TMC2130)
      stepperE3.setCurrent(E3_MAX_CURRENT, R_SENSE, HOLD_MULTIPLIER);
    #endif
  #endif

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

    #if DISABLED(NO_WORKSPACE_OFFSETS)
      CONFIG_ECHO_START;
      if (!forReplay) {
        SERIAL_ECHOLNPGM("Home offset (mm)");
        CONFIG_ECHO_START;
      }
      SERIAL_ECHOPAIR("  M206 X", home_offset[X_AXIS]);
      SERIAL_ECHOPAIR(" Y", home_offset[Y_AXIS]);
      SERIAL_ECHOPAIR(" Z", home_offset[Z_AXIS]);
      SERIAL_EOL;
    #endif

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
      for (uint8_t py = 1; py <= GRID_MAX_POINTS_Y; py++) {
        for (uint8_t px = 1; px <= GRID_MAX_POINTS_X; px++) {
          CONFIG_ECHO_START;
          SERIAL_ECHOPAIR("  G29 S3 X", (int)px);
          SERIAL_ECHOPAIR(" Y", (int)py);
          SERIAL_ECHOPGM(" Z");
          SERIAL_PROTOCOL_F(mbl.z_values[py-1][px-1], 5);
          SERIAL_EOL;
        }
      }

    #elif ENABLED(AUTO_BED_LEVELING_UBL)

      if (!forReplay) {
        SERIAL_ECHOLNPGM("Unified Bed Leveling:");
        CONFIG_ECHO_START;
      }

      SERIAL_ECHOLNPAIR("  M420 S", ubl.state.active ? 1 : 0);

      if (!forReplay) {
        SERIAL_ECHOPGM("\nUBL is ");
        ubl.state.active ? SERIAL_CHAR('A') : SERIAL_ECHOPGM("Ina");
        SERIAL_ECHOLNPAIR("ctive\n\nActive Mesh Slot: ", ubl.state.eeprom_storage_slot);

        SERIAL_ECHOPGM("z_offset: ");
        SERIAL_ECHO_F(ubl.state.z_offset, 6);
        SERIAL_EOL;

        SERIAL_ECHOPAIR("EEPROM can hold ", (int)((UBL_LAST_EEPROM_INDEX - ubl.eeprom_start) / sizeof(ubl.z_values)));
        SERIAL_ECHOLNPGM(" meshes.\n");

        SERIAL_ECHOLNPGM("GRID_MAX_POINTS_X  " STRINGIFY(GRID_MAX_POINTS_X));
        SERIAL_ECHOLNPGM("GRID_MAX_POINTS_Y  " STRINGIFY(GRID_MAX_POINTS_Y));

        SERIAL_ECHOLNPGM("UBL_MESH_MIN_X     " STRINGIFY(UBL_MESH_MIN_X));
        SERIAL_ECHOLNPGM("UBL_MESH_MIN_Y     " STRINGIFY(UBL_MESH_MIN_Y));

        SERIAL_ECHOLNPGM("UBL_MESH_MAX_X     " STRINGIFY(UBL_MESH_MAX_X));
        SERIAL_ECHOLNPGM("UBL_MESH_MAX_Y     " STRINGIFY(UBL_MESH_MAX_Y));

        SERIAL_ECHOLNPGM("MESH_X_DIST        " STRINGIFY(MESH_X_DIST));
        SERIAL_ECHOLNPGM("MESH_Y_DIST        " STRINGIFY(MESH_Y_DIST));
        SERIAL_EOL;
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
        SERIAL_ECHOLNPGM("Delta settings: L=diagonal rod, R=radius, S=segments-per-second, ABC=diagonal rod trim, IJK=tower angle trim");
        CONFIG_ECHO_START;
      }
      SERIAL_ECHOPAIR("  M665 L", delta_diagonal_rod);
      SERIAL_ECHOPAIR(" R", delta_radius);
      SERIAL_ECHOPAIR(" S", delta_segments_per_second);
      SERIAL_ECHOPAIR(" A", delta_diagonal_rod_trim[A_AXIS]);
      SERIAL_ECHOPAIR(" B", delta_diagonal_rod_trim[B_AXIS]);
      SERIAL_ECHOPAIR(" C", delta_diagonal_rod_trim[C_AXIS]);
      SERIAL_ECHOPAIR(" I", delta_tower_angle_trim[A_AXIS]);
      SERIAL_ECHOPAIR(" J", delta_tower_angle_trim[B_AXIS]);
      SERIAL_ECHOPAIR(" K", delta_tower_angle_trim[C_AXIS]);
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
      CONFIG_ECHO_START;
      if (!forReplay) {
        SERIAL_ECHOLNPGM("Z-Probe Offset (mm):");
        CONFIG_ECHO_START;
      }
      SERIAL_ECHOPAIR("  M851 Z", zprobe_zoffset);
      SERIAL_EOL;
    #endif

    /**
     * TMC2130 stepper driver current
     */
    #if ENABLED(HAVE_TMC2130)
      CONFIG_ECHO_START;
      if (!forReplay) {
        SERIAL_ECHOLNPGM("Stepper driver current:");
        CONFIG_ECHO_START;
      }
      SERIAL_ECHO("  M906");
      #if ENABLED(X_IS_TMC2130)
        SERIAL_ECHOPAIR(" X", stepperX.getCurrent());
      #endif
      #if ENABLED(Y_IS_TMC2130)
        SERIAL_ECHOPAIR(" Y", stepperY.getCurrent());
      #endif
      #if ENABLED(Z_IS_TMC2130)
        SERIAL_ECHOPAIR(" Z", stepperZ.getCurrent());
      #endif
      #if ENABLED(X2_IS_TMC2130)
        SERIAL_ECHOPAIR(" X2", stepperX2.getCurrent());
      #endif
      #if ENABLED(Y2_IS_TMC2130)
        SERIAL_ECHOPAIR(" Y2", stepperY2.getCurrent());
      #endif
      #if ENABLED(Z2_IS_TMC2130)
        SERIAL_ECHOPAIR(" Z2", stepperZ2.getCurrent());
      #endif
      #if ENABLED(E0_IS_TMC2130)
        SERIAL_ECHOPAIR(" E0", stepperE0.getCurrent());
      #endif
      #if ENABLED(E1_IS_TMC2130)
        SERIAL_ECHOPAIR(" E1", stepperE1.getCurrent());
      #endif
      #if ENABLED(E2_IS_TMC2130)
        SERIAL_ECHOPAIR(" E2", stepperE2.getCurrent());
      #endif
      #if ENABLED(E3_IS_TMC2130)
        SERIAL_ECHOPAIR(" E3", stepperE3.getCurrent());
      #endif
      SERIAL_EOL;
    #endif
  }

#endif // !DISABLE_M503
