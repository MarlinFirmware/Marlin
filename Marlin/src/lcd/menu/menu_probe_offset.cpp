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
// Calibrate Probe offset menu.
//

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(PROBE_OFFSET_WIZARD)

#include "menu_item.h"
#include "menu_addon.h"
#include "../gcode.h"
#include "../../gcode/queue.h"
#include "../../module/motion.h"
#include "../../module/planner.h"
#include "../../module/probe.h"

#if HAS_LEVELING
  #include "../../feature/bedlevel/bedlevel.h"
#endif

// Global storage
float z_offset_backup, calculated_z_offset, z_offset_ref;

TERN_(HAS_LEVELING, bool leveling_was_active);



void set_offset_and_go_back(const float &z) {
  probe.offset.z = z;
  SET_SOFT_ENDSTOP_LOOSE(false);
  TERN_(HAS_LEVELING, set_bed_leveling_enabled(leveling_was_active));
  ui.goto_previous_screen_no_defer();
}

void _goto_manual_move_z(const float scale) {
  ui.manual_move.menu_scale = scale;
  ui.goto_screen(lcd_move_z);
}

void probe_offset_wizard_menu() {
  START_MENU();
  calculated_z_offset = probe.offset.z + current_position.z + z_offset_ref;

  if (LCD_HEIGHT >= 4)
    STATIC_ITEM(MSG_MOVE_NOZZLE_TO_BED, SS_CENTER|SS_INVERT);

  STATIC_ITEM_P(PSTR("Z="), SS_CENTER, ftostr42_52(current_position.z));
  STATIC_ITEM(MSG_ZPROBE_ZOFFSET, SS_LEFT, ftostr42_52(calculated_z_offset));

  SUBMENU(MSG_MOVE_1MM,  []{ _goto_manual_move_z( 1);    });
  SUBMENU(MSG_MOVE_01MM, []{ _goto_manual_move_z( 0.1f); });

  if ((SHORT_MANUAL_Z_MOVE) > 0.0f && (SHORT_MANUAL_Z_MOVE) < 0.1f) {
    char tmp[20], numstr[10];
    // Determine digits needed right of decimal
    const uint8_t digs = !UNEAR_ZERO((SHORT_MANUAL_Z_MOVE) * 1000 - int((SHORT_MANUAL_Z_MOVE) * 1000)) ? 4 :
                          !UNEAR_ZERO((SHORT_MANUAL_Z_MOVE) *  100 - int((SHORT_MANUAL_Z_MOVE) *  100)) ? 3 : 2;
    sprintf_P(tmp, GET_TEXT(MSG_MOVE_Z_DIST), dtostrf(SHORT_MANUAL_Z_MOVE, 1, digs, numstr));
    #if DISABLED(HAS_GRAPHICAL_TFT)
      extern const char NUL_STR[];
      SUBMENU_P(NUL_STR, []{ _goto_manual_move_z(float(SHORT_MANUAL_Z_MOVE)); });
      MENU_ITEM_ADDON_START(0 + ENABLED(HAS_MARLINUI_HD44780));
      lcd_put_u8str(tmp);
      MENU_ITEM_ADDON_END();
    #else
      SUBMENU_P(tmp, []{ _goto_manual_move_z(float(SHORT_MANUAL_Z_MOVE)); });
    #endif
  }

  ACTION_ITEM(MSG_BUTTON_DONE, []{
    set_offset_and_go_back(calculated_z_offset);
    do_z_clearance(20.0
      #ifdef Z_AFTER_HOMING
        - 20.0 + Z_AFTER_HOMING
      #endif
    );
    #if EITHER(Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN, USE_PROBE_FOR_Z_HOMING)
      queue.inject_P(G28_STR);
    #endif
  });

  ACTION_ITEM(MSG_BUTTON_CANCEL, []{
    set_offset_and_go_back(z_offset_backup);
  });

  END_MENU();
}


void goto_probe_offset_wizard() {
  set_all_unhomed();
  _lcd_draw_homing();

  // Store probe.offset.z for Case: Cancel
  z_offset_backup = probe.offset.z;
  
  #ifdef PROBE_OFFSET_START
    probe.offset.z = PROBE_OFFSET_START;
  #endif

  // Store Bed-Leveling-State and disable
  #if HAS_LEVELING
    leveling_was_active = planner.leveling_active;
    set_bed_leveling_enabled(false);
  #endif
  
  // Home all Axis
  home_all_axes();

  planner.synchronize();

  if (all_axes_homed()) {

    // Disable soft endstops for free Z movement
    SET_SOFT_ENDSTOP_LOOSE(true);

    // Set Z Value for Wizard Position to 0
    z_offset_ref = 0;

    #if (defined(PROBE_OFFSET_WIZARD_XY_POS) || NONE(Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN, USE_PROBE_FOR_Z_HOMING))

      #ifdef PROBE_OFFSET_WIZARD_XY_POS
        // Get X and Y from Configuration
      constexpr xy_pos_t wizard_pos = PROBE_OFFSET_WIZARD_XY_POS;
      #else
        // Set Bed Center as probe point
        constexpr xy_pos_t wizard_pos = XY_CENTER;  
      #endif

      // Probe for Z reference
      ui.wait_for_move = true;
      z_offset_ref = probe.probe_at_point(wizard_pos, PROBE_PT_STOW, 0, true);
      ui.wait_for_move = false;
      ui.synchronize();

      if (ui.wait_for_move) return;

    #endif

    // Move Nozzle to Probing/Homing Position
    ui.wait_for_move = true;
    current_position = current_position + probe.offset_xy;
    line_to_current_position(MMM_TO_MMS(HOMING_FEEDRATE_XY));
    ui.wait_for_move = false;
    ui.synchronize();

    // Go to Menu for Calibration
    if (ui.wait_for_move) return;
    ui.goto_screen(probe_offset_wizard_menu);

  }

}

#endif // PROBE_OFFSET_WIZARD
