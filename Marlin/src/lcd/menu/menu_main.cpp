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
#include "../../module/temperature.h"
#include "../../gcode/queue.h"
#include "../../module/printcounter.h"
#include "../../module/stepper.h"

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../feature/power_loss_recovery.h"
#endif

#if ENABLED(SDSUPPORT)
  #include "../../sd/cardreader.h"
#endif

void lcd_pause() {
  #if ENABLED(POWER_LOSS_RECOVERY)
    if (recovery.enabled) recovery.save(true, false);
  #endif

  #if ENABLED(PARK_HEAD_ON_PAUSE)
    lcd_advanced_pause_show_message(ADVANCED_PAUSE_MESSAGE_INIT, ADVANCED_PAUSE_MODE_PAUSE_PRINT);  // Show message immediately to let user know about pause in progress
    enqueue_and_echo_commands_P(PSTR("M25 P\nM24"));
  #elif ENABLED(SDSUPPORT)
    enqueue_and_echo_commands_P(PSTR("M25"));
  #elif defined(ACTION_ON_PAUSE)
    host_action_pause();
  #endif
  planner.synchronize();
}

void lcd_resume() {
  #if ENABLED(SDSUPPORT)
    if (card.isPaused()) enqueue_and_echo_commands_P(PSTR("M24"));
  #endif
  #ifdef ACTION_ON_RESUME
    host_action_resume();
  #endif
}

void lcd_stop() {
  #if ENABLED(SDSUPPORT)
    wait_for_heatup = wait_for_user = false;
    card.flag.abort_sd_printing = true;
  #endif
  #ifdef ACTION_ON_CANCEL
    host_action_cancel();
  #endif
  ui.set_status_P(PSTR(MSG_PRINT_ABORTED), -1);
  ui.return_to_status();
}

void menu_abort_confirm() {
  START_MENU();
  MENU_BACK(MSG_MAIN);
  MENU_ITEM(function, MSG_STOP_PRINT, lcd_stop);
  END_MENU();
}

#if ENABLED(PRUSA_MMU2)
  #include "../../lcd/menu/menu_mmu2.h"
#endif

void menu_tune();
void menu_motion();
void menu_temperature();
void menu_configuration();
void menu_user();
void menu_temp_e0_filament_change();
void menu_change_filament();
void menu_info();
void menu_led();

#if ENABLED(MIXING_EXTRUDER)
  void menu_mixer();
#endif

void menu_main() {
  START_MENU();
  MENU_BACK(MSG_WATCH);

  const bool busy = printer_busy()
    #if ENABLED(SDSUPPORT)
      , card_detected = card.isDetected()
      , card_open = card_detected && card.isFileOpen()
    #endif
  ;

  if (busy) {
    MENU_ITEM(function, MSG_PAUSE_PRINT, lcd_pause);
    #if ENABLED(SDSUPPORT) || defined(ACTION_ON_CANCEL)
      MENU_ITEM(submenu, MSG_STOP_PRINT, menu_abort_confirm);
    #endif
    MENU_ITEM(submenu, MSG_TUNE, menu_tune);
  }
  else {
    #if !HAS_ENCODER_WHEEL && ENABLED(SDSUPPORT)
      //
      // Autostart
      //
      #if ENABLED(MENU_ADDAUTOSTART)
        if (!busy) MENU_ITEM(function, MSG_AUTOSTART, card.beginautostart);
      #endif

      if (card_detected) {
        if (!card_open) {
          MENU_ITEM(submenu, MSG_CARD_MENU, menu_sdcard);
          #if !PIN_EXISTS(SD_DETECT)
            MENU_ITEM(gcode, MSG_CHANGE_SDCARD, PSTR("M21"));  // SD-card changed by user
          #endif
        }
      }
      else {
        #if !PIN_EXISTS(SD_DETECT)
          MENU_ITEM(gcode, MSG_INIT_SDCARD, PSTR("M21")); // Manually init SD-card
        #endif
        MENU_ITEM(function, MSG_NO_CARD, NULL);
      }
    #endif // !HAS_ENCODER_WHEEL && SDSUPPORT

    #if ENABLED(SDSUPPORT) || defined(ACTION_ON_RESUME)
      #if ENABLED(SDSUPPORT)
        if (card.isFileOpen() && card.isPaused())
      #endif
          MENU_ITEM(function, MSG_RESUME_PRINT, lcd_resume);
    #endif

    MENU_ITEM(submenu, MSG_MOTION, menu_motion);
  }

  MENU_ITEM(submenu, MSG_TEMPERATURE, menu_temperature);

  #if ENABLED(MIXING_EXTRUDER)
    MENU_ITEM(submenu, MSG_MIXER, menu_mixer);
  #endif

  #if ENABLED(MMU2_MENUS)
    if (!busy) MENU_ITEM(submenu, MSG_MMU2_MENU, menu_mmu2);
  #endif

  MENU_ITEM(submenu, MSG_CONFIGURATION, menu_configuration);

  #if ENABLED(CUSTOM_USER_MENUS)
    MENU_ITEM(submenu, MSG_USER_MENU, menu_user);
  #endif

  #if ENABLED(ADVANCED_PAUSE_FEATURE)
    #if E_STEPPERS == 1 && DISABLED(FILAMENT_LOAD_UNLOAD_GCODES)
      if (thermalManager.targetHotEnoughToExtrude(active_extruder))
        MENU_ITEM(gcode, MSG_FILAMENTCHANGE, PSTR("M600 B0"));
      else
        MENU_ITEM(submenu, MSG_FILAMENTCHANGE, menu_temp_e0_filament_change);
    #else
      MENU_ITEM(submenu, MSG_FILAMENTCHANGE, menu_change_filament);
    #endif
  #endif

  #if ENABLED(LCD_INFO_MENU)
    MENU_ITEM(submenu, MSG_INFO_MENU, menu_info);
  #endif

  #if ENABLED(LED_CONTROL_MENU)
    MENU_ITEM(submenu, MSG_LED_CONTROL, menu_led);
  #endif

  //
  // Switch power on/off
  //
  #if HAS_POWER_SWITCH
    if (powersupply_on)
      MENU_ITEM(gcode, MSG_SWITCH_PS_OFF, PSTR("M81"));
    else
      MENU_ITEM(gcode, MSG_SWITCH_PS_ON, PSTR("M80"));
  #endif

  #if HAS_ENCODER_WHEEL && ENABLED(SDSUPPORT)
    //
    // Autostart
    //
    #if ENABLED(MENU_ADDAUTOSTART)
      if (!busy) MENU_ITEM(function, MSG_AUTOSTART, card.beginautostart);
    #endif

    if (card_detected) {
      if (!card_open) {
        MENU_ITEM(submenu, MSG_CARD_MENU, menu_sdcard);
        #if !PIN_EXISTS(SD_DETECT)
          MENU_ITEM(gcode, MSG_CHANGE_SDCARD, PSTR("M21"));  // SD-card changed by user
        #endif
      }
    }
    else {
      #if !PIN_EXISTS(SD_DETECT)
        MENU_ITEM(gcode, MSG_INIT_SDCARD, PSTR("M21")); // Manually init SD-card
      #endif
      MENU_ITEM(function, MSG_NO_CARD, NULL);
    }
  #endif // HAS_ENCODER_WHEEL && SDSUPPORT

  END_MENU();
}

#endif // HAS_LCD_MENU
