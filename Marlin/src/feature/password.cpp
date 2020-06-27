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
  #define INC_9 10

  uint32_t password_value;
  uint32_t password_value_keyed_in;
  bool password_set;
  char password_string[INCREMENT(PASSWORD_LENGTH)];
  uint8_t password_digit;
  uint8_t password_digit_no;
  selectFunc_t password_return_fn;
  selectFunc_t password_success;
  selectFunc_t password_fail;
  
  //
  // Authenticate user with password
  //
  void password_authenticate_user_persistent() {
    password_success = ui.status_screen;
    password_fail = password_key_in;
    password_authenticate_user();
  }

  void password_authenticate_user() {
    if (password_set) {
      password_return_fn = password_authenticate_user_return;
      
      ui.defer_status_screen();
      ui.goto_screen(password_key_in);
      ui.update();
    } else {
      ui.goto_screen(password_success);
    }
  }

  void password_authenticate_user_return() {
    if (password_value_keyed_in == password_value) {
      ui.defer_status_screen(false);
      ui.goto_screen(password_success);
    } else {
      ui.buzz(200,600);
      ui.buzz(0,0);
      ui.goto_screen(password_fail);
    }
  }
  
  void password_menu_media() {
    password_success = menu_media;
    password_fail = menu_main;
    password_authenticate_user();
  }

  //
  // Password key-in screens
  //
  void password_key_in() {
    
    password_value_keyed_in = 0;
    password_digit_no = PASSWORD_LENGTH;
    password_digit = 0;
    for (uint8_t i=0 ; i<PASSWORD_LENGTH ; i++) {
      password_string[i] = '*';
    }
    password_string[PASSWORD_LENGTH] = '\0';
    password_key_in_menu();
    
  }

  void password_key_in_menu() {
    START_MENU();
    STATIC_ITEM(MSG_PASSWORD, SS_LEFT, &password_string[0]);
    EDIT_ITEM_FAST(uint8, MSG_ENTER_DIGIT, &password_digit, 0, 9, password_digit_keyed_in, false);
    SUBMENU(MSG_CLEAR, password_clear);
    END_MENU();
  }

  void password_clear() {
    ui.goto_previous_screen();
    ui.goto_screen(password_key_in);
  }

  void password_digit_keyed_in() {

    password_string[PASSWORD_LENGTH - password_digit_no] = '0' + password_digit;
    --password_digit_no;
    password_value_keyed_in += password_digit * POW(10, password_digit_no);

    // Exit edit screen menu and go to another screen
    ui.goto_previous_screen();
    ui.use_click();
    ui.goto_screen(password_key_in_menu);
    
    // After password has been keyed in
    if (password_digit_no == 0) {
      (*password_return_fn)();
    }
  }

  // 
  // Set/Change Password
  //
  void set_change_password() {
      password_return_fn = set_change_password_return;
      password_key_in();
  }

  void set_change_password_return() {
    password_set = true;
    password_value = password_value_keyed_in;
    ui.completion_feedback(true);
    ui.goto_screen(set_change_password_return_menu);
  }

  void set_change_password_return_menu() {
    START_SCREEN();
    BACK_ITEM(MSG_PASSWORD_SETTINGS);
    STATIC_ITEM(MSG_PASSWORD_SET, SS_LEFT, &password_string[0]);
    STATIC_ITEM(MSG_REMINDER_SAVE_SETTINGS, SS_LEFT);
    END_SCREEN();
  }
  
  void remove_password() {
    password_set = false;
    password_string[0] = '0';
    password_string[1] = '\0';
    ui.completion_feedback(true);
    ui.goto_screen(set_change_password_return_menu);
  }

  //
  // Password Menu
  //
  void menu_password() {
    START_MENU();
    BACK_ITEM(MSG_ADVANCED_SETTINGS);
    SUBMENU(MSG_SET_CHANGE_PASSWORD, set_change_password);
    SUBMENU(MSG_REMOVE_PASSWORD, remove_password);
    #if ENABLED(EEPROM_SETTINGS)
    ACTION_ITEM(MSG_STORE_EEPROM, ui.store_settings);
    #endif
    END_MENU();
  }
#endif // ENABLED(PASSWORD_FEATURE)