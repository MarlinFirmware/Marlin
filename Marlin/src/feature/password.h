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
#pragma once

#include "../lcd/ultralcd.h"

class Password {

  static bool is_set;
  static uint32_t value, value_keyed_in;
  static screenFunc_t success_fn, fail_fn;

  static void authenticate_user();
  static void authenticate_user_persistent();
  static void authenticate_user_return();
  static void menu_password();
  static void menu_password_entry();
  static void screen_password_entry();
  static void digit_entered();
  static void screen_set_password();
  static void set_password_return();
  static void menu_password_return();
  static void remove_password();
  static void clear();
  static void menu_media();

};

extern Password password;
