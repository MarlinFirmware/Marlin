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
#include "../lcd/menu/menu.h"
#if ENABLED(PASSWORD_FEATURE)
  extern uint32_t password_value;
  extern bool password_set;
  extern selectFunc_t password_success;
  extern selectFunc_t password_fail;
  
  

  extern void password_authenticate_user();
  extern void password_authenticate_user_persistent();
  extern void password_authenticate_user_return();
  extern void menu_password();
  extern void password_key_in_menu();
  extern void password_key_in();
  extern void password_digit_keyed_in();
  extern void set_change_password();
  extern void set_change_password_return();
  extern void set_change_password_return_menu();
  extern void remove_password();
  extern void password_clear();
  extern void password_menu_media();

  

#endif