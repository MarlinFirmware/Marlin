/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../inc/MarlinConfig.h"

#if ENABLED(ULTRA_LCD) && ENABLED(LCD_SET_PROGRESS_MANUALLY)

#include "../gcode.h"
#include "../../module/parser.h"
#include "../../lcd/ultralcd.h"
#include "../../sd/cardreader.h"

/**
 * M73: Set percentage complete (for display on LCD)
 *
 * Example:
 *   M73 P25 ; Set progress to 25%
 *
 * Notes:
 *   This has no effect during an SD print job
 */
void GcodeSuite::M73() {
  if (!IS_SD_PRINTING && parser.seen('P')) {
    progress_bar_percent = parser.value_byte();
    NOMORE(progress_bar_percent, 100);
  }
}

#endif // ULTRA_LCD && LCD_SET_PROGRESS_MANUALLY
