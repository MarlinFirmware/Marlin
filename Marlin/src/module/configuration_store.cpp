/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#define EEPROM_VERSION "V71"
#define EEPROM_OFFSET 100

// Check the integrity of data offsets.
// Can be disabled for production build.
//#define DEBUG_EEPROM_READWRITE

#include "configuration_store.h"

#include "endstops.h"
#include "planner.h"
#include "stepper.h"
#include "temperature.h"
#include "../lcd/ultralcd.h"
#include "../core/language.h"
#include "../libs/vector_3.h"   // for matrix_3x3
#include "../gcode/gcode.h"
#include "../Marlin.h"

#if EITHER(EEPROM_SETTINGS, SD_FIRMWARE_UPDATE)
  #include "../HAL/shared/persistent_store_api.h"
#endif

#include "probe.h"

#if HAS_LEVELING
  #include "../feature/bedlevel/bedlevel.h"
#endif

#if ENABLED(EXTENSIBLE_UI)
  #include "../lcd/extensible_ui/ui_api.h"
#endif

#if HAS_SERVOS
  #include "servo.h"
#endif

#if HAS_SERVOS && HAS_SERVO_ANGLES
  #define EEPROM_NUM_SERVOS NUM_SERVOS
#else
  #define EEPROM_NUM_SERVOS NUM_SERVO_PLUGS
#endif

#include "../feature/fwretract.h"

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../feature/power_loss_recovery.h"
#endif

#include "../feature/pause.h"

#if ENABLED(BACKLASH_COMPENSATION)
  #include "../feature/backlash.h"
#endif

#if HAS_FILAMENT_SENSOR
  #include "../feature/runout.h"
#endif

#if ENABLED(EXTRA_LIN_ADVANCE_K)
  extern float saved_extruder_advance_K[EXTRUDERS];
#endif

#if EXTRUDERS > 1
  #include "tool_change.h"
  void M217_report(const bool eeprom);
#endif

#if ENABLED(BLTOUCH)
  #include "../feature/bltouch.h"
#endif

#if HAS_TRINAMIC
  #include "stepper/indirection.h"
  #include "../feature/tmc_util.h"
#endif

#pragma pack(push, 1) // No padding between variables

typedef struct { uint16_t X, Y, Z, X2, Y2, Z2, Z3, E0, E1, E2, E3, E4, E5; } tmc_stepper_current_t;
typedef struct { uint32_t X, Y, Z, X2, Y2, Z2, Z3, E0, E1, E2, E3, E4, E5; } tmc_hybrid_threshold_t;
typedef struct {  int16_t X, Y, Z, X2;                                     } tmc_sgt_t;
typedef struct {     bool X, Y, Z, X2, Y2, Z2, Z3, E0, E1, E2, E3, E4, E5; } tmc_stealth_enabled_t;

// Limit an index to an array size
#define ALIM(I,ARR) _MIN(I, COUNT(ARR) - 1)

// Defaults for reset / fill in on load
static const uint32_t   _DMA[] PROGMEM = DEFAULT_MAX_ACCELERATION;
static const float     _DASU[] PROGMEM = DEFAULT_AXIS_STEPS_PER_UNIT;
static const feedRate_t _DMF[] PROGMEM = DEFAULT_MAX_FEEDRATE;

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

  xyze_float_t planner_max_jerk;                        // M205 XYZE  planner.max_jerk
  float planner_junction_deviation_mm;                  // M205 J     planner.junction_deviation_mm

  xyz_pos_t home_offset;                                // M206 XYZ / M665 TPZ

  #if HAS_HOTEND_OFFSET
    xyz_pos_t hotend_offset[HOTENDS - 1];               // M218 XYZ
  #endif

  //
  // FILAMENT_RUNOUT_SENSOR
  //
  bool runout_sensor_enabled;                           // M412 S
  float runout_distance_mm;                             // M412 D

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

  xyz_pos_t probe_offset;

  //
  // ABL_PLANAR
  //
  matrix_3x3 planner_bed_level_matrix;                  // planner.bed_level_matrix

  //
  // AUTO_BED_LEVELING_BILINEAR
  //
  uint8_t grid_max_x, grid_max_y;                       // GRID_MAX_POINTS_X, GRID_MAX_POINTS_Y
  xy_int_t bilinear_grid_spacing, bilinear_start;       // G29 L F
  #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
    bed_mesh_t z_values;                                // G29
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
  uint16_t servo_angles[EEPROM_NUM_SERVOS][2];          // M281 P L U

  //
  // BLTOUCH
  //
  bool bltouch_last_written_mode;

  //
  // DELTA / [XYZ]_DUAL_ENDSTOPS
  //
  #if ENABLED(DELTA)
    float delta_height;                                 // M666 H
    abc_float_t delta_endstop_adj;                      // M666 XYZ
    float delta_radius,                                 // M665 R
          delta_diagonal_rod,                           // M665 L
          delta_segments_per_second,                    // M665 S
          delta_calibration_radius;                     // M665 B
    abc_float_t delta_tower_angle_trim;                 // M665 XYZ
  #elif EITHER(X_DUAL_ENDSTOPS, Y_DUAL_ENDSTOPS) || Z_MULTI_ENDSTOPS
    float x2_endstop_adj,                               // M666 X
          y2_endstop_adj,                               // M666 Y
          z2_endstop_adj,                               // M666 Z (S2)
          z3_endstop_adj;                               // M666 Z (S3)
  #endif

  //
  // ULTIPANEL
  //
  int16_t ui_preheat_hotend_temp[2],                    // M145 S0 H
          ui_preheat_bed_temp[2];                       // M145 S0 B
  uint8_t ui_preheat_fan_speed[2];                      // M145 S0 F

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
  // User-defined Thermistors
  //
  #if HAS_USER_THERMISTORS
    user_thermistor_t user_thermistor[USER_THERMISTORS]; // M305 P0 R4700 T100000 B3950
  #endif

  //
  // HAS_LCD_CONTRAST
  //
  int16_t lcd_contrast;                                 // M250 C

  //
  // POWER_LOSS_RECOVERY
  //
  bool recovery_enabled;                                // M413 S

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
  tmc_sgt_t tmc_sgt;                                    // M914 X Y Z X2
  tmc_stealth_enabled_t tmc_stealth_enabled;            // M569 X Y Z X2 Y2 Z2 Z3 E0 E1 E2 E3 E4 E5

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
  xyz_pos_t coordinate_system[MAX_COORDINATE_SYSTEMS];  // G54-G59.3

  //
  // SKEW_CORRECTION
  //
  skew_factor_t planner_skew_factor;                    // M852 I J K  planner.skew_factor

  //
  // ADVANCED_PAUSE_FEATURE
  //
  #if EXTRUDERS
    fil_change_settings_t fc_settings[EXTRUDERS];       // M603 T U L
  #endif

  //
  // Tool-change settings
  //
  #if EXTRUDERS > 1
    toolchange_settings_t toolchange_settings;          // M217 S P R
  #endif

  //
  // BACKLASH_COMPENSATION
  //
  xyz_float_t backlash_distance_mm;                     // M425 X Y Z
  uint8_t backlash_correction;                          // M425 F
  float backlash_smoothing_mm;                          // M425 S

  //
  // EXTENSIBLE_UI
  //
  #if ENABLED(EXTENSIBLE_UI)
    // This is a significant hardware change; don't reserve space when not present
    uint8_t extui_data[ExtUI::eeprom_data_size];
  #endif

} SettingsData;

//static_assert(sizeof(SettingsData) <= E2END + 1, "EEPROM too small to contain SettingsData!");

MarlinSettings settings;

uint16_t MarlinSettings::datasize() { return sizeof(SettingsData); }

/**
 * Post-process after Retrieve or Reset
 */

#if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
  float new_z_fade_height;
#endif

void MarlinSettings::postprocess() {
  xyze_pos_t oldpos = current_position;

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
  #elif EXTRUDERS
    for (uint8_t i = COUNT(planner.e_factor); i--;)
      planner.refresh_e_factor(i);
  #endif

  // Software endstops depend on home_offset
  LOOP_XYZ(i) {
    update_workspace_offset((AxisEnum)i);
    update_software_endstops((AxisEnum)i);
  }

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

  #if HAS_LINEAR_E_JERK
    planner.recalculate_max_e_jerk();
  #endif

  // Refresh steps_to_mm with the reciprocal of axis_steps_per_mm
  // and init stepper.count[], planner.position[] with current_position
  planner.refresh_positioning();

  // Various factors can change the current position
  if (oldpos != current_position)
    report_current_position();
}

#if BOTH(PRINTCOUNTER, EEPROM_SETTINGS)
  #include "printcounter.h"
  static_assert(
    !WITHIN(STATS_EEPROM_ADDRESS, EEPROM_OFFSET, EEPROM_OFFSET + sizeof(SettingsData)) &&
    !WITHIN(STATS_EEPROM_ADDRESS + sizeof(printStatistics), EEPROM_OFFSET, EEPROM_OFFSET + sizeof(SettingsData)),
    "STATS_EEPROM_ADDRESS collides with EEPROM settings storage."
  );
#endif

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

#ifdef ARCHIM2_SPI_FLASH_EEPROM_BACKUP_SIZE
  static_assert(
      EEPROM_OFFSET + sizeof(SettingsData) < ARCHIM2_SPI_FLASH_EEPROM_BACKUP_SIZE,
      "ARCHIM2_SPI_FLASH_EEPROM_BACKUP_SIZE is insufficient to capture all EEPROM data."
    );
#endif

#define DEBUG_OUT ENABLED(EEPROM_CHITCHAT)
#include "../core/debug_out.h"

#if ENABLED(EEPROM_SETTINGS)

  #define EEPROM_START()          if (!persistentStore.access_start()) { SERIAL_ECHO_MSG("No EEPROM."); return false; } \
                                  int eeprom_index = EEPROM_OFFSET
  #define EEPROM_FINISH()         persistentStore.access_finish()
  #define EEPROM_SKIP(VAR)        (eeprom_index += sizeof(VAR))
  #define EEPROM_WRITE(VAR)       do{ persistentStore.write_data(eeprom_index, (uint8_t*)&VAR, sizeof(VAR), &working_crc);              }while(0)
  #define EEPROM_READ(VAR)        do{ persistentStore.read_data(eeprom_index, (uint8_t*)&VAR, sizeof(VAR), &working_crc, !validating);  }while(0)
  #define EEPROM_READ_ALWAYS(VAR) do{ persistentStore.read_data(eeprom_index, (uint8_t*)&VAR, sizeof(VAR), &working_crc);               }while(0)
  #define EEPROM_ASSERT(TST,ERR)  do{ if (!(TST)) { SERIAL_ERROR_MSG(ERR); eeprom_error = true; } }while(0)

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

  bool MarlinSettings::size_error(const uint16_t size) {
    if (size != datasize()) {
      DEBUG_ERROR_MSG("EEPROM datasize error.");
      return true;
    }
    return false;
  }

  /**
   * M500 - Store Configuration
   */
  bool MarlinSettings::save() {
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

    //
    // Planner Motion
    //
    {
      EEPROM_WRITE(planner.settings);

      #if HAS_CLASSIC_JERK
        EEPROM_WRITE(planner.max_jerk);
        #if HAS_LINEAR_E_JERK
          dummy = float(DEFAULT_EJERK);
          EEPROM_WRITE(dummy);
        #endif
      #else
        const xyze_pos_t planner_max_jerk = { 10, 10, 0.4, float(DEFAULT_EJERK) };
        EEPROM_WRITE(planner_max_jerk);
      #endif

      #if DISABLED(CLASSIC_JERK)
        EEPROM_WRITE(planner.junction_deviation_mm);
      #else
        dummy = 0.02f;
        EEPROM_WRITE(dummy);
      #endif
    }

    //
    // Home Offset
    //
    {
      _FIELD_TEST(home_offset);

      #if HAS_SCARA_OFFSET
        EEPROM_WRITE(scara_home_offset);
      #else
        #if !HAS_HOME_OFFSET
          const xyz_pos_t home_offset{0};
        #endif
        EEPROM_WRITE(home_offset);
      #endif

      #if HAS_HOTEND_OFFSET
        // Skip hotend 0 which must be 0
        for (uint8_t e = 1; e < HOTENDS; e++)
          EEPROM_WRITE(hotend_offset[e]);
      #endif
    }

    //
    // Filament Runout Sensor
    //
    {
      #if HAS_FILAMENT_SENSOR
        const bool &runout_sensor_enabled = runout.enabled;
      #else
        const bool runout_sensor_enabled = true;
      #endif
      #if HAS_FILAMENT_SENSOR && defined(FILAMENT_RUNOUT_DISTANCE_MM)
        const float &runout_distance_mm = runout.runout_distance();
      #else
        const float runout_distance_mm = 0;
      #endif
      _FIELD_TEST(runout_sensor_enabled);
      EEPROM_WRITE(runout_sensor_enabled);
      EEPROM_WRITE(runout_distance_mm);
    }

    //
    // Global Leveling
    //
    {
      const float zfh = (
        #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
          planner.z_fade_height
        #else
          10.0
        #endif
      );
      EEPROM_WRITE(zfh);
    }

    //
    // Mesh Bed Leveling
    //
    {
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
      #endif
    }

    //
    // Probe XYZ Offsets
    //
    {
      _FIELD_TEST(probe_offset);
      EEPROM_WRITE(probe_offset);
    }

    //
    // Planar Bed Leveling matrix
    //
    {
      #if ABL_PLANAR
        EEPROM_WRITE(planner.bed_level_matrix);
      #else
        dummy = 0;
        for (uint8_t q = 9; q--;) EEPROM_WRITE(dummy);
      #endif
    }

    //
    // Bilinear Auto Bed Leveling
    //
    {
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
        const xy_int_t bilinear_start{0}, bilinear_grid_spacing{0};
        dummy = 0;
        EEPROM_WRITE(grid_max_x);
        EEPROM_WRITE(grid_max_y);
        EEPROM_WRITE(bilinear_grid_spacing);
        EEPROM_WRITE(bilinear_start);
        for (uint16_t q = grid_max_x * grid_max_y; q--;) EEPROM_WRITE(dummy);
      #endif
    }

    //
    // Unified Bed Leveling
    //
    {
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
    }

    //
    // Servo Angles
    //
    {
      _FIELD_TEST(servo_angles);

      #if !HAS_SERVO_ANGLES
        uint16_t servo_angles[EEPROM_NUM_SERVOS][2] = { { 0, 0 } };
      #endif
      EEPROM_WRITE(servo_angles);
    }

    //
    // BLTOUCH
    //
    {
      _FIELD_TEST(bltouch_last_written_mode);
      #if ENABLED(BLTOUCH)
        const bool &bltouch_last_written_mode = bltouch.last_written_mode;
      #else
        constexpr bool bltouch_last_written_mode = false;
      #endif
      EEPROM_WRITE(bltouch_last_written_mode);
    }

    //
    // DELTA Geometry or Dual Endstops offsets
    //
    {
      #if ENABLED(DELTA)

        _FIELD_TEST(delta_height);

        EEPROM_WRITE(delta_height);              // 1 float
        EEPROM_WRITE(delta_endstop_adj);         // 3 floats
        EEPROM_WRITE(delta_radius);              // 1 float
        EEPROM_WRITE(delta_diagonal_rod);        // 1 float
        EEPROM_WRITE(delta_segments_per_second); // 1 float
        EEPROM_WRITE(delta_calibration_radius);  // 1 float
        EEPROM_WRITE(delta_tower_angle_trim);    // 3 floats

      #elif EITHER(X_DUAL_ENDSTOPS, Y_DUAL_ENDSTOPS) || Z_MULTI_ENDSTOPS

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
    }

    //
    // LCD Preheat settings
    //
    {
      _FIELD_TEST(ui_preheat_hotend_temp);

      #if HOTENDS && HAS_LCD_MENU
        const int16_t (&ui_preheat_hotend_temp)[2]  = ui.preheat_hotend_temp,
                      (&ui_preheat_bed_temp)[2]     = ui.preheat_bed_temp;
        const uint8_t (&ui_preheat_fan_speed)[2]    = ui.preheat_fan_speed;
      #else
        constexpr int16_t ui_preheat_hotend_temp[2] = { PREHEAT_1_TEMP_HOTEND, PREHEAT_2_TEMP_HOTEND },
                          ui_preheat_bed_temp[2]    = { PREHEAT_1_TEMP_BED, PREHEAT_2_TEMP_BED };
        constexpr uint8_t ui_preheat_fan_speed[2]   = { PREHEAT_1_FAN_SPEED, PREHEAT_2_FAN_SPEED };
      #endif

      EEPROM_WRITE(ui_preheat_hotend_temp);
      EEPROM_WRITE(ui_preheat_bed_temp);
      EEPROM_WRITE(ui_preheat_fan_speed);
    }

    //
    // PIDTEMP
    //
    {
      _FIELD_TEST(hotendPID);
      HOTEND_LOOP() {
        PIDC_t pidc = {
                       PID_PARAM(Kp, e),
          unscalePID_i(PID_PARAM(Ki, e)),
          unscalePID_d(PID_PARAM(Kd, e)),
                       PID_PARAM(Kc, e)
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

      const PID_t bed_pid = {
        #if DISABLED(PIDTEMPBED)
          DUMMY_PID_VALUE, DUMMY_PID_VALUE, DUMMY_PID_VALUE
        #else
          // Store the unscaled PID values
          thermalManager.temp_bed.pid.Kp,
          unscalePID_i(thermalManager.temp_bed.pid.Ki),
          unscalePID_d(thermalManager.temp_bed.pid.Kd)
        #endif
      };
      EEPROM_WRITE(bed_pid);
    }

    //
    // User-defined Thermistors
    //
    #if HAS_USER_THERMISTORS
    {
      _FIELD_TEST(user_thermistor);
      EEPROM_WRITE(thermalManager.user_thermistor);
    }
    #endif

    //
    // LCD Contrast
    //
    {
      _FIELD_TEST(lcd_contrast);

      const int16_t lcd_contrast =
        #if HAS_LCD_CONTRAST
          ui.contrast
        #elif defined(DEFAULT_LCD_CONTRAST)
          DEFAULT_LCD_CONTRAST
        #else
          127
        #endif
      ;
      EEPROM_WRITE(lcd_contrast);
    }

    //
    // Power-Loss Recovery
    //
    {
      _FIELD_TEST(recovery_enabled);

      const bool recovery_enabled =
        #if ENABLED(POWER_LOSS_RECOVERY)
          recovery.enabled
        #else
          true
        #endif
      ;
      EEPROM_WRITE(recovery_enabled);
    }

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
      #if BOTH(FWRETRACT, FWRETRACT_AUTORETRACT)
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
          tmc_hybrid_threshold.X = stepperX.get_pwm_thrs();
        #endif
        #if AXIS_HAS_STEALTHCHOP(Y)
          tmc_hybrid_threshold.Y = stepperY.get_pwm_thrs();
        #endif
        #if AXIS_HAS_STEALTHCHOP(Z)
          tmc_hybrid_threshold.Z = stepperZ.get_pwm_thrs();
        #endif
        #if AXIS_HAS_STEALTHCHOP(X2)
          tmc_hybrid_threshold.X2 = stepperX2.get_pwm_thrs();
        #endif
        #if AXIS_HAS_STEALTHCHOP(Y2)
          tmc_hybrid_threshold.Y2 = stepperY2.get_pwm_thrs();
        #endif
        #if AXIS_HAS_STEALTHCHOP(Z2)
          tmc_hybrid_threshold.Z2 = stepperZ2.get_pwm_thrs();
        #endif
        #if AXIS_HAS_STEALTHCHOP(Z3)
          tmc_hybrid_threshold.Z3 = stepperZ3.get_pwm_thrs();
        #endif
        #if MAX_EXTRUDERS
          #if AXIS_HAS_STEALTHCHOP(E0)
            tmc_hybrid_threshold.E0 = stepperE0.get_pwm_thrs();
          #endif
          #if MAX_EXTRUDERS > 1
            #if AXIS_HAS_STEALTHCHOP(E1)
              tmc_hybrid_threshold.E1 = stepperE1.get_pwm_thrs();
            #endif
            #if MAX_EXTRUDERS > 2
              #if AXIS_HAS_STEALTHCHOP(E2)
                tmc_hybrid_threshold.E2 = stepperE2.get_pwm_thrs();
              #endif
              #if MAX_EXTRUDERS > 3
                #if AXIS_HAS_STEALTHCHOP(E3)
                  tmc_hybrid_threshold.E3 = stepperE3.get_pwm_thrs();
                #endif
                #if MAX_EXTRUDERS > 4
                  #if AXIS_HAS_STEALTHCHOP(E4)
                    tmc_hybrid_threshold.E4 = stepperE4.get_pwm_thrs();
                  #endif
                  #if MAX_EXTRUDERS > 5
                    #if AXIS_HAS_STEALTHCHOP(E5)
                      tmc_hybrid_threshold.E5 = stepperE5.get_pwm_thrs();
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
      tmc_sgt_t tmc_sgt{0};
      #if USE_SENSORLESS
        #if X_SENSORLESS
          tmc_sgt.X = stepperX.homing_threshold();
        #endif
        #if X2_SENSORLESS
          tmc_sgt.X2 = stepperX2.homing_threshold();
        #endif
        #if Y_SENSORLESS
          tmc_sgt.Y = stepperY.homing_threshold();
        #endif
        #if Z_SENSORLESS
          tmc_sgt.Z = stepperZ.homing_threshold();
        #endif
      #endif
      EEPROM_WRITE(tmc_sgt);
    }

    //
    // TMC stepping mode
    //
    {
      _FIELD_TEST(tmc_stealth_enabled);

      tmc_stealth_enabled_t tmc_stealth_enabled = { false, false, false, false, false, false, false, false, false, false, false, false, false };

      #if HAS_STEALTHCHOP
        #if AXIS_HAS_STEALTHCHOP(X)
          tmc_stealth_enabled.X = stepperX.get_stealthChop_status();
        #endif
        #if AXIS_HAS_STEALTHCHOP(Y)
          tmc_stealth_enabled.Y = stepperY.get_stealthChop_status();
        #endif
        #if AXIS_HAS_STEALTHCHOP(Z)
          tmc_stealth_enabled.Z = stepperZ.get_stealthChop_status();
        #endif
        #if AXIS_HAS_STEALTHCHOP(X2)
          tmc_stealth_enabled.X2 = stepperX2.get_stealthChop_status();
        #endif
        #if AXIS_HAS_STEALTHCHOP(Y2)
          tmc_stealth_enabled.Y2 = stepperY2.get_stealthChop_status();
        #endif
        #if AXIS_HAS_STEALTHCHOP(Z2)
          tmc_stealth_enabled.Z2 = stepperZ2.get_stealthChop_status();
        #endif
        #if AXIS_HAS_STEALTHCHOP(Z3)
          tmc_stealth_enabled.Z3 = stepperZ3.get_stealthChop_status();
        #endif
        #if MAX_EXTRUDERS
          #if AXIS_HAS_STEALTHCHOP(E0)
            tmc_stealth_enabled.E0 = stepperE0.get_stealthChop_status();
          #endif
          #if MAX_EXTRUDERS > 1
            #if AXIS_HAS_STEALTHCHOP(E1)
              tmc_stealth_enabled.E1 = stepperE1.get_stealthChop_status();
            #endif
            #if MAX_EXTRUDERS > 2
              #if AXIS_HAS_STEALTHCHOP(E2)
                tmc_stealth_enabled.E2 = stepperE2.get_stealthChop_status();
              #endif
              #if MAX_EXTRUDERS > 3
                #if AXIS_HAS_STEALTHCHOP(E3)
                  tmc_stealth_enabled.E3 = stepperE3.get_stealthChop_status();
                #endif
                #if MAX_EXTRUDERS > 4
                  #if AXIS_HAS_STEALTHCHOP(E4)
                    tmc_stealth_enabled.E4 = stepperE4.get_stealthChop_status();
                  #endif
                  #if MAX_EXTRUDERS > 5
                    #if AXIS_HAS_STEALTHCHOP(E5)
                      tmc_stealth_enabled.E5 = stepperE5.get_stealthChop_status();
                    #endif
                  #endif // MAX_EXTRUDERS > 5
                #endif // MAX_EXTRUDERS > 4
              #endif // MAX_EXTRUDERS > 3
            #endif // MAX_EXTRUDERS > 2
          #endif // MAX_EXTRUDERS > 1
        #endif // MAX_EXTRUDERS
      #endif
      EEPROM_WRITE(tmc_stealth_enabled);
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
        const xyz_ulong_t no_current{0};
        EEPROM_WRITE(no_current);
      #endif
    }

    //
    // CNC Coordinate Systems
    //

    _FIELD_TEST(coordinate_system);

    #if ENABLED(CNC_COORDINATE_SYSTEMS)
      EEPROM_WRITE(gcode.coordinate_system);
    #else
      const xyz_pos_t coordinate_system[MAX_COORDINATE_SYSTEMS] = { { 0 } };
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
    #if EXTRUDERS
    {
      #if DISABLED(ADVANCED_PAUSE_FEATURE)
        const fil_change_settings_t fc_settings[EXTRUDERS] = { 0, 0 };
      #endif
      _FIELD_TEST(fc_settings);
      EEPROM_WRITE(fc_settings);
    }
    #endif

    //
    // Multiple Extruders
    //

    #if EXTRUDERS > 1
      _FIELD_TEST(toolchange_settings);
      EEPROM_WRITE(toolchange_settings);
    #endif

    //
    // Backlash Compensation
    //
    {
      #if ENABLED(BACKLASH_GCODE)
        const xyz_float_t &backlash_distance_mm = backlash.distance_mm;
        const uint8_t &backlash_correction = backlash.correction;
      #else
        const xyz_float_t backlash_distance_mm{0};
        const uint8_t backlash_correction = 0;
      #endif
      #if ENABLED(BACKLASH_GCODE) && defined(BACKLASH_SMOOTHING_MM)
        const float &backlash_smoothing_mm = backlash.smoothing_mm;
      #else
        const float backlash_smoothing_mm = 3;
      #endif
      _FIELD_TEST(backlash_distance_mm);
      EEPROM_WRITE(backlash_distance_mm);
      EEPROM_WRITE(backlash_correction);
      EEPROM_WRITE(backlash_smoothing_mm);
    }

    //
    // Extensible UI User Data
    //
    #if ENABLED(EXTENSIBLE_UI)
      {
        char extui_data[ExtUI::eeprom_data_size] = { 0 };
        ExtUI::onStoreSettings(extui_data);
        _FIELD_TEST(extui_data);
        EEPROM_WRITE(extui_data);
      }
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
      DEBUG_ECHO_START();
      DEBUG_ECHOLNPAIR("Settings Stored (", eeprom_size, " bytes; crc ", (uint32_t)final_crc, ")");

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

    #if ENABLED(EXTENSIBLE_UI)
      ExtUI::onConfigurationStoreWritten(!eeprom_error);
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
      DEBUG_ECHO_START();
      DEBUG_ECHOLNPAIR("EEPROM version mismatch (EEPROM=", stored_ver, " Marlin=" EEPROM_VERSION ")");
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
        uint32_t tmp1[XYZ + esteppers];
        float tmp2[XYZ + esteppers];
        feedRate_t tmp3[XYZ + esteppers];
        EEPROM_READ(tmp1);                         // max_acceleration_mm_per_s2
        EEPROM_READ(planner.settings.min_segment_time_us);
        EEPROM_READ(tmp2);                         // axis_steps_per_mm
        EEPROM_READ(tmp3);                         // max_feedrate_mm_s

        if (!validating) LOOP_XYZE_N(i) {
          const bool in = (i < esteppers + XYZ);
          planner.settings.max_acceleration_mm_per_s2[i] = in ? tmp1[i] : pgm_read_dword(&_DMA[ALIM(i, _DMA)]);
          planner.settings.axis_steps_per_mm[i]          = in ? tmp2[i] : pgm_read_float(&_DASU[ALIM(i, _DASU)]);
          planner.settings.max_feedrate_mm_s[i]          = in ? tmp3[i] : pgm_read_float(&_DMF[ALIM(i, _DMF)]);
        }

        EEPROM_READ(planner.settings.acceleration);
        EEPROM_READ(planner.settings.retract_acceleration);
        EEPROM_READ(planner.settings.travel_acceleration);
        EEPROM_READ(planner.settings.min_feedrate_mm_s);
        EEPROM_READ(planner.settings.min_travel_feedrate_mm_s);

        #if HAS_CLASSIC_JERK
          EEPROM_READ(planner.max_jerk);
          #if HAS_LINEAR_E_JERK
            EEPROM_READ(dummy);
          #endif
        #else
          for (uint8_t q = 4; q--;) EEPROM_READ(dummy);
        #endif

        #if DISABLED(CLASSIC_JERK)
          EEPROM_READ(planner.junction_deviation_mm);
        #else
          EEPROM_READ(dummy);
        #endif
      }

      //
      // Home Offset (M206 / M665)
      //
      {
        _FIELD_TEST(home_offset);

        #if HAS_SCARA_OFFSET
          EEPROM_READ(scara_home_offset);
        #else
          #if !HAS_HOME_OFFSET
            xyz_pos_t home_offset;
          #endif
          EEPROM_READ(home_offset);
        #endif
      }

      //
      // Hotend Offsets, if any
      //
      {
        #if HAS_HOTEND_OFFSET
          // Skip hotend 0 which must be 0
          for (uint8_t e = 1; e < HOTENDS; e++)
            EEPROM_READ(hotend_offset[e]);
        #endif
      }

      //
      // Filament Runout Sensor
      //
      {
        #if HAS_FILAMENT_SENSOR
          bool &runout_sensor_enabled = runout.enabled;
        #else
          bool runout_sensor_enabled;
        #endif
        _FIELD_TEST(runout_sensor_enabled);
        EEPROM_READ(runout_sensor_enabled);

        float runout_distance_mm;
        EEPROM_READ(runout_distance_mm);
        #if HAS_FILAMENT_SENSOR && defined(FILAMENT_RUNOUT_DISTANCE_MM)
          if (!validating) runout.set_runout_distance(runout_distance_mm);
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
        _FIELD_TEST(probe_offset);
        #if HAS_BED_PROBE
          xyz_pos_t &zpo = probe_offset;
        #else
          xyz_pos_t zpo;
        #endif
        EEPROM_READ(zpo);
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
            xy_int_t bgs, bs;
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
        _FIELD_TEST(servo_angles);

        #if ENABLED(EDITABLE_SERVO_ANGLES)
          uint16_t (&servo_angles_arr)[EEPROM_NUM_SERVOS][2] = servo_angles;
        #else
          uint16_t servo_angles_arr[EEPROM_NUM_SERVOS][2];
        #endif
        EEPROM_READ(servo_angles_arr);
      }

      //
      // BLTOUCH
      //
      {
        _FIELD_TEST(bltouch_last_written_mode);
        #if ENABLED(BLTOUCH)
          bool &bltouch_last_written_mode = bltouch.last_written_mode;
        #else
          bool bltouch_last_written_mode;
        #endif
        EEPROM_READ(bltouch_last_written_mode);
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

        #elif EITHER(X_DUAL_ENDSTOPS, Y_DUAL_ENDSTOPS) || Z_MULTI_ENDSTOPS

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
        _FIELD_TEST(ui_preheat_hotend_temp);

        #if HOTENDS && HAS_LCD_MENU
          int16_t (&ui_preheat_hotend_temp)[2]  = ui.preheat_hotend_temp,
                  (&ui_preheat_bed_temp)[2]     = ui.preheat_bed_temp;
          uint8_t (&ui_preheat_fan_speed)[2]    = ui.preheat_fan_speed;
        #else
          int16_t ui_preheat_hotend_temp[2], ui_preheat_bed_temp[2];
          uint8_t ui_preheat_fan_speed[2];
        #endif
        EEPROM_READ(ui_preheat_hotend_temp); // 2 floats
        EEPROM_READ(ui_preheat_bed_temp);    // 2 floats
        EEPROM_READ(ui_preheat_fan_speed);   // 2 floats
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
              // Scale PID values since EEPROM values are unscaled
              PID_PARAM(Kp, e) = pidc.Kp;
              PID_PARAM(Ki, e) = scalePID_i(pidc.Ki);
              PID_PARAM(Kd, e) = scalePID_d(pidc.Kd);
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
          if (!validating && pid.Kp != DUMMY_PID_VALUE) {
            // Scale PID values since EEPROM values are unscaled
            thermalManager.temp_bed.pid.Kp = pid.Kp;
            thermalManager.temp_bed.pid.Ki = scalePID_i(pid.Ki);
            thermalManager.temp_bed.pid.Kd = scalePID_d(pid.Kd);
          }
        #endif
      }

      //
      // User-defined Thermistors
      //
      #if HAS_USER_THERMISTORS
      {
        _FIELD_TEST(user_thermistor);
        EEPROM_READ(thermalManager.user_thermistor);
      }
      #endif

      //
      // LCD Contrast
      //
      {
        _FIELD_TEST(lcd_contrast);

        int16_t lcd_contrast;
        EEPROM_READ(lcd_contrast);
        #if HAS_LCD_CONTRAST
          ui.set_contrast(lcd_contrast);
        #endif
      }

      //
      // Power-Loss Recovery
      //
      {
        _FIELD_TEST(recovery_enabled);

        #if ENABLED(POWER_LOSS_RECOVERY)
          EEPROM_READ(recovery.enabled);
        #else
          bool recovery_enabled;
          EEPROM_READ(recovery_enabled);
        #endif
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
        #if BOTH(FWRETRACT, FWRETRACT_AUTORETRACT)
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
          if (!validating) {
            #if AXIS_HAS_STEALTHCHOP(X)
              stepperX.set_pwm_thrs(tmc_hybrid_threshold.X);
            #endif
            #if AXIS_HAS_STEALTHCHOP(Y)
              stepperY.set_pwm_thrs(tmc_hybrid_threshold.Y);
            #endif
            #if AXIS_HAS_STEALTHCHOP(Z)
              stepperZ.set_pwm_thrs(tmc_hybrid_threshold.Z);
            #endif
            #if AXIS_HAS_STEALTHCHOP(X2)
              stepperX2.set_pwm_thrs(tmc_hybrid_threshold.X2);
            #endif
            #if AXIS_HAS_STEALTHCHOP(Y2)
              stepperY2.set_pwm_thrs(tmc_hybrid_threshold.Y2);
            #endif
            #if AXIS_HAS_STEALTHCHOP(Z2)
              stepperZ2.set_pwm_thrs(tmc_hybrid_threshold.Z2);
            #endif
            #if AXIS_HAS_STEALTHCHOP(Z3)
              stepperZ3.set_pwm_thrs(tmc_hybrid_threshold.Z3);
            #endif
            #if AXIS_HAS_STEALTHCHOP(E0)
              stepperE0.set_pwm_thrs(tmc_hybrid_threshold.E0);
            #endif
            #if AXIS_HAS_STEALTHCHOP(E1)
              stepperE1.set_pwm_thrs(tmc_hybrid_threshold.E1);
            #endif
            #if AXIS_HAS_STEALTHCHOP(E2)
              stepperE2.set_pwm_thrs(tmc_hybrid_threshold.E2);
            #endif
            #if AXIS_HAS_STEALTHCHOP(E3)
              stepperE3.set_pwm_thrs(tmc_hybrid_threshold.E3);
            #endif
            #if AXIS_HAS_STEALTHCHOP(E4)
              stepperE4.set_pwm_thrs(tmc_hybrid_threshold.E4);
            #endif
            #if AXIS_HAS_STEALTHCHOP(E5)
              stepperE5.set_pwm_thrs(tmc_hybrid_threshold.E5);
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
                stepperX.homing_threshold(tmc_sgt.X);
              #endif
              #if AXIS_HAS_STALLGUARD(X2) && !X2_SENSORLESS
                stepperX2.homing_threshold(tmc_sgt.X);
              #endif
            #endif
            #if X2_SENSORLESS
              stepperX2.homing_threshold(tmc_sgt.X2);
            #endif
            #ifdef Y_STALL_SENSITIVITY
              #if AXIS_HAS_STALLGUARD(Y)
                stepperY.homing_threshold(tmc_sgt.Y);
              #endif
              #if AXIS_HAS_STALLGUARD(Y2)
                stepperY2.homing_threshold(tmc_sgt.Y);
              #endif
            #endif
            #ifdef Z_STALL_SENSITIVITY
              #if AXIS_HAS_STALLGUARD(Z)
                stepperZ.homing_threshold(tmc_sgt.Z);
              #endif
              #if AXIS_HAS_STALLGUARD(Z2)
                stepperZ2.homing_threshold(tmc_sgt.Z);
              #endif
              #if AXIS_HAS_STALLGUARD(Z3)
                stepperZ3.homing_threshold(tmc_sgt.Z);
              #endif
            #endif
          }
        #endif
      }

      // TMC stepping mode
      {
        _FIELD_TEST(tmc_stealth_enabled);

        tmc_stealth_enabled_t tmc_stealth_enabled;
        EEPROM_READ(tmc_stealth_enabled);

        #if HAS_TRINAMIC

          #define SET_STEPPING_MODE(ST) stepper##ST.stored.stealthChop_enabled = tmc_stealth_enabled.ST; stepper##ST.refresh_stepping_mode();
          if (!validating) {
            #if AXIS_HAS_STEALTHCHOP(X)
              SET_STEPPING_MODE(X);
            #endif
            #if AXIS_HAS_STEALTHCHOP(Y)
              SET_STEPPING_MODE(Y);
            #endif
            #if AXIS_HAS_STEALTHCHOP(Z)
              SET_STEPPING_MODE(Z);
            #endif
            #if AXIS_HAS_STEALTHCHOP(X2)
              SET_STEPPING_MODE(X2);
            #endif
            #if AXIS_HAS_STEALTHCHOP(Y2)
              SET_STEPPING_MODE(Y2);
            #endif
            #if AXIS_HAS_STEALTHCHOP(Z2)
              SET_STEPPING_MODE(Z2);
            #endif
            #if AXIS_HAS_STEALTHCHOP(Z3)
              SET_STEPPING_MODE(Z3);
            #endif
            #if AXIS_HAS_STEALTHCHOP(E0)
              SET_STEPPING_MODE(E0);
            #endif
            #if AXIS_HAS_STEALTHCHOP(E1)
              SET_STEPPING_MODE(E1);
            #endif
            #if AXIS_HAS_STEALTHCHOP(E2)
              SET_STEPPING_MODE(E2);
            #endif
            #if AXIS_HAS_STEALTHCHOP(E3)
              SET_STEPPING_MODE(E3);
            #endif
            #if AXIS_HAS_STEALTHCHOP(E4)
              SET_STEPPING_MODE(E4);
            #endif
            #if AXIS_HAS_STEALTHCHOP(E5)
              SET_STEPPING_MODE(E5);
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
          xyz_pos_t coordinate_system[MAX_COORDINATE_SYSTEMS];
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
      #if EXTRUDERS
      {
        #if DISABLED(ADVANCED_PAUSE_FEATURE)
          fil_change_settings_t fc_settings[EXTRUDERS];
        #endif
        _FIELD_TEST(fc_settings);
        EEPROM_READ(fc_settings);
      }
      #endif

      //
      // Tool-change settings
      //
      #if EXTRUDERS > 1
        _FIELD_TEST(toolchange_settings);
        EEPROM_READ(toolchange_settings);
      #endif

      //
      // Backlash Compensation
      //
      {
        #if ENABLED(BACKLASH_GCODE)
          xyz_float_t &backlash_distance_mm = backlash.distance_mm;
          uint8_t &backlash_correction = backlash.correction;
        #else
          float backlash_distance_mm[XYZ];
          uint8_t backlash_correction;
        #endif
        #if ENABLED(BACKLASH_GCODE) && defined(BACKLASH_SMOOTHING_MM)
          float &backlash_smoothing_mm = backlash.smoothing_mm;
        #else
          float backlash_smoothing_mm;
        #endif
        _FIELD_TEST(backlash_distance_mm);
        EEPROM_READ(backlash_distance_mm);
        EEPROM_READ(backlash_correction);
        EEPROM_READ(backlash_smoothing_mm);
      }

      //
      // Extensible UI User Data
      //
      #if ENABLED(EXTENSIBLE_UI)
        // This is a significant hardware change; don't reserve EEPROM space when not present
        {
          const char extui_data[ExtUI::eeprom_data_size] = { 0 };
          _FIELD_TEST(extui_data);
          EEPROM_READ(extui_data);
          if (!validating) ExtUI::onLoadSettings(extui_data);
        }
      #endif

      eeprom_error = size_error(eeprom_index - (EEPROM_OFFSET));
      if (eeprom_error) {
        DEBUG_ECHO_START();
        DEBUG_ECHOLNPAIR("Index: ", int(eeprom_index - (EEPROM_OFFSET)), " Size: ", datasize());
      }
      else if (working_crc != stored_crc) {
        eeprom_error = true;
        DEBUG_ERROR_START();
        DEBUG_ECHOLNPAIR("EEPROM CRC mismatch - (stored) ", stored_crc, " != ", working_crc, " (calculated)!");
      }
      else if (!validating) {
        DEBUG_ECHO_START();
        DEBUG_ECHO(version);
        DEBUG_ECHOLNPAIR(" stored settings retrieved (", eeprom_index - (EEPROM_OFFSET), " bytes; crc ", (uint32_t)working_crc, ")");
      }

      if (!validating && !eeprom_error) postprocess();

      #if ENABLED(AUTO_BED_LEVELING_UBL)
        if (!validating) {
          ubl.report_state();

          if (!ubl.sanity_check()) {
            SERIAL_EOL();
            #if ENABLED(EEPROM_CHITCHAT)
              ubl.echo_name();
              DEBUG_ECHOLNPGM(" initialized.\n");
            #endif
          }
          else {
            eeprom_error = true;
            #if ENABLED(EEPROM_CHITCHAT)
              DEBUG_ECHOPGM("?Can't enable ");
              ubl.echo_name();
              DEBUG_ECHOLNPGM(".");
            #endif
            ubl.reset();
          }

          if (ubl.storage_slot >= 0) {
            load_mesh(ubl.storage_slot);
            DEBUG_ECHOLNPAIR("Mesh ", ubl.storage_slot, " loaded from storage.");
          }
          else {
            ubl.reset();
            DEBUG_ECHOLNPGM("UBL reset");
          }
        }
      #endif
    }

    #if ENABLED(EEPROM_CHITCHAT) && DISABLED(DISABLE_M503)
      if (!validating) report();
    #endif
    EEPROM_FINISH();

    return !eeprom_error;
  }

  #ifdef ARCHIM2_SPI_FLASH_EEPROM_BACKUP_SIZE
    extern bool restoreEEPROM();
  #endif

  bool MarlinSettings::validate() {
    validating = true;
    #ifdef ARCHIM2_SPI_FLASH_EEPROM_BACKUP_SIZE
      bool success = _load();
      if (!success && restoreEEPROM()) {
        SERIAL_ECHOLNPGM("Recovered backup EEPROM settings from SPI Flash");
        success = _load();
      }
    #else
      const bool success = _load();
    #endif
    validating = false;
    return success;
  }

  bool MarlinSettings::load() {
    if (validate()) {
      const bool success = _load();
      #if ENABLED(EXTENSIBLE_UI)
        ExtUI::onConfigurationStoreRead(success);
      #endif
      return success;
    }
    reset();
    #if ENABLED(EEPROM_AUTO_INIT)
      (void)save();
      SERIAL_ECHO_MSG("EEPROM Initialized");
    #endif
    return false;
  }

  #if ENABLED(AUTO_BED_LEVELING_UBL)

    inline void ubl_invalid_slot(const int s) {
      #if ENABLED(EEPROM_CHITCHAT)
        DEBUG_ECHOLNPGM("?Invalid slot.");
        DEBUG_ECHO(s);
        DEBUG_ECHOLNPGM(" mesh slots available.");
      #else
        UNUSED(s);
      #endif
    }

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
          ubl_invalid_slot(a);
          DEBUG_ECHOLNPAIR("E2END=", persistentStore.capacity() - 1, " meshes_end=", meshes_end, " slot=", slot);
          DEBUG_EOL();
          return;
        }

        int pos = mesh_slot_offset(slot);
        uint16_t crc = 0;

        // Write crc to MAT along with other data, or just tack on to the beginning or end
        persistentStore.access_start();
        const bool status = persistentStore.write_data(pos, (uint8_t *)&ubl.z_values, sizeof(ubl.z_values), &crc);
        persistentStore.access_finish();

        if (status) SERIAL_ECHOLNPGM("?Unable to save mesh data.");
        else        DEBUG_ECHOLNPAIR("Mesh saved in slot ", slot);

      #else

        // Other mesh types

      #endif
    }

    void MarlinSettings::load_mesh(const int8_t slot, void * const into/*=nullptr*/) {

      #if ENABLED(AUTO_BED_LEVELING_UBL)

        const int16_t a = settings.calc_num_meshes();

        if (!WITHIN(slot, 0, a - 1)) {
          ubl_invalid_slot(a);
          return;
        }

        int pos = mesh_slot_offset(slot);
        uint16_t crc = 0;
        uint8_t * const dest = into ? (uint8_t*)into : (uint8_t*)&ubl.z_values;

        persistentStore.access_start();
        const uint16_t status = persistentStore.read_data(pos, dest, sizeof(ubl.z_values), &crc);
        persistentStore.access_finish();

        if (status) SERIAL_ECHOLNPGM("?Unable to load mesh data.");
        else        DEBUG_ECHOLNPAIR("Mesh loaded from slot ", slot);

        EEPROM_FINISH();

      #else

        // Other mesh types

      #endif
    }

    //void MarlinSettings::delete_mesh() { return; }
    //void MarlinSettings::defrag_meshes() { return; }

  #endif // AUTO_BED_LEVELING_UBL

#else // !EEPROM_SETTINGS

  bool MarlinSettings::save() {
    DEBUG_ERROR_MSG("EEPROM disabled");
    return false;
  }

#endif // !EEPROM_SETTINGS

/**
 * M502 - Reset Configuration
 */
void MarlinSettings::reset() {
  LOOP_XYZE_N(i) {
    planner.settings.max_acceleration_mm_per_s2[i] = pgm_read_dword(&_DMA[ALIM(i, _DMA)]);
    planner.settings.axis_steps_per_mm[i]          = pgm_read_float(&_DASU[ALIM(i, _DASU)]);
    planner.settings.max_feedrate_mm_s[i]          = pgm_read_float(&_DMF[ALIM(i, _DMF)]);
  }

  planner.settings.min_segment_time_us = DEFAULT_MINSEGMENTTIME;
  planner.settings.acceleration = DEFAULT_ACCELERATION;
  planner.settings.retract_acceleration = DEFAULT_RETRACT_ACCELERATION;
  planner.settings.travel_acceleration = DEFAULT_TRAVEL_ACCELERATION;
  planner.settings.min_feedrate_mm_s = feedRate_t(DEFAULT_MINIMUMFEEDRATE);
  planner.settings.min_travel_feedrate_mm_s = feedRate_t(DEFAULT_MINTRAVELFEEDRATE);

  #if HAS_CLASSIC_JERK
    #ifndef DEFAULT_XJERK
      #define DEFAULT_XJERK 0
    #endif
    #ifndef DEFAULT_YJERK
      #define DEFAULT_YJERK 0
    #endif
    #ifndef DEFAULT_ZJERK
      #define DEFAULT_ZJERK 0
    #endif
    planner.max_jerk.set(DEFAULT_XJERK, DEFAULT_YJERK, DEFAULT_ZJERK);
    #if HAS_CLASSIC_E_JERK
      planner.max_jerk.e = DEFAULT_EJERK;
    #endif
  #endif

  #if DISABLED(CLASSIC_JERK)
    planner.junction_deviation_mm = float(JUNCTION_DEVIATION_MM);
  #endif

  #if HAS_SCARA_OFFSET
    scara_home_offset.reset();
  #elif HAS_HOME_OFFSET
    home_offset.reset();
  #endif

  #if HAS_HOTEND_OFFSET
    reset_hotend_offsets();
  #endif

  //
  // Filament Runout Sensor
  //

  #if HAS_FILAMENT_SENSOR
    runout.enabled = true;
    runout.reset();
    #ifdef FILAMENT_RUNOUT_DISTANCE_MM
      runout.set_runout_distance(FILAMENT_RUNOUT_DISTANCE_MM);
    #endif
  #endif

  //
  // Tool-change Settings
  //

  #if EXTRUDERS > 1
    #if ENABLED(TOOLCHANGE_FILAMENT_SWAP)
      toolchange_settings.swap_length = TOOLCHANGE_FIL_SWAP_LENGTH;
      toolchange_settings.extra_prime = TOOLCHANGE_FIL_EXTRA_PRIME;
      toolchange_settings.prime_speed = TOOLCHANGE_FIL_SWAP_PRIME_SPEED;
      toolchange_settings.retract_speed = TOOLCHANGE_FIL_SWAP_RETRACT_SPEED;
    #endif
    #if ENABLED(TOOLCHANGE_PARK)
      constexpr xyz_pos_t tpxy = TOOLCHANGE_PARK_XY;
      toolchange_settings.change_point = tpxy;
    #endif
    toolchange_settings.z_raise = TOOLCHANGE_ZRAISE;
  #endif

  #if ENABLED(BACKLASH_GCODE)
    backlash.correction = (BACKLASH_CORRECTION) * 255;
    constexpr xyz_float_t tmp = BACKLASH_DISTANCE_MM;
    backlash.distance_mm = tmp;
    #ifdef BACKLASH_SMOOTHING_MM
      backlash.smoothing_mm = BACKLASH_SMOOTHING_MM;
    #endif
  #endif

  #if ENABLED(EXTENSIBLE_UI)
    ExtUI::onFactoryReset();
  #endif

  //
  // Magnetic Parking Extruder
  //

  #if ENABLED(MAGNETIC_PARKING_EXTRUDER)
    mpe_settings_init();
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
    constexpr float dpo[XYZ] = NOZZLE_TO_PROBE_OFFSET;
    static_assert(COUNT(dpo) == 3, "NOZZLE_TO_PROBE_OFFSET must contain offsets for X, Y, and Z.");
    LOOP_XYZ(a) probe_offset[a] = dpo[a];
  #endif

  //
  // Servo Angles
  //

  #if ENABLED(EDITABLE_SERVO_ANGLES)
    COPY(servo_angles, base_servo_angles);
  #endif

  //
  // BLTOUCH
  //
  //#if ENABLED(BLTOUCH)
  //  bltouch.last_written_mode;
  //#endif

  //
  // Endstop Adjustments
  //

  #if ENABLED(DELTA)
    const abc_float_t adj = DELTA_ENDSTOP_ADJ, dta = DELTA_TOWER_ANGLE_TRIM;
    delta_height = DELTA_HEIGHT;
    delta_endstop_adj = adj;
    delta_radius = DELTA_RADIUS;
    delta_diagonal_rod = DELTA_DIAGONAL_ROD;
    delta_segments_per_second = DELTA_SEGMENTS_PER_SECOND;
    delta_calibration_radius = DELTA_CALIBRATION_RADIUS;
    delta_tower_angle_trim = dta;

  #elif EITHER(X_DUAL_ENDSTOPS, Y_DUAL_ENDSTOPS) || Z_MULTI_ENDSTOPS

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

  //
  // Preheat parameters
  //

  #if HOTENDS && HAS_LCD_MENU
    ui.preheat_hotend_temp[0] = PREHEAT_1_TEMP_HOTEND;
    ui.preheat_hotend_temp[1] = PREHEAT_2_TEMP_HOTEND;
    ui.preheat_bed_temp[0] = PREHEAT_1_TEMP_BED;
    ui.preheat_bed_temp[1] = PREHEAT_2_TEMP_BED;
    ui.preheat_fan_speed[0] = PREHEAT_1_FAN_SPEED;
    ui.preheat_fan_speed[1] = PREHEAT_2_FAN_SPEED;
  #endif

  //
  // Hotend PID
  //

  #if ENABLED(PIDTEMP)
    HOTEND_LOOP() {
      PID_PARAM(Kp, e) = float(DEFAULT_Kp);
      PID_PARAM(Ki, e) = scalePID_i(DEFAULT_Ki);
      PID_PARAM(Kd, e) = scalePID_d(DEFAULT_Kd);
      #if ENABLED(PID_EXTRUSION_SCALING)
        PID_PARAM(Kc, e) = DEFAULT_Kc;
      #endif
    }
  #endif

  //
  // PID Extrusion Scaling
  //

  #if ENABLED(PID_EXTRUSION_SCALING)
    thermalManager.lpq_len = 20;  // Default last-position-queue size
  #endif

  //
  // Heated Bed PID
  //

  #if ENABLED(PIDTEMPBED)
    thermalManager.temp_bed.pid.Kp = DEFAULT_bedKp;
    thermalManager.temp_bed.pid.Ki = scalePID_i(DEFAULT_bedKi);
    thermalManager.temp_bed.pid.Kd = scalePID_d(DEFAULT_bedKd);
  #endif

  //
  // User-Defined Thermistors
  //

  #if HAS_USER_THERMISTORS
    thermalManager.reset_user_thermistors();
  #endif

  //
  // LCD Contrast
  //

  #if HAS_LCD_CONTRAST
    ui.set_contrast(DEFAULT_LCD_CONTRAST);
  #endif

  //
  // Power-Loss Recovery
  //

  #if ENABLED(POWER_LOSS_RECOVERY)
    recovery.enable(true);
  #endif

  //
  // Firmware Retraction
  //

  #if ENABLED(FWRETRACT)
    fwretract.reset();
  #endif

  //
  // Volumetric & Filament Size
  //

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

  //
  // Linear Advance
  //

  #if ENABLED(LIN_ADVANCE)
    LOOP_L_N(i, EXTRUDERS) {
      planner.extruder_advance_K[i] = LIN_ADVANCE_K;
    #if ENABLED(EXTRA_LIN_ADVANCE_K)
      saved_extruder_advance_K[i] = LIN_ADVANCE_K;
    #endif
    }
  #endif

  //
  // Motor Current PWM
  //

  #if HAS_MOTOR_CURRENT_PWM
    constexpr uint32_t tmp_motor_current_setting[3] = PWM_MOTOR_CURRENT;
    for (uint8_t q = 3; q--;)
      stepper.digipot_current(q, (stepper.motor_current_setting[q] = tmp_motor_current_setting[q]));
  #endif

  //
  // CNC Coordinate System
  //

  #if ENABLED(CNC_COORDINATE_SYSTEMS)
    (void)gcode.select_coordinate_system(-1); // Go back to machine space
  #endif

  //
  // Skew Correction
  //

  #if ENABLED(SKEW_CORRECTION_GCODE)
    planner.skew_factor.xy = XY_SKEW_FACTOR;
    #if ENABLED(SKEW_CORRECTION_FOR_Z)
      planner.skew_factor.xz = XZ_SKEW_FACTOR;
      planner.skew_factor.yz = YZ_SKEW_FACTOR;
    #endif
  #endif

  //
  // Advanced Pause filament load & unload lengths
  //

  #if ENABLED(ADVANCED_PAUSE_FEATURE)
    for (uint8_t e = 0; e < EXTRUDERS; e++) {
      fc_settings[e].unload_length = FILAMENT_CHANGE_UNLOAD_LENGTH;
      fc_settings[e].load_length = FILAMENT_CHANGE_FAST_LOAD_LENGTH;
    }
  #endif

  postprocess();

  DEBUG_ECHO_START();
  DEBUG_ECHOLNPGM("Hardcoded Default Settings Loaded");

  #if ENABLED(EXTENSIBLE_UI)
    ExtUI::onFactoryReset();
  #endif
}

#if DISABLED(DISABLE_M503)

  #define CONFIG_ECHO_START()       do{ if (!forReplay) SERIAL_ECHO_START(); }while(0)
  #define CONFIG_ECHO_MSG(STR)      do{ CONFIG_ECHO_START(); SERIAL_ECHOLNPGM(STR); }while(0)
  #define CONFIG_ECHO_HEADING(STR)  do{ if (!forReplay) { CONFIG_ECHO_START(); SERIAL_ECHOLNPGM(STR); } }while(0)

  #if HAS_TRINAMIC
    inline void say_M906(const bool forReplay) { CONFIG_ECHO_START(); SERIAL_ECHOPGM("  M906"); }
    #if HAS_STEALTHCHOP
      void say_M569(const bool forReplay, const char * const etc=nullptr, const bool newLine = false) {
        CONFIG_ECHO_START();
        SERIAL_ECHOPGM("  M569 S1");
        if (etc) {
          SERIAL_CHAR(' ');
          serialprintPGM(etc);
        }
        if (newLine) SERIAL_EOL();
      }
    #endif
    #if ENABLED(HYBRID_THRESHOLD)
      inline void say_M913(const bool forReplay) { CONFIG_ECHO_START(); SERIAL_ECHOPGM("  M913"); }
    #endif
    #if USE_SENSORLESS
      inline void say_M914() { SERIAL_ECHOPGM("  M914"); }
    #endif
  #endif

  #if ENABLED(ADVANCED_PAUSE_FEATURE)
    inline void say_M603(const bool forReplay) { CONFIG_ECHO_START(); SERIAL_ECHOPGM("  M603 "); }
  #endif

  inline void say_units(const bool colon) {
    serialprintPGM(
      #if ENABLED(INCH_MODE_SUPPORT)
        parser.linear_unit_factor != 1.0 ? PSTR(" (in)") :
      #endif
      PSTR(" (mm)")
    );
    if (colon) SERIAL_ECHOLNPGM(":");
  }

  void report_M92(const bool echo=true, const int8_t e=-1);

  /**
   * M503 - Report current settings in RAM
   *
   * Unless specifically disabled, M503 is available even without EEPROM
   */
  void MarlinSettings::report(const bool forReplay) {
    /**
     * Announce current units, in case inches are being displayed
     */
    CONFIG_ECHO_START();
    #if ENABLED(INCH_MODE_SUPPORT)
      SERIAL_ECHOPGM("  G2");
      SERIAL_CHAR(parser.linear_unit_factor == 1.0 ? '1' : '0');
      SERIAL_ECHOPGM(" ;");
      say_units(false);
    #else
      SERIAL_ECHOPGM("  G21    ; Units in mm");
      say_units(false);
    #endif
    SERIAL_EOL();

    #if HAS_LCD_MENU

      // Temperature units - for Ultipanel temperature options

      CONFIG_ECHO_START();
      #if ENABLED(TEMPERATURE_UNITS_SUPPORT)
        SERIAL_ECHOPGM("  M149 ");
        SERIAL_CHAR(parser.temp_units_code());
        SERIAL_ECHOPGM(" ; Units in ");
        serialprintPGM(parser.temp_units_name());
      #else
        SERIAL_ECHOLNPGM("  M149 C ; Units in Celsius");
      #endif

    #endif

    SERIAL_EOL();

    #if DISABLED(NO_VOLUMETRICS)

      /**
       * Volumetric extrusion M200
       */
      if (!forReplay) {
        CONFIG_ECHO_START();
        SERIAL_ECHOPGM("Filament settings:");
        if (parser.volumetric_enabled)
          SERIAL_EOL();
        else
          SERIAL_ECHOLNPGM(" Disabled");
      }

      CONFIG_ECHO_START();
      SERIAL_ECHOLNPAIR("  M200 D", LINEAR_UNIT(planner.filament_size[0]));
      #if EXTRUDERS > 1
        CONFIG_ECHO_START();
        SERIAL_ECHOLNPAIR("  M200 T1 D", LINEAR_UNIT(planner.filament_size[1]));
        #if EXTRUDERS > 2
          CONFIG_ECHO_START();
          SERIAL_ECHOLNPAIR("  M200 T2 D", LINEAR_UNIT(planner.filament_size[2]));
          #if EXTRUDERS > 3
            CONFIG_ECHO_START();
            SERIAL_ECHOLNPAIR("  M200 T3 D", LINEAR_UNIT(planner.filament_size[3]));
            #if EXTRUDERS > 4
              CONFIG_ECHO_START();
              SERIAL_ECHOLNPAIR("  M200 T4 D", LINEAR_UNIT(planner.filament_size[4]));
              #if EXTRUDERS > 5
                CONFIG_ECHO_START();
                SERIAL_ECHOLNPAIR("  M200 T5 D", LINEAR_UNIT(planner.filament_size[5]));
              #endif // EXTRUDERS > 5
            #endif // EXTRUDERS > 4
          #endif // EXTRUDERS > 3
        #endif // EXTRUDERS > 2
      #endif // EXTRUDERS > 1

      if (!parser.volumetric_enabled)
        CONFIG_ECHO_MSG("  M200 D0");

    #endif // !NO_VOLUMETRICS

    CONFIG_ECHO_HEADING("Steps per unit:");
    report_M92(!forReplay);

    CONFIG_ECHO_HEADING("Maximum feedrates (units/s):");
    CONFIG_ECHO_START();
    SERIAL_ECHOLNPAIR(
        "  M203 X", LINEAR_UNIT(planner.settings.max_feedrate_mm_s[X_AXIS])
      , " Y", LINEAR_UNIT(planner.settings.max_feedrate_mm_s[Y_AXIS])
      , " Z", LINEAR_UNIT(planner.settings.max_feedrate_mm_s[Z_AXIS])
      #if DISABLED(DISTINCT_E_FACTORS)
        , " E", VOLUMETRIC_UNIT(planner.settings.max_feedrate_mm_s[E_AXIS])
      #endif
    );
    #if ENABLED(DISTINCT_E_FACTORS)
      CONFIG_ECHO_START();
      for (uint8_t i = 0; i < E_STEPPERS; i++) {
        SERIAL_ECHOLNPAIR(
            "  M203 T", (int)i
          , " E", VOLUMETRIC_UNIT(planner.settings.max_feedrate_mm_s[E_AXIS_N(i)])
        );
      }
    #endif

    CONFIG_ECHO_HEADING("Maximum Acceleration (units/s2):");
    CONFIG_ECHO_START();
    SERIAL_ECHOLNPAIR(
        "  M201 X", LINEAR_UNIT(planner.settings.max_acceleration_mm_per_s2[X_AXIS])
      , " Y", LINEAR_UNIT(planner.settings.max_acceleration_mm_per_s2[Y_AXIS])
      , " Z", LINEAR_UNIT(planner.settings.max_acceleration_mm_per_s2[Z_AXIS])
      #if DISABLED(DISTINCT_E_FACTORS)
        , " E", VOLUMETRIC_UNIT(planner.settings.max_acceleration_mm_per_s2[E_AXIS])
      #endif
    );
    #if ENABLED(DISTINCT_E_FACTORS)
      CONFIG_ECHO_START();
      for (uint8_t i = 0; i < E_STEPPERS; i++)
        SERIAL_ECHOLNPAIR(
            "  M201 T", (int)i
          , " E", VOLUMETRIC_UNIT(planner.settings.max_acceleration_mm_per_s2[E_AXIS_N(i)])
        );
    #endif

    CONFIG_ECHO_HEADING("Acceleration (units/s2): P<print_accel> R<retract_accel> T<travel_accel>");
    CONFIG_ECHO_START();
    SERIAL_ECHOLNPAIR(
        "  M204 P", LINEAR_UNIT(planner.settings.acceleration)
      , " R", LINEAR_UNIT(planner.settings.retract_acceleration)
      , " T", LINEAR_UNIT(planner.settings.travel_acceleration)
    );

    if (!forReplay) {
      CONFIG_ECHO_START();
      SERIAL_ECHOPGM("Advanced: B<min_segment_time_us> S<min_feedrate> T<min_travel_feedrate>");
      #if DISABLED(CLASSIC_JERK)
        SERIAL_ECHOPGM(" J<junc_dev>");
      #endif
      #if HAS_CLASSIC_JERK
        SERIAL_ECHOPGM(" X<max_x_jerk> Y<max_y_jerk> Z<max_z_jerk>");
        #if HAS_CLASSIC_E_JERK
          SERIAL_ECHOPGM(" E<max_e_jerk>");
        #endif
      #endif
      SERIAL_EOL();
    }
    CONFIG_ECHO_START();
    SERIAL_ECHOLNPAIR(
        "  M205 B", LINEAR_UNIT(planner.settings.min_segment_time_us)
      , " S", LINEAR_UNIT(planner.settings.min_feedrate_mm_s)
      , " T", LINEAR_UNIT(planner.settings.min_travel_feedrate_mm_s)
      #if DISABLED(CLASSIC_JERK)
        , " J", LINEAR_UNIT(planner.junction_deviation_mm)
      #endif
      #if HAS_CLASSIC_JERK
        , " X", LINEAR_UNIT(planner.max_jerk.x)
        , " Y", LINEAR_UNIT(planner.max_jerk.y)
        , " Z", LINEAR_UNIT(planner.max_jerk.z)
        #if HAS_CLASSIC_E_JERK
          , " E", LINEAR_UNIT(planner.max_jerk.e)
        #endif
      #endif
    );

    #if HAS_M206_COMMAND
      CONFIG_ECHO_HEADING("Home offset:");
      CONFIG_ECHO_START();
      SERIAL_ECHOLNPAIR("  M206"
        #if IS_CARTESIAN
          " X", LINEAR_UNIT(home_offset.x),
          " Y", LINEAR_UNIT(home_offset.y),
        #endif
        " Z", LINEAR_UNIT(home_offset.z)
      );
    #endif

    #if HAS_HOTEND_OFFSET
      CONFIG_ECHO_HEADING("Hotend offsets:");
      CONFIG_ECHO_START();
      for (uint8_t e = 1; e < HOTENDS; e++) {
        SERIAL_ECHOPAIR(
          "  M218 T", (int)e,
          " X", LINEAR_UNIT(hotend_offset[e].x), " Y", LINEAR_UNIT(hotend_offset[e].y)
        );
        SERIAL_ECHOLNPAIR_F(" Z", LINEAR_UNIT(hotend_offset[e].z), 3);
      }
    #endif

    /**
     * Bed Leveling
     */
    #if HAS_LEVELING

      #if ENABLED(MESH_BED_LEVELING)

        CONFIG_ECHO_HEADING("Mesh Bed Leveling:");

      #elif ENABLED(AUTO_BED_LEVELING_UBL)

        if (!forReplay) {
          CONFIG_ECHO_START();
          ubl.echo_name();
          SERIAL_ECHOLNPGM(":");
        }

      #elif HAS_ABL_OR_UBL

        CONFIG_ECHO_HEADING("Auto Bed Leveling:");

      #endif

      CONFIG_ECHO_START();
      SERIAL_ECHOLNPAIR(
        "  M420 S", planner.leveling_active ? 1 : 0
        #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
          , " Z", LINEAR_UNIT(planner.z_fade_height)
        #endif
      );

      #if ENABLED(MESH_BED_LEVELING)

        if (leveling_is_valid()) {
          for (uint8_t py = 0; py < GRID_MAX_POINTS_Y; py++) {
            for (uint8_t px = 0; px < GRID_MAX_POINTS_X; px++) {
              CONFIG_ECHO_START();
              SERIAL_ECHOPAIR("  G29 S3 X", (int)px + 1, " Y", (int)py + 1);
              SERIAL_ECHOLNPAIR_F(" Z", LINEAR_UNIT(mbl.z_values[px][py]), 5);
            }
          }
        }

      #elif ENABLED(AUTO_BED_LEVELING_UBL)

        if (!forReplay) {
          SERIAL_EOL();
          ubl.report_state();
          SERIAL_ECHOLNPAIR("\nActive Mesh Slot: ", ubl.storage_slot);
          SERIAL_ECHOLNPAIR("EEPROM can hold ", calc_num_meshes(), " meshes.\n");
        }

       //ubl.report_current_mesh();   // This is too verbose for large meshes. A better (more terse)
                                                  // solution needs to be found.
      #elif ENABLED(AUTO_BED_LEVELING_BILINEAR)

        if (leveling_is_valid()) {
          for (uint8_t py = 0; py < GRID_MAX_POINTS_Y; py++) {
            for (uint8_t px = 0; px < GRID_MAX_POINTS_X; px++) {
              CONFIG_ECHO_START();
              SERIAL_ECHOPAIR("  G29 W I", (int)px, " J", (int)py);
              SERIAL_ECHOLNPAIR_F(" Z", LINEAR_UNIT(z_values[px][py]), 5);
            }
          }
        }

      #endif

    #endif // HAS_LEVELING

    #if ENABLED(EDITABLE_SERVO_ANGLES)

      CONFIG_ECHO_HEADING("Servo Angles:");
      for (uint8_t i = 0; i < NUM_SERVOS; i++) {
        switch (i) {
          #if ENABLED(SWITCHING_EXTRUDER)
            case SWITCHING_EXTRUDER_SERVO_NR:
            #if EXTRUDERS > 3
              case SWITCHING_EXTRUDER_E23_SERVO_NR:
            #endif
          #elif ENABLED(SWITCHING_NOZZLE)
            case SWITCHING_NOZZLE_SERVO_NR:
          #elif (ENABLED(BLTOUCH) && defined(BLTOUCH_ANGLES)) || (defined(Z_SERVO_ANGLES) && defined(Z_PROBE_SERVO_NR))
            case Z_PROBE_SERVO_NR:
          #endif
            CONFIG_ECHO_START();
            SERIAL_ECHOLNPAIR("  M281 P", int(i), " L", servo_angles[i][0], " U", servo_angles[i][1]);
          default: break;
        }
      }

    #endif // EDITABLE_SERVO_ANGLES

    #if HAS_SCARA_OFFSET

      CONFIG_ECHO_HEADING("SCARA settings: S<seg-per-sec> P<theta-psi-offset> T<theta-offset>");
      CONFIG_ECHO_START();
      SERIAL_ECHOLNPAIR(
          "  M665 S", delta_segments_per_second
        , " P", scara_home_offset.a
        , " T", scara_home_offset.b
        , " Z", LINEAR_UNIT(scara_home_offset.z)
      );

    #elif ENABLED(DELTA)

      CONFIG_ECHO_HEADING("Endstop adjustment:");
      CONFIG_ECHO_START();
      SERIAL_ECHOLNPAIR(
          "  M666 X", LINEAR_UNIT(delta_endstop_adj.a)
        , " Y", LINEAR_UNIT(delta_endstop_adj.b)
        , " Z", LINEAR_UNIT(delta_endstop_adj.c)
      );

      CONFIG_ECHO_HEADING("Delta settings: L<diagonal_rod> R<radius> H<height> S<segments_per_s> B<calibration radius> XYZ<tower angle corrections>");
      CONFIG_ECHO_START();
      SERIAL_ECHOLNPAIR(
          "  M665 L", LINEAR_UNIT(delta_diagonal_rod)
        , " R", LINEAR_UNIT(delta_radius)
        , " H", LINEAR_UNIT(delta_height)
        , " S", delta_segments_per_second
        , " B", LINEAR_UNIT(delta_calibration_radius)
        , " X", LINEAR_UNIT(delta_tower_angle_trim.a)
        , " Y", LINEAR_UNIT(delta_tower_angle_trim.b)
        , " Z", LINEAR_UNIT(delta_tower_angle_trim.c)
      );

    #elif EITHER(X_DUAL_ENDSTOPS, Y_DUAL_ENDSTOPS) || Z_MULTI_ENDSTOPS

      CONFIG_ECHO_HEADING("Endstop adjustment:");
      CONFIG_ECHO_START();
      SERIAL_ECHOPGM("  M666");
      #if ENABLED(X_DUAL_ENDSTOPS)
        SERIAL_ECHOPAIR(" X", LINEAR_UNIT(endstops.x2_endstop_adj));
      #endif
      #if ENABLED(Y_DUAL_ENDSTOPS)
        SERIAL_ECHOPAIR(" Y", LINEAR_UNIT(endstops.y2_endstop_adj));
      #endif
      #if ENABLED(Z_TRIPLE_ENDSTOPS)
        SERIAL_ECHOLNPAIR("S1 Z", LINEAR_UNIT(endstops.z2_endstop_adj));
        CONFIG_ECHO_START();
        SERIAL_ECHOPAIR("  M666 S2 Z", LINEAR_UNIT(endstops.z3_endstop_adj));
      #elif ENABLED(Z_DUAL_ENDSTOPS)
        SERIAL_ECHOPAIR(" Z", LINEAR_UNIT(endstops.z2_endstop_adj));
      #endif
      SERIAL_EOL();

    #endif // [XYZ]_DUAL_ENDSTOPS

    #if HOTENDS && HAS_LCD_MENU

      CONFIG_ECHO_HEADING("Material heatup parameters:");
      for (uint8_t i = 0; i < COUNT(ui.preheat_hotend_temp); i++) {
        CONFIG_ECHO_START();
        SERIAL_ECHOLNPAIR(
            "  M145 S", (int)i
          , " H", TEMP_UNIT(ui.preheat_hotend_temp[i])
          , " B", TEMP_UNIT(ui.preheat_bed_temp[i])
          , " F", int(ui.preheat_fan_speed[i])
        );
      }

    #endif

    #if HAS_PID_HEATING

      CONFIG_ECHO_HEADING("PID settings:");

      #if ENABLED(PIDTEMP)
        HOTEND_LOOP() {
          CONFIG_ECHO_START();
          SERIAL_ECHOPAIR("  M301"
            #if HOTENDS > 1 && ENABLED(PID_PARAMS_PER_HOTEND)
              " E", e,
            #endif
              " P", PID_PARAM(Kp, e)
            , " I", unscalePID_i(PID_PARAM(Ki, e))
            , " D", unscalePID_d(PID_PARAM(Kd, e))
          );
          #if ENABLED(PID_EXTRUSION_SCALING)
            SERIAL_ECHOPAIR(" C", PID_PARAM(Kc, e));
            if (e == 0) SERIAL_ECHOPAIR(" L", thermalManager.lpq_len);
          #endif
          SERIAL_EOL();
        }
      #endif // PIDTEMP

      #if ENABLED(PIDTEMPBED)
        CONFIG_ECHO_START();
        SERIAL_ECHOLNPAIR(
            "  M304 P", thermalManager.temp_bed.pid.Kp
          , " I", unscalePID_i(thermalManager.temp_bed.pid.Ki)
          , " D", unscalePID_d(thermalManager.temp_bed.pid.Kd)
        );
      #endif

    #endif // PIDTEMP || PIDTEMPBED

    #if HAS_USER_THERMISTORS
      CONFIG_ECHO_HEADING("User thermistors:");
      for (uint8_t i = 0; i < USER_THERMISTORS; i++)
        thermalManager.log_user_thermistor(i, true);
    #endif

    #if HAS_LCD_CONTRAST
      CONFIG_ECHO_HEADING("LCD Contrast:");
      CONFIG_ECHO_START();
      SERIAL_ECHOLNPAIR("  M250 C", ui.contrast);
    #endif

    #if ENABLED(POWER_LOSS_RECOVERY)
      CONFIG_ECHO_HEADING("Power-Loss Recovery:");
      CONFIG_ECHO_START();
      SERIAL_ECHOLNPAIR("  M413 S", int(recovery.enabled));
    #endif

    #if ENABLED(FWRETRACT)

      CONFIG_ECHO_HEADING("Retract: S<length> F<units/m> Z<lift>");
      CONFIG_ECHO_START();
      SERIAL_ECHOLNPAIR(
          "  M207 S", LINEAR_UNIT(fwretract.settings.retract_length)
        , " W", LINEAR_UNIT(fwretract.settings.swap_retract_length)
        , " F", LINEAR_UNIT(MMS_TO_MMM(fwretract.settings.retract_feedrate_mm_s))
        , " Z", LINEAR_UNIT(fwretract.settings.retract_zraise)
      );

      CONFIG_ECHO_HEADING("Recover: S<length> F<units/m>");
      CONFIG_ECHO_START();
      SERIAL_ECHOLNPAIR(
          "  M208 S", LINEAR_UNIT(fwretract.settings.retract_recover_extra)
        , " W", LINEAR_UNIT(fwretract.settings.swap_retract_recover_extra)
        , " F", LINEAR_UNIT(MMS_TO_MMM(fwretract.settings.retract_recover_feedrate_mm_s))
      );

      #if ENABLED(FWRETRACT_AUTORETRACT)

        CONFIG_ECHO_HEADING("Auto-Retract: S=0 to disable, 1 to interpret E-only moves as retract/recover");
        CONFIG_ECHO_START();
        SERIAL_ECHOLNPAIR("  M209 S", fwretract.autoretract_enabled ? 1 : 0);

      #endif // FWRETRACT_AUTORETRACT

    #endif // FWRETRACT

    /**
     * Probe Offset
     */
    #if HAS_BED_PROBE
      if (!forReplay) {
        CONFIG_ECHO_START();
        SERIAL_ECHOPGM("Z-Probe Offset");
        say_units(true);
      }
      CONFIG_ECHO_START();
      SERIAL_ECHOLNPAIR("  M851 X", LINEAR_UNIT(probe_offset.x),
                              " Y", LINEAR_UNIT(probe_offset.y),
                              " Z", LINEAR_UNIT(probe_offset.z));
    #endif

    /**
     * Bed Skew Correction
     */
    #if ENABLED(SKEW_CORRECTION_GCODE)
      CONFIG_ECHO_HEADING("Skew Factor: ");
      CONFIG_ECHO_START();
      #if ENABLED(SKEW_CORRECTION_FOR_Z)
        SERIAL_ECHOPAIR_F("  M852 I", LINEAR_UNIT(planner.skew_factor.xy), 6);
        SERIAL_ECHOPAIR_F(" J", LINEAR_UNIT(planner.skew_factor.xz), 6);
        SERIAL_ECHOLNPAIR_F(" K", LINEAR_UNIT(planner.skew_factor.yz), 6);
      #else
        SERIAL_ECHOLNPAIR_F("  M852 S", LINEAR_UNIT(planner.skew_factor.xy), 6);
      #endif
    #endif

    #if HAS_TRINAMIC

      /**
       * TMC stepper driver current
       */
      CONFIG_ECHO_HEADING("Stepper driver current:");

      #if AXIS_IS_TMC(X) || AXIS_IS_TMC(Y) || AXIS_IS_TMC(Z)
        say_M906(forReplay);
        SERIAL_ECHOLNPAIR(
          #if AXIS_IS_TMC(X)
            " X", stepperX.getMilliamps(),
          #endif
          #if AXIS_IS_TMC(Y)
            " Y", stepperY.getMilliamps(),
          #endif
          #if AXIS_IS_TMC(Z)
            " Z", stepperZ.getMilliamps()
          #endif
        );
      #endif

      #if AXIS_IS_TMC(X2) || AXIS_IS_TMC(Y2) || AXIS_IS_TMC(Z2)
        say_M906(forReplay);
        SERIAL_ECHOPGM(" I1");
        SERIAL_ECHOLNPAIR(
          #if AXIS_IS_TMC(X2)
            " X", stepperX2.getMilliamps(),
          #endif
          #if AXIS_IS_TMC(Y2)
            " Y", stepperY2.getMilliamps(),
          #endif
          #if AXIS_IS_TMC(Z2)
            " Z", stepperZ2.getMilliamps()
          #endif
        );
      #endif

      #if AXIS_IS_TMC(Z3)
        say_M906(forReplay);
        SERIAL_ECHOLNPAIR(" I2 Z", stepperZ3.getMilliamps());
      #endif

      #if AXIS_IS_TMC(E0)
        say_M906(forReplay);
        SERIAL_ECHOLNPAIR(" T0 E", stepperE0.getMilliamps());
      #endif
      #if AXIS_IS_TMC(E1)
        say_M906(forReplay);
        SERIAL_ECHOLNPAIR(" T1 E", stepperE1.getMilliamps());
      #endif
      #if AXIS_IS_TMC(E2)
        say_M906(forReplay);
        SERIAL_ECHOLNPAIR(" T2 E", stepperE2.getMilliamps());
      #endif
      #if AXIS_IS_TMC(E3)
        say_M906(forReplay);
        SERIAL_ECHOLNPAIR(" T3 E", stepperE3.getMilliamps());
      #endif
      #if AXIS_IS_TMC(E4)
        say_M906(forReplay);
        SERIAL_ECHOLNPAIR(" T4 E", stepperE4.getMilliamps());
      #endif
      #if AXIS_IS_TMC(E5)
        say_M906(forReplay);
        SERIAL_ECHOLNPAIR(" T5 E", stepperE5.getMilliamps());
      #endif
      SERIAL_EOL();

      /**
       * TMC Hybrid Threshold
       */
      #if ENABLED(HYBRID_THRESHOLD)
        CONFIG_ECHO_HEADING("Hybrid Threshold:");
        #if AXIS_HAS_STEALTHCHOP(X) || AXIS_HAS_STEALTHCHOP(Y) || AXIS_HAS_STEALTHCHOP(Z)
          say_M913(forReplay);
        #endif
        #if AXIS_HAS_STEALTHCHOP(X)
          SERIAL_ECHOPAIR(" X", stepperX.get_pwm_thrs());
        #endif
        #if AXIS_HAS_STEALTHCHOP(Y)
          SERIAL_ECHOPAIR(" Y", stepperY.get_pwm_thrs());
        #endif
        #if AXIS_HAS_STEALTHCHOP(Z)
          SERIAL_ECHOPAIR(" Z", stepperZ.get_pwm_thrs());
        #endif
        #if AXIS_HAS_STEALTHCHOP(X) || AXIS_HAS_STEALTHCHOP(Y) || AXIS_HAS_STEALTHCHOP(Z)
          SERIAL_EOL();
        #endif

        #if AXIS_HAS_STEALTHCHOP(X2) || AXIS_HAS_STEALTHCHOP(Y2) || AXIS_HAS_STEALTHCHOP(Z2)
          say_M913(forReplay);
          SERIAL_ECHOPGM(" I1");
        #endif
        #if AXIS_HAS_STEALTHCHOP(X2)
          SERIAL_ECHOPAIR(" X", stepperX2.get_pwm_thrs());
        #endif
        #if AXIS_HAS_STEALTHCHOP(Y2)
          SERIAL_ECHOPAIR(" Y", stepperY2.get_pwm_thrs());
        #endif
        #if AXIS_HAS_STEALTHCHOP(Z2)
          SERIAL_ECHOPAIR(" Z", stepperZ2.get_pwm_thrs());
        #endif
        #if AXIS_HAS_STEALTHCHOP(X2) || AXIS_HAS_STEALTHCHOP(Y2) || AXIS_HAS_STEALTHCHOP(Z2)
          SERIAL_EOL();
        #endif

        #if AXIS_HAS_STEALTHCHOP(Z3)
          say_M913(forReplay);
          SERIAL_ECHOLNPAIR(" I2 Z", stepperZ3.get_pwm_thrs());
        #endif

        #if AXIS_HAS_STEALTHCHOP(E0)
          say_M913(forReplay);
          SERIAL_ECHOLNPAIR(" T0 E", stepperE0.get_pwm_thrs());
        #endif
        #if AXIS_HAS_STEALTHCHOP(E1)
          say_M913(forReplay);
          SERIAL_ECHOLNPAIR(" T1 E", stepperE1.get_pwm_thrs());
        #endif
        #if AXIS_HAS_STEALTHCHOP(E2)
          say_M913(forReplay);
          SERIAL_ECHOLNPAIR(" T2 E", stepperE2.get_pwm_thrs());
        #endif
        #if AXIS_HAS_STEALTHCHOP(E3)
          say_M913(forReplay);
          SERIAL_ECHOLNPAIR(" T3 E", stepperE3.get_pwm_thrs());
        #endif
        #if AXIS_HAS_STEALTHCHOP(E4)
          say_M913(forReplay);
          SERIAL_ECHOLNPAIR(" T4 E", stepperE4.get_pwm_thrs());
        #endif
        #if AXIS_HAS_STEALTHCHOP(E5)
          say_M913(forReplay);
          SERIAL_ECHOLNPAIR(" T5 E", stepperE5.get_pwm_thrs());
        #endif
        SERIAL_EOL();
      #endif // HYBRID_THRESHOLD

      /**
       * TMC Sensorless homing thresholds
       */
      #if USE_SENSORLESS
        CONFIG_ECHO_HEADING("StallGuard threshold:");
        #if X_SENSORLESS || Y_SENSORLESS || Z_SENSORLESS
          CONFIG_ECHO_START();
          say_M914();
          #if X_SENSORLESS
            SERIAL_ECHOPAIR(" X", stepperX.homing_threshold());
          #endif
          #if Y_SENSORLESS
            SERIAL_ECHOPAIR(" Y", stepperY.homing_threshold());
          #endif
          #if Z_SENSORLESS
            SERIAL_ECHOPAIR(" Z", stepperZ.homing_threshold());
          #endif
          SERIAL_EOL();
        #endif

        #if X2_SENSORLESS || Y2_SENSORLESS || Z2_SENSORLESS
          CONFIG_ECHO_START();
          say_M914();
          SERIAL_ECHOPGM(" I1");
          #if X2_SENSORLESS
            SERIAL_ECHOPAIR(" X", stepperX2.homing_threshold());
          #endif
          #if Y2_SENSORLESS
            SERIAL_ECHOPAIR(" Y", stepperY2.homing_threshold());
          #endif
          #if Z2_SENSORLESS
            SERIAL_ECHOPAIR(" Z", stepperZ2.homing_threshold());
          #endif
          SERIAL_EOL();
        #endif

        #if Z3_SENSORLESS
          CONFIG_ECHO_START();
          say_M914();
          SERIAL_ECHOLNPAIR(" I2 Z", stepperZ3.homing_threshold());
        #endif

      #endif // USE_SENSORLESS

      /**
       * TMC stepping mode
       */
      #if HAS_STEALTHCHOP
        CONFIG_ECHO_HEADING("Driver stepping mode:");
        #if AXIS_HAS_STEALTHCHOP(X)
          const bool chop_x = stepperX.get_stealthChop_status();
        #else
          constexpr bool chop_x = false;
        #endif
        #if AXIS_HAS_STEALTHCHOP(Y)
          const bool chop_y = stepperY.get_stealthChop_status();
        #else
          constexpr bool chop_y = false;
        #endif
        #if AXIS_HAS_STEALTHCHOP(Z)
          const bool chop_z = stepperZ.get_stealthChop_status();
        #else
          constexpr bool chop_z = false;
        #endif

        if (chop_x || chop_y || chop_z) {
          say_M569(forReplay);
          if (chop_x) SERIAL_ECHOPGM(" X");
          if (chop_y) SERIAL_ECHOPGM(" Y");
          if (chop_z) SERIAL_ECHOPGM(" Z");
          SERIAL_EOL();
        }

        #if AXIS_HAS_STEALTHCHOP(X2)
          const bool chop_x2 = stepperX2.get_stealthChop_status();
        #else
          constexpr bool chop_x2 = false;
        #endif
        #if AXIS_HAS_STEALTHCHOP(Y2)
          const bool chop_y2 = stepperY2.get_stealthChop_status();
        #else
          constexpr bool chop_y2 = false;
        #endif
        #if AXIS_HAS_STEALTHCHOP(Z2)
          const bool chop_z2 = stepperZ2.get_stealthChop_status();
        #else
          constexpr bool chop_z2 = false;
        #endif

        if (chop_x2 || chop_y2 || chop_z2) {
          say_M569(forReplay, PSTR("I1"));
          if (chop_x2) SERIAL_ECHOPGM(" X");
          if (chop_y2) SERIAL_ECHOPGM(" Y");
          if (chop_z2) SERIAL_ECHOPGM(" Z");
          SERIAL_EOL();
        }

        #if AXIS_HAS_STEALTHCHOP(Z3)
          if (stepperZ3.get_stealthChop_status()) { say_M569(forReplay, PSTR("I2 Z"), true); }
        #endif

        #if AXIS_HAS_STEALTHCHOP(E0)
          if (stepperE0.get_stealthChop_status()) { say_M569(forReplay, PSTR("T0 E"), true); }
        #endif
        #if AXIS_HAS_STEALTHCHOP(E1)
          if (stepperE1.get_stealthChop_status()) { say_M569(forReplay, PSTR("T1 E"), true); }
        #endif
        #if AXIS_HAS_STEALTHCHOP(E2)
          if (stepperE2.get_stealthChop_status()) { say_M569(forReplay, PSTR("T2 E"), true); }
        #endif
        #if AXIS_HAS_STEALTHCHOP(E3)
          if (stepperE3.get_stealthChop_status()) { say_M569(forReplay, PSTR("T3 E"), true); }
        #endif
        #if AXIS_HAS_STEALTHCHOP(E4)
          if (stepperE4.get_stealthChop_status()) { say_M569(forReplay, PSTR("T4 E"), true); }
        #endif
        #if AXIS_HAS_STEALTHCHOP(E5)
          if (stepperE5.get_stealthChop_status()) { say_M569(forReplay, PSTR("T5 E"), true); }
        #endif

      #endif // HAS_STEALTHCHOP

    #endif // HAS_TRINAMIC

    /**
     * Linear Advance
     */
    #if ENABLED(LIN_ADVANCE)
      CONFIG_ECHO_HEADING("Linear Advance:");
      CONFIG_ECHO_START();
      #if EXTRUDERS < 2
        SERIAL_ECHOLNPAIR("  M900 K", planner.extruder_advance_K[0]);
      #else
        LOOP_L_N(i, EXTRUDERS)
          SERIAL_ECHOLNPAIR("  M900 T", int(i), " K", planner.extruder_advance_K[i]);
      #endif
    #endif

    #if HAS_MOTOR_CURRENT_PWM
      CONFIG_ECHO_HEADING("Stepper motor currents:");
      CONFIG_ECHO_START();
      SERIAL_ECHOLNPAIR(
          "  M907 X", stepper.motor_current_setting[0]
        , " Z", stepper.motor_current_setting[1]
        , " E", stepper.motor_current_setting[2]
      );
    #endif

    /**
     * Advanced Pause filament load & unload lengths
     */
    #if ENABLED(ADVANCED_PAUSE_FEATURE)
      CONFIG_ECHO_HEADING("Filament load/unload lengths:");
      #if EXTRUDERS == 1
        say_M603(forReplay);
        SERIAL_ECHOLNPAIR("L", LINEAR_UNIT(fc_settings[0].load_length), " U", LINEAR_UNIT(fc_settings[0].unload_length));
      #else
        #define _ECHO_603(N) do{ say_M603(forReplay); SERIAL_ECHOLNPAIR("T" STRINGIFY(N) " L", LINEAR_UNIT(fc_settings[N].load_length), " U", LINEAR_UNIT(fc_settings[N].unload_length)); }while(0)
        _ECHO_603(0);
        _ECHO_603(1);
        #if EXTRUDERS > 2
          _ECHO_603(2);
          #if EXTRUDERS > 3
            _ECHO_603(3);
            #if EXTRUDERS > 4
              _ECHO_603(4);
              #if EXTRUDERS > 5
                _ECHO_603(5);
              #endif // EXTRUDERS > 5
            #endif // EXTRUDERS > 4
          #endif // EXTRUDERS > 3
        #endif // EXTRUDERS > 2
      #endif // EXTRUDERS == 1
    #endif // ADVANCED_PAUSE_FEATURE

    #if EXTRUDERS > 1
      CONFIG_ECHO_HEADING("Tool-changing:");
      CONFIG_ECHO_START();
      M217_report(true);
    #endif

    #if ENABLED(BACKLASH_GCODE)
      CONFIG_ECHO_HEADING("Backlash compensation:");
      CONFIG_ECHO_START();
      SERIAL_ECHOLNPAIR(
        "  M425 F", backlash.get_correction(),
        " X", LINEAR_UNIT(backlash.distance_mm.x),
        " Y", LINEAR_UNIT(backlash.distance_mm.y),
        " Z", LINEAR_UNIT(backlash.distance_mm.z)
        #ifdef BACKLASH_SMOOTHING_MM
          , " S", LINEAR_UNIT(backlash.smoothing_mm)
        #endif
      );
    #endif

    #if HAS_FILAMENT_SENSOR
      CONFIG_ECHO_HEADING("Filament runout sensor:");
      CONFIG_ECHO_START();
      SERIAL_ECHOLNPAIR(
        "  M412 S", int(runout.enabled)
        #ifdef FILAMENT_RUNOUT_DISTANCE_MM
          , " D", LINEAR_UNIT(runout.runout_distance())
        #endif
      );
    #endif
  }

#endif // !DISABLE_M503

#pragma pack(pop)
