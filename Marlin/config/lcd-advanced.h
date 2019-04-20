/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#if ENABLED(ULTIPANEL)
  #define MANUAL_FEEDRATE {50*60, 50*60, 4*60, 60} // Feedrates for manual moves along X, Y, Z, E from panel
  #define MANUAL_E_MOVES_RELATIVE // Show LCD extruder moves as relative rather than absolute positions
  #define ULTIPANEL_FEEDMULTIPLY  // Comment to disable setting feedrate multiplier via encoder
#endif

// Change values more rapidly when the encoder is rotated faster
#define ENCODER_RATE_MULTIPLIER
#if ENABLED(ENCODER_RATE_MULTIPLIER)
  #define ENCODER_10X_STEPS_PER_SEC   30  // (steps/s) Encoder rate for 10x speed
  #define ENCODER_100X_STEPS_PER_SEC  80  // (steps/s) Encoder rate for 100x speed
#endif

// Play a beep when the feedrate is changed from the Status Screen
//#define BEEP_ON_FEEDRATE_CHANGE
#if ENABLED(BEEP_ON_FEEDRATE_CHANGE)
  #define FEEDRATE_CHANGE_BEEP_DURATION   10
  #define FEEDRATE_CHANGE_BEEP_FREQUENCY 440
#endif

// Include a page of printer information in the LCD Main Menu
//#define LCD_INFO_MENU

// Scroll a longer status message into view
//#define STATUS_MESSAGE_SCROLLING

// On the Info Screen, display XY with one decimal place when possible
//#define LCD_DECIMAL_SMALL_XY

// The timeout (in ms) to return to the status screen from sub-menus
//#define LCD_TIMEOUT_TO_STATUS 15000

// Add an 'M73' G-code to set the current percentage
//#define LCD_SET_PROGRESS_MANUALLY

#if HAS_CHARACTER_LCD && HAS_PRINT_PROGRESS
  //#define LCD_PROGRESS_BAR              // Show a progress bar on HD44780 LCDs for SD printing
  #if ENABLED(LCD_PROGRESS_BAR)
    #define PROGRESS_BAR_BAR_TIME 2000    // (ms) Amount of time to show the bar
    #define PROGRESS_BAR_MSG_TIME 3000    // (ms) Amount of time to show the status message
    #define PROGRESS_MSG_EXPIRE   0       // (ms) Amount of time to retain the status message (0=forever)
    //#define PROGRESS_MSG_ONCE           // Show the message for MSG_TIME then clear it
    //#define LCD_PROGRESS_BAR_TEST       // Add a menu item to test the progress bar
  #endif
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
#if HAS_GRAPHICAL_LCD
  // Show SD percentage next to the progress bar
  //#define DOGM_SD_PERCENT

  // Enable to save many cycles by drawing a hollow frame on the Info Screen
  #define XYZ_HOLLOW_FRAME

  // Enable to save many cycles by drawing a hollow frame on Menu Screens
  #define MENU_HOLLOW_FRAME

  // A bigger font is available for edit items. Costs 3120 bytes of PROGMEM.
  // Western only. Not available for Cyrillic, Kana, Turkish, Greek, or Chinese.
  //#define USE_BIG_EDIT_FONT

  // A smaller font may be used on the Info Screen. Costs 2300 bytes of PROGMEM.
  // Western only. Not available for Cyrillic, Kana, Turkish, Greek, or Chinese.
  //#define USE_SMALL_INFOFONT

  // Enable this option and reduce the value to optimize screen updates.
  // The normal delay is 10µs. Use the lowest value that still gives a reliable display.
  //#define DOGM_SPI_DELAY_US 5

  // Swap the CW/CCW indicators in the graphics overlay
  //#define OVERLAY_GFX_REVERSE

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
  #if ENABLED(U8GLIB_ST7920)
    //#define LIGHTWEIGHT_UI
    #if ENABLED(LIGHTWEIGHT_UI)
      #define STATUS_EXPIRE_SECONDS 20
    #endif
  #endif

  /**
   * Status (Info) Screen customizations
   * These options may affect code size and screen render time.
   * Custom status screens can forcibly override these settings.
   */
  //#define STATUS_COMBINE_HEATERS    // Use combined heater images instead of separate ones
  //#define STATUS_HOTEND_NUMBERLESS  // Use plain hotend icons instead of numbered ones (with 2+ hotends)
  #define STATUS_HOTEND_INVERTED      // Show solid nozzle bitmaps when heating (Requires STATUS_HOTEND_ANIM)
  #define STATUS_HOTEND_ANIM          // Use a second bitmap to indicate hotend heating
  #define STATUS_BED_ANIM             // Use a second bitmap to indicate bed heating
  //#define STATUS_ALT_BED_BITMAP     // Use the alternative bed bitmap
  //#define STATUS_ALT_FAN_BITMAP     // Use the alternative fan bitmap
  //#define STATUS_FAN_FRAMES 3       // :[0,1,2,3,4] Number of fan animation frames
  //#define STATUS_HEAT_PERCENT       // Show heating in a progress bar
  //#define BOOT_MARLIN_LOGO_SMALL    // Show a smaller Marlin logo on the Boot Screen (saving 399 bytes of flash)

  // Frivolous Game Options
  //#define MARLIN_BRICKOUT
  //#define MARLIN_INVADERS
  //#define MARLIN_SNAKE

#endif // HAS_GRAPHICAL_LCD
