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

#include "../../inc/MarlinConfig.h"

#if ENABLED(FT_MOTION)

#include "shaping.h"

// Refresh the gains used by shaping functions.
void AxisShaping::set_axis_shaping_A(
  const ftMotionShaper_t shaper,
  const float zeta
  OPTARG(HAS_FTM_EI_SHAPING, const float vtol)
) {

  const float K = exp(-zeta * M_PI / sqrt(1.f - sq(zeta))),
              K2 = sq(K),
              K3 = K2 * K,
              K4 = K3 * K;

  switch (shaper) {

    #if ENABLED(FTM_SHAPER_ZV)
      case ftMotionShaper_ZV:
        max_i = 1U;
        Ai[0] = 1.0f / (1.0f + K);
        Ai[1] = Ai[0] * K;
        break;
    #endif

    #if ENABLED(FTM_SHAPER_ZVD)
      case ftMotionShaper_ZVD:
        max_i = 2U;
        Ai[0] = 1.0f / (1.0f + 2.0f * K + K2);
        Ai[1] = Ai[0] * 2.0f * K;
        Ai[2] = Ai[0] * K2;
        break;
    #endif

    #if ENABLED(FTM_SHAPER_ZVDD)
      case ftMotionShaper_ZVDD:
        max_i = 3U;
        Ai[0] = 1.0f / (1.0f + 3.0f * K + 3.0f * K2 + K3);
        Ai[1] = Ai[0] * 3.0f * K;
        Ai[2] = Ai[0] * 3.0f * K2;
        Ai[3] = Ai[0] * K3;
        break;
    #endif

    #if ENABLED(FTM_SHAPER_ZVDDD)
      case ftMotionShaper_ZVDDD:
        max_i = 4U;
        Ai[0] = 1.0f / (1.0f + 4.0f * K + 6.0f * K2 + 4.0f * K3 + K4);
        Ai[1] = Ai[0] * 4.0f * K;
        Ai[2] = Ai[0] * 6.0f * K2;
        Ai[3] = Ai[0] * 4.0f * K3;
        Ai[4] = Ai[0] * K4;
        break;
    #endif

    #if ENABLED(FTM_SHAPER_EI)
      case ftMotionShaper_EI: {
        max_i = 2U;
        Ai[0] = 0.25f * (1.0f + vtol);
        Ai[1] = 0.50f * (1.0f - vtol) * K;
        Ai[2] = Ai[0] * K2;

        const float adj = 1.0f / (Ai[0] + Ai[1] + Ai[2]);
        for (uint32_t i = 0; i < 3U; i++) Ai[i] *= adj;
      } break;
    #endif

    #if ENABLED(FTM_SHAPER_2HEI)
      case ftMotionShaper_2HEI: {
        max_i = 3U;
        const float vtolx2 = sq(vtol);
        const float X = POW(vtolx2 * (sqrt(1.0f - vtolx2) + 1.0f), 1.0f / 3.0f);
        Ai[0] = (3.0f * sq(X) + 2.0f * X + 3.0f * vtolx2) / (16.0f * X);
        Ai[1] = (0.5f - Ai[0]) * K;
        Ai[2] = Ai[1] * K;
        Ai[3] = Ai[0] * K3;

        const float adj = 1.0f / (Ai[0] + Ai[1] + Ai[2] + Ai[3]);
        for (uint32_t i = 0; i < 4U; i++) Ai[i] *= adj;
      } break;
    #endif

    #if ENABLED(FTM_SHAPER_3HEI)
      case ftMotionShaper_3HEI: {
        max_i = 4U;
        Ai[0] = 0.0625f * ( 1.0f + 3.0f * vtol + 2.0f * sqrt( 2.0f * ( vtol + 1.0f ) * vtol ) );
        Ai[1] = 0.25f * ( 1.0f - vtol ) * K;
        Ai[2] = ( 0.5f * ( 1.0f + vtol ) - 2.0f * Ai[0] ) * K2;
        Ai[3] = Ai[1] * K2;
        Ai[4] = Ai[0] * K4;

        const float adj = 1.0f / (Ai[0] + Ai[1] + Ai[2] + Ai[3] + Ai[4]);
        for (uint32_t i = 0; i < 5U; i++) Ai[i] *= adj;
      } break;
    #endif

    #if ENABLED(FTM_SHAPER_MZV)
      case ftMotionShaper_MZV: {
        max_i = 2U;
        const float Bx = 1.4142135623730950488016887242097f * K;
        Ai[0] = 1.0f / (1.0f + Bx + K2);
        Ai[1] = Ai[0] * Bx;
        Ai[2] = Ai[0] * K2;
      }
      break;
    #endif

    default:
    case ftMotionShaper_NONE:
      max_i = 0;
      Ai[0] = 1.0f; // No echoes so the whole impulse is applied in the first tap
      break;
  }

} // set_axis_shaping_A

// Refresh the indices used by shaping functions.
// Ai[] must be precomputed (if zeta or vtol change, call set_axis_shaping_A first)
void AxisShaping::set_axis_shaping_N(const ftMotionShaper_t shaper, const float f, const float zeta) {
  // Note that protections are omitted for DBZ and for index exceeding array length.
  const float df = SQRT(1.f - sq(zeta));

  float base = 0.0f;

  switch (shaper) {

    #if ANY(FTM_SHAPER_ZV, FTM_SHAPER_ZVD, FTM_SHAPER_EI, FTM_SHAPER_ZVDD, FTM_SHAPER_2HEI, FTM_SHAPER_ZVDDD, FTM_SHAPER_3HEI)
      TERN_(FTM_SHAPER_ZV,    case ftMotionShaper_ZV:    )
      TERN_(FTM_SHAPER_ZVD,   case ftMotionShaper_ZVD:   )
      TERN_(FTM_SHAPER_EI,    case ftMotionShaper_EI:    )
      TERN_(FTM_SHAPER_ZVDD,  case ftMotionShaper_ZVDD:  )
      TERN_(FTM_SHAPER_2HEI,  case ftMotionShaper_2HEI:  )
      TERN_(FTM_SHAPER_ZVDDD, case ftMotionShaper_ZVDDD: )
      TERN_(FTM_SHAPER_3HEI,  case ftMotionShaper_3HEI:  )
        base = 0.5f;
        break;
    #endif

    #if ENABLED(FTM_SHAPER_MZV)
      case ftMotionShaper_MZV: base = 0.375f; break;
    #endif

    case ftMotionShaper_NONE:
    default:
      // No echoes. max_i already set to 0 by set_axis_shaping_A
      break;
  }

  #if HAS_FTM_SHAPING

    // Compute echo indices
    Ni[1] = LROUND((base / f / df) * FTM_FS);
    for (uint8_t i = 2; i <= max_i; ++i) Ni[i] = Ni[i - 1] + Ni[1];

    // Group delay in samples (i.e., Axis delay caused by shaping): sum(Ai * Ni[i]).
    // Skipping i=0 since the uncompensated delay of the first impulse is always zero,
    // so Ai[0] * Ni[0] == 0
    float centroid = 0.0f;
    for (uint8_t i = 1; i <= max_i; ++i) centroid -= Ai[i] * Ni[i];
    Ni[0] = LROUND(centroid);

    // The resulting echo index can be negative, this is ok because it will be offset
    // by the max delay of all axes before it is used.
    for (uint8_t i = 1; i <= max_i; ++i) Ni[i] += Ni[0];

  #endif
} // set_axis_shaping_N

#endif // FT_MOTION
