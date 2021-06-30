/*************
 * screens.h *
 *************/

/****************************************************************************
 *   Written By Mark Pelletier  2017 - Aleph Objects, Inc.                  *
 *   Written By Marcio Teixeira 2018 - Aleph Objects, Inc.                  *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   To view a copy of the GNU General Public License, go to the following  *
 *   location: <https://www.gnu.org/licenses/>.                             *
 ****************************************************************************/

#pragma once


/********************************* DL CACHE SLOTS ******************************/

// In order to reduce SPI traffic, we cache display lists (DL) in RAMG. This
// is done using the CLCD::DLCache class, which takes a unique ID for each
// cache location. These IDs are defined here:

enum {
  STATUS_SCREEN_CACHE,
  MENU_SCREEN_CACHE,
  TUNE_SCREEN_CACHE,
  ALERT_BOX_CACHE,
  SPINNER_CACHE,
  ADVANCED_SETTINGS_SCREEN_CACHE,
  MOVE_AXIS_SCREEN_CACHE,
  TEMPERATURE_SCREEN_CACHE,
  STEPS_SCREEN_CACHE,
  MAX_FEEDRATE_SCREEN_CACHE,
  MAX_VELOCITY_SCREEN_CACHE,
  MAX_ACCELERATION_SCREEN_CACHE,
  DEFAULT_ACCELERATION_SCREEN_CACHE,
  FLOW_PERCENT_SCREEN_CACHE,
  LEVELING_SCREEN_CACHE,
  ZOFFSET_SCREEN_CACHE,
  BED_MESH_VIEW_SCREEN_CACHE,
  BED_MESH_EDIT_SCREEN_CACHE,
  STEPPER_CURRENT_SCREEN_CACHE,
  #if HAS_JUNCTION_DEVIATION
    JUNC_DEV_SCREEN_CACHE,
  #else
    JERK_SCREEN_CACHE,
  #endif
  CASE_LIGHT_SCREEN_CACHE,
  FILAMENT_MENU_CACHE,
  LINEAR_ADVANCE_SCREEN_CACHE,
  PREHEAT_MENU_CACHE,
  PREHEAT_TIMER_SCREEN_CACHE,
  LOAD_CHOCOLATE_SCREEN_CACHE,
  MOVE_XYZ_SCREEN_CACHE,
  MOVE_E_SCREEN_CACHE,
  FILES_SCREEN_CACHE,
  INTERFACE_SETTINGS_SCREEN_CACHE,
  INTERFACE_SOUNDS_SCREEN_CACHE,
  LOCK_SCREEN_CACHE,
  DISPLAY_TIMINGS_SCREEN_CACHE
};

// To save MCU RAM, the status message is "baked" in to the status screen
// cache, so we reserve a large chunk of memory for the DL cache

#define STATUS_SCREEN_DL_SIZE        4096
#define ALERT_BOX_DL_SIZE            3072
#define SPINNER_DL_SIZE              3072
#define FILE_SCREEN_DL_SIZE          4160
#define PRINTING_SCREEN_DL_SIZE      2048

/************************* MENU SCREEN DECLARATIONS *************************/

#include "../generic/base_screen.h"
#include "../generic/base_numeric_adjustment_screen.h"
#include "../generic/dialog_box_base_class.h"
#include "../generic/boot_screen.h"
#include "../generic/about_screen.h"
#include "../generic/kill_screen.h"
#include "../generic/alert_dialog_box.h"
#include "../generic/spinner_dialog_box.h"
#include "../generic/restore_failsafe_dialog_box.h"
#include "../generic/save_settings_dialog_box.h"
#include "../generic/confirm_start_print_dialog_box.h"
#include "../generic/confirm_abort_print_dialog_box.h"
#include "../generic/confirm_user_request_alert_box.h"
#include "../generic/touch_calibration_screen.h"
#include "../generic/move_axis_screen.h"
#include "../generic/steps_screen.h"
#include "../generic/feedrate_percent_screen.h"
#include "../generic/max_velocity_screen.h"
#include "../generic/max_acceleration_screen.h"
#include "../generic/default_acceleration_screen.h"
#include "../generic/temperature_screen.h"
#include "../generic/interface_sounds_screen.h"
#include "../generic/interface_settings_screen.h"
#include "../generic/lock_screen.h"
#include "../generic/endstop_state_screen.h"
#include "../generic/display_tuning_screen.h"
#include "../generic/statistics_screen.h"
#include "../generic/stepper_current_screen.h"
#include "../generic/leveling_menu.h"
#include "../generic/z_offset_screen.h"
#include "../generic/bed_mesh_base.h"
#include "../generic/bed_mesh_view_screen.h"
#include "../generic/bed_mesh_edit_screen.h"
#include "../generic/case_light_screen.h"
#include "../generic/linear_advance_screen.h"
#include "../generic/files_screen.h"
#include "../generic/move_axis_screen.h"
#include "../generic/flow_percent_screen.h"
#include "../generic/tune_menu.h"
#if HAS_JUNCTION_DEVIATION
  #include "../generic/junction_deviation_screen.h"
#else
  #include "../generic/jerk_screen.h"
#endif

#include "status_screen.h"
#include "main_menu.h"
#include "advanced_settings_menu.h"
#include "preheat_menu.h"
#include "preheat_screen.h"
#include "load_chocolate.h"
#include "move_xyz_screen.h"
#include "move_e_screen.h"
