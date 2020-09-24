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

#pragma once

#include <stdint.h>

extern bool home_flag;
extern bool AutohomeZflag;

// Facade for touch LCD 

void DWINTouch_inactivity_callback();
void DWINTouch_refresh();
void DWINTouch_init();
void DWINTouch_bedlevel_update_callback(uint8_t count);
void DWINTouch_bedlevel_finish_callback();
void DWINTouch_autohome_callback();
void DWINTouch_autohome_update_callback();
void DWINTouch_autohome_complete_callback();
bool DWINTouch_autohome_is_lcd_ready();
void DWINTouch_print_completed_callback();
void DWINTouch_error_home_failed();
void DWINTouch_error_probe_failed();
void DWINTouch_temperature_refresh();
void DWINTouch_error_max_temp();
void DWINTouch_error_min_temp();
void DWINTouch_error_runaway_temp();
void DWINTouch_heating_callback();
void DWINTouch_notify_filament_runout();
void DWINTouch_filament_loaded();
void DWINTouch_user_confirm_required();