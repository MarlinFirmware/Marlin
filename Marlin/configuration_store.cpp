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
 * Settings and EEPROM storage
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

// Change EEPROM version if the structure changes
#define EEPROM_VERSION "V52"
#define EEPROM_OFFSET 100

// Check the integrity of data offsets.
// Can be disabled for production build.
//#define DEBUG_EEPROM_READWRITE

#include "configuration_store.h"
#include "Marlin.h"
#include "language.h"
#include "endstops.h"
#include "planner.h"
#include "temperature.h"
#include "ultralcd.h"
#include "stepper.h"
#include "gcode.h"
#include "vector_3.h"

#if ENABLED(MESH_BED_LEVELING)
  #include "mesh_bed_leveling.h"
#endif

#if HAS_TRINAMIC
  #include "stepper_indirection.h"
#endif

#if ENABLED(AUTO_BED_LEVELING_UBL)
  #include "ubl.h"
#endif

#if ENABLED(FWRETRACT)
  #include "fwretract.h"
#endif

#pragma pack(push, 1) // No padding between variables

typedef struct PID { float Kp, Ki, Kd; } PID;
typedef struct PIDC { float Kp, Ki, Kd, Kc; } PIDC;

/**
 * Current EEPROM Layout
 *
 * Keep this data structure up to date so
 * EEPROM size is known at compile time!
 */
typedef struct SettingsDataStruct {
  char      version[4];                                 // Vnn\0
  uint16_t  crc;                                        // Data Checksum

  //
  // DISTINCT_E_FACTORS
  //
  uint8_t   esteppers;                                  // XYZE_N - XYZ

  float     planner_axis_steps_per_mm[XYZE_N],          // M92 XYZE   planner.axis_steps_per_mm[XYZE_N]
            planner_max_feedrate_mm_s[XYZE_N];          // M203 XYZE  planner.max_feedrate_mm_s[XYZE_N]
  uint32_t  planner_max_acceleration_mm_per_s2[XYZE_N]; // M201 XYZE  planner.max_acceleration_mm_per_s2[XYZE_N]
  float     planner_acceleration,                       // M204 P     planner.acceleration
            planner_retract_acceleration,               // M204 R     planner.retract_acceleration
            planner_travel_acceleration,                // M204 T     planner.travel_acceleration
            planner_min_feedrate_mm_s,                  // M205 S     planner.min_feedrate_mm_s
            planner_min_travel_feedrate_mm_s;           // M205 T     planner.min_travel_feedrate_mm_s
  uint32_t  planner_min_segment_time_us;                // M205 B     planner.min_segment_time_us
  float     planner_max_jerk[XYZE];                     // M205 XYZE  planner.max_jerk[XYZE]

  float home_offset[XYZ];                               // M206 XYZ

  #if HOTENDS > 1
    float hotend_offset[XYZ][HOTENDS - 1];              // M218 XYZ
  #endif

  //
  // ENABLE_LEVELING_FADE_HEIGHT
  //
  float planner_z_fade_height;                          // M420 Zn  planner.z_fade_height

  //
  // MESH_BED_LEVELING
  //
  float mbl_z_offset;                                   // mbl.z_offset
  uint8_t mesh_num_x, mesh_num_y;                       // GRID_MAX_POINTS_X, GRID_MAX_POINTS_Y
  #if ENABLED(MESH_BED_LEVELING)
    float mbl_z_values[GRID_MAX_POINTS_X][GRID_MAX_POINTS_Y]; // mbl.z_values
  #else
    float mbl_z_values[3][3];
  #endif

  //
  // HAS_BED_PROBE
  //
  float zprobe_zoffset;                                 // M851 Z

  //
  // ABL_PLANAR
  //
  matrix_3x3 planner_bed_level_matrix;                  // planner.bed_level_matrix

  //
  // AUTO_BED_LEVELING_BILINEAR
  //
  uint8_t grid_max_x, grid_max_y;                       // GRID_MAX_POINTS_X, GRID_MAX_POINTS_Y
  int bilinear_grid_spacing[2],
      bilinear_start[2];                                // G29 L F
  #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
    float z_values[GRID_MAX_POINTS_X][GRID_MAX_POINTS_Y]; // G29
  #else
    float z_values[3][3];
  #endif

  //
  // AUTO_BED_LEVELING_UBL
  //
  bool planner_leveling_active;                         // M420 S  planner.leveling_active
  int8_t ubl_storage_slot;                              // ubl.storage_slot

  //
  // DELTA / [XYZ]_DUAL_ENDSTOPS
  //
  #if ENABLED(DELTA)
    float delta_height,                                 // M666 H
          delta_endstop_adj[ABC],                       // M666 XYZ
          delta_radius,                                 // M665 R
          delta_diagonal_rod,                           // M665 L
          delta_segments_per_second,                    // M665 S
          delta_calibration_radius,                     // M665 B
          delta_tower_angle_trim[ABC];                  // M665 XYZ
  #elif ENABLED(X_DUAL_ENDSTOPS) || ENABLED(Y_DUAL_ENDSTOPS) || ENABLED(Z_DUAL_ENDSTOPS)
    float x_endstop_adj,                                // M666 X
          y_endstop_adj,                                // M666 Y
          z_endstop_adj;                                // M666 Z
  #endif

  //
  // ULTIPANEL
  //
  int16_t lcd_preheat_hotend_temp[2],                   // M145 S0 H
          lcd_preheat_bed_temp[2],                      // M145 S0 B
          lcd_preheat_fan_speed[2];                     // M145 S0 F

  //
  // PIDTEMP
  //
  PIDC hotendPID[MAX_EXTRUDERS];                        // M301 En PIDC / M303 En U

  int lpq_len;                                          // M301 L

  //
  // PIDTEMPBED
  //
  PID bedPID;                                           // M304 PID / M303 E-1 U

  //
  // HAS_LCD_CONTRAST
  //
  int16_t lcd_contrast;                                // M250 C

  //
  // FWRETRACT
  //
  bool autoretract_enabled;                             // M209 S
  float retract_length,                                 // M207 S
        retract_feedrate_mm_s,                          // M207 F
        retract_zlift,                                  // M207 Z
        retract_recover_length,                         // M208 S
        retract_recover_feedrate_mm_s,                  // M208 F
        swap_retract_length,                            // M207 W
        swap_retract_recover_length,                    // M208 W
        swap_retract_recover_feedrate_mm_s;             // M208 R

  //
  // !NO_VOLUMETRIC
  //
  bool parser_volumetric_enabled;                       // M200 D  parser.volumetric_enabled
  float planner_filament_size[MAX_EXTRUDERS];           // M200 T D  planner.filament_size[]

  //
  // HAS_TRINAMIC
  //
  uint16_t tmc_stepper_current[11];                     // M906 X Y Z X2 Y2 Z2 E0 E1 E2 E3 E4
  int16_t tmc_sgt[XYZ];                                 // M914 X Y Z

  //
  // LIN_ADVANCE
  //
  float planner_extruder_advance_K;                     // M900 K    planner.extruder_advance_K

  //
  // HAS_MOTOR_CURRENT_PWM
  //
  uint32_t motor_current_setting[XYZ];                  // M907 X Z E

  //
  // CNC_COORDINATE_SYSTEMS
  //
  float coordinate_system[MAX_COORDINATE_SYSTEMS][XYZ]; // G54-G59.3

  //
  // SKEW_CORRECTION
  //
  float planner_xy_skew_factor,                         // M852 I  planner.xy_skew_factor
        planner_xz_skew_factor,                         // M852 J  planner.xz_skew_factor
        planner_yz_skew_factor;                         // M852 K  planner.yz_skew_factor

  //
  // ADVANCED_PAUSE_FEATURE
  //
  float filament_change_unload_length[MAX_EXTRUDERS],   // M603 T U
        filament_change_load_length[MAX_EXTRUDERS];     // M603 T L

} SettingsData;

#pragma pack(pop)

MarlinSettings settings;

#if ENABLED(AUTO_BED_LEVELING_BILINEAR)
  extern void refresh_bed_level();
#endif

uint16_t MarlinSettings::datasize() { return sizeof(SettingsData); }

/**
 * Post-process after Retrieve or Reset
 */

#if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
  float new_z_fade_height;
#endif

void MarlinSettings::postprocess() {
  const float oldpos[] = { current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS] };

  // steps per s2 needs to be updated to agree with units per s2
  planner.reset_acceleration_rates();

  // Make sure delta kinematics are updated before refreshing the
  // planner position so the stepper counts will be set correctly.
  #if ENABLED(DELTA)
    recalc_delta_settings();
  #endif

  #if ENABLED(PIDTEMP)
    thermalManager.updatePID();
  #endif

  #if DISABLED(NO_VOLUMETRICS)
    planner.calculate_volumetric_multipliers();
  #else
    for (uint8_t i = COUNT(planner.e_factor); i--;)
      planner.refresh_e_factor(i);
  #endif

  #if HAS_HOME_OFFSET || ENABLED(DUAL_X_CARRIAGE)
    // Software endstops depend on home_offset
    LOOP_XYZ(i) update_software_endstops((AxisEnum)i);
  #endif

  #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
    set_z_fade_height(new_z_fade_height, false); // false = no report
  #endif

  #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
    refresh_bed_level();
  #endif

  #if HAS_MOTOR_CURRENT_PWM
    stepper.refresh_motor_power();
  #endif

  #if ENABLED(FWRETRACT)
    fwretract.refresh_autoretract();
  #endif

  // Refresh steps_to_mm with the reciprocal of axis_steps_per_mm
  // and init stepper.count[], planner.position[] with current_position
  planner.refresh_positioning();

  // Various factors can change the current position
  if (memcmp(oldpos, current_position, sizeof(oldpos)))
    report_current_position();
}

#if ENABLED(EEPROM_SETTINGS)

  #define DUMMY_PID_VALUE 3000.0f
  #define EEPROM_START() int eeprom_index = EEPROM_OFFSET
  #define EEPROM_SKIP(VAR) eeprom_index += sizeof(VAR)
  #define EEPROM_WRITE(VAR) write_data(eeprom_index, (uint8_t*)&VAR, sizeof(VAR), &working_crc)
  #define EEPROM_READ(VAR) read_data(eeprom_index, (uint8_t*)&VAR, sizeof(VAR), &working_crc)
  #define EEPROM_READ_ALWAYS(VAR) read_data(eeprom_index, (uint8_t*)&VAR, sizeof(VAR), &working_crc, true)
  #define EEPROM_ASSERT(TST,ERR) if (!(TST)) do{ SERIAL_ERROR_START(); SERIAL_ERRORLNPGM(ERR); eeprom_error = true; }while(0)

  #if ENABLED(DEBUG_EEPROM_READWRITE)
    #define _FIELD_TEST(FIELD) \
      EEPROM_ASSERT( \
        eeprom_error || eeprom_index == offsetof(SettingsData, FIELD) + EEPROM_OFFSET, \
        "Field " STRINGIFY(FIELD) " mismatch." \
      )
  #else
    #define _FIELD_TEST(FIELD) NOOP
  #endif

  const char version[4] = EEPROM_VERSION;

  bool MarlinSettings::eeprom_error, MarlinSettings::validating;

  void MarlinSettings::write_data(int &pos, const uint8_t *value, uint16_t size, uint16_t *crc) {
    if (eeprom_error) { pos += size; return; }
    while (size--) {
      uint8_t * const p = (uint8_t * const)pos;
      uint8_t v = *value;
      // EEPROM has only ~100,000 write cycles,
      // so only write bytes that have changed!
      if (v != eeprom_read_byte(p)) {
        eeprom_write_byte(p, v);
        if (eeprom_read_byte(p) != v) {
          SERIAL_ECHO_START();
          SERIAL_ECHOLNPGM(MSG_ERR_EEPROM_WRITE);
          eeprom_error = true;
          return;
        }
      }
      crc16(crc, &v, 1);
      pos++;
      value++;
    };
  }

  void MarlinSettings::read_data(int &pos, uint8_t* value, uint16_t size, uint16_t *crc, const bool force/*=false*/) {
    if (eeprom_error) { pos += size; return; }
    do {
      uint8_t c = eeprom_read_byte((unsigned char*)pos);
      if (!validating || force) *value = c;
      crc16(crc, &c, 1);
      pos++;
      value++;
    } while (--size);
  }

  bool MarlinSettings::size_error(const uint16_t size) {
    if (size != datasize()) {
      SERIAL_ERROR_START();
      SERIAL_ERRORLNPGM("EEPROM datasize error.");
      return true;
    }
    return false;
  }

  /**
   * M500 - Store Configuration
   */
  bool MarlinSettings::save() {
    float dummy = 0.0f;
    char ver[4] = "ERR";

    uint16_t working_crc = 0;

    EEPROM_START();

    eeprom_error = false;

    EEPROM_WRITE(ver);     // invalidate data first
    EEPROM_SKIP(working_crc); // Skip the checksum slot

    working_crc = 0; // clear before first "real data"

    _FIELD_TEST(esteppers);

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
    EEPROM_WRITE(planner.min_segment_time_us);
    EEPROM_WRITE(planner.max_jerk);

    _FIELD_TEST(home_offset);

    #if !HAS_HOME_OFFSET
      const float home_offset[XYZ] = { 0 };
    #endif
    EEPROM_WRITE(home_offset);

    #if HOTENDS > 1
      // Skip hotend 0 which must be 0
      for (uint8_t e = 1; e < HOTENDS; e++)
        LOOP_XYZ(i) EEPROM_WRITE(hotend_offset[i][e]);
    #endif

    //
    // Global Leveling
    //

    #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
      const float zfh = planner.z_fade_height;
    #else
      const float zfh = 10.0;
    #endif
    EEPROM_WRITE(zfh);

    //
    // Mesh Bed Leveling
    //

    #if ENABLED(MESH_BED_LEVELING)
      // Compile time test that sizeof(mbl.z_values) is as expected
      static_assert(
        sizeof(mbl.z_values) == GRID_MAX_POINTS * sizeof(mbl.z_values[0][0]),
        "MBL Z array is the wrong size."
      );
      const uint8_t mesh_num_x = GRID_MAX_POINTS_X, mesh_num_y = GRID_MAX_POINTS_Y;
      EEPROM_WRITE(mbl.z_offset);
      EEPROM_WRITE(mesh_num_x);
      EEPROM_WRITE(mesh_num_y);
      EEPROM_WRITE(mbl.z_values);
    #else // For disabled MBL write a default mesh
      dummy = 0.0f;
      const uint8_t mesh_num_x = 3, mesh_num_y = 3;
      EEPROM_WRITE(dummy); // z_offset
      EEPROM_WRITE(mesh_num_x);
      EEPROM_WRITE(mesh_num_y);
      for (uint8_t q = mesh_num_x * mesh_num_y; q--;) EEPROM_WRITE(dummy);
    #endif // MESH_BED_LEVELING

    _FIELD_TEST(zprobe_zoffset);

    #if !HAS_BED_PROBE
      const float zprobe_zoffset = 0;
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
      // Compile time test that sizeof(z_values) is as expected
      static_assert(
        sizeof(z_values) == GRID_MAX_POINTS * sizeof(z_values[0][0]),
        "Bilinear Z array is the wrong size."
      );
      const uint8_t grid_max_x = GRID_MAX_POINTS_X, grid_max_y = GRID_MAX_POINTS_Y;
      EEPROM_WRITE(grid_max_x);            // 1 byte
      EEPROM_WRITE(grid_max_y);            // 1 byte
      EEPROM_WRITE(bilinear_grid_spacing); // 2 ints
      EEPROM_WRITE(bilinear_start);        // 2 ints
      EEPROM_WRITE(z_values);              // 9-256 floats
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

    _FIELD_TEST(planner_leveling_active);

    #if ENABLED(AUTO_BED_LEVELING_UBL)
      EEPROM_WRITE(planner.leveling_active);
      EEPROM_WRITE(ubl.storage_slot);
    #else
      const bool ubl_active = false;
      const int8_t storage_slot = -1;
      EEPROM_WRITE(ubl_active);
      EEPROM_WRITE(storage_slot);
    #endif // AUTO_BED_LEVELING_UBL

    // 11 floats for DELTA / [XYZ]_DUAL_ENDSTOPS
    #if ENABLED(DELTA)

      _FIELD_TEST(delta_height);

      EEPROM_WRITE(delta_height);              // 1 float
      EEPROM_WRITE(delta_endstop_adj);         // 3 floats
      EEPROM_WRITE(delta_radius);              // 1 float
      EEPROM_WRITE(delta_diagonal_rod);        // 1 float
      EEPROM_WRITE(delta_segments_per_second); // 1 float
      EEPROM_WRITE(delta_calibration_radius);  // 1 float
      EEPROM_WRITE(delta_tower_angle_trim);    // 3 floats

    #elif ENABLED(X_DUAL_ENDSTOPS) || ENABLED(Y_DUAL_ENDSTOPS) || ENABLED(Z_DUAL_ENDSTOPS)

      _FIELD_TEST(x_endstop_adj);

      // Write dual endstops in X, Y, Z order. Unused = 0.0
      dummy = 0.0f;
      #if ENABLED(X_DUAL_ENDSTOPS)
        EEPROM_WRITE(x_endstop_adj);             // 1 float
      #else
        EEPROM_WRITE(dummy);
      #endif

      #if ENABLED(Y_DUAL_ENDSTOPS)
        EEPROM_WRITE(y_endstop_adj);             // 1 float
      #else
        EEPROM_WRITE(dummy);
      #endif

      #if ENABLED(Z_DUAL_ENDSTOPS)
        EEPROM_WRITE(z_endstop_adj);             // 1 float
      #else
        EEPROM_WRITE(dummy);
      #endif

    #endif

    _FIELD_TEST(lcd_preheat_hotend_temp);

    #if DISABLED(ULTIPANEL)
      constexpr int16_t lcd_preheat_hotend_temp[2] = { PREHEAT_1_TEMP_HOTEND, PREHEAT_2_TEMP_HOTEND },
                        lcd_preheat_bed_temp[2] = { PREHEAT_1_TEMP_BED, PREHEAT_2_TEMP_BED },
                        lcd_preheat_fan_speed[2] = { PREHEAT_1_FAN_SPEED, PREHEAT_2_FAN_SPEED };
    #endif

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

    _FIELD_TEST(lpq_len);

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

    _FIELD_TEST(lcd_contrast);

    #if !HAS_LCD_CONTRAST
      const int16_t lcd_contrast = 32;
    #endif
    EEPROM_WRITE(lcd_contrast);

    #if DISABLED(FWRETRACT)
      const bool autoretract_enabled = false;
      const float autoretract_defaults[] = { 3, 45, 0, 0, 0, 13, 0, 8 };
      EEPROM_WRITE(autoretract_enabled);
      EEPROM_WRITE(autoretract_defaults);
    #else
      EEPROM_WRITE(fwretract.autoretract_enabled);
      EEPROM_WRITE(fwretract.retract_length);
      EEPROM_WRITE(fwretract.retract_feedrate_mm_s);
      EEPROM_WRITE(fwretract.retract_zlift);
      EEPROM_WRITE(fwretract.retract_recover_length);
      EEPROM_WRITE(fwretract.retract_recover_feedrate_mm_s);
      EEPROM_WRITE(fwretract.swap_retract_length);
      EEPROM_WRITE(fwretract.swap_retract_recover_length);
      EEPROM_WRITE(fwretract.swap_retract_recover_feedrate_mm_s);
    #endif

    //
    // Volumetric & Filament Size
    //

    _FIELD_TEST(parser_volumetric_enabled);

    #if DISABLED(NO_VOLUMETRICS)

      EEPROM_WRITE(parser.volumetric_enabled);

      // Save filament sizes
      for (uint8_t q = 0; q < MAX_EXTRUDERS; q++) {
        if (q < COUNT(planner.filament_size)) dummy = planner.filament_size[q];
        EEPROM_WRITE(dummy);
      }

    #else

      const bool volumetric_enabled = false;
      dummy = DEFAULT_NOMINAL_FILAMENT_DIA;
      EEPROM_WRITE(volumetric_enabled);
      for (uint8_t q = MAX_EXTRUDERS; q--;) EEPROM_WRITE(dummy);

    #endif

    //
    // Save TMC2130 or TMC2208 Configuration, and placeholder values
    //

    _FIELD_TEST(tmc_stepper_current);

    uint16_t currents[11] = {
      #if HAS_TRINAMIC
        #if X_IS_TRINAMIC
          stepperX.getCurrent(),
        #else
          0,
        #endif
        #if Y_IS_TRINAMIC
          stepperY.getCurrent(),
        #else
          0,
        #endif
        #if Z_IS_TRINAMIC
          stepperZ.getCurrent(),
        #else
          0,
        #endif
        #if X2_IS_TRINAMIC
          stepperX2.getCurrent(),
        #else
          0,
        #endif
        #if Y2_IS_TRINAMIC
          stepperY2.getCurrent(),
        #else
          0,
        #endif
        #if Z2_IS_TRINAMIC
          stepperZ2.getCurrent(),
        #else
          0,
        #endif
        #if E0_IS_TRINAMIC
          stepperE0.getCurrent(),
        #else
          0,
        #endif
        #if E1_IS_TRINAMIC
          stepperE1.getCurrent(),
        #else
          0,
        #endif
        #if E2_IS_TRINAMIC
          stepperE2.getCurrent(),
        #else
          0,
        #endif
        #if E3_IS_TRINAMIC
          stepperE3.getCurrent(),
        #else
          0,
        #endif
        #if E4_IS_TRINAMIC
          stepperE4.getCurrent()
        #else
          0
        #endif
      #else
        0
      #endif
    };
    EEPROM_WRITE(currents);

    //
    // TMC2130 Sensorless homing threshold
    //
    int16_t thrs[XYZ] = {
      #if ENABLED(SENSORLESS_HOMING)
        #if ENABLED(X_IS_TMC2130) && defined(X_HOMING_SENSITIVITY)
          stepperX.sgt(),
        #else
          0,
        #endif
        #if ENABLED(Y_IS_TMC2130) && defined(Y_HOMING_SENSITIVITY)
          stepperY.sgt(),
        #else
          0
        #endif
        #if ENABLED(Z_IS_TMC2130) && defined(Z_HOMING_SENSITIVITY)
          stepperZ.sgt()
        #else
          0
        #endif
      #else
        0
      #endif
    };
    EEPROM_WRITE(thrs);

    //
    // Linear Advance
    //

    _FIELD_TEST(planner_extruder_advance_K);

    #if ENABLED(LIN_ADVANCE)
      EEPROM_WRITE(planner.extruder_advance_K);
    #else
      dummy = 0.0f;
      EEPROM_WRITE(dummy);
    #endif

    _FIELD_TEST(motor_current_setting);

    #if HAS_MOTOR_CURRENT_PWM
      for (uint8_t q = XYZ; q--;) EEPROM_WRITE(stepper.motor_current_setting[q]);
    #else
      const uint32_t dummyui32[XYZ] = { 0 };
      EEPROM_WRITE(dummyui32);
    #endif

    //
    // CNC Coordinate Systems
    //

    _FIELD_TEST(coordinate_system);

    #if ENABLED(CNC_COORDINATE_SYSTEMS)
      EEPROM_WRITE(coordinate_system); // 27 floats
    #else
      dummy = 0.0f;
      for (uint8_t q = MAX_COORDINATE_SYSTEMS * XYZ; q--;) EEPROM_WRITE(dummy);
    #endif

    //
    // Skew correction factors
    //

    _FIELD_TEST(planner_xy_skew_factor);

    #if ENABLED(SKEW_CORRECTION)
      EEPROM_WRITE(planner.xy_skew_factor);
      EEPROM_WRITE(planner.xz_skew_factor);
      EEPROM_WRITE(planner.yz_skew_factor);
    #else
      dummy = 0.0f;
      for (uint8_t q = 3; q--;) EEPROM_WRITE(dummy);
    #endif

    //
    // Advanced Pause filament load & unload lengths
    //

    _FIELD_TEST(filament_change_unload_length);

    #if ENABLED(ADVANCED_PAUSE_FEATURE)
      for (uint8_t q = 0; q < MAX_EXTRUDERS; q++) {
        if (q < COUNT(filament_change_unload_length)) dummy = filament_change_unload_length[q];
        EEPROM_WRITE(dummy);
      }
      for (uint8_t q = 0; q < MAX_EXTRUDERS; q++) {
        if (q < COUNT(filament_change_load_length)) dummy = filament_change_load_length[q];
        EEPROM_WRITE(dummy);
      }
    #else
      dummy = 0.0f;
      for (uint8_t q = MAX_EXTRUDERS * 2; q--;) EEPROM_WRITE(dummy);
    #endif

    //
    // Validate CRC and Data Size
    //
    if (!eeprom_error) {
      const uint16_t eeprom_size = eeprom_index - (EEPROM_OFFSET),
                     final_crc = working_crc;

      // Write the EEPROM header
      eeprom_index = EEPROM_OFFSET;

      EEPROM_WRITE(version);
      EEPROM_WRITE(final_crc);

      // Report storage size
      #if ENABLED(EEPROM_CHITCHAT)
        SERIAL_ECHO_START();
        SERIAL_ECHOPAIR("Settings Stored (", eeprom_size);
        SERIAL_ECHOPAIR(" bytes; crc ", (uint32_t)final_crc);
        SERIAL_ECHOLNPGM(")");
      #endif

      eeprom_error |= size_error(eeprom_size);
    }

    //
    // UBL Mesh
    //
    #if ENABLED(UBL_SAVE_ACTIVE_ON_M500)
      if (ubl.storage_slot >= 0)
        store_mesh(ubl.storage_slot);
    #endif

    return !eeprom_error;
  }

  /**
   * M501 - Retrieve Configuration
   */
  bool MarlinSettings::_load() {
    uint16_t working_crc = 0;

    EEPROM_START();

    char stored_ver[4];
    EEPROM_READ_ALWAYS(stored_ver);

    uint16_t stored_crc;
    EEPROM_READ_ALWAYS(stored_crc);

    // Version has to match or defaults are used
    if (strncmp(version, stored_ver, 3) != 0) {
      if (stored_ver[3] != '\0') {
        stored_ver[0] = '?';
        stored_ver[1] = '\0';
      }
      #if ENABLED(EEPROM_CHITCHAT)
        SERIAL_ECHO_START();
        SERIAL_ECHOPGM("EEPROM version mismatch ");
        SERIAL_ECHOPAIR("(EEPROM=", stored_ver);
        SERIAL_ECHOLNPGM(" Marlin=" EEPROM_VERSION ")");
      #endif
      if (!validating) reset();
      eeprom_error = true;
    }
    else {
      float dummy = 0;
      #if DISABLED(AUTO_BED_LEVELING_UBL) || DISABLED(FWRETRACT) || ENABLED(NO_VOLUMETRICS)
        bool dummyb;
      #endif

      working_crc = 0;  // Init to 0. Accumulated by EEPROM_READ

      _FIELD_TEST(esteppers);

      // Number of esteppers may change
      uint8_t esteppers;
      EEPROM_READ_ALWAYS(esteppers);

      //
      // Planner Motion
      //

      // Get only the number of E stepper parameters previously stored
      // Any steppers added later are set to their defaults
      const float def1[] = DEFAULT_AXIS_STEPS_PER_UNIT, def2[] = DEFAULT_MAX_FEEDRATE;
      const uint32_t def3[] = DEFAULT_MAX_ACCELERATION;
      float tmp1[XYZ + esteppers], tmp2[XYZ + esteppers];
      uint32_t tmp3[XYZ + esteppers];
      EEPROM_READ(tmp1);
      EEPROM_READ(tmp2);
      EEPROM_READ(tmp3);
      if (!validating) LOOP_XYZE_N(i) {
        planner.axis_steps_per_mm[i]          = i < XYZ + esteppers ? tmp1[i] : def1[i < COUNT(def1) ? i : COUNT(def1) - 1];
        planner.max_feedrate_mm_s[i]          = i < XYZ + esteppers ? tmp2[i] : def2[i < COUNT(def2) ? i : COUNT(def2) - 1];
        planner.max_acceleration_mm_per_s2[i] = i < XYZ + esteppers ? tmp3[i] : def3[i < COUNT(def3) ? i : COUNT(def3) - 1];
      }

      EEPROM_READ(planner.acceleration);
      EEPROM_READ(planner.retract_acceleration);
      EEPROM_READ(planner.travel_acceleration);
      EEPROM_READ(planner.min_feedrate_mm_s);
      EEPROM_READ(planner.min_travel_feedrate_mm_s);
      EEPROM_READ(planner.min_segment_time_us);
      EEPROM_READ(planner.max_jerk);

      //
      // Home Offset (M206)
      //

      _FIELD_TEST(home_offset);

      #if !HAS_HOME_OFFSET
        float home_offset[XYZ];
      #endif
      EEPROM_READ(home_offset);

      //
      // Hotend Offsets, if any
      //

      #if HOTENDS > 1
        // Skip hotend 0 which must be 0
        for (uint8_t e = 1; e < HOTENDS; e++)
          LOOP_XYZ(i) EEPROM_READ(hotend_offset[i][e]);
      #endif

      //
      // Global Leveling
      //

      #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
        EEPROM_READ(new_z_fade_height);
      #else
        EEPROM_READ(dummy);
      #endif

      //
      // Mesh (Manual) Bed Leveling
      //

      uint8_t mesh_num_x, mesh_num_y;
      EEPROM_READ(dummy);
      EEPROM_READ_ALWAYS(mesh_num_x);
      EEPROM_READ_ALWAYS(mesh_num_y);

      #if ENABLED(MESH_BED_LEVELING)
        if (!validating) mbl.z_offset = dummy;
        if (mesh_num_x == GRID_MAX_POINTS_X && mesh_num_y == GRID_MAX_POINTS_Y) {
          // EEPROM data fits the current mesh
          EEPROM_READ(mbl.z_values);
        }
        else {
          // EEPROM data is stale
          if (!validating) mbl.reset();
          for (uint16_t q = mesh_num_x * mesh_num_y; q--;) EEPROM_READ(dummy);
        }
      #else
        // MBL is disabled - skip the stored data
        for (uint16_t q = mesh_num_x * mesh_num_y; q--;) EEPROM_READ(dummy);
      #endif // MESH_BED_LEVELING

      _FIELD_TEST(zprobe_zoffset);

      #if !HAS_BED_PROBE
        float zprobe_zoffset;
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
      EEPROM_READ_ALWAYS(grid_max_x);                       // 1 byte
      EEPROM_READ_ALWAYS(grid_max_y);                       // 1 byte
      #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
        if (grid_max_x == GRID_MAX_POINTS_X && grid_max_y == GRID_MAX_POINTS_Y) {
          if (!validating) set_bed_leveling_enabled(false);
          EEPROM_READ(bilinear_grid_spacing);        // 2 ints
          EEPROM_READ(bilinear_start);               // 2 ints
          EEPROM_READ(z_values);                     // 9 to 256 floats
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

      //
      // Unified Bed Leveling active state
      //

      _FIELD_TEST(planner_leveling_active);

      #if ENABLED(AUTO_BED_LEVELING_UBL)
        EEPROM_READ(planner.leveling_active);
        EEPROM_READ(ubl.storage_slot);
      #else
        uint8_t dummyui8;
        EEPROM_READ(dummyb);
        EEPROM_READ(dummyui8);
      #endif // AUTO_BED_LEVELING_UBL

      //
      // DELTA Geometry or Dual Endstops offsets
      //

      #if ENABLED(DELTA)

        _FIELD_TEST(delta_height);

        EEPROM_READ(delta_height);              // 1 float
        EEPROM_READ(delta_endstop_adj);         // 3 floats
        EEPROM_READ(delta_radius);              // 1 float
        EEPROM_READ(delta_diagonal_rod);        // 1 float
        EEPROM_READ(delta_segments_per_second); // 1 float
        EEPROM_READ(delta_calibration_radius);  // 1 float
        EEPROM_READ(delta_tower_angle_trim);    // 3 floats

      #elif ENABLED(X_DUAL_ENDSTOPS) || ENABLED(Y_DUAL_ENDSTOPS) || ENABLED(Z_DUAL_ENDSTOPS)

        _FIELD_TEST(x_endstop_adj);

        #if ENABLED(X_DUAL_ENDSTOPS)
          EEPROM_READ(x_endstop_adj);             // 1 float
        #else
          EEPROM_READ(dummy);
        #endif
        #if ENABLED(Y_DUAL_ENDSTOPS)
          EEPROM_READ(y_endstop_adj);             // 1 float
        #else
          EEPROM_READ(dummy);
        #endif
        #if ENABLED(Z_DUAL_ENDSTOPS)
          EEPROM_READ(z_endstop_adj);             // 1 float
        #else
          EEPROM_READ(dummy);
        #endif

      #endif

      //
      // LCD Preheat settings
      //

      _FIELD_TEST(lcd_preheat_hotend_temp);

      #if DISABLED(ULTIPANEL)
        int16_t lcd_preheat_hotend_temp[2], lcd_preheat_bed_temp[2], lcd_preheat_fan_speed[2];
      #endif
      EEPROM_READ(lcd_preheat_hotend_temp); // 2 floats
      EEPROM_READ(lcd_preheat_bed_temp);    // 2 floats
      EEPROM_READ(lcd_preheat_fan_speed);   // 2 floats

      //EEPROM_ASSERT(
      //  WITHIN(lcd_preheat_fan_speed, 0, 255),
      //  "lcd_preheat_fan_speed out of range"
      //);

      //
      // Hotend PID
      //

      #if ENABLED(PIDTEMP)
        for (uint8_t e = 0; e < MAX_EXTRUDERS; e++) {
          EEPROM_READ(dummy); // Kp
          if (e < HOTENDS && dummy != DUMMY_PID_VALUE) {
            // do not need to scale PID values as the values in EEPROM are already scaled
            if (!validating) PID_PARAM(Kp, e) = dummy;
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

      //
      // PID Extrusion Scaling
      //

      _FIELD_TEST(lpq_len);

      #if DISABLED(PID_EXTRUSION_SCALING)
        int lpq_len;
      #endif
      EEPROM_READ(lpq_len);

      //
      // Heated Bed PID
      //

      #if ENABLED(PIDTEMPBED)
        EEPROM_READ(dummy); // bedKp
        if (dummy != DUMMY_PID_VALUE) {
          if (!validating) thermalManager.bedKp = dummy;
          EEPROM_READ(thermalManager.bedKi);
          EEPROM_READ(thermalManager.bedKd);
        }
      #else
        for (uint8_t q=3; q--;) EEPROM_READ(dummy); // bedKp, bedKi, bedKd
      #endif

      //
      // LCD Contrast
      //

      _FIELD_TEST(lcd_contrast);

      #if !HAS_LCD_CONTRAST
        int16_t lcd_contrast;
      #endif
      EEPROM_READ(lcd_contrast);

      //
      // Firmware Retraction
      //

      #if ENABLED(FWRETRACT)
        EEPROM_READ(fwretract.autoretract_enabled);
        EEPROM_READ(fwretract.retract_length);
        EEPROM_READ(fwretract.retract_feedrate_mm_s);
        EEPROM_READ(fwretract.retract_zlift);
        EEPROM_READ(fwretract.retract_recover_length);
        EEPROM_READ(fwretract.retract_recover_feedrate_mm_s);
        EEPROM_READ(fwretract.swap_retract_length);
        EEPROM_READ(fwretract.swap_retract_recover_length);
        EEPROM_READ(fwretract.swap_retract_recover_feedrate_mm_s);
      #else
        EEPROM_READ(dummyb);
        for (uint8_t q=8; q--;) EEPROM_READ(dummy);
      #endif

      //
      // Volumetric & Filament Size
      //

      _FIELD_TEST(parser_volumetric_enabled);

      #if DISABLED(NO_VOLUMETRICS)

        EEPROM_READ(parser.volumetric_enabled);

        for (uint8_t q = 0; q < MAX_EXTRUDERS; q++) {
          EEPROM_READ(dummy);
          if (!validating && q < COUNT(planner.filament_size))
            planner.filament_size[q] = dummy;
        }

      #else

        EEPROM_READ(dummyb);
        for (uint8_t q=MAX_EXTRUDERS; q--;) EEPROM_READ(dummy);

      #endif

      //
      // TMC2130 Stepper Current
      //

      _FIELD_TEST(tmc_stepper_current);

      #if HAS_TRINAMIC
        #define SET_CURR(N,Q) stepper##Q.setCurrent(currents[N] ? currents[N] : Q##_CURRENT, R_SENSE, HOLD_MULTIPLIER)
        uint16_t currents[11];
        EEPROM_READ(currents);
        if (!validating) {
          #if X_IS_TRINAMIC
            SET_CURR(0, X);
          #endif
          #if Y_IS_TRINAMIC
            SET_CURR(1, Y);
          #endif
          #if Z_IS_TRINAMIC
            SET_CURR(2, Z);
          #endif
          #if X2_IS_TRINAMIC
            SET_CURR(3, X2);
          #endif
          #if Y2_IS_TRINAMIC
            SET_CURR(4, Y2);
          #endif
          #if Z2_IS_TRINAMIC
            SET_CURR(5, Z2);
          #endif
          #if E0_IS_TRINAMIC
            SET_CURR(6, E0);
          #endif
          #if E1_IS_TRINAMIC
            SET_CURR(7, E1);
          #endif
          #if E2_IS_TRINAMIC
            SET_CURR(8, E2);
          #endif
          #if E3_IS_TRINAMIC
            SET_CURR(9, E3);
          #endif
          #if E4_IS_TRINAMIC
            SET_CURR(10, E4);
          #endif
        }
      #else
        uint16_t val;
        for (uint8_t q=11; q--;) EEPROM_READ(val);
      #endif

      /*
       * TMC2130 Sensorless homing threshold.
       * X and X2 use the same value
       * Y and Y2 use the same value
       * Z and Z2 use the same value
       */
      int16_t thrs[XYZ];
      EEPROM_READ(thrs);
      #if ENABLED(SENSORLESS_HOMING)
        if (!validating) {
          #ifdef X_HOMING_SENSITIVITY
            #if ENABLED(X_IS_TMC2130)
              stepperX.sgt(thrs[0]);
            #endif
            #if ENABLED(X2_IS_TMC2130)
              stepperX2.sgt(thrs[0]);
            #endif
          #endif
          #ifdef Y_HOMING_SENSITIVITY
            #if ENABLED(Y_IS_TMC2130)
              stepperY.sgt(thrs[1]);
            #endif
            #if ENABLED(Y2_IS_TMC2130)
              stepperY2.sgt(thrs[1]);
            #endif
          #endif
          #ifdef Z_HOMING_SENSITIVITY
            #if ENABLED(Z_IS_TMC2130)
              stepperZ.sgt(thrs[2]);
            #endif
            #if ENABLED(Z2_IS_TMC2130)
              stepperZ2.sgt(thrs[2]);
            #endif
          #endif
        }
      #endif

      //
      // Linear Advance
      //

      _FIELD_TEST(planner_extruder_advance_K);

      #if ENABLED(LIN_ADVANCE)
        EEPROM_READ(planner.extruder_advance_K);
      #else
        EEPROM_READ(dummy);
      #endif

      //
      // Motor Current PWM
      //

      _FIELD_TEST(motor_current_setting);

      #if HAS_MOTOR_CURRENT_PWM
        for (uint8_t q = XYZ; q--;) EEPROM_READ(stepper.motor_current_setting[q]);
      #else
        uint32_t dummyui32[XYZ];
        EEPROM_READ(dummyui32);
      #endif

      //
      // CNC Coordinate System
      //

      _FIELD_TEST(coordinate_system);

      #if ENABLED(CNC_COORDINATE_SYSTEMS)
        if (!validating) (void)select_coordinate_system(-1); // Go back to machine space
        EEPROM_READ(coordinate_system);                  // 27 floats
      #else
        for (uint8_t q = MAX_COORDINATE_SYSTEMS * XYZ; q--;) EEPROM_READ(dummy);
      #endif

      //
      // Skew correction factors
      //

      _FIELD_TEST(planner_xy_skew_factor);

      #if ENABLED(SKEW_CORRECTION_GCODE)
        EEPROM_READ(planner.xy_skew_factor);
        #if ENABLED(SKEW_CORRECTION_FOR_Z)
          EEPROM_READ(planner.xz_skew_factor);
          EEPROM_READ(planner.yz_skew_factor);
        #else
          EEPROM_READ(dummy);
          EEPROM_READ(dummy);
        #endif
      #else
        for (uint8_t q = 3; q--;) EEPROM_READ(dummy);
      #endif

      //
      // Advanced Pause filament load & unload lengths
      //

      _FIELD_TEST(filament_change_unload_length);

      #if ENABLED(ADVANCED_PAUSE_FEATURE)
        for (uint8_t q = 0; q < MAX_EXTRUDERS; q++) {
          EEPROM_READ(dummy);
          if (!validating && q < COUNT(filament_change_unload_length)) filament_change_unload_length[q] = dummy;
        }
        for (uint8_t q = 0; q < MAX_EXTRUDERS; q++) {
          EEPROM_READ(dummy);
          if (!validating && q < COUNT(filament_change_load_length)) filament_change_load_length[q] = dummy;
        }
      #else
        for (uint8_t q = MAX_EXTRUDERS * 2; q--;) EEPROM_READ(dummy);
      #endif

      eeprom_error = size_error(eeprom_index - (EEPROM_OFFSET));
      if (eeprom_error) {
        SERIAL_ECHO_START();
        SERIAL_ECHOPAIR("Index: ", int(eeprom_index - (EEPROM_OFFSET)));
        SERIAL_ECHOLNPAIR(" Size: ", datasize());
      }
      else if (working_crc != stored_crc) {
        eeprom_error = true;
        #if ENABLED(EEPROM_CHITCHAT)
          SERIAL_ERROR_START();
          SERIAL_ERRORPGM("EEPROM CRC mismatch - (stored) ");
          SERIAL_ERROR(stored_crc);
          SERIAL_ERRORPGM(" != ");
          SERIAL_ERROR(working_crc);
          SERIAL_ERRORLNPGM(" (calculated)!");
        #endif
      }
      else if (!validating) {
        #if ENABLED(EEPROM_CHITCHAT)
          SERIAL_ECHO_START();
          SERIAL_ECHO(version);
          SERIAL_ECHOPAIR(" stored settings retrieved (", eeprom_index - (EEPROM_OFFSET));
          SERIAL_ECHOPAIR(" bytes; crc ", (uint32_t)working_crc);
          SERIAL_ECHOLNPGM(")");
        #endif
      }

      if (!validating) {
        if (eeprom_error) reset(); else postprocess();
      }

      #if ENABLED(AUTO_BED_LEVELING_UBL)
        ubl.report_state();

        if (!validating) {
          if (!ubl.sanity_check()) {
            SERIAL_EOL();
            #if ENABLED(EEPROM_CHITCHAT)
              ubl.echo_name();
              SERIAL_ECHOLNPGM(" initialized.\n");
            #endif
          }
          else {
            eeprom_error = true;
            #if ENABLED(EEPROM_CHITCHAT)
              SERIAL_PROTOCOLPGM("?Can't enable ");
              ubl.echo_name();
              SERIAL_PROTOCOLLNPGM(".");
            #endif
            ubl.reset();
          }

          if (ubl.storage_slot >= 0) {
            load_mesh(ubl.storage_slot);
            #if ENABLED(EEPROM_CHITCHAT)
              SERIAL_ECHOPAIR("Mesh ", ubl.storage_slot);
              SERIAL_ECHOLNPGM(" loaded from storage.");
            #endif
          }
          else {
            ubl.reset();
            #if ENABLED(EEPROM_CHITCHAT)
              SERIAL_ECHOLNPGM("UBL System reset()");
            #endif
          }
        }
      #endif
    }

    #if ENABLED(EEPROM_CHITCHAT) && DISABLED(DISABLE_M503)
      if (!validating) report();
    #endif

    return !eeprom_error;
  }

  bool MarlinSettings::validate() {
    validating = true;
    const bool success = _load();
    validating = false;
    return success;
  }

  bool MarlinSettings::load() {
    if (validate()) return _load();
    reset();
    return true;
  }

  #if ENABLED(AUTO_BED_LEVELING_UBL)

    #if ENABLED(EEPROM_CHITCHAT)
      void ubl_invalid_slot(const int s) {
        SERIAL_PROTOCOLLNPGM("?Invalid slot.");
        SERIAL_PROTOCOL(s);
        SERIAL_PROTOCOLLNPGM(" mesh slots available.");
      }
    #endif

    int16_t MarlinSettings::meshes_start_index() {
      return (datasize() + EEPROM_OFFSET + 32) & 0xFFF8;  // Pad the end of configuration data so it can float up
                                                          // or down a little bit without disrupting the mesh data
    }

    uint16_t MarlinSettings::calc_num_meshes() {
      return (meshes_end - meshes_start_index()) / sizeof(ubl.z_values);
    }

    void MarlinSettings::store_mesh(const int8_t slot) {

      #if ENABLED(AUTO_BED_LEVELING_UBL)
        const int16_t a = calc_num_meshes();
        if (!WITHIN(slot, 0, a - 1)) {
          #if ENABLED(EEPROM_CHITCHAT)
            ubl_invalid_slot(a);
            SERIAL_PROTOCOLPAIR("E2END=", E2END);
            SERIAL_PROTOCOLPAIR(" meshes_end=", meshes_end);
            SERIAL_PROTOCOLLNPAIR(" slot=", slot);
            SERIAL_EOL();
          #endif
          return;
        }

        uint16_t crc = 0;
        int pos = meshes_end - (slot + 1) * sizeof(ubl.z_values);

        write_data(pos, (uint8_t *)&ubl.z_values, sizeof(ubl.z_values), &crc);

        // Write crc to MAT along with other data, or just tack on to the beginning or end

        #if ENABLED(EEPROM_CHITCHAT)
          SERIAL_PROTOCOLLNPAIR("Mesh saved in slot ", slot);
        #endif

      #else

        // Other mesh types

      #endif
    }

    void MarlinSettings::load_mesh(const int8_t slot, void * const into/*=NULL*/) {

      #if ENABLED(AUTO_BED_LEVELING_UBL)

        const int16_t a = settings.calc_num_meshes();

        if (!WITHIN(slot, 0, a - 1)) {
          #if ENABLED(EEPROM_CHITCHAT)
            ubl_invalid_slot(a);
          #endif
          return;
        }

        uint16_t crc = 0;
        int pos = meshes_end - (slot + 1) * sizeof(ubl.z_values);
        uint8_t * const dest = into ? (uint8_t*)into : (uint8_t*)&ubl.z_values;
        read_data(pos, dest, sizeof(ubl.z_values), &crc);

        // Compare crc with crc from MAT, or read from end

        #if ENABLED(EEPROM_CHITCHAT)
          SERIAL_PROTOCOLLNPAIR("Mesh loaded from slot ", slot);
        #endif

      #else

        // Other mesh types

      #endif
    }

    //void MarlinSettings::delete_mesh() { return; }
    //void MarlinSettings::defrag_meshes() { return; }

  #endif // AUTO_BED_LEVELING_UBL

#else // !EEPROM_SETTINGS

  bool MarlinSettings::save() {
    SERIAL_ERROR_START();
    SERIAL_ERRORLNPGM("EEPROM disabled");
    return false;
  }

#endif // !EEPROM_SETTINGS

/**
 * M502 - Reset Configuration
 */
void MarlinSettings::reset() {
  static const float tmp1[] PROGMEM = DEFAULT_AXIS_STEPS_PER_UNIT, tmp2[] PROGMEM = DEFAULT_MAX_FEEDRATE;
  static const uint32_t tmp3[] PROGMEM = DEFAULT_MAX_ACCELERATION;
  LOOP_XYZE_N(i) {
    planner.axis_steps_per_mm[i]          = pgm_read_float(&tmp1[i < COUNT(tmp1) ? i : COUNT(tmp1) - 1]);
    planner.max_feedrate_mm_s[i]          = pgm_read_float(&tmp2[i < COUNT(tmp2) ? i : COUNT(tmp2) - 1]);
    planner.max_acceleration_mm_per_s2[i] = pgm_read_dword_near(&tmp3[i < COUNT(tmp3) ? i : COUNT(tmp3) - 1]);
  }

  planner.acceleration = DEFAULT_ACCELERATION;
  planner.retract_acceleration = DEFAULT_RETRACT_ACCELERATION;
  planner.travel_acceleration = DEFAULT_TRAVEL_ACCELERATION;
  planner.min_feedrate_mm_s = DEFAULT_MINIMUMFEEDRATE;
  planner.min_travel_feedrate_mm_s = DEFAULT_MINTRAVELFEEDRATE;
  planner.min_segment_time_us = DEFAULT_MINSEGMENTTIME;
  planner.max_jerk[X_AXIS] = DEFAULT_XJERK;
  planner.max_jerk[Y_AXIS] = DEFAULT_YJERK;
  planner.max_jerk[Z_AXIS] = DEFAULT_ZJERK;
  planner.max_jerk[E_AXIS] = DEFAULT_EJERK;

  #if HAS_HOME_OFFSET
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

  //
  // Global Leveling
  //

  #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
    new_z_fade_height = 0.0;
  #endif

  #if HAS_LEVELING
    reset_bed_level();
  #endif

  #if HAS_BED_PROBE
    zprobe_zoffset = Z_PROBE_OFFSET_FROM_EXTRUDER;
  #endif

  #if ENABLED(DELTA)
    const float adj[ABC] = DELTA_ENDSTOP_ADJ,
                dta[ABC] = DELTA_TOWER_ANGLE_TRIM;
    delta_height = DELTA_HEIGHT;
    COPY(delta_endstop_adj, adj);
    delta_radius = DELTA_RADIUS;
    delta_diagonal_rod = DELTA_DIAGONAL_ROD;
    delta_segments_per_second = DELTA_SEGMENTS_PER_SECOND;
    delta_calibration_radius = DELTA_CALIBRATION_RADIUS;
    COPY(delta_tower_angle_trim, dta);

  #elif ENABLED(X_DUAL_ENDSTOPS) || ENABLED(Y_DUAL_ENDSTOPS) || ENABLED(Z_DUAL_ENDSTOPS)

    #if ENABLED(X_DUAL_ENDSTOPS)
      x_endstop_adj = (
        #ifdef X_DUAL_ENDSTOPS_ADJUSTMENT
          X_DUAL_ENDSTOPS_ADJUSTMENT
        #else
          0
        #endif
      );
    #endif
    #if ENABLED(Y_DUAL_ENDSTOPS)
      y_endstop_adj = (
        #ifdef Y_DUAL_ENDSTOPS_ADJUSTMENT
          Y_DUAL_ENDSTOPS_ADJUSTMENT
        #else
          0
        #endif
      );
    #endif
    #if ENABLED(Z_DUAL_ENDSTOPS)
      z_endstop_adj = (
        #ifdef Z_DUAL_ENDSTOPS_ADJUSTMENT
          Z_DUAL_ENDSTOPS_ADJUSTMENT
        #else
          0
        #endif
      );
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

  #if HAS_LCD_CONTRAST
    lcd_contrast = DEFAULT_LCD_CONTRAST;
  #endif

  #if ENABLED(FWRETRACT)
    fwretract.reset();
  #endif

  #if DISABLED(NO_VOLUMETRICS)

    parser.volumetric_enabled =
      #if ENABLED(VOLUMETRIC_DEFAULT_ON)
        true
      #else
        false
      #endif
    ;
    for (uint8_t q = 0; q < COUNT(planner.filament_size); q++)
      planner.filament_size[q] = DEFAULT_NOMINAL_FILAMENT_DIA;

  #endif

  endstops.enable_globally(
    #if ENABLED(ENDSTOPS_ALWAYS_ON_DEFAULT)
      true
    #else
      false
    #endif
  );

  #if X_IS_TRINAMIC
    stepperX.setCurrent(X_CURRENT, R_SENSE, HOLD_MULTIPLIER);
  #endif
  #if Y_IS_TRINAMIC
    stepperY.setCurrent(Y_CURRENT, R_SENSE, HOLD_MULTIPLIER);
  #endif
  #if Z_IS_TRINAMIC
    stepperZ.setCurrent(Z_CURRENT, R_SENSE, HOLD_MULTIPLIER);
  #endif
  #if X2_IS_TRINAMIC
    stepperX2.setCurrent(X2_CURRENT, R_SENSE, HOLD_MULTIPLIER);
  #endif
  #if Y2_IS_TRINAMIC
    stepperY2.setCurrent(Y2_CURRENT, R_SENSE, HOLD_MULTIPLIER);
  #endif
  #if Z2_IS_TRINAMIC
    stepperZ2.setCurrent(Z2_CURRENT, R_SENSE, HOLD_MULTIPLIER);
  #endif
  #if E0_IS_TRINAMIC
    stepperE0.setCurrent(E0_CURRENT, R_SENSE, HOLD_MULTIPLIER);
  #endif
  #if E1_IS_TRINAMIC
    stepperE1.setCurrent(E1_CURRENT, R_SENSE, HOLD_MULTIPLIER);
  #endif
  #if E2_IS_TRINAMIC
    stepperE2.setCurrent(E2_CURRENT, R_SENSE, HOLD_MULTIPLIER);
  #endif
  #if E3_IS_TRINAMIC
    stepperE3.setCurrent(E3_CURRENT, R_SENSE, HOLD_MULTIPLIER);
  #endif
  #if E4_IS_TRINAMIC
    stepperE4.setCurrent(E4_CURRENT, R_SENSE, HOLD_MULTIPLIER);
  #endif

  #if ENABLED(SENSORLESS_HOMING)
    #ifdef X_HOMING_SENSITIVITY
      #if ENABLED(X_IS_TMC2130)
        stepperX.sgt(X_HOMING_SENSITIVITY);
      #endif
      #if ENABLED(X2_IS_TMC2130)
        stepperX2.sgt(X_HOMING_SENSITIVITY);
      #endif
    #endif
    #ifdef Y_HOMING_SENSITIVITY
      #if ENABLED(Y_IS_TMC2130)
        stepperY.sgt(Y_HOMING_SENSITIVITY);
      #endif
      #if ENABLED(Y2_IS_TMC2130)
        stepperY2.sgt(Y_HOMING_SENSITIVITY);
      #endif
    #endif
    #ifdef Z_HOMING_SENSITIVITY
      #if ENABLED(Z_IS_TMC2130)
        stepperZ.sgt(Z_HOMING_SENSITIVITY);
      #endif
      #if ENABLED(Z2_IS_TMC2130)
        stepperZ2.sgt(Z_HOMING_SENSITIVITY);
      #endif
    #endif
  #endif

  #if ENABLED(LIN_ADVANCE)
    planner.extruder_advance_K = LIN_ADVANCE_K;
  #endif

  #if HAS_MOTOR_CURRENT_PWM
    uint32_t tmp_motor_current_setting[XYZ] = PWM_MOTOR_CURRENT;
    for (uint8_t q = XYZ; q--;)
      stepper.digipot_current(q, (stepper.motor_current_setting[q] = tmp_motor_current_setting[q]));
  #endif

  #if ENABLED(SKEW_CORRECTION_GCODE)
    planner.xy_skew_factor = XY_SKEW_FACTOR;
    #if ENABLED(SKEW_CORRECTION_FOR_Z)
      planner.xz_skew_factor = XZ_SKEW_FACTOR;
      planner.yz_skew_factor = YZ_SKEW_FACTOR;
    #endif
  #endif

  #if ENABLED(ADVANCED_PAUSE_FEATURE)
    for (uint8_t e = 0; e < E_STEPPERS; e++) {
      filament_change_unload_length[e] = FILAMENT_CHANGE_UNLOAD_LENGTH;
      filament_change_load_length[e] = FILAMENT_CHANGE_LOAD_LENGTH;
    }
  #endif

  postprocess();

  #if ENABLED(EEPROM_CHITCHAT)
    SERIAL_ECHO_START();
    SERIAL_ECHOLNPGM("Hardcoded Default Settings Loaded");
  #endif
}

#if DISABLED(DISABLE_M503)

  #define CONFIG_ECHO_START do{ if (!forReplay) SERIAL_ECHO_START(); }while(0)

  /**
   * M503 - Report current settings in RAM
   *
   * Unless specifically disabled, M503 is available even without EEPROM
   */
  void MarlinSettings::report(const bool forReplay) {

    /**
     * Announce current units, in case inches are being displayed
     */
    CONFIG_ECHO_START;
    #if ENABLED(INCH_MODE_SUPPORT)
      #define LINEAR_UNIT(N) (float(N) / parser.linear_unit_factor)
      #define VOLUMETRIC_UNIT(N) (float(N) / (parser.volumetric_enabled ? parser.volumetric_unit_factor : parser.linear_unit_factor))
      SERIAL_ECHOPGM("  G2");
      SERIAL_CHAR(parser.linear_unit_factor == 1.0 ? '1' : '0');
      SERIAL_ECHOPGM(" ; Units in ");
      serialprintPGM(parser.linear_unit_factor == 1.0 ? PSTR("mm\n") : PSTR("inches\n"));
    #else
      #define LINEAR_UNIT(N) (N)
      #define VOLUMETRIC_UNIT(N) (N)
      SERIAL_ECHOLNPGM("  G21    ; Units in mm");
    #endif

    #if ENABLED(ULTIPANEL)

      // Temperature units - for Ultipanel temperature options

      CONFIG_ECHO_START;
      #if ENABLED(TEMPERATURE_UNITS_SUPPORT)
        #define TEMP_UNIT(N) parser.to_temp_units(N)
        SERIAL_ECHOPGM("  M149 ");
        SERIAL_CHAR(parser.temp_units_code());
        SERIAL_ECHOPGM(" ; Units in ");
        serialprintPGM(parser.temp_units_name());
      #else
        #define TEMP_UNIT(N) (N)
        SERIAL_ECHOLNPGM("  M149 C ; Units in Celsius");
      #endif

    #endif

    SERIAL_EOL();

    #if DISABLED(NO_VOLUMETRICS)

      /**
       * Volumetric extrusion M200
       */
      if (!forReplay) {
        CONFIG_ECHO_START;
        SERIAL_ECHOPGM("Filament settings:");
        if (parser.volumetric_enabled)
          SERIAL_EOL();
        else
          SERIAL_ECHOLNPGM(" Disabled");
      }

      CONFIG_ECHO_START;
      SERIAL_ECHOPAIR("  M200 D", LINEAR_UNIT(planner.filament_size[0]));
      SERIAL_EOL();
      #if EXTRUDERS > 1
        CONFIG_ECHO_START;
        SERIAL_ECHOPAIR("  M200 T1 D", LINEAR_UNIT(planner.filament_size[1]));
        SERIAL_EOL();
        #if EXTRUDERS > 2
          CONFIG_ECHO_START;
          SERIAL_ECHOPAIR("  M200 T2 D", LINEAR_UNIT(planner.filament_size[2]));
          SERIAL_EOL();
          #if EXTRUDERS > 3
            CONFIG_ECHO_START;
            SERIAL_ECHOPAIR("  M200 T3 D", LINEAR_UNIT(planner.filament_size[3]));
            SERIAL_EOL();
            #if EXTRUDERS > 4
              CONFIG_ECHO_START;
              SERIAL_ECHOPAIR("  M200 T4 D", LINEAR_UNIT(planner.filament_size[4]));
              SERIAL_EOL();
            #endif // EXTRUDERS > 4
          #endif // EXTRUDERS > 3
        #endif // EXTRUDERS > 2
      #endif // EXTRUDERS > 1

      if (!parser.volumetric_enabled) {
        CONFIG_ECHO_START;
        SERIAL_ECHOLNPGM("  M200 D0");
      }

    #endif // !NO_VOLUMETRICS

    if (!forReplay) {
      CONFIG_ECHO_START;
      SERIAL_ECHOLNPGM("Steps per unit:");
    }
    CONFIG_ECHO_START;
    SERIAL_ECHOPAIR("  M92 X", LINEAR_UNIT(planner.axis_steps_per_mm[X_AXIS]));
    SERIAL_ECHOPAIR(" Y", LINEAR_UNIT(planner.axis_steps_per_mm[Y_AXIS]));
    SERIAL_ECHOPAIR(" Z", LINEAR_UNIT(planner.axis_steps_per_mm[Z_AXIS]));
    #if DISABLED(DISTINCT_E_FACTORS)
      SERIAL_ECHOPAIR(" E", VOLUMETRIC_UNIT(planner.axis_steps_per_mm[E_AXIS]));
    #endif
    SERIAL_EOL();
    #if ENABLED(DISTINCT_E_FACTORS)
      CONFIG_ECHO_START;
      for (uint8_t i = 0; i < E_STEPPERS; i++) {
        SERIAL_ECHOPAIR("  M92 T", (int)i);
        SERIAL_ECHOLNPAIR(" E", VOLUMETRIC_UNIT(planner.axis_steps_per_mm[E_AXIS + i]));
      }
    #endif

    if (!forReplay) {
      CONFIG_ECHO_START;
      SERIAL_ECHOLNPGM("Maximum feedrates (units/s):");
    }
    CONFIG_ECHO_START;
    SERIAL_ECHOPAIR("  M203 X", LINEAR_UNIT(planner.max_feedrate_mm_s[X_AXIS]));
    SERIAL_ECHOPAIR(" Y", LINEAR_UNIT(planner.max_feedrate_mm_s[Y_AXIS]));
    SERIAL_ECHOPAIR(" Z", LINEAR_UNIT(planner.max_feedrate_mm_s[Z_AXIS]));
    #if DISABLED(DISTINCT_E_FACTORS)
      SERIAL_ECHOPAIR(" E", VOLUMETRIC_UNIT(planner.max_feedrate_mm_s[E_AXIS]));
    #endif
    SERIAL_EOL();
    #if ENABLED(DISTINCT_E_FACTORS)
      CONFIG_ECHO_START;
      for (uint8_t i = 0; i < E_STEPPERS; i++) {
        SERIAL_ECHOPAIR("  M203 T", (int)i);
        SERIAL_ECHOLNPAIR(" E", VOLUMETRIC_UNIT(planner.max_feedrate_mm_s[E_AXIS + i]));
      }
    #endif

    if (!forReplay) {
      CONFIG_ECHO_START;
      SERIAL_ECHOLNPGM("Maximum Acceleration (units/s2):");
    }
    CONFIG_ECHO_START;
    SERIAL_ECHOPAIR("  M201 X", LINEAR_UNIT(planner.max_acceleration_mm_per_s2[X_AXIS]));
    SERIAL_ECHOPAIR(" Y", LINEAR_UNIT(planner.max_acceleration_mm_per_s2[Y_AXIS]));
    SERIAL_ECHOPAIR(" Z", LINEAR_UNIT(planner.max_acceleration_mm_per_s2[Z_AXIS]));
    #if DISABLED(DISTINCT_E_FACTORS)
      SERIAL_ECHOPAIR(" E", VOLUMETRIC_UNIT(planner.max_acceleration_mm_per_s2[E_AXIS]));
    #endif
    SERIAL_EOL();
    #if ENABLED(DISTINCT_E_FACTORS)
      CONFIG_ECHO_START;
      for (uint8_t i = 0; i < E_STEPPERS; i++) {
        SERIAL_ECHOPAIR("  M201 T", (int)i);
        SERIAL_ECHOLNPAIR(" E", VOLUMETRIC_UNIT(planner.max_acceleration_mm_per_s2[E_AXIS + i]));
      }
    #endif

    if (!forReplay) {
      CONFIG_ECHO_START;
      SERIAL_ECHOLNPGM("Acceleration (units/s2): P<print_accel> R<retract_accel> T<travel_accel>");
    }
    CONFIG_ECHO_START;
    SERIAL_ECHOPAIR("  M204 P", LINEAR_UNIT(planner.acceleration));
    SERIAL_ECHOPAIR(" R", LINEAR_UNIT(planner.retract_acceleration));
    SERIAL_ECHOLNPAIR(" T", LINEAR_UNIT(planner.travel_acceleration));

    if (!forReplay) {
      CONFIG_ECHO_START;
      SERIAL_ECHOLNPGM("Advanced: S<min_feedrate> T<min_travel_feedrate> B<min_segment_time_us> X<max_xy_jerk> Z<max_z_jerk> E<max_e_jerk>");
    }
    CONFIG_ECHO_START;
    SERIAL_ECHOPAIR("  M205 S", LINEAR_UNIT(planner.min_feedrate_mm_s));
    SERIAL_ECHOPAIR(" T", LINEAR_UNIT(planner.min_travel_feedrate_mm_s));
    SERIAL_ECHOPAIR(" B", planner.min_segment_time_us);
    SERIAL_ECHOPAIR(" X", LINEAR_UNIT(planner.max_jerk[X_AXIS]));
    SERIAL_ECHOPAIR(" Y", LINEAR_UNIT(planner.max_jerk[Y_AXIS]));
    SERIAL_ECHOPAIR(" Z", LINEAR_UNIT(planner.max_jerk[Z_AXIS]));
    SERIAL_ECHOLNPAIR(" E", LINEAR_UNIT(planner.max_jerk[E_AXIS]));

    #if HAS_M206_COMMAND
      if (!forReplay) {
        CONFIG_ECHO_START;
        SERIAL_ECHOLNPGM("Home offset:");
      }
      CONFIG_ECHO_START;
      SERIAL_ECHOPAIR("  M206 X", LINEAR_UNIT(home_offset[X_AXIS]));
      SERIAL_ECHOPAIR(" Y", LINEAR_UNIT(home_offset[Y_AXIS]));
      SERIAL_ECHOLNPAIR(" Z", LINEAR_UNIT(home_offset[Z_AXIS]));
    #endif

    #if HOTENDS > 1
      if (!forReplay) {
        CONFIG_ECHO_START;
        SERIAL_ECHOLNPGM("Hotend offsets:");
      }
      CONFIG_ECHO_START;
      for (uint8_t e = 1; e < HOTENDS; e++) {
        SERIAL_ECHOPAIR("  M218 T", (int)e);
        SERIAL_ECHOPAIR(" X", LINEAR_UNIT(hotend_offset[X_AXIS][e]));
        SERIAL_ECHOPAIR(" Y", LINEAR_UNIT(hotend_offset[Y_AXIS][e]));
        #if ENABLED(DUAL_X_CARRIAGE) || ENABLED(SWITCHING_NOZZLE) ||ENABLED(PARKING_EXTRUDER)
          SERIAL_ECHOPAIR(" Z", LINEAR_UNIT(hotend_offset[Z_AXIS][e]));
        #endif
        SERIAL_EOL();
      }
    #endif

    /**
     * Bed Leveling
     */
    #if HAS_LEVELING

      #if ENABLED(MESH_BED_LEVELING)

        if (!forReplay) {
          CONFIG_ECHO_START;
          SERIAL_ECHOLNPGM("Mesh Bed Leveling:");
        }

      #elif ENABLED(AUTO_BED_LEVELING_UBL)

        if (!forReplay) {
          CONFIG_ECHO_START;
          ubl.echo_name();
          SERIAL_ECHOLNPGM(":");
        }

      #elif HAS_ABL

        if (!forReplay) {
          CONFIG_ECHO_START;
          SERIAL_ECHOLNPGM("Auto Bed Leveling:");
        }

      #endif

      CONFIG_ECHO_START;
      SERIAL_ECHOPAIR("  M420 S", planner.leveling_active ? 1 : 0);
      #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
        SERIAL_ECHOPAIR(" Z", LINEAR_UNIT(planner.z_fade_height));
      #endif
      SERIAL_EOL();

      #if ENABLED(MESH_BED_LEVELING)

        if (leveling_is_valid()) {
          for (uint8_t py = 0; py < GRID_MAX_POINTS_Y; py++) {
            for (uint8_t px = 0; px < GRID_MAX_POINTS_X; px++) {
              CONFIG_ECHO_START;
              SERIAL_ECHOPAIR("  G29 S3 X", (int)px + 1);
              SERIAL_ECHOPAIR(" Y", (int)py + 1);
              SERIAL_ECHOPGM(" Z");
              SERIAL_PROTOCOL_F(LINEAR_UNIT(mbl.z_values[px][py]), 5);
              SERIAL_EOL();
            }
          }
        }

      #elif ENABLED(AUTO_BED_LEVELING_UBL)

        if (!forReplay) {
          SERIAL_EOL();
          ubl.report_state();
          SERIAL_ECHOLNPAIR("\nActive Mesh Slot: ", ubl.storage_slot);
          SERIAL_ECHOPAIR("EEPROM can hold ", calc_num_meshes());
          SERIAL_ECHOLNPGM(" meshes.\n");
        }

        ubl.report_current_mesh();

      #elif ENABLED(AUTO_BED_LEVELING_BILINEAR)

        if (leveling_is_valid()) {
          for (uint8_t py = 0; py < GRID_MAX_POINTS_Y; py++) {
            for (uint8_t px = 0; px < GRID_MAX_POINTS_X; px++) {
              CONFIG_ECHO_START;
              SERIAL_ECHOPAIR("  G29 W I", (int)px + 1);
              SERIAL_ECHOPAIR(" J", (int)py + 1);
              SERIAL_ECHOPGM(" Z");
              SERIAL_PROTOCOL_F(LINEAR_UNIT(z_values[px][py]), 5);
              SERIAL_EOL();
            }
          }
        }

      #endif

    #endif // HAS_LEVELING

    #if ENABLED(DELTA)
      if (!forReplay) {
        CONFIG_ECHO_START;
        SERIAL_ECHOLNPGM("Endstop adjustment:");
      }
      CONFIG_ECHO_START;
      SERIAL_ECHOPAIR("  M666 X", LINEAR_UNIT(delta_endstop_adj[X_AXIS]));
      SERIAL_ECHOPAIR(" Y", LINEAR_UNIT(delta_endstop_adj[Y_AXIS]));
      SERIAL_ECHOLNPAIR(" Z", LINEAR_UNIT(delta_endstop_adj[Z_AXIS]));
      if (!forReplay) {
        CONFIG_ECHO_START;
        SERIAL_ECHOLNPGM("Delta settings: L<diagonal_rod> R<radius> H<height> S<segments_per_s> B<calibration radius> XYZ<tower angle corrections>");
      }
      CONFIG_ECHO_START;
      SERIAL_ECHOPAIR("  M665 L", LINEAR_UNIT(delta_diagonal_rod));
      SERIAL_ECHOPAIR(" R", LINEAR_UNIT(delta_radius));
      SERIAL_ECHOPAIR(" H", LINEAR_UNIT(delta_height));
      SERIAL_ECHOPAIR(" S", delta_segments_per_second);
      SERIAL_ECHOPAIR(" B", LINEAR_UNIT(delta_calibration_radius));
      SERIAL_ECHOPAIR(" X", LINEAR_UNIT(delta_tower_angle_trim[A_AXIS]));
      SERIAL_ECHOPAIR(" Y", LINEAR_UNIT(delta_tower_angle_trim[B_AXIS]));
      SERIAL_ECHOPAIR(" Z", LINEAR_UNIT(delta_tower_angle_trim[C_AXIS]));
      SERIAL_EOL();

    #elif ENABLED(X_DUAL_ENDSTOPS) || ENABLED(Y_DUAL_ENDSTOPS) || ENABLED(Z_DUAL_ENDSTOPS)
      if (!forReplay) {
        CONFIG_ECHO_START;
        SERIAL_ECHOLNPGM("Endstop adjustment:");
      }
      CONFIG_ECHO_START;
      SERIAL_ECHOPGM("  M666");
      #if ENABLED(X_DUAL_ENDSTOPS)
        SERIAL_ECHOPAIR(" X", LINEAR_UNIT(x_endstop_adj));
      #endif
      #if ENABLED(Y_DUAL_ENDSTOPS)
        SERIAL_ECHOPAIR(" Y", LINEAR_UNIT(y_endstop_adj));
      #endif
      #if ENABLED(Z_DUAL_ENDSTOPS)
        SERIAL_ECHOPAIR(" Z", LINEAR_UNIT(z_endstop_adj));
      #endif
      SERIAL_EOL();
    #endif // DELTA

    #if ENABLED(ULTIPANEL)
      if (!forReplay) {
        CONFIG_ECHO_START;
        SERIAL_ECHOLNPGM("Material heatup parameters:");
      }
      for (uint8_t i = 0; i < COUNT(lcd_preheat_hotend_temp); i++) {
        CONFIG_ECHO_START;
        SERIAL_ECHOPAIR("  M145 S", (int)i);
        SERIAL_ECHOPAIR(" H", TEMP_UNIT(lcd_preheat_hotend_temp[i]));
        SERIAL_ECHOPAIR(" B", TEMP_UNIT(lcd_preheat_bed_temp[i]));
        SERIAL_ECHOLNPAIR(" F", lcd_preheat_fan_speed[i]);
      }
    #endif // ULTIPANEL

    #if HAS_PID_HEATING

      if (!forReplay) {
        CONFIG_ECHO_START;
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
              SERIAL_EOL();
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
          SERIAL_EOL();
        }
      #endif // PIDTEMP

      #if ENABLED(PIDTEMPBED)
        CONFIG_ECHO_START;
        SERIAL_ECHOPAIR("  M304 P", thermalManager.bedKp);
        SERIAL_ECHOPAIR(" I", unscalePID_i(thermalManager.bedKi));
        SERIAL_ECHOPAIR(" D", unscalePID_d(thermalManager.bedKd));
        SERIAL_EOL();
      #endif

    #endif // PIDTEMP || PIDTEMPBED

    #if HAS_LCD_CONTRAST
      if (!forReplay) {
        CONFIG_ECHO_START;
        SERIAL_ECHOLNPGM("LCD Contrast:");
      }
      CONFIG_ECHO_START;
      SERIAL_ECHOLNPAIR("  M250 C", lcd_contrast);
    #endif

    #if ENABLED(FWRETRACT)

      if (!forReplay) {
        CONFIG_ECHO_START;
        SERIAL_ECHOLNPGM("Retract: S<length> F<units/m> Z<lift>");
      }
      CONFIG_ECHO_START;
      SERIAL_ECHOPAIR("  M207 S", LINEAR_UNIT(fwretract.retract_length));
      SERIAL_ECHOPAIR(" W", LINEAR_UNIT(fwretract.swap_retract_length));
      SERIAL_ECHOPAIR(" F", MMS_TO_MMM(LINEAR_UNIT(fwretract.retract_feedrate_mm_s)));
      SERIAL_ECHOLNPAIR(" Z", LINEAR_UNIT(fwretract.retract_zlift));

      if (!forReplay) {
        CONFIG_ECHO_START;
        SERIAL_ECHOLNPGM("Recover: S<length> F<units/m>");
      }
      CONFIG_ECHO_START;
      SERIAL_ECHOPAIR("  M208 S", LINEAR_UNIT(fwretract.retract_recover_length));
      SERIAL_ECHOPAIR(" W", LINEAR_UNIT(fwretract.swap_retract_recover_length));
      SERIAL_ECHOLNPAIR(" F", MMS_TO_MMM(LINEAR_UNIT(fwretract.retract_recover_feedrate_mm_s)));

      if (!forReplay) {
        CONFIG_ECHO_START;
        SERIAL_ECHOLNPGM("Auto-Retract: S=0 to disable, 1 to interpret E-only moves as retract/recover");
      }
      CONFIG_ECHO_START;
      SERIAL_ECHOLNPAIR("  M209 S", fwretract.autoretract_enabled ? 1 : 0);

    #endif // FWRETRACT

    /**
     * Probe Offset
     */
    #if HAS_BED_PROBE
      if (!forReplay) {
        CONFIG_ECHO_START;
        SERIAL_ECHOLNPGM("Z-Probe Offset (mm):");
      }
      CONFIG_ECHO_START;
      SERIAL_ECHOLNPAIR("  M851 Z", LINEAR_UNIT(zprobe_zoffset));
    #endif

    /**
     * Bed Skew Correction
     */
    #if ENABLED(SKEW_CORRECTION_GCODE)
      if (!forReplay) {
        CONFIG_ECHO_START;
        SERIAL_ECHOLNPGM("Skew Factor: ");
      }
      CONFIG_ECHO_START;
      #if ENABLED(SKEW_CORRECTION_FOR_Z)
        SERIAL_ECHOPGM("  M852 I");
        SERIAL_ECHO_F(LINEAR_UNIT(planner.xy_skew_factor), 6);
        SERIAL_ECHOPGM(" J");
        SERIAL_ECHO_F(LINEAR_UNIT(planner.xz_skew_factor), 6);
        SERIAL_ECHOPGM(" K");
        SERIAL_ECHO_F(LINEAR_UNIT(planner.yz_skew_factor), 6);
        SERIAL_EOL();
      #else
        SERIAL_ECHOPGM("  M852 S");
        SERIAL_ECHO_F(LINEAR_UNIT(planner.xy_skew_factor), 6);
        SERIAL_EOL();
      #endif
    #endif

    /**
     * TMC2130 stepper driver current
     */
    #if HAS_TRINAMIC
      if (!forReplay) {
        CONFIG_ECHO_START;
        SERIAL_ECHOLNPGM("Stepper driver current:");
      }
      CONFIG_ECHO_START;
      SERIAL_ECHOPGM("  M906");
      #if ENABLED(X_IS_TMC2130) || ENABLED(X_IS_TMC2208)
        SERIAL_ECHOPAIR(" X ", stepperX.getCurrent());
      #endif
      #if ENABLED(Y_IS_TMC2130) || ENABLED(Y_IS_TMC2208)
        SERIAL_ECHOPAIR(" Y ", stepperY.getCurrent());
      #endif
      #if ENABLED(Z_IS_TMC2130) || ENABLED(Z_IS_TMC2208)
        SERIAL_ECHOPAIR(" Z ", stepperZ.getCurrent());
      #endif
      #if ENABLED(X2_IS_TMC2130) || ENABLED(X2_IS_TMC2208)
        SERIAL_ECHOPAIR(" X2 ", stepperX2.getCurrent());
      #endif
      #if ENABLED(Y2_IS_TMC2130) || ENABLED(Y2_IS_TMC2208)
        SERIAL_ECHOPAIR(" Y2 ", stepperY2.getCurrent());
      #endif
      #if ENABLED(Z2_IS_TMC2130) || ENABLED(Z2_IS_TMC2208)
        SERIAL_ECHOPAIR(" Z2 ", stepperZ2.getCurrent());
      #endif
      #if ENABLED(E0_IS_TMC2130) || ENABLED(E0_IS_TMC2208)
        SERIAL_ECHOPAIR(" E0 ", stepperE0.getCurrent());
      #endif
      #if ENABLED(E1_IS_TMC2130) || ENABLED(E1_IS_TMC2208)
        SERIAL_ECHOPAIR(" E1 ", stepperE1.getCurrent());
      #endif
      #if ENABLED(E2_IS_TMC2130) || ENABLED(E2_IS_TMC2208)
        SERIAL_ECHOPAIR(" E2 ", stepperE2.getCurrent());
      #endif
      #if ENABLED(E3_IS_TMC2130) || ENABLED(E3_IS_TMC2208)
        SERIAL_ECHOPAIR(" E3 ", stepperE3.getCurrent());
      #endif
      #if ENABLED(E4_IS_TMC2130) || ENABLED(E4_IS_TMC2208)
        SERIAL_ECHOPAIR(" E4 ", stepperE4.getCurrent());
      #endif
      SERIAL_EOL();
    #endif

    /**
     * TMC2130 Sensorless homing thresholds
     */
    #if ENABLED(SENSORLESS_HOMING)
      if (!forReplay) {
        CONFIG_ECHO_START;
        SERIAL_ECHOLNPGM("Sensorless homing threshold:");
      }
      CONFIG_ECHO_START;
      SERIAL_ECHOPGM("  M914");
      #ifdef X_HOMING_SENSITIVITY
        #if ENABLED(X_IS_TMC2130)
          SERIAL_ECHOPAIR(" X", stepperX.sgt());
        #endif
        #if ENABLED(X2_IS_TMC2130)
          SERIAL_ECHOPAIR(" X2 ", stepperX2.sgt());
        #endif
      #endif
      #ifdef Y_HOMING_SENSITIVITY
        #if ENABLED(Y_IS_TMC2130)
          SERIAL_ECHOPAIR(" Y", stepperY.sgt());
        #endif
        #if ENABLED(X2_IS_TMC2130)
          SERIAL_ECHOPAIR(" Y2 ", stepperY2.sgt());
        #endif
      #endif
      #ifdef Z_HOMING_SENSITIVITY
        #if ENABLED(Z_IS_TMC2130)
          SERIAL_ECHOPAIR(" Z", stepperZ.sgt());
        #endif
        #if ENABLED(Z2_IS_TMC2130)
          SERIAL_ECHOPAIR(" Z2 ", stepperZ2.sgt());
        #endif
      #endif
      SERIAL_EOL();
    #endif

    /**
     * Linear Advance
     */
    #if ENABLED(LIN_ADVANCE)
      if (!forReplay) {
        CONFIG_ECHO_START;
        SERIAL_ECHOLNPGM("Linear Advance:");
      }
      CONFIG_ECHO_START;
      SERIAL_ECHOLNPAIR("  M900 K", planner.extruder_advance_K);
    #endif

    #if HAS_MOTOR_CURRENT_PWM
      CONFIG_ECHO_START;
      if (!forReplay) {
        SERIAL_ECHOLNPGM("Stepper motor currents:");
        CONFIG_ECHO_START;
      }
      SERIAL_ECHOPAIR("  M907 X", stepper.motor_current_setting[0]);
      SERIAL_ECHOPAIR(" Z", stepper.motor_current_setting[1]);
      SERIAL_ECHOPAIR(" E", stepper.motor_current_setting[2]);
      SERIAL_EOL();
    #endif

    /**
     * Advanced Pause filament load & unload lengths
     */
    #if ENABLED(ADVANCED_PAUSE_FEATURE)
      if (!forReplay) {
        CONFIG_ECHO_START;
        SERIAL_ECHOLNPGM("Filament load/unload lengths:");
      }
      CONFIG_ECHO_START;
      #if EXTRUDERS == 1
        SERIAL_ECHOPAIR("  M603 L", LINEAR_UNIT(filament_change_load_length[0]));
        SERIAL_ECHOLNPAIR(" U", LINEAR_UNIT(filament_change_unload_length[0]));
      #else
        SERIAL_ECHOPAIR("  M603 T0 L", LINEAR_UNIT(filament_change_load_length[0]));
        SERIAL_ECHOLNPAIR(" U", LINEAR_UNIT(filament_change_unload_length[0]));
        CONFIG_ECHO_START;
        SERIAL_ECHOPAIR("  M603 T1 L", LINEAR_UNIT(filament_change_load_length[1]));
        SERIAL_ECHOLNPAIR(" U", LINEAR_UNIT(filament_change_unload_length[1]));
        #if EXTRUDERS > 2
          CONFIG_ECHO_START;
          SERIAL_ECHOPAIR("  M603 T2 L", LINEAR_UNIT(filament_change_load_length[2]));
          SERIAL_ECHOLNPAIR(" U", LINEAR_UNIT(filament_change_unload_length[2]));
          #if EXTRUDERS > 3
            CONFIG_ECHO_START;
            SERIAL_ECHOPAIR("  M603 T3 L", LINEAR_UNIT(filament_change_load_length[3]));
            SERIAL_ECHOLNPAIR(" U", LINEAR_UNIT(filament_change_unload_length[3]));
            #if EXTRUDERS > 4
              CONFIG_ECHO_START;
              SERIAL_ECHOPAIR("  M603 T4 L", LINEAR_UNIT(filament_change_load_length[4]));
              SERIAL_ECHOLNPAIR(" U", LINEAR_UNIT(filament_change_unload_length[4]));
            #endif // EXTRUDERS > 4
          #endif // EXTRUDERS > 3
        #endif // EXTRUDERS > 2
      #endif // EXTRUDERS == 1
    #endif // ADVANCED_PAUSE_FEATURE
  }

#endif // !DISABLE_M503
