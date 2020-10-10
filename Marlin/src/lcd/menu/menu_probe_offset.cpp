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

#if ENABLED(PROBE_OFFSET_WIZARD)

#ifndef PROBE_OFFSET_START
  #error "PROBE_OFFSET_WIZARD requires a PROBE_OFFSET_START with a negative value."
#else
  static_assert(PROBE_OFFSET_START < 0, "PROBE_OFFSET_START must be < 0. Please update your configuration.");
#endif

#include "menu_item.h"
#include "menu_addon.h"
#include "../../module/motion.h"
#include "../../module/planner.h"
#include "../../module/probe.h"

#if HAS_LEVELING
  #include "../../feature/bedlevel/bedlevel.h"
#endif

// Global storage
float z_offset_backup, calculated_z_offset;

TERN_(HAS_LEVELING, bool leveling_was_active);
TERN_(HAS_SOFTWARE_ENDSTOPS, bool store_soft_endstops_enabled);

void prepare_for_calibration() {
  z_offset_backup = probe.offset.z;

  // Disable soft endstops for free Z movement
  #if HAS_SOFTWARE_ENDSTOPS
    store_soft_endstops_enabled = soft_endstops_enabled;
    soft_endstops_enabled = false;
  #endif

  // Disable leveling for raw planner motion
  #if HAS_LEVELING
    leveling_was_active = planner.leveling_active;
    set_bed_leveling_enabled(false);
  #endif
}

void set_offset_and_go_back(const float &z) {
  probe.offset.z = z;
  TERN_(HAS_SOFTWARE_ENDSTOPS, soft_endstops_enabled = store_soft_endstops_enabled);
  TERN_(HAS_LEVELING, set_bed_leveling_enabled(leveling_was_active));
  ui.goto_previous_screen_no_defer();
}

void _goto_manual_move_z(const float scale) {
  ui.manual_move.menu_scale = scale;
  ui.goto_screen(lcd_move_z);
}

void probe_offset_wizard_menu() {
  START_MENU();
  calculated_z_offset = probe.offset.z + current_position.z;

  if (LCD_HEIGHT >= 4)
    STATIC_ITEM(MSG_MOVE_NOZZLE_TO_BED, SS_CENTER|SS_INVERT);

  STATIC_ITEM_P(PSTR("Z="), SS_CENTER, ftostr42_52(current_position.z));
  STATIC_ITEM(MSG_ZPROBE_ZOFFSET, SS_LEFT, ftostr42_52(calculated_z_offset));

  SUBMENU(MSG_MOVE_1MM,  []{ _goto_manual_move_z( 1);    });
  SUBMENU(MSG_MOVE_01MM, []{ _goto_manual_move_z( 0.1f); });

  if ((SHORT_MANUAL_Z_MOVE) > 0.0f && (SHORT_MANUAL_Z_MOVE) < 0.1f) {
    extern const char NUL_STR[];
    SUBMENU_P(NUL_STR, []{ _goto_manual_move_z(float(SHORT_MANUAL_Z_MOVE)); });
    MENU_ITEM_ADDON_START(0 + ENABLED(HAS_MARLINUI_HD44780));
      char tmp[20], numstr[10];
      // Determine digits needed right of decimal
      const uint8_t digs = !UNEAR_ZERO((SHORT_MANUAL_Z_MOVE) * 1000 - int((SHORT_MANUAL_Z_MOVE) * 1000)) ? 4 :
                           !UNEAR_ZERO((SHORT_MANUAL_Z_MOVE) *  100 - int((SHORT_MANUAL_Z_MOVE) *  100)) ? 3 : 2;
      sprintf_P(tmp, GET_TEXT(MSG_MOVE_Z_DIST), dtostrf(SHORT_MANUAL_Z_MOVE, 1, digs, numstr));
      lcd_put_u8str(tmp);
    MENU_ITEM_ADDON_END();
  }

  ACTION_ITEM(MSG_BUTTON_DONE, []{
    set_offset_and_go_back(calculated_z_offset);
    do_z_clearance(20.0
      #ifdef Z_AFTER_HOMING
        - 20.0 + Z_AFTER_HOMING
      #endif
    );
  });

  ACTION_ITEM(MSG_BUTTON_CANCEL, []{
    set_offset_and_go_back(z_offset_backup);
  });

  END_MENU();
}

void goto_probe_offset_wizard() {
  ui.defer_status_screen();

  prepare_for_calibration();

  probe.offset.z = PROBE_OFFSET_START;

  set_all_unhomed();
  queue.inject_P(G28_STR);

  ui.goto_screen([]{
    _lcd_draw_homing();
    if (all_axes_homed()) {
      ui.goto_screen(probe_offset_wizard_menu);
      ui.defer_status_screen();
    }
  });
}

#endif // PROBE_OFFSET_WIZARD
