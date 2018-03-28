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

#ifndef __ENUM_H__
#define __ENUM_H__

#include "MarlinConfig.h"

/**
 * Axis indices as enumerated constants
 *
 * Special axis:
 *  - A_AXIS and B_AXIS are used by COREXY printers
 *  - X_HEAD and Y_HEAD is used for systems that don't have a 1:1 relationship
 *    between X_AXIS and X Head movement, like CoreXY bots
 */
enum AxisEnum : unsigned char {
  X_AXIS    = 0,
  A_AXIS    = 0,
  Y_AXIS    = 1,
  B_AXIS    = 1,
  Z_AXIS    = 2,
  C_AXIS    = 2,
  E_AXIS    = 3,
  X_HEAD    = 4,
  Y_HEAD    = 5,
  Z_HEAD    = 6,
  ALL_AXES  = 0xFE,
  NO_AXIS   = 0xFF
};

#define LOOP_S_LE_N(VAR, S, N) for (uint8_t VAR=S; VAR<=N; VAR++)
#define LOOP_S_L_N(VAR, S, N) for (uint8_t VAR=S; VAR<N; VAR++)
#define LOOP_LE_N(VAR, N) LOOP_S_LE_N(VAR, 0, N)
#define LOOP_L_N(VAR, N) LOOP_S_L_N(VAR, 0, N)

#define LOOP_NA(VAR) LOOP_L_N(VAR, NUM_AXIS)
#define LOOP_XYZ(VAR) LOOP_S_LE_N(VAR, X_AXIS, Z_AXIS)
#define LOOP_XYZE(VAR) LOOP_S_LE_N(VAR, X_AXIS, E_AXIS)
#define LOOP_XYZE_N(VAR) LOOP_S_L_N(VAR, X_AXIS, XYZE_N)
#define LOOP_ABC(VAR) LOOP_S_LE_N(VAR, A_AXIS, C_AXIS)
#define LOOP_ABCE(VAR) LOOP_S_LE_N(VAR, A_AXIS, E_AXIS)
#define LOOP_ABCE_N(VAR) LOOP_S_L_N(VAR, A_AXIS, XYZE_N)

typedef enum {
  LINEARUNIT_MM,
  LINEARUNIT_INCH
} LinearUnit;

typedef enum {
  TEMPUNIT_C,
  TEMPUNIT_K,
  TEMPUNIT_F
} TempUnit;

/**
 * Debug flags
 * Not yet widely applied
 */
enum DebugFlags : unsigned char {
  DEBUG_NONE          = 0,
  DEBUG_ECHO          = _BV(0), ///< Echo commands in order as they are processed
  DEBUG_INFO          = _BV(1), ///< Print messages for code that has debug output
  DEBUG_ERRORS        = _BV(2), ///< Not implemented
  DEBUG_DRYRUN        = _BV(3), ///< Ignore temperature setting and E movement commands
  DEBUG_COMMUNICATION = _BV(4), ///< Not implemented
  DEBUG_LEVELING      = _BV(5), ///< Print detailed output for homing and leveling
  DEBUG_MESH_ADJUST   = _BV(6), ///< UBL bed leveling
  DEBUG_ALL           = 0xFF
};

enum EndstopEnum : char {
  X_MIN,
  Y_MIN,
  Z_MIN,
  Z_MIN_PROBE,
  X_MAX,
  Y_MAX,
  Z_MAX,
  X2_MIN,
  X2_MAX,
  Y2_MIN,
  Y2_MAX,
  Z2_MIN,
  Z2_MAX
};

#if ENABLED(EMERGENCY_PARSER)
  enum e_parser_state : char {
    state_RESET,
    state_N,
    state_M,
    state_M1,
    state_M10,
    state_M108,
    state_M11,
    state_M112,
    state_M4,
    state_M41,
    state_M410,
    state_IGNORE // to '\n'
  };
#endif

#if ENABLED(ADVANCED_PAUSE_FEATURE)
  enum AdvancedPauseMenuResponse : char {
    ADVANCED_PAUSE_RESPONSE_WAIT_FOR,
    ADVANCED_PAUSE_RESPONSE_EXTRUDE_MORE,
    ADVANCED_PAUSE_RESPONSE_RESUME_PRINT
  };

  #if ENABLED(ULTIPANEL)
    enum AdvancedPauseMessage : char {
      ADVANCED_PAUSE_MESSAGE_INIT,
      ADVANCED_PAUSE_MESSAGE_UNLOAD,
      ADVANCED_PAUSE_MESSAGE_INSERT,
      ADVANCED_PAUSE_MESSAGE_LOAD,
      ADVANCED_PAUSE_MESSAGE_PURGE,
      #if ENABLED(ADVANCED_PAUSE_CONTINUOUS_PURGE)
        ADVANCED_PAUSE_MESSAGE_CONTINUOUS_PURGE,
      #endif
      ADVANCED_PAUSE_MESSAGE_OPTION,
      ADVANCED_PAUSE_MESSAGE_RESUME,
      ADVANCED_PAUSE_MESSAGE_STATUS,
      ADVANCED_PAUSE_MESSAGE_CLICK_TO_HEAT_NOZZLE,
      ADVANCED_PAUSE_MESSAGE_WAIT_FOR_NOZZLES_TO_HEAT
    };
  #endif

  enum AdvancedPauseMode : char {
    ADVANCED_PAUSE_MODE_PAUSE_PRINT,
    ADVANCED_PAUSE_MODE_LOAD_FILAMENT,
    ADVANCED_PAUSE_MODE_UNLOAD_FILAMENT
  };
#endif

/**
 * States for managing Marlin and host communication
 * Marlin sends messages if blocked or busy
 */
#if ENABLED(HOST_KEEPALIVE_FEATURE)
  enum MarlinBusyState : char {
    NOT_BUSY,           // Not in a handler
    IN_HANDLER,         // Processing a GCode
    IN_PROCESS,         // Known to be blocking command input (as in G29)
    PAUSED_FOR_USER,    // Blocking pending any input
    PAUSED_FOR_INPUT    // Blocking pending text input (concept)
  };
#endif

/**
 * SD Card
 */
enum LsAction : char { LS_SerialPrint, LS_Count, LS_GetFilename };

/**
 * Ultra LCD
 */
enum LCDViewAction : char {
  LCDVIEW_NONE,
  LCDVIEW_REDRAW_NOW,
  LCDVIEW_CALL_REDRAW_NEXT,
  LCDVIEW_CLEAR_CALL_REDRAW,
  LCDVIEW_CALL_NO_REDRAW
};

/**
 * Dual X Carriage modes. A Dual Nozzle can also do duplication.
 */
#if ENABLED(DUAL_X_CARRIAGE) || ENABLED(DUAL_NOZZLE_DUPLICATION_MODE)
  enum DualXMode : char {
    DXC_FULL_CONTROL_MODE,  // DUAL_X_CARRIAGE only
    DXC_AUTO_PARK_MODE,     // DUAL_X_CARRIAGE only
    DXC_DUPLICATION_MODE
  };
#endif

/**
 * Workspace planes only apply to G2/G3 moves
 * (and "canned cycles" - not a current feature)
 */
#if ENABLED(CNC_WORKSPACE_PLANES)
  enum WorkspacePlane : char { PLANE_XY, PLANE_ZX, PLANE_YZ };
#endif

#endif // __ENUM_H__
