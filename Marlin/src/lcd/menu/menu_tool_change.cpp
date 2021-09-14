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

//
// Tool Change Menu
//

#include "../../inc/MarlinConfigPre.h"

#if HAS_LCD_MENU && ENABLED(MANUAL_SWITCHING_TOOLHEAD)

#include "menu.h"
#include "menu_item.h"
#include "../../module/tool_change.h"

#if ENABLED(SWITCHING_TOOLHEAD_PARKING)
  #include "../../libs/nozzle.h"
  xyz_pos_t toolchange_resume_position;
#endif

/**
 * Callback for a completed/successful tool change.
 *
 */
void menu_tool_change_done() {
  tool_change(editable.uint8);
  thermalManager.heating_enabled = true;
  ui.set_status_P(PSTR("Tool Changed"));
  ui.return_to_status();

  #if ENABLED(SWITCHING_TOOLHEAD_PARKING)
    // Move XY to starting position, then Z
    do_blocking_move_to_xy(toolchange_resume_position, feedRate_t(NOZZLE_PARK_XY_FEEDRATE));

    // Move Z_AXIS to saved position
    do_blocking_move_to_z(toolchange_resume_position.z, feedRate_t(NOZZLE_PARK_Z_FEEDRATE));
  #endif
}


/**
 * Callback for a cancelled tool change.
 *
 */
void menu_tool_change_cancel() {
  thermalManager.heating_enabled = true;
  ui.set_status_P(PSTR("M117 Tool Change Cancelled"));
  ui.return_to_status();

  #if ENABLED(SWITCHING_TOOLHEAD_PARKING)
    // Move XY to starting position, then Z
    do_blocking_move_to_xy(toolchange_resume_position, feedRate_t(NOZZLE_PARK_XY_FEEDRATE));

    // Move Z_AXIS to saved position
    do_blocking_move_to_z(toolchange_resume_position.z, feedRate_t(NOZZLE_PARK_Z_FEEDRATE));
  #endif
}


/**
 * Menu shown when a tool is being changed, prompting the user
 * to install the selected tool, and allowing cancelling of the
 * tool change.
 *
 */
void menu_tool_changing() {
  thermalManager.heating_enabled = false;
  #if ENABLED(SWITCHING_TOOLHEAD_PARKING)
    toolchange_resume_position = current_position;
    nozzle.park(0);
  #endif

  ui.push_current_screen();
  ui.goto_screen([]{
    const char* tool_name;
    #ifdef SWITCHING_TOOLHEAD_TOOL_NAMES
      tool_name = toolhead_names[editable.uint8];
    #else
      if (editable.uint8 < HOTENDS) {
        char buf[7 + 1 + 1]; // "Hotend ", digit, null
        sprintf_P(buf, PSTR("Hotend %c"), (editable.uint8 + 1) + '0');
        tool_name = buf;
      } else {
        char buf[5 + 1 + 1]; // "Tool ", digit, null
        sprintf_P(buf, PSTR("Tool %c"), (editable.uint8 + 1) + '0');
        tool_name = buf;
      }
    #endif

    MenuItem_confirm::select_screen(
      GET_TEXT(MSG_BUTTON_DONE),
      GET_TEXT(MSG_BUTTON_CANCEL),
      menu_tool_change_done,
      menu_tool_change_cancel,
      PSTR("Install tool:"),
      tool_name,
      (const char *)nullptr
    );
  });
}

/**
 * Hotend Tool menu, listing all tools set up in Configuration.h
 * under SWITCHING_TOOLHEAD_HOTEND_QTY, SWITCHING_TOOLHEAD_HOTEND_NAMES.
 *
 */
void menu_tool_change_hotend() {
  START_MENU();

  //
  // ^ Tool Change
  //
  BACK_ITEM(MSG_TOOL_CHANGE);

  // Display Hotend 1 .. Hotend n, or hotend names.
  LOOP_L_N(e, SWITCHING_TOOLHEAD_TOOL_QTY) {
    if (e == HOTENDS) break;
    if (e == active_extruder) continue;

    editable.uint8 = e;
    #ifdef SWITCHING_TOOLHEAD_TOOL_NAMES
      SUBMENU_P(toolhead_names[e], []{ menu_tool_changing(); });
    #else
      SUBMENU_N(e, MSG_HOTEND_N, []{ menu_tool_changing(); });
    #endif
  }

  END_MENU();
}

/**
 *
 */
void menu_tool_change_laser_spindle() {
  START_MENU();

  //
  // ^ Tool Change
  //
  BACK_ITEM(MSG_TOOL_CHANGE);

  END_MENU();
}


/**
 * Unpowered Tool menu, listing all tools set up in Configuration.h
 * under SWITCHING_TOOLHEAD_UNPOWERED_QTY, SWITCHING_TOOLHEAD_UNPOWERED_NAMES.
 *
 */
void menu_tool_change_unpowered() {
  START_MENU();

  //
  // ^ Tool Change
  //
  BACK_ITEM(MSG_TOOL_CHANGE);

  // Display Tool 1 .. Tool n, or tool names.
  LOOP_S_L_N(e, HOTENDS, SWITCHING_TOOLHEAD_TOOL_QTY) {
    if (e == active_extruder) continue;

    editable.uint8 = e;
    #ifdef SWITCHING_TOOLHEAD_TOOL_NAMES
      SUBMENU_P(toolhead_names[e], []{ menu_tool_changing(); });
    #else
      SUBMENU_N(e-HOTENDS, MSG_TOOL_N, []{ menu_tool_changing(); });
    #endif
  }

  END_MENU();
}


/**
 * Main "Tool Change" menu, with options for Hotends,
 * Laser/Spindle, and unpowered tools.
 *
 */
void menu_tool_change() {
  START_MENU();

  //
  // ^ Main
  //
  BACK_ITEM(MSG_MAIN);

  #if HOTENDS > 0
    SUBMENU(MSG_HOTEND, menu_tool_change_hotend);
  #endif

  #if ENABLED(SWITCHING_TOOLHEAD_LASER_SPINDLE)
    SUBMENU(MSG_LASER_SPINDLE, menu_tool_change_laser_spindle);
  #endif

  #if UNPOWERED_TOOLS > 0
    SUBMENU(MSG_TOOL_CHANGE_UNPOWERED, menu_tool_change_unpowered);
  #endif

  END_MENU();
}

#endif
