/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2022 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#include "../../MarlinCore.h"

#if ALL(HAS_LCD_MENU, MANUAL_SWITCHING_TOOLHEAD)

#include "menu.h"
#include "menu_item.h"
#include "../../module/tool_change.h"
#include "../../feature/pause.h"
#include "../../gcode/queue.h"

/**
 * Inject a toolchange gcode (Tn) using the editable.uint8 field,
 * and then return to the status screen.
 */
inline void inject_toolchange_gcode() {
  char tgc[3] = { '\0' };
  const char n = editable.uint8 + '0';
  sprintf_P(tgc, PSTR("T%c"), n);
  queue.inject(tgc);
  ui.return_to_status();
}

/**
 * Hotend Tool menu, listing all tools set up in Configuration.h
 * under SWITCHING_TOOLHEAD_HOTEND_QTY, SWITCHING_TOOLHEAD_HOTEND_NAMES.
 */
void menu_tool_change_hotend() {
  START_MENU();

  //
  // ^ Tool Change
  //
  BACK_ITEM(MSG_TOOL_CHANGE);

  // Display Hotend 1 .. Hotend n, or hotend names.
  for (uint8_t e = 0, e < MAN_ST_NUM_TOOLS; ++e) {
    if (e >= HOTENDS) break;
    if (e == active_extruder) continue;

    editable.uint8 = e;
    if (printingIsActive()) {
      CONFIRM_ITEM_P(PSTR("Change Tool?"),
        MSG_YES, MSG_NO,
        inject_toolchange_gcode, ui.goto_previous_screen,
        PSTR("Change Tool?"), (const char *)nullptr, PSTR("?"));
    }
    else
      ACTION_ITEM_P(tool_name(e), inject_toolchange_gcode);
  }

  END_MENU();
}

/**
 * TODO
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
 */
void menu_tool_change_unpowered() {
  START_MENU();

  //
  // ^ Tool Change
  //
  BACK_ITEM(MSG_TOOL_CHANGE);

  // Display Tool 1 .. Tool n, or tool names.
  for (uint8_t e = HOTENDS; e < MAN_ST_NUM_TOOLS; ++e) {
    if (e == active_extruder) continue;

    editable.uint8 = e;
    if (printingIsActive()) {
      CONFIRM_ITEM_P(
        PSTR("Change Tool?"),
        MSG_YES, MSG_NO,
        inject_toolchange_gcode, ui.goto_previous_screen,
        PSTR("Change Tool?"), (const char *)nullptr, PSTR("?"));
    }
    else
      ACTION_ITEM_P(tool_name(e), inject_toolchange_gcode);
  }

  END_MENU();
}

/**
 * Main "Tool Change" menu, with options for Hotends,
 * Laser/Spindle, and unpowered tools.
 */
void menu_tool_change() {
  START_MENU();

  // display the current tool
  STATIC_ITEM_P(tool_name(active_extruder), SS_DEFAULT|SS_INVERT);

  BACK_ITEM(MSG_MAIN);

  #if HOTENDS > 0
    if (HAS_MULTI_HOTEND || active_extruder != (MAN_ST_NUM_TOOLS - 1))
      SUBMENU(MSG_HOTEND, menu_tool_change_hotend);
  #endif

  #if UNPOWERED_TOOLS > 0
    if (UNPOWERED_TOOLS > 1 || active_extruder != (MAN_ST_NUM_TOOLS - 1))
      SUBMENU(MSG_TOOL_CHANGE_UNPOWERED, menu_tool_change_unpowered);
  #endif

  #if ENABLED(STM_LASER_SPINDLE)
    SUBMENU(MSG_LASER_SPINDLE, menu_tool_change_laser_spindle);
  #endif

  END_MENU();
}

#endif // HAS_LCD_MENU && MANUAL_SWITCHING_TOOLHEAD
