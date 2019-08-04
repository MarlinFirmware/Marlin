/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

//
// Info Menu
//

#include "../../inc/MarlinConfigPre.h"

#if HAS_LCD_MENU && ENABLED(LCD_INFO_MENU)

#include "menu.h"

#if HAS_GAMES
  #include "game/game.h"
#endif

#if ENABLED(PRINTCOUNTER)

  #include "../../module/printcounter.h"

  //
  // About Printer > Printer Stats
  //
  void menu_info_stats() {
    if (ui.use_click()) return ui.goto_previous_screen();

    char buffer[21];
    printStatistics stats = print_job_timer.getStats();

    START_SCREEN();                                                                                // 12345678901234567890
    STATIC_ITEM(MSG_INFO_PRINT_COUNT ": ", false, false, i16tostr3left(stats.totalPrints));        // Print Count: 999
    STATIC_ITEM(MSG_INFO_COMPLETED_PRINTS": ", false, false, i16tostr3left(stats.finishedPrints)); // Completed  : 666

    duration_t elapsed = stats.printTime;
    elapsed.toString(buffer);

    STATIC_ITEM(MSG_INFO_PRINT_TIME ":", false, false);                                            // Total print Time:
    STATIC_ITEM("> ", false, false, buffer);                                                       // > 99y 364d 23h 59m 59s

    elapsed = stats.longestPrint;
    elapsed.toString(buffer);

    STATIC_ITEM(MSG_INFO_PRINT_LONGEST ":", false, false);                                         // Longest job time:
    STATIC_ITEM("> ", false, false, buffer);                                                       // > 99y 364d 23h 59m 59s

    sprintf_P(buffer, PSTR("%ld.%im"), long(stats.filamentUsed / 1000), int16_t(stats.filamentUsed / 100) % 10);
    STATIC_ITEM(MSG_INFO_PRINT_FILAMENT ":", false, false);                                        // Extruded total:
    STATIC_ITEM("> ", false, false, buffer);                                                       // > 125m

    #if SERVICE_INTERVAL_1 > 0
      elapsed = stats.nextService1;
      elapsed.toString(buffer);
      STATIC_ITEM(SERVICE_NAME_1 MSG_SERVICE_IN, false, false);                                    // Service X in:
      STATIC_ITEM("> ", false, false, buffer);                                                     // > 7d 12h 11m 10s
    #endif
    #if SERVICE_INTERVAL_2 > 0
      elapsed = stats.nextService2;
      elapsed.toString(buffer);
      STATIC_ITEM(SERVICE_NAME_2 MSG_SERVICE_IN, false, false);
      STATIC_ITEM("> ", false, false, buffer);
    #endif
    #if SERVICE_INTERVAL_3 > 0
      elapsed = stats.nextService3;
      elapsed.toString(buffer);
      STATIC_ITEM(SERVICE_NAME_3 MSG_SERVICE_IN, false, false);
      STATIC_ITEM("> ", false, false, buffer);
    #endif

    END_SCREEN();
  }

#endif

//
// About Printer > Thermistors
//
void menu_info_thermistors() {
  if (ui.use_click()) return ui.goto_previous_screen();
  START_SCREEN();
  #define THERMISTOR_ID TEMP_SENSOR_0
  #include "../thermistornames.h"
  STATIC_ITEM("T0: " THERMISTOR_NAME, false, true);
  STATIC_ITEM(MSG_INFO_MIN_TEMP ": " STRINGIFY(HEATER_0_MINTEMP), false);
  STATIC_ITEM(MSG_INFO_MAX_TEMP ": " STRINGIFY(HEATER_0_MAXTEMP), false);

  #if TEMP_SENSOR_1 != 0
    #undef THERMISTOR_ID
    #define THERMISTOR_ID TEMP_SENSOR_1
    #include "../thermistornames.h"
    STATIC_ITEM("T1: " THERMISTOR_NAME, false, true);
    STATIC_ITEM(MSG_INFO_MIN_TEMP ": " STRINGIFY(HEATER_1_MINTEMP), false);
    STATIC_ITEM(MSG_INFO_MAX_TEMP ": " STRINGIFY(HEATER_1_MAXTEMP), false);
  #endif

  #if TEMP_SENSOR_2 != 0
    #undef THERMISTOR_ID
    #define THERMISTOR_ID TEMP_SENSOR_2
    #include "../thermistornames.h"
    STATIC_ITEM("T2: " THERMISTOR_NAME, false, true);
    STATIC_ITEM(MSG_INFO_MIN_TEMP ": " STRINGIFY(HEATER_2_MINTEMP), false);
    STATIC_ITEM(MSG_INFO_MAX_TEMP ": " STRINGIFY(HEATER_2_MAXTEMP), false);
  #endif

  #if TEMP_SENSOR_3 != 0
    #undef THERMISTOR_ID
    #define THERMISTOR_ID TEMP_SENSOR_3
    #include "../thermistornames.h"
    STATIC_ITEM("T3: " THERMISTOR_NAME, false, true);
    STATIC_ITEM(MSG_INFO_MIN_TEMP ": " STRINGIFY(HEATER_3_MINTEMP), false);
    STATIC_ITEM(MSG_INFO_MAX_TEMP ": " STRINGIFY(HEATER_3_MAXTEMP), false);
  #endif

  #if TEMP_SENSOR_4 != 0
    #undef THERMISTOR_ID
    #define THERMISTOR_ID TEMP_SENSOR_4
    #include "../thermistornames.h"
    STATIC_ITEM("T4: " THERMISTOR_NAME, false, true);
    STATIC_ITEM(MSG_INFO_MIN_TEMP ": " STRINGIFY(HEATER_4_MINTEMP), false);
    STATIC_ITEM(MSG_INFO_MAX_TEMP ": " STRINGIFY(HEATER_4_MAXTEMP), false);
  #endif

  #if TEMP_SENSOR_5 != 0
    #undef THERMISTOR_ID
    #define THERMISTOR_ID TEMP_SENSOR_5
    #include "../thermistornames.h"
    STATIC_ITEM("T5: " THERMISTOR_NAME, false, true);
    STATIC_ITEM(MSG_INFO_MIN_TEMP ": " STRINGIFY(HEATER_5_MINTEMP), false);
    STATIC_ITEM(MSG_INFO_MAX_TEMP ": " STRINGIFY(HEATER_5_MAXTEMP), false);
  #endif

  #if HAS_HEATED_BED
    #undef THERMISTOR_ID
    #define THERMISTOR_ID TEMP_SENSOR_BED
    #include "../thermistornames.h"
    STATIC_ITEM("TBed:" THERMISTOR_NAME, false, true);
    STATIC_ITEM(MSG_INFO_MIN_TEMP ": " STRINGIFY(BED_MINTEMP), false);
    STATIC_ITEM(MSG_INFO_MAX_TEMP ": " STRINGIFY(BED_MAXTEMP), false);
  #endif
  END_SCREEN();
}

//
// About Printer > Board Info
//
void menu_info_board() {
  if (ui.use_click()) return ui.goto_previous_screen();
  START_SCREEN();
  STATIC_ITEM(BOARD_NAME, true, true);                           // MyPrinterController
  STATIC_ITEM(MSG_INFO_BAUDRATE ": " STRINGIFY(BAUDRATE), true); // Baud: 250000
  STATIC_ITEM(MSG_INFO_PROTOCOL ": " PROTOCOL_VERSION, true);    // Protocol: 1.0
  STATIC_ITEM(MSG_INFO_PSU ": " PSU_NAME, true);
  END_SCREEN();
}

//
// About Printer > Printer Info
//
#if DISABLED(LCD_PRINTER_INFO_IS_BOOTSCREEN)
  void menu_info_printer() {
    if (ui.use_click()) return ui.goto_previous_screen();
    START_SCREEN();
    STATIC_ITEM(MSG_MARLIN, true, true);                             // Marlin
    STATIC_ITEM(SHORT_BUILD_VERSION, true);                          // x.x.x-Branch
    STATIC_ITEM(STRING_DISTRIBUTION_DATE, true);                     // YYYY-MM-DD HH:MM
    STATIC_ITEM(MACHINE_NAME, true);                                 // My3DPrinter
    STATIC_ITEM(WEBSITE_URL, true);                                  // www.my3dprinter.com
    STATIC_ITEM(MSG_INFO_EXTRUDERS ": " STRINGIFY(EXTRUDERS), true); // Extruders: 2
    #if ENABLED(AUTO_BED_LEVELING_3POINT)
      STATIC_ITEM(MSG_3POINT_LEVELING, true);                        // 3-Point Leveling
    #elif ENABLED(AUTO_BED_LEVELING_LINEAR)
      STATIC_ITEM(MSG_LINEAR_LEVELING, true);                        // Linear Leveling
    #elif ENABLED(AUTO_BED_LEVELING_BILINEAR)
      STATIC_ITEM(MSG_BILINEAR_LEVELING, true);                      // Bi-linear Leveling
    #elif ENABLED(AUTO_BED_LEVELING_UBL)
      STATIC_ITEM(MSG_UBL_LEVELING, true);                           // Unified Bed Leveling
    #elif ENABLED(MESH_BED_LEVELING)
      STATIC_ITEM(MSG_MESH_LEVELING, true);                          // Mesh Leveling
    #endif
    END_SCREEN();
  }
#else
  void menu_show_marlin_bootscreen() {
    if (ui.use_click()) { ui.goto_previous_screen_no_defer(); }
    ui.draw_marlin_bootscreen();
  }

  #if ENABLED(SHOW_CUSTOM_BOOTSCREEN)
    void menu_show_custom_bootscreen() {
      if (ui.use_click()) { ui.goto_screen(menu_show_marlin_bootscreen); }
      ui.draw_custom_bootscreen();
    }
  #endif
#endif // LCD_PRINTER_INFO_IS_BOOTSCREEN

//
// "About Printer" submenu
//
void menu_info() {
  START_MENU();
  MENU_BACK(MSG_MAIN);
  #if ENABLED(LCD_PRINTER_INFO_IS_BOOTSCREEN)
    MENU_ITEM(submenu, MSG_INFO_PRINTER_MENU, (
      #if ENABLED(SHOW_CUSTOM_BOOTSCREEN)
        menu_show_custom_bootscreen
      #else
        menu_show_marlin_bootscreen
      #endif
    ));
  #else
    MENU_ITEM(submenu, MSG_INFO_PRINTER_MENU, menu_info_printer);        // Printer Info >
    MENU_ITEM(submenu, MSG_INFO_BOARD_MENU, menu_info_board);            // Board Info >
    MENU_ITEM(submenu, MSG_INFO_THERMISTOR_MENU, menu_info_thermistors); // Thermistors >
  #endif

  #if ENABLED(PRINTCOUNTER)
    MENU_ITEM(submenu, MSG_INFO_STATS_MENU, menu_info_stats);          // Printer Stats >
  #endif

  #if HAS_GAMES
    #if ENABLED(GAMES_EASTER_EGG)
      MENU_ITEM_DUMMY();
      MENU_ITEM_DUMMY();
    #endif
    MENU_ITEM(submenu, MSG_GAMES, (
      #if HAS_GAME_MENU
        menu_game
      #elif ENABLED(MARLIN_BRICKOUT)
        brickout.enter_game
      #elif ENABLED(MARLIN_INVADERS)
        invaders.enter_game
      #elif ENABLED(MARLIN_SNAKE)
        snake.enter_game
      #elif ENABLED(MARLIN_MAZE)
        maze.enter_game
      #endif
    ));
  #endif

  END_MENU();
}

#endif // HAS_LCD_MENU && LCD_INFO_MENU
