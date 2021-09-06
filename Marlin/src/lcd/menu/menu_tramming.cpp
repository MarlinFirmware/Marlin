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
// Bed Tramming Wizard
//

#include "../../inc/MarlinConfigPre.h"

#if BOTH(HAS_LCD_MENU, ASSISTED_TRAMMING_WIZARD)

#include "menu_item.h"

#include "../../feature/tramming.h"

#include "../../module/motion.h"
#include "../../module/probe.h"
#include "../../gcode/queue.h"

//#define DEBUG_OUT 1
#include "../../core/debug_out.h"

static float z_measured[G35_PROBE_COUNT];
static bool z_isvalid[G35_PROBE_COUNT];
static uint8_t tram_index = 0;
static int8_t reference_index; // = 0

#if HAS_LEVELING
  #include "../../feature/bedlevel/bedlevel.h"
#endif

static bool probe_single_point() {
  do_blocking_move_to_z(TERN(BLTOUCH, Z_CLEARANCE_DEPLOY_PROBE, Z_CLEARANCE_BETWEEN_PROBES));
  // Stow after each point with BLTouch "HIGH SPEED" mode for push-pin safety
  const float z_probed_height = probe.probe_at_point(tramming_points[tram_index], TERN(BLTOUCH_HS_MODE, PROBE_PT_STOW, PROBE_PT_RAISE), 0, true);
  z_measured[tram_index] = z_probed_height;
  if (reference_index < 0) reference_index = tram_index;
  move_to_tramming_wait_pos();

  DEBUG_ECHOLNPAIR("probe_single_point(", tram_index, ") = ", z_probed_height, "mm");
  return (z_isvalid[tram_index] = !isnan(z_probed_height));
}

static void _menu_single_probe() {
  DEBUG_ECHOLNPAIR("Screen: single probe screen Arg:", tram_index);
  START_MENU();
  STATIC_ITEM(MSG_BED_TRAMMING, SS_LEFT);
  STATIC_ITEM(MSG_LAST_VALUE_SP, SS_LEFT, z_isvalid[tram_index] ? ftostr42_52(z_measured[reference_index] - z_measured[tram_index]) : "---");
  ACTION_ITEM(MSG_UBL_BC_INSERT2, []{ if (probe_single_point()) ui.refresh(); });
  ACTION_ITEM(MSG_BUTTON_DONE, []{ ui.goto_previous_screen(); });
  END_MENU();
}

static void tramming_wizard_menu() {
  START_MENU();
  STATIC_ITEM(MSG_SELECT_ORIGIN);

  // Draw a menu item for each tramming point
  for (tram_index = 0; tram_index < G35_PROBE_COUNT; tram_index++)
    SUBMENU_P((char*)pgm_read_ptr(&tramming_point_name[tram_index]), _menu_single_probe);

  ACTION_ITEM(MSG_BUTTON_DONE, []{
    probe.stow(); // Stow before exiting Tramming Wizard
    ui.goto_previous_screen_no_defer();
  });
  END_MENU();
}

// Init the wizard and enter the submenu
void goto_tramming_wizard() {
  DEBUG_ECHOLNPAIR("Screen: goto_tramming_wizard", 1);
  ui.defer_status_screen();

  // Initialize measured point flags
  ZERO(z_isvalid);
  reference_index = -1;

  // Inject G28, wait for homing to complete,
  set_all_unhomed();
  queue.inject_P(TERN(CAN_SET_LEVELING_AFTER_G28, PSTR("G28L0"), G28_STR));

  ui.goto_screen([]{
    _lcd_draw_homing();
    if (all_axes_homed())
      ui.goto_screen(tramming_wizard_menu);
  });
}

#endif // HAS_LCD_MENU && ASSISTED_TRAMMING_WIZARD
