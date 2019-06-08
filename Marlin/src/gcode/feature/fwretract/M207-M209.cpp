/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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
void GcodeSuite::M207() {
  if (parser.seen('S')) fwretract.settings.retract_length = parser.value_axis_units(E_AXIS);
  if (parser.seen('F')) fwretract.settings.retract_feedrate_mm_s = MMM_TO_MMS(parser.value_axis_units(E_AXIS));
  if (parser.seen('Z')) fwretract.settings.retract_zraise = parser.value_linear_units();
  if (parser.seen('W')) fwretract.settings.swap_retract_length = parser.value_axis_units(E_AXIS);
}

/**
 * M208: Set firmware un-retraction values
 *
 *   S[+units]    retract_recover_extra (in addition to M207 S*)
 *   W[+units]    swap_retract_recover_extra (multi-extruder)
 *   F[units/min] retract_recover_feedrate_mm_s
 *   R[units/min] swap_retract_recover_feedrate_mm_s
 */
void GcodeSuite::M208() {
  if (parser.seen('S')) fwretract.settings.retract_recover_extra = parser.value_axis_units(E_AXIS);
  if (parser.seen('F')) fwretract.settings.retract_recover_feedrate_mm_s = MMM_TO_MMS(parser.value_axis_units(E_AXIS));
  if (parser.seen('R')) fwretract.settings.swap_retract_recover_feedrate_mm_s = MMM_TO_MMS(parser.value_axis_units(E_AXIS));
  if (parser.seen('W')) fwretract.settings.swap_retract_recover_extra = parser.value_axis_units(E_AXIS);
}

#if ENABLED(FWRETRACT_AUTORETRACT)

  /**
   * M209: Enable automatic retract (M209 S1)
   *   For slicers that don't support G10/11, reversed extrude-only
   *   moves will be classified as retraction.
   */
  void GcodeSuite::M209() {
    if (MIN_AUTORETRACT <= MAX_AUTORETRACT && parser.seen('S'))
      fwretract.enable_autoretract(parser.value_bool());
  }

#endif // FWRETRACT_AUTORETRACT

#endif // FWRETRACT
