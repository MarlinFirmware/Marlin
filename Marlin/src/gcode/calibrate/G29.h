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

#if ENABLED(MESH_BED_LEVELING) || ENABLED(PROBE_MANUALLY)

  #if ENABLED(PROBE_MANUALLY) && ENABLED(LCD_BED_LEVELING)
    extern bool lcd_wait_for_move;
  #endif

  inline void _manual_goto_xy(const float &x, const float &y) {
    const float old_feedrate_mm_s = feedrate_mm_s;
    #if MANUAL_PROBE_HEIGHT > 0
      const float prev_z = current_position[Z_AXIS];
      feedrate_mm_s = homing_feedrate(Z_AXIS);
      current_position[Z_AXIS] = LOGICAL_Z_POSITION(MANUAL_PROBE_HEIGHT);
      line_to_current_position();
    #endif

    feedrate_mm_s = MMM_TO_MMS(XY_PROBE_SPEED);
    current_position[X_AXIS] = LOGICAL_X_POSITION(x);
    current_position[Y_AXIS] = LOGICAL_Y_POSITION(y);
    line_to_current_position();

    #if MANUAL_PROBE_HEIGHT > 0
      feedrate_mm_s = homing_feedrate(Z_AXIS);
      current_position[Z_AXIS] = prev_z; // move back to the previous Z.
      line_to_current_position();
    #endif

    feedrate_mm_s = old_feedrate_mm_s;
    stepper.synchronize();

    #if ENABLED(PROBE_MANUALLY) && ENABLED(LCD_BED_LEVELING)
      lcd_wait_for_move = false;
    #endif
  }

#endif

#if ENABLED(MESH_BED_LEVELING)
  #include "G29-mbl.h"
#elif ENABLED(AUTO_BED_LEVELING_UBL)
  #include "G29-ubl.h"
#elif HAS_ABL
  #include "G29-abl.h"
#endif
