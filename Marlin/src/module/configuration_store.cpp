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
#define EEPROM_VERSION "V62"
#define EEPROM_OFFSET 100

// Check the integrity of data offsets.
// Can be disabled for production build.
//#define DEBUG_EEPROM_READWRITE

#include "configuration_store.h"

#if ADD_PORT_ARG
  #define PORTARG_SOLO     const int8_t port
  #define PORTARG_AFTER   ,const int8_t port
  #define PORTVAR_SOLO     port
#else
  #define PORTARG_SOLO
  #define PORTARG_AFTER
  #define PORTVAR_SOLO
#endif

#include "endstops.h"
#include "planner.h"
#include "stepper.h"
#include "temperature.h"
#include "../lcd/ultralcd.h"
#include "../core/language.h"
#include "../libs/vector_3.h"
#include "../gcode/gcode.h"
#include "../Marlin.h"

#if HAS_LEVELING
  #include "../feature/bedlevel/bedlevel.h"
#endif

#if HAS_SERVOS
  #include "servo.h"
#else
  #undef NUM_SERVOS
  #define NUM_SERVOS NUM_SERVO_PLUGS
#endif

#if HAS_BED_PROBE
  #include "../module/probe.h"
#endif

#include "../feature/fwretract.h"
#include "../feature/pause.h"

#if EXTRUDERS > 1
  #include "tool_change.h"
  void M217_report(const bool eeprom);
#endif

#if HAS_TRINAMIC
  #include "stepper_indirection.h"
  #include "../feature/tmc_util.h"
  #define TMC_GET_PWMTHRS(A,Q) _tmc_thrs(stepper##Q.microsteps(), stepper##Q.TPWMTHRS(), planner.settings.axis_steps_per_mm[_AXIS(A)])
#endif

#pragma pack(push, 1) // No padding between variables

typedef struct { uint16_t X, Y, Z, X2, Y2, Z2, Z3, E0, E1, E2, E3, E4, E5; } tmc_stepper_current_t;
typedef struct { uint32_t X, Y, Z, X2, Y2, Z2, Z3, E0, E1, E2, E3, E4, E5; } tmc_hybrid_threshold_t;
typedef struct {  int16_t X, Y, Z;                                         } tmc_sgt_t;

// Limit an index to an array size
#define ALIM(I,ARR) MIN(I, COUNT(ARR) - 1)

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

  planner_settings_t planner_settings;

  float planner_max_jerk[XYZE],                         // M205 XYZE  planner.max_jerk[XYZE]
        planner_junction_deviation_mm;                  // M205 J     planner.junction_deviation_mm

  float home_offset[XYZ];                               // M206 XYZ

  #if HAS_HOTEND_OFFSET
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

  float zprobe_zoffset;

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
  // SERVO_ANGLES
  //
  uint16_t servo_angles[NUM_SERVOS][2];                 // M281 P L U

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
  #elif ENABLED(X_DUAL_ENDSTOPS) || ENABLED(Y_DUAL_ENDSTOPS) || Z_MULTI_ENDSTOPS
    float x2_endstop_adj,                               // M666 X
          y2_endstop_adj,                               // M666 Y
          z2_endstop_adj,                               // M666 Z (S2)
          z3_endstop_adj;                               // M666 Z (S3)
  #endif

  //
  // ULTIPANEL
  //
  int16_t lcd_preheat_hotend_temp[2],                   // M145 S0 H
          lcd_preheat_bed_temp[2];                      // M145 S0 B
  uint8_t lcd_preheat_fan_speed[2];                     // M145 S0 F

  //
  // PIDTEMP
  //
  PIDC_t hotendPID[HOTENDS];                            // M301 En PIDC / M303 En U
  int16_t lpq_len;                                      // M301 L

  //
  // PIDTEMPBED
  //
  PID_t bedPID;                                         // M304 PID / M303 E-1 U

  //
  // HAS_LCD_CONTRAST
  //
  int16_t lcd_contrast;                                 // M250 C

  //
  // FWRETRACT
  //
  fwretract_settings_t fwretract_settings;              // M207 S F Z W, M208 S F W R
  bool autoretract_enabled;                             // M209 S

  //
  // !NO_VOLUMETRIC
  //
  bool parser_volumetric_enabled;                       // M200 D  parser.volumetric_enabled
  float planner_filament_size[EXTRUDERS];               // M200 T D  planner.filament_size[]

  //
  // HAS_TRINAMIC
  //
  tmc_stepper_current_t tmc_stepper_current;            // M906 X Y Z X2 Y2 Z2 Z3 E0 E1 E2 E3 E4 E5
  tmc_hybrid_threshold_t tmc_hybrid_threshold;          // M913 X Y Z X2 Y2 Z2 Z3 E0 E1 E2 E3 E4 E5
  tmc_sgt_t tmc_sgt;                                    // M914 X Y Z

  //
  // LIN_ADVANCE
  //
  float planner_extruder_advance_K[EXTRUDERS];          // M900 K  planner.extruder_advance_K

  //
  // HAS_MOTOR_CURRENT_PWM
  //
  uint32_t motor_current_setting[3];                    // M907 X Z E

  //
  // CNC_COORDINATE_SYSTEMS
  //
  float coordinate_system[MAX_COORDINATE_SYSTEMS][XYZ]; // G54-G59.3

  //
  // SKEW_CORRECTION
  //
  skew_factor_t planner_skew_factor;                    // M852 I J K  planner.skew_factor

  //
  // ADVANCED_PAUSE_FEATURE
  //
  fil_change_settings_t fc_settings[EXTRUDERS];         // M603 T U L

  //
  // SINGLENOZZLE toolchange values
  //
  #if EXTRUDERS > 1
    toolchange_settings_t toolchange_settings;                // M217 S P R
  #endif

} SettingsData;

MarlinSettings settings;

uint16_t MarlinSettings::datasize() { return sizeof(SettingsData); }

/**
 * Post-process after Retrieve or Reset
 */

#if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
  float new_z_fade_height;
#endif

void MarlinSettings::postprocess() {
  const float oldpos[XYZE] = { current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS] };

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

  #if ENABLED(JUNCTION_DEVIATION) && ENABLED(LIN_ADVANCE)
    planner.recalculate_max_e_jerk();
  #endif

  // Refresh steps_to_mm with the reciprocal of axis_steps_per_mm
  // and init stepper.count[], planner.position[] with current_position
  planner.refresh_positioning();

  // Various factors can change the current position
  if (memcmp(oldpos, current_position, sizeof(oldpos)))
    report_current_position();
}

#if ENABLED(SD_FIRMWARE_UPDATE)

  #if ENABLED(EEPROM_SETTINGS)
    static_assert(
      !WITHIN(SD_FIRMWARE_UPDATE_EEPROM_ADDR, EEPROM_OFFSET, EEPROM_OFFSET + sizeof(SettingsData)),
      "SD_FIRMWARE_UPDATE_EEPROM_ADDR collides with EEPROM settings storage."
    );
  #endif

  bool MarlinSettings::sd_update_status() {
    uint8_t val;
    persistentStore.read_data(SD_FIRMWARE_UPDATE_EEPROM_ADDR, &val);
    return (val == SD_FIRMWARE_UPDATE_ACTIVE_VALUE);
  }

  bool MarlinSettings::set_sd_update_status(const bool enable) {
    if (enable != sd_update_status())
      persistentStore.write_data(
        SD_FIRMWARE_UPDATE_EEPROM_ADDR,
        enable ? SD_FIRMWARE_UPDATE_ACTIVE_VALUE : SD_FIRMWARE_UPDATE_INACTIVE_VALUE
      );
    return true;
  }

#endif // SD_FIRMWARE_UPDATE

#if ENABLED(EEPROM_SETTINGS)
  #include "../HAL/shared/persistent_store_api.h"

  #define EEPROM_START() int eeprom_index = EEPROM_OFFSET; persistentStore.access_start()
  #define EEPROM_FINISH() persistentStore.access_finish()
  #define EEPROM_SKIP(VAR) eeprom_index += sizeof(VAR)
  #define EEPROM_WRITE(VAR) persistentStore.write_data(eeprom_index, (uint8_t*)&VAR, sizeof(VAR), &working_crc)
  #define EEPROM_READ(VAR) persistentStore.read_data(eeprom_index, (uint8_t*)&VAR, sizeof(VAR), &working_crc, !validating)
  #define EEPROM_READ_ALWAYS(VAR) persistentStore.read_data(eeprom_index, (uint8_t*)&VAR, sizeof(VAR), &working_crc)
  #define EEPROM_ASSERT(TST,ERR) do{ if (!(TST)) { SERIAL_ERROR_START_P(port); SERIAL_ERRORLNPGM_P(port, ERR); eeprom_error = true; } }while(0)

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

  bool MarlinSettings::size_error(const uint16_t size PORTARG_AFTER) {
    if (size != datasize()) {
      #if ENABLED(EEPROM_CHITCHAT)
        SERIAL_ERROR_START_P(port);
        SERIAL_ERRORLNPGM_P(port, "EEPROM datasize error.");
      #endif
      return true;
    }
    return false;
  }

  /**
   * M500 - Store Configuration
   */
  bool MarlinSettings::save(PORTARG_SOLO) {
    float dummy = 0;
    char ver[4] = "ERR";

    uint16_t working_crc = 0;

    EEPROM_START();

    eeprom_error = false;
    #if ENABLED(FLASH_EEPROM_EMULATION)
      EEPROM_SKIP(ver);   // Flash doesn't allow rewriting without erase
    #else
      EEPROM_WRITE(ver);  // invalidate data first
    #endif
    EEPROM_SKIP(working_crc); // Skip the checksum slot

    working_crc = 0; // clear before first "real data"

    _FIELD_TEST(esteppers);

    const uint8_t esteppers = COUNT(planner.settings.axis_steps_per_mm) - XYZ;
    EEPROM_WRITE(esteppers);

    EEPROM_WRITE(planner.settings);

    #if HAS_CLASSIC_JERK
      EEPROM_WRITE(planner.max_jerk);
      #if ENABLED(JUNCTION_DEVIATION) && ENABLED(LIN_ADVANCE)
        dummy = float(DEFAULT_EJERK);
        EEPROM_WRITE(dummy);
      #endif
    #else
      const float planner_max_jerk[XYZE] = { float(DEFAULT_XJERK), float(DEFAULT_YJERK), float(DEFAULT_ZJERK), float(DEFAULT_EJERK) };
      EEPROM_WRITE(planner_max_jerk);
    #endif

    #if ENABLED(JUNCTION_DEVIATION)
      EEPROM_WRITE(planner.junction_deviation_mm);
    #else
      dummy = 0.02f;
      EEPROM_WRITE(dummy);
    #endif

    _FIELD_TEST(home_offset);

    #if !HAS_HOME_OFFSET
      const float home_offset[XYZ] = { 0 };
    #endif
    EEPROM_WRITE(home_offset);

    #if HAS_HOTEND_OFFSET
      // Skip hotend 0 which must be 0
      for (uint8_t e = 1; e < HOTENDS; e++)
        LOOP_XYZ(i) EEPROM_WRITE(hotend_offset[i][e]);
    #endif

    //
    // Global Leveling
    //

    const float zfh = (
      #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
        planner.z_fade_height
      #else
        10.0
      #endif
    );
    EEPROM_WRITE(zfh);

    //
    // Mesh Bed Leveling
    //

    #if ENABLED(MESH_BED_LEVELING)
      // Compile time test that sizeof(mbl.z_values) is as expected
      static_assert(
        sizeof(mbl.z_values) == (GRID_MAX_POINTS) * sizeof(mbl.z_values[0][0]),
        "MBL Z array is the wrong size."
      );
      const uint8_t mesh_num_x = GRID_MAX_POINTS_X, mesh_num_y = GRID_MAX_POINTS_Y;
      EEPROM_WRITE(mbl.z_offset);
      EEPROM_WRITE(mesh_num_x);
      EEPROM_WRITE(mesh_num_y);
      EEPROM_WRITE(mbl.z_values);
    #else // For disabled MBL write a default mesh
      dummy = 0;
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
      dummy = 0;
      for (uint8_t q = 9; q--;) EEPROM_WRITE(dummy);
    #endif

    //
    // Bilinear Auto Bed Leveling
    //

    #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
      // Compile time test that sizeof(z_values) is as expected
      static_assert(
        sizeof(z_values) == (GRID_MAX_POINTS) * sizeof(z_values[0][0]),
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
      dummy = 0;
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

    #if !HAS_SERVOS || DISABLED(EDITABLE_SERVO_ANGLES)
      #if ENABLED(SWITCHING_EXTRUDER)
        constexpr uint16_t sesa[][2] = SWITCHING_EXTRUDER_SERVO_ANGLES;
      #endif
      constexpr uint16_t servo_angles[NUM_SERVOS][2] = {
        #if ENABLED(SWITCHING_EXTRUDER)
          [SWITCHING_EXTRUDER_SERVO_NR] = { sesa[0][0], sesa[0][1] }
          #if EXTRUDERS > 3
            , [SWITCHING_EXTRUDER_E23_SERVO_NR] = { sesa[1][0], sesa[1][1] }
          #endif
        #elif ENABLED(SWITCHING_NOZZLE)
          [SWITCHING_NOZZLE_SERVO_NR] = SWITCHING_NOZZLE_SERVO_ANGLES
        #elif defined(Z_SERVO_ANGLES) && defined(Z_PROBE_SERVO_NR)
          [Z_PROBE_SERVO_NR] = Z_SERVO_ANGLES
        #endif
      };
    #endif

    EEPROM_WRITE(servo_angles);

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

    #elif ENABLED(X_DUAL_ENDSTOPS) || ENABLED(Y_DUAL_ENDSTOPS) || Z_MULTI_ENDSTOPS

      _FIELD_TEST(x2_endstop_adj);

      // Write dual endstops in X, Y, Z order. Unused = 0.0
      dummy = 0;
      #if ENABLED(X_DUAL_ENDSTOPS)
        EEPROM_WRITE(endstops.x2_endstop_adj);   // 1 float
      #else
        EEPROM_WRITE(dummy);
      #endif

      #if ENABLED(Y_DUAL_ENDSTOPS)
        EEPROM_WRITE(endstops.y2_endstop_adj);   // 1 float
      #else
        EEPROM_WRITE(dummy);
      #endif

      #if Z_MULTI_ENDSTOPS
        EEPROM_WRITE(endstops.z2_endstop_adj);   // 1 float
      #else
        EEPROM_WRITE(dummy);
      #endif

      #if ENABLED(Z_TRIPLE_ENDSTOPS)
        EEPROM_WRITE(endstops.z3_endstop_adj);   // 1 float
      #else
        EEPROM_WRITE(dummy);
      #endif

    #endif

    _FIELD_TEST(lcd_preheat_hotend_temp);

    #if DISABLED(ULTIPANEL)
      constexpr int16_t lcd_preheat_hotend_temp[2] = { PREHEAT_1_TEMP_HOTEND, PREHEAT_2_TEMP_HOTEND },
                        lcd_preheat_bed_temp[2] = { PREHEAT_1_TEMP_BED, PREHEAT_2_TEMP_BED };
      constexpr uint8_t lcd_preheat_fan_speed[2] = { PREHEAT_1_FAN_SPEED, PREHEAT_2_FAN_SPEED };
    #endif

    EEPROM_WRITE(lcd_preheat_hotend_temp);
    EEPROM_WRITE(lcd_preheat_bed_temp);
    EEPROM_WRITE(lcd_preheat_fan_speed);

    //
    // PIDTEMP
    //
    {
      _FIELD_TEST(hotendPID);
      HOTEND_LOOP() {
        PIDC_t pidc = {
          PID_PARAM(Kp, e), PID_PARAM(Ki, e), PID_PARAM(Kd, e), PID_PARAM(Kc, e)
        };
        EEPROM_WRITE(pidc);
      }

      _FIELD_TEST(lpq_len);
      #if ENABLED(PID_EXTRUSION_SCALING)
        EEPROM_WRITE(thermalManager.lpq_len);
      #else
        const int16_t lpq_len = 20;
        EEPROM_WRITE(lpq_len);
      #endif
    }

    //
    // PIDTEMPBED
    //
    {
      _FIELD_TEST(bedPID);
      #if DISABLED(PIDTEMPBED)
        const PID_t bed_pid = { DUMMY_PID_VALUE, DUMMY_PID_VALUE, DUMMY_PID_VALUE };
        EEPROM_WRITE(bed_pid);
      #else
        EEPROM_WRITE(thermalManager.bed_pid);
      #endif
    }

    //
    // LCD Contrast
    //

    _FIELD_TEST(lcd_contrast);

    #if !HAS_LCD_CONTRAST
      const int16_t lcd_contrast = 32;
    #endif
    EEPROM_WRITE(lcd_contrast);

    //
    // Firmware Retraction
    //
    {
      _FIELD_TEST(fwretract_settings);

      #if ENABLED(FWRETRACT)
        EEPROM_WRITE(fwretract.settings);
      #else
        const fwretract_settings_t autoretract_defaults = { 3, 45, 0, 0, 0, 13, 0, 8 };
        EEPROM_WRITE(autoretract_defaults);
      #endif
      #if ENABLED(FWRETRACT) && ENABLED(FWRETRACT_AUTORETRACT)
        EEPROM_WRITE(fwretract.autoretract_enabled);
      #else
        const bool autoretract_enabled = false;
        EEPROM_WRITE(autoretract_enabled);
      #endif
    }

    //
    // Volumetric & Filament Size
    //
    {
      _FIELD_TEST(parser_volumetric_enabled);

      #if DISABLED(NO_VOLUMETRICS)

        EEPROM_WRITE(parser.volumetric_enabled);
        EEPROM_WRITE(planner.filament_size);

      #else

        const bool volumetric_enabled = false;
        dummy = DEFAULT_NOMINAL_FILAMENT_DIA;
        EEPROM_WRITE(volumetric_enabled);
        for (uint8_t q = EXTRUDERS; q--;) EEPROM_WRITE(dummy);

      #endif
    }

    //
    // TMC Configuration
    //
    {
      _FIELD_TEST(tmc_stepper_current);

      tmc_stepper_current_t tmc_stepper_current = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

      #if HAS_TRINAMIC
        #if AXIS_IS_TMC(X)
          tmc_stepper_current.X = stepperX.getMilliamps();
        #endif
        #if AXIS_IS_TMC(Y)
          tmc_stepper_current.Y = stepperY.getMilliamps();
        #endif
        #if AXIS_IS_TMC(Z)
          tmc_stepper_current.Z = stepperZ.getMilliamps();
        #endif
        #if AXIS_IS_TMC(X2)
          tmc_stepper_current.X2 = stepperX2.getMilliamps();
        #endif
        #if AXIS_IS_TMC(Y2)
          tmc_stepper_current.Y2 = stepperY2.getMilliamps();
        #endif
        #if AXIS_IS_TMC(Z2)
          tmc_stepper_current.Z2 = stepperZ2.getMilliamps();
        #endif
        #if AXIS_IS_TMC(Z3)
          tmc_stepper_current.Z3 = stepperZ3.getMilliamps();
        #endif
        #if MAX_EXTRUDERS
          #if AXIS_IS_TMC(E0)
            tmc_stepper_current.E0 = stepperE0.getMilliamps();
          #endif
          #if MAX_EXTRUDERS > 1
            #if AXIS_IS_TMC(E1)
              tmc_stepper_current.E1 = stepperE1.getMilliamps();
            #endif
            #if MAX_EXTRUDERS > 2
              #if AXIS_IS_TMC(E2)
                tmc_stepper_current.E2 = stepperE2.getMilliamps();
              #endif
              #if MAX_EXTRUDERS > 3
                #if AXIS_IS_TMC(E3)
                  tmc_stepper_current.E3 = stepperE3.getMilliamps();
                #endif
                #if MAX_EXTRUDERS > 4
                  #if AXIS_IS_TMC(E4)
                    tmc_stepper_current.E4 = stepperE4.getMilliamps();
                  #endif
                  #if MAX_EXTRUDERS > 5
                    #if AXIS_IS_TMC(E5)
                      tmc_stepper_current.E5 = stepperE5.getMilliamps();
                    #endif
                  #endif // MAX_EXTRUDERS > 5
                #endif // MAX_EXTRUDERS > 4
              #endif // MAX_EXTRUDERS > 3
            #endif // MAX_EXTRUDERS > 2
          #endif // MAX_EXTRUDERS > 1
        #endif // MAX_EXTRUDERS
      #endif
      EEPROM_WRITE(tmc_stepper_current);
    }

    //
    // TMC Hybrid Threshold, and placeholder values
    //
    {
      _FIELD_TEST(tmc_hybrid_threshold);

      #if ENABLED(HYBRID_THRESHOLD)
       tmc_hybrid_threshold_t tmc_hybrid_threshold = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        #if AXIS_HAS_STEALTHCHOP(X)
          tmc_hybrid_threshold.X = TMC_GET_PWMTHRS(X, X);
        #endif
        #if AXIS_HAS_STEALTHCHOP(Y)
          tmc_hybrid_threshold.Y = TMC_GET_PWMTHRS(Y, Y);
        #endif
        #if AXIS_HAS_STEALTHCHOP(Z)
          tmc_hybrid_threshold.Z = TMC_GET_PWMTHRS(Z, Z);
        #endif
        #if AXIS_HAS_STEALTHCHOP(X2)
          tmc_hybrid_threshold.X2 = TMC_GET_PWMTHRS(X, X2);
        #endif
        #if AXIS_HAS_STEALTHCHOP(Y2)
          tmc_hybrid_threshold.Y2 = TMC_GET_PWMTHRS(Y, Y2);
        #endif
        #if AXIS_HAS_STEALTHCHOP(Z2)
          tmc_hybrid_threshold.Z2 = TMC_GET_PWMTHRS(Z, Z2);
        #endif
        #if AXIS_HAS_STEALTHCHOP(Z3)
          tmc_hybrid_threshold.Z3 = TMC_GET_PWMTHRS(Z, Z3);
        #endif
        #if MAX_EXTRUDERS
          #if AXIS_HAS_STEALTHCHOP(E0)
            tmc_hybrid_threshold.E0 = TMC_GET_PWMTHRS(E, E0);
          #endif
          #if MAX_EXTRUDERS > 1
            #if AXIS_HAS_STEALTHCHOP(E1)
              tmc_hybrid_threshold.E1 = TMC_GET_PWMTHRS(E, E1);
            #endif
            #if MAX_EXTRUDERS > 2
              #if AXIS_HAS_STEALTHCHOP(E2)
                tmc_hybrid_threshold.E2 = TMC_GET_PWMTHRS(E, E2);
              #endif
              #if MAX_EXTRUDERS > 3
                #if AXIS_HAS_STEALTHCHOP(E3)
                  tmc_hybrid_threshold.E3 = TMC_GET_PWMTHRS(E, E3);
                #endif
                #if MAX_EXTRUDERS > 4
                  #if AXIS_HAS_STEALTHCHOP(E4)
                    tmc_hybrid_threshold.E4 = TMC_GET_PWMTHRS(E, E4);
                  #endif
                  #if MAX_EXTRUDERS > 5
                    #if AXIS_HAS_STEALTHCHOP(E5)
                      tmc_hybrid_threshold.E5 = TMC_GET_PWMTHRS(E, E5);
                    #endif
                  #endif // MAX_EXTRUDERS > 5
                #endif // MAX_EXTRUDERS > 4
              #endif // MAX_EXTRUDERS > 3
            #endif // MAX_EXTRUDERS > 2
          #endif // MAX_EXTRUDERS > 1
        #endif // MAX_EXTRUDERS
      #else
        const tmc_hybrid_threshold_t tmc_hybrid_threshold = {
          .X  = 100, .Y  = 100, .Z  =   3,
          .X2 = 100, .Y2 = 100, .Z2 =   3, .Z3 =   3,
          .E0 =  30, .E1 =  30, .E2 =  30,
          .E3 =  30, .E4 =  30, .E5 =  30
        };
      #endif
      EEPROM_WRITE(tmc_hybrid_threshold);
    }

    //
    // TMC StallGuard threshold
    //
    {
      tmc_sgt_t tmc_sgt = { 0, 0, 0 };
      #if USE_SENSORLESS
        #if X_SENSORLESS
          tmc_sgt.X = stepperX.sgt();
        #endif
        #if Y_SENSORLESS
          tmc_sgt.Y = stepperY.sgt();
        #endif
        #if Z_SENSORLESS
          tmc_sgt.Z = stepperZ.sgt();
        #endif
      #endif
      EEPROM_WRITE(tmc_sgt);
    }

    //
    // Linear Advance
    //
    {
      _FIELD_TEST(planner_extruder_advance_K);

      #if ENABLED(LIN_ADVANCE)
        EEPROM_WRITE(planner.extruder_advance_K);
      #else
        dummy = 0;
        for (uint8_t q = EXTRUDERS; q--;) EEPROM_WRITE(dummy);
      #endif
    }

    //
    // Motor Current PWM
    //
    {
      _FIELD_TEST(motor_current_setting);

      #if HAS_MOTOR_CURRENT_PWM
        EEPROM_WRITE(stepper.motor_current_setting);
      #else
        const uint32_t dummyui32[XYZ] = { 0 };
        EEPROM_WRITE(dummyui32);
      #endif
    }

    //
    // CNC Coordinate Systems
    //

    _FIELD_TEST(coordinate_system);

    #if ENABLED(CNC_COORDINATE_SYSTEMS)
      EEPROM_WRITE(gcode.coordinate_system);
    #else
      const float coordinate_system[MAX_COORDINATE_SYSTEMS][XYZ] = { { 0 } };
      EEPROM_WRITE(coordinate_system);
    #endif

    //
    // Skew correction factors
    //
    _FIELD_TEST(planner_skew_factor);
    EEPROM_WRITE(planner.skew_factor);

    //
    // Advanced Pause filament load & unload lengths
    //
    {
      #if DISABLED(ADVANCED_PAUSE_FEATURE)
        const fil_change_settings_t fc_settings[EXTRUDERS] = { 0, 0 };
      #endif
      _FIELD_TEST(fc_settings);
      EEPROM_WRITE(fc_settings);
    }

    //
    // SINGLENOZZLE
    //

    #if EXTRUDERS > 1
      _FIELD_TEST(toolchange_settings);
      EEPROM_WRITE(toolchange_settings);
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
        SERIAL_ECHO_START_P(port);
        SERIAL_ECHOPAIR_P(port, "Settings Stored (", eeprom_size);
        SERIAL_ECHOPAIR_P(port, " bytes; crc ", (uint32_t)final_crc);
        SERIAL_ECHOLNPGM_P(port, ")");
      #endif

      eeprom_error |= size_error(eeprom_size);
    }
    EEPROM_FINISH();

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
  bool MarlinSettings::_load(PORTARG_SOLO) {
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
        SERIAL_ECHO_START_P(port);
        SERIAL_ECHOPGM_P(port, "EEPROM version mismatch ");
        SERIAL_ECHOPAIR_P(port, "(EEPROM=", stored_ver);
        SERIAL_ECHOLNPGM_P(port, " Marlin=" EEPROM_VERSION ")");
      #endif
      eeprom_error = true;
    }
    else {
      float dummy = 0;
      working_crc = 0;  // Init to 0. Accumulated by EEPROM_READ

      _FIELD_TEST(esteppers);

      // Number of esteppers may change
      uint8_t esteppers;
      EEPROM_READ_ALWAYS(esteppers);

      //
      // Planner Motion
      //
      {
        // Get only the number of E stepper parameters previously stored
        // Any steppers added later are set to their defaults
        const uint32_t def1[] = DEFAULT_MAX_ACCELERATION;
        const float def2[] = DEFAULT_AXIS_STEPS_PER_UNIT, def3[] = DEFAULT_MAX_FEEDRATE;

        uint32_t tmp1[XYZ + esteppers];
        EEPROM_READ(tmp1);                         // max_acceleration_mm_per_s2
        EEPROM_READ(planner.settings.min_segment_time_us);

        float tmp2[XYZ + esteppers], tmp3[XYZ + esteppers];
        EEPROM_READ(tmp2);                         // axis_steps_per_mm
        EEPROM_READ(tmp3);                         // max_feedrate_mm_s
        if (!validating) LOOP_XYZE_N(i) {
          const bool in = (i < esteppers + XYZ);
          planner.settings.max_acceleration_mm_per_s2[i] = in ? tmp1[i] : def1[ALIM(i, def1)];
          planner.settings.axis_steps_per_mm[i]          = in ? tmp2[i] : def2[ALIM(i, def2)];
          planner.settings.max_feedrate_mm_s[i]          = in ? tmp3[i] : def3[ALIM(i, def3)];
        }

        EEPROM_READ(planner.settings.acceleration);
        EEPROM_READ(planner.settings.retract_acceleration);
        EEPROM_READ(planner.settings.travel_acceleration);
        EEPROM_READ(planner.settings.min_feedrate_mm_s);
        EEPROM_READ(planner.settings.min_travel_feedrate_mm_s);

        #if HAS_CLASSIC_JERK
          EEPROM_READ(planner.max_jerk);
          #if ENABLED(JUNCTION_DEVIATION) && ENABLED(LIN_ADVANCE)
            EEPROM_READ(dummy);
          #endif
        #else
          for (uint8_t q = 4; q--;) EEPROM_READ(dummy);
        #endif

        #if ENABLED(JUNCTION_DEVIATION)
          EEPROM_READ(planner.junction_deviation_mm);
        #else
          EEPROM_READ(dummy);
        #endif
      }

      //
      // Home Offset (M206)
      //
      {
        _FIELD_TEST(home_offset);

        #if !HAS_HOME_OFFSET
          float home_offset[XYZ];
        #endif
        EEPROM_READ(home_offset);
      }

      //
      // Hotend Offsets, if any
      //
      {
        #if HAS_HOTEND_OFFSET
          // Skip hotend 0 which must be 0
          for (uint8_t e = 1; e < HOTENDS; e++)
            LOOP_XYZ(i) EEPROM_READ(hotend_offset[i][e]);
        #endif
      }

      //
      // Global Leveling
      //
      {
        #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
          EEPROM_READ(new_z_fade_height);
        #else
          EEPROM_READ(dummy);
        #endif
      }

      //
      // Mesh (Manual) Bed Leveling
      //
      {
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
      }

      //
      // Probe Z Offset
      //
      {
        _FIELD_TEST(zprobe_zoffset);

        #if !HAS_BED_PROBE
          float zprobe_zoffset;
        #endif
        EEPROM_READ(zprobe_zoffset);
      }

      //
      // Planar Bed Leveling matrix
      //
      {
        #if ABL_PLANAR
          EEPROM_READ(planner.bed_level_matrix);
        #else
          for (uint8_t q = 9; q--;) EEPROM_READ(dummy);
        #endif
      }

      //
      // Bilinear Auto Bed Leveling
      //
      {
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
      }

      //
      // Unified Bed Leveling active state
      //
      {
        _FIELD_TEST(planner_leveling_active);

        #if ENABLED(AUTO_BED_LEVELING_UBL)
          EEPROM_READ(planner.leveling_active);
          EEPROM_READ(ubl.storage_slot);
        #else
          bool planner_leveling_active;
          uint8_t ubl_storage_slot;
          EEPROM_READ(planner_leveling_active);
          EEPROM_READ(ubl_storage_slot);
        #endif
      }

      //
      // SERVO_ANGLES
      //
      {
        #if !HAS_SERVOS || DISABLED(EDITABLE_SERVO_ANGLES)
          uint16_t servo_angles[NUM_SERVOS][2];
        #endif
        EEPROM_READ(servo_angles);
      }

      //
      // DELTA Geometry or Dual Endstops offsets
      //
      {
        #if ENABLED(DELTA)

          _FIELD_TEST(delta_height);

          EEPROM_READ(delta_height);              // 1 float
          EEPROM_READ(delta_endstop_adj);         // 3 floats
          EEPROM_READ(delta_radius);              // 1 float
          EEPROM_READ(delta_diagonal_rod);        // 1 float
          EEPROM_READ(delta_segments_per_second); // 1 float
          EEPROM_READ(delta_calibration_radius);  // 1 float
          EEPROM_READ(delta_tower_angle_trim);    // 3 floats

        #elif ENABLED(X_DUAL_ENDSTOPS) || ENABLED(Y_DUAL_ENDSTOPS) || Z_MULTI_ENDSTOPS

          _FIELD_TEST(x2_endstop_adj);

          #if ENABLED(X_DUAL_ENDSTOPS)
            EEPROM_READ(endstops.x2_endstop_adj);  // 1 float
          #else
            EEPROM_READ(dummy);
          #endif
          #if ENABLED(Y_DUAL_ENDSTOPS)
            EEPROM_READ(endstops.y2_endstop_adj);  // 1 float
          #else
            EEPROM_READ(dummy);
          #endif
          #if Z_MULTI_ENDSTOPS
            EEPROM_READ(endstops.z2_endstop_adj); // 1 float
          #else
            EEPROM_READ(dummy);
          #endif
          #if ENABLED(Z_TRIPLE_ENDSTOPS)
            EEPROM_READ(endstops.z3_endstop_adj); // 1 float
          #else
            EEPROM_READ(dummy);
          #endif

        #endif
      }

      //
      // LCD Preheat settings
      //
      {
        _FIELD_TEST(lcd_preheat_hotend_temp);

        #if DISABLED(ULTIPANEL)
          int16_t lcd_preheat_hotend_temp[2], lcd_preheat_bed_temp[2];
          uint8_t lcd_preheat_fan_speed[2];
        #endif
        EEPROM_READ(lcd_preheat_hotend_temp); // 2 floats
        EEPROM_READ(lcd_preheat_bed_temp);    // 2 floats
        EEPROM_READ(lcd_preheat_fan_speed);   // 2 floats
      }

      //
      // Hotend PID
      //
      {
        HOTEND_LOOP() {
          PIDC_t pidc;
          EEPROM_READ(pidc);
          #if ENABLED(PIDTEMP)
            if (!validating && pidc.Kp != DUMMY_PID_VALUE) {
              // No need to scale PID values since EEPROM values are scaled
              PID_PARAM(Kp, e) = pidc.Kp;
              PID_PARAM(Ki, e) = pidc.Ki;
              PID_PARAM(Kd, e) = pidc.Kd;
              #if ENABLED(PID_EXTRUSION_SCALING)
                PID_PARAM(Kc, e) = pidc.Kc;
              #endif
            }
          #endif
        }
      }

      //
      // PID Extrusion Scaling
      //
      {
        _FIELD_TEST(lpq_len);
        #if ENABLED(PID_EXTRUSION_SCALING)
          EEPROM_READ(thermalManager.lpq_len);
        #else
          int16_t lpq_len;
          EEPROM_READ(lpq_len);
        #endif
      }

      //
      // Heated Bed PID
      //
      {
        PID_t pid;
        EEPROM_READ(pid);
        #if ENABLED(PIDTEMPBED)
          if (!validating && pid.Kp != DUMMY_PID_VALUE)
            memcpy(&thermalManager.bed_pid, &pid, sizeof(pid));
        #endif
      }

      //
      // LCD Contrast
      //
      {
        _FIELD_TEST(lcd_contrast);
        #if !HAS_LCD_CONTRAST
          int16_t lcd_contrast;
        #endif
        EEPROM_READ(lcd_contrast);
      }

      //
      // Firmware Retraction
      //
      {
        _FIELD_TEST(fwretract_settings);

        #if ENABLED(FWRETRACT)
          EEPROM_READ(fwretract.settings);
        #else
          fwretract_settings_t fwretract_settings;
          EEPROM_READ(fwretract_settings);
        #endif
        #if ENABLED(FWRETRACT) && ENABLED(FWRETRACT_AUTORETRACT)
          EEPROM_READ(fwretract.autoretract_enabled);
        #else
          bool autoretract_enabled;
          EEPROM_READ(autoretract_enabled);
        #endif
      }

      //
      // Volumetric & Filament Size
      //
      {
        struct {
          bool volumetric_enabled;
          float filament_size[EXTRUDERS];
        } storage;

        _FIELD_TEST(parser_volumetric_enabled);
        EEPROM_READ(storage);

        #if DISABLED(NO_VOLUMETRICS)
          if (!validating) {
            parser.volumetric_enabled = storage.volumetric_enabled;
            COPY(planner.filament_size, storage.filament_size);
          }
        #endif
      }

      //
      // TMC Stepper Settings
      //

      if (!validating) reset_stepper_drivers();

      // TMC Stepper Current
      {
        _FIELD_TEST(tmc_stepper_current);

        tmc_stepper_current_t currents;
        EEPROM_READ(currents);

        #if HAS_TRINAMIC

          #define SET_CURR(Q) stepper##Q.rms_current(currents.Q ? currents.Q : Q##_CURRENT)
          if (!validating) {
            #if AXIS_IS_TMC(X)
              SET_CURR(X);
            #endif
            #if AXIS_IS_TMC(Y)
              SET_CURR(Y);
            #endif
            #if AXIS_IS_TMC(Z)
              SET_CURR(Z);
            #endif
            #if AXIS_IS_TMC(X2)
              SET_CURR(X2);
            #endif
            #if AXIS_IS_TMC(Y2)
              SET_CURR(Y2);
            #endif
            #if AXIS_IS_TMC(Z2)
              SET_CURR(Z2);
            #endif
            #if AXIS_IS_TMC(Z3)
              SET_CURR(Z3);
            #endif
            #if AXIS_IS_TMC(E0)
              SET_CURR(E0);
            #endif
            #if AXIS_IS_TMC(E1)
              SET_CURR(E1);
            #endif
            #if AXIS_IS_TMC(E2)
              SET_CURR(E2);
            #endif
            #if AXIS_IS_TMC(E3)
              SET_CURR(E3);
            #endif
            #if AXIS_IS_TMC(E4)
              SET_CURR(E4);
            #endif
            #if AXIS_IS_TMC(E5)
              SET_CURR(E5);
            #endif
          }
        #endif
      }

      // TMC Hybrid Threshold
      {
        tmc_hybrid_threshold_t tmc_hybrid_threshold;
        _FIELD_TEST(tmc_hybrid_threshold);
        EEPROM_READ(tmc_hybrid_threshold);

        #if ENABLED(HYBRID_THRESHOLD)
          #define TMC_SET_PWMTHRS(A,Q) tmc_set_pwmthrs(stepper##Q, tmc_hybrid_threshold.Q, planner.settings.axis_steps_per_mm[_AXIS(A)])
          if (!validating) {
            #if AXIS_HAS_STEALTHCHOP(X)
              TMC_SET_PWMTHRS(X, X);
            #endif
            #if AXIS_HAS_STEALTHCHOP(Y)
              TMC_SET_PWMTHRS(Y, Y);
            #endif
            #if AXIS_HAS_STEALTHCHOP(Z)
              TMC_SET_PWMTHRS(Z, Z);
            #endif
            #if AXIS_HAS_STEALTHCHOP(X2)
              TMC_SET_PWMTHRS(X, X2);
            #endif
            #if AXIS_HAS_STEALTHCHOP(Y2)
              TMC_SET_PWMTHRS(Y, Y2);
            #endif
            #if AXIS_HAS_STEALTHCHOP(Z2)
              TMC_SET_PWMTHRS(Z, Z2);
            #endif
            #if AXIS_HAS_STEALTHCHOP(Z3)
              TMC_SET_PWMTHRS(Z, Z3);
            #endif
            #if AXIS_HAS_STEALTHCHOP(E0)
              TMC_SET_PWMTHRS(E, E0);
            #endif
            #if AXIS_HAS_STEALTHCHOP(E1)
              TMC_SET_PWMTHRS(E, E1);
            #endif
            #if AXIS_HAS_STEALTHCHOP(E2)
              TMC_SET_PWMTHRS(E, E2);
            #endif
            #if AXIS_HAS_STEALTHCHOP(E3)
              TMC_SET_PWMTHRS(E, E3);
            #endif
            #if AXIS_HAS_STEALTHCHOP(E4)
              TMC_SET_PWMTHRS(E, E4);
            #endif
            #if AXIS_HAS_STEALTHCHOP(E5)
              TMC_SET_PWMTHRS(E, E5);
            #endif
          }
        #endif
      }

      //
      // TMC StallGuard threshold.
      // X and X2 use the same value
      // Y and Y2 use the same value
      // Z, Z2 and Z3 use the same value
      //
      {
        tmc_sgt_t tmc_sgt;
        _FIELD_TEST(tmc_sgt);
        EEPROM_READ(tmc_sgt);
        #if USE_SENSORLESS
          if (!validating) {
            #ifdef X_STALL_SENSITIVITY
              #if AXIS_HAS_STALLGUARD(X)
                stepperX.sgt(tmc_sgt.X);
              #endif
              #if AXIS_HAS_STALLGUARD(X2)
                stepperX2.sgt(tmc_sgt.X);
              #endif
            #endif
            #ifdef Y_STALL_SENSITIVITY
              #if AXIS_HAS_STALLGUARD(Y)
                stepperY.sgt(tmc_sgt.Y);
              #endif
              #if AXIS_HAS_STALLGUARD(Y2)
                stepperY2.sgt(tmc_sgt.Y);
              #endif
            #endif
            #ifdef Z_STALL_SENSITIVITY
              #if AXIS_HAS_STALLGUARD(Z)
                stepperZ.sgt(tmc_sgt.Z);
              #endif
              #if AXIS_HAS_STALLGUARD(Z2)
                stepperZ2.sgt(tmc_sgt.Z);
              #endif
              #if AXIS_HAS_STALLGUARD(Z3)
                stepperZ3.sgt(tmc_sgt.Z);
              #endif
            #endif
          }
        #endif
      }

      //
      // Linear Advance
      //
      {
        float extruder_advance_K[EXTRUDERS];
        _FIELD_TEST(planner_extruder_advance_K);
        EEPROM_READ(extruder_advance_K);
        #if ENABLED(LIN_ADVANCE)
          if (!validating)
            COPY(planner.extruder_advance_K, extruder_advance_K);
        #endif
      }

      //
      // Motor Current PWM
      //
      {
        uint32_t motor_current_setting[3];
        _FIELD_TEST(motor_current_setting);
        EEPROM_READ(motor_current_setting);
        #if HAS_MOTOR_CURRENT_PWM
          if (!validating)
            COPY(stepper.motor_current_setting, motor_current_setting);
        #endif
      }

      //
      // CNC Coordinate System
      //
      {
        _FIELD_TEST(coordinate_system);
        #if ENABLED(CNC_COORDINATE_SYSTEMS)
          if (!validating) (void)gcode.select_coordinate_system(-1); // Go back to machine space
          EEPROM_READ(gcode.coordinate_system);
        #else
          float coordinate_system[MAX_COORDINATE_SYSTEMS][XYZ];
          EEPROM_READ(coordinate_system);
        #endif
      }

      //
      // Skew correction factors
      //
      {
        skew_factor_t skew_factor;
        _FIELD_TEST(planner_skew_factor);
        EEPROM_READ(skew_factor);
        #if ENABLED(SKEW_CORRECTION_GCODE)
          if (!validating) {
            planner.skew_factor.xy = skew_factor.xy;
            #if ENABLED(SKEW_CORRECTION_FOR_Z)
              planner.skew_factor.xz = skew_factor.xz;
              planner.skew_factor.yz = skew_factor.yz;
            #endif
          }
        #endif
      }

      //
      // Advanced Pause filament load & unload lengths
      //
      {
        #if DISABLED(ADVANCED_PAUSE_FEATURE)
          fil_change_settings_t fc_settings[EXTRUDERS];
        #endif
        _FIELD_TEST(fc_settings);
        EEPROM_READ(fc_settings);
      }

      //
      // SINGLENOZZLE toolchange values
      //
      #if EXTRUDERS > 1
        _FIELD_TEST(toolchange_settings);
        EEPROM_READ(toolchange_settings);
      #endif

      eeprom_error = size_error(eeprom_index - (EEPROM_OFFSET));
      if (eeprom_error) {
        #if ENABLED(EEPROM_CHITCHAT)
          SERIAL_ECHO_START_P(port);
          SERIAL_ECHOPAIR_P(port, "Index: ", int(eeprom_index - (EEPROM_OFFSET)));
          SERIAL_ECHOLNPAIR_P(port, " Size: ", datasize());
        #endif
      }
      else if (working_crc != stored_crc) {
        eeprom_error = true;
        #if ENABLED(EEPROM_CHITCHAT)
          SERIAL_ERROR_START_P(port);
          SERIAL_ERRORPGM_P(port, "EEPROM CRC mismatch - (stored) ");
          SERIAL_ERROR_P(port, stored_crc);
          SERIAL_ERRORPGM_P(port, " != ");
          SERIAL_ERROR_P(port, working_crc);
          SERIAL_ERRORLNPGM_P(port, " (calculated)!");
        #endif
      }
      else if (!validating) {
        #if ENABLED(EEPROM_CHITCHAT)
          SERIAL_ECHO_START_P(port);
          SERIAL_ECHO_P(port, version);
          SERIAL_ECHOPAIR_P(port, " stored settings retrieved (", eeprom_index - (EEPROM_OFFSET));
          SERIAL_ECHOPAIR_P(port, " bytes; crc ", (uint32_t)working_crc);
          SERIAL_ECHOLNPGM_P(port, ")");
        #endif
      }

      if (!validating && !eeprom_error) postprocess();

      #if ENABLED(AUTO_BED_LEVELING_UBL)
        if (!validating) {
          ubl.report_state();

          if (!ubl.sanity_check()) {
            SERIAL_EOL_P(port);
            #if ENABLED(EEPROM_CHITCHAT)
              ubl.echo_name();
              SERIAL_ECHOLNPGM_P(port, " initialized.\n");
            #endif
          }
          else {
            eeprom_error = true;
            #if ENABLED(EEPROM_CHITCHAT)
              SERIAL_PROTOCOLPGM_P(port, "?Can't enable ");
              ubl.echo_name();
              SERIAL_PROTOCOLLNPGM_P(port, ".");
            #endif
            ubl.reset();
          }

          if (ubl.storage_slot >= 0) {
            load_mesh(ubl.storage_slot);
            #if ENABLED(EEPROM_CHITCHAT)
              SERIAL_ECHOPAIR_P(port, "Mesh ", ubl.storage_slot);
              SERIAL_ECHOLNPGM_P(port, " loaded from storage.");
            #endif
          }
          else {
            ubl.reset();
            #if ENABLED(EEPROM_CHITCHAT)
              SERIAL_ECHOLNPGM_P(port, "UBL System reset()");
            #endif
          }
        }
      #endif
    }

    #if ENABLED(EEPROM_CHITCHAT) && DISABLED(DISABLE_M503)
      if (!validating) report(PORTVAR_SOLO);
    #endif
    EEPROM_FINISH();

    return !eeprom_error;
  }

  bool MarlinSettings::validate(PORTARG_SOLO) {
    validating = true;
    const bool success = _load(PORTVAR_SOLO);
    validating = false;
    return success;
  }

  bool MarlinSettings::load(PORTARG_SOLO) {
    if (validate(PORTVAR_SOLO)) return _load(PORTVAR_SOLO);
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

    const uint16_t MarlinSettings::meshes_end = persistentStore.capacity() - 129; // 128 (+1 because of the change to capacity rather than last valid address)
                                                                                  // is a placeholder for the size of the MAT; the MAT will always
                                                                                  // live at the very end of the eeprom

    uint16_t MarlinSettings::meshes_start_index() {
      return (datasize() + EEPROM_OFFSET + 32) & 0xFFF8;  // Pad the end of configuration data so it can float up
                                                          // or down a little bit without disrupting the mesh data
    }

    uint16_t MarlinSettings::calc_num_meshes() {
      return (meshes_end - meshes_start_index()) / sizeof(ubl.z_values);
    }

    int MarlinSettings::mesh_slot_offset(const int8_t slot) {
      return meshes_end - (slot + 1) * sizeof(ubl.z_values);
    }

    void MarlinSettings::store_mesh(const int8_t slot) {

      #if ENABLED(AUTO_BED_LEVELING_UBL)
        const int16_t a = calc_num_meshes();
        if (!WITHIN(slot, 0, a - 1)) {
          #if ENABLED(EEPROM_CHITCHAT)
            ubl_invalid_slot(a);
            SERIAL_PROTOCOLPAIR("E2END=", persistentStore.capacity() - 1);
            SERIAL_PROTOCOLPAIR(" meshes_end=", meshes_end);
            SERIAL_PROTOCOLLNPAIR(" slot=", slot);
            SERIAL_EOL();
          #endif
          return;
        }

        int pos = mesh_slot_offset(slot);
        uint16_t crc = 0;

        persistentStore.access_start();
        const bool status = persistentStore.write_data(pos, (uint8_t *)&ubl.z_values, sizeof(ubl.z_values), &crc);
        persistentStore.access_finish();

        if (status)
          SERIAL_PROTOCOLPGM("?Unable to save mesh data.\n");

        // Write crc to MAT along with other data, or just tack on to the beginning or end

        #if ENABLED(EEPROM_CHITCHAT)
          if (!status)
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

        int pos = mesh_slot_offset(slot);
        uint16_t crc = 0;
        uint8_t * const dest = into ? (uint8_t*)into : (uint8_t*)&ubl.z_values;

        persistentStore.access_start();
        const uint16_t status = persistentStore.read_data(pos, dest, sizeof(ubl.z_values), &crc);
        persistentStore.access_finish();

        if (status)
          SERIAL_PROTOCOLPGM("?Unable to load mesh data.\n");

        #if ENABLED(EEPROM_CHITCHAT)
          else
            SERIAL_PROTOCOLLNPAIR("Mesh loaded from slot ", slot);
        #endif
        EEPROM_FINISH();

      #else

        // Other mesh types

      #endif
    }

    //void MarlinSettings::delete_mesh() { return; }
    //void MarlinSettings::defrag_meshes() { return; }

  #endif // AUTO_BED_LEVELING_UBL

#else // !EEPROM_SETTINGS

  bool MarlinSettings::save(PORTARG_SOLO) {
    #if ENABLED(EEPROM_CHITCHAT)
      SERIAL_ERROR_START_P(port);
      SERIAL_ERRORLNPGM_P(port, "EEPROM disabled");
    #endif
    return false;
  }

#endif // !EEPROM_SETTINGS

/**
 * M502 - Reset Configuration
 */
void MarlinSettings::reset(PORTARG_SOLO) {
  static const float tmp1[] PROGMEM = DEFAULT_AXIS_STEPS_PER_UNIT, tmp2[] PROGMEM = DEFAULT_MAX_FEEDRATE;
  static const uint32_t tmp3[] PROGMEM = DEFAULT_MAX_ACCELERATION;
  LOOP_XYZE_N(i) {
    planner.settings.axis_steps_per_mm[i]          = pgm_read_float(&tmp1[ALIM(i, tmp1)]);
    planner.settings.max_feedrate_mm_s[i]          = pgm_read_float(&tmp2[ALIM(i, tmp2)]);
    planner.settings.max_acceleration_mm_per_s2[i] = pgm_read_dword(&tmp3[ALIM(i, tmp3)]);
  }

  planner.settings.min_segment_time_us = DEFAULT_MINSEGMENTTIME;
  planner.settings.acceleration = DEFAULT_ACCELERATION;
  planner.settings.retract_acceleration = DEFAULT_RETRACT_ACCELERATION;
  planner.settings.travel_acceleration = DEFAULT_TRAVEL_ACCELERATION;
  planner.settings.min_feedrate_mm_s = DEFAULT_MINIMUMFEEDRATE;
  planner.settings.min_travel_feedrate_mm_s = DEFAULT_MINTRAVELFEEDRATE;

  #if HAS_CLASSIC_JERK
    planner.max_jerk[X_AXIS] = DEFAULT_XJERK;
    planner.max_jerk[Y_AXIS] = DEFAULT_YJERK;
    planner.max_jerk[Z_AXIS] = DEFAULT_ZJERK;
    #if DISABLED(JUNCTION_DEVIATION) || DISABLED(LIN_ADVANCE)
      planner.max_jerk[E_AXIS] = DEFAULT_EJERK;
    #endif
  #endif

  #if ENABLED(JUNCTION_DEVIATION)
    planner.junction_deviation_mm = float(JUNCTION_DEVIATION_MM);
  #endif

  #if HAS_HOME_OFFSET
    ZERO(home_offset);
  #endif

  #if HAS_HOTEND_OFFSET
    constexpr float tmp4[XYZ][HOTENDS] = { HOTEND_OFFSET_X, HOTEND_OFFSET_Y, HOTEND_OFFSET_Z };
    static_assert(
      tmp4[X_AXIS][0] == 0 && tmp4[Y_AXIS][0] == 0 && tmp4[Z_AXIS][0] == 0,
      "Offsets for the first hotend must be 0.0."
    );
    LOOP_XYZ(i) HOTEND_LOOP() hotend_offset[i][e] = tmp4[i][e];
    #if ENABLED(DUAL_X_CARRIAGE)
      hotend_offset[X_AXIS][1] = MAX(X2_HOME_POS, X2_MAX_POS);
    #endif
  #endif

  #if EXTRUDERS > 1
    #if ENABLED(SINGLENOZZLE)
      toolchange_settings.swap_length = SINGLENOZZLE_SWAP_LENGTH;
      toolchange_settings.prime_speed = SINGLENOZZLE_SWAP_PRIME_SPEED;
      toolchange_settings.retract_speed = SINGLENOZZLE_SWAP_RETRACT_SPEED;
      #if ENABLED(SINGLENOZZLE_SWAP_PARK)
        toolchange_settings.change_point = SINGLENOZZLE_TOOLCHANGE_XY;
      #endif
    #endif
    toolchange_settings.z_raise = TOOLCHANGE_ZRAISE;
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

  //
  // Servo Angles
  //

  #if HAS_SERVOS && ENABLED(EDITABLE_SERVO_ANGLES)

    #if ENABLED(SWITCHING_EXTRUDER)

      #if EXTRUDERS > 3
        #define REQ_ANGLES 4
      #else
        #define REQ_ANGLES 2
      #endif
      constexpr uint16_t extruder_angles[] = SWITCHING_EXTRUDER_SERVO_ANGLES;
      static_assert(COUNT(extruder_angles) == REQ_ANGLES, "SWITCHING_EXTRUDER_SERVO_ANGLES needs " STRINGIFY(REQ_ANGLES) " angles.");
      servo_angles[SWITCHING_EXTRUDER_SERVO_NR][0] = extruder_angles[0];
      servo_angles[SWITCHING_EXTRUDER_SERVO_NR][1] = extruder_angles[1];
      #if EXTRUDERS > 3
        servo_angles[SWITCHING_EXTRUDER_E23_SERVO_NR][0] = extruder_angles[2];
        servo_angles[SWITCHING_EXTRUDER_E23_SERVO_NR][1] = extruder_angles[3];
      #endif

    #elif ENABLED(SWITCHING_NOZZLE)

      constexpr uint16_t nozzle_angles[2] = SWITCHING_NOZZLE_SERVO_ANGLES;
      servo_angles[SWITCHING_NOZZLE_SERVO_NR][0] = nozzle_angles[0];
      servo_angles[SWITCHING_NOZZLE_SERVO_NR][1] = nozzle_angles[1];

    #elif defined(Z_SERVO_ANGLES) && defined(Z_PROBE_SERVO_NR)

      constexpr uint16_t z_probe_angles[2] = Z_SERVO_ANGLES;
      servo_angles[Z_PROBE_SERVO_NR][0] = z_probe_angles[0];
      servo_angles[Z_PROBE_SERVO_NR][1] = z_probe_angles[1];

    #endif

  #endif // HAS_SERVOS && EDITABLE_SERVO_ANGLES

  #if ENABLED(DELTA)
    const float adj[ABC] = DELTA_ENDSTOP_ADJ, dta[ABC] = DELTA_TOWER_ANGLE_TRIM;
    delta_height = DELTA_HEIGHT;
    COPY(delta_endstop_adj, adj);
    delta_radius = DELTA_RADIUS;
    delta_diagonal_rod = DELTA_DIAGONAL_ROD;
    delta_segments_per_second = DELTA_SEGMENTS_PER_SECOND;
    delta_calibration_radius = DELTA_CALIBRATION_RADIUS;
    COPY(delta_tower_angle_trim, dta);

  #elif ENABLED(X_DUAL_ENDSTOPS) || ENABLED(Y_DUAL_ENDSTOPS) || Z_MULTI_ENDSTOPS

    #if ENABLED(X_DUAL_ENDSTOPS)
      endstops.x2_endstop_adj = (
        #ifdef X_DUAL_ENDSTOPS_ADJUSTMENT
          X_DUAL_ENDSTOPS_ADJUSTMENT
        #else
          0
        #endif
      );
    #endif
    #if ENABLED(Y_DUAL_ENDSTOPS)
      endstops.y2_endstop_adj = (
        #ifdef Y_DUAL_ENDSTOPS_ADJUSTMENT
          Y_DUAL_ENDSTOPS_ADJUSTMENT
        #else
          0
        #endif
      );
    #endif
    #if ENABLED(Z_DUAL_ENDSTOPS)
      endstops.z2_endstop_adj = (
        #ifdef Z_DUAL_ENDSTOPS_ADJUSTMENT
          Z_DUAL_ENDSTOPS_ADJUSTMENT
        #else
          0
        #endif
      );
    #elif ENABLED(Z_TRIPLE_ENDSTOPS)
      endstops.z2_endstop_adj = (
        #ifdef Z_TRIPLE_ENDSTOPS_ADJUSTMENT2
          Z_TRIPLE_ENDSTOPS_ADJUSTMENT2
        #else
          0
        #endif
      );
      endstops.z3_endstop_adj = (
        #ifdef Z_TRIPLE_ENDSTOPS_ADJUSTMENT3
          Z_TRIPLE_ENDSTOPS_ADJUSTMENT3
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
    HOTEND_LOOP() {
      PID_PARAM(Kp, e) = float(DEFAULT_Kp);
      PID_PARAM(Ki, e) = scalePID_i(DEFAULT_Ki);
      PID_PARAM(Kd, e) = scalePID_d(DEFAULT_Kd);
      #if ENABLED(PID_EXTRUSION_SCALING)
        PID_PARAM(Kc, e) = DEFAULT_Kc;
      #endif
    }
    #if ENABLED(PID_EXTRUSION_SCALING)
      thermalManager.lpq_len = 20; // default last-position-queue size
    #endif
  #endif // PIDTEMP

  #if ENABLED(PIDTEMPBED)
    thermalManager.bed_pid.Kp = DEFAULT_bedKp;
    thermalManager.bed_pid.Ki = scalePID_i(DEFAULT_bedKi);
    thermalManager.bed_pid.Kd = scalePID_d(DEFAULT_bedKd);
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

  reset_stepper_drivers();

  #if ENABLED(LIN_ADVANCE)
    LOOP_L_N(i, EXTRUDERS) planner.extruder_advance_K[i] = LIN_ADVANCE_K;
  #endif

  #if HAS_MOTOR_CURRENT_PWM
    uint32_t tmp_motor_current_setting[3] = PWM_MOTOR_CURRENT;
    for (uint8_t q = 3; q--;)
      stepper.digipot_current(q, (stepper.motor_current_setting[q] = tmp_motor_current_setting[q]));
  #endif

  #if ENABLED(SKEW_CORRECTION_GCODE)
    planner.skew_factor.xy = XY_SKEW_FACTOR;
    #if ENABLED(SKEW_CORRECTION_FOR_Z)
      planner.skew_factor.xz = XZ_SKEW_FACTOR;
      planner.skew_factor.yz = YZ_SKEW_FACTOR;
    #endif
  #endif

  #if ENABLED(ADVANCED_PAUSE_FEATURE)
    for (uint8_t e = 0; e < EXTRUDERS; e++) {
      fc_settings[e].unload_length = FILAMENT_CHANGE_UNLOAD_LENGTH;
      fc_settings[e].load_length = FILAMENT_CHANGE_FAST_LOAD_LENGTH;
    }
  #endif

  postprocess();

  #if ENABLED(EEPROM_CHITCHAT)
    SERIAL_ECHO_START_P(port);
    SERIAL_ECHOLNPGM_P(port, "Hardcoded Default Settings Loaded");
  #endif
}

#if DISABLED(DISABLE_M503)

  #define CONFIG_ECHO_START do{ if (!forReplay) SERIAL_ECHO_START_P(port); }while(0)

  #if HAS_TRINAMIC
    void say_M906(PORTARG_SOLO) { SERIAL_ECHOPGM_P(port, "  M906"); }
    #if ENABLED(HYBRID_THRESHOLD)
      void say_M913(PORTARG_SOLO) { SERIAL_ECHOPGM_P(port, "  M913"); }
    #endif
    #if USE_SENSORLESS
      void say_M914(PORTARG_SOLO) { SERIAL_ECHOPGM_P(port, "  M914"); }
    #endif
  #endif

  #if ENABLED(ADVANCED_PAUSE_FEATURE)
    void say_M603(PORTARG_SOLO) { SERIAL_ECHOPGM_P(port, "  M603 "); }
  #endif

  inline void say_units(
    #if NUM_SERIAL > 1
      const int8_t port,
    #endif
    const bool colon
  ) {
    serialprintPGM_P(port,
      #if ENABLED(INCH_MODE_SUPPORT)
        parser.linear_unit_factor != 1.0 ? PSTR(" (in)") :
      #endif
      PSTR(" (mm)")
    );
    if (colon) SERIAL_ECHOLNPGM_P(port, ":");
  }
  #if NUM_SERIAL > 1
    #define SAY_UNITS_P(PORT, COLON) say_units(PORT, COLON)
  #else
    #define SAY_UNITS_P(PORT, COLON) say_units(COLON)
  #endif

  /**
   * M503 - Report current settings in RAM
   *
   * Unless specifically disabled, M503 is available even without EEPROM
   */
  void MarlinSettings::report(const bool forReplay
    #if NUM_SERIAL > 1
      , const int8_t port/*=-1*/
    #endif
  ) {
    /**
     * Announce current units, in case inches are being displayed
     */
    CONFIG_ECHO_START;
    #if ENABLED(INCH_MODE_SUPPORT)
      SERIAL_ECHOPGM_P(port, "  G2");
      SERIAL_CHAR_P(port, parser.linear_unit_factor == 1.0 ? '1' : '0');
      SERIAL_ECHOPGM_P(port, " ;");
      SAY_UNITS_P(port, false);
    #else
      SERIAL_ECHOPGM_P(port, "  G21    ; Units in mm");
      SAY_UNITS_P(port, false);
    #endif
    SERIAL_EOL_P(port);

    #if ENABLED(ULTIPANEL)

      // Temperature units - for Ultipanel temperature options

      CONFIG_ECHO_START;
      #if ENABLED(TEMPERATURE_UNITS_SUPPORT)
        SERIAL_ECHOPGM_P(port, "  M149 ");
        SERIAL_CHAR_P(port, parser.temp_units_code());
        SERIAL_ECHOPGM_P(port, " ; Units in ");
        serialprintPGM_P(port, parser.temp_units_name());
      #else
        SERIAL_ECHOLNPGM_P(port, "  M149 C ; Units in Celsius");
      #endif

    #endif

    SERIAL_EOL_P(port);

    #if DISABLED(NO_VOLUMETRICS)

      /**
       * Volumetric extrusion M200
       */
      if (!forReplay) {
        CONFIG_ECHO_START;
        SERIAL_ECHOPGM_P(port, "Filament settings:");
        if (parser.volumetric_enabled)
          SERIAL_EOL_P(port);
        else
          SERIAL_ECHOLNPGM_P(port, " Disabled");
      }

      CONFIG_ECHO_START;
      SERIAL_ECHOPAIR_P(port, "  M200 D", LINEAR_UNIT(planner.filament_size[0]));
      SERIAL_EOL_P(port);
      #if EXTRUDERS > 1
        CONFIG_ECHO_START;
        SERIAL_ECHOPAIR_P(port, "  M200 T1 D", LINEAR_UNIT(planner.filament_size[1]));
        SERIAL_EOL_P(port);
        #if EXTRUDERS > 2
          CONFIG_ECHO_START;
          SERIAL_ECHOPAIR_P(port, "  M200 T2 D", LINEAR_UNIT(planner.filament_size[2]));
          SERIAL_EOL_P(port);
          #if EXTRUDERS > 3
            CONFIG_ECHO_START;
            SERIAL_ECHOPAIR_P(port, "  M200 T3 D", LINEAR_UNIT(planner.filament_size[3]));
            SERIAL_EOL_P(port);
            #if EXTRUDERS > 4
              CONFIG_ECHO_START;
              SERIAL_ECHOPAIR_P(port, "  M200 T4 D", LINEAR_UNIT(planner.filament_size[4]));
              SERIAL_EOL_P(port);
              #if EXTRUDERS > 5
                CONFIG_ECHO_START;
                SERIAL_ECHOPAIR_P(port, "  M200 T5 D", LINEAR_UNIT(planner.filament_size[5]));
                SERIAL_EOL_P(port);
              #endif // EXTRUDERS > 5
            #endif // EXTRUDERS > 4
          #endif // EXTRUDERS > 3
        #endif // EXTRUDERS > 2
      #endif // EXTRUDERS > 1

      if (!parser.volumetric_enabled) {
        CONFIG_ECHO_START;
        SERIAL_ECHOLNPGM_P(port, "  M200 D0");
      }

    #endif // !NO_VOLUMETRICS

    if (!forReplay) {
      CONFIG_ECHO_START;
      SERIAL_ECHOLNPGM_P(port, "Steps per unit:");
    }
    CONFIG_ECHO_START;
    SERIAL_ECHOPAIR_P(port, "  M92 X", LINEAR_UNIT(planner.settings.axis_steps_per_mm[X_AXIS]));
    SERIAL_ECHOPAIR_P(port, " Y", LINEAR_UNIT(planner.settings.axis_steps_per_mm[Y_AXIS]));
    SERIAL_ECHOPAIR_P(port, " Z", LINEAR_UNIT(planner.settings.axis_steps_per_mm[Z_AXIS]));
    #if DISABLED(DISTINCT_E_FACTORS)
      SERIAL_ECHOPAIR_P(port, " E", VOLUMETRIC_UNIT(planner.settings.axis_steps_per_mm[E_AXIS]));
    #endif
    SERIAL_EOL_P(port);
    #if ENABLED(DISTINCT_E_FACTORS)
      CONFIG_ECHO_START;
      for (uint8_t i = 0; i < E_STEPPERS; i++) {
        SERIAL_ECHOPAIR_P(port, "  M92 T", (int)i);
        SERIAL_ECHOLNPAIR_P(port, " E", VOLUMETRIC_UNIT(planner.settings.axis_steps_per_mm[E_AXIS + i]));
      }
    #endif

    if (!forReplay) {
      CONFIG_ECHO_START;
      SERIAL_ECHOLNPGM_P(port, "Maximum feedrates (units/s):");
    }
    CONFIG_ECHO_START;
    SERIAL_ECHOPAIR_P(port, "  M203 X", LINEAR_UNIT(planner.settings.max_feedrate_mm_s[X_AXIS]));
    SERIAL_ECHOPAIR_P(port, " Y", LINEAR_UNIT(planner.settings.max_feedrate_mm_s[Y_AXIS]));
    SERIAL_ECHOPAIR_P(port, " Z", LINEAR_UNIT(planner.settings.max_feedrate_mm_s[Z_AXIS]));
    #if DISABLED(DISTINCT_E_FACTORS)
      SERIAL_ECHOPAIR_P(port, " E", VOLUMETRIC_UNIT(planner.settings.max_feedrate_mm_s[E_AXIS]));
    #endif
    SERIAL_EOL_P(port);
    #if ENABLED(DISTINCT_E_FACTORS)
      CONFIG_ECHO_START;
      for (uint8_t i = 0; i < E_STEPPERS; i++) {
        SERIAL_ECHOPAIR_P(port, "  M203 T", (int)i);
        SERIAL_ECHOLNPAIR_P(port, " E", VOLUMETRIC_UNIT(planner.settings.max_feedrate_mm_s[E_AXIS + i]));
      }
    #endif

    if (!forReplay) {
      CONFIG_ECHO_START;
      SERIAL_ECHOLNPGM_P(port, "Maximum Acceleration (units/s2):");
    }
    CONFIG_ECHO_START;
    SERIAL_ECHOPAIR_P(port, "  M201 X", LINEAR_UNIT(planner.settings.max_acceleration_mm_per_s2[X_AXIS]));
    SERIAL_ECHOPAIR_P(port, " Y", LINEAR_UNIT(planner.settings.max_acceleration_mm_per_s2[Y_AXIS]));
    SERIAL_ECHOPAIR_P(port, " Z", LINEAR_UNIT(planner.settings.max_acceleration_mm_per_s2[Z_AXIS]));
    #if DISABLED(DISTINCT_E_FACTORS)
      SERIAL_ECHOPAIR_P(port, " E", VOLUMETRIC_UNIT(planner.settings.max_acceleration_mm_per_s2[E_AXIS]));
    #endif
    SERIAL_EOL_P(port);
    #if ENABLED(DISTINCT_E_FACTORS)
      CONFIG_ECHO_START;
      for (uint8_t i = 0; i < E_STEPPERS; i++) {
        SERIAL_ECHOPAIR_P(port, "  M201 T", (int)i);
        SERIAL_ECHOLNPAIR_P(port, " E", VOLUMETRIC_UNIT(planner.settings.max_acceleration_mm_per_s2[E_AXIS + i]));
      }
    #endif

    if (!forReplay) {
      CONFIG_ECHO_START;
      SERIAL_ECHOLNPGM_P(port, "Acceleration (units/s2): P<print_accel> R<retract_accel> T<travel_accel>");
    }
    CONFIG_ECHO_START;
    SERIAL_ECHOPAIR_P(port, "  M204 P", LINEAR_UNIT(planner.settings.acceleration));
    SERIAL_ECHOPAIR_P(port, " R", LINEAR_UNIT(planner.settings.retract_acceleration));
    SERIAL_ECHOLNPAIR_P(port, " T", LINEAR_UNIT(planner.settings.travel_acceleration));

    if (!forReplay) {
      CONFIG_ECHO_START;
      SERIAL_ECHOPGM_P(port, "Advanced: B<min_segment_time_us> S<min_feedrate> T<min_travel_feedrate>");
      #if ENABLED(JUNCTION_DEVIATION)
        SERIAL_ECHOPGM_P(port, " J<junc_dev>");
      #endif
      #if HAS_CLASSIC_JERK
        SERIAL_ECHOPGM_P(port, " X<max_x_jerk> Y<max_y_jerk> Z<max_z_jerk>");
        #if DISABLED(JUNCTION_DEVIATION) || DISABLED(LIN_ADVANCE)
          SERIAL_ECHOPGM_P(port, " E<max_e_jerk>");
        #endif
      #endif
      SERIAL_EOL_P(port);
    }
    CONFIG_ECHO_START;
    SERIAL_ECHOPAIR_P(port, "  M205 B", LINEAR_UNIT(planner.settings.min_segment_time_us));
    SERIAL_ECHOPAIR_P(port, " S", LINEAR_UNIT(planner.settings.min_feedrate_mm_s));
    SERIAL_ECHOPAIR_P(port, " T", LINEAR_UNIT(planner.settings.min_travel_feedrate_mm_s));

    #if ENABLED(JUNCTION_DEVIATION)
      SERIAL_ECHOPAIR_P(port, " J", LINEAR_UNIT(planner.junction_deviation_mm));
    #endif
    #if HAS_CLASSIC_JERK
      SERIAL_ECHOPAIR_P(port, " X", LINEAR_UNIT(planner.max_jerk[X_AXIS]));
      SERIAL_ECHOPAIR_P(port, " Y", LINEAR_UNIT(planner.max_jerk[Y_AXIS]));
      SERIAL_ECHOPAIR_P(port, " Z", LINEAR_UNIT(planner.max_jerk[Z_AXIS]));
      #if DISABLED(JUNCTION_DEVIATION) || DISABLED(LIN_ADVANCE)
        SERIAL_ECHOPAIR_P(port, " E", LINEAR_UNIT(planner.max_jerk[E_AXIS]));
      #endif
    #endif

    SERIAL_EOL_P(port);

    #if HAS_M206_COMMAND
      if (!forReplay) {
        CONFIG_ECHO_START;
        SERIAL_ECHOLNPGM_P(port, "Home offset:");
      }
      CONFIG_ECHO_START;
      SERIAL_ECHOPAIR_P(port, "  M206 X", LINEAR_UNIT(home_offset[X_AXIS]));
      SERIAL_ECHOPAIR_P(port, " Y", LINEAR_UNIT(home_offset[Y_AXIS]));
      SERIAL_ECHOLNPAIR_P(port, " Z", LINEAR_UNIT(home_offset[Z_AXIS]));
    #endif

    #if HAS_HOTEND_OFFSET
      if (!forReplay) {
        CONFIG_ECHO_START;
        SERIAL_ECHOLNPGM_P(port, "Hotend offsets:");
      }
      CONFIG_ECHO_START;
      for (uint8_t e = 1; e < HOTENDS; e++) {
        SERIAL_ECHOPAIR_P(port, "  M218 T", (int)e);
        SERIAL_ECHOPAIR_P(port, " X", LINEAR_UNIT(hotend_offset[X_AXIS][e]));
        SERIAL_ECHOPAIR_P(port, " Y", LINEAR_UNIT(hotend_offset[Y_AXIS][e]));
        SERIAL_ECHO_P(port, " Z");
        SERIAL_ECHO_F_P(port, LINEAR_UNIT(hotend_offset[Z_AXIS][e]), 3);
        SERIAL_EOL_P(port);
      }
    #endif

    /**
     * Bed Leveling
     */
    #if HAS_LEVELING

      #if ENABLED(MESH_BED_LEVELING)

        if (!forReplay) {
          CONFIG_ECHO_START;
          SERIAL_ECHOLNPGM_P(port, "Mesh Bed Leveling:");
        }

      #elif ENABLED(AUTO_BED_LEVELING_UBL)

        if (!forReplay) {
          CONFIG_ECHO_START;
          ubl.echo_name();
          SERIAL_ECHOLNPGM_P(port, ":");
        }

      #elif HAS_ABL

        if (!forReplay) {
          CONFIG_ECHO_START;
          SERIAL_ECHOLNPGM_P(port, "Auto Bed Leveling:");
        }

      #endif

      CONFIG_ECHO_START;
      SERIAL_ECHOPAIR_P(port, "  M420 S", planner.leveling_active ? 1 : 0);
      #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
        SERIAL_ECHOPAIR_P(port, " Z", LINEAR_UNIT(planner.z_fade_height));
      #endif
      SERIAL_EOL_P(port);

      #if ENABLED(MESH_BED_LEVELING)

        if (leveling_is_valid()) {
          for (uint8_t py = 0; py < GRID_MAX_POINTS_Y; py++) {
            for (uint8_t px = 0; px < GRID_MAX_POINTS_X; px++) {
              CONFIG_ECHO_START;
              SERIAL_ECHOPAIR_P(port, "  G29 S3 X", (int)px + 1);
              SERIAL_ECHOPAIR_P(port, " Y", (int)py + 1);
              SERIAL_ECHOPGM_P(port, " Z");
              SERIAL_ECHO_F_P(port, LINEAR_UNIT(mbl.z_values[px][py]), 5);
              SERIAL_EOL_P(port);
            }
          }
        }

      #elif ENABLED(AUTO_BED_LEVELING_UBL)

        if (!forReplay) {
          SERIAL_EOL_P(port);
          ubl.report_state();
          SERIAL_ECHOLNPAIR_P(port, "\nActive Mesh Slot: ", ubl.storage_slot);
          SERIAL_ECHOPAIR_P(port, "EEPROM can hold ", calc_num_meshes());
          SERIAL_ECHOLNPGM_P(port, " meshes.\n");
        }

//      ubl.report_current_mesh(PORTVAR_SOLO);   // This is too verbose for large mesh's.   A better (more terse)
                                                 // solution needs to be found.
      #elif ENABLED(AUTO_BED_LEVELING_BILINEAR)

        if (leveling_is_valid()) {
          for (uint8_t py = 0; py < GRID_MAX_POINTS_Y; py++) {
            for (uint8_t px = 0; px < GRID_MAX_POINTS_X; px++) {
              CONFIG_ECHO_START;
              SERIAL_ECHOPAIR_P(port, "  G29 W I", (int)px);
              SERIAL_ECHOPAIR_P(port, " J", (int)py);
              SERIAL_ECHOPGM_P(port, " Z");
              SERIAL_ECHO_F_P(port, LINEAR_UNIT(z_values[px][py]), 5);
              SERIAL_EOL_P(port);
            }
          }
        }

      #endif

    #endif // HAS_LEVELING

    #if HAS_SERVOS && ENABLED(EDITABLE_SERVO_ANGLES)

      if (!forReplay) {
        CONFIG_ECHO_START;
        SERIAL_ECHOLNPGM_P(port, "Servo Angles:");
      }
      for (uint8_t i = 0; i < NUM_SERVOS; i++) {
        switch (i) {
          #if ENABLED(SWITCHING_EXTRUDER)
            case SWITCHING_EXTRUDER_SERVO_NR:
            #if EXTRUDERS > 3
              case SWITCHING_EXTRUDER_E23_SERVO_NR:
            #endif
          #elif ENABLED(SWITCHING_NOZZLE)
            case SWITCHING_NOZZLE_SERVO_NR:
          #elif defined(Z_SERVO_ANGLES) && defined(Z_PROBE_SERVO_NR)
            case Z_PROBE_SERVO_NR:
          #endif
            CONFIG_ECHO_START;
            SERIAL_ECHOPAIR_P(port, "  M281 P", int(i));
            SERIAL_ECHOPAIR_P(port, " L", servo_angles[i][0]);
            SERIAL_ECHOPAIR_P(port, " U", servo_angles[i][1]);
            SERIAL_EOL_P(port);
          default: break;
        }
      }

    #endif // HAS_SERVOS && EDITABLE_SERVO_ANGLES

    #if ENABLED(DELTA)

      if (!forReplay) {
        CONFIG_ECHO_START;
        SERIAL_ECHOLNPGM_P(port, "Endstop adjustment:");
      }
      CONFIG_ECHO_START;
      SERIAL_ECHOPAIR_P(port, "  M666 X", LINEAR_UNIT(delta_endstop_adj[X_AXIS]));
      SERIAL_ECHOPAIR_P(port, " Y", LINEAR_UNIT(delta_endstop_adj[Y_AXIS]));
      SERIAL_ECHOLNPAIR_P(port, " Z", LINEAR_UNIT(delta_endstop_adj[Z_AXIS]));
      if (!forReplay) {
        CONFIG_ECHO_START;
        SERIAL_ECHOLNPGM_P(port, "Delta settings: L<diagonal_rod> R<radius> H<height> S<segments_per_s> B<calibration radius> XYZ<tower angle corrections>");
      }
      CONFIG_ECHO_START;
      SERIAL_ECHOPAIR_P(port, "  M665 L", LINEAR_UNIT(delta_diagonal_rod));
      SERIAL_ECHOPAIR_P(port, " R", LINEAR_UNIT(delta_radius));
      SERIAL_ECHOPAIR_P(port, " H", LINEAR_UNIT(delta_height));
      SERIAL_ECHOPAIR_P(port, " S", delta_segments_per_second);
      SERIAL_ECHOPAIR_P(port, " B", LINEAR_UNIT(delta_calibration_radius));
      SERIAL_ECHOPAIR_P(port, " X", LINEAR_UNIT(delta_tower_angle_trim[A_AXIS]));
      SERIAL_ECHOPAIR_P(port, " Y", LINEAR_UNIT(delta_tower_angle_trim[B_AXIS]));
      SERIAL_ECHOPAIR_P(port, " Z", LINEAR_UNIT(delta_tower_angle_trim[C_AXIS]));
      SERIAL_EOL_P(port);

    #elif ENABLED(X_DUAL_ENDSTOPS) || ENABLED(Y_DUAL_ENDSTOPS) || ENABLED(Z_DUAL_ENDSTOPS)

      if (!forReplay) {
        CONFIG_ECHO_START;
        SERIAL_ECHOLNPGM_P(port, "Endstop adjustment:");
      }
      CONFIG_ECHO_START;
      SERIAL_ECHOPGM_P(port, "  M666");
      #if ENABLED(X_DUAL_ENDSTOPS)
        SERIAL_ECHOPAIR_P(port, " X", LINEAR_UNIT(endstops.x2_endstop_adj));
      #endif
      #if ENABLED(Y_DUAL_ENDSTOPS)
        SERIAL_ECHOPAIR_P(port, " Y", LINEAR_UNIT(endstops.y2_endstop_adj));
      #endif
      #if ENABLED(Z_TRIPLE_ENDSTOPS)
        SERIAL_ECHOLNPAIR_P(port, "S1 Z", LINEAR_UNIT(endstops.z2_endstop_adj));
        CONFIG_ECHO_START;
        SERIAL_ECHOPAIR_P(port, "  M666 S2 Z", LINEAR_UNIT(endstops.z3_endstop_adj));
      #elif ENABLED(Z_DUAL_ENDSTOPS)
        SERIAL_ECHOPAIR_P(port, " Z", LINEAR_UNIT(endstops.z2_endstop_adj));
      #endif
      SERIAL_EOL_P(port);

    #endif // [XYZ]_DUAL_ENDSTOPS

    #if ENABLED(ULTIPANEL)
      if (!forReplay) {
        CONFIG_ECHO_START;
        SERIAL_ECHOLNPGM_P(port, "Material heatup parameters:");
      }
      for (uint8_t i = 0; i < COUNT(lcd_preheat_hotend_temp); i++) {
        CONFIG_ECHO_START;
        SERIAL_ECHOPAIR_P(port, "  M145 S", (int)i);
        SERIAL_ECHOPAIR_P(port, " H", TEMP_UNIT(lcd_preheat_hotend_temp[i]));
        SERIAL_ECHOPAIR_P(port, " B", TEMP_UNIT(lcd_preheat_bed_temp[i]));
        SERIAL_ECHOLNPAIR_P(port, " F", int(lcd_preheat_fan_speed[i]));
      }
    #endif // ULTIPANEL

    #if HAS_PID_HEATING

      if (!forReplay) {
        CONFIG_ECHO_START;
        SERIAL_ECHOLNPGM_P(port, "PID settings:");
      }
      #if ENABLED(PIDTEMP)
        #if HOTENDS > 1
          if (forReplay) {
            HOTEND_LOOP() {
              CONFIG_ECHO_START;
              SERIAL_ECHOPAIR_P(port, "  M301 E", e);
              SERIAL_ECHOPAIR_P(port, " P", PID_PARAM(Kp, e));
              SERIAL_ECHOPAIR_P(port, " I", unscalePID_i(PID_PARAM(Ki, e)));
              SERIAL_ECHOPAIR_P(port, " D", unscalePID_d(PID_PARAM(Kd, e)));
              #if ENABLED(PID_EXTRUSION_SCALING)
                SERIAL_ECHOPAIR_P(port, " C", PID_PARAM(Kc, e));
                if (e == 0) SERIAL_ECHOPAIR_P(port, " L", thermalManager.lpq_len);
              #endif
              SERIAL_EOL_P(port);
            }
          }
          else
        #endif // HOTENDS > 1
        // !forReplay || HOTENDS == 1
        {
          CONFIG_ECHO_START;
          SERIAL_ECHOPAIR_P(port, "  M301 P", PID_PARAM(Kp, 0)); // for compatibility with hosts, only echo values for E0
          SERIAL_ECHOPAIR_P(port, " I", unscalePID_i(PID_PARAM(Ki, 0)));
          SERIAL_ECHOPAIR_P(port, " D", unscalePID_d(PID_PARAM(Kd, 0)));
          #if ENABLED(PID_EXTRUSION_SCALING)
            SERIAL_ECHOPAIR_P(port, " C", PID_PARAM(Kc, 0));
            SERIAL_ECHOPAIR_P(port, " L", thermalManager.lpq_len);
          #endif
          SERIAL_EOL_P(port);
        }
      #endif // PIDTEMP

      #if ENABLED(PIDTEMPBED)
        CONFIG_ECHO_START;
        SERIAL_ECHOPAIR_P(port, "  M304 P", thermalManager.bed_pid.Kp);
        SERIAL_ECHOPAIR_P(port, " I", unscalePID_i(thermalManager.bed_pid.Ki));
        SERIAL_ECHOPAIR_P(port, " D", unscalePID_d(thermalManager.bed_pid.Kd));
        SERIAL_EOL_P(port);
      #endif

    #endif // PIDTEMP || PIDTEMPBED

    #if HAS_LCD_CONTRAST
      if (!forReplay) {
        CONFIG_ECHO_START;
        SERIAL_ECHOLNPGM_P(port, "LCD Contrast:");
      }
      CONFIG_ECHO_START;
      SERIAL_ECHOLNPAIR_P(port, "  M250 C", lcd_contrast);
    #endif

    #if ENABLED(FWRETRACT)

      if (!forReplay) {
        CONFIG_ECHO_START;
        SERIAL_ECHOLNPGM_P(port, "Retract: S<length> F<units/m> Z<lift>");
      }
      CONFIG_ECHO_START;
      SERIAL_ECHOPAIR_P(port, "  M207 S", LINEAR_UNIT(fwretract.settings.retract_length));
      SERIAL_ECHOPAIR_P(port, " W", LINEAR_UNIT(fwretract.settings.swap_retract_length));
      SERIAL_ECHOPAIR_P(port, " F", MMS_TO_MMM(LINEAR_UNIT(fwretract.settings.retract_feedrate_mm_s)));
      SERIAL_ECHOLNPAIR_P(port, " Z", LINEAR_UNIT(fwretract.settings.retract_zraise));

      if (!forReplay) {
        CONFIG_ECHO_START;
        SERIAL_ECHOLNPGM_P(port, "Recover: S<length> F<units/m>");
      }
      CONFIG_ECHO_START;
      SERIAL_ECHOPAIR_P(port, "  M208 S", LINEAR_UNIT(fwretract.settings.retract_recover_length));
      SERIAL_ECHOPAIR_P(port, " W", LINEAR_UNIT(fwretract.settings.swap_retract_recover_length));
      SERIAL_ECHOLNPAIR_P(port, " F", MMS_TO_MMM(LINEAR_UNIT(fwretract.settings.retract_recover_feedrate_mm_s)));

      #if ENABLED(FWRETRACT_AUTORETRACT)

        if (!forReplay) {
          CONFIG_ECHO_START;
          SERIAL_ECHOLNPGM_P(port, "Auto-Retract: S=0 to disable, 1 to interpret E-only moves as retract/recover");
        }
        CONFIG_ECHO_START;
        SERIAL_ECHOLNPAIR_P(port, "  M209 S", fwretract.autoretract_enabled ? 1 : 0);

      #endif // FWRETRACT_AUTORETRACT

    #endif // FWRETRACT

    /**
     * Probe Offset
     */
    #if HAS_BED_PROBE
      if (!forReplay) {
        CONFIG_ECHO_START;
        SERIAL_ECHOPGM_P(port, "Z-Probe Offset (mm):");
        SAY_UNITS_P(port, true);
      }
      CONFIG_ECHO_START;
      SERIAL_ECHOLNPAIR_P(port, "  M851 Z", LINEAR_UNIT(zprobe_zoffset));
    #endif

    /**
     * Bed Skew Correction
     */
    #if ENABLED(SKEW_CORRECTION_GCODE)
      if (!forReplay) {
        CONFIG_ECHO_START;
        SERIAL_ECHOLNPGM_P(port, "Skew Factor: ");
      }
      CONFIG_ECHO_START;
      #if ENABLED(SKEW_CORRECTION_FOR_Z)
        SERIAL_ECHOPGM_P(port, "  M852 I");
        SERIAL_ECHO_F_P(port, LINEAR_UNIT(planner.skew_factor.xy), 6);
        SERIAL_ECHOPGM_P(port, " J");
        SERIAL_ECHO_F_P(port, LINEAR_UNIT(planner.skew_factor.xz), 6);
        SERIAL_ECHOPGM_P(port, " K");
        SERIAL_ECHO_F_P(port, LINEAR_UNIT(planner.skew_factor.yz), 6);
        SERIAL_EOL_P(port);
      #else
        SERIAL_ECHOPGM_P(port, "  M852 S");
        SERIAL_ECHO_F_P(port, LINEAR_UNIT(planner.skew_factor.xy), 6);
        SERIAL_EOL_P(port);
      #endif
    #endif

    #if HAS_TRINAMIC

      /**
       * TMC stepper driver current
       */
      if (!forReplay) {
        CONFIG_ECHO_START;
        SERIAL_ECHOLNPGM_P(port, "Stepper driver current:");
      }
      CONFIG_ECHO_START;
      #if AXIS_IS_TMC(X) || AXIS_IS_TMC(Y) || AXIS_IS_TMC(Z)
        say_M906(PORTVAR_SOLO);
      #endif
      #if AXIS_IS_TMC(X)
        SERIAL_ECHOPAIR_P(port, " X", stepperX.getMilliamps());
      #endif
      #if AXIS_IS_TMC(Y)
        SERIAL_ECHOPAIR_P(port, " Y", stepperY.getMilliamps());
      #endif
      #if AXIS_IS_TMC(Z)
        SERIAL_ECHOPAIR_P(port, " Z", stepperZ.getMilliamps());
      #endif
      #if AXIS_IS_TMC(X) || AXIS_IS_TMC(Y) || AXIS_IS_TMC(Z)
        SERIAL_EOL_P(port);
      #endif

      #if AXIS_IS_TMC(X2) || AXIS_IS_TMC(Y2) || AXIS_IS_TMC(Z2)
        say_M906(PORTVAR_SOLO);
        SERIAL_ECHOPGM_P(port, " I1");
      #endif
      #if AXIS_IS_TMC(X2)
        SERIAL_ECHOPAIR_P(port, " X", stepperX2.getMilliamps());
      #endif
      #if AXIS_IS_TMC(Y2)
        SERIAL_ECHOPAIR_P(port, " Y", stepperY2.getMilliamps());
      #endif
      #if AXIS_IS_TMC(Z2)
        SERIAL_ECHOPAIR_P(port, " Z", stepperZ2.getMilliamps());
      #endif
      #if AXIS_IS_TMC(X2) || AXIS_IS_TMC(Y2) || AXIS_IS_TMC(Z2)
        SERIAL_EOL_P(port);
      #endif

      #if AXIS_IS_TMC(Z3)
        say_M906(PORTVAR_SOLO);
        SERIAL_ECHOLNPAIR_P(port, " I2 Z", stepperZ3.getMilliamps());
      #endif

      #if AXIS_IS_TMC(E0)
        say_M906(PORTVAR_SOLO);
        SERIAL_ECHOLNPAIR_P(port, " T0 E", stepperE0.getMilliamps());
      #endif
      #if AXIS_IS_TMC(E1)
        say_M906(PORTVAR_SOLO);
        SERIAL_ECHOLNPAIR_P(port, " T1 E", stepperE1.getMilliamps());
      #endif
      #if AXIS_IS_TMC(E2)
        say_M906(PORTVAR_SOLO);
        SERIAL_ECHOLNPAIR_P(port, " T2 E", stepperE2.getMilliamps());
      #endif
      #if AXIS_IS_TMC(E3)
        say_M906(PORTVAR_SOLO);
        SERIAL_ECHOLNPAIR_P(port, " T3 E", stepperE3.getMilliamps());
      #endif
      #if AXIS_IS_TMC(E4)
        say_M906(PORTVAR_SOLO);
        SERIAL_ECHOLNPAIR_P(port, " T4 E", stepperE4.getMilliamps());
      #endif
      #if AXIS_IS_TMC(E5)
        say_M906(PORTVAR_SOLO);
        SERIAL_ECHOLNPAIR_P(port, " T5 E", stepperE5.getMilliamps());
      #endif
      SERIAL_EOL_P(port);

      /**
       * TMC Hybrid Threshold
       */
      #if ENABLED(HYBRID_THRESHOLD)
        if (!forReplay) {
          CONFIG_ECHO_START;
          SERIAL_ECHOLNPGM_P(port, "Hybrid Threshold:");
        }
        CONFIG_ECHO_START;
        #if AXIS_HAS_STEALTHCHOP(X) || AXIS_HAS_STEALTHCHOP(Y) || AXIS_HAS_STEALTHCHOP(Z)
          say_M913(PORTVAR_SOLO);
        #endif
        #if AXIS_HAS_STEALTHCHOP(X)
          SERIAL_ECHOPAIR_P(port, " X", TMC_GET_PWMTHRS(X, X));
        #endif
        #if AXIS_HAS_STEALTHCHOP(Y)
          SERIAL_ECHOPAIR_P(port, " Y", TMC_GET_PWMTHRS(Y, Y));
        #endif
        #if AXIS_HAS_STEALTHCHOP(Z)
          SERIAL_ECHOPAIR_P(port, " Z", TMC_GET_PWMTHRS(Z, Z));
        #endif
        #if AXIS_HAS_STEALTHCHOP(X) || AXIS_HAS_STEALTHCHOP(Y) || AXIS_HAS_STEALTHCHOP(Z)
          SERIAL_EOL_P(port);
        #endif

        #if AXIS_HAS_STEALTHCHOP(X2) || AXIS_HAS_STEALTHCHOP(Y2) || AXIS_HAS_STEALTHCHOP(Z2)
          say_M913(PORTVAR_SOLO);
          SERIAL_ECHOPGM_P(port, " I1");
        #endif
        #if AXIS_HAS_STEALTHCHOP(X2)
          SERIAL_ECHOPAIR_P(port, " X", TMC_GET_PWMTHRS(X, X2));
        #endif
        #if AXIS_HAS_STEALTHCHOP(Y2)
          SERIAL_ECHOPAIR_P(port, " Y", TMC_GET_PWMTHRS(Y, Y2));
        #endif
        #if AXIS_HAS_STEALTHCHOP(Z2)
          SERIAL_ECHOPAIR_P(port, " Z", TMC_GET_PWMTHRS(Z, Z2));
        #endif
        #if AXIS_HAS_STEALTHCHOP(X2) || AXIS_HAS_STEALTHCHOP(Y2) || AXIS_HAS_STEALTHCHOP(Z2)
          SERIAL_EOL_P(port);
        #endif

        #if AXIS_HAS_STEALTHCHOP(Z3)
          say_M913(PORTVAR_SOLO);
          SERIAL_ECHOPGM_P(port, " I2");
          SERIAL_ECHOLNPAIR_P(port, " Z", TMC_GET_PWMTHRS(Z, Z3));
        #endif

        #if AXIS_HAS_STEALTHCHOP(E0)
          say_M913(PORTVAR_SOLO);
          SERIAL_ECHOLNPAIR_P(port, " T0 E", TMC_GET_PWMTHRS(E, E0));
        #endif
        #if AXIS_HAS_STEALTHCHOP(E1)
          say_M913(PORTVAR_SOLO);
          SERIAL_ECHOLNPAIR_P(port, " T1 E", TMC_GET_PWMTHRS(E, E1));
        #endif
        #if AXIS_HAS_STEALTHCHOP(E2)
          say_M913(PORTVAR_SOLO);
          SERIAL_ECHOLNPAIR_P(port, " T2 E", TMC_GET_PWMTHRS(E, E2));
        #endif
        #if AXIS_HAS_STEALTHCHOP(E3)
          say_M913(PORTVAR_SOLO);
          SERIAL_ECHOLNPAIR_P(port, " T3 E", TMC_GET_PWMTHRS(E, E3));
        #endif
        #if AXIS_HAS_STEALTHCHOP(E4)
          say_M913(PORTVAR_SOLO);
          SERIAL_ECHOLNPAIR_P(port, " T4 E", TMC_GET_PWMTHRS(E, E4));
        #endif
        #if AXIS_HAS_STEALTHCHOP(E5)
          say_M913(PORTVAR_SOLO);
          SERIAL_ECHOLNPAIR_P(port, " T5 E", TMC_GET_PWMTHRS(E, E5));
        #endif
        SERIAL_EOL_P(port);
      #endif // HYBRID_THRESHOLD

      /**
       * TMC Sensorless homing thresholds
       */
      #if USE_SENSORLESS
        if (!forReplay) {
          CONFIG_ECHO_START;
          SERIAL_ECHOLNPGM_P(port, "TMC2130 StallGuard threshold:");
        }
        CONFIG_ECHO_START;
        #if X_SENSORLESS || Y_SENSORLESS || Z_SENSORLESS
          say_M914(PORTVAR_SOLO);
          #if X_SENSORLESS
            SERIAL_ECHOPAIR_P(port, " X", stepperX.sgt());
          #endif
          #if Y_SENSORLESS
            SERIAL_ECHOPAIR_P(port, " Y", stepperY.sgt());
          #endif
          #if Z_SENSORLESS
            SERIAL_ECHOPAIR_P(port, " Z", stepperZ.sgt());
          #endif
          SERIAL_EOL_P(port);
        #endif

        #define HAS_X2_SENSORLESS (defined(X_STALL_SENSITIVITY) && AXIS_HAS_STALLGUARD(X2))
        #define HAS_Y2_SENSORLESS (defined(Y_STALL_SENSITIVITY) && AXIS_HAS_STALLGUARD(Y2))
        #define HAS_Z2_SENSORLESS (defined(Z_STALL_SENSITIVITY) && AXIS_HAS_STALLGUARD(Z2))
        #define HAS_Z3_SENSORLESS (defined(Z_STALL_SENSITIVITY) && AXIS_HAS_STALLGUARD(Z3))
        #if HAS_X2_SENSORLESS || HAS_Y2_SENSORLESS || HAS_Z2_SENSORLESS
          say_M914(PORTVAR_SOLO);
          SERIAL_ECHOPGM_P(port, " I1");
          #if HAS_X2_SENSORLESS
            SERIAL_ECHOPAIR_P(port, " X", stepperX2.sgt());
          #endif
          #if HAS_Y2_SENSORLESS
            SERIAL_ECHOPAIR_P(port, " Y", stepperY2.sgt());
          #endif
          #if HAS_Z2_SENSORLESS
            SERIAL_ECHOPAIR_P(port, " Z", stepperZ2.sgt());
          #endif
          SERIAL_EOL_P(port);
        #endif

        #if HAS_Z3_SENSORLESS
          say_M914(PORTVAR_SOLO);
          SERIAL_ECHOPGM_P(port, " I2");
          SERIAL_ECHOLNPAIR_P(port, " Z", stepperZ3.sgt());
        #endif

      #endif // USE_SENSORLESS

    #endif // HAS_TRINAMIC

    /**
     * Linear Advance
     */
    #if ENABLED(LIN_ADVANCE)
      if (!forReplay) {
        CONFIG_ECHO_START;
        SERIAL_ECHOLNPGM_P(port, "Linear Advance:");
      }

      CONFIG_ECHO_START;
      #if EXTRUDERS < 2
        SERIAL_ECHOLNPAIR_P(port, "  M900 K", planner.extruder_advance_K[0]);
      #else
        LOOP_L_N(i, EXTRUDERS) {
          SERIAL_ECHOPAIR_P(port, "  M900 T", int(i));
          SERIAL_ECHOLNPAIR_P(port, " K", planner.extruder_advance_K[i]);
        }
      #endif
    #endif

    #if HAS_MOTOR_CURRENT_PWM
      CONFIG_ECHO_START;
      if (!forReplay) {
        SERIAL_ECHOLNPGM_P(port, "Stepper motor currents:");
        CONFIG_ECHO_START;
      }
      SERIAL_ECHOPAIR_P(port, "  M907 X", stepper.motor_current_setting[0]);
      SERIAL_ECHOPAIR_P(port, " Z", stepper.motor_current_setting[1]);
      SERIAL_ECHOPAIR_P(port, " E", stepper.motor_current_setting[2]);
      SERIAL_EOL_P(port);
    #endif

    /**
     * Advanced Pause filament load & unload lengths
     */
    #if ENABLED(ADVANCED_PAUSE_FEATURE)
      if (!forReplay) {
        CONFIG_ECHO_START;
        SERIAL_ECHOLNPGM_P(port, "Filament load/unload lengths:");
      }
      CONFIG_ECHO_START;
      #if EXTRUDERS == 1
        say_M603(PORTVAR_SOLO);
        SERIAL_ECHOPAIR_P(port, "L", LINEAR_UNIT(fc_settings[0].load_length));
        SERIAL_ECHOLNPAIR_P(port, " U", LINEAR_UNIT(fc_settings[0].unload_length));
      #else
        say_M603(PORTVAR_SOLO);
        SERIAL_ECHOPAIR_P(port, "T0 L", LINEAR_UNIT(fc_settings[0].load_length));
        SERIAL_ECHOLNPAIR_P(port, " U", LINEAR_UNIT(fc_settings[0].unload_length));
        CONFIG_ECHO_START;
        say_M603(PORTVAR_SOLO);
        SERIAL_ECHOPAIR_P(port, "T1 L", LINEAR_UNIT(fc_settings[1].load_length));
        SERIAL_ECHOLNPAIR_P(port, " U", LINEAR_UNIT(fc_settings[1].unload_length));
        #if EXTRUDERS > 2
          CONFIG_ECHO_START;
          say_M603(PORTVAR_SOLO);
          SERIAL_ECHOPAIR_P(port, "T2 L", LINEAR_UNIT(fc_settings[2].load_length));
          SERIAL_ECHOLNPAIR_P(port, " U", LINEAR_UNIT(fc_settings[2].unload_length));
          #if EXTRUDERS > 3
            CONFIG_ECHO_START;
            say_M603(PORTVAR_SOLO);
            SERIAL_ECHOPAIR_P(port, "T3 L", LINEAR_UNIT(fc_settings[3].load_length));
            SERIAL_ECHOLNPAIR_P(port, " U", LINEAR_UNIT(fc_settings[3].unload_length));
            #if EXTRUDERS > 4
              CONFIG_ECHO_START;
              say_M603(PORTVAR_SOLO);
              SERIAL_ECHOPAIR_P(port, "T4 L", LINEAR_UNIT(fc_settings[4].load_length));
              SERIAL_ECHOLNPAIR_P(port, " U", LINEAR_UNIT(fc_settings[4].unload_length));
              #if EXTRUDERS > 5
                CONFIG_ECHO_START;
                say_M603(PORTVAR_SOLO);
                SERIAL_ECHOPAIR_P(port, "T5 L", LINEAR_UNIT(fc_settings[5].load_length));
                SERIAL_ECHOLNPAIR_P(port, " U", LINEAR_UNIT(fc_settings[5].unload_length));
              #endif // EXTRUDERS > 5
            #endif // EXTRUDERS > 4
          #endif // EXTRUDERS > 3
        #endif // EXTRUDERS > 2
      #endif // EXTRUDERS == 1
    #endif // ADVANCED_PAUSE_FEATURE

    #if ENABLED(SINGLENOZZLE)
      CONFIG_ECHO_START;
      if (!forReplay) {
        SERIAL_ECHOLNPGM_P(port, "SINGLENOZZLE:");
        CONFIG_ECHO_START;
      }
      M217_report(true);
    #endif
  }

#endif // !DISABLE_M503

#pragma pack(pop)
