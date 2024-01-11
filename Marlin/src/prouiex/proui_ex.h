/**
 * Professional Firmware UI extensions
 * Author: Miguel A. Risco-Castillo
 * Version: 3.2.0
 * Date: 2023/11/09
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * For commercial applications additional licenses can be requested
 */
#pragma once

#include "../inc/MarlinConfigPre.h"

#if (PROBING_MARGIN!=10)
  #error "PROUI_EX requires a PROBING_MARGIN of 10."
#endif
#if HAS_PROUI_RUNOUT_SENSOR && DISABLED(FILAMENT_RUNOUT_SENSOR)
  #error "HAS_PROUI_RUNOUT_SENSOR requires FILAMENT_RUNOUT_SENSOR."
#endif
#if HAS_PROUI_RUNOUT_SENSOR && DISABLED(HAS_FILAMENT_RUNOUT_DISTANCE)
  #error "PROUI_EX requires FILAMENT_RUNOUT_DISTANCE_MM if HAS_PROUI_RUNOUT_SENSOR was enabled."
#endif
#if HAS_PROUI_MAXTEMP && DISABLED(HAS_HOTEND)
  #error "HAS_PROUI_MAXTEMP requires HAS_HOTEND."
#endif

#include "../core/types.h"

constexpr uint8_t TBMaxOpt = 5;       // Amount of shortcuts on screen
#if HAS_BED_PROBE
  #define DEF_TBOPT {1, 2, 3, 4, 5}   // Default shorcuts for ALB/UBL
#else
  #define DEF_TBOPT {1, 2, 5, 6, 7};  // Default shortcuts for MM
#endif

#ifdef HAL_STM32
  #ifndef LOW
    #define LOW  0x0
  #endif
  #ifndef HIGH
    #define HIGH 0x1
  #endif
#endif

#define X_BED_MIN 150
#define Y_BED_MIN 150
constexpr uint16_t DEF_X_BED_SIZE = X_BED_SIZE;
constexpr uint16_t DEF_Y_BED_SIZE = Y_BED_SIZE;
constexpr int16_t DEF_X_MIN_POS = X_MIN_POS;
constexpr int16_t DEF_Y_MIN_POS = Y_MIN_POS;
constexpr int16_t DEF_X_MAX_POS = X_MAX_POS;
constexpr int16_t DEF_Y_MAX_POS = Y_MAX_POS;
constexpr int16_t DEF_Z_MAX_POS = Z_MAX_POS;
constexpr bool DEF_INVERT_E0_DIR = INVERT_E0_DIR;
#if ENABLED(NOZZLE_PARK_FEATURE)
  constexpr xyz_int_t DEF_NOZZLE_PARK_POINT = NOZZLE_PARK_POINT;
#else
  constexpr xyz_int_t DEF_NOZZLE_PARK_POINT = {0};
#endif
constexpr uint8_t DEF_GRID_MAX_POINTS_X = TERN(HAS_MESH, GRID_MAX_POINTS_X, 3);
constexpr uint8_t DEF_GRID_MAX_POINTS_Y = TERN(HAS_MESH, GRID_MAX_POINTS_Y, 3);
#define GRID_MIN 3
#define GRID_LIMIT 9
#ifndef MESH_INSET
  #define MESH_INSET 25
#endif
#ifndef MESH_MIN_X
  #define MESH_MIN_X MESH_INSET
#endif
#ifndef MESH_MIN_Y
  #define MESH_MIN_Y MESH_INSET
#endif
#ifndef MESH_MAX_X
  #define MESH_MAX_X  X_BED_SIZE - (MESH_INSET)
#endif
#ifndef MESH_MAX_Y
  #define MESH_MAX_Y  Y_BED_SIZE - (MESH_INSET)
#endif
constexpr uint16_t DEF_MESH_MIN_X = MESH_MIN_X;
constexpr uint16_t DEF_MESH_MAX_X = MESH_MAX_X;
constexpr uint16_t DEF_MESH_MIN_Y = MESH_MIN_Y;
constexpr uint16_t DEF_MESH_MAX_Y = MESH_MAX_Y;
#define MIN_MESH_INSET TERN(HAS_BED_PROBE, PROBING_MARGIN, 5)
#define MAX_MESH_INSET X_BED_SIZE
constexpr uint16_t DEF_PROBING_MARGIN = PROBING_MARGIN;
#define MIN_PROBE_MARGIN 5
#define MAX_PROBE_MARGIN 60
constexpr uint16_t DEF_Z_PROBE_FEEDRATE_SLOW = (Z_PROBE_FEEDRATE_FAST / 2);
#ifndef MULTIPLE_PROBING
  #define MULTIPLE_PROBING 0
#endif
#define DEF_FIL_MOTION_SENSOR ENABLED(FILAMENT_MOTION_SENSOR)
#ifndef FIL_RUNOUT_STATE
  #define FIL_RUNOUT_STATE 0x0
#endif

#if HAS_MESH
  typedef struct {
    uint8_t grid_max_points_x = DEF_GRID_MAX_POINTS_X;
    uint8_t grid_max_points_y = DEF_GRID_MAX_POINTS_Y;
    uint16_t mesh_min_x = DEF_MESH_MIN_X;
    uint16_t mesh_max_x = DEF_MESH_MAX_X;
    uint16_t mesh_min_y = DEF_MESH_MIN_Y;
    uint16_t mesh_max_y = DEF_MESH_MAX_Y;
  } MeshSet_t;
  extern MeshSet_t meshSet;
#endif

typedef struct { // Do not change this data structure
  uint16_t x_bed_size = DEF_X_BED_SIZE;
  uint16_t y_bed_size = DEF_Y_BED_SIZE;
  int16_t x_min_pos  = DEF_X_MIN_POS;
  int16_t y_min_pos  = DEF_Y_MIN_POS;
  int16_t x_max_pos  = DEF_X_MAX_POS;
  int16_t y_max_pos  = DEF_Y_MAX_POS;
  int16_t z_max_pos  = DEF_Z_MAX_POS;
  float probezfix = DEF_PROBEZFIX;
  uint16_t zprobefeedslow = DEF_Z_PROBE_FEEDRATE_SLOW;
  uint8_t multiple_probing = MULTIPLE_PROBING;
  bool Invert_E0 = DEF_INVERT_E0_DIR;
  xyz_int_t Park_point = DEF_NOZZLE_PARK_POINT;
  bool Runout_active_state = FIL_RUNOUT_STATE;
  bool FilamentMotionSensor = DEF_FIL_MOTION_SENSOR;
  celsius_t hotend_maxtemp = HEATER_0_MAXTEMP;
  uint8_t TBopt[TBMaxOpt] = DEF_TBOPT;
} PRO_data_t;
extern PRO_data_t PRO_data;

class ProUIEx {
public:
  static constexpr size_t eeprom_data_size = sizeof(PRO_data_t);
  static void init();
  static void update();
  static void copySettingsTo(char * const buff);
  static void copySettingsFrom(const char * const buff);
#if HAS_ABL_OR_UBL
  static bool cancel_lev;
  static void heatedBed();
  static void stopLeveling();
  static bool quitLeveling();
  static void meshUpdate(const uint8_t x, const uint8_t y, const_float_t zval);
  static void levelingDone();
#endif
#if HAS_MEDIA
  static void C10();
#endif
#if HAS_PROUI_RUNOUT_SENSOR
  static void setRunoutState(uint32_t ulPin);
  #ifdef DWIN_LCD_PROUI
    static void drawRunoutActive(bool selected);
    static void applyRunoutActive();
  #endif
  static void C412();
  static void C412_report(const bool forReplay=true);
#endif
#if HAS_MESH
  static void checkMeshInsets();
  static void applyMeshLimits();
  static void maxMeshArea();
  static void centerMeshArea();
  static void C29();
  static void C29_report(const bool forReplay=true);
#endif
  static void C100();
  static void C100_report(const bool forReplay=true);
  static void C101();
  static void C101_report(const bool forReplay=true);
  static void C102();
  static void C102_report(const bool forReplay=true);
  #if HAS_PROUI_MAXTEMP
    static void C104();
    static void C104_report(const bool forReplay=true);
  #endif
  static void C115();
#if ENABLED(NOZZLE_PARK_FEATURE)
  static void C125();
  static void C125_report(const bool forReplay=true);
#endif
#if HAS_EXTRUDERS
  static void C562();
  static void C562_report(const bool forReplay=true);
#endif
  static void C851();
  static void C851_report(const bool forReplay=true);
#if HAS_TOOLBAR
  static void C810();
  static void C810_report(const bool forReplay=true);
#endif
  static void updateAxis(const AxisEnum axis);
  static void applyPhySet();
  static void checkParkingPos();
  static void setData();
  #if ANY(AUTO_BED_LEVELING_BILINEAR, MESH_BED_LEVELING)
    static float getZvalues(const uint8_t sy, const uint8_t x, const uint8_t y, const float *values);
  #endif
};

extern ProUIEx proUIEx;

#undef X_BED_SIZE
#undef Y_BED_SIZE
#undef X_MIN_POS
#undef Y_MIN_POS
#undef X_MAX_POS
#undef Y_MAX_POS
#undef Z_MAX_POS
#undef NOZZLE_PARK_POINT
#if HAS_MESH
  #undef GRID_MAX_POINTS_X
  #undef GRID_MAX_POINTS_Y
  #undef GRID_MAX_POINTS
  #undef MESH_MIN_X
  #undef MESH_MAX_X
  #undef MESH_MIN_Y
  #undef MESH_MAX_Y
#endif
#if HAS_BED_PROBE
  #undef Z_PROBE_FEEDRATE_SLOW
#endif
#undef INVERT_E0_DIR

#define X_BED_SIZE (float)PRO_data.x_bed_size
#define Y_BED_SIZE (float)PRO_data.y_bed_size
#define X_MIN_POS  (float)PRO_data.x_min_pos
#define Y_MIN_POS  (float)PRO_data.y_min_pos
#define X_MAX_POS  (float)PRO_data.x_max_pos
#define Y_MAX_POS  (float)PRO_data.y_max_pos
#define Z_MAX_POS  (float)PRO_data.z_max_pos
#define NOZZLE_PARK_POINT {(float)PRO_data.Park_point.x, (float)PRO_data.Park_point.y, (float)PRO_data.Park_point.z}
#if HAS_MESH
  #define GRID_MAX_POINTS_X meshSet.grid_max_points_x
  #define GRID_MAX_POINTS_Y meshSet.grid_max_points_y
  #define GRID_MAX_POINTS (meshSet.grid_max_points_x * meshSet.grid_max_points_y)
  #define MESH_MIN_X meshSet.mesh_min_x
  #define MESH_MAX_X meshSet.mesh_max_x
  #define MESH_MIN_Y meshSet.mesh_min_y
  #define MESH_MAX_Y meshSet.mesh_max_y
  extern float mesh_x_dist;
  extern float mesh_y_dist;
  #define MESH_X_DIST mesh_x_dist
  #define MESH_Y_DIST mesh_y_dist
#endif
#if HAS_BED_PROBE
  #define Z_PROBE_FEEDRATE_SLOW PRO_data.zprobefeedslow
#endif
#define INVERT_E0_DIR PRO_data.Invert_E0
