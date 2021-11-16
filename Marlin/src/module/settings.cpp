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

/**
 * settings.cpp
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
 */

// Change EEPROM version if the structure changes
#define EEPROM_VERSION "V85"
#define EEPROM_OFFSET 100

// Check the integrity of data offsets.
// Can be disabled for production build.
//#define DEBUG_EEPROM_READWRITE

#include "settings.h"

#include "endstops.h"
#include "planner.h"
#include "stepper.h"
#include "temperature.h"

#include "../lcd/marlinui.h"
#include "../libs/vector_3.h"   // for matrix_3x3
#include "../gcode/gcode.h"
#include "../MarlinCore.h"

#if EITHER(EEPROM_SETTINGS, SD_FIRMWARE_UPDATE)
  #include "../HAL/shared/eeprom_api.h"
#endif

#include "probe.h"

#if HAS_LEVELING
  #include "../feature/bedlevel/bedlevel.h"
#endif

#if ENABLED(Z_STEPPER_AUTO_ALIGN)
  #include "../feature/z_stepper_align.h"
#endif

#if ENABLED(EXTENSIBLE_UI)
  #include "../lcd/extui/ui_api.h"
#elif ENABLED(DWIN_CREALITY_LCD_ENHANCED)
  #include "../lcd/e3v2/enhanced/dwin.h"
#elif ENABLED(DWIN_CREALITY_LCD_JYERSUI)
  #include "../lcd/e3v2/jyersui/dwin.h"
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
  #include "../feature/powerloss.h"
#endif

#if HAS_POWER_MONITOR
  #include "../feature/power_monitor.h"
#endif

#include "../feature/pause.h"

#if ENABLED(BACKLASH_COMPENSATION)
  #include "../feature/backlash.h"
#endif

#if HAS_FILAMENT_SENSOR
  #include "../feature/runout.h"
  #ifndef FIL_RUNOUT_ENABLED_DEFAULT
    #define FIL_RUNOUT_ENABLED_DEFAULT true
  #endif
#endif

#if ENABLED(EXTRA_LIN_ADVANCE_K)
  extern float other_extruder_advance_K[EXTRUDERS];
#endif

#if HAS_MULTI_EXTRUDER
  #include "tool_change.h"
  void M217_report(const bool eeprom);
#endif

#if ENABLED(BLTOUCH)
  #include "../feature/bltouch.h"
#endif

#if HAS_TRINAMIC_CONFIG
  #include "stepper/indirection.h"
  #include "../feature/tmc_util.h"
#endif

#if HAS_PTC
  #include "../feature/probe_temp_comp.h"
#endif

#include "../feature/controllerfan.h"

#if ENABLED(CASE_LIGHT_ENABLE)
  #include "../feature/caselight.h"
#endif

#if ENABLED(PASSWORD_FEATURE)
  #include "../feature/password/password.h"
#endif

#if ENABLED(TOUCH_SCREEN_CALIBRATION)
  #include "../lcd/tft_io/touch_calibration.h"
#endif

#if HAS_ETHERNET
  #include "../feature/ethernet.h"
#endif

#if ENABLED(SOUND_MENU_ITEM)
  #include "../libs/buzzer.h"
#endif

#if ENABLED(DGUS_LCD_UI_MKS)
  #include "../lcd/extui/dgus/DGUSScreenHandler.h"
  #include "../lcd/extui/dgus/DGUSDisplayDef.h"
#endif

#pragma pack(push, 1) // No padding between variables

#if HAS_ETHERNET
  void ETH0_report();
  void MAC_report();
#endif

#define _EN_ITEM(N) , E##N

typedef struct { uint16_t LINEAR_AXIS_LIST(X, Y, Z, I, J, K), X2, Y2, Z2, Z3, Z4 REPEAT(E_STEPPERS, _EN_ITEM); } tmc_stepper_current_t;
typedef struct { uint32_t LINEAR_AXIS_LIST(X, Y, Z, I, J, K), X2, Y2, Z2, Z3, Z4 REPEAT(E_STEPPERS, _EN_ITEM); } tmc_hybrid_threshold_t;
typedef struct {  int16_t LINEAR_AXIS_LIST(X, Y, Z, I, J, K), X2, Y2, Z2, Z3, Z4;                              } tmc_sgt_t;
typedef struct {     bool LINEAR_AXIS_LIST(X, Y, Z, I, J, K), X2, Y2, Z2, Z3, Z4 REPEAT(E_STEPPERS, _EN_ITEM); } tmc_stealth_enabled_t;

#undef _EN_ITEM

// Limit an index to an array size
#define ALIM(I,ARR) _MIN(I, (signed)COUNT(ARR) - 1)

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
  uint8_t   esteppers;                                  // DISTINCT_AXES - LINEAR_AXES

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
  float mbl_z_values[TERN(MESH_BED_LEVELING, GRID_MAX_POINTS_X, 3)]   // mbl.z_values
                    [TERN(MESH_BED_LEVELING, GRID_MAX_POINTS_Y, 3)];

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
  xy_pos_t bilinear_grid_spacing, bilinear_start;       // G29 L F
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
  // Temperature first layer compensation values
  //
  #if HAS_PTC
    #if ENABLED(PTC_PROBE)
      int16_t z_offsets_probe[COUNT(ptc.z_offsets_probe)]; // M871 P I V
    #endif
    #if ENABLED(PTC_BED)
      int16_t z_offsets_bed[COUNT(ptc.z_offsets_bed)];     // M871 B I V
    #endif
    #if ENABLED(PTC_HOTEND)
      int16_t z_offsets_hotend[COUNT(ptc.z_offsets_hotend)];     // M871 E I V
    #endif
  #endif

  //
  // BLTOUCH
  //
  bool bltouch_last_written_mode;

  //
  // Kinematic Settings
  //
  #if IS_KINEMATIC
    float segments_per_second;                          // M665 S
    #if ENABLED(DELTA)
      float delta_height;                               // M666 H
      abc_float_t delta_endstop_adj;                    // M666 X Y Z
      float delta_radius,                               // M665 R
            delta_diagonal_rod;                         // M665 L
      abc_float_t delta_tower_angle_trim,               // M665 X Y Z
                  delta_diagonal_rod_trim;              // M665 A B C
    #endif
  #endif

  //
  // Extra Endstops offsets
  //
  #if HAS_EXTRA_ENDSTOPS
    float x2_endstop_adj,                               // M666 X
          y2_endstop_adj,                               // M666 Y
          z2_endstop_adj,                               // M666 (S2) Z
          z3_endstop_adj,                               // M666 (S3) Z
          z4_endstop_adj;                               // M666 (S4) Z
  #endif

  //
  // Z_STEPPER_AUTO_ALIGN, Z_STEPPER_ALIGN_KNOWN_STEPPER_POSITIONS
  //
  #if ENABLED(Z_STEPPER_AUTO_ALIGN)
    xy_pos_t z_stepper_align_xy[NUM_Z_STEPPER_DRIVERS];             // M422 S X Y
    #if ENABLED(Z_STEPPER_ALIGN_KNOWN_STEPPER_POSITIONS)
      xy_pos_t z_stepper_align_stepper_xy[NUM_Z_STEPPER_DRIVERS];   // M422 W X Y
    #endif
  #endif

  //
  // Material Presets
  //
  #if HAS_PREHEAT
    preheat_t ui_material_preset[PREHEAT_COUNT];        // M145 S0 H B F
  #endif

  //
  // PIDTEMP
  //
  PIDCF_t hotendPID[HOTENDS];                           // M301 En PIDCF / M303 En U
  int16_t lpq_len;                                      // M301 L

  //
  // PIDTEMPBED
  //
  PID_t bedPID;                                         // M304 PID / M303 E-1 U

  //
  // PIDTEMPCHAMBER
  //
  PID_t chamberPID;                                     // M309 PID / M303 E-2 U

  //
  // User-defined Thermistors
  //
  #if HAS_USER_THERMISTORS
    user_thermistor_t user_thermistor[USER_THERMISTORS]; // M305 P0 R4700 T100000 B3950
  #endif

  //
  // Power monitor
  //
  uint8_t power_monitor_flags;                          // M430 I V W

  //
  // HAS_LCD_CONTRAST
  //
  uint8_t lcd_contrast;                                 // M250 C

  //
  // HAS_LCD_BRIGHTNESS
  //
  uint8_t lcd_brightness;                               // M256 B

  //
  // Controller fan settings
  //
  controllerFan_settings_t controllerFan_settings;      // M710

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
  bool parser_volumetric_enabled;                       // M200 S  parser.volumetric_enabled
  float planner_filament_size[EXTRUDERS];               // M200 T D  planner.filament_size[]
  float planner_volumetric_extruder_limit[EXTRUDERS];   // M200 T L  planner.volumetric_extruder_limit[]

  //
  // HAS_TRINAMIC_CONFIG
  //
  tmc_stepper_current_t tmc_stepper_current;            // M906 X Y Z X2 Y2 Z2 Z3 Z4 E0 E1 E2 E3 E4 E5
  tmc_hybrid_threshold_t tmc_hybrid_threshold;          // M913 X Y Z X2 Y2 Z2 Z3 Z4 E0 E1 E2 E3 E4 E5
  tmc_sgt_t tmc_sgt;                                    // M914 X Y Z X2 Y2 Z2 Z3 Z4
  tmc_stealth_enabled_t tmc_stealth_enabled;            // M569 X Y Z X2 Y2 Z2 Z3 Z4 E0 E1 E2 E3 E4 E5

  //
  // LIN_ADVANCE
  //
  float planner_extruder_advance_K[_MAX(EXTRUDERS, 1)]; // M900 K  planner.extruder_advance_K

  //
  // HAS_MOTOR_CURRENT_PWM
  //
  #ifndef MOTOR_CURRENT_COUNT
    #define MOTOR_CURRENT_COUNT LINEAR_AXES
  #endif
  uint32_t motor_current_setting[MOTOR_CURRENT_COUNT];  // M907 X Z E ...

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
  #if HAS_EXTRUDERS
    fil_change_settings_t fc_settings[EXTRUDERS];       // M603 T U L
  #endif

  //
  // Tool-change settings
  //
  #if HAS_MULTI_EXTRUDER
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
    uint8_t extui_data[ExtUI::eeprom_data_size];
  #endif

  //
  // Ender-3 V2 DWIN
  //
  #if ENABLED(DWIN_CREALITY_LCD_ENHANCED)
    uint8_t dwin_data[eeprom_data_size];
  #elif ENABLED(DWIN_CREALITY_LCD_JYERSUI)
    uint8_t dwin_settings[CrealityDWIN.eeprom_data_size];
  #endif

  //
  // CASELIGHT_USES_BRIGHTNESS
  //
  #if CASELIGHT_USES_BRIGHTNESS
    uint8_t caselight_brightness;                        // M355 P
  #endif

  //
  // PASSWORD_FEATURE
  //
  #if ENABLED(PASSWORD_FEATURE)
    bool password_is_set;
    uint32_t password_value;
  #endif

  //
  // TOUCH_SCREEN_CALIBRATION
  //
  #if ENABLED(TOUCH_SCREEN_CALIBRATION)
    touch_calibration_t touch_calibration_data;
  #endif

  // Ethernet settings
  #if HAS_ETHERNET
    bool ethernet_hardware_enabled;                     // M552 S
    uint32_t ethernet_ip,                               // M552 P
             ethernet_dns,
             ethernet_gateway,                          // M553 P
             ethernet_subnet;                           // M554 P
  #endif

  //
  // Buzzer enable/disable
  //
  #if ENABLED(SOUND_MENU_ITEM)
    bool buzzer_enabled;
  #endif

  //
  // MKS UI controller
  //
  #if ENABLED(DGUS_LCD_UI_MKS)
    uint8_t mks_language_index;                         // Display Language
    xy_int_t mks_corner_offsets[5];                     // Bed Tramming
    xyz_int_t mks_park_pos;                             // Custom Parking (without NOZZLE_PARK)
    celsius_t mks_min_extrusion_temp;                   // Min E Temp (shadow M302 value)
  #endif

  #if HAS_MULTI_LANGUAGE
    uint8_t ui_language;                                // M414 S
  #endif

} SettingsData;

//static_assert(sizeof(SettingsData) <= MARLIN_EEPROM_SIZE, "EEPROM too small to contain SettingsData!");

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
  TERN_(DELTA, recalc_delta_settings());

  TERN_(PIDTEMP, thermalManager.updatePID());

  #if DISABLED(NO_VOLUMETRICS)
    planner.calculate_volumetric_multipliers();
  #elif EXTRUDERS
    for (uint8_t i = COUNT(planner.e_factor); i--;)
      planner.refresh_e_factor(i);
  #endif

  // Software endstops depend on home_offset
  LOOP_LINEAR_AXES(i) {
    update_workspace_offset((AxisEnum)i);
    update_software_endstops((AxisEnum)i);
  }

  TERN_(ENABLE_LEVELING_FADE_HEIGHT, set_z_fade_height(new_z_fade_height, false)); // false = no report

  TERN_(AUTO_BED_LEVELING_BILINEAR, refresh_bed_level());

  TERN_(HAS_MOTOR_CURRENT_PWM, stepper.refresh_motor_power());

  TERN_(FWRETRACT, fwretract.refresh_autoretract());

  TERN_(HAS_LINEAR_E_JERK, planner.recalculate_max_e_jerk());

  TERN_(CASELIGHT_USES_BRIGHTNESS, caselight.update_brightness());

  TERN_(EXTENSIBLE_UI, ExtUI::onPostprocessSettings());

  // Refresh mm_per_step with the reciprocal of axis_steps_per_mm
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
  static_assert(EEPROM_OFFSET + sizeof(SettingsData) < ARCHIM2_SPI_FLASH_EEPROM_BACKUP_SIZE,
                "ARCHIM2_SPI_FLASH_EEPROM_BACKUP_SIZE is insufficient to capture all EEPROM data.");
#endif

//
// This file simply uses the DEBUG_ECHO macros to implement EEPROM_CHITCHAT.
// For deeper debugging of EEPROM issues enable DEBUG_EEPROM_READWRITE.
//
#define DEBUG_OUT EITHER(EEPROM_CHITCHAT, DEBUG_LEVELING_FEATURE)
#include "../core/debug_out.h"

#if ENABLED(EEPROM_SETTINGS)

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
  int MarlinSettings::eeprom_index;
  uint16_t MarlinSettings::working_crc;

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
    float dummyf = 0;
    char ver[4] = "ERR";

    if (!EEPROM_START(EEPROM_OFFSET)) return false;

    eeprom_error = false;

    // Write or Skip version. (Flash doesn't allow rewrite without erase.)
    TERN(FLASH_EEPROM_EMULATION, EEPROM_SKIP, EEPROM_WRITE)(ver);

    EEPROM_SKIP(working_crc); // Skip the checksum slot

    working_crc = 0; // clear before first "real data"

    const uint8_t esteppers = COUNT(planner.settings.axis_steps_per_mm) - LINEAR_AXES;
    _FIELD_TEST(esteppers);
    EEPROM_WRITE(esteppers);

    //
    // Planner Motion
    //
    {
      EEPROM_WRITE(planner.settings);

      #if HAS_CLASSIC_JERK
        EEPROM_WRITE(planner.max_jerk);
        #if HAS_LINEAR_E_JERK
          dummyf = float(DEFAULT_EJERK);
          EEPROM_WRITE(dummyf);
        #endif
      #else
        const xyze_pos_t planner_max_jerk = LOGICAL_AXIS_ARRAY(float(DEFAULT_EJERK), 10, 10, 0.4, 0.4, 0.4, 0.4);
        EEPROM_WRITE(planner_max_jerk);
      #endif

      TERN_(CLASSIC_JERK, dummyf = 0.02f);
      EEPROM_WRITE(TERN(CLASSIC_JERK, dummyf, planner.junction_deviation_mm));
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
    }

    //
    // Hotend Offsets, if any
    //
    {
      #if HAS_HOTEND_OFFSET
        // Skip hotend 0 which must be 0
        LOOP_S_L_N(e, 1, HOTENDS)
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
        constexpr int8_t runout_sensor_enabled = -1;
      #endif
      _FIELD_TEST(runout_sensor_enabled);
      EEPROM_WRITE(runout_sensor_enabled);

      #if HAS_FILAMENT_RUNOUT_DISTANCE
        const float &runout_distance_mm = runout.runout_distance();
      #else
        constexpr float runout_distance_mm = 0;
      #endif
      EEPROM_WRITE(runout_distance_mm);
    }

    //
    // Global Leveling
    //
    {
      const float zfh = TERN(ENABLE_LEVELING_FADE_HEIGHT, planner.z_fade_height, (DEFAULT_LEVELING_FADE_HEIGHT));
      EEPROM_WRITE(zfh);
    }

    //
    // Mesh Bed Leveling
    //
    {
      #if ENABLED(MESH_BED_LEVELING)
        static_assert(
          sizeof(mbl.z_values) == (GRID_MAX_POINTS) * sizeof(mbl.z_values[0][0]),
          "MBL Z array is the wrong size."
        );
      #else
        dummyf = 0;
      #endif

      const uint8_t mesh_num_x = TERN(MESH_BED_LEVELING, GRID_MAX_POINTS_X, 3),
                    mesh_num_y = TERN(MESH_BED_LEVELING, GRID_MAX_POINTS_Y, 3);

      EEPROM_WRITE(TERN(MESH_BED_LEVELING, mbl.z_offset, dummyf));
      EEPROM_WRITE(mesh_num_x);
      EEPROM_WRITE(mesh_num_y);

      #if ENABLED(MESH_BED_LEVELING)
        EEPROM_WRITE(mbl.z_values);
      #else
        for (uint8_t q = mesh_num_x * mesh_num_y; q--;) EEPROM_WRITE(dummyf);
      #endif
    }

    //
    // Probe XYZ Offsets
    //
    {
      _FIELD_TEST(probe_offset);
      #if HAS_BED_PROBE
        const xyz_pos_t &zpo = probe.offset;
      #else
        constexpr xyz_pos_t zpo{0};
      #endif
      EEPROM_WRITE(zpo);
    }

    //
    // Planar Bed Leveling matrix
    //
    {
      #if ABL_PLANAR
        EEPROM_WRITE(planner.bed_level_matrix);
      #else
        dummyf = 0;
        for (uint8_t q = 9; q--;) EEPROM_WRITE(dummyf);
      #endif
    }

    //
    // Bilinear Auto Bed Leveling
    //
    {
      #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
        static_assert(
          sizeof(z_values) == (GRID_MAX_POINTS) * sizeof(z_values[0][0]),
          "Bilinear Z array is the wrong size."
        );
      #else
        const xy_pos_t bilinear_start{0}, bilinear_grid_spacing{0};
      #endif

      const uint8_t grid_max_x = TERN(AUTO_BED_LEVELING_BILINEAR, GRID_MAX_POINTS_X, 3),
                    grid_max_y = TERN(AUTO_BED_LEVELING_BILINEAR, GRID_MAX_POINTS_Y, 3);
      EEPROM_WRITE(grid_max_x);
      EEPROM_WRITE(grid_max_y);
      EEPROM_WRITE(bilinear_grid_spacing);
      EEPROM_WRITE(bilinear_start);

      #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
        EEPROM_WRITE(z_values);              // 9-256 floats
      #else
        dummyf = 0;
        for (uint16_t q = grid_max_x * grid_max_y; q--;) EEPROM_WRITE(dummyf);
      #endif
    }

    //
    // Unified Bed Leveling
    //
    {
      _FIELD_TEST(planner_leveling_active);
      const bool ubl_active = TERN(AUTO_BED_LEVELING_UBL, planner.leveling_active, false);
      const int8_t storage_slot = TERN(AUTO_BED_LEVELING_UBL, ubl.storage_slot, -1);
      EEPROM_WRITE(ubl_active);
      EEPROM_WRITE(storage_slot);
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
    // Thermal first layer compensation values
    //
    #if HAS_PTC
      #if ENABLED(PTC_PROBE)
        EEPROM_WRITE(ptc.z_offsets_probe);
      #endif
      #if ENABLED(PTC_BED)
        EEPROM_WRITE(ptc.z_offsets_bed);
      #endif
      #if ENABLED(PTC_HOTEND)
        EEPROM_WRITE(ptc.z_offsets_hotend);
      #endif
    #else
      // No placeholder data for this feature
    #endif

    //
    // BLTOUCH
    //
    {
      _FIELD_TEST(bltouch_last_written_mode);
      const bool bltouch_last_written_mode = TERN(BLTOUCH, bltouch.last_written_mode, false);
      EEPROM_WRITE(bltouch_last_written_mode);
    }

    //
    // Kinematic Settings
    //
    #if IS_KINEMATIC
    {
      EEPROM_WRITE(segments_per_second);
      #if ENABLED(DELTA)
        _FIELD_TEST(delta_height);
        EEPROM_WRITE(delta_height);              // 1 float
        EEPROM_WRITE(delta_endstop_adj);         // 3 floats
        EEPROM_WRITE(delta_radius);              // 1 float
        EEPROM_WRITE(delta_diagonal_rod);        // 1 float
        EEPROM_WRITE(delta_tower_angle_trim);    // 3 floats
        EEPROM_WRITE(delta_diagonal_rod_trim);   // 3 floats
      #endif
    }
    #endif

    //
    // Extra Endstops offsets
    //
    #if HAS_EXTRA_ENDSTOPS
    {
      _FIELD_TEST(x2_endstop_adj);

      // Write dual endstops in X, Y, Z order. Unused = 0.0
      dummyf = 0;
      EEPROM_WRITE(TERN(X_DUAL_ENDSTOPS, endstops.x2_endstop_adj, dummyf));   // 1 float
      EEPROM_WRITE(TERN(Y_DUAL_ENDSTOPS, endstops.y2_endstop_adj, dummyf));   // 1 float
      EEPROM_WRITE(TERN(Z_MULTI_ENDSTOPS, endstops.z2_endstop_adj, dummyf));  // 1 float

      #if ENABLED(Z_MULTI_ENDSTOPS) && NUM_Z_STEPPER_DRIVERS >= 3
        EEPROM_WRITE(endstops.z3_endstop_adj);   // 1 float
      #else
        EEPROM_WRITE(dummyf);
      #endif

      #if ENABLED(Z_MULTI_ENDSTOPS) && NUM_Z_STEPPER_DRIVERS >= 4
        EEPROM_WRITE(endstops.z4_endstop_adj);   // 1 float
      #else
        EEPROM_WRITE(dummyf);
      #endif
    }
    #endif

    #if ENABLED(Z_STEPPER_AUTO_ALIGN)
      EEPROM_WRITE(z_stepper_align.xy);
      #if ENABLED(Z_STEPPER_ALIGN_KNOWN_STEPPER_POSITIONS)
        EEPROM_WRITE(z_stepper_align.stepper_xy);
      #endif
    #endif

    //
    // LCD Preheat settings
    //
    #if HAS_PREHEAT
      _FIELD_TEST(ui_material_preset);
      EEPROM_WRITE(ui.material_preset);
    #endif

    //
    // PIDTEMP
    //
    {
      _FIELD_TEST(hotendPID);
      HOTEND_LOOP() {
        PIDCF_t pidcf = {
          #if DISABLED(PIDTEMP)
            NAN, NAN, NAN,
            NAN, NAN
          #else
                         PID_PARAM(Kp, e),
            unscalePID_i(PID_PARAM(Ki, e)),
            unscalePID_d(PID_PARAM(Kd, e)),
                         PID_PARAM(Kc, e),
                         PID_PARAM(Kf, e)
          #endif
        };
        EEPROM_WRITE(pidcf);
      }

      _FIELD_TEST(lpq_len);
      #if DISABLED(PID_EXTRUSION_SCALING)
        const int16_t lpq_len = 20;
      #endif
      EEPROM_WRITE(TERN(PID_EXTRUSION_SCALING, thermalManager.lpq_len, lpq_len));
    }

    //
    // PIDTEMPBED
    //
    {
      _FIELD_TEST(bedPID);

      const PID_t bed_pid = {
        #if DISABLED(PIDTEMPBED)
          NAN, NAN, NAN
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
    // PIDTEMPCHAMBER
    //
    {
      _FIELD_TEST(chamberPID);

      const PID_t chamber_pid = {
        #if DISABLED(PIDTEMPCHAMBER)
          NAN, NAN, NAN
        #else
          // Store the unscaled PID values
          thermalManager.temp_chamber.pid.Kp,
          unscalePID_i(thermalManager.temp_chamber.pid.Ki),
          unscalePID_d(thermalManager.temp_chamber.pid.Kd)
        #endif
      };
      EEPROM_WRITE(chamber_pid);
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
    // Power monitor
    //
    {
      #if HAS_POWER_MONITOR
        const uint8_t &power_monitor_flags = power_monitor.flags;
      #else
        constexpr uint8_t power_monitor_flags = 0x00;
      #endif
      _FIELD_TEST(power_monitor_flags);
      EEPROM_WRITE(power_monitor_flags);
    }

    //
    // LCD Contrast
    //
    {
      _FIELD_TEST(lcd_contrast);
      const uint8_t lcd_contrast = TERN(HAS_LCD_CONTRAST, ui.contrast, 127);
      EEPROM_WRITE(lcd_contrast);
    }

    //
    // LCD Brightness
    //
    {
      _FIELD_TEST(lcd_brightness);
      const uint8_t lcd_brightness = TERN(HAS_LCD_BRIGHTNESS, ui.brightness, 255);
      EEPROM_WRITE(lcd_brightness);
    }

    //
    // Controller Fan
    //
    {
      _FIELD_TEST(controllerFan_settings);
      #if ENABLED(USE_CONTROLLER_FAN)
        const controllerFan_settings_t &cfs = controllerFan.settings;
      #else
        controllerFan_settings_t cfs = controllerFan_defaults;
      #endif
      EEPROM_WRITE(cfs);
    }

    //
    // Power-Loss Recovery
    //
    {
      _FIELD_TEST(recovery_enabled);
      const bool recovery_enabled = TERN(POWER_LOSS_RECOVERY, recovery.enabled, ENABLED(PLR_ENABLED_DEFAULT));
      EEPROM_WRITE(recovery_enabled);
    }

    //
    // Firmware Retraction
    //
    {
      _FIELD_TEST(fwretract_settings);
      #if DISABLED(FWRETRACT)
        const fwretract_settings_t autoretract_defaults = { 3, 45, 0, 0, 0, 13, 0, 8 };
      #endif
      EEPROM_WRITE(TERN(FWRETRACT, fwretract.settings, autoretract_defaults));

      #if DISABLED(FWRETRACT_AUTORETRACT)
        const bool autoretract_enabled = false;
      #endif
      EEPROM_WRITE(TERN(FWRETRACT_AUTORETRACT, fwretract.autoretract_enabled, autoretract_enabled));
    }

    //
    // Volumetric & Filament Size
    //
    {
      _FIELD_TEST(parser_volumetric_enabled);

      #if DISABLED(NO_VOLUMETRICS)

        EEPROM_WRITE(parser.volumetric_enabled);
        EEPROM_WRITE(planner.filament_size);
        #if ENABLED(VOLUMETRIC_EXTRUDER_LIMIT)
          EEPROM_WRITE(planner.volumetric_extruder_limit);
        #else
          dummyf = DEFAULT_VOLUMETRIC_EXTRUDER_LIMIT;
          for (uint8_t q = EXTRUDERS; q--;) EEPROM_WRITE(dummyf);
        #endif

      #else

        const bool volumetric_enabled = false;
        EEPROM_WRITE(volumetric_enabled);
        dummyf = DEFAULT_NOMINAL_FILAMENT_DIA;
        for (uint8_t q = EXTRUDERS; q--;) EEPROM_WRITE(dummyf);
        dummyf = DEFAULT_VOLUMETRIC_EXTRUDER_LIMIT;
        for (uint8_t q = EXTRUDERS; q--;) EEPROM_WRITE(dummyf);

      #endif
    }

    //
    // TMC Configuration
    //
    {
      _FIELD_TEST(tmc_stepper_current);

      tmc_stepper_current_t tmc_stepper_current{0};

      #if HAS_TRINAMIC_CONFIG
        #if AXIS_IS_TMC(X)
          tmc_stepper_current.X = stepperX.getMilliamps();
        #endif
        #if AXIS_IS_TMC(Y)
          tmc_stepper_current.Y = stepperY.getMilliamps();
        #endif
        #if AXIS_IS_TMC(Z)
          tmc_stepper_current.Z = stepperZ.getMilliamps();
        #endif
        #if AXIS_IS_TMC(I)
          tmc_stepper_current.I = stepperI.getMilliamps();
        #endif
        #if AXIS_IS_TMC(J)
          tmc_stepper_current.J = stepperJ.getMilliamps();
        #endif
        #if AXIS_IS_TMC(K)
          tmc_stepper_current.K = stepperK.getMilliamps();
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
        #if AXIS_IS_TMC(Z4)
          tmc_stepper_current.Z4 = stepperZ4.getMilliamps();
        #endif
        #if AXIS_IS_TMC(E0)
          tmc_stepper_current.E0 = stepperE0.getMilliamps();
        #endif
        #if AXIS_IS_TMC(E1)
          tmc_stepper_current.E1 = stepperE1.getMilliamps();
        #endif
        #if AXIS_IS_TMC(E2)
          tmc_stepper_current.E2 = stepperE2.getMilliamps();
        #endif
        #if AXIS_IS_TMC(E3)
          tmc_stepper_current.E3 = stepperE3.getMilliamps();
        #endif
        #if AXIS_IS_TMC(E4)
          tmc_stepper_current.E4 = stepperE4.getMilliamps();
        #endif
        #if AXIS_IS_TMC(E5)
          tmc_stepper_current.E5 = stepperE5.getMilliamps();
        #endif
        #if AXIS_IS_TMC(E6)
          tmc_stepper_current.E6 = stepperE6.getMilliamps();
        #endif
        #if AXIS_IS_TMC(E7)
          tmc_stepper_current.E7 = stepperE7.getMilliamps();
        #endif
      #endif
      EEPROM_WRITE(tmc_stepper_current);
    }

    //
    // TMC Hybrid Threshold, and placeholder values
    //
    {
      _FIELD_TEST(tmc_hybrid_threshold);

      #if ENABLED(HYBRID_THRESHOLD)
        tmc_hybrid_threshold_t tmc_hybrid_threshold{0};
        TERN_(X_HAS_STEALTHCHOP,  tmc_hybrid_threshold.X =  stepperX.get_pwm_thrs());
        TERN_(Y_HAS_STEALTHCHOP,  tmc_hybrid_threshold.Y =  stepperY.get_pwm_thrs());
        TERN_(Z_HAS_STEALTHCHOP,  tmc_hybrid_threshold.Z =  stepperZ.get_pwm_thrs());
        TERN_(I_HAS_STEALTHCHOP,  tmc_hybrid_threshold.I =  stepperI.get_pwm_thrs());
        TERN_(J_HAS_STEALTHCHOP,  tmc_hybrid_threshold.J =  stepperJ.get_pwm_thrs());
        TERN_(K_HAS_STEALTHCHOP,  tmc_hybrid_threshold.K =  stepperK.get_pwm_thrs());
        TERN_(X2_HAS_STEALTHCHOP, tmc_hybrid_threshold.X2 = stepperX2.get_pwm_thrs());
        TERN_(Y2_HAS_STEALTHCHOP, tmc_hybrid_threshold.Y2 = stepperY2.get_pwm_thrs());
        TERN_(Z2_HAS_STEALTHCHOP, tmc_hybrid_threshold.Z2 = stepperZ2.get_pwm_thrs());
        TERN_(Z3_HAS_STEALTHCHOP, tmc_hybrid_threshold.Z3 = stepperZ3.get_pwm_thrs());
        TERN_(Z4_HAS_STEALTHCHOP, tmc_hybrid_threshold.Z4 = stepperZ4.get_pwm_thrs());
        TERN_(E0_HAS_STEALTHCHOP, tmc_hybrid_threshold.E0 = stepperE0.get_pwm_thrs());
        TERN_(E1_HAS_STEALTHCHOP, tmc_hybrid_threshold.E1 = stepperE1.get_pwm_thrs());
        TERN_(E2_HAS_STEALTHCHOP, tmc_hybrid_threshold.E2 = stepperE2.get_pwm_thrs());
        TERN_(E3_HAS_STEALTHCHOP, tmc_hybrid_threshold.E3 = stepperE3.get_pwm_thrs());
        TERN_(E4_HAS_STEALTHCHOP, tmc_hybrid_threshold.E4 = stepperE4.get_pwm_thrs());
        TERN_(E5_HAS_STEALTHCHOP, tmc_hybrid_threshold.E5 = stepperE5.get_pwm_thrs());
        TERN_(E6_HAS_STEALTHCHOP, tmc_hybrid_threshold.E6 = stepperE6.get_pwm_thrs());
        TERN_(E7_HAS_STEALTHCHOP, tmc_hybrid_threshold.E7 = stepperE7.get_pwm_thrs());
      #else
        #define _EN_ITEM(N) , .E##N =  30
        const tmc_hybrid_threshold_t tmc_hybrid_threshold = {
          LINEAR_AXIS_LIST(.X = 100, .Y = 100, .Z = 3, .I = 3, .J = 3, .K = 3),
          .X2 = 100, .Y2 = 100, .Z2 = 3, .Z3 = 3, .Z4 = 3
          REPEAT(E_STEPPERS, _EN_ITEM)
        };
        #undef _EN_ITEM
      #endif
      EEPROM_WRITE(tmc_hybrid_threshold);
    }

    //
    // TMC StallGuard threshold
    //
    {
      tmc_sgt_t tmc_sgt{0};
      #if USE_SENSORLESS
        LINEAR_AXIS_CODE(
          TERN_(X_SENSORLESS, tmc_sgt.X = stepperX.homing_threshold()),
          TERN_(Y_SENSORLESS, tmc_sgt.Y = stepperY.homing_threshold()),
          TERN_(Z_SENSORLESS, tmc_sgt.Z = stepperZ.homing_threshold()),
          TERN_(I_SENSORLESS, tmc_sgt.I = stepperI.homing_threshold()),
          TERN_(J_SENSORLESS, tmc_sgt.J = stepperJ.homing_threshold()),
          TERN_(K_SENSORLESS, tmc_sgt.K = stepperK.homing_threshold())
        );
        TERN_(X2_SENSORLESS, tmc_sgt.X2 = stepperX2.homing_threshold());
        TERN_(Y2_SENSORLESS, tmc_sgt.Y2 = stepperY2.homing_threshold());
        TERN_(Z2_SENSORLESS, tmc_sgt.Z2 = stepperZ2.homing_threshold());
        TERN_(Z3_SENSORLESS, tmc_sgt.Z3 = stepperZ3.homing_threshold());
        TERN_(Z4_SENSORLESS, tmc_sgt.Z4 = stepperZ4.homing_threshold());
      #endif
      EEPROM_WRITE(tmc_sgt);
    }

    //
    // TMC stepping mode
    //
    {
      _FIELD_TEST(tmc_stealth_enabled);

      tmc_stealth_enabled_t tmc_stealth_enabled = { false };
      TERN_(X_HAS_STEALTHCHOP,  tmc_stealth_enabled.X  = stepperX.get_stored_stealthChop());
      TERN_(Y_HAS_STEALTHCHOP,  tmc_stealth_enabled.Y  = stepperY.get_stored_stealthChop());
      TERN_(Z_HAS_STEALTHCHOP,  tmc_stealth_enabled.Z  = stepperZ.get_stored_stealthChop());
      TERN_(I_HAS_STEALTHCHOP,  tmc_stealth_enabled.I  = stepperI.get_stored_stealthChop());
      TERN_(J_HAS_STEALTHCHOP,  tmc_stealth_enabled.J  = stepperJ.get_stored_stealthChop());
      TERN_(K_HAS_STEALTHCHOP,  tmc_stealth_enabled.K  = stepperK.get_stored_stealthChop());
      TERN_(X2_HAS_STEALTHCHOP, tmc_stealth_enabled.X2 = stepperX2.get_stored_stealthChop());
      TERN_(Y2_HAS_STEALTHCHOP, tmc_stealth_enabled.Y2 = stepperY2.get_stored_stealthChop());
      TERN_(Z2_HAS_STEALTHCHOP, tmc_stealth_enabled.Z2 = stepperZ2.get_stored_stealthChop());
      TERN_(Z3_HAS_STEALTHCHOP, tmc_stealth_enabled.Z3 = stepperZ3.get_stored_stealthChop());
      TERN_(Z4_HAS_STEALTHCHOP, tmc_stealth_enabled.Z4 = stepperZ4.get_stored_stealthChop());
      TERN_(E0_HAS_STEALTHCHOP, tmc_stealth_enabled.E0 = stepperE0.get_stored_stealthChop());
      TERN_(E1_HAS_STEALTHCHOP, tmc_stealth_enabled.E1 = stepperE1.get_stored_stealthChop());
      TERN_(E2_HAS_STEALTHCHOP, tmc_stealth_enabled.E2 = stepperE2.get_stored_stealthChop());
      TERN_(E3_HAS_STEALTHCHOP, tmc_stealth_enabled.E3 = stepperE3.get_stored_stealthChop());
      TERN_(E4_HAS_STEALTHCHOP, tmc_stealth_enabled.E4 = stepperE4.get_stored_stealthChop());
      TERN_(E5_HAS_STEALTHCHOP, tmc_stealth_enabled.E5 = stepperE5.get_stored_stealthChop());
      TERN_(E6_HAS_STEALTHCHOP, tmc_stealth_enabled.E6 = stepperE6.get_stored_stealthChop());
      TERN_(E7_HAS_STEALTHCHOP, tmc_stealth_enabled.E7 = stepperE7.get_stored_stealthChop());
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
        dummyf = 0;
        for (uint8_t q = _MAX(EXTRUDERS, 1); q--;) EEPROM_WRITE(dummyf);
      #endif
    }

    //
    // Motor Current PWM
    //
    {
      _FIELD_TEST(motor_current_setting);

      #if HAS_MOTOR_CURRENT_SPI || HAS_MOTOR_CURRENT_PWM
        EEPROM_WRITE(stepper.motor_current_setting);
      #else
        const uint32_t no_current[MOTOR_CURRENT_COUNT] = { 0 };
        EEPROM_WRITE(no_current);
      #endif
    }

    //
    // CNC Coordinate Systems
    //

    _FIELD_TEST(coordinate_system);

    #if DISABLED(CNC_COORDINATE_SYSTEMS)
      const xyz_pos_t coordinate_system[MAX_COORDINATE_SYSTEMS] = { { 0 } };
    #endif
    EEPROM_WRITE(TERN(CNC_COORDINATE_SYSTEMS, gcode.coordinate_system, coordinate_system));

    //
    // Skew correction factors
    //
    _FIELD_TEST(planner_skew_factor);
    EEPROM_WRITE(planner.skew_factor);

    //
    // Advanced Pause filament load & unload lengths
    //
    #if HAS_EXTRUDERS
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

    #if HAS_MULTI_EXTRUDER
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
    // Creality DWIN User Data
    //
    #if ENABLED(DWIN_CREALITY_LCD_ENHANCED)
    {
      char dwin_data[eeprom_data_size] = { 0 };
      DWIN_StoreSettings(dwin_data);
      _FIELD_TEST(dwin_data);
      EEPROM_WRITE(dwin_data);
    }
    #elif ENABLED(DWIN_CREALITY_LCD_JYERSUI)
    {
      char dwin_settings[CrealityDWIN.eeprom_data_size] = { 0 };
      CrealityDWIN.Save_Settings(dwin_settings);
      _FIELD_TEST(dwin_settings);
      EEPROM_WRITE(dwin_settings);
    }
    #endif

    //
    // Case Light Brightness
    //
    #if CASELIGHT_USES_BRIGHTNESS
      EEPROM_WRITE(caselight.brightness);
    #endif

    //
    // Password feature
    //
    #if ENABLED(PASSWORD_FEATURE)
      EEPROM_WRITE(password.is_set);
      EEPROM_WRITE(password.value);
    #endif

    //
    // TOUCH_SCREEN_CALIBRATION
    //
    #if ENABLED(TOUCH_SCREEN_CALIBRATION)
      EEPROM_WRITE(touch_calibration.calibration);
    #endif

    //
    // Ethernet network info
    //
    #if HAS_ETHERNET
    {
      _FIELD_TEST(ethernet_hardware_enabled);
      const bool ethernet_hardware_enabled = ethernet.hardware_enabled;
      const uint32_t ethernet_ip      = ethernet.ip,
                     ethernet_dns     = ethernet.myDns,
                     ethernet_gateway = ethernet.gateway,
                     ethernet_subnet  = ethernet.subnet;
      EEPROM_WRITE(ethernet_hardware_enabled);
      EEPROM_WRITE(ethernet_ip);
      EEPROM_WRITE(ethernet_dns);
      EEPROM_WRITE(ethernet_gateway);
      EEPROM_WRITE(ethernet_subnet);
    }
    #endif

    //
    // Buzzer enable/disable
    //
    #if ENABLED(SOUND_MENU_ITEM)
      EEPROM_WRITE(ui.buzzer_enabled);
    #endif

    //
    // MKS UI controller
    //
    #if ENABLED(DGUS_LCD_UI_MKS)
      EEPROM_WRITE(mks_language_index);
      EEPROM_WRITE(mks_corner_offsets);
      EEPROM_WRITE(mks_park_pos);
      EEPROM_WRITE(mks_min_extrusion_temp);
    #endif

    //
    // Selected LCD language
    //
    #if HAS_MULTI_LANGUAGE
      EEPROM_WRITE(ui.language);
    #endif

    //
    // Report final CRC and Data Size
    //
    if (!eeprom_error) {
      const uint16_t eeprom_size = eeprom_index - (EEPROM_OFFSET),
                     final_crc = working_crc;

      // Write the EEPROM header
      eeprom_index = EEPROM_OFFSET;

      EEPROM_WRITE(version);
      EEPROM_WRITE(final_crc);

      // Report storage size
      DEBUG_ECHO_MSG("Settings Stored (", eeprom_size, " bytes; crc ", (uint32_t)final_crc, ")");

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

    if (!eeprom_error) LCD_MESSAGE(MSG_SETTINGS_STORED);

    TERN_(EXTENSIBLE_UI, ExtUI::onConfigurationStoreWritten(!eeprom_error));

    return !eeprom_error;
  }

  /**
   * M501 - Retrieve Configuration
   */
  bool MarlinSettings::_load() {
    if (!EEPROM_START(EEPROM_OFFSET)) return false;

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
      DEBUG_ECHO_MSG("EEPROM version mismatch (EEPROM=", stored_ver, " Marlin=" EEPROM_VERSION ")");
      TERN_(DWIN_CREALITY_LCD_ENHANCED, LCD_MESSAGE(MSG_ERR_EEPROM_VERSION));

      IF_DISABLED(EEPROM_AUTO_INIT, ui.eeprom_alert_version());
      eeprom_error = true;
    }
    else {
      float dummyf = 0;
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
        uint32_t tmp1[LINEAR_AXES + esteppers];
        float tmp2[LINEAR_AXES + esteppers];
        feedRate_t tmp3[LINEAR_AXES + esteppers];
        EEPROM_READ((uint8_t *)tmp1, sizeof(tmp1)); // max_acceleration_mm_per_s2
        EEPROM_READ(planner.settings.min_segment_time_us);
        EEPROM_READ((uint8_t *)tmp2, sizeof(tmp2)); // axis_steps_per_mm
        EEPROM_READ((uint8_t *)tmp3, sizeof(tmp3)); // max_feedrate_mm_s

        if (!validating) LOOP_DISTINCT_AXES(i) {
          const bool in = (i < esteppers + LINEAR_AXES);
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
            EEPROM_READ(dummyf);
          #endif
        #else
          for (uint8_t q = LOGICAL_AXES; q--;) EEPROM_READ(dummyf);
        #endif

        EEPROM_READ(TERN(CLASSIC_JERK, dummyf, planner.junction_deviation_mm));
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
          LOOP_S_L_N(e, 1, HOTENDS)
            EEPROM_READ(hotend_offset[e]);
        #endif
      }

      //
      // Filament Runout Sensor
      //
      {
        int8_t runout_sensor_enabled;
        _FIELD_TEST(runout_sensor_enabled);
        EEPROM_READ(runout_sensor_enabled);
        #if HAS_FILAMENT_SENSOR
          runout.enabled = runout_sensor_enabled < 0 ? FIL_RUNOUT_ENABLED_DEFAULT : runout_sensor_enabled;
        #endif

        TERN_(HAS_FILAMENT_SENSOR, if (runout.enabled) runout.reset());

        float runout_distance_mm;
        EEPROM_READ(runout_distance_mm);
        #if HAS_FILAMENT_RUNOUT_DISTANCE
          if (!validating) runout.set_runout_distance(runout_distance_mm);
        #endif
      }

      //
      // Global Leveling
      //
      EEPROM_READ(TERN(ENABLE_LEVELING_FADE_HEIGHT, new_z_fade_height, dummyf));

      //
      // Mesh (Manual) Bed Leveling
      //
      {
        uint8_t mesh_num_x, mesh_num_y;
        EEPROM_READ(dummyf);
        EEPROM_READ_ALWAYS(mesh_num_x);
        EEPROM_READ_ALWAYS(mesh_num_y);

        #if ENABLED(MESH_BED_LEVELING)
          if (!validating) mbl.z_offset = dummyf;
          if (mesh_num_x == (GRID_MAX_POINTS_X) && mesh_num_y == (GRID_MAX_POINTS_Y)) {
            // EEPROM data fits the current mesh
            EEPROM_READ(mbl.z_values);
          }
          else {
            // EEPROM data is stale
            if (!validating) mbl.reset();
            for (uint16_t q = mesh_num_x * mesh_num_y; q--;) EEPROM_READ(dummyf);
          }
        #else
          // MBL is disabled - skip the stored data
          for (uint16_t q = mesh_num_x * mesh_num_y; q--;) EEPROM_READ(dummyf);
        #endif // MESH_BED_LEVELING
      }

      //
      // Probe Z Offset
      //
      {
        _FIELD_TEST(probe_offset);
        #if HAS_BED_PROBE
          const xyz_pos_t &zpo = probe.offset;
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
          for (uint8_t q = 9; q--;) EEPROM_READ(dummyf);
        #endif
      }

      //
      // Bilinear Auto Bed Leveling
      //
      {
        uint8_t grid_max_x, grid_max_y;
        EEPROM_READ_ALWAYS(grid_max_x);                // 1 byte
        EEPROM_READ_ALWAYS(grid_max_y);                // 1 byte
        #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
          if (grid_max_x == (GRID_MAX_POINTS_X) && grid_max_y == (GRID_MAX_POINTS_Y)) {
            if (!validating) set_bed_leveling_enabled(false);
            EEPROM_READ(bilinear_grid_spacing);        // 2 ints
            EEPROM_READ(bilinear_start);               // 2 ints
            EEPROM_READ(z_values);                     // 9 to 256 floats
          }
          else // EEPROM data is stale
        #endif // AUTO_BED_LEVELING_BILINEAR
          {
            // Skip past disabled (or stale) Bilinear Grid data
            xy_pos_t bgs, bs;
            EEPROM_READ(bgs);
            EEPROM_READ(bs);
            for (uint16_t q = grid_max_x * grid_max_y; q--;) EEPROM_READ(dummyf);
          }
      }

      //
      // Unified Bed Leveling active state
      //
      {
        _FIELD_TEST(planner_leveling_active);
        #if ENABLED(AUTO_BED_LEVELING_UBL)
          const bool &planner_leveling_active = planner.leveling_active;
          const int8_t &ubl_storage_slot = ubl.storage_slot;
        #else
          bool planner_leveling_active;
          int8_t ubl_storage_slot;
        #endif
        EEPROM_READ(planner_leveling_active);
        EEPROM_READ(ubl_storage_slot);
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
      // Thermal first layer compensation values
      //
      #if HAS_PTC
        #if ENABLED(PTC_PROBE)
          EEPROM_READ(ptc.z_offsets_probe);
        #endif
        # if ENABLED(PTC_BED)
          EEPROM_READ(ptc.z_offsets_bed);
        #endif
        #if ENABLED(PTC_HOTEND)
          EEPROM_READ(ptc.z_offsets_hotend);
        #endif
        ptc.reset_index();
      #else
        // No placeholder data for this feature
      #endif

      //
      // BLTOUCH
      //
      {
        _FIELD_TEST(bltouch_last_written_mode);
        #if ENABLED(BLTOUCH)
          const bool &bltouch_last_written_mode = bltouch.last_written_mode;
        #else
          bool bltouch_last_written_mode;
        #endif
        EEPROM_READ(bltouch_last_written_mode);
      }

      //
      // Kinematic Segments-per-second
      //
      #if IS_KINEMATIC
      {
        EEPROM_READ(segments_per_second);
        #if ENABLED(DELTA)
          _FIELD_TEST(delta_height);
          EEPROM_READ(delta_height);              // 1 float
          EEPROM_READ(delta_endstop_adj);         // 3 floats
          EEPROM_READ(delta_radius);              // 1 float
          EEPROM_READ(delta_diagonal_rod);        // 1 float
          EEPROM_READ(delta_tower_angle_trim);    // 3 floats
          EEPROM_READ(delta_diagonal_rod_trim);   // 3 floats
        #endif
      }
      #endif

      //
      // Extra Endstops offsets
      //
      #if HAS_EXTRA_ENDSTOPS
      {
        _FIELD_TEST(x2_endstop_adj);

        EEPROM_READ(TERN(X_DUAL_ENDSTOPS, endstops.x2_endstop_adj, dummyf));  // 1 float
        EEPROM_READ(TERN(Y_DUAL_ENDSTOPS, endstops.y2_endstop_adj, dummyf));  // 1 float
        EEPROM_READ(TERN(Z_MULTI_ENDSTOPS, endstops.z2_endstop_adj, dummyf)); // 1 float

        #if ENABLED(Z_MULTI_ENDSTOPS) && NUM_Z_STEPPER_DRIVERS >= 3
          EEPROM_READ(endstops.z3_endstop_adj); // 1 float
        #else
          EEPROM_READ(dummyf);
        #endif
        #if ENABLED(Z_MULTI_ENDSTOPS) && NUM_Z_STEPPER_DRIVERS >= 4
          EEPROM_READ(endstops.z4_endstop_adj); // 1 float
        #else
          EEPROM_READ(dummyf);
        #endif
      }
      #endif

      #if ENABLED(Z_STEPPER_AUTO_ALIGN)
        EEPROM_READ(z_stepper_align.xy);
        #if ENABLED(Z_STEPPER_ALIGN_KNOWN_STEPPER_POSITIONS)
          EEPROM_READ(z_stepper_align.stepper_xy);
        #endif
      #endif

      //
      // LCD Preheat settings
      //
      #if HAS_PREHEAT
        _FIELD_TEST(ui_material_preset);
        EEPROM_READ(ui.material_preset);
      #endif

      //
      // Hotend PID
      //
      {
        HOTEND_LOOP() {
          PIDCF_t pidcf;
          EEPROM_READ(pidcf);
          #if ENABLED(PIDTEMP)
            if (!validating && !isnan(pidcf.Kp)) {
              // Scale PID values since EEPROM values are unscaled
              PID_PARAM(Kp, e) = pidcf.Kp;
              PID_PARAM(Ki, e) = scalePID_i(pidcf.Ki);
              PID_PARAM(Kd, e) = scalePID_d(pidcf.Kd);
              TERN_(PID_EXTRUSION_SCALING, PID_PARAM(Kc, e) = pidcf.Kc);
              TERN_(PID_FAN_SCALING, PID_PARAM(Kf, e) = pidcf.Kf);
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
          const int16_t &lpq_len = thermalManager.lpq_len;
        #else
          int16_t lpq_len;
        #endif
        EEPROM_READ(lpq_len);
      }

      //
      // Heated Bed PID
      //
      {
        PID_t pid;
        EEPROM_READ(pid);
        #if ENABLED(PIDTEMPBED)
          if (!validating && !isnan(pid.Kp)) {
            // Scale PID values since EEPROM values are unscaled
            thermalManager.temp_bed.pid.Kp = pid.Kp;
            thermalManager.temp_bed.pid.Ki = scalePID_i(pid.Ki);
            thermalManager.temp_bed.pid.Kd = scalePID_d(pid.Kd);
          }
        #endif
      }

      //
      // Heated Chamber PID
      //
      {
        PID_t pid;
        EEPROM_READ(pid);
        #if ENABLED(PIDTEMPCHAMBER)
          if (!validating && !isnan(pid.Kp)) {
            // Scale PID values since EEPROM values are unscaled
            thermalManager.temp_chamber.pid.Kp = pid.Kp;
            thermalManager.temp_chamber.pid.Ki = scalePID_i(pid.Ki);
            thermalManager.temp_chamber.pid.Kd = scalePID_d(pid.Kd);
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
      // Power monitor
      //
      {
        #if HAS_POWER_MONITOR
          uint8_t &power_monitor_flags = power_monitor.flags;
        #else
          uint8_t power_monitor_flags;
        #endif
        _FIELD_TEST(power_monitor_flags);
        EEPROM_READ(power_monitor_flags);
      }

      //
      // LCD Contrast
      //
      {
        _FIELD_TEST(lcd_contrast);
        uint8_t lcd_contrast;
        EEPROM_READ(lcd_contrast);
        if (!validating) {
          TERN_(HAS_LCD_CONTRAST, ui.set_contrast(lcd_contrast));
        }
      }

      //
      // LCD Brightness
      //
      {
        _FIELD_TEST(lcd_brightness);
        uint8_t lcd_brightness;
        EEPROM_READ(lcd_brightness);
        TERN_(HAS_LCD_BRIGHTNESS, if (!validating) ui.set_brightness(lcd_brightness));
      }

      //
      // Controller Fan
      //
      {
        _FIELD_TEST(controllerFan_settings);
        #if ENABLED(CONTROLLER_FAN_EDITABLE)
          const controllerFan_settings_t &cfs = controllerFan.settings;
        #else
          controllerFan_settings_t cfs = { 0 };
        #endif
        EEPROM_READ(cfs);
      }

      //
      // Power-Loss Recovery
      //
      {
        _FIELD_TEST(recovery_enabled);
        #if ENABLED(POWER_LOSS_RECOVERY)
          const bool &recovery_enabled = recovery.enabled;
        #else
          bool recovery_enabled;
        #endif
        EEPROM_READ(recovery_enabled);
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
          float volumetric_extruder_limit[EXTRUDERS];
        } storage;

        _FIELD_TEST(parser_volumetric_enabled);
        EEPROM_READ(storage);

        #if DISABLED(NO_VOLUMETRICS)
          if (!validating) {
            parser.volumetric_enabled = storage.volumetric_enabled;
            COPY(planner.filament_size, storage.filament_size);
            #if ENABLED(VOLUMETRIC_EXTRUDER_LIMIT)
              COPY(planner.volumetric_extruder_limit, storage.volumetric_extruder_limit);
            #endif
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

        #if HAS_TRINAMIC_CONFIG

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
            #if AXIS_IS_TMC(Z4)
              SET_CURR(Z4);
            #endif
            #if AXIS_IS_TMC(I)
              SET_CURR(I);
            #endif
            #if AXIS_IS_TMC(J)
              SET_CURR(J);
            #endif
            #if AXIS_IS_TMC(K)
              SET_CURR(K);
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
            #if AXIS_IS_TMC(E6)
              SET_CURR(E6);
            #endif
            #if AXIS_IS_TMC(E7)
              SET_CURR(E7);
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
            TERN_(X_HAS_STEALTHCHOP,  stepperX.set_pwm_thrs(tmc_hybrid_threshold.X));
            TERN_(Y_HAS_STEALTHCHOP,  stepperY.set_pwm_thrs(tmc_hybrid_threshold.Y));
            TERN_(Z_HAS_STEALTHCHOP,  stepperZ.set_pwm_thrs(tmc_hybrid_threshold.Z));
            TERN_(X2_HAS_STEALTHCHOP, stepperX2.set_pwm_thrs(tmc_hybrid_threshold.X2));
            TERN_(Y2_HAS_STEALTHCHOP, stepperY2.set_pwm_thrs(tmc_hybrid_threshold.Y2));
            TERN_(Z2_HAS_STEALTHCHOP, stepperZ2.set_pwm_thrs(tmc_hybrid_threshold.Z2));
            TERN_(Z3_HAS_STEALTHCHOP, stepperZ3.set_pwm_thrs(tmc_hybrid_threshold.Z3));
            TERN_(Z4_HAS_STEALTHCHOP, stepperZ4.set_pwm_thrs(tmc_hybrid_threshold.Z4));
            TERN_(I_HAS_STEALTHCHOP,  stepperI.set_pwm_thrs(tmc_hybrid_threshold.I));
            TERN_(J_HAS_STEALTHCHOP,  stepperJ.set_pwm_thrs(tmc_hybrid_threshold.J));
            TERN_(K_HAS_STEALTHCHOP,  stepperK.set_pwm_thrs(tmc_hybrid_threshold.K));
            TERN_(E0_HAS_STEALTHCHOP, stepperE0.set_pwm_thrs(tmc_hybrid_threshold.E0));
            TERN_(E1_HAS_STEALTHCHOP, stepperE1.set_pwm_thrs(tmc_hybrid_threshold.E1));
            TERN_(E2_HAS_STEALTHCHOP, stepperE2.set_pwm_thrs(tmc_hybrid_threshold.E2));
            TERN_(E3_HAS_STEALTHCHOP, stepperE3.set_pwm_thrs(tmc_hybrid_threshold.E3));
            TERN_(E4_HAS_STEALTHCHOP, stepperE4.set_pwm_thrs(tmc_hybrid_threshold.E4));
            TERN_(E5_HAS_STEALTHCHOP, stepperE5.set_pwm_thrs(tmc_hybrid_threshold.E5));
            TERN_(E6_HAS_STEALTHCHOP, stepperE6.set_pwm_thrs(tmc_hybrid_threshold.E6));
            TERN_(E7_HAS_STEALTHCHOP, stepperE7.set_pwm_thrs(tmc_hybrid_threshold.E7));
          }
        #endif
      }

      //
      // TMC StallGuard threshold.
      //
      {
        tmc_sgt_t tmc_sgt;
        _FIELD_TEST(tmc_sgt);
        EEPROM_READ(tmc_sgt);
        #if USE_SENSORLESS
          if (!validating) {
            LINEAR_AXIS_CODE(
              TERN_(X_SENSORLESS, stepperX.homing_threshold(tmc_sgt.X)),
              TERN_(Y_SENSORLESS, stepperY.homing_threshold(tmc_sgt.Y)),
              TERN_(Z_SENSORLESS, stepperZ.homing_threshold(tmc_sgt.Z)),
              TERN_(I_SENSORLESS, stepperI.homing_threshold(tmc_sgt.I)),
              TERN_(J_SENSORLESS, stepperJ.homing_threshold(tmc_sgt.J)),
              TERN_(K_SENSORLESS, stepperK.homing_threshold(tmc_sgt.K))
            );
            TERN_(X2_SENSORLESS, stepperX2.homing_threshold(tmc_sgt.X2));
            TERN_(Y2_SENSORLESS, stepperY2.homing_threshold(tmc_sgt.Y2));
            TERN_(Z2_SENSORLESS, stepperZ2.homing_threshold(tmc_sgt.Z2));
            TERN_(Z3_SENSORLESS, stepperZ3.homing_threshold(tmc_sgt.Z3));
            TERN_(Z4_SENSORLESS, stepperZ4.homing_threshold(tmc_sgt.Z4));
          }
        #endif
      }

      // TMC stepping mode
      {
        _FIELD_TEST(tmc_stealth_enabled);

        tmc_stealth_enabled_t tmc_stealth_enabled;
        EEPROM_READ(tmc_stealth_enabled);

        #if HAS_TRINAMIC_CONFIG

          #define SET_STEPPING_MODE(ST) stepper##ST.stored.stealthChop_enabled = tmc_stealth_enabled.ST; stepper##ST.refresh_stepping_mode();
          if (!validating) {
            TERN_(X_HAS_STEALTHCHOP,  SET_STEPPING_MODE(X));
            TERN_(Y_HAS_STEALTHCHOP,  SET_STEPPING_MODE(Y));
            TERN_(Z_HAS_STEALTHCHOP,  SET_STEPPING_MODE(Z));
            TERN_(I_HAS_STEALTHCHOP,  SET_STEPPING_MODE(I));
            TERN_(J_HAS_STEALTHCHOP,  SET_STEPPING_MODE(J));
            TERN_(K_HAS_STEALTHCHOP,  SET_STEPPING_MODE(K));
            TERN_(X2_HAS_STEALTHCHOP, SET_STEPPING_MODE(X2));
            TERN_(Y2_HAS_STEALTHCHOP, SET_STEPPING_MODE(Y2));
            TERN_(Z2_HAS_STEALTHCHOP, SET_STEPPING_MODE(Z2));
            TERN_(Z3_HAS_STEALTHCHOP, SET_STEPPING_MODE(Z3));
            TERN_(Z4_HAS_STEALTHCHOP, SET_STEPPING_MODE(Z4));
            TERN_(E0_HAS_STEALTHCHOP, SET_STEPPING_MODE(E0));
            TERN_(E1_HAS_STEALTHCHOP, SET_STEPPING_MODE(E1));
            TERN_(E2_HAS_STEALTHCHOP, SET_STEPPING_MODE(E2));
            TERN_(E3_HAS_STEALTHCHOP, SET_STEPPING_MODE(E3));
            TERN_(E4_HAS_STEALTHCHOP, SET_STEPPING_MODE(E4));
            TERN_(E5_HAS_STEALTHCHOP, SET_STEPPING_MODE(E5));
            TERN_(E6_HAS_STEALTHCHOP, SET_STEPPING_MODE(E6));
            TERN_(E7_HAS_STEALTHCHOP, SET_STEPPING_MODE(E7));
          }
        #endif
      }

      //
      // Linear Advance
      //
      {
        float extruder_advance_K[_MAX(EXTRUDERS, 1)];
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
        _FIELD_TEST(motor_current_setting);
        uint32_t motor_current_setting[MOTOR_CURRENT_COUNT]
          #if HAS_MOTOR_CURRENT_SPI
             = DIGIPOT_MOTOR_CURRENT
          #endif
        ;
        #if HAS_MOTOR_CURRENT_SPI
          DEBUG_ECHO_MSG("DIGIPOTS Loading");
        #endif
        EEPROM_READ(motor_current_setting);
        #if HAS_MOTOR_CURRENT_SPI
          DEBUG_ECHO_MSG("DIGIPOTS Loaded");
        #endif
        #if HAS_MOTOR_CURRENT_SPI || HAS_MOTOR_CURRENT_PWM
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
      #if HAS_EXTRUDERS
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
      #if HAS_MULTI_EXTRUDER
        _FIELD_TEST(toolchange_settings);
        EEPROM_READ(toolchange_settings);
      #endif

      //
      // Backlash Compensation
      //
      {
        #if ENABLED(BACKLASH_GCODE)
          const xyz_float_t &backlash_distance_mm = backlash.distance_mm;
          const uint8_t &backlash_correction = backlash.correction;
        #else
          xyz_float_t backlash_distance_mm;
          uint8_t backlash_correction;
        #endif
        #if ENABLED(BACKLASH_GCODE) && defined(BACKLASH_SMOOTHING_MM)
          const float &backlash_smoothing_mm = backlash.smoothing_mm;
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
      { // This is a significant hardware change; don't reserve EEPROM space when not present
        const char extui_data[ExtUI::eeprom_data_size] = { 0 };
        _FIELD_TEST(extui_data);
        EEPROM_READ(extui_data);
        if (!validating) ExtUI::onLoadSettings(extui_data);
      }
      #endif

      //
      // Creality DWIN User Data
      //
      #if ENABLED(DWIN_CREALITY_LCD_ENHANCED)
      {
        const char dwin_data[eeprom_data_size] = { 0 };
        _FIELD_TEST(dwin_data);
        EEPROM_READ(dwin_data);
        if (!validating) DWIN_LoadSettings(dwin_data);
      }
      #elif ENABLED(DWIN_CREALITY_LCD_JYERSUI)
      {
        const char dwin_settings[CrealityDWIN.eeprom_data_size] = { 0 };
        _FIELD_TEST(dwin_settings);
        EEPROM_READ(dwin_settings);
        if (!validating) CrealityDWIN.Load_Settings(dwin_settings);
      }
      #endif

      //
      // Case Light Brightness
      //
      #if CASELIGHT_USES_BRIGHTNESS
        _FIELD_TEST(caselight_brightness);
        EEPROM_READ(caselight.brightness);
      #endif

      //
      // Password feature
      //
      #if ENABLED(PASSWORD_FEATURE)
        _FIELD_TEST(password_is_set);
        EEPROM_READ(password.is_set);
        EEPROM_READ(password.value);
      #endif

      //
      // TOUCH_SCREEN_CALIBRATION
      //
      #if ENABLED(TOUCH_SCREEN_CALIBRATION)
        _FIELD_TEST(touch_calibration_data);
        EEPROM_READ(touch_calibration.calibration);
      #endif

      //
      // Ethernet network info
      //
      #if HAS_ETHERNET
        _FIELD_TEST(ethernet_hardware_enabled);
        uint32_t ethernet_ip, ethernet_dns, ethernet_gateway, ethernet_subnet;
        EEPROM_READ(ethernet.hardware_enabled);
        EEPROM_READ(ethernet_ip);      ethernet.ip      = ethernet_ip;
        EEPROM_READ(ethernet_dns);     ethernet.myDns   = ethernet_dns;
        EEPROM_READ(ethernet_gateway); ethernet.gateway = ethernet_gateway;
        EEPROM_READ(ethernet_subnet);  ethernet.subnet  = ethernet_subnet;
      #endif

      //
      // Buzzer enable/disable
      //
      #if ENABLED(SOUND_MENU_ITEM)
        _FIELD_TEST(buzzer_enabled);
        EEPROM_READ(ui.buzzer_enabled);
      #endif

      //
      // MKS UI controller
      //
      #if ENABLED(DGUS_LCD_UI_MKS)
        _FIELD_TEST(mks_language_index);
        EEPROM_READ(mks_language_index);
        EEPROM_READ(mks_corner_offsets);
        EEPROM_READ(mks_park_pos);
        EEPROM_READ(mks_min_extrusion_temp);
      #endif

      //
      // Selected LCD language
      //
      #if HAS_MULTI_LANGUAGE
      {
        uint8_t ui_language;
        EEPROM_READ(ui_language);
        if (ui_language >= NUM_LANGUAGES) ui_language = 0;
        ui.set_language(ui_language);
      }
      #endif

      //
      // Validate Final Size and CRC
      //
      eeprom_error = size_error(eeprom_index - (EEPROM_OFFSET));
      if (eeprom_error) {
        DEBUG_ECHO_MSG("Index: ", eeprom_index - (EEPROM_OFFSET), " Size: ", datasize());
        IF_DISABLED(EEPROM_AUTO_INIT, ui.eeprom_alert_index());
      }
      else if (working_crc != stored_crc) {
        eeprom_error = true;
        DEBUG_ERROR_MSG("EEPROM CRC mismatch - (stored) ", stored_crc, " != ", working_crc, " (calculated)!");
        TERN_(DWIN_CREALITY_LCD_ENHANCED, LCD_MESSAGE(MSG_ERR_EEPROM_CRC));
        IF_DISABLED(EEPROM_AUTO_INIT, ui.eeprom_alert_crc());
      }
      else if (!validating) {
        DEBUG_ECHO_START();
        DEBUG_ECHO(version);
        DEBUG_ECHOLNPGM(" stored settings retrieved (", eeprom_index - (EEPROM_OFFSET), " bytes; crc ", (uint32_t)working_crc, ")");
      }

      if (!validating && !eeprom_error) postprocess();

      #if ENABLED(AUTO_BED_LEVELING_UBL)
        if (!validating) {
          ubl.report_state();

          if (!ubl.sanity_check()) {
            #if BOTH(EEPROM_CHITCHAT, DEBUG_LEVELING_FEATURE)
              ubl.echo_name();
              DEBUG_ECHOLNPGM(" initialized.\n");
            #endif
          }
          else {
            eeprom_error = true;
            #if BOTH(EEPROM_CHITCHAT, DEBUG_LEVELING_FEATURE)
              DEBUG_ECHOPGM("?Can't enable ");
              ubl.echo_name();
              DEBUG_ECHOLNPGM(".");
            #endif
            ubl.reset();
          }

          if (ubl.storage_slot >= 0) {
            load_mesh(ubl.storage_slot);
            DEBUG_ECHOLNPGM("Mesh ", ubl.storage_slot, " loaded from storage.");
          }
          else {
            ubl.reset();
            DEBUG_ECHOLNPGM("UBL reset");
          }
        }
      #endif
    }

    #if ENABLED(EEPROM_CHITCHAT) && DISABLED(DISABLE_M503)
      // Report the EEPROM settings
      if (!validating && TERN1(EEPROM_BOOT_SILENT, IsRunning())) report();
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
      TERN_(EXTENSIBLE_UI, ExtUI::onConfigurationStoreRead(success));
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
      DEBUG_ECHOLNPGM("?Invalid slot.\n", s, " mesh slots available.");
      UNUSED(s);
    }

    // 128 (+1 because of the change to capacity rather than last valid address)
    // is a placeholder for the size of the MAT; the MAT will always
    // live at the very end of the eeprom
    const uint16_t MarlinSettings::meshes_end = persistentStore.capacity() - 129;

    uint16_t MarlinSettings::meshes_start_index() {
      // Pad the end of configuration data so it can float up
      // or down a little bit without disrupting the mesh data
      return (datasize() + EEPROM_OFFSET + 32) & 0xFFF8;
    }

    #define MESH_STORE_SIZE sizeof(TERN(OPTIMIZED_MESH_STORAGE, mesh_store_t, ubl.z_values))

    uint16_t MarlinSettings::calc_num_meshes() {
      return (meshes_end - meshes_start_index()) / MESH_STORE_SIZE;
    }

    int MarlinSettings::mesh_slot_offset(const int8_t slot) {
      return meshes_end - (slot + 1) * MESH_STORE_SIZE;
    }

    void MarlinSettings::store_mesh(const int8_t slot) {

      #if ENABLED(AUTO_BED_LEVELING_UBL)
        const int16_t a = calc_num_meshes();
        if (!WITHIN(slot, 0, a - 1)) {
          ubl_invalid_slot(a);
          DEBUG_ECHOLNPGM("E2END=", persistentStore.capacity() - 1, " meshes_end=", meshes_end, " slot=", slot);
          DEBUG_EOL();
          return;
        }

        int pos = mesh_slot_offset(slot);
        uint16_t crc = 0;

        #if ENABLED(OPTIMIZED_MESH_STORAGE)
          int16_t z_mesh_store[GRID_MAX_POINTS_X][GRID_MAX_POINTS_Y];
          ubl.set_store_from_mesh(ubl.z_values, z_mesh_store);
          uint8_t * const src = (uint8_t*)&z_mesh_store;
        #else
          uint8_t * const src = (uint8_t*)&ubl.z_values;
        #endif

        // Write crc to MAT along with other data, or just tack on to the beginning or end
        persistentStore.access_start();
        const bool status = persistentStore.write_data(pos, src, MESH_STORE_SIZE, &crc);
        persistentStore.access_finish();

        if (status) SERIAL_ECHOLNPGM("?Unable to save mesh data.");
        else        DEBUG_ECHOLNPGM("Mesh saved in slot ", slot);

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
        #if ENABLED(OPTIMIZED_MESH_STORAGE)
          int16_t z_mesh_store[GRID_MAX_POINTS_X][GRID_MAX_POINTS_Y];
          uint8_t * const dest = (uint8_t*)&z_mesh_store;
        #else
          uint8_t * const dest = into ? (uint8_t*)into : (uint8_t*)&ubl.z_values;
        #endif

        persistentStore.access_start();
        const uint16_t status = persistentStore.read_data(pos, dest, MESH_STORE_SIZE, &crc);
        persistentStore.access_finish();

        #if ENABLED(OPTIMIZED_MESH_STORAGE)
          if (into) {
            float z_values[GRID_MAX_POINTS_X][GRID_MAX_POINTS_Y];
            ubl.set_mesh_from_store(z_mesh_store, z_values);
            memcpy(into, z_values, sizeof(z_values));
          }
          else
            ubl.set_mesh_from_store(z_mesh_store, ubl.z_values);
        #endif

        if (status) SERIAL_ECHOLNPGM("?Unable to load mesh data.");
        else        DEBUG_ECHOLNPGM("Mesh loaded from slot ", slot);

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
  LOOP_DISTINCT_AXES(i) {
    planner.settings.max_acceleration_mm_per_s2[i] = pgm_read_dword(&_DMA[ALIM(i, _DMA)]);
    planner.settings.axis_steps_per_mm[i] = pgm_read_float(&_DASU[ALIM(i, _DASU)]);
    planner.settings.max_feedrate_mm_s[i] = pgm_read_float(&_DMF[ALIM(i, _DMF)]);
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
    #if HAS_Y_AXIS && !defined(DEFAULT_YJERK)
      #define DEFAULT_YJERK 0
    #endif
    #if HAS_Z_AXIS && !defined(DEFAULT_ZJERK)
      #define DEFAULT_ZJERK 0
    #endif
    #if LINEAR_AXES >= 4 && !defined(DEFAULT_IJERK)
      #define DEFAULT_IJERK 0
    #endif
    #if LINEAR_AXES >= 5 && !defined(DEFAULT_JJERK)
      #define DEFAULT_JJERK 0
    #endif
    #if LINEAR_AXES >= 6 && !defined(DEFAULT_KJERK)
      #define DEFAULT_KJERK 0
    #endif
    planner.max_jerk.set(
      LINEAR_AXIS_LIST(DEFAULT_XJERK, DEFAULT_YJERK, DEFAULT_ZJERK, DEFAULT_IJERK, DEFAULT_JJERK, DEFAULT_KJERK)
    );
    TERN_(HAS_CLASSIC_E_JERK, planner.max_jerk.e = DEFAULT_EJERK);
  #endif

  TERN_(HAS_JUNCTION_DEVIATION, planner.junction_deviation_mm = float(JUNCTION_DEVIATION_MM));

  #if HAS_SCARA_OFFSET
    scara_home_offset.reset();
  #elif HAS_HOME_OFFSET
    home_offset.reset();
  #endif

  TERN_(HAS_HOTEND_OFFSET, reset_hotend_offsets());

  //
  // Filament Runout Sensor
  //

  #if HAS_FILAMENT_SENSOR
    runout.enabled = FIL_RUNOUT_ENABLED_DEFAULT;
    runout.reset();
    TERN_(HAS_FILAMENT_RUNOUT_DISTANCE, runout.set_runout_distance(FILAMENT_RUNOUT_DISTANCE_MM));
  #endif

  //
  // Tool-change Settings
  //

  #if HAS_MULTI_EXTRUDER
    #if ENABLED(TOOLCHANGE_FILAMENT_SWAP)
      toolchange_settings.swap_length     = TOOLCHANGE_FS_LENGTH;
      toolchange_settings.extra_resume    = TOOLCHANGE_FS_EXTRA_RESUME_LENGTH;
      toolchange_settings.retract_speed   = TOOLCHANGE_FS_RETRACT_SPEED;
      toolchange_settings.unretract_speed = TOOLCHANGE_FS_UNRETRACT_SPEED;
      toolchange_settings.extra_prime     = TOOLCHANGE_FS_EXTRA_PRIME;
      toolchange_settings.prime_speed     = TOOLCHANGE_FS_PRIME_SPEED;
      toolchange_settings.fan_speed       = TOOLCHANGE_FS_FAN_SPEED;
      toolchange_settings.fan_time        = TOOLCHANGE_FS_FAN_TIME;
    #endif

    #if ENABLED(TOOLCHANGE_FS_PRIME_FIRST_USED)
      enable_first_prime = false;
    #endif

    #if ENABLED(TOOLCHANGE_PARK)
      constexpr xyz_pos_t tpxy = TOOLCHANGE_PARK_XY;
      toolchange_settings.enable_park = true;
      toolchange_settings.change_point = tpxy;
    #endif

    toolchange_settings.z_raise = TOOLCHANGE_ZRAISE;

    #if ENABLED(TOOLCHANGE_MIGRATION_FEATURE)
      migration = migration_defaults;
    #endif

  #endif

  #if ENABLED(BACKLASH_GCODE)
    backlash.correction = (BACKLASH_CORRECTION) * 255;
    constexpr xyz_float_t tmp = BACKLASH_DISTANCE_MM;
    backlash.distance_mm = tmp;
    #ifdef BACKLASH_SMOOTHING_MM
      backlash.smoothing_mm = BACKLASH_SMOOTHING_MM;
    #endif
  #endif

  TERN_(EXTENSIBLE_UI, ExtUI::onFactoryReset());
  TERN_(DWIN_CREALITY_LCD_ENHANCED, DWIN_SetDataDefaults());
  TERN_(DWIN_CREALITY_LCD_JYERSUI, CrealityDWIN.Reset_Settings());

  //
  // Case Light Brightness
  //
  TERN_(CASELIGHT_USES_BRIGHTNESS, caselight.brightness = CASE_LIGHT_DEFAULT_BRIGHTNESS);

  //
  // TOUCH_SCREEN_CALIBRATION
  //
  TERN_(TOUCH_SCREEN_CALIBRATION, touch_calibration.calibration_reset());

  //
  // Buzzer enable/disable
  //
  TERN_(SOUND_MENU_ITEM, ui.buzzer_enabled = true);

  //
  // Magnetic Parking Extruder
  //
  TERN_(MAGNETIC_PARKING_EXTRUDER, mpe_settings_init());

  //
  // Global Leveling
  //
  TERN_(ENABLE_LEVELING_FADE_HEIGHT, new_z_fade_height = (DEFAULT_LEVELING_FADE_HEIGHT));
  TERN_(HAS_LEVELING, reset_bed_level());

  #if HAS_BED_PROBE
    constexpr float dpo[] = NOZZLE_TO_PROBE_OFFSET;
    static_assert(COUNT(dpo) == LINEAR_AXES, "NOZZLE_TO_PROBE_OFFSET must contain offsets for each linear axis X, Y, Z....");
    #if HAS_PROBE_XY_OFFSET
      LOOP_LINEAR_AXES(a) probe.offset[a] = dpo[a];
    #else
      probe.offset.set(LINEAR_AXIS_LIST(0, 0, dpo[Z_AXIS], 0, 0, 0));
    #endif
  #endif

  //
  // Z Stepper Auto-alignment points
  //
  TERN_(Z_STEPPER_AUTO_ALIGN, z_stepper_align.reset_to_default());

  //
  // Servo Angles
  //
  TERN_(EDITABLE_SERVO_ANGLES, COPY(servo_angles, base_servo_angles)); // When not editable only one copy of servo angles exists

  //
  // Probe Temperature Compensation
  //
  TERN_(HAS_PTC, ptc.reset());

  //
  // BLTOUCH
  //
  //#if ENABLED(BLTOUCH)
  //  bltouch.last_written_mode;
  //#endif

  //
  // Kinematic settings
  //

  #if IS_KINEMATIC
    segments_per_second = (
      TERN_(DELTA, DELTA_SEGMENTS_PER_SECOND)
      TERN_(IS_SCARA, SCARA_SEGMENTS_PER_SECOND)
      TERN_(POLARGRAPH, POLAR_SEGMENTS_PER_SECOND)
    );
    #if ENABLED(DELTA)
      const abc_float_t adj = DELTA_ENDSTOP_ADJ, dta = DELTA_TOWER_ANGLE_TRIM, ddr = DELTA_DIAGONAL_ROD_TRIM_TOWER;
      delta_height = DELTA_HEIGHT;
      delta_endstop_adj = adj;
      delta_radius = DELTA_RADIUS;
      delta_diagonal_rod = DELTA_DIAGONAL_ROD;
      delta_tower_angle_trim = dta;
      delta_diagonal_rod_trim = ddr;
    #endif
  #endif

  //
  // Endstop Adjustments
  //

  #if ENABLED(X_DUAL_ENDSTOPS)
    #ifndef X2_ENDSTOP_ADJUSTMENT
      #define X2_ENDSTOP_ADJUSTMENT 0
    #endif
    endstops.x2_endstop_adj = X2_ENDSTOP_ADJUSTMENT;
  #endif

  #if ENABLED(Y_DUAL_ENDSTOPS)
    #ifndef Y2_ENDSTOP_ADJUSTMENT
      #define Y2_ENDSTOP_ADJUSTMENT 0
    #endif
    endstops.y2_endstop_adj = Y2_ENDSTOP_ADJUSTMENT;
  #endif

  #if ENABLED(Z_MULTI_ENDSTOPS)
    #ifndef Z2_ENDSTOP_ADJUSTMENT
      #define Z2_ENDSTOP_ADJUSTMENT 0
    #endif
    endstops.z2_endstop_adj = Z2_ENDSTOP_ADJUSTMENT;
    #if NUM_Z_STEPPER_DRIVERS >= 3
      #ifndef Z3_ENDSTOP_ADJUSTMENT
        #define Z3_ENDSTOP_ADJUSTMENT 0
      #endif
      endstops.z3_endstop_adj = Z3_ENDSTOP_ADJUSTMENT;
    #endif
    #if NUM_Z_STEPPER_DRIVERS >= 4
      #ifndef Z4_ENDSTOP_ADJUSTMENT
        #define Z4_ENDSTOP_ADJUSTMENT 0
      #endif
      endstops.z4_endstop_adj = Z4_ENDSTOP_ADJUSTMENT;
    #endif
  #endif

  //
  // Preheat parameters
  //
  #if HAS_PREHEAT
    #define _PITEM(N,T) PREHEAT_##N##_##T,
    #if HAS_HOTEND
      constexpr uint16_t hpre[] = { REPEAT2_S(1, INCREMENT(PREHEAT_COUNT), _PITEM, TEMP_HOTEND) };
    #endif
    #if HAS_HEATED_BED
      constexpr uint16_t bpre[] = { REPEAT2_S(1, INCREMENT(PREHEAT_COUNT), _PITEM, TEMP_BED) };
    #endif
    #if HAS_FAN
      constexpr uint8_t fpre[] = { REPEAT2_S(1, INCREMENT(PREHEAT_COUNT), _PITEM, FAN_SPEED) };
    #endif
    LOOP_L_N(i, PREHEAT_COUNT) {
      TERN_(HAS_HOTEND,     ui.material_preset[i].hotend_temp = hpre[i]);
      TERN_(HAS_HEATED_BED, ui.material_preset[i].bed_temp = bpre[i]);
      TERN_(HAS_FAN,        ui.material_preset[i].fan_speed = fpre[i]);
    }
  #endif

  //
  // Hotend PID
  //

  #if ENABLED(PIDTEMP)
    #if ENABLED(PID_PARAMS_PER_HOTEND)
      constexpr float defKp[] =
        #ifdef DEFAULT_Kp_LIST
          DEFAULT_Kp_LIST
        #else
          ARRAY_BY_HOTENDS1(DEFAULT_Kp)
        #endif
      , defKi[] =
        #ifdef DEFAULT_Ki_LIST
          DEFAULT_Ki_LIST
        #else
          ARRAY_BY_HOTENDS1(DEFAULT_Ki)
        #endif
      , defKd[] =
        #ifdef DEFAULT_Kd_LIST
          DEFAULT_Kd_LIST
        #else
          ARRAY_BY_HOTENDS1(DEFAULT_Kd)
        #endif
      ;
      static_assert(WITHIN(COUNT(defKp), 1, HOTENDS), "DEFAULT_Kp_LIST must have between 1 and HOTENDS items.");
      static_assert(WITHIN(COUNT(defKi), 1, HOTENDS), "DEFAULT_Ki_LIST must have between 1 and HOTENDS items.");
      static_assert(WITHIN(COUNT(defKd), 1, HOTENDS), "DEFAULT_Kd_LIST must have between 1 and HOTENDS items.");
      #if ENABLED(PID_EXTRUSION_SCALING)
        constexpr float defKc[] =
          #ifdef DEFAULT_Kc_LIST
            DEFAULT_Kc_LIST
          #else
            ARRAY_BY_HOTENDS1(DEFAULT_Kc)
          #endif
        ;
        static_assert(WITHIN(COUNT(defKc), 1, HOTENDS), "DEFAULT_Kc_LIST must have between 1 and HOTENDS items.");
      #endif
      #if ENABLED(PID_FAN_SCALING)
        constexpr float defKf[] =
          #ifdef DEFAULT_Kf_LIST
            DEFAULT_Kf_LIST
          #else
            ARRAY_BY_HOTENDS1(DEFAULT_Kf)
          #endif
        ;
        static_assert(WITHIN(COUNT(defKf), 1, HOTENDS), "DEFAULT_Kf_LIST must have between 1 and HOTENDS items.");
      #endif
      #define PID_DEFAULT(N,E) def##N[E]
    #else
      #define PID_DEFAULT(N,E) DEFAULT_##N
    #endif
    HOTEND_LOOP() {
      PID_PARAM(Kp, e) =      float(PID_DEFAULT(Kp, ALIM(e, defKp)));
      PID_PARAM(Ki, e) = scalePID_i(PID_DEFAULT(Ki, ALIM(e, defKi)));
      PID_PARAM(Kd, e) = scalePID_d(PID_DEFAULT(Kd, ALIM(e, defKd)));
      TERN_(PID_EXTRUSION_SCALING, PID_PARAM(Kc, e) = float(PID_DEFAULT(Kc, ALIM(e, defKc))));
      TERN_(PID_FAN_SCALING, PID_PARAM(Kf, e) = float(PID_DEFAULT(Kf, ALIM(e, defKf))));
    }
  #endif

  //
  // PID Extrusion Scaling
  //
  TERN_(PID_EXTRUSION_SCALING, thermalManager.lpq_len = 20); // Default last-position-queue size

  //
  // Heated Bed PID
  //

  #if ENABLED(PIDTEMPBED)
    thermalManager.temp_bed.pid.Kp = DEFAULT_bedKp;
    thermalManager.temp_bed.pid.Ki = scalePID_i(DEFAULT_bedKi);
    thermalManager.temp_bed.pid.Kd = scalePID_d(DEFAULT_bedKd);
  #endif

  //
  // Heated Chamber PID
  //

  #if ENABLED(PIDTEMPCHAMBER)
    thermalManager.temp_chamber.pid.Kp = DEFAULT_chamberKp;
    thermalManager.temp_chamber.pid.Ki = scalePID_i(DEFAULT_chamberKi);
    thermalManager.temp_chamber.pid.Kd = scalePID_d(DEFAULT_chamberKd);
  #endif

  //
  // User-Defined Thermistors
  //
  TERN_(HAS_USER_THERMISTORS, thermalManager.reset_user_thermistors());

  //
  // Power Monitor
  //
  TERN_(POWER_MONITOR, power_monitor.reset());

  //
  // LCD Contrast
  //
  TERN_(HAS_LCD_CONTRAST, ui.set_contrast(DEFAULT_LCD_CONTRAST));

  //
  // LCD Brightness
  //
  TERN_(HAS_LCD_BRIGHTNESS, ui.set_brightness(DEFAULT_LCD_BRIGHTNESS));

  //
  // Controller Fan
  //
  TERN_(USE_CONTROLLER_FAN, controllerFan.reset());

  //
  // Power-Loss Recovery
  //
  TERN_(POWER_LOSS_RECOVERY, recovery.enable(ENABLED(PLR_ENABLED_DEFAULT)));

  //
  // Firmware Retraction
  //
  TERN_(FWRETRACT, fwretract.reset());

  //
  // Volumetric & Filament Size
  //

  #if DISABLED(NO_VOLUMETRICS)
    parser.volumetric_enabled = ENABLED(VOLUMETRIC_DEFAULT_ON);
    LOOP_L_N(q, COUNT(planner.filament_size))
      planner.filament_size[q] = DEFAULT_NOMINAL_FILAMENT_DIA;
    #if ENABLED(VOLUMETRIC_EXTRUDER_LIMIT)
      LOOP_L_N(q, COUNT(planner.volumetric_extruder_limit))
        planner.volumetric_extruder_limit[q] = DEFAULT_VOLUMETRIC_EXTRUDER_LIMIT;
    #endif
  #endif

  endstops.enable_globally(ENABLED(ENDSTOPS_ALWAYS_ON_DEFAULT));

  reset_stepper_drivers();

  //
  // Linear Advance
  //

  #if ENABLED(LIN_ADVANCE)
    LOOP_L_N(i, EXTRUDERS) {
      planner.extruder_advance_K[i] = LIN_ADVANCE_K;
      TERN_(EXTRA_LIN_ADVANCE_K, other_extruder_advance_K[i] = LIN_ADVANCE_K);
    }
  #endif

  //
  // Motor Current PWM
  //

  #if HAS_MOTOR_CURRENT_PWM
    constexpr uint32_t tmp_motor_current_setting[MOTOR_CURRENT_COUNT] = PWM_MOTOR_CURRENT;
    LOOP_L_N(q, MOTOR_CURRENT_COUNT)
      stepper.set_digipot_current(q, (stepper.motor_current_setting[q] = tmp_motor_current_setting[q]));
  #endif

  //
  // DIGIPOTS
  //
  #if HAS_MOTOR_CURRENT_SPI
    static constexpr uint32_t tmp_motor_current_setting[] = DIGIPOT_MOTOR_CURRENT;
    DEBUG_ECHOLNPGM("Writing Digipot");
    LOOP_L_N(q, COUNT(tmp_motor_current_setting))
      stepper.set_digipot_current(q, tmp_motor_current_setting[q]);
    DEBUG_ECHOLNPGM("Digipot Written");
  #endif

  //
  // CNC Coordinate System
  //
  TERN_(CNC_COORDINATE_SYSTEMS, (void)gcode.select_coordinate_system(-1)); // Go back to machine space

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
    LOOP_L_N(e, EXTRUDERS) {
      fc_settings[e].unload_length = FILAMENT_CHANGE_UNLOAD_LENGTH;
      fc_settings[e].load_length = FILAMENT_CHANGE_FAST_LOAD_LENGTH;
    }
  #endif

  #if ENABLED(PASSWORD_FEATURE)
    #ifdef PASSWORD_DEFAULT_VALUE
      password.is_set = true;
      password.value = PASSWORD_DEFAULT_VALUE;
    #else
      password.is_set = false;
    #endif
  #endif

  //
  // MKS UI controller
  //
  TERN_(DGUS_LCD_UI_MKS, MKS_reset_settings());

  postprocess();

  DEBUG_ECHO_MSG("Hardcoded Default Settings Loaded");
}

#if DISABLED(DISABLE_M503)

  #define CONFIG_ECHO_START()       gcode.report_echo_start(forReplay)
  #define CONFIG_ECHO_MSG(V...)     do{ CONFIG_ECHO_START(); SERIAL_ECHOLNPGM(V); }while(0)
  #define CONFIG_ECHO_MSG_P(V...)   do{ CONFIG_ECHO_START(); SERIAL_ECHOLNPGM_P(V); }while(0)
  #define CONFIG_ECHO_HEADING(STR)  gcode.report_heading(forReplay, F(STR))

  void M92_report(const bool echo=true, const int8_t e=-1);

  /**
   * M503 - Report current settings in RAM
   *
   * Unless specifically disabled, M503 is available even without EEPROM
   */
  void MarlinSettings::report(const bool forReplay) {
    //
    // Announce current units, in case inches are being displayed
    //
    CONFIG_ECHO_HEADING("Linear Units");
    #if ENABLED(INCH_MODE_SUPPORT)
      SERIAL_ECHO_MSG("  G2", AS_DIGIT(parser.linear_unit_factor == 1.0), " ;");
    #else
      SERIAL_ECHO_MSG("  G21 ;");
    #endif
    gcode.say_units();

    //
    // M149 Temperature units
    //
    #if ENABLED(TEMPERATURE_UNITS_SUPPORT)
      gcode.M149_report(forReplay);
    #else
      CONFIG_ECHO_HEADING(STR_TEMPERATURE_UNITS);
      CONFIG_ECHO_MSG("  M149 C ; Units in Celsius");
    #endif

    //
    // M200 Volumetric Extrusion
    //
    IF_DISABLED(NO_VOLUMETRICS, gcode.M200_report(forReplay));

    //
    // M92 Steps per Unit
    //
    gcode.M92_report(forReplay);

    //
    // M203 Maximum feedrates (units/s)
    //
    gcode.M203_report(forReplay);

    //
    // M201 Maximum Acceleration (units/s2)
    //
    gcode.M201_report(forReplay);

    //
    // M204 Acceleration (units/s2)
    //
    gcode.M204_report(forReplay);

    //
    // M205 "Advanced" Settings
    //
    gcode.M205_report(forReplay);

    //
    // M206 Home Offset
    //
    TERN_(HAS_M206_COMMAND, gcode.M206_report(forReplay));

    //
    // M218 Hotend offsets
    //
    TERN_(HAS_HOTEND_OFFSET, gcode.M218_report(forReplay));

    //
    // Bed Leveling
    //
    #if HAS_LEVELING

      gcode.M420_report(forReplay);

      #if ENABLED(MESH_BED_LEVELING)

        if (leveling_is_valid()) {
          LOOP_L_N(py, GRID_MAX_POINTS_Y) {
            LOOP_L_N(px, GRID_MAX_POINTS_X) {
              CONFIG_ECHO_START();
              SERIAL_ECHOPGM("  G29 S3 I", px, " J", py);
              SERIAL_ECHOLNPAIR_F_P(SP_Z_STR, LINEAR_UNIT(mbl.z_values[px][py]), 5);
            }
          }
          CONFIG_ECHO_START();
          SERIAL_ECHOLNPAIR_F("  G29 S4 Z", LINEAR_UNIT(mbl.z_offset), 5);
        }

      #elif ENABLED(AUTO_BED_LEVELING_UBL)

        if (!forReplay) {
          SERIAL_EOL();
          ubl.report_state();
          SERIAL_ECHO_MSG("Active Mesh Slot ", ubl.storage_slot);
          SERIAL_ECHO_MSG("EEPROM can hold ", calc_num_meshes(), " meshes.\n");
        }

       //ubl.report_current_mesh();   // This is too verbose for large meshes. A better (more terse)
                                                  // solution needs to be found.
      #elif ENABLED(AUTO_BED_LEVELING_BILINEAR)

        if (leveling_is_valid()) {
          LOOP_L_N(py, GRID_MAX_POINTS_Y) {
            LOOP_L_N(px, GRID_MAX_POINTS_X) {
              CONFIG_ECHO_START();
              SERIAL_ECHOPGM("  G29 W I", px, " J", py);
              SERIAL_ECHOLNPAIR_F_P(SP_Z_STR, LINEAR_UNIT(z_values[px][py]), 5);
            }
          }
        }

      #endif

    #endif // HAS_LEVELING

    //
    // Editable Servo Angles
    //
    TERN_(EDITABLE_SERVO_ANGLES, gcode.M281_report(forReplay));

    //
    // Kinematic Settings
    //
    TERN_(IS_KINEMATIC, gcode.M665_report(forReplay));

    //
    // M666 Endstops Adjustment
    //
    #if EITHER(DELTA, HAS_EXTRA_ENDSTOPS)
      gcode.M666_report(forReplay);
    #endif

    //
    // Z Auto-Align
    //
    TERN_(Z_STEPPER_AUTO_ALIGN, gcode.M422_report(forReplay));

    //
    // LCD Preheat Settings
    //
    #if HAS_PREHEAT
      gcode.M145_report(forReplay);
    #endif

    //
    // PID
    //
    TERN_(PIDTEMP,        gcode.M301_report(forReplay));
    TERN_(PIDTEMPBED,     gcode.M304_report(forReplay));
    TERN_(PIDTEMPCHAMBER, gcode.M309_report(forReplay));

    #if HAS_USER_THERMISTORS
      LOOP_L_N(i, USER_THERMISTORS)
        thermalManager.M305_report(i, forReplay);
    #endif

    //
    // LCD Contrast
    //
    TERN_(HAS_LCD_CONTRAST, gcode.M250_report(forReplay));

    //
    // LCD Brightness
    //
    TERN_(HAS_LCD_BRIGHTNESS, gcode.M256_report(forReplay));

    //
    // Controller Fan
    //
    TERN_(CONTROLLER_FAN_EDITABLE, gcode.M710_report(forReplay));

    //
    // Power-Loss Recovery
    //
    TERN_(POWER_LOSS_RECOVERY, gcode.M413_report(forReplay));

    //
    // Firmware Retraction
    //
    #if ENABLED(FWRETRACT)
      gcode.M207_report(forReplay);
      gcode.M208_report(forReplay);
      TERN_(FWRETRACT_AUTORETRACT, gcode.M209_report(forReplay));
    #endif

    //
    // Probe Offset
    //
    TERN_(HAS_BED_PROBE, gcode.M851_report(forReplay));

    //
    // Bed Skew Correction
    //
    TERN_(SKEW_CORRECTION_GCODE, gcode.M852_report(forReplay));

    #if HAS_TRINAMIC_CONFIG
      //
      // TMC Stepper driver current
      //
      gcode.M906_report(forReplay);

      //
      // TMC Hybrid Threshold
      //
      TERN_(HYBRID_THRESHOLD, gcode.M913_report(forReplay));

      //
      // TMC Sensorless homing thresholds
      //
      TERN_(USE_SENSORLESS, gcode.M914_report(forReplay));
    #endif

    //
    // TMC stepping mode
    //
    TERN_(HAS_STEALTHCHOP, gcode.M569_report(forReplay));

    //
    // Linear Advance
    //
    TERN_(LIN_ADVANCE, gcode.M900_report(forReplay));

    //
    // Motor Current (SPI or PWM)
    //
    #if HAS_MOTOR_CURRENT_SPI || HAS_MOTOR_CURRENT_PWM
      gcode.M907_report(forReplay);
    #endif

    //
    // Advanced Pause filament load & unload lengths
    //
    TERN_(ADVANCED_PAUSE_FEATURE, gcode.M603_report(forReplay));

    //
    // Tool-changing Parameters
    //
    E_TERN_(gcode.M217_report(forReplay));

    //
    // Backlash Compensation
    //
    TERN_(BACKLASH_GCODE, gcode.M425_report(forReplay));

    //
    // Filament Runout Sensor
    //
    TERN_(HAS_FILAMENT_SENSOR, gcode.M412_report(forReplay));

    #if HAS_ETHERNET
      CONFIG_ECHO_HEADING("Ethernet");
      if (!forReplay) ETH0_report();
      CONFIG_ECHO_START(); SERIAL_ECHO_SP(2); MAC_report();
      CONFIG_ECHO_START(); SERIAL_ECHO_SP(2); gcode.M552_report();
      CONFIG_ECHO_START(); SERIAL_ECHO_SP(2); gcode.M553_report();
      CONFIG_ECHO_START(); SERIAL_ECHO_SP(2); gcode.M554_report();
    #endif

    TERN_(HAS_MULTI_LANGUAGE, gcode.M414_report(forReplay));
  }

#endif // !DISABLE_M503

#pragma pack(pop)
