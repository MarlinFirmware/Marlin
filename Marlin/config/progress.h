/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
CONFIG_VERSION(02020000)

// Add 'M73' to set print job progress, overrides Marlin's built-in estimate
//#define SET_PROGRESS_MANUALLY
#if ENABLED(SET_PROGRESS_MANUALLY)
  #define SET_PROGRESS_PERCENT            // Add 'P' parameter to set percentage done
  #define SET_REMAINING_TIME              // Add 'R' parameter to set remaining time
  //#define SET_INTERACTION_TIME          // Add 'C' parameter to set time until next filament change or other user interaction
  //#define M73_REPORT                    // Report M73 values to host
  #if ALL(M73_REPORT, HAS_MEDIA)
    #define M73_REPORT_SD_ONLY            // Report only when printing from SD
  #endif
#endif

// LCD Print Progress options. Multiple times may be displayed in turn.
#if HAS_DISPLAY && ANY(HAS_MEDIA, SET_PROGRESS_MANUALLY)
  #define SHOW_PROGRESS_PERCENT           // Show print progress percentage (doesn't affect progress bar)
  #define SHOW_ELAPSED_TIME               // Display elapsed printing time (prefix 'E')
  //#define SHOW_REMAINING_TIME           // Display estimated time to completion (prefix 'R')
  #if ENABLED(SET_INTERACTION_TIME)
    #define SHOW_INTERACTION_TIME         // Display time until next user interaction ('C' = filament change)
  #endif
  //#define PRINT_PROGRESS_SHOW_DECIMALS  // Show/report progress with decimal digits, not all UIs support this

  #if ANY(HAS_MARLINUI_HD44780, IS_TFTGLCD_PANEL)
    //#define LCD_PROGRESS_BAR            // Show a progress bar on HD44780 LCDs for SD printing
    #if ENABLED(LCD_PROGRESS_BAR)
      #define PROGRESS_BAR_BAR_TIME 2000  // (ms) Amount of time to show the bar
      #define PROGRESS_BAR_MSG_TIME 3000  // (ms) Amount of time to show the status message
      #define PROGRESS_MSG_EXPIRE      0  // (ms) Amount of time to retain the status message (0=forever)
      //#define PROGRESS_MSG_ONCE         // Show the message for MSG_TIME then clear it
      //#define LCD_PROGRESS_BAR_TEST     // Add a menu item to test the progress bar
    #endif
  #endif
#endif
