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

#include "../../../inc/MarlinConfig.h"

#if ENABLED(PARK_HEAD_ON_PAUSE)

#include "../../gcode.h"
#include "../../parser.h"
#include "../../../feature/pause.h"
#include "../../../module/motion.h"
#include "../../../sd/cardreader.h"
#include "../../../module/printcounter.h"

#if HAS_LCD_MENU
  #include "../../../lcd/ultralcd.h"
#endif

/**
 * M125: Store current position and move to filament change position.
 *       Called on pause (by M25) to prevent material leaking onto the
 *       object. On resume (M24) the head will be moved back and the
 *       print will resume.
 *
 *       When not actively SD printing, M125 simply moves to the park
 *       position and waits, resuming with a button click or M108.
 *       Without PARK_HEAD_ON_PAUSE the M125 command does nothing.
 *
 *    L = override retract length
 *    X = override X
 *    Y = override Y
 *    Z = override Z raise
 */
void GcodeSuite::M125() {
  // Initial retract before move to filament change position
  const float retract = -ABS(parser.seen('L') ? parser.value_axis_units(E_AXIS) : 0
    #ifdef PAUSE_PARK_RETRACT_LENGTH
      + (PAUSE_PARK_RETRACT_LENGTH)
    #endif
  );

  point_t park_point = NOZZLE_PARK_POINT;

  // Move XY axes to filament change position or given position
  if (parser.seenval('X')) park_point.x = parser.linearval('X');
  if (parser.seenval('Y')) park_point.y = parser.linearval('Y');

  // Lift Z axis
  if (parser.seenval('Z')) park_point.z = parser.linearval('Z');

  #if HAS_HOTEND_OFFSET && DISABLED(DUAL_X_CARRIAGE) && DISABLED(DELTA)
    park_point.x += (active_extruder ? hotend_offset[X_AXIS][active_extruder] : 0);
    park_point.y += (active_extruder ? hotend_offset[Y_AXIS][active_extruder] : 0);
  #endif

  #if ENABLED(SDSUPPORT)
    const bool sd_printing = IS_SD_PRINTING();
  #else
    constexpr bool sd_printing = false;
  #endif

  #if HAS_LCD_MENU
    const bool show_lcd = parser.seenval('P');
    lcd_advanced_pause_show_message(ADVANCED_PAUSE_MESSAGE_INIT, ADVANCED_PAUSE_MODE_PAUSE_PRINT, active_extruder);
  #else
    constexpr bool show_lcd = false;
  #endif

  if (pause_print(retract, park_point, 0, show_lcd)) {
    if (!sd_printing || show_lcd ) {
      wait_for_confirmation(false, 0);
      resume_print(0, 0, PAUSE_PARK_RETRACT_LENGTH, 0);
    }
  }
}

#endif // PARK_HEAD_ON_PAUSE
