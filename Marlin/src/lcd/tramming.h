/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2025 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#pragma once

#include "../inc/MarlinConfigPre.h"

#if ENABLED(BED_TRAMMING_USE_PROBE)
  #include "../module/probe.h"
  #ifndef BED_TRAMMING_PROBE_TOLERANCE
    #if ENABLED(DWIN_LCD_PROUI)
      #define BED_TRAMMING_PROBE_TOLERANCE 0.05f
    #endif
    #if ENABLED(DWIN_LCD_PROUI)
      #define BED_TRAMMING_PROBE_TOLERANCE 0.2f
    #endif
  #endif
  #define GOOD_POINTS_TO_STR(N) ui8tostr2(N)
  #define LAST_Z_TO_STR(N) ftostr53_63(N) //ftostr42_52(N)
#else
  constexpr float lfrb[4] = BED_TRAMMING_INSET_LFRB;
  constexpr xy_pos_t lf { (X_MIN_BED) + lfrb[0], (Y_MIN_BED) + lfrb[1] },
                     rb { (X_MAX_BED) - lfrb[2], (Y_MAX_BED) - lfrb[3] };
#endif

static_assert(BED_TRAMMING_Z_HOP >= 0, "BED_TRAMMING_Z_HOP must be >= 0. Please update your configuration.");

enum NamedPlace : uint8_t { NOPOINT = 0, LF, RF, RB, LB, FC, RC, LC, BC, CC };

#ifndef BED_TRAMMING_LEVELING_ORDER
  #define BED_TRAMMING_LEVELING_ORDER { LF, RF, LB, RB } // Default
  //#define BED_TRAMMING_LEVELING_ORDER { LF, LB, RF  }  // 3 hard-coded points
  //#define BED_TRAMMING_LEVELING_ORDER { LF, RF }       // 3-Point tramming - Rear
  //#define BED_TRAMMING_LEVELING_ORDER { LF, LB }       // 3-Point tramming - Right
  //#define BED_TRAMMING_LEVELING_ORDER { RF, RB }       // 3-Point tramming - Left
  //#define BED_TRAMMING_LEVELING_ORDER { LB, RB }       // 3-Point tramming - Front
#endif

constexpr int8_t lco[] = BED_TRAMMING_LEVELING_ORDER;
constexpr bool tramming_3_points = COUNT(lco) == 2;
static_assert(tramming_3_points || COUNT(lco) == 4, "BED_TRAMMING_LEVELING_ORDER must have exactly 2 or 4 corners.");
constexpr bool corners_are(NamedPlace a, NamedPlace b) {
  return (lco[0] == a && lco[1] == b) || (lco[0] == b && lco[1] == a);
}

constexpr int8_t lcodiff = ABS(lco[0] - lco[1]);
static_assert(COUNT(lco) == 4 || lcodiff == 1 || lcodiff == 3, "The first two BED_TRAMMING_LEVELING_ORDER corners must be on the same edge.");

constexpr int8_t nr_edge_points = tramming_3_points ? 3 : 4;
constexpr int8_t available_points = nr_edge_points + ENABLED(BED_TRAMMING_INCLUDE_CENTER);
constexpr int8_t center_index = TERN(BED_TRAMMING_INCLUDE_CENTER, available_points - 1, -1);

// Center of the opposite edge for the 3rd point
static inline xy_pos_t third_point() {
  #if ENABLED(BED_TRAMMING_USE_PROBE)
    constexpr float slop = 0.01f;
    const xy_pos_t lf = { (X_MIN_BED) + probe.min_x() + slop, (Y_MIN_BED) + probe.min_y() + slop },
                   rb = { (X_MAX_BED) - probe.max_x() - slop, (Y_MAX_BED) - probe.max_y() - slop };
  #endif
  if (corners_are(LB, RB))
    return xy_pos_t({ (lf.x + rb.x) / 2, lf.y }); // Front Center
  if (corners_are(LF, LB))
    return xy_pos_t({ rb.x, (lf.y + rb.y) / 2 }); // Right Center
  if (corners_are(RF, RB))
    return xy_pos_t({ lf.x, (lf.y + rb.y) / 2 }); // Left Center
  //if (corners_are(LF, RF))
    return xy_pos_t({ (lf.x + rb.x) / 2, rb.y }); // Back Center
}

inline static xy_pos_t tram_point_by_index(const uint8_t i) {
  #if ENABLED(BED_TRAMMING_USE_PROBE)
    constexpr float slop = 0.01f;
    const xy_pos_t lf = { (X_MIN_BED) + probe.min_x() + slop, (Y_MIN_BED) + probe.min_y() + slop },
                   rb = { (X_MAX_BED) - probe.max_x() - slop, (Y_MAX_BED) - probe.max_y() - slop };
  #endif
  if (tramming_3_points) {
    switch (i) {
      case 0 ... 1:
        // First two corners set explicitly by the configuration
        switch (lco[i]) {
          case LF: return lf;                       // Left Front
          case RF: return xy_pos_t({ rb.x, lf.y }); // Right Front
          case RB: return rb;                       // Right Back
          case LB: return xy_pos_t({ lf.x, rb.y }); // Left Back
        }
        break;

      // 3rd point is based on the other two
      case 2: return third_point();

      // 4th point is optional center
      #if ENABLED(BED_TRAMMING_INCLUDE_CENTER)
        case 3: return xy_pos_t({ X_CENTER, Y_CENTER });
      #endif
    }
  }
  else {
    // Four-Corner Bed Tramming with optional center
    if (TERN0(BED_TRAMMING_INCLUDE_CENTER, i == center_index)) {
      return xy_pos_t({ X_CENTER, Y_CENTER });
    }
    else {
      switch (lco[i]) {
        case LF: return lf;                       // Left Front
        case RF: return xy_pos_t({ rb.x, lf.y }); // Right Front
        case RB: return rb;                       // Right Back
        case LB: return xy_pos_t({ lf.x, rb.y }); // Left Back
      }
    }
  }
  return lf;
}
