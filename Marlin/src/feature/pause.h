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

/**
 * feature/pause.h - Pause feature support functions
 * This may be combined with related G-codes if features are consolidated.
 */

#ifndef _PAUSE_H_
#define _PAUSE_H_

extern bool move_away_flag;

bool pause_print(const float &retract, const float &z_lift, const float &x_pos, const float &y_pos,
                        const float &unload_length=0 , const int8_t max_beep_count=0, const bool show_lcd=false
);

void wait_for_filament_reload(const int8_t max_beep_count=0);

void resume_print(const float &load_length=0, const float &initial_extrude_length=0, const int8_t max_beep_count=0);

#endif // _PAUSE_H_
