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

/**
 * lcd/extui/anycubic_chiron/chiron_defs.h
 *
 * Extensible_UI implementation for Anycubic Chiron
 * Written By Nick Wells, 2020 [https://github.com/SwiftNick]
 *  (not affiliated with Anycubic, Ltd.)
 */

#include "../anycubic/common_defs.h"

// TFT panel messages
#define AC_msg_mesh_changes_abandoned F("Mesh changes abandoned, previous mesh restored.")
#define AC_msg_mesh_changes_saved     F("Mesh changes saved.")
#define AC_msg_old_panel_detected     F("Standard TFT panel detected!")
#define AC_msg_new_panel_detected     F("New TFT panel detected!")
#define AC_msg_auto_panel_detection   F("Auto detect panel type (assuming new panel)")
#define AC_msg_old_panel_set          F("Set for standard TFT panel.")
#define AC_msg_new_panel_set          F("Set for new TFT panel.")

#define AC_msg_powerloss_recovery     F("Resuming from power outage! select the same SD file then press resume")
// Error messages must not contain spaces
#define AC_msg_error_bed_temp         F("Abnormal_bed_temp")
#define AC_msg_error_hotend_temp      F("Abnormal_hotend_temp")
#define AC_msg_error_sd_card          F("SD_card_error")
#define AC_msg_filament_out           F("Filament_runout")
#define AC_msg_power_loss             F("Power_failure")
#define AC_msg_eeprom_version         F("EEPROM_ver_wrong")

#define MARLIN_msg_EEPROM_version     PSTR("EEPROM Version Error")

#define AC_Test_for_OldPanel          F("SIZE")                    // An old panel will respond with 'SXY 480 320' a new panel wont respond.
#define AC_Test_for_NewPanel          F("J200")                    // A new panel will respond with '[0]=0   [1]=0' to '[19]=0   '  an old panel wont respond

namespace Anycubic {

  enum paused_state_t : uint8_t {
    AC_paused_heater_timed_out,
    AC_paused_purging_filament,
    AC_paused_idle
  };

  enum printer_state_t : uint8_t {
    AC_printer_booting,
    AC_printer_idle,
    AC_printer_probing,
    AC_printer_printing,
    AC_printer_pausing,
    AC_printer_paused,
    AC_printer_stopping,
    AC_printer_resuming_from_power_outage
  };

  enum panel_type_t : uint8_t { // order is important here as we assume new panel if type is unknown
    AC_panel_unknown,
    AC_panel_new,
    AC_panel_standard
  };

  enum last_error_t : uint8_t {
    AC_error_none,
    AC_error_abnormal_temp_t0,
    AC_error_abnormal_temp_bed,
    AC_error_noSD,
    AC_error_powerloss,
    AC_error_filament_runout,
    AC_error_EEPROM
  };

} // Anycubic
