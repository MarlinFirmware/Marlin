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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "../inc/MarlinConfigPre.h"

#if ENABLED(PASSWORD_FEATURE)

#include "password.h"
#include "../lcd/menu/menu.h"

Password password;

uint32_t     Password::value, Password::value_keyed_in;
bool         Password::is_set;
char         Password::string[INCREMENT(PASSWORD_LENGTH)];
uint8_t      Password::digit, Password::digit_no;
screenFunc_t Password::return_fn,
             Password::success_fn,
             Password::fail_fn;

//
// Authenticate user with password
//
void Password::authenticate_user_persistent() {
  success_fn = ui.status_screen;
  fail_fn = screen_password_entry;
  authenticate_user();
}

void Password::authenticate_user() {
  if (is_set) {
    return_fn = authenticate_user_return;
    ui.goto_screen(screen_password_entry);
    ui.defer_status_screen();
    ui.update();
  }
  else
    ui.goto_screen(success_fn);
}

void Password::authenticate_user_return() {
  if (value_keyed_in == value)
    ui.goto_screen(success_fn);
  else {
    ui.buzz(200,600);
    ui.buzz(0,0);
    ui.goto_screen(fail_fn);
  }
}

void Password::menu_media() {
  success_fn = menu_media;
  fail_fn = menu_main;
  authenticate_user();
}

//
// Password entry screens
//
void Password::screen_password_entry() {
  value_keyed_in = 0;
  digit_no = PASSWORD_LENGTH;
  digit = 0;
  LOOP_L_N(i, PASSWORD_LENGTH) password_string[i] = '*';
  password_string[PASSWORD_LENGTH] = '\0';
  menu_password_entry();
}

void Password::menu_password_entry() {
  START_MENU();
  STATIC_ITEM(MSG_PASSWORD, SS_LEFT, &password_string[0]);
  EDIT_ITEM_FAST(uint8, MSG_ENTER_DIGIT, &digit, 0, 9, digit_entered, false);
  SUBMENU(MSG_CLEAR, clear);
  END_MENU();
}

void Password::clear() {
  ui.goto_previous_screen();
  ui.goto_screen(screen_password_entry);
}

void Password::digit_entered() {
  password_string[PASSWORD_LENGTH - digit_no] = '0' + digit;
  --digit_no;
  value_keyed_in += digit * POW(10, digit_no);

  // Exit edit screen menu and go to another screen
  ui.goto_previous_screen();
  ui.use_click();
  ui.goto_screen(menu_password_entry);

  // After password has been keyed in
  if (digit_no == 0)
    (*return_fn)();
}

//
// Set/Change Password
//
void Password::screen_set_password() {
  return_fn = set_password_return;
  screen_password_entry();
}

void Password::set_password_return() {
  is_set = true;
  value = value_keyed_in;
  ui.completion_feedback(true);
  ui.goto_screen(menu_password_return);
}

void Password::menu_password_return() {
  START_SCREEN();
  BACK_ITEM(MSG_PASSWORD_SETTINGS);
  STATIC_ITEM(MSG_PASSWORD_SET, SS_LEFT, &password_string[0]);
  STATIC_ITEM(MSG_REMINDER_SAVE_SETTINGS, SS_LEFT);
  END_SCREEN();
}

void Password::remove_password() {
  is_set = false;
  password_string[0] = '0';
  password_string[1] = '\0';
  ui.completion_feedback(true);
  ui.goto_screen(menu_password_return);
}

//
// Password Menu
//
void Password::menu_password() {
  START_MENU();
  BACK_ITEM(MSG_ADVANCED_SETTINGS);
  SUBMENU(MSG_SET_CHANGE_PASSWORD, screen_set_password);
  SUBMENU(MSG_REMOVE_PASSWORD, remove_password);
  #if ENABLED(EEPROM_SETTINGS)
    ACTION_ITEM(MSG_STORE_EEPROM, ui.store_settings);
  #endif
  END_MENU();
}

#endif // PASSWORD_FEATURE
