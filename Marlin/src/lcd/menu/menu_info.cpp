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

//
// Info Menu
//

#include "../../inc/MarlinConfigPre.h"

#if BOTH(HAS_MARLINUI_MENU, LCD_INFO_MENU)

#include "menu_item.h"

#if HAS_GAMES
  #include "game/game.h"
#endif

#define VALUE_ITEM(MSG, VALUE, STYL)    do{ char msg[21]; strcpy_P(msg, PSTR(": ")); strcpy(msg + 2, VALUE); STATIC_ITEM(MSG, STYL, msg); }while(0)
#define VALUE_ITEM_F(MSG, PVALUE, STYL) do{ char msg[21]; strcpy_P(msg, PSTR(": ")); strcpy_P(msg + 2, PSTR(PVALUE)); STATIC_ITEM(MSG, STYL, msg); }while(0)

#if ENABLED(PRINTCOUNTER)

  #include "../../module/printcounter.h"

  //
  // About Printer > Printer Stats
  //
  void menu_info_stats() {
    if (ui.use_click()) return ui.go_back();

    printStatistics stats = print_job_timer.getStats();

    char buffer[21];

    START_SCREEN();                                                                         // 12345678901234567890
    VALUE_ITEM(MSG_INFO_PRINT_COUNT, i16tostr3left(stats.totalPrints), SS_LEFT);            // Print Count: 999
    VALUE_ITEM(MSG_INFO_COMPLETED_PRINTS, i16tostr3left(stats.finishedPrints), SS_LEFT);    // Completed  : 666

    STATIC_ITEM(MSG_INFO_PRINT_TIME, SS_LEFT);                                              // Total print Time:
    STATIC_ITEM_F(F("> "), SS_LEFT, duration_t(stats.printTime).toString(buffer));       // > 99y 364d 23h 59m 59s

    STATIC_ITEM(MSG_INFO_PRINT_LONGEST, SS_LEFT);                                           // Longest job time:
    STATIC_ITEM_F(F("> "), SS_LEFT, duration_t(stats.longestPrint).toString(buffer));    // > 99y 364d 23h 59m 59s

    STATIC_ITEM(MSG_INFO_PRINT_FILAMENT, SS_LEFT);                                          // Extruded total:
    sprintf_P(buffer, PSTR("%ld.%im")
      , long(stats.filamentUsed / 1000)
      , int16_t(stats.filamentUsed / 100) % 10
    );
    STATIC_ITEM_F(F("> "), SS_LEFT, buffer);                                             // > 125m

    #if SERVICE_INTERVAL_1 > 0 || SERVICE_INTERVAL_2 > 0 || SERVICE_INTERVAL_3 > 0
      strcpy_P(buffer, GET_TEXT(MSG_SERVICE_IN));
    #endif

    #if SERVICE_INTERVAL_1 > 0
      STATIC_ITEM_F(F(SERVICE_NAME_1 " "), SS_LEFT, buffer);                             // Service X in:
      STATIC_ITEM_F(F("> "), SS_LEFT, duration_t(stats.nextService1).toString(buffer));  // > 7d 12h 11m 10s
    #endif

    #if SERVICE_INTERVAL_2 > 0
      STATIC_ITEM_F(F(SERVICE_NAME_2 " "), SS_LEFT, buffer);
      STATIC_ITEM_F(F("> "), SS_LEFT, duration_t(stats.nextService2).toString(buffer));
    #endif

    #if SERVICE_INTERVAL_3 > 0
      STATIC_ITEM_F(F(SERVICE_NAME_3 " "), SS_LEFT, buffer);
      STATIC_ITEM_F(F("> "), SS_LEFT, duration_t(stats.nextService3).toString(buffer));
    #endif

    END_SCREEN();
  }

#endif

//
// About Printer > Thermistors
//
void menu_info_thermistors() {
  if (ui.use_click()) return ui.go_back();

  START_SCREEN();

  #if HAS_EXTRUDERS
    #define THERMISTOR_ID TEMP_SENSOR_0
    #include "../thermistornames.h"
    STATIC_ITEM_F(F(STR_E0 ": " THERMISTOR_NAME), SS_INVERT);
    PSTRING_ITEM(MSG_INFO_MIN_TEMP, STRINGIFY(HEATER_0_MINTEMP), SS_LEFT);
    PSTRING_ITEM(MSG_INFO_MAX_TEMP, STRINGIFY(HEATER_0_MAXTEMP), SS_LEFT);
    STATIC_ITEM(TERN(WATCH_HOTENDS, MSG_INFO_RUNAWAY_ON, MSG_INFO_RUNAWAY_OFF), SS_LEFT);
  #endif

  #if TEMP_SENSOR_1 != 0
    #undef THERMISTOR_ID
    #define THERMISTOR_ID TEMP_SENSOR_1
    #include "../thermistornames.h"
    STATIC_ITEM_F(F(STR_E1 ": " THERMISTOR_NAME), SS_INVERT);
    PSTRING_ITEM(MSG_INFO_MIN_TEMP, STRINGIFY(HEATER_1_MINTEMP), SS_LEFT);
    PSTRING_ITEM(MSG_INFO_MAX_TEMP, STRINGIFY(HEATER_1_MAXTEMP), SS_LEFT);
    STATIC_ITEM(TERN(WATCH_HOTENDS, MSG_INFO_RUNAWAY_ON, MSG_INFO_RUNAWAY_OFF), SS_LEFT);
  #endif

  #if TEMP_SENSOR_2 != 0
    #undef THERMISTOR_ID
    #define THERMISTOR_ID TEMP_SENSOR_2
    #include "../thermistornames.h"
    STATIC_ITEM_F(F(STR_E2 ": " THERMISTOR_NAME), SS_INVERT);
    PSTRING_ITEM(MSG_INFO_MIN_TEMP, STRINGIFY(HEATER_2_MINTEMP), SS_LEFT);
    PSTRING_ITEM(MSG_INFO_MAX_TEMP, STRINGIFY(HEATER_2_MAXTEMP), SS_LEFT);
    STATIC_ITEM(TERN(WATCH_HOTENDS, MSG_INFO_RUNAWAY_ON, MSG_INFO_RUNAWAY_OFF), SS_LEFT);
  #endif

  #if TEMP_SENSOR_3 != 0
    #undef THERMISTOR_ID
    #define THERMISTOR_ID TEMP_SENSOR_3
    #include "../thermistornames.h"
    STATIC_ITEM_F(F(STR_E3 ": " THERMISTOR_NAME), SS_INVERT);
    PSTRING_ITEM(MSG_INFO_MIN_TEMP, STRINGIFY(HEATER_3_MINTEMP), SS_LEFT);
    PSTRING_ITEM(MSG_INFO_MAX_TEMP, STRINGIFY(HEATER_3_MAXTEMP), SS_LEFT);
    STATIC_ITEM(TERN(WATCH_HOTENDS, MSG_INFO_RUNAWAY_ON, MSG_INFO_RUNAWAY_OFF), SS_LEFT);
  #endif

  #if TEMP_SENSOR_4 != 0
    #undef THERMISTOR_ID
    #define THERMISTOR_ID TEMP_SENSOR_4
    #include "../thermistornames.h"
    STATIC_ITEM_F(F(STR_E4 ": " THERMISTOR_NAME), SS_INVERT);
    PSTRING_ITEM(MSG_INFO_MIN_TEMP, STRINGIFY(HEATER_4_MINTEMP), SS_LEFT);
    PSTRING_ITEM(MSG_INFO_MAX_TEMP, STRINGIFY(HEATER_4_MAXTEMP), SS_LEFT);
    STATIC_ITEM(TERN(WATCH_HOTENDS, MSG_INFO_RUNAWAY_ON, MSG_INFO_RUNAWAY_OFF), SS_LEFT);
  #endif

  #if TEMP_SENSOR_5 != 0
    #undef THERMISTOR_ID
    #define THERMISTOR_ID TEMP_SENSOR_5
    #include "../thermistornames.h"
    STATIC_ITEM_F(F(STR_E5 ": " THERMISTOR_NAME), SS_INVERT);
    PSTRING_ITEM(MSG_INFO_MIN_TEMP, STRINGIFY(HEATER_5_MINTEMP), SS_LEFT);
    PSTRING_ITEM(MSG_INFO_MAX_TEMP, STRINGIFY(HEATER_5_MAXTEMP), SS_LEFT);
    STATIC_ITEM(TERN(WATCH_HOTENDS, MSG_INFO_RUNAWAY_ON, MSG_INFO_RUNAWAY_OFF), SS_LEFT);
  #endif

  #if TEMP_SENSOR_6 != 0
    #undef THERMISTOR_ID
    #define THERMISTOR_ID TEMP_SENSOR_6
    #include "../thermistornames.h"
    STATIC_ITEM_F(F(STR_E6 ": " THERMISTOR_NAME), SS_INVERT);
    PSTRING_ITEM(MSG_INFO_MIN_TEMP, STRINGIFY(HEATER_6_MINTEMP), SS_LEFT);
    PSTRING_ITEM(MSG_INFO_MAX_TEMP, STRINGIFY(HEATER_6_MAXTEMP), SS_LEFT);
    STATIC_ITEM(TERN(WATCH_HOTENDS, MSG_INFO_RUNAWAY_ON, MSG_INFO_RUNAWAY_OFF), SS_LEFT);
  #endif

  #if TEMP_SENSOR_7 != 0
    #undef THERMISTOR_ID
    #define THERMISTOR_ID TEMP_SENSOR_7
    #include "../thermistornames.h"
    STATIC_ITEM_F(F(STR_E7 ": " THERMISTOR_NAME), SS_INVERT);
    PSTRING_ITEM(MSG_INFO_MIN_TEMP, STRINGIFY(HEATER_7_MINTEMP), SS_LEFT);
    PSTRING_ITEM(MSG_INFO_MAX_TEMP, STRINGIFY(HEATER_7_MAXTEMP), SS_LEFT);
    STATIC_ITEM(TERN(WATCH_HOTENDS, MSG_INFO_RUNAWAY_ON, MSG_INFO_RUNAWAY_OFF), SS_LEFT);
  #endif

  #if HAS_HEATED_BED
    #undef THERMISTOR_ID
    #define THERMISTOR_ID TEMP_SENSOR_BED
    #include "../thermistornames.h"
    STATIC_ITEM_F(F("BED: " THERMISTOR_NAME), SS_INVERT);
    PSTRING_ITEM(MSG_INFO_MIN_TEMP, STRINGIFY(BED_MINTEMP), SS_LEFT);
    PSTRING_ITEM(MSG_INFO_MAX_TEMP, STRINGIFY(BED_MAXTEMP), SS_LEFT);
    STATIC_ITEM(TERN(WATCH_BED, MSG_INFO_RUNAWAY_ON, MSG_INFO_RUNAWAY_OFF), SS_LEFT);
  #endif

  #if HAS_HEATED_CHAMBER
    #undef THERMISTOR_ID
    #define THERMISTOR_ID TEMP_SENSOR_CHAMBER
    #include "../thermistornames.h"
    STATIC_ITEM_F(F("CHAM: " THERMISTOR_NAME), SS_INVERT);
    PSTRING_ITEM(MSG_INFO_MIN_TEMP, STRINGIFY(CHAMBER_MINTEMP), SS_LEFT);
    PSTRING_ITEM(MSG_INFO_MAX_TEMP, STRINGIFY(CHAMBER_MAXTEMP), SS_LEFT);
    STATIC_ITEM(TERN(WATCH_CHAMBER, MSG_INFO_RUNAWAY_ON, MSG_INFO_RUNAWAY_OFF), SS_LEFT);
  #endif

  #if HAS_COOLER
    #undef THERMISTOR_ID
    #define THERMISTOR_ID TEMP_SENSOR_COOLER
    #include "../thermistornames.h"
    STATIC_ITEM_F(F("COOL: " THERMISTOR_NAME), SS_INVERT);
    PSTRING_ITEM(MSG_INFO_MIN_TEMP, STRINGIFY(COOLER_MINTEMP), SS_LEFT);
    PSTRING_ITEM(MSG_INFO_MAX_TEMP, STRINGIFY(COOLER_MAXTEMP), SS_LEFT);
    STATIC_ITEM(TERN(WATCH_COOLER, MSG_INFO_RUNAWAY_ON, MSG_INFO_RUNAWAY_OFF), SS_LEFT);
  #endif

  END_SCREEN();
}

//
// About Printer > Board Info
//
void menu_info_board() {
  if (ui.use_click()) return ui.go_back();

  START_SCREEN();
  STATIC_ITEM_F(F(BOARD_INFO_NAME), SS_DEFAULT|SS_INVERT);      // MyPrinterController
  #ifdef BOARD_WEBSITE_URL
    STATIC_ITEM_F(F(BOARD_WEBSITE_URL), SS_LEFT);               // www.my3dprinter.com
  #endif
  PSTRING_ITEM(MSG_INFO_BAUDRATE, STRINGIFY(BAUDRATE), SS_CENTER); // Baud: 250000
  PSTRING_ITEM(MSG_INFO_PROTOCOL, PROTOCOL_VERSION, SS_CENTER);    // Protocol: 1.0
  PSTRING_ITEM(MSG_INFO_PSU, PSU_NAME, SS_CENTER);
  END_SCREEN();
}

//
// About Printer > Printer Info
//
#if ENABLED(LCD_PRINTER_INFO_IS_BOOTSCREEN)

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

#else

  void menu_info_printer() {
    if (ui.use_click()) return ui.go_back();
    START_SCREEN();
    STATIC_ITEM(MSG_MARLIN, SS_DEFAULT|SS_INVERT);                // Marlin
    STATIC_ITEM_F(F(SHORT_BUILD_VERSION));                        // x.x.x-Branch
    STATIC_ITEM_F(F(STRING_DISTRIBUTION_DATE));                   // YYYY-MM-DD HH:MM
    STATIC_ITEM_F(F(MACHINE_NAME), SS_DEFAULT|SS_INVERT);         // My3DPrinter
    STATIC_ITEM_F(F(WEBSITE_URL));                                // www.my3dprinter.com
    PSTRING_ITEM(MSG_INFO_EXTRUDERS, STRINGIFY(EXTRUDERS), SS_CENTER); // Extruders: 2
    #if HAS_LEVELING
      STATIC_ITEM(
        TERN_(AUTO_BED_LEVELING_3POINT, MSG_3POINT_LEVELING)      // 3-Point Leveling
        TERN_(AUTO_BED_LEVELING_LINEAR, MSG_LINEAR_LEVELING)      // Linear Leveling
        TERN_(AUTO_BED_LEVELING_BILINEAR, MSG_BILINEAR_LEVELING)  // Bi-linear Leveling
        TERN_(AUTO_BED_LEVELING_UBL, MSG_UBL_LEVELING)            // Unified Bed Leveling
        TERN_(MESH_BED_LEVELING, MSG_MESH_LEVELING)               // Mesh Leveling
      );
    #endif
    END_SCREEN();
  }

#endif

//
// "About Printer" submenu
//
void menu_info() {
  START_MENU();
  BACK_ITEM(MSG_MAIN);
  #if ENABLED(LCD_PRINTER_INFO_IS_BOOTSCREEN)
    SUBMENU(MSG_INFO_PRINTER_MENU, TERN(SHOW_CUSTOM_BOOTSCREEN, menu_show_custom_bootscreen, menu_show_marlin_bootscreen));
  #else
    SUBMENU(MSG_INFO_PRINTER_MENU, menu_info_printer);           // Printer Info >
    SUBMENU(MSG_INFO_BOARD_MENU, menu_info_board);               // Board Info >
    #if HAS_EXTRUDERS
      SUBMENU(MSG_INFO_THERMISTOR_MENU, menu_info_thermistors);  // Thermistors >
    #endif
  #endif

  #if ENABLED(PRINTCOUNTER)
    SUBMENU(MSG_INFO_STATS_MENU, menu_info_stats);               // Printer Stats >
  #endif

  #if HAS_GAMES
  {
    #if ENABLED(GAMES_EASTER_EGG)
      SKIP_ITEM(); SKIP_ITEM(); SKIP_ITEM();
    #endif

    // Game sub-menu or the individual game
    SUBMENU(
      #if HAS_GAME_MENU
        MSG_GAMES, menu_game
      #elif ENABLED(MARLIN_BRICKOUT)
        MSG_BRICKOUT, brickout.enter_game
      #elif ENABLED(MARLIN_INVADERS)
        MSG_INVADERS, invaders.enter_game
      #elif ENABLED(MARLIN_SNAKE)
        MSG_SNAKE, snake.enter_game
      #elif ENABLED(MARLIN_MAZE)
        MSG_MAZE, maze.enter_game
      #endif
    );
  }
  #endif

  END_MENU();
}

#endif // HAS_MARLINUI_MENU && LCD_INFO_MENU
