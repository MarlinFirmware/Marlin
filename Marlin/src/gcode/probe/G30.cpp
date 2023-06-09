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
#include "../../lcd/marlinui.h"

#if ENABLED(E3S1PRO_RTS)
  #include "../../lcd/rts/e3s1pro/lcd_rts.h"
#endif

#if HAS_PTC
  #include "../../feature/probe_temp_comp.h"
#endif

#if HAS_MULTI_HOTEND
  #include "../../module/tool_change.h"
#endif

/**
 * G30: Do a single Z probe at the given XY (default: current)
 *
 * Parameters:
 *
 *   X   Probe X position (default current X)
 *   Y   Probe Y position (default current Y)
 *   E   Engage the probe for each probe (default 1)
 *   C   Enable probe temperature compensation (0 or 1, default 1)
 */
void GcodeSuite::G30() {

  xy_pos_t old_pos = current_position,
           probepos = current_position;

  const bool seenX = parser.seenval('X');
  if (seenX) probepos.x = RAW_X_POSITION(parser.value_linear_units());
  const bool seenY = parser.seenval('Y');
  if (seenY) probepos.y = RAW_Y_POSITION(parser.value_linear_units());

  probe.use_probing_tool();

  if (probe.can_reach(probepos)) {

    if (seenX) old_pos.x = probepos.x;
    if (seenY) old_pos.y = probepos.y;

    // Disable leveling so the planner won't mess with us
    TERN_(HAS_LEVELING, set_bed_leveling_enabled(false));

    remember_feedrate_scaling_off();

    TERN_(DWIN_CREALITY_LCD_JYERSUI, process_subcommands_now(F("G28O")));

    const ProbePtRaise raise_after = parser.boolval('E', true) ? PROBE_PT_STOW : PROBE_PT_NONE;

    TERN_(HAS_PTC, ptc.set_enabled(!parser.seen('C') || parser.value_bool()));
    const float measured_z = probe.probe_at_point(probepos, raise_after);
    TERN_(HAS_PTC, ptc.set_enabled(true));
    if (!isnan(measured_z)) {
      SERIAL_ECHOLNPGM("Bed X: ", probepos.asLogical().x, " Y: ", probepos.asLogical().y, " Z: ", measured_z);
      #if ANY(DWIN_LCD_PROUI, DWIN_CREALITY_LCD_JYERSUI, E3S1PRO_RTS)
        char msg[31], str_1[6], str_2[6], str_3[7];
        sprintf_P(msg, PSTR("X:%s, Y:%s, Z:%s"),
          dtostrf(probepos.x, 1, 1, str_1),
          dtostrf(probepos.y, 1, 1, str_2),
          dtostrf(measured_z, 1, 3, str_3)
        );
        ui.set_status(msg);
      #endif
      #if ENABLED(E3S1PRO_RTS)
        struct TrammingPoint {
          float x;
          float y;
          int vp;
        };

        TrammingPoint trammingPoints[] = {
          {117.50, 117.50, CRTOUCH_TRAMMING_POINT_1_VP},
          {155.00, 157.50, CRTOUCH_TRAMMING_POINT_1_VP},
          {45.00, 45.00, CRTOUCH_TRAMMING_POINT_6_VP},
          {190.00, 45.00, CRTOUCH_TRAMMING_POINT_7_VP},
          {265.00, 45.00, CRTOUCH_TRAMMING_POINT_7_VP},
          {45.00, 190.00, CRTOUCH_TRAMMING_POINT_8_VP},
          {45.00, 270.00, CRTOUCH_TRAMMING_POINT_8_VP},
          {190.00, 190.00, CRTOUCH_TRAMMING_POINT_9_VP},
          {265.00, 270.00, CRTOUCH_TRAMMING_POINT_9_VP}
        };

        for (const auto& point : trammingPoints) {
          if (probepos.x == point.x && probepos.y == point.y) {
            rts.sendData(measured_z * 1000, point.vp);
          }
        }
      #endif

    }

    restore_feedrate_and_scaling();

    do_blocking_move_to(old_pos);

    if (raise_after == PROBE_PT_STOW)
      probe.move_z_after_probing();

    report_current_position();
  }
  else {
    SERIAL_ECHOLNF(GET_EN_TEXT_F(MSG_ZPROBE_OUT));
    LCD_MESSAGE(MSG_ZPROBE_OUT);
  }

  probe.use_probing_tool(false);
}

#endif // HAS_BED_PROBE
