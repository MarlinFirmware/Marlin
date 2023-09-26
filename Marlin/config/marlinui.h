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

#if HAS_MARLINUI_MENU

  #if HAS_BED_PROBE
    // Add calibration in the Probe Offsets menu to compensate for X-axis twist.
    //#define X_AXIS_TWIST_COMPENSATION
    #if ENABLED(X_AXIS_TWIST_COMPENSATION)
      /**
       * Enable to init the Probe Z-Offset when starting the Wizard.
       * Use a height slightly above the estimated nozzle-to-probe Z offset.
       * For example, with an offset of -5, consider a starting height of -4.
       */
      #define XATC_START_Z 0.0
      #define XATC_MAX_POINTS 3             // Number of points to probe in the wizard
      #define XATC_Y_POSITION Y_CENTER      // (mm) Y position to probe
      #define XATC_Z_OFFSETS { 0, 0, 0 }    // Z offsets for X axis sample points
    #endif

    // Show Deploy / Stow Probe options in the Motion menu.
    #define PROBE_DEPLOY_STOW_MENU
  #endif

  // Include a page of printer information in the LCD Main Menu
  //#define LCD_INFO_MENU
  #if ENABLED(LCD_INFO_MENU)
    //#define LCD_PRINTER_INFO_IS_BOOTSCREEN // Show bootscreen(s) instead of Printer Info pages
  #endif

  // Add 50/100mm moves to MarlinUI even with a smaller bed
  //#define LARGE_MOVE_ITEMS

  // BACK menu items keep the highlight at the top
  //#define TURBO_BACK_MENU_ITEM

  // Insert a menu for preheating at the top level to allow for quick access
  //#define PREHEAT_SHORTCUT_MENU_ITEM

  // Add Configuration > Debug Menu > Endstop Test for endstop/probe/runout testing
  //#define LCD_ENDSTOP_TEST

#endif // HAS_MARLINUI_MENU
