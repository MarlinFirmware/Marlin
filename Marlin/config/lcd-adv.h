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

#if HAS_DISPLAY

  //#define SOUND_MENU_ITEM   // Add a mute option to the LCD menu
  #define SOUND_ON_DEFAULT    // Buzzer/speaker default enabled state

  // The timeout to return to the status screen from sub-menus
  //#define LCD_TIMEOUT_TO_STATUS 15000   // (ms)

  // LCD Backlight Timeout
  //#define LCD_BACKLIGHT_TIMEOUT_MINS 1  // (minutes) Timeout before turning off the backlight

  // Scroll a longer status message into view
  //#define STATUS_MESSAGE_SCROLLING

  // Apply a timeout to low-priority status messages
  //#define STATUS_MESSAGE_TIMEOUT_SEC 30 // (seconds)

  // On the Info Screen, display XY with one decimal place when possible
  //#define LCD_DECIMAL_SMALL_XY

  // Show the E position (filament used) during printing
  //#define LCD_SHOW_E_TOTAL

  // Display a negative temperature instead of "err"
  //#define SHOW_TEMPERATURE_BELOW_ZERO

#endif // HAS_DISPLAY

#if HAS_MANUAL_MOVE_MENU
  #define MANUAL_FEEDRATE { 50*60, 50*60, 4*60, 2*60 } // (mm/min) Feedrates for manual moves along X, Y, Z, E from panel
  #define FINE_MANUAL_MOVE 0.025    // (mm) Smallest manual move (< 0.1mm) applying to Z on most machines
  #if IS_ULTIPANEL
    #define MANUAL_E_MOVES_RELATIVE // Display extruder move distance rather than "position"
    #define ULTIPANEL_FEEDMULTIPLY  // Encoder sets the feedrate multiplier on the Status Screen
  #endif
#endif

// Play a beep when the feedrate is changed from the Status Screen
//#define BEEP_ON_FEEDRATE_CHANGE
#if ENABLED(BEEP_ON_FEEDRATE_CHANGE)
  #define FEEDRATE_CHANGE_BEEP_DURATION   10
  #define FEEDRATE_CHANGE_BEEP_FREQUENCY 440
#endif

/**
 * Additional options for Graphical Displays
 *
 * Use the optimizations here to improve printing performance,
 * which can be adversely affected by graphical display drawing,
 * especially when doing several short moves, and when printing
 * on DELTA and SCARA machines.
 *
 * Some of these options may result in the display lagging behind
 * controller events, as there is a trade-off between reliable
 * printing performance versus fast display updates.
 */
#if HAS_MARLINUI_U8GLIB

  // Show the bitmap in Marlin/_Statusscreen.h on the status screen.
  //#define CUSTOM_STATUS_SCREEN_IMAGE

  // Save many cycles by drawing a hollow frame or no frame on the Info Screen
  //#define XYZ_NO_FRAME
  #define XYZ_HOLLOW_FRAME

  // A bigger font is available for edit items. Costs 3120 bytes of flash.
  // Western only. Not available for Cyrillic, Kana, Turkish, Greek, or Chinese.
  //#define USE_BIG_EDIT_FONT

  // A smaller font may be used on the Info Screen. Costs 2434 bytes of flash.
  // Western only. Not available for Cyrillic, Kana, Turkish, Greek, or Chinese.
  //#define USE_SMALL_INFOFONT

  /**
   * Graphical Display Sleep
   *
   * The U8G library provides sleep / wake functions for SH1106, SSD1306,
   * SSD1309, and some other DOGM displays.
   * Enable this option to save energy and prevent OLED pixel burn-in.
   * Adds the menu item Configuration > LCD Timeout (m) to set a wait period
   * from 0 (disabled) to 99 minutes.
   */
  //#define DISPLAY_SLEEP_MINUTES 2  // (minutes) Timeout before turning off the screen. Set with M255 S.

  /**
   * ST7920-based LCDs can emulate a 16 x 4 character display using
   * the ST7920 character-generator for very fast screen updates.
   * Enable LIGHTWEIGHT_UI to use this special display mode.
   *
   * Since LIGHTWEIGHT_UI has limited space, the position and status
   * message occupy the same line. Set STATUS_EXPIRE_SECONDS to the
   * length of time to display the status message before clearing.
   *
   * Set STATUS_EXPIRE_SECONDS to zero to never clear the status.
   * This will prevent position updates from being displayed.
   */
  #if IS_U8GLIB_ST7920
    // Enable this option and reduce the value to optimize screen updates.
    // The normal delay is 10µs. Use the lowest value that still gives a reliable display.
    //#define DOGM_SPI_DELAY_US 5

    //#define LIGHTWEIGHT_UI
    #if ENABLED(LIGHTWEIGHT_UI)
      #define STATUS_EXPIRE_SECONDS 20
    #endif
  #endif

  /**
   * Status (Info) Screen customization
   * These options may affect code size and screen render time.
   * Custom status screens can forcibly override these settings.
   */
  //#define STATUS_COMBINE_HEATERS    // Use combined heater images instead of separate ones
  //#define STATUS_HOTEND_NUMBERLESS  // Use plain hotend icons instead of numbered ones (with 2+ hotends)
  #define STATUS_HOTEND_INVERTED      // Show solid nozzle bitmaps when heating (Requires STATUS_HOTEND_ANIM for numbered hotends)
  #define STATUS_HOTEND_ANIM          // Use a second bitmap to indicate hotend heating
  #define STATUS_BED_ANIM             // Use a second bitmap to indicate bed heating
  #define STATUS_CHAMBER_ANIM         // Use a second bitmap to indicate chamber heating
  //#define STATUS_CUTTER_ANIM        // Use a second bitmap to indicate spindle / laser active
  //#define STATUS_COOLER_ANIM        // Use a second bitmap to indicate laser cooling
  //#define STATUS_FLOWMETER_ANIM     // Use multiple bitmaps to indicate coolant flow
  //#define STATUS_ALT_BED_BITMAP     // Use the alternative bed bitmap
  //#define STATUS_ALT_FAN_BITMAP     // Use the alternative fan bitmap
  //#define STATUS_FAN_FRAMES 3       // :[0,1,2,3,4] Number of fan animation frames

  // Only one STATUS_HEAT_* option can be enabled
  //#define STATUS_HEAT_PERCENT       // Show heating in a progress bar
  //#define STATUS_HEAT_POWER         // Show heater output power as a vertical bar

  // Frivolous Game Options
  //#define MARLIN_BRICKOUT
  //#define MARLIN_INVADERS
  //#define MARLIN_SNAKE
  //#define GAMES_EASTER_EGG          // Add extra blank lines above the "Games" sub-menu

#endif // HAS_MARLINUI_U8GLIB

#if HAS_MARLINUI_U8GLIB || IS_DWIN_MARLINUI
  #define MENU_HOLLOW_FRAME           // Enable to save many cycles by drawing a hollow frame on Menu Screens
  //#define OVERLAY_GFX_REVERSE       // Swap the CW/CCW indicators in the graphics overlay
#endif

//
// Additional options for DGUS / DWIN displays
//
#if HAS_DGUS_LCD
  #define LCD_BAUDRATE 115200

  #define DGUS_RX_BUFFER_SIZE 128
  #define DGUS_TX_BUFFER_SIZE 48
  //#define SERIAL_STATS_RX_BUFFER_OVERRUNS  // Fix Rx overrun situation (Currently only for AVR)

  #define DGUS_UPDATE_INTERVAL_MS  500    // (ms) Interval between automatic screen updates

  #if DGUS_UI_IS(FYSETC, MKS, HIPRECY)
    #define DGUS_PRINT_FILENAME           // Display the filename during printing
    #define DGUS_PREHEAT_UI               // Display a preheat screen during heatup

    #if DGUS_UI_IS(FYSETC, MKS)
      //#define DGUS_UI_MOVE_DIS_OPTION   // Disabled by default for FYSETC and MKS
    #else
      #define DGUS_UI_MOVE_DIS_OPTION     // Enabled by default for UI_HIPRECY
    #endif

    #define DGUS_FILAMENT_LOADUNLOAD
    #if ENABLED(DGUS_FILAMENT_LOADUNLOAD)
      #define DGUS_FILAMENT_PURGE_LENGTH 10
      #define DGUS_FILAMENT_LOAD_LENGTH_PER_TIME 0.5 // (mm) Adjust in proportion to DGUS_UPDATE_INTERVAL_MS
    #endif

    #define DGUS_UI_WAITING               // Show a "waiting" screen between some screens
    #if ENABLED(DGUS_UI_WAITING)
      #define DGUS_UI_WAITING_STATUS 10
      #define DGUS_UI_WAITING_STATUS_PERIOD 8 // Increase to slower waiting status looping
    #endif

  #elif DGUS_UI_IS(E3S1PRO)
    /**
     * The stock Ender-3 S1 Pro/Plus display firmware has rather poor SD file handling.
     *
     * The autoscroll is mainly useful for status messages, filenames, and the "About" page.
     *
     * NOTE: The Advanced SD Card option is affected by the stock touchscreen firmware, so
     *       pages 5 and up will display "4/4". This may get fixed in a screen firmware update.
     */
    #define DGUS_SOFTWARE_AUTOSCROLL        // Enable long text software auto-scroll
    #define DGUS_AUTOSCROLL_START_CYCLES 1  // Refresh cycles without scrolling at the beginning of text strings
    #define DGUS_AUTOSCROLL_END_CYCLES 1    // ... at the end of text strings

    #define DGUS_ADVANCED_SDCARD            // Allow more than 20 files and navigating directories
    #define DGUS_USERCONFIRM                // Reuse the SD Card page to show various messages
  #endif
#endif // HAS_DGUS_LCD

//
// Additional options for AnyCubic Chiron TFT displays
//
#if ENABLED(ANYCUBIC_LCD_CHIRON)
  // By default the type of panel is automatically detected.
  // Enable one of these options if you know the panel type.
  //#define CHIRON_TFT_STANDARD
  //#define CHIRON_TFT_NEW

  // Enable the longer Anycubic powerup startup tune
  //#define AC_DEFAULT_STARTUP_TUNE

  /**
   * Display Folders
   * By default the file browser lists all G-code files (including those in subfolders) in a flat list.
   * Enable this option to display a hierarchical file browser.
   *
   * NOTES:
   * - Without this option it helps to enable SDCARD_SORT_ALPHA so files are sorted before/after folders.
   * - When used with the "new" panel, folder names will also have '.gcode' appended to their names.
   *   This hack is currently required to force the panel to show folders.
   */
  #define AC_SD_FOLDER_VIEW
#endif

//
// Specify additional languages for the UI. Default specified by LCD_LANGUAGE.
//
#if ANY(DOGLCD, TFT_COLOR_UI, TOUCH_UI_FTDI_EVE, IS_DWIN_MARLINUI, ANYCUBIC_LCD_VYPER)
  //#define LCD_LANGUAGE_2 fr
  //#define LCD_LANGUAGE_3 de
  //#define LCD_LANGUAGE_4 es
  //#define LCD_LANGUAGE_5 it
  #ifdef LCD_LANGUAGE_2
    //#define LCD_LANGUAGE_AUTO_SAVE // Automatically save language to EEPROM on change
  #endif
#endif

//
// Classic UI Options
//
#if TFT_SCALED_DOGLCD
  //#define TFT_MARLINUI_COLOR 0xFFFF // White
  //#define TFT_MARLINBG_COLOR 0x0000 // Black
  //#define TFT_DISABLED_COLOR 0x0003 // Almost black
  //#define TFT_BTCANCEL_COLOR 0xF800 // Red
  //#define TFT_BTARROWS_COLOR 0xDEE6 // 11011 110111 00110 Yellow
  //#define TFT_BTOKMENU_COLOR 0x145F // 00010 100010 11111 Cyan
#endif
