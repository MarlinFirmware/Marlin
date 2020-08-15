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
// Advanced Settings Menus
//

#include "../../inc/MarlinConfigPre.h"

#if BOTH(HAS_LCD_MENU, PASSWORD_FEATURE)

#include "../../feature/password/password.h"

#include "menu.h"
#include "menu_addon.h"

void menu_advanced_settings();

screenFunc_t success_screen, fail_screen;
bool authenticating; // = false
char string[(PASSWORD_LENGTH) + 1];
static uint8_t digit_no;

//
// Screen for both editing and setting the password
//
void Password::menu_password_entry() {
  START_MENU();

  // "Login" or "New Code"
  STATIC_ITEM_P(authenticating ? GET_TEXT(MSG_LOGIN_REQUIRED) : GET_TEXT(MSG_EDIT_PASSWORD), SS_CENTER|SS_INVERT);

  STATIC_ITEM_P(PSTR(""), SS_CENTER|SS_INVERT, string);

  // Make the digit edit item look like a sub-menu
  PGM_P const label = GET_TEXT(MSG_ENTER_DIGIT);
  EDIT_ITEM_P(uint8, label, &editable.uint8, 0, 9, digit_entered);
  MENU_ITEM_ADDON_START(utf8_strlen_P(label) + 1);
    lcd_put_wchar(' ');
    lcd_put_wchar('1' + digit_no);
    SETCURSOR_X(LCD_WIDTH - 1);
    lcd_put_wchar('>');
  MENU_ITEM_ADDON_END();

  ACTION_ITEM(MSG_START_OVER, start_over);

  if (!authenticating) BACK_ITEM(MSG_BUTTON_CANCEL);

  END_MENU();
}

//
// Authentication check
//
void Password::authentication_done() {
  ui.goto_screen(is_locked ? fail_screen : success_screen);
  ui.completion_feedback(!is_locked);
}

// A single digit was completed
void Password::digit_entered() {
  uint32_t multiplier = CAT(1e, PASSWORD_LENGTH); // 1e5 = 100000
  LOOP_LE_N(i, digit_no) multiplier /= 10;
  value_entry += editable.uint8 * multiplier;
  string[digit_no++] = '0' + editable.uint8;

  // Exit edit screen menu and go to another screen
  ui.goto_previous_screen();
  ui.use_click();
  ui.goto_screen(menu_password_entry);

  // After password has been keyed in
  if (digit_no == PASSWORD_LENGTH) {
    if (authenticating)
      authentication_check();
    else
      set_password_done();
  }
}

//
// Set/Change Password
//
void Password::screen_password_entry() {
  value_entry = 0;
  digit_no = 0;
  editable.uint8 = 0;
  memset(string, '-', PASSWORD_LENGTH);
  string[PASSWORD_LENGTH] = '\0';
  menu_password_entry();
}

void Password::screen_set_password() {
  authenticating = false;
  screen_password_entry();
}

void Password::authenticate_user(const screenFunc_t in_succ_scr, const screenFunc_t in_fail_scr) {
  success_screen = in_succ_scr;
  fail_screen = in_fail_scr;
  if (is_set) {
    authenticating = true;
    ui.goto_screen(screen_password_entry);
    ui.defer_status_screen();
    ui.update();
  }
  else {
    ui.goto_screen(in_succ_scr);
    is_locked = false;
  }
}

void Password::access_menu_password() {
  authenticate_user(menu_password, menu_advanced_settings);
}

#if ENABLED(PASSWORD_ON_SD_PRINT_MENU)
  void Password::media_gatekeeper() {
    authenticate_user(menu_media, menu_main);
  }
#endif

void Password::start_over() {
  ui.goto_previous_screen(); // Goto previous screen, if any
  ui.goto_screen(screen_password_entry);
}

void Password::menu_password_report() {
  START_SCREEN();
  BACK_ITEM(MSG_PASSWORD_SETTINGS);
  STATIC_ITEM(MSG_PASSWORD_SET, SS_LEFT, string);
  STATIC_ITEM(MSG_REMINDER_SAVE_SETTINGS, SS_LEFT);
  END_SCREEN();
}

void Password::set_password_done() {
  is_set = true;
  value = value_entry;
  ui.completion_feedback(true);
  ui.goto_screen(menu_password_report);
}

void Password::remove_password() {
  is_set = false;
  string[0] = '0';
  string[1] = '\0';
  ui.completion_feedback(true);
  ui.goto_screen(menu_password_report);
}

//
// Password Menu
//
void Password::menu_password() {
  START_MENU();
  BACK_ITEM(MSG_ADVANCED_SETTINGS);
  SUBMENU(MSG_CHANGE_PASSWORD, screen_set_password);
  ACTION_ITEM(MSG_REMOVE_PASSWORD, []{ ui.save_previous_screen(); remove_password(); } );
  #if ENABLED(EEPROM_SETTINGS)
    ACTION_ITEM(MSG_STORE_EEPROM, ui.store_settings);
  #endif
  END_MENU();
}

#endif // HAS_LCD_MENU && PASSWORD_FEATURE
