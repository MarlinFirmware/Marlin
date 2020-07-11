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

#if HAS_LCD_MENU
  #include "../lcd/menu/menu.h"
#endif

#include "../gcode/gcode.h"

#include "../core/serial.h"

Password password;


// public:
bool         Password::is_set, Password::is_locked;
uint32_t     Password::value, Password::value_entry;


#if HAS_LCD_MENU

// private:
char         Password::string[PASSWORD_LENGTH + 1];
uint8_t      Password::digit, Password::digit_no;
screenFunc_t Password::return_fn,
             Password::success_fn,
             Password::fail_fn;





void Password::authenticate_user() {
  if (is_set) {
    return_fn = authenticate_user_return;
    ui.goto_screen(screen_password_entry);
    ui.defer_status_screen();
    ui.update();
  }
  else {
    ui.goto_screen(success_fn);
    is_locked = false;
  }
}

void Password::authenticate_user_return() {
  if (value_entry == value) {
    ui.goto_screen(success_fn);
    is_locked = false;  
  } 
  else {
    ui.buzz(200,600);
    ui.buzz(0,0);
    ui.goto_screen(fail_fn);
    SERIAL_ECHOPGM_P(GET_TEXT(MSG_WRONG_PASSWORD));
    SERIAL_EOL();
  }
}

#if ENABLED(PASSWORD_ON_SD_PRINT_MENU)
  void Password::menu_media() {
    success_fn = menu_media;
    fail_fn = menu_main;
    authenticate_user();
  }
#endif

void Password::access_menu_password() {
  success_fn = menu_password;
  fail_fn = menu_advanced_settings;
  authenticate_user();
}

//
// Password entry screens
//
void Password::screen_password_entry() {
  value_entry = 0;
  digit_no = 0;
  digit = 0;
  memset(string, '*', PASSWORD_LENGTH);
  string[PASSWORD_LENGTH] = '\0';
  menu_password_entry();
}

void Password::menu_password_entry() {
  START_MENU();
  STATIC_ITEM(MSG_PASSWORD, SS_LEFT, string);
  EDIT_ITEM_FAST(uint8, MSG_ENTER_DIGIT, &digit, 0, 9, digit_entered, false);
  SUBMENU(MSG_CLEAR, clear);
  END_MENU();
}

void Password::clear() {
  ui.goto_previous_screen();
  ui.goto_screen(screen_password_entry);
}

void Password::digit_entered() {
  // The previous POW() function resulted in rounding errors for larger passwords
  uint32_t multiplier = 1;
  for (uint8_t i=0 ; i< PASSWORD_LENGTH - 1 - digit_no ; i++) {
    multiplier *= 10;
  }
  
  value_entry += digit * multiplier;
  string[digit_no++] = '0' + digit;

  // Exit edit screen menu and go to another screen
  ui.goto_previous_screen();
  ui.use_click();
  ui.goto_screen(menu_password_entry);

  // After password has been keyed in
  if (digit_no == PASSWORD_LENGTH) (*return_fn)();
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
  value = value_entry;
  ui.completion_feedback(true);
  ui.goto_screen(menu_password_return);
}

void Password::menu_password_return() {
  START_SCREEN();
  BACK_ITEM(MSG_PASSWORD_SETTINGS);
  STATIC_ITEM(MSG_PASSWORD_SET, SS_LEFT, string);
  STATIC_ITEM(MSG_REMINDER_SAVE_SETTINGS, SS_LEFT);
  END_SCREEN();
}

void Password::remove_password() {
  is_set = false;
  string[0] = '0';
  string[1] = '\0';
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

#endif // HAS_LCD_MENU

//
// Authenticate user with password. 
// Called from Setup, after SD Prinitng Stops/Aborts, and M510
//
void Password::authenticate_user_persistent() {
  is_locked = true;
  #if HAS_LCD_MENU
    success_fn = ui.status_screen;
    fail_fn = screen_password_entry;
    authenticate_user();
  #endif
}

//
// M510 Lock Printer
//
void GcodeSuite::M510() {
  password.authenticate_user_persistent();
}

//
// M511 Unlock Printer
//
#if DISABLED(DISABLE_M511)
  void GcodeSuite::M511() {
    if(password.is_locked) {
      password.value_entry = parser.ulongval('P');
      #if HAS_LCD_MENU
        SERIAL_ECHOPAIR("Pwd is ", password.value);
        password.authenticate_user_return();
      #else
        if (password.value_entry == password.value) {
          is_locked = false;  
        } else {
          SERIAL_ECHOPGM_P(GET_TEXT(MSG_WRONG_PASSWORD));
          SERIAL_EOL();
        }
      #endif
    }
      
  }
#endif

//
// M512 Set/Change/Remove Password
//
#if DISABLED(DISABLE_M512)
  void GcodeSuite::M512() {
    if (password.is_set && (parser.ulongval('P') != password.value) ) {
      SERIAL_ECHOPGM_P(GET_TEXT(MSG_WRONG_PASSWORD));
      SERIAL_EOL();
      return;
     }

    if (parser.seenval('N')) {
      password.value_entry = parser.ulongval('N');

      uint32_t multiplier = 1;
      for (uint8_t i=0 ; i< PASSWORD_LENGTH; i++) {
      multiplier *= 10;
      }
      if (password.value_entry < multiplier) {
        password.is_set = true;
        password.value = password.value_entry;
        serial_echopair_PGM(GET_TEXT(MSG_PASSWORD_SET), password.value);
        SERIAL_EOL();
      }
    } else {
      password.is_set = false;
      SERIAL_ECHOPGM_P(GET_TEXT(MSG_PASSWORD_REMOVED));
      SERIAL_EOL();
    }
    SERIAL_ECHOPGM_P(GET_TEXT(MSG_REMINDER_SAVE_SETTINGS));
    SERIAL_EOL();
  }
#endif

#endif // PASSWORD_FEATURE
