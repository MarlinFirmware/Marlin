/*****************
 * screen_data.h *
 *****************/

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
 *   location: <https://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

#pragma once

#include "../ftdi_eve_lib/ftdi_eve_lib.h"

// To save RAM, store state information related to a particular screen
// in a union. The values should be initialized in the onEntry method.

struct base_numeric_adjustment_t {uint8_t increment;};

union screen_data_t {
  struct base_numeric_adjustment_t             BaseNumericAdjustmentScreen;
  struct {uint8_t volume; uint8_t brightness;} InterfaceSettingsScreen;
  struct {char passcode[5];}                   LockScreen;
  struct {bool isError;}                       AlertDialogBox;
  struct {bool auto_hide;}                     SpinnerDialogBox;
  struct {uint8_t file_index;}                 ConfirmStartPrintDialogBox;
  struct {
    uint8_t e_tag, t_tag, repeat_tag;
    ExtUI::extruder_t saved_extruder;
    #if FILAMENT_UNLOAD_PURGE_LENGTH > 0
      bool need_purge;
    #endif
  } ChangeFilamentScreen;
  struct {
    struct {
      uint8_t is_dir  : 1;
      uint8_t is_root : 1;
    } flags;
    uint8_t   selected_tag;
    uint8_t   num_page;
    uint8_t   cur_page;
    #if ENABLED(SCROLL_LONG_FILENAMES) && (FTDI_API_LEVEL >= 810)
      uint16_t  scroll_pos;
      uint16_t  scroll_max;
    #endif
  } FilesScreen;
  struct {
    struct base_numeric_adjustment_t placeholder;
    float e_rel[ExtUI::extruderCount];
  } MoveAxisScreen;
  #if HAS_MESH
    struct {
      enum : uint8_t {
        MSG_NONE,
        MSG_MESH_COMPLETE,
        MSG_MESH_INCOMPLETE
      } message;
      uint8_t count;
      uint8_t highlightedTag;
    } BedMeshScreen;
  #endif
  #if ENABLED(TOUCH_UI_DEVELOPER_MENU)
    struct {
      uint32_t next_watchdog_trigger;
      const char*  message;
    } StressTestScreen;
  #endif
  #if ENABLED(TOUCH_UI_COCOA_PRESS)
    struct {
      uint32_t start_ms;
    } PreheatTimerScreen;
  #endif
  #if ENABLED(BABYSTEPPING)
    struct {
      struct base_numeric_adjustment_t placeholder;
      xyz_int_t rel;
      #if EXTRUDERS > 1
        bool link_nozzles;
      #endif
      bool show_offsets;
    } NudgeNozzleScreen;
  #endif
};

extern screen_data_t screen_data;
