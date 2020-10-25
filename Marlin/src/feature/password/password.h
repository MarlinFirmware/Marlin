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
#pragma once

#include "../../lcd/marlinui.h"

class Password {
public:
  static bool is_set, is_locked;
  static uint32_t value, value_entry;

  Password() { is_locked = false; }

  static void lock_machine();
  static void authentication_check();

  #if HAS_LCD_MENU
    static void access_menu_password();
    static void authentication_done();
    static void media_gatekeeper();

    private:
    static void authenticate_user(const screenFunc_t, const screenFunc_t);
    static void menu_password();
    static void menu_password_entry();
    static void screen_password_entry();
    static void screen_set_password();
    static void start_over();

    static void digit_entered();
    static void set_password_done();
    static void menu_password_report();

    static void remove_password();
  #endif
};

extern Password password;
