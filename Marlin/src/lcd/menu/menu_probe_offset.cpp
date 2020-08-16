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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

//
// Calibrate Probe offset menu.
//

#include "../../inc/MarlinConfigPre.h"

#if BOTH(HAS_LCD_MENU, PROBE_OFFSET_MENU) && DISABLED(BABYSTEP_ZPROBE_OFFSET)

#include "menu.h"
#include "menu_addon.h"
#include "../../module/motion.h"
#include "../../module/planner.h"

#if HAS_LEVELING
  #include "../../feature/bedlevel/bedlevel.h"
#endif

#if HAS_BED_PROBE
  #include "../../module/probe.h"
#endif

#ifndef PROBE_OFFSET_START
  #define PROBE_OFFSET_START -4.0
#endif

static_assert(PROBE_OFFSET_START < 0, "PROBE_OFFSET_START must be < 0. Please update your configuration.");

#if HAS_LEVELING
  static bool leveling_was_active = false;
#endif

#if HAS_SOFTWARE_ENDSTOPS
  bool store_soft_endstops_enabled;
#endif
float store_z_offset;
float calculated_z_offset;

static void _lcd_probe_offset_move_undo(){
  #if HAS_SOFTWARE_ENDSTOPS
    soft_endstops_enabled = store_soft_endstops_enabled;
  #endif
  probe.offset.z = store_z_offset;
  ui.goto_previous_screen_no_defer();

}

static void _lcd_probe_offset_move_done(){
  probe.offset.z = calculated_z_offset;
  #if HAS_SOFTWARE_ENDSTOPS
    soft_endstops_enabled = store_soft_endstops_enabled;
  #endif

  #if HAS_LEVELING
    set_bed_leveling_enabled(leveling_was_active);
  #endif
  #if Z_AFTER_HOMING
    do_z_clearance(Z_AFTER_HOMING);
  #else
    do_z_clearance(20.0);
  #endif

  ui.goto_previous_screen_no_defer();
};

void _goto_manual_move_z(const float scale) {
  ui.defer_status_screen();
  ui.manual_move.menu_scale = scale;
  ui.goto_screen(lcd_move_z);
}

static void _lcd_probe_offset_move_z() {
  START_MENU();
  calculated_z_offset = probe.offset.z + current_position.z;

  if (LCD_HEIGHT >= 4)
    STATIC_ITEM(MSG_MOVE_Z, SS_CENTER|SS_INVERT);

  VALUE_ITEM(MSG_MESH_EDIT_Z, ftostr42_52(current_position.z), SS_LEFT);
  VALUE_ITEM(MSG_ZPROBE_ZOFFSET, ftostr42_52(calculated_z_offset), SS_LEFT);

  SUBMENU(MSG_MOVE_1MM,  []{ _goto_manual_move_z( 1);    });
  SUBMENU(MSG_MOVE_01MM, []{ _goto_manual_move_z( 0.1f); });
  char tmp[20], numstr[10];

  if ((SHORT_MANUAL_Z_MOVE) > 0.0f && (SHORT_MANUAL_Z_MOVE) < 0.1f) {
    extern const char NUL_STR[];
    SUBMENU_P(NUL_STR, []{ _goto_manual_move_z(float(SHORT_MANUAL_Z_MOVE)); });
    MENU_ITEM_ADDON_START(0 + ENABLED(HAS_CHARACTER_LCD));
      // Determine digits needed right of decimal
      const uint8_t digs = !UNEAR_ZERO((SHORT_MANUAL_Z_MOVE) * 1000 - int((SHORT_MANUAL_Z_MOVE) * 1000)) ? 4 :
                           !UNEAR_ZERO((SHORT_MANUAL_Z_MOVE) *  100 - int((SHORT_MANUAL_Z_MOVE) *  100)) ? 3 : 2;
      sprintf_P(tmp, GET_TEXT(MSG_MOVE_Z_DIST), dtostrf(SHORT_MANUAL_Z_MOVE, 1, digs, numstr));
      lcd_put_u8str(tmp);
    MENU_ITEM_ADDON_END();
  }

  ACTION_ITEM(MSG_BUTTON_DONE,   _lcd_probe_offset_move_done);
  ACTION_ITEM(MSG_BUTTON_CANCEL, _lcd_probe_offset_move_undo);

  END_MENU();
}

void _lcd_probe_offset() {
  ui.defer_status_screen();

  set_all_unhomed();
  queue.inject_P(G28_STR);

  store_z_offset = probe.offset.z;
  probe.offset.z = PROBE_OFFSET_START;

  // Disable leveling so the planner won't mess with us
  #if HAS_LEVELING
    leveling_was_active = planner.leveling_active;
    set_bed_leveling_enabled(false);
  #endif

  #if HAS_SOFTWARE_ENDSTOPS
    store_soft_endstops_enabled = soft_endstops_enabled;
    soft_endstops_enabled = false;
  #endif

  ui.goto_screen([]{
    _lcd_draw_homing();
    if (all_axes_homed()) {
      ui.goto_screen(_lcd_probe_offset_move_z);
      ui.defer_status_screen();
    }
  });
}

#endif // HAS_LCD_MENU && PROBE_OFFSET_MENU && !BABYSTEP_ZPROBE_OFFSET
