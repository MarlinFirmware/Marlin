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

#if ENABLED(ASSISTED_TRAMMING_WIZARD)

#include "menu_item.h"
#include "menu_addon.h"

#include "../../module/motion.h"
#include "../../module/planner.h"
#include "../../module/probe.h"

#if HAS_LEVELING
  #include "../../feature/bedlevel/bedlevel.h"
#endif

//#define DEBUG_OUT 1
#include "../../core/debug_out.h"

constexpr xy_pos_t screws_tilt_adjust_pos[] = TRAMMING_POINT_XY;//getting tramming points xy coords

#define G35_PROBE_COUNT COUNT(screws_tilt_adjust_pos)
float z_measured[G35_PROBE_COUNT] = { 0 };
int selected_point = 0;

static PGMSTR(point_name_1, TRAMMING_POINT_NAME_1);
static PGMSTR(point_name_2, TRAMMING_POINT_NAME_2);
static PGMSTR(point_name_3, TRAMMING_POINT_NAME_3);
#ifdef TRAMMING_POINT_NAME_4
  static PGMSTR(point_name_4, TRAMMING_POINT_NAME_4);
  #ifdef TRAMMING_POINT_NAME_5
    static PGMSTR(point_name_5, TRAMMING_POINT_NAME_5);
  #endif
#endif

static PGM_P const tramming_point_name[] PROGMEM = {
  point_name_1, point_name_2, point_name_3
  #ifdef TRAMMING_POINT_NAME_4
    , point_name_4
    #ifdef TRAMMING_POINT_NAME_5
      , point_name_5
    #endif
  #endif
};

bool probe_single_point(){
  const float z_probed_height = probe.probe_at_point(screws_tilt_adjust_pos[selected_point], PROBE_PT_RAISE, 0, true);
  DEBUG_ECHOLNPAIR("Result:", z_probed_height, "mm");
  z_measured[selected_point] = z_probed_height;
  return !isnan(z_probed_height);
}

void single_probe_screen(int point) {
  selected_point = point;
  DEBUG_ECHOLNPAIR("Screen: single probe screen Arg:", point);
  START_MENU();
  STATIC_ITEM(MSG_LEVEL_CORNERS, SS_LEFT);
  STATIC_ITEM(MSG_LAST_VALUE_SP, SS_LEFT, ftostr42_52(z_measured[0] - z_measured[point])); // Print diff
  ACTION_ITEM(MSG_UBL_BC_INSERT2, []{ if (probe_single_point()) ui.refresh(); });
  ACTION_ITEM(MSG_BUTTON_DONE, []{ ui.goto_previous_screen_no_defer(); }); // Back
  END_MENU();
}

void tramming_wizard_menu() {
  START_MENU();
  DEBUG_ECHOLNPAIR("Screen: tramming_wizard_menu");
  STATIC_ITEM(MSG_SELECT_ORIGIN);

  // A Menu Item for each point
  SUBMENU_P(tramming_point_name[0], []{ single_probe_screen(0); });   // Probe P1 - Goto Probing screen
  SUBMENU_P(tramming_point_name[1], []{ single_probe_screen(1); });   // Probe P2
  SUBMENU_P(tramming_point_name[2], []{ single_probe_screen(2); });   // Probe P3
  #ifdef TRAMMING_POINT_NAME_4
    SUBMENU_P(tramming_point_name[3], []{ single_probe_screen(3); }); // Probe P4, only on core if defined
    #ifdef TRAMMING_POINT_NAME_5
      SUBMENU(tramming_point_name[4], []{ single_probe_screen(4); });
    #endif
  #endif
  ACTION_ITEM(MSG_BUTTON_DONE, []{ ui.goto_previous_screen_no_defer(); });
  END_MENU();
}

// Init the wizard and enter the submenu
void goto_tramming_wizard() {
  DEBUG_ECHOLNPAIR("Screen: goto_tramming_wizard", 1);
  selected_point = 0;
  ui.defer_status_screen();
  //probe_single_point(); // Probe first point to get differences

  // Inject G28, wait for homing to complete,
  set_all_unhomed();
  queue.inject_P(G28_STR);
  ui.goto_screen([]{
    _lcd_draw_homing();
    if (all_axes_homed())
      ui.goto_screen(tramming_wizard_menu);
  });
}

#endif // ASSISTED_TRAMMING_WIZARD
