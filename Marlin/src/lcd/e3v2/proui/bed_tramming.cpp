/**
 * PROUI Bed Tramming
 * Author: Miguel A. Risco-Castillo
 * version: 2.2.0
 * Date: 2023/09/07
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * For commercial applications additional licenses can be requested
 */

#include "../../../inc/MarlinConfigPre.h"

#if ALL(DWIN_LCD_PROUI, LCD_BED_TRAMMING)

#include "../../../core/types.h"
#include "../../../gcode/gcode.h"
#include "../../marlinui.h"

#if HAS_MESH || HAS_BED_PROBE
  #include "../../../feature/bedlevel/bedlevel.h"
  #include "../../../module/probe.h"
#endif

#include "bed_tramming.h"
#include "dwin.h"

#if defined(BED_TRAMMING_INSET_LFRB)
  constexpr uint16_t inset_lfrb[] = BED_TRAMMING_INSET_LFRB;
#endif

#if HAS_TRAMMING_WIZARD
  extern bed_mesh_t tram_zmesh;
#endif

bool tram(const uint8_t point OPTARG(HAS_BED_PROBE, bool stow_probe/*=true*/)) {
  #if defined(BED_TRAMMING_INSET_LFRB)
    uint16_t lfrb[] = { inset_lfrb[0], inset_lfrb[1], (uint16_t)(X_BED_SIZE - inset_lfrb[2]), (uint16_t)(Y_BED_SIZE - inset_lfrb[3]) };
  #elif HAS_MESH
    uint16_t lfrb[] = { MESH_MIN_X, MESH_MIN_Y, MESH_MAX_X, MESH_MAX_Y };
  #else
    uint16_t lfrb[] = { 30, 30, (uint16_t)(X_BED_SIZE - 30), (uint16_t)(Y_BED_SIZE - 30) };
  #endif
  uint16_t x = 0, y = 0;
  gcode.process_subcommands_now(F("G28O"));
  switch (point) {
    case LF: LCD_MESSAGE(MSG_TRAM_FL); x = lfrb[0]; y = lfrb[1]; break;
    case RF: LCD_MESSAGE(MSG_TRAM_FR); x = lfrb[2]; y = lfrb[1]; break;
    case LB: LCD_MESSAGE(MSG_TRAM_BL); x = lfrb[0]; y = lfrb[3]; break;
    case RB: LCD_MESSAGE(MSG_TRAM_BR); x = lfrb[2]; y = lfrb[3]; break;
    case TRAM_C : LCD_MESSAGE(MSG_TRAM_C); x = X_BED_SIZE / 2; y = Y_BED_SIZE / 2; break;
  }
  #if HAS_BED_PROBE
    if (hmiData.fullManualTramming) {
      TERN_(HAS_LEVELING, set_bed_leveling_enabled(false));
      gcode.process_subcommands_now(MString<100>(
        F("M420S0\nG90\nG0F300Z" STRINGIFY(BED_TRAMMING_Z_HOP) "\nG0F5000X"), x, 'Y', y, F("\nG0F300Z" STRINGIFY(BED_TRAMMING_HEIGHT))
      ));
      return false;
    }
    else {
      if (stow_probe) probe.stow();
      const float zval = probe_at_point(x, y, stow_probe);
      TERN_(HAS_TRAMMING_WIZARD, tram_zmesh[point % 2][point / 2] = zval);
      bool tram_error = isnan(zval);
      if (tram_error) {
        LCD_MESSAGE(MSG_M48_OUT_OF_BOUNDS);
      } else {
        ui.set_status(TS(F("X:"), x, F(" Y:"), y, F(" Z:"), p_float_t(zval, 2)));
      }
      return tram_error;
    }
  #else // !HAS_BED_PROBE
    gcode.process_subcommands_now(MString<100>(
      F("M420S0\nG28O\nG90\nG0F300Z" STRINGIFY(BED_TRAMMING_Z_HOP) "\nG0F5000X"), x, 'Y', y, F("\nG0F300Z" STRINGIFY(BED_TRAMMING_HEIGHT))
    ));
    return false;
  #endif
}

#endif // DWIN_LCD_PROUI && LCD_BED_TRAMMING
