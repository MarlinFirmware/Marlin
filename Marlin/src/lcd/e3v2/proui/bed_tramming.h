/**
 * PROUI Bed Tramming Wizard
 * Author: Miguel A. Risco-Castillo
 * version: 2.1.0
 * Date: 2023/07/18
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

#pragma once

#include "../../../inc/MarlinConfigPre.h"

#if HAS_TRAMMING_WIZARD && !ALL(DWIN_LCD_PROUI, PROUI_EX, HAS_BED_PROBE, LCD_BED_TRAMMING)
  #error "HAS_TRAMMING_WIZARD requires DWIN_LCD_PROUI, PROUI_EX, HAS_BED_PROBE and LCD_BED_TRAMMING."
#endif

// Tramming points
#define LF 0
#define RF 1
#define LB 2
#define RB 3
#define TRAM_C  4

#if HAS_TRAMMING_WIZARD
  #ifndef BED_TRAMMING_PROBE_TOLERANCE
    #define BED_TRAMMING_PROBE_TOLERANCE 0.05
  #endif
  #ifndef BED_TRAMMING_LEVELING_ORDER
    #define BED_TRAMMING_LEVELING_ORDER { LF, RF, RB, LB }  // 4 elements only LF, RF, RB and LB are allowed
  #endif
  constexpr float bed_tramming_probe_tolerance = BED_TRAMMING_PROBE_TOLERANCE;
  constexpr uint8_t bed_tramming_leveling_order[] = BED_TRAMMING_LEVELING_ORDER;
  void trammingWizard();
#endif

bool tram(uint8_t point OPTARG(HAS_BED_PROBE, bool stow_probe=true));
