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

/**
 * lcd/extui/lib/chiron_defs.h
 *
 * Extensible_UI implementation for Anycubic Chiron
 * Written By Nick Wells, 2020 [https://github.com/SwiftNick]
 *  (not affiliated with Anycubic, Ltd.)
 */

#pragma once
#include "../../../../inc/MarlinConfigPre.h"
//#define ACDEBUGLEVEL 255

#if ACDEBUGLEVEL
  // Bit-masks for selective debug:
  enum ACDebugMask : uint8_t {
    AC_INFO   =  1,
    AC_ACTION =  2,
    AC_FILE   =  4,
    AC_PANEL  =  8,
    AC_MARLIN = 16,
    AC_SOME   = 32,
    AC_ALL    = 64
  };
  #define ACDEBUG(mask) ( ((mask) & ACDEBUGLEVEL) == mask )  // Debug flag macro
#else
  #define ACDEBUG(mask) false
#endif

#define TFTSer LCD_SERIAL                    // Serial interface for TFT panel now uses marlinserial
#define MAX_FOLDER_DEPTH                4    // Limit folder depth TFT has a limit for the file path
#define MAX_CMND_LEN                   16 * MAX_FOLDER_DEPTH // Maximum Length for a Panel command
#define MAX_PATH_LEN                   16 * MAX_FOLDER_DEPTH // Maximum number of characters in a SD file path

#define AC_HEATER_FAULT_VALIDATION_TIME 5    // number of 1/2 second loops before signalling a heater fault
#define AC_LOWEST_MESHPOINT_VAL        Z_PROBE_LOW_POINT // The lowest value you can set for a single mesh point offset

 // TFT panel commands
#define  AC_msg_sd_card_inserted       PSTR("J00")
#define  AC_msg_sd_card_removed        PSTR("J01")
#define  AC_msg_no_sd_card             PSTR("J02")
#define  AC_msg_usb_connected          PSTR("J03")
#define  AC_msg_print_from_sd_card     PSTR("J04")
#define  AC_msg_pause                  PSTR("J05")
#define  AC_msg_nozzle_heating         PSTR("J06")
#define  AC_msg_nozzle_heating_done    PSTR("J07")
#define  AC_msg_bed_heating            PSTR("J08")
#define  AC_msg_bed_heating_done       PSTR("J09")
#define  AC_msg_nozzle_temp_abnormal   PSTR("J10")
#define  AC_msg_kill_lcd               PSTR("J11")
#define  AC_msg_ready                  PSTR("J12")
#define  AC_msg_low_nozzle_temp        PSTR("J13")
#define  AC_msg_print_complete         PSTR("J14")
#define  AC_msg_filament_out_alert     PSTR("J15")
#define  AC_msg_stop                   PSTR("J16")
#define  AC_msg_main_board_has_reset   PSTR("J17")
#define  AC_msg_paused                 PSTR("J18")
#define  AC_msg_j19_unknown            PSTR("J19")
#define  AC_msg_sd_file_open_success   PSTR("J20")
#define  AC_msg_sd_file_open_failed    PSTR("J21")
#define  AC_msg_level_monitor_finished PSTR("J22")
#define  AC_msg_filament_out_block     PSTR("J23")
#define  AC_msg_probing_not_allowed    PSTR("J24")
#define  AC_msg_probing_complete       PSTR("J25")
#define  AC_msg_start_probing          PSTR("J26")
#define  AC_msg_version                PSTR("J27")

#define MARLIN_msg_start_probing       PSTR("Probing Point 1/25")
#define MARLIN_msg_probing_failed      PSTR("Probing Failed")
#define MARLIN_msg_ready               PSTR(" Ready.")
#define MARLIN_msg_print_paused        PSTR("Print Paused")
#define MARLIN_msg_print_aborted       PSTR("Print Aborted")
#define MARLIN_msg_extruder_heating    PSTR("E Heating...")
#define MARLIN_msg_bed_heating         PSTR("Bed Heating...")

#define MARLIN_msg_nozzle_parked       PSTR("Nozzle Parked")
#define MARLIN_msg_heater_timeout      PSTR("Heater Timeout")
#define MARLIN_msg_reheating           PSTR("Reheating...")
#define MARLIN_msg_reheat_done         PSTR("Reheat finished.")
#define MARLIN_msg_filament_purging    PSTR("Filament Purging...")
#define MARLIN_msg_special_pause       PSTR("PB")
#define AC_cmnd_auto_unload_filament   PSTR("M701")                    // Use Marlin unload routine
#define AC_cmnd_auto_load_filament     PSTR("M702 M0 PB")              // Use Marlin load routing then pause for user to clean nozzle

#define AC_cmnd_manual_load_filament   PSTR("M83\nG1 E50 F700\nM82")   // replace the manual panel commands with something a little faster
#define AC_cmnd_manual_unload_filament PSTR("M83\nG1 E-50 F1200\nM82")
#define AC_cmnd_enable_levelling       PSTR("M420 S1 V1")
#define AC_cmnd_power_loss_recovery    PSTR("G28 X Y R5\nG28 Z")       // Lift, home X and Y then home Z when in 'safe' position

namespace Anycubic {
  enum heater_state_t : uint8_t {
    AC_heater_off,
    AC_heater_temp_set,
    AC_heater_temp_reached
  };

  enum paused_state_t : uint8_t {
    AC_paused_heater_timed_out,
    AC_paused_purging_filament,
    AC_paused_idle
  };

  enum printer_state_t : uint8_t {
    AC_printer_idle,
    AC_printer_probing,
    AC_printer_printing,
    AC_printer_pausing,
    AC_printer_paused,
    AC_printer_stopping,
    AC_printer_resuming_from_power_outage
  };

  enum timer_event_t : uint8_t {
    AC_timer_started,
    AC_timer_paused,
    AC_timer_stopped
  };

  enum media_event_t : uint8_t {
    AC_media_inserted,
    AC_media_removed,
    AC_media_error
  };
  enum file_menu_t : uint8_t {
    AC_menu_file,
    AC_menu_command,
    AC_menu_change_to_file,
    AC_menu_change_to_command
  };
}
