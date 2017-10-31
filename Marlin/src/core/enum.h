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

#include "../inc/MarlinConfig.h"

/**
 * Axis indices as enumerated constants
 *
 * Special axis:
 *  - A_AXIS and B_AXIS are used by COREXY printers
 *  - X_HEAD and Y_HEAD is used for systems that don't have a 1:1 relationship
 *    between X_AXIS and X Head movement, like CoreXY bots
 */
enum AxisEnum {
  NO_AXIS   = -1,
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
  ALL_AXES  = 100
};

#define LOOP_S_LE_N(VAR, S, N) for (uint8_t VAR=S; VAR<=N; VAR++)
#define LOOP_S_L_N(VAR, S, N) for (uint8_t VAR=S; VAR<N; VAR++)
#define LOOP_LE_N(VAR, N) LOOP_S_LE_N(VAR, 0, N)
#define LOOP_L_N(VAR, N) LOOP_S_L_N(VAR, 0, N)

#define LOOP_NA(VAR) LOOP_L_N(VAR, NUM_AXIS)
#define LOOP_XYZ(VAR) LOOP_S_LE_N(VAR, X_AXIS, Z_AXIS)
#define LOOP_XYZE(VAR) LOOP_S_LE_N(VAR, X_AXIS, E_AXIS)
#define LOOP_XYZE_N(VAR) LOOP_S_L_N(VAR, X_AXIS, XYZE_N)

typedef enum {
  LINEARUNIT_MM,
  LINEARUNIT_INCH
} LinearUnit;

typedef enum {
  TEMPUNIT_C,
  TEMPUNIT_K,
  TEMPUNIT_F
} TempUnit;

#if ENABLED(EMERGENCY_PARSER)
  enum e_parser_state {
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
  enum AdvancedPauseMenuResponse {
    ADVANCED_PAUSE_RESPONSE_WAIT_FOR,
    ADVANCED_PAUSE_RESPONSE_EXTRUDE_MORE,
    ADVANCED_PAUSE_RESPONSE_RESUME_PRINT
  };

  #if ENABLED(ULTIPANEL)
    enum AdvancedPauseMessage {
      ADVANCED_PAUSE_MESSAGE_INIT,
      ADVANCED_PAUSE_MESSAGE_UNLOAD,
      ADVANCED_PAUSE_MESSAGE_INSERT,
      ADVANCED_PAUSE_MESSAGE_LOAD,
      ADVANCED_PAUSE_MESSAGE_EXTRUDE,
      ADVANCED_PAUSE_MESSAGE_OPTION,
      ADVANCED_PAUSE_MESSAGE_RESUME,
      ADVANCED_PAUSE_MESSAGE_STATUS,
      ADVANCED_PAUSE_MESSAGE_CLICK_TO_HEAT_NOZZLE,
      ADVANCED_PAUSE_MESSAGE_WAIT_FOR_NOZZLES_TO_HEAT
    };
  #endif
#endif

/**
 * SD Card
 */
enum LsAction { LS_SerialPrint, LS_Count, LS_GetFilename };

/**
 * Ultra LCD
 */
enum LCDViewAction {
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
  enum DualXMode {
    DXC_FULL_CONTROL_MODE,  // DUAL_X_CARRIAGE only
    DXC_AUTO_PARK_MODE,     // DUAL_X_CARRIAGE only
    DXC_DUPLICATION_MODE
  };
#endif

#endif // __ENUM_H__
