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

#include "../../inc/MarlinConfig.h"

#if HAS_BED_PROBE

#include "../gcode.h"
#include "../../module/motion.h"
#include "../../module/probe.h"
#include "../../feature/bedlevel/bedlevel.h"

#if HAS_PTC
  #include "../../feature/probe_temp_comp.h"
#endif

#if ENABLED(DWIN_LCD_PROUI)
  #include "../../lcd/marlinui.h"
#endif

/**
 * G30: Do a single Z probe at the current XY
 *
 * Parameters:
 *
 *   X   Probe X position (default current X)
 *   Y   Probe Y position (default current Y)
 *   E   Engage the probe for each probe (default 1)
 *   C   Enable probe temperature compensation (0 or 1, default 1)
 */
void GcodeSuite::G30() {

  const xy_pos_t pos = { parser.linearval('X', current_position.x + probe.offset_xy.x),
                         parser.linearval('Y', current_position.y + probe.offset_xy.y) };

  if (!probe.can_reach(pos)) {
    #if ENABLED(DWIN_LCD_PROUI)
      SERIAL_ECHOLNF(GET_EN_TEXT_F(MSG_ZPROBE_OUT));
      LCD_MESSAGE(MSG_ZPROBE_OUT);
    #endif
    return;
  }

  // Disable leveling so the planner won't mess with us
  TERN_(HAS_LEVELING, set_bed_leveling_enabled(false));

  remember_feedrate_scaling_off();

  TERN_(DWIN_LCD_PROUI, process_subcommands_now(F("G28O")));

  const ProbePtRaise raise_after = parser.boolval('E', true) ? PROBE_PT_STOW : PROBE_PT_NONE;

  TERN_(HAS_PTC, ptc.set_enabled(!parser.seen('C') || parser.value_bool()));
  const float measured_z = probe.probe_at_point(pos, raise_after, 1);
  TERN_(HAS_PTC, ptc.set_enabled(true));
  if (!isnan(measured_z)) {
    SERIAL_ECHOLNPGM("Bed X: ", pos.x, " Y: ", pos.y, " Z: ", measured_z);
    #if ENABLED(DWIN_LCD_PROUI)
      char msg[31], str_1[6], str_2[6], str_3[6];
      sprintf_P(msg, PSTR("X:%s, Y:%s, Z:%s"),
        dtostrf(pos.x, 1, 1, str_1),
        dtostrf(pos.y, 1, 1, str_2),
        dtostrf(measured_z, 1, 2, str_3)
      );
      ui.set_status(msg);
    #endif
  }

  restore_feedrate_and_scaling();

  if (raise_after == PROBE_PT_STOW)
    probe.move_z_after_probing();

  report_current_position();
}

#endif // HAS_BED_PROBE
