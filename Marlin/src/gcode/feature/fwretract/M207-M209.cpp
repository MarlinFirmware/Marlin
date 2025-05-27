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

#include "../../../inc/MarlinConfig.h"

#if ENABLED(FWRETRACT)

#include "../../../feature/fwretract.h"
#include "../../gcode.h"

/**
 * M207: Set firmware retraction values
 *
 *   S[+units]    retract_length
 *   W[+units]    swap_retract_length (multi-extruder)
 *   F[units/min] retract_feedrate_mm_s
 *   Z[units]     retract_zraise
 */
void GcodeSuite::M207() { fwretract.M207(); }

void GcodeSuite::M207_report(const bool forReplay/*=true*/) {
  TERN_(MARLIN_SMALL_BUILD, return);

  report_heading_etc(forReplay, F(STR_RETRACT_S_F_Z));
  fwretract.M207_report();
}

/**
 * M208: Set firmware un-retraction values
 *
 *   S[+units]    retract_recover_extra (in addition to M207 S*)
 *   W[+units]    swap_retract_recover_extra (multi-extruder)
 *   F[units/min] retract_recover_feedrate_mm_s
 *   R[units/min] swap_retract_recover_feedrate_mm_s
 */
void GcodeSuite::M208() { fwretract.M208(); }

void GcodeSuite::M208_report(const bool forReplay/*=true*/) {
  TERN_(MARLIN_SMALL_BUILD, return);

  report_heading_etc(forReplay, F(STR_RECOVER_S_F));
  fwretract.M208_report();
}

#if ENABLED(FWRETRACT_AUTORETRACT)

  /**
   * M209: Enable automatic retract (M209 S1)
   *
   *   For slicers that don't support G10/11, reversed
   *   extruder-only moves can be classified as retraction.
   */
  void GcodeSuite::M209() { fwretract.M209(); }

  void GcodeSuite::M209_report(const bool forReplay/*=true*/) {
    TERN_(MARLIN_SMALL_BUILD, return);

    report_heading_etc(forReplay, F(STR_AUTO_RETRACT_S));
    fwretract.M209_report();
  }

#endif

#endif // FWRETRACT
