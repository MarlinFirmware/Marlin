/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

//
// Main Menu
//

#include "../../inc/MarlinConfigPre.h"

#if HAS_LCD_MENU

#include "menu.h"

void menu_tune();
void menu_basic();
#if ENABLED(SDSUPPORT)

  #include "../../sd/cardreader.h"
  #include "../../gcode/queue.h"
  #include "../../module/printcounter.h"

  #if ENABLED(POWER_LOSS_RECOVERY)
    #include "../../feature/power_loss_recovery.h"
  #endif

  void lcd_sdcard_pause() {
    #if ENABLED(POWER_LOSS_RECOVERY)
      if (recovery.enabled) recovery.save(true, false);
    #endif
    enqueue_and_echo_commands_P(PSTR("M25"));
  }

  void lcd_sdcard_resume() {
    #if ENABLED(PARK_HEAD_ON_PAUSE)
      enqueue_and_echo_commands_P(PSTR("M24"));
    #else
      card.startFileprint();
      print_job_timer.start();
      ui.reset_status();
    #endif
  }

  void lcd_sdcard_stop() {
    wait_for_heatup = wait_for_user = false;
    card.flag.abort_sd_printing = true;
    ui.set_status_P(PSTR(MSG_PRINT_ABORTED), -1);
    ui.return_to_status();
  }

#endif // SDSUPPORT
void menu_control();
void menu_info();

void menu_main() {
  START_MENU();
  MENU_BACK(MSG_WATCH);

  const bool busy = printer_busy();
  if (busy)
    MENU_ITEM(submenu, MSG_TUNE, menu_tune);
  else {
    // MENU_ITEM(submenu, MSG_MOTION, menu_motion);
    // MENU_ITEM(submenu, MSG_TEMPERATURE, menu_temperature);

  MENU_ITEM(submenu, MSG_PREPARE, menu_basic);
  #if ENABLED(SDSUPPORT)
      if (card.flag.cardOK) {
        if (card.isFileOpen()) {
          if (IS_SD_PRINTING())
            MENU_ITEM(function, MSG_PAUSE_PRINT, lcd_sdcard_pause);
          else
            MENU_ITEM(function, MSG_RESUME_PRINT, lcd_sdcard_resume);
          MENU_ITEM(function, MSG_STOP_PRINT, lcd_sdcard_stop);
        }
        else {
          MENU_ITEM(submenu, MSG_CARD_MENU, menu_sdcard);
        }
      }
  #endif // SDSUPPORT
  MENU_ITEM(submenu, MSG_CONTROL, menu_control);
  
  #if ENABLED(LCD_INFO_MENU)
    MENU_ITEM(submenu, MSG_INFO_MENU, menu_info);
  #endif
  }
  END_MENU();
}

#endif // HAS_LCD_MENU
